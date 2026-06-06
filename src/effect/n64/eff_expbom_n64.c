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


u8 effExpBomN64Entry(void) {
    return 0;
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
