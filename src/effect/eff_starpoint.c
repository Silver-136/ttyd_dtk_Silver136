#include "eff/eff_starpoint.h"
#include "driver/dispdrv.h"
#include "driver/icondrv.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "icon_ids.h"
#include "poly_trig.h"

#define PI 3.1415927f
#define HALF_PI 1.5707964f

typedef struct EffStarPointWork {
    s32 type;                // 0x0
    s32 timer;               // 0x4
    s32 frame;               // 0x8
    Vec position;            // 0xC
    f32 velX;                // 0x18
    f32 velZ;                // 0x1C
    f32 velY;                // 0x20
    f32 gravity;             // 0x24
    u8 field_0x28[4];        // 0x28
    s32 alpha;               // 0x2C
    s32 delay;               // 0x30
} EffStarPointWork;

// cheap polynomial approximations, valid for x in [0, 2pi]

static inline f32 _cosf(f32 x) {
    f32 a = 0.03705f;
    f32 b = 0.4967f;
    f32 c = 1.0f;
    f32 x2;

    if (x <= PI) {
        { s32 gz = 0; if (gz != 0) { a = (a / x) / (x / a); } } // FAKE: dead guard, no code (COLOURING W); its float temps keep the hoisted 2.0 deferred (f31)
        if (x <= HALF_PI) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
        }
        else {
            x = HALF_PI - (x - HALF_PI);
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = -a;
        }
    }
    else if (x < 4.712389f) {
        x = x - PI;
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
        a = -a;
    }
    else {
        x = HALF_PI - (x - 4.712389f);
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
    }
    return a;
}

#pragma opt_propagation off
#pragma opt_lifetimes off
EffectEntry* effStarPointEntry(s32 type, s32 count, f32 x, f32 y, f32 z) {
    s32 delay;
    EffectEntry* effect;
    EffStarPointWork* work;
    s32 i;
    f32 r;
    f32 angle;

    effect = effEntry();
    effect->type = "StarPoint";
    effect->count = count + 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffStarPointWork) * effect->count);
    effect->userdata = work;
    effect->callback = effStarPointMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->timer = 60;
    work->frame = 0;

    for (i = 1, delay = 4, work++; i < effect->count; i++, delay += 3, work++) {
        r = (f32)(rand() % 360);
        work->position.x = 0.0f;
        work->position.y = 0.0f;
        work->position.z = 0.0f;
        angle = r * 6.2831855f / 360.0f;
        work->velX = 2.0f * polySinf(angle);
        work->velZ = 2.0f * _cosf(angle);
        work->velY = 14.0f;
        work->gravity = 2.0f;
        work->alpha = 0;
        work->delay = delay;
    }
    return effect;
}
#pragma opt_propagation reset
#pragma opt_lifetimes reset

void effStarPointMain(EffectEntry* effect) {
    EffStarPointWork* work = effect->userdata;
    Vec position;
    s32 i;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    work->timer--;
    work->frame++;
    if (work->timer < 0) {
        effDelete(effect);
        return;
    }
    for (i = 1, work++; i < effect->count; i++, work++) {
        if (work->delay) {
            work->delay--;
            work->alpha = 0;
        }
        else {
            work->velY -= work->gravity;
            work->position.x += work->velX;
            work->position.y += work->velY;
            work->position.z += work->velZ;
            if (work->velY < 0.0f) {
                work->alpha -= 2;
                if (work->alpha < 0) {
                    work->alpha = 0;
                }
            }
            else {
                work->alpha = 255;
            }
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 1, (DispCallback)effStarPointDisp, effect);
}

void effStarPointDisp(CameraId cameraId, EffectEntry* effect) {
    EffStarPointWork* work = effect->userdata;
    Mtx mtx;
    Mtx rot;
    Mtx trans;
    s32 i;

    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(CAMERA_3D)->viewYaw));
    PSMTXConcat(mtx, rot, mtx);
    for (i = 1, work++; i < effect->count; i++, work++) {
        if (work->alpha) {
            PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
            PSMTXConcat(mtx, trans, trans);
            iconDispGx2(trans, 0, ICON_18C);
        }
    }
}

