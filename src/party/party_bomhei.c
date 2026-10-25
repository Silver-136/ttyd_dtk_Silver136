#include "dolphin/types.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mario_sbr.h"
#include "mario/mariost.h"
#include "memory.h"
#include "party.h"
#include "pmario_sound.h"
#include "system.h"
#include "driver/animdrv.h"
#include "mario/mario_motion.h"
#include "driver/camdrv.h"
#include "driver/hitdrv.h"
#include "driver/mobjdrv.h"
#include <dolphin/mtx.h>
#include <string.h>
#include "party/party_hit.h"
#include "party/party_move.h"
#include "eff/eff_sandars.h"
#include "driver/npcdrv.h"
#include "evt/evt_cmd.h"

#ifdef VERSION_G8ME01
// US has no local duplicate: this is the global bomhei_init
#define bomhei_init_800BFBB0 bomhei_init
#endif

// US MarioWork has 4 more bytes before these raw offsets
#ifdef VERSION_G8ME01
#define MW_US 4
#else
#define MW_US 0
#endif

extern void partyChgRunMode(PartyEntry* party, s32 mode);
extern void partyGoodbyeInit(PartyEntry* party);
extern s32 partyGoodbyeMain(PartyEntry* party);
extern void partyGetAppearPos(PartyEntry* party, Vec* pos);

extern u8* caseCheckHitObj2(void* hit);

extern s32 evt_cam_shake(void* evt, BOOL isFirstCall);

s32 m_w_7_sync[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 9, 8, 7, 5, 3, 1, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 9, 8, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0};

EVT_BEGIN(longQuake_evt)
    CALL(evt_cam_shake, 4, PTR(0xF24A7A8F), PTR(0xF24A7A8F), 600)
    RETURN
EVT_END()

static EVT_BEGIN(quake_evt)
    CALL(evt_cam_shake, 4, PTR(0xF24A7A8F), PTR(0xF24A7A8F), 200)
    RETURN
EVT_END()

static inline BOOL isBombRock(HitEntry* hit) {
    u8* c;
    if (hit == NULL) {
        return FALSE;
    }
    if (hitGetAttr(hit) & 0x80000000) {
        if (strcmp(mobjHitObjPtrToPtr(hit)->animName, "MOBJ_BombRock") == 0) {
            return TRUE;
        }
    } else {
        c = caseCheckHitObj2(hit);
        if (c != NULL && *(s32*)(c + 4) == 15) {
            return TRUE;
        }
    }
    return FALSE;
}


#define BOMHEI_CLEAN_SFX(p)                    \
    if ((u32)(p)->work.bomhei.sfxId != 0xFFFFFFFFU) {             \
        psndSFXOff((p)->work.bomhei.sfxId);         \
        (p)->work.bomhei.sfxId = -1;                \
    }                                          \
    if ((u32)(p)->work.bomhei.sfxId2 != 0xFFFFFFFFU) {             \
        psndSFXOff((p)->work.bomhei.sfxId2);         \
        (p)->work.bomhei.sfxId2 = -1;                \
    }

#define BOMHEI_CLEAN_FLAGS(p)                  \
    (p)->flags2 &= ~(1 << 4);                  \
    (p)->flags &= ~(1 << 25);                  \
    (p)->flags &= ~(1U << 31);                 \
    (p)->flags &= ~(1 << 8);

#define BOMHEI_FREE(p)                         \
    if ((p)->useStruct != NULL) {         \
        __memFree(0, (p)->useStruct);     \
        (p)->useStruct = NULL;            \
    }

void bomhei_bye(PartyEntry* party) {
    if (party->trigFlags & 8) {
        party->trigFlags &= ~8;
        partyGoodbyeInit(party);
        party->playerPtr->flags &= ~(1 << 13);
        BOMHEI_CLEAN_SFX(party);
    }
    if (partyGoodbyeMain(party)) {
        partyChgRunMode(party, 14);
    }
}

