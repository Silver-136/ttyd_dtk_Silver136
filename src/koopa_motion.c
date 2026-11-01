// Bowser's motions in the side-scrolling interludes: walk, jump, fall, power-ups and damage.

#include "version.h"
#include "mario/mario.h"
#include "koopa.h"
#include "mario/mario_motion.h"
#include "manager/evtmgr.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/types.h>
#include "evt/evt_cam.h"
#include "mario/mario_cam.h"
#include "mario/mario_sbr.h"
#include "mot/mot_jump.h"
#include "driver/animdrv.h"
#include "koopa_motion.h"
#include "evt/evt_cmd.h"
#include "koopa_hit.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "eff/eff_mizutama.h"
#include "mario/mario_hit.h"

/**
 * Bowser's walk and dash speeds per power-up level in 3D stages (kpa_walk, kpa_dash), then the walk animation
 * rates stored with them (all 0), then the scale-flicker steps of the power-up and power-down motions.
 */
f32 _walkSpdTbl[4] = {0.6f, 1.4f, 1.4f, 1.4f};
f32 _dashSpdTbl[4] = {1.2f, 2.0f, 2.0f, 2.0f};
s32 _rateTbl[4] = {0, 0, 0, 0};
s32 _rateTbl2[4] = {0, 0, 0, 0};
f32 scaleDt[9] = {1.0f, 0.0f, 2.0f, 1.0f, 3.0f, 2.0f, 4.0f, 3.0f, 5.0f};

/** Small 30-frame camera shake when big or giant Bowser lands (quake_kpaLandOn). */
EVT_BEGIN(quake_kpaLandOn_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 30)
    RETURN
EVT_END()

/** Stronger 60-frame camera shake for Bowser's ground pound landing. */
EVT_BEGIN(quake_kpaHipAttack_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.01953125f), 60)
    RETURN
EVT_END()

/** Starts the fast drop of the ground pound (sub-state 0x14) with its fixed downward physics. */
static inline void kpa_hip_launch(MarioWork* mario) {
    mario->currSubMotionId = 20;
    mario->jumpAccelerationY = -0.4744f;
    mario->jumpJerkY = 0.4704f;
    mario->jumpSnapY = -0.1904f;
    mario->jumpVelocityY = mario->jumpAccelerationY;
}

/**
 * In the side-scrolling stages, A jumps (or swims when fully underwater); returns 1 if it changed motion.
 * Always 0 in 3D maps.
 */
static inline s32 kpa_stay_chk(void) {
    MarioWork* mario = marioGetPtr();
    if (kpaGetStageViewType() == 1) {
        return 0;
    }
    if (!(mario->buttonTrg & PAD_BUTTON_A)) {
        return 0;
    }
    if (kpaGetBodyStatus() == KPA_BODY_SUBMERGED) {
        marioChgMot(MARIO_MOTION_KPA_SWIM);
        return 1;
    }
    marioChgMot(MARIO_MOTION_JUMP);
    return 1;
}

/**
 * Spawns the water droplet effect around swimming Bowser:
 * on every frame of the stroke (sub-states 0x1E/0x1F),
 * otherwise every 10 frames while rising and every 30 while sinking.
 */
static inline void kpa_swim_splash(void) {
    MarioWork* mario = marioGetPtr();
    s32 st;
    s32 lim;
    mario->kpaWork->swimSplashTimer++;
    st = mario->currSubMotionId;
    if (st == 0x1E || st == 0x1F) {
        if (mario->kpaWork->swimSplashTimer > 0) {
            f32 rx;
            f32 rz;
            mario->kpaWork->swimSplashTimer = 0;
            rx = (f32)(irand(40) - 20);
            rz = (f32)(irand(40) - 20);
            effMizutamaEntry(0, mario->position.x + rx, 0.6f * mario->wPlayerCollisionBox.y + (mario->position.y + rz), mario->position.z,
                             mario->directionView < 180.0f ? 225.0f : 135.0f);
        }
    } else {
        s32 in = FALSE;
        if (st >= 0 && st < 20) {
            in = TRUE;
        }
        lim = in ? 10 : 30;
        if (mario->kpaWork->swimSplashTimer > lim) {
            mario->kpaWork->swimSplashTimer = 0;
            effMizutamaEntry(0, mario->position.x, mario->position.y + mario->wPlayerCollisionBox.y / 3.0f, mario->position.z,
                             mario->directionView < 180.0f ? 225.0f : 135.0f);
        }
    }
}

/**
 * TRUE when Bowser may ground pound: not already doing one
 * and KpaWork flag 0x10 (set when he starts swimming) is clear.
 */
BOOL kpaHipAttackOk(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_HIP) {
        return FALSE;
    }
    return ((mario->kpaWork->flags >> 4) & 1) ^ 1;
}

/**
 * Side-scrolling horizontal speed from the stick:
 * accelerates toward a walk (1.3) or dash (2.3) top speed, much lower when fully underwater,
 * and slows down when the stick is released (faster against a wall).
 * Sets kpaSpeed, baseSpeed and facing; returns the speed.
 */
