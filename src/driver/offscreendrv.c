#include "driver/offscreendrv.h"

extern void* gp;
extern s32 work;
extern void* __memAlloc();

void offscreenAddBoundingBox(s32, u16, u16, u16, u16) {
}


u8 offscreenDisp(void) {
    return 0;
}


int offscreenEntry(char* param_1) {
    return 0;
}


u8 offscreenMain(void) {
    return 0;
}


u8 offscreenDelete(char* param_1) {
    return 0;
}


u32* offscreenNameToPtr(char* param_1) {
    return 0;
}


int offscreenNameToId(char* param_1) {
    return 0;
}


u8 offscreenGetTexObj(int param_1, u32** param_2, u32* param_3) {
    return 0;
}


u8 offscreenInit(void) {
    void* base = &work;
    s32 count = 10;

    *(s32*)base = count;
    *(void**)((s32)base + 4) = __memAlloc(0, count * 0x50);
    memset(*(void**)((s32)base + 4), 0, *(s32*)base * 0x50);
    *(s32*)((s32)base + 8) = count;
    *(void**)((s32)base + 0xC) = __memAlloc(0, *(s32*)((s32)base + 8) * 0x50);
    memset(*(void**)((s32)base + 0xC), 0, *(s32*)((s32)base + 8) * 0x50);
}


s32 offscreenGetBoundingBox(int param_1, void* param_2, void* param_3, void* param_4, void* param_5) {
    void* wp = &work;
    void* entry;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        wp = (void*)((s32)wp + 8);
    }
    entry = (void*)((s32)*(void**)((s32)wp + 4) + param_1 * 0x50);
    if ((*(s32*)entry & 1) == 0) {
        return 0;
    }
    if ((*(s32*)entry & 2) == 0) {
        return 0;
    }
    *(u16*)param_2 = *(u16*)((s32)entry + 0x14);
    *(u16*)param_3 = *(u16*)((s32)entry + 0x16);
    *(u16*)param_4 = *(u16*)((s32)entry + 0x18);
    *(u16*)param_5 = *(u16*)((s32)entry + 0x1A);
    return 1;
}


void offscreenReset(s32 value) {
    void* wp = &work;

    if (value != 0) {
        wp = (void*)((s32)wp + 8);
    }
    memset(*(void**)((s32)wp + 4), 0, *(s32*)wp * 0x50);
}


s32 offscreenGetBoundingBox_BeforeFrame(int param_1, void* param_2, void* param_3, void* param_4, void* param_5) {
    void* wp = &work;
    void* entry;
    u16 x0;

    if (*(s32*)((s32)gp + 0x14) != 0) {
        wp = (void*)((s32)wp + 8);
    }
    entry = (void*)((s32)*(void**)((s32)wp + 4) + param_1 * 0x50);
    if ((*(s32*)entry & 1) == 0) {
        return 0;
    }
    x0 = *(u16*)((s32)entry + 0x48);
    if (x0 > *(u16*)((s32)entry + 0x4C)) {
        return 0;
    }
    *(u16*)param_2 = x0;
    *(u16*)param_3 = *(u16*)((s32)entry + 0x4A);
    *(u16*)param_4 = *(u16*)((s32)entry + 0x4C);
    *(u16*)param_5 = *(u16*)((s32)entry + 0x4E);
    return 1;
}