void bomhei_init_800BFBB0(PartyEntry* party) {
    party->memberKind = 3;
    party->moveKind = 0;
    party->height = 28.0f;
    party->width = 28.0f;
    party->work.bomhei.state = 100;
    party->work.bomhei.reserved_17C = -1;
    party->work.bomhei.sfxId = -1;
    party->work.bomhei.sfxId2 = -1;
}

u8 bomheiGetStatus() {
    u32 partyId = marioGetPartyId();
    PartyEntry* party = partyGetPtr(partyId);
    u8 status = 2;

    if (party == NULL || (party->currentMemberId == PARTY_MEMBER_BOBBERY && (party->flags & PARTY_FLAG_IS_BEING_USED) == 0)) {
        status = 0;
        return status;
    } else if (party->useMotionId == 60) {
        status = 1;
    }
    return status;
}

void bomhei_exit(PartyEntry* party) {
    PartyEntry* p;
    party->playerPtr->flags &= ~(1 << 13);
    p = partyGetPtr(marioGetPartyId());
    if (p != NULL && p->currentMemberId == PARTY_MEMBER_BOBBERY && (p->flags & PARTY_FLAG_IS_BEING_USED)) {
        p->playerPtr->flags &= ~(1 << 13);
        if (p->runMode != 9) {
            partyChgRunMode(p, 2);
            p->moveMode = 0;
        }
        BOMHEI_CLEAN_FLAGS(p);
        BOMHEI_CLEAN_SFX(p);
        BOMHEI_FREE(p);
    }
}

void bomhei_use_cancel() {
    PartyEntry* p = partyGetPtr(marioGetPartyId());
    if (p != NULL && p->currentMemberId == PARTY_MEMBER_BOBBERY && (p->flags & PARTY_FLAG_IS_BEING_USED)) {
        p->playerPtr->flags &= ~(1 << 13);
        if (p->runMode != 9) {
            partyChgRunMode(p, 2);
            p->moveMode = 0;
        }
        BOMHEI_CLEAN_FLAGS(p);
        BOMHEI_CLEAN_SFX(p);
        BOMHEI_FREE(p);
    }
}

void bomhei_use_cancel2() {
    PartyEntry* p = partyGetPtr(marioGetPartyId());
    if (p != NULL && p->currentMemberId == PARTY_MEMBER_BOBBERY && (p->flags & PARTY_FLAG_IS_BEING_USED) && (p->playerPtr->flags & (1 << 13))) {
        p->playerPtr->flags &= ~(1 << 13);
        marioChgPose("M_S_1");
        if (p->runMode != 9) {
            partyChgRunMode(p, 2);
            p->moveMode = 0;
        }
        BOMHEI_CLEAN_FLAGS(p);
        BOMHEI_CLEAN_SFX(p);
        BOMHEI_FREE(p);
    }
}

void bomhei_use_post(PartyEntry* party) {
    MarioWork* player = party->playerPtr;
    const char* name;
    if (party->useMotionId >= 70 && party->useMotionId < 100) {
        partyGetAppearPos(party, &party->position);
    }
    if (player->flags & (1 << 13)) {
        player->flags &= ~(1 << 13);
        if (player->currMotionId == 1) {
            name = "M_W_1";
        } else if (player->currMotionId == 2) {
            name = "M_R_1";
        } else if (player->flags & (1 << 16)) {
            name = "M_J_1B";
        } else {
            name = "M_S_1";
        }
        marioChgPose(name);
    }
    BOMHEI_CLEAN_FLAGS(party);
    if (party->runMode != 9) {
        party->moveMode = 0;
        party->trigFlags |= 8;
    }
    party->playerPtr->flags &= ~(1 << 13);
    BOMHEI_CLEAN_SFX(party);
    BOMHEI_FREE(party);
}

void bomhei_use_main(PartyEntry* party);
void mario_controll(PartyEntry* party);
HitEntry* check_front(PartyEntry* party);