f32 kpaStickSpd(s32 dash) {
    MarioWork* mario = marioGetPtr();
    f32 stick = mario->wStickDir1;
    f32 spd;
    f32 max;
    f32 lim = 70.0f;
    f32 k;
    if (stick >= lim) {
        stick = lim;
    }
    if (stick <= -lim) {
        stick = -lim;
    }
    spd = mario->kpaSpeed;
    if (!stick) {
        k = -0.2f;
        if (mario->hitobjWall) {
            k = -0.7f;
        }
        spd += spd * k;
        if (__fabsf(spd) < 0.01f) {
            spd = 0.0f;
        }
    } else {
        if (dash == 0) {
            max = kpaGetBodyStatus() == KPA_BODY_SUBMERGED ? 0.2f : 1.3f;
        } else {
            max = kpaGetBodyStatus() == KPA_BODY_SUBMERGED ? 0.633f : 2.3f;
        }
        if (stick * spd > 0.0f) {
            spd += 0.002f * stick;
            if (spd >= max) {
                spd = max;
            }
            if (spd <= -max) {
                spd = -max;
            }
        } else {
            spd += 0.002f * stick;
            if (spd >= max) {
                spd = max;
            }
            if (spd <= -max) {
                spd = -max;
            }
        }
    }
    mario->kpaSpeed = spd;
    mario->baseSpeed = __fabsf(spd);
    {
        f32 a = angleABf(0.0f, 0.0f, spd, 0.0f);
        if (mario->baseSpeed) {
            mario->directionView = a;
        }
    }
    return mario->baseSpeed;
}

/**
 * Standing motion for Bowser: idle pose, then A jumps (side-scrolling), B breathes fire,
 * and stick input starts walking.
 */
void kpa_stay(void) {
    MarioWork* mario = marioGetPtr();
    f32 spd;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->airTimer = 0;
        mario->multiTimer = 0;
        mario->currSubMotionId = 0;
        if (kpaGetAttackMode() == 0) {
            if (!(mario->kpaWork->flags & KPA_WORK_FLAG_WEARY)) {
                kpaChgPose("KPA_S_1", "KPA2_S_1");
            } else {
                kpaChgPose("KPA_S_2", "KPA2_S_1");
            }
        }
        if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            mario->baseSpeed = 0.0f;
        }
    }
    if (kpaGetAttackMode() == 0) {
        if (!kpa_stay_chk()) {
            if (mario->buttonTrg & PAD_BUTTON_B) {
                kpaAttackStart();
            } else {
                if (mario->forceMoveTimer == 0) {
                    spd = kpaStickSpd(0);
                } else {
                    spd = mario->baseSpeed;
                }
                if (kpaGetStageViewType() == 0) {
                    mario->baseSpeed = spd;
                    if (mario->baseSpeed > 0.0f) {
                        marioChgMot(MARIO_MOTION_WALK);
                    }
                } else if (mario->controlStickSensitivity > 0.0f) {
                    marioChgMot(MARIO_MOTION_WALK);
                }
            }
        }
    }
}

/**
 * Walking motion for Bowser: plays footstep sounds on land and a slowed swim-walk animation under water,
 * B breathes fire, A jumps; uses stick speed in the side-scrolling stages
 * and the per-level walk speed in 3D maps; a full stick tilt switches to dashing and releasing it stops.
 */
