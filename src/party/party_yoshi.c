#include "dolphin/types.h"
#include "driver/animdrv.h"
#include "driver/effdrv.h"
#include "eff/n64/eff_sweat_n64.h"
#include "driver/hitdrv.h"
#include "mario/mario_sbr.h"
#include "mario/mario.h"
#include "mario/mario_motion.h"
#include "mario/mario_party.h"
#include "memory.h"
#include "party.h"
#include "system.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "party/party_hit.h"
#include "mario/mario_hit.h"
#include "mot/mot_damage.h"
#include "mot/mot_jump.h"
#include "mario/mario_cam.h"
#include <dolphin/pad.h>
#include "party/party_yoshi.h"
#include "pmario_sound.h"
#include "sfx_name_word.h"

typedef struct YoshiWork {
    u8 pad0[0x4];         // 0x0
    s32 dir;              // 0x4
    s32 count;            // 0x8
    f32 angle;            // 0xC
    s32 timer;            // 0x10
    EffectEntry* effect;  // 0x14
} YoshiWork;


extern f32 marioGetMoveRate(void);
extern HitEntry* dou10_yoko_yari2(void* out);
double sin(double);

f32 ofs_PYS_S_2[52] = {
    0.0f, -0.011f, -0.042f, -0.087f, -0.142f, -0.202f, -0.262f, -0.317f, -0.362f, -0.393f, -0.404f, -0.404f, -0.404f,
    -0.404f, -0.404f, -0.387f, -0.341f, -0.276f, -0.202f, -0.128f, -0.063f, -0.017f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, -0.011f, -0.042f, -0.087f, -0.142f, -0.202f, -0.262f, -0.317f, -0.362f, -0.393f, -0.404f, -0.404f, -0.404f,
    -0.404f, -0.404f, -0.387f, -0.341f, -0.276f, -0.202f, -0.128f, -0.063f, -0.017f, 0.0f, 0.0f, 0.0f, 0.0f,
};
static f32 marioJumpStandData[10] = {1.52f, -0.188f, 2.925f, -0.847f, -0.1f, -0.4f, 0.024f, -0.004f, 0.125f, 0.7f};
f32 ofs_PYS_R_2[10] = {0.0f, 0.0f, 0.366f, 0.555f, 0.38f, 0.0f, 0.0f, 0.366f, 0.555f, 0.38f};
f32 ofs_PYS_F_1[10] = {0.0f, 0.0f, 0.433f, 0.557f, 0.512f, 0.0f, 0.0f, 0.433f, 0.557f, 0.512f};

#define YW(party) ((YoshiWork*)(party)->useStruct)
#define ADD_OFS(party)                                              \
    if ((party)->hitobjStandOn) {                                       \
        (party)->position.x += (party)->floorMove.x;                  \
        (party)->position.y += (party)->floorMove.y;                  \
        (party)->position.z += (party)->floorMove.z;                  \
    }
#define SYNC_MARIO(mario, party)                                    \
    (mario)->camTarget.x = (party)->position.x;                       \
    (mario)->camTarget.z = (party)->position.z;                       \
    (mario)->position.x = (party)->position.x;                       \
    (mario)->position.z = (party)->position.z;                       \
    (mario)->displayDirection = (party)->displayDirection;                       \
    (mario)->displayDirectionTarget = (party)->displayDirectionTarget;
void yoshi_damageToge(PartyEntry* party);


f32 evt_yoshi_spd;
s32 evt_yoshi_frm;
f32 jumpheight;

void inertia(PartyEntry* party);
void searchGround(PartyEntry* party);
void searchGround2(PartyEntry* party);
void checkWall(PartyEntry* party, f32 spd, f32 dir);
void L_getStick(PartyEntry* party, f32* angle, f32* speed);
void L_yoshiFlyMove(PartyEntry* party);

void evt_set_yoshi_frm(s32 msec) {
    evt_yoshi_frm = sysMsec2Frame(msec);
}

void evt_set_yoshi_spd(f32 spd) {
    evt_yoshi_spd = spd;
}

s32 evt_get_yoshi_frm(void) {
    return evt_yoshi_frm;
}

void L_sweatEntry(PartyEntry* party, void* hit) {
    party->hitobjStandOn = hit;
    if (yoshiGetStatus() == 1 && !(party->playerPtr->flags & 0xF0000)) {
        party->playerPtr->hitobjStandOn = hit;
    }
}

void L_sweatMain(PartyEntry* party) {
    if (party->useStruct && ((YoshiWork*)party->useStruct)->effect) {
        f32 x = party->position.x;
        f32 y = party->position.y + 18.0f;
        f32 z = party->position.z;
        ((EffSweatN64Work*)((YoshiWork*)party->useStruct)->effect->userdata)->position.x = x;
        ((EffSweatN64Work*)((YoshiWork*)party->useStruct)->effect->userdata)->position.y = y;
        ((EffSweatN64Work*)((YoshiWork*)party->useStruct)->effect->userdata)->position.z = z;
        if (--((YoshiWork*)party->useStruct)->timer <= 0) {
            effDelete(((YoshiWork*)party->useStruct)->effect);
            ((YoshiWork*)party->useStruct)->effect = NULL;
        }
    }
}

s32 yoshiGetStatus(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    s32 status = 5;
    u8 motion;

    if (party == NULL || party->currentMemberId != PARTY_MEMBER_YOSHI || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return 0;
    }
    motion = party->useMotionId;
    if (motion >= 10 && motion <= 13) {
        status = 1;
    } else if (motion >= 20 && motion <= 22) {
        status = 2;
    } else if (motion >= 100 && motion <= 101) {
        status = 5;
    } else if (motion >= 60 && motion <= 65) {
        status = 3;
    } else if (motion >= 50 && motion <= 52) {
        status = 4;
    }
    return status;
}

