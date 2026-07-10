#include "effect/n64/eff_wangflush_n64.h"

typedef struct EffWangFlushWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 field_10;
    f32 field_14;
    f32 field_18;
    f32 field_1C;
    f32 field_20;
    s32 field_24;
    s32 field_28;
    s32 param;
    s32 field_30;
} EffWangFlushWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_WangflushN64_802fc258[];
extern f32 float_0_8042649c;
extern f32 float_30_804264c4;
extern f32 float_0p8_804264c8;

u8 effWangFlushDisp(int param_1, int param_2) {
    return 0;
}


void effWangFlushMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effWangFlushDisp(void);
    extern Vec3 vec3_802fc240[];
    extern f32 float_0p5_804264a0;
    extern f32 float_0p78_804264a4;
    extern f32 float_0p29_804264a8;
    extern f32 float_0p93_804264ac;
    extern f32 float_6p2832_804264b0;
    extern f32 float_360_804264b4;
    extern f32 float_0p9_804264b8;
    extern f32 float_144_804264bc;
    extern f32 float_64_804264c0;
    extern double sin(double);
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    Vec3* base;
    s32 timer;
    s32 frame;
    f32 angle;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fc240;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x2C) -= 1;
    *(s32*)(work + 0x30) += 1;
    timer = *(s32*)(work + 0x2C);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x30);
    if (frame <= 6) {
        *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) + float_0p5_804264a0 * (f32)(0xDA - *(s32*)(work + 0x24)));
    }
    if (frame > 6) {
        *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) * float_0p78_804264a4);
    }
    *(f32*)(work + 0x18) += *(f32*)(work + 0x1C);
    *(f32*)(work + 0x1C) += float_0p29_804264a8;
    *(f32*)(work + 0x10) += *(f32*)(work + 0x14);
    *(f32*)(work + 0x14) *= float_0p93_804264ac;
    angle = ((f32)(timer * 50) * float_6p2832_804264b0) / float_360_804264b4;
    *(f32*)(work + 0x20) = float_0p9_804264b8 + float_0p5_804264a0 * (f32)sin(angle);
    angle = ((f32)(timer * 40) * float_6p2832_804264b0) / float_360_804264b4;
    *(s32*)(work + 0x28) = (s32)(float_144_804264bc + float_64_804264c0 * (f32)sin(angle));
    dispEntry(4, 2, effWangFlushDisp, effect, dispCalcZ(&dispPos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effWangFlushN64Entry(s32 type, s32 param, f32 x, f32 y, f32 z) {
    void* entry;
    EffWangFlushWork* work;
    f32 zero;
    f32 defaultAngle;
    f32 scale;
    s32 zeroInt;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_WangflushN64_802fc258;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x34);
    *(EffWangFlushWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effWangFlushMain;

    zeroInt = 0;
    zero = float_0_8042649c;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->param = param;
    work->field_30 = zeroInt;
    work->field_24 = zeroInt;
    work->field_10 = zero;
    if (type == 0) {
        defaultAngle = float_30_804264c4;
        work->field_14 = defaultAngle;
    } else {
        work->field_14 = zero;
    }
    zero = float_0_8042649c;
    zeroInt = 0;
    scale = float_0p8_804264c8;
    work->field_18 = zero;
    work->field_1C = scale;
    work->field_28 = zeroInt;
    work->field_20 = zero;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
