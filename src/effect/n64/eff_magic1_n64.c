#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

static u8 anime_data[12] = {0x00, 0xFF, 0xD0, 0x1A, 0x78, 0x4D, 0xD0, 0xFE, 0xF0, 0xFE, 0x4C, 0x6E};

void effMagic1Main(EffectEntry* effect);
void effMagic1Disp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effMagic1N64Entry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 tx, f32 ty, f32 tz) {
    EffectEntry* effect;
    EffMagic1N64Work* w;

    effect = effEntry();
    effect->type = "Magic1N64";
    effect->count = 1;
    w = __memAlloc(HEAP_EFFECT, sizeof(EffMagic1N64Work));
    effect->userdata = w;
    effect->callback = effMagic1Main;
    effect->flags |= 2;
    w->age = 0;
    w->type = type;
    w->alpha = 255;
    w->position.x = x;
    w->position.y = y;
    w->position.z = z;
    w->scale = 1.0f;
    switch (type) {
    case 0: {
        f32 inv;
        inv = 1.0f / (f32)life;
        w->offset.x = 0.0f;
        w->offset.y = 0.0f;
        w->offset.z = 0.0f;
        w->velocity.x = inv * (tx - x);
        w->velocity.y = inv * (ty - y);
        w->velocity.z = inv * (tz - z);
        w->life = life;
        break;
    }
    default:
        w->offset.x = 29.0f;
        w->offset.y = 0.0f;
        w->offset.z = 0.0f;
        w->velocity.x = 6.2f;
        w->velocity.y = 0.0f;
        w->velocity.z = 0.0f;
        w->life = 24;
        break;
    }
    return effect;
}

void effMagic1Main(EffectEntry* effect) {
    EffMagic1N64Work* work = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 type;

    tmp.x = work->offset.x;
    tmp.y = work->offset.y;
    tmp.z = work->offset.z;
    position = tmp;
    if ((u32)marioStGetSystemLevel() != 0) {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMagic1Disp, effect);
        return;
    }
    if (effect->flags & 4) {
        effect->flags &= ~4;
        work->life = 0;
    }
    work->life--;
    work->age++;
    if (work->life < 0) {
        effDelete(effect);
        return;
    }
    type = work->type;
    if (type == 0 && work->life == 0) {
        EffectEntry* e = effMagic1N64Entry(1, 0x30, work->position.x + work->offset.x, work->position.y + work->offset.y,
                                           work->position.z + work->offset.z, 0.0f, 0.0f, 0.0f);
        ((EffMagic1N64Work*)e->userdata)->scale = work->scale;
    }
    if (type == 1) {
        work->velocity.x *= 0.83f;
        work->alpha *= 0.9f;
    }
    work->offset.x += work->velocity.x;
    work->offset.y += work->velocity.y;
    work->offset.z += work->velocity.z;
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMagic1Disp, effect);
}