void marioRideYoshi(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    PartyEntry* extra;

    party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
    party->trigFlags &= ~PARTY_TRIG_CHG_MOVE_MODE;
    party->trigFlags &= ~PARTY_TRIG_CHG_MOT;
    party->flags |= PARTY_FLAG_IS_BEING_USED;
    party->runMode = 3;
    party->useMotionId = 11;
    if (party->useStruct == NULL) {
        party->useStruct = __memAlloc(HEAP_DEFAULT, sizeof(YoshiWork));
        memset(party->useStruct, 0, sizeof(YoshiWork));
        ((YoshiWork*)party->useStruct)->effect = NULL;
    }
    extra = partyGetPtr(marioGetExtraPartyId());
    if (extra) {
        party_force_ride_yoshi(extra);
    }
}

s32 marioGetoffYoshi(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (!(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return 0;
    }
    if (yoshiGetStatus() == 1) {
        party->useMotionId = 100;
        return 1;
    }
    return 0;
}

static inline void L_resetOfs(PartyEntry* party) {
    PartyEntry* extra;

    MarioWork* player = party->playerPtr;

    player->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    player->dispOfs2 = (Vec){0.0f, 0.0f, 0.0f};
    extra = partyGetPtr(marioGetExtraPartyId());
    if (extra) {
        extra->dispOfs = (Vec){0.0f, 0.0f, 0.0f};
    }
}

static inline u32 L_chgMotion(PartyEntry* party, u8 motion) {
    if (party->useMotionId == 5) {
        return FALSE;
    }
    party->useMotionId = motion;
    return TRUE;
}

static inline void L_fall(PartyEntry* party) {
    if (!(party->flags & PARTY_FLAG_IS_FALLING)) {
        party->flags |= PARTY_FLAG_IS_FALLING;
        party->gravity = -0.4f;
        party->work.common.accelY = -0.4f;
        party->work.yoshi.gravityAccel = 0.0f;
        party->accelY = 0.0f;
    }
    party->work.yoshi.gravityAccel += party->accelY;
    party->work.common.accelY += party->work.yoshi.gravityAccel;
    party->gravity += party->work.common.accelY;
    if (party->gravity < -15.0f) {
        party->gravity = -15.0f;
    }
}

void marioYoshiForceCancel(void) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    PartyEntry* extra;

    if (yoshiGetStatus() == 0) {
        return;
    }
    L_resetOfs(party);
    marioChgPose("M_S_1");
    if (party->flags & PARTY_FLAG_IS_BEING_USED) {
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        if (party->useStruct) {
            __memFree(HEAP_DEFAULT, party->useStruct);
            party->useStruct = NULL;
        }
        if (mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE) {
            partyChgRunMode(party, 0xd);
            extra = partyGetPtr(marioGetExtraPartyId());
            if (extra) {
                partyChgRunMode(extra, 0xd);
            }
            return;
        }
    }
    if (party->runMode != 8 && party->runMode != 0xd) {
        partyChgRunMode(party, 2);
        party->moveMode = 0;
    }
}

void yoshi_bye(PartyEntry* party) {
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        partyGoodbyeInit(party);
    }
    if (party->flags & PARTY_FLAG_IS_BEING_USED) {
        if (yoshiGetStatus() == 1) {
            party->useMotionId = 100;
        }
        yoshi_use(party);
    } else if (partyGoodbyeMain(party)) {
        partyChgRunMode(party, 0xe);
    }
}

void yoshi_exit(PartyEntry* party) {
    party->playerPtr->flags &= ~0x10000000;
    marioYoshiForceCancel();
    marioChgMot(MARIO_MOTION_STAY);
}

void yoshi_jumpStand(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());

    if (party == NULL || party->currentMemberId != PARTY_MEMBER_YOSHI || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return;
    }
    if (!(party->useMotionId >= 60 && party->useMotionId <= 66)) {
            party->useMotionId = 60;
        }
}

void yoshi_damageToge(PartyEntry* party) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* extra;

    L_sweatEntry(party, NULL);
    marioChgMot(MARIO_MOTION_DAMAGE_TOGE);
    party->useMotionId = 10;
    partyChgPose(party, "PYS_S_2");
    marioChgPose("M_Y_2");
    mario->mot.yoshi.velX = 0.0f;
    mario->mot.yoshi.velZ = 0.0f;
    partyChgRunMode(party, 0xd);
    extra = partyGetPtr(marioGetExtraPartyId());
    if (extra) {
        partyChgRunMode(extra, 0xd);
    }
}

void yoshiAnimeCtrl(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;

    if (!strcmp(party->animName, "PYS_S_2")) {
        if (++mario->poseTime >= 52) {
            mario->poseTime = 0;
        }
        party->poseTime = mario->poseTime;
        animPoseSetLocalTime(party->paperAnimGroup, mario->poseTime);
    } else if (!strcmp(party->animName, "PYS_R_2")) {
        if (++mario->poseTime >= 10) {
            mario->poseTime = 0;
        }
        party->poseTime = mario->poseTime;
        animPoseSetLocalTime(party->paperAnimGroup, mario->poseTime);
    } else if (!strcmp(party->animName, "PYS_F_1")) {
        if (++mario->poseTime >= 10) {
            mario->poseTime = 0;
        }
        party->poseTime = mario->poseTime;
        animPoseSetLocalTime(party->paperAnimGroup, mario->poseTime);
    }
}

void yoshi_init(PartyEntry* party) {
    party->memberKind = 4;
    party->moveKind = 0;
    party->height = 26.0f;
    party->width = 20.0f;
    party->yoshiPsndSFXId = -1;
}

