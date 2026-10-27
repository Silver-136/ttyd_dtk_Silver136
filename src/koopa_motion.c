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
#include "koopa.h"
#include "mot/mot_jump.h"
#include "driver/animdrv.h"
#include "koopa_motion.h"
#include "evt/evt_cmd.h"
#include "koopa_hit.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "eff/eff_mizutama.h"
#include "mario/mario_hit.h"

f32 kpaStickSpd(s32 dash);

typedef struct V3 {
    s32 a, b, c;
} V3;

f32 _walkSpdTbl[4] = {0.6f, 1.4f, 1.4f, 1.4f};
f32 _dashSpdTbl[4] = {1.2f, 2.0f, 2.0f, 2.0f};
s32 _rateTbl[4] = {0, 0, 0, 0};
s32 _rateTbl2[4] = {0, 0, 0, 0};
f32 scaleDt[9] = {1.0f, 0.0f, 2.0f, 1.0f, 3.0f, 2.0f, 4.0f, 3.0f, 5.0f};

EVT_BEGIN(quake_kpaLandOn_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.009765625f), 30)
    RETURN
EVT_END()

EVT_BEGIN(quake_kpaHipAttack_evt)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0f), FLOAT(0.01953125f), 60)
    RETURN
EVT_END()


static inline void kpa_hip_launch(MarioWork* m) {
    m->currSubMotionId = 0x14;
    m->jumpAccelerationY = -0.4744f;
    m->jumpJerkY = 0.4704f;
    m->jumpSnapY = -0.1904f;
    m->jumpVelocityY = m->jumpAccelerationY;
}

static inline s32 kpa_stay_chk(void) {
    MarioWork* m2 = marioGetPtr();
    if (kpaGetStageViewType() == 1) {
        return 0;
    }
    if (!(m2->buttonTrg & PAD_BUTTON_A)) {
        return 0;
    }
    if (kpaGetBodyStatus() == 2) {
        marioChgMot(MARIO_MOTION_KPA_SWIM);
        return 1;
    }
    marioChgMot(MARIO_MOTION_JUMP);
    return 1;
}

static inline void kpa_swim_splash(void) {
    MarioWork* m = marioGetPtr();
    s32 st;
    s32 lim;
    m->kpaWork->swimSplashTimer++;
    st = (s32)m->currSubMotionId;
    if (st == 0x1E || st == 0x1F) {
        if (m->kpaWork->swimSplashTimer > 0) {
            f32 rx;
            f32 rz;
            m->kpaWork->swimSplashTimer = 0;
            rx = (f32)(irand(40) - 20);
            rz = (f32)(irand(40) - 20);
            effMizutamaEntry(0, m->position.x + rx, 0.6f * m->wPlayerCollisionBox.y + (m->position.y + rz), m->position.z,
                             m->directionView < 180.0f ? 225.0f : 135.0f);
        }
    } else {
        s32 in = FALSE;
        if (st >= 0 && st < 0x14) {
            in = TRUE;
        }
        lim = in ? 10 : 30;
        if (m->kpaWork->swimSplashTimer > lim) {
            m->kpaWork->swimSplashTimer = 0;
            effMizutamaEntry(0, m->position.x, m->position.y + m->wPlayerCollisionBox.y / 3.0f, m->position.z,
                             m->directionView < 180.0f ? 225.0f : 135.0f);
        }
    }
}

BOOL kpaHipAttackOk(void) {
    MarioWork* m = marioGetPtr();
    if (m->currMotionId == MARIO_MOTION_HIP) {
        return FALSE;
    }
    return ((m->kpaWork->flags >> 4) & 1) ^ 1;
}

