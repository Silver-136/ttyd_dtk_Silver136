#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdio.h>
#include <string.h>
#include "eff/eff_pointget.h"

typedef struct EffPointGetWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 number;    // 0x10
    f32 scale;     // 0x14
    s32 duration;  // 0x18
    s32 timer;     // 0x1C
    s32 state;     // 0x20
} EffPointGetWork;

void effPointGetMain(EffectEntry* effect);
void effPointGetDisp(CameraId cameraId, EffectEntry* effect);

#ifdef VERSION_G8ME01
typedef struct PointGetTex {
    s32 kind;
    s32 id;
    s32 idShadow;
} PointGetTex;

typedef struct PointGetTexSet {
    PointGetTex* tbl[2];
    s32 count;
} PointGetTexSet;

static PointGetTex tex_jp[3] = { { 0, 0x7E, 0x7F }, { -1, -1, -1 }, { 1, 0x7C, 0x7D } };
static PointGetTex tex_us[5] = { { 1, 0x7C, 0x7D }, { -1, -1, -1 }, { 0, 0x7E, 0x7F }, { -1, -1, -1 }, { 1, 0x80, 0x81 } };
static PointGetTex tex_shadow_us[5] = { { 1, 0x7C, 0x7D }, { -1, -1, -1 }, { 0, 0x7E, 0x7F }, { -1, -1, -1 }, { 1, 0x82, 0x83 } };
static PointGetTex tex_lang3[3] = { { 0, 0x7C, 0x7D }, { -1, -1, -1 }, { 1, 0x7E, 0x7F } };
static PointGetTex tex_shadow_lang3[3] = { { 0, 0x7C, 0x7D }, { -1, -1, -1 }, { 1, 0x80, 0x81 } };
static PointGetTex tex_lang4[3] = { { 0, 0x7C, 0x7D }, { -1, -1, -1 }, { 1, 0x7E, 0x7F } };
static PointGetTex tex_shadow_lang4[3] = { { 0, 0x7C, 0x7D }, { -1, -1, -1 }, { 1, 0x80, 0x81 } };
static PointGetTex tex_lang5[7] = { { 1, 0x7C, 0x7D }, { -1, -1, -1 }, { 0, 0x7E, 0x7F }, { -1, -1, -1 }, { 1, 0x80, 0x81 }, { -1, -1, -1 }, { 1, 0x84, 0x85 } };
static PointGetTex tex_shadow_lang5[7] = { { 1, 0x7C, 0x7D }, { -1, -1, -1 }, { 0, 0x7E, 0x7F }, { -1, -1, -1 }, { 1, 0x82, 0x83 }, { -1, -1, -1 }, { 1, 0x84, 0x85 } };
static PointGetTex tex_lang6[2] = { { 0, 0x7E, 0x7F }, { 1, 0x7C, 0x7D } };
static PointGetTex tex_shadow_lang6[2] = { { 0, 0x7E, 0x7F }, { 1, 0x80, 0x81 } };

static PointGetTexSet texid_tbl[6] = {
    { { tex_jp, tex_jp }, 3 },
    { { tex_us, tex_shadow_us }, 5 },
    { { tex_lang3, tex_shadow_lang3 }, 3 },
    { { tex_lang4, tex_shadow_lang4 }, 3 },
    { { tex_lang5, tex_shadow_lang5 }, 7 },
    { { tex_lang6, tex_shadow_lang6 }, 2 },
};
#else
static s32 texid_tbl[2][4] = { { 0x98, 0x99, 0x9A, 0x9B }, { 0x98, 0x99, 0x9A, 0x9B } };
#endif

EffectEntry* effPointGetEntry(s32 type, f32 x, f32 y, f32 z, s32 number, s32 duration) {
    EffectEntry* effect;
    EffPointGetWork* work;

    effect = effEntry();
    effect->type = "PointGet";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffPointGetWork) * effect->count);
    effect->userdata = work;
    effect->callback = effPointGetMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x - 2000.0f;
#ifdef VERSION_G8ME01
    work->position.y = y - 30.0f;
#else
    work->position.y = y;
#endif
    work->position.z = z;
    work->number = number;
    work->scale = 1.0f;
    work->duration = duration;
    work->timer = 0;
    work->state = 0;
    return effect;
}

