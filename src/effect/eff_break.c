#include "eff/eff_break.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/n64/eff_stardust_n64.h"

typedef struct EffBreakWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    s32 timer;     // 0x10
    f32 offsetY;   // 0x14
    s32 field_0x18; // 0x18, only set to 0
    s32 state;     // 0x1C
    f32 scale;     // 0x20
} EffBreakWork;

#ifdef VERSION_G8ME01
static s16 tex_jp[] = { 145, 146, 147, 148, 149, 150 };
static s16 tex_us[] = { 155, 156, 157, 158, 159, 160 };
static s16 tex_lang3[] = { 151, 152, 153, 154, 155, 156 };
static s16 tex_lang4[] = { 157, 158, 159, 160, 161, 162 };
static s16 tex_lang5[] = { 159, 160, 161, 162, 163, 164, 165 };
static s16 tex_lang6[] = { 145, 146, 147, 148, 149, 150 };
typedef struct TexIdEntry {
    s16* ids;
    s32 count;
} TexIdEntry;
static TexIdEntry texid_tbl[] = {
    { tex_jp, 6 }, { tex_us, 6 }, { tex_lang3, 6 }, { tex_lang4, 6 }, { tex_lang5, 7 }, { tex_lang6, 6 },
};
#else
static s32 tex_jp[] = { 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2 };
static s32 tex_us[] = { 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6 };
static s32* texid_tbl[] = { tex_jp, tex_us };
#endif

// texture offset (t) per frame: ty_data slides a letter in (3 -> 0, then a small bounce), ty_data2 slides it
// out (0 -> 3, negated for every second letter)
static f32 ty_data[] = {
    3.0f, 3.0f, 2.7857144f, 2.5714285f, 2.357143f, 2.142857f, 1.9285715f,
    1.7142857f, 1.5f, 1.2857143f, 1.0714285f, 0.85714287f, 0.64285713f, 0.42857143f,
    0.21428572f, 0.0f, 0.083333336f, 0.16666667f, 0.25f, 0.125f, 0.0f,
};
static f32 ty_data2[] = {
    0.0f, 0.15f, 0.3f, 0.45f, 0.6f, 0.75f, 0.9f,
    1.05f, 1.2f, 1.35f, 1.5f, 1.65f, 1.8f, 1.95f,
    2.1f, 2.25f, 2.4f, 2.55f, 2.7f, 2.85f, 3.0f,
};

EffectEntry* effBreakEntry(s32 type, s32 duration, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    EffBreakWork* work;
    EffBreakWork* w;
    s32 count;
    s32 i;

#ifdef VERSION_G8ME01
    GXTexObj texObj;
    f32 a;
    f32 b;
    s32 j;

    effect = effEntry();
    count = texid_tbl[gp->language].count + 1;
    effect->type = "Break";
#else
    effect = effEntry();
    // FAKE: dead language test (both languages use 7 entries); plain `count = 7;` changes the object
    count = 7;
    if ((u32)gp->language != LANGUAGE_JP) {
        count = 7;
    }
    effect->type = "Break";
#endif
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffBreakWork) * count);
    effect->userdata = work;
    effect->callback = effBreakMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->timer = duration;
    work->scale = 2.0f;
    w = work + 1;
    for (i = 1; i < effect->count; i++, w++) {
#ifdef VERSION_G8ME01
        // a: centre of this letter (letters 72 apart), b: half the word width; the texture objects fetched in
        // these loops are not used
        a = 0.0f;
        b = a;
        for (j = 0; j < i - 1; j++) {
            effGetTexObj(texid_tbl[gp->language].ids[j], &texObj);
            a += 72.0f;
        }
        effGetTexObj(texid_tbl[gp->language].ids[j], &texObj);
        a += 36.0f;
        for (j = 0; j < effect->count - 1; j++) {
            effGetTexObj(texid_tbl[gp->language].ids[j], &texObj);
            b += 36.0f;
        }
        w->position.x = a - b;
#else
        w->position.x = 2.0f * (s32)((i - 1) * 36 + 18 - (((effect->count - 2) * 36 + 24) / 2));
#endif
        w->position.y = 0.0f;
        w->position.z = 0.0f;
        w->offsetY = 10.0f;
        w->field_0x18 = 0;
        w->timer = (i - 1) * 21;
        w->state = 0;
    }
    return effect;
}

void effBreakMain(EffectEntry* effect) {
    EffBreakWork* work = effect->userdata;
    Vec position;
    Vec tmp = { 0.0f, 0.0f, 0.0f };
    s32 limit;
    s32 i;
    s32 done = 0;
    f32 scale;

    tmp.x = work->position.x;
    tmp.y = work->position.y;
    tmp.z = work->position.z;
    position = tmp;
    scale = work->scale;
    limit = work->timer;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        switch (work->state) {
            case 0:
                work->timer--;
                if (work->timer <= 0) {
                    work->timer = 0;
                    work->state++;
                }
                break;
            case 1:
                if ((u32)work->timer < 21) {
                    work->offsetY = ty_data[work->timer];
                    work->timer++;
                    if (work->offsetY == 0.0f) {
                        effStardustN64SetDrawCam(effStardustN64Entry(1, position.x + work->position.x,
                                                                      position.y + work->position.y,
                                                                      position.z + work->position.z, scale * 20.0f),
                                                 8);
                    }
                } else {
                    work->timer = (effect->count - 1 - i) * 21;
                    work->state++;
                }
                break;
            case 2:
                work->timer--;
                if (work->timer <= 0) {
                    work->timer = 0;
                    work->state++;
                }
                break;
            case 3:
                work->timer++;
                if (work->timer > limit) {
                    work->timer = 0;
                    work->state++;
                }
                break;
            case 4:
                if ((u32)work->timer < 21) {
                    work->offsetY = ty_data2[work->timer];
                    if ((i & 1) == 0) {
                        work->offsetY = -work->offsetY;
                    }
                    work->timer++;
                } else {
                    work->state++;
                }
                break;
            case 5:
                done++;
                break;
        }
    }
    if (done >= effect->count - 1) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_2D, dispCalcZ(&position), 2, (DispCallback)effBreakDisp, effect);
    }
}

void effBreakDisp(CameraId cameraId, EffectEntry* effect) {
    EffBreakWork* work = effect->userdata;
    CameraEntry* camera = camGetPtr(cameraId);
    f32 scale = work->scale;
    f32 width;
    f32 x0;
    f32 y1;
    GXTexObj texObj;
    Mtx mtx;
    Mtx rot;
    Mtx scaleMtx;
    Mtx base;
    s32 i;

    PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat(camera->view, mtx, base);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    width = 48.0f;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
#ifdef VERSION_G8ME01
        effGetTexObj(texid_tbl[gp->language].ids[i - 1], &texObj);
#else
        effGetTexObj(texid_tbl[gp->language != LANGUAGE_JP][i - 1], &texObj);
#endif
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        PSMTXTrans(mtx, 0.0f, work->offsetY, 0.0f);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX2x4);
        PSMTXTrans(mtx, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scaleMtx, scale, scale, scale);
        PSMTXConcat(base, mtx, mtx);
        PSMTXConcat(mtx, scaleMtx, mtx);
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        x0 = -width / 2.0f;
        y1 = width / 2.0f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(x0, y1, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(y1, y1, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(y1, x0, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(x0, x0, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}
