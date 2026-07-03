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


s32 evt_inline_evt_id(void* event) {
    extern void* evtEntryType(void*, s32, s32, s32);
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

s32 evt_case_between(int param_1) {
    return 0;
}


s32 evt_case_or(void* param_1) {
    return 0;
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

s32 evt_inline_evt(void* event) {
    extern void* evtEntryType(void*, s32, s32, s32);
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


s32 evt_run_evt_id(void* event) {
    extern s32 evtGetValue(void*, s32);
    extern s32 evtSetValue(void*, s32, s32);
    extern void* evtEntryType(void*, s32, s32, s32);
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

s32 evt_run_evt(void* event) {
    extern s32 evtGetValue(void*, s32);
    extern void* evtEntryType(void*, s32, s32, s32);
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
}

s32 evt_wait_msec(void* event) {
    extern s32 evtGetValue(void*, s32);
    extern unsigned int __OSBusClock;
    unsigned int now_hi;
    unsigned int now_lo;
    unsigned int old_lo;
    unsigned int diff;
    unsigned int ticks_per_msec;
    unsigned int elapsed;
    s32 wait;

    now_hi = *(unsigned int*)event;
    now_lo = *(unsigned int*)((int)event + 4);
    if (*(unsigned char*)((int)event + 0xD) == 0) {
        wait = evtGetValue(event, **(s32**)((int)event + 0x18));
        *(s32*)((int)event + 0x78) = wait;
        *(unsigned int*)((int)event + 0x7C) = now_hi;
        *(unsigned int*)((int)event + 0x80) = now_lo;
        *(unsigned char*)((int)event + 0xD) = 1;
    }

    wait = *(s32*)((int)event + 0x78);
    if (wait == 0) {
        return 2;
    }

    old_lo = *(unsigned int*)((int)event + 0x80);
    ticks_per_msec = (__OSBusClock >> 2) / 1000;
    diff = now_lo - old_lo;
    elapsed = diff / ticks_per_msec;
    return elapsed >= (unsigned int)wait;
}

s32 evt_if_large_equal(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 left;
    s32 right;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;
    s32 opcode;

    args = *(s32**)((int)event + 0x18);
    left = evtGetValue(event, args[0]);
    right = evtGetValue(event, args[1]);
    if (left < right) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
        while (1) {
            cmd = *ip++;
            opcode = cmd & 0xFFFF;
            ip += (int)cmd >> 0x10;
            if (opcode == 0x20) {
                if (depth == 0) break;
            } else if (opcode >= 0x20) {
                if (opcode < 0x22 && --depth < 0) break;
            } else if (opcode == 1) {
                if (--depth < 0) break;
            } else if (opcode >= 0xC) {
                depth++;
            }
        }
        *(unsigned int**)((int)event + 0x14) = ip;
    }
    return 2;
}

s32 evt_if_small_equal(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 left;
    s32 right;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;
    s32 opcode;

    args = *(s32**)((int)event + 0x18);
    left = evtGetValue(event, args[0]);
    right = evtGetValue(event, args[1]);
    if (left > right) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
        while (1) {
            cmd = *ip++;
            opcode = cmd & 0xFFFF;
            ip += (int)cmd >> 0x10;
            if (opcode == 0x20) {
                if (depth == 0) break;
            } else if (opcode >= 0x20) {
                if (opcode < 0x22 && --depth < 0) break;
            } else if (opcode == 1) {
                if (--depth < 0) break;
            } else if (opcode >= 0xC) {
                depth++;
            }
        }
        *(unsigned int**)((int)event + 0x14) = ip;
    }
    return 2;
}

s32 evt_if_large(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 left;
    s32 right;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;
    s32 opcode;

    args = *(s32**)((int)event + 0x18);
    left = evtGetValue(event, args[0]);
    right = evtGetValue(event, args[1]);
    if (left <= right) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
        while (1) {
            cmd = *ip++;
            opcode = cmd & 0xFFFF;
            ip += (int)cmd >> 0x10;
            if (opcode == 0x20) {
                if (depth == 0) break;
            } else if (opcode >= 0x20) {
                if (opcode < 0x22 && --depth < 0) break;
            } else if (opcode == 1) {
                if (--depth < 0) break;
            } else if (opcode >= 0xC) {
                depth++;
            }
        }
        *(unsigned int**)((int)event + 0x14) = ip;
    }
    return 2;
}

s32 evt_if_small(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 left;
    s32 right;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;
    s32 opcode;

    args = *(s32**)((int)event + 0x18);
    left = evtGetValue(event, args[0]);
    right = evtGetValue(event, args[1]);
    if (left >= right) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
        while (1) {
            cmd = *ip++;
            opcode = cmd & 0xFFFF;
            ip += (int)cmd >> 0x10;
            if (opcode == 0x20) {
                if (depth == 0) break;
            } else if (opcode >= 0x20) {
                if (opcode < 0x22 && --depth < 0) break;
            } else if (opcode == 1) {
                if (--depth < 0) break;
            } else if (opcode >= 0xC) {
                depth++;
            }
        }
        *(unsigned int**)((int)event + 0x14) = ip;
    }
    return 2;
}