static void main_dl(EffectEntry* effect, Mtx base) {
    u8* a;
    f32 yr;

    EffMagic1N64Work* w = effect->userdata;
    s32 type = w->type;
    f32 scale = w->scale;
    f32 ang;
    f32 radius;
    s32 life = w->life;
    s32 alpha = w->alpha;
    f32 ox = w->offset.x;
    f32 oy = w->offset.y;
    f32 oz = w->offset.z;
    f32 c;
    f32 s;
    f32 t;
    s32 i;
    s32 j;
    s16 sz;
    f32 yaw;
    GXTexObj texObj;
    Mtx mf8;
    Mtx mc8;
    Mtx m98;
    Mtx m68;
    Mtx m38;

    switch (type) {
        case 0:
            radius = 9.0f;
            ang = (f32)(life * 35);
            yaw = -camGetPtr(CAMERA_3D)->viewYaw;
            break;
        default:
            ox *= scale;
            radius = 6.0f;
            yaw = 0.0f;
            ang = (f32)(life * 25);
            break;
    }
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    effGetTexObjN64(0x10, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    for (i = 0; i < 3; i++) {
        if (i > 0) {
            switch (type) {
                case 0:
                ang -= 70.0f;
                ox -= 2.0f * w->velocity.x;
                oy -= 2.0f * w->velocity.y;
                oz -= 2.0f * w->velocity.z;
                break;
                default:
                ang -= 50.0f;
                ox -= 2.0f * (1.2048193f * w->velocity.x);
                break;
            }
            alpha = (s32)(100.0f * (f32)w->alpha / 255.0f);
        }
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
        PSMTXTrans(mf8, ox, oy, oz);
        PSMTXScale(m98, scale, scale, scale);
        PSMTXRotRad(mc8, 'y', yaw * 0.017453292f);
        PSMTXConcat(mc8, mf8, mf8);
        PSMTXConcat(mf8, m98, m68);

        for (j = 0, a = anime_data; j < 3; j++) {
            t = 6.2831855f * ang / 360.0f;
            c = (f32)cos(t);
            s = (f32)sin(t);
            PSMTXTrans(m38, radius * s, radius * c, 0.0f);
            ang += 120.0f;
            PSMTXConcat(m68, m38, m38);
            PSMTXConcat(base, m38, m38);
            GXLoadPosMtxImm(m38, GX_PNMTX0);
            GXSetTevColor(GX_TEVREG0, (GXColor){(u8)(s32)effMagic1Blend(a[1], 0, (f32)(s32)(180 + (gp->retraceCount << 5) + a[0])),
                                                (u8)(s32)effMagic1Blend(a[2], 0, (f32)(s32)(180 + (gp->retraceCount << 5) + a[0])),
                                                (u8)(s32)effMagic1Blend(a[3], 0, (f32)(s32)(180 + (gp->retraceCount << 5) + a[0])), alpha});
            GXSetTevColor(GX_TEVREG1, (GXColor){(u8)(s32)effMagic1Blend(a[1], 255, (f32)(s32)((gp->retraceCount << 5) + a[0])),
                                                (u8)(s32)effMagic1Blend(a[2], 255, (f32)(s32)((gp->retraceCount << 5) + a[0])),
                                                (u8)(s32)effMagic1Blend(a[3], 255, (f32)(s32)((gp->retraceCount << 5) + a[0])), 255});
            if (type == 0) sz = 15; else sz = 10;
            PSMTXScale(m98, 2.0f, 0.25f, 1.0f);
            PSMTXTrans(mf8, 0.0f, (f32)(3 - j) * 64.0f / 256.0f, 0.0f);
            PSMTXConcat(mf8, m98, mf8);
            GXLoadTexMtxImm(mf8, GX_TEXMTX0, GX_MTX2x4);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3s16(-sz, sz, 0);
            GXTexCoord2f32(0.0f, 0.0f);
            GXPosition3s16(sz, sz, 0);
            GXTexCoord2f32(1.0f, 0.0f);
            GXPosition3s16(sz, -sz, 0);
            GXTexCoord2f32(1.0f, 1.0f);
            GXPosition3s16(-sz, -sz, 0);
            GXTexCoord2f32(0.0f, 1.0f);
            a += 4;
        }
    }
}

void effMagic1Disp(CameraId cameraId, EffectEntry* effect) {
    CameraEntry* camera;
    EffMagic1N64Work* w;
    f32 yaw;
    s32 type;
    s32 i;
    Mtx m98;
    Mtx m68;
    Mtx m38;
    Mtx m8;

    camera = camGetPtr(cameraId);
    w = effect->userdata;
    type = w->type;
    switch (type) {
        case 0:
            yaw = 0.0f;
            break;
        default:
            yaw = -camGetPtr(CAMERA_3D)->viewYaw;
            break;
    }
    PSMTXTrans(m98, w->position.x, w->position.y, w->position.z);
    PSMTXRotRad(m68, 'y', yaw * 0.017453292f);
    PSMTXConcat(m98, m68, m38);
    PSMTXConcat(camera->view, m38, m38);
    switch (type) {
        case 0:
            main_dl(effect, m38);
            break;
        case 1:
            for (i = 0; i < 12; i++) {
                PSMTXRotRad(m8, 'z', 0.017453292f * (f32)(i * 30));
                PSMTXConcat(m38, m8, m8);
                main_dl(effect, m8);
            }
            break;
    }
}
