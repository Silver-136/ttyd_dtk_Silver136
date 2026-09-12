#include "effect/n64/eff_cool_leaf_n64.h"

char size12x12_tex64x64_brown_vtx[112] = {
    0xFF, 0xFA, 0xFF, 0xFA, 0, 0, 0, 0, 0, 0, 0x19, 0x2A, 0x11, 0xFF,
    0, 6, 0xFF, 0xFA, 0, 0, 8, 0, 0, 0, 0x19, 0x2A, 0x11, 0xFF,
    0, 6, 0, 6, 0, 0, 8, 0, 8, 0, 0x68, 0x56, 0x10, 0xFF,
    0xFF, 0xFA, 0, 6, 0, 0, 0, 0, 8, 0, 0x68, 0x56, 0x10, 0xFF,
    0xFF, 0xFA, 0xFF, 0xFA, 0, 0, 0, 0, 8, 0, 0x18, 0x1B, 0x21, 0xFF,
    0, 6, 0xFF, 0xFA, 0, 0, 8, 0, 8, 0, 0x18, 0x1B, 0x21, 0xFF,
    0, 6, 0, 6, 0, 0, 8, 0, 0x10, 0, 0x30, 0x36, 0x25, 0xFF,
    0xFF, 0xFA, 0, 6, 0, 0, 0, 0, 0x10, 0, 0x30, 0x36, 0x25, 0xFF,
};

char size12x12_tex64x64_red_vtx[112] = {
    0xFF, 0xFA, 0xFF, 0xFA, 0, 0, 0, 0, 0, 0, 0x32, 0x15, 0x11, 0xFF,
    0, 6, 0xFF, 0xFA, 0, 0, 8, 0, 0, 0, 0x32, 0x15, 0x11, 0xFF,
    0, 6, 0, 6, 0, 0, 8, 0, 8, 0, 0x68, 0x2B, 0x10, 0xFF,
    0xFF, 0xFA, 0, 6, 0, 0, 0, 0, 8, 0, 0x68, 0x2B, 0x10, 0xFF,
    0xFF, 0xFA, 0xFF, 0xFA, 0, 0, 0, 0, 8, 0, 0x30, 0x0D, 0x21, 0xFF,
    0, 6, 0xFF, 0xFA, 0, 0, 8, 0, 8, 0, 0x30, 0x0D, 0x21, 0xFF,
    0, 6, 0, 6, 0, 0, 8, 0, 0x10, 0, 0x60, 0x1B, 0x25, 0xFF,
    0xFF, 0xFA, 0, 6, 0, 0, 0, 0, 0x10, 0, 0x60, 0x1B, 0x25, 0xFF,
};

extern f32 float_0_80424fd8;
#pragma optimize_for_size off
void* effCoolLeafN64Entry(f32 x, f32 y, f32 z, u32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern s32 rand(void);
    extern void effCoolLeafMain(void* effect);
    extern char str_CoolLeafN64_802fada0[];
    extern f32 float_0p05_80425000;
    extern f32 float_2_80424fe8;
    extern f32 float_0p1_80425028;
    extern f32 float_neg3_8042502c;
    extern f32 float_3_80425030;
    extern f32 float_neg6_80425034;
    extern f32 float_6_80425038;
    extern f32 float_0p008_8042503c;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 r;
    s32 r2;
    s32 r3;
    s32 mod;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_CoolLeafN64_802fada0;
    *(s32*)((s32)entry + 8) = 5;
    work = __memAlloc(3, 0x12C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effCoolLeafMain;

    *(u32*)(work + 0) = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    zero = float_0_80424fd8;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x28) = 100;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;

    part = work + 0x3C;
    for (i = 1; i < 5; i++, part += 0x3C) {
        r = rand() % 0x33;
        r2 = rand() % 0x15;
        r3 = rand() % 0x169;
        mod = rand() % 0x65;

        *(f32*)(part + 4) = (f32)(r + 0x4B);
        *(f32*)(part + 8) = (f32)(r2 + 0x46);
        *(f32*)(part + 0xC) = zero;
        *(f32*)(part + 0x18) = (f32)r3;
        *(f32*)(part + 0x20) = (f32)(0x168 - r3);

        if (type == 1) {
            *(f32*)(part + 0x1C) =
                float_2_80424fe8 * float_0p1_80425028 * (f32)mod;
            *(f32*)(part + 0x10) = zero;
            *(f32*)(part + 0x14) = zero;
            *(f32*)(part + 0x30) = float_neg6_80425034;
            *(f32*)(part + 0x34) = zero;
            *(f32*)(part + 0x38) = float_6_80425038;
        } else if ((s32)type < 1) {
            if ((s32)type > -1) {
                *(f32*)(part + 0x1C) = float_0p1_80425028 * (f32)mod;
                *(f32*)(part + 0x10) = zero;
                *(f32*)(part + 0x14) = zero;
                *(f32*)(part + 0x30) = float_neg3_8042502c;
                *(f32*)(part + 0x34) = zero;
                *(f32*)(part + 0x38) = float_3_80425030;
            }
        } else if ((s32)type < 3) {
            *(f32*)(part + 0x1C) =
                float_2_80424fe8 * float_0p1_80425028 * (f32)mod;
            r = rand() % 0xB;
            *(f32*)(part + 0x10) =
                -*(f32*)(part + 4) *
                (float_0p008_8042503c * (f32)r + float_0p05_80425000);
            r = rand() % 0xB;
            *(f32*)(part + 0x14) =
                -*(f32*)(part + 8) *
                (float_0p008_8042503c * (f32)r + float_0p05_80425000);
        }
    }

    return entry;
}
#pragma optimize_for_size on


