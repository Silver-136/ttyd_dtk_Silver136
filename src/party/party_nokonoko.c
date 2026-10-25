#include "mario/mario.h"
#include "mario/mario_party.h"
#include "driver/itemdrv.h"
#include "driver/mobjdrv.h"
#include "party.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/mtx.h>
#include "party/party_move.h"
#include "party/party_bomhei.h"
#include "mario/mario_motion.h"
#include <dolphin/pad.h>
#include "sfx_name_word.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "party/party_nokonoko.h"
#include "mario/mario_hit.h"

extern void partyUpdMoveSpd(PartyEntry* party);
extern void partyWalkInit(PartyEntry* party, s32 mode);


static s32 nokoSe;
#include "driver/hitdrv.h"
#include "mario/mario_sbr.h"

#include "eff/eff_kemu_test.h"
#include "eff/n64/eff_small_star_n64.h"
#include "mario/mariost.h"
void effKemuTestDrawCam();
EffectEntry* effSmallStarN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 dx, f32 dy,
                                  f32 dz);
HitEntry* hitCheckFilter(HitFilterCallback callback, f32* hitX, f32* hitY, f32* hitZ, f32* distance, f32* hitNX,
                         f32* hitNY, f32* hitNZ, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz);
#ifdef VERSION_G8ME01
void partyChkWallAround(PartyEntry* party, Vec* pos, f32 spd, f32 dir, f32 width, f32 height);
#endif
extern void partyChgPose(PartyEntry* party, char* pose);
double sqrt(double);
HitEntry* ccwall(PartyEntry* party, s32 mode);
void lookupSafetyPosSub(Vec pos, f32 height, f32 angle, Vec* out);
void lookupSafetyPos3(Vec pos, f32 height, Vec* out);
void lookupSafetyPos2(Vec pos, f32 height, Vec* out);
u32 lookupSafetyPos(Vec pos, f32 height, Vec* out);

s32 nokonokoGetStatus(PartyEntry* party) {
    u32 m;
    s32 status;
    if (party == NULL) {
        return 0;
    }
    if (party->currentMemberId != 2 || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return 0;
    }
    status = 7;
    m = party->useMotionId;
    if (m <= 2) {
        return 1;
    }
    if (m >= 10 && m <= 13) {
        return 2;
    }
    if (m >= 20 && m <= 21) {
        return 3;
    }
    if (m >= 50 && m <= 71) {
        return 4;
    }
    if (m >= 80 && m <= 100) {
        return 5;
    }
    if (m >= 110 && m <= 114) {
        status = 6;
    }
    return status;
}

BOOL nokonoko_holdItem(void) {
    PartyEntry* party = partyGetPtr(marioGetPartyId());
    if (party == NULL) {
        return FALSE;
    }
    if (party->currentMemberId != 2 || !(party->flags & PARTY_FLAG_IS_BEING_USED)) {
        return FALSE;
    }
    return party->work.nokonoko.heldItem != NULL;
}

void nokonoko_bye(PartyEntry* party) {
    if (party->trigFlags & 8) {
        party->trigFlags &= ~8;
        partyGoodbyeInit(party);
    }
    if (partyGoodbyeMain(party)) {
        partyChgRunMode(party, 14);
    }
}

void nokonoko_init(PartyEntry* party) {
    party->memberKind = 2;
    party->moveKind = 0;
    party->height = 37.0f;
    party->width = 24.0f;
    nokoSe = -1;
}

void nokonoko_move(PartyEntry* party) {
    marioGetPtr();
    if (party->trigFlags & 8) {
        party->trigFlags &= ~8;
        partyWalkInit(party, 1);
    }
    partyUpdMoveSpd(party);
    partyRecordFootmark(party);
    switch (party->moveMode) {
    case 0:
        party_move_mode_stay(party);
        break;
    case 1:
        party_move_mode_walk(party);
        break;
    case 2:
        party_move_mode_behind_mario(party);
        break;
    case 3:
        party_move_mode_beside_mario(party);
        break;
    case 4:
        break;
    }
}

void nokonoko_use_cancel(void) {
    PartyEntry* party;
    marioGetPtr();
    party = partyGetPtr(marioGetPartyId());
    if (nokonokoGetStatus(party) != 0) {
        if (party->flags & PARTY_FLAG_IS_BEING_USED) {
            marioGetPtr();
            if ((u32)nokoSe != 0xFFFFFFFFU) {
                psndSFXOff(nokoSe);
                nokoSe = -1;
            }
            party->flags &= ~PARTY_FLAG_IS_BEING_USED;
            party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
            party->flags &= ~PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
            party->flags2 &= ~0x20;
            party->flags2 &= ~0x10;
            party->height = 37.0f;
            party->work.nokonoko.shellSpinBits = 0;
            party->work.nokonoko.word17C = 0;
            party->work.nokonoko.word180 = 0;
        }
        if (party->runMode != 8 && party->runMode != 0xd) {
            partyChgRunMode(party, 2);
            party->moveMode = 0;
        }
    }
}

void nokonoko_use_post(PartyEntry* party) {
    if (party->work.nokonoko.heldItem != NULL) {
        itemNokoForceGet(party->work.nokonoko.heldItem);
        party->work.nokonoko.heldItem = NULL;
    }
    marioGetPtr();
    if ((u32)nokoSe != 0xFFFFFFFFU) {
        psndSFXOff(nokoSe);
        nokoSe = -1;
    }
    party->flags &= ~PARTY_FLAG_IS_BEING_USED;
    party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
    party->flags &= ~PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
    party->flags &= ~PARTY_FLAG_FORCE_FALL;
    party->flags2 &= ~0x20;
    party->flags2 &= ~0x10;
    party->height = 37.0f;
    party->work.nokonoko.shellSpinBits = 0;
    party->work.nokonoko.word17C = 0;
    party->work.nokonoko.word180 = 0;
}


