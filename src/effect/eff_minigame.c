#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef struct MiniGameWork {
    s32 type;   // 0x0
    f32 x;      // 0x4
    f32 y;      // 0x8
    f32 z;      // 0xC
    f32 scaleX; // 0x10
    f32 scaleY; // 0x14
    f32 rotA;   // 0x18
    f32 rotB;   // 0x1C
    f32 vel;    // 0x20
    f32 acc;    // 0x24
    s32 delay;  // 0x28
    s32 pad2c;  // 0x2C
    s32 state;  // 0x30
    s32 alpha;  // 0x34
} MiniGameWork;

void effMiniGameMain(EffectEntry* effect);
void effMiniGameDisp(CameraId cameraId, EffectEntry* effect);

#ifdef VERSION_G8ME01
typedef struct MiniGameTbl {
    u8* tbl;
    s32 len;
} MiniGameTbl;

u8 start_jp_tbl[] = {0x9E, 0xA5, 0x9F, 0xA6, 0xA0, 0xA7, 0xA1, 0xA8, 0xA2, 0xA9, 0xA3, 0xAA, 0xA4, 0xAB, 0xA2, 0xA9};
u8 start_us_tbl[] = {0xA8, 0xAF, 0xA9, 0xB0, 0xAA, 0xB1, 0xAB, 0xB2, 0xAC, 0xB3, 0xAD, 0xB4, 0xAE, 0xB5, 0xAC, 0xB3};
u8 start_lang3_tbl[] = {0xA1, 0xA8, 0xA2, 0xA9, 0xA3, 0xAA, 0xA4, 0xAB, 0xA5, 0xAC, 0xA6, 0xAD, 0xA7, 0xAE, 0xA5, 0xAC};
u8 start_lang4_tbl[] = {0xA9, 0xB2, 0xAA, 0xB3, 0xAB, 0xB4, 0xAC, 0xB5, 0xAD, 0xB6, 0xAE, 0xB7, 0xAF, 0xB8, 0xB0, 0xB9, 0xB1, 0xBA};
u8 start_lang5_tbl[] = {0xAE, 0xB9, 0xAF, 0xBA, 0xB0, 0xBB, 0xB1, 0xBC, 0xB2, 0xBD, 0xB3, 0xBE, 0xB4, 0xBF, 0xB5, 0xC0, 0xB2, 0xBD, 0xB6, 0xC1, 0xB7, 0xC2, 0xB4, 0xBF, 0xB8, 0xC3};
u8 start_lang6_tbl[] = {0x9D, 0xA7, 0x9E, 0xA8, 0x9F, 0xA9, 0xA0, 0xAA, 0xA1, 0xAB, 0xA2, 0xAC, 0xA3, 0xAD, 0xA4, 0xAE, 0xA5, 0xAF, 0xA6, 0xB0, 0xA1, 0xAB};
u8 finish_jp_tbl[] = {0xAC, 0xB1, 0xAD, 0xB2, 0xAE, 0xB3, 0xAD, 0xB2, 0xAF, 0xB4, 0xB0, 0xB5};
u8 finish_us_tbl[] = {0xB6, 0xBB, 0xB7, 0xBC, 0xB8, 0xBD, 0xB7, 0xBC, 0xB9, 0xBE, 0xBA, 0xBF};
u8 finish_lang3_tbl[] = {0xAF, 0xB3, 0xB0, 0xB4, 0xB1, 0xB5, 0xB2, 0xB6};
u8 finish_lang4_tbl[] = {0xBB, 0xC0, 0xBC, 0xC1, 0xBC, 0xC1, 0xBD, 0xC2, 0xBE, 0xC3, 0xBF, 0xC4, 0xBF, 0xC4};
u8 finish_lang5_tbl[] = {0xC4, 0xC8, 0xC5, 0xC9, 0xC6, 0xCA, 0xC7, 0xCB};
u8 finish_lang6_tbl[] = {0xB1, 0xB6, 0xB2, 0xB7, 0xB2, 0xB7, 0xB3, 0xB8, 0xB4, 0xB9, 0xB5, 0xBA};
u8 retire_jp_tbl[] = {0xB6, 0xBA, 0xB7, 0xBB, 0xB8, 0xBC, 0xB9, 0xBD, 0xB6, 0xBA, 0xB7, 0xBB};
u8 retire_us_tbl[] = {0xC0, 0xC4, 0xC1, 0xC5, 0xC2, 0xC6, 0xC3, 0xC7, 0xC0, 0xC4, 0xC1, 0xC5};
u8 retire_lang3_tbl[] = {0xB7, 0xBC, 0xB8, 0xBD, 0xB9, 0xBE, 0xBA, 0xBF, 0xBB, 0xC0};
u8 retire_lang4_tbl[] = {0xC5, 0xCA, 0xC6, 0xCB, 0xC7, 0xCC, 0xC8, 0xCD, 0xC9, 0xCE};
u8 retire_lang5_tbl[] = {0xCC, 0xD1, 0xCD, 0xD2, 0xCC, 0xD1, 0xCE, 0xD3, 0xCF, 0xD4, 0xD0, 0xD5, 0xCE, 0xD3, 0xD0, 0xD5};
u8 retire_lang6_tbl[] = {0xBB, 0xC0, 0xBC, 0xC1, 0xBD, 0xC2, 0xBE, 0xC3, 0xBF, 0xC4};
MiniGameTbl start_tbl[] = {{start_jp_tbl, 0x10}, {start_us_tbl, 0x10}, {start_lang3_tbl, 0x10}, {start_lang4_tbl, 0x12}, {start_lang5_tbl, 0x1a}, {start_lang6_tbl, 0x16}};
MiniGameTbl finish_tbl[] = {{finish_jp_tbl, 0xc}, {finish_us_tbl, 0xc}, {finish_lang3_tbl, 0x8}, {finish_lang4_tbl, 0xe}, {finish_lang5_tbl, 0x8}, {finish_lang6_tbl, 0xc}};
MiniGameTbl retire_tbl[] = {{retire_jp_tbl, 0xc}, {retire_us_tbl, 0xc}, {retire_lang3_tbl, 0xa}, {retire_lang4_tbl, 0xa}, {retire_lang5_tbl, 0x10}, {retire_lang6_tbl, 0xa}};
MiniGameTbl* type_tbl[] = {start_tbl, finish_tbl, retire_tbl};
#else
u8 start_tbl[16] = "hifgdez{}~jkwx}~";
u8 finish_tbl[12] = "norsuvrs|{pq";
u8 retire_tbl[] = {0x79, 0x78, 0x6C, 0x6D, 0x7F, 0x7E, 0x74, 0x73, 0x79, 0x78, 0x6C, 0x6D};
u8 start_type[] = {0x00, 0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x14};
u8 finish_type[] = {0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x00, 0x00};
u8 retire_type[] = {0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x00, 0x00};
u8* type_tbl[] = {start_type, finish_type, retire_type};
#endif
u8 poyon_scale[] = {0x64, 0x64, 0x69, 0x5E, 0x6E, 0x59, 0x75, 0x52, 0x7B, 0x4C, 0x81, 0x46, 0x86, 0x41, 0x8A, 0x3D,
                    0x8C, 0x3B, 0x8C, 0x3C, 0x85, 0x42, 0x77, 0x50, 0x67, 0x60, 0x58, 0x6F, 0x50, 0x78, 0x4E, 0x79,
                    0x52, 0x75, 0x58, 0x6F, 0x5E, 0x69, 0x64, 0x64};

