#include "effect/n64/eff_small_star_n64.h"

void effSmallStarN64CamChg(void* effect, s8 camId) {
    void* work = *(void**)((s32)effect + 0xC);
    s32 i;

    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work = (void*)((s32)work + 0x34)) {
        *(s8*)((s32)work + 0x30) = camId;
    }
}


void* effSmallStarN64Entry(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, s32 type, s32 count) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effSmallStarMain(void*);
    extern char str_SmallStarN64_802fbf68[];
    extern f64 sqrt(f64);
    extern f64 sin(f64);
    extern f64 cos(f64);
    static f32 rotation;
    void* entry;
    u8* part;
    f32 length;
    f32 nx, ny, nz;
    f32 sideX, sideY;
    f32 angle;
    f32 spread;
    s32 i;

    entry = effEntry();
    if (count == 0) return 0;
    length = (f32)sqrt((f64)(dirX * dirX + dirY * dirY + dirZ * dirZ));
    if (length == 0.0f) return 0;
    nx = -dirX / length;
    ny = -dirY / length;
    nz = -dirZ / length;
    sideX = ny == 0.0f ? 1.0f : -nx / ny;
    sideY = ny == 0.0f ? 0.0f : 1.0f;
    length = (f32)sqrt((f64)(sideX * sideX + sideY * sideY));
    sideX /= length;
    sideY /= length;

    *(char**)((s32)entry + 0x14) = str_SmallStarN64_802fbf68;
    *(s32*)((s32)entry + 8) = count;
    part = __memAlloc(3, count * 0x34);
    *(u8**)((s32)entry + 0xC) = part;
    *(void**)((s32)entry + 0x10) = effSmallStarMain;
    for (i = 0; i < count; i++, part += 0x34) {
        angle = count == 1 ? rotation : rotation + 360.0f * (f32)i / (f32)(count - 1);
        spread = type == 3 && count != 1 ? 100.0f * (f32)i / (f32)(count - 1) - 50.0f : 0.0f;
        *(s32*)part = type;
        *(f32*)(part + 4) = x;
        *(f32*)(part + 8) = y;
        *(f32*)(part + 0xC) = z;
        *(f32*)(part + 0x10) = nx * 8.0f + sideX * 8.0f * (f32)cos((6.2832f * angle) / 360.0f);
        *(f32*)(part + 0x14) = ny * 8.0f + sideY * 8.0f * (f32)cos((6.2832f * angle) / 360.0f) + spread * 0.01f;
        *(f32*)(part + 0x18) = nz * 8.0f + 8.0f * (f32)sin((6.2832f * angle) / 360.0f);
        if (type == 2) {
            *(f32*)(part + 0x10) *= 0.5f;
            *(f32*)(part + 0x14) *= 0.5f;
            *(f32*)(part + 0x18) *= 0.5f;
            *(s32*)(part + 0x28) = 0x28;
        } else {
            *(s32*)(part + 0x28) = 0x18;
        }
        *(f32*)(part + 0x1C) = 0.0f;
        *(f32*)(part + 0x20) = type == 0 ? 0.0f : 20.0f;
        *(s32*)(part + 0x24) = 0xFF;
        *(s32*)(part + 0x2C) = 0;
        *(s8*)(part + 0x30) = 4;
    }
    rotation += 15.0f;
    if (rotation > 360.0f) rotation = 0.0f;
    return entry;
}

void effSmallStarMain(void* effect) {
    extern void effDelete(void*);extern f32 dispCalcZ(void*);extern void dispEntry(s32,s32,void*,void*,f32);extern void effSmallStarDisp(void);extern f32 float_0p8_80426030,float_0p94_80426028,float_0_8042602c,float_0p7_80426038,float_0p6_80426034,float_1_80426014;
    u8* work=*(u8**)((s32)effect+0xC);s32 timer,i,type=*(s32*)work;timer=*(s32*)(work+0x28)-1;*(s32*)(work+0x28)=timer;*(s32*)(work+0x2C)+=1;if(timer<0){effDelete(effect);return;}
    for(i=0;i<*(s32*)((s32)effect+8);i++,work+=0x34){if(type==2){*(f32*)(work+0x14)=float_1_80426014;*(f32*)(work+0x10)*=float_0p94_80426028;*(f32*)(work+0x18)*=float_0p94_80426028;if(*(f32*)(work+0x14)<float_0_8042602c){*(f32*)(work+0x20)*=float_0p8_80426030;*(s32*)(work+0x24)=(s32)((f32)*(s32*)(work+0x24)*float_0p6_80426034);}}else{if(timer>5){*(f32*)(work+0x10)*=float_0p8_80426030;*(f32*)(work+0x14)*=float_0p8_80426030;*(f32*)(work+0x18)*=float_0p8_80426030;}if(timer<5){*(f32*)(work+0x20)*=float_0p8_80426030;*(s32*)(work+0x24)=(s32)((f32)*(s32*)(work+0x24)*float_0p6_80426034);*(f32*)(work+0x10)*=float_0p7_80426038;*(f32*)(work+0x14)*=float_0p7_80426038;*(f32*)(work+0x18)*=float_0p7_80426038;}}*(f32*)(work+4)+=*(f32*)(work+0x10);*(f32*)(work+8)+=*(f32*)(work+0x14);*(f32*)(work+0xC)+=*(f32*)(work+0x18);*(f32*)(work+0x1C)+=*(f32*)(work+0x20);}dispEntry(*(u8*)(*(u8**)((s32)effect+0xC)+0x30),2,effSmallStarDisp,effect,dispCalcZ(*(u8**)((s32)effect+0xC)+4));
}

u8 effSmallStarDisp(s32 param_1, void* effEntry) {
    return 0;
}
