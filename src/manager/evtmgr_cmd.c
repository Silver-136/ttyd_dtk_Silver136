#include "manager/evtmgr_cmd.h"

#include "manager/evtmgr.h"
extern int strcmp(const char*, const char*);
extern void* evtGetWork(void);
extern void* evtEntryType(void*, s32, s32, s32);
extern void* evtBrotherEntry(void*, void*, unsigned int);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma opt_common_subs off

s32 evt_while(void* event) {
    extern s32 evtGetValue(void*, s32);
    extern s32 evtSetValue(void*, s32, s32);
    s32 idx;
    s32 offset;
    s32 count;

    idx = *(signed char*)((int)event + 0xE);
    offset = idx << 2;
    count = *(s32*)((int)event + offset + 0x108);

    if (count == 0) {
        *(void**)((int)event + 0x14) = *(void**)((int)event + offset + 0xE8);
        return 2;
    }

    if (count >= (s32)0xFF676980) {
        count--;
        *(s32*)((int)event + offset + 0x108) = count;
    } else {
        s32 value = evtGetValue(event, count);
        evtSetValue(event, count, value - 1);
        count = value - 1;
    }

    if (count != 0) {
        *(void**)((int)event + 0x14) = *(void**)((int)event + offset + 0xE8);
    } else {
        *(unsigned char*)((int)event + 0xE) = *(unsigned char*)((int)event + 0xE) - 1;
    }
    return 2;
#pragma opt_common_subs reset
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_wait_msec(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args = *(s32**)((u8*)event + 0x18);
    u64 time = *(u64*)event;
    s32 wait;
    u64 tickDiff;
    s32 msecDiff;

    switch (*(s8*)((u8*)event + 0xD)) {
        case 0:
            wait = evtGetValue(event, args[0]);
            *(s32*)((u8*)event + 0x78) = wait;
            *(s32*)((u8*)event + 0x7C) = (s32)((time >> 32) & 0xFFFFFFFF);
            *(s32*)((u8*)event + 0x80) = (s32)(time & 0xFFFFFFFF);
            *(u8*)((u8*)event + 0xD) = 1;
            break;
    }

    wait = *(s32*)((u8*)event + 0x78);
    if (wait == 0) {
        return 2;
    }

    tickDiff = (s32)time - *(s32*)((u8*)event + 0x80);
    msecDiff = (s32)(tickDiff /
                     (__mulhwu(0x10624DD3, *(volatile u32*)0x800000F8 >> 2) >> 6));
    return msecDiff >= wait;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

int evt_if_str_equal(void* pEvt) {
    u8* event = (u8*)pEvt;
    s32* args;
    char* left;
    char* right;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    left = (char*)evtGetValue(pEvt, args[0]);
    right = (char*)evtGetValue(pEvt, args[1]);
    if (left == 0) left = "";
    if (right == 0) right = "";
    if (strcmp(left, right)) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

int evt_if_str_not_equal(void* pEvt) {
    u8* event = (u8*)pEvt;
    s32* args;
    char* left;
    char* right;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    left = (char*)evtGetValue(pEvt, args[0]);
    right = (char*)evtGetValue(pEvt, args[1]);
    if (left == 0) left = "";
    if (right == 0) right = "";
    if (!strcmp(left, right)) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

int evt_if_str_small(void* pEvt) {
    u8* event = (u8*)pEvt;
    s32* args;
    char* left;
    char* right;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    left = (char*)evtGetValue(pEvt, args[0]);
    right = (char*)evtGetValue(pEvt, args[1]);
    if (left == 0) left = "";
    if (right == 0) right = "";
    if (strcmp(left, right) >= 0) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

int evt_if_str_large(void* pEvt) {
    u8* event = (u8*)pEvt;
    s32* args;
    char* left;
    char* right;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    left = (char*)evtGetValue(pEvt, args[0]);
    right = (char*)evtGetValue(pEvt, args[1]);
    if (left == 0) left = "";
    if (right == 0) right = "";
    if (strcmp(left, right) <= 0) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

int evt_if_str_small_equal(void* pEvt) {
    u8* event = (u8*)pEvt;
    s32* args;
    char* left;
    char* right;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    left = (char*)evtGetValue(pEvt, args[0]);
    right = (char*)evtGetValue(pEvt, args[1]);
    if (left == 0) left = "";
    if (right == 0) right = "";
    if (strcmp(left, right) > 0) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

int evt_if_str_large_equal(void* pEvt) {
    u8* event = (u8*)pEvt;
    s32* args;
    char* left;
    char* right;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    left = (char*)evtGetValue(pEvt, args[0]);
    right = (char*)evtGetValue(pEvt, args[1]);
    if (left == 0) left = "";
    if (right == 0) right = "";
    if (strcmp(left, right) < 0) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_iff_equal(int param_1) {
    u8* event = (u8*)param_1;
    s32* args;
    f32 value1;
    f32 value2;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    value1 = evtGetFloat((void*)param_1, args[0]);
    value2 = evtGetFloat((void*)param_1, args[1]);
    if (value1 != value2) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_iff_not_equal(int param_1) {
    u8* event = (u8*)param_1;
    s32* args;
    f32 value1;
    f32 value2;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    value1 = evtGetFloat((void*)param_1, args[0]);
    value2 = evtGetFloat((void*)param_1, args[1]);
    if (value1 == value2) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_iff_small(int param_1) {
    u8* event = (u8*)param_1;
    s32* args;
    f32 value1;
    f32 value2;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    value1 = evtGetFloat((void*)param_1, args[0]);
    value2 = evtGetFloat((void*)param_1, args[1]);
    if (value1 >= value2) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_iff_large(int param_1) {
    u8* event = (u8*)param_1;
    s32* args;
    f32 value1;
    f32 value2;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    value1 = evtGetFloat((void*)param_1, args[0]);
    value2 = evtGetFloat((void*)param_1, args[1]);
    if (value1 <= value2) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_iff_small_equal(int param_1) {
    u8* event = (u8*)param_1;
    s32* args;
    f32 value1;
    f32 value2;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    value1 = evtGetFloat((void*)param_1, args[0]);
    value2 = evtGetFloat((void*)param_1, args[1]);
    if (value1 > value2) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_iff_large_equal(int param_1) {
    u8* event = (u8*)param_1;
    s32* args;
    f32 value1;
    f32 value2;
    s32 opcode;
    s32 depth;
    u32* ip;
    u32 cmd;

    args = *(s32**)(event + 0x18);
    value1 = evtGetFloat((void*)param_1, args[0]);
    value2 = evtGetFloat((void*)param_1, args[1]);
    if (value1 < value2) {
        depth = 0;
        ip = *(u32**)(event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (s32)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(u32**)(event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

s32 evt_if_equal(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 value1;
    s32 value2;
    s32 opcode;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;

    args = *(s32**)((int)event + 0x18);
    value1 = evtGetValue(event, args[0]);
    value2 = evtGetValue(event, args[1]);
    if (value1 != value2) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (int)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(unsigned int**)((int)event + 0x14) = ip;
        return 2;
    }
    return 2;
}

s32 evt_if_not_equal(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 value1;
    s32 value2;
    s32 opcode;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;

    args = *(s32**)((int)event + 0x18);
    value1 = evtGetValue(event, args[0]);
    value2 = evtGetValue(event, args[1]);
    if (value1 == value2) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (int)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(unsigned int**)((int)event + 0x14) = ip;
        return 2;
    }
    return 2;
}

s32 evt_if_small(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 value1;
    s32 value2;
    s32 opcode;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;

    args = *(s32**)((int)event + 0x18);
    value1 = evtGetValue(event, args[0]);
    value2 = evtGetValue(event, args[1]);
    if (value1 >= value2) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (int)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(unsigned int**)((int)event + 0x14) = ip;
        return 2;
    }
    return 2;
}

s32 evt_if_large(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 value1;
    s32 value2;
    s32 opcode;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;

    args = *(s32**)((int)event + 0x18);
    value1 = evtGetValue(event, args[0]);
    value2 = evtGetValue(event, args[1]);
    if (value1 <= value2) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (int)cmd >> 0x10;
        if (opcode == 0x20) goto case_20;
        if (opcode >= 0x20) goto high;
        if (opcode == 1) goto decrement;
        if (opcode < 1) goto loop;
        if (opcode >= 0xC) goto increment;
        goto loop;
high:
        if (opcode >= 0x22) goto loop;
decrement:
        if (--depth >= 0) goto loop;
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) goto loop;
found:
        *(unsigned int**)((int)event + 0x14) = ip;
        return 2;
    }
    return 2;
}

s32 evt_if_small_equal(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 value1;
    s32 value2;
    s32 opcode;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;

    args = *(s32**)((int)event + 0x18);
    value1 = evtGetValue(event, args[0]);
    value2 = evtGetValue(event, args[1]);
    if (value1 > value2) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (int)cmd >> 0x10;
        if (opcode == 0x20) {
            goto case_20;
        }
        if (opcode >= 0x20) {
            goto high;
        }
        if (opcode == 1) {
            goto decrement;
        }
        if (opcode < 1) {
            goto loop;
        }
        if (opcode >= 0xC) {
            goto increment;
        }
        goto loop;
high:
        if (opcode >= 0x22) {
            goto loop;
        }
decrement:
        if (--depth >= 0) {
            goto loop;
        }
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) {
            goto loop;
        }
found:
        *(unsigned int**)((int)event + 0x14) = ip;
        return 2;
    }
    return 2;
}

s32 evt_if_large_equal(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 value1;
    s32 value2;
    s32 opcode;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;

    args = *(s32**)((int)event + 0x18);
    value1 = evtGetValue(event, args[0]);
    value2 = evtGetValue(event, args[1]);
    if (value1 < value2) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (int)cmd >> 0x10;
        if (opcode == 0x20) {
            goto case_20;
        }
        if (opcode >= 0x20) {
            goto high;
        }
        if (opcode == 1) {
            goto decrement;
        }
        if (opcode < 1) {
            goto loop;
        }
        if (opcode >= 0xC) {
            goto increment;
        }
        goto loop;
high:
        if (opcode >= 0x22) {
            goto loop;
        }
decrement:
        if (--depth >= 0) {
            goto loop;
        }
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) {
            goto loop;
        }
found:
        *(unsigned int**)((int)event + 0x14) = ip;
        return 2;
    }
    return 2;
}

s32 evt_if_flag(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 value;
    s32 flags;
    s32 opcode;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;

    args = *(s32**)((int)event + 0x18);
    value = evtGetValue(event, args[0]);
    flags = args[1];
    if ((value & flags) == 0) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (int)cmd >> 0x10;
        if (opcode == 0x20) {
            goto case_20;
        }
        if (opcode >= 0x20) {
            goto high;
        }
        if (opcode == 1) {
            goto decrement;
        }
        if (opcode < 1) {
            goto loop;
        }
        if (opcode >= 0xC) {
            goto increment;
        }
        goto loop;
high:
        if (opcode >= 0x22) {
            goto loop;
        }
decrement:
        if (--depth >= 0) {
            goto loop;
        }
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) {
            goto loop;
        }
found:
        *(unsigned int**)((int)event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_if_not_flag(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 value;
    s32 flags;
    s32 opcode;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;

    args = *(s32**)((int)event + 0x18);
    value = evtGetValue(event, args[0]);
    flags = args[1];
    if ((value & flags) != 0) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
loop:
        cmd = *ip++;
        opcode = cmd & 0xFFFF;
        ip += (int)cmd >> 0x10;
        if (opcode == 0x20) {
            goto case_20;
        }
        if (opcode >= 0x20) {
            goto high;
        }
        if (opcode == 1) {
            goto decrement;
        }
        if (opcode < 1) {
            goto loop;
        }
        if (opcode >= 0xC) {
            goto increment;
        }
        goto loop;
high:
        if (opcode >= 0x22) {
            goto loop;
        }
decrement:
        if (--depth >= 0) {
            goto loop;
        }
        goto found;
increment:
        depth++;
        goto loop;
case_20:
        if (depth != 0) {
            goto loop;
        }
found:
        *(unsigned int**)((int)event + 0x14) = ip;
        return 2;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_case_equal(int param_1) {
    s8 idx;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;
    u32* args;
    u32* save;
    u32* cur;
    s32 opcode;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    targetValue = evtGetValue((struct EventEntry*)param_1, args[0]);
    state = *(u8*)(param_1 + idx + 0x128);
    inputValue = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = (s8)state;

    if (state <= 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop0:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                goto loop0;
            case 0x31:
                if (--depth != 0) {
                    goto loop0;
                }
                break;
            default:
                goto loop0;
        }
        *(u32**)(param_1 + 0x14) = save;
    } else if (targetValue != inputValue) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop1:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto setpos;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto setpos;
                }
                break;
        }
        goto loop1;
setpos:
        *(u32**)(param_1 + 0x14) = save;
    } else {
        *(s8*)(param_1 + idx + 0x128) = 0;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 evt_case_not_equal(int param_1) {
    s8 idx;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;
    u32* args;
    u32* save;
    u32* cur;
    s32 opcode;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    targetValue = evtGetValue((struct EventEntry*)param_1, args[0]);
    state = *(u8*)(param_1 + idx + 0x128);
    inputValue = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = (s8)state;

    if (state <= 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop0:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                goto loop0;
            case 0x31:
                if (--depth != 0) {
                    goto loop0;
                }
                break;
            default:
                goto loop0;
        }
        *(u32**)(param_1 + 0x14) = save;
    } else if (targetValue == inputValue) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop1:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto setpos;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto setpos;
                }
                break;
        }
        goto loop1;
setpos:
        *(u32**)(param_1 + 0x14) = save;
    } else {
        *(s8*)(param_1 + idx + 0x128) = 0;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_case_small(int param_1) {
    s8 idx;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;
    u32* args;
    u32* save;
    u32* cur;
    s32 opcode;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    targetValue = evtGetValue((struct EventEntry*)param_1, args[0]);
    state = *(u8*)(param_1 + idx + 0x128);
    inputValue = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = (s8)state;

    if (state <= 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop0:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                goto loop0;
            case 0x31:
                if (--depth != 0) {
                    goto loop0;
                }
                break;
            default:
                goto loop0;
        }
        *(u32**)(param_1 + 0x14) = save;
    } else if (targetValue <= inputValue) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop1:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto setpos;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto setpos;
                }
                break;
        }
        goto loop1;
setpos:
        *(u32**)(param_1 + 0x14) = save;
    } else {
        *(s8*)(param_1 + idx + 0x128) = 0;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_case_small_equal(int param_1) {
    s8 idx;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;
    u32* args;
    u32* save;
    u32* cur;
    s32 opcode;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    targetValue = evtGetValue((struct EventEntry*)param_1, args[0]);
    state = *(u8*)(param_1 + idx + 0x128);
    inputValue = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = (s8)state;

    if (state <= 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop0:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                goto loop0;
            case 0x31:
                if (--depth != 0) {
                    goto loop0;
                }
                break;
            default:
                goto loop0;
        }
        *(u32**)(param_1 + 0x14) = save;
    } else if (targetValue < inputValue) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop1:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto setpos;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto setpos;
                }
                break;
        }
        goto loop1;
setpos:
        *(u32**)(param_1 + 0x14) = save;
    } else {
        *(s8*)(param_1 + idx + 0x128) = 0;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_case_large(int param_1) {
    s8 idx;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;
    u32* args;
    u32* save;
    u32* cur;
    s32 opcode;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    targetValue = evtGetValue((struct EventEntry*)param_1, args[0]);
    state = *(u8*)(param_1 + idx + 0x128);
    inputValue = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = (s8)state;

    if (state <= 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop0:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                goto loop0;
            case 0x31:
                if (--depth != 0) {
                    goto loop0;
                }
                break;
            default:
                goto loop0;
        }
        *(u32**)(param_1 + 0x14) = save;
    } else if (targetValue >= inputValue) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop1:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto setpos;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto setpos;
                }
                break;
        }
        goto loop1;