void kpa_walk(void) {
    MarioWork* m = marioGetPtr();
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        m->currSubMotionId = 0;
        m->multiTimer = 0;
#if VERSION <= VERSION_DEMO_US
        if (kpaGetStageViewType() == 0) {
            if (m->controlStickSensitivity) {
                kpaChgPose("KPA_W_1", "KPA2_W_1");
            } else if (!(m->kpaWork->flags & KPA_WORK_FLAG_WEARY)) {
#else
        if (!(m->kpaWork->flags & KPA_WORK_FLAG_WEARY)) {
#endif
            kpaChgPose("KPA_W_1", "KPA2_W_1");
        } else {
            kpaChgPose("KPA_W_2", "KPA2_W_1");
        }
#if VERSION <= VERSION_DEMO_US
        } else {
            kpaChgPose("KPA_W_1", "KPA2_W_1");
        }
#endif
        m->mot.kpa.animFrame = 0;
        m->mot.kpa.reserved_2 = 0;
        m->kpaWork->standParamA = 0.02f;
        m->kpaWork->standParamB = -0.02f;
    }
    if (!kpa_stay_chk()) {
        if (m->buttonTrg & PAD_BUTTON_B) {
            kpaAttackStart();
            marioChgMot(MARIO_MOTION_STAY);
            return;
        }
        if (kpaGetBodyStatus() == KPA_BODY_SUBMERGED) {
            m->mot.kpa.animFrame++;
            if (m->mot.kpa.animFrame > 0x50) {
                m->mot.kpa.animFrame = 0;
            }
            m->poseTime = m->mot.kpa.animFrame / 2;
            animPoseSetLocalTime(m->paperAnimGroupId[marioAnimeId()], (f32)m->poseTime);
            if (kpaGetBodyStatus() == KPA_BODY_DRY) {
                if (m->multiTimer % 40 == 0) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE2), &m->position);
                } else if (m->multiTimer % 20 == 0) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE1), &m->position);
                }
                m->multiTimer++;
            }
        } else if (kpaGetBodyStatus() == KPA_BODY_DRY) {
            if (m->multiTimer % 40 == 0) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE2), &m->position);
            } else if (m->multiTimer % 20 == 0) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE1), &m->position);
            }
            m->multiTimer++;
        }
        if (m->forceMoveTimer == 0) {
            if (kpaGetStageViewType() == 0) {
                m->baseSpeed = kpaStickSpd(0);
            } else {
                MarioWork* mm = marioGetPtr();
                s32 lv = kpaGetLevel();
                f32 sp = _walkSpdTbl[lv];
                s32 rt = _rateTbl[lv];
                mm->kpaWork->walkRate = rt;
                m->baseSpeed = sp;
            }
        }
        if (kpaGetStageViewType() == 0) {
            if (!m->baseSpeed && !(m->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                marioChgMot(MARIO_MOTION_STAY);
                return;
            }
        } else {
            if (!m->controlStickSensitivity && !(m->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                marioChgMot(MARIO_MOTION_STAY);
                return;
            }
        }
        if (!marioKeyOffChk()) {
            marioCamZoomOffReq();
            if (kpaGetStageViewType() == 1) {
                m->directionView = m->controlStickAngle;
            }
            if (m->wStickDir1 * m->wStickDir1 + m->wStickDir2 * m->wStickDir2 > 3025) {
                marioChgMot(MARIO_MOTION_DASH);
            }
        }
    }
}

/**
 * Running motion for Bowser: as kpa_walk but with dash speeds and faster footsteps;
 * easing off the stick goes back to walking.
 */
void kpa_dash(void) {
    MarioWork* m = marioGetPtr();
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        kpaChgPose("KPA_R_1", "KPA2_R_1");
        m->currSubMotionId = 0;
        m->multiTimer = 0;
        m->mot.kpa.animFrame = 0;
    }
    if (!kpa_stay_chk()) {
        if (m->buttonTrg & PAD_BUTTON_B) {
            kpaAttackStart();
            marioChgMot(MARIO_MOTION_STAY);
            return;
        }
        if (kpaGetBodyStatus() == KPA_BODY_SUBMERGED) {
            m->mot.kpa.animFrame++;
            if (m->mot.kpa.animFrame > 0x50) {
                m->mot.kpa.animFrame = 0;
            }
            m->poseTime = m->mot.kpa.animFrame / 2;
            animPoseSetLocalTime(m->paperAnimGroupId[marioAnimeId()], (f32)m->poseTime);
        }
        if (kpaGetBodyStatus() == KPA_BODY_DRY) {
            if (m->multiTimer % 30 == 0) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE2), &m->position);
            } else if (m->multiTimer % 15 == 0) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE1), &m->position);
            }
            m->multiTimer++;
        }
        if (m->forceMoveTimer == 0) {
            if (kpaGetStageViewType() == 0) {
                m->baseSpeed = kpaStickSpd(1);
            } else {
                MarioWork* mm = marioGetPtr();
                s32 lv = kpaGetLevel();
                f32 sp = _dashSpdTbl[lv];
                s32 rt = _rateTbl2[lv];
                mm->kpaWork->walkRate = rt;
                m->baseSpeed = sp;
            }
        }
        if (kpaGetStageViewType() == 0) {
            if (!m->baseSpeed && !(m->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                marioChgMot(MARIO_MOTION_STAY);
                return;
            }
        } else {
            if (!m->controlStickSensitivity && !(m->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
                marioChgMot(MARIO_MOTION_STAY);
                return;
            }
        }
        if (!marioKeyOffChk()) {
            marioCamZoomOffReq();
            if (kpaGetStageViewType() == 1) {
                m->directionView = m->controlStickAngle;
            }
            if (m->wStickDir1 * m->wStickDir1 + m->wStickDir2 * m->wStickDir2 <= 3025) {
                marioChgMot(MARIO_MOTION_WALK);
            }
        }
    }
}

/**
 * Jumping motion for Bowser: jump pose and sound, rises with the level's jump physics,
 * cut short if A is released after 3 frames, then switches to falling at the peak.
 */
