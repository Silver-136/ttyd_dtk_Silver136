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

static inline void peachStepSe(s32 d1, s32 d2) {
    MarioWork* mp = marioGetPtr();
    s32 a, b;
    if (!(mp->peachFlags & PEACH_FLAG_GUNDAN)) {
        if ((mp->peachFlags & PEACH_FLAG_INVISIBLE) && mp->scale.y == 0.0f) {
            a = 0x80C;
            b = 0x80B;
        } else {
            a = 0x7C4;
            b = 0x7C3;
        }
    } else {
        a = 0x7E9;
        b = 0x7E8;
    }
    if (mp->multiTimer % d1 == 0) {
        psndSFXOn_3D(SFX_NAME_WORD(a), &mp->position);
    } else if (mp->multiTimer % d2 == 0) {
        psndSFXOn_3D(SFX_NAME_WORD(b), &mp->position);
    }
    mp->multiTimer++;
}

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

void peachChgPose(const char* name) {
    MarioWork* mp = marioGetPtr();
    if (mp->dispFlags & MARIO_DISP_FLAG_POSE_LOCK) return;
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

void peachPreInit(void) {}

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
        if (mp->airTimer > 5) marioChkGnd2();
    } else if (mp->flags & MARIO_FLAG_IS_FALLING) {
        marioMove();
        marioFall();
        marioChkOverhead();
        if (mp->currMotionId != MARIO_MOTION_HIP && mp->currMotionId != MARIO_MOTION_HIP_2 && mp->airTimer > 5) marioChkGnd2();
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

void peach_stay(void) {
    MarioWork* mp = marioGetPtr();
#define MT (mp->multiTimer)
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mp->flags &= 0xFFF0FFFF;
        mp->airTimer = 0;
        MT = 0;
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
                            if (++MT >= sysMsec2Frame(50000)) {
                                marioChgPose("P_U_1");
                                MT = 80;
                                mp->currSubMotionId = 1;
                                mp->soundId[0] = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_VOICE_PEACH_SAD1_1), &mp->position);
                                marioAdjustMoveDir();
                            }
                            break;
                        case 1:
                            if (--MT < 0) {
                                marioChgPose("P_S_1");
                                MT = 100;
                                mp->currSubMotionId = 2;
                            }
                            break;
                        case 2:
                            if (--MT < 0) {
                                marioChgPose("P_N_1");
                                MT = 0;
                                mp->currSubMotionId = 20;
                                mp->mot.peach.sleepTimer = 0;
                            }
                            break;
                        case 20:
                            if (++mp->mot.peach.sleepTimer >= sysMsec2Frame(10000)) {
                                mp->mot.peach.sleepTimer = sysMsec2Frame(10000);
                            }
                            if (++MT >= 100) {
                                MT = 0;
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
#undef MT
}

void peach_walk(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mp->flags &= 0xFFF0FFFF;
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
    if ((s8)mp->wStickDir1 * (s8)mp->wStickDir1 + (s8)mp->wStickDir2 * (s8)mp->wStickDir2 > 3025) {
        marioChgMot(MARIO_MOTION_DASH);
    } else {
        mp->directionView = mp->controlStickAngle;
        peachStepSe(40, 20);
        if (!mp->controlStickSensitivity) {
            marioChgMot(MARIO_MOTION_STAY);
        }
    }
}

void peach_dash(void) {
    MarioWork* mp = marioGetPtr();
    if (mp->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mp->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mp->flags &= 0xFFF0FFFF;
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
    } else if ((s8)mp->wStickDir1 * (s8)mp->wStickDir1 + (s8)mp->wStickDir2 * (s8)mp->wStickDir2 <= 3025) {
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