setpos:
        *(u32**)(param_1 + 0x14) = save;
    } else {
        *(s8*)(param_1 + idx + 0x128) = 0;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_case_large_equal(int param_1) {
    s8 idx;
    s32 depth;
    s32 targetValue;
    s32 state;
    s32 inputValue;
    u32* args;
    u32* save;
    u32* cur;
    s32 opcode;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    targetValue = evtGetValue((struct EventEntry*)param_1, args[0]);
    state = *(u8*)(param_1 + idx + 0x128);
    inputValue = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = (s8)state;

    if (state <= 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop0:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                goto loop0;
            case 0x31:
                if (--depth != 0) {
                    goto loop0;
                }
                break;
            default:
                goto loop0;
        }
        *(u32**)(param_1 + 0x14) = save;
    } else if (targetValue > inputValue) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop1:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto setpos;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto setpos;
                }
                break;
        }
        goto loop1;
setpos:
        *(u32**)(param_1 + 0x14) = save;
    } else {
        *(s8*)(param_1 + idx + 0x128) = 0;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_case_between(int param_1) {
    s8 idx;
    s32 depth;
    s32 min;
    s32 max;
    s32 state;
    s32 inputValue;
    u32* args;
    u32* save;
    u32* cur;
    s32 opcode;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    min = evtGetValue((struct EventEntry*)param_1, args[0]);
    max = evtGetValue((struct EventEntry*)param_1, args[1]);
    state = *(u8*)(param_1 + idx + 0x128);
    inputValue = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = (s8)state;

    if (state <= 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop0:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                goto loop0;
            case 0x31:
                if (--depth != 0) {
                    goto loop0;
                }
                break;
            default:
                goto loop0;
        }
        *(u32**)(param_1 + 0x14) = save;
    } else if (min <= inputValue && inputValue <= max) {
        *(s8*)(param_1 + idx + 0x128) = 0;
    } else {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop1:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto setpos;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto setpos;
                }
                break;
        }
        goto loop1;
setpos:
        *(u32**)(param_1 + 0x14) = save;
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_case_flag(s32 param_1) {
    s8 idx;
    s32 depth;
    u32 targetMask;
    s32 state;
    s32 inputValue;
    u32* args;
    u32* save;
    u32* cur;
    s32 opcode;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    targetMask = args[0];
    state = *(u8*)(param_1 + idx + 0x128);
    inputValue = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = (s8)state;

    if (state <= 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop0:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                goto loop0;
            case 0x31:
                if (--depth != 0) {
                    goto loop0;
                }
                break;
            default:
                goto loop0;
        }
        *(u32**)(param_1 + 0x14) = save;
        return 2;
    }

    if ((inputValue & targetMask) == 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop1:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto setpos;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto setpos;
                }
                break;
        }
        goto loop1;
setpos:
        *(u32**)(param_1 + 0x14) = save;
    } else {
        *(s8*)(param_1 + idx + 0x128) = 0;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_case_or(int param_1) {
    s8 idx;
    s32 depth;
    s32 value;
    s32 opcode;
    u32* save;
    u32* cur;
    u32* args;
    s32 entryIdx;
    s32 expected;
    s8 state;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    value = evtGetValue((struct EventEntry*)param_1, *args);
    entryIdx = param_1 + idx;
    expected = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = *(u8*)(entryIdx + 0x128);
    state = (s8)state;

    if (state == 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
scan_end:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 16;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                goto scan_end;
            case 0x31:
                if (--depth != 0) {
                    goto scan_end;
                }
                break;
            default:
                goto scan_end;
        }
        *(u32**)(param_1 + 0x14) = save;
    } else if (value == expected) {
        *(s8*)(entryIdx + 0x128) = -1;
    } else if (state != -1) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
scan_case:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 16;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto case_done;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto case_done;
                }
                break;
        }
        goto scan_case;
case_done:
        *(u32**)(param_1 + 0x14) = save;
    }
    return 2;
}

#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

s32 evt_case_and(int param_1) {
    s8 idx;
    s32 depth;
    s32 value;
    s32 opcode;
    u32* save;
    u32* cur;
    u32* args;
    s32 entryIdx;
    s32 expected;
    s8 state;

    idx = *(u8*)(param_1 + 0xF);
    idx = (s8)idx;
    args = *(u32**)(param_1 + 0x18);
    value = evtGetValue((struct EventEntry*)param_1, *args);
    entryIdx = param_1 + idx;
    expected = *(s32*)(param_1 + (idx << 2) + 0x130);
    state = *(u8*)(entryIdx + 0x128);
    state = (s8)state;

    if (state == 0) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop0:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x22:
            case 1:
                depth++;
                goto loop0;
            case 0x31:
                depth--;
                if (depth != 0) {
                    goto loop0;
                }
                break;
            default:
                goto loop0;
        }
        *(u32**)(param_1 + 0x14) = save;
        return 2;
    }
    if (state == -2) {
        depth = 1;
        cur = *(u32**)(param_1 + 0x14);
loop1:
        save = cur;
        opcode = *save & 0xFFFF;
        cur = save + 1;
        cur += (s32)*save >> 0x10;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto setpos1;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto setpos1;
                }
                break;
        }
        goto loop1;
setpos1:
        *(u32**)(param_1 + 0x14) = save;
        return 2;
    }
    if (value == expected) {
        *(s8*)(entryIdx + 0x128) = -1;
        return 2;
    }
    *(s8*)(entryIdx + 0x128) = -2;
    depth = 1;
    cur = *(u32**)(param_1 + 0x14);
loop2:
    save = cur;
    opcode = *save & 0xFFFF;
    cur = save + 1;
    cur += (s32)*save >> 0x10;
    switch (opcode) {
        case 0x01:
        case 0x22:
            depth++;
            break;
        case 0x31:
            if (--depth == 0) {
                goto setpos2;
            }
            break;
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        case 0x28:
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
            if (depth == 1) {
                goto setpos2;
            }
            break;
    }
    goto loop2;
