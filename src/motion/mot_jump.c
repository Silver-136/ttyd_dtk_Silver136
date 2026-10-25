#include "sfx_name_word.h"
#include "mario/mario_hit.h"
#include "koopa_motion.h"
#include "koopa.h"
#include "mario/mario_sbr.h"
#include "system.h"
#include "mot/mot_jump.h"
#ifdef VERSION_G8ME01
#include "mario/mariost.h"
#include <string.h>
BOOL marioPriCheckJabara(void);
#endif
#include "dolphin/types.h"
#include "driver/hitdrv.h"
#include "mario/mario.h"
#include "mario/mario_cam.h"
#include "mario/mario_motion.h"
#include "mot/mot_slit.h"
#include "pmario_sound.h"
#include <string.h>
#include "driver/mobjdrv.h"

#ifdef VERSION_G8ME01
#define kpa_fall_80213C38 kpa_fall
#endif
void marioChgMotAuto(void);
void marioSlitForceCancel(void);
void* marioChkFront(Vec* out, Vec* pos, f32 dir, f32 dist);


static f32 marioJumpStandData[] = {1.52f, -0.188f, 2.925f, -0.847f, -0.1f, -0.4f, 0.024f, -0.004f, 0.125f, 0.7f};
f32 marioJumpSwData[] = {2.5f, -0.1f, 0.02f, -0.01f, -0.08f, -0.4f, 0.0f, -0.002f};

static inline void chgPoseB(MarioWork* mario) {
    if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && mario->characterId == CHARACTER_MARIO) {
        marioChgPose("M_J_1B");
    }
}
static inline void chgPoseC(MarioWork* mario) {
    if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && mario->characterId == CHARACTER_MARIO) {
        marioChgPose("M_J_1C");
    }
}

void marioPreJump(void) {
    MarioWork* mario = marioGetPtr();
    mario->flags &= ~MARIO_FLAG_IS_FALLING;
    mario->flags |= MARIO_FLAG_IS_JUMPING;
    mario->airTimer = 0;
    mario->jumpPeakAirTime = 0;
    mario->hitobjJumpFrom = mario->hitobjStandOn;
    clear_hitobj_ride();
    marioSetJumpPara();
    mario->lastGroundPos = mario->position;
#ifdef VERSION_G8ME01
    *&mario->jumpStartSpeed = mario->baseSpeed;
#endif
}

f32 marioGetJumpHeight(void) {
    MarioWork* mario = marioGetPtr();
    return mario->mot.jump.standHeight;
}

void mot_jumpStand(void) {
    MarioWork* mario = marioGetPtr();
    f32 prev;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->baseSpeed = 0.0f;
        marioPreJump();
        mario->mot.jump.dist = 0.0f;
        mario->currSubMotionId = 0;
        mario->multiTimer = 0;
    }
    switch (mario->currSubMotionId) {
    case 0:
        if (++mario->multiTimer >= 3) {
            mario->currSubMotionId = 4;
            mario->jumpVelocityY = marioJumpStandData[0];
            mario->jumpAccelerationY = marioJumpStandData[1];
            mario->jumpJerkY = marioJumpStandData[2];
            mario->jumpSnapY = marioJumpStandData[3];
            chgPoseB(mario);
        }
        break;
    case 4:
        prev = mario->jumpVelocityY;
        marioMakeJumpPara();
        if (prev >= mario->jumpVelocityY) {
            mario->currSubMotionId = 6;
        }
        mario->position.y += mario->jumpVelocityY;
        mario->mot.jump.dist += mario->jumpVelocityY;
        break;
    case 6:
        mario->position.y += mario->jumpVelocityY;
        mario->mot.jump.dist += mario->jumpVelocityY;
        if (mario->mot.jump.dist >= mario->mot.jump.standHeight * marioJumpStandData[9]) {
            mario->currSubMotionId = 8;
        }
        if (mario->jumpVelocityY <= 0.0f) {
            mario->currSubMotionId = 8;
        }
        break;
    case 8:
        mario->jumpVelocityY += (-1.0f - mario->jumpVelocityY) * marioJumpStandData[8];
        mario->position.y += mario->jumpVelocityY;
        mario->mot.jump.dist += mario->jumpVelocityY;
        if (mario->jumpVelocityY <= 0.0f) {
            mario->currSubMotionId = 10;
            chgPoseC(mario);
            marioSetPeakYpos();
            mario->jumpVelocityY = marioJumpStandData[4];
            mario->jumpAccelerationY = marioJumpStandData[5];
            mario->jumpJerkY = marioJumpStandData[6];
            mario->jumpSnapY = marioJumpStandData[7];
            mario->flags &= ~MARIO_FLAG_IS_JUMPING;
            mario->flags |= MARIO_FLAG_IS_FALLING;
            mario->currSubMotionId = 20;
        }
        break;
    case 10:
        marioMakeJumpPara();
        if (mario->jumpVelocityY <= 0.0f) {
            chgPoseC(mario);
            marioSetPeakYpos();
            mario->jumpVelocityY = marioJumpStandData[4];
            mario->jumpAccelerationY = marioJumpStandData[5];
            mario->jumpJerkY = marioJumpStandData[6];
            mario->jumpSnapY = marioJumpStandData[7];
            mario->flags &= ~MARIO_FLAG_IS_JUMPING;
            mario->flags |= MARIO_FLAG_IS_FALLING;
            mario->currSubMotionId = 20;
        }
        mario->position.y += mario->jumpVelocityY;
        break;
    case 20:
        if (--mario->multiTimer <= 0) {
            mario->baseSpeed = 0.0f;
            mario->currSubMotionId = 30;
        }
        break;
    }
    mario->airTimer++;
    if (marioSlitButton() != 2) {
        marioSlitForceCancel();
    }
}

