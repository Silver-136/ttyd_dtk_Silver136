#include "effect/eff_charge.h"


u8 effChargeDisp(void) {
    return 0;
}


u8 effChargeMain(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effChargeEntry(s32 type, s32 arg, f32 x, f32 y, f32 z, f32 scale) {
    typedef struct EffectEntry {
        s32 flags;
        s32 unk4;
        s32 live;
        void* work;
        void* main;
        char* name;
    } EffectEntry;
    extern EffectEntry* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern void effChargeMain(void);
    extern char str_Charge_802f9728[];

    EffectEntry* effect;
    u8* work;
    u8* particle;
    s32 i;
    s32 prev;
    s32 cur;
    f32 angle;

    effect = effEntry();
    effect->name = str_Charge_802f9728;
    effect->live = 31;
    work = __memAlloc(3, 0x8B8);
    effect->work = work;
    effect->main = effChargeMain;

    *(s32*)(work + 4) = type;
    *(s32*)(work + 0x1C) = 0;
    *(s32*)(work + 0x18) = arg;
    *(s32*)(work + 0x14) = 0;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    *(f32*)(work + 0x28) = scale;

    switch (type) {
        case 0:
            *(s32*)(work + 0x30) = 255;
            *(s32*)(work + 0x34) = 0;
            *(s32*)(work + 0x38) = 255;
            *(s32*)(work + 0x3C) = 255;
            *(s32*)(work + 0x40) = 255;
            *(s32*)(work + 0x44) = 255;
            break;
        case 1:
            *(s32*)(work + 0x30) = 255;
            *(s32*)(work + 0x34) = 20;
            *(s32*)(work + 0x38) = 33;
            *(s32*)(work + 0x3C) = 50;
            *(s32*)(work + 0x40) = 20;
            *(s32*)(work + 0x44) = 10;
            break;
        default:
            *(s32*)(work + 0x30) = 0;
            *(s32*)(work + 0x34) = 0;
            *(s32*)(work + 0x38) = 0;
            *(s32*)(work + 0x3C) = 255;
            *(s32*)(work + 0x40) = 255;
            *(s32*)(work + 0x44) = 255;
            break;
    }

    particle = work + 0x48;
    for (i = 1; i < 31; i += 2) {
        prev = i - 1;
        cur = i;
        angle = 360.0f * (f32)prev;
        *(f32*)(particle + 0x20) = (angle * 9.0f) / 30.0f;
        *(f32*)(particle + 0x24) = 0.0f;
        *(f32*)(particle + 0x2C) = (f32)((prev * 100) / 30);
        angle = 360.0f * (f32)cur;
        *(s32*)particle = 0;
        *(f32*)(particle + 0x68) = (angle * 9.0f) / 30.0f;
        *(f32*)(particle + 0x6C) = 0.0f;
        *(f32*)(particle + 0x74) = (f32)((cur * 100) / 30);
        *(s32*)(particle + 0x48) = 0;
        particle += 0x90;
    }

    return effect;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