void bomhei_use(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    MapObjectEntry* mobj;

    if (party->flags & PARTY_FLAG_IS_BEING_USED) {
        bomhei_use_main(party);
        if ((party->useMotionId >= 30 && party->useMotionId <= 32) ||
            (party->useMotionId >= 50 && party->useMotionId <= 51) || party->useMotionId == 60) {
            partyChkGnd(party);
            if (party->flags & 2) {
                partyChgMot(party, 0);
                party->velY = 0.0f;
            }
            if (strcmp(gp->currentMapName, "muj_05") == 0 && party->hitobjStandOn != NULL &&
                strcmp(hitGetName(party->hitobjStandOn), "A_dokuro_bero") == 0) {
                party->targetDir = angleABf(party->position.x, party->position.z, 638.0f, 12.5f);
                party->moveDir = party->targetDir;
            }
        }
        if (party->work.bomhei.state != 0x66) {
            if (mario->currMotionId == 0x1F || mario->currMotionId == 0x20) {
                party->work.bomhei.state = 0x66;
                return;
            }
            mario_controll(party);
        }
        if (party->useMotionId <= 10 && mario->hitobjStandOn != NULL && (hitGetAttr(mario->hitobjStandOn) & 0x80000000)) {
            mobj = mobjHitObjPtrToPtr(mario->hitobjStandOn);
            if (strcmp(mobj->animName, "MOBJ_RedJumpStand") == 0 ||
                strcmp(mobj->animName, "MOBJ_RedJumpStand2") == 0 ||
                strcmp(mobj->animName, "MOBJ_BlueJumpStand") == 0 ||
                strcmp(mobj->animName, "MOBJ_BlueJumpStand2") == 0) {
                bomhei_use_cancel();
                return;
            }
        }
        if (!(party->flags & 0x80000000) && marioKeyOffChk()) {
            bomhei_use_cancel();
        } else if (mario->forceMoveTimer != 0) {
            bomhei_use_cancel();
        } else if (party->useMotionId <= 10 && (mario->dispFlags & 0x1000000)) {
            bomhei_use_cancel();
        }
    }
}

typedef struct BomheiWork {
    u32 flags;  // 0x0
    s32 timer;  // 0x4
} BomheiWork;

extern void partyChgPose(PartyEntry* party, const char* pose);
extern void marioSetSplash(s32 kind, Vec pos);

static inline f32 bomheiNeg(f32 x) {
    return -x;
}

static inline s16 bomheiToS16(f32 x) {
    return x;
}

#define BWORK(p) ((BomheiWork*)(p)->useStruct)

static inline BOOL bomheiChkHitAttr(PartyEntry* party) {
    HitEntry* hit;
    u32 attr;

    hit = party->hitobjFront;
    if (hit != NULL && (hitGetAttr(hit) & 0xC00)) {
        party->useMotionId = 60;
        return TRUE;
    }
    hit = party->hitobjHead;
    if (hit != NULL && (hitGetAttr(hit) & 0xC00)) {
        party->useMotionId = 60;
        return TRUE;
    }
    hit = party->hitobjStandOn;
    if (hit != NULL) {
        attr = hitGetAttr(hit);
        if (attr & 0xC00) {
            party->useMotionId = 60;
            return TRUE;
        }
        if (attr & 0x200) {
            party->useMotionId = 40;
            return TRUE;
        }
    }
    return FALSE;
}

static inline void bomheiSetHoldPos(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 height = 36.0f;

    if (strcmp(mario->animName, "M_W_7") == 0) {
        height += m_w_7_sync[party->work.bomhei.syncFrame] / 4.0f;
    }
    party->position.x = mario->position.x;
    party->position.y = mario->position.y + height;
    party->position.z = mario->position.z;
    movePos(&party->position.x, &party->position.z, -6.0f, toMovedirSimple(mario->displayDirectionTarget));
    party->position.z += 2.0f;
    party->displayDirectionTarget = mario->displayDirectionTarget;
    party->displayDirection = mario->displayDirection;
}

