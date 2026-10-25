#include "eff/eff_hibashira.h"
#include "driver/animdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"

typedef struct EffHibashiraWork {
    s32 type;     // 0x0
    Vec position; // 0x4, element 0: origin, others: offset from origin
    f32 scale;    // 0x10
    s32 poseId;   // 0x14
    s32 alpha;    // 0x18
    s32 timer;    // 0x1C, element 0 only, counts down while fading out
    s32 frame;    // 0x20
    s32 delay;    // 0x24
} EffHibashiraWork;

EffectEntry* effHibashiraEntry(s32 type, s32 count, s32 duration, f32 x, f32 y, f32 z, f32 endX,
                               f32 endY, f32 endZ, f32 scale) {
    EffectEntry* effect;
    EffHibashiraWork* work;
    EffHibashiraWork* pillar;
    s32 num, i;
    f32 dx, dy, dz;

    effect = effEntry();
    num = 1;
    switch (type) {
        case 0:
        case 1:
        case 2:
            num = 2;
            break;
        case 3:
        case 4:
        case 5:
            num = count + 1;
            break;
    }
    effect->type = "hibashira";
    effect->count = num;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffHibashiraWork) * effect->count);
    effect->userdata = work;
    effect->callback = effHibashiraMain;
    effect->flags |= 2;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->poseId = -1;
    work->alpha = 255;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->frame = 0;

    // spread the pillars evenly from start to end
    dx = endX - x;
    dy = endY - y;
    dz = endZ - z;
    pillar = work + 1;
    for (i = 1; i < effect->count; i++, pillar++) {
        pillar->position.x = (i - 1) * (dx / (num - 1));
        pillar->position.y = (i - 1) * (dy / (num - 1));
        pillar->position.z = (i - 1) * (dz / (num - 1));
        {
            f32 s = sin(3.1415927f * i / (effect->count - 1));
            pillar->scale = s + 0.5f;
        }
        pillar->poseId = -1;
        pillar->alpha = 255;
        pillar->timer = 0;
        pillar->frame = 0;
        pillar->delay = (i - 1) * 4;
    }
    return effect;
}

void effHibashiraMain(EffectEntry* effect) {
    s32 k;
    EffHibashiraWork* work = effect->userdata;
    EffHibashiraWork* pillar;
    Vec position;
    s32 type;
    s32 i;
    s32 inBattle;

    position = (Vec){ work->position.x, work->position.y, work->position.z };
    inBattle = gp->inBattle ? TRUE : FALSE;
    type = work->type;

    // soft delete: start fading out
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->timer = 16;
    }
    switch (type) {
        case 0:
        case 1:
        case 3:
        case 4:
            if (!animGroupBaseAsync("EFF_hibashira", inBattle, NULL)) {
                return;
            }
            break;
        case 2:
        case 5:
            if (!animGroupBaseAsync("EFF_hibashira2", inBattle, NULL)) {
                return;
            }
            break;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    if (work->timer < 16) {
        work->alpha = work->timer * 16;
    }
    if (work->timer < 0) {
        for (i = 0, pillar = work; i < effect->count; pillar++, i++) {
            if (pillar->poseId != -1) {
                animPoseRelease(pillar->poseId);
            }
        }
        effDelete(effect);
        return;
    }

    for (work++, k = 1; k < effect->count; k++, work++) {
        if (work->delay) {
            work->delay--;
            continue;
        }
        if (work->poseId == -1) {
            switch (type) {
                case 0:
                case 3:
                    work->poseId = animPoseEntry("EFF_hibashira", inBattle);
                    animPoseSetAnim(work->poseId, "A_1", TRUE);
                    break;
                case 1:
                case 4:
                    work->poseId = animPoseEntry("EFF_hibashira", inBattle);
                    animPoseSetAnim(work->poseId, "A_2", TRUE);
                    break;
                case 2:
                case 5:
                    work->poseId = animPoseEntry("EFF_hibashira2", inBattle);
                    animPoseSetAnim(work->poseId, "A_1", TRUE);
                    break;
            }
        }
        work->frame++;
        if (work->frame > 1000) {
            work->frame = 1000;
        }
        if (work->frame > 5) {
            work->alpha -= 10;
        }
        if (work->alpha < 0) {
            work->alpha = 0;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effHibashiraDisp, effect);
}

void effHibashiraDisp(CameraId cameraId, EffectEntry* effect) {
    EffHibashiraWork* work = effect->userdata;
    s32 i, alpha;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx base;

    alpha = work->alpha;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, base);
    for (work++, i = 1; i < effect->count; i++, work++) {
        if (work->poseId != -1) {
            PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
            PSMTXScale(scale, work->scale, work->scale, work->scale);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(base, mtx, mtx);
            animPoseSetMaterialFlagOn(work->poseId, 0x40);
            animPoseSetMaterialEvtColor(work->poseId,
                                        (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha * alpha / 255 });
            animPoseMain(work->poseId);
            animPoseDrawMtx(work->poseId, mtx, 1, 0.0f, 10.0f);
            animPoseDrawMtx(work->poseId, mtx, 2, 0.0f, 10.0f);
            animPoseDrawMtx(work->poseId, mtx, 3, 0.0f, 10.0f);
        }
    }
}
