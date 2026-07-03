#include "effect/n64/eff_pturn_n64.h"

void effPturnDisp(void* camera, void* effect) {
    extern void pturn_main_dispp(void* camera, void* effect);
    extern void pturn_num_dispp(void* camera, void* effect);

    void* work = *(void**)((s32)effect + 0xC);

    if (*(s32*)((s32)work + 4) != 0) {
        pturn_main_dispp(camera, effect);
        if (*(s32*)work != 1) {
            pturn_num_dispp(camera, effect);
        }
    }
}


void pturn_num_dispp(void* camera, void* effect) {
    ;
}


void pturn_main_dispp(void* camera, void* effect) {
    ;
}


u8 effPturnMain(void) {
    return 0;
}


void* effPturnN64Entry(s32 type, s32 number, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effPturnMain(void*);
    extern char str_PturnN64_802fbd90[];
    extern f32 float_1_80425de4;
    extern f32 float_0_80425de8;
    void* entry;
    u8* work;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_PturnN64_802fbd90;
    *(s32*)((s32)entry + 8) = 2;
    work = __memAlloc(3, 0x90);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effPturnMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(f32*)(work + 0x34) = float_1_80425de4;
    if (type == 1) {
        *(s32*)(work + 0x38) = 1;
        *(s32*)(work + 0x3C) = 0;
    } else {
        *(s32*)(work + 0x38) = 0;
        *(s32*)(work + 0x3C) = number;
    }
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    switch (type) {
        case 0:
        case 1:
            *(s32*)(work + 0x28) = 0x64;
            break;
        case 10:
            *(s32*)(work + 0x28) = 0x15;
            break;
    }
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x44) = 0;
    *(s32*)(work + 4) = 1;
    *(f32*)(work + 0x58) = float_0_80425de8;
    *(f32*)(work + 0x54) = float_0_80425de8;
    *(f32*)(work + 0x50) = float_0_80425de8;
    *(f32*)(work + 0x68) = float_0_80425de8;
    *(f32*)(work + 0x64) = float_0_80425de8;
    *(f32*)(work + 0x60) = float_0_80425de8;
    *(f32*)(work + 0x5C) = float_0_80425de8;
    *(f32*)(work + 0x88) = float_0_80425de8;
    *(s32*)(work + 0x70) = 0;
    *(s32*)(work + 0x6C) = 0xFF;
    return entry;
}

