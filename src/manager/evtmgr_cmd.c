#include "manager/evtmgr_cmd.h"


s32 evtmgrCmd(struct EventEntry* entry) {
    extern void evtDelete(void*);
    extern void evtDeleteID(s32);
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
            cmd = (u32)args[0];
            *(u8*)(e + 0x0A) = (u8)cmd;
            *(u8*)(e + 0x09) = (s8)(cmd >> 16);
            *(s32*)(e + 0x18) = (s32)(args + 1);
            *(s32*)(e + 0x14) = (s32)(args + 1 + ((s32)cmd >> 16));
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
            *(u8*)(e + 0x0E) = *(u8*)(e + 0x0E) + 1;
            i = (s8)*(u8*)(e + 0x0E);
            *(s32*)(e + 0xE8 + i * 4) = (s32)(args + 1);
            *(s32*)(e + 0x108 + i * 4) = args[0];
            state = 2;
            break;

        case 0x05:
            state = evt_while(entry);
            break;

        case 0x06:
            depth = 0;
            ip = *(u32**)(e + 0x14);
scan_loop_break:
            do {
                cmd = *ip;
                ip += ((s32)cmd >> 16) + 1;
                op = cmd & 0xFFFF;
                if (op == 0x05) {
                    depth++;
                } else if (op == 0x01 || op == 0x06) {
                    depth--;
                }
            } while (depth > -1);
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
                if (op == 0x05) {
                    depth++;
                } else if (op == 0x01 || op == 0x06) {
                    depth--;
                    if (depth < 0) {
                        *(s32*)(e + 0x14) = (s32)ip;
                        state = 2;
                        goto finish;
                    }
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
            } else {
                *(s32*)(e + 0x78) = *(s32*)(e + 0x78) - 1;
                state = (*(s32*)(e + 0x78) == 0) ? 1 : 0;
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
            do {
                cmd = *ip;
                op = cmd & 0xFFFF;
                ip += ((s32)cmd >> 16) + 1;
                if (op == 0x20) {
                    continue;
                }
                if (op < 0x20) {
                    if (op != 0x01 && op != 0 && op > 0x0B) {
                        depth++;
                    }
                } else if (op > 0x21) {
                    continue;
                }
                if (op == 0x01 || op == 0x21) {
                    depth--;
                }
            } while (depth > -1);
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
                do {
                    save = ip;
                    cmd = *save;
                    op = cmd & 0xFFFF;
                    ip = save + ((s32)cmd >> 16) + 1;
                    if (op == 0x22 || op == 0x01) {
                        depth++;
                    }
                } while (op != 0x31 || (--depth != 0));
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
            do {
                save = ip;
                cmd = *save;
                op = cmd & 0xFFFF;
                ip = save + ((s32)cmd >> 16) + 1;
                if (op == 0x22 || op == 0x01) {
                    depth++;
                }
            } while (op != 0x31 || (--depth != 0));
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
            v1 = evtGetValue(entry, args[1]);
            i = evtGetValue(entry, v0);
            evtSetValue(entry, v0, i - (i / v1) * v1);
            state = 2;
            break;

        case 0x3A:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            f0 = evtGetFloat(entry, args[1]);
            evtSetFloat(entry, v0, evtGetFloat(entry, v0) + f0);
            state = 2;
            break;

        case 0x3B:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            f0 = evtGetFloat(entry, args[1]);
            evtSetFloat(entry, v0, evtGetFloat(entry, v0) - f0);
            state = 2;
            break;

        case 0x3C:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            f0 = evtGetFloat(entry, args[1]);
            evtSetFloat(entry, v0, evtGetFloat(entry, v0) * f0);
            state = 2;
            break;

        case 0x3D:
            args = *(s32**)(e + 0x18);
            v0 = args[0];
            f0 = evtGetFloat(entry, args[1]);
            evtSetFloat(entry, v0, evtGetFloat(entry, v0) / f0);
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
        case 0x41:
        case 0x42:
            args = *(s32**)(e + 0x18);
            ip = *(u32**)(e + 0x150);
            depth = op - 0x3E;
            for (i = 0; i < depth; i++) {
                evtSetValue(entry, args[i], ip[i]);
            }
            *(s32*)(e + 0x150) = (s32)(ip + depth);
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
        case 0x47:
        case 0x48:
            args = *(s32**)(e + 0x18);
            ptr = *(void**)(e + 0x150);
            depth = op - 0x44;
            for (i = 0; i < depth; i++) {
                evtSetFloat(entry, args[i], *(f32*)((s32)ptr + i * 4));
            }
            *(s32*)(e + 0x150) = (s32)ptr + depth * 4;
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

int evt_debug_put_reg(void* param_1) {
    extern int sprintf(char* str, const char* format, ...);
    extern void* evtGetWork(void);
    extern s32 swByteGet(s32 index);
    extern s32 _swByteGet(s32 index);
    extern s32 swGet(s32 index);
    extern s32 _swGet(s32 index);

    static char out[256];
    static const struct {
        f64 d;
        char addr[0x10];
        char flt0[0x14];
        char uf[0x10];
        char uwAddr[0x10];
        char uwFloat[0x14];
        char uwInt[0x10];
        char gswAddr[0x10];
        char gswFloat[0x14];
        char gswInt[0x10];
        char lswAddr[0x10];
        char lswFloat[0x14];
        char lswInt[0x10];
        char gswf[0x10];
        char lswf[0x10];
        char gf[0x10];
        char lf[0x10];
        char gwAddr[0x10];
        char gwFloat[0x14];
        char gwInt[0x10];
        char lwAddr[0x10];
        char lwFloat[0x14];
        char lwInt[0x10];
        char def[0x10];
    } fmt = {
        4503601774854144.0,
        "ADDR:%08X",
        "FLOAT:%4.2f",
        "UF:%3d:%d",
        "UW:%08X",
        "UW:%3d:%4.2f",
        "UW:%3d:%d",
        "GSW:%08X",
        "GSW:%3d:%4.2f",
        "GSW:%3d:%d",
        "LSW:%08X",
        "LSW:%3d:%4.2f",
        "LSW:%3d:%d",
        "GSWF:%3d:%d",
        "LSWF:%3d:%d",
        "GF:%3d:%d",
        "LF:%3d:%d",
        "GW:%3d:%08X",
        "GW:%3d:%4.2f",
        "GW:%3d:%d",
        "LW:%3d:%08X",
        "LW:%3d:%4.2f",
        "LW:%3d:%d",
        "%d"
    };

    u8* e;
    s32* args;
    u8* work;
    const char* base;
    s32 value;
    s32 idx;
    s32 fetched;
    s32 q;
    s32 bit;
    u32 mask;
    f32 fvalue;

    e = (u8*)param_1;
    args = *(s32**)(e + 0x18);
    work = (u8*)evtGetWork();
    value = args[0];
    base = (const char*)&fmt;

    if (value <= -270000000) {
        sprintf(out, base + 0x8, value);
    } else if (value <= -220000000) {
        fvalue = (f32)(value + 230000000) * 0.0009765625f;
        sprintf(out, base + 0x18, (f64)fvalue);
    } else if (value <= -200000000) {
        idx = value + 210000000;
        q = idx >> 5;
        bit = idx & 31;
        mask = 1U << bit;
        fetched = *(s32*)(*(s32*)(e + 0x158) + q * 4) & mask;
        sprintf(out, base + 0x2C, idx, fetched);
    } else if (value <= -180000000) {
        idx = value + 190000000;
        fetched = *(s32*)(*(s32*)(e + 0x154) + idx * 4);
        if (fetched <= -270000000) {
            sprintf(out, base + 0x3C, fetched);
        } else if (fetched <= -220000000) {
            fvalue = (f32)(fetched + 230000000) * 0.0009765625f;
            sprintf(out, base + 0x4C, idx, (f64)fvalue);
        } else {
            sprintf(out, base + 0x60, idx, fetched);
        }
    } else if (value <= -160000000) {
        idx = value + 170000000;
        fetched = swByteGet(idx);
        if (fetched <= -270000000) {
            sprintf(out, base + 0x70, fetched);
        } else if (fetched <= -220000000) {
            fvalue = (f32)(fetched + 230000000) * 0.0009765625f;
            sprintf(out, base + 0x80, idx, (f64)fvalue);
        } else {
            sprintf(out, base + 0x94, idx, fetched);
        }
    } else if (value <= -140000000) {
        idx = value + 150000000;
        fetched = _swByteGet(idx);
        if (fetched <= -270000000) {
            sprintf(out, base + 0xA4, fetched);
        } else if (fetched <= -220000000) {
            fvalue = (f32)(fetched + 230000000) * 0.0009765625f;
            sprintf(out, base + 0xB4, idx, (f64)fvalue);
        } else {
            sprintf(out, base + 0xC8, idx, fetched);
        }
    } else if (value <= -120000000) {
        idx = value + 130000000;
        fetched = swGet(idx);
        sprintf(out, base + 0xD8, idx, fetched);
    } else if (value <= -100000000) {
        idx = value + 110000000;
        fetched = _swGet(idx);
        sprintf(out, base + 0xE8, idx, fetched);
    } else if (value <= -80000000) {
        idx = value + 90000000;
        q = idx >> 5;
        bit = idx & 31;
        mask = 1U << bit;
        fetched = *(s32*)(work + 0x84 + q * 4) & mask;
        sprintf(out, base + 0xF8, idx, fetched);
    } else if (value <= -60000000) {
        idx = value + 70000000;
        q = idx >> 5;
        bit = idx & 31;
        mask = 1U << bit;
        fetched = *(s32*)(e + 0xDC + q * 4) & mask;
        sprintf(out, base + 0x108, idx, fetched);
    } else if (value <= -40000000) {
        idx = value + 50000000;
        fetched = *(s32*)(work + 0x4 + idx * 4);
        if (fetched <= -270000000) {
            sprintf(out, base + 0x118, idx, fetched);
        } else if (fetched <= -220000000) {
            fvalue = (f32)(fetched + 230000000) * 0.0009765625f;
            sprintf(out, base + 0x128, idx, (f64)fvalue);
        } else {
            sprintf(out, base + 0x13C, idx, fetched);
        }
    } else if (value <= -20000000) {
        idx = value + 30000000;
        fetched = *(s32*)(e + 0x9C + idx * 4);
        if (fetched <= -270000000) {
            sprintf(out, base + 0x14C, idx, fetched);
        } else if (fetched <= -220000000) {
            fvalue = (f32)(fetched + 230000000) * 0.0009765625f;
            sprintf(out, base + 0x15C, idx, (f64)fvalue);
        } else {
            sprintf(out, base + 0x170, idx, fetched);
        }
    } else {
        sprintf(out, base + 0x180, value);
    }

    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

f32 evtGetFloat(struct EventEntry* entry, s32 index) {
    extern void* evtGetWork(void);
    extern s32 swByteGet(s32);
    extern s32 _swByteGet(s32);

    void* work = evtGetWork();
    s32 value = index;
    u32 bitIndex;
    u32 mask;

    if (index < -270000000) {
        return (f32)index;
    }
    if (index < -250000000) {
        return (f32)index;
    }
    if (index < -220000000) {
        return (f32)(index + 230000000) * 0.0009765625f;
    }
    if (index < -180000000) {
        value = *(s32*)(*(s32*)((s32)entry + 0x154) + ((index + 190000000) * 4));
    } else if (index < -160000000) {
        value = swByteGet(index + 170000000);
    } else if (index < -140000000) {
        value = _swByteGet(index + 150000000);
    } else if (index < -80000000) {
        bitIndex = index + 90000000;
        mask = 1 << (bitIndex & 0x1F);
        return (*(u32*)((s32)work + 0x84 + ((bitIndex >> 5) * 4)) & mask) ? 1.0f : 0.0f;
    } else if (index < -60000000) {
        bitIndex = index + 70000000;
        mask = 1 << (bitIndex & 0x1F);
        return (*(u32*)((s32)entry + 0xE8 + (((bitIndex >> 5) - 3) * 4)) & mask) ? 1.0f : 0.0f;
    } else if (index < -40000000) {
        value = *(s32*)((s32)work + 4 + ((index + 50000000) * 4));
    } else if (index < -20000000) {
        value = *(s32*)((s32)entry + 0x9C + ((index + 30000000) * 4));
    }

    if (value < -220000000) {
        return (f32)(value + 230000000) * 0.0009765625f;
    }
    return (f32)value;
}

s32 evtGetValue(struct EventEntry* entry, s32 index) {
    extern void* evtGetWork(void);
    extern s32 swByteGet(s32);
    extern s32 _swByteGet(s32);
    extern s32 swGet(s32);
    extern s32 _swGet(s32);

    void* work = evtGetWork();
    s32 value = index;
    u32 bitIndex;
    u32 mask;

    if (index <= -270000000) {
        return index;
    }
    if (index <= -250000000) {
        return index;
    }
    if (index < -220000000) {
        return (s32)((f32)(index + 230000000) * 0.0009765625f);
    }
    if (index < -200000000) {
        bitIndex = index + 210000000;
        mask = 1 << (bitIndex & 0x1F);
        return ((*(u32*)(*(s32*)((s32)entry + 0x158) + ((bitIndex >> 5) * 4)) & mask) != 0);
    }
    if (index < -180000000) {
        value = *(s32*)(*(s32*)((s32)entry + 0x154) + ((index + 190000000) * 4));
        if (value > -270000000 && value < -220000000) {
            value = (s32)((f32)(value + 230000000) * 0.0009765625f);
        }
        return value;
    }
    if (index < -160000000) {
        return swByteGet(index + 170000000);
    }
    if (index < -140000000) {
        return _swByteGet(index + 150000000);
    }
    if (index < -120000000) {
        return swGet(index + 130000000);
    }
    if (index < -100000000) {
        return _swGet(index + 110000000);
    }
    if (index < -80000000) {
        bitIndex = index + 90000000;
        mask = 1 << (bitIndex & 0x1F);
        return ((*(u32*)((s32)work + 0x84 + ((bitIndex >> 5) * 4)) & mask) != 0);
    }
    if (index < -60000000) {
        bitIndex = index + 70000000;
        mask = 1 << (bitIndex & 0x1F);
        return ((*(u32*)((s32)entry + 0xE8 + (((bitIndex >> 5) - 3) * 4)) & mask) != 0);
    }
    if (index < -40000000) {
        value = *(s32*)((s32)work + 4 + ((index + 50000000) * 4));
        if (value > -270000000 && value < -220000000) {
            value = (s32)((f32)(value + 230000000) * 0.0009765625f);
        }
        return value;
    }
    if (index < -20000000) {
        value = *(s32*)((s32)entry + 0x9C + ((index + 30000000) * 4));
        if (value > -270000000 && value < -220000000) {
            value = (s32)((f32)(value + 230000000) * 0.0009765625f);
        }
    }
    return value;
}

s32 evtSetValue(struct EventEntry* entry, s32 index, s32 value) {
    extern void* evtGetWork(void);
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

    void* work = evtGetWork();
    s32 old = value;
    u32 bitIndex;
    u32 mask;
    s32* ptr;

    if (index <= -270000000) {
        return value;
    }
    if (index < -220000000) {
        if (value < -220000000) {
            value = (s32)((f32)(value + 230000000) * 0.0009765625f);
        }
    } else if (index < -200000000) {
        bitIndex = index + 210000000;
        mask = 1 << (bitIndex & 0x1F);
        ptr = (s32*)(*(s32*)((s32)entry + 0x158) + ((bitIndex >> 5) * 4));
        if (value == 0) {
            *ptr = *ptr & ~mask;
        } else {
            *ptr = *ptr | mask;
        }
    } else if (index < -180000000) {
        ptr = (s32*)(*(s32*)((s32)entry + 0x154) + ((index + 190000000) * 4));
        old = *ptr;
        *ptr = value;
        value = old;
    } else if (index < -160000000) {
        old = swByteGet(index + 170000000);
        swByteSet(index + 170000000, value);
        value = old;
    } else if (index < -140000000) {
        old = _swByteGet(index + 150000000);
        _swByteSet(index + 150000000, value & 0xFF);
        value = old;
    } else if (index < -120000000) {
        old = swGet(index + 130000000);
        if (value == 0) {
            swClear(index + 130000000);
        } else {
            swSet(index + 130000000);
        }
        value = old;
    } else if (index < -100000000) {
        old = _swGet(index + 110000000);
        if (value == 0) {
            _swClear(index + 110000000);
        } else {
            _swSet(index + 110000000);
        }
        value = old;
    } else if (index < -80000000) {
        bitIndex = index + 90000000;
        mask = 1 << (bitIndex & 0x1F);
        ptr = (s32*)((s32)work + 0x84 + ((bitIndex >> 5) * 4));
        if (value == 0) {
            *ptr = *ptr & ~mask;
        } else {
            *ptr = *ptr | mask;
        }
    } else if (index < -60000000) {
        bitIndex = index + 70000000;
        mask = 1 << (bitIndex & 0x1F);
        ptr = (s32*)((s32)entry + 0xE8 + (((bitIndex >> 5) - 3) * 4));
        if (value == 0) {
            *ptr = *ptr & ~mask;
        } else {
            *ptr = *ptr | mask;
        }
    } else if (index < -40000000) {
        ptr = (s32*)((s32)work + 4 + ((index + 50000000) * 4));
        old = *ptr;
        *ptr = value;
        value = old;
    } else if (index < -20000000) {
        ptr = (s32*)((s32)entry + 0x9C + ((index + 30000000) * 4));
        old = *ptr;
        *ptr = value;
        value = old;
    }
    return value;
}

f32 evtSetFloat(struct EventEntry* entry, s32 index, f32 value) {
    extern void* evtGetWork(void);

    void* work = evtGetWork();
    s32 stored = (s32)(value * 1024.0f) - 230000000;
    s32 old;
    u32 bitIndex;
    u32 mask;
    s32* ptr;

    if (index > -270000000 && index > -220000000) {
        if (index < -180000000) {
            ptr = (s32*)(*(s32*)((s32)entry + 0x154) + ((index + 190000000) * 4));
            old = *ptr;
            *ptr = stored;
            if (old <= -220000000) {
                return (f32)(old + 230000000) * 0.0009765625f;
            }
            return (f32)old;
        } else if (index < -80000000) {
            bitIndex = index + 90000000;
            mask = 1 << (bitIndex & 0x1F);
            ptr = (s32*)((s32)work + 0x84 + ((bitIndex >> 5) * 4));
            if (value == 0.0f) {
                *ptr = *ptr & ~mask;
            } else {
                *ptr = *ptr | mask;
            }
        } else if (index < -60000000) {
            bitIndex = index + 70000000;
            mask = 1 << (bitIndex & 0x1F);
            ptr = (s32*)((s32)entry + 0xE8 + (((bitIndex >> 5) - 3) * 4));
            if (value == 0.0f) {
                *ptr = *ptr & ~mask;
            } else {
                *ptr = *ptr | mask;
            }
        } else if (index < -40000000) {
            ptr = (s32*)((s32)work + 4 + ((index + 50000000) * 4));
            old = *ptr;
            *ptr = stored;
            if (old <= -220000000) {
                return (f32)(old + 230000000) * 0.0009765625f;
            }
            return (f32)old;
        } else if (index < -20000000) {
            ptr = (s32*)((s32)entry + 0x9C + ((index + 30000000) * 4));
            old = *ptr;
            *ptr = stored;
            if (old <= -220000000) {
                return (f32)(old + 230000000) * 0.0009765625f;
            }
            return (f32)old;
        }
    }
    return value;
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