void bomhei_use_main(PartyEntry* party) {
    MarioWork* mario = party->playerPtr;
    f32 x, z;
    f32 tmp;

    if (party->trigFlags & 8) {
        party->trigFlags &= ~8;
        party->flags &= ~0x70;
        party->flags &= ~0x3000000;
        party->flags &= ~0x4000000;
        party->useStruct = __memAlloc(HEAP_DEFAULT, sizeof(BomheiWork));
        memset(party->useStruct, 0, sizeof(BomheiWork));
        party->useMotionId = 0;
    }

    switch (party->useMotionId) {
    case 0:
        party->useMotionId = 1;
        party->work.bomhei.state = 100;
        party->work.bomhei.timer = 0;
        party->moveFrames = 4;
        BWORK(party)->timer = 0;
        BWORK(party)->flags = 0;
        partyChgPose(party, "R_1");
    case 1:
        party->moveTarget = mario->position;
        movePos(&party->moveTarget.x, &party->moveTarget.z, -6.0f, toMovedirSimple(mario->displayDirectionTarget));
        party->position.x += (party->moveTarget.x - party->position.x) / party->moveFrames;
        party->position.y += (party->moveTarget.y - party->position.y) / party->moveFrames;
        party->position.z += (party->moveTarget.z - party->position.z) / party->moveFrames;
        if (--party->moveFrames > 0) {
            break;
        }
        party->moveFrames = 9;
        x = mario->position.x;
        z = mario->position.z;
        movePos(&x, &z, -6.0f, toMovedirSimple(mario->displayDirectionTarget));
        party->moveSpeed = distABf(party->position.x, party->position.z, x, z) / 18.0f;
        party->targetDir = angleABf(party->position.x, party->position.z, x, z);
        party->moveDir = party->targetDir;
        party->velY = 7.5f;
        tmp = 0.8333333f;
        party->accelY = -tmp;
        partyChgPose(party, "J_1A");
        party->useMotionId = 2;
        party->work.bomhei.state = 10;
        break;
    case 2:
        party->position.y += party->velY;
        party->velY += party->accelY;
        if (--party->moveFrames > 0) {
            break;
        }
        party->flags2 |= 0x10;
        partyChgPose(party, "L_1");
        BWORK(party)->timer = (s16)sysMsec2Frame(3000);
        party->useMotionId = 10;
        party->work.bomhei.state = 20;
        break;
    case 10:
        if (BWORK(party)->flags & 2) {
            partyChgPose(party, "L_2");
        }
        bomheiSetHoldPos(party);
        bomheiChkHitAttr(party);
        if ((party->flags & 0x80000000) ||
            (mario->hitobjStandOn != NULL && mario->airTimer == 0 && !(mario->buttonHold & 0x400))) {
            party->useMotionId = 20;
        }
        break;
    }

    switch (party->useMotionId) {
    case 20:
        psndSFXOn_3D((const char*)0x939, &party->playerPtr->position);
        party->work.bomhei.state = 30;
        party->useMotionId = 21;
        x = party->position.x;
        z = party->position.z;
        movePos(&x, &z, 38.0f, toMovedirSimple(mario->displayDirectionTarget));
        party->moveSpeed = distABf(party->position.x, party->position.z, x, z) / 20.0f;
        party->targetDir = angleABf(party->position.x, party->position.z, x, z);
        party->moveDir = party->targetDir;
        party->velY = 3.5555556f;
        tmp = 0.35555556f;
        party->accelY = -tmp;
        tmp = 10.5f;
        party->moveFrames = (s16)tmp;
        party->flags2 &= ~0x10;
        party->flags |= 0x10;
        bomheiChkHitAttr(party);
        break;
    case 21:
        partyMoveNoHosei(party);
        if (--party->moveFrames > 0) {
            bomheiChkHitAttr(party);
            break;
        }
        party->velY = 0.0f;
        party->accelY = -0.33333334f;
        tmp = 10.5f;
        party->moveFrames = (s16)tmp;
        party->flags &= ~0x10;
        if (party->hitobjStandOn != NULL) {
            party->useMotionId = 30;
        } else {
            party->useMotionId = 22;
        }
        bomheiChkHitAttr(party);
        break;
    case 22:
        partyMoveNoHosei(party);
        if (party->hitobjStandOn != NULL && !bomheiChkHitAttr(party)) {
            party->useMotionId = !(BWORK(party)->flags & 4) ? 30 : 50;
        }
        break;
    case 30:
        party->moveFrames = sysMsec2Frame(200);
        party->useMotionId = 31;
    case 31:
        if (!(party->flags & 0x80000000) && (gp->buttonNew[0] & 0x400)) {
            BWORK(party)->timer = (s16)(sysMsec2Frame(500) + 1);
            party->useMotionId = 50;
        }
        partyChgPose(party, "S_1");
        if (--party->moveFrames > 0) {
            bomheiChkHitAttr(party);
            break;
        }
        party->useMotionId = 32;
    case 32:
        if (!(party->flags & 0x80000000) && (gp->buttonNew[0] & 0x400)) {
            BWORK(party)->timer = (s16)(sysMsec2Frame(500) + 1);
        }
        partyChgPose(party, !(BWORK(party)->flags & 2) ? "W_1" : "W_2");
        party->moveSpeed = 0.6f;
        partyMove(party);
        if (BWORK(party)->timer <= 0) {
            party->useMotionId = 50;
        }
        bomheiChkHitAttr(party);
        break;
    }

    if (party->useMotionId < 50 && BWORK(party)->timer > 0) {
        BWORK(party)->timer--;
        if (BWORK(party)->timer == (s16)sysMsec2Frame(2500)) {
            BWORK(party)->flags |= 2;
            if ((u32)party->work.bomhei.sfxId2 == 0xFFFFFFFFU) {
                party->work.bomhei.sfxId = psndSFXOn_3D((const char*)0x93C, &party->playerPtr->position);
            }
        } else if (BWORK(party)->timer == (s16)sysMsec2Frame(500)) {
            party->useMotionId = 50;
            if (mario->flags & 0x2000) {
                party->work.bomhei.state = 40;
            } else {
                party->useMotionId = 50;
            }
        }
    }

    switch (party->useMotionId) {
    case 40:
        if ((u32)party->work.bomhei.sfxId != 0xFFFFFFFFU) {
            psndSFXOff(party->work.bomhei.sfxId);
            party->work.bomhei.sfxId = -1;
        }
        marioSetSplash(0, party->position);
        BWORK(party)->flags |= 8;
        BWORK(party)->timer = (s16)(sysMsec2Frame(500) + 30);
        BWORK(party)->flags |= 2;
        party->flags |= 0x2000000;
        party->useMotionId = 41;
        party->moveFrames = 6;
        break;
    case 41:
        party->position.y += party->velY;
        party->velY += party->accelY;
        if (--party->moveFrames <= 0) {
            party->useMotionId = 42;
            party->velY = 0.0f;
            party->accelY = 0.0f;
        }
    case 42:
        tmp = 0.1f * (-1.0f - party->moveSpeed);
        party->moveSpeed += tmp;
        if (party->moveSpeed <= 0.0f) {
            party->moveSpeed = 0.0f;
        }
        break;
    }

    switch (party->useMotionId) {
    case 50:
        if ((u32)party->work.bomhei.sfxId != 0xFFFFFFFFU) {
            psndSFXOff(party->work.bomhei.sfxId);
            party->work.bomhei.sfxId = -1;
        }
        if ((u32)party->work.bomhei.sfxId2 == 0xFFFFFFFFU) {
            party->work.bomhei.sfxId2 = psndSFXOn_3D((const char*)0x93D, &party->playerPtr->position);
        }
        partyChgPose(party, "A_3");
        party->useMotionId = 51;
        party->moveFrames = 40;
    case 51:
        if (!(mario->flags & 0x2000)) {
            if (party->flags & 2) {
                party->moveSpeed = 0.0f;
            }
            partyMove(party);
        } else {
            bomheiSetHoldPos(party);
        }
        if (--party->moveFrames > 0) {
            bomheiChkHitAttr(party);
            break;
        }
        if ((u32)party->work.bomhei.sfxId2 != 0xFFFFFFFFU) {
            psndSFXOff(party->work.bomhei.sfxId2);
            party->work.bomhei.sfxId2 = -1;
        }
        BWORK(party)->flags &= ~2;
        party->hitobjAction = check_front(party);
        if (mario->flags & 0x2000) {
            mario->flags &= ~0x2000;
        }
        party->flags &= ~0x20;
        BWORK(party)->flags &= ~8;
        party->useMotionId = 60;
        break;
    }

    switch (party->useMotionId) {
    case 60: {
        EffectEntry* eff;
        BOMHEI_CLEAN_SFX(party);
        psndSFXOn_3D((const char*)0x93B, &party->position);
        BWORK(party)->timer = 0;
        party->moveSpeed = 0.0f;
        partyMove(party);
        if (isBombRock(party->hitobjAction)) {
            evtEntryType(longQuake_evt, 0, 0, 0);
        } else {
            evtEntryType(quake_evt, 0, 0, 0);
        }
        eff = effSandarsEntry(pouchGetPartyAttackLv((PartyId)3), party->position.x,
                              party->position.y + party->height, party->position.z, 0.0f);
        ((EffSandarsWork*)eff->userdata)->camId = party->camId;
        fbatSandersBombTriggerOn();
        party->useMotionId = 70;
        break;
    }
    case 70:
        party->moveSpeed = 0.0f;
        party->velY = 8.0f;
        party->accelY = 0.0f;
        party->moveFrames = 60;
        party->useMotionId = 71;
    case 71: {
        f32 sx, sy, sz;
        party->position.y += party->velY;
        marioGetScreenPos(party->position, &sx, &sy, &sz);
        if (sy <= -300.0f) {
            party->moveFrames = sysMsec2Frame(800);
            party->useMotionId = 80;
        }
        break;
    }
    case 80:
        if (--party->moveFrames <= 0) {
            Vec pos;
            partyGetAppearPos(party, &pos);
            party->position.x = pos.x;
            party->position.y = 200.0f + pos.y;
            party->position.z = pos.z;
            party->velY = 0.0f;
            party->accelY = -1.0f;
            party->moveSpeed = 0.0f;
            party->flags &= ~0x10;
            party->flags |= 0x20;
            party->flags |= 0x2000000;
            partyChgPose(party, "J_1B");
            party->useMotionId = 81;
        }
        break;
    case 81: {
        HitEntry* hit;
        f32 limit;
        f32 sx, sy, sz;
        if (37.0f + mario->position.y > party->position.y) {
            party->flags &= ~0x2000000;
        }
        hit = partySearchGround(party, 11.0f, party->velY);
        limit = mario->screenY - 25.0f;
        marioGetScreenPos(party->position, &sx, &sy, &sz);
        if (sy > limit) {
            party->flags &= ~0x2000000;
        } else if (hit != NULL) {
            party->hitobjStandOn = hit;
            party->flags &= ~0x2000000;
        }
        partyMove(party);
        if (party->velY <= -6.0f) {
            party->accelY = 0.0f;
            party->velY = -6.0f;
        }
        if (!(party->flags & 0x2000000)) {
            if (party->position.y < -2000.0f) {
                party->position = mario->position;
                party->position.y += 200.0f;
                party->flags &= ~0x70;
            }
            if (party->hitobjStandOn != NULL && !((hitGetAttr(party->hitobjStandOn) >> 9) & 1)) {
                partyChgPose(party, "F_1");
                party->useMotionId = 82;
                party->moveFrames = 30;
                psndSFXOn_3D((const char*)0x93A, &party->position);
            }
        }
        break;
    }
    case 82:
        if (--party->moveFrames <= 0) {
            party->useMotionId = 100;
        }
        partyMove(party);
        break;
    case 100:
        partyChgRunMode(party, 2);
        party->moveMode = 0;
        party->flags2 &= ~0x10;
        party->flags &= ~0x80000000;
        party->flags &= ~0x100;
        BOMHEI_FREE(party);
        break;
    }
}

