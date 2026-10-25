#include "eff/eff_n64.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <stdlib.h>
#include "eff/n64/eff_snow_dust_n64.h"

typedef struct EffSnowmanN64Work {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 lifetime;  // 0x10
    s32 frame;     // 0x14
    s32 r;         // 0x18
    s32 g;         // 0x1C
    s32 b;         // 0x20
    s32 alpha;     // 0x24
    f32 scale;     // 0x28
    f32 rotY;      // 0x2C
    Vec offset;    // 0x30
    Vec speed;     // 0x3C
    f32 hscale;    // 0x48
    f32 wscale;    // 0x4C
    f32 gravity;   // 0x50
    s32 counter;   // 0x54
    s32 state;     // 0x58
    s32 phase;     // 0x5C
} EffSnowmanN64Work;

EffectEntry* effVaporN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 delay);

static s8 jump_hscale_data[] = {100, 90, 82, 76, 72, 70, 74, 89, 103, 116, 118, 120, 120, 120, 120, 118, 114, 110};
static u8 land_hscale_data[] = "dZRLHFJYdlnlf`_`ceddddddddd";

static u8 jump2_hscale_data[] = {100, 90, 82, 76, 72, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 74, 89, 103, 116, 118, 120, 120, 120, 120, 118, 114, 110};

static s8 jump2_ry_data[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 10, 16, 22, 28, 33, 38, 40, 41, 41, 40, 38, 33, 28, 22, 16, 10, 5, 1, 1, 5, 10, 16, 22, 28, 33, 38, 40, 41, 41, 40, 38, 33, 28, 22, 16, 10, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void effSnowmanMain(EffectEntry* effect);
void effSnowmanDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effSnowmanN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffSnowmanN64Work* work;

    effect = effEntry();
    effect->type = "SnowmanN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffSnowmanN64Work));
    effect->userdata = work;
    effect->callback = effSnowmanMain;
    effect->flags |= 2;
    work->type = type;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->alpha = 0;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->offset.x = 0.0f;
    work->offset.y = 200.0f;
    work->offset.z = 0.0f;
    work->speed.x = 0.0f;
    work->speed.y = 0.0f;
    work->speed.z = 0.0f;
    work->counter = 0;
    work->state = 2;
    work->scale = scale;
    work->phase = 0;
    work->r = 200;
    work->g = 220;
    work->b = 240;
    work->speed.y = -8.0f;
    work->rotY = 0.0f;
    work->gravity = -1.0f;
    return effect;
}

void effSnowmanMain(EffectEntry* effect) {
    EffSnowmanN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 y;
    f32 spd;
    f32 zero;
    f32 sx;
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    zero = 0.0f;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->lifetime = 16;
    }
    if (work->lifetime < 1000) {
        work->lifetime--;
    }
    work->frame++;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    frame = work->frame;
    if (work->lifetime < 16) {
        work->alpha = work->lifetime * 16;
    }
    if (frame < 16) {
        work->alpha = frame * 16 + 15;
    }
    sx = work->speed.x;
    if (sx != zero) {
        spd = work->speed.y;
        if (spd < zero) {
            spd = -spd;
        }
        if (sx < zero) {
            work->rotY -= spd;
        } else {
            work->rotY += spd;
        }
    }
    switch (work->state) {
        case 0:
            work->wscale = 0.01f * land_hscale_data[work->counter++];
            work->hscale = 2.0f - work->wscale;
            if (work->counter >= 27) {
                work->state = 2;
            }
            break;
        case 1:
            work->wscale = 0.01f * jump_hscale_data[work->counter++];
            work->hscale = 2.0f - work->wscale;
            if (work->counter >= 18) {
                work->state = 2;
            }
            if (work->counter >= 8) {
                work->offset.x += work->speed.x;
                work->offset.y += work->speed.y;
                work->offset.z += work->speed.z;
                work->speed.y += -1.0f;
            }
            break;
        case 3:
            work->wscale = 0.01f * jump2_hscale_data[work->counter++];
            work->hscale = 2.0f - work->wscale;
            work->rotY = jump2_ry_data[work->counter++];
            if (work->counter >= 74) {
                work->state = 2;
            }
            if (work->counter >= 64) {
                work->offset.x += work->speed.x;
                work->offset.y += work->speed.y;
                work->offset.z += work->speed.z;
                work->speed.y += -1.0f;
            }
            break;
        case 2:
            work->wscale = 0.01f * jump_hscale_data[17];
            work->hscale = 2.0f - work->wscale;
            work->offset.x += work->speed.x;
            work->offset.y += work->speed.y;
            work->offset.z += work->speed.z;
            work->speed.y += work->gravity;
            if (work->offset.y < zero) {
                switch (work->phase) {
                    case 0:
                        work->state = 0;
                        break;
                    case 1:
                        work->speed.x = 2.0f;
                        work->speed.z = 2.0f;
                        work->speed.y = 10.0f;
                        work->state = 1;
                        break;
                    case 2:
                        work->speed.x = 2.0f;
                        work->speed.z = -2.0f;
                        work->speed.y = 10.0f;
                        work->state = 1;
                        break;
                    case 3:
                        work->speed.x = -2.0f;
                        work->speed.z = -2.0f;
                        work->speed.y = 10.0f;
                        work->state = 1;
                        break;
                    case 4:
                        work->speed.x = 10.0f;
                        work->speed.z = zero;
                        work->state = 1;
                        work->speed.y = 14.0f;
                        work->gravity = -1.0f;
                        break;
                    default:
                        work->speed.x = zero;
                        work->speed.y = zero;
                        work->speed.z = zero;
                        work->offset.y = zero;
                        work->state = 2;
                        break;
                }
                work->offset.y = zero;
                work->counter = 0;
                work->phase++;
            }
            break;
    }
    if ((frame & 7) == 1) {
        effVaporN64Entry(2, work->position.x + work->offset.x + (f32)(rand() % 60) - 30.0f,
                         50.0f + (work->position.y + work->offset.y + (f32)(rand() % 100)),
                         work->position.z + work->offset.z, 4.0f, 40);
        effSnowDustN64Entry(1, 10, 30, work->position.x + work->offset.x, 10.0f + (work->position.y + work->offset.y),
                            60.0f + (work->position.z + work->offset.z), 60.0f, 100.0f, 2.0f);
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effSnowmanDisp, effect);
}

void effSnowmanDisp(CameraId cameraId, EffectEntry* effect) {
}
