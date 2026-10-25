#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

double cos(double);
typedef struct effN64Vtx {
    s16 x, y, z;
    s16 s, t;
    u8 r, g, b, a;
} effN64Vtx;

void effGetTexObjN64(u32 id, GXTexObj* obj);
void effSetVtxDescN64(effN64Vtx* vtx);
void tri2();

typedef struct PturnElem {
    s32 type;   // 0x0
    s32 active; // 0x4
    Vec pos;    // 0x8
    Vec vel;    // 0x14
    f32 y20;    // 0x20
    s32 alpha;  // 0x24
    s32 age;    // 0x28
    s32 frame;  // 0x2C
    f32 scale;  // 0x30
    f32 scale2; // 0x34
    s32 n1;     // 0x38
    s32 n2;     // 0x3C
    f32 rot;    // 0x40
    s32 delay;  // 0x44
} PturnElem;


effN64Vtx size48x48_tex32x32_pturn_vtx[] = {
    {-24, -24, 0, -512, 512, 255, 255, 255, 255},
    {24, -24, 0, 512, -512, 255, 255, 255, 255},
    {24, 24, 0, 1536, 512, 255, 255, 255, 255},
    {-24, 24, 0, 512, 1536, 255, 255, 255, 255},
};

effN64Vtx size24x24_tex32x32_pturn_vtx[] = {
    {-12, -12, 0, 0, 0, 250, 250, 0, 255},
    {12, -12, 0, 1024, 0, 250, 250, 0, 255},
    {12, 12, 0, 1024, 1024, 250, 250, 0, 255},
    {-12, 12, 0, 0, 1024, 250, 250, 0, 255},
};

effN64Vtx size16x24_tex32x32_l_pturn_vtx[] = {
    {-16, -12, 0, 0, 0, 250, 250, 0, 255},
    {4, -12, 0, 1024, 0, 250, 250, 0, 255},
    {4, 12, 0, 1024, 1024, 250, 250, 0, 255},
    {-16, 12, 0, 0, 1024, 250, 250, 0, 255},
};

effN64Vtx size16x24_tex32x32_r_pturn_vtx[] = {
    {-4, -12, 0, 0, 0, 250, 250, 0, 255},
    {16, -12, 0, 1024, 0, 250, 250, 0, 255},
    {16, 12, 0, 1024, 1024, 250, 250, 0, 255},
    {-4, 12, 0, 0, 1024, 250, 250, 0, 255},
};

f32 scale_dat[9] = {5.0f, 4.5f, 4.0f, 3.0f, 2.0f, 1.0f, 0.7f, 1.2f, 1.0f};

void effPturnMain(EffectEntry* effect);
void effPturnDisp(CameraId cameraId, EffectEntry* effect);
void pturn_main_dispp(CameraId cameraId, EffectEntry* effect);
void pturn_num_dispp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effPturnN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    PturnElem* work;

    effect = effEntry();
    effect->type = "PturnN64";
    effect->count = 2;
    work = __memAlloc(HEAP_EFFECT, sizeof(PturnElem) * 2);
    effect->userdata = work;
    effect->callback = effPturnMain;
    effect->flags |= 2;
    work->type = type;
    work->scale2 = 1.0f;
    if (type == 1) {
        work->n1 = 1;
        work->n2 = 0;
    } else {
        work->n1 = 0;
        work->n2 = lifetime;
    }
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    switch (type) {
    case 0:
        work->age = 100;
        break;
    case 1:
        work->age = 100;
        break;
    case 10:
        work->age = 21;
        break;
    }
    work->frame = 0;
    work->delay = 0;
    work->active = 1;
    work[1].pos.z = 0.0f;
    work[1].pos.y = 0.0f;
    work[1].pos.x = 0.0f;
    work[1].y20 = 0.0f;
    work[1].vel.z = 0.0f;
    work[1].vel.y = 0.0f;
    work[1].vel.x = 0.0f;
    work[1].rot = 0.0f;
    work[1].age = 0;
    work[1].alpha = 255;
    return effect;
}

void effPturnMain(EffectEntry* effect) {
    PturnElem* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 n2;
    s32 frame;
    s32 n1;
    s32 alpha;
    s32 life;
    s32 type;
    f32 a;
    f32 b;
    f32 c;
    f32 scl;

    tmp.x = work->pos.x;
    tmp.y = work->pos.y;
    tmp.z = work->pos.z;
    position = tmp;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->age = 21;
    }
    if (work->age < 100) {
        work->age--;
    }
    if (work->age < 0) {
        effDelete(effect);
        return;
    }
    work->frame++;
    if (work->frame > 324000) {
        work->frame = 256;
    }
    type = work->type;
    life = work->age;
    frame = work->frame;
    n2 = work->n2;
    n1 = work->n1;
    scl = work->scale2;
    if (type == 0) {
        if (work->delay > 0) {
            work->delay--;
        } else if (n2 != n1) {
            if (n2 < 0) {
                n2 = 0;
            }
            if (n2 > n1) {
                alpha = 60;
                if (type == 0) {
                    alpha = 100;
                }
                work->age = alpha;
                work->frame = 1;
                work->n1++;
                work[1].age = 0;
            } else if (n2 < n1) {
                effPturnN64Entry(10, 0, work->pos.x, work->pos.y + work[1].y20, work->pos.z);
                work->n1--;
            }
            work->delay = 8;
        }
    }
    a = 6.2831855f * (f32)(frame * 12);
    b = 0.05f * (f32)life;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        s32 f2;
        s32 age = work->age;
        f32 y;
        work->scale = (age >= 9 || type == 10) ? 1.0f : scale_dat[age];
        work->scale = work->scale * scl;
        f2 = (type == 0 || type == 1);
        if (f2) {
            c = (f32)cos(a / 360.0f);
            y = 4.0f * c;
        } else {
            y = 0.0f;
        }
        work->y20 = y;
        work->pos.x = work->pos.x + work->vel.x;
        work->pos.y = work->pos.y + work->vel.y;
        work->pos.z = work->pos.z + work->vel.z;
        if (life == 20) {
            work->vel.y = 4.0f;
            work->vel.x = -2.0f;
        }
        if (life < 20) {
            work->y20 *= b;
            work->vel.y += -0.5f;
            work->rot += 10.0f;
        }
        if (life < 10) {
            work->alpha = life * 25;
        }
        work->age++;
    }
    dispEntry(4, dispCalcZ(&position), 2, (DispCallback)effPturnDisp, effect);
}

