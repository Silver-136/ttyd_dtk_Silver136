#include "eff/eff_scanning.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>

void effScanningMain(EffectEntry* effect);
void effScanningDisp(CameraId camId, void* param);

EffectEntry* effScanningEntry(s32 int2, f32 float3, f32 float4, f32 float5) {
    EffectEntry* entry;
    effScanningData* data;
    
    
    entry = effEntry();
    entry->type = "Scanning";
    entry->count = 1;
    data = __memAlloc(HEAP_EFFECT, sizeof(effScanningData) * entry->count);
    entry->userdata = data;
    entry->callback = effScanningMain;
    entry->flags |= 2;

    data->field_0x0 = int2;
    data->position.x = float3;
    data->position.y = float4;
    data->position.z = float5;
    data->field_0x10 = 1.0f;
    data->animId = -1;
    data->field_0x18 = 0;
    data->field_0x1C = 0;
    data->field_0x20 = 255;

    return entry;
}

void effScanningMain(EffectEntry* effect) {
    effScanningData* data = effect->userdata;
    Vec position;
    BOOL inBattle;

    position = (Vec){ data->position.x, data->position.y, data->position.z };
    inBattle = gp->inBattle != 0;

    if (effect->flags & 4) { //soft delete
        effect->flags &= ~4;
        if (data->animId != -1) {
            animPoseRelease(data->animId);
        }
        effDelete(effect);
        return;
    }
    switch (data->field_0x18) {
        case 0:
            if (!animGroupBaseAsync("EFF_Scan", inBattle, NULL)) {
                return;
            }
            data->animId = animPoseEntry("EFF_Scan", (u32)inBattle);
            animPoseSetAnim(data->animId, "Z_1", TRUE);
            data->field_0x18++;
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
        PSMTXScale(scale, data->field_0x10, data->field_0x10, data->field_0x10);
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(camId)->viewYaw));
        PSMTXConcat(mtx, rot, mtx);
        PSMTXConcat(mtx, scale, mtx);
        animPoseMain(data->animId);
        animPoseDrawMtx(data->animId, mtx, 1, 0.0f, 10.0f);
        animPoseDrawMtx(data->animId, mtx, 2, 0.0f, 10.0f);
        animPoseDrawMtx(data->animId, mtx, 3, 0.0f, 10.0f);
    }
}