void mot_jumpSw(void) {
    MarioWork* mario = marioGetPtr();
    void* mobj;
    f32 dir;
    f32 x, z;
    f32 s, c;
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        marioResetCamFollowRate();
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        camFollowYOff();
        marioPreJump();
        chgPoseB(mario);
        if (mario->hitobjJumpFrom != NULL) {
            if (hitGetAttr((HitEntry*)mario->hitobjJumpFrom) & 0x80000000) {
                mobj = mobjHitObjPtrToPtr(mario->hitobjJumpFrom);
                mario->mot.jump.standSpeed = ((MapObjectEntry*)mobj)->position.x;
                mario->mot.jump.standHeight = ((MapObjectEntry*)mobj)->position.z;
                {
                Vec v;
                Vec t = {0.0f, 0.0f, 0.0f};
                f32 out;
                t.x = ((MapObjectEntry*)mobj)->position.x;
                t.y = 0.5f * mario->wPlayerCollisionBox.y + ((MapObjectEntry*)mobj)->position.y;
                t.z = ((MapObjectEntry*)mobj)->position.z;
                v = t;
                if (marioChkFront((Vec*)&out, &v, mario->directionView, 5.0f * mario->wPlayerCollisionBox.x) != NULL && out >= 0.0f &&
                    out <= 0.5f * mario->wPlayerCollisionBox.x + 30.0f) {
                    mario->moveDirection = mario->directionView = revise360(180.0f + mario->directionView);
                }
                }
                {
                sincosf(&s, &c, mario->directionView);
                x = 30.0f * s + ((MapObjectEntry*)mobj)->position.x;
                z = 30.0f * c + ((MapObjectEntry*)mobj)->position.z;
                mario->multiTimer = 20;
                mario->baseSpeed = distABf(mario->position.x, mario->position.z, x, z) / (f32)(s32)mario->multiTimer;
                }
            }
        } else {
            dir = revise360(180.0f + mario->directionView);
            sincosf(&s, &c, dir);
            x = 30.0f * s + mario->position.x;
            z = 30.0f * c + mario->position.z;
            mario->directionView = dir;
            mario->moveDirection = dir;
            mario->multiTimer = 20;
            mario->baseSpeed = distABf(mario->position.x, mario->position.z, x, z) / (f32)(s32)mario->multiTimer;
        }
        mario->currSubMotionId = 0;
    }
    switch ((s32)mario->currSubMotionId) {
    case 0:
        mario->multiTimer = (s32)mario->multiTimer - 1;
        marioMakeJumpPara();
        if (mario->jumpVelocityY <= 0.0f) {
            chgPoseC(mario);
            marioSetPeakYpos();
            mario->jumpVelocityY = marioJumpSwData[4];
            mario->jumpAccelerationY = marioJumpSwData[5];
            mario->jumpJerkY = marioJumpSwData[6];
            mario->jumpSnapY = marioJumpSwData[7];
            mario->flags &= ~MARIO_FLAG_IS_JUMPING;
            mario->flags |= MARIO_FLAG_IS_FALLING;
            mario->currSubMotionId = 10;
        }
        mario->position.y += mario->jumpVelocityY;
        break;
    case 10:
        if (--mario->multiTimer <= 0) {
            mario->baseSpeed = 0.0f;
            mario->currSubMotionId = 20;
        }
        break;
    }
    mario->airTimer++;
}

