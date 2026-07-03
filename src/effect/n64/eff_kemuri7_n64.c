#include "effect/n64/eff_kemuri7_n64.h"

typedef f32 Mtx[3][4];

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffKemuri7Work {
    s32 active;
    u8 pad_04[2];
    s16 timer;
    u8 alpha;
    u8 pad_09[3];
    f32 x;
    f32 y;
    f32 z;
    u8 pad_18[0x10];
    f32 angle;
    u8 pad_2c[4];
    Mtx matrix;
    u8 pad_60[0x28];
} EffKemuri7Work;

void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, s32 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx ab);

extern const Vec3 vec3_802fb280;
extern f32 float_deg2rad_80425774;


u8 effKemuri7Disp(int param_1, void* effEntry) {
    return 0;
}


void* effKemuri7N64Entry(f32 x, f32 y, f32 z, f32 angle, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effKemuri7Main(void*);
    extern f32 reviseAngle(f32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_Kemuri7N64_802fb28c[];
    extern f32 float_0_80425768;
    extern f32 float_1_80425770;
    extern f32 float_30_80425778;
    extern f32 float_15_8042577c;
    extern f32 float_180_80425780;
    extern f32 float_6p2832_80425784;
    extern f32 float_360_80425788;
    extern f32 float_45_8042578c;
    extern f32 float_5_80425790;
    void* entry;
    u8* work;
    f32 revised;
    f32 rad;
    f32 adjusted;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri7N64_802fb28c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x88);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri7Main;
    *(s32*)work = 1;
    *(f32*)(work + 0x6C) = float_0_80425768;
    *(f32*)(work + 0xC) = x;
    *(f32*)(work + 0x10) = y;
    *(f32*)(work + 0x14) = z;
    *(f32*)(work + 0x18) = float_1_80425770;
    *(f32*)(work + 0x1C) = float_1_80425770;
    *(f32*)(work + 0x20) = float_1_80425770;
    *(u8*)(work + 8) = 0xFF;
    *(f32*)(work + 0x60) = scale;
    *(f32*)(work + 0x70) = float_0_80425768;
    *(f32*)(work + 0x74) = float_0_80425768;
    *(s16*)(work + 6) = 0xFF;
    *(f32*)(work + 0x24) = float_0_80425768;
    *(f32*)(work + 0x28) = angle + (float_30_80425778 * scale) - float_15_8042577c;
    *(f32*)(work + 0x2C) = float_0_80425768;

    revised = reviseAngle(angle);
    rad = (float_6p2832_80425784 * revised) / float_360_80425788;
    *(f32*)(work + 0x80) = (f32)sin(rad);
    *(f32*)(work + 0x84) = (f32)cos(rad);

    adjusted = float_45_8042578c + revised - (float_180_80425780 * scale);
    rad = (float_6p2832_80425784 * adjusted) / float_360_80425788;
    *(f32*)(work + 0xC) += float_5_80425790 * (f32)sin(rad);
    *(f32*)(work + 0x14) += float_5_80425790 * (f32)cos(rad);
    *(u8*)(work + 8) = 0xFF;
    return entry;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effKemuri7Main(void* effEntry) {
    Mtx rot;
    Mtx trans;
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffKemuri7Work* work;
    register s32 i;
    register s32 anyAlive;
    s32 timer;

    anyAlive = 0;
    i = 0;
    work = *(EffKemuri7Work**)((s32)effEntry + 0xC);
    zero = &vec3_802fb280;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    while (i < *(s32*)((s32)effEntry + 0x8)) {
        if (work->active != 0) {
            timer = work->timer - 1;
            work->timer = timer;
            if ((s16)timer <= 0) {
                work->active = 0;
            } else {
                anyAlive = 1;
                PSMTXTrans(trans, work->x, work->y, work->z);
                PSMTXRotRad(rot, 0x79, float_deg2rad_80425774 * work->angle);
                PSMTXConcat(trans, rot, work->matrix);
                work->alpha--;
            }
        }
        i++;
        work++;
    }

    if (anyAlive == 0) {
        effDelete(effEntry);
    } else {
        dispEntry(4, 2, effKemuri7Disp, effEntry, dispCalcZ(&dispPos));
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
