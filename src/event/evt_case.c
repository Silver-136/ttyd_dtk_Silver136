#include "evt/evt_case.h"
#include "driver/casedrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include <string.h>

static inline s32 _caseEntry(s32 conditionId, BOOL flag, const char* hitObjName, u32 swFlag,
                             void* evtCode, EvtWord* lwData, s32 priority) {
    CaseSetup setup;
    CaseSetup* ptr = &setup;

    if (flag) {
        conditionId |= 0x8000;
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

static inline s32 _evtCaseEntry(EventEntry* event, s32 conditionId, BOOL flag,
                                const char* hitObjName, u32 swFlag, void* evtCode, s32 priority) {
    CaseSetup setup;
    CaseSetup* ptr = &setup;

    if (flag) {
        conditionId |= 0x8000;
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

s32 evtRunCaseEntry(s32 conditionId, BOOL flag, const char* hitObjName, u32 swFlag, void* evtCode,
                    EvtWord* lwData) {
    return _caseEntry(conditionId, flag, hitObjName, swFlag, evtCode, lwData, 0);
}

USER_FUNC(evt_exit_case_evt) {
    caseDelete(event->caseId);
    return EVT_RETURN_DONE;
}

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
