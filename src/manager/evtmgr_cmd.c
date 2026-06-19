#include "manager/evtmgr_cmd.h"


s32 evtmgrCmd(struct EventEntry* entry) {
    return 0;
}


int evt_debug_put_reg(void* param_1) {
    return 0;
}


f32 evtGetFloat(struct EventEntry* entry, s32 index) {
    return 0.0f;
}


s32 evtGetValue(struct EventEntry* entry, s32 index) {
    return 0;
}


s32 evtSetValue(struct EventEntry* entry, s32 index, s32 value) {
    return 0;
}


f32 evtSetFloat(struct EventEntry* entry, s32 index, f32 value) {
    return 0.0f;
}


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
        if (opcode == 0x23) {
            goto loop1;
        }
        if (opcode < 0x23) {
            if (opcode == 1 || opcode >= 0x22) {
                depth++;
            }
            goto loop1;
        }
        if (opcode == 0x31) {
            depth--;
            if (depth == 0) {
                goto setpos1;
            }
            goto loop1;
        }
        if (opcode >= 0x31) {
            goto loop1;
        }
        if (opcode >= 0x30) {
            goto loop1;
        }
        if (depth != 1) {
            goto loop1;
        }
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
    if (opcode == 0x23) {
        goto loop2;
    }
    if (opcode < 0x23) {
        if (opcode == 1 || opcode >= 0x22) {
            depth++;
        }
        goto loop2;
    }
    if (opcode == 0x31) {
        depth--;
        if (depth == 0) {
            goto setpos2;
        }
        goto loop2;
    }
    if (opcode >= 0x31) {
        goto loop2;
    }
    if (opcode >= 0x30) {
        goto loop2;
    }
    if (depth != 1) {
        goto loop2;
    }
setpos2:
    *(u32**)(param_1 + 0x14) = save;
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 evt_case_end(int param_1) {
    return 0;
}


int evt_inline_evt_id(void* pEvt) {
    return 0;
}


s32 evt_case_between(int param_1) {
    return 0;
}


s32 evt_case_or(void* param_1) {
    return 0;
}


s32 evtGetNumber(struct EventEntry* entry, s32 index) {
    return 0;
}


int evt_inline_evt(void* pEvt) {
    return 0;
}


s32 evt_case_large_equal(int param_1) {
    return 0;
}


s32 evt_case_large(void* param_1) {
    return 0;
}


s32 evt_case_small_equal(int param_1) {
    return 0;
}


s32 evt_case_small(void* param_1) {
    return 0;
}


s32 evt_case_not_equal(void* param_1) {
    return 0;
}


s32 evt_case_equal(void* pEvt) {
    return 0;
}


s32 evt_case_flag(int param_1) {
    return 0;
}


int evt_run_evt_id(void* pEvt) {
    return 0;
}


int evt_run_evt(void* pEvt) {
    return 0;
}


int evt_if_str_large_equal(void* pEvt) {
    return 0;
}


int evt_if_str_small_equal(void* param_1) {
    return 0;
}


int evt_if_str_large(void* param_1) {
    return 0;
}


int evt_if_str_small(void* param_1) {
    return 0;
}


int evt_if_str_not_equal(void* param_1) {
    return 0;
}


int evt_if_str_equal(void* param_1) {
    return 0;
}


s32 evt_iff_large(int param_1) {
    return 0;
}


int evt_iff_small(void* param_1) {
    return 0;
}


s32 evt_iff_large_equal(int param_1) {
    return 0;
}


s32 evt_iff_small_equal(int param_1) {
    return 0;
}


int evt_iff_not_equal(void* param_1) {
    return 0;
}


int evt_iff_equal(void* param_1) {
    return 0;
}


int evt_while(void* pEvt) {
    return 0;
}


int evt_wait_msec(void* param_1) {
    return 0;
}


s32 evt_if_large_equal(void* evt) {
    return 0;
}


s32 evt_if_small_equal(int param_1) {
    return 0;
}


s32 evt_if_large(int param_1) {
    return 0;
}


s32 evt_if_small(int param_1) {
    return 0;
}


s32 evt_if_not_equal(int param_1) {
    return 0;
}


s32 evt_if_equal(void* pEvt) {
    return 0;
}


s32 evt_if_not_flag(int param_1) {
    return 0;
}


s32 evt_if_flag(int param_1) {
    return 0;
}


int evt_brother_evt_id(void* pEvt) {
    return 0;
}


int evt_brother_evt(void* pEvt) {
    return 0;
}