void kpa_jump(void) {
    MarioWork* mario = marioGetPtr();
    f32 prevVy;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags |= MARIO_FLAG_HAS_INPUT_JUMP;
        mario->multiTimer = 2;
        mario->currSubMotionId = 0;
        marioPreJump();
        if (mario->currMotionId != MARIO_MOTION_JUMP_3) {
            camFollowYOff();
        }
        kpaChgPose("KPA_J_1A", "KPA2_J_1A");
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_JUMP1), &mario->position);
        if (mario->airTimer == 0) {
            mario->fallStartPos = mario->position;
        }
        mario->mot.kpa.animFrame = 0;
    }
    mario->airTimer++;
    if (mario->currMotionId != MARIO_MOTION_JUMP_3) {
        prevVy = mario->jumpVelocityY;
        marioMakeJumpPara();
        if (mario->jumpVelocityY > 0.0f && mario->airTimer >= 4 && !(mario->buttonHold & PAD_BUTTON_A)) {
            mario->jumpVelocityY += -0.526f;
        }
        if (mario->jumpVelocityY <= 0.0f) {
            if (prevVy > 0.0f) {
                mario->jumpPeakAirTime = mario->airTimer;
            }
            marioSetPeakYpos();
            kpaSetFallPara();
            marioMakeJumpPara();
            marioChgMot(MARIO_MOTION_FALL);
        }
        mario->position.y += mario->jumpVelocityY;
    }
}

/**
 * Falling motion for Bowser (a non-Bowser player just gets Mario's fall pose):
 * turns camera follow back on near the ground, marks entering the water in kpa_03
 * and switches to swimming once fully underwater. Retail keeps the fire pose while breathing fire.
 */
void kpa_fall(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~MARIO_FLAG_IS_JUMPING;
        mario->flags |= MARIO_FLAG_IS_FALLING;
        mario->currSubMotionId = 0;
        if (mario->currMotionId != MARIO_MOTION_FALL_2) {
#if VERSION <= VERSION_DEMO_US
            if ((s32)(u8)mario->characterId == CHARACTER_BOWSER) {
#else
            if (mario->characterId == CHARACTER_BOWSER) {
                if (!kpaFireAttackCheck()) {
#endif
                    kpaChgPose("KPA_J_1B", "KPA2_J_1B");
#if VERSION > VERSION_DEMO_US
                }
#endif
            } else if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
                marioChgPose("M_J_1C");
            }
        } else {
            mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        }
        if (mario->airTimer == 0) {
            mario->fallStartPos = mario->position;
        }
    }
    switch (mario->currSubMotionId) {
    case 0:
        if (mario->position.y <= mario->lastGroundPos.y) {
            if (mario->position.y <= 75.0f) {
                // BUG: `|=` sets MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF instead of testing it, so the test is always
                // true (kpa_hip tests the bit with `&`)
                if ((mario->dispFlags |= MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF) != 0) {
                    camFollowYOn();
                }
            }
            kpaCamFollow();
            mario->currSubMotionId = 10;
        }
        break;
    }
    mario->airTimer++;
    if (kpaGetStageType() == 1 && kpaGetBodyStatus() <= KPA_BODY_PARTLY_SUBMERGED) {
        mario->kpaWork->flags |= KPA_WORK_FLAG_WATER_FALL_SFX;
    }
    if (kpaGetBodyStatus() == KPA_BODY_SUBMERGED) {
        mario = marioGetPtr();
        if (mario->currMotionId != MARIO_MOTION_KPA_SWIM) {
            marioChgMot2(MARIO_MOTION_KPA_SWIM);
        }
        kpaClearHitobjRide();
        mario->flags &= ~MARIO_FLAG_IS_JUMPING;
        mario->flags |= MARIO_FLAG_IS_FALLING;
        mario->currSubMotionId = 0x1E;
        mario->airTimer = 0;
        mario->multiTimer = 0;
        mario->baseSpeed = 0.0f;
        mario->kpaSpeed = 0.0f;
    }
}

/**
 * Ground pound: hangs for 6 frames, drops at up to 100 units per frame,
 * and on landing plays the impact sound and the big camera shake;
 * 20 frames later he stands up, or drops again if A is held and he is not standing on an object.
 */
void kpa_hip(void) {
    MarioWork* mario = marioGetPtr();
    f32 fall;
    f32 y;
    void* hit;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~MARIO_FLAG_HAS_INPUT_JUMP;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_HIP_DROP1), &mario->position);
        marioAdjustMoveDir();
        mario->currSubMotionId = 10;
        mario->multiTimer = 6;
        mario->jumpVelocityY = 0.0f;
        mario->jumpAccelerationY = 0.0f;
        mario->jumpJerkY = 0.0f;
        mario->jumpSnapY = 0.0f;
        mario->baseSpeed = 0.0f;
        marioChgPose("KPA2_J_1C");
    }