static void L_searchHead(PartyEntry* party);
static inline f32 L_getOfs(PartyEntry* party) {
    MarioWork* p = party->playerPtr;
    f32 ofs = 0.0f;

    if (!strcmp(party->animName, "PYS_S_2")) {
        ofs += 2.0f * ofs_PYS_S_2[p->poseTime];
    } else if (!strcmp(party->animName, "PYS_R_2")) {
        ofs += 2.0f * ofs_PYS_R_2[p->poseTime];
    } else if (!strcmp(party->animName, "PYS_F_1")) {
        ofs += 2.0f * ofs_PYS_F_1[p->poseTime];
    }
    return ofs;
}

void yoshi_use(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    PartyEntry* extra;
    f32 dir;
    f32 spd;
    HitEntry* hit;
    s32 attr;
    f32 angle;
    f32 speed;
    f32 ox;
    f32 oz;

    if (!mario->keyOffCount) {
        partyUpdateKeyData(party);
    } else {
        L_partyClearCont(party);
    }
    if (party->trigFlags & PARTY_TRIG_CHG_RUN_MODE) {
        party->trigFlags &= ~PARTY_TRIG_CHG_RUN_MODE;
        party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
        party->flags &= ~(PARTY_FLAG_DISABLE_FRONT_WALL_CHK | PARTY_FLAG_FORCE_FALL);
        party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
        party->useStruct = __memAlloc(HEAP_DEFAULT, sizeof(YoshiWork));
        memset(party->useStruct, 0, sizeof(YoshiWork));
        YW(party)->effect = NULL;
        if (party->flags & PARTY_FLAG_IS_EVT_USE) {
            evt_yoshi_spd = 1.0f;
        } else {
            evt_yoshi_frm = -1;
            evt_yoshi_spd = 0.0f;
        }
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
            party->useMotionId = 0;
        } else {
            party->useMotionId = 4;
        }
        party->moveFrames = 3;
        mario->baseSpeed = 0.0f;
        mario->mot.yoshi.bobOfs = 0.0f;
        mario->mot.yoshi.bobSpeed = 0.0f;
        mario->mot.yoshi.velX = 0.0f;
        mario->mot.yoshi.velZ = 0.0f;
        YW(party)->dir = 0;
    }

    switch (party->useMotionId) {
        case 0:
            if (--party->moveFrames > 0) {
                if (!marioKeyOffChk() && mario->hitobjStandOn) {
                    return;
                }
                if (party->flags & PARTY_FLAG_IS_BEING_USED) {
                    party->flags &= ~PARTY_FLAG_IS_BEING_USED;
                    if (party->useStruct) {
                        __memFree(HEAP_DEFAULT, party->useStruct);
                        party->useStruct = NULL;
                    }
                }
                if (party->runMode != 8 && party->runMode != 0xd) {
                    partyChgRunMode(party, 2);
                    party->moveMode = 0;
                }
                return;
            }
            party->useMotionId = 4;
        case 4:
            mario->directionView = mario->directionView <= 180.0f ? 90.0f : 270.0f;
            mario->moveDirection = mario->directionView;
            party->moveTarget = mario->position;
            party->moveFrames = 4;
            party->velY = 0.0f;
            party->gravity = 0.0f;
            party->work.common.accelY = 0.0f;
            party->work.yoshi.gravityAccel = 0.0f;
            party->accelY = 0.0f;
            party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->moveSpeed = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z) / party->moveFrames;
            party->useMotionId = 5;
        case 5:
            party->moveTarget = mario->position;
            party->position.x += (party->moveTarget.x - party->position.x) / party->moveFrames;
            party->position.y += (party->moveTarget.y - party->position.y) / party->moveFrames;
            party->position.z += (party->moveTarget.z - party->position.z) / party->moveFrames;
            if (--party->moveFrames != 0) {
                searchGround(party);
                ADD_OFS(party);
                return;
            }
            mario->trigFlags |= MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI;
            marioChgSmallJumpMotion();
            mario->baseSpeed = 0.0f;
            party->moveDir = mario->moveDirection;
            party->targetDir = mario->directionView;
            party->displayDirectionTarget = mario->displayDirectionTarget;
            party->displayDirection = mario->displayDirection;
            party->moveSpeed = 0.0f;
            party->useMotionId = 7;
            ADD_OFS(party);
            partyChkGnd(party);
            return;
        case 7:
            if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                if (marioKeyOffChk()) {
                    party->useMotionId = 101;
                    break;
                }
                if (!(party->flags & PARTY_FLAG_IS_GROUNDED)) {
                    party->useMotionId = 101;
                    break;
                }
            }
            if (mario->hitobjStandOn == NULL) {
                ADD_OFS(party);
                partyChkGnd(party);
                return;
            }
            party->useMotionId = 10;
            extra = partyGetPtr(marioGetExtraPartyId());
            if (extra) {
                partyChgRunMode(extra, 0xc);
            }
            partyChkGnd(party);
            break;
    }

    switch (party->useMotionId) {
        case 10:
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_RIDE1), &party->position);
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
        case 11:
            marioChgMot(MARIO_MOTION_YOSHI);
            partyChgPose(party, "PYS_H_1");
            marioChgPose("M_Y_1");
            party->moveSpeed = 0.0f;
            party->moveDistance = 0.0f;
            party->position.x = mario->position.x;
            party->position.y = mario->position.y;
            party->position.z = mario->position.z;
            mario->camTarget.x = party->position.x;
            mario->camTarget.z = party->position.z;
            party->useMotionId = 12;
            mario->lastGroundPos = party->position;
            break;
        case 12:
            party->playerPtr->poseTime = 0;
            party->useMotionId = 13;
        case 13:
            if (mario->flags & MARIO_FLAG_FORCED_MOVEMENT) {
                marioChgPose("M_Y_2");
                partyChgPose(party, "PYS_R_2");
                party->hitobjFront = NULL;
                party->hitobjPush = NULL;
                party->hitobjHead = NULL;
                mario->hitobjFront = NULL;
                mario->hitobjPush = NULL;
                mario->hitobjWall = NULL;
                mario->hitobjHead = NULL;
                dir = mario->directionView;
                spd = mario->baseSpeed;
                L_searchHead(party);
                checkWall(party, spd, dir);
                SYNC_MARIO(mario, party);
                party_ride_yoshi_force_move();
                mario->lastGroundPos = party->position;
                searchGround(party);
                if (--mario->forceMoveTimer <= 0) {
                    mario->forceMoveTimer = 0;
                    mario->flags &= ~MARIO_FLAG_FORCED_MOVEMENT;
                }
                YW(party)->count++;
                if (YW(party)->count % 10 == 0) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_MOVE1), &party->position);
                }
                break;
            }
            if (party->runMode == 8) {
                break;
            }
            if (yoshiGetStatus() == 1) {
                party->floorMoveX = mario->floorMove.x;
                party->floorMoveY = mario->floorMove.y;
                party->floorMoveZ = mario->floorMove.z;
            }
            if (party->flags & PARTY_FLAG_IS_GROUNDED) {
                if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                    if (party->buttonTrg & PAD_BUTTON_A) {
                        party->useMotionId = 20;
                        break;
                    }
                } else if (evt_yoshi_frm >= 0) {
                    party->useMotionId = 20;
                    break;
                }
            }
            if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                angle = mario->directionView;
                speed = evt_yoshi_spd;
            } else {
                L_getStick(party, &angle, &speed);
            }
            if (speed > 0.0f) {
                party->moveSpeed = speed;
                party->targetDir = angle;
                party->moveDir = angle;
                marioChgPose("M_Y_2");
                partyChgPose(party, "PYS_R_2");
                YW(party)->count++;
                if (YW(party)->count % 10 == 0) {
                    psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_MOVE1), &party->position);
                }
            } else {
                if (strcmp(party->animName, "PYS_S_2")) {
                    mario->poseTime = 0;
                }
                partyChgPose(party, "PYS_S_2");
                party->moveSpeed = 0.0f;
                YW(party)->count = 0;
            }
            inertia(party);
            party->position.x = mario->position.x;
            party->position.z = mario->position.z;
            party->hitobjFront = NULL;
            party->hitobjPush = NULL;
            mario->hitobjFront = NULL;
            mario->hitobjPush = NULL;
            mario->hitobjWall = NULL;
            dir = party->targetDir;
            spd = party->moveSpeed;
            L_searchHead(party);
            checkWall(party, spd, dir);
            SYNC_MARIO(mario, party);
            if (party->buttonTrg & (PAD_BUTTON_B | PAD_BUTTON_X)) {
                party->useMotionId = 100;
            }
            searchGround(party);
            mario->moveDist = spd;
            break;
    }

    switch (party->useMotionId) {
        case 100:
            L_resetOfs(party);
            marioChgSmallJumpMotion();
            mario->baseSpeed = 0.0f;
            partyChgPose(party, "PYS_H_2");
            party->moveSpeed = 0.0f;
            party->useMotionId = 101;
            ADD_OFS(party);
            partyChkGnd(party);
            return;
        case 101:
            if (mario->hitobjStandOn == NULL) {
                ADD_OFS(party);
                partyChkGnd(party);
                return;
            }
            party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            if (party->runMode != 8) {
                partyChgRunMode(party, 2);
                party->moveMode = 0;
            }
            ADD_OFS(party);
            partyChkGnd(party);
#ifdef VERSION_G8ME01
            movePos(&party->position.x, &party->position.z, mario->hitobjFront ? -0.1f : 0.1f, toMovedir(mario->displayDirection));
#endif
            if (party->useStruct) {
                __memFree(HEAP_DEFAULT, party->useStruct);
                party->useStruct = NULL;
            }
            return;
    }

    switch (party->useMotionId) {
        case 40:
            party->flags |= PARTY_FLAG_IS_STEPPING;
            party->velY = 0.0f;
            party->accelY = 0.0f;
            party->gravity = 0.0f;
            party->moveFrames = 3;
            party->velY = (party->stepY - party->position.y) / party->moveFrames;
            party->useMotionId = 41;
        case 41:
            party->position.y += party->velY;
            if (--party->moveFrames <= 0) {
                party->flags &= ~PARTY_FLAG_IS_STEPPING;
                party->useMotionId = 13;
            }
            break;
    }

    switch (party->useMotionId) {
        case 20:
#ifdef VERSION_G8ME01
            YW(party)->dir = (party->displayDirectionTarget >= 90.0f && party->displayDirectionTarget <= 270.0f) ? 1 : -1;
#else
            YW(party)->dir = (mario->displayDirection >= 90.0f && mario->displayDirection <= 270.0f) ? 1 : -1;
#endif
            mario->hitobjJumpFrom = mario->hitobjStandOn;
            mario->hitobjStandOn = NULL;
            mario->lastGroundPos = party->position;
            partyChgPose(party, "PYS_F_1");
            marioChgPose("M_Y_3");
            mario->airTimer = 0;
            party->useMotionId = 21;
            party->moveSpeed += -0.01f;
            if (party->moveSpeed <= 0.0f) {
                party->moveSpeed = 0.0f;
            }
            YW(party)->angle = 1.0f;
            mario->mot.yoshi.bobSpeed = sin(3.141592653589793f * YW(party)->angle / 180.0f);
            mario->mot.yoshi.bobOfs = 0.0f;
            mario->mot.yoshi.baseY = party->position.y;
            party->yoshiPsndSFXId = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_YOSH_JUMP1), &party->position);
        case 21: {
                f32 s = sin(3.141592653589793f * YW(party)->angle / 180.0f);
                mario->mot.yoshi.bobSpeed = s * (YW(party)->angle <= 180.0f ? 0.5f : 0.37f);
            }
            mario->mot.yoshi.bobOfs += mario->mot.yoshi.bobSpeed;
            YW(party)->angle += 7.0f;
            if (YW(party)->angle >= 360.0f) {
                YW(party)->angle = 360.0f;
                party->useMotionId = 22;
            }
        case 22: {
                f32 y = 18.0f + party->position.y;
                f32 x = party->position.x;
                f32 z = party->position.z;
                if (YW(party)->effect == NULL) {
                    YW(party)->effect = effSweatN64Entry(1, x, y, z, 1.0f, 70.0f * (party->displayDirectionTarget == 180.0f ? -1.0f : 1.0f), 0x16);
                    YW(party)->timer = 20;
                }
            }
            hit = partySearchGround(party, 11.0f, 11.0f);
            if (hit == NULL) {
                party->position.y = mario->mot.yoshi.baseY + mario->mot.yoshi.bobOfs;
            } else {
                attr = hitGetAttr(hit);
                if (attr & 0x200) {
                    if (party->position.y <= party->groundY) {
                        party->useMotionId = 70;
                    }
                } else if (attr & 0xC00) {
                    if (party->position.y <= party->groundY) {
                        yoshi_damageToge(party);
                        return;
                    }
                }
                L_sweatEntry(party, NULL);
                mario->mot.yoshi.baseY = party->groundY;
                party->position.y = party->groundY + mario->mot.yoshi.bobOfs;
            }
            party->position.x = mario->position.x;
            party->position.z = mario->position.z;
            if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                f32 s = evt_yoshi_spd;
                angle = mario->directionView;
                speed = s;
                party->moveSpeed = s;
            } else {
                L_getStick(party, &angle, &speed);
                L_yoshiFlyMove(party);
            }
            party->hitobjFront = NULL;
            party->hitobjPush = NULL;
            dir = party->targetDir;
            spd = party->moveSpeed;
            L_searchHead(party);
            checkWall(party, spd, dir);
            if (party->hitobjStandOn == NULL && mario->airTimer >= 10) {
                searchGround2(party);
                if (party->hitobjStandOn) {
                    mario->hitobjJumpFrom = NULL;
                    if (party->yoshiPsndSFXId != -1) {
                        psndSFXOff(party->yoshiPsndSFXId);
                        party->yoshiPsndSFXId = -1;
                    }
                    party->useMotionId = 10;
                    partyChgPose(party, "PYS_S_2");
                    marioChgPose("M_Y_2");
                    mario->mot.yoshi.velX = 0.0f;
                    mario->mot.yoshi.velZ = 0.0f;
                    if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                        evt_yoshi_spd = 0.0f;
                        evt_yoshi_frm = -1;
                        party->moveSpeed = 0.0f;
                        party->useMotionId = 52;
                    }
                    break;
                }
                YW(party)->dir = 0;
            }
            SYNC_MARIO(mario, party);
            mario->airTimer++;
            if (mario->airTimer > 14) {
                if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                    if (mario->airTimer > 68) {
                        evt_yoshi_frm = 0;
                        party->velY = 0.0f;
                        party->gravity = -1.0f;
                        party->useMotionId = 50;
                    }
                } else if (mario->airTimer > 68 || !(party->buttonHold & PAD_BUTTON_A) || marioKeyOffChk()) {
                    party->velY = 0.0f;
                    party->gravity = -1.0f;
                    party->useMotionId = 50;
                }
            }
            break;
    }

    yoshiAnimeCtrl(party);
    if (party->useMotionId == 13 || yoshiGetStatus() == 3 || yoshiGetStatus() == 4 || yoshiGetStatus() == 2) {
        ox = 0.0f;
        oz = 0.0f;
        movePos(&ox, &oz, -6.0f, toMovedir(party->displayDirection));
        mario->dispOfs.x = ox;
        mario->dispOfs.z = oz;
        mario->dispOfs2.y = 8.0f + L_getOfs(party);
        extra = partyGetPtr(marioGetExtraPartyId());
        if (extra) {
            extra->dispOfs.y = L_getOfs(party);
        }
    }

    switch (party->useMotionId) {
        case 60:
            party->moveSpeed = 0.0f;
            party->flags &= ~PARTY_FLAG_IS_FALLING;
            party->flags |= PARTY_FLAG_IS_JUMPING;
            mario->airTimer = 0;
            mario->jumpPeakAirTime = 0;
            party->hitobjJumpFrom = party->hitobjStandOn;
            L_sweatEntry(party, NULL);
            mario->hitobjStandOn = NULL;
            mario->lastGroundPos = mario->position;
            jumpheight = 100.0f;
            mario->jumpDistance = 0.0f;
            party->useMotionId = 61;
            party->moveFrames = 0;
            party->gravity = marioJumpStandData[0];
            party->work.common.accelY = marioJumpStandData[1];
            party->work.yoshi.gravityAccel = marioJumpStandData[2];
            party->accelY = marioJumpStandData[3];
        case 61: {
                f32 prev = party->gravity;
                party->work.yoshi.gravityAccel += party->accelY;
                party->work.common.accelY += party->work.yoshi.gravityAccel;
                party->gravity += party->work.common.accelY;
                if (prev >= party->gravity) {
                    party->useMotionId = 62;
                }
            }
            party->position.y += party->gravity;
            mario->jumpDistance += party->gravity;
            break;
        case 62:
            party->position.y += party->gravity;
            mario->jumpDistance += party->gravity;
            if (mario->jumpDistance >= 0.7f * marioGetJumpHeight()) {
                party->useMotionId = 63;
            }
            break;
        case 63:
            party->gravity += 0.125f * (-1.0f - party->gravity);
            party->position.y += party->gravity;
            mario->jumpDistance += party->gravity;
            if (party->gravity <= 0.0f) {
                party->useMotionId = 64;
                marioSetPeakYpos();
                party->gravity = -0.4f;
                party->work.common.accelY = -0.4f;
                party->work.yoshi.gravityAccel = 0.0f;
                party->accelY = 0.0f;
                party->flags &= ~PARTY_FLAG_IS_JUMPING;
                party->flags |= PARTY_FLAG_IS_FALLING;
                party->useMotionId = 64;
            }
            break;
        case 64:
            party->work.yoshi.gravityAccel += party->accelY;
            party->work.common.accelY += party->work.yoshi.gravityAccel;
            party->gravity += party->work.common.accelY;
            if (party->gravity <= 0.0f) {
                marioSetPeakYpos();
                party->gravity = -0.4f;
                party->work.common.accelY = -0.4f;
                party->work.yoshi.gravityAccel = 0.0f;
                party->accelY = 0.0f;
                party->flags &= ~PARTY_FLAG_IS_JUMPING;
                party->flags |= PARTY_FLAG_IS_FALLING;
                party->useMotionId = 65;
            }
            party->position.y += party->gravity;
            break;
        case 65:
            if (--party->moveFrames <= 0) {
                party->moveSpeed = 0.0f;
                party->velY = 0.0f;
                party->gravity = -1.0f;
                party->useMotionId = 50;
            }
            break;
    }

    if (party->useMotionId >= 64 && party->useMotionId <= 65) {
        mario->airTimer++;
        searchGround(party);
        if (party->hitobjStandOn) {
            mario->hitobjJumpFrom = NULL;
            party->velY = 0.0f;
            party->gravity = -1.0f;
            party->useMotionId = 50;
        }
    }

    switch (party->useMotionId) {
        case 50:
        case 51:
            party->position.x = mario->position.x;
            party->position.z = mario->position.z;
            if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                f32 s = evt_yoshi_spd;
                angle = mario->directionView;
                speed = s;
                party->moveSpeed = s;
            } else {
                L_getStick(party, &angle, &speed);
                L_yoshiFlyMove(party);
            }
            if (!(party->buttonHold & PAD_BUTTON_A) || marioKeyOffChk()) {
                if (party->useMotionId == 50) {
                    party->useMotionId = 51;
                }
                {
                    MarioWork* m = marioGetPtr();
                    m->mot.yoshi.bobSpeed += m->mot.yoshi.bobOfs;
                }
            }
            if (mario->mot.yoshi.bobSpeed <= -10.0f) {
                mario->mot.yoshi.bobSpeed = -10.0f;
            }
            party->hitobjFront = NULL;
            party->hitobjPush = NULL;
            mario->hitobjFront = NULL;
            mario->hitobjPush = NULL;
            mario->hitobjWall = NULL;
            dir = party->targetDir;
            spd = party->moveSpeed;
            L_searchHead(party);
            checkWall(party, spd, dir);
            searchGround(party);
            if (party->hitobjStandOn) {
                mario->hitobjJumpFrom = NULL;
                party->useMotionId = 13;
                partyChgPose(party, "PYS_S_2");
                marioChgPose("M_Y_2");
                mario->mot.yoshi.velX = 0.0f;
                mario->mot.yoshi.velZ = 0.0f;
                YW(party)->dir = 0;
                if (party->flags & PARTY_FLAG_IS_EVT_USE) {
                    evt_yoshi_spd = 0.0f;
                    evt_yoshi_frm = -1;
                    party->moveSpeed = 0.0f;
                    party->useMotionId = 52;
                }
            }
            break;
        case 52:
            if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                party->useMotionId = 13;
            }
            break;
    }

    L_sweatMain(party);
    if (party->useMotionId == 70) {
        if (marioChkDamageSink()) {
            party->position = party->playerPtr->position;
            return;
        }
        party->useMotionId = 71;
        return;
    }
    if (party->useMotionId == 71) {
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        if (party->useStruct) {
            __memFree(HEAP_DEFAULT, party->useStruct);
            party->useStruct = NULL;
        }
        marioSetFallPara();
        marioChgMot(MARIO_MOTION_DAMAGE);
        partyChgRunMode(party, 0xd);
        extra = partyGetPtr(marioGetExtraPartyId());
        if (extra) {
            partyChgRunMode(extra, 0xd);
        }
        return;
    }
    if (party->useMotionId >= 10 && party->useMotionId <= 13 && mario->currMotionId != MARIO_MOTION_YOSHI) {
        party->flags &= ~PARTY_FLAG_IS_BEING_USED;
        if (party->useStruct) {
            __memFree(HEAP_DEFAULT, party->useStruct);
            party->useStruct = NULL;
        }
        partyChgRunMode(party, 2);
        party->moveMode = 0;
        L_resetOfs(party);
        return;
    }
    ADD_OFS(party);
    mario->camTarget.x = party->position.x;
    mario->camTarget.z = party->position.z;
    mario->displayDirection = party->displayDirection;
    mario->displayDirectionTarget = party->displayDirectionTarget;
    mario->position = party->position;
    marioUpdateCamCollision(party->position);
}

