#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double sin(double);
double cos(double);

typedef struct EffReleaseN64Work {
    s32 type;      // 0x0
    s32 sub;       // 0x4
    Vec position;  // 0x8
    s32 lifetime;  // 0x14
    s32 frame;     // 0x18
    s32 r;         // 0x1C
    s32 g;         // 0x20
    s32 b;         // 0x24
    s32 a;         // 0x28
    s32 r2;        // 0x2C
    s32 g2;        // 0x30
    s32 b2;        // 0x34
    s32 a2;        // 0x38
    f32 scale;     // 0x3C
    f32 size[4];   // 0x40
    s32 alpha[4];  // 0x50
    s32 unk60[4];  // 0x60
    s32 state;     // 0x70
    s32 step;      // 0x74
} EffReleaseN64Work;

typedef struct AnimeEntry {
    u16 time;
    u16 value;
} AnimeEntry;


void effReleaseMain(EffectEntry* effect);
void effReleaseDisp(CameraId cameraId, EffectEntry* effect);


static AnimeEntry anime_data[30] = {
    {0x0064, 0x00FF},
    {0x00C8, 0x00FF},
    {0x012C, 0x00FF},
    {0x0190, 0x00FF},
    {0x01F4, 0x00F5},
    {0x0258, 0x00EB},
    {0x02BC, 0x00E1},
    {0x0320, 0x00D7},
    {0x0384, 0x00CD},
    {0x03E8, 0x00C3},
    {0x044C, 0x00B9},
    {0x04B0, 0x00AF},
    {0x0514, 0x00A5},
    {0x0578, 0x009B},
    {0x05DC, 0x0091},
    {0x0640, 0x0087},
    {0x06A4, 0x007D},
    {0x0708, 0x0073},
    {0x076C, 0x0069},
    {0x07D0, 0x005F},
    {0x0834, 0x0055},
    {0x0898, 0x004B},
    {0x08FC, 0x0041},
    {0x0960, 0x0037},
    {0x09C4, 0x002D},
    {0x0A28, 0x001E},
    {0x0A8C, 0x0014},
    {0x0AF0, 0x000A},
    {0x0B54, 0x0005},
    {0x0BB8, 0x0000},
};

AnimeEntry anime_data_2[30] = {
    {0x0BB8, 0x0000},
    {0x0B54, 0x0005},
    {0x0AF0, 0x000A},
    {0x0A8C, 0x0014},
    {0x0A28, 0x001E},
    {0x09C4, 0x002D},
    {0x0960, 0x0037},
    {0x08FC, 0x0041},
    {0x0898, 0x004B},
    {0x0834, 0x0055},
    {0x07D0, 0x005F},
    {0x076C, 0x0069},
    {0x0708, 0x0073},
    {0x06A4, 0x007D},
    {0x0640, 0x0087},
    {0x05DC, 0x0091},
    {0x0578, 0x009B},
    {0x0514, 0x00A5},
    {0x04B0, 0x00AF},
    {0x044C, 0x00B9},
    {0x03E8, 0x00C3},
    {0x0384, 0x00CD},
    {0x0320, 0x00D7},
    {0x02BC, 0x00E1},
    {0x0258, 0x00EB},
    {0x01F4, 0x00F5},
    {0x0190, 0x00FF},
    {0x012C, 0x00FF},
    {0x00C8, 0x00FF},
    {0x0064, 0x00FF},
};

static effN64Vtx size16x16_tex64x64_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 2048, 0, 0, 0, 0, 255},
    {8, 8, 0, 2048, 2048, 0, 0, 0, 255},
    {-8, 8, 0, 0, 2048, 0, 0, 0, 255},
};