#if VERSION <= VERSION_DEMO_US
    if (mario->fallStartPos.y >= mario->position.y && ((u32)mario->dispFlags & MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF) && mario->position.y > 75.0f) {
#else
    if (mario->fallStartPos.y >= mario->position.y && (mario->dispFlags & MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF)) {
#endif
        camFollowYOn();
    }
    switch (mario->currSubMotionId) {
    case 10:
        marioChkLandon(&y, 0.0f);
        if (--mario->multiTimer <= 0) {
            kpa_hip_launch(mario);
        } else {
            break;
        }
    case 20:
        fall = marioGetFallSpd();
        hit = marioChkLandon(&y, fall);
        if (fall < -100.0f) {
            fall = -100.0f;
            mario->jumpAccelerationY = mario->jumpJerkY = mario->jumpSnapY = 0.0f;
            mario->jumpVelocityY = fall;
        }
        if (hit) {
            mario->hitobjStandOn = hit;
            mario->hitobjJumpFrom = NULL;
            mario->currMotionId = MARIO_MOTION_HIP_2;
            mario->currSubMotionId = 21;
            mario->multiTimer = 20;
            mario->position.y = y;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_HIP_DROP2), &mario->position);
            evtEntry(quake_kpaHipAttack_evt, 0, 0);
            mario->airTimer = 0;
            mario->jumpPeakAirTime = 0;
        } else {
            mario->position.y += fall;
        }
        break;
    case 21:
        if (--mario->multiTimer <= 0) {
            if (mario->position.y > 75.0f) {
                camFollowYOn();
            }
            if ((mario->buttonHold & PAD_BUTTON_A) && !mario->hitobjStandOn) {
                kpa_hip_launch(mario);
            } else {
                mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
                marioChgMotSub(MARIO_MOTION_STAY, 0);
            }
        }
        break;
    }
}

/**
 * Power-up pickup (the kpa_powerup script helper):
 * grows Bowser one level with the power-up motion and returns TRUE;
 * at the giant level gives star invincibility instead and returns FALSE.
 */
BOOL kpaPowUp(void) {
    MarioWork* m = marioGetPtr();
    if (kpaGetLevel() < 3) {
        m->kpaWork->level++;
        kpaCancelAttackMode();
        marioChgMot(MARIO_MOTION_KPA_POW_UP);
        return TRUE;
    }
    kpaMSetMutekiTimer();
    kpaMutekiOn();
    return FALSE;
}

/**
 * Damage: shrinks Bowser one level with the power-down motion and 3 seconds without hits, returning TRUE;
 * at the smallest level sets KpaWork flag 0x10000000 instead and returns FALSE.
 * Called from the NPC contact code.
 * That flag is Bowser's death: the Super Koopa Bros. stage scripts poll it with evt_koopa_chk_dead
 * and run kpa_dead, which plays his defeat and restarts the stage.
 */
BOOL kpaPowDown(void) {
    MarioWork* mario = marioGetPtr();
    kpaCancelAttackMode();
    if (kpaGetLevel() > 0) {
        mario->kpaWork->level--;
        mario->kpaNoHitTimer = sysMsec2Frame(3000);
        marioChgMot(MARIO_MOTION_KPA_POW_DOWN);
        return TRUE;
    }
    mario->kpaWork->flags |= KPA_WORK_FLAG_DEAD;
    return FALSE;
}

/** Sets Bowser's collision box for his level: width from kpaGetWidth, height 24, 74, 103.6 or 222. */
void kpaSetSpec(void) {
    MarioWork* mario = marioGetPtr();
    s32 lv;
    mario->wPlayerCollisionBox.x = kpaGetWidth(mario->kpaWork->level);
    lv = mario->kpaWork->level;
    if (lv == 0) {
        mario->wPlayerCollisionBox.y = 24.0f;
    } else if (lv == 1) {
        mario->wPlayerCollisionBox.y = 74.0f;
    } else if (lv == 2) {
        mario->wPlayerCollisionBox.y = 103.6f;
    } else if (lv == 3) {
        mario->wPlayerCollisionBox.y = 222.0f;
    }
}

/**
 * Power-up motion: flickers Bowser's scale between the old and new level size in five 5-frame steps (scaleDt),
 * then applies the new collision, shifts the camera for his size and returns to standing;
 * reaching giant also starts star invincibility.
 */
void kpa_powup(void) {
    MarioWork* m = marioGetPtr();
    MarioWork* m2;
    f32 step;
    f32 base;
    s32 lv;
    f32 k;
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        m->airTimer = 0;
        m->multiTimer = 0;
        kpaChgPose("KPA_Y_1", "KPA2_Y_1");
        m->currSubMotionId = 0;
        if (!(m->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            m->baseSpeed = 0.0f;
        }
        m->flags |= MARIO_FLAG_CHG_MOT_DISABLED;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_KOOPA_HAPPY1_1), &m->position);
    }
    lv = kpaGetLevel();
    if (lv <= 3) {
        base = kpaGetBaseScale(lv - 1);
        step = (kpaGetBaseScale(lv) - base) / 5.0f;
    } else {
        marioChgMot(MARIO_MOTION_STAY);
        return;
    }
    switch (m->currSubMotionId) {
    case 0:
        m->multiTimer = 5;
        m->currSubMotionId = 1;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_BIG1), &m->position);
        break;
    default:
        if (--m->multiTimer > 0) {
            break;
        }
        k = scaleDt[m->currSubMotionId - 1];
        m->multiTimer = 5;
        m->scale.x = step * k + base;
        m->scale.y = m->scale.x;
        m->currSubMotionId++;
        if (5.0f == k) {
            m2 = marioGetPtr();
            if (m2->kpaWork->level == 3) {
                kpaMSetMutekiTimer();
                kpaMutekiOn();
            }
            kpaSetSpec();
            marioResetCamShiftRate();
            m2->camShiftTargetX = 20.0f * (f32)(-m2->kpaWork->level);
            m->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
            marioChgMot(MARIO_MOTION_STAY);
            m->mutekiTimer = 0;
        }
    }
}

