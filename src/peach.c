// Peach as the player in the Peach interludes (X-Naut fortress): her poses, states (normal, transparent, happy,
// angry...) and motions.

#include "version.h"
#include "dolphin/types.h"
#include "mario/mario.h"
#include "mario/mario_motion.h"
#include "mot/mot_walk.h"
#include "pmario_sound.h"
#include "system.h"
#include "driver/animdrv.h"
#include "peach.h"
#include "sfx_name_word.h"
#include "libc_decls.h"
#include "mario/mario_cam.h"
#include "mario/mario_hit.h"
#include "mario/mario_sbr.h"
#include "driver/npcdrv.h"
#include "mario/mario_balloon.h"
#include "party.h"
#include "mario/mario_kemuri.h"


void peachChgPose(const char* name);

/**
 * Plays Peach's footstep sounds while she walks or dashes:
 * one sound every d1 frames and the other on the frames in between that are multiples of d2.
 * The sounds differ when she wears the X-Naut disguise and when she is invisible with no height.
 */
static inline void peachStepSe(s32 period, s32 halfPeriod) {
    MarioWork* mario = marioGetPtr();
    s32 sfx, halfSfx;
    if (!(mario->peachFlags & PEACH_FLAG_GUNDAN)) {
        if ((mario->peachFlags & PEACH_FLAG_INVISIBLE) && mario->scale.y == 0.0f) {
            sfx = PSND_SFX_PEACH_BAREFOOT_MOVE1R;
            halfSfx = PSND_SFX_PEACH_BAREFOOT_MOVE1L;
        } else {
            sfx = PSND_SFX_PEACH_MOVE1R;
            halfSfx = PSND_SFX_PEACH_MOVE1L;
        }
    } else {
        sfx = PSND_SFX_PEACH_3RD_P_MOVE1R;
        halfSfx = PSND_SFX_PEACH_3RD_P_MOVE1L;
    }
    if (mario->multiTimer % period == 0) {
        psndSFXOn_3D(SFX_NAME_WORD(sfx), &mario->position);
    } else if (mario->multiTimer % halfPeriod == 0) {
        psndSFXOn_3D(SFX_NAME_WORD(halfSfx), &mario->position);
    }
    mario->multiTimer++;
}

/**
 * Condition 0 of evt_peach_set_condition: clears Peach's mood and drops the X-Naut disguise if she wears it,
 * back to her ordinary poses.
 */
void peachSetStsNormal(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->peachFlags & PEACH_FLAG_GUNDAN) {
        peachTransformOff();
    } else {
        mario->peachFlags &= ~PEACH_FLAG_STS_MASK;
        peachChgPose("P_S_1");
    }
}

void peachSetStsAftershower(void) {
    MarioWork* mario = marioGetPtr();
    mario->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mario->peachFlags |= PEACH_FLAG_AFTERSHOWER;
    peachChgPose("P_S_1");
}

void peachSetStsInvisible(void) {
    MarioWork* mario = marioGetPtr();
    mario->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mario->peachFlags |= PEACH_FLAG_INVISIBLE;
    peachChgPose("P_S_1");
}

void peachSetStsHappy(void) {
    MarioWork* mario = marioGetPtr();
    mario->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mario->peachFlags |= PEACH_FLAG_HAPPY;
    peachChgPose("P_S_1");
}

void peachSetStsUneasy(void) {
    MarioWork* mario = marioGetPtr();
    mario->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mario->peachFlags |= PEACH_FLAG_UNEASY;
    peachChgPose("P_S_1");
}

void peachSetStsAngry(void) {
    MarioWork* mario = marioGetPtr();
    mario->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mario->peachFlags |= PEACH_FLAG_ANGRY;
    peachChgPose("P_S_1");
}

void peachSetStsOrdinary(void) {
    MarioWork* mario = marioGetPtr();
    mario->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mario->peachFlags |= PEACH_FLAG_ORDINARY;
    peachChgPose("P_S_1");
}