f32 kpaStickSpd(s32 dash) {
    MarioWork* m = marioGetPtr();
    f32 stick = (s8)m->wStickDir1;
    f32 spd;
    f32 max;
    f32 lim = 70.0f;
    f32 k;
    if (stick >= lim) {
        stick = lim;
    }
    lim = 70.0f;
    if (stick <= -lim) {
        stick = -lim;
    }
    spd = m->kpaSpeed;
    if (!stick) {
        k = -0.2f;
        if (m->hitobjWall) {
            k = -0.7f;
        }
        spd += spd * k;
        if (__fabsf(spd) < 0.01f) {
            spd = 0.0f;
        }
    } else {
        if (dash == 0) {
            max = kpaGetBodyStatus() == 2 ? 0.2f : 1.3f;
        } else {
            max = kpaGetBodyStatus() == 2 ? 0.633f : 2.3f;
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
    m->kpaSpeed = spd;
    m->baseSpeed = __fabsf(spd);
    {
        f32 a = angleABf(0.0f, 0.0f, spd, 0.0f);
        if (m->baseSpeed) {
            m->directionView = a;
        }
    }
    return m->baseSpeed;
}

void kpa_stay(void) {
    MarioWork* m = marioGetPtr();
    f32 spd;
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        m->airTimer = 0;
        m->multiTimer = 0;
        m->currSubMotionId = 0;
        if (kpaGetAttackMode() == 0) {
            if (!(m->kpaWork->flags & KPA_WORK_FLAG_WEARY)) {
                kpaChgPose("KPA_S_1", "KPA2_S_1");
            } else {
                kpaChgPose("KPA_S_2", "KPA2_S_1");
            }
        }
        if (!(m->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            m->baseSpeed = 0.0f;
        }
    }
    if (kpaGetAttackMode() == 0) {
        s32 r = kpa_stay_chk();
        if (r == 0) {
            if (m->buttonTrg & PAD_BUTTON_B) {
                kpaAttackStart();
            } else {
                if (m->forceMoveTimer == 0) {
                    spd = kpaStickSpd(0);
                } else {
                    spd = m->baseSpeed;
                }
                if (kpaGetStageViewType() == 0) {
                    m->baseSpeed = spd;
                    if (m->baseSpeed > 0.0f) {
                        marioChgMot(MARIO_MOTION_WALK);
                    }
                } else if (m->controlStickSensitivity > 0.0f) {
                    marioChgMot(MARIO_MOTION_WALK);
                }
            }
        }
    }
}

void kpa_walk(void) {
    MarioWork* m = marioGetPtr();
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        m->currSubMotionId = 0;
        m->multiTimer = 0;
        if (!(m->kpaWork->flags & KPA_WORK_FLAG_WEARY)) {
            kpaChgPose("KPA_W_1", "KPA2_W_1");
        } else {
            kpaChgPose("KPA_W_2", "KPA2_W_1");
        }
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
        if (kpaGetBodyStatus() == 2) {
            m->mot.kpa.animFrame++;
            if (m->mot.kpa.animFrame > 0x50) {
                m->mot.kpa.animFrame = 0;
            }
            m->poseTime = m->mot.kpa.animFrame / 2;
            animPoseSetLocalTime(m->paperAnimGroupId[marioAnimeId()], (f32)m->poseTime);
            if (kpaGetBodyStatus() == 0) {
                if ((s32)m->multiTimer % 40 == 0) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE2), &m->position);
                } else if ((s32)m->multiTimer % 20 == 0) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE1), &m->position);
                }
                m->multiTimer++;
            }
        } else if (kpaGetBodyStatus() == 0) {
            if ((s32)m->multiTimer % 40 == 0) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE2), &m->position);
            } else if ((s32)m->multiTimer % 20 == 0) {
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
        if (kpaGetBodyStatus() == 2) {
            m->mot.kpa.animFrame++;
            if (m->mot.kpa.animFrame > 0x50) {
                m->mot.kpa.animFrame = 0;
            }
            m->poseTime = m->mot.kpa.animFrame / 2;
            animPoseSetLocalTime(m->paperAnimGroupId[marioAnimeId()], (f32)m->poseTime);
        }
        if (kpaGetBodyStatus() == 0) {
            if ((s32)m->multiTimer % 30 == 0) {
                psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_MOVE2), &m->position);
            } else if ((s32)m->multiTimer % 15 == 0) {
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

void kpa_jump(void) {
    MarioWork* m = marioGetPtr();
    f32 prevVy;
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags |= MARIO_FLAG_HAS_INPUT_JUMP;
        m->multiTimer = 2;
        m->currSubMotionId = 0;
        marioPreJump();
        if (m->currMotionId != MARIO_MOTION_JUMP_3) {
            camFollowYOff();
        }
        kpaChgPose("KPA_J_1A", "KPA2_J_1A");
        {
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_JUMP1), &m->position);
        if (m->airTimer == 0) {
            {
                m->fallStartPos = m->position;
            }
        }
        }
        m->mot.kpa.animFrame = 0;
    }
    m->airTimer++;
    if (m->currMotionId != MARIO_MOTION_JUMP_3) {
        prevVy = m->jumpVelocityY;
        marioMakeJumpPara();
        if (m->jumpVelocityY > 0.0f && m->airTimer >= 4 && !(m->buttonHold & PAD_BUTTON_A)) {
            m->jumpVelocityY += -0.526f;
        }
        if (m->jumpVelocityY <= 0.0f) {
            if (prevVy > 0.0f) {
                m->jumpPeakAirTime = m->airTimer;
            }
            marioSetPeakYpos();
            kpaSetFallPara();
            marioMakeJumpPara();
            marioChgMot(MARIO_MOTION_FALL);
        }
        m->position.y += m->jumpVelocityY;
    }
}