/** Power-down motion: the shrinking version of kpa_powup, ending invincibility at the end. */
void kpa_powdown(void) {
    MarioWork* m = marioGetPtr();
    MarioWork* m2;
    f32 step;
    f32 base;
    s32 lv;
    f32 k;
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        m->airTimer = 0;
        m->multiTimer = 0;
        kpaChgPose("KPA_D_1", "KPA2_D_1");
        m->currSubMotionId = 0;
        if (!(m->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            m->baseSpeed = 0.0f;
        }
        m->flags |= MARIO_FLAG_CHG_MOT_DISABLED;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_KOOPA_SATIATED1_1), &m->position);
    }
    lv = kpaGetLevel();
    if (lv >= 0) {
        base = kpaGetBaseScale(lv + 1);
        step = (base - kpaGetBaseScale(lv)) / 5.0f;
    } else {
        marioChgMot(MARIO_MOTION_STAY);
        return;
    }
    switch (m->currSubMotionId) {
    case 0:
        m->multiTimer = 5;
        m->currSubMotionId = 1;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_SMALL1), &m->position);
        break;
    default:
        if (--m->multiTimer > 0) {
            break;
        }
        k = scaleDt[m->currSubMotionId - 1];
        m->multiTimer = 5;
        m->scale.x = base - step * k;
        m->scale.y = m->scale.x;
        m->currSubMotionId++;
        if (5.0f == k) {
            m2 = marioGetPtr();
            kpaMutekiOff();
            kpaSetSpec();
            marioResetCamShiftRate();
            m2->camShiftTargetX = 20.0f * (f32)(-m2->kpaWork->level);
            m->flags &= ~MARIO_FLAG_CHG_MOT_DISABLED;
            marioChgMot(MARIO_MOTION_STAY);
        }
    }
}

#pragma dont_inline on
#if VERSION <= VERSION_DEMO_US
/**
 * Underwater horizontal movement: accelerates kpaSpeed from the stick up to 40
 * and turns it into a walking speed (retail caps it at 2, the demo at 1 and also moves him here).
 */
