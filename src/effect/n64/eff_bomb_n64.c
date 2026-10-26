#include "eff/n64/eff_bomb_n64.h"
#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/n64/eff_kemuri8_n64.h"
#include "eff/n64/eff_confetti_n64.h"

typedef struct EffBombN64Work {
    s32 type;        // 0x0
    Vec position;    // 0x4
    u8 pad_10[0x20]; // 0x10
    s32 lifetime;    // 0x30
    s32 frame;       // 0x34
    u8 pad_38[4];    // 0x38
    s32 flag;        // 0x3C
    s32 camId;       // 0x40
    s32 texId;       // 0x44
    u8 pad_48[0x1C]; // 0x48
    f32 flashScale;  // 0x64, scale of the texture 0x18 flash quad (PSMTXScale in the disp); grows by flashScaleAdd
    f32 flashScaleAdd; // 0x68, per-frame growth of flashScale; increases until frame 8, then *= 0.6
    u8 pad_6c[0x10]; // 0x6C
    s32 alpha;       // 0x7C
    u8 pad_80[8];    // 0x80
    s32 type2;       // 0x88
    u8 pad_8c[0x1C]; // 0x8C
    f32 scale;       // 0xA8
    f32 scaleAdd;    // 0xAC
    f32 rot;         // 0xB0
    f32 rotAdd;      // 0xB4
    u8 pad_b8[8];    // 0xB8
    s32 alpha2;      // 0xC0
    u8 pad_c4[8];    // 0xC4
} EffBombN64Work;


static effN64Vtx size8x8_tex32x32_vtx[] = {
    {-4, -4, 0, 0, 0, 255, 255, 255, 255},
    {4, -4, 0, 1024, 0, 255, 255, 255, 255},
    {4, 4, 0, 1024, 1024, 255, 255, 255, 255},
    {-4, 4, 0, 0, 1024, 255, 255, 255, 255},
};

static void effBombMain(EffectEntry* effect);
static void effBombDisp(CameraId cameraId, EffectEntry* effect);

void effBombN64SetCamId(EffectEntry* effect, s32 camId) {
    ((EffBombN64Work*)effect->userdata)->camId = camId;
}

EffectEntry* effBombN64Entry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffBombN64Work* work;
    s32 flag;

    effect = effEntry();
    flag = 0;
    if (type >= 3) {
        flag = 1;
        type -= 3;
    }
    effect->type = "BombN64";
    effect->count = 3;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffBombN64Work));
    effect->userdata = work;
    effect->callback = effBombMain;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->frame = 0;
    work->lifetime = 60;
    work->type = type;
    work->flag = flag;
    work->camId = 4;
    work->texId = type != 0 ? type : -1;
    work->alpha = 0;
    work->flashScale = type == 1 ? 1.0f : 3.0f;
    work->flashScaleAdd = type == 1 ? 1.0f : 3.0f;
    work->type2 = type;
    work->alpha2 = 255;
    switch (type) {
        case 0:
            work->scale = 9.9f;
            work->scaleAdd = 1.6f;
            break;
        case 1:
            work->scale = 13.9f;
            work->scaleAdd = 2.4f;
            break;
        case 2:
            work->scale = 17.8f;
            work->scaleAdd = 3.2f;
            break;
    }
    work->rot = 0.0f;
    work->rotAdd = 30.0f;
    return effect;
}

static void effBombMain(EffectEntry* effect) {
    EffBombN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;
    s32 frame;
    s32 camId;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    camId = work->camId;
    work->lifetime--;
    if (work->lifetime < 0) {
        effDelete(effect);
        return;
    }
    work->frame++;
    frame = work->frame;
    type = work->type;
    if (frame == 7) {
        if (work->flag == 0) {
            effKemuri8N64SetCamId(effKemuri8N64Entry(type, work->position.x, work->position.y, work->position.z), camId);
        } else {
            effKemuri8N64SetCamId(effKemuri8N64Entry(type + 3, work->position.x, work->position.y, work->position.z), camId);
        }
    } else if (frame == 1) {
        effConfettiN64SetCamId(effConfettiN64Entry(type + 4, 50, work->position.x, work->position.y, work->position.z, 1.0f), camId);
    }
    work->flashScale += work->flashScaleAdd;
    if (frame < 8) {
        work->flashScaleAdd += type == 2 ? 1.0f : 0.4f;
        work->alpha = (s32)(0.6f * (255.0f - work->alpha));
    } else {
        work->flashScaleAdd *= 0.6f;
        work->alpha *= 0.9f;
    }
    if (frame >= 3) {
        if (frame == 3) {
            work->scale = 1.0f;
        }
        work->scale += work->scaleAdd;
    }
    if (frame >= 3) {
        if (frame < 6) {
            work->scaleAdd += 0.1f;
        } else {
            work->scaleAdd *= 0.5f;
            work->alpha2 *= 0.9f;
        }
    }
    work->rot += work->rotAdd;
    dispEntry(camId, dispCalcZ(&position), 2, (DispCallback)effBombDisp, effect);
}

static void effBombDisp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffBombN64Work* work;

    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx base;
    GXTexObj texObj;
    u32 a;
    u32 b;
    s32 frame;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    frame = work->frame;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, base);
    PSMTXConcat(camera->view, base, base);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(mtx, 0.0625f, 0.0625f, 0.0f);
    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
    GXSetCullMode(GX_CULL_NONE);
    effSetVtxDescN64(size8x8_tex32x32_vtx);
    if (work->texId != -1) {
        effGetTexObjN64(0x18, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xF0, work->alpha});
        PSMTXScale(scale, work->flashScale, work->flashScale, 1.0f);
        PSMTXConcat(base, scale, scale);
        GXLoadPosMtxImm(scale, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
    if (frame == 1) {
        a = 0xC0;
    } else if (frame == 2) {
        a = 0xFF;
    } else {
        a = work->alpha2;
    }
    if (frame > 2) {
        s32 t = 0x7F;
        if (frame < 8) {
            t = frame << 4;
        }
        b = t + 0x80;
    } else {
        b = 0xFF;
    }
    GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, b, a});
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXRotRad(rot, 'z', 0.017453292f * work->rot);
    PSMTXConcat(rot, scale, scale);
    PSMTXConcat(base, scale, scale);
    GXLoadPosMtxImm(scale, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    switch (frame % 3) {
        case 0:
            effGetTexObjN64(0x19, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
        case 1:
            effGetTexObjN64(0x1A, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
        case 2:
            effGetTexObjN64(0x1B, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
    }
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
