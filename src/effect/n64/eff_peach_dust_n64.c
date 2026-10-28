#include "eff/eff_n64.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "memory.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <stdlib.h>
#include "libc_decls.h"
#include "eff/eff_magic1_n64.h"
#include "eff/n64/eff_peach_dust_n64.h"

typedef struct EffPeachDustParticle {
    s32 pad0;     // 0x0
    Vec position; // 0x4
    Vec velocity; // 0x10
    s32 lifetime; // 0x1C
    s32 frame;    // 0x20
    f32 scale;    // 0x24
    s32 tex;      // 0x28
} EffPeachDustParticle;

typedef struct EffPeachDustWork {
    s32 type;     // 0x0
    Vec position; // 0x4
    f32 unk10;    // 0x10
    f32 unk14;    // 0x14
    f32 unk18;    // 0x18
    s32 lifetime; // 0x1C
    s32 frame;    // 0x20
    f32 unk24;    // 0x24
    s32 unk28;    // 0x28
    EffPeachDustParticle particles[100];
} EffPeachDustWork;

char data[768] = ".......+..............@.#...........$%&.*=..........-...;>.........,'....)!.......~~......{.......~~......]@@^....~/........(_......#:<.....+.......[}|.....+.......1.......23......45.....677......8......7.......90......7.......a22b...c...........deee............f++++..........ff....+........ff......+........+......+.......+.......ff......+........+...f++.........+..ff...........+f..+............+.ff............+..+............+..f+.f+++f.....+....+f..+......+.......+.......+.......+.......+........+++++++f.....gh............+g.ggg..........g...hh..........g....g..........hh..hh.hh.......g..hg+ghgg......hghg.g...g.......hggg.....+.....g+.h......g....g..........g....g..........g.....g.........g.....g.........hgg...g...........g...g.........hg.....gggggggggh...";

static effN64Vtx size8x8_tex22x22_vtx[] = {
    {-8, -8, 0, 0, 0, 0, 0, 0, 255},
    {8, -8, 0, 704, 0, 0, 0, 0, 255},
    {8, 8, 0, 704, 704, 0, 0, 0, 255},
    {-8, 8, 0, 0, 704, 0, 0, 0, 255},
    {-8, -8, 0, 704, 0, 0, 0, 0, 255},
    {8, -8, 0, 1408, 0, 0, 0, 0, 255},
    {8, 8, 0, 1408, 704, 0, 0, 0, 255},
    {-8, 8, 0, 704, 704, 0, 0, 0, 255},
    {-8, -8, 0, 1408, 0, 0, 0, 0, 255},
    {8, -8, 0, 2112, 0, 0, 0, 0, 255},
    {8, 8, 0, 2112, 704, 0, 0, 0, 255},
    {-8, 8, 0, 1408, 704, 0, 0, 0, 255},
    {-8, -8, 0, 2112, 0, 0, 0, 0, 255},
    {8, -8, 0, 2816, 0, 0, 0, 0, 255},
    {8, 8, 0, 2816, 704, 0, 0, 0, 255},
    {-8, 8, 0, 2112, 704, 0, 0, 0, 255},
    {-8, -8, 0, 2816, 0, 0, 0, 0, 255},
    {8, -8, 0, 3520, 0, 0, 0, 0, 255},
    {8, 8, 0, 3520, 704, 0, 0, 0, 255},
    {-8, 8, 0, 2816, 704, 0, 0, 0, 255},
    {-8, -8, 0, 3520, 0, 0, 0, 0, 255},
    {8, -8, 0, 4224, 0, 0, 0, 0, 255},
    {8, 8, 0, 4224, 704, 0, 0, 0, 255},
    {-8, 8, 0, 3520, 704, 0, 0, 0, 255},
    {-8, -8, 0, 4224, 0, 0, 0, 0, 255},
    {8, -8, 0, 4928, 0, 0, 0, 0, 255},
    {8, 8, 0, 4928, 704, 0, 0, 0, 255},
    {-8, 8, 0, 4224, 704, 0, 0, 0, 255},
    {-8, -8, 0, 4928, 0, 0, 0, 0, 255},
    {8, -8, 0, 5632, 0, 0, 0, 0, 255},
    {8, 8, 0, 5632, 704, 0, 0, 0, 255},
    {-8, 8, 0, 4928, 704, 0, 0, 0, 255},
};