void L_yoshiFlyMove(PartyEntry* party) {
    MarioWork* mario = marioGetPtr();
    f32 z;
    f32 x;
    f32 spd;
    f32 max;
    f32 one = 1.0f;

    inertia(party);
    z = -mario->mot.yoshi.velZ;
    x = mario->mot.yoshi.velX;
    spd = 0.1f * distABf(0.0f, 0.0f, x, z);
    max = mario->mBaseDashSpeed * one;
    if (spd >= max) {
        spd = max;
    }
    z = angleABf(0.0f, 0.0f, x, z);
    if (spd < 0.01f) {
        spd = 0.0f;
    }
    party->moveSpeed = spd;
    if (spd) {
        party->targetDir = revise360(z + party->camDir);
        party->moveDir = revise360(z + party->camDir);
    }
    mario->moveDist = spd;
}

void inertia(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 sx = party->stickX;
    f32 vx = mario->mot.yoshi.velX;
    f32 vz = mario->mot.yoshi.velZ;
    f32 k;
    f32 lim = 16.0f;
    f32 acc = 0.01f;
    f32 zero = 0.0f;
    s32 dir;

    if (!sx) {
#ifdef VERSION_G8ME01
        if (yoshiGetStatus() != 2) {
            vz = vx = 0.0f;
        } else {
            k = -0.07f;
            if (mario->hitobjWall) {
                if (lim) {
                }
                k = -0.5f;
            }
            vx = vx * k + vx;
            vz = vz * k + vz;
            if (__fabsf(vx) < 0.01f) {
                vx = 0.0f;
            }
            if (__fabsf(vz) < 0.01f) {
                vz = 0.0f;
            }
        }
#else
        k = -0.07f;
        if (mario->hitobjWall) {
            if (lim) {
            }
            k = -0.5f;
        }
        vx = vx * k + vx;
        vz = vz * k + vz;
        if (__fabsf(vx) < 0.01f) {
            vx = 0.0f;
        }
        if (__fabsf(vz) < 0.01f) {
            vz = 0.0f;
        }
#endif
    } else if (vx * vx + vz * vz <= 256.0f) {
        dir = ((YoshiWork*)party->useStruct)->dir;
        vx += sx * acc;
        if (dir > 0 && vx <= 0.0f) {
            vx = 0.0f;
        }
        if (dir < 0 && vx >= 0.0f) {
            vx = 0.0f;
        }
        vz -= zero;
    } else {
        vz -= zero;
        vx += sx * acc;
        if (vx < -lim) {
            vx = -lim;
        }
        if (vx > lim) {
            vx = lim;
        }
        if (vz < -lim) {
            vz = -lim;
        }
        if (vz > lim) {
            vz = lim;
        }
    }
    mario->mot.yoshi.velX = vx;
    mario->mot.yoshi.velZ = vz;
}

