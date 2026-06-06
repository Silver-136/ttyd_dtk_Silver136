#include "effect/eff_ice.h"

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern void* effMayaAnimAlloc(void* data);
extern void effPlayMayaAnim(void* anim);
extern volatile f32 float_1_80424764;
extern void* ice_effAnimDt;

const char str_Ice_80424788[] = "Ice";


u8 effIceMain(u32* param_1) {
    return 0;
}


u8 effIceDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effIceEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 duration) {
    void* entry;
    void* work;
    s32 main;
    f32 one;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Ice_80424788;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x30);
    *(void**)((s32)entry + 0xC) = work;
    main = (s32)effIceMain;
    one = float_1_80424764;
    *(s32*)((s32)entry + 0x10) = main;
    *(u32*)entry |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(f32*)((s32)work + 0x14) = one;
    *(f32*)((s32)work + 0x18) = one;
    *(f32*)((s32)work + 0x1C) = one;
    if (duration > 0) {
        *(s32*)((s32)work + 0x20) = duration;
    } else {
        *(s32*)((s32)work + 0x20) = 1000;
    }
    *(s32*)((s32)work + 0x24) = 0;
    *(s32*)((s32)work + 0x28) = 0;
    *(void**)((s32)work + 0x2C) = effMayaAnimAlloc(&ice_effAnimDt);
    effPlayMayaAnim(*(void**)((s32)work + 0x2C));
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