static void kpaSwimMove(void) {
#else
/**
 * Underwater horizontal movement: accelerates kpaSpeed from the stick up to 40
 * and turns it into a walking speed (retail caps it at 2, the demo at 1 and also moves him here).
 */
void kpaSwimMove(void) {
#endif
    MarioWork* mario = marioGetPtr();
    f32 spd = mario->kpaSpeed;
    f32 stick = mario->wStickDir1;
    f32 moveSpeed;
    f32 lim = 40.0f;
    f32 rate = 0.01f;
    if (stick) {
        if (stick * spd < 0.0f) {
            spd = 0.0f;
        }
        if (spd * spd <= 1225.0f) {
            spd += stick * rate;
        } else {
            spd += stick * rate;
            if (spd < -lim) {
                spd = -lim;
            }
            if (spd > lim) {
                spd = lim;
            }
        }
    }
    mario->kpaSpeed = spd;
#if VERSION <= VERSION_DEMO_US
    moveSpeed = 0.03f * __fabsf(spd);
    if (moveSpeed >= 1.0f) {
        moveSpeed = 1.0f;
#else
    moveSpeed = 0.06f * __fabsf(spd);
    if (moveSpeed >= 2.0f) {
        moveSpeed = 2.0f;
#endif
    }
    {
        f32 a = angleABf(0.0f, 0.0f, spd, 0.0f);
        if (moveSpeed < 0.01f) {
            moveSpeed = 0.0f;
        }
        mario->baseSpeed = moveSpeed;
        if (moveSpeed) {
            mario->directionView = a;
        }
    }
    mario->moveDist = moveSpeed;
#if VERSION <= VERSION_DEMO_US
    movePos(&mario->position.x, &mario->position.z, mario->baseSpeed, mario->directionView);
#endif
}

#pragma dont_inline reset

/**
 * Swimming motion in kpa_03: A starts a swim stroke upward (limited near the surface),
 * he sinks slowly between strokes, B breathes fire (which freezes the stick),
 * and stepping onto a platform stops him. Also spawns the droplet effects.
 */
/**
 * Swimming motion in kpa_03: A starts a swim stroke upward (limited near the surface),
 * he sinks slowly between strokes, B breathes fire (which freezes the stick),
 * and stepping onto a platform stops him. Also spawns the droplet effects.
 */
void kpa_swim(void) {
    MarioWork* mario = marioGetPtr();
    f32 v;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->airTimer = 0;
        mario->multiTimer = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
        mario->kpaSpeed = 0.0f;
        marioGetPtr()->kpaWork->flags |= KPA_WORK_FLAG_NO_HIP_ATTACK;
    }
    if (mario->buttonTrg & PAD_BUTTON_B) {
        kpaAttackStart2();
    }
    if (kpaFireAttackCheck() == 1) {
        mario->wStickDir1 = 0;
        mario->wStickDir2 = 0;
    } else if (kpaGetBodyStatus() == KPA_BODY_SUBMERGED && (mario->buttonTrg & PAD_BUTTON_A)) {
#if VERSION <= VERSION_DEMO_US
        s32 st = (s32)mario->currSubMotionId;
#else
        s32 st = mario->currSubMotionId;
#endif
        if (st >= 0 && st <= 1 && mario->position.y < 280.0f) {
            mario->currSubMotionId = 0;
        } else if (st >= 0x1E && st <= 0x20) {
            mario->currSubMotionId = 0;
        }
    }
    kpaSwimMove();
    kpa_swim_splash();
#if VERSION <= VERSION_DEMO_US
    switch ((s32)mario->currSubMotionId) {
#else
    switch (mario->currSubMotionId) {
#endif
    case 0:
        if (mario->hitobjStandOn != NULL) {
            mario->jumpVelocityY = 1.0f;
            mario->jumpAccelerationY = 0.0f;
            mario->jumpJerkY = 0.0f;
            mario->jumpSnapY = -0.0001f;
        } else {
            mario->jumpVelocityY = 1.52f;
            mario->jumpAccelerationY = 0.005f;
            mario->jumpJerkY = 0.006f;
            mario->jumpSnapY = -0.0008f;
        }
        kpaClearHitobjRide();
        mario->currSubMotionId = 1;
        mario->flags &= ~MARIO_FLAG_IS_FALLING;
        mario->flags |= MARIO_FLAG_IS_JUMPING;
#if VERSION <= VERSION_DEMO_US
        psndSFXOn_3D((const char*)"SFX_KUPPA_SWIM1", &mario->position);
#else
        psndSFXOn_3D("SFX_KUPPA_SWIM1", &mario->position);
#endif
        if (kpaGetAttackMode() == 0) {
            kpaChgPose("KPA_W_1", "KPA2_W_2");
        }
    case 1:
        if (kpaGetHitobjHead2()) {
            mario->currSubMotionId = 0x1E;
            mario->jumpVelocityY += 0.02f * (-2.0f - mario->jumpVelocityY);
            mario->jumpAccelerationY = 0.0f;
            mario->jumpJerkY = 0.0f;
            mario->jumpSnapY = 0.0f;
            break;
        }
        v = marioMakeJumpPara();
        mario->position.y += v;
        if (mario->position.y + mario->wPlayerCollisionBox.y >= 370.0f) {
            if (v < 0.0f) {
                mario->currSubMotionId = 0x28;
            }
#if VERSION <= VERSION_DEMO_US
            mario->jumpVelocityY += 0.02f * (-2.0f - mario->jumpVelocityY);
            mario->jumpAccelerationY = 0.0f;
            mario->jumpJerkY = 0.0f;
            mario->jumpSnapY = 0.0f;
#else
#if VERSION < VERSION_US
            mario->jumpVelocityY += 0.02f * (-2.0f - mario->jumpVelocityY);
            mario->jumpAccelerationY = 0.0f;
            mario->jumpJerkY = 0.0f;
            mario->jumpSnapY = 0.0f;
#endif
#endif
        } else if (v < 0.0f) {
            mario->currSubMotionId = 0x1E;
        }
        break;
    case 10:
        kpaClearHitobjRide();
        mario->jumpVelocityY = 2.0f;
        mario->jumpAccelerationY = -0.03f;
        mario->jumpJerkY = 0.0f;
        mario->jumpSnapY = 0.0f;
        mario->currSubMotionId = 11;
        mario->flags &= ~MARIO_FLAG_IS_FALLING;
        mario->flags |= MARIO_FLAG_IS_JUMPING;
        if (kpaGetAttackMode() == 0) {
            kpaChgPose("KPA_W_1", "KPA2_W_2");
        }
    case 11:
        v = marioMakeJumpPara();
        mario->position.y += v;
        if (kpaGetBodyStatus() == KPA_BODY_PARTLY_SUBMERGED) {
            mario->jumpVelocityY += 0.06f * (-2.0f - mario->jumpVelocityY);
            mario->jumpAccelerationY = 0.0f;
            mario->jumpJerkY = 0.0f;
            mario->jumpSnapY = 0.0f;
            if (v < 0.0f) {
                mario->currSubMotionId = 0x28;
            }
        } else if (v < 0.0f) {
            mario->currSubMotionId = 0x1E;
        }
        break;
    case 20:
        mario->kpaWork->flags |= KPA_WORK_FLAG_WATER_FALL_SFX;
        mario->flags &= ~MARIO_FLAG_IS_FALLING;
        mario->flags |= MARIO_FLAG_IS_JUMPING;
        kpaChgPose("KPA_J_1A", "KPA2_J_1A");
#if VERSION <= VERSION_DEMO_US
        psndSFXOn_3D((const char*)"SFX_KUPPA_WATER_UP1", &mario->position);
#else
        psndSFXOn_3D("SFX_KUPPA_WATER_UP1", &mario->position);
#endif
        mario->jumpVelocityY = 5.0f;
        mario->jumpAccelerationY = -0.13f;
        mario->jumpJerkY = 0.0f;
        mario->jumpSnapY = -0.001f;
        mario->currSubMotionId = 21;
    case 21:
        v = marioMakeJumpPara();
        mario->position.y += v;
        if (v <= 0.5f) {
            mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
            kpaSetFallPara();
            marioMakeJumpPara();
            marioChgMot(MARIO_MOTION_FALL);
            return;
        }
        break;
    case 22:
        break;
    }
#if VERSION <= VERSION_DEMO_US
    switch ((s32)mario->currSubMotionId) {
#else
    switch (mario->currSubMotionId) {
#endif
    case 0x1E:
        if (mario->kpaWork->flags & KPA_WORK_FLAG_WATER_FALL_SFX) {
            mario->kpaWork->flags &= ~KPA_WORK_FLAG_WATER_FALL_SFX;
#if VERSION <= VERSION_DEMO_US
            psndSFXOn_3D((const char*)0x827, &mario->position);
#else
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_WATER_FALL1), &mario->position);
#endif
        }
        if (kpaGetAttackMode() == 0) {
            kpaChgPose("KPA_W_1", "KPA2_Z_2");
        }
        mario->flags &= ~MARIO_FLAG_IS_JUMPING;
        mario->flags |= MARIO_FLAG_IS_FALLING;
        if (mario->jumpVelocityY < 0.0f) {
            mario->currSubMotionId = 0x1F;
        } else {
            mario->jumpVelocityY = -0.08f;
            mario->jumpAccelerationY = -0.03f;
            mario->jumpJerkY = 0.009f;
            mario->jumpSnapY = 0.0f;
            mario->currSubMotionId = 0x20;
        }
        kpa_swim_splash();
        if (marioBgmodeChk() != 1) {
            camFollowYOn();
        }
        break;
    case 0x1F: {
        f32 d = (2.0f - mario->jumpVelocityY) * (f32)(kpaGetBodyStatus() == KPA_BODY_SUBMERGED);
        if (d) {
            d = 2.6f;
        } else {
            d = 0.21f;
        }
        mario->jumpVelocityY += d;
        mario->jumpAccelerationY = 0.0f;
        mario->jumpJerkY = 0.0f;
        mario->jumpSnapY = 0.0f;
        if (mario->jumpVelocityY >= 0.0f) {
            mario->jumpVelocityY = -0.08f;
            mario->jumpAccelerationY = -0.03f;
            mario->jumpJerkY = 0.009f;
            mario->jumpSnapY = 0.0f;
            mario->currSubMotionId = 0x20;
        }
        if (kpaChkHitobjRide()) {
            mario->kpaSpeed = 0.0f;
            mario->baseSpeed = 0.0f;
        }
        kpa_swim_splash();
        break;
    }
    case 0x20:
        if (kpaChkHitobjRide()) {
            mario->kpaSpeed = 0.0f;
            mario->baseSpeed = 0.0f;
        }
        break;
    case 0x28:
        if (kpaGetAttackMode() == 0) {
            kpaChgPose("KPA_W_1", "KPA2_Z_2");
        }
        mario->flags &= ~MARIO_FLAG_IS_JUMPING;
        mario->flags |= MARIO_FLAG_IS_FALLING;
        mario->jumpVelocityY = -0.283f;
        mario->jumpAccelerationY = -0.023f;
        mario->jumpJerkY = 0.01f;
        mario->jumpSnapY = -0.001f;
        mario->currSubMotionId = 0x29;
        mario->multiTimer = 0x18;
    case 0x29:
#if VERSION <= VERSION_DEMO_US
        if (mario->buttonTrg & PAD_BUTTON_A) {
            mario->currSubMotionId = 20;
            } else {
                mario->multiTimer = (s32)mario->multiTimer - 1;
                if ((s32)mario->multiTimer <= 0) {
#else
        if ((mario->buttonTrg & PAD_BUTTON_A) && !kpaFireAttackCheck()) {
            mario->currSubMotionId = 20;
        } else {
            mario->multiTimer--;
            if (mario->multiTimer <= 0) {
#endif
                mario->currSubMotionId = 0x1E;
            }
        }
        break;
    }
}

/** Landing feedback outside kpa_03: the landing thud, and for big or giant Bowser a small camera shake. */
void quake_kpaLandOn(void) {
    MarioWork* mario = marioGetPtr();
    if (kpaGetStageType() != 1) {
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_LANDING1), &mario->position);
        if (kpaGetLevel() >= 2) {
            evtEntry(quake_kpaLandOn_evt, 0, 0);
        }
    }
}

/** Starts the ground pound camera shake. */
void quake_kpaHipAttack(void) {
    evtEntry(quake_kpaHipAttack_evt, 0, 0);
}