void effCoolLeafMain(void* effect) {
    extern void effDelete(void*);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern s32 effTblRandN64(s32,s32);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32,s32,void*,void*,f32);
    extern void effCoolLeafDisp(s32,void*);
    extern f32 float_0p95_80424ffc, float_30_80425010, float_neg200_80425014;
    extern f32 float_0p98_8042501c;
    typedef struct LocalVec3 { f32 x, y, z; } LocalVec3;
    extern const LocalVec3 vec3_802fad88;
    u8* entry = effect;
    u8* work = *(u8**)(entry + 0xC);
    u8* leaf;
    LocalVec3 seed = vec3_802fad88;
    LocalVec3 pos;
    u32 type;
    s32 frame, i;
    f32 phase;
    volatile f32 s03 = 0.3f;
    volatile f32 s08 = 0.8f;
    volatile f32 sTau2 = 12.5664f;
    volatile f32 s360 = 360.0f;
    volatile f32 s005 = 0.05f;
    volatile f32 s04 = 0.4f;
    volatile f32 s02 = 0.2f;
    volatile f32 s08464 = 0.8464f;
    volatile f32 s094 = 0.94f;
    volatile f32 sTau = 6.2832f;
    volatile f32 s10 = 10.0f;
    volatile f32 s05 = 0.5f;
    volatile f32 sNeg08 = -0.8f;
    volatile f32 s0 = 0.0f;
    volatile f32 s096 = 0.96f;
    f32 c03 = s03;
    f32 c08 = s08;
    f32 cTau2 = sTau2;
    f32 c360 = s360;
    f32 c005 = s005;
    f32 c04 = s04;
    f32 c02 = s02;
    f32 c08464 = s08464;
    f32 c094 = s094;
    f32 cTau = sTau;
    f32 c10 = s10;
    f32 c05 = s05;
    f32 cNeg08 = sNeg08;
    f32 c0 = s0;
    f32 c096 = s096;

    seed.x = *(f32*)(work + 4);
    seed.y = *(f32*)(work + 8);
    seed.z = *(f32*)(work + 0xC);
    pos = seed;
    --*(s32*)(work + 0x28);
    ++*(s32*)(work + 0x2C);
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }
    frame = *(s32*)(work + 0x2C);
    type = *(u32*)work;
    if (frame < 10) {
        *(s32*)(work + 0x24) = (s32)(c03 *
            (f32)(255 - *(s32*)(work + 0x24)) + (f32)*(s32*)(work + 0x24));
    }
    if (*(s32*)(work + 0x28) < 10) {
        *(s32*)(work + 0x24) = (s32)((f32)*(s32*)(work + 0x24) * c08);
    }

    leaf = work + 0x3C;
    if (type == 1) {
        for (i = 1; i < *(s32*)(entry + 8); i++, leaf += 0x3C, frame += 20) {
            phase = (cTau2 * *(f32*)(leaf + 0x18)) / c360;
            *(f32*)(leaf + 0x14) -= c005;
            *(f32*)(leaf + 0x10) += c04 * (f32)sin(phase);
            *(f32*)(leaf + 0x14) -= c005;
            *(f32*)(leaf + 0x10) *= c08464;
            *(f32*)(leaf + 0x1C) += (f32)(effTblRandN64(50, frame) - 25);
            phase = (cTau * *(f32*)(leaf + 0x1C)) / c360;
            *(f32*)(leaf + 0x18) += c10 * (f32)sin(phase);
            phase = (cTau * *(f32*)(leaf + 0x1C)) / c360;
            *(f32*)(leaf + 0x20) += c10 * (f32)cos(phase * c05);
            *(f32*)(leaf + 4) += *(f32*)(leaf + 0x10);
            *(f32*)(leaf + 8) += *(f32*)(leaf + 0x14);
            if (*(f32*)(leaf + 8) >= float_30_80425010) {
                *(f32*)(leaf + 0x30) += (*(f32*)(leaf + 4) <= c0) ? c08 : cNeg08;
                *(f32*)(leaf + 0x38) += (*(f32*)(leaf + 0xC) <= c0) ? c08 : cNeg08;
            } else {
                *(f32*)(leaf + 0x30) += (*(f32*)(leaf + 4) <= float_neg200_80425014) ? c08 : cNeg08;
                *(f32*)(leaf + 0x38) += (*(f32*)(leaf + 0xC) <= c0) ? c08 : cNeg08;
                *(f32*)(leaf + 0x14) *= c096;
            }
            *(f32*)(leaf + 0x30) *= c096;
            *(f32*)(leaf + 0x38) *= c096;
            *(f32*)(leaf + 4) += *(f32*)(leaf + 0x30);
            *(f32*)(leaf + 8) += *(f32*)(leaf + 0x34);
            *(f32*)(leaf + 0xC) += *(f32*)(leaf + 0x38);
        }
    } else if ((s32)type < 1) {
        if ((s32)type > -1) {
            for (i = 1; i < *(s32*)(entry + 8); i++, leaf += 0x3C, frame += 20) {
                phase = (cTau2 * *(f32*)(leaf + 0x18)) / c360;
                *(f32*)(leaf + 0x14) -= c005;
                *(f32*)(leaf + 0x10) += c02 * (f32)sin(phase);
                *(f32*)(leaf + 0x10) *= c094;
                phase = (cTau * *(f32*)(leaf + 0x18)) / c360;
                *(f32*)(leaf + 0x14) *= c005 * (f32)sin(phase) + float_0p95_80424ffc;
                *(f32*)(leaf + 0x1C) += (f32)(effTblRandN64(50, frame) - 25);
                phase = (cTau * *(f32*)(leaf + 0x1C)) / c360;
                *(f32*)(leaf + 0x18) += c10 * (f32)sin(phase);
                phase = (cTau * *(f32*)(leaf + 0x1C)) / c360;
                *(f32*)(leaf + 0x20) += c10 * (f32)cos(phase * c05);
                *(f32*)(leaf + 4) += *(f32*)(leaf + 0x10);
                *(f32*)(leaf + 8) += *(f32*)(leaf + 0x14);
                if (*(f32*)(leaf + 8) >= float_30_80425010) {
                    if (*(f32*)(leaf + 4) <= c0) *(f32*)(leaf + 0x30) += c04;
                    else *(f32*)(leaf + 0x30) -= c04;
                    if (*(f32*)(leaf + 0xC) <= c0) *(f32*)(leaf + 0x38) += c04;
                    else *(f32*)(leaf + 0x38) -= c04;
                } else {
                    if (*(f32*)(leaf + 4) <= float_neg200_80425014) *(f32*)(leaf + 0x30) += c04;
                    else *(f32*)(leaf + 0x30) -= c04;
                    if (*(f32*)(leaf + 0xC) <= c0) *(f32*)(leaf + 0x38) += c04;
                    else *(f32*)(leaf + 0x38) -= c04;
                    *(f32*)(leaf + 0x14) *= float_0p98_8042501c;
                }
                *(f32*)(leaf + 0x30) *= float_0p98_8042501c;
                *(f32*)(leaf + 0x38) *= float_0p98_8042501c;
                *(f32*)(leaf + 4) += *(f32*)(leaf + 0x30);
                *(f32*)(leaf + 8) += *(f32*)(leaf + 0x34);
                *(f32*)(leaf + 0xC) += *(f32*)(leaf + 0x38);
            }
        }
    } else if ((s32)type < 3) {
        for (i = 1; i < *(s32*)(entry + 8); i++, leaf += 0x3C, frame += 20) {
            *(f32*)(leaf + 0x1C) += (f32)(effTblRandN64(50, frame) - 25);
            phase = (cTau * *(f32*)(leaf + 0x1C)) / c360;
            *(f32*)(leaf + 0x18) += c10 * (f32)sin(phase);
            phase = (cTau * *(f32*)(leaf + 0x1C)) / c360;
            *(f32*)(leaf + 0x20) += c10 * (f32)cos(phase * c05);
            *(f32*)(leaf + 4) += *(f32*)(leaf + 0x10);
            *(f32*)(leaf + 8) += *(f32*)(leaf + 0x14);
        }
    }
    dispEntry(4, 2, effCoolLeafDisp, effect, dispCalcZ(&pos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effCoolLeafDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 tex, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetArray(s32 attr, void* data, s32 stride);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern f32 float_deg2rad_80424fcc;
    extern u32 dat_80424fc8;
    extern f32 float_4_80424fd0;
    extern f32 float_1_80424fd4;
    u8 texObj[0x20];
    f32 baseMtx[3][4];
    f32 rotMtx[3][4];
    f32 texMtx[3][4];
    f32 tmpMtx[3][4];
    f32 itemMtx[3][4];
    u32 color;
    u8* work;
    u8* part;
    void* cam;
    void* cam3d;
    s32 type;
    s32 i;

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    part = work;
    type = *(s32*)work;
    PSMTXTrans(tmpMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam3d = camGetPtr(4);
    PSMTXRotRad(rotMtx, float_deg2rad_80424fcc * -*(f32*)((s32)cam3d + 0x114), 'y');
    PSMTXConcat(tmpMtx, rotMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);

    color = dat_80424fc8;
    ((u8*)&color)[3] = *(u8*)(work + 0x24);
    GXSetTevColor(1, &color);
    effGetTexObjN64(0x14, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_4_80424fd0, float_1_80424fd4, float_0_80424fd8);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xA, 0x8, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 6, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 3);
    GXSetVtxDesc(0xB, 3);
    GXSetVtxDesc(0xD, 3);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 0xC);

    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part += 0x3C) {
        PSMTXTrans(itemMtx, *(f32*)(part + 0x40), *(f32*)(part + 0x44), *(f32*)(part + 0x48));
        PSMTXRotRad(rotMtx, float_deg2rad_80424fcc * *(f32*)(part + 0x54), 'z');
        PSMTXConcat(itemMtx, rotMtx, itemMtx);
        PSMTXRotRad(rotMtx, float_deg2rad_80424fcc * *(f32*)(part + 0x5C), 'y');
        PSMTXConcat(itemMtx, rotMtx, itemMtx);
        PSMTXConcat(baseMtx, itemMtx, itemMtx);
        GXLoadPosMtxImm(itemMtx, 0);
        GXSetCurrentMtx(0);
        if (type == 1) {
            GXSetArray(9, size12x12_tex64x64_red_vtx, 0xE);
            GXSetArray(0xB, size12x12_tex64x64_red_vtx + 0xA, 0xE);
            GXSetArray(0xD, size12x12_tex64x64_red_vtx + 6, 0xE);
        } else if ((type < 1) && (type > -1)) {
            GXSetArray(9, size12x12_tex64x64_brown_vtx, 0xE);
            GXSetArray(0xB, size12x12_tex64x64_brown_vtx + 0xA, 0xE);
            GXSetArray(0xD, size12x12_tex64x64_brown_vtx + 6, 0xE);
        }
        GXBegin(0x90, 0, 0xC);
        tri2(0, 1, 2, 0, 0, 2, 3);
        tri2(6, 5, 4, 4, 7, 6, 4);
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
