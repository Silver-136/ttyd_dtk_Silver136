#include "effect/eff_toge_flush.h"

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern f32 float_0_804246c0;
extern f32 float_30_804246f4;
extern f32 float_0p8_804246f8;

const char str_TogeFlush_802f9748[] = "TogeFlush";


u8 effTogeFlushDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effTogeFlushMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct TogeFlushWork {
        s32 type;
        Vec3 pos;
        f32 rot;
        f32 rotStep;
        f32 spin;
        f32 scale;
        f32 alphaScale;
        s32 alpha;
        s32 unk28;
        s32 life;
        s32 frame;
    } TogeFlushWork;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effTogeFlushDisp(void);
    extern f64 sin(f64);

    TogeFlushWork* work;
    s32 life;

    work = *(TogeFlushWork**)((s32)effect + 0xC);
    work->life--;
    work->frame++;
    life = work->life;
    if (life < 0) {
        effDelete(effect);
        return;
    }
    if (work->frame <= 6) {
        work->alpha = (s32)((f32)(218 - work->alpha) * 0.5f + (f32)work->alpha);
    }
    if (work->frame > 6) {
        work->alpha = (s32)((f32)work->alpha * 0.78f);
    }

    work->spin += work->scale;
    work->scale += 0.29f;
    work->rot += work->rotStep;
    work->rotStep *= 0.93f;
    work->alphaScale = (0.5f * (f32)sin((6.2832f * (f32)(life * 50)) / 360.0f)) + 0.9f;
    work->unk28 = (s32)((64.0f * (f32)sin((6.2832f * (f32)(life * 40)) / 360.0f)) + 144.0f);
    dispEntry(4, 2, effTogeFlushDisp, effect, dispCalcZ(&work->pos));
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTogeFlushEntry(f32 x, s32 type, f32 y, s32 arg, f32 z) {
    void* entry;
    void* work;
    f32 zero;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_TogeFlush_802f9748;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x34);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effTogeFlushMain;
    zero = float_0_804246c0;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x2C) = arg;
    *(s32*)((s32)work + 0x30) = 0;
    *(s32*)((s32)work + 0x24) = 0;
    *(f32*)((s32)work + 0x10) = zero;
    if (type == 0 || type == 3) {
        *(f32*)((s32)work + 0x14) = float_30_804246f4;
    } else {
        *(f32*)((s32)work + 0x14) = zero;
    }
    *(f32*)((s32)work + 0x18) = zero;
    *(f32*)((s32)work + 0x1C) = float_0p8_804246f8;
    *(s32*)((s32)work + 0x28) = 0;
    *(f32*)((s32)work + 0x20) = zero;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