void kpa_fall(void) {
    MarioWork* m = marioGetPtr();
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags &= ~MARIO_FLAG_IS_JUMPING;
        m->flags |= MARIO_FLAG_IS_FALLING;
        m->currSubMotionId = 0;
        if (m->currMotionId != MARIO_MOTION_FALL_2) {
            if (m->characterId == CHARACTER_BOWSER) {
                if (!kpaFireAttackCheck()) {
                    kpaChgPose("KPA_J_1B", "KPA2_J_1B");
                }
            } else if (!(m->flags & MARIO_FLAG_IS_CARRYING_BOBBERY)) {
                marioChgPose("M_J_1C");
            }
        } else {
            m->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        }
        if (m->airTimer == 0) {
            m->fallStartPos = m->position;
        }
    }
    switch ((s32)m->currSubMotionId) {
    case 0:
        if (m->position.y <= m->lastGroundPos.y) {
            if (m->position.y <= 75.0f) {
                if ((m->dispFlags |= MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF) != 0) {
                    camFollowYOn();
                }
            }
            kpaCamFollow();
            m->currSubMotionId = 10;
        }
        break;
    }
    m->airTimer++;
    if (kpaGetStageType() == 1 && kpaGetBodyStatus() <= 1) {
        m->kpaWork->flags |= KPA_WORK_FLAG_WATER_FALL_SFX;
    }
    if (kpaGetBodyStatus() == 2) {
        m = marioGetPtr();
        if (m->currMotionId != MARIO_MOTION_KPA_SWIM) {
            marioChgMot2(MARIO_MOTION_KPA_SWIM);
        }
        kpaClearHitobjRide();
        m->flags &= ~MARIO_FLAG_IS_JUMPING;
        m->flags |= MARIO_FLAG_IS_FALLING;
        m->currSubMotionId = 0x1E;
        m->airTimer = 0;
        m->multiTimer = 0;
        m->baseSpeed = 0.0f;
        m->kpaSpeed = 0.0f;
    }
}

