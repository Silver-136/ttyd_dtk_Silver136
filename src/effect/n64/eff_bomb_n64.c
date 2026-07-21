#include "effect/n64/eff_bomb_n64.h"

void effBombN64SetCamId(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x40) = camId;
}

void* effBombN64Entry(s32 type, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effBombMain(void);
    extern char str_BombN64_802fabd0[];
    extern f32 float_0_80424d58;
    extern f32 float_1_80424d5c;
    extern f32 float_3_80424d78;
    extern f32 float_9p9_80424d7c;
    extern f32 float_1p6_80424d80;
    extern f32 float_13p9_80424d84;
    extern f32 float_2p4_80424d88;
    extern f32 float_17p8_80424d8c;
    extern f32 float_3p2_80424d90;
    extern f32 float_30_80424d94;
    void* entry;
    u8* work;
    s32 big;

    big = 0;
    if (type >= 3) {
        big = 1;
        type -= 3;
    }

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_BombN64_802fabd0;
    *(s32*)((s32)entry + 8) = 3;
    work = __memAlloc(3, 0xCC);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBombMain;

    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x30) = 0x3C;
    *(s32*)work = type;
    *(s32*)(work + 0x3C) = big;
    *(s32*)(work + 0x40) = 4;
    *(s32*)(work + 0x44) = (type != 0) ? type : -1;
    *(s32*)(work + 0x7C) = 0;
    if (type == 1) {
        *(f32*)(work + 0x64) = float_1_80424d5c;
        *(f32*)(work + 0x68) = float_1_80424d5c;
    } else {
        *(f32*)(work + 0x64) = float_3_80424d78;
        *(f32*)(work + 0x68) = float_3_80424d78;
    }
    *(s32*)(work + 0x88) = type;
    *(s32*)(work + 0xC0) = 0xFF;

    switch (type) {
        case 0:
            *(f32*)(work + 0xA8) = float_9p9_80424d7c;
            *(f32*)(work + 0xAC) = float_1p6_80424d80;
            break;
        case 1:
            *(f32*)(work + 0xA8) = float_13p9_80424d84;
            *(f32*)(work + 0xAC) = float_2p4_80424d88;
            break;
        case 2:
            *(f32*)(work + 0xA8) = float_17p8_80424d8c;
            *(f32*)(work + 0xAC) = float_3p2_80424d90;
            break;
    }

    *(f32*)(work + 0xB0) = float_0_80424d58;
    *(f32*)(work + 0xB4) = float_30_80424d94;
    return entry;
}
void effBombMain(void* effEntry) {
    extern void effDelete(void* effect);
    extern void* effKemuri8N64Entry(s32 type, f32 x, f32 y, f32 z);
    extern void effKemuri8N64SetCamId(void* effect, s32 camId);
    extern void* effConfettiN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 scale);
    extern void effConfettiN64SetCamId(void* effect, s32 camId);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 camId, s32 layer, void* callback, void* param, f32 z);
    extern void effBombDisp(void);
    extern f32 float_1_80424d5c;
    extern f32 float_0p4_80424d60;
    extern f32 float_0p6_80424d64;
    extern f32 float_255_80424d68;
    extern f32 float_0p9_80424d6c;
    extern f32 float_0p1_80424d70;
    extern f32 float_0p5_80424d74;
    f32 pos[3];
    void* spawned;
    u8* work;
    s32 frame;
    s32 type;
    s32 camId;

    work = *(u8**)((s32)effEntry + 0xC);
    pos[0] = *(f32*)(work + 4);
    pos[1] = *(f32*)(work + 8);
    pos[2] = *(f32*)(work + 0xC);

    camId = *(s32*)(work + 0x40);
    *(s32*)(work + 0x30) = *(s32*)(work + 0x30) - 1;
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effEntry);
        return;
    }

    *(s32*)(work + 0x34) = *(s32*)(work + 0x34) + 1;
    frame = *(s32*)(work + 0x34);
    type = *(s32*)work;

    if (frame == 7) {
        if (*(s32*)(work + 0x3C) == 0) {
            spawned = effKemuri8N64Entry(type, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
        } else {
            spawned = effKemuri8N64Entry(type + 3, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
        }
        effKemuri8N64SetCamId(spawned, camId);
    } else if (frame == 1) {
        spawned = effConfettiN64Entry(type + 4, 0x32, *(f32*)(work + 4), *(f32*)(work + 8),
                                      *(f32*)(work + 0xC), float_1_80424d5c);
        effConfettiN64SetCamId(spawned, camId);
    }

    *(f32*)(work + 0x64) += *(f32*)(work + 0x68);
    if (frame < 8) {
        if (type == 2) {
            *(f32*)(work + 0x68) += float_1_80424d5c;
        } else {
            *(f32*)(work + 0x68) += float_0p4_80424d60;
        }
        *(s32*)(work + 0x7C) =
            (s32)((float_255_80424d68 - (f32)*(s32*)(work + 0x7C)) * float_0p6_80424d64);
    } else {
        *(f32*)(work + 0x68) *= float_0p6_80424d64;
        *(s32*)(work + 0x7C) = (s32)((f32)*(s32*)(work + 0x7C) * float_0p9_80424d6c);
    }

    if (frame >= 3) {
        if (frame == 3) {
            *(f32*)(work + 0xA8) = float_1_80424d5c;
        }
        *(f32*)(work + 0xA8) += *(f32*)(work + 0xAC);
    }

    if (frame >= 3) {
        if (frame < 6) {
            *(f32*)(work + 0xAC) += float_0p1_80424d70;
        } else {
            *(f32*)(work + 0xAC) *= float_0p5_80424d74;
            *(s32*)(work + 0xC0) = (s32)((f32)*(s32*)(work + 0xC0) * float_0p9_80424d6c);
        }
    }

    *(f32*)(work + 0xB0) += *(f32*)(work + 0xB4);
    dispEntry(camId, 2, effBombDisp, effEntry, dispCalcZ(pos));
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effBombDisp(s32 cameraId, void* effEntry) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
    extern f32 float_deg2rad_80424d50;
    extern f32 float_0p0625_80424d54;
    extern f32 float_0_80424d58;
    extern f32 float_1_80424d5c;
    extern u32 dat_80424d48;
    extern u32 dat_80424d4c;
    extern u8 size8x8_tex32x32_vtx[];
    void* cam;
    void* cam3d;
    u8* work;
    s32 frame;
    s32 texFrame;
    s32 alpha;
    s32 subAlpha;
    u32 color;
    u8 texObj[0x20];
    f32 baseMtx[3][4];
    f32 rotMtx[3][4];
    f32 viewMtx[3][4];
    f32 scaleMtx[3][4];

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effEntry + 0xC);
    frame = *(s32*)(work + 0x34);

    PSMTXTrans(baseMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80424d50 * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXConcat(baseMtx, rotMtx, viewMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), viewMtx, viewMtx);

    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 2);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(baseMtx, float_0p0625_80424d54, float_0p0625_80424d54, float_0_80424d58);
    GXLoadTexMtxImm(baseMtx, 0x1E, 1);
    GXSetCullMode(0);
    effSetVtxDescN64(size8x8_tex32x32_vtx);

    if (*(s32*)(work + 0x44) != -1) {
        effGetTexObjN64(0x18, texObj);
        GXLoadTexObj(texObj, 0);
        color = dat_80424d48;
        ((u8*)&color)[3] = *(u8*)(work + 0x7C);
        GXSetTevColor(1, &color);
        PSMTXScale(scaleMtx, *(f32*)(work + 0x64), *(f32*)(work + 0x64), float_1_80424d5c);
        PSMTXConcat(viewMtx, scaleMtx, scaleMtx);
        GXLoadPosMtxImm(scaleMtx, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }

    if (frame == 1) {
        alpha = 0xC0;
    } else if (frame == 2) {
        alpha = 0xFF;
    } else {
        alpha = *(s32*)(work + 0xC0);
    }

    if (frame < 3) {
        subAlpha = 0xFF;
    } else {
        subAlpha = 0x7F;
        if (frame < 8) {
            subAlpha = frame << 4;
        }
        subAlpha += 0x80;
    }

    color = dat_80424d4c;
    ((u8*)&color)[2] = (u8)subAlpha;
    ((u8*)&color)[3] = (u8)alpha;
    GXSetTevColor(1, &color);
    PSMTXScale(scaleMtx, *(f32*)(work + 0xA8), *(f32*)(work + 0xA8), *(f32*)(work + 0xA8));
    PSMTXRotRad(rotMtx, float_deg2rad_80424d50 * *(f32*)(work + 0xB0), 'z');
    PSMTXConcat(rotMtx, scaleMtx, scaleMtx);
    PSMTXConcat(viewMtx, scaleMtx, scaleMtx);
    GXLoadPosMtxImm(scaleMtx, 0);
    GXSetCurrentMtx(0);

    texFrame = frame % 3;
    if (texFrame == 0) {
        effGetTexObjN64(0x19, texObj);
        GXLoadTexObj(texObj, 0);
    } else if (texFrame == 1) {
        effGetTexObjN64(0x1A, texObj);
        GXLoadTexObj(texObj, 0);
    } else if (texFrame == 2) {
        effGetTexObjN64(0x1B, texObj);
        GXLoadTexObj(texObj, 0);
    }
    GXBegin(0x90, 0, 6);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