effN64Vtx size16x16_tex256x32_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 8192, 0, 0, 0, 0, 255},
    {8, -6, 0, 8192, 1024, 0, 0, 0, 255},
    {-8, -6, 0, 0, 1024, 0, 0, 0, 255},
    {-8, -6, 0, 0, 0, 0, 0, 0, 255},
    {8, -6, 0, 8192, 0, 0, 0, 0, 255},
    {8, -4, 0, 8192, 1024, 0, 0, 0, 255},
    {-8, -4, 0, 0, 1024, 0, 0, 0, 255},
    {-8, -4, 0, 0, 0, 0, 0, 0, 255},
    {8, -4, 0, 8192, 0, 0, 0, 0, 255},
    {8, -2, 0, 8192, 1024, 0, 0, 0, 255},
    {-8, -2, 0, 0, 1024, 0, 0, 0, 255},
    {-8, -2, 0, 0, 0, 0, 0, 0, 255},
    {8, -2, 0, 8192, 0, 0, 0, 0, 255},
    {8, 0, 0, 8192, 1024, 0, 0, 0, 255},
    {-8, 0, 0, 0, 1024, 0, 0, 0, 255},
    {-8, 0, 0, 8192, 1024, 0, 0, 0, 255},
    {8, 0, 0, 0, 1024, 0, 0, 0, 255},
    {8, 2, 0, 0, 0, 0, 0, 0, 255},
    {-8, 2, 0, 8192, 0, 0, 0, 0, 255},
    {-8, 2, 0, 8192, 1024, 0, 0, 0, 255},
    {8, 2, 0, 0, 1024, 0, 0, 0, 255},
    {8, 4, 0, 0, 0, 0, 0, 0, 255},
    {-8, 4, 0, 8192, 0, 0, 0, 0, 255},
    {-8, 4, 0, 8192, 1024, 0, 0, 0, 255},
    {8, 4, 0, 0, 1024, 0, 0, 0, 255},
    {8, 6, 0, 0, 0, 0, 0, 0, 255},
    {-8, 6, 0, 8192, 0, 0, 0, 0, 255},
    {-8, 6, 0, 8192, 1024, 0, 0, 0, 255},
    {8, 6, 0, 0, 1024, 0, 0, 0, 255},
    {8, 8, 0, 0, 0, 0, 0, 0, 255},
    {-8, 8, 0, 8192, 0, 0, 0, 0, 255},
};

effN64Vtx size16x16_tex128x128_vtx[] = {
    {-8, 0, -8, 0, 0, 0, 0, 0, 255},
    {8, 0, -8, 4096, 0, 0, 0, 0, 255},
    {8, 0, 8, 4096, 4096, 0, 0, 0, 255},
    {-8, 0, 8, 0, 4096, 0, 0, 0, 255},
};

effN64Vtx size16x16_tex64x64_0_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 2048, 0, 0, 0, 0, 255},
    {8, 0, 0, 2048, 1024, 0, 0, 0, 255},
    {-8, 0, 0, 0, 1024, 0, 0, 0, 255},
    {-8, 0, 0, 2048, 1024, 0, 0, 0, 255},
    {8, 0, 0, 0, 1024, 0, 0, 0, 255},
    {8, 8, 0, 0, 0, 0, 0, 0, 255},
    {-8, 8, 0, 2048, 0, 0, 0, 0, 255},
};

effN64Vtx rectangle_vtx[] = {
    {0, 0, 0, 2048, 1024, 0, 0, 0, 255},
    {640, 0, 0, 0, 1024, 0, 0, 0, 255},
    {640, 480, 0, 0, 0, 0, 0, 0, 255},
    {0, 480, 0, 2048, 0, 0, 0, 0, 255},
};

EffectEntry* effReleaseN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    EffectEntry* effect;
    EffReleaseN64Work* work;
    s32 sub;

    effect = effEntry();
    effect->type = "ReleaseN64";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffReleaseN64Work));
    effect->userdata = work;
    effect->callback = effReleaseMain;
    work->type = type & 0xFF00;
    sub = type & 0xFF;
    work->sub = sub;
    work->frame = 0;
    if (lifetime <= 0) {
        work->lifetime = 1000;
    } else {
        work->lifetime = lifetime;
    }
    work->a = 255;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->scale = scale;
    work->r = 255;
    work->g = 255;
    work->b = 255;
    work->r2 = 255;
    work->g2 = 255;
    work->b2 = 255;
    work->a2 = 255;
    work->unk60[0] = 0;
    work->unk60[1] = 0;
    work->unk60[2] = 0;
    work->unk60[3] = 0;
    work->size[0] = 1.0f;
    work->size[1] = 0.0f;
    work->size[2] = 0.0f;
    work->size[3] = 0.0f;
    work->alpha[0] = 255;
    work->alpha[1] = 0;
    work->alpha[2] = 0;
    work->alpha[3] = 0;
    if (sub == 3) {
        work->state = 1;
    } else {
        work->state = 0;
    }
    work->step = 0;
    return effect;
}

