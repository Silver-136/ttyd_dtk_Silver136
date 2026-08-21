#include "effect/n64/eff_magic1_n64.h"

typedef f32 Mtx[3][4];

void* camGetPtr(s32 cameraId);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx mtx, s32 axis, f32 rad);
void PSMTXConcat(void* a, void* b, void* ab);

extern const f32 float_0_804259cc;
extern const f32 float_deg2rad_804259d0;
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMagic1N64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 targetX, f32 targetY, f32 targetZ) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effMagic1Main(void*);
    extern char str_Magic1N64_802fb430[];
    extern const f32 float_1_80425a04;
    extern const f32 float_29_80425a1c;
    extern const f32 float_6p2_80425a20;
    void* entry;
    u8* work;
    f32 invTime;
    f32 one;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Magic1N64_802fb430;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x38);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMagic1Main;
    *(s32*)entry |= 2;

    one = float_1_80425a04;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)work = type;
    *(s32*)(work + 0x34) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x28) = one;

    if (type == 0) {
        zero = float_0_804259cc;
        invTime = one / (f32)time;
        *(f32*)(work + 0x10) = zero;
        *(f32*)(work + 0x14) = zero;
        *(f32*)(work + 0x18) = zero;
        *(f32*)(work + 0x1C) = invTime * (targetX - x);
        *(f32*)(work + 0x20) = invTime * (targetY - y);
        *(f32*)(work + 0x24) = invTime * (targetZ - z);
        *(s32*)(work + 0x30) = time;
    } else {
        zero = float_0_804259cc;
        *(f32*)(work + 0x10) = float_29_80425a1c;
        *(f32*)(work + 0x14) = zero;
        *(f32*)(work + 0x18) = zero;
        *(f32*)(work + 0x1C) = float_6p2_80425a20;
        *(f32*)(work + 0x20) = zero;
        *(f32*)(work + 0x24) = zero;
        *(s32*)(work + 0x30) = 0x18;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagic1Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern s32 marioStGetSystemLevel(void);
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effMagic1Disp(s32, void*);
    extern const Vec3 vec3_802fb418;
    extern const f32 float_0p83_80425a14;
    extern const f32 float_0p9_80425a18;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 type;
    u32 systemLevel;
    void* spawned;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb418;
    pos.x = *(f32*)(work + 0x10);
    pos.y = *(f32*)(work + 0x14);
    pos.z = *(f32*)(work + 0x18);
    dispPos = pos;

    systemLevel = (u32)marioStGetSystemLevel();
    if (systemLevel != 0U) {
        dispEntry(4, 2, effMagic1Disp, effect, dispCalcZ(&dispPos));
        return;
    }

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x30) = 0;
    }
    *(s32*)(work + 0x30) -= 1;
    *(s32*)(work + 0x2C) += 1;
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effect);
        return;
    }

    type = *(s32*)work;
    if (type == 0 && *(s32*)(work + 0x30) == 0) {
        spawned = effMagic1N64Entry(1, 0x30,
                                    *(f32*)(work + 4) + *(f32*)(work + 0x10),
                                    *(f32*)(work + 8) + *(f32*)(work + 0x14),
                                    *(f32*)(work + 0xC) + *(f32*)(work + 0x18),
                                    float_0_804259cc, float_0_804259cc, float_0_804259cc);
        *(f32*)((s32)*(void**)((s32)spawned + 0xC) + 0x28) = *(f32*)(work + 0x28);
    }

    if (type == 1) {
        *(f32*)(work + 0x1C) *= float_0p83_80425a14;
        *(s32*)(work + 0x34) = (s32)((f32)*(s32*)(work + 0x34) * float_0p9_80425a18);
    }

    *(f32*)(work + 0x10) += *(f32*)(work + 0x1C);
    *(f32*)(work + 0x14) += *(f32*)(work + 0x20);
    *(f32*)(work + 0x18) += *(f32*)(work + 0x24);
    dispEntry(4, 2, effMagic1Disp, effect, dispCalcZ(&dispPos));
}
static u8 anime_data[12] = {
    0x00, 0xFF, 0xD0, 0x1A,
    0x78, 0x4D, 0xD0, 0xFE,
    0xF0, 0xFE, 0x4C, 0x6E,
};

