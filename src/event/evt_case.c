#include "event/evt_case.h"

extern s32 evtGetValue();
extern void caseDelete(s32);
extern s32* caseIdToPtr(s32);

s32 evt_run_case_evt(void* evt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern s32 evtGetNumber(void* evt, s32 value);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern s32 caseEntry(void* param);
    extern void evtSetValue(void* evt, s32 var, s32 value);
    typedef struct CaseEntryParam {
        u16 type;
        u16 pad;
        s32 name;
        s32 a;
        s32 zero;
        s32 data[0x10];
        s32 gap;
        s32 evtId;
        s32 kind;
    } CaseEntryParam;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 type = evtGetValue(evt, args[0]);
    s32 flag = evtGetValue(evt, args[1]);
    s32 name = evtGetValue(evt, args[2]);
    s32 a = evtGetNumber(evt, args[3]);
    s32 evtId = evtGetValue(evt, args[4]);
    CaseEntryParam param;
    void* p = &param;
    s32 storeType = type;
    s32 id;

    if (flag != 0) {
        storeType = type | 0x8000;
    }
    param.evtId = evtId;
    param.type = storeType;
    param.name = name;
    param.a = a;
    param.zero = 0;
    param.kind = 0;
    memcpy((void*)((s32)p + 0x10), (void*)((s32)evt + 0x9C), 0x40);
    id = caseEntry(p);
    if (args[5] != 0) {
        evtSetValue(evt, args[5], id);
    }
    return 2;
}


s32 evt_run_case_evt_bero(void* evt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern s32 evtGetNumber(void* evt, s32 value);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern s32 caseEntry(void* param);
    extern void evtSetValue(void* evt, s32 var, s32 value);
    typedef struct CaseEntryParam {
        u16 type;
        u16 pad;
        s32 name;
        s32 a;
        s32 zero;
        s32 data[0x10];
        s32 gap;
        s32 evtId;
        s32 kind;
    } CaseEntryParam;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 type = evtGetValue(evt, args[0]);
    s32 flag = evtGetValue(evt, args[1]);
    s32 name = evtGetValue(evt, args[2]);
    s32 a = evtGetNumber(evt, args[3]);
    s32 evtId = evtGetValue(evt, args[4]);
    CaseEntryParam param;
    void* p = &param;
    s32 storeType = type;
    s32 id;

    if (flag != 0) {
        storeType = type | 0x8000;
    }
    param.evtId = evtId;
    param.type = storeType;
    param.name = name;
    param.a = a;
    param.zero = 0;
    param.kind = 0x14;
    memcpy((void*)((s32)p + 0x10), (void*)((s32)evt + 0x9C), 0x40);
    id = caseEntry(p);
    if (args[5] != 0) {
        evtSetValue(evt, args[5], id);
    }
    return 2;
}
s32 evtRunCaseEntry(s32 type, s32 flag, s32 a, s32 b, s32 c, void* data) {
    extern void* memset(void* dst, s32 value, u32 size);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern s32 caseEntry(void* param);
    typedef struct CaseEntryParam {
        u16 type;
        u16 pad;
        s32 name;
        s32 a;
        s32 zero;
        s32 data[0x10];
        s32 gap;
        s32 evtId;
        s32 kind;
    } CaseEntryParam;
    CaseEntryParam param;
    void* p = &param;

    if (flag != 0) {
        type |= 0x8000;
    }
    param.type = type;
    param.name = a;
    param.a = b;
    param.zero = 0;
    param.evtId = c;
    param.kind = 0;
    if (data != 0) {
        memcpy((void*)((s32)p + 0x10), data, 0x40);
    } else {
        memset((void*)((s32)p + 0x10), 0, 0x40);
    }
    return caseEntry(p);
}

s32 evt_exit_case_evt(void* pEvt) {
    caseDelete(*(s32*)((s32)pEvt + 0x16C));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_del_case_evt(void* evt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern void* caseIdToPtr(s32 id);
    extern s32 evtCheckID(s32 id);
    extern void evtDeleteID(s32 id);
    extern void caseDelete(s32 id);
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 flag = evtGetValue(evt, args[0]);
    s32 id = evtGetValue(evt, args[1]);
    void* entry;

    if (flag == 0) {
        entry = caseIdToPtr(id);
        if (evtCheckID(*(s32*)((s32)entry + 0x94)) != 0) {
            evtDeleteID(*(s32*)((s32)entry + 0x94));
        }
    }
    caseDelete(id);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_set_case_wrk(void* pEvt) {
    void* args = *(void**)((s32)pEvt + 0x18);
    s32 id = evtGetValue(pEvt, *(s32*)args);
    s32 index = evtGetValue(pEvt, *(s32*)((s32)args + 4));
    s32 value = evtGetValue(pEvt, *(s32*)((s32)args + 8));
    s32* entry = caseIdToPtr(id);

    entry[index + 0x15] = value;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
