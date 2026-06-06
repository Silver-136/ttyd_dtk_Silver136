#include "effect/n64/eff_kemuri2_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffKemuri2Work {
    s32 active;
    s16 type;
    u8 pad_06[2];
    f32 x;
    f32 y;
    f32 z;
    u8 pad_14[0x30];
    f32 moveScale;
    f32 yVel;
    f32 yAccel;
    f32 vx;
    f32 vz;
    s32 animId;
    s32 animFrame;
    s32 animValue;
} EffKemuri2Work;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fb1d8;
extern char str_Kemuri2N64_802fb1f0[];
extern s8* anim_data_table[];
extern f32 float_0p33333_80425650;
extern f32 float_1p75_80425654;
extern f32 float_neg0p16_80425658;


u8 main_dl(int param_1) {
    return 0;
}


u8 effKemuri2Disp(void) {
    return 0;
}


void effKemuri2Main(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffKemuri2Work* work;
    register s32 i;
    s32 frame;
    s32 value;
    s8* anim;

    work = *(EffKemuri2Work**)((s32)effEntry + 0xC);
    zero = &vec3_802fb1d8;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    anim = anim_data_table[work->animId];
    frame = work->animFrame;
    work->animFrame = frame + 1;
    value = anim[frame];
    work->animValue = value;
    if (value < 0) {
        effDelete(effEntry);
    } else {
        i = 0;
        while (i < *(s32*)((s32)effEntry + 0x8)) {
            i++;
            work->x += work->moveScale * work->vx;
            work->z += work->moveScale * work->vz;
            work->yVel += work->yAccel;
            work->y += work->yVel;
            work++;
        }
        dispEntry(4, 2, effKemuri2Disp, effEntry, dispCalcZ(&dispPos));
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effKemuri2N64Entry(s32 type, f32 x, f32 y, f32 z, f32 vx, f32 vz) {
    void* entry = effEntry();
    EffKemuri2Work* work;
    f32 yAccel;
    f32 yVel;
    f32 moveScale;

    *(char**)((s32)entry + 0x14) = str_Kemuri2N64_802fb1f0;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x64);
    *(EffKemuri2Work**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuri2Main;
    work->animId = type == 2;
    moveScale = float_0p33333_80425650;
    work->animFrame = 0;
    yVel = float_1p75_80425654;
    work->type = type;
    yAccel = float_neg0p16_80425658;
    work->active = 1;
    work->vx = vx;
    work->vz = vz;
    work->x = x;
    work->y = y;
    work->z = z;
    work->moveScale = moveScale;
    work->yVel = yVel;
    work->yAccel = yAccel;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