void effReleaseMain(EffectEntry* effect) {
    EffReleaseN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 frame;
    s32 sub;
    f32 sn;
    f32 v;
    s32 i;
    s32 step;
    s32 val;
    AnimeEntry* e;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    sub = work->sub;
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
        work->a = work->lifetime << 4;
    }
    switch (work->state) {
    case 0:
        work->step++;
        work->alpha[0] = (frame & 3) * 30 + 200;
        sn = (f32)sin(6.2831855f * (f32)(frame * 20) / 360.0f);
        work->size[0] = 0.04f * sn + 0.5f;
        if (work->type != 0) {
            sn = (f32)sin(6.2831855f * (f32)frame / 360.0f);
            work->position.x = work->position.x + sn;
            work->position.y = work->position.y + (f32)cos(6.2831855f * (1.235631f * (f32)frame) / 360.0f);
            if (work->step > 50) {
                work->step = 0;
                if (sub == 3) {
                    work->state = 100;
                } else {
                    work->state = 1;
                }
            }
        }
        break;
    case 100:
        switch (work->step) {
        case 0:
            work->size[3] = 4.0f;
            work->alpha[3] = 255;
            break;
        case 1:
            work->size[3] = 8.0f;
            work->alpha[3] = 255;
            break;
        default:
            work->size[3] = 0.0f;
            work->alpha[3] = 0;
            break;
        }
        if (work->step < 30) {
            v = 0.01f * (f32)anime_data[work->step].time + 0.3f * (f32)work->step;
            val = anime_data[work->step].value & 0xFF;
        } else {
            v = 0.01f * (f32)anime_data[29].time + 0.3f * (f32)work->step;
            val = anime_data[29].value & 0xFF;
        }
        work->alpha[0] = 0;
        work->size[0] = 0.0f;
        work->alpha[1] = val & 0xFF;
        work->alpha[2] = val & 0xFF;
        work->size[1] = v;
        work->size[2] = 0.6f + v;
        work->step++;
        break;
    case 1:
        switch (work->step) {
        case 0:
            work->size[3] = 4.0f;
            work->alpha[3] = 255;
            break;
        case 1:
            work->size[3] = 8.0f;
            work->alpha[3] = 255;
            break;
        default:
            work->size[3] = 0.0f;
            work->alpha[3] = 0;
            break;
        }
        v = 0.01f * (f32)anime_data[work->step].time;
        val = anime_data[work->step].value & 0xFF;
        work->alpha[0] = 0;
        work->alpha[1] = val;
        work->alpha[2] = val;
        work->size[0] = 0.0f;
        work->size[1] = v;
        work->size[2] = 1.2f * v + 4.0f;
        work->step++;
        if (work->step >= 30) {
            work->step = 0;
            work->state = 4;
        }
        break;
    case 4:
        switch (29 - work->step) {
        case 0:
            work->size[3] = 4.0f;
            work->alpha[3] = 255;
            break;
        case 1:
            work->size[3] = 8.0f;
            work->alpha[3] = 255;
            break;
        default:
            work->size[3] = 0.0f;
            work->alpha[3] = 0;
            break;
        }
        v = 0.01f * (f32)anime_data_2[work->step].time;
        val = anime_data_2[work->step].value & 0xFF;
        work->alpha[0] = 0;
        work->size[0] = 0.0f;
        work->alpha[1] = val;
        work->alpha[2] = 0;
        work->size[1] = v;
        work->size[2] = 0.0f;
        work->step++;
        if (work->step >= 30) {
            work->step = 0;
            work->state = 5;
        }
        break;
    case 5:
    default:
        work->size[0] = 0.0f;
        work->size[1] = 0.0f;
        work->size[2] = 0.0f;
        work->alpha[0] = 0;
        work->alpha[1] = 0;
        work->alpha[2] = 0;
        work->size[3] = 0.0f;
        work->alpha[3] = 0;
        work->size[3] = 0.0f;
        work->step++;
        break;
    }
    for (i = 0; i < 4; i++) {
        if (work->alpha[i] > 255) {
            work->alpha[i] = 255;
        }
        if (work->alpha[i] < 0) {
            work->alpha[i] = 0;
        }
        if (work->size[i] < 0.0f) {
            work->size[i] = 0.0f;
        }
    }
    dispEntry(4, dispCalcZ(&position), 2, (DispCallback)effReleaseDisp, effect);
}

