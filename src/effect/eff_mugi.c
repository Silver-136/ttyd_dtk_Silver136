#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "driver/mapdrv.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"


#include "driver/animdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "mario/mario.h"


typedef struct effMugiKemuriData {
    s32 flag; //0x0
    s32 type; //0x4
    Vec pos; //0x8
    Vec rot; //0x14
    Vec scale; //0x20
    f32 speed; //0x2C
    f32 hSpeedScale; //0x30, horizontal step per frame = direction * vertical speed * this
    f32 rising; //0x34, 1.0 until the vertical speed turns negative
    f32 field_0x38; //0x38
    f32 vy; //0x3C
    f32 ay; //0x40
    f32 jy; //0x44
    f32 ky; //0x48
    f32 dirX; //0x4C
    f32 dirZ; //0x50
    s32 timer; //0x54
    s32 alpha; //0x58
    f32 field_0x5C; //0x5C
    s32 poseId; //0x60
} effMugiKemuriData;

void effMugiKemuriMain(EffectEntry* effect);
void effMugiKemuriDisp(CameraId camId, void* param);

inline f32 inline_fn(effMugiKemuriData *data) {
    return 6.2831855f * reviseAngle(data->rot.y);
}

EffectEntry* effMugiKemuriEntry(f32 x, f32 y, f32 z, f32 angle) {
    EffectEntry* effect;
    effMugiKemuriData* data;
    s32 i;
    f32 rad;

    effect = effEntry();
    effect->type = "MugiKemuri";
    effect->count = 5;
    data = __memAlloc(HEAP_EFFECT, sizeof(effMugiKemuriData) * 5);
    effect->userdata = data;
    memset(data, 0, sizeof(effMugiKemuriData) * 5);
    effect->callback = effMugiKemuriMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    for (i = 0; i < 5; i++, data++) {
        data->flag = 1;
        data->hSpeedScale = 0.29999998f;
        data->rising = 0.0f;
        data->field_0x38 = 0.0f;
        data->pos.x = x;
        data->pos.y = y;
        data->pos.z = z;
        data->scale.x = 0.7f;
        data->scale.y = 0.7f;
        data->scale.z = 0.7f;
        data->alpha = 0xFF;
        data->timer = 60;
        data->rot.x = 0.0f;
        data->rot.y = angle + i * 72;
        data->rot.z = 0.0f;
        data->vy = 1.75f;
        data->ay = -0.08f;
        data->jy = 0.0f;
        data->ky = 0.0f;
        data->speed = -3.9f;
        rad = inline_fn(data) / 360.0f;
        data->dirX = (f32)sin(rad);
        data->dirZ = (f32)cos(rad);
        data->poseId = -1;
    }
    return effect;
}

void effMugiKemuri2Main(EffectEntry* effect);
void effMugiKemuri2Disp(CameraId camId, void* param);

EffectEntry* effMugiKemuri2Entry(s32 type, f32 x, f32 y, f32 z, f32 angle, f32 dir) {
    EffectEntry* effect;
    effMugiKemuriData* data;
    s32 i;
    Vec unused = {0.0f, 0.0f, 0.0f};

    __fabs(x * 0.005);
    effect = effEntry();

    effect->type = "MugiKemuri2";
    effect->count = 5;
    data = __memAlloc(HEAP_EFFECT, sizeof(effMugiKemuriData) * 5);
    effect->userdata = data;
    memset(data, 0, sizeof(effMugiKemuriData) * 5);
    effect->callback = effMugiKemuri2Main;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    for (i = 0; i < 5; i++, data++) {
        data->flag = 1;
        data->type = type;
        data->field_0x38 = 0.0f;
        data->pos.x = x;
        data->pos.y = y;
        data->pos.z = z;
        data->scale.x = 0.7f;
        data->scale.y = 0.7f;
        data->scale.z = 0.7f;
        data->alpha = 0xFF;
        data->rising = 1.0f;
        data->hSpeedScale = 5.4f;
        data->speed = dir ? -10 : 10;
        data->vy = 0.5f;
        data->ay = -0.07f;
        data->jy = 0.003f;
        data->ky = 0.0f;
        data->timer = 15;
        data->rot.y = angle;
        data->rot.x = 0.0f;
        data->rot.z = 0.0f;
        data->field_0x5C = ((rand() % 100) >> 4) & 1;
        {
            f32 rad = (6.2831855f * reviseAngle(angle + (dir ? -90 : 90))) / 360.0f;
            data->dirX = (f32)sin(rad);
            data->dirZ = (f32)cos(rad);
        }
        data->poseId = -1;
    }
    return effect;
}

