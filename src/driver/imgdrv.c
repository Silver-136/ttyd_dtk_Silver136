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
    u32 y;
    u32 x;
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


u8 imgDisp(void) {
    return 0;
}


u8 imgShadowDisp(void) {
    return 0;
}


u8 imgCapture_Prim(int param_1, int param_2) {
    return 0;
}


void zFill(void) {
}


u8 imgDisp_ProjPlane(s32 param_1, int param_2) {
    return 0;
}


u8 imgMain(void) {
    return 0;
}


void imgEntry(s32 name, s32 flag) {
}


void imgAutoRelease(s32 value) {
}


void imgRelease(s32 imageId, void* heap) {
}


u8 imgCapture(int param_1) {
    return 0;
}


void* imgNameToPtr(s32 name, s32 flag) {
    return 0;
}


u8 imgFreeCapture(int param_1, int param_2) {
    return 0;
}