void effReleaseDisp(CameraId cameraId, EffectEntry* effect) {
    GXColor c0;
    GXColor c1;
    CameraEntry* camera;
    EffReleaseN64Work* work;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    Mtx44 proj;
    f32 projv[7];
    s32 variant;
    s32 i;
    s32 alpha;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    variant = work->sub;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXScale(scale, work->scale, work->scale, work->scale);
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(mtx, scale, tmp);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    {


        GXSetTevColor(GX_TEVREG1, (GXColor){work->r2, work->g2, work->b2, work->a2});
    }
    GXSetCullMode(GX_CULL_NONE);
    for (i = 0; i < 4; i++) {
        if (variant == 1 && i == 2) {
            continue;
        }
        if (work->size[i] == 0.0f || work->alpha[i] == 0) {
            continue;
        }
        PSMTXScale(scale, work->size[i], work->size[i], work->size[i]);
        PSMTXConcat(tmp, scale, scale);
        PSMTXConcat(camera->view, scale, scale);
        GXLoadPosMtxImm(scale, GX_PNMTX0);
        GXSetCurrentMtx(0);
        {


            GXSetTevColor(GX_TEVREG0, (GXColor){work->r, work->g, work->b, work->alpha[i]});
        }
        switch (i) {
        case 0:
            effGetTexObjN64(0x44, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            effSetVtxDescN64(size16x16_tex64x64_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            break;
        case 1:
            effGetTexObjN64(0x45, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            effSetVtxDescN64(size16x16_tex256x32_vtx);
            PSMTXScale(scale, 0.00390625f, 0.00390625f, 0.0f);
            PSMTXTrans(mtx, 0.0f, 0.0f, 0.0f);
            PSMTXConcat(scale, mtx, scale);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            tri2(0x1c, 0x1d, 0x1e, 0x1c, 0x1c, 0x1e, 0x1f, 0x1c);
            PSMTXScale(scale, 0.00390625f, 0.00390625f, 0.0f);
            PSMTXTrans(mtx, 0.0f, 32.0f, 0.0f);
            PSMTXConcat(scale, mtx, scale);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(4, 5, 6, 4, 4, 6, 7, 4);
            tri2(0x18, 0x19, 0x1a, 0x18, 0x18, 0x1a, 0x1b, 0x18);
            PSMTXScale(scale, 0.00390625f, 0.00390625f, 0.0f);
            PSMTXTrans(mtx, 0.0f, 64.0f, 0.0f);
            PSMTXConcat(scale, mtx, scale);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(8, 9, 10, 8, 8, 10, 11, 8);
            tri2(0x14, 0x15, 0x16, 0x14, 0x14, 0x16, 0x17, 0x14);
            PSMTXScale(scale, 0.00390625f, 0.00390625f, 0.0f);
            PSMTXTrans(mtx, 0.0f, 96.0f, 0.0f);
            PSMTXConcat(scale, mtx, scale);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(0xc, 0xd, 0xe, 0xc, 0xc, 0xe, 0xf, 0xc);
            tri2(0x10, 0x11, 0x12, 0x10, 0x10, 0x12, 0x13, 0x10);
            break;
        case 2:
            effGetTexObjN64(0x46, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            PSMTXScale(scale, 0.015625f, 0.015625f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            effSetVtxDescN64(size16x16_tex128x128_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        case 3:
            effGetTexObjN64(0x47, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            PSMTXScale(scale, 0.015625f, 0.03125f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            effSetVtxDescN64(size16x16_tex64x64_0_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 12);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            tri2(4, 5, 6, 4, 4, 6, 7, 4);
            break;
        case 4:
            break;
        }
    }
    if (variant == 2 || variant == 3) {
        alpha = 0;
        if (work->state == 1 || work->state == 100) {
            if (work->step > 2 && work->step <= 10) {
                alpha = 255 - (work->step - 2) * 30;
            }
        }
        if (work->state == 5) {
            if (work->step > 0 && work->step <= 8) {
                alpha = 255 - work->step * 30;
            }
        }
        if (alpha > 0) {
            GXGetProjectionv(projv);
            C_MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 608.0f, 0.0f, 1.0f);
            GXSetProjection(proj, GX_ORTHOGRAPHIC);
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
            {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0xD0, 0xD0, 0xD0, alpha});
            }
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
            PSMTXIdentity(mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(0);
            effSetVtxDescN64(rectangle_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            GXSetProjectionv(projv);
        }
    }
}
