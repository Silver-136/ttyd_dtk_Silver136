#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <string.h>
#include "eff/eff_funemizu.h"
#include "eff/n64/eff_water_damage_n64.h"
#include "eff/eff_kemu_test.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

void effSplashMain(EffectEntry* effect);
void effSplashDisp(CameraId cameraId, EffectEntry* effect);
void effSplashGetCol(CameraId cameraId, EffectEntry* effect);

typedef struct EffSplashWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    f32 f10;       // 0x10
    f32 f14;       // 0x14
    f32 sx;        // 0x18
    f32 sy;        // 0x1C
    f32 sz;        // 0x20
    s32 timer;     // 0x24
    s32 delay;     // 0x28
    f32 vy;        // 0x2C
    f32 grav;      // 0x30
    s32 pad34;     // 0x34
    s32 hasColor;  // 0x38
    GXColor color; // 0x3C
} EffSplashWork;

static s16 geso_shibuki_vertex_tbl[] __attribute__((aligned(32))) = { -27648, 0, 0, -21503, 0, 0, -15871, 0, 0, -10751, 0, 0, -5631, 0, 0, 0, 0, 0, -28426, 333, 0, -23324, 3291, 0, -18243, 5029, 0, -13562, 6464, 0, -8405, 6754, 0, -1797, 4829, 0, -28349, 2048, 0, -23897, 4665, 0, -19006, 6861, 0, -14421, 8524, 0, -9359, 9043, 0, -3897, 6702, 0};
static s16 geso_shibuki_texcoord0_tbl[] __attribute__((aligned(32))) = { -11004, 6276, -3325, 6276, -3325, 18299, -11004, 18299, 4352, 6276, 4352, 18299, 12031, 6276, 12031, 18299, 19709, 6276, 19709, 18299, 27388, 6276, 27388, 18299, -11004, 512, -3325, 512, 4352, 512, 12031, 512, 19709, 512, 27388, 0 };
static u8 geso_scale_dt[] = { 0x60, 0x60, 0x72, 0x88, 0x9F, 0xB6, 0xCC, 0xDF, 0xEE, 0xF7, 0xF9, 0xF7, 0xF5, 0xF3, 0xF1, 0xEE, 0xEB, 0xE8, 0xE4, 0xE1, 0xDD, 0xD9, 0xD5, 0xD0, 0xCC, 0xC7, 0xC2, 0xBD, 0xB8, 0xB3, 0xAE, 0xA8, 0xA3, 0x9D, 0x97, 0x91, 0x8C, 0x86, 0x80, 0x7A, 0x74, 0x6D, 0x67, 0x61, 0x5B, 0x55, 0x4F, 0x49, 0x43, 0x3D, 0x37, 0x31, 0x2B, 0x25, 0x20, 0x1A, 0x15, 0x0F, 0x0A, 0x05, 0x00};
static s8 geso_trans_dt[] = { -40, -12, -40, -12, -39, -9, -39, -6, -39, -3, -39, 0, -38, 3, -38, 5, -38, 8, -39, 10, -40, 12, -41, 13, -42, 13, -43, 13, -45, 13, -47, 13, -49, 13, -51, 12, -52, 12, -54, 12, -56, 12, -57, 12, -58, 12, -59, 12, -61, 12, -62, 12, -63, 12, -65, 12, -66, 12, -67, 12, -69, 12, -70, 12, -71, 12, -73, 12, -74, 12, -75, 12, -76, 12, -78, 12, -79, 12, -80, 12, -81, 12, -83, 12, -84, 12, -85, 12, -87, 12, -88, 12, -89, 12, -90, 12, -92, 12, -93, 12, -94, 12, -95, 12, -97, 12, -98, 12, -99, 12, -100, 12, -102, 12, -103, 12, -104, 12, -106, 12, -107, 12, -108, 12, -109, 12, -111, 12, -112, 12, -113, 12, -114, 12, -116, 12, -117, 12, -118, 12, -120, 12};
static s8 geso_shibuki_normal_tbl[] __attribute__((aligned(32))) = { 0, 0, 64};
static u8 geso_shibuki_color0_tbl[] __attribute__((aligned(32))) = { 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static u8 geso_shibuki_dl_0_0[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x0C, 0x00, 0x01, 0x0C, 0x0D, 0x00, 0x01, 0x0D, 0x06, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 geso_shibuki_dl_0_1[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x0D, 0x00, 0x01, 0x0D, 0x0E, 0x00, 0x01, 0x0E, 0x07, 0x00, 0x01, 0x01, 0x08, 0x00, 0x01, 0x04, 0x01, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 geso_shibuki_dl_0_2[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x0E, 0x00, 0x01, 0x0E, 0x0F, 0x00, 0x01, 0x0F, 0x08, 0x00, 0x01, 0x04, 0x09, 0x00, 0x01, 0x06, 0x02, 0x00, 0x00, 0x05, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 geso_shibuki_dl_0_3[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x0F, 0x00, 0x01, 0x0F, 0x10, 0x00, 0x01, 0x10, 0x09, 0x00, 0x01, 0x06, 0x0A, 0x00, 0x01, 0x08, 0x03, 0x00, 0x00, 0x07, 0x04, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 geso_shibuki_dl_0_4[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x10, 0x00, 0x01, 0x10, 0x11, 0x00, 0x01, 0x11, 0x0A, 0x00, 0x01, 0x08, 0x0B, 0x00, 0x01, 0x0A, 0x04, 0x00, 0x00, 0x09, 0x05, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8* geso_shibuki_dl_0_tbl[] = { geso_shibuki_dl_0_0, geso_shibuki_dl_0_1, geso_shibuki_dl_0_2, geso_shibuki_dl_0_3, geso_shibuki_dl_0_4 };
static u8 geso_shibuki_dl_0_size_tbl[] = { 1, 1, 1, 1, 1};

EffectEntry* effSplashEntry(s32 type, f32 x, f32 y, f32 z, f32 size) {
    EffectEntry* effect;
    EffSplashWork* work;
    s32 i;
    s32 n1;
    s32 n2;
    f32 ang;
    f32 r;
    f32 s;
    f32 c;

    effect = effEntry();
    n1 = 0;
    n2 = 0;
    switch (type) {
    case 0:
    case 1:
        n1 = 2;
        n2 = 10;
        break;
    case 2:
        n1 = 1;
        n2 = 10;
        break;
    case 3:
        size *= 0.5f;
        n1 = 6;
        n2 = 0;
        effWaterDamageN64Entry(2, 60, x, y, z, 1.0f);
        break;
    case 4:
        n1 = 2;
        n2 = 10;
        break;
    }
    effect->type = "splash";
    effect->count = n1 + n2 + 1;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(EffSplashWork));
    effect->userdata = work;
    effect->callback = effSplashMain;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    work->sz = size;
    work->sy = size;
    work->sx = size;
    work->timer = 0;
    work->hasColor = 0;
    work++;
    for (i = 1; i < n1 + 1; i++, work++) {
        work->position.z = 0.0f;
        work->position.y = 0.0f;
        work->position.x = 0.0f;
        work->f10 = 0.0f;
        work->f14 = 0.0f;
        work->delay = 0;
        work->sz = 1.0f;
        work->sy = 1.0f;
        work->sx = 1.0f;
        switch (type) {
        case 0:
            if (i == 1) {
                work->vy = 5.0f;
                work->grav = 0.4f;
                work->sx = work->sy = work->sz = 0.7f;
            } else {
                work->vy = 4.0f;
                work->grav = 0.4f;
            }
            break;
        case 1:
            if (i == 1) {
                work->vy = 7.0f;
                work->grav = 0.4f;
            } else {
                work->vy = 4.0f;
                work->grav = 0.4f;
            }
            break;
        case 2:
            work->vy = 2.0f;
            work->grav = 0.1f;
            break;
        case 3:
            work->vy = 0.0f;
            work->grav = 0.0f;
            switch (i - 1) {
            case 0:
            case 1:
                work->position.z = 0.0f;
                break;
            case 2:
            case 3:
                work->position.z = 10.0f;
                break;
            case 4:
            case 5:
                work->position.z = 20.0f;
                break;
            }
            break;
        case 4:
            if (i == 1) {
                work->vy = 4.0f;
                work->grav = 0.48f;
            } else {
                work->vy = 3.6f;
                work->grav = 0.8f;
            }
            work->sz = 0.01f;
            break;
        }
    }
    for (i = n1 + 1; i < effect->count; i++, work++) {
        ang = (f32)irand(360);
        r = (f32)irand(5);
        ang = 6.2831855f * ang / 360.0f;
        s = sin(ang);
        work->f10 = r * s;
        c = cos(ang);
        work->f14 = r * c;
        work->position.x = 2.0f * work->f10;
        work->position.y = 0.0f;
        work->position.z = 2.0f * work->f14;
        work->vy = 10.0f;
        work->grav = 1.0f;
        work->delay = irand(10);
        work->sz = 0.5f;
        work->sy = 0.5f;
        work->sx = 0.5f;
    }
    if (type == 2) {
        r = 6.2831855f * camGetPtr(CAMERA_3D)->viewYaw / 360.0f;
        s = (f32)sin(r);
        c = (f32)cos(r);
        { f32 dx = size * (-30.0f * s); f32 dz = size * (30.0f * c);
        effKemuTestEntry(6, x + dx, y + 5.0f * size, z + dz, size); }
    }
    return effect;
}

void effSplashMain(EffectEntry* effect) {
    EffSplashWork* work = effect->userdata;
    Vec position;
    Vec v = {0.0f, 0.0f, 0.0f};
    s32 type;
    s32 i;
    u32 t;
    s32 maxT;

    v.x = work->position.x;
    v.y = work->position.y;
    v.z = work->position.z;
    position = v;
    type = work->type;
    if (work->hasColor == 0) {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 8, (DispCallback)effSplashGetCol, effect);
        return;
    }
    switch (type) {
    case 0:
    case 1:
        maxT = 30;
        break;
    case 2:
    case 4:
        maxT = 60;
        break;
    case 3:
        maxT = 71;
        break;
    }
    t = work->timer;
    work->timer = t + 1;
    if (work->timer > maxT) {
        effDelete(effect);
        return;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay) {
            work->delay--;
            continue;
        }
        work->position.x += work->f10;
        work->position.z += work->f14;
        work->position.y += work->vy;
        work->vy -= work->grav;
        switch (type) {
        case 1:
            if (i == 1 && work->vy < 0.0f) {
                work->sx *= 0.95f;
                work->sy *= 0.95f;
                work->sz *= 0.95f;
            }
            break;
        case 3:
            if (i < 7) {
                if (t < 61) {
                    work->sy = (f32)geso_scale_dt[t] / 96.0f;
                }
                if (t < 71) {

                    work->position.x = 10.0f * ((f32)(geso_trans_dt[t * 2] + 40) / 8.0f);
                    work->position.y = 10.0f * ((f32)(geso_trans_dt[t * 2 + 1] - 12) / 8.0f);
                }
                switch (i - 1) {
                case 0:
                case 1:
                    work->position.x *= 0.3f;
                    work->position.y *= 0.5f;
                    work->sy *= 0.6f;
                    break;
                case 2:
                case 3:
                    work->position.x *= 0.9f;
                    break;
                case 4:
                case 5:
                    work->position.x *= 1.3f;
                    work->position.y *= 0.3f;
                    work->sy *= 0.6f;
                    break;
                }
                switch (i - 1) {
                case 1:
                case 3:
                case 5:
                    work->sx = -1.0f;
                    work->position.x *= -1.0f;
                    break;
                }
            }
            break;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 1, (DispCallback)effSplashDisp, effect);
}

void effSplashGetCol(CameraId cameraId, EffectEntry* effect) {
    EffSplashWork* work = effect->userdata;
    CameraEntry* cam;
    f32 proj[7];
    f32 vp[6];
    f32 sx, sy, sz;
    union { u32 word; struct { u8 a, r, g, b; } color; } pixel;
    GXColor c;

    cam = camGetPtr(cameraId);
    GXSetProjection(cam->projection, cam->projType);
    GXGetProjectionv(proj);
    GXGetViewportv(vp);
    GXProject(work->position.x, work->position.y, work->position.z, cam->view, proj, vp, &sx, &sy, &sz);
    if (sx > 0.0f && sx < 608.0f && sy > 0.0f && sy < 480.0f) {
        sysWaitDrawSync();
        GXPeekARGB((u16)sx, (u16)sy, &pixel.word);
        work->color.r = pixel.color.r;
        work->color.g = pixel.color.g;
        work->color.b = pixel.color.b;
        work->color.a = 255;
    }
    work->hasColor = 1;
    c = effFunemizuGetMapColor();
    work->color.r = c.r;
    work->color.g = c.g;
    work->color.b = c.b;
}

void effSplashDisp(CameraId cameraId, EffectEntry* effect) {
    EffSplashWork* work = effect->userdata;
    Mtx trans;
    Mtx scale;
    Mtx rot;
    Mtx mtx;
    GXTexObj texObj;
    GXColor color;

    s32 i;
    s32 k;
    s32 type;
    s32 n;
    CameraEntry* cam;
    f32 a, b, c;
    f32 r0, g0, b0, r1, g1, b1;
    f32 ang, sn, cs;
    f32 w;

    u32 k2;
    u8* sz;
    u8** dl;
    s32 kb;


    cam = camGetPtr(cameraId);
    type = work->type;
    color = work->color;
    n = 0;
    switch (type) {
    case 0:
    case 1:
    case 4:
        n = 2;
        break;
    case 2:
        n = 1;
        break;
    case 3:
        n = 6;
        break;
    }
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(CAMERA_3D)->viewYaw));
    PSMTXScale(scale, work->sx, work->sy, work->sz);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
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
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    work++;
    for (i = 1; i < n + 1; i++, work++) {
        if (work->delay != 0) {
            continue;
        }
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->sx, work->sy, work->sz);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        switch (type) {
        case 0:
        case 1:
        case 2:
        case 4:
            effGetTexObj(0x41, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
        case 3:
            effGetTexObj(0x43, &texObj);
            GXLoadTexObj(&texObj, GX_TEXMAP0);
            break;
        }
        switch (type) {
        case 0:
        case 1:
        case 4:
            PSMTXTrans(trans, (f32)gp->retraceCount / 20.0f, 0.0f, 0.0f);
            break;
        case 2:
            PSMTXTrans(trans, (f32)gp->retraceCount / 40.0f, 0.0f, 0.0f);
            break;
        case 3:
            PSMTXTrans(trans, (f32)gp->retraceCount / 10.0f, 0.0f, 0.0f);
            break;
        }
        GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
        switch (type) {
        case 0:
        case 2:
            a = 23.0f;
            b = 15.0f;
            c = -37.0f;
            break;
        case 1:
            if (i == 1) {
                a = 1.8f;
                b = 6.0f;
                c = -85.0f;
            } else {
                a = b = 11.6f;
                c = -21.0f;
            }
            break;
        case 4:
            if (i == 1) {
                a = b = 10.0f;
                c = -20.0f;
            } else {
                a = 15.0f;
                b = 12.0f;
                c = -10.0f;
            }
            break;
        }
        switch (type) {
        case 0:
        case 1:
        case 4:
            b0 = g0 = r0 = 1.0f;
            r1 = color.r / 255.0f;
            g1 = color.g / 255.0f;
            b1 = color.b / 255.0f;
            break;
        case 2:
            r0 = 1.0f;
            g0 = 0.869f;
            b0 = 0.0f;
            r1 = 1.0f;
            g1 = 0.314f;
            b1 = 0.0f;
            break;
        }
        switch (type) {
        case 0:
        case 1:
        case 2:
        case 4:
            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 18);
            for (k = 0; k < 9; k++) {
                ang = 6.2831855f * (f32)k / 8.0f;
                cs = (f32)cos(ang);
                sn = (f32)sin(ang);
                kb = k & 1;
                GXPosition3f32(a * sn, 0.0f, a * cs);
                GXColor4u8((u8)(255.0f * r0), (u8)(255.0f * g0), (u8)(255.0f * b0), 255);
                GXTexCoord2f32((f32)kb, 0.1f);
                cs = (f32)cos(ang);
                sn = (f32)sin(ang);
                GXPosition3f32(b * sn, c, b * cs);
                GXColor4u8((u8)(255.0f * r1), (u8)(255.0f * g1), (u8)(255.0f * b1), 255);
                GXTexCoord2f32((f32)kb, 2.1f);
            }
            break;
        case 3:
            *(GXColor*)geso_shibuki_color0_tbl = color;
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
            GXSetArray(GX_VA_POS, geso_shibuki_vertex_tbl, 6);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
            GXSetArray(GX_VA_NRM, geso_shibuki_normal_tbl, 3);
            GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            GXSetArray(GX_VA_CLR0, geso_shibuki_color0_tbl, 4);
            GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 14);
            GXSetArray(GX_VA_TEX0, geso_shibuki_texcoord0_tbl, 4);
            {
                
                for (k2 = 0, sz = geso_shibuki_dl_0_size_tbl, dl = geso_shibuki_dl_0_tbl; k2 < 5; k2++, sz++, dl++) {
                    GXCallDisplayList(*dl, *sz << 5);
                }
            }
            break;
        }
    }
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    w = 8.0f;

    for (i = n + 1; i < effect->count; i++, work++) {
        if (work->delay != 0) {
            continue;
        }
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(CAMERA_3D)->viewYaw));
        PSMTXScale(scale, work->sx, work->sy, work->sz);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        effGetTexObj(0x40, &texObj);
        GXLoadTexObj(&texObj, GX_TEXMAP0);
        PSMTXIdentity(trans);
        GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
        switch (type) {
            case 2:
                r0 = 1.0f;
                g0 = 0.869f;
                b0 = 0.0f;
                break;
            default:
                r0 = (1.0f + color.r / 255.0f) / 2;
                g0 = (1.0f + color.g / 255.0f) / 2;
                b0 = (1.0f + color.b / 255.0f) / 2;
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-w / 2, (w / 2), 0.0f);
        GXColor4u8((u8)(255.0f * r0), (u8)(255.0f * g0), (u8)(255.0f * b0), 255);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32((w / 2), (w / 2), 0.0f);
        GXColor4u8((u8)(255.0f * r0), (u8)(255.0f * g0), (u8)(255.0f * b0), 255);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32((w / 2), -w / 2, 0.0f);
        GXColor4u8((u8)(255.0f * r0), (u8)(255.0f * g0), (u8)(255.0f * b0), 255);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w / 2, -w / 2, 0.0f);
        GXColor4u8((u8)(255.0f * r0), (u8)(255.0f * g0), (u8)(255.0f * b0), 255);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}



