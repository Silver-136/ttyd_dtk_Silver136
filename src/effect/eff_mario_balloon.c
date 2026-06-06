#include "effect/eff_mario_balloon.h"


u8 effMarioBalloonDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effMarioBalloonMain(void* effEntry) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void* marioGetPtr(void);
    extern f32 float_10_80422d48;
    extern f32 float_4_80422d4c;
    extern Vec3 vec3_802f3a90[];
    extern f32 dispCalcZ(Vec3* pos);
    extern u8 dispEntry();
    void* work = *(void**)((s32)effEntry + 0xC);
    void* npc = *(void**)((s32)work + 0x18);
    Vec3 pos;

    if (npc == 0) {
        void* mario = marioGetPtr();
        f32 x = *(f32*)((s32)mario + 0x8C);
        f32 add = float_10_80422d48;
        *(f32*)((s32)work + 4) = x;
        *(f32*)((s32)work + 8) = *(f32*)((s32)mario + 0x90) + *(f32*)((s32)mario + 0x1BC) + add;
        *(f32*)((s32)work + 0xC) = *(f32*)((s32)mario + 0x94);
    } else {
        f32 x = *(f32*)((s32)npc + 0x8C);
        f32 add = float_10_80422d48;
        *(f32*)((s32)work + 4) = x;
        *(f32*)((s32)work + 8) = *(f32*)((s32)npc + 0x90) + *(f32*)((s32)npc + 0x150) + add;
        *(f32*)((s32)work + 0xC) = *(f32*)((s32)npc + 0x94);
    }

    *(f32*)((s32)work + 0x10) += float_4_80422d4c;
    pos = vec3_802f3a90[0];
    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    return dispEntry(*(s32*)((s32)work + 0x1C), 8, effMarioBalloonDisp, effEntry, dispCalcZ(&pos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMarioBalloonEntry(int type, void* npc) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern char str_MarioBalloon_802f3a9c[];
    extern f32 float_0_80422d28;
    extern f32 float_0p4_80422d50;
    extern f32 float_0p8_80422d54;
    void* entry;
    void* work;

    entry = effEntry();
    *(void**)((s32)entry + 0x14) = str_MarioBalloon_802f3a9c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, *(s32*)((s32)entry + 8) << 5);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMarioBalloonMain;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 0x10) = float_0_80422d28;
    *(void**)((s32)work + 0x18) = npc;
    *(s32*)((s32)work + 0x1C) = 4;
    switch (type) {
        case 0:
            *(f32*)((s32)work + 0x14) = float_0p4_80422d50;
            break;
        case 1:
            *(f32*)((s32)work + 0x14) = float_0p8_80422d54;
            break;
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
