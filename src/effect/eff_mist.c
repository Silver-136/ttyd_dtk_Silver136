#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "driver/mapdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"

typedef struct EffMistWork {
    s32 type;      // 0x0
    Vec position;  // 0x4
    Vec scale;     // 0x10
    Vec velocity;  // 0x1C
    Vec accel;     // 0x28
    f32 alpha;     // 0x34
    s32 timer;     // 0x38
    s32 life;      // 0x3C
    f32 yaw;       // 0x40
    u8 r, g, b;    // 0x44
    u8 pad;
} EffMistWork;

void effMistMain(EffectEntry* effect);
void effMistDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effMistEntry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 yaw) {
    EffectEntry* effect;
    EffMistWork* work;
    EffMistWork* p;
    s32 i;
    s32 count;
    f32 t;

    effect = effEntry();
    count = type == 4 ? 2 : 11;
    effect->type = "Mist";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, count * sizeof(EffMistWork));
    effect->userdata = work;
    effect->callback = effMistMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    if (life <= 0) {
        work->life = 1000;
    } else {
        work->life = life;
    }
    work->timer = 0;
    work->alpha = 255.0f;
    work->yaw = yaw - camGetPtr(CAMERA_3D)->viewYaw;
    for (i = 1, p = work + 1; i < effect->count; i++, p++) {
        p->position.x = irand(50) - 25;
        p->position.y = irand(50) - 25;
        p->position.z = 0.0f;
        p->scale.x = p->scale.y = p->scale.z = 0.8f + irand(12) / 10.0f;
        p->velocity.x = (irand(10) - 5) / 20.0f;
        p->velocity.y = (irand(10) - 5) / 20.0f;
        p->velocity.z = 0.0f;
        p->alpha = 0.0f;
        p->life = irand(30) + 30;
        p->timer = 0;
        p->yaw = yaw - camGetPtr(CAMERA_3D)->viewYaw;
        switch (type) {
            case 0:
                p->r = 255;
                p->g = 255;
                p->b = 255;
                break;
            case 1:
                p->r = 11;
                p->g = 9;
                p->b = 96;
                break;
            case 2:
                p->r = 225;
                p->g = 153;
                p->b = 175;
                break;
            case 3:
                p->r = 73;
                p->g = 101;
                p->b = 0;
                break;
            case 4:
                p->position.x = 0.0f;
                p->position.y = 0.0f;
                p->position.z = 1.0f;
                t = sin(6.2831855f * p->yaw / 360.0f);
                p->velocity.x = 1.1f * t;
                p->velocity.y = -2.0f;
                t = cos(6.2831855f * p->yaw / 360.0f);
                p->velocity.z = 1.1f * -t;
                p->accel.x = 0.95f;
                p->accel.y = -0.04f;
                p->accel.z = 0.94f;
                p->r = 255;
                p->g = 255;
                p->b = 255;
                p->scale.z = p->scale.y = p->scale.x = 0.3f;
                p->alpha = 200.0f;
                break;
            case 5:
                p->r = 70;
                p->g = 0;
                p->b = 50;
                break;
            case 6:
                p->r = 0;
                p->g = 0;
                p->b = 0;
                break;
        }
    }
    return effect;
}

void effMistMain(EffectEntry* effect) {
    EffMistWork* work = effect->userdata;
    Vec position;
    Vec v = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 type;

    v.x = work->position.x;
    v.y = work->position.y;
    v.z = work->position.z;
    position = v;
    type = work->type;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->life = 32;
    }
    if (work->life < 1000) {
        work->life--;
    }
    work->timer++;
    if (work->life < 0) {
        effDelete(effect);
        return;
    }
    if (work->life < 32) {
        work->alpha = work->life * 8;
    }
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        switch (type) {
            case 4:
                work->position.x += work->velocity.x;
                work->position.y += work->velocity.y;
                work->position.z += work->velocity.z;
                work->velocity.x *= 0.9f;
                work->velocity.y *= 0.88f;
                work->velocity.z *= 0.9f;
                work->position.y += work->accel.x;
                work->accel.x += work->accel.y;
                work->accel.y *= work->accel.z;
                work->scale.x *= 1.02f;
                work->scale.y *= 1.02f;
                work->scale.z *= 1.02f;
                work->alpha -= 2.0f;
                if (work->alpha < 0.0f) {
                    work->alpha = 0.0f;
                }
                break;
            default:
                if (work->timer < 255) {
                    work->timer += irand(5);
                    work->alpha = work->timer;
                    if (work->alpha > 255.0f) {
                        work->alpha = 255.0f;
                    }
                } else {
                    work->alpha -= irand(5);
                    if (work->alpha < 0.0f) {
                        work->timer = 0;
                        work->alpha = 0.0f;
                    }
                }
                work->position.x += work->velocity.x;
                work->position.y += work->velocity.y;
                work->position.z += work->velocity.z;
        }
    }
    dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effMistDisp, effect);
}

void effMistDisp(CameraId cameraId, EffectEntry* effect) {
    EffMistWork* work = effect->userdata;
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx mtx;
    GXTexObj texObj;
    s32 i;
    CameraEntry* cam;
    s32 alpha;
    f32 w = 32.0f;
    f32 h = 16.0f;

    cam = camGetPtr(cameraId);
    alpha = work->alpha;
    if (work->type == 4 && (mapGetWork()->entries[0].flags & MAP_ENTRY_BLEND)) {
        return;
    }
    effGetTexObj(0x44, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_RASA, GX_CA_TEXA, GX_CA_ZERO);
    PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
    PSMTXRotRad(rot, 'y', MTXDegToRad(-camGetPtr(cameraId)->viewYaw));
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        PSMTXTrans(trans, work->position.x, work->position.y, work->position.z);
        PSMTXScale(scale, work->scale.x, work->scale.y, work->scale.z);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(mtx, trans, trans);
        GXLoadPosMtxImm(trans, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetChanMatColor(GX_COLOR0A0, (GXColor){work->r, work->g, work->b, work->alpha * alpha / 255.0f});
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-w / 2.0f, h, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(h, h, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(h, -w / 2.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-w / 2.0f, -w / 2.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}
