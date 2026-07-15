#include "effect/n64/eff_pikkyolo_n64.h"


void* effPikkyoloN64Entry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 time) {
    extern void* effEntry(void); extern void* __memAlloc(s32,s32); extern void effPikkyoloMain(void*); extern char str_PikkyoloN64_802fbcb0[]; extern s32 rand(void); extern f32 float_0p5_80425d0c,float_0p8_80425d18;
    void* entry;u8* work; s32 i;
    entry=effEntry();*(char**)((s32)entry+0x14)=str_PikkyoloN64_802fbcb0;*(s32*)((s32)entry+8)=6;work=__memAlloc(3,0x168);*(u8**)((s32)entry+0xC)=work;*(void**)((s32)entry+0x10)=effPikkyoloMain;*(s32*)entry|=2;
    for(i=0;i<6;i++,work+=0x3C){*(s32*)work=type;*(f32*)(work+4)=x;*(f32*)(work+8)=y;*(f32*)(work+0xC)=z;*(f32*)(work+0x10)=float_0p5_80425d0c*(f32)(rand()%11-5);*(f32*)(work+0x14)=float_0p8_80425d18*(f32)(rand()%6+1);*(f32*)(work+0x18)=float_0p5_80425d0c*(f32)(rand()%10-5);*(s32*)(work+0x1C)=time<1?1000:time;*(s32*)(work+0x20)=0;*(f32*)(work+0x24)=scale;*(s16*)(work+0x32)=0xFF;}
    return entry;
}

u8 effPikkyoloDisp(void) {
    return 0;
}


void effPikkyoloMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPikkyoloDisp(void);
    extern const Vec3 vec3_802fbc98;
    extern f32 float_0p5_80425d0c;
    extern f32 float_0_80425d08;
    extern f32 float_neg0p7_80425d14;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;
    s32 offset;
    f32 half;
    f32 zero;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbc98;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x1C) = 0x10;
    }
    if (*(s32*)(work + 0x1C) < 0x3E8) {
        *(s32*)(work + 0x1C) -= 1;
    }
    *(s32*)(work + 0x20) += 1;
    if (*(s32*)(work + 0x1C) < 0) {
        effDelete(effect);
        return;
    }
    if (*(s32*)(work + 0x1C) < 8) {
        *(s16*)(work + 0x32) = *(s32*)(work + 0x1C) << 5;
    }

    half = float_0p5_80425d0c;
    zero = float_0_80425d08;
    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work += 0x3C) {
        *(f32*)(work + 0x14) -= half;
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 8) += *(f32*)(work + 0x14);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        if (*(f32*)(work + 8) < zero && *(f32*)(work + 0x14) < zero) {
            *(f32*)(work + 8) = zero;
            *(f32*)(work + 0x14) *= float_neg0p7_80425d14;
        }
        offset = (((i & 1) << 1) - 1) * 0xE;
        *(f32*)(work + 0x28) += (f32)offset;
    }
    dispEntry(4, 2, effPikkyoloDisp, effect, dispCalcZ(&dispPos));
}