void peachSetStsCarry(void) {
    MarioWork* mario = marioGetPtr();
    mario->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mario->peachFlags |= PEACH_FLAG_CARRY;
    peachChgPose("P_S_1");
}

/**
 * Peach's version of marioChgPose: maps the base pose names (P_Z_1, P_S_1, P_W_1, P_R_1, P_T_1) to the variant
 * for her current condition, or to the X-Naut model's Z_1/S_1/...
 * while disguised, then starts the pose unless the pose is locked or already playing.
 */
void peachChgPose(const char* name) {
    MarioWork* mario = marioGetPtr();
    if (mario->dispFlags & MARIO_DISP_FLAG_POSE_LOCK) {
        return;
    }
    if (mario->peachFlags & PEACH_FLAG_GUNDAN) {
        if (strcmp(name, "P_Z_1") == 0) name = "Z_1";
        else if (strcmp(name, "P_S_1") == 0) name = "S_1";
        else if (strcmp(name, "P_W_1") == 0) name = "W_1";
        else if (strcmp(name, "P_R_1") == 0) name = "R_1";
        else if (strcmp(name, "P_T_1") == 0) name = "T_1";
    } else if (mario->peachFlags & PEACH_FLAG_AFTERSHOWER) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_2";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_2";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_2";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_2";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_2";
    } else if (mario->peachFlags & PEACH_FLAG_INVISIBLE) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_3";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_3";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_3";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_3";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_3";
    } else if (mario->peachFlags & PEACH_FLAG_UNEASY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_4";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_4";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_4";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_4";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_4";
    } else if (mario->peachFlags & PEACH_FLAG_HAPPY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_5";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_5";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_5";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_5";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_5";
    } else if (mario->peachFlags & PEACH_FLAG_ANGRY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_6";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_6";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_6";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_6";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_6";
    } else if (mario->peachFlags & PEACH_FLAG_ORDINARY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_7";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_7";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_7";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_7";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_13";
    } else if (mario->peachFlags & PEACH_FLAG_CARRY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_8";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_8";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_8";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_8";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_14";
    }
    if (mario->animName == NULL || strcmp(mario->animName, name) != 0) {
        mario->animName = name;
        mario->poseTime = 0;
        mario->dispFlags &= ~0x30000000;
        mario->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
    }
}

/**
 * Puts Peach in the X-Naut disguise (evt_peach_transform_gundan_on):
 * switches her model to the X-Naut c_zako_n, its talk poses and a 30x30 collision box.
 */
void peachTransformOn(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->characterId != CHARACTER_PEACH) {
        marioSetCharMode(CHARACTER_PEACH);
    }
    if (!(mario->peachFlags & PEACH_FLAG_GUNDAN)) {
        mario->peachFlags |= PEACH_FLAG_GUNDAN;
        if (mario->paperAnimGroupId[0] >= 0) {
            animPoseRelease(mario->paperAnimGroupId[0]);
        }
        mario->paperAnimGroupId[0] = animPoseEntry("c_zako_n", 2);
        npcSetMarioAutoTalkPose("S_1", "T_1");
        mario->wPlayerCollisionBox.y = 30.0f;
        mario->wPlayerCollisionBox.x = 30.0f;
    }
}

/**
 * Takes off Peach's X-Naut disguise (evt_peach_transform_gundan_off):
 * back to the c_peach model, her own talk poses and her 20 wide, 47 tall collision box.
 */
void peachTransformOff(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->characterId != CHARACTER_PEACH) {
        marioSetCharMode(CHARACTER_PEACH);
    }
    if (mario->peachFlags & PEACH_FLAG_GUNDAN) {
        mario->peachFlags &= ~PEACH_FLAG_GUNDAN;
        if (mario->paperAnimGroupId[0] >= 0) {
            animPoseRelease(mario->paperAnimGroupId[0]);
        }
        mario->paperAnimGroupId[0] = animPoseEntry("c_peach", 2);
        npcSetMarioAutoTalkPose("P_S_1", "P_T_1");
        mario->wPlayerCollisionBox.y = 47.0f;
        mario->wPlayerCollisionBox.x = 20.0f;
    }
}

