#include "effect/n64/eff_snowfall_n64.h"


u8 effSnowfallDisp(int param_1, int param_2) {
    return 0;
}


u8 effSnowfallN64Entry(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void cam_clip_chk(u8* work, s32 xRange, s32 yRange, s32 timer) {
    extern void* camGetPtr(s32 camera);
    extern f32 float_0p5_804260bc;
    extern f32 float_0_80426098;
    u8* cam;
    f32 centerX;
    f32 centerY;
    f32 centerZ;
    s32 delta;
    s32 count;

    cam = camGetPtr(4);
    centerX = *(f32*)(cam + 0x18);
    centerY = (*(f32*)(cam + 0x1C) + *(f32*)(cam + 0x10)) * float_0p5_804260bc;
    centerZ = (*(f32*)(cam + 0x20) + *(f32*)(cam + 0x118)) * float_0p5_804260bc;

    delta = (s32)(*(f32*)(work + 8) - centerX);
    if (delta > xRange) {
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 8) = centerX - (f32)xRange + (f32)delta;
    } else if (delta < -xRange) {
        delta = -delta;
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 8) = centerX + (f32)xRange - (f32)delta;
    }

    delta = (s32)(*(f32*)(work + 0xC) - centerY);
    if (delta > yRange) {
        count = delta / yRange;
        delta -= count * yRange;
        *(f32*)(work + 0xC) = centerY - (f32)yRange + (f32)delta;
    } else if (delta < -yRange) {
        if (timer < 100) {
            *(f32*)(work + 0x18) = float_0_80426098;
            *(s32*)(work + 0x28) = 0;
        } else {
            delta = -delta;
            count = delta / yRange;
            delta -= count * yRange;
            *(f32*)(work + 0xC) = centerY + (f32)yRange - (f32)delta;
        }
    }

    delta = (s32)(*(f32*)(work + 0x10) - centerZ);
    if (delta > xRange) {
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 0x10) = centerZ - (f32)xRange + (f32)delta;
    } else if (delta < -xRange) {
        delta = -delta;
        count = delta / xRange;
        delta -= count * xRange;
        *(f32*)(work + 0x10) = centerZ + (f32)xRange - (f32)delta;
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effSnowfallMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void* camGetPtr(s32 camera);
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern s32 cam_clip_chk(void* work, u32 flags, s32 value, s32 timer);
    extern void effSnowfallDisp(void);
    extern const Vec3 vec3_802fbf90;
    extern f32 float_166_804260a4;
    extern void* gp;
    u8* work;
    u8* part;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    s32 timer;
    s32 count;
    s32 value;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbf90;
    pos.x = *(f32*)(work + 8);
    pos.y = *(f32*)(work + 0xC);
    pos.z = *(f32*)(work + 0x10);
    dispPos = pos;
    camGetPtr(4);
    if (*(s32*)(work + 0x20) < 100) {
        *(s32*)(work + 0x20) -= 1;
    }
    if (*(s32*)(work + 0x20) < 0) {
        effDelete(effect);
        return;
    }
    *(s32*)(work + 0x24) += 1;
    timer = *(s32*)(work + 0x20);
    count = *(s32*)(work + 0x2C);
    if (timer < 10) {
        *(s32*)(work + 0x28) = timer * 25;
    }
    value = (s32)float_166_804260a4;
    part = work + 0x34;
    for (i = 0; i < count; i++, part += 0x34) {
        if (*(s32*)(part + 0x30) > 0) {
            *(s32*)(part + 0x30) -= 1;
            if (*(s32*)(part + 0x30) > 0) {
                continue;
            }
        }
        *(f32*)(part + 8) += *(f32*)(part + 0x14);
        *(f32*)(part + 0xC) += *(f32*)(part + 0x18);
        *(f32*)(part + 0x10) += *(f32*)(part + 0x1C);
        cam_clip_chk(part, 0x226, value, timer);
    }
    if (*(s32*)((s32)gp + 0x14) == 0) {
        dispEntry(4, 0xA, effSnowfallDisp, effect, dispCalcZ(&dispPos));
    } else {
        dispEntry(7, 0xA, effSnowfallDisp, effect, dispCalcZ(&dispPos));
    }
}

