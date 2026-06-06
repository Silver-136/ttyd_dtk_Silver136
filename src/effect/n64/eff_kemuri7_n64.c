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


u8 effKemuri7N64Entry(void) {
    return 0;
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
