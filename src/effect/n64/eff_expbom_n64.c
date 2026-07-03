#include "effect/n64/eff_expbom_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffExpBomWork {
    s16 unk_00;
    s16 life;
    s16 unk_04;
    s16 divTimer;
    s32 timer;
    f32 baseX;
    f32 baseY;
    f32 baseZ;
    f32 x;
    f32 y;
    f32 vx;
    f32 vy;
    u8 pad_28[4];
} EffExpBomWork;

void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fae10;
extern f32 float_1p25_804250d0;


u8 effExpBomDisp(int param_1, int param_2) {
    return 0;
}


void* effExpBomN64Entry(f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effExpBomMain(void*);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_ExpBomN64_802fae28[];
    extern f32 float_0_804250ac;
    extern f32 float_2_804250d4;
    extern f32 float_6p2832_804250b4;
    extern f32 float_90_804250b8;
    extern f32 float_360_804250c0;
    extern f32 bom_dir[];
    extern f32 bom_spd[];
    void* entry;
    u8* work;
    s32 i;
    f32 angle;
    f32 spd;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ExpBomN64_802fae28;
    *(s32*)((s32)entry + 8) = 10;
    work = __memAlloc(3, 0x1B8);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effExpBomMain;

    for (i = 0; i < *(s32*)((s32)entry + 8); i++) {
        *(s16*)(work + 0) = 0;
        *(s16*)(work + 4) = (s16)(rand() % 6 + 0x10);
        *(s16*)(work + 2) = *(s16*)(work + 4);
        *(s16*)(work + 6) = 4;
        *(s32*)(work + 8) = 0;
        *(f32*)(work + 0xC) = x;
        *(f32*)(work + 0x10) = y;
        *(f32*)(work + 0x14) = z;
        *(f32*)(work + 0x18) = float_0_804250ac;
        *(f32*)(work + 0x1C) = float_0_804250ac;
        angle = (float_90_804250b8 + bom_dir[i]) * float_6p2832_804250b4 / float_360_804250c0;
        spd = float_2_804250d4 * bom_spd[i];
        *(f32*)(work + 0x20) = spd * (f32)sin(angle);
        *(f32*)(work + 0x24) = -spd * (f32)cos(angle);
        work += 0x2C;
    }

    return entry;
}

void effExpBomMain(void* effect) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffExpBomWork* work;
    register s32 i;
    register s32 anyAlive;
    f32 div;

    anyAlive = 0;
    i = 0;
    work = *(EffExpBomWork**)((s32)effect + 0xC);
    zero = &vec3_802fae10;
    pos = *zero;
    pos.x = work->baseX;
    pos.y = work->baseY;
    pos.z = work->baseZ;
    dispPos = pos;

    while (i < *(s32*)((s32)effect + 0x8)) {
        if (work->life >= 0) {
            work->timer--;
            if (work->timer < 0) {
                anyAlive = 1;
                work->x += work->vx;
                work->y += work->vy;
                work->unk_00++;
                work->life--;
                if (work->life >= 0) {
                    work->divTimer--;
                    if (work->divTimer < 0) {
                        div = float_1p25_804250d0;
                        work->vx /= div;
                        work->vy /= div;
                    }
                }
            }
        }
        i++;
        work++;
    }

    if (anyAlive == 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effExpBomDisp, effect, dispCalcZ(&dispPos));
    }
}