EffectEntry* effMiniGameEntry(s32 type, f32 x, f32 y, f32 z) {
    EffectEntry* effect;
    MiniGameWork* work;
    s32 i;
    s32 n;
    s32 st;

    effect = effEntry();
#ifdef VERSION_G8ME01
    n = type_tbl[type][gp->language].len / 2 + 1;
#else
    n = 1;
    switch (type) {
    case 0:
        n = 9;
        break;
    case 1:
        n = 7;
        break;
    case 2:
        n = 7;
        break;
    }
#endif
    effect->type = "miniGame";
    effect->count = n;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(MiniGameWork));
    effect->userdata = work;
    effect->callback = (EffectCallback)effMiniGameMain;
    effect->flags |= 2;
    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scaleY = 1.0f;
    work->scaleX = 1.0f;
    work->rotB = 0.0f;
    work->delay = 0;
    work->pad2c = 0;
    for (i = 1, work = work + 1; i < effect->count; i++, work++) {
#ifdef VERSION_G8ME01
        if (type == 0) {
            work->state = (i < 4) ? 0 : 20;
        } else if (type == 1) {
            work->state = 10;
        } else if (type == 2) {
            work->state = 40;
        }
#else
        work->state = type_tbl[type][i - 1];
#endif
        work->rotB = 0.0f;
        work->alpha = 255;
        st = work->state;
        switch (st) {
        case 0:
            work->x = 0.0f;
            work->delay = (i - 1) * gp->framerate;
            work->y = 0.0f;
            work->z = 0.0f;
            work->scaleY = 0.0f;
            work->scaleX = 0.0f;
            work->rotA = 0.0f;
            work->pad2c = 0;
            work->alpha = 0;
            break;
        case 30:
        case 40:
            if (st == 40) {
                work->x = 26.0f * (f32)(i - 1) - 26.0f * (f32)(effect->count - 1) / 2.0f;
                work->delay = (i - 1) * 10;
                work->state = 50;
            } else {
                work->x = 0.0f;
                st = i - 1;
                work->delay = st * gp->framerate;
            }
            work->y = 500.0f;
            work->z = 0.0f;
            work->scaleY = 1.0f;
            work->scaleX = 1.0f;
            work->rotA = 0.0f;
            work->pad2c = 0;
            work->vel = 0.0f;
            work->acc = 1.0f;
            break;
        case 10:
        case 20:
            work->x = 1000.0f;
            work->y = 0.0f;
            work->z = 0.0f;
            work->scaleY = 1.0f;
            work->scaleX = 1.0f;
            work->rotA = 0.0f;
            if (work->state == 10) {
                work->delay = (i - 1) * 10;
            } else {
                work->delay = gp->framerate * 3;
                work->scaleY = 0.0f;
                work->scaleX = 0.0f;
                work->x = 26.0f * (f32)(i - 3) - 26.0f * (f32)(effect->count - 3) / 2.0f;
                work->alpha = 0;
            }
            work->pad2c = 0;
            work->vel = 0.0f;
            work->acc = 1.0f;
            break;
        }
    }
    return effect;
}