void mot_jumpNpc(void) {
    MarioWork* mario = marioGetPtr();
    f32 s, c;
    void* mobj;
    f32 dir;
    f32 x, z;
    if (mario->characterId == CHARACTER_BOWSER) {
        kpa_jumpNpc();
        return;
    }
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        marioResetCamFollowRate();
        if (mario->characterId != CHARACTER_BOWSER) {
            mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        }
        camFollowYOff();
        marioPreJump();
        if (mario->characterId == CHARACTER_BOWSER) {
            mario->jumpVelocityY = 2.6f;
            mario->jumpAccelerationY = -0.06f;
            mario->jumpJerkY = -0.01f;
            mario->jumpSnapY = -0.002f;
        } else {
            mario->jumpVelocityY = 1.0f;
            mario->jumpAccelerationY = -0.04f;
            mario->jumpJerkY = -0.01f;
            mario->jumpSnapY = -0.02f;
        }
        chgPoseB(mario);
        if (mario->hitobjJumpFrom != NULL) {
            if (hitGetAttr((HitEntry*)mario->hitobjJumpFrom) & 0x80000000) {
                mobj = mobjHitObjPtrToPtr(mario->hitobjJumpFrom);
                mario->mot.jump.standSpeed = ((MapObjectEntry*)mobj)->position.x;
                mario->mot.jump.standHeight = ((MapObjectEntry*)mobj)->position.z;
                {
                Vec v;
                Vec t = {0.0f, 0.0f, 0.0f};
                Vec out;
                t.x = ((MapObjectEntry*)mobj)->position.x;
                t.y = 0.5f * mario->wPlayerCollisionBox.y + ((MapObjectEntry*)mobj)->position.y;
                t.z = ((MapObjectEntry*)mobj)->position.z;
                v = t;
                if (marioChkFront(&out, &v, mario->directionView, 5.0f * mario->wPlayerCollisionBox.x) != NULL && out.x >= 0.0f &&
                    out.x <= 0.5f * mario->wPlayerCollisionBox.x + 30.0f) {
                    mario->moveDirection = mario->directionView = revise360(180.0f + mario->directionView);
                }
                
                
                
                sincosf(&out.z, &out.y, mario->directionView);
                x = 30.0f * out.z + ((MapObjectEntry*)mobj)->position.x;
                z = 30.0f * out.y + ((MapObjectEntry*)mobj)->position.z;
                mario->multiTimer = 20;
                mario->baseSpeed = distABf(mario->position.x, mario->position.z, x, z) / (f32)(s32)mario->multiTimer;
                }
            }
        } else {
            revise360(180.0f + mario->directionView);
            dir = mario->directionView;
            sincosf(&s, &c, dir);
            x = 30.0f * s + mario->position.x;
            z = 30.0f * c + mario->position.z;
            mario->directionView = dir;
            mario->moveDirection = dir;
            mario->multiTimer = 20;
            mario->baseSpeed = distABf(mario->position.x, mario->position.z, x, z) / (f32)(s32)mario->multiTimer;
        }
        mario->currSubMotionId = 0;
    }
    switch ((s32)mario->currSubMotionId) {
    case 0:
        mario->multiTimer = (s32)mario->multiTimer - 1;
        marioMakeJumpPara();
        if (mario->jumpVelocityY <= 0.0f) {
            chgPoseC(mario);
            marioSetPeakYpos();
            mario->jumpVelocityY = -0.03f;
            mario->jumpAccelerationY = -0.02f;
            mario->jumpJerkY = 0.04f;
            mario->jumpSnapY = 0.0f;
            mario->flags &= ~MARIO_FLAG_IS_JUMPING;
            mario->flags |= MARIO_FLAG_IS_FALLING;
            mario->currSubMotionId = 20;
        }
        mario->position.y += mario->jumpVelocityY;
        break;
    case 20:
        if (--mario->multiTimer <= 0) {
            mario->baseSpeed = 0.0f;
            mario->currSubMotionId = 30;
        }
        break;
    }
    mario->airTimer++;
}