/** Called by marioSetCharMode when the player becomes Peach; does nothing. */
void peachPreInit(void) {}

/**
 * Peach's per-frame player update, run by marioMain instead of Mario's:
 * moves her, handles stepping, jumping and falling against the ground, counts down forced movement,
 * and updates balloons, dust, the partner slot and the camera. Always returns 1.
 */
s32 peachMain(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->currMotionId == MARIO_MOTION_JUMP_3) {
        mario->basePos = mario->position;
        mario->floorMove.x = 0.0f;
        mario->floorMove.y = 0.0f;
        mario->floorMove.z = 0.0f;
        marioBottomless();
        marioPreDisp();
        marioBalloonMain();
        mario_kemuri();
        partyMain();
        marioCamMain();
        mario->frameCounter++;
        return 1;
    }
    if (mario->flags & MARIO_FLAG_IS_STEPPING) {
        marioMove();
        marioChkOverhead();
        if (mario->airTimer > 100) {
            marioChgMot(MARIO_MOTION_FALL);
            marioSetFallPara();
        }
    } else if (mario->flags & MARIO_FLAG_IS_JUMPING) {
        marioJump();
        marioMove();
        marioChkOverhead();
        if (mario->airTimer > 5) {
            marioChkGnd2();
        }
    } else if (mario->flags & MARIO_FLAG_IS_FALLING) {
        marioMove();
        marioFall();
        marioChkOverhead();
        if (mario->currMotionId != MARIO_MOTION_HIP && mario->currMotionId != MARIO_MOTION_HIP_2 && mario->airTimer > 5) {
            marioChkGnd2();
        }
    } else {
        marioMove();
        marioChkOverhead();
        marioChkGnd();
    }
    if (mario->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        mario->forceMoveTimer--;
        if (mario->forceMoveTimer <= 0) {
            mario->forceMoveTimer = 0;
            mario->flags &= ~MARIO_FLAG_FORCED_MOVEMENT;
        }
    }
    marioBoots();
    marioBottomless();
    marioPreDisp();
    marioBalloonMain();
    mario_kemuri();
    partyMain();
    marioCamMain();
    mario->frameCounter++;
    mario->basePos = mario->position;
    mario->floorMove.x = 0.0f;
    mario->floorMove.y = 0.0f;
    mario->floorMove.z = 0.0f;
    return 1;
}

/**
 * Peach's standing motion (from mot_stay): switches to walking on stick input.
 * With no condition set, after 50 seconds idle she plays P_U_1, stands again,
 * then dozes off in P_N_1 until control is taken away.
 */
void peach_stay(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->airTimer = 0;
        mario->multiTimer = 0;
        mario->currSubMotionId = 0;
        peachChgPose("P_S_1");
        if (!(mario->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            mario->baseSpeed = 0.0f;
        }
    }
    if (mario->controlStickSensitivity > 0.0f) {
        marioChgMot(MARIO_MOTION_WALK);
    }
    if (!(mario->flags & MARIO_FLAG_KEY_OFF)) {
        switch (mario->dispFlags & MARIO_DISP_FLAG_PAPER) {
            case 0:
                if (!(mario->peachFlags & (PEACH_FLAG_STS_MASK | PEACH_FLAG_GUNDAN))) {
                    switch (mario->currSubMotionId) {
                        case 0:
                            if (++mario->multiTimer >= sysMsec2Frame(50000)) {
                                marioChgPose("P_U_1");
                                mario->multiTimer = 80;
                                mario->currSubMotionId = 1;
                                mario->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_PEACH_SAD1_1), &mario->position);
                                marioAdjustMoveDir();
                            }
                            break;
                        case 1:
                            if (--mario->multiTimer < 0) {
                                marioChgPose("P_S_1");
                                mario->multiTimer = 100;
                                mario->currSubMotionId = 2;
                            }
                            break;
                        case 2:
                            if (--mario->multiTimer < 0) {
                                marioChgPose("P_N_1");
                                mario->multiTimer = 0;
                                mario->currSubMotionId = 20;
                                mario->mot.peach.sleepTimer = 0;
                            }
                            break;
                        case 20:
                            if (++mario->mot.peach.sleepTimer >= sysMsec2Frame(10000)) {
                                mario->mot.peach.sleepTimer = sysMsec2Frame(10000);
                            }
                            if (++mario->multiTimer >= 100) {
                                mario->multiTimer = 0;
                            }
                            if (mario->flags & MARIO_FLAG_KEY_OFF) {
                                marioChgPose("P_S_1");
                                mario->currSubMotionId = 0;
                            }
                            break;
                    }
                }
                break;
        }
    }
}