static inline void noko_se_off(void) {
    marioGetPtr();
    if ((u32)nokoSe != 0xFFFFFFFFU) {
        psndSFXOff(nokoSe);
        nokoSe = -1;
    }
}

static inline void noko_spin(PartyEntry* party) {
    f32 dir;
    if (revise360(party->targetDir - party->camDir) > 180.0f) {
        dir = 1.0f;
    } else {
        dir = -1.0f;
    }
    party->displayDirection = revise360(50.0f * dir + party->displayDirection);
}

static inline void noko_star(PartyEntry* party, s32 n) {
    f32 c = (f32)cos(6.2831855f * party->targetDir / 360.0f);
    f32 s = (f32)sin(6.2831855f * party->targetDir / 360.0f);
    effSmallStarN64Entry(3, n, party->position.x, party->position.y + party->height, party->position.z, s, (-1.0f), -c);
}

#define NOKO_ITEM(party, body)                                                                                     \
    if (party->work.nokonoko.heldItem == NULL) {                                                                   \
        party->work.nokonoko.heldItem = itemNearDistCheck(party->position.x, party->position.y, party->position.z, \
        0.5f * party->width + 12.0f);                                                                              \
        if (party->work.nokonoko.heldItem != NULL) {                                                               \
            itemPickUp(party->work.nokonoko.heldItem);                                                             \
            body                                                                                                   \
        }                                                                                                          \
    }
