// Case driver: "cases" run an event script when Mario touches, jumps on, stands on or hammers a
// named hit object (or when a custom condition function says so).

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
//.sdata
static CaseWork* wp = &work;


/** Allocates and clears the table of 64 case entries at boot. */
void caseInit(void) {
    wp->count = 64;
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(CaseEntry));
    memset(wp->entries, 0, wp->count * sizeof(CaseEntry));
}

/** Releases every case entry, e.g. on a map change; the table stays allocated. */
void caseReInit(void) {
    CaseEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        entry->flags &= ~CASE_FLAG_ACTIVE;
    }
}

/**
 * Registers a case from a CaseSetup: the event script to start, its priority,
 * the hit object name and condition (or a save flag or a check function) that fires it.
 * Returns the case id the scripts use to delete it. Called by the evt_case helpers.
 */
s32 caseEntry(CaseSetup* setup) {
    CaseEntry* entry;
    int i = 0;
    for (entry = wp->entries; i < wp->count; i++, entry++) {
        if (!(entry->flags & CASE_FLAG_ACTIVE)) {
            break; //unused entry
        }
    }
    memset(entry, 0, sizeof(CaseEntry));
    entry->caseId = i;
    entry->flags = 0;
    entry->conditionId = setup->conditionId & ~CASE_SETUP_SHOW_HINT;
    if (setup->conditionId & CASE_SETUP_SHOW_HINT) {
        entry->flags |= CASE_FLAG_SHOW_HINT;
    }
    entry->flags |= CASE_FLAG_ACTIVE;
    strcpy(entry->hitObjName, setup->hitObjName);
    entry->swFlag = setup->swFlag;
    entry->activeFunc = setup->activeFunc;
    entry->evtCode = setup->evtCode;
    entry->priority = setup->priority;
    entry->evtId = 0;
    memcpy(entry->lwData, setup->lwData, sizeof(entry->lwData));
    // FAKE: no-op 64-bit mask; a plain `return i;` swaps the loop registers in both versions
    return i & 0xFFFFFFFFFFFFFFFFu;
}

/** Releases case entry caseId so it no longer fires. */
void caseDelete(s32 caseId) {
    wp->entries[caseId].flags &= ~CASE_FLAG_ACTIVE;
}

