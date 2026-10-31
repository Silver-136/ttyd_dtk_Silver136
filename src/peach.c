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
    MarioWork* mp = marioGetPtr();
    s32 sfx, halfSfx;
    if (!(mp->peachFlags & PEACH_FLAG_GUNDAN)) {
        if ((mp->peachFlags & PEACH_FLAG_INVISIBLE) && mp->scale.y == 0.0f) {
            sfx = 0x80C;
            halfSfx = 0x80B;
        } else {
            sfx = 0x7C4;
            halfSfx = 0x7C3;
        }
    } else {
        sfx = 0x7E9;
        halfSfx = 0x7E8;
    }
    if (mp->multiTimer % period == 0) {
        psndSFXOn_3D(SFX_NAME_WORD(sfx), &mp->position);
    } else if (mp->multiTimer % halfPeriod == 0) {
        psndSFXOn_3D(SFX_NAME_WORD(halfSfx), &mp->position);
    }
    mp->multiTimer++;
}

/**
 * Condition 0 of evt_peach_set_condition: clears Peach's mood and drops the X-Naut disguise if she wears it,
 * back to her ordinary poses.
 */
void peachSetStsNormal(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->peachFlags & PEACH_FLAG_GUNDAN) {
        peachTransformOff();
    } else {
        mp->peachFlags &= ~PEACH_FLAG_STS_MASK;
        peachChgPose("P_S_1");
    }
}

void peachSetStsAftershower(void) {
    MarioWork* mp = marioGetPtr();
    mp->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mp->peachFlags |= PEACH_FLAG_AFTERSHOWER;
    peachChgPose("P_S_1");
}

void peachSetStsInvisible(void) {
    MarioWork* mp = marioGetPtr();
    mp->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mp->peachFlags |= PEACH_FLAG_INVISIBLE;
    peachChgPose("P_S_1");
}

void peachSetStsHappy(void) {
    MarioWork* mp = marioGetPtr();
    mp->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mp->peachFlags |= PEACH_FLAG_HAPPY;
    peachChgPose("P_S_1");
}

void peachSetStsUneasy(void) {
    MarioWork* mp = marioGetPtr();
    mp->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mp->peachFlags |= PEACH_FLAG_UNEASY;
    peachChgPose("P_S_1");
}

void peachSetStsAngry(void) {
    MarioWork* mp = marioGetPtr();
    mp->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mp->peachFlags |= PEACH_FLAG_ANGRY;
    peachChgPose("P_S_1");
}

void peachSetStsOrdinary(void) {
    MarioWork* mp = marioGetPtr();
    mp->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mp->peachFlags |= PEACH_FLAG_ORDINARY;
    peachChgPose("P_S_1");
}

void peachSetStsCarry(void) {
    MarioWork* mp = marioGetPtr();
    mp->peachFlags &= ~PEACH_FLAG_STS_MASK;
    mp->peachFlags |= PEACH_FLAG_CARRY;
    peachChgPose("P_S_1");
}

/**
 * Peach's version of marioChgPose: maps the base pose names (P_Z_1, P_S_1, P_W_1, P_R_1, P_T_1) to the variant
 * for her current condition, or to the X-Naut model's Z_1/S_1/...
 * while disguised, then starts the pose unless the pose is locked or already playing.
 */
void peachChgPose(const char* name) {
    MarioWork* mp = marioGetPtr();
    if (mp->dispFlags & MARIO_DISP_FLAG_POSE_LOCK) {
        return;
    }
    if (mp->peachFlags & PEACH_FLAG_GUNDAN) {
        if (strcmp(name, "P_Z_1") == 0) name = "Z_1";
        else if (strcmp(name, "P_S_1") == 0) name = "S_1";
        else if (strcmp(name, "P_W_1") == 0) name = "W_1";
        else if (strcmp(name, "P_R_1") == 0) name = "R_1";
        else if (strcmp(name, "P_T_1") == 0) name = "T_1";
    } else if (mp->peachFlags & PEACH_FLAG_AFTERSHOWER) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_2";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_2";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_2";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_2";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_2";
    } else if (mp->peachFlags & PEACH_FLAG_INVISIBLE) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_3";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_3";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_3";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_3";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_3";
    } else if (mp->peachFlags & PEACH_FLAG_UNEASY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_4";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_4";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_4";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_4";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_4";
    } else if (mp->peachFlags & PEACH_FLAG_HAPPY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_5";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_5";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_5";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_5";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_5";
    } else if (mp->peachFlags & PEACH_FLAG_ANGRY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_6";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_6";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_6";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_6";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_6";
    } else if (mp->peachFlags & PEACH_FLAG_ORDINARY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_7";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_7";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_7";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_7";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_13";
    } else if (mp->peachFlags & PEACH_FLAG_CARRY) {
        if (strcmp(name, "P_Z_1") == 0) name = "P_Z_8";
        else if (strcmp(name, "P_S_1") == 0) name = "P_S_8";
        else if (strcmp(name, "P_W_1") == 0) name = "P_W_8";
        else if (strcmp(name, "P_R_1") == 0) name = "P_R_8";
        else if (strcmp(name, "P_T_1") == 0) name = "P_T_14";
    }
    if (mp->animName == NULL || strcmp(mp->animName, name) != 0) {
        mp->animName = name;
        mp->poseTime = 0;
        mp->dispFlags &= ~0x30000000;
        mp->trigFlags |= MARIO_TRIG_FLAG_CHG_ANIM;
    }
}