void nokonoko_use(PartyEntry* party) {
    Vec out;
    Vec out2;
    Vec rp;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 mx;
    f32 mz;
    f32 dir;
    f32 spd;
    f32 s2x;
    u8* moveDistPtr;
    f32 s2y;
    f32 s2z;
    f32 s3x;
    f32 s3y;
    f32 s3z;
    u32 flags;
    HitEntry* hit;
    BOOL b;
    f32 dy;
    f32 dx;
    f32 qx;
    f32 qy;
    f32 qz;
    f32 dz;
    MarioWork* mario = party->playerPtr;

    if (!(party->flags & PARTY_FLAG_IS_EVT_USE) && marioKeyOffChk()) {
        marioGetScreenPos(party->position, &sx, &sy, &sz);
        if (!marioChkInScreen((s32)sx, (s32)sy)) {
            mx = mario->position.x;
            mz = mario->position.z;
            movePos(&mx, &mz, 160.0f, angleABf(mario->position.x, mario->position.z, party->position.x, party->position.z));
            party->position.x = mx;
            party->position.z = mz;
            nokonoko_finish(party);
            return;
        }
    }
    if (party->trigFlags & 8) {
        party->trigFlags &= ~8;
        party->work.nokonoko.heldItem = NULL;
        party->work.nokonoko.shellSpinBits = 0;
        party->work.nokonoko.word17C = 0;
        party->work.nokonoko.word180 = 0;
        if (party->flags & PARTY_FLAG_IS_EVT_USE) {
            if (party->actionTime > 0) {
                party->work.nokonoko.shellFlags |= 1;
            }
        } else {
            party->work.nokonoko.shellFlags &= ~1;
        }
        party->useMotionId = 0;
    }

    switch (party->useMotionId) {
    case 0:
        if (mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE || mario->currMotionId == MARIO_MOTION_DAMAGE) {
            party->useMotionId = 70;
            break;
        }
        partyClearJumpPara(party);
        party->flags |= PARTY_FLAG_DISABLE_FLOORS;
        party->flags2 |= 0x10;
        partyChgPose(party, "PNK_K_2");
        party->height = 18.5f;
        party->useMotionId = 1;
        party->moveFrames = 100;
        party->work.nokonoko.heldItem = NULL;
        party->work.nokonoko.dashed = 0;
        marioAdjustMoveDir();
        mario->baseSpeed = 0.0f;
        party->moveTarget = mario->position;
        movePos(&party->moveTarget.x, &party->moveTarget.z, 2.0f, toMovedirSimple(mario->displayDirectionTarget));
        party->moveFrames = 4;
        party->velY = 0.0f;
        party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        party->moveSpeed = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z) /
                           (f32)party->moveFrames;
        party->displayDirection = revise360(45.0f + party->displayDirection);
        party->useMotionId = 1;
        marioChgSmallJumpMotion();
        mario->trigFlags |= MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI;
        mario->baseSpeed = 0.0f;
        break;
    case 1:
        if (mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE || mario->currMotionId == MARIO_MOTION_DAMAGE) {
            party->useMotionId = 70;
            break;
        }
        party->displayDirection = revise360(45.0f + party->displayDirection);
        party->position.x += (party->moveTarget.x - party->position.x) / (f32)party->moveFrames;
        party->position.y += (party->moveTarget.y - party->position.y) / (f32)party->moveFrames;
        party->position.z += (party->moveTarget.z - party->position.z) / (f32)party->moveFrames;
        if (--party->moveFrames > 0) {
            break;
        }
        party->position = party->moveTarget;
        party->moveSpeed = 0.0f;
        party->displayDirectionTarget = mario->displayDirectionTarget;
        party->displayDirection = mario->displayDirectionTarget;
        party->useMotionId = 2;
        partyChgPose(party, "PNK_A_1");
    case 2:
        if (mario->currMotionId == MARIO_MOTION_DAMAGE_TOGE || mario->currMotionId == MARIO_MOTION_DAMAGE) {
            party->useMotionId = 70;
            break;
        }
        if (mario->hitobjStandOn == NULL) {
            break;
        }
        marioChgMot(MARIO_MOTION_JUMP);
        mario->flags |= MARIO_FLAG_STOP_AIR_MOVEMENT;
        mario->baseSpeed = 0.0f;
        party->useMotionId = 10;
        party->moveFrames = 15;
        party->moveSpeed = 0.0f;
        party->moveDistance = 0.0f;
        party->targetDir = toMovedir(mario->displayDirectionTarget);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_LEAP1), &party->position);
        break;
    case 10:
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA1), &party->position);
        noko_star(party, 3);
        party->flags |= PARTY_FLAG_DISABLE_FLOORS;
        party->flags |= PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
        party->useMotionId = 11;
    case 11:
        party->moveSpeed += 2.0f;
        if (party->moveSpeed > 10.0f) {
            party->moveSpeed = 10.0f;
        }
    case 12:
        noko_spin(party);
        if (gp->retraceCount & 1) {
            effKemuTestDrawCam(effKemuTestEntry(0, party->position.x, party->height / 4.0f + party->position.y,
                                                party->position.z, 0.3f),
                               party->camId);
        }
        party->moveDistance += party->moveSpeed;
        NOKO_ITEM(party, party->useMotionId = 50; psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA_WALL1), &party->position); noko_star(party, 1); break;)
        if (ccwall(party, 0) != NULL) {
            party->useMotionId = 50;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA_WALL1), &party->position);
            noko_star(party, 1);
            break;
        }
        if (party->moveDistance >= 120.0f) {
            party->useMotionId = 12;
            party->moveSpeed += 0.15f * ((-1.0f) - party->moveSpeed);
            if (party->moveSpeed <= 0.0f) {
                party->moveSpeed = 0.0f;
                if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                    if (mario->buttonHold & PAD_BUTTON_X) {
                        party->useMotionId = 13;
                        party->moveFrames = 0;
                    } else {
                        party->useMotionId = 50;
                    }
                } else if (party->work.nokonoko.shellFlags & 1) {
                    party->moveFrames = 5;
                    party->useMotionId = 21;
                    party->work.nokonoko.dashed = 1;
                    if ((u32)nokoSe == 0xFFFFFFFFU) {
                        nokoSe = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_HOLD1), &party->position);
                    }
                } else {
                    party->useMotionId = 50;
                }
            }
        }
        break;
    case 13:
        if (++party->moveFrames >= 5) {
            party->moveFrames = 5;
            party->useMotionId = 20;
            party->work.nokonoko.dashed = 1;
            if ((u32)nokoSe == 0xFFFFFFFFU) {
                nokoSe = psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_HOLD1), &party->position);
            }
        }
    case 20:
        noko_spin(party);
        if (gp->retraceCount % 30 == 0) {
            effKemuTestDrawCam(effKemuTestEntry(5, party->position.x, party->position.y, party->position.z, 1.0f),
                               party->camId);
        }
        if (ccwall(party, 0) != NULL) {
            party->useMotionId = 50;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA_WALL1), &party->position);
            noko_star(party, 1);
            noko_se_off();
            break;
        }
        NOKO_ITEM(party, party->useMotionId = 50; psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA_WALL1), &party->position); noko_star(party, 1); noko_se_off(); break;)
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
            if (mario->currMotionId == MARIO_MOTION_SHIP || mario->currMotionId == MARIO_MOTION_JABARA || mario->currMotionId == MARIO_MOTION_PLANE || mario->currMotionId == MARIO_MOTION_ROLL || (u16)(mario->currMotionId - 0x1f) <= 1 ||
                (mario->flags & MARIO_FLAG_FORCED_MOVEMENT) || !marioChkKey()) {
                nokonoko_finish(party);
                noko_se_off();
                break;
            }
            if (mario->buttonHold & PAD_BUTTON_X) {
                break;
            }
            party->useMotionId = party->work.nokonoko.dashed != 0 ? 80 : 50;
            noko_se_off();
            break;
        }
        if (party->work.nokonoko.shellFlags & 1) {
            party->useMotionId = 80;
        } else {
            party->useMotionId = 50;
        }
        noko_se_off();
        break;
    case 21:
        noko_spin(party);
        if (gp->retraceCount % 30 == 0) {
            effKemuTestDrawCam(effKemuTestEntry(5, party->position.x, party->position.y, party->position.z, 1.0f),
                               party->camId);
        }
        break;
    }

    moveDistPtr = (u8*)&party->moveDistance;
    switch (party->useMotionId) {
    case 50:
        partyClearJumpPara(party);
        party->useMotionId = 51;
        party->moveSpeed = 0.0f;
        party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA_BACK1), &party->position);
        break;
    case 51: {
        f32 lim = party->work.nokonoko.dashed != 0 ? 20.0f : 10.0f;
        party->moveSpeed += 0.3f * (lim - party->moveSpeed);
        if (party->moveSpeed >= lim) {
            party->moveSpeed = lim;
            if ((*(f32*)moveDistPtr) < 10.0f) {
                party->useMotionId = 52;
            }
        }
    }
    case 52:
        if (ccwall(party, 1) != NULL) {
            partyGetMoveDirSpd(party, &dir, &spd);
#ifdef VERSION_G8ME01
            partyChkWallAround(party, &party->position, spd, dir, party->width, party->height);
#else
            marioChkWallAround(&party->position, 1, spd, dir, party->width, party->height);
#endif
            party->useMotionId = 70;
            psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA_WALL1), &party->position);
            noko_star(party, 1);
            return;
        }
        noko_spin(party);
        party->moveSpeed += 0.1f * ((-1.0f) - party->moveSpeed);
        (*(f32*)moveDistPtr) -= party->moveSpeed;
        if ((*(f32*)moveDistPtr) <= 0.0f) {
            (*(f32*)moveDistPtr) = 0.0f;
            party->useMotionId = 53;
            party->moveSpeed += 0.1f * ((-1.0f) - party->moveSpeed);
            party->work.yoshi.gravityAccel = 50.0f;
        }
        break;
    case 53:
        if (ccwall(party, 1) != NULL) {
            party->useMotionId = 70;
            return;
        }
        party->work.yoshi.gravityAccel -= 2.2f;
        if (party->work.yoshi.gravityAccel <= 0.0f) {
            party->work.yoshi.gravityAccel = 0.0f;
        }
        noko_spin(party);
        party->moveSpeed += ((-2.0f) - party->moveSpeed) * (party->work.nokonoko.dashed != 0 ? 0.2f : 0.1f);
        (*(f32*)moveDistPtr) -= party->moveSpeed;
        if (party->moveSpeed <= 0.0f) {
            party->moveSpeed = 0.0f;
            flags = lookupSafetyPos(party->moveTarget, party->height, &out);
            if (party->hitobjStandOn != NULL && ((u32)(flags - 1) <= 1 || flags == 4 || flags == 8)) {
                party->useMotionId = 70;
                break;
            }
            switch (flags) {
            case 3:
                lookupSafetyPosSub(party->moveTarget, party->height, 135.0f, &out);
                break;
            case 5:
                lookupSafetyPosSub(party->moveTarget, party->height, 315.0f, &out);
                break;
            case 10:
                lookupSafetyPosSub(party->moveTarget, party->height, 225.0f, &out);
                break;
            case 12:
                lookupSafetyPosSub(party->moveTarget, party->height, 315.0f, &out);
                break;
            case 7:
            case 14:
            case 15:
                party->useMotionId = 70;
                break;
            case 1:
                lookupSafetyPosSub(party->moveTarget, party->height, 270.0f, &out);
                break;
            case 8:
                lookupSafetyPos3(party->moveTarget, party->height, &out);
                break;
            default:
                lookupSafetyPos2(party->moveTarget, party->height, &out);
                break;
            }
            party->useMotionId = 54;
            party->moveTarget = out;
            (*(f32*)moveDistPtr) = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
            party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        }
        break;
    case 54:
        noko_spin(party);
        party->moveSpeed += 0.5f;
        if (party->moveSpeed >= 4.0f) {
            party->moveSpeed = 4.0f;
        }
        (*(f32*)moveDistPtr) -= party->moveSpeed;
        if ((*(f32*)moveDistPtr) <= 10.0f) {
            party->useMotionId = 70;
            party->moveSpeed = 0.0f;
            return;
        }
        break;
    case 55:
        if (ccwall(party, 1) != NULL) {
            party->useMotionId = 70;
            return;
        }
        noko_spin(party);
        party->work.yoshi.gravityAccel -= 2.2f;
        if (party->work.yoshi.gravityAccel <= 0.0f) {
            party->work.yoshi.gravityAccel = 0.0f;
        }
        party->moveSpeed += ((-2.0f) - party->moveSpeed) * (party->work.nokonoko.dashed != 0 ? 0.2f : 0.1f);
        if (party->moveSpeed <= 0.0f) {
            party->useMotionId = 70;
            party->moveSpeed = 0.0f;
            return;
        }
        break;
    case 70:
        dx = party->position.x - mario->position.x;
        dy = party->position.y - mario->position.y;
        dz = party->position.z - mario->position.z;
        qx = dx * dx;
        qy = dy * dy;
        qz = dz * dz;
        if ((f32)sqrt(qz + (qx + qy)) < 5.0f) {
            f32 a;
            a = mario->directionView + (mario->directionView >= 180.0 ? (-90.0f) : 90.0f);
            movePos(&party->position.x, &party->position.z, 0.5f * mario->wPlayerCollisionBox.x, a);
            ccwall(party, 1);
        }
        party->moveSpeed = 0.0f;
        party->flags2 &= ~0x10;
        party->flags2 |= 0x20;
        party->useMotionId = 71;
        break;
    case 71:
        if (party->work.nokonoko.heldItem != NULL) {
            itemNokoForceGet(party->work.nokonoko.heldItem);
            party->work.nokonoko.heldItem = NULL;
        }
        nokonoko_finish(party);
        return;
    }

    switch (party->useMotionId) {
    case 80:
        party->useMotionId = 81;
        party->moveSpeed = 0.0f;
        party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        (*(f32*)moveDistPtr) += 75.0f;
        psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA_BACK1), &party->position);
    case 81:
        noko_spin(party);
        party->moveSpeed += 3.0f;
        if (party->moveSpeed >= 18.0f) {
            party->moveSpeed = 18.0f;
        }
        NOKO_ITEM(party, psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA_WALL1), &party->position); noko_star(party, 1);)
        hit = ccwall(party, 1);
        if (hit != NULL) {
            if (!(hitGetAttr(hit) & 0x80000000)) {
                party->useMotionId = 100;
                return;
            }
            mobjHitObjPtrToPtr(hit);
            party->useMotionId = 110;
            return;
        }
        (*(f32*)moveDistPtr) -= party->moveSpeed;
        if ((*(f32*)moveDistPtr) <= 20.0f) {
            party->useMotionId = 82;
        }
        break;
    case 82:
        noko_spin(party);
        party->moveSpeed += 0.35f * ((-1.0f) - party->moveSpeed);
        NOKO_ITEM(party, psndSFXOn_3D(SFX_NAME_WORD(PSND_SFX_PARTY_PNK_KOURA_WALL1), &party->position); noko_star(party, 1);)
        if (ccwall(party, 1) != NULL) {
            party->useMotionId = 100;
            break;
        }
        (*(f32*)moveDistPtr) -= party->moveSpeed;
        if ((*(f32*)moveDistPtr) <= 0.0f) {
            (*(f32*)moveDistPtr) = distABf(party->moveTarget.x, party->moveTarget.z, party->position.x, party->position.z);
            party->useMotionId = 84;
            party->work.yoshi.gravityAccel = 50.0f;
            party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        }
        break;
    case 84:
        noko_spin(party);
        if (ccwall(party, 1) != NULL) {
            party->useMotionId = 100;
            break;
        }
        (*(f32*)moveDistPtr) -= party->moveSpeed;
        if ((*(f32*)moveDistPtr) <= 30.0f) {
            party->useMotionId = 86;
            party->moveSpeed = 0.0f;
            party->moveFrames = sysMsec2Frame(500);
        }
        break;
    case 86:
        noko_spin(party);
        if (--party->moveFrames <= 0) {
            party->useMotionId = 100;
        }
        if (ccwall(party, 1) != NULL) {
            party->useMotionId = 100;
        }
        break;
    case 100:
        ccwall(party, 1);
        dy = party->position.x - mario->position.x;
        dx = party->position.y - mario->position.y;
        dz = party->position.z - mario->position.z;
        qx = dy * dy;
        qy = dx * dx;
        qz = dz * dz;
        if ((f32)sqrt(qz + (qx + qy)) < 5.0f) {
            f32 a;
            a = mario->directionView + (mario->directionView >= 180.0 ? 90.0f : (-90.0f));
            movePos(&party->position.x, &party->position.z, 0.5f * mario->wPlayerCollisionBox.x, a);
            ccwall(party, 1);
        }
        party->moveSpeed = 0.0f;
        party->flags2 &= ~0x10;
        if (party->work.nokonoko.heldItem != NULL) {
            itemNokoForceGet(party->work.nokonoko.heldItem);
            party->work.nokonoko.heldItem = NULL;
        }
        if (party->hitobjStandOn == NULL) {
            party->useMotionId = 88;
            break;
        }
        if (party->work.nokonoko.heldItem != NULL) {
            itemNokoForceGet(party->work.nokonoko.heldItem);
            party->work.nokonoko.heldItem = NULL;
        }
        nokonoko_finish(party);
        return;
    case 88:
        party->moveSpeed = 0.0f;
        flags = lookupSafetyPos(party->moveTarget, party->height, &out2);
        if (party->hitobjStandOn != NULL && ((u32)(flags - 1) <= 1 || flags == 4 || flags == 8)) {
            party->useMotionId = 70;
            break;
        }
        switch (flags) {
        case 3:
            lookupSafetyPosSub(party->moveTarget, party->height, 135.0f, &out2);
            break;
        case 5:
            lookupSafetyPosSub(party->moveTarget, party->height, 315.0f, &out2);
            break;
        case 10:
            lookupSafetyPosSub(party->moveTarget, party->height, 225.0f, &out2);
            break;
        case 12:
            lookupSafetyPosSub(party->moveTarget, party->height, 315.0f, &out2);
            break;
        case 7:
        case 14:
        case 15:
            party->useMotionId = 70;
            break;
        case 1:
            lookupSafetyPosSub(party->moveTarget, party->height, 270.0f, &out2);
            break;
        case 8:
            lookupSafetyPos3(party->moveTarget, party->height, &out2);
            break;
        default:
            lookupSafetyPos2(party->moveTarget, party->height, &out2);
            break;
        }
        party->useMotionId = 90;
        party->moveTarget = out2;
        (*(f32*)moveDistPtr) = distABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        party->targetDir = angleABf(party->position.x, party->position.z, party->moveTarget.x, party->moveTarget.z);
        party->work.yoshi.gravityAccel = 50.0f;
        break;
    case 90:
        noko_spin(party);
        party->moveSpeed += 0.5f;
        if (party->moveSpeed >= 4.0f) {
            party->moveSpeed = 4.0f;
        }
        (*(f32*)moveDistPtr) -= party->moveSpeed;
        if ((*(f32*)moveDistPtr) <= 10.0f) {
            party->useMotionId = 70;
            party->moveSpeed = 0.0f;
        }
        break;
    case 110:
        party->moveSpeed = 0.0f;
        party->velY = 15.0f;
        party->accelY = 0.0f;
        party->moveFrames = sysMsec2Frame(1000);
        party->useMotionId = 111;
    case 111:
        party->position.y += party->velY;
        marioGetScreenPos(party->position, &s2x, &s2y, &s2z);
        if (s2y <= (-300.0f)) {
            party->moveFrames = sysMsec2Frame(800);
            party->useMotionId = 112;
        }
        break;
    case 112:
        if (--party->moveFrames > 0) {
            break;
        }
        if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
            partyGetReturnPos(&rp);
        } else {
            rp = mario->position;
            b = FALSE;
            if (mario->displayDirectionTarget >= 90.0f && mario->displayDirectionTarget <= 270.0f) {
                b = TRUE;
            }
            movePos(&rp.x, &rp.z, 30.0f, toMovedir2(b ? 0.0f : 180.0f, (-30.0f)));
        }
        party->position.x = rp.x;
        party->position.y = 1000.0f + rp.y;
        party->position.z = rp.z;
        party->velY = 0.0f;
        party->accelY = (-1.0f);
        party->moveSpeed = 0.0f;
        party->flags |= PARTY_FLAG_IS_FALLING;
        party->flags |= PARTY_FLAG_FORCE_FALL;
        party->flags &= ~PARTY_FLAG_DISABLE_FLOORS;
        party->flags &= ~PARTY_FLAG_DISABLE_FRONT_WALL_CHK;
        party->flags2 &= ~0x20;
        party->flags2 &= ~0x10;
        party->height = 37.0f;
        party->useMotionId = 113;
        break;
    case 113:
        marioGetScreenPos(party->position, &s3x, &s3y, &s3z);
        if (s3y > 240.0f) {
            party->flags &= ~PARTY_FLAG_FORCE_FALL;
        }
        partyMove(party);
        if (party->velY <= (-6.0f)) {
            party->accelY = 0.0f;
            party->velY = (-6.0f);
        }
        if (party->position.y < (-2000.0f)) {
            party->position = mario->position;
            party->position.y += 200.0f;
        }
        if (party->hitobjStandOn != NULL) {
            party->displayDirectionTarget = mario->displayDirectionTarget;
            party->displayDirection = party->displayDirectionTarget;
            if (!((hitGetAttr(party->hitobjStandOn) >> 9) & 1)) {
                party->useMotionId = 114;
                party->moveFrames = 30;
            }
        }
        break;
    case 114:
        if (--party->moveFrames > 0) {
            break;
        }
        nokonoko_finish(party);
        return;
    }
    if (party->useMotionId != 100) {
        partyMoveNoHosei(party);
    }
}