s32 mario_bomhei_keychk() {
    MarioWork* mario = marioGetPtr();
    PartyEntry* p = partyGetPtr(marioGetPartyId());
    u8 m;
    if (p == NULL) return 1;
    if (p->currentMemberId != PARTY_MEMBER_BOBBERY) return 1;
    if ((p->flags & PARTY_FLAG_IS_BEING_USED) == 0) return 1;
    m = p->work.bomhei.state;
    if (m == 10 || (m >= 20 && m <= 21) || m == 101 || (m >= 40 && m <= 41) || (m >= 30 && m <= 36)) return 0;
    m = p->useMotionId;
    if ((m >= 50 && m <= 51) || m == 60 || (m >= 70 && m <= 71)) return 0;
    return 1;
}

s32 mario_bomhei_keychk2() {
    MarioWork* mario = marioGetPtr();
    PartyEntry* p = partyGetPtr(marioGetPartyId());
    u8 m;
    if (p == NULL) return 1;
    if (p->currentMemberId != PARTY_MEMBER_BOBBERY) return 1;
    if ((p->flags & PARTY_FLAG_IS_BEING_USED) == 0) return 1;
    m = p->work.bomhei.state;
    if (m == 10 || m == 101 || (m >= 30 && m <= 36)) return 0;
    return 1;
}