void searchGround(PartyEntry* party) {
    HitEntry* hit;
    MarioWork* mario;
    s32 attr;
    f32 groundY;
    f32 range;
    Vec pos;
    f32 stepY;
    f32 stepA;
    f32 stepB;
    f32 diff;
    void* step;

    mario = party->playerPtr;
    hit = NULL;
    if (!(party->flags & PARTY_FLAG_FORCE_FALL)) {
        range = __fabsf(party->gravity);
        if (range < 1.0f) {
            range = 1.0f;
        }
        hit = partySearchGround(party, range, range);
        groundY = party->groundY;
    }
    if (hit) {
        attr = hitGetAttr(hit);
        if (party->yoshiPsndSFXId != -1) {
            psndSFXOff(party->yoshiPsndSFXId);
            party->yoshiPsndSFXId = -1;
        }
        if (attr & 0x200) {
            if (!L_chgMotion(party, 70)) {
                return;
            }
            pos = party->position;
            marioSetSplash(0, &pos);
            L_sweatEntry(party, NULL);
            L_fall(party);
            party->position.y += party->gravity;
            set_damage_root_ypos(mario->position.y - 8.0f);
            L_resetOfs(party);
            party->flags |= PARTY_FLAG_FORCE_FALL;
            if (party->gravity <= -6.0f) {
                party->gravity = -6.0f;
                party->work.common.accelY = 0.0f;
                party->work.yoshi.gravityAccel = 0.0f;
                party->accelY = 0.0f;
            }
            mario->jumpVelocityY = party->gravity;
            mario->jumpAccelerationY = party->work.common.accelY;
            mario->jumpJerkY = party->work.yoshi.gravityAccel;
            mario->jumpSnapY = party->accelY;
            marioChgMot(MARIO_MOTION_DAMAGE);
            return;
        }
        if (attr & 0xC00) {
            yoshi_damageToge(party);
            partyChgRunMode(party, 0xd);
            {
                PartyEntry* extra = partyGetPtr(marioGetExtraPartyId());
                if (extra) {
                    partyChgRunMode(extra, 0xd);
                }
            }
            return;
        }
        if (hit && (hitGetAttr(hit) & HIT_ATTR_DAMAGE_RETURN)) {
            mario->damageHit = hit;
        }
        if (!(party->flags & (PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING))) {
            step = partyChkFrontStep(party, 11.0f, &stepY, &stepA, &stepB);
            diff = stepY - party->position.y;
            if (step && diff <= 11.0f && diff >= 5.0f && party->groundSlope <= 30.0f) {
                party->stepY = stepY;
                if (party->moveSpeed) {
                    if (L_chgMotion(party, 40)) {
                        return;
                    }
                }
            }
        }
    }
    L_sweatEntry(party, hit);
    if (hit == NULL) {
        party->flags &= ~PARTY_FLAG_IS_GROUNDED;
        L_fall(party);
        party->position.y += party->gravity;
    } else {
        party->flags |= PARTY_FLAG_IS_GROUNDED;
        if (party->flags & PARTY_FLAG_IS_FALLING) {
            party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
            mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
            party->velY = 0.0f;
            party->position.y = groundY;
            if (party->yoshiPsndSFXId != -1) {
                psndSFXOff(party->yoshiPsndSFXId);
                party->yoshiPsndSFXId = -1;
            }
        } else {
            party->position.y = groundY;
        }
    }
}