/**
 * Peach's walking motion (from mot_walk): walks at Mario's walk speed,
 * switches to dashing when the stick is pushed far, to standing when it is released,
 * with footsteps every 20 frames.
 */
void peach_walk(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        if (mario->forceMoveTimer == 0) {
            mario->baseSpeed = marioGetWalkSpd();
        }
        peachChgPose("P_W_1");
        mario->currSubMotionId = 0;
        mario->multiTimer = 0;
    }
    if (mario->forceMoveTimer == 0) {
        mario->baseSpeed = marioGetWalkSpd();
    }
    if (mario->wStickDir1 * mario->wStickDir1 + mario->wStickDir2 * mario->wStickDir2 > 3025) {
        marioChgMot(MARIO_MOTION_DASH);
    } else {
        mario->directionView = mario->controlStickAngle;
        peachStepSe(40, 20);
        if (!mario->controlStickSensitivity) {
            marioChgMot(MARIO_MOTION_STAY);
        }
    }
}

/**
 * Peach's dashing motion (from mot_walk): runs at Mario's dash speed,
 * drops back to walking when the stick eases off (unless the movement is forced) and to standing when released.
 */
void peach_dash(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        if (mario->forceMoveTimer == 0) {
            mario->baseSpeed = marioGetDashSpd();
        }
        peachChgPose("P_R_1");
        mario->currSubMotionId = 0;
        mario->multiTimer = 0;
    }
    if (mario->forceMoveTimer == 0) {
        mario->baseSpeed = marioGetDashSpd();
    }
    if (mario->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        peachStepSe(20, 10);
    } else if (mario->wStickDir1 * mario->wStickDir1 + mario->wStickDir2 * mario->wStickDir2 <= 3025) {
        marioChgMot(MARIO_MOTION_WALK);
    } else {
        mario->directionView = mario->controlStickAngle;
        peachStepSe(20, 10);
        if (!mario->controlStickSensitivity) {
            marioChgMot(MARIO_MOTION_STAY);
        }
    }
}

/**
 * Peach's talking motion (from mario_motion): stops her and turns her to the talk pose.
 * In the PAL release she returns to the standing pose after 60 frames while in the X-Naut disguise.
 */
void peach_talk(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->multiTimer = 0;
        mario->airTimer = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
#if VERSION == VERSION_EU
        if (mario->peachFlags & PEACH_FLAG_GUNDAN) {
            mario->multiTimer = 60;
            mario->currSubMotionId = 10;
        }
#endif
        marioAdjustMoveDir();
        peachChgPose("P_T_1");
    }
#if VERSION == VERSION_EU
    // PAL: while transformed, the talk pose drops back to the stand pose after 60 frames
    switch (mario->currSubMotionId) {
        case 10:
            if (--mario->multiTimer <= 0) {
                peachChgPose("P_S_1");
                mario->currSubMotionId = 11;
            }
            break;
        case 0:
        case 11:
            break;
    }
#endif
}