s32 evt_if_not_equal(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 left;
    s32 right;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;
    s32 opcode;

    args = *(s32**)((int)event + 0x18);
    left = evtGetValue(event, args[0]);
    right = evtGetValue(event, args[1]);
    if (left == right) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
        while (1) {
            cmd = *ip++;
            opcode = cmd & 0xFFFF;
            ip += (int)cmd >> 0x10;
            if (opcode == 0x20) {
                if (depth == 0) break;
            } else if (opcode >= 0x20) {
                if (opcode < 0x22 && --depth < 0) break;
            } else if (opcode == 1) {
                if (--depth < 0) break;
            } else if (opcode >= 0xC) {
                depth++;
            }
        }
        *(unsigned int**)((int)event + 0x14) = ip;
    }
    return 2;
}

s32 evt_if_equal(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 left;
    s32 right;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;
    s32 opcode;

    args = *(s32**)((int)event + 0x18);
    left = evtGetValue(event, args[0]);
    right = evtGetValue(event, args[1]);
    if (left != right) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
        while (1) {
            cmd = *ip++;
            opcode = cmd & 0xFFFF;
            ip += (int)cmd >> 0x10;
            if (opcode == 0x20) {
                if (depth == 0) {
                    break;
                }
            } else if (opcode >= 0x20) {
                if (opcode < 0x22) {
                    depth--;
                    if (depth < 0) {
                        break;
                    }
                }
            } else if (opcode == 1) {
                depth--;
                if (depth < 0) {
                    break;
                }
            } else if (opcode >= 0xC) {
                depth++;
            }
        }
        *(unsigned int**)((int)event + 0x14) = ip;
    }
    return 2;
}

s32 evt_if_not_flag(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;
    s32 opcode;

    args = *(s32**)((int)event + 0x18);
    if ((evtGetValue(event, args[0]) & args[1]) != 0) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
        while (1) {
            cmd = *ip++;
            opcode = cmd & 0xFFFF;
            ip += (int)cmd >> 0x10;
            if (opcode == 0x20) {
                if (depth == 0) break;
            } else if (opcode >= 0x20) {
                if (opcode < 0x22 && --depth < 0) break;
            } else if (opcode == 1) {
                if (--depth < 0) break;
            } else if (opcode >= 0xC) {
                depth++;
            }
        }
        *(unsigned int**)((int)event + 0x14) = ip;
    }
    return 2;
}

s32 evt_if_flag(void* event) {
    extern s32 evtGetValue(void*, s32);
    s32* args;
    s32 depth;
    unsigned int* ip;
    unsigned int cmd;
    s32 opcode;

    args = *(s32**)((int)event + 0x18);
    if ((evtGetValue(event, args[0]) & args[1]) == 0) {
        depth = 0;
        ip = *(unsigned int**)((int)event + 0x14);
        while (1) {
            cmd = *ip++;
            opcode = cmd & 0xFFFF;
            ip += (int)cmd >> 0x10;
            if (opcode == 0x20) {
                if (depth == 0) break;
            } else if (opcode >= 0x20) {
                if (opcode < 0x22 && --depth < 0) break;
            } else if (opcode == 1) {
                if (--depth < 0) break;
            } else if (opcode >= 0xC) {
                depth++;
            }
        }
        *(unsigned int**)((int)event + 0x14) = ip;
    }
    return 2;
}

s32 evt_brother_evt_id(void* event) {
    extern void* evtBrotherEntry(void*, void*, unsigned int);
    extern s32 evtSetValue(void*, s32, s32);
    unsigned int* old_ip;
    unsigned int* ip;
    unsigned int cmd;
    s32 dst;
    void* child;

    dst = **(s32**)((int)event + 0x18);
    old_ip = *(unsigned int**)((int)event + 0x14);
    ip = old_ip;
    do {
        cmd = *ip++;
        ip += (int)cmd >> 0x10;
    } while ((cmd & 0xFFFF) != 0x70);

    *(unsigned int**)((int)event + 0x14) = ip;
    child = evtBrotherEntry(event, old_ip, 0x60);
    *(unsigned int*)((int)child + 0x160) = *(unsigned int*)((int)event + 0x160);
    *(unsigned char*)((int)child + 0xC) = *(unsigned char*)((int)event + 0xC);
    *(unsigned int*)((int)child + 0x154) = *(unsigned int*)((int)event + 0x154);
    *(unsigned int*)((int)child + 0x158) = *(unsigned int*)((int)event + 0x158);
    *(unsigned int*)((int)child + 0x170) = *(unsigned int*)((int)event + 0x170);
    evtSetValue(event, dst, *(s32*)((int)child + 0x15C));
    return 2;
}

s32 evt_brother_evt(void* event) {
    extern void* evtBrotherEntry(void*, void*, unsigned int);
    unsigned int* old_ip;
    unsigned int* ip;
    unsigned int cmd;
    void* child;

    old_ip = *(unsigned int**)((int)event + 0x14);
    ip = old_ip;
    do {
        cmd = *ip++;
        ip += (int)cmd >> 0x10;
    } while ((cmd & 0xFFFF) != 0x70);

    *(unsigned int**)((int)event + 0x14) = ip;
    child = evtBrotherEntry(event, old_ip, 0x60);
    *(unsigned int*)((int)child + 0x160) = *(unsigned int*)((int)event + 0x160);
    *(unsigned char*)((int)child + 0xC) = *(unsigned char*)((int)event + 0xC);
    *(unsigned int*)((int)child + 0x154) = *(unsigned int*)((int)event + 0x154);
    *(unsigned int*)((int)child + 0x158) = *(unsigned int*)((int)event + 0x158);
    *(unsigned int*)((int)child + 0x170) = *(unsigned int*)((int)event + 0x170);
    *(unsigned int*)((int)child + 0x150) = *(unsigned int*)((int)event + 0x150);
    return 2;
}

