#include "data/item_data.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/icondrv.h"
#include "eff/eff_kemu_test.h"
#include "manager/evtmgr.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "icon_ids.h"
#include "evt/evt_snd.h"
#include "evt/evt_cmd.h"
#include "eff/eff_itemget.h"
#include "libc_decls.h"
#include "eff/eff_kemutest.h"

extern ItemData itemDataTable[];

typedef struct EffNanigaWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    Vec screen;    // 0x10
    f32 scale;     // 0x1C
    s32 state;     // 0x20
    s32 timer;     // 0x24
    s32* list;     // 0x28
    s32 count;     // 0x2C
    s32 item;      // 0x30
    f32 offset;    // 0x34
    f32 minOffset; // 0x38
    f32 maxOffset; // 0x3C
    s32 pose;      // 0x40
    s32 alpha;     // 0x44
    s32 mode;      // 0x48
    s32 sfxId;     // 0x4C
} EffNanigaWork;

void effNanigaMain(EffectEntry* effect);
void effNanigaDisp(CameraId cameraId, EffectEntry* effect);
void effNanigaDisp2(CameraId cameraId, EffectEntry* effect);
void effNanigaDisp3(CameraId cameraId, EffectEntry* effect);

static EVT_BEGIN(sound_evt)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_WHAT3"), 4264967296, 4264967297, 4264967298, 0)
    WAIT_MS(833)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_WHAT4"), 4264967296, 4264967297, 4264967298, 0)
    WAIT_MS(833)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ITEM_WHAT5"), 4264967296, 4264967297, 4264967298, 0)
    RETURN
EVT_END()

EffectEntry* effNanigaEntry(s32 type, f32 x, f32 y, f32 z, s32* list, s32 item) {
    EffectEntry* effect;
    EffNanigaWork* work;
    BOOL inBattle;
    s32 i;
    Vec pos;

    effect = effEntry();
    inBattle = gp->inBattle != 0;
    effect->type = "Naniga";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffNanigaWork));
    effect->userdata = work;
    effect->callback = effNanigaMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = 20.0f + y;
    work->position.z = z;
    work->scale = 1.0f;
    work->state = 0;
    work->timer = 0;
    work->list = list;
    work->count = 0;
    while (list[work->count] != 0) {
        work->count++;
    }
    work->item = item;
    work->minOffset = 0.0f;
    work->offset = 0.0f;
    for (i = 0; i < work->count; i++) {
        if (item == list[i]) {
            break;
        }
    }
    work->maxOffset = 40.0f * (f32)i + 3.0f * (40.0f * (f32)work->count);
    i = rand() % 3;
    if (i == 0) {
        work->mode = 1;
        work->maxOffset += 40.0f;
    } else if (i == 1) {
        work->mode = 2;
        work->maxOffset -= 40.0f;
    } else {
        work->mode = 0;
    }
    animGroupBaseAsync("EFF_naniga", inBattle, NULL);
    work->pose = -1;
    work->alpha = 255;
    pos = (Vec){ 0.0f, 0.0f, 0.0f };
    pos.x = work->position.x;
    pos.y = work->position.y;
    pos.z = work->position.z;
    work->screen = pos;
    PSMTXMultVec(camGetPtr(CAMERA_3D)->view, &work->screen, &work->screen);
    PSMTX44MultVec(camGetPtr(CAMERA_3D)->projection, &work->screen, &work->screen);
    work->screen.x *= (f32)gp->fbWidth / 2.0f;
    work->screen.y *= (f32)gp->efbHeight / 2.0f;
    return effect;
}

