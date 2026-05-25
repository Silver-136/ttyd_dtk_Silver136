#include "driver/imgdrv.h"

extern void* wp;

void* __memAlloc(s32 heap, u32 size);
void* memset(void* dst, int value, u32 size);

void imgSetShadow(void* img, s32 shadow) {
    *(s32*)((s32)img + 0x100) = shadow;
}

void imgClearVirtualPoint(void* img) {
    *(u32*)((s32)img + 0xCC) |= 8;
}

void imgSetVirtualPoint(void* img, void* point) {
    u32 flags;
    u32 x;
    u32 y;
    u32 z;

    flags = *(u32*)((s32)img + 0xCC);
    x = *(u32*)((s32)point + 0);
    flags &= ~8;
    y = *(u32*)((s32)point + 4);
    *(u32*)((s32)img + 0xCC) = flags;
    z = *(u32*)((s32)point + 8);
    *(u32*)((s32)img + 0xF0) = x;
    *(u32*)((s32)img + 0xF4) = y;
    *(u32*)((s32)img + 0xF8) = z;
}

void imgInit(void) {
    *(s32*)wp = 10;
    *(void**)((s32)wp + 4) = __memAlloc(0, *(s32*)wp * 0x158);
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x158);
}
