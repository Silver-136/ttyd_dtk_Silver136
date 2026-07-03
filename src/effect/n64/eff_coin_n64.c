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


void* effCoinN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effCoinMain(void*);
    extern char str_CoinN64_802fad58[];
    extern s32 _data[];
    extern f32 float_0p01_80424f64;
    void* entry;
    u8* work;
    u8* sub;
    s32 i;
    s32* data;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_CoinN64_802fad58;
    *(s32*)((s32)entry + 8) = 5;
    work = __memAlloc(3, 0xB4);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effCoinMain;

    *(s32*)work = type;
    *(s32*)(work + 0x18) = 0;
    *(s32*)(work + 0x14) = 0xE;
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;

    sub = work + 0x24;
    data = _data;
    for (i = 1; i < 5; i++) {
        *(f32*)(sub + 8) = (f32)data[1];
        *(f32*)(sub + 0xC) = (f32)data[2];
        *(f32*)(sub + 0x10) = (f32)data[3];
        *(s32*)(sub + 4) = -1;
        *(s32*)(sub + 0x20) = data[0];
        *(f32*)(sub + 0x1C) = scale * (f32)data[4] * float_0p01_80424f64;
        data += 5;
        sub += 0x24;
    }

    return entry;
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