void nokonoko_finish(PartyEntry* party) {
    if (party != NULL && party->currentMemberId == 2 && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        nokonoko_use_post(party);
        if (party->runMode != 9) {
            if (!(party->flags & PARTY_FLAG_IS_EVT_USE)) {
                partyChgRunMode(party, 2);
                party->moveMode = 0;
            } else {
                party->flags &= 0x7FFFFFFF;
                partyChgRunMode(party, 0);
            }
        }
    }
}

void nokotaro_hold_cancel(PartyEntry* party) {
    party->useMotionId = 20;
}

HitEntry* ccwall(PartyEntry* party, s32 mode) {
    f32 s;
    f32 c;
    f32 dist;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 nx;
    f32 ny;
    f32 nz;
    f32 s1;
    f32 c1;
    f32 s2;
    f32 c2;
    f32 s3;
    f32 c3;
    HitEntry* hit;
    f32 py;
    f32 vx;
    f32 vz;
    f32 d;
    f32 y0;
    f32 y;
    f32 z;
    f32 x;
    f32 yofs;
    f32 k;
    f32 w;

    yofs = 10.0f;
    x = party->position.x;
    z = party->position.z;
    y0 = party->position.y + (w = party->height);
    k = 30.0f;
    dist = w + k;
    y0 = yofs + y0;
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x,
                         y0, z, 0.0f, -1.0f, 0.0f);
    if (((hitGetAttr(hit) >> 9) & 1) == 1) {
        hit = NULL;
    }
    party->hitobjStandOn = hit;
    if (hit != NULL) {
        hy = (f32)(s32)(100.0f * hy + 0.5f) / 100.0f;
        py = (f32)(s32)(100.0f * party->position.y + 0.5f) / 100.0f;
        if (__fabsf(py - hy) <= 11.0f) {

            if (py <= hy) {
                if (mode == 0) {
                    party->position.y = hy;
                    if (__fabsf(py - hy) >= 1.0f) {
                        party->work.nokonoko.shellFlags |= 2;
                    }
                }
            } else {
                if ((party->work.nokonoko.shellFlags & 2) && mode == 1) {
                    party->position.y = hy;
                }
                if (__fabsf(py - hy) <= 6.0f) {
                    party->position.y = hy;
                }
            }
        }
    }

    party->hitobjAction = NULL;
    party->hitobjFront = NULL;
    yofs += 1.0f;
    x = party->position.x;
    z = party->position.z;
    y = party->position.y + yofs;
    dist = 0.5f * party->width + party->moveSpeed;
    sincosf(&s, &c, party->targetDir);
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, s, 0.0f, c);
    if (hit != NULL && !(hitGetAttr(hit) & 5)) {
        sincosf(&s1, &c1, 180.0f + party->targetDir);
        party->position.x = hx + 0.5f * (s1 * party->width);
        party->position.z = hz + 0.5f * (c1 * party->width);
        vx = s * party->moveSpeed;
        vz = c * party->moveSpeed;
        d = vx * nx + vz * nz;
        party->position.x += 0.5f * (vx - d * nx);
        party->position.z += 0.5f * (vz - d * nz);
        if (party->work.nokonoko.dashed != 0 || mode == 0) {
            party->hitobjAction = hit;
        }
        party->hitobjFront = hit;
        return hit;
    }

    w = 0.4f * party->width;
    z = w * (f32)cos(3.1415927f * (party->targetDir - 90.0f) / 180.0f) + party->position.z;
    x = w * (f32)sin(3.1415927f * (party->targetDir - 90.0f) / 180.0f) + party->position.x;
    y = party->position.y + yofs;
    dist = 0.4f * party->width + party->moveSpeed;
    sincosf(&s, &c, party->targetDir);
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, s, 0.0f, c);
    if (hit != NULL && !(hitGetAttr(hit) & 5)) {
        sincosf(&s2, &c2, 180.0f + party->targetDir);
        party->position.x = hx + 0.5f * (s2 * party->width);
        party->position.z = hz + 0.5f * (c2 * party->width);
        party->position.z -= w * (f32)cos(3.1415927f * (party->targetDir - 90.0f) / 180.0f);
        party->position.x -= w * (f32)sin(3.1415927f * (party->targetDir - 90.0f) / 180.0f);
        vx = s * party->moveSpeed;
        vz = c * party->moveSpeed;
        d = vx * nx + vz * nz;
        party->position.x += 0.5f * (vx - d * nx);
        party->position.z += 0.5f * (vz - d * nz);
        if (party->work.nokonoko.dashed != 0 || mode == 0) {
            party->hitobjAction = hit;
        }
        party->hitobjFront = hit;
        party->position.x = hx;
        return hit;
    }

    w = 0.4f * party->width;
    z = w * (f32)cos(3.1415927f * (90.0f + party->targetDir) / 180.0f) + party->position.z;
    x = w * (f32)sin(3.1415927f * (90.0f + party->targetDir) / 180.0f) + party->position.x;
    y = party->position.y + yofs;
    dist = 0.4f * party->width + party->moveSpeed;
    dist = 0.4f * party->width + party->moveSpeed;
    sincosf(&s, &c, party->targetDir);
    hit = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, s, 0.0f, c);
    if (hit != NULL && !(hitGetAttr(hit) & 5)) {
        sincosf(&s3, &c3, 180.0f + party->targetDir);
        party->position.x = hx + 0.5f * (s3 * party->width);
        party->position.z = hz + 0.5f * (c3 * party->width);
        party->position.z -= w * (f32)cos(3.1415927f * (90.0f + party->targetDir) / 180.0f);
        party->position.x -= w * (f32)sin(3.1415927f * (90.0f + party->targetDir) / 180.0f);
        vx = s * party->moveSpeed;
        vz = c * party->moveSpeed;
        d = vx * nx + vz * nz;
        party->position.x += 0.5f * (vx - d * nx);
        party->position.z += 0.5f * (vz - d * nz);
        if (party->work.nokonoko.dashed != 0 || mode == 0) {
            party->hitobjAction = hit;
        }
        party->hitobjFront = hit;
        party->position.x = hx;
        return hit;
    }
    return NULL;
}