void effMiniGameMain(EffectEntry* effect) {
    MiniGameWork* work = effect->userdata;
    Vec pos;
    Vec tmp;
    s32 i;
    s32 n;

    tmp = (Vec){0.0f, 0.0f, 0.0f};
    tmp.x = work->x;
    tmp.y = work->y;
    tmp.z = work->z;
    pos = tmp;
    n = 0;
    for (i = 1, work++; i < effect->count; i++, work++) {
        switch (work->state) {
        case 0:
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
                psndSFXOn((const char*)0x8D1);
            }
            break;
        case 1:
            work->scaleX += (1.0f - work->scaleX) / 4.0f;
            work->scaleY += (1.0f - work->scaleY) / 4.0f;
            work->alpha += (s32)((f32)(255 - work->alpha) / 4.0f);
            if (work->alpha > 250) {
                work->alpha = 255;
                work->state++;
            }
            break;
        case 2:
            work->delay = (s32)(0.7f * (f32)gp->framerate);
            work->state++;
            break;
        case 3:
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
            }
            break;
        case 4:
            work->scaleX += (10.0f - work->scaleX) / 4.0f;
            work->scaleY += (10.0f - work->scaleY) / 4.0f;
            work->alpha += (s32)((f32)(-work->alpha) / 4.0f);
            if (work->alpha < 5) {
                work->alpha = 0;
                work->state++;
            }
            break;
        case 5:
            n++;
            break;
        case 10:
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
            }
            break;
        case 11:
            work->x += work->vel;
            work->vel -= work->acc;
            if (work->x < 26.0f * (f32)(i - 1) - 26.0f * (f32)(effect->count - 1) / 2.0f) {
                work->x = 26.0f * (f32)(i - 1) - 26.0f * (f32)(effect->count - 1) / 2.0f;
                work->delay = 0;
                work->state++;
            }
            break;
        case 12:
            if ((u32)work->delay < 20) {
                work->scaleX = (f32)poyon_scale[work->delay * 2 + 1] / 100.0f;
                work->scaleY = (f32)poyon_scale[work->delay * 2] / 100.0f;
                work->delay++;
            } else {
                work->acc = 2.0f;
                work->vel = 0.0f;
                work->delay = 120;
                work->state++;
            }
            break;
        case 13:
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
            }
            break;
        case 14:
            work->x += work->vel;
            work->vel -= work->acc;
            if (work->x < -1000.0f) {
                work->state++;
            }
            break;
        case 15:
            n++;
            break;
        case 20:
            if (work->delay == 0) {
                psndSFXOn((const char*)0x8D2);
            }
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
            }
            break;
        case 21:
            work->scaleX += (1.0f - work->scaleX) / 4.0f;
            work->scaleY += (1.0f - work->scaleY) / 4.0f;
            work->alpha += (s32)((f32)(255 - work->alpha) / 4.0f);
            if (work->alpha > 250) {
                work->alpha = 255;
                work->state++;
            }
            break;
        case 22:
            work->delay = 60;
            work->state++;
            break;
        case 23:
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
            }
            break;
        case 24:
            work->x += work->vel;
            work->vel -= work->acc;
            if (work->x < -1000.0f) {
                work->state++;
            }
            break;
        case 25:
            n++;
            break;
        case 30:
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
            }
            break;
        case 31:
            work->y += work->vel;
            work->vel -= work->acc;
            if (work->y < 0.0f) {
                work->y = 0.0f;
                work->delay = 0;
                work->state++;
            }
            break;
        case 32:
            if ((u32)work->delay < 20) {
                work->scaleX = (f32)poyon_scale[work->delay * 2] / 100.0f;
                work->scaleY = (f32)poyon_scale[work->delay * 2 + 1] / 100.0f;
                work->delay++;
            } else {
                work->acc = 1.0f;
                work->vel = 0.0f;
                work->delay = (s32)(0.7f * (f32)gp->framerate);
                work->state++;
            }
            break;
        case 33:
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
            }
            break;
        case 34:
            work->x -= 10.0f;
            work->y += 10.0f;
            work->rotB += 30.0f;
            if (work->y > 300.0f) {
                work->state++;
            }
            break;
        case 35:
            n++;
            break;
        case 50:
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
            }
            break;
        case 51:
            work->y += work->vel;
            work->vel -= work->acc;
            if (work->y < 0.0f) {
                work->y = 0.0f;
                work->delay = 0;
                work->state++;
            }
            break;
        case 52:
            if ((u32)work->delay < 20) {
                work->scaleX = (f32)poyon_scale[work->delay * 2] / 100.0f;
                work->scaleY = (f32)poyon_scale[work->delay * 2 + 1] / 100.0f;
                work->delay++;
            } else {
                work->acc = 1.0f;
                work->vel = 0.0f;
                work->delay = (s32)(0.7f * (f32)gp->framerate);
                work->state++;
            }
            break;
        case 53:
            work->delay--;
            if (work->delay < 0) {
                work->delay = 0;
                work->state++;
            }
            break;
        case 54:
            work->y += work->vel;
            work->vel -= work->acc;
            work->rotA += 3.0f;
            if (work->y < -500.0f) {
                work->state++;
            }
            break;
        case 55:
            n++;
            break;
        }
    }
    if (n >= effect->count - 1) {
        effDelete(effect);
    } else if (effect->flags & 4) {
        effect->flags &= ~4;
        effDelete(effect);
    } else {
        dispEntry((CameraId)8, dispCalcZ(&pos), 2, (DispCallback)effMiniGameDisp, effect);
    }
}

