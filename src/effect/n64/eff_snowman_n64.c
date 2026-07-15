#include "effect/n64/eff_snowman_n64.h"

typedef struct EffSnowmanWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    f32 field_28;
    f32 field_2C;
    f32 field_30;
    f32 field_34;
    f32 field_38;
    f32 field_3C;
    f32 field_40;
    f32 field_44;
    s32 field_48;
    s32 field_4C;
    f32 field_50;
    s32 field_54;
    s32 field_58;
    s32 field_5C;
} EffSnowmanWork;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);

extern const char str_SnowmanN64_802fbfd0[];
extern f32 float_0_804260c0;
extern f32 float_neg1_804260cc;
extern f32 float_200_804260f0;
extern f32 float_neg8_804260f4;

void effSnowmanDisp(void) {
    ;
}


void effSnowmanMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effSnowmanDisp(void);
    extern s32 rand(void);
    extern void* effVaporN64Entry(f32, f32, f32, f32, s32, s32);
    extern void* effSnowDustN64Entry(f32, f32, f32, f32, f32, f32, s32, s32, s32);
    extern u8 land_hscale_data[];
    extern s8 jump_hscale_data[];
    extern u8 jump2_hscale_data[];
    extern s8 jump2_ry_data[];
    u8* work = *(u8**)((s32)effect + 0xC);
    Vec3 pos;
    s32 timer;
    s32 frame;
    s32 state;
    s32 anim;
    f32 v;

    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) *(s32*)(work + 0x10) -= 1;
    *(s32*)(work + 0x14) += 1;
    timer = *(s32*)(work + 0x10);
    frame = *(s32*)(work + 0x14);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer < 0x10) *(s32*)(work + 0x24) = timer << 4;
    if (frame < 0x10) *(s32*)(work + 0x24) = (frame << 4) + 0xF;

    if (*(f32*)(work + 0x3C) != 0.0f) {
        v = *(f32*)(work + 0x40);
        if (v < 0.0f) v = -v;
        if (*(f32*)(work + 0x3C) < 0.0f) *(f32*)(work + 0x2C) -= v;
        else *(f32*)(work + 0x2C) += v;
    }

    state = *(s32*)(work + 0x58);
    anim = *(s32*)(work + 0x54);
    if (state == 0) {
        *(f32*)(work + 0x4C) = 0.01f * (f32)land_hscale_data[anim];
        *(f32*)(work + 0x48) = 2.0f - *(f32*)(work + 0x4C);
        *(s32*)(work + 0x54) = anim + 1;
        if (anim + 1 > 0x1A) *(s32*)(work + 0x58) = 2;
    } else if (state == 1) {
        *(f32*)(work + 0x4C) = 0.01f * (f32)jump_hscale_data[anim];
        *(f32*)(work + 0x48) = 2.0f - *(f32*)(work + 0x4C);
        *(s32*)(work + 0x54) = anim + 1;
        if (anim + 1 > 0x11) *(s32*)(work + 0x58) = 2;
        if (anim + 1 > 7) {
            *(f32*)(work + 0x30) += *(f32*)(work + 0x3C);
            *(f32*)(work + 0x34) += *(f32*)(work + 0x40);
            *(f32*)(work + 0x38) += *(f32*)(work + 0x44);
            *(f32*)(work + 0x40) -= 1.0f;
        }
    } else if (state == 3) {
        *(f32*)(work + 0x4C) = 0.01f * (f32)jump2_hscale_data[anim];
        *(f32*)(work + 0x48) = 2.0f - *(f32*)(work + 0x4C);
        *(f32*)(work + 0x2C) = (f32)jump2_ry_data[anim + 1];
        *(s32*)(work + 0x54) = anim + 2;
        if (anim + 2 > 0x49) *(s32*)(work + 0x58) = 2;
    } else if (state == 2) {
        *(f32*)(work + 0x30) += *(f32*)(work + 0x3C);
        *(f32*)(work + 0x34) += *(f32*)(work + 0x40);
        *(f32*)(work + 0x38) += *(f32*)(work + 0x44);
        *(f32*)(work + 0x40) += *(f32*)(work + 0x50);
        if (*(f32*)(work + 0x34) < 0.0f) {
            s32 sequence = *(s32*)(work + 0x5C);
            *(s32*)(work + 0x5C) += 1;
            *(f32*)(work + 0x34) = 0.0f;
            *(s32*)(work + 0x54) = 0;
            if (sequence == 0) *(s32*)(work + 0x58) = 0;
            else if (sequence < 4) {
                *(s32*)(work + 0x58) = 1;
                *(f32*)(work + 0x3C) = sequence == 3 ? -2.0f : 2.0f;
                *(f32*)(work + 0x40) = 10.0f;
                *(f32*)(work + 0x44) = sequence == 2 ? -2.0f : 2.0f;
            } else if (sequence == 4) {
                *(s32*)(work + 0x58) = 1;
                *(f32*)(work + 0x3C) = 10.0f;
                *(f32*)(work + 0x40) = 14.0f;
                *(f32*)(work + 0x44) = 0.0f;
                *(f32*)(work + 0x50) = -1.0f;
            }
        }
    }

    if ((frame & 7) == 1) {
        effVaporN64Entry(*(f32*)(work + 4) + *(f32*)(work + 0x30) + (f32)(rand() % 60) - 30.0f,
                         *(f32*)(work + 8) + *(f32*)(work + 0x34) + (f32)(rand() % 100) + 50.0f,
                         *(f32*)(work + 0xC) + *(f32*)(work + 0x38), 4.0f, 2, 40);
        effSnowDustN64Entry(*(f32*)(work + 4) + *(f32*)(work + 0x30),
                            *(f32*)(work + 8) + *(f32*)(work + 0x34) + 10.0f,
                            *(f32*)(work + 0xC) + *(f32*)(work + 0x38) + 60.0f,
                            60.0f, 100.0f, 2.0f, 1, 10, 30);
    }
    dispEntry(4, 2, effSnowmanDisp, effect, dispCalcZ(&pos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSnowmanN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffSnowmanWork* work;
    f32 negOne;
    f32 negEight;
    f32 twoHundred;
    f32 zero;
    s32 zeroInt;
    s32 zeroInt2;
    s32 two;
    s32 i200;
    s32 i220;
    s32 i240;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_SnowmanN64_802fbfd0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x60);
    *(EffSnowmanWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSnowmanMain;
    *(u32*)entry |= 2;

    zeroInt = 0;
    work->type = type;
    work->frame = zeroInt;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }

    zeroInt2 = 0;
    zero = float_0_804260c0;
    work->field_24 = zeroInt2;
    two = 2;
    twoHundred = float_200_804260f0;
    i200 = 200;
    work->x = x;
    i220 = 220;
    i240 = 240;
    negEight = float_neg8_804260f4;
    work->y = y;
    negOne = float_neg1_804260cc;
    work->z = z;
    work->field_30 = zero;
    work->field_34 = twoHundred;
    work->field_38 = zero;
    work->field_3C = zero;
    work->field_40 = zero;
    work->field_44 = zero;
    work->field_54 = zeroInt2;
    work->field_58 = two;
    work->field_28 = scale;
    work->field_5C = zeroInt2;
    work->field_18 = i200;
    work->field_1C = i220;
    work->field_20 = i240;
    work->field_40 = negEight;
    work->field_2C = zero;
    work->field_50 = negOne;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