static u8 scale_data[] = {
    0x1E, 0x3C, 0x5A, 0x64, 0x68, 0x6A, 0x6C, 0x6E, 0x70, 0x71, 0x71, 0x6E, 0x6B,
    0x67, 0x64, 0x68, 0x6A, 0x6C, 0x6E, 0x70, 0x71, 0x71, 0x6E, 0x6B, 0x67,
};

static u8 color_rotation_data[] = {
    0xFE, 0xAC, 0xAC, 0xFE, 0xAC, 0xD5, 0xFE, 0xB4, 0x9A, 0xD5, 0xB4, 0xFE,
    0xB4, 0xB4, 0xFE, 0xB4, 0xDD, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5,
    0xB4, 0xFE, 0xB4, 0xD5, 0xFE, 0xB4, 0xFE, 0xFE, 0xB4, 0xFE, 0xD5, 0xAC,
};

void effPeachDustMain(EffectEntry* effect);
void effPeachDustDisp(CameraId cameraId, EffectEntry* effect);

EffectEntry* effPeachDustN64Entry(s32 type, f32 x, f32 y, f32 z, f32 unused, f32 rotY) {
    char* base;
    EffectEntry* effect;
    EffPeachDustParticle* p;
    s32 count;
    EffPeachDustWork* work;
    s32 j;
    s32 i;
    f32 ang;
    f32 r;

    effect = effEntry();
    effect->type = "PeachDustN64";
    effect->count = 101;
    work = __memAlloc(HEAP_EFFECT, sizeof(EffPeachDustWork));
    effect->userdata = work;
    effect->callback = effPeachDustMain;
    work->position.x = x;
    work->position.y = y;
    work->position.z = z;
    x = 6.2831855f * rotY;
    work->frame = 0;
    work->type = type;
    work->lifetime = 80;
    base = data + type * 256;
    p = work->particles;
    count = 1;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            // FAKE: (s32) cast on the char; without it the object changes
            if ((s32)base[(15 - i) * 16 + j] == '.') {
                continue;
            }
            r = 0.1f * (rand() % 20 - 10) + (f32)((j - 8) * 2);
            ang = x / 360.0f;
            p->position.x = r * (f32)cos(ang);
            p->position.y = 0.1f * (rand() % 20 - 10) + (f32)(i * 2);
            p->position.z = r * (f32)sin(ang);
            p->velocity.x = 0.04f * p->position.x;
            p->velocity.y = 0.04f * p->position.y;
            p->velocity.z = 0.001f * (rand() % 10);
            p->lifetime = rand() % 7 + 30;
            p->frame = 0;
            p++;
            count++;
            if (count >= 101) {
                break;
            }
        }
        if (count >= 101) {
            break;
        }
    }
    for (; count < 101; count++) {
        p->scale = 0.0f;
        p->velocity.z = 0.0f;
        p->velocity.y = 0.0f;
        p->velocity.x = 0.0f;
        p->position.z = 0.0f;
        p->position.y = 0.0f;
        p->position.x = 0.0f;
        p->lifetime = -1;
        p->frame = 0;
        p++;
    }
    return effect;
}