void mario_controll(PartyEntry* party) {
    MarioWork* player = party->playerPtr;
    s32 cnt;

    switch (party->work.bomhei.state) {
    case 10:
        marioAdjustMoveDir();
        marioChgPose("M_U_2");
        player->flags |= 0x2000;
        party->work.bomhei.state = 101;
        break;
    case 20:
        player->flags |= 0x2000;
        party->work.bomhei.state = 21;
        break;
    case 21:
        if (strcmp(player->animName, "M_W_7") == 0) {
            animPoseSetLocalTime(player->paperAnimGroupId[marioAnimeId()], party->work.bomhei.syncFrame);
            cnt = party->work.bomhei.syncFrame + 1;
            party->work.bomhei.syncFrame = cnt;
            if ((u8)cnt >= 40) {
                party->work.bomhei.syncFrame = 0;
            }
        } else {
            party->work.bomhei.syncFrame = 0;
        }
        break;
    case 40:
        marioChgMot(0);
        party->work.bomhei.state = 41;
        party->work.bomhei.timer = 40;
        break;
    case 41:
        if (--party->work.bomhei.timer <= 0) {
            marioChgMot(0);
            party->work.bomhei.state = 100;
        }
        break;
    case 30:
        marioChgMot(0);
        party->work.bomhei.state = 31;
        break;
    case 31:
        marioChgPose("M_C_1");
        player->flags &= ~0x2000;
        party->work.bomhei.timer = 50;
        party->work.bomhei.state = 32;
        break;
    case 32:
        if (--party->work.bomhei.timer <= 0) {
            party->work.bomhei.state = 36;
            party->work.bomhei.timer = 10;
        }
        break;
    case 36:
        animPoseSetLocalTime(player->paperAnimGroupId[marioAnimeId()], party->work.bomhei.timer);
        if (--party->work.bomhei.timer < 0) {
            marioChgMot(0);
            party->work.bomhei.state = 100;
        }
        break;
    }
}