void caseMain(void) {
    MarioWork* mario;
    PartyEntry* party;
    s32 pressedA;
    CaseEntry* entry;
    s32 i;
    u16 btn;
    HitEntry* standOn;
    HitEntry* under;
    HitEntry* jumpFrom;
    HitEntry* front;
    HitEntry* push;
    HitEntry* hammer;
    HitEntry* head;
#if VERSION <= VERSION_DEMO_US
s32 fbatMode;
#endif
    mario = marioGetPtr();
    partyGetPtr(marioGetPartyId());
    standOn = mario->hitobjStandOn;
    under = mario->hitobjUnder;
    jumpFrom = mario->hitobjJumpFrom;
    front = mario->hitobjFront;
    push = mario->hitobjPush;
    hammer = mario->hitobjHammer;
    head = mario->hitobjHead;
    btn = mario->buttonTrg;
#if VERSION <= VERSION_DEMO_US
if ((u32)marioStGetSystemLevel() == 0 && !winCheck() && gp->inBattle == 0 &&
        ((fbatMode = fbatGetPointer()->mode) == 0 || fbatMode == 1) && marioCaseEventValidChk()) {
#else
    if (marioStGetSystemLevel() == 0 && !winCheck() && gp->inBattle == 0 &&
        !fbatPreventMarioEventChk() && marioCaseEventValidChk()) {
#endif
        if (mario->currMotionId != MARIO_MOTION_STAY && mario->currMotionId != MARIO_MOTION_WALK && mario->currMotionId != MARIO_MOTION_DASH &&
            yoshiGetStatus() != YOSHI_STATUS_RIDING) {
            btn = 0;
        }
        pressedA = btn & PAD_BUTTON_A;
        entry = wp->entries;
        for (i = 0; i < wp->count; i++, entry++) {
#if VERSION <= VERSION_DEMO_US
            if (!(entry->flags & CASE_FLAG_ACTIVE))
                continue;
#else
            if (!(entry->flags & CASE_FLAG_ACTIVE)) continue;
#endif
            switch (entry->conditionId) {
            case CASE_STAND_ON:
#if VERSION <= VERSION_DEMO_US
                    if (standOn != NULL && !strcmp(hitGetName(standOn), entry->hitObjName))
                        SET_HIT(entry);
#else
                if (standOn != NULL && !strcmp(hitGetName(standOn), entry->hitObjName)) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
                }
#endif
                break;
            case CASE_NOT_STAND_ON:
                if (standOn != NULL) {
#if VERSION <= VERSION_DEMO_US
                        if (strcmp(hitGetName(standOn), entry->hitObjName) != 0)
                            SET_HIT(entry);
#else
                    if (strcmp(hitGetName(standOn), entry->hitObjName) != 0) {
                        entry->flags |= CASE_FLAG_TRIGGERED;
                    }
#endif
                } else {
#if VERSION <= VERSION_DEMO_US
                        SET_HIT(entry);
#else
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            case CASE_UNDER:
#if VERSION <= VERSION_DEMO_US
                    if (under != NULL && !strcmp(hitGetName(under), entry->hitObjName))
                        SET_HIT(entry);
#else
                if (under != NULL && !strcmp(hitGetName(under), entry->hitObjName)) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
                }
#endif
                break;
            case CASE_NOT_UNDER:
                if (under != NULL) {
#if VERSION <= VERSION_DEMO_US
                        if (strcmp(hitGetName(under), entry->hitObjName) != 0)
                            SET_HIT(entry);
#else
                    if (strcmp(hitGetName(under), entry->hitObjName) != 0) {
                        entry->flags |= CASE_FLAG_TRIGGERED;
                    }
#endif
                } else {
#if VERSION <= VERSION_DEMO_US
                        SET_HIT(entry);
#else
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            case CASE_FRONT:
#if VERSION <= VERSION_DEMO_US
                    if (front != NULL && !strcmp(hitGetName(front), entry->hitObjName))
                        SET_HIT(entry);
#else
                if (front != NULL && !strcmp(hitGetName(front), entry->hitObjName)) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
                }
#endif
                break;
            case CASE_JUMP_FROM_A:
                if (jumpFrom != NULL && mario->airTimer == 1 && fbatNpcTalkCheck() == NULL &&
                    !strcmp(hitGetName(jumpFrom), entry->hitObjName) && (mario->buttonTrg & PAD_BUTTON_A)) {
#if VERSION <= VERSION_DEMO_US
                        SET_HIT(entry);
#else
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            case CASE_STAND_ON_A:
                if (standOn != NULL && fbatNpcTalkCheck() == NULL &&
                    !strcmp(hitGetName(standOn), entry->hitObjName) && pressedA) {
#if VERSION <= VERSION_DEMO_US
                        SET_HIT(entry);
#else
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            case CASE_STAND_ON_STICK:
                if (standOn != NULL && !strcmp(hitGetName(standOn), entry->hitObjName) &&
#if VERSION <= VERSION_DEMO_US
                        (s8)mario->wStickDir2 > 30) {
                        SET_HIT(entry);
#else
                    mario->wStickDir2 > 30) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            case CASE_STAND_ON_STICK_UP:
                if (standOn != NULL && !strcmp(hitGetName(standOn), entry->hitObjName) &&
                    (keyGetDirTrg(0) & DEMO_STICK_UP)) {
#if VERSION <= VERSION_DEMO_US
                        SET_HIT(entry);
#else
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            case CASE_FRONT_A:
                if (front != NULL && fbatNpcTalkCheck() == NULL &&
                    !strcmp(hitGetName(front), entry->hitObjName) && pressedA) {
#if VERSION <= VERSION_DEMO_US
                        SET_HIT(entry);
#else
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            case CASE_PUSH:
#if VERSION <= VERSION_DEMO_US
                    if (push != NULL && !strcmp(hitGetName(push), entry->hitObjName))
                        SET_HIT(entry);
#else
                if (push != NULL && !strcmp(hitGetName(push), entry->hitObjName)) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
                }
#endif
                break;
            case CASE_NOT_PUSH:
#if VERSION <= VERSION_DEMO_US
                    if (push == NULL || strcmp(hitGetName(push), entry->hitObjName) != 0)
                        SET_HIT(entry);
#else
                if (push == NULL || strcmp(hitGetName(push), entry->hitObjName) != 0) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
                }
#endif
                break;
            case CASE_PUSH_A:
                if (push != NULL && fbatNpcTalkCheck() == NULL &&
                    !strcmp(hitGetName(push), entry->hitObjName) && pressedA) {
#if VERSION <= VERSION_DEMO_US
                        SET_HIT(entry);
#else
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            case CASE_HAMMER:
#if VERSION <= VERSION_DEMO_US
                    if (hammer != NULL && !strcmp(hitGetName(hammer), entry->hitObjName))
                        SET_HIT(entry);
#else
                if (hammer != NULL && !strcmp(hitGetName(hammer), entry->hitObjName)) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
                }
#endif
                break;
            case CASE_HAMMER_2:
                if (hammer != NULL && mario->currMotionId == MARIO_MOTION_HAMMER_2 &&
                    !strcmp(hitGetName(hammer), entry->hitObjName)) {
#if VERSION <= VERSION_DEMO_US
                        SET_HIT(entry);
#else
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            case CASE_BOBBERY:
                party = partyGetPtr(marioGetPartyId());
                if (party != NULL && party->currentMemberId == PARTY_MEMBER_BOBBERY) {
                    HitEntry* ph = party->hitobjAction;
#if VERSION <= VERSION_DEMO_US
                        if (ph != NULL && !strcmp(hitGetName(ph), entry->hitObjName))
                            SET_HIT(entry);
#else
                    if (ph != NULL && !strcmp(hitGetName(ph), entry->hitObjName)) {
                        entry->flags |= CASE_FLAG_TRIGGERED;
                    }
#endif
                }
                break;
            case CASE_HEAD:
#if VERSION <= VERSION_DEMO_US
                    if (head != NULL && !strcmp(hitGetName(head), entry->hitObjName))
                        SET_HIT(entry);
#else
                if (head != NULL && !strcmp(hitGetName(head), entry->hitObjName)) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
                }
#endif
                break;
            case CASE_GSWF:
#if VERSION <= VERSION_DEMO_US
                    if (swGet(entry->swFlag))
                        SET_HIT(entry);
#else
                if (swGet(entry->swFlag)) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
                }
#endif
                break;
            case CASE_LSWF:
#if VERSION <= VERSION_DEMO_US
                    if (_swGet(entry->swFlag))
                        SET_HIT(entry);
#else
                if (_swGet(entry->swFlag)) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
                }
#endif
                break;
            case CASE_FUNC:
#if VERSION <= VERSION_DEMO_US
                    if (entry->activeFunc != NULL && ((s32 (*)(CaseEntry*))entry->activeFunc)(entry)) {
                        SET_HIT(entry);
#else
                if (entry->activeFunc != NULL && ((s32 (*)(CaseEntry*))entry->activeFunc)(entry)) {
                    entry->flags |= CASE_FLAG_TRIGGERED;
#endif
                }
                break;
            }
        }
        entry = wp->entries;
        for (i = 0; i < wp->count; i++, entry++) {
            if ((entry->flags & CASE_FLAG_ACTIVE) && (entry->flags & CASE_FLAG_TRIGGERED)) {
                EventEntry* evt;
#if VERSION <= VERSION_DEMO_US
                if (!evtCheckID(entry->evtId))
                    entry->evtId = 0;
#else
                if (!evtCheckID(entry->evtId)) entry->evtId = 0;
#endif
                if (entry->evtId == 0) {
#if VERSION <= VERSION_DEMO_US
                    evt = evtEntry(entry->evtCode, entry->priority, EVENT_START_IMMEDIATE);
#else
                    evt = evtEntry(entry->evtCode, entry->priority, EVENT_START_IMMEDIATE);
#endif
                    evt->caseId = entry->caseId;
                    entry->evtId = evt->eventId;
                    memcpy(evt->lwData, entry->lwData, sizeof(entry->lwData));
#if VERSION <= VERSION_DEMO_US
                    if (entry->priority == 0x14)
                        npcStopForEvent();
#else
                    if (entry->priority == 0x14) npcStopForEvent();
#endif
                }
                entry->flags &= ~CASE_FLAG_TRIGGERED;
            }
        }
    }
}

/**
 * Returns the active case watching hit object obj (any hit-object condition, not flag or function cases),
 * or NULL; also NULL in battle. Mario's motions use it to tell whether a surface or wall has an event attached.
 */
CaseEntry* caseCheckHitObj(void* obj) {
    CaseEntry* entry;
    int i;
    if (gp->inBattle != 0) return NULL;
    entry = wp->entries;
    for (i = 0; i < wp->count; i++, entry++) {
        // a hit-object condition: not CASE_FUNC, CASE_GSWF or CASE_LSWF
        if ((entry->flags & CASE_FLAG_ACTIVE) && entry->conditionId != CASE_FUNC &&
            (u32)(entry->conditionId - CASE_GSWF) > 1) {
            if (!strcmp(hitGetName(obj), entry->hitObjName)) return entry;
        }
    }
    return NULL;
}

/**
 * Like caseCheckHitObj, but only for cases fired by Bobbery's blast on obj;
 * Bobbery's field code uses it to see whether a wall reacts to a bomb.
 */
CaseEntry* caseCheckHitObj2(void* obj) {
    CaseEntry* entry;
    int i;
    if (gp->inBattle != 0) return NULL;
    entry = wp->entries;
    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & CASE_FLAG_ACTIVE) && entry->conditionId != CASE_FUNC &&
            // the first two tests are always true for CASE_BOBBERY (as in the game code)
            (u32)(entry->conditionId - CASE_GSWF) > 1 && entry->conditionId == CASE_BOBBERY) {
            if (!strcmp(hitGetName(obj), entry->hitObjName)) return entry;
        }
    }
    return NULL;
}

/** Returns case caseId's entry, or NULL if that slot is no longer in use. */
CaseEntry* caseIdToPtr(s32 caseId) {
    CaseEntry* entry = &wp->entries[caseId];
    if (!(entry->flags & CASE_FLAG_ACTIVE)) return NULL;
    return entry;
}