void kpa_hip(void) {
    MarioWork* m = marioGetPtr();
    f32 fall;
    f32 y;
    void* hit;
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags &= ~MARIO_FLAG_HAS_INPUT_JUMP;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_HIP_DROP1), &m->position);
        marioAdjustMoveDir();
        m->currSubMotionId = 10;
        m->multiTimer = 6;
        m->jumpVelocityY = 0.0f;
        m->jumpAccelerationY = 0.0f;
        m->jumpJerkY = 0.0f;
        m->jumpSnapY = 0.0f;
        m->baseSpeed = 0.0f;
        marioChgPose("KPA2_J_1C");
    }
    if (m->fallStartPos.y >= m->position.y && ((u32)m->dispFlags & MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF)) {
        camFollowYOn();
    }
    switch ((s32)m->currSubMotionId) {
    case 10:
        marioChkLandon(&y, 0.0f);
        if (--m->multiTimer <= 0) {
            kpa_hip_launch(m);
        } else {
            break;
        }
    case 20:
        fall = marioGetFallSpd();
        hit = marioChkLandon(&y, fall);
        if (fall < -100.0f) {
            fall = -100.0f;
            m->jumpAccelerationY = m->jumpJerkY = m->jumpSnapY = 0.0f;
            m->jumpVelocityY = fall;
        }
        if (hit) {
            m->hitobjStandOn = hit;
            m->hitobjJumpFrom = NULL;
            m->currMotionId = MARIO_MOTION_HIP_2;
            m->currSubMotionId = 0x15;
            m->multiTimer = 0x14;
            m->position.y = y;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_HIP_DROP2), &m->position);
            evtEntry(quake_kpaHipAttack_evt, 0, 0);
            m->airTimer = 0;
            m->jumpPeakAirTime = 0;
        } else {
            m->position.y += fall;
        }
        break;
    case 21:
        if (--m->multiTimer <= 0) {
            if (m->position.y > 75.0f) {
                camFollowYOn();
            }
            if ((m->buttonHold & PAD_BUTTON_A) && !m->hitobjStandOn) {
                kpa_hip_launch(m);
            } else {
                m->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
                marioChgMotSub(MARIO_MOTION_STAY, 0);
            }
        }
        break;
    }
}

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

BOOL kpaPowDown(void) {
    MarioWork* m = marioGetPtr();
    kpaCancelAttackMode();
    if (kpaGetLevel() > 0) {
        m->kpaWork->level--;
        m->kpaNoHitTimer = sysMsec2Frame(3000);
        marioChgMot(MARIO_MOTION_KPA_POW_DOWN);
        return TRUE;
    }
    m->kpaWork->flags |= KPA_WORK_FLAG_DEAD;
    return FALSE;
}

void kpaSetSpec(void) {
    MarioWork* m = marioGetPtr();
    s32 lv;
    m->wPlayerCollisionBox.x = kpaGetWidth(m->kpaWork->level);
    lv = m->kpaWork->level;
    if (lv == 0) {
        m->wPlayerCollisionBox.y = 24.0f;
    } else if (lv == 1) {
        m->wPlayerCollisionBox.y = 74.0f;
    } else if (lv == 2) {
        m->wPlayerCollisionBox.y = 103.6f;
    } else if (lv == 3) {
        m->wPlayerCollisionBox.y = 222.0f;
    }
}

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
    switch ((s32)m->currSubMotionId) {
    case 0:
        m->multiTimer = 5;
        m->currSubMotionId = 1;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_BIG1), &m->position);
        break;
    default:
        if (--m->multiTimer > 0) {
            break;
        }
        k = (scaleDt - 1)[(s32)m->currSubMotionId];
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
    switch ((s32)m->currSubMotionId) {
    case 0:
        m->multiTimer = 5;
        m->currSubMotionId = 1;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_SMALL1), &m->position);
        break;
    default:
        if (--m->multiTimer > 0) {
            break;
        }
        k = (scaleDt - 1)[(s32)m->currSubMotionId];
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
void kpaSwimMove(void) {
    MarioWork* m = marioGetPtr();
    f32 spd = m->kpaSpeed;
    f32 stick = (s8)m->wStickDir1;
    f32 f31;
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
            lim = 40.0f;
            if (spd > lim) {
                spd = lim;
            }
        }
    }
    m->kpaSpeed = spd;
    f31 = 0.06f * __fabsf(spd);
    if (f31 >= 2.0f) {
        f31 = 2.0f;
    }
    {
        f32 a = angleABf(0.0f, 0.0f, spd, 0.0f);
        if (f31 < 0.01f) {
            f31 = 0.0f;
        }
        m->baseSpeed = f31;
        if (f31) {
            m->directionView = a;
        }
    }
    m->moveDist = f31;
}

#pragma dont_inline reset

