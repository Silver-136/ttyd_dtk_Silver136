#include "effect/n64/eff_vapor_n64.h"

typedef struct EffVaporWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 lifetime;
    s32 frame;
    s32 r0;
    s32 g0;
    s32 b0;
    s32 a0;
    s32 r1;
    s32 g1;
    s32 b1;
    s32 a1;
    f32 rot;
    f32 yVel;
    f32 scale;
} EffVaporWork;

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effDelete(void* effect);
void dispEntry(s32 cameraId, s32 layer, void* callback, void* param);
f32 dispCalcZ(Vec3* pos);

extern const char str_VaporN64_802fc230[];
extern f32 float_0_80426464;
extern f32 float_0p5_8042645c;
extern f32 float_0p02_80426480;
extern f32 float_neg1p5_80426484;
extern f32 float_1_80426488;
extern Vec3 vec3_802fc210[];

u8 effVaporDisp(void) {
    return 0;
}


void effVaporMain(void* effect) {
    EffVaporWork* work;
    Vec3 dispPos;
    Vec3 pos;
    u32 flags;
    s32 type;

    work = *(EffVaporWork**)((s32)effect + 0xC);
    pos = vec3_802fc210[0];
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(u32*)effect;
    type = work->type;
    if (flags & 4) {
        *(u32*)effect = flags & ~4;
        work->lifetime = 16;
    }

    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;

    if (work->lifetime < 0) {
        effDelete(effect);
    } else {
        if (work->lifetime < 16) {
            work->a0 = work->lifetime << 4;
        }
        if (type < 2) {
            if (type >= 0) {
                work->yVel += float_0p02_80426480;
            }
        }
        work->y += work->yVel;
        if (work->yVel > float_0p5_8042645c) {
            work->yVel = float_0p5_8042645c;
        }
        if (work->yVel < float_neg1p5_80426484) {
            work->yVel = float_neg1p5_80426484;
        }
        work->rot += float_1_80426488;
        dispCalcZ(&dispPos);
        dispEntry(4, 2, effVaporDisp, effect);
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effVaporN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    void* entry;
    EffVaporWork* work;
    s32 full;
    s32 zeroInt;
    f32 zero;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_VaporN64_802fc230;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x44);
    *(EffVaporWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effVaporMain;
    *(u32*)entry |= 2;

    zeroInt = 0;
    work->type = type;
    work->frame = zeroInt;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }

    full = 0xFF;
    zero = float_0_80426464;
    work->a0 = full;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->r0 = full;
    work->g0 = full;
    work->b0 = full;
    work->r1 = full;
    work->g1 = full;
    work->b1 = full;
    work->a1 = full;
    work->rot = zero;
    work->yVel = zero;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
