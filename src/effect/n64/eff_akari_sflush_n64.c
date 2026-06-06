#include "effect/n64/eff_akari_sflush_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffAkariSflushWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    s32 r;
    s32 g;
    s32 b;
    s32 alpha;
    f32 scale;
    f32 scale2;
    s32 unk30;
} EffAkariSflushWork;

void* effEntry(void);
void effDelete(void* effect);
void* __memAlloc(s32 heap, s32 size);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);

extern const Vec3 vec3_802fab08;
extern char str_AkariSflushN64_802fab20[];
extern f32 float_1_80424c8c;
extern f32 float_0p3_80424c98;

u8 effAkariSflushDisp(void) {
    return 0;
}


void effAkariSflushMain(void* effect) {
    void* entry = effect;
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffAkariSflushWork* work;
    s32 timer;
    s32 frame;

    work = *(EffAkariSflushWork**)((s32)entry + 0xC);
    zero = &vec3_802fab08;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    if (*(u32*)entry & 4) {
        *(u32*)entry &= ~4;
        work->timer = 0x10;
    }

    if (work->timer < 1000) {
        work->timer--;
    }
    work->frame++;

    timer = work->timer;
    if (timer < 0) {
        effDelete(entry);
    } else {
        frame = work->frame;
        if (timer < 8) {
            work->scale2 += float_0p3_80424c98 * (f32)(8 - timer);
        }
        if (frame < 0x10) {
            work->alpha = frame * 0x10 + 0xF;
        }
        if (timer < 8) {
            work->alpha = timer * 0x20 + 0x1F;
        }
        dispEntry(4, 2, effAkariSflushDisp, entry, dispCalcZ(&dispPos));
    }
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effAkariSflushN64Entry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffAkariSflushWork* work;
    f32 scale2;

    *(char**)((s32)entry + 0x14) = str_AkariSflushN64_802fab20;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x34);
    *(EffAkariSflushWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effAkariSflushMain;
    *(u32*)entry |= 2;

    work->type = type;
    work->frame = 0;
    if (timer <= 0) {
        work->timer = 1000;
    } else {
        work->timer = timer;
    }
    work->alpha = 0xFF;
    scale2 = float_1_80424c8c;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->r = 0xFF;
    work->g = 0xE6;
    work->b = 0x32;
    work->unk30 = 0;
    work->scale2 = scale2;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