static inline void jumpSfx(MarioWork* mario) {
    s32 r = irand(10000);
    s32 id;
    if (r < 2000) {
        id = 0x9D;
    } else if (r < 4000) {
        id = 0x9E;
    } else if (r < 6000) {
        id = 0x9F;
    } else if (r < 8000) {
        id = 0xA0;
    } else {
        id = 0xA1;
    }
    psndSFXOn_3D(SFX_NAME_WORD(id), &mario->position);
}

#ifdef VERSION_G8ME01
void N_mot_jumpNpc_post(void) {
    MarioWork* mario = marioGetPtr();
    if (!strcmp(gp->currentMapName, "gon_10") && (mario->dispFlags & 0x20000)) {
        camFollowYOn();
    }
}
#endif
void mot_jump(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->characterId == CHARACTER_BOWSER) {
        kpa_jump();
        return;
    }
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        marioResetCamFollowRate();
        mario->flags |= MARIO_FLAG_HAS_INPUT_JUMP;
        mario->multiTimer = 2;
        mario->currSubMotionId = 0;
        marioPreJump();
        if (mario->currMotionId == MARIO_MOTION_JUMP_3) {
            camFollowYOn();
        } else {
            camFollowYOff();
        }
        chgPoseB(mario);
        if (mario->currMotionId != MARIO_MOTION_JUMP_3) {
            jumpSfx(mario);
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_JUMP1), &mario->position);
        if (mario->airTimer == 0) {
            mario->fallStartPos = mario->position;
        }
        mario->trigFlags &= ~MARIO_TRIG_FLAG_SET_JUMP_PARAMS;
    }
    switch ((s32)mario->currSubMotionId) {
    case 0:
        if (--mario->multiTimer <= 0) {
            chgPoseB(mario);
            mario->currSubMotionId = 10;
        }
        break;
    }
    mario->airTimer++;
}

void mot_jumpSmall(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->characterId == CHARACTER_BOWSER) {
        kpa_jump();
        return;
    }
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        marioResetCamFollowRate();
        mario->flags |= MARIO_FLAG_HAS_INPUT_JUMP;
        mario->multiTimer = 2;
        mario->currSubMotionId = 0;
        marioPreJump();
        if (mario->currMotionId != MARIO_MOTION_JUMP_3) {
            camFollowYOff();
        }
        chgPoseB(mario);
        if (mario->currMotionId != MARIO_MOTION_JUMP_3) {
            jumpSfx(mario);
        }
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_MARIO_JUMP1), &mario->position);
        if (mario->airTimer == 0) {
            mario->fallStartPos = mario->position;
        }
        mario->trigFlags &= ~MARIO_TRIG_FLAG_SET_JUMP_PARAMS;
    }
    switch ((s32)mario->currSubMotionId) {
    case 0:
        if (--mario->multiTimer <= 0) {
            chgPoseB(mario);
            mario->currSubMotionId = 10;
        }
        break;
    }
    mario->airTimer++;
}

void mot_upstairs(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags |= MARIO_FLAG_IS_STEPPING;
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        marioClearJumpPara();
        mario->multiTimer = 3;
        mario->jumpVelocityY = (mario->stepUpY - mario->position.y) / (f32)(s32)mario->multiTimer;
        mario->currSubMotionId = 0;
    }
    switch ((s32)mario->currSubMotionId) {
    case 0:
        mario->position.y = mario->position.y + mario->jumpVelocityY;
        if (--mario->multiTimer <= 0) {
            mario->flags &= ~MARIO_FLAG_STOP_AIR_MOVEMENT;
            mario->flags &= ~MARIO_FLAG_USE_FORCE_DIRECTION;
            mario->flags &= ~MARIO_FLAG_IS_STEPPING;
            if (mario->flags & MARIO_FLAG_PAPER_MODE) {
                motSlitContinue();
            } else if (mario->dispFlags & 0x1000000) {
                marioChgMot2(0x15);
            } else {
                marioChgMotAuto();
            }
        }
        break;
    }
}