void effPeachDustMain(EffectEntry* effect) {
    EffPeachDustParticle* p = effect->userdata;
    Vec position;
    Vec tmp = {0.0f, 0.0f, 0.0f};
    s32 i;
    s32 dead = 0;
    s32 f;
    s32 g;

    tmp.x = p->position.x;
    tmp.y = p->position.y;
    tmp.z = p->position.z;
    position = tmp;
    p->lifetime--;
    p->frame++;
    f = p->frame;
    p++;
    for (i = 0; i < effect->count - 1; i++, p++) {
        p->tex = (f + i + i) & 7;
        if (p->lifetime < 0) {
            dead++;
        }
        p->lifetime--;
        if (p->lifetime > 30 || p->lifetime < 0) {
            p->tex = -1;
        } else {
            p->frame++;
            f = p->frame;
            g = f - 1;
            if (g < 25) {
                p->scale = 0.4f * (0.01f * scale_data[g]);
            } else {
                p->scale += 0.1f * (0.1f - p->scale);
            }
            p->position.x += p->velocity.x;
            p->position.y += p->velocity.y;
            p->velocity.y += p->velocity.z;
            // z moves with the x step; the third stored velocity component accelerates y (above).
            p->position.z += p->velocity.x;
        }
    }
    if (dead >= effect->count - 1) {
        effDelete(effect);
    } else {
        dispEntry(CAMERA_3D, dispCalcZ(&position), 2, (DispCallback)effPeachDustDisp, effect);
    }
}

void effPeachDustDisp(CameraId cameraId, EffectEntry* effect) {
    EffPeachDustParticle* p;
    s32 i;
    s32 tex;
    CameraEntry* camera;
    s32 off;
    effN64Vtx* vtx;
    GXTexObj texObj;
    Mtx tr;
    Mtx scaleMtx;
    Mtx rot;
    Mtx mtx;
    s32 r;
    s32 g;
    s32 b;

    camera = camGetPtr(cameraId);
    p = effect->userdata;
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    PSMTXScale(scaleMtx, 0.0056818184f, 0.045454547f, 0.0f);
    GXLoadTexMtxImm(scaleMtx, GX_TEXMTX0, GX_MTX2x4);
    effGetTexObjN64(0x20, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    off = (p->frame - 1) * 3;
    PSMTXTrans(tr, p->position.x, p->position.y, p->position.z);
    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(CAMERA_3D)->viewYaw);
    PSMTXConcat(tr, rot, mtx);
    PSMTXConcat(camera->view, mtx, mtx);
    p++;
    for (i = 0; i < effect->count - 1; i++, p++) {
        tex = p->tex;
        if (tex < 0) {
            continue;
        }
        PSMTXTrans(tr, p->position.x, p->position.y, p->position.z);
        tr[0][0] = p->scale;
        tr[1][1] = p->scale;
        tr[2][2] = p->scale;
        PSMTXConcat(mtx, tr, tr);
        GXLoadPosMtxImm(tr, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        if (off >= 36) {
            off = 0;
        }
        // FAKE: k and h hold the 1.4f/0.7f factors in a block of their own; the literal factors change the object
        {
            f32 k = 1.4f;
            f32 h = 0.7f;

            r = color_rotation_data[off++] * k;
            g = color_rotation_data[off++] * h;
            b = color_rotation_data[off++] * h;
        }
        if (r > 255) {
            r = 255;
        }
        if (g > 255) {
            g = 255;
        }
        if (b > 255) {
            b = 255;
        }
        GXSetTevColor(GX_TEVREG0, (GXColor){r, g, b, 255});
        switch (tex & 7) {
            case 0:
                vtx = &size8x8_tex22x22_vtx[12];
                break;
            case 1:
                vtx = &size8x8_tex22x22_vtx[16];
                break;
            case 2:
                vtx = &size8x8_tex22x22_vtx[20];
                break;
            case 3:
                vtx = &size8x8_tex22x22_vtx[24];
                break;
            case 4:
                vtx = &size8x8_tex22x22_vtx[28];
                break;
            case 5:
                vtx = &size8x8_tex22x22_vtx[0];
                break;
            case 6:
                vtx = &size8x8_tex22x22_vtx[4];
                break;
            case 7:
                vtx = &size8x8_tex22x22_vtx[8];
                break;
        }
        effSetVtxDescN64(vtx);
        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