void nokoStrippedPool(f32* p) { p[0] = 10.0f; p[1] = 30.0f; p[2] = 100.0f; p[3] = 11.0f; p[4] = 6.0f; }
u32 lookupSafetyPos(Vec pos, f32 height, Vec* out) {
    Vec p[4];
    f32 dist;
    f32 hx; f32 hy; f32 hz; f32 nx; f32 ny; f32 nz;
    f32 rad;
    f32 x; f32 y; f32 z; f32 c;
    f32 w;
    HitEntry* h0;
    HitEntry* h1;
    HitEntry* h2;
    HitEntry* h3;
    u32 flags;
    MarioWork* player = marioGetPtr();
    w = player->wPlayerCollisionBox.x;
    // FAKE: comma makes the first rad web an early-coloured temp (target f31; same value)
    rad = (0, (3.1415927f * revise360(toMovedir(player->displayDirectionTarget) - 180.0f)) / 180.0f);
    x = w * (f32)sin(rad) + pos.x;
    p[3].x = x;
    c = (f32)cos(rad);
    y = pos.y;
    p[3].y = y;
    z = w * -c + pos.z;
    p[3].z = z;
    // FAKE: y + height held in the dead c (named copy into the f2 arg) gives the target probe-1 schedule; same value
    c = y + height;
    dist = 0.1f + height;
    h0 = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, c, z, 0.0f, (-1.0f), 0.0f);
    rad = (3.1415927f * revise360(toMovedir(player->displayDirectionTarget) - 90.0f)) / 180.0f;
    x = w * (f32)sin(rad) + pos.x;
    p[2].x = x;
    c = (f32)cos(rad);
    y = pos.y;
    p[2].y = y;
    z = w * -c + pos.z;
    p[2].z = z;
    dist = 0.1f + height;
    h1 = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y + height, z, 0.0f, (-1.0f), 0.0f);
    rad = (3.1415927f * revise360(90.0f + toMovedir(player->displayDirectionTarget))) / 180.0f;
    x = w * (f32)sin(rad) + pos.x;
    p[1].x = x;
    c = (f32)cos(rad);
    y = pos.y;
    p[1].y = y;
    z = w * -c + pos.z;
    p[1].z = z;
    dist = 0.1f + height;
    h2 = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y + height, z, 0.0f, (-1.0f), 0.0f);
    rad = (3.1415927f * revise360(toMovedir(player->displayDirectionTarget))) / 180.0f;
    x = w * (f32)sin(rad) + pos.x;
    p[0].x = x;
    c = (f32)cos(rad);
    y = pos.y;
    p[0].y = y;
    z = w * -c + pos.z;
    p[0].z = z;
    dist = 0.1f + height;
    h3 = hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y + height, z, 0.0f, (-1.0f), 0.0f);
    flags = 0;
    if (h2) flags |= 4;
    if (h3) flags |= 8;
    if (h0) flags |= 1;
    if (h1) flags |= 2;
    if (h0) {
        if (h1 && h2 && h3) {
        out->x = 0.5f * (p[3].x + p[1].x); out->z = 0.5f * (p[3].z + p[1].z); out->y = p[3].y; return flags;
        }
        if (h1) {
        out->x = 0.5f * (p[3].x + p[2].x); out->z = 0.5f * (p[3].z + p[2].z); out->y = pos.y; return flags;
        }
        if (h2) {
        out->x = 0.5f * (p[3].x + p[1].x); out->z = 0.5f * (p[3].z + p[1].z); out->y = p[3].y; return flags;
        }
        if (h3) {
        out->x = 0.5f * (p[3].x + p[0].x); out->z = 0.5f * (p[3].z + p[0].z); out->y = p[3].y; return flags;
        }
        *out = p[3];
    } else if (h3) {
        if (h1) {
        out->x = 0.5f * (p[0].x + p[2].x); out->z = 0.5f * (p[0].z + p[2].z); out->y = p[0].y; return flags;
        }
        if (h2) {
        out->x = 0.5f * (p[1].x + p[2].x); out->z = 0.5f * (p[1].z + p[2].z); out->y = p[1].y; return flags;
        }
        *out = p[0];
    } else {
        if (h1) {
            *out = p[2];
            return flags;
        }
        if (h2) {
            *out = p[1];
            return flags;
        }
    }
    *out = pos;
    return flags;
}