void kpa_swim(void) {
    MarioWork* m = marioGetPtr();
    f32 v;
    if (m->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        m->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        m->airTimer = 0;
        m->multiTimer = 0;
        m->currSubMotionId = 0;
        m->baseSpeed = 0.0f;
        m->kpaSpeed = 0.0f;
        marioGetPtr()->kpaWork->flags |= KPA_WORK_FLAG_NO_HIP_ATTACK;
    }
    if (m->buttonTrg & PAD_BUTTON_B) {
        kpaAttackStart2();
    }
    if (kpaFireAttackCheck() == 1) {
        m->wStickDir1 = 0;
        m->wStickDir2 = 0;
    } else if (kpaGetBodyStatus() == 2 && (m->buttonTrg & PAD_BUTTON_A)) {
        s32 st = (s32)m->currSubMotionId;
        if (st >= 0 && st <= 1 && m->position.y < 280.0f) {
            m->currSubMotionId = 0;
        } else if (st >= 0x1E && st <= 0x20) {
            m->currSubMotionId = 0;
        }
    }
    kpaSwimMove();
    kpa_swim_splash();
    switch ((s32)m->currSubMotionId) {
    case 0:
        if (m->hitobjStandOn != 0) {
            m->jumpVelocityY = 1.0f;
            m->jumpAccelerationY = 0.0f;
            m->jumpJerkY = 0.0f;
            m->jumpSnapY = -0.0001f;
        } else {
            m->jumpVelocityY = 1.52f;
            m->jumpAccelerationY = 0.005f;
            m->jumpJerkY = 0.006f;
            m->jumpSnapY = -0.0008f;
        }
        kpaClearHitobjRide();
        m->currSubMotionId = 1;
        m->flags &= ~MARIO_FLAG_IS_FALLING;
        m->flags |= MARIO_FLAG_IS_JUMPING;
        psndSFXOn_3D("SFX_KUPPA_SWIM1", &m->position);
        if (kpaGetAttackMode() == 0) {
            kpaChgPose("KPA_W_1", "KPA2_W_2");
        }
    case 1:
        if (kpaGetHitobjHead2()) {
            m->currSubMotionId = 0x1E;
            m->jumpVelocityY += 0.02f * (-2.0f - m->jumpVelocityY);
            m->jumpAccelerationY = 0.0f;
            m->jumpJerkY = 0.0f;
            m->jumpSnapY = 0.0f;
            break;
        }
        v = marioMakeJumpPara();
        m->position.y += v;
        if (m->position.y + m->wPlayerCollisionBox.y >= 370.0f) {
            if (v < 0.0f) {
                m->currSubMotionId = 0x28;
            }
#ifndef VERSION_G8ME01
            m->jumpVelocityY += 0.02f * (-2.0f - m->jumpVelocityY);
            m->jumpAccelerationY = 0.0f;
            m->jumpJerkY = 0.0f;
            m->jumpSnapY = 0.0f;
#endif
        } else if (v < 0.0f) {
            m->currSubMotionId = 0x1E;
        }
        break;
    case 10:
        kpaClearHitobjRide();
        m->jumpVelocityY = 2.0f;
        m->jumpAccelerationY = -0.03f;
        m->jumpJerkY = 0.0f;
        m->jumpSnapY = 0.0f;
        m->currSubMotionId = 0xB;
        m->flags &= ~MARIO_FLAG_IS_FALLING;
        m->flags |= MARIO_FLAG_IS_JUMPING;
        if (kpaGetAttackMode() == 0) {
            kpaChgPose("KPA_W_1", "KPA2_W_2");
        }
    case 11:
        v = marioMakeJumpPara();
        m->position.y += v;
        if (kpaGetBodyStatus() == 1) {
            m->jumpVelocityY += 0.06f * (-2.0f - m->jumpVelocityY);
            m->jumpAccelerationY = 0.0f;
            m->jumpJerkY = 0.0f;
            m->jumpSnapY = 0.0f;
            if (v < 0.0f) {
                m->currSubMotionId = 0x28;
            }
        } else if (v < 0.0f) {
            m->currSubMotionId = 0x1E;
        }
        break;
    case 20:
        m->kpaWork->flags |= KPA_WORK_FLAG_WATER_FALL_SFX;
        m->flags &= ~MARIO_FLAG_IS_FALLING;
        m->flags |= MARIO_FLAG_IS_JUMPING;
        kpaChgPose("KPA_J_1A", "KPA2_J_1A");
        psndSFXOn_3D("SFX_KUPPA_WATER_UP1", &m->position);
        m->jumpVelocityY = 5.0f;
        m->jumpAccelerationY = -0.13f;
        m->jumpJerkY = 0.0f;
        m->jumpSnapY = -0.001f;
        m->currSubMotionId = 0x15;
    case 21:
        v = marioMakeJumpPara();
        m->position.y += v;
        if (v <= 0.5f) {
            m->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
            kpaSetFallPara();
            marioMakeJumpPara();
            marioChgMot(MARIO_MOTION_FALL);
            return;
        }
        break;
    case 22:
        break;
    }
    switch ((s32)m->currSubMotionId) {
    case 0x1E:
        if (m->kpaWork->flags & KPA_WORK_FLAG_WATER_FALL_SFX) {
            m->kpaWork->flags &= ~KPA_WORK_FLAG_WATER_FALL_SFX;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_WATER_FALL1), &m->position);
        }
        if (kpaGetAttackMode() == 0) {
            kpaChgPose("KPA_W_1", "KPA2_Z_2");
        }
        m->flags &= ~MARIO_FLAG_IS_JUMPING;
        m->flags |= MARIO_FLAG_IS_FALLING;
        if (m->jumpVelocityY < 0.0f) {
            m->currSubMotionId = 0x1F;
        } else {
            m->jumpVelocityY = -0.08f;
            m->jumpAccelerationY = -0.03f;
            m->jumpJerkY = 0.009f;
            m->jumpSnapY = 0.0f;
            m->currSubMotionId = 0x20;
        }
        kpa_swim_splash();
        if (marioBgmodeChk() != 1) {
            camFollowYOn();
        }
        break;
    case 0x1F: {
        f32 d = (2.0f - m->jumpVelocityY) * (f32)(kpaGetBodyStatus() == 2);
        if (d) {
            d = 2.6f;
        } else {
            d = 0.21f;
        }
        m->jumpVelocityY += d;
        m->jumpAccelerationY = 0.0f;
        m->jumpJerkY = 0.0f;
        m->jumpSnapY = 0.0f;
        if (m->jumpVelocityY >= 0.0f) {
            m->jumpVelocityY = -0.08f;
            m->jumpAccelerationY = -0.03f;
            m->jumpJerkY = 0.009f;
            m->jumpSnapY = 0.0f;
            m->currSubMotionId = 0x20;
        }
        if (kpaChkHitobjRide()) {
            m->kpaSpeed = 0.0f;
            m->baseSpeed = 0.0f;
        }
        kpa_swim_splash();
        break;
    }
    case 0x20:
        if (kpaChkHitobjRide()) {
            m->kpaSpeed = 0.0f;
            m->baseSpeed = 0.0f;
        }
        break;
    case 0x28:
        if (kpaGetAttackMode() == 0) {
            kpaChgPose("KPA_W_1", "KPA2_Z_2");
        }
        m->flags &= ~MARIO_FLAG_IS_JUMPING;
        m->flags |= MARIO_FLAG_IS_FALLING;
        m->jumpVelocityY = -0.283f;
        m->jumpAccelerationY = -0.023f;
        m->jumpJerkY = 0.01f;
        m->jumpSnapY = -0.001f;
        m->currSubMotionId = 0x29;
        m->multiTimer = 0x18;
    case 0x29:
        if ((m->buttonTrg & PAD_BUTTON_A) && !kpaFireAttackCheck()) {
            m->currSubMotionId = 0x14;
        } else {
            m->multiTimer = (s32)m->multiTimer - 1;
            if ((s32)m->multiTimer <= 0) {
                m->currSubMotionId = 0x1E;
            }
        }
        break;
    }
}

void quake_kpaLandOn(void) {
    MarioWork* m = marioGetPtr();
    if (kpaGetStageType() != 1) {
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_KUPPA_LANDING1), &m->position);
        if (kpaGetLevel() >= 2) {
            evtEntry(quake_kpaLandOn_evt, 0, 0);
        }
    }
}

void quake_kpaHipAttack(void) {
    evtEntry(quake_kpaHipAttack_evt, 0, 0);
}
