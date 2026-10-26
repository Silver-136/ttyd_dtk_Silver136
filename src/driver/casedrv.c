#include "driver/casedrv.h"
#include "driver/hitdrv.h"
#include "driver/npcdrv.h"
#include "driver/swdrv.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include "manager/evtmgr.h"
#include "party/party_yoshi.h"
#include "mario/mario.h"
#include "mario/mario_party.h"
#include "mario/mariost.h"
#include "memory.h"
#include "party.h"
#include "system.h"
#include "win/win_main.h"
#include <string.h>

extern void* fbatNpcTalkCheck(void);

typedef struct CaseWork {
    s32 count; //0x0
    CaseEntry* entries; //0x4
} CaseWork;

//.sbss
static CaseWork work;
static CaseWork* wp = &work;


void caseInit(void) {
    wp->count = 64;
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(CaseEntry));
    memset(wp->entries, 0, wp->count * sizeof(CaseEntry));
}

void caseReInit(void) {
    CaseEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        entry->flags &= ~1;
    }
}

s32 caseEntry(CaseSetup* setup) {
    CaseEntry* entry;
    int i = 0;
    for (entry = wp->entries; i < wp->count; i++, entry++) {
        if (!(entry->flags & 1)) {
            break; //unused entry
        }
    }
    memset(entry, 0, sizeof(CaseEntry));
    entry->caseId = i;
    entry->flags = 0;
    entry->conditionId = setup->conditionId & ~0x8000;
    if (setup->conditionId & 0x8000) {
        entry->flags |= 4u;
    }
    entry->flags |= 1u;
    strcpy(entry->hitObjName, setup->hitObjName);
    entry->swFlag = setup->swFlag;
    entry->activeFunc = setup->activeFunc;
    entry->evtCode = setup->evtCode;
    entry->priority = setup->priority;
    entry->lwData[16] = 0;
    memcpy(entry->lwData, setup->lwData, sizeof(entry->lwData));
    return i & 0xFFFFFFFFFFFFFFFFu;
}

void caseDelete(s32 caseId) {
    wp->entries[caseId].flags &= ~1u;
}

#define SET_HIT(entry) (entry)->flags |= 2

