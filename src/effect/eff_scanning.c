#include "eff/eff_scanning.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>
#include "driver/effdrv.h"

void effScanningMain(EffectEntry* effect);
void effScanningDisp(CameraId camId, void* param);

EffectEntry* effScanningEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* entry;
    effScanningData* data;

    entry = effEntry();
    entry->type = "Scanning";
    entry->count = 1;
    data = __memAlloc(HEAP_EFFECT, sizeof(effScanningData) * entry->count);
    entry->userdata = data;
    entry->callback = effScanningMain;
    entry->flags |= EFFECT_FLAG_SOFT_DELETABLE;

    data->type = type;
    data->position.x = x;
    data->position.y = y;
    data->position.z = z;
    data->scale = 1.0f;
    data->animId = -1;
    data->state = 0;
    data->field_0x1C = 0;
    data->alpha = 255;

    return entry;
}

void effScanningMain(EffectEntry* effect) {
    effScanningData* data = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ data->position.x, data->position.y, data->position.z };
    inBattle = gp->inBattle != 0;

    // soft delete
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        if (data->animId != -1) {
            animPoseRelease(data->animId);
        }
        effDelete(effect);
        return;
    }
    switch (data->state) {
        case 0:
            if (!animGroupBaseAsync("EFF_Scan", inBattle, NULL)) {
                return;
            }
            // FAKE: no-op (u32) cast (animPoseEntry takes s32); removing it changes the object
            data->animId = animPoseEntry("EFF_Scan", (u32)inBattle);
            animPoseSetAnim(data->animId, "Z_1", TRUE);
            data->state++;
            break;
        case 1:
            if (animPoseGetLoopTimes(data->animId) > 1.0f) {
                animPoseRelease(data->animId);
                effDelete(effect);
                return;
            }
            break;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effScanningDisp, effect);
}

void effScanningDisp(CameraId camId, void* param) {
    effScanningData* data = ((EffectEntry*)param)->userdata;
    Mtx mtx;
    Mtx scale;
    Mtx rot;

    if (data->animId != -1) {
        PSMTXTrans(mtx, data->position.x, data->position.y, data->position.z);
        PSMTXScale(scale, data->scale, data->scale, data->scale);
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(camId)->viewYaw));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        animPoseMain(data->animId);
        animPoseDrawMtx(data->animId, mtx, 1, 0.0f, 10.0f);
        animPoseDrawMtx(data->animId, mtx, 2, 0.0f, 10.0f);
        animPoseDrawMtx(data->animId, mtx, 3, 0.0f, 10.0f);
    }
}
