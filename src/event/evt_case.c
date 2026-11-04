// Script commands that register, run and remove case events (hit-triggered scripts, casedrv).

#include "evt/evt_case.h"
#include "driver/casedrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include <string.h>

/**
 * Registers a case event with casedrv: conditionId is what triggers it (with CASE_COND_FLAG_BALLOON when flag is
 * set), hitObjName and swFlag pick the hit object, evtCode runs with lwData copied in (or zeroed).
 * Returns the case id.
 */
static inline s32 _caseEntry(s32 conditionId, BOOL flag, const char* hitObjName, u32 swFlag,
                             void* evtCode, EvtWord* lwData, s32 priority) {
    CaseSetup setup;
    // FAKE: spare pointer to setup; using setup directly changes the object
    CaseSetup* ptr = &setup;

    if (flag) {
        conditionId |= CASE_COND_FLAG_BALLOON;
    }
    ptr->conditionId = conditionId;
    ptr->hitObjName = hitObjName;
    ptr->swFlag = swFlag;
    ptr->activeFunc = NULL;
    ptr->evtCode = evtCode;
    ptr->priority = priority;
    if (lwData) {
        memcpy(ptr->lwData, lwData, sizeof(setup.lwData));
    } else {
        memset(ptr->lwData, 0, sizeof(setup.lwData));
    }
    return caseEntry(ptr);
}

/** Same as _caseEntry, but the case script gets a copy of the calling script's LW variables. */
static inline s32 _evtCaseEntry(EventEntry* event, s32 conditionId, BOOL flag,
                                const char* hitObjName, u32 swFlag, void* evtCode, s32 priority) {
    CaseSetup setup;
    // FAKE: spare pointer to setup; using setup directly changes the object
    CaseSetup* ptr = &setup;

    if (flag) {
        conditionId |= CASE_COND_FLAG_BALLOON;
    }
    ptr->conditionId = conditionId;
    ptr->hitObjName = hitObjName;
    ptr->swFlag = swFlag;
    ptr->activeFunc = NULL;
    ptr->evtCode = evtCode;
    ptr->priority = priority;
    memcpy(ptr->lwData, event->lwData, sizeof(setup.lwData));
    return caseEntry(ptr);
}

/**
 * u16 conditionId, BOOL flag, const char* hitObjName, u32 swFlag, void* evtCode, s32 caseIdStoreIndex
 */
// u16 conditionId, BOOL flag, const char* hitObjName, u32 swFlag, void* evtCode,
// s32 caseIdStoreIndex
USER_FUNC(evt_run_case_evt) {
    EvtWord* args = event->args;
    s32 conditionId;
    BOOL flag;
    const char* hitObjName;
    u32 swFlag;
    void* evtCode;
    s32 caseId;

    conditionId = evtGetValue(event, args[0]);
    flag = evtGetValue(event, args[1]);
    hitObjName = (const char*)evtGetValue(event, args[2]);
    swFlag = evtGetNumber(event, args[3]);
    evtCode = (void*)evtGetValue(event, args[4]);
    caseId = _evtCaseEntry(event, conditionId, flag, hitObjName, swFlag, evtCode, 0);
    if (args[5]) {
        evtSetValue(event, args[5], caseId);
    }
    return EVT_RETURN_DONE;
}

/** same as evt_run_case_evt, but with priority 20 */
// same as evt_run_case_evt, but with priority 20
USER_FUNC(evt_run_case_evt_bero) {
    EvtWord* args = event->args;
    s32 conditionId;
    BOOL flag;
    const char* hitObjName;
    u32 swFlag;
    void* evtCode;
    s32 caseId;

    conditionId = evtGetValue(event, args[0]);
    flag = evtGetValue(event, args[1]);
    hitObjName = (const char*)evtGetValue(event, args[2]);
    swFlag = evtGetNumber(event, args[3]);
    evtCode = (void*)evtGetValue(event, args[4]);
    caseId = _evtCaseEntry(event, conditionId, flag, hitObjName, swFlag, evtCode, 20);
    if (args[5]) {
        evtSetValue(event, args[5], caseId);
    }
    return EVT_RETURN_DONE;
}

/**
 * C entry point for registering a case event at priority 0 (_caseEntry);
 * evt_map.c uses it for the airport ride cases, evt_shop.c and evt_win.c for theirs. Returns the case id.
 */
s32 evtRunCaseEntry(s32 conditionId, BOOL flag, const char* hitObjName, u32 swFlag, void* evtCode,
                    EvtWord* lwData) {
    return _caseEntry(conditionId, flag, hitObjName, swFlag, evtCode, lwData, 0);
}

/** Deletes the case that started the running script (event->caseId), so it no longer triggers. */
USER_FUNC(evt_exit_case_evt) {
    caseDelete(event->caseId);
    return EVT_RETURN_DONE;
}

/** BOOL keepEvent, s32 caseId */
// BOOL keepEvent, s32 caseId
USER_FUNC(evt_del_case_evt) {
    EvtWord* args = event->args;
    BOOL keepEvent;
    s32 caseId;
    CaseEntry* entry;

    keepEvent = evtGetValue(event, args[0]);
    caseId = evtGetValue(event, args[1]);
    if (!keepEvent) {
        entry = caseIdToPtr(caseId);
        if (evtCheckID(entry->evtId)) {
            evtDeleteID(entry->evtId);
        }
    }
    caseDelete(caseId);
    return EVT_RETURN_DONE;
}

/** s32 caseId, s32 index, s32 value */
// s32 caseId, s32 index, s32 value
USER_FUNC(evt_set_case_wrk) {
    EvtWord* args = event->args;
    s32 caseId, index, value;
    CaseEntry* entry;

    caseId = evtGetValue(event, args[0]);
    index = evtGetValue(event, args[1]);
    value = evtGetValue(event, args[2]);
    entry = caseIdToPtr(caseId);
    entry->lwData[index] = value;
    return EVT_RETURN_DONE;
}