#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


static void main_dl(void* effect, f32 mtx[3][4]) {
    typedef f32 Mtx[3][4];
    typedef struct GXColor {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } GXColor;

    extern f64 sin(f64);
    extern f64 cos(f64);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetTevColor(s32, GXColor);
    extern void GXBegin(s32, s32, s32);

    extern u8* gp;
    extern GXColor unk_80429750;

    extern const u32 dat_804259c8;
    extern const f32 float_0_804259cc;
    extern const f32 float_deg2rad_804259d0;
    extern const f32 float_9_804259d4;
    extern const f32 float_6_804259d8;
    extern const f32 float_70_804259dc;
    extern const f32 float_2_804259e0;
    extern const f32 float_50_804259e4;
    extern const f32 float_1p2048_804259e8;
    extern const f32 float_100_804259ec;
    extern const f32 float_255_804259f0;
    extern const f32 float_6p2832_804259f4;
    extern const f32 float_360_804259f8;
    extern const f32 float_120_804259fc;
    extern const f32 float_0p5_80425a00;
    extern const f32 float_1_80425a04;
    extern const f32 float_0p25_80425a08;
    extern const f32 float_64_80425a0c;
    extern const f32 float_0p0039062_80425a10;

    u8* w;
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx out;
    Mtx texMtx;
    u8 tex[0x20];
    GXColor color1;
    GXColor color2;
    u8* anime;
    s32 type;
    s32 frame;
    s32 alpha;
    s32 i;
    s32 j;
    s32 phase;
    s32 channel;
    s32 edge;
    f32 baseAngle;
    f32 yaw;
    f32 yawRad;
    f32 step;
    f32 px;
    f32 py;
    f32 pz;
    f32 size;
    f32 angle;
    f32 c;
    f32 sn;
    f32 wave;

    w = *(u8**)((u8*)effect + 0xC);
    type = *(s32*)w;
    size = *(f32*)(w + 0x28);
    frame = *(s32*)(w + 0x30);
    alpha = *(s32*)(w + 0x34);
    px = *(f32*)(w + 0x10);
    py = *(f32*)(w + 0x14);
    pz = *(f32*)(w + 0x18);

    if (type == 0) {
        baseAngle = (f32)(frame * 35);
        step = float_9_804259d4;
        yaw = -*(f32*)((u8*)camGetPtr(4) + 0x114);
    } else {
        baseAngle = (f32)(frame * 25);
        px *= size;
        step = float_6_804259d8;
        yaw = float_0_804259cc;
    }

    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 4, 8, 15);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);

    effGetTexObjN64(0x10, tex);
    GXLoadTexObj(tex, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    yawRad = float_deg2rad_804259d0 * yaw;

    for (i = 0; i < 3; i++) {
        if (i > 0) {
            if (type == 0) {
                baseAngle -= float_70_804259dc;
                px = -(float_2_804259e0 * *(f32*)(w + 0x1C) - px);
                py = -(float_2_804259e0 * *(f32*)(w + 0x20) - py);
                pz = -(float_2_804259e0 * *(f32*)(w + 0x24) - pz);
            } else {
                px = -(float_2_804259e0 *
                           (float_1p2048_804259e8 * *(f32*)(w + 0x1C)) -
                       px);
                baseAngle -= float_50_804259e4;
            }

            alpha = (s32)(float_100_804259ec *
                          (f32)*(s32*)(w + 0x34) /
                          float_255_804259f0);
        }

        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

        PSMTXTrans(trans, px, py, pz);
        PSMTXScale(scale, size, size, size);
        PSMTXRotRad(rot, 0x79, yawRad);
        PSMTXConcat(rot, trans, trans);
        PSMTXConcat(trans, scale, out);

        anime = anime_data;
        for (j = 0; j < 3; j++, anime += 4) {
            angle = float_6p2832_804259f4 * baseAngle /
                    float_360_804259f8;
            c = (f32)cos(angle);
            sn = (f32)sin(angle);

            PSMTXTrans(trans,
                       step * c,
                       step * sn,
                       float_0_804259cc);
            baseAngle += float_120_804259fc;

            PSMTXConcat(out, trans, trans);
            PSMTXConcat(mtx, trans, trans);
            GXLoadPosMtxImm(trans, 0);

            color1 = unk_80429750;

            phase = (*(s32*)(gp + 0x1C) << 5) + anime[0] + 0xB4;
            wave = float_1_80425a04 +
                   (f32)sin(float_6p2832_804259f4 *
                            (f32)phase /
                            float_360_804259f8);
            channel = anime[1];
            color1.r = (u8)(float_0p5_80425a00 *
                                ((f32)(-channel) * wave) +
                            (f32)channel);

            phase = (*(s32*)(gp + 0x1C) << 5) + anime[0] + 0xB4;
            wave = float_1_80425a04 +
                   (f32)sin(float_6p2832_804259f4 *
                            (f32)phase /
                            float_360_804259f8);
            channel = anime[2];
            color1.g = (u8)(float_0p5_80425a00 *
                                ((f32)(-channel) * wave) +
                            (f32)channel);

            phase = (*(s32*)(gp + 0x1C) << 5) + anime[0] + 0xB4;
            wave = float_1_80425a04 +
                   (f32)sin(float_6p2832_804259f4 *
                            (f32)phase /
                            float_360_804259f8);
            channel = anime[3];
            color1.b = (u8)(float_0p5_80425a00 *
                                ((f32)(-channel) * wave) +
                            (f32)channel);
            color1.a = (u8)alpha;

            GXSetTevColor(1, color1);

            *(u32*)&color2 = dat_804259c8;

            phase = (*(s32*)(gp + 0x1C) << 5) + anime[0];
            wave = float_1_80425a04 +
                   (f32)sin(float_6p2832_804259f4 *
                            (f32)phase /
                            float_360_804259f8);
            channel = anime[1];
            color2.r = (u8)(float_0p5_80425a00 *
                                ((f32)(0xFF - channel) * wave) +
                            (f32)channel);

            phase = (*(s32*)(gp + 0x1C) << 5) + anime[0];
            wave = float_1_80425a04 +
                   (f32)sin(float_6p2832_804259f4 *
                            (f32)phase /
                            float_360_804259f8);
            channel = anime[2];
            color2.g = (u8)(float_0p5_80425a00 *
                                ((f32)(0xFF - channel) * wave) +
                            (f32)channel);

            phase = (*(s32*)(gp + 0x1C) << 5) + anime[0];
            wave = float_1_80425a04 +
                   (f32)sin(float_6p2832_804259f4 *
                            (f32)phase /
                            float_360_804259f8);
            channel = anime[3];
            color2.b = (u8)(float_0p5_80425a00 *
                                ((f32)(0xFF - channel) * wave) +
                            (f32)channel);

            GXSetTevColor(2, color2);

            edge = 10;
            if (type == 0) {
                edge = 15;
            }

            PSMTXScale(scale,
                       float_2_804259e0,
                       float_0p25_80425a08,
                       float_1_80425a04);
            PSMTXTrans(trans,
                       float_0_804259cc,
                       float_64_80425a0c *
                           (f32)(3 - j) *
                           float_0p0039062_80425a10,
                       float_0_804259cc);
            PSMTXConcat(trans, scale, trans);
            GXLoadTexMtxImm(trans, 0x1E, 1);

            GXBegin(0x80, 0, 4);

            *(volatile s16*)0xCC008000 = (s16)-edge;
            *(volatile s16*)0xCC008000 = (s16)edge;
            *(volatile s16*)0xCC008000 = 0;
            *(volatile f32*)0xCC008000 = float_0_804259cc;
            *(volatile f32*)0xCC008000 = float_0_804259cc;

            *(volatile s16*)0xCC008000 = (s16)edge;
            *(volatile s16*)0xCC008000 = (s16)edge;
            *(volatile s16*)0xCC008000 = 0;
            *(volatile f32*)0xCC008000 = float_1_80425a04;
            *(volatile f32*)0xCC008000 = float_0_804259cc;

            *(volatile s16*)0xCC008000 = (s16)edge;
            *(volatile s16*)0xCC008000 = (s16)-edge;
            *(volatile s16*)0xCC008000 = 0;
            *(volatile f32*)0xCC008000 = float_1_80425a04;
            *(volatile f32*)0xCC008000 = float_1_80425a04;

            *(volatile s16*)0xCC008000 = (s16)-edge;
            *(volatile s16*)0xCC008000 = (s16)-edge;
            *(volatile s16*)0xCC008000 = 0;
            *(volatile f32*)0xCC008000 = float_0_804259cc;
            *(volatile f32*)0xCC008000 = float_1_80425a04;
        }
    }
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagic1Disp(s32 cameraId, void* effEntry) {
    Mtx trans;
    Mtx rot;
    Mtx mtx;
    Mtx loopRot;
    register s32* work;
    register void* cam;
    register s32 type;
    f32 angle;
    s32 i;
    s32 rotAngle;

    cam = camGetPtr(cameraId);
    work = *(s32**)((s32)effEntry + 0xC);
    type = work[0];

    switch (type) {
        case 0:
            angle = float_0_804259cc;
            break;
        default:
            angle = -*(f32*)((s32)camGetPtr(4) + 0x114);
            break;
    }

    PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_804259d0 * angle);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);

    switch (type) {
        case 0:
            main_dl(effEntry, mtx);
            break;
        case 1:
            angle = float_deg2rad_804259d0;
            i = 0;
            rotAngle = 0;
            while (i < 12) {
                PSMTXRotRad(loopRot, 0x7A, angle * (f32)rotAngle);
                PSMTXConcat(mtx, loopRot, loopRot);
                main_dl(effEntry, loopRot);
                i++;
                rotAngle += 30;
            }
            break;
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

const u32 dat_804259c8 = 0x000000FF;
const f32 float_0_804259cc = 0.0f;
const f32 float_deg2rad_804259d0 = 0.01745329238474369f;
const f32 float_9_804259d4 = 9.0f;
const f32 float_6_804259d8 = 6.0f;
const f32 float_70_804259dc = 70.0f;
const f32 float_2_804259e0 = 2.0f;
const f32 float_50_804259e4 = 50.0f;
const f32 float_1p2048_804259e8 = 1.2048193216323853f;
const f32 float_100_804259ec = 100.0f;
const f32 float_255_804259f0 = 255.0f;
const f32 float_6p2832_804259f4 = 6.2831854820251465f;
const f32 float_360_804259f8 = 360.0f;
const f32 float_120_804259fc = 120.0f;
const f32 float_0p5_80425a00 = 0.5f;
const f32 float_1_80425a04 = 1.0f;
const f32 float_0p25_80425a08 = 0.25f;
const f32 float_64_80425a0c = 64.0f;
const f32 float_0p0039062_80425a10 = 0.00390625f;
const f32 float_0p83_80425a14 = 0.8299999833106995f;
const f32 float_0p9_80425a18 = 0.8999999761581421f;
const f32 float_29_80425a1c = 29.0f;
const f32 float_6p2_80425a20 = 6.199999809265137f;