/**
 * Puts Peach in the X-Naut disguise (evt_peach_transform_gundan_on):
 * switches her model to the X-Naut c_zako_n, its talk poses and a 30x30 collision box.
 */
void peachTransformOn(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->characterId != CHARACTER_PEACH) {
        marioSetCharMode(CHARACTER_PEACH);
    }
    if (!(mp->peachFlags & PEACH_FLAG_GUNDAN)) {
        mp->peachFlags |= PEACH_FLAG_GUNDAN;
        if (mp->paperAnimGroupId[0] >= 0) {
            animPoseRelease(mp->paperAnimGroupId[0]);
        }
        mp->paperAnimGroupId[0] = animPoseEntry("c_zako_n", 2);
        npcSetMarioAutoTalkPose("S_1", "T_1");
        mp->wPlayerCollisionBox.y = 30.0f;
        mp->wPlayerCollisionBox.x = 30.0f;
    }
}

/**
 * Takes off Peach's X-Naut disguise (evt_peach_transform_gundan_off):
 * back to the c_peach model, her own talk poses and her 20 wide, 47 tall collision box.
 */
void peachTransformOff(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->characterId != CHARACTER_PEACH) {
        marioSetCharMode(CHARACTER_PEACH);
    }
    if (mp->peachFlags & PEACH_FLAG_GUNDAN) {
        mp->peachFlags &= ~PEACH_FLAG_GUNDAN;
        if (mp->paperAnimGroupId[0] >= 0) {
            animPoseRelease(mp->paperAnimGroupId[0]);
        }
        mp->paperAnimGroupId[0] = animPoseEntry("c_peach", 2);
        npcSetMarioAutoTalkPose("P_S_1", "P_T_1");
        mp->wPlayerCollisionBox.y = 47.0f;
        mp->wPlayerCollisionBox.x = 20.0f;
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
    MarioWork* mp = marioGetPtr();
    if (mp->currMotionId == MARIO_MOTION_JUMP_3) {
        mp->basePos = mp->position;
        mp->floorMove.x = 0.0f;
        mp->floorMove.y = 0.0f;
        mp->floorMove.z = 0.0f;
        marioBottomless();
        marioPreDisp();
        marioBalloonMain();
        mario_kemuri();
        partyMain();
        marioCamMain();
        mp->frameCounter++;
        return 1;
    }
    if (mp->flags & MARIO_FLAG_IS_STEPPING) {
        marioMove();
        marioChkOverhead();
        if (mp->airTimer > 100) {
            marioChgMot(MARIO_MOTION_FALL);
            marioSetFallPara();
        }
    } else if (mp->flags & MARIO_FLAG_IS_JUMPING) {
        marioJump();
        marioMove();
        marioChkOverhead();
        if (mp->airTimer > 5) {
            marioChkGnd2();
        }
    } else if (mp->flags & MARIO_FLAG_IS_FALLING) {
        marioMove();
        marioFall();
        marioChkOverhead();
        if (mp->currMotionId != MARIO_MOTION_HIP && mp->currMotionId != MARIO_MOTION_HIP_2 && mp->airTimer > 5) {
            marioChkGnd2();
        }
    } else {
        marioMove();
        marioChkOverhead();
        marioChkGnd();
    }
    if (mp->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        mp->forceMoveTimer--;
        if (mp->forceMoveTimer <= 0) {
            mp->forceMoveTimer = 0;
            mp->flags &= ~MARIO_FLAG_FORCED_MOVEMENT;
        }
    }
    marioBoots();
    marioBottomless();
    marioPreDisp();
    marioBalloonMain();
    mario_kemuri();
    partyMain();
    marioCamMain();
    mp->frameCounter++;
    mp->basePos = mp->position;
    mp->floorMove.x = 0.0f;
    mp->floorMove.y = 0.0f;
    mp->floorMove.z = 0.0f;
    return 1;
}