setpos2:
    *(u32**)(param_1 + 0x14) = save;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_case_end(int event) {
    s32 opcode;
    s32 depth;
    u8* entry;
    u32* command;
    u32* next;

    entry = (u8*)(event + *(s8*)(event + 0xF));
    if (*(s8*)(entry + 0x128) == 0) {
        depth = 1;
        next = *(u32**)(event + 0x14);
        while (1) {
            command = next;
            opcode = *command & 0xFFFF;
            next = command + 1;
            next += (s32)*command >> 16;
            switch (opcode) {
                case 0x01:
                case 0x22:
                    depth++;
                    break;
                case 0x31:
                    if (--depth == 0) {
                        *(u32**)(event + 0x14) = command;
                        return 2;
                    }
                    break;
            }
        }
    }

    if (*(s8*)(entry + 0x128) == -1) {
        *(s8*)(entry + 0x128) = 0;
        depth = 1;
        next = *(u32**)(event + 0x14);
        while (1) {
            command = next;
            opcode = *command & 0xFFFF;
            next = command + 1;
            next += (s32)*command >> 16;
            switch (opcode) {
                case 0x01:
                case 0x22:
                    depth++;
                    break;
                case 0x31:
                    if (--depth == 0) {
                        *(u32**)(event + 0x14) = command;
                        return 2;
                    }
                    break;
            }
        }
    }

    depth = 1;
    *(s8*)(entry + 0x128) = 1;
    next = *(u32**)(event + 0x14);
    while (1) {
        command = next;
        opcode = *command & 0xFFFF;
        next = command + 1;
        next += (s32)*command >> 16;
        switch (opcode) {
            case 0x01:
            case 0x22:
                depth++;
                break;
            case 0x31:
                if (--depth == 0) {
                    goto case_done;
                }
                break;
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
            case 0x29:
            case 0x2A:
            case 0x2B:
            case 0x2C:
            case 0x2D:
            case 0x2E:
            case 0x2F:
                if (depth == 1) {
                    goto case_done;
                }
                break;
        }
    }
case_done:
    *(u32**)(event + 0x14) = command;
    return 2;
}

s32 evt_run_evt(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32 code;
    void* child;

    code = evtGetValue(event, **(s32**)((int)event + 0x18));
    child = evtEntryType((void*)code, *(unsigned char*)((int)event + 0xB), 0, *(unsigned char*)((int)event + 0xC));
    *(unsigned int*)((int)child + 0x160) = *(unsigned int*)((int)event + 0x160);
    *(unsigned int*)((int)child + 0x170) = *(unsigned int*)((int)event + 0x170);
    *(unsigned int*)((int)child + 0x9C) = *(unsigned int*)((int)event + 0x9C);
    *(unsigned int*)((int)child + 0xA0) = *(unsigned int*)((int)event + 0xA0);
    *(unsigned int*)((int)child + 0xA4) = *(unsigned int*)((int)event + 0xA4);
    *(unsigned int*)((int)child + 0xA8) = *(unsigned int*)((int)event + 0xA8);
    *(unsigned int*)((int)child + 0xAC) = *(unsigned int*)((int)event + 0xAC);
    *(unsigned int*)((int)child + 0xB0) = *(unsigned int*)((int)event + 0xB0);
    *(unsigned int*)((int)child + 0xB4) = *(unsigned int*)((int)event + 0xB4);
    *(unsigned int*)((int)child + 0xB8) = *(unsigned int*)((int)event + 0xB8);
    *(unsigned int*)((int)child + 0xBC) = *(unsigned int*)((int)event + 0xBC);
    *(unsigned int*)((int)child + 0xC0) = *(unsigned int*)((int)event + 0xC0);
    *(unsigned int*)((int)child + 0xC4) = *(unsigned int*)((int)event + 0xC4);
    *(unsigned int*)((int)child + 0xC8) = *(unsigned int*)((int)event + 0xC8);
    *(unsigned int*)((int)child + 0xCC) = *(unsigned int*)((int)event + 0xCC);
    *(unsigned int*)((int)child + 0xD0) = *(unsigned int*)((int)event + 0xD0);
    *(unsigned int*)((int)child + 0xD4) = *(unsigned int*)((int)event + 0xD4);
    *(unsigned int*)((int)child + 0xD8) = *(unsigned int*)((int)event + 0xD8);
    *(unsigned int*)((int)child + 0xDC) = *(unsigned int*)((int)event + 0xDC);
    *(unsigned int*)((int)child + 0xE0) = *(unsigned int*)((int)event + 0xE0);
    *(unsigned int*)((int)child + 0xE4) = *(unsigned int*)((int)event + 0xE4);
    *(unsigned int*)((int)child + 0x154) = *(unsigned int*)((int)event + 0x154);
    *(unsigned int*)((int)child + 0x158) = *(unsigned int*)((int)event + 0x158);
    return 2;
}

s32 evt_run_evt_id(void* event) {
    extern s32 evtGetValue(void*, s32);
    extern s32 evtSetValue(void*, s32, s32);
    s32* args;
    s32 dst;
    s32 code;
    void* child;

    args = *(s32**)((int)event + 0x18);
    code = evtGetValue(event, args[0]);
    dst = args[1];
    child = evtEntryType((void*)code, *(unsigned char*)((int)event + 0xB), 0, *(unsigned char*)((int)event + 0xC));
    *(unsigned int*)((int)child + 0x160) = *(unsigned int*)((int)event + 0x160);
    *(unsigned int*)((int)child + 0x170) = *(unsigned int*)((int)event + 0x170);
    *(unsigned int*)((int)child + 0x9C) = *(unsigned int*)((int)event + 0x9C);
    *(unsigned int*)((int)child + 0xA0) = *(unsigned int*)((int)event + 0xA0);
    *(unsigned int*)((int)child + 0xA4) = *(unsigned int*)((int)event + 0xA4);
    *(unsigned int*)((int)child + 0xA8) = *(unsigned int*)((int)event + 0xA8);
    *(unsigned int*)((int)child + 0xAC) = *(unsigned int*)((int)event + 0xAC);
    *(unsigned int*)((int)child + 0xB0) = *(unsigned int*)((int)event + 0xB0);
    *(unsigned int*)((int)child + 0xB4) = *(unsigned int*)((int)event + 0xB4);
    *(unsigned int*)((int)child + 0xB8) = *(unsigned int*)((int)event + 0xB8);
    *(unsigned int*)((int)child + 0xBC) = *(unsigned int*)((int)event + 0xBC);
    *(unsigned int*)((int)child + 0xC0) = *(unsigned int*)((int)event + 0xC0);
    *(unsigned int*)((int)child + 0xC4) = *(unsigned int*)((int)event + 0xC4);
    *(unsigned int*)((int)child + 0xC8) = *(unsigned int*)((int)event + 0xC8);
    *(unsigned int*)((int)child + 0xCC) = *(unsigned int*)((int)event + 0xCC);
    *(unsigned int*)((int)child + 0xD0) = *(unsigned int*)((int)event + 0xD0);
    *(unsigned int*)((int)child + 0xD4) = *(unsigned int*)((int)event + 0xD4);
    *(unsigned int*)((int)child + 0xD8) = *(unsigned int*)((int)event + 0xD8);
    *(unsigned int*)((int)child + 0xDC) = *(unsigned int*)((int)event + 0xDC);
    *(unsigned int*)((int)child + 0xE0) = *(unsigned int*)((int)event + 0xE0);
    *(unsigned int*)((int)child + 0xE4) = *(unsigned int*)((int)event + 0xE4);
    *(unsigned int*)((int)child + 0x154) = *(unsigned int*)((int)event + 0x154);
    *(unsigned int*)((int)child + 0x158) = *(unsigned int*)((int)event + 0x158);
    evtSetValue(event, dst, *(s32*)((int)child + 0x15C));
    return 2;
}

s32 evt_inline_evt(void* event) {
    unsigned int* start;
    unsigned int* ip;
    unsigned int cmd;
    void* child;

    start = *(unsigned int**)((int)event + 0x14);
    ip = start;
    do {
        cmd = *ip++;
        ip += (int)cmd >> 0x10;
    } while ((cmd & 0xFFFF) != 0x6D);

    *(unsigned int**)((int)event + 0x14) = ip;
    child = evtEntryType(start, *(unsigned char*)((int)event + 0xB), 0x60, *(unsigned char*)((int)event + 0xC));
    *(unsigned int*)((int)child + 0x160) = *(unsigned int*)((int)event + 0x160);
    *(unsigned int*)((int)child + 0x154) = *(unsigned int*)((int)event + 0x154);
    *(unsigned int*)((int)child + 0x158) = *(unsigned int*)((int)event + 0x158);
    *(unsigned int*)((int)child + 0x170) = *(unsigned int*)((int)event + 0x170);
    *(unsigned int*)((int)child + 0x17C) = *(unsigned int*)((int)event + 0x17C);
    *(unsigned int*)((int)child + 0x178) = *(unsigned int*)((int)event + 0x178);
    *(unsigned int*)((int)child + 0x180) = *(unsigned int*)((int)event + 0x180);
    *(unsigned int*)((int)child + 0x184) = *(unsigned int*)((int)event + 0x184);
    *(unsigned int*)((int)child + 0x188) = *(unsigned int*)((int)event + 0x188);
    *(unsigned int*)((int)child + 0x18C) = *(unsigned int*)((int)event + 0x18C);
    *(unsigned int*)((int)child + 0x190) = *(unsigned int*)((int)event + 0x190);
    *(unsigned int*)((int)child + 0x9C) = *(unsigned int*)((int)event + 0x9C);
    *(unsigned int*)((int)child + 0xA0) = *(unsigned int*)((int)event + 0xA0);
    *(unsigned int*)((int)child + 0xA4) = *(unsigned int*)((int)event + 0xA4);
    *(unsigned int*)((int)child + 0xA8) = *(unsigned int*)((int)event + 0xA8);
    *(unsigned int*)((int)child + 0xAC) = *(unsigned int*)((int)event + 0xAC);
    *(unsigned int*)((int)child + 0xB0) = *(unsigned int*)((int)event + 0xB0);
    *(unsigned int*)((int)child + 0xB4) = *(unsigned int*)((int)event + 0xB4);
    *(unsigned int*)((int)child + 0xB8) = *(unsigned int*)((int)event + 0xB8);
    *(unsigned int*)((int)child + 0xBC) = *(unsigned int*)((int)event + 0xBC);
    *(unsigned int*)((int)child + 0xC0) = *(unsigned int*)((int)event + 0xC0);
    *(unsigned int*)((int)child + 0xC4) = *(unsigned int*)((int)event + 0xC4);
    *(unsigned int*)((int)child + 0xC8) = *(unsigned int*)((int)event + 0xC8);
    *(unsigned int*)((int)child + 0xCC) = *(unsigned int*)((int)event + 0xCC);
    *(unsigned int*)((int)child + 0xD0) = *(unsigned int*)((int)event + 0xD0);
    *(unsigned int*)((int)child + 0xD4) = *(unsigned int*)((int)event + 0xD4);
    *(unsigned int*)((int)child + 0xD8) = *(unsigned int*)((int)event + 0xD8);
    *(unsigned int*)((int)child + 0xDC) = *(unsigned int*)((int)event + 0xDC);
    *(unsigned int*)((int)child + 0xE0) = *(unsigned int*)((int)event + 0xE0);
    *(unsigned int*)((int)child + 0xE4) = *(unsigned int*)((int)event + 0xE4);
    return 2;
}