// FAKE: sin via an inline helper makes each sin result an inline @ temp (numbered after the other temps), which gives the
// target colouring in lookupSafetyPos2; same value as (f32)sin(rad)
static inline f32 nokoSin(f32 rad) { return (f32)sin(rad); }

void lookupSafetyPos2(Vec pos, f32 height, Vec* out) {
    f32 rad;
    f32 dx;
    f32 dz;
    f32 d;
    f32 dist;
    f32 w;
    f32 x;
    f32 z;
    f32 y;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 nx;
    f32 ny;
    f32 nz;
    MarioWork* player = marioGetPtr();

    w = player->wPlayerCollisionBox.x;
    // (f32)(w * d) keeps the product a separate fmuls (no fmadds), as in the target
    rad = (3.1415927f * 90.0f) / 180.0f;
    dx = nokoSin(rad);
    dz = (f32)cos(rad);
    x = pos.x + (f32)(w * dx);
    y = pos.y;
    z = pos.z + (f32)(w * -dz);
    d = 0.1f + height;
    dist = d;
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y + height, z, 0.0f, (-1.0f), 0.0f) != NULL) {
        out->x = x;
        out->z = z;
        out->y = y;
        return;
    }
    rad = (3.1415927f * 270.0f) / 180.0f;
    dx = nokoSin(rad);
    dz = (f32)cos(rad);
    x = pos.x + (f32)(w * dx);
    y = pos.y;
    z = pos.z + (f32)(w * -dz);
    dist = d;
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y + height, z, 0.0f, (-1.0f), 0.0f) != NULL) {
        out->x = x;
        out->z = z;
        out->y = y;
        return;
    }
    rad = (3.1415927f * 180.0f) / 180.0f;
    dx = nokoSin(rad);
    dz = (f32)cos(rad);
    x = pos.x;
    z = pos.z;
    y = pos.y;
    dist = d;
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y + height, z, 0.0f, (-1.0f), 0.0f) != NULL) {
        out->x = x;
        out->z = z;
        out->y = y;
        return;
    }
    rad = (3.1415927f * 0.0f) / 180.0f;
    dx = nokoSin(rad);
    dz = (f32)cos(rad);
    x = pos.x + (f32)(w * dx);
    y = pos.y;
    z = pos.z + (f32)(w * -dz);
    dist = d;
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y + height, z, 0.0f, (-1.0f), 0.0f) != NULL) {
        out->x = x;
        out->z = z;
        out->y = y;
        return;
    }
    *out = pos;
}

