#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"


typedef struct EffRecoverHeartN64Work {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 velX;     // 0x10
    f32 velY;     // 0x14
    f32 velZ;     // 0x18
    f32 rotZ;     // 0x1C
    f32 unk20;    // 0x20
    f32 offX;     // 0x24
    s32 alpha;    // 0x28
    s32 frame;    // 0x2C
    s32 elapsed;    // 0x30
    s32 delay;    // 0x34
    f32 scaleW;   // 0x38
    f32 scaleH;   // 0x3C
    s32 count;    // 0x40
    s32 flag;     // 0x44
} EffRecoverHeartN64Work;


#ifdef __MWERKS__
#endif

#ifdef __MWERKS__
#endif

effN64Vtx size32x32_tex32x32_heart_vtx[] = {
    {-16, -16, 0, 0, 0, 255, 255, 255, 255},
    {16, -16, 0, 1024, 0, 255, 255, 255, 255},
    {16, 16, 0, 1024, 1024, 255, 255, 255, 255},
    {-16, 16, 0, 0, 1024, 255, 255, 255, 255},
};

effN64Vtx size16x16_tex32x32_heart_vtx[] = {
    {-8, -8, 0, 0, 0, 255, 255, 255, 255},
    {8, -8, 0, 1024, 0, 255, 255, 255, 255},
    {8, 8, 0, 1024, 1024, 255, 255, 255, 255},
    {-8, 8, 0, 0, 1024, 255, 255, 255, 255},
};

effN64Vtx size24x24_tex32x32_heart_vtx[] = {
    {-12, -15, 0, 0, 0, 250, 250, 250, 255},
    {12, -15, 0, 1024, 0, 250, 250, 250, 255},
    {12, 9, 0, 1024, 1024, 250, 250, 250, 255},
    {-12, 9, 0, 0, 1024, 250, 250, 250, 255},
};

effN64Vtx size16x24_tex32x32_l_heart_vtx[] = {
    {-16, -15, 0, 0, 0, 250, 250, 250, 255},
    {4, -15, 0, 1024, 0, 250, 250, 250, 255},
    {4, 9, 0, 1024, 1024, 250, 250, 250, 255},
    {-16, 9, 0, 0, 1024, 250, 250, 250, 255},
};

effN64Vtx size16x24_tex32x32_r_heart_vtx[] = {
    {-4, -15, 0, 0, 0, 250, 250, 250, 255},
    {16, -15, 0, 1024, 0, 250, 250, 250, 255},
    {16, 9, 0, 1024, 1024, 250, 250, 250, 255},
    {-4, 9, 0, 0, 1024, 250, 250, 250, 255},
};

effN64Vtx size24x24_tex32x32_flower_vtx[] = {
    {-12, -15, 0, 0, 0, 36, 190, 20, 255},
    {12, -15, 0, 1024, 0, 36, 190, 20, 255},
    {12, 9, 0, 1024, 1024, 36, 190, 20, 255},
    {-12, 9, 0, 0, 1024, 36, 190, 20, 255},
};

effN64Vtx size16x24_tex32x32_l_flower_vtx[] = {
    {-16, -15, 0, 0, 0, 36, 190, 20, 255},
    {4, -15, 0, 1024, 0, 36, 190, 20, 255},
    {4, 9, 0, 1024, 1024, 36, 190, 20, 255},
    {-16, 9, 0, 0, 1024, 36, 190, 20, 255},
};

effN64Vtx size16x24_tex32x32_r_flower_vtx[] = {
    {-4, -15, 0, 0, 0, 36, 190, 20, 255},
    {16, -15, 0, 1024, 0, 36, 190, 20, 255},
    {16, 9, 0, 1024, 1024, 36, 190, 20, 255},
    {-4, 9, 0, 0, 1024, 36, 190, 20, 255},
};

f32 scale_w_dat[] = {0.25f, 0.5f, 0.75f, 1.0f, 0.9f, 1.1f, 1.0f};
f32 scale_h_dat[] = {0.25f, 0.5f, 0.75f, 1.0f, 1.2f, 0.9f, 1.0f};

void effRecoverHeartMain(EffectEntry* effect);
void effRecoverHeartDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effRecoverHeartN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffRecoverHeartN64Work* work;
    EffRecoverHeartN64Work* p;
    s32 i;
    s32 k;


    effect = effEntry();
    effect->type = "RecoverHeartN64";
    effect->count = 2;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffRecoverHeartN64Work) * 2);
    effect->userdata = work;
    effect->callback = effRecoverHeartMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->frame = 46;
    work->elapsed = 0;
    if (count < 0) {
        work->flag = 1;
        count = -count;
    } else {
        work->flag = 0;
    }
    work->count = count;
    k = 0;

    for (i = 1; i < 2; i++) {

        p = &work[i];
        p->position.x = 0.0f;
        p->position.y = 0.0f;
        p->position.z = 0.0f;
        p->offX = 0.0f;
        p->velX = 0.0f;
        p->velZ = 0.0f;
        p->unk20 = 0.0f;
        p->rotZ = 0.0f;
        if (i == 1) {
            p->position.x = 0.0f;
            p->delay = 0;
            p->scaleW = 0.0f;
            p->scaleH = 0.0f;
            p->velY = 1.4f;
        } else {
            k = (i & 1) * 2 - 1;
            p->position.x = k * ((i - 1) >> 1) * 4;
            p->velY = 1.4f;
            p->delay = 11;
            p->scaleW = 0.0f;
            p->scaleH = 0.0f;
        }
        p->frame = 0;
        p->alpha = 255;
    }
    return effect;
}