s32 evt_inline_evt_id(void* event) {
    extern s32 evtSetValue(void*, s32, s32);
    unsigned int* start;
    unsigned int* ip;
    unsigned int cmd;
    s32 dst;
    void* child;

    dst = **(s32**)((int)event + 0x18);
    start = *(unsigned int**)((int)event + 0x14);
    ip = start;
    do {
        cmd = *ip++;
        ip += (int)cmd >> 0x10;
    } while ((cmd & 0xFFFF) != 0x6D);

    *(unsigned int**)((int)event + 0x14) = ip;
    child = evtEntryType(start, *(unsigned char*)((int)event + 0xB), 0x60, *(unsigned char*)((int)event + 0xC));
    *(unsigned int*)((int)child + 0x160) = *(unsigned int*)((int)event + 0x160);
    *(unsigned int*)((int)child + 0x154) = *(unsigned int*)((int)event + 0x154);
    *(unsigned int*)((int)child + 0x158) = *(unsigned int*)((int)event + 0x158);
    *(unsigned int*)((int)child + 0x170) = *(unsigned int*)((int)event + 0x170);
    *(unsigned int*)((int)child + 0x17C) = *(unsigned int*)((int)event + 0x17C);
    *(unsigned int*)((int)child + 0x178) = *(unsigned int*)((int)event + 0x178);
    *(unsigned int*)((int)child + 0x180) = *(unsigned int*)((int)event + 0x180);
    *(unsigned int*)((int)child + 0x184) = *(unsigned int*)((int)event + 0x184);
    *(unsigned int*)((int)child + 0x188) = *(unsigned int*)((int)event + 0x188);
    *(unsigned int*)((int)child + 0x18C) = *(unsigned int*)((int)event + 0x18C);
    *(unsigned int*)((int)child + 0x190) = *(unsigned int*)((int)event + 0x190);
    *(unsigned int*)((int)child + 0x9C) = *(unsigned int*)((int)event + 0x9C);
    *(unsigned int*)((int)child + 0xA0) = *(unsigned int*)((int)event + 0xA0);
    *(unsigned int*)((int)child + 0xA4) = *(unsigned int*)((int)event + 0xA4);
    *(unsigned int*)((int)child + 0xA8) = *(unsigned int*)((int)event + 0xA8);
    *(unsigned int*)((int)child + 0xAC) = *(unsigned int*)((int)event + 0xAC);
    *(unsigned int*)((int)child + 0xB0) = *(unsigned int*)((int)event + 0xB0);
    *(unsigned int*)((int)child + 0xB4) = *(unsigned int*)((int)event + 0xB4);
    *(unsigned int*)((int)child + 0xB8) = *(unsigned int*)((int)event + 0xB8);
    *(unsigned int*)((int)child + 0xBC) = *(unsigned int*)((int)event + 0xBC);
    *(unsigned int*)((int)child + 0xC0) = *(unsigned int*)((int)event + 0xC0);
    *(unsigned int*)((int)child + 0xC4) = *(unsigned int*)((int)event + 0xC4);
    *(unsigned int*)((int)child + 0xC8) = *(unsigned int*)((int)event + 0xC8);
    *(unsigned int*)((int)child + 0xCC) = *(unsigned int*)((int)event + 0xCC);
    *(unsigned int*)((int)child + 0xD0) = *(unsigned int*)((int)event + 0xD0);
    *(unsigned int*)((int)child + 0xD4) = *(unsigned int*)((int)event + 0xD4);
    *(unsigned int*)((int)child + 0xD8) = *(unsigned int*)((int)event + 0xD8);
    *(unsigned int*)((int)child + 0xDC) = *(unsigned int*)((int)event + 0xDC);
    *(unsigned int*)((int)child + 0xE0) = *(unsigned int*)((int)event + 0xE0);
    *(unsigned int*)((int)child + 0xE4) = *(unsigned int*)((int)event + 0xE4);
    evtSetValue(event, dst, *(s32*)((int)child + 0x15C));
    return 2;
}

s32 evt_brother_evt(void* event) {
    extern void* evtBrotherEntry(void*, void*, unsigned int);
    s32* script = *(s32**)((int)event + 0x14);
    s32* brotherEnd;
    void* copy;
    s32 opcode;

    brotherEnd = script;
    do {
        opcode = *brotherEnd & 0xFFFF;
        brotherEnd += *brotherEnd++ >> 16;
    } while (opcode != 0x70);
    *(s32**)((int)event + 0x14) = brotherEnd;

    copy = evtBrotherEntry(event, script, 0x60);
    *(unsigned int*)((int)copy + 0x160) = *(unsigned int*)((int)event + 0x160);
    *(unsigned char*)((int)copy + 0xC) = *(unsigned char*)((int)event + 0xC);
    *(unsigned int*)((int)copy + 0x154) = *(unsigned int*)((int)event + 0x154);
    *(unsigned int*)((int)copy + 0x158) = *(unsigned int*)((int)event + 0x158);
    *(unsigned int*)((int)copy + 0x170) = *(unsigned int*)((int)event + 0x170);
    *(unsigned int*)((int)copy + 0x150) = *(unsigned int*)((int)event + 0x150);
    return 2;
}