void lookupSafetyPos3(Vec pos, f32 height, Vec* out) {
    f32 y;
    f32 rad;
    f32 dx;
    f32 dz;
    f32 d;
    f32 dist;
    f32 w;
    f32 x;
    f32 z2;
    f32 z;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 nx;
    f32 ny;
    f32 nz;
    MarioWork* player = marioGetPtr();

    w = player->wPlayerCollisionBox.x;
    // (f32)(w * d) keeps the product a separate fmuls (no fmadds), as in the target
    rad = (3.1415927f * 90.0f) / 180.0f;
    dx = (f32)sin(rad);
    dz = (f32)cos(rad);
    x = pos.x + (f32)(w * dx);
    y = pos.y;
    z = pos.z + (f32)(w * -dz);
    d = 0.1f + height;
    dist = d;
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y + height, z, 0.0f, -1.0f, 0.0f) != NULL) {
        out->x = x;
        out->z = z;
        out->y = y;
        return;
    }
    rad = (3.1415927f * 270.0f) / 180.0f;
    dx = (f32)sin(rad);
    dz = (f32)cos(rad);
    x = pos.x + (f32)(w * dx);
    y = pos.y;
    z2 = pos.z + (f32)(w * -dz);
    dist = d;
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y + height, z2, 0.0f, -1.0f, 0.0f) != NULL) {
        out->x = x;
        out->z = z2;
        out->y = y;
        return;
    }
    *out = pos;
}

void lookupSafetyPosSub(Vec pos, f32 height, f32 angle, Vec* out) {
    f32 rad;
    f32 x;
    f32 y;
    f32 z;
    f32 dist;
    f32 hx;
    f32 hy;
    f32 hz;
    f32 nx;
    f32 ny;
    f32 nz;
    Vec* p;
    MarioWork* player = marioGetPtr();
    rad = (3.1415927f * angle) / 180.0f;
    angle = player->wPlayerCollisionBox.x;
    x = angle * (f32)sin(rad) + pos.x;
    z = angle * -(f32)cos(rad) + pos.z;
    y = pos.y + height;
    dist = 0.1f + height;
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, 0.0f, (-1.0f), 0.0f) != NULL) {
        Vec v = {0.0f, 0.0f, 0.0f};
        v.x = x;
        v.y = y;
        v.z = z;
        p = &v;
    } else {
        p = &pos;
    }
    *out = *p;
}