void effMiniGameDisp(CameraId cameraId, EffectEntry* effect) {
    MiniGameWork* work = effect->userdata;
    CameraEntry* cam;
    MiniGameWork* p;
    s32 i;
    s32 s;
    s32 type;
    s32 texId;
    f32 xoff;
    f32 yoff;
    f32 size;
    f32 half;
    u8* t;
    Mtx mc8;
    Mtx m98;
    Mtx m68;
    Mtx m38;
    GXTexObj tex;
    f32 rad;

    cam = camGetPtr(cameraId);
    type = work->type;
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    PSMTXTrans(mc8, work->x, work->y, work->z);
    PSMTXRotRad(m68, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    PSMTXScale(m98, work->scaleX, work->scaleY, 1.0f);
    PSMTXConcat(mc8, m68, m38);
    PSMTXConcat(m38, m98, m38);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
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
    size = 40.0f;
    rad = 0.017453292f;
    half = 20.0f;
    for (s = 0; s < 2; s++) {
        p = effect->userdata;
        for (i = 1, p++; i < effect->count; i++, p++) {
            PSMTXTrans(mc8, 0.0f, 20.0f, 0.0f);
            PSMTXRotRad(m68, 'z', rad * p->rotB);
            PSMTXConcat(mc8, m68, m68);
            PSMTXTrans(mc8, 0.0f, -20.0f, 0.0f);
            PSMTXConcat(m68, mc8, m68);
            PSMTXTrans(mc8, p->x, p->y, p->z);
            PSMTXConcat(mc8, m68, mc8);
            PSMTXScale(m98, p->scaleX, p->scaleY, 1.0f);
            PSMTXRotRad(m68, 'z', rad * p->rotA);
            PSMTXConcat(mc8, m68, mc8);
            PSMTXConcat(mc8, m98, mc8);
            PSMTXConcat(m38, mc8, mc8);
            if (s == 0) {
                PSMTXTrans(m98, 3.0f, -3.0f, 0.0f);
                PSMTXConcat(mc8, m98, mc8);
            }
            PSMTXConcat(cam->view, mc8, mc8);
            GXLoadPosMtxImm(mc8, GX_PNMTX0);
            GXSetCurrentMtx(GX_PNMTX0);
#ifdef VERSION_G8ME01
            texId = type_tbl[type][gp->language].tbl[(i - 1) * 2 + (1 - s)];
#else
            switch (type) {
            case 0:
                texId = start_tbl[(i - 1) * 2 + (1 - s)];
                break;
            case 1:
                texId = finish_tbl[(i - 1) * 2 + (1 - s)];
                break;
            case 2:
                texId = retire_tbl[(i - 1) * 2 + (1 - s)];
                break;
            }
#endif
            effGetTexObj(texId, &tex);
            if (s != 0 && p->scaleX <= 1.0f) {
                GXInitTexObjLOD(&tex, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
            }
            GXLoadTexObj(&tex, GX_TEXMAP0);
            if (s == 0) {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){0, 0, 0, p->alpha * 140 / 255});
            } else {
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){255, 255, 255, p->alpha * 255 / 255});
            }
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            xoff = 0.0f;
            yoff = 0.0f;
            if (type == 0) {
                yoff = -size / 2.0f;
            } else if (type == 1) {
                yoff = -size / 2.0f;
                xoff = half;
            }
            GXPosition3f32(-size / 2.0f + xoff, size + yoff, 0.0f);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3f32(half + xoff, size + yoff, 0.0f);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3f32(half + xoff, yoff, 0.0f);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3f32(-size / 2.0f + xoff, yoff, 0.0f);
            GXTexCoord2f32(0.0f, 1.0f);
        }
    }
}

