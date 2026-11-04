// Paper Mario 64 effect: a snowman that drops in, then hops along a preset path kicking up snow dust and vapor.

#include "version.h"
#include "eff/eff_n64.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <stdlib.h>
#include "eff/n64/eff_snow_dust_n64.h"
#include "eff/n64/eff_vapor_n64.h"

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

/**
 * Width of the snowman in percent over a hop: it crouches, stretches as it leaves the ground,
 * then settles. The height is set to the opposite, so it keeps its volume.
 */
static s8 jump_hscale_data[] = {100, 90, 82, 76, 72, 70, 74, 89, 103, 116, 118, 120, 120, 120, 120, 118, 114, 110};
/** Width in percent over the 27-frame landing squash, written as characters ('d' is 100). */
static u8 land_hscale_data[] = "dZRLHFJYdlnlf`_`ceddddddddd";

/** Width in percent over the long spinning hop: a long crouch, then the stretch as it takes off. */
static u8 jump2_hscale_data[] = {100, 90, 82, 76, 72, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 74, 89, 103, 116, 118, 120, 120, 120, 120, 118, 114, 110};

/** Turn in degrees over the long spinning hop: two sways back and forth before it leaves the ground. */
static s8 jump2_ry_data[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 5, 10, 16, 22, 28, 33, 38, 40, 41, 41, 40, 38, 33, 28, 22, 16, 10, 5, 1, 1, 5, 10, 16, 22, 28, 33, 38, 40, 41, 41, 40, 38, 33, 28, 22, 16, 10, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#if VERSION <= VERSION_DEMO_JP
typedef struct SnowmanJumpStep {
    s16 x;
    s16 y;
    s8 dy;
    u8 wscale;
    u8 hscale;
} SnowmanJumpStep;
static SnowmanJumpStep adata[] = {
    {-200, 200, 30, 100, 100}, {-180, 180, 30, 100, 100}, {-160, 160, 30, 100, 100}, {-140, 140, 30, 100, 100},
    {-120, 120, 30, 100, 100}, {-100, 100, 30, 100, 100}, {-80, 80, 30, 100, 100}, {-60, 60, 30, 100, 100},
    {-40, 40, 30, 100, 100}, {-20, 20, 30, 100, 100}, {0, 0, 30, 100, 100}, {0, 0, 30, 120, 80},
    {0, 0, 10, 130, 70}, {0, 0, 5, 135, 65}, {0, 0, 2, 138, 62}, {0, 0, 0, 140, 60},
    {0, 0, 0, 140, 60}, {0, 0, -1, 140, 60}, {0, 0, -1, 140, 60}, {0, 0, -2, 138, 62},
    {0, 0, -2, 135, 65}, {0, 0, -3, 130, 70}, {0, 0, -4, 120, 80}, {0, 0, -6, 100, 100},
    {0, 0, -6, 100, 100}, {0, 0, -8, 100, 100}, {0, 1, -12, 100, 100}, {0, 3, -18, 100, 100},
    {0, 5, -26, 100, 100}, {0, 10, -36, 100, 100}, {0, 15, -48, 100, 100}, {0, 20, -62, 100, 100},
    {0, 28, -78, 100, 100}, {5, 30, -96, 100, 100}, {8, 30, -96, 100, 100}, {10, 30, -96, 100, 100},
};
#endif

void effSnowmanMain(EffectEntry* effect);
void effSnowmanDisp(CameraId cameraId, void* param);

/**
 * Spawns a snowman at (x, y, z); a lifetime of 0 or less lasts until soft-deleted.
 * Spawned by event scripts through evt_eff.
 */
EffectEntry* effSnowmanN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffSnowmanN64Work* work;

    effect = effEntry();
    effect->type = "SnowmanN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffSnowmanN64Work));
    effect->userdata = work;
    effect->callback = effSnowmanMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
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

/**
 * Fades in over 16 frames (and out over 16 after a delete request; lifetime 1000 means endless) and turns
 * while moving sideways.
 * States: 0 a 27-frame landing squash, 1 a hop (moves from its 8th frame), 3 a spinning hop,
 * 2 airborne under gravity; each landing starts the next phase of a fixed route:
 * squash, three diagonal hops, one long hop along +X, then stop.
 * Every 8 frames it puffs a vapor cloud (effVaporN64Entry) and snow dust (effSnowDustN64Entry type 1).
 */
void effSnowmanMain(EffectEntry* effect) {
    EffSnowmanN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    f32 spd;
    f32 zero;
    f32 sx;
    s32 frame;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    zero = 0.0f;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
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
        effVaporN64Entry(2, 40, work->position.x + work->offset.x + (f32)(rand() % 60) - 30.0f, 50.0f + (work->position.y + work->offset.y + (f32)(rand() % 100)), work->position.z + work->offset.z, 4.0f);
        effSnowDustN64Entry(1, 10, 30, work->position.x + work->offset.x, 10.0f + (work->position.y + work->offset.y),
                            60.0f + (work->position.z + work->offset.z), 60.0f, 100.0f, 2.0f);
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), DISP_MODE_XLU, effSnowmanDisp, effect);
}

/** Empty: the snowman body is never drawn, so only the vapor and snow-dust puffs that effSnowmanMain spawns show. */
void effSnowmanDisp(CameraId cameraId, void* param) {
}