void mot_fall(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->characterId == CHARACTER_BOWSER) {
        kpa_fall_80213C38();
        return;
    }
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~MARIO_FLAG_IS_JUMPING;
        mario->flags |= MARIO_FLAG_IS_FALLING;
#ifdef VERSION_G8ME01
        mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_MOVE_CHK;
        mario->trigFlags &= ~MARIO_TRIG_FLAG_START_JABARA_SPIN;
#endif
        if (mario->soundId[2] != (u32)-1) {
            psndSFXOff(mario->soundId[2]);
            mario->soundId[2] = -1;
        }
        marioResetCamFollowRate();
        mario->currSubMotionId = 0;
        if (mario->currMotionId != MARIO_MOTION_FALL_2) {
            if (mario->characterId == CHARACTER_BOWSER) {
                kpaChgPose("KPA_J_1B", "KPA2_J_1B");
            } else if (!(mario->flags & MARIO_FLAG_IS_CARRYING_BOBBERY) && mario->characterId == CHARACTER_MARIO) {
                marioChgPose("M_J_1C");
            }
        } else {
            mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        }
        if (mario->airTimer == 0) {
            mario->fallStartPos = mario->position;
        }
    }
    switch ((s32)mario->currSubMotionId) {
    case 0:
        if (mario->position.y <= mario->lastGroundPos.y) {
            if (!marioBgmodeChk()) {
                camFollowYOn();
            }
            mario->currSubMotionId = 10;
        }
        break;
    }
    mario->airTimer++;
#ifdef VERSION_G8ME01
    mario->trigFlags &= ~MARIO_TRIG_FLAG_JABARA_CHK;
    if (marioPriCheckJabara() && mario->jumpStartSpeed == 0.0f && mario->jumpPeakAirTime != 0 && mario->jumpPeakAirTime < mario->airTimer) {
        if (mario->airTimer < 0x12) {
            if (mario->controlStickSensitivity) {
                mario->trigFlags |= MARIO_TRIG_FLAG_JABARA_MOVE_CHK;
            }
        } else {
            mario->trigFlags |= MARIO_TRIG_FLAG_JABARA_CHK;
            if (!(mario->trigFlags & MARIO_TRIG_FLAG_JABARA_MOVE_CHK) && mario->controlStickSensitivity) {
                mario->trigFlags |= MARIO_TRIG_FLAG_START_JABARA_SPIN;
            }
        }
    }
#endif
    if (mario->screenY >= 0x1AE) {
        f32 r = marioGetCamFollowRate();
        r += 0.1f;
        if (r >= 0.8f) {
            r = 0.8f;
        }
        marioSetCamFollowRate(r);
    }
}

void mot_fall_post(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->soundId[2] != (u32)-1) {
        psndSFXOff(mario->soundId[2]);
        mario->soundId[2] = -1;
    }
}

void mot_land(void) {
    MarioWork* mario = marioGetPtr();
    if (mario->trigFlags & MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION) {
        mario->trigFlags &= ~MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION;
        mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
        mario->airTimer = 0;
        mario->jumpPeakAirTime = 0;
        mario->currSubMotionId = 0;
        mario->baseSpeed = 0.0f;
        marioResetCamFollowRate();
        if (mario->soundId[2] != (u32)-1) {
            psndSFXOff(mario->soundId[2]);
            mario->soundId[2] = -1;
        }
    }
    switch ((s32)mario->currSubMotionId) {
    case 0:
        marioChgMot(MARIO_MOTION_STAY);
        break;
    case 1:
        if (--mario->multiTimer <= 0) {
            marioChgMot(MARIO_MOTION_STAY);
        }
        break;
    case 10:
        if (--mario->multiTimer <= 0) {
            marioChgMot(MARIO_MOTION_STAY);
        }
        break;
    }
}