s32 evt_brother_evt_id(void* event) {
    extern void* evtBrotherEntry(void*, void*, unsigned int);
    extern s32 evtSetValue(void*, s32, s32);
    s32* args = *(s32**)((int)event + 0x18);
    s32* script = *(s32**)((int)event + 0x14);
    s32* brotherEnd;
    void* copy;
    s32 opcode;
    s32 index = args[0];

    brotherEnd = script;
    do {
        opcode = *brotherEnd & 0xFFFF;
        brotherEnd += *brotherEnd++ >> 16;
    } while (opcode != 0x70);
    *(s32**)((int)event + 0x14) = brotherEnd;

    copy = evtBrotherEntry(event, script, 0x60);
    *(unsigned int*)((int)copy + 0x160) = *(unsigned int*)((int)event + 0x160);
    *(unsigned char*)((int)copy + 0xC) = *(unsigned char*)((int)event + 0xC);
    *(unsigned int*)((int)copy + 0x154) = *(unsigned int*)((int)event + 0x154);
    *(unsigned int*)((int)copy + 0x158) = *(unsigned int*)((int)event + 0x158);
    *(unsigned int*)((int)copy + 0x170) = *(unsigned int*)((int)event + 0x170);
    evtSetValue(event, index, *(s32*)((int)copy + 0x15C));
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

extern const char str_ADDR_PCT08X_802bffb8[];
extern const char str_FLOAT_PCT4_2f_802bffc8[];
extern const char str_UF_PCT3d_PCTd_802bffdc[];
extern const char str_UW_PCT3d_PCT08X_802bffec[];
extern const char str_UW_PCT3d_PCT4_2f_802bfffc[];
extern const char str_UW_PCT3d_PCTd_802c0010[];
extern const char str_GSW_PCT3d_PCT08X_802c0020[];
extern const char str_GSW_PCT3d_PCT4_2f_802c0030[];
extern const char str_GSW_PCT3d_PCTd_802c0044[];
extern const char str_LSW_PCT3d_PCT08X_802c0054[];
extern const char str_LSW_PCT3d_PCT4_2f_802c0064[];
extern const char str_LSW_PCT3d_PCTd_802c0078[];
extern const char str_GSWF_PCT3d_PCTd_802c0088[];
extern const char str_LSWF_PCT3d_PCTd_802c0098[];
extern const char str_GF_PCT3d_PCTd_802c00a8[];
extern const char str_LF_PCT3d_PCTd_802c00b8[];
extern const char str_GW_PCT3d_PCT08X_802c00c8[];
extern const char str_GW_PCT3d_PCT4_2f_802c00d8[];
extern const char str_GW_PCT3d_PCTd_802c00ec[];
extern const char str_LW_PCT3d_PCT08X_802c00fc[];
extern const char str_LW_PCT3d_PCT4_2f_802c010c[];
extern const char str_LW_PCT3d_PCTd_802c0120[];
extern const char str_PCTd_802c0130[];

int evt_debug_put_reg(void* param_1) {
    extern int sprintf(char* str, const char* format, ...);
    extern s32 swByteGet(s32 index);
    extern s32 _swByteGet(s32 index);
    extern s32 swGet(s32 index);
    extern s32 _swGet(s32 index);

    static char str[256];
    s32* args;
    u8* work;
    u8* entry;
    s32 reg;
    s32 mask;
    s32 data;
    s32 val;
    s32 converted;
    f32 fvalue;

    entry = param_1;
    args = *(s32**)(entry + 0x18);
    work = evtGetWork();
    reg = args[0];

    if (reg <= -270000000) {
        sprintf(str, str_ADDR_PCT08X_802bffb8, reg);
    } else if (reg <= -220000000) {
        converted = reg;
        if (converted <= -220000000) {
            converted += 230000000;
            fvalue = converted;
            fvalue /= 1024.0f;
        } else {
            fvalue = converted;
        }
        sprintf(str, str_FLOAT_PCT4_2f_802bffc8, (f64)fvalue);
    } else if (reg <= -200000000) {
        reg += 210000000;
        mask = 1 << (reg % 32);
        data = *(s32*)(*(s32*)(entry + 0x158) + (reg / 32) * 4);
        sprintf(str, str_UF_PCT3d_PCTd_802bffdc, reg, mask & data);
    } else if (reg <= -180000000) {
        reg += 190000000;
        val = *(s32*)(*(s32*)(entry + 0x154) + reg * 4);
        if (val <= -270000000) {
            sprintf(str, str_UW_PCT3d_PCT08X_802bffec, val, val);
        } else if (val <= -220000000) {
            converted = val;
            if (converted <= -220000000) {
                converted += 230000000;
                fvalue = converted;
                fvalue /= 1024.0f;
            } else {
                fvalue = converted;
            }
            sprintf(str, str_UW_PCT3d_PCT4_2f_802bfffc, reg, (f64)fvalue);
        } else {
            sprintf(str, str_UW_PCT3d_PCTd_802c0010, reg, val);
        }
    } else if (reg <= -160000000) {
        reg += 170000000;
        val = swByteGet(reg);
        if (val <= -270000000) {
            sprintf(str, str_GSW_PCT3d_PCT08X_802c0020, val);
        } else if (val <= -220000000) {
            converted = val;
            if (converted <= -220000000) {
                converted += 230000000;
                fvalue = converted;
                fvalue /= 1024.0f;
            } else {
                fvalue = converted;
            }
            sprintf(str, str_GSW_PCT3d_PCT4_2f_802c0030, reg, (f64)fvalue);
        } else {
            sprintf(str, str_GSW_PCT3d_PCTd_802c0044, reg, val);
        }
    } else if (reg <= -140000000) {
        reg += 150000000;
        val = _swByteGet(reg);
        if (val <= -270000000) {
            sprintf(str, str_LSW_PCT3d_PCT08X_802c0054, val);
        } else if (val <= -220000000) {
            converted = val;
            if (converted <= -220000000) {
                converted += 230000000;
                fvalue = converted;
                fvalue /= 1024.0f;
            } else {
                fvalue = converted;
            }
            sprintf(str, str_LSW_PCT3d_PCT4_2f_802c0064, reg, (f64)fvalue);
        } else {
            sprintf(str, str_LSW_PCT3d_PCTd_802c0078, reg, val);
        }
    } else if (reg <= -120000000) {
        reg += 130000000;
        sprintf(str, str_GSWF_PCT3d_PCTd_802c0088, reg, swGet(reg));
    } else if (reg <= -100000000) {
        reg += 110000000;
        sprintf(str, str_LSWF_PCT3d_PCTd_802c0098, reg, _swGet(reg));
    } else if (reg <= -80000000) {
        reg += 90000000;
        mask = 1 << (reg % 32);
        data = *(s32*)(work + 0x84 + (reg / 32) * 4);
        sprintf(str, str_GF_PCT3d_PCTd_802c00a8, reg, data & mask);
    } else if (reg <= -60000000) {
        reg += 70000000;
        mask = 1 << (reg % 32);
        data = *(s32*)(entry + 0xDC + (reg / 32) * 4);
        sprintf(str, str_LF_PCT3d_PCTd_802c00b8, reg, data & mask);
    } else if (reg <= -40000000) {
        reg += 50000000;
        val = *(s32*)(work + 0x4 + reg * 4);
        if (val <= -270000000) {
            sprintf(str, str_GW_PCT3d_PCT08X_802c00c8, reg, val);
        } else if (val <= -220000000) {
            converted = val;
            if (converted <= -220000000) {
                converted += 230000000;
                fvalue = converted;
                fvalue /= 1024.0f;
            } else {
                fvalue = converted;
            }
            sprintf(str, str_GW_PCT3d_PCT4_2f_802c00d8, reg, (f64)fvalue);
        } else {
            sprintf(str, str_GW_PCT3d_PCTd_802c00ec, reg, val);
        }
    } else if (reg <= -20000000) {
        reg += 30000000;
        val = *(s32*)(entry + 0x9C + reg * 4);
        if (val <= -270000000) {
            sprintf(str, str_LW_PCT3d_PCT08X_802c00fc, reg, val);
        } else if (val <= -220000000) {
            converted = val;
            if (converted <= -220000000) {
                converted += 230000000;
                fvalue = converted;
                fvalue /= 1024.0f;
            } else {
                fvalue = converted;
            }
            sprintf(str, str_LW_PCT3d_PCT4_2f_802c010c, reg, (f64)fvalue);
        } else {
            sprintf(str, str_LW_PCT3d_PCTd_802c0120, reg, val);
        }
    } else {
        sprintf(str, str_PCTd_802c0130, reg);
    }

    return 2;
}

const char str_ADDR_PCT08X_802bffb8[] = "ADDR:%08X";
const char str_FLOAT_PCT4_2f_802bffc8[] = "FLOAT:%4.2f";
const char str_UF_PCT3d_PCTd_802bffdc[] = "UF:%3d:%d";
const char str_UW_PCT3d_PCT08X_802bffec[] = "UW:%08X";
const char str_UW_PCT3d_PCT4_2f_802bfffc[] = "UW:%3d:%4.2f";
const char str_UW_PCT3d_PCTd_802c0010[] = "UW:%3d:%d";
const char str_GSW_PCT3d_PCT08X_802c0020[] = "GSW:%08X";
const char str_GSW_PCT3d_PCT4_2f_802c0030[] = "GSW:%3d:%4.2f";
const char str_GSW_PCT3d_PCTd_802c0044[] = "GSW:%3d:%d";
const char str_LSW_PCT3d_PCT08X_802c0054[] = "LSW:%08X";
const char str_LSW_PCT3d_PCT4_2f_802c0064[] = "LSW:%3d:%4.2f";
const char str_LSW_PCT3d_PCTd_802c0078[] = "LSW:%3d:%d";
const char str_GSWF_PCT3d_PCTd_802c0088[] = "GSWF:%3d:%d";
const char str_LSWF_PCT3d_PCTd_802c0098[] = "LSWF:%3d:%d";
const char str_GF_PCT3d_PCTd_802c00a8[] = "GF:%3d:%d";
const char str_LF_PCT3d_PCTd_802c00b8[] = "LF:%3d:%d";
const char str_GW_PCT3d_PCT08X_802c00c8[] = "GW:%3d:%08X";
const char str_GW_PCT3d_PCT4_2f_802c00d8[] = "GW:%3d:%4.2f";
const char str_GW_PCT3d_PCTd_802c00ec[] = "GW:%3d:%d";
const char str_LW_PCT3d_PCT08X_802c00fc[] = "LW:%3d:%08X";
const char str_LW_PCT3d_PCT4_2f_802c010c[] = "LW:%3d:%4.2f";
const char str_LW_PCT3d_PCTd_802c0120[] = "LW:%3d:%d";
const char str_PCTd_802c0130[0xE] = "%d";

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



s32 evtmgrCmd(struct EventEntry* entry) {
    extern void evtDelete(void*);
    extern void* evtChildEntry(void*, void*, u32);
    extern void evtSetPri(void*, s32);
    extern void evtSetSpeed(void*, s32);
    extern void evtSetType(void*, s32);
    extern void* evtRestart(void*);
    extern void evtStopAll(u32);
    extern void evtStartAll(u32);
    extern void evtStopOther(void*, u32);
    extern void evtStartOther(void*, u32);
    extern void evtStopID(s32);
    extern void evtStartID(s32);
    extern s32 evtCheckID(s32);
    extern void* evtGetPtr(s32);
    extern void* _mapAlloc(void*, s32);
    extern void* mapalloc_base_ptr;

    extern s32 evt_while(void*);
    extern s32 evt_wait_msec(void*);
    extern s32 evt_if_str_equal(void*);
    extern s32 evt_if_str_not_equal(void*);
    extern s32 evt_if_str_small(void*);
    extern s32 evt_if_str_large(void*);
    extern s32 evt_if_str_small_equal(void*);
    extern s32 evt_if_str_large_equal(void*);
    extern s32 evt_iff_equal(void*);
    extern s32 evt_iff_not_equal(void*);
    extern s32 evt_iff_small(void*);
    extern s32 evt_iff_large(s32);
    extern s32 evt_iff_small_equal(s32);
    extern s32 evt_iff_large_equal(s32);
    extern s32 evt_if_equal(void*);
    extern s32 evt_if_not_equal(s32);
    extern s32 evt_if_small(s32);
    extern s32 evt_if_large(s32);
    extern s32 evt_if_small_equal(s32);
    extern s32 evt_if_large_equal(void*);
    extern s32 evt_if_flag(s32);
    extern s32 evt_if_not_flag(s32);
    extern s32 evt_case_equal(void*);
    extern s32 evt_case_not_equal(void*);
    extern s32 evt_case_small(void*);
    extern s32 evt_case_large(void*);
    extern s32 evt_case_small_equal(s32);
    extern s32 evt_case_large_equal(s32);
    extern s32 evt_case_or(void*);
    extern s32 evt_case_and(s32);
    extern s32 evt_case_flag(s32);
    extern s32 evt_case_end(s32);
    extern s32 evt_case_between(s32);
    extern s32 evt_run_evt(void*);
    extern s32 evt_run_evt_id(void*);
    extern s32 evt_inline_evt(void*);
    extern s32 evt_inline_evt_id(void*);
    extern s32 evt_brother_evt(void*);
    extern s32 evt_brother_evt_id(void*);
    extern s32 evt_debug_put_reg(void*);

    u8* e;
    s32 state;
    s32 op;
    s32* args;
    u32* ip;
    u32* save;
    u32 cmd;
    s32 depth;
    s32 i;
    s32 v0;
    s32 v1;
    u32 u0;
    u32 u1;
    void* ptr;
    f32 f0;
    f32 f1;
    s32 (*sleepFunc)(void*, s32);

    e = (u8*)entry;

restart:
    state = 2;
    op = *(u8*)(e + 0x0A);

    switch (op) {
        case 0x00:
            state = 3;
            *(s32*)(e + 0x1A8) = *(s32*)(e + 0x14);
            args = *(s32**)(e + 0x14);
            *(u8*)(e + 0x0A) = (u8)*args;
            i = *args++ >> 16;
            *(u8*)(e + 0x09) = (u8)i;
            *(s32*)(e + 0x18) = (s32)args;
            args += i;
            *(s32*)(e + 0x14) = (s32)args;
            *(u8*)(e + 0x0D) = 0;
            break;

        case 0x01:
            evtDelete(entry);
            state = 0xFF;
            break;

        case 0x02:
            state = 2;
            break;

        case 0x03:
            args = *(s32**)(e + 0x18);
            ptr = (void*)evtGetValue(entry, args[0]);
            if ((s32)ptr > -0x1017DF81) {
                for (i = 0; i < 0x10; i++) {
                    if ((s32)*(s8*)(e + 0x1C + i) == (s32)ptr) {
                        ptr = *(void**)(e + 0x2C + i * 4);
                        break;
                    }
                    ptr = *(void**)(e + 0x18);
                }
            }
            *(s32*)(e + 0x14) = (s32)ptr;
            state = 2;
            break;

        case 0x04:
            args = *(s32**)(e + 0x18);
            v0 = *args++;
            *(u8*)(e + 0x0E) = *(u8*)(e + 0x0E) + 1;
            i = (s8)*(u8*)(e + 0x0E);
            *(s32*)(e + 0xE8 + i * 4) = (s32)args;
            *(s32*)(e + 0x108 + i * 4) = v0;
            state = 2;
            break;

        case 0x05:
            state = evt_while(entry);
            break;

        case 0x06:
            depth = 0;
            ip = *(u32**)(e + 0x14);
            while (1) {
                cmd = *ip;
                ip += ((s32)cmd >> 16) + 1;
                op = cmd & 0xFFFF;
                switch (op) {
                    case 0x01:
                    case 0x06:
                        if (--depth >= 0) {
                            break;
                        } else {
                            goto found_break;
                        }
                    case 0x05:
                        depth++;
                        break;
                }
            }
found_break:
            *(s32*)(e + 0x14) = (s32)ip;
            *(u8*)(e + 0x0E) = *(u8*)(e + 0x0E) - 1;
            state = 2;
            break;

        case 0x07:
            depth = 0;
            ip = *(u32**)(e + 0x14);
            do {
                cmd = *ip;
                op = cmd & 0xFFFF;
                switch (op) {
                    case 0x01:
                    case 0x06:
                        if (--depth >= 0) {
                            break;
                        } else {
                            *(s32*)(e + 0x14) = (s32)ip;
                            state = 2;
                            goto finish;
                        }
                    case 0x05:
                        depth++;
                        break;
                }
                ip += ((s32)cmd >> 16) + 1;
            } while (1);

        case 0x08:
            if (*(u8*)(e + 0x0D) == 0) {
                args = *(s32**)(e + 0x18);
                *(s32*)(e + 0x78) = evtGetValue(entry, args[0]);
                *(u8*)(e + 0x0D) = 1;
            }
            if (*(s32*)(e + 0x78) == 0) {
                state = 2;
                break;
            }
            *(s32*)(e + 0x78) = *(s32*)(e + 0x78) - 1;
            if (*(s32*)(e + 0x78) != 0) {
                state = 0;
            } else {
                state = 1;
            }
            break;

        case 0x09:
            state = evt_wait_msec(entry);
            break;

        case 0x0A:
            args = *(s32**)(e + 0x18);
            u0 = evtGetValue(entry, args[0]);
            state = (u0 == 0) ? 2 : 0;
            break;

        case 0x0B:
            state = 2;
            break;

        case 0x0C: state = evt_if_str_equal(entry); break;
        case 0x0D: state = evt_if_str_not_equal(entry); break;
        case 0x0E: state = evt_if_str_small(entry); break;
        case 0x0F: state = evt_if_str_large(entry); break;
        case 0x10: state = evt_if_str_small_equal(entry); break;
        case 0x11: state = evt_if_str_large_equal(entry); break;
        case 0x12: state = evt_iff_equal(entry); break;
        case 0x13: state = evt_iff_not_equal(entry); break;
        case 0x14: state = evt_iff_small(entry); break;
        case 0x15: state = evt_iff_large((s32)entry); break;
        case 0x16: state = evt_iff_small_equal((s32)entry); break;
        case 0x17: state = evt_iff_large_equal((s32)entry); break;
        case 0x18: state = evt_if_equal(entry); break;
        case 0x19: state = evt_if_not_equal((s32)entry); break;
        case 0x1A: state = evt_if_small((s32)entry); break;
        case 0x1B: state = evt_if_large((s32)entry); break;
        case 0x1C: state = evt_if_small_equal((s32)entry); break;
        case 0x1D: state = evt_if_large_equal(entry); break;
        case 0x1E: state = evt_if_flag((s32)entry); break;
        case 0x1F: state = evt_if_not_flag((s32)entry); break;

        case 0x20:
            depth = 0;
            ip = *(u32**)(e + 0x14);
            while (1) {
                cmd = *ip;
                op = cmd & 0xFFFF;
                ip += ((s32)cmd >> 16) + 1;
                switch (op) {
                    case 0x01:
                    case 0x21:
                        if (--depth >= 0) {
                            break;
                        } else {
                            goto found_end_if;
                        }
                    case 0x0C:
                    case 0x0D:
                    case 0x0E:
                    case 0x0F:
                    case 0x10:
                    case 0x11:
                    case 0x12:
                    case 0x13:
                    case 0x14:
                    case 0x15:
                    case 0x16:
                    case 0x17:
                    case 0x18:
                    case 0x19:
                    case 0x1A:
                    case 0x1B:
                    case 0x1C:
                    case 0x1D:
                    case 0x1E:
                    case 0x1F:
                        depth++;
                        break;
                    case 0x20:
                        if (depth == 0) {
                            goto found_end_if;
                        }
                        break;
                }
            }
found_end_if:
            *(s32*)(e + 0x14) = (s32)ip;
            state = 2;
            break;

        case 0x21:
            state = 2;
            break;

        case 0x22:
            args = *(s32**)(e + 0x18);
            u0 = evtGetValue(entry, args[0]);
            *(u8*)(e + 0x0F) = *(u8*)(e + 0x0F) + 1;
            i = (s8)*(u8*)(e + 0x0F);
            *(s32*)(e + 0x130 + i * 4) = (s32)u0;
            *(s8*)(e + 0x128 + i) = 1;
            state = 2;
            break;

        case 0x23:
            args = *(s32**)(e + 0x18);
            *(u8*)(e + 0x0F) = *(u8*)(e + 0x0F) + 1;
            i = (s8)*(u8*)(e + 0x0F);
            *(s32*)(e + 0x130 + i * 4) = args[0];
            *(s8*)(e + 0x128 + i) = 1;
            state = 2;
            break;

        case 0x24: state = evt_case_equal(entry); break;
        case 0x25: state = evt_case_not_equal(entry); break;
        case 0x26: state = evt_case_small(entry); break;
        case 0x27: state = evt_case_large(entry); break;
        case 0x28: state = evt_case_small_equal((s32)entry); break;
        case 0x29: state = evt_case_large_equal((s32)entry); break;

        case 0x2A:
            if (*(s8*)(e + 0x128 + (s8)*(u8*)(e + 0x0F)) < 1) {
                depth = 1;
                ip = *(u32**)(e + 0x14);
                while (1) {
                    save = ip;
                    cmd = *save;
                    op = cmd & 0xFFFF;
                    ip = save + ((s32)cmd >> 16) + 1;
                    switch (op) {
                        case 0x01:
                        case 0x22:
                            depth++;
                            break;
                        case 0x31:
                            if (--depth == 0) {
                                goto found_case_end_switch;
                            }
                            break;
                    }
                }
found_case_end_switch:
                *(s32*)(e + 0x14) = (s32)save;
            } else {
                *(s8*)(e + 0x128 + (s8)*(u8*)(e + 0x0F)) = 0;
            }
            state = 2;
            break;

        case 0x2B: state = evt_case_or(entry); break;
        case 0x2C: state = evt_case_and((s32)entry); break;
        case 0x2D: state = evt_case_flag((s32)entry); break;
        case 0x2E: state = evt_case_end((s32)entry); break;
        case 0x2F: state = evt_case_between((s32)entry); break;

        case 0x30:
            depth = 1;
            ip = *(u32**)(e + 0x14);
            while (1) {
                save = ip;
                cmd = *save;
                op = cmd & 0xFFFF;
                ip = save + ((s32)cmd >> 16) + 1;
                switch (op) {
                    case 0x01:
                    case 0x22:
                        depth++;
                        break;
                    case 0x31:
                        if (--depth == 0) {
                            goto found_switch_break;
                        }
                        break;
                }
            }
found_switch_break:
            *(s32*)(e + 0x14) = (s32)save;
            state = 2;
            break;

        case 0x31:
            state = 2;
            *(s8*)(e + 0x128 + (s8)*(u8*)(e + 0x0F)) = 0;
            *(u8*)(e + 0x0F) = *(u8*)(e + 0x0F) - 1;
            break;

        case 0x32:
            args = *(s32**)(e + 0x18);
            evtSetValue(entry, args[0], evtGetValue(entry, args[1]));
            state = 2;
            break;

        case 0x33:
            args = *(s32**)(e + 0x18);
            evtSetValue(entry, args[0], args[1]);
            state = 2;
            break;

        case 0x34:
            args = *(s32**)(e + 0x18);
            evtSetFloat(entry, args[0], evtGetFloat(entry, args[1]));
            state = 2;
            break;

        case 0x35:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            v1 = evtGetValue(entry, args[1]);
            evtSetValue(entry, v0, evtGetValue(entry, v0) + v1);
            state = 2;
            break;

        case 0x36:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            v1 = evtGetValue(entry, args[1]);
            evtSetValue(entry, v0, evtGetValue(entry, v0) - v1);
            state = 2;
            break;

        case 0x37:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            v1 = evtGetValue(entry, args[1]);
            evtSetValue(entry, v0, evtGetValue(entry, v0) * v1);
            state = 2;
            break;

        case 0x38:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            v1 = evtGetValue(entry, args[1]);
            evtSetValue(entry, v0, evtGetValue(entry, v0) / v1);
            state = 2;
            break;

        case 0x39:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            v1 = (s32)(evtGetValue(entry, args[1]) + 0.5f);
            i = (s32)(evtGetValue(entry, v0) + 0.5f);
            evtSetValue(entry, v0, i % v1);
            state = 2;
            break;

        case 0x3A:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            f0 = evtGetFloat(entry, args[1]);
            f1 = evtGetFloat(entry, v0);
            evtSetFloat(entry, v0, f1 + f0);
            state = 2;
            break;

        case 0x3B:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            f0 = evtGetFloat(entry, args[1]);
            f1 = evtGetFloat(entry, v0);
            evtSetFloat(entry, v0, f1 - f0);
            state = 2;
            break;

        case 0x3C:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            f0 = evtGetFloat(entry, args[1]);
            f1 = evtGetFloat(entry, v0);
            evtSetFloat(entry, v0, f1 * f0);
            state = 2;
            break;

        case 0x3D:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            f0 = evtGetFloat(entry, args[1]);
            f1 = evtGetFloat(entry, v0);
            evtSetFloat(entry, v0, f1 / f0);
            state = 2;
            break;

        case 0x3E:
            args = *(s32**)(e + 0x18);
            *(s32*)(e + 0x150) = evtGetValue(entry, args[0]);
            state = 2;
            break;

        case 0x3F:
            args = *(s32**)(e + 0x18);
            ip = *(u32**)(e + 0x150);
            *(s32*)(e + 0x150) = (s32)(ip + 1);
            evtSetValue(entry, args[0], *ip);
            state = 2;
            break;

        case 0x40:
            args = *(s32**)(e + 0x18);
            ip = *(u32**)(e + 0x150);
            evtSetValue(entry, args[0], *ip++);
            evtSetValue(entry, args[1], *ip++);
            *(u32**)(e + 0x150) = ip;
            state = 2;
            break;

        case 0x41:
            args = *(s32**)(e + 0x18);
            ip = *(u32**)(e + 0x150);
            evtSetValue(entry, args[0], *ip++);
            evtSetValue(entry, args[1], *ip++);
            evtSetValue(entry, args[2], *ip++);
            *(u32**)(e + 0x150) = ip;
            state = 2;
            break;

        case 0x42:
            args = *(s32**)(e + 0x18);
            ip = *(u32**)(e + 0x150);
            evtSetValue(entry, args[0], *ip++);
            evtSetValue(entry, args[1], *ip++);
            evtSetValue(entry, args[2], *ip++);
            evtSetValue(entry, args[3], *ip++);
            *(u32**)(e + 0x150) = ip;
            state = 2;
            break;

        case 0x43:
            args = *(s32**)(e + 0x18);
            ip = *(u32**)(e + 0x150);
            v0 = evtGetValue(entry, args[1]);
            evtSetValue(entry, args[0], *(s32*)((s32)ip + v0 * 4));
            state = 2;
            break;

        case 0x44:
            args = *(s32**)(e + 0x18);
            *(s32*)(e + 0x150) = evtGetValue(entry, args[0]);
            state = 2;
            break;

        case 0x45:
            args = *(s32**)(e + 0x18);
            ptr = *(void**)(e + 0x150);
            *(s32*)(e + 0x150) = (s32)ptr + 4;
            evtSetFloat(entry, args[0], *(f32*)ptr);
            state = 2;
            break;

        case 0x46:
            args = *(s32**)(e + 0x18);
            ip = *(u32**)(e + 0x150);
            evtSetFloat(entry, args[0], *(f32*)ip++);
            evtSetFloat(entry, args[1], *(f32*)ip++);
            *(u32**)(e + 0x150) = ip;
            state = 2;
            break;

        case 0x47:
            args = *(s32**)(e + 0x18);
            ip = *(u32**)(e + 0x150);
            evtSetFloat(entry, args[0], *(f32*)ip++);
            evtSetFloat(entry, args[1], *(f32*)ip++);
            evtSetFloat(entry, args[2], *(f32*)ip++);
            *(u32**)(e + 0x150) = ip;
            state = 2;
            break;

        case 0x48:
            args = *(s32**)(e + 0x18);
            ip = *(u32**)(e + 0x150);
            evtSetFloat(entry, args[0], *(f32*)ip++);
            evtSetFloat(entry, args[1], *(f32*)ip++);
            evtSetFloat(entry, args[2], *(f32*)ip++);
            evtSetFloat(entry, args[3], *(f32*)ip++);
            *(u32**)(e + 0x150) = ip;
            state = 2;
            break;

        case 0x49:
            args = *(s32**)(e + 0x18);
            v0 = evtGetValue(entry, args[1]);
            evtSetFloat(entry, args[0], (f32)*(s32*)(*(s32*)(e + 0x150) + v0 * 4));
            state = 2;
            break;

        case 0x4A:
            args = *(s32**)(e + 0x18);
            *(s32*)(e + 0x154) = evtGetValue(entry, args[0]);
            state = 2;
            break;

        case 0x4B:
            args = *(s32**)(e + 0x18);
            *(s32*)(e + 0x158) = evtGetValue(entry, args[0]);
            state = 2;
            break;

        case 0x4C:
            args = *(s32**)(e + 0x18);
            u0 = evtGetValue(entry, args[0]);
            ptr = _mapAlloc(mapalloc_base_ptr, u0 << 2);
            *(s32*)(e + 0x154) = (s32)ptr;
            evtSetValue(entry, args[1], (s32)ptr);
            state = 2;
            break;

        case 0x4D:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            evtSetValue(entry, v0, evtGetValue(entry, v0) & evtGetValue(entry, args[1]));
            state = 2;
            break;

        case 0x4E:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            evtSetValue(entry, v0, evtGetValue(entry, v0) & args[1]);
            state = 2;
            break;

        case 0x4F:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            evtSetValue(entry, v0, evtGetValue(entry, v0) | evtGetValue(entry, args[1]));
            state = 2;
            break;

        case 0x50:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            evtSetValue(entry, v0, evtGetValue(entry, v0) | args[1]);
            state = 2;
            break;

        case 0x51:
            args = *(s32**)(e + 0x18);
            u0 = evtGetValue(entry, args[1]);
            v0 = (s32)(u0 * 60) / 1000 + ((s32)(u0 * 60) >> 31);
            evtSetValue(entry, args[0], v0 - (v0 >> 31));
            state = 2;
            break;

        case 0x52:
            args = *(s32**)(e + 0x18);
            u0 = evtGetValue(entry, args[1]);
            v0 = (s32)(u0 * 1000) / 60 + ((s32)(u0 * 1000) >> 31);
            evtSetValue(entry, args[0], v0 - (v0 >> 31));
            state = 2;
            break;

        case 0x53:
            args = *(s32**)(e + 0x18);
            *(s32*)args[1] = evtGetValue(entry, args[0]);
            state = 2;
            break;

        case 0x54:
            args = *(s32**)(e + 0x18);
            *(f32*)args[1] = evtGetFloat(entry, args[0]);
            state = 2;
            break;

        case 0x55:
            args = *(s32**)(e + 0x18);
            evtSetValue(entry, args[0], **(s32**)(args + 1));
            state = 2;
            break;

        case 0x56:
            args = *(s32**)(e + 0x18);
            evtSetFloat(entry, args[0], **(f32**)(args + 1));
            state = 2;
            break;

        case 0x57:
            args = *(s32**)(e + 0x18);
            evtSetValue(entry, evtGetValue(entry, args[0]), evtGetValue(entry, args[1]));
            state = 2;
            break;

        case 0x58:
            args = *(s32**)(e + 0x18);
            evtSetFloat(entry, evtGetValue(entry, args[0]), evtGetFloat(entry, args[1]));
            state = 2;
            break;

        case 0x59:
            args = *(s32**)(e + 0x18);
            u0 = evtGetValue(entry, args[0]);
            evtSetValue(entry, args[1], evtGetValue(entry, u0));
            state = 2;
            break;

        case 0x5A:
            args = *(s32**)(e + 0x18);
            u0 = evtGetValue(entry, args[0]);
            evtSetFloat(entry, args[1], evtGetFloat(entry, u0));
            state = 2;
            break;

        case 0x5B:
            args = *(s32**)(e + 0x18);
            if (*(u8*)(e + 0x0D) == 0) {
                u0 = evtGetValue(entry, args[0]);
                *(s32*)(e + 0x98) = u0;
                *(u8*)(e + 0x09) = *(u8*)(e + 0x09) - 1;
                *(s32*)(e + 0x18) = (s32)(args + 1);
                *(u8*)(e + 0x0D) = 1;
                sleepFunc = (s32 (*)(void*, s32))u0;
                state = sleepFunc(entry, 1);
            } else {
                sleepFunc = (s32 (*)(void*, s32))*(s32*)(e + 0x98);
                state = sleepFunc(entry, 0);
            }
            break;

        case 0x5C: state = evt_run_evt(entry); break;
        case 0x5D: state = evt_run_evt_id(entry); break;

        case 0x5E:
            args = *(s32**)(e + 0x18);
            ptr = (void*)evtGetValue(entry, args[0]);
            evtChildEntry(entry, ptr, 0);
            state = 0xFF;
            *(u8*)(e + 0x0A) = 0;
            break;

        case 0x5F:
            args = *(s32**)(e + 0x18);
            evtDeleteID(evtGetValue(entry, args[0]));
            state = 2;
            break;

        case 0x60:
            args = *(s32**)(e + 0x18);
            *(s32*)(e + 0x1A0) = evtGetValue(entry, args[0]);
            evtRestart(entry);
            state = 2;
            break;

        case 0x61:
            args = *(s32**)(e + 0x18);
            evtSetPri(entry, evtGetValue(entry, args[0]));
            state = 2;
            break;

        case 0x62:
            args = *(s32**)(e + 0x18);
            u0 = args[0];
            evtGetFloat(entry, u0);
            evtSetSpeed(entry, u0);
            state = 2;
            break;

        case 0x63:
            args = *(s32**)(e + 0x18);
            evtSetType(entry, evtGetValue(entry, args[0]));
            state = 2;
            break;

        case 0x64:
            args = *(s32**)(e + 0x18);
            evtStopAll(evtGetValue(entry, args[0]));
            state = 2;
            break;

        case 0x65:
            args = *(s32**)(e + 0x18);
            evtStartAll(evtGetValue(entry, args[0]));
            state = 2;
            break;

        case 0x66:
            args = *(s32**)(e + 0x18);
            evtStopOther(entry, evtGetValue(entry, args[0]));
            state = 2;
            break;

        case 0x67:
            args = *(s32**)(e + 0x18);
            evtStartOther(entry, evtGetValue(entry, args[0]));
            state = 2;
            break;

        case 0x68:
            args = *(s32**)(e + 0x18);
            evtStopID(evtGetValue(entry, args[0]));
            state = 2;
            break;

        case 0x69:
            args = *(s32**)(e + 0x18);
            evtStartID(evtGetValue(entry, args[0]));
            state = 2;
            break;

        case 0x6A:
            args = *(s32**)(e + 0x18);
            v0 = evtGetValue(entry, args[0]);
            evtSetValue(entry, args[1], evtCheckID(v0));
            state = 2;
            break;

        case 0x6B: state = evt_inline_evt(entry); break;
        case 0x6C: state = evt_inline_evt_id(entry); break;

        case 0x6D:
            evtDelete(entry);
            state = 0xFF;
            break;

        case 0x6E: state = evt_brother_evt(entry); break;
        case 0x6F: state = evt_brother_evt_id(entry); break;

        case 0x70:
            evtDelete(entry);
            state = 0;
            break;

        case 0x71:
            state = 2;
            break;

        case 0x72:
            state = 2;
            break;

        case 0x73:
            state = evt_debug_put_reg(entry);
            break;

        case 0x74:
            args = *(s32**)(e + 0x18);
            *(s32*)(e + 0x1A4) = args[0];
            state = 2;
            break;

        case 0x75:
            state = 2;
            break;

        case 0x76:
            for (i = 0; i < 0x100; i++) {
                if (evtGetPtr(i) == entry) {
                    break;
                }
            }
            state = 1;
            break;

        default:
            state = 2;
            break;
    }

finish:
    if (state != 3) {
        if (state == 0xFF) {
            return -1;
        }
        if (state < 0) {
            return 1;
        }
        if (state == 0) {
            return 0;
        }
        if (state == 1) {
            *(u8*)(e + 0x0A) = 0;
            return 0;
        }
        if (state == 2) {
            *(u8*)(e + 0x0A) = 0;
        }
    }
    goto restart;
}

s32 evtGetValue(struct EventEntry* entry, s32 index) {
    extern s32 swByteGet(s32);
    extern s32 _swByteGet(s32);
    extern s32 swGet(s32);
    extern s32 _swGet(s32);

    typedef struct EvtGetValueWork {
        u8 pad_0[4];
        s32 gwData[32];
        u32 gfData[32];
    } EvtGetValueWork;
    typedef struct EvtGetValueEntry {
        u8 pad_0[0x9C];
        s32 lwData[16];
        u32 lfData[30];
        s32* uwBase;
        u32* ufBase;
    } EvtGetValueEntry;
    EvtGetValueWork* wp = evtGetWork();
    EvtGetValueEntry* ep = (EvtGetValueEntry*)entry;
    s32 mask;
    s32 data;
    f32 converted;

    if (index <= -270000000) {
        return index;
    }
    if (index <= -250000000) {
        return index;
    }
    if (index <= -220000000) {
        if (index <= -220000000) {
            index += 230000000;
            converted = index;
            converted /= 1024.0f;
        } else {
            converted = index;
        }
        return (s32)converted;
    }
    if (index <= -200000000) {
        index += 210000000;
        mask = 1 << (index % 32);
        data = ep->ufBase[index / 32];
        index = mask & data;
        return !(index == 0);
    }
    if (index <= -180000000) {
        index += 190000000;
        data = ep->uwBase[index];
        if (data <= -270000000) {
            return data;
        }
        if (data <= -220000000) {
            if (data <= -220000000) {
                data += 230000000;
                converted = data;
                converted /= 1024.0f;
            } else {
                converted = data;
            }
            data = (s32)converted;
        }
        return data;
    }
    if (index <= -160000000) {
        return swByteGet(index + 170000000);
    }
    if (index <= -140000000) {
        return _swByteGet(index + 150000000);
    }
    if (index <= -120000000) {
        return swGet(index + 130000000);
    }
    if (index <= -100000000) {
        return _swGet(index + 110000000);
    }
    if (index <= -80000000) {
        index += 90000000;
        mask = 1 << (index % 32);
        data = wp->gfData[index / 32];
        return (data & mask) != 0;
    }
    if (index <= -60000000) {
        index += 70000000;
        mask = 1 << (index % 32);
        data = ep->lfData[index / 32];
        return (data & mask) != 0;
    }
    if (index <= -40000000) {
        index += 50000000;
        data = wp->gwData[index];
        if (data <= -270000000) {
            return data;
        }
        if (data <= -220000000) {
            if (data <= -220000000) {
                data += 230000000;
                converted = data;
                converted /= 1024.0f;
            } else {
                converted = data;
            }
            data = (s32)converted;
        }
        return data;
    }
    if (index <= -20000000) {
        index += 30000000;
        data = ep->lwData[index];
        if (data <= -270000000) {
            return data;
        }
        if (data <= -220000000) {
            if (data <= -220000000) {
                data += 230000000;
                converted = data;
                converted /= 1024.0f;
            } else {
                converted = data;
            }
            data = (s32)converted;
        }
        return data;
    }
    return index;
}

s32 evtGetNumber(struct EventEntry* entry, s32 value) {
    (void)entry;

    if (value <= (s32)0xEFE82080) {
        return value;
    }
    if (value <= (s32)0xF1194D80) {
        return value;
    }
    if (value <= (s32)0xF2E31100) {
        return value;
    }
    if (value <= (s32)0xF4143E00) {
        return value + 0x0C845880;
    }
    if (value <= (s32)0xF5456B00) {
        return value + 0x0B532B80;
    }
    if (value <= (s32)0xF6769800) {
        return value + 0x0A21FE80;
    }
    if (value <= (s32)0xF7A7C500) {
        return value + 0x08F0D180;
    }
    if (value <= (s32)0xF8D8F200) {
        return value + 0x07BFA480;
    }
    if (value <= (s32)0xFA0A1F00) {
        return value + 0x068E7780;
    }
    if (value <= (s32)0xFB3B4C00) {
        return value + 0x055D4A80;
    }
    if (value <= (s32)0xFC6C7900) {
        return value + 0x042C1D80;
    }
    if (value <= (s32)0xFD9DA600) {
        return value + 0x02FAF080;
    }
    if (value <= (s32)0xFECED300) {
        value += 0x01C9C380;
    }
    return value;
}

s32 evtSetValue(struct EventEntry* entry, s32 index, s32 value) {
    extern s32 swByteGet(s32);
    extern void swByteSet(s32, s32);
    extern s32 _swByteGet(s32);
    extern void _swByteSet(s32, s32);
    extern s32 swGet(s32);
    extern void swSet(s32);
    extern void swClear(s32);
    extern s32 _swGet(s32);
    extern void _swSet(s32);
    extern void _swClear(s32);
    typedef struct EvtSetValueWork {
        u8 pad_0[4];
        s32 gwData[32];
        u32 gfData[32];
    } EvtSetValueWork;
    typedef struct EvtSetValueEntry {
        u8 pad_0[0x9C];
        s32 lwData[16];
        u32 lfData[30];
        s32* uwBase;
        u32* ufBase;
    } EvtSetValueEntry;
    EvtSetValueWork* wp = evtGetWork();
    EvtSetValueEntry* ep = (EvtSetValueEntry*)entry;
    s32 result;
    s32 shift;

    if (index <= -270000000) {
        return value;
    }
    if (index <= -220000000) {
        if (value <= -220000000) {
            return (s32)((f32)(value + 230000000) * 0.0009765625f);
        }
        return value;
    }
    if (index <= -200000000) {
        index += 210000000;
        shift = index % 32;
        if (value != 0) {
            ep->ufBase[index / 32] |= 1 << shift;
        } else {
            ep->ufBase[index / 32] &= ~(1 << shift);
        }
        return value;
    }
    if (index <= -180000000) {
        index += 190000000;
        result = ep->uwBase[index];
        ep->uwBase[index] = value;
        return result;
    }
    if (index <= -160000000) {
        index += 170000000;
        result = swByteGet(index);
        swByteSet(index, value);
        return result;
    }
    if (index <= -140000000) {
        index += 150000000;
        result = _swByteGet(index);
        _swByteSet(index, (u8)value);
        return result;
    }
    if (index <= -120000000) {
        index += 130000000;
        result = swGet(index);
        if (value != 0) {
            swSet(index);
        } else {
            swClear(index);
        }
        return result;
    }
    if (index <= -100000000) {
        index += 110000000;
        result = _swGet(index);
        if (value != 0) {
            _swSet(index);
        } else {
            _swClear(index);
        }
        return result;
    }
    if (index <= -80000000) {
        index += 90000000;
        shift = index % 32;
        if (value != 0) {
            wp->gfData[index / 32] |= 1 << shift;
        } else {
            wp->gfData[index / 32] &= ~(1 << shift);
        }
        return value;
    }
    if (index <= -60000000) {
        index += 70000000;
        shift = index % 32;
        if (value != 0) {
            ep->lfData[index / 32] |= 1 << shift;
        } else {
            ep->lfData[index / 32] &= ~(1 << shift);
        }
        return value;
    }
    if (index <= -40000000) {
        index += 50000000;
        result = wp->gwData[index];
        wp->gwData[index] = value;
        return result;
    }
    if (index <= -20000000) {
        index += 30000000;
        result = ep->lwData[index];
        ep->lwData[index] = value;
        return result;
    }
    return value;
}

f32 evtGetFloat(struct EventEntry* entry, s32 index) {
    extern s32 swByteGet(s32);
    extern s32 _swByteGet(s32);
    void* work = evtGetWork();

    if (index <= -270000000) {
        return (f32)index;
    }
    if (index <= -250000000) {
        return (f32)index;
    }
    if (index <= -220000000) {
        return (f32)(index + 230000000) * 0.0009765625f;
    }
    if (index <= -180000000) {
        index += 190000000;
        index = *(s32*)(*(s32*)((s32)entry + 0x154) + index * 4);
        if (index <= -220000000) {
            return (f32)(index + 230000000) * 0.0009765625f;
        }
        return (f32)index;
    }
    if (index <= -160000000) {
        index += 170000000;
        index = swByteGet(index);
        if (index <= -220000000) {
            return (f32)(index + 230000000) * 0.0009765625f;
        }
        return (f32)index;
    }
    if (index <= -140000000) {
        index += 150000000;
        index = _swByteGet(index);
        if (index <= -220000000) {
            return (f32)(index + 230000000) * 0.0009765625f;
        }
        return (f32)index;
    }
    if (index <= -80000000) {
        index += 90000000;
        if (*(u32*)((s32)work + 0x84 + (index / 32) * 4) & (1 << (index % 32))) {
            return 1.0f;
        }
        return 0.0f;
    }
    if (index <= -60000000) {
        index += 70000000;
        if (*(u32*)((s32)entry + 0xE8 + ((index / 32) - 3) * 4) & (1 << (index % 32))) {
            return 1.0f;
        }
        return 0.0f;
    }
    if (index <= -40000000) {
        index += 50000000;
        index = *(s32*)((s32)work + 4 + index * 4);
        if (index <= -220000000) {
            return (f32)(index + 230000000) * 0.0009765625f;
        }
        return (f32)index;
    }
    if (index <= -20000000) {
        index += 30000000;
        index = *(s32*)((s32)entry + 0x9C + index * 4);
        if (index <= -220000000) {
            return (f32)(index + 230000000) * 0.0009765625f;
        }
        return (f32)index;
    }
    if (index <= -220000000) {
        return (f32)(index + 230000000) * 0.0009765625f;
    }
    return (f32)index;
}

f32 evtSetFloat(struct EventEntry* entry, s32 index, f32 value) {
    typedef struct EvtSetFloatWork {
        u8 pad_0[4];
        s32 gwData[32];
        u32 gfData[32];
    } EvtSetFloatWork;
    typedef struct EvtSetFloatEntry {
        u8 pad_0[0x9C];
        s32 lwData[16];
        u32 lfData[30];
        s32* uwBase;
    } EvtSetFloatEntry;
    EvtSetFloatWork* wp = evtGetWork();
    EvtSetFloatEntry* ep = (EvtSetFloatEntry*)entry;
    s32 result;
    s32 shift;

    if (index <= -270000000) {
        return value;
    }
    if (index <= -220000000) {
        return value;
    }
    if (index <= -180000000) {
        index += 190000000;
        result = ep->uwBase[index];
        ep->uwBase[index] = (s32)(value * 1024.0f) - 230000000;
        if (result <= -220000000) {
            return (f32)(result + 230000000) * 0.0009765625f;
        }
        return (f32)result;
    }
    if (index <= -80000000) {
        index += 90000000;
        shift = index % 32;
        if (value) {
            wp->gfData[index / 32] |= 1 << shift;
        } else {
            wp->gfData[index / 32] &= ~(1 << shift);
        }
        return value;
    }
    if (index <= -60000000) {
        index += 70000000;
        shift = index % 32;
        if (value) {
            ep->lfData[index / 32] |= 1 << shift;
        } else {
            ep->lfData[index / 32] &= ~(1 << shift);
        }
        return value;
    }
    if (index <= -40000000) {
        index += 50000000;
        result = wp->gwData[index];
        wp->gwData[index] = (s32)(value * 1024.0f) - 230000000;
        if (result <= -220000000) {
            return (f32)(result + 230000000) * 0.0009765625f;
        }
        return (f32)result;
    }
    if (index <= -20000000) {
        index += 30000000;
        result = ep->lwData[index];
        ep->lwData[index] = (s32)(value * 1024.0f) - 230000000;
        if (result <= -220000000) {
            return (f32)(result + 230000000) * 0.0009765625f;
        }
        return (f32)result;
    }
    return value;
}
