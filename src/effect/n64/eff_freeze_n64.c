#include "effect/n64/eff_freeze_n64.h"

typedef struct EffFreezeWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 alpha;
    u8 color2R;
    u8 color2G;
    u8 color2B;
    u8 color2A;
    f32 scale;
    u8 unk_24;
    u8 unk_25;
    u8 pad_26[2];
    void* snowDust;
} EffFreezeWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void* effSnowDustN64Entry(s32, s32, s32, f32, f32, f32, f32, f32, f32);

extern char str_FreezeN64_802fafe8[];
extern f32 float_10_80425388;
extern f32 float_32_804253a8;
extern f32 float_30_804253ac;


void effFreezeMain(void* effect) {
    extern s32 rand(void);
    extern void* effGlassN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 timer);
    extern void effDelete(void* effect);
    extern void effVaporN64Entry(f32 x, f32 y, f32 z, s32 type, s32 timer);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effFreezeDisp(s32 cameraId, void* effect);
    extern f32 float_0_80425384;
    extern f32 float_10_80425388;
    extern f32 float_3_8042538c;
    extern f32 float_0p3_80425390;
    extern f32 float_0p1_80425394;
    extern f32 float_0p7_80425398;
    extern f32 float_0p01_8042539c;
    extern f32 float_neg0p1_804253a0;
    extern f32 float_40_804253a4;

    u8* work;
    f32 pos[3];
    s32 timer;
    s32 frame;
    s32 i;
    s32 r;
    void* glass;
    u8* glassWork;
    void* snowDust;
    u8* snowWork;
    f32 scale;

    work = *(u8**)((s32)effect + 0xC);
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x10;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        (*(s32*)(work + 0x10))--;
    }
    (*(s32*)(work + 0x14))++;

    timer = *(s32*)(work + 0x10);
    if (timer < 0) {
        for (i = 0; i < 10; i++) {
            r = rand();
            scale = *(f32*)(work + 0x20);
            glass = effGlassN64Entry(
                (*(f32*)(work + 4) + (f32)(rand() % 0x14)) - float_10_80425388,
                (*(f32*)(work + 0x20) * (f32)(rand() % 0x28)) + *(f32*)(work + 8),
                float_3_8042538c + *(f32*)(work + 0xC),
                scale * ((float_0p1_80425394 * float_0p7_80425398 * (f32)(r % 10)) + float_0p3_80425390),
                (i & 1) + 2,
                i + 0x1E);
            glassWork = *(u8**)((s32)glass + 0xC);
            *(f32*)(glassWork + 0x44) = float_0p1_80425394 * (f32)(rand() % 10);
            *(f32*)(glassWork + 0x48) = (float_0p01_8042539c * (f32)(rand() % 0x1E)) + float_0p1_80425394;
            *(f32*)(glassWork + 0x3C) = (f32)(rand() % 0x168);
            *(f32*)(glassWork + 0x40) = (f32)(rand() % 0x14);
            *(f32*)(glassWork + 0x4C) = (f32)((rand() % 10) - 5);
            *(f32*)(glassWork + 0x50) = (f32)((rand() % 10) - 5);
            *(f32*)(glassWork + 0x54) = float_0_80425384;
            *(f32*)(glassWork + 0x58) = float_neg0p1_804253a0;
        }
        effDelete(*(void**)(work + 0x28));
        effDelete(effect);
        return;
    }

    frame = *(s32*)(work + 0x14);
    (*(s8*)(work + 0x25))--;
    if (*(s8*)(work + 0x25) < 0) {
        *(s8*)(work + 0x25) = (s8)((rand() % 0x3C) + 8);
    }
    if (*(s8*)(work + 0x25) < 8) {
        *(u8*)(work + 0x24) = *(s8*)(work + 0x25) / 2;
    } else {
        *(u8*)(work + 0x24) = 0;
    }

    if ((timer > 0x10) && ((frame % 0x10) == 0)) {
        effVaporN64Entry(*(f32*)(work + 4),
                         (float_40_804253a4 * *(f32*)(work + 0x20)) + *(f32*)(work + 8),
                         *(f32*)(work + 0xC), 2, 0x1E);
    }

    snowDust = *(void**)(work + 0x28);
    snowWork = *(u8**)((s32)snowDust + 0xC);
    *(f32*)(snowWork + 4) = *(f32*)(work + 4);
    *(f32*)(snowWork + 8) = float_10_80425388 + *(f32*)(work + 8);
    *(f32*)(snowWork + 0xC) = *(f32*)(work + 0xC);

    dispEntry(4, 2, effFreezeDisp, effect, dispCalcZ(pos));
}

u8 effFreezeDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFreezeN64Entry(s32 type, s32 param2, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffFreezeWork* work;
    s32 flags;
    f32 ten;
    f32 dustY;

    *(char**)((s32)entry + 0x14) = str_FreezeN64_802fafe8;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x2C);
    *(EffFreezeWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFreezeMain;
    flags = *(s32*)entry;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    if (param2 <= 0) {
        work->timer = 1000;
    } else {
        work->timer = param2;
    }
    ten = float_10_80425388;
    work->alpha = 0xFF;
    work->x = x;
    work->y = y;
    dustY = ten + y;
    work->z = z;
    work->scale = scale;
    work->colorR = 0xFF;
    work->colorG = 0xFF;
    work->colorB = 0xFF;
    work->color2R = 0xF0;
    work->color2G = 0xFF;
    work->color2B = 0xFF;
    work->color2A = 0xFF;
    work->unk_25 = 0;
    work->unk_24 = 0;
    work->snowDust = effSnowDustN64Entry(1, 4, 0, x, dustY, z,
                                         float_32_804253a8, float_30_804253ac, scale);

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
