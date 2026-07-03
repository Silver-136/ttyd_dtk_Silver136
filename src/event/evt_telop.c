#include "event/evt_telop.h"


u8 telop(void) {
    return 0;
}


s32 evt_telop_entry(void* param_1, int param_2) {
    extern void* memset(void* dest, s32 val, u32 size);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 swGet(s32 id);
    extern void swSet(s32 id);
    extern f32 intplGetValue(s32 type, f32 start, f32 end, u32 time, u32 duration);
    extern s32 dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, s32 param);
    extern void* wp;
    extern u8 telop(void);
    extern f32 float_neg256_80424b58;
    extern f32 float_256_80424b5c;
    extern f32 float_300_80424b60;
    s32* args;
    u32 elapsed;
    u32 ticksPerMs;

    args = *(s32**)((s32)param_1 + 0x18);
    if (param_2 != 0) {
        *(s32*)((s32)param_1 + 0x78) = 0;
        memset(wp, 0, 0x10);
        *(s32*)wp = evtGetValue(param_1, args[0]);
        if (swGet(*(s32*)wp + 0x28) != 0) {
            return 2;
        }
        swSet(*(s32*)wp + 0x28);
    }

    switch (*(s32*)((s32)param_1 + 0x78)) {
        case 0:
            *(s32*)((s32)wp + 0xC) = *(s32*)((s32)param_1 + 4);
            *(s32*)((s32)wp + 8) = *(s32*)param_1;
            *(s32*)((s32)param_1 + 0x78) = *(s32*)((s32)param_1 + 0x78) + 1;
            break;
        case 1:
            ticksPerMs = (((*(u32*)0x800000F8 >> 2) * 0x10624DD3U) >> 6);
            elapsed = (*(u32*)((s32)param_1 + 4) - *(u32*)((s32)wp + 0xC)) / ticksPerMs;
            if (elapsed < 1000) {
                *(s32*)((s32)wp + 4) =
                    (s32)intplGetValue(0xB, float_neg256_80424b58, float_256_80424b5c, elapsed, 1000);
            } else {
                *(s32*)((s32)wp + 0xC) = *(s32*)((s32)param_1 + 4);
                *(s32*)((s32)wp + 8) = *(s32*)param_1;
                *(s32*)((s32)param_1 + 0x78) = *(s32*)((s32)param_1 + 0x78) + 1;
            }
            dispEntry(8, 0, telop, float_300_80424b60, 0);
            break;
        case 2:
            ticksPerMs = (((*(u32*)0x800000F8 >> 2) * 0x10624DD3U) >> 6);
            elapsed = (*(u32*)((s32)param_1 + 4) - *(u32*)((s32)wp + 0xC)) / ticksPerMs;
            if (elapsed > 5000) {
                *(s32*)((s32)wp + 0xC) = *(s32*)((s32)param_1 + 4);
                *(s32*)((s32)wp + 8) = *(s32*)param_1;
                *(s32*)((s32)param_1 + 0x78) = *(s32*)((s32)param_1 + 0x78) + 1;
            }
            dispEntry(8, 0, telop, float_300_80424b60, 0);
            break;
        case 3:
            ticksPerMs = (((*(u32*)0x800000F8 >> 2) * 0x10624DD3U) >> 6);
            elapsed = (*(u32*)((s32)param_1 + 4) - *(u32*)((s32)wp + 0xC)) / ticksPerMs;
            if (elapsed < 1000) {
                *(s32*)((s32)wp + 4) =
                    (s32)intplGetValue(0xB, float_256_80424b5c, float_neg256_80424b58, elapsed, 1000);
            } else {
                *(s32*)((s32)param_1 + 0x78) = *(s32*)((s32)param_1 + 0x78) + 1;
            }
            dispEntry(8, 0, telop, float_300_80424b60, 0);
            break;
        case 4:
            return 2;
    }

    return 0;
}

