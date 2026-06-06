#include "effect/n64/eff_coin_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffCoinSubWork {
    s32 unk_00;
    s32 frame;
    u8 pad_08[0x18];
    s32 delay;
} EffCoinSubWork;

typedef struct EffCoinWork {
    u8 pad_00[8];
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    u8 pad_1c[8];
    EffCoinSubWork sub[1];
} EffCoinWork;

void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fad40;


u8 effCoinDisp(void) {
    return 0;
}


u8 effCoinN64Entry(void) {
    return 0;
}


void effCoinMain(void* effect) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffCoinWork* work;
    register s32 i;
    register EffCoinSubWork* sub;

    work = *(EffCoinWork**)((s32)effect + 0xC);
    zero = &vec3_802fad40;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    work->timer--;
    work->frame++;
    if (work->timer < 0) {
        effDelete(effect);
    } else {
        i = 1;
        sub = work->sub;
        while (i < *(s32*)((s32)effect + 0x8)) {
            if (sub->delay > 0) {
                sub->delay--;
                if (sub->delay > 0) {
                    i++;
                    sub++;
                    continue;
                }
            }
            sub->frame++;
            if (sub->frame >= 10) {
                sub->delay = -1;
            }
            i++;
            sub++;
        }
        dispEntry(4, 2, effCoinDisp, effect, dispCalcZ(&dispPos));
    }
}