void effNanigaMain(EffectEntry* effect) {
    EffNanigaWork* work = effect->userdata;
    BOOL inBattle;
    Vec pos;
    Vec v;
    s32 i;
    EventEntry* evt;
    s32 state;

    v = (Vec){ 0.0f, 0.0f, 0.0f };
    v.x = work->position.x;
    v.y = work->position.y;
    v.z = work->position.z;
    pos = v;
    inBattle = gp->inBattle ? TRUE : FALSE;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        if (work->pose != -1) {
            animPoseRelease(work->pose);
        }
        effDelete(effect);
        return;
    }
    switch (work->state) {
    case 0:
        work->state++;
        work->timer = 30;
    case 1:
        if (work->timer == 30) {
            for (i = 0; i < 4; i++) {
                effKemuTestDrawCam(effKemuTestEntry(2, work->screen.x, work->screen.y, work->screen.z,
                                                    0.5f + (f32)irand(200) / 100.0f),
                                   8);
            }
            psndSFXOn_3D("SFX_ITEM_WHAT1", &pos);
        }
        if (--work->timer < 0) {
            work->timer = 60;
            work->state++;
        }
        break;
    case 2:
        work->timer--;
        if (work->timer < 0) {
            work->timer = 0;
            work->state++;
            work->sfxId = psndSFXOn_3D("SFX_ITEM_WHAT2", &pos);
        }
        break;
    case 3:
        work->offset = intplGetValue(EASING_CUBIC_OUT, work->minOffset, work->maxOffset, work->timer, 300);
        work->timer++;
        if (work->timer > 300) {
            psndSFXOff(work->sfxId);
            switch (work->mode) {
            case 0:
                work->timer = 60;
                work->state = 10;
                break;
            case 1:
                work->timer = 0;
                work->state = 4;
                break;
            case 2:
                work->timer = 0;
                work->state = 5;
                break;
            }
        }
        break;
    case 4:
        if (work->timer < 10) {
            work->timer++;
        } else if (work->timer < 20) {
            work->offset -= 4.0f;
            work->timer++;
        } else {
            work->offset = work->maxOffset - 40.0f;
            work->timer = 60;
            work->state = 10;
        }
        break;
    case 5:
        if (work->timer < 10) {
            work->timer++;
        } else if (work->timer < 20) {
            work->offset += 4.0f;
            work->timer++;
        } else {
            work->offset = 40.0f + work->maxOffset;
            work->timer = 60;
            work->state = 10;
        }
        break;
    case 10:
        if (work->timer != 0) {
            work->timer--;
        } else if (animGroupBaseAsync("EFF_naniga", inBattle, NULL)) {
            work->pose = animPoseEntry("EFF_naniga", inBattle);
            animPoseSetAnim(work->pose, "A_1", TRUE);
            work->state++;
            evt = evtEntryType(sound_evt, 0, 0, 0);
            evt->lwData[0] = FLOAT(pos.x);
            evt->lwData[1] = FLOAT(pos.y);
            evt->lwData[2] = FLOAT(pos.z);
        }
        break;
    case 11:
        if (animPoseGetLoopTimes(work->pose) > 0.8f) {
            effItemGetEntry(10, work->position.x, work->position.y, work->position.z);
            work->state++;
        }
        break;
    case 12:
        work->alpha -= 4;
        if (work->alpha < 0) {
            animPoseRelease(work->pose);
            work->pose = -1;
            work->timer = 30;
            work->state++;
        }
        break;
    case 13:
        if (work->timer != 0) {
            work->timer--;
        } else {
            effDelete(effect);
            return;
        }
        break;
    }
    while (work->offset < 0.0f) {
        work->offset += 40.0f * (f32)work->count;
    }
    while (work->offset >= 40.0f * (f32)(work->count + 2)) {
        work->offset -= 40.0f * (f32)work->count;
    }
    state = work->state;
    switch (state) {
    case 0:
    case 1:
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 10:
        dispEntry(CAMERA_2D, dispCalcZ(&pos), 1, (DispCallback)effNanigaDisp, effect);
        break;
    default:
        if (state == 11 && animPoseGetLoopTimes(work->pose) < 0.8f) {
            break;
        }
        dispEntry(CAMERA_3D, dispCalcZ(&pos), 1, (DispCallback)effNanigaDisp3, effect);
        break;
    }
    if (work->pose != -1) {
        dispEntry(CAMERA_3D, dispCalcZ(&pos), 2, (DispCallback)effNanigaDisp2, effect);
    }
}