void effPturnDisp(CameraId cameraId, EffectEntry* effect) {
    PturnElem* work = effect->userdata;
    if (work->active != 0) {
        pturn_main_dispp(cameraId, effect);
        if (work->type != 1) {
            pturn_num_dispp(cameraId, effect);
        }
    }
}

void pturn_main_dispp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    PturnElem* p;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    s32 i;
    s32 n1;
    s32 type;

    camera = camGetPtr(cameraId);
    p = effect->userdata;
    PSMTXTrans(mtx, p->pos.x, p->pos.y, p->pos.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, tmp);
    PSMTXConcat(camera->view, tmp, tmp);
    n1 = p->n1;
    type = p->type;
    p++;
    for (i = 1; i < effect->count; i++, p++) {
        if (n1 > 0 || type == 10) {
            effGetTexObjN64(0x92, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            PSMTXTrans(mtx, p->pos.x, p->pos.y, p->pos.z);
            PSMTXScale(scale, p->scale, p->scale, 1.0f);
            PSMTXRotRad(rot, 'z', 0.017453292f * p->rot);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(mtx, rot, mtx);
            PSMTXTrans(scale, 0.0f, p->y20, 0.0f);
            PSMTXConcat(mtx, scale, mtx);
            PSMTXConcat(tmp, mtx, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(0);
            GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, p->alpha});
            if (p->alpha == 255) {
                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
            } else {
                GXSetNumChans(1);
                GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
                GXSetNumTevStages(1);
                GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
                GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            }
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
            PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
            GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
            GXSetCullMode(GX_CULL_NONE);
            effSetVtxDescN64(size48x48_tex32x32_pturn_vtx);
            GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
}

void pturn_num_dispp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    GXTexObj texObj;
    Mtx mtx;
    Mtx scale;
    Mtx rot;
    Mtx tmp;
    s32 off0;
    s32 off1;
    s32 off2;
    PturnElem* p;
    s32 i;
    s32 n;

    camera = camGetPtr(cameraId);
    p = effect->userdata;
    PSMTXTrans(mtx, p->pos.x, p->pos.y, p->pos.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(mtx, rot, tmp);
    PSMTXConcat(camera->view, tmp, tmp);
    n = p->n1;
    off0 = 352 - n * 32;
    off1 = 352 - (n % 10) * 32;
    off2 = 352 - (n / 10) * 32;
    p++;
    for (i = 1; i < effect->count; i++, p++) {
        PSMTXTrans(mtx, p->pos.x, p->pos.y, p->pos.z);
        PSMTXScale(scale, p->scale, p->scale, 1.0f);
        PSMTXRotRad(rot, 'z', 0.017453292f * p->rot);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(mtx, rot, mtx);
        PSMTXTrans(scale, 0.0f, p->y20, 0.0f);
        PSMTXConcat(mtx, scale, mtx);
        PSMTXConcat(tmp, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(0);
        if (n > 0) {
            GXSetNumChans(1);
            GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
            GXSetNumTevStages(1);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevColor(GX_TEVREG0, (GXColor){0, 0, 0, p->alpha});
            effGetTexObjN64(0x53, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
            if (n < 10) {
                PSMTXScale(scale, 0.03125f, 0.0026041667f, 0.0f);
                PSMTXTrans(mtx, 0.0f, (f32)off0, 0.0f);
                PSMTXConcat(scale, mtx, scale);
                GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
                effSetVtxDescN64(size24x24_tex32x32_pturn_vtx);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
            } else {
                PSMTXScale(scale, 0.03125f, 0.0026041667f, 0.0f);
                PSMTXTrans(mtx, 0.0f, (f32)off1, 0.0f);
                PSMTXConcat(scale, mtx, scale);
                GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
                effSetVtxDescN64(size16x24_tex32x32_r_pturn_vtx);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
                PSMTXScale(scale, 0.03125f, 0.0026041667f, 0.0f);
                PSMTXTrans(mtx, 0.0f, (f32)off2, 0.0f);
                PSMTXConcat(scale, mtx, scale);
                GXLoadTexMtxImm(scale, GX_TEXMTX0, GX_MTX2x4);
                effSetVtxDescN64(size16x24_tex32x32_l_pturn_vtx);
                GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
                tri2(0, 1, 2, 0, 0, 2, 3, 0);
            }
        }
    }
}