void searchGround2(PartyEntry* party) {
    HitEntry* hit = NULL;
    MarioWork* mario = party->playerPtr;
    s32 attr;

    if (!(party->flags & PARTY_FLAG_FORCE_FALL)) {
        hit = partySearchGround(party, 11.0f, 0.0f);
    }
    if (hit == NULL) {
        L_sweatEntry(party, hit);
        return;
    }
    attr = hitGetAttr(hit);
    if (attr & 0x200) {
        Vec pos = party->position;
        marioSetSplash(0, &pos);
        L_sweatEntry(party, NULL);
        party->gravity = -0.4f;
        party->work.common.accelY = -0.4f;
        party->work.yoshi.gravityAccel = 0.0f;
        party->accelY = 0.0f;
        party->work.yoshi.gravityAccel += party->accelY;
        party->work.common.accelY += party->work.yoshi.gravityAccel;
        party->gravity += party->work.common.accelY;
        if (party->gravity < -15.0f) {
            party->gravity = -15.0f;
        }
        party->position.y += party->gravity;
        party->useMotionId = 70;
        return;
    }
    if (attr & 0x800) {
        yoshi_damageToge(party);
    }
    party->flags |= PARTY_FLAG_IS_GROUNDED;
    party->velY = 0.0f;
    party->position.y = party->groundY;
    party->flags &= ~(PARTY_FLAG_IS_JUMPING | PARTY_FLAG_IS_FALLING | PARTY_FLAG_IS_STEPPING);
    mario->flags &= ~(MARIO_FLAG_IS_JUMPING | MARIO_FLAG_IS_FALLING | MARIO_FLAG_IS_STEPPING | MARIO_FLAG_HAS_INPUT_JUMP);
}

