#include "effect/eff_torch.h"

typedef struct TorchVec {
    f32 x;
    f32 y;
    f32 z;
} TorchVec;

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern f32 float_1_804247ac;
extern f32 float_0p5_804247bc;
extern const TorchVec vec3_802f9a88[];

const char str_Torch_804247b4[] = "Torch";

u8 effTorchMain(int param_1);


u8 effTorchDisp(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTorchEntry(f32 x, f32 y, f32 z, f32 scale, s32 type) {
    void* entry;
    void* work;
    TorchVec pos;
    s32 main;
    f32 size;
    f32 one;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Torch_804247b4;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x20);
    main = (s32)effTorchMain;
    *(void**)((s32)entry + 0xC) = work;
    *(s32*)((s32)entry + 0x10) = main;
    pos = vec3_802f9a88[0];
    size = float_0p5_804247bc * scale;
    one = float_1_804247ac;
    *(s32*)((s32)work + 0) = type;
    *(u8*)((s32)work + 0x1C) = 0;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    *(TorchVec*)((s32)work + 4) = pos;
    *(f32*)((s32)work + 0x10) = size;
    *(f32*)((s32)work + 0x14) = one;
    *(f32*)((s32)work + 0x18) = one;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 effTorchMain(int param_1) {
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, void* param, f32 priority);
    extern void effTorchDisp(void);

    void* effect;
    void* work;

    effect = (void*)param_1;
    work = *(void**)(param_1 + 0xC);
    dispEntry(4, 2, effTorchDisp, effect, dispCalcZ((void*)((s32)work + 4)));
}