/**
 * Peach's standing motion (from mot_stay): switches to walking on stick input.
 * With no condition set, after 50 seconds idle she plays P_U_1, stands again,
 * then dozes off in P_N_1 until control is taken away.
 */
void peach_stay(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mp->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mp->airTimer = 0;
        mp->multiTimer = 0;
        mp->currSubMotionId = 0;
        peachChgPose("P_S_1");
        if (!(mp->flags & MARIO_FLAG_FORCED_MOVEMENT)) {
            mp->baseSpeed = 0.0f;
        }
    }
    if (mp->controlStickSensitivity > 0.0f) {
        marioChgMot(MARIO_MOTION_WALK);
    }
    if (!(mp->flags & MARIO_FLAG_KEY_OFF)) {
        switch (mp->dispFlags & MARIO_DISP_FLAG_PAPER) {
            case 0:
                if (!(mp->peachFlags & (PEACH_FLAG_STS_MASK | PEACH_FLAG_GUNDAN))) {
                    switch (mp->currSubMotionId) {
                        case 0:
                            if (++mp->multiTimer >= sysMsec2Frame(50000)) {
                                marioChgPose("P_U_1");
                                mp->multiTimer = 80;
                                mp->currSubMotionId = 1;
                                mp->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_PEACH_SAD1_1), &mp->position);
                                marioAdjustMoveDir();
                            }
                            break;
                        case 1:
                            if (--mp->multiTimer < 0) {
                                marioChgPose("P_S_1");
                                mp->multiTimer = 100;
                                mp->currSubMotionId = 2;
                            }
                            break;
                        case 2:
                            if (--mp->multiTimer < 0) {
                                marioChgPose("P_N_1");
                                mp->multiTimer = 0;
                                mp->currSubMotionId = 20;
                                mp->mot.peach.sleepTimer = 0;
                            }
                            break;
                        case 20:
                            if (++mp->mot.peach.sleepTimer >= sysMsec2Frame(10000)) {
                                mp->mot.peach.sleepTimer = sysMsec2Frame(10000);
                            }
                            if (++mp->multiTimer >= 100) {
                                mp->multiTimer = 0;
                            }
                            if (mp->flags & MARIO_FLAG_KEY_OFF) {
                                marioChgPose("P_S_1");
                                mp->currSubMotionId = 0;
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
    MarioWork* mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mp->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        if (mp->forceMoveTimer == 0) {
            mp->baseSpeed = marioGetWalkSpd();
        }
        peachChgPose("P_W_1");
        mp->currSubMotionId = 0;
        mp->multiTimer = 0;
    }
    if (mp->forceMoveTimer == 0) {
        mp->baseSpeed = marioGetWalkSpd();
    }
    if (mp->wStickDir1 * mp->wStickDir1 + mp->wStickDir2 * mp->wStickDir2 > 3025) {
        marioChgMot(MARIO_MOTION_DASH);
    } else {
        mp->directionView = mp->controlStickAngle;
        peachStepSe(40, 20);
        if (!mp->controlStickSensitivity) {
            marioChgMot(MARIO_MOTION_STAY);
        }
    }
}

/**
 * Peach's dashing motion (from mot_walk): runs at Mario's dash speed,
 * drops back to walking when the stick eases off (unless the movement is forced) and to standing when released.
 */
void peach_dash(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mp->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        if (mp->forceMoveTimer == 0) {
            mp->baseSpeed = marioGetDashSpd();
        }
        peachChgPose("P_R_1");
        mp->currSubMotionId = 0;
        mp->multiTimer = 0;
    }
    if (mp->forceMoveTimer == 0) {
        mp->baseSpeed = marioGetDashSpd();
    }
    if (mp->flags & MARIO_FLAG_FORCED_MOVEMENT) {
        peachStepSe(20, 10);
    } else if (mp->wStickDir1 * mp->wStickDir1 + mp->wStickDir2 * mp->wStickDir2 <= 3025) {
        marioChgMot(MARIO_MOTION_WALK);
    } else {
        mp->directionView = mp->controlStickAngle;
        peachStepSe(20, 10);
        if (!mp->controlStickSensitivity) {
            marioChgMot(MARIO_MOTION_STAY);
        }
    }
}

void peach_talk(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mp->multiTimer = 0;
        mp->airTimer = 0;
        mp->currSubMotionId = 0;
        mp->baseSpeed = 0.0f;
        marioAdjustMoveDir();
        peachChgPose("P_T_1");
    }
}