void effMugiKemuriMain(EffectEntry* effect) {
    s32 i;
    effMugiKemuriData* data;
    effMugiKemuriData* p;
    s32 alive;
    s32 group;
    const char* name;
    Vec pos2;
    Vec pos = {0.0f, 0.0f, 0.0f};

    data = effect->userdata;
    pos.x = data->pos.x;
    pos.y = data->pos.y;
    pos.z = data->pos.z;
    pos2 = pos;
    group = gp->inBattle != 0;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        p = effect->userdata;
        for (i = 0; i < effect->count; i++, p++) {
            if (p->poseId != -1) {
                animPoseRelease(p->poseId);
            }
        }
        effDelete(effect);
        return;
    }
    p = data;
    for (i = 0; i < effect->count; i++, p++) {
        if (p->poseId == -1) {
            name = strncmp(gp->currentMapName, "win", 3) == 0 ? "EFF_Hana" : "EFF_Mugi";
            if (!animGroupBaseAsync(name, group, NULL)) {
                return;
            }
            p->poseId = animPoseEntry(name, group);
            animPoseSetAnim(p->poseId, "Z_1", 1);
        }
    }
    data = effect->userdata;
    alive = 0;
    for (i = 0; i < effect->count; i++, data++) {
        if (data->flag) {
            if (--data->timer <= 0) {
                data->flag = 0;
            } else {
                alive = 1;
                data->speed *= 0.85f;
                data->pos.x += data->speed * data->dirX;
                data->pos.z += data->speed * data->dirZ;
                data->jy += data->ky;
                data->ay += data->jy;
                data->vy += data->ay;
                data->pos.y += data->vy;
                if (data->vy < 0.0f) {
                    data->jy = 0.004f;
                }
                data->rot.x -= 1.0f;
                if (data->rot.x < -100.0f) {
                    data->rot.x = -100.0f;
                }
            }
        }
    }
    if (!alive) {
        p = effect->userdata;
        for (i = 0; i < effect->count; i++, p++) {
            if (p->poseId != -1) {
                animPoseRelease(p->poseId);
            }
        }
        effDelete(effect);
        return;
    }

    dispEntry(CAMERA_3D, dispCalcZ(&pos2), 2, effMugiKemuriDisp, effect);
}


void effMugiKemuri2Main(EffectEntry* effect) {
    s32 i;
    effMugiKemuriData* data;
    effMugiKemuriData* p;
    s32 alive;
    s32 group;
    s32 alive2;
    const char* name;
    MarioWork* mario;
    Vec pos2;
    Vec pos = {0.0f, 0.0f, 0.0f};

    data = effect->userdata;
    pos.x = data->pos.x;
    pos.y = data->pos.y;
    pos.z = data->pos.z;
    pos2 = pos;
    group = gp->inBattle != 0;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        p = effect->userdata;
        for (i = 0; i < effect->count; i++, p++) {
            if (p->poseId != -1) {
                animPoseRelease(p->poseId);
            }
        }
        effDelete(effect);
        return;
    }
    p = data;
    for (i = 0; i < effect->count; i++, p++) {
        if (p->poseId == -1) {
            name = strncmp(gp->currentMapName, "win", 3) == 0 ? "EFF_Hana" : "EFF_Mugi";
            if (!animGroupBaseAsync(name, group, NULL)) {
                return;
            }
            p->poseId = animPoseEntry(name, group);
            animPoseSetAnim(p->poseId, "Z_1", 1);
        }
    }
    data = effect->userdata;
    alive = 0;
    for (i = 0; i < effect->count; i++, data++) {
        if (data->flag) {
            if (--data->timer <= 0) {
                data->flag = 0;
            } else {
                alive = 1;
                mario = marioGetPtr();
                if (data->type == 0 && data->rising && mario->currMotionId == MARIO_MOTION_STAY) {
                    data->timer++;
                    continue;
                }
                data->jy += data->ky;
                data->ay += data->jy;
                data->vy += data->ay;
                data->rot.z += data->vy * data->speed;
                data->pos.x -= data->dirX * (data->vy * data->hSpeedScale);
                data->pos.z -= data->dirZ * (data->vy * data->hSpeedScale);
                data->alpha -= 12;
                if (data->vy < 0.0f) {
                    data->rising = 0.0f;
                }
            }
        }
    }
    if (!alive) {
        alive2 = 0;
        data = effect->userdata;
        for (i = 0; i < effect->count; i++, data++) {
            data->alpha -= 10;
            if (data->alpha < 0) {
                data->alpha = 0;
            } else {
                alive2 = 1;
            }
        }
        if (!alive2) {
            data = effect->userdata;
            for (i = 0; i < effect->count; i++, data++) {
                if (data->poseId != -1) {
                    animPoseRelease(data->poseId);
                }
            }
            effDelete(effect);
            return;
        }
    }

    dispEntry(CAMERA_3D, dispCalcZ(&pos2), 2, effMugiKemuri2Disp, effect);
}