void effNanigaDisp(CameraId cameraId, EffectEntry* effect) {
    EffNanigaWork* work = effect->userdata;
    CameraEntry* camera = camGetPtr(cameraId);
    Mtx mT, mR, mS, mC;
    GXTexObj texObj;
    u32 left, top, width, height;
    f32 w, lo, hi;
    f32 oldScale;
    s32 i;

    w = 40.0f;
    oldScale = work->scale;
    work->scale *= 1.5f;
    PSMTXTrans(mT, work->screen.x, work->screen.y, work->screen.z);
    PSMTXRotRad(mR, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(mS, work->scale, work->scale, work->scale);
    PSMTXConcat(mT, mR, mT);
    PSMTXConcat(mT, mS, mC);
    GXGetScissor(&left, &top, &width, &height);
    GXSetScissor(
        (u32)((f32)(u32)((f32)gp->fbWidth / 2.0f + work->screen.x) - 20.0f * work->scale),
        (u32)((f32)(u32)((f32)gp->efbHeight / 2.0f - work->screen.y) - 20.0f * work->scale),
        (u32)(40.0f * work->scale), (u32)(40.0f * work->scale));
    effGetTexObj(0x57, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXConcat(camera->view, mC, mT);
    GXLoadPosMtxImm(mT, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    lo = -w / 2.0f;
    hi = 20.0f;
    GXPosition3f32(lo, hi, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(hi, hi, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(hi, lo, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(lo, lo, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    for (i = 0; i < work->count + 5; i++) {
        ((void (*)(GXTexObj*, s32))iconGetTexObj)(&texObj, itemDataTable[work->list[i % work->count]].icon_id);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        PSMTXTrans(mT, 0.0f, -40.0f * (f32)i + work->offset, 0.0f);
        PSMTXScale(mS, 0.9f, 0.9f, 0.9f);
        PSMTXConcat(mC, mT, mT);
        PSMTXConcat(mT, mS, mT);
        PSMTXConcat(camera->view, mT, mT);
        GXLoadPosMtxImm(mT, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(lo, hi, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(hi, hi, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(hi, lo, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(lo, lo, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
    GXSetScissor(left, top, width, height);
    work->scale = oldScale;
}

void effNanigaDisp2(CameraId cameraId, EffectEntry* effect) {
    EffNanigaWork* work = effect->userdata;
    Mtx mT, mR, mS, mC;

    camGetPtr(cameraId);
    PSMTXTrans(mT, work->position.x, work->position.y - 20.0f, work->position.z);
    PSMTXRotRad(mR, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(mS, work->scale, work->scale, work->scale);
    PSMTXConcat(mT, mR, mT);
    PSMTXConcat(mT, mS, mC);
    animPoseSetMaterialFlagOn(work->pose, 0x40);
    animPoseSetMaterialEvtColor(work->pose, (GXColor){ 0xFF, 0xFF, 0xFF, work->alpha });
    animPoseMain(work->pose);
    animPoseDrawMtx(work->pose, mC, 1, 0.0f, 10.0f);
    animPoseDrawMtx(work->pose, mC, 2, 0.0f, 10.0f);
    animPoseDrawMtx(work->pose, mC, 3, 0.0f, 10.0f);
}

void effNanigaDisp3(CameraId cameraId, EffectEntry* effect) {
    EffNanigaWork* work = effect->userdata;
    ItemData* item;
    Mtx mT, mR, mS, mC;

    camGetPtr(cameraId);
    item = &itemDataTable[work->item];
    PSMTXTrans(mT, work->position.x, work->position.y - 16.0f, work->position.z);
    PSMTXRotRad(mR, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(mS, work->scale, work->scale, work->scale);
    PSMTXConcat(mT, mR, mT);
    PSMTXConcat(mT, mS, mC);
    if (work->state < 4) {
        iconDispGxCol(mC, 0, ICON_184, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    } else {
        iconDispGxCol(mC, 0, item->icon_id, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
    }
}