static inline BOOL bomheiChkReturnGround(Vec* pos, f32 dist, f32 angle) {
    HitCheckQuery query;
    HitEntry* hit;
    f32 x, z;
    f32 rf;

    marioGetPtr();
    x = pos->x;
    z = pos->z;
    movePos(&x, &z, dist, angle);
    query.targetDistance = 4.0f;
    {
        Vec start = {0.0f, 0.0f, 0.0f};
        Vec dir = {0.0f, -1.0f, 0.0f};
        start.x = x;
        start.y = 2.0f + pos->y;
        start.z = z;
        query.targetPos = start;
        query.targetDir = dir;
    }
    hit = hitCheckVecFilter(&query, NULL);
    if (hit != NULL && !(hitGetAttr(hit) & 0xE00)) {
    if (query.hitPos.x >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    pos->x = (s32)((1000.0f * query.hitPos.x) + rf) / 1000.0f;
    if (query.hitPos.y >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    pos->y = (s32)((1000.0f * query.hitPos.y) + rf) / 1000.0f;
    if (query.hitPos.z >= 0.0f) {
        rf = 0.5f;
    } else {
        rf = -0.5f;
    }
    pos->z = (s32)((1000.0f * query.hitPos.z) + rf) / 1000.0f;
    return TRUE;
    }
    return FALSE;
}

void partyGetReturnPos(Vec* out) {
    MarioWork* mario = marioGetPtr();
    f32 x, z, s, c;
    f32 hx, hy, hz, dist, nx, ny, nz;
    f32 y;
    f32 angle;
    Vec p;

    x = mario->position.x;
    y = 2.0f + mario->position.y;
    z = mario->position.z;
    dist = mario->wPlayerCollisionBox.x;
    angle = mario->targetYaw;
    sincosf(&s, &c, angle);
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, s, 0.0f, c) == NULL) {
        p = mario->position;
        if (bomheiChkReturnGround(&p, 6.0f, angle)) {
            out->x = p.x;
            out->y = p.y;
            out->z = p.z;
            return;
        }
    }

    x = mario->position.x;
    y = 2.0f + mario->position.y;
    z = mario->position.z;
    dist = mario->wPlayerCollisionBox.x;
    angle = revise360(toMovedirSimple(mario->displayDirectionTarget) - 180.0f);
    sincosf(&s, &c, angle);
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, s, 0.0f, c) == NULL) {
        p = mario->position;
        if (bomheiChkReturnGround(&p, mario->wPlayerCollisionBox.x, angle)) {
            out->x = p.x;
            out->y = p.y;
            out->z = p.z;
            return;
        }
    }

    angle = toMovedirSimple(mario->displayDirectionTarget);
    sincosf(&s, &c, angle);
    if (hitCheckFilter(NULL, &hx, &hy, &hz, &dist, &nx, &ny, &nz, x, y, z, s, 0.0f, c) == NULL) {
        p = mario->position;
        if (bomheiChkReturnGround(&p, mario->wPlayerCollisionBox.x, angle)) {
            out->x = p.x;
            out->y = p.y;
            out->z = p.z;
            return;
        }
    }

    x = mario->position.x;
    z = mario->position.z;
    movePos(&x, &z, mario->wPlayerCollisionBox.x, revise360(180.0f + mario->targetYaw));
    out->x = x;
    out->y = mario->position.y;
    out->z = z;
}