static void L_searchHead(PartyEntry* party) {
    MarioWork* p = party->playerPtr;
    Vec pos = party->position;
    f32 height = partyGetHeight(party);

    p->hitobjHead = marioSearchHead(&pos, &height, 10.0f);
}

void checkWall(PartyEntry* party, f32 spd, f32 dir) {
    MarioWork* mario = marioGetPtr();
    HitEntry* hit;
    Vec pos;
    s32 work1;
    s32 work2;
    s32 attr;

    pos = party->position;
    hit = partySearchWallFront(party, &pos, spd, dir);
    if (hit) {
        party->hitobjFront = hit;
        mario->hitobjFront = hit;
        if (spd) {
            if ((party->flags & PARTY_FLAG_IS_BEING_USED) && marioChkBero(hit) == TRUE) {
                party->hitobjPush = hit;
                mario->hitobjPush = hit;
                mario->hitobjWall = hit;
            } else {
                party->hitobjPush = NULL;
                mario->hitobjPush = NULL;
                mario->hitobjWall = NULL;
            }
        }
        if (hitGetAttr(hit) & HIT_ATTR_TOGE) {
            yoshi_damageToge(party);
        }
        attr = hitGetAttr(hit);
        if ((attr & 0x800004) && !(attr & 0x80080)) {
            movePos(&party->position.x, &party->position.z, spd, dir);
            return;
        }
        party->position = pos;
    } else if (!spd) {
        pos = party->position;
        hit = partySearchWallFront(party, &pos, spd, party->targetDir);
        party->hitobjFront = hit;
        mario->hitobjFront = hit;
        if (hit == NULL) {
            f32 ang = toMovedir(party->displayDirection);
            pos = party->position;
            hit = partySearchWallFront(party, &pos, spd, ang);
            party->hitobjFront = hit;
            mario->hitobjFront = hit;
        }
        if (party->buttonTrg & PAD_BUTTON_A) {
            pos = party->position;
            hit = partySearchWallFront(party, &pos, spd, party->targetDir);
            if (hit == NULL) {
                party->hitobjAction = hit;
                mario->hitobjFrontA = hit;
            } else {
                party->hitobjAction = (void*)party->hitobjFront;
                mario->hitobjFrontA = (void*)party->hitobjFront;
            }
        }
        hit = dou10_yoko_yari2(&work1);
        if (hit && (hitGetAttr(hit) & HIT_ATTR_TOGE)) {
            yoshi_damageToge(party);
        }
    } else {
        hit = dou10_yoko_yari2(&work2);
        if (hit && (hitGetAttr(hit) & HIT_ATTR_TOGE)) {
            yoshi_damageToge(party);
        }
    }
#ifdef VERSION_G8ME01
    partyChkWallAround(party, &pos, spd, dir, party->width, mario->wPlayerCollisionBox.y);
#else
    marioChkWallAround(&pos, 1, spd, dir, party->width, mario->wPlayerCollisionBox.y);
#endif
    party->position.x = pos.x;
    party->position.z = pos.z;
}

