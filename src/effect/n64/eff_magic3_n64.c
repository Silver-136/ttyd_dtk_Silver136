#include "effect/n64/eff_magic3_n64.h"

extern f64 sin(f64);

void* effMagic3N64Entry(f32 x, f32 y, f32 z, s32 type, s32 arg) {
    typedef f32 Mtx[3][4];
    typedef struct Vec { f32 x, y, z; } Vec;
    typedef union FloatBits { f32 f; u32 u; } FloatBits;
    extern void* effEntry(void);
    extern void* __memAlloc(s32,s32);
    extern void effMagic3Main(void*);
    extern void* camGetPtr(s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 __frsqrte(f64);
    extern f32 __float_nan;
    extern void PSMTXRotAxisRad(Mtx,Vec*,f32);
    extern char str_Magic3N64_802fb4a8[];
    extern const f32 vec3_802fb470[];
    extern const f64 double_0p5_802fb490;
    extern const f64 double_3_802fb498;
    extern const f64 double_0_802fb4a0;
    void* entry = effEntry();
    u8* work;
    u8* camera;
    Mtx rot;
    Vec axis;
    Vec direction;
    Vec perpendicular;
    FloatBits bits;
    f32 length;
    f32 lengthSq;
    f32 perpendicularLengthSq;
    f32 cameraAngle;
    f32 spread;
    f64 root;
    f64 inv;
    s32 floatClass;
    s32 i;

    direction.x = 0.0f;
    direction.y = -1.0f;
    direction.z = 0.0f;
    lengthSq = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
    root = (f64)lengthSq;
    if (root > double_0_802fb4a0) {
        inv = __frsqrte(root);
        inv = double_0p5_802fb490 * inv * (double_3_802fb498 - root * inv * inv);
        inv = double_0p5_802fb490 * inv * (double_3_802fb498 - root * inv * inv);
        root = (f64)(f32)(root * (double_0p5_802fb490 * inv *
            (double_3_802fb498 - root * inv * inv)));
    } else if (root < double_0_802fb4a0) {
        root = (f64)__float_nan;
    } else {
        bits.f = lengthSq;
        if ((bits.u & 0x7F800000) == 0x7F800000) {
            floatClass = (bits.u & 0x7FFFFF) == 0 ? 2 : 1;
        } else if ((bits.u & 0x7F800000) == 0) {
            floatClass = (bits.u & 0x7FFFFF) == 0 ? 3 : 5;
        } else {
            floatClass = 4;
        }
        if (floatClass == 1) {
            root = (f64)__float_nan;
        }
    }
    length = (f32)root;
    if (length == 0.0f) {
        return 0;
    }
    direction.x /= length;
    direction.y /= length;
    direction.z /= length;
    if (direction.x != 0.0f) {
        perpendicular.x = -direction.y / direction.x;
        perpendicular.y = 1.0f;
        perpendicular.z = 0.0f;
    } else if (direction.y != 0.0f) {
        perpendicular.x = 1.0f;
        perpendicular.y = -direction.x / direction.y;
        perpendicular.z = 0.0f;
    } else {
        perpendicular.x = 1.0f;
        perpendicular.y = 0.0f;
        perpendicular.z = -direction.x / direction.z;
    }
    perpendicularLengthSq = perpendicular.x * perpendicular.x +
                            perpendicular.y * perpendicular.y +
                            perpendicular.z * perpendicular.z;
    if (perpendicularLengthSq == 0.0f) {
        return 0;
    }
    direction.x *= 18.0f;
    direction.y *= 18.0f;
    direction.z *= 18.0f;

    *(char**)((u8*)entry + 0x14) = str_Magic3N64_802fb4a8;
    *(s32*)((u8*)entry + 8) = 5;
    work = __memAlloc(3, 0xF0);
    *(u8**)((u8*)entry + 0xC) = work;
    *(void**)((u8*)entry + 0x10) = effMagic3Main;
    for (i = 0; i < 5; i++, work += 0x30) {
        camera = camGetPtr(4);
        cameraAngle = 6.2832f * *(f32*)(camera + 0x114) / 360.0f;
        axis.x = vec3_802fb470[0];
        axis.y = vec3_802fb470[1];
        axis.z = vec3_802fb470[2];
        axis.x = (f32)sin((f64)cameraAngle);
        axis.z = -(f32)cos((f64)cameraAngle);
        if (i == 1) {
            spread = 0.0f;
        } else {
            spread = 100.0f * (f32)i * 0.25f - 50.0f;
        }
        PSMTXRotAxisRad(rot, &axis, spread * 0.017453292f);
        *(s32*)work = type;
        *(f32*)(work + 4) = x;
        *(f32*)(work + 8) = y;
        *(f32*)(work + 0xC) = z;
        *(f32*)(work + 0x10) = 0.6f *
            (rot[0][0] * direction.x + rot[0][1] * direction.y + rot[0][2] * direction.z);
        *(f32*)(work + 0x14) = rot[1][0] * direction.x + rot[1][1] * direction.y + rot[1][2] * direction.z;
        *(f32*)(work + 0x18) = 0.6f *
            (rot[2][0] * direction.x + rot[2][1] * direction.y + rot[2][2] * direction.z);
        *(f32*)(work + 0x1C) = (f32)(50 - i * 25);
        *(f32*)(work + 0x20) = 0.5f;
        *(s32*)(work + 0x28) = arg;
        *(s32*)(work + 0x2C) = 0;
        *(s32*)(work + 0x24) = 0;
    }
    return entry;
}

void effMagic3Main(void* effect) {
    typedef struct Vec3 { f32 x; f32 y; f32 z; } Vec3;
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effMagic3Disp(s32, s32);
    extern f32 vec3_802fb47c[3];
    extern f32 float_0p8_80425a94, float_0p4_80425a98, float_6p2832_80425a90;
    extern f32 float_360_80425a9c, float_3_80425aa0, float_0p7_80425aa8;
    extern f32 float_0p6_80425aa4, float_30_80425aac;
    u8* work;
    Vec3 initialPos;
    Vec3 displayPos;
    s32 timer;
    s32 i;
    f32 phase;
    f32 wave;
    f32 halfWave;

    work = *(u8**)((s32)effect + 0xC);
    initialPos.x = vec3_802fb47c[0];
    initialPos.y = vec3_802fb47c[1];
    initialPos.z = vec3_802fb47c[2];
    initialPos.x = *(f32*)(work + 4);
    initialPos.y = *(f32*)(work + 8);
    initialPos.z = *(f32*)(work + 0xC);
    displayPos = initialPos;
    *(s32*)(work + 0x28) -= 1;
    *(s32*)(work + 0x2C) += 1;
    timer = *(s32*)(work + 0x28);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    phase = float_6p2832_80425a90 * (f32)(*(s32*)(work + 0x2C) * 30);
    for (i = 0; i < *(s32*)((s32)effect + 8); i++) {
        if (timer > 5) {
            *(f32*)(work + 0x10) *= float_0p8_80425a94;
            *(f32*)(work + 0x14) *= float_0p8_80425a94;
            *(f32*)(work + 0x18) *= float_0p8_80425a94;
            *(s32*)(work + 0x24) = (s32)(float_0p4_80425a98 *
                (f32)(255 - *(s32*)(work + 0x24)) +
                (f32)*(s32*)(work + 0x24));
            wave = (f32)sin((f64)(phase / float_360_80425a9c));
            halfWave = (f32)sin((f64)(float_6p2832_80425a90 *
                (f32)(*(s32*)(work + 0x24) / 2) / float_360_80425a9c));
            *(f32*)(work + 0x20) = float_3_80425aa0 * halfWave + wave;
        }
        if (timer < 5) {
            *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) * float_0p6_80425aa4);
            *(f32*)(work + 0x10) *= float_0p7_80425aa8;
            *(f32*)(work + 0x14) *= float_0p7_80425aa8;
            *(f32*)(work + 0x18) *= float_0p7_80425aa8;
            wave = (f32)sin((f64)(phase / float_360_80425a9c));
            halfWave = (f32)sin((f64)(float_6p2832_80425a90 *
                (f32)(*(s32*)(work + 0x24) / 2) / float_360_80425a9c));
            *(f32*)(work + 0x20) = (f32)(255 - *(s32*)(work + 0x24)) /
                float_30_80425aac + float_3_80425aa0 * halfWave + wave;
        }
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 8) += *(f32*)(work + 0x14);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        work += 0x30;
    }
    dispEntry(4, 2, effMagic3Disp, effect, dispCalcZ(&displayPos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagic3Disp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
    extern u32 dat_80425a78;
    extern f32 float_0p015625_80425a7c;
    extern f32 float_0p03125_80425a80;
    extern f32 float_0_80425a84;
    extern f32 float_deg2rad_80425a88;
    extern f32 float_1_80425a8c;
    extern u8 size32x16_tex64x32_vtx[];
    f32 trans[3][4];
    f32 scaleMtx[3][4];
    f32 rotY[3][4];
    f32 rotZ[3][4];
    u8 texObj[0x20];
    void* cam;
    u8* item;
    s32 i;
    u32 colorTmp;
    u32 color;
    f32 deg2rad;
    f32 camRot;

    cam = camGetPtr(cameraId);
    item = *(u8**)((s32)effect + 0xC);

    colorTmp = dat_80425a78;
    ((u8*)&colorTmp)[3] = *(s32*)(item + 0x24);
    color = colorTmp;
    GXSetTevColor(1, &color);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p015625_80425a7c, float_0p03125_80425a80, float_0_80425a84);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    effGetTexObjN64(0x1E, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 2);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetCullMode(0);
    effSetVtxDescN64(size32x16_tex64x32_vtx);

    deg2rad = float_deg2rad_80425a88;
    for (i = 0; i < *(s32*)((s32)effect + 8); i++, item += 0x30) {
        PSMTXTrans(trans, *(f32*)(item + 4), *(f32*)(item + 8), *(f32*)(item + 0xC));
        camRot = -*(f32*)((s32)camGetPtr(4) + 0x114);
        PSMTXRotRad(rotY, 0x79, deg2rad * camRot);
        PSMTXRotRad(rotZ, 0x7A, deg2rad * *(f32*)(item + 0x1C));
        PSMTXScale(scaleMtx, *(f32*)(item + 0x20), *(f32*)(item + 0x20), float_1_80425a8c);
        PSMTXConcat(trans, rotY, trans);
        PSMTXConcat(trans, rotZ, trans);
        PSMTXConcat(trans, scaleMtx, trans);
        PSMTXConcat((void*)((s32)cam + 0x11C), trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

u8 size32x16_tex64x32_vtx[56] = {
    0xFF, 0xF0, 0xFF, 0xF8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xFF,
    0, 0x10, 0xFF, 0xF8, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0xFF,
    0, 0x10, 0, 8, 0, 0, 8, 0, 4, 0, 0, 0, 0, 0xFF,
    0xFF, 0xF0, 0, 8, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0xFF,
};

const f32 vec3_802fb470[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802fb47c[3] = { 0.0f, 0.0f, 0.0f };
const char str_Magic3N64_802fb4a8[] = "Magic3N64";