HitEntry* check_front(PartyEntry* party) {
    const s32 angles[8] = {0, 45, -45, 90, -90, 135, -135, 180};
    static const Vec down = {0.0f, -1.0f, 0.0f};
    HitCheckQuery q;
    f32 height;
    HitEntry* hit;
    HitEntry* found = NULL;
    HitEntry* last = NULL;
    f32 movedir;
    f32 ang;
    f32 lim;
    u32 i;

    height = 0.8f * party->height;
    movedir = partyToMovedir(party, party->displayDirection);
    for (i = 0; i < 8; i++) {
        sincosf(&q.targetDir.x, &q.targetDir.z, movedir + (f32)angles[i]);
        q.targetDir.y = 0.0f;
        q.targetPos = party->position;
        q.targetPos.y = q.targetPos.y + height;
        q.targetDistance = 100.0f;
        hit = hitCheckVecFilter(&q, NULL);
        if (hit != NULL) {
            ang = revise360(movedir + (f32)angles[i] - camGetPtr(CAMERA_3D)->viewYaw);
            if ((ang >= 60.0f && ang <= 120.0f) || (ang >= 240.0f && ang <= 300.0f)) {
                lim = 50.0f;
            } else {
                lim = 30.0f;
            }
            if (q.targetDistance <= lim && isBombRock(hit)) {
                return hit;
            }
            found = NULL;
        }
    }

    q.targetPos = party->position;
    q.targetDir = down;
    q.targetDistance = 2.0f + height;
    hit = hitCheckVecFilter(&q, NULL);
    if (hit != NULL) {
        if (isBombRock(hit)) {
            return hit;
        }
        last = hit;
    }

    hit = party->hitobjStandOn;
    if (hit != NULL && isBombRock(hit)) {
        return hit;
    }
    if (found != NULL) {
        return found;
    }
    if (last != NULL) {
        return last;
    }
    return hit;
}