void L_getStick(PartyEntry* party, f32* angle, f32* speed) {
    s32 x;
    s32 y;
    MarioWork* mario = party->playerPtr;
    f32 dist;
    f32 ang;

    if (mario->keyOffCount) {
        *speed = 0.0f;
        *angle = party->moveDir;
        return;
    }
    y = -party->stickY;
    x = party->stickX;
    dist = distABf(0.0f, 0.0f, x, -y);
    if (dist >= 70.0f) {
        dist = 70.0f;
    }
    ang = angleABf(0.0f, 0.0f, x, -y);
    if (!dist) {
        ang = party->moveDir;
    }
    *angle = revise360(ang + party->camDir);
    if (dist > 0.0f) {
        if (x * x + y * y > 3025) {
            dist = 1.5f * (mario->mBaseDashSpeed * marioGetMoveRate());
        } else {
            dist = 1.5f * (mario->mBaseWalkSpeed * marioGetMoveRate());
        }
        if (hitGetAttr(mario->hitobjStandOn) & HIT_ATTR_SHALLOW_WATER) {
            mario->flags |= MARIO_FLAG_IN_SHALLOW_WATER;
            dist *= 0.75f;
        } else {
            mario->flags &= ~MARIO_FLAG_IN_SHALLOW_WATER;
        }
        dist *= mario->playerGravity;
    }
    *speed = dist;
}