void effMugiKemuriDisp(CameraId camId, void* param) {
    EffectEntry* effect = param;
    effMugiKemuriData* data;
    s32 i;
    Mtx scale, trans, rx, ry, rz;

    data = effect->userdata;
    for (i = 0; i < effect->count; i++, data++) {
        if (data->flag || data->alpha != 0) {
            PSMTXRotRad(rx, 'x', 0.017453292f * data->rot.x);
            PSMTXRotRad(ry, 'y', 0.017453292f * data->rot.y);
            PSMTXRotRad(rz, 'z', 0.017453292f * data->rot.z);
            PSMTXTrans(trans, data->pos.x, data->pos.y, data->pos.z);
            PSMTXScale(scale, data->scale.x, data->scale.y, data->scale.z);
            PSMTXConcat(rx, rz, rz);
            PSMTXConcat(ry, rz, rz);
            PSMTXConcat(rz, scale, scale);
            PSMTXConcat(trans, scale, trans);
            if (data->poseId != -1) {
                animPoseSetMaterialFlagOn(data->poseId, 0x40);
                animPoseSetMaterialEvtColor(data->poseId, (GXColor){0xFF, 0xFF, 0xFF, (u8)data->alpha});
                animPoseMain(data->poseId);
                animPoseDrawMtx(data->poseId, trans, 1, 0.0f, 15.0f);
                animPoseDrawMtx(data->poseId, trans, 2, 0.0f, 15.0f);
                animPoseDrawMtx(data->poseId, trans, 3, 0.0f, 15.0f);
            }
        }
    }
}

void effMugiKemuri2Disp(CameraId camId, void* param) {
    EffectEntry* effect = param;
    effMugiKemuriData* data;
    s32 i;
    Mtx scale, trans, rx, ry, rz;

    data = effect->userdata;
    for (i = 0; i < effect->count; i++, data++) {
        if (data->flag || data->alpha != 0) {
            PSMTXRotRad(rx, 'x', 0.017453292f * data->rot.x);
            PSMTXRotRad(ry, 'y', 0.017453292f * data->rot.y);
            PSMTXRotRad(rz, 'z', 0.017453292f * data->rot.z);
            PSMTXTrans(trans, data->pos.x, data->pos.y, data->pos.z);
            PSMTXScale(scale, data->scale.x, data->scale.y, data->scale.z);
            PSMTXConcat(rx, rz, rz);
            PSMTXConcat(ry, rz, rz);
            PSMTXConcat(rz, scale, scale);
            PSMTXConcat(trans, scale, trans);
            if (data->poseId != -1) {
                animPoseSetMaterialFlagOn(data->poseId, 0x40);
                animPoseSetMaterialEvtColor(data->poseId, (GXColor){0xFF, 0xFF, 0xFF, (u8)data->alpha});
                animPoseMain(data->poseId);
                animPoseDrawMtx(data->poseId, trans, 1, 0.0f, 15.0f);
                animPoseDrawMtx(data->poseId, trans, 2, 0.0f, 15.0f);
                animPoseDrawMtx(data->poseId, trans, 3, 0.0f, 15.0f);
            }
        }
    }
}