void caseMain(void) {
    MarioWork* mario;
    PartyEntry* party;
    s32 flag100;
    CaseEntry* entry;
    s32 i;
    u16 btn;
    HitEntry* standOn;
    HitEntry* h1f8;
    HitEntry* jumpFrom;
    HitEntry* h1e0;
    HitEntry* push;
    HitEntry* h1fc;
    HitEntry* h1f4;

    mario = marioGetPtr();
    partyGetPtr(marioGetPartyId());
    standOn = mario->hitobjStandOn;
    h1f8 = mario->hitobjUnder;
    jumpFrom = mario->hitobjJumpFrom;
    h1e0 = mario->hitobjFront;
    push = mario->hitobjPush;
    h1fc = mario->hitobjHammer;
    h1f4 = mario->hitobjHead;
    btn = mario->buttonTrg;
    if (marioStGetSystemLevel() == 0 && !winCheck() && gp->inBattle == 0 &&
        !N_fbatPreventMarioEventChk() && marioCaseEventValidChk()) {
        if (mario->currMotionId != MARIO_MOTION_STAY && mario->currMotionId != MARIO_MOTION_WALK && mario->currMotionId != MARIO_MOTION_DASH &&
            yoshiGetStatus() != 1) {
            btn = 0;
        }
        flag100 = btn & PAD_BUTTON_A;
        entry = wp->entries;
        for (i = 0; i < wp->count; i++, entry++) {
            if (!(entry->flags & 1)) continue;
            switch (entry->conditionId) {
            case CASE_STAND_ON:
                if (standOn != NULL && !strcmp(hitGetName(standOn), entry->hitObjName)) SET_HIT(entry);
                break;
            case CASE_NOT_STAND_ON:
                if (standOn != NULL) {
                    if (strcmp(hitGetName(standOn), entry->hitObjName) != 0) SET_HIT(entry);
                } else {
                    SET_HIT(entry);
                }
                break;
            case CASE_UNDER:
                if (h1f8 != NULL && !strcmp(hitGetName(h1f8), entry->hitObjName)) SET_HIT(entry);
                break;
            case CASE_NOT_UNDER:
                if (h1f8 != NULL) {
                    if (strcmp(hitGetName(h1f8), entry->hitObjName) != 0) SET_HIT(entry);
                } else {
                    SET_HIT(entry);
                }
                break;
            case CASE_FRONT:
                if (h1e0 != NULL && !strcmp(hitGetName(h1e0), entry->hitObjName)) SET_HIT(entry);
                break;
            case CASE_JUMP_FROM_A:
                if (jumpFrom != NULL && mario->airTimer == 1 && fbatNpcTalkCheck() == NULL &&
                    !strcmp(hitGetName(jumpFrom), entry->hitObjName) && (mario->buttonTrg & PAD_BUTTON_A)) {
                    SET_HIT(entry);
                }
                break;
            case CASE_STAND_ON_A:
                if (standOn != NULL && fbatNpcTalkCheck() == NULL &&
                    !strcmp(hitGetName(standOn), entry->hitObjName) && flag100) {
                    SET_HIT(entry);
                }
                break;
            case CASE_STAND_ON_STICK:
                if (standOn != NULL && !strcmp(hitGetName(standOn), entry->hitObjName) &&
                    (s8)mario->wStickDir2 > 30) {
                    SET_HIT(entry);
                }
                break;
            case CASE_STAND_ON_STICK_UP:
                if (standOn != NULL && !strcmp(hitGetName(standOn), entry->hitObjName) &&
                    (keyGetDirTrg(0) & DEMO_STICK_UP)) {
                    SET_HIT(entry);
                }
                break;
            case CASE_FRONT_A:
                if (h1e0 != NULL && fbatNpcTalkCheck() == NULL &&
                    !strcmp(hitGetName(h1e0), entry->hitObjName) && flag100) {
                    SET_HIT(entry);
                }
                break;
            case CASE_PUSH:
                if (push != NULL && !strcmp(hitGetName(push), entry->hitObjName)) SET_HIT(entry);
                break;
            case CASE_NOT_PUSH:
                if (push == NULL || strcmp(hitGetName(push), entry->hitObjName) != 0) SET_HIT(entry);
                break;
            case CASE_PUSH_A:
                if (push != NULL && fbatNpcTalkCheck() == NULL &&
                    !strcmp(hitGetName(push), entry->hitObjName) && flag100) {
                    SET_HIT(entry);
                }
                break;
            case CASE_HAMMER:
                if (h1fc != NULL && !strcmp(hitGetName(h1fc), entry->hitObjName)) SET_HIT(entry);
                break;
            case CASE_HAMMER_2:
                if (h1fc != NULL && mario->currMotionId == MARIO_MOTION_HAMMER_2 &&
                    !strcmp(hitGetName(h1fc), entry->hitObjName)) {
                    SET_HIT(entry);
                }
                break;
            case CASE_BOBBERY:
                party = partyGetPtr(marioGetPartyId());
                if (party != NULL && party->currentMemberId == PARTY_MEMBER_BOBBERY) {
                    HitEntry* ph = party->hitobjAction;
                    if (ph != NULL && !strcmp(hitGetName(ph), entry->hitObjName)) SET_HIT(entry);
                }
                break;
            case CASE_HEAD:
                if (h1f4 != NULL && !strcmp(hitGetName(h1f4), entry->hitObjName)) SET_HIT(entry);
                break;
            case CASE_GSWF:
                if (swGet(entry->swFlag)) SET_HIT(entry);
                break;
            case CASE_LSWF:
                if (_swGet(entry->swFlag)) SET_HIT(entry);
                break;
            case CASE_FUNC:
                if (entry->activeFunc != NULL && ((s32 (*)(CaseEntry*))entry->activeFunc)(entry)) {
                    SET_HIT(entry);
                }
                break;
            }
        }
        entry = wp->entries;
        for (i = 0; i < wp->count; i++, entry++) {
            if ((entry->flags & 1) && (entry->flags & 2)) {
                EventEntry* evt;
                if (!evtCheckID(entry->evtId)) entry->evtId = 0;
                if (entry->evtId == 0) {
                    evt = evtEntry(entry->evtCode, entry->priority, EVENT_START_IMMEDIATE);
                    evt->caseId = entry->caseId;
                    entry->evtId = evt->eventId;
                    memcpy(evt->lwData, entry->lwData, sizeof(entry->lwData));
                    if (entry->priority == 0x14) npcStopForEvent();
                }
                entry->flags &= ~2;
            }
        }
    }
}

CaseEntry* caseCheckHitObj(void* obj) {
    CaseEntry* entry;
    int i;
    if (gp->inBattle != 0) return NULL;
    entry = wp->entries;
    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & 1) && entry->conditionId != CASE_FUNC &&
            (u32)(entry->conditionId - CASE_GSWF) > 1) {
            if (!strcmp(hitGetName(obj), entry->hitObjName)) return entry;
        }
    }
    return NULL;
}

CaseEntry* caseCheckHitObj2(void* obj) {
    CaseEntry* entry;
    int i;
    if (gp->inBattle != 0) return NULL;
    entry = wp->entries;
    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & 1) && entry->conditionId != CASE_FUNC &&
            (u32)(entry->conditionId - CASE_GSWF) > 1 && entry->conditionId == CASE_BOBBERY) {
            if (!strcmp(hitGetName(obj), entry->hitObjName)) return entry;
        }
    }
    return NULL;
}

CaseEntry* caseIdToPtr(s32 caseId) {
    CaseEntry* entry = &wp->entries[caseId];
    if (!(entry->flags & 1)) return NULL;
    return entry;
}