void effRecoverHeartMain(EffectEntry* effect) {
    s32 lifetime;
    EffRecoverHeartN64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    f32 ang;
    f32 fade;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->frame--;
    work->elapsed++;
    lifetime = work->frame;
    if (lifetime < 0) {
        effDelete(effect);
        return;
    }
    fade = 0.05f * lifetime;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        s32 frame = work->frame;
        if (work->delay > 0) {
            work->delay--;
            if (work->delay > 0) {
                continue;
            }
        }
        if (frame >= 7) {
            work->scaleW = work->scaleH = 1.0f;
        } else {
            work->scaleW = scale_w_dat[frame];
            work->scaleH = scale_h_dat[frame];
        }
        work->position.x += work->velX;
        work->position.y += work->velY;
        work->position.z += work->velZ;
        ang = 6.2831855f * (f32)(lifetime * 12 + i * 38) / 360.0f;
        work->rotZ = -30.0f * (f32)sin(ang);
        work->offX = 8.0f * (f32)cos(ang);
        if (lifetime < 20) {
            work->offX *= 0.05f * lifetime;
            work->rotZ *= 0.05f * lifetime;
        }
        if (lifetime < 10) {
            work->alpha = lifetime * 25;
            work->velY *= 0.9f;
        }
        work->frame++;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effRecoverHeartDisp, effect);
}

void effRecoverHeartDisp(CameraId cameraId, EffectEntry* effect) {
    s32 j;
    CameraEntry* camera;
    EffRecoverHeartN64Work* work;
    s32 i;
    s32 count;
    s32 type;
    s32 flag;
    s32 k;
    s32 tex;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx m90;
    Mtx m60;
    Mtx m30;
    GXTexObj texObj;

    camera = camGetPtr(cameraId);
    work = effect->userdata;
    flag = work->flag;
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, m90);
    type = work->type;
    count = work->count;
    work = work + 1;
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetCullMode(GX_CULL_NONE);
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay > 0) {
            continue;
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, work->alpha});
        if (work->alpha == 255) {
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
        } else {
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        }
        tex = (type == 0 || type == 2);
        if (tex) {
            tex = 0x51;
            if (flag == 0) {
                tex = 0x50;
            }
        } else {
            tex = 0x52;
        }
        effGetTexObjN64(tex, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
        GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
        PSMTXTrans(mtx, work->position.x + work->offX, work->position.y, work->position.z);
        PSMTXScale(scale, work->scaleW, work->scaleH, 1.0f);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(m90, mtx, m60);
        PSMTXRotRad(rot, 'z', 0.017453292f * work->rotZ);
        PSMTXConcat(m60, rot, m30);
        if (i == 1 && type != 2) {
            effSetVtxDescN64(size32x32_tex32x32_heart_vtx);
            PSMTXConcat(camera->view, m30, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
            if (count < 10 && flag == 0) {
                effGetTexObjN64(0x53, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                PSMTXScale(scale, 0.03125f, 0.0026041667f, 0.0f);
                PSMTXTrans(mtx, 0.0f, (f32)(0x160 - count * 32), 0.0f);
                PSMTXConcat(scale, mtx, scale);
                GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
                if (type == 0) {
                    effSetVtxDescN64(size24x24_tex32x32_heart_vtx);
                } else {
                    effSetVtxDescN64(size24x24_tex32x32_flower_vtx);
                }
                PSMTXConcat(camera->view, m60, mtx);
                GXLoadPosMtxImm(mtx, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
            } else {
                k = count % 10;
                j = (flag == 0) ? count / 10 : 10;
                effGetTexObjN64(0x53, &texObj);
                GXLoadTexObj(&texObj, GX_TEXMAP0);
                PSMTXScale(scale, 0.03125f, 0.0026041667f, 0.0f);
                PSMTXTrans(mtx, 0.0f, (f32)(0x160 - k * 32), 0.0f);
                PSMTXConcat(scale, mtx, scale);
                GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
                if (type == 0) {
                    effSetVtxDescN64(size16x24_tex32x32_r_heart_vtx);
                } else {
                    effSetVtxDescN64(size16x24_tex32x32_r_flower_vtx);
                }
                PSMTXConcat(camera->view, m60, mtx);
                GXLoadPosMtxImm(mtx, GX_PNMTX0);
                GXSetCurrentMtx(GX_PNMTX0);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                PSMTXScale(scale, 0.03125f, 0.0026041667f, 0.0f);
                PSMTXTrans(mtx, 0.0f, (f32)(0x160 - j * 32), 0.0f);
                PSMTXConcat(scale, mtx, scale);
                GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
                if (type == 0) {
                    effSetVtxDescN64(size16x24_tex32x32_l_heart_vtx);
                } else {
                    effSetVtxDescN64(size16x24_tex32x32_l_flower_vtx);
                }
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
            }
        } else {
            PSMTXConcat(camera->view, m30, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            effSetVtxDescN64(size16x16_tex32x32_heart_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
}