void effPointGetMain(EffectEntry* effect) {
    EffPointGetWork* work = effect->userdata;
    Vec position;
    Vec tmp = { 0.0f, 0.0f, 0.0f };

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    work->timer++;
    switch (work->state) {
        case 0:
            work->position.x = intplGetValue(11, -1000.0f, 0.0f, work->timer, 45);
            if (work->timer >= 45) {
                work->timer = 0;
                work->state++;
            }
            break;
        case 1:
            if (work->duration == 0) {
                if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
                    effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
                    work->timer = 0;
                    work->state++;
                }
            } else if (work->timer >= work->duration) {
                work->timer = 0;
                work->state++;
            }
            break;
        case 2:
            work->position.x = intplGetValue(11, 0.0f, 1000.0f, work->timer, 60);
            if (work->timer >= 60) {
                effDelete(effect);
                return;
            }
            break;
    }
    dispEntry(CAMERA_2D, dispCalcZ(&position), 2, (DispCallback)effPointGetDisp, effect);
}

void effPointGetDisp(CameraId cameraId, EffectEntry* effect) {
#ifdef VERSION_G8ME01
    f32 w;
    f32 w1;
#endif
    EffPointGetWork* work = effect->userdata;
    CameraEntry* camera = camGetPtr(cameraId);
    f32 scale = work->scale;
#ifdef VERSION_G8ME01
    f32 pos;
    f32 total;
    f32 ox;
    f32 oy;
    f32 h;
    GXColor color;
#else
    f32 base;
    f32 y1;
    f32 width;
    f32 wide;
    f32 x0;
    f32 ox;
    f32 oy;
#endif
    GXTexObj texObj;
    Mtx mtx;
    Mtx scaleMtx;
    Mtx baseMtx;
    char buf[32];
    s32 i;
    s32 pass;
#ifdef VERSION_G8ME01
    s32 j;
    // FAKE: the digit table is read with one flat index over the rows' three words
    // (language * 3 + slot) through row 0's tbl[], as the target computes it; the natural
    // texid_tbl[gp->language].tbl[slot] shares the row address with .count and differs.
    PointGetTex* tbl = texid_tbl[0].tbl[gp->language * 3 + (work->number > 1 ? 1 : 0)];
    s32 count = texid_tbl[gp->language].count;
    s32 kind;
    s32 id;
    s32 idShadow;

    sprintf(buf, "%d", work->number);
    total = 0.0f;
    for (i = 0; i < count; i++) {
        kind = tbl[i].kind;
        id = tbl[i].id;
        switch (kind) {
            case 0:
                effGetTexObj(id, &texObj);
                w = (f32)strlen(buf) * GXGetTexObjWidth(&texObj) / 10.0f;
                total += w / 2.0f;
                break;
            case 1:
                effGetTexObj(id, &texObj);
                total += GXGetTexObjWidth(&texObj) / 2.0f;
                break;
            case -1:
                total += 5.0f;
                break;
        }
    }
    if (total > 200.0f) {
        scale *= 200.0f / total;
    }
#else

    sprintf(buf, "%d", work->number);
    width = 32.0f * (f32)strlen(buf);
#endif
    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXScale(scaleMtx, scale, scale, scale);
    PSMTXConcat(mtx, scaleMtx, baseMtx);
    PSMTXConcat(camera->view, baseMtx, baseMtx);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
#ifdef VERSION_G8ME01
    for (pass = 1; pass >= 0; pass--) {
        ox = (f32)(pass * 4);
        oy = (f32)(-pass * 4);
        color = pass != 0 ? (GXColor){ 0, 0, 0, 0x80 } : (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
        pos = 0.0f;
        for (i = 0; i < count; i++) {
            kind = tbl[i].kind;
            id = tbl[i].id;
            idShadow = tbl[i].idShadow;
            switch (kind) {
                case 0:
                    effGetTexObj(pass != 0 ? idShadow : id, &texObj);
                    GXLoadTexObj(&texObj, GX_TEXMAP0);
                    effGetTexObj(id, &texObj);
                    GXSetChanMatColor(GX_COLOR0A0, color);
                    for (j = 0; j < strlen(buf); j++) {
                        pos += (GXGetTexObjWidth(&texObj) / 10.0f) / 2.0f;
                        PSMTXTrans(mtx, ox + (pos - total), oy, 0.0f);
                        PSMTXScale(scaleMtx, 1.4285715f, 1.4285715f, 1.4285715f);
                        PSMTXConcat(baseMtx, mtx, mtx);
                        PSMTXConcat(mtx, scaleMtx, mtx);
                        GXLoadPosMtxImm(mtx, GX_PNMTX0);
                        GXSetCurrentMtx(GX_PNMTX0);
                        PSMTXScale(scaleMtx, 0.1f, 1.0f, 1.0f);
                        PSMTXTrans(mtx, (f32)((s8)buf[j] - 0x30), 0.0f, 0.0f);
                        PSMTXConcat(scaleMtx, mtx, mtx);
                        GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
                        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                        w = GXGetTexObjWidth(&texObj) / 10.0f;
                        h = GXGetTexObjHeight(&texObj);
                        GXPosition3f32(-w / 2.0f, h, 0.0f);
                        GXTexCoord2f32(0.0f, 0.0f);
                        GXPosition3f32(w / 2.0f, h, 0.0f);
                        GXTexCoord2f32(1.0f, 0.0f);
                        GXPosition3f32(w / 2.0f, 0.0f, 0.0f);
                        GXTexCoord2f32(1.0f, 1.0f);
                        GXPosition3f32(-w / 2.0f, 0.0f, 0.0f);
                        GXTexCoord2f32(0.0f, 1.0f);
                        pos += (GXGetTexObjWidth(&texObj) / 10.0f) / 2.0f;
                    }
                    break;
                case 1:
                    effGetTexObj(pass != 0 ? idShadow : id, &texObj);
                    GXLoadTexObj(&texObj, GX_TEXMAP0);
                    effGetTexObj(id, &texObj);
                    GXSetChanMatColor(GX_COLOR0A0, color);
                    pos += GXGetTexObjWidth(&texObj) / 2.0f;
                    PSMTXTrans(mtx, ox + (pos - total), oy, 0.0f);
                    PSMTXConcat(baseMtx, mtx, mtx);
                    GXLoadPosMtxImm(mtx, GX_PNMTX0);
                    GXSetCurrentMtx(GX_PNMTX0);
                    PSMTXIdentity(mtx);
                    GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    w1 = GXGetTexObjWidth(&texObj);
                    h = GXGetTexObjHeight(&texObj);
                    GXPosition3f32(-w1 / 2.0f, h, 0.0f);
                    GXTexCoord2f32(0.0f, 0.0f);
                    GXPosition3f32(w1 / 2.0f, h, 0.0f);
                    GXTexCoord2f32(1.0f, 0.0f);
                    GXPosition3f32(w1 / 2.0f, 0.0f, 0.0f);
                    GXTexCoord2f32(1.0f, 1.0f);
                    GXPosition3f32(-w1 / 2.0f, 0.0f, 0.0f);
                    GXTexCoord2f32(0.0f, 1.0f);
                    pos += GXGetTexObjWidth(&texObj) / 2.0f;
                    break;
                case -1:
                    pos += 10.0f;
                    break;
            }
        }
#else
    base = -(362.0f + width);
    width = 48.0f;
    wide = 352.0f;
    y1 = 24.0f;
    for (pass = 1; pass >= 0; pass--) {
        x0 = -width / 2.0f;
        ox = (f32)(pass * 4);
        oy = (f32)(-pass * 4);
        if (pass != 0) {
            effGetTexObj(texid_tbl[gp->language != 0][3], &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0, 0, 0, 0x80 });
        } else {
            effGetTexObj(texid_tbl[gp->language != 0][2], &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        }
        for (i = 0; i < strlen(buf); i++) {
            PSMTXTrans(mtx, ox + (32.0f * (f32)i + base / 2.0f), oy, 0.0f);
            PSMTXConcat(baseMtx, mtx, mtx);
            GXLoadPosMtxImm(mtx, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
            PSMTXScale(scaleMtx, 0.1f, 1.0f, 1.0f);
            PSMTXTrans(mtx, (f32)((s8)buf[i] - 0x30), 0.0f, 0.0f);
            PSMTXConcat(scaleMtx, mtx, mtx);
            GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(0.0f, y1, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(width, y1, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(width, x0, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(0.0f, x0, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
        if (pass != 0) {
            effGetTexObj(texid_tbl[gp->language != 0][1], &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0, 0, 0, 0x80 });
        } else {
            effGetTexObj(texid_tbl[gp->language != 0][0], &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF });
        }
        PSMTXTrans(mtx, 10.0f + (ox + (32.0f * (f32)strlen(buf) + base / 2.0f)), oy, 0.0f);
        PSMTXConcat(baseMtx, mtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        PSMTXIdentity(mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(0.0f, y1, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(wide, y1, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(wide, x0, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(0.0f, x0, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
#endif
    }
}
