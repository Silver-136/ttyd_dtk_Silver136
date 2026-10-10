#include "effect/eff_toiki.h"

typedef struct GXColor {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} GXColor;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effToikiEntry(s32 type, s32 arg1, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effToikiMain(void);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern char str_Toiki_80428228;
    extern const f32 float_0_80428200;
    extern f32 float_6p2832_80428218;
    extern f32 float_200_8042821c;
    extern f32 float_360_80428220;
    extern f32 float_0p5_80428230;
    extern f32 float_255_80428234;
    extern f32 float_2p5_80428238;

    void* effect;
    void* work;
    f32 angle;

    effect = effEntry();
    *(char**)((s32)effect + 0x14) = &str_Toiki_80428228;
    *(s32*)((s32)effect + 8) = 1;
    work = __memAlloc(3, 0x34);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effToikiMain;
    *(u32*)effect |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x1C) = float_0p5_80428230;
    *(f32*)((s32)work + 0x20) = float_0_80428200;
    *(s32*)((s32)work + 0x30) = arg1;
    *(s32*)((s32)work + 0x2C) = 0;
    *(f32*)((s32)work + 0x28) = float_255_80428234;
    angle = ((*(f32*)((s32)work + 0x20) + float_200_8042821c) * float_6p2832_80428218) / float_360_80428220;
    *(f32*)((s32)work + 0x10) = float_2p5_80428238 * (f32)cos(angle);
    angle = ((*(f32*)((s32)work + 0x20) + float_200_8042821c) * float_6p2832_80428218) / float_360_80428220;
    *(f32*)((s32)work + 0x14) = float_2p5_80428238 * (f32)sin(angle);
    *(f32*)((s32)work + 0x18) = float_0_80428200;
    *(f32*)((s32)work + 0x24) = float_0_80428200;
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* stub-fill: effToikiMain | prototype_only | source_prototype */
void effToikiMain(void* effect) {
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(f32* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
    extern void effToikiDisp(s32 cameraId, void* effect);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXMultVec(f32 mtx[3][4], f32* src, f32* dst);
    extern void* effMistEntry(s32 type, s32 life, f32 x, f32 y, f32 z, f32 scale);
    extern f32 float_0p95_80428208;
    extern f32 float_1_8042820c;
    extern f32 float_4_80428210;
    extern f32 float_320_80428214;
    extern f32 float_6p2832_80428218;
    extern f32 float_200_8042821c;
    extern f32 float_360_80428220;
    extern f32 float_deg2rad_80428204;
    extern const f32 float_0_80428200;
    extern f32 float_0p015_80428224;

    void* work;
    f32 pos[3];
    f32 mist[3];
    f32 mtx[3][4];
    s32 timer;
    s32 type;

    work = *(void**)((s32)effect + 0xC);
    pos[0] = *(f32*)((s32)work + 4);
    pos[1] = *(f32*)((s32)work + 8);
    pos[2] = *(f32*)((s32)work + 0xC);
    type = *(s32*)((s32)work + 0);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4u;
        *(s32*)((s32)work + 0x30) = 0x10;
    }
    if (*(s32*)((s32)work + 0x30) < 1000) {
        *(s32*)((s32)work + 0x30) -= 1;
    }
    if (*(s32*)((s32)work + 0x30) < 0) {
        effDelete(effect);
        return;
    }

    *(f32*)((s32)work + 4) += *(f32*)((s32)work + 0x10);
    *(f32*)((s32)work + 8) += *(f32*)((s32)work + 0x14);
    *(f32*)((s32)work + 0xC) += *(f32*)((s32)work + 0x18);
    *(f32*)((s32)work + 0x10) *= float_0p95_80428208;
    *(f32*)((s32)work + 0x14) *= float_0p95_80428208;
    *(f32*)((s32)work + 0x18) *= float_0p95_80428208;

    if (*(s32*)((s32)work + 0x30) < 0x10) {
        *(f32*)((s32)work + 0x28) = (f32)(*(s32*)((s32)work + 0x30) << 4);
    }

    if (*(f32*)((s32)work + 0x24) > float_1_8042820c &&
        *(f32*)((s32)work + 0x24) < float_4_80428210) {
        timer = *(s32*)((s32)work + 0x30);
        if ((timer % 20) == 0) {
            f32 radians = ((float_200_8042821c + *(f32*)((s32)work + 0x20)) * float_6p2832_80428218) / float_360_80428220;
            f32 dist = float_320_80428214 * *(f32*)((s32)work + 0x1C);
            mist[0] = *(f32*)((s32)work + 4) + (dist * (f32)cos(radians));
            mist[1] = *(f32*)((s32)work + 8) + (dist * (f32)sin(radians));
            mist[2] = *(f32*)((s32)work + 0xC) + float_1_8042820c;
            PSMTXRotRad(mtx, 0x79, float_deg2rad_80428204 * -*(f32*)((s32)camGetPtr(4) + 0x114));
            PSMTXMultVec(mtx, mist, mist);
            effMistEntry(type + 1, 0x3C, mist[0], mist[1], mist[2], float_0_80428200);
        }
    }

    *(s32*)((s32)work + 0x2C) += 1;
    *(f32*)((s32)work + 0x24) += float_0p015_80428224;
    if (*(f32*)((s32)work + 0x24) > float_4_80428210) {
        *(f32*)((s32)work + 0x24) = float_4_80428210;
    }
    dispEntry(4, 2, effToikiDisp, dispCalcZ(pos), effect);
}

/* CHATGPT STUB FILL: main/effect/eff_toiki 20260624_184929 */

/* stub-fill: effToikiDisp | missing_definition | ghidra_signature */
__declspec(section ".sdata") u8 queen_toiki_normal_tbl[6] = {
    0x00, 0x00, 0x40, 0x20, 0x24, 0x28
};

__declspec(section ".sdata") u8 queen_toiki_color0_tbl[8] __attribute__ ((aligned(32))) = {
    0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_3[32] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x04, 0x29, 0x01, 0x01, 0x2A, 0x3D, 0x01, 0x00, 0x2B, 0x34, 0x01, 0x01, 0x29, 0x3C, 0x01, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_4[32] = {
    0x98, 0x00, 0x04, 0x3D, 0x01, 0x00, 0x2B, 0x28, 0x01, 0x00, 0x27, 0x3C, 0x01, 0x00, 0x28, 0x33, 0x01, 0x00, 0x26, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_5[32] = {
    0x98, 0x00, 0x04, 0x28, 0x01, 0x00, 0x27, 0x27, 0x01, 0x00, 0x25, 0x33, 0x01, 0x00, 0x26, 0x32, 0x01, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_6[32] = {
    0x98, 0x00, 0x04, 0x27, 0x01, 0x00, 0x25, 0x26, 0x01, 0x00, 0x23, 0x32, 0x01, 0x00, 0x24, 0x31, 0x01, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_7[32] = {
    0x98, 0x00, 0x04, 0x26, 0x01, 0x00, 0x23, 0x3A, 0x01, 0x00, 0x23, 0x31, 0x01, 0x00, 0x22, 0x39, 0x01, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_8[32] = {
    0x98, 0x00, 0x04, 0x3A, 0x01, 0x00, 0x23, 0x25, 0x01, 0x00, 0x20, 0x39, 0x01, 0x00, 0x22, 0x30, 0x01, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_9[32] = {
    0x98, 0x00, 0x04, 0x25, 0x01, 0x00, 0x20, 0x24, 0x01, 0x00, 0x21, 0x30, 0x01, 0x00, 0x1F, 0x2F, 0x01, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_10[32] = {
    0x98, 0x00, 0x04, 0x24, 0x01, 0x00, 0x21, 0x37, 0x01, 0x00, 0x1D, 0x2F, 0x01, 0x00, 0x1E, 0x38, 0x01, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_11[32] = {
    0x98, 0x00, 0x04, 0x37, 0x01, 0x00, 0x1D, 0x23, 0x01, 0x00, 0x16, 0x38, 0x01, 0x00, 0x1C, 0x2E, 0x01, 0x00, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_12[32] = {
    0x98, 0x00, 0x04, 0x23, 0x01, 0x00, 0x16, 0x22, 0x01, 0x00, 0x14, 0x2E, 0x01, 0x00, 0x17, 0x2D, 0x01, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_13[32] = {
    0x98, 0x00, 0x04, 0x15, 0x00, 0x01, 0x1A, 0x1E, 0x00, 0x00, 0x1B, 0x0A, 0x00, 0x01, 0x19, 0x1D, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 queen_toiki_dl_0_size_tbl[14] = {
    0x03, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
};

static GXColor color_tbl[3] = {
    { 0x03, 0x02, 0x21, 0xFF },
    { 0xD8, 0x0F, 0x51, 0xFF },
    { 0x0B, 0x1B, 0x00, 0xFF },
};

/*
 * Target-backed zero Vec3.  Keeping it immediately before effToikiDisp lets
 * MWCC retain the function's natural conversion double after the vector at
 * the target +0x10 boundary without hand-authoring the compiler pool.
 */
const f32 vec3_80300bd0[3] = {
    0.0f, 0.0f, 0.0f
};

void effToikiDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, GXColor*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void GXLoadTexMtxImm(f32[3][4], s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetArray(s32, void*, s32);
    extern void GXCallDisplayList(void*, s32);
    extern const f32 float_0_80428200;
    extern f32 float_deg2rad_80428204;
    extern GXColor unk_804298e0;
    extern u8 lbl_803B64A0[];

    u8* data = lbl_803B64A0;
    s32* work = *(s32**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    s32 type = work[0];
    u8 texObj[0x20];
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scaleMtx[3][4];
    GXColor color = unk_804298e0;
    GXColor tevColor;
    f32 size;
    void** dl;
    u8* dlSize;
    u32 i;

    effGetTexObj(0x5C, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    color.r = color_tbl[type].r;
    color.g = color_tbl[type].g;
    color.b = color_tbl[type].b;
    color.a = (u8)(s32)*(f32*)&work[10];
    tevColor = color;
    GXSetTevColor(1, &tevColor);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXTrans(trans, *(f32*)&work[9], float_0_80428200, float_0_80428200);
    GXLoadTexMtxImm(trans, 0x1E, 1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 0xA, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);

    PSMTXTrans(trans, *(f32*)&work[1], *(f32*)&work[2], *(f32*)&work[3]);
    PSMTXRotRad(rot, 0x79,
                float_deg2rad_80428204 *
                    -*(f32*)((s32)camGetPtr(cameraId) + 0x114));
    PSMTXConcat(trans, rot, trans);
    PSMTXRotRad(rot, 0x7A, float_deg2rad_80428204 * *(f32*)&work[8]);
    PSMTXConcat(trans, rot, trans);
    size = *(f32*)&work[7];
    PSMTXScale(scaleMtx, size, size, size);
    PSMTXConcat(trans, scaleMtx, trans);
    PSMTXConcat((f32(*)[4])((s32)camera + 0x34), trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxAttrFmt(0, 9, 1, 3, 6);
    GXSetArray(9, data + 0x40, 6);
    GXSetVtxDesc(0xA, 2);
    GXSetVtxAttrFmt(0, 0xA, 0, 1, 6);
    GXSetArray(0xA, queen_toiki_normal_tbl, 3);
    GXSetVtxDesc(0xB, 2);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetArray(0xB, queen_toiki_color0_tbl, 4);
    GXSetVtxDesc(0xD, 2);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 0xE);
    GXSetArray(0xD, data + 0x1C0, 4);

    dl = (void**)(data + 0x360);
    i = 0;
    dlSize = queen_toiki_dl_0_size_tbl;
    do {
        GXCallDisplayList(*dl, (u32)*dlSize << 5);
        i++;
        dl++;
        dlSize++;
    } while (i < 14);
}

/*
 * Late-defined target-backed Queen breath geometry/display-list data.
 *
 * The unresolved compiler/static-local +/-1 prefix is intentionally not
 * represented here.  The renderer continues to use its existing
 * lbl_803B64A0 common-base source shape.  Natural 32-byte alignment of the
 * semantic arrays/display lists reproduces the target's internal gaps.
 */
s16 queen_toiki_vertex_tbl[186] __attribute__ ((aligned(32))) = {
    -20614, -6729, 0, -20385, -6698, 0, -19918, -6319, 0, -19960,
    -6208, 0, -20494, -6254, 0, -22117, -6948, 0, -22371, -7251,
    0, -22119, -7687, 0, -21487, -7712, 0, -20289, -7445, 0,
    -6210, -1817, 0, -21254, -6859, 0, -20363, -7017, 0, -19239,
    -6349, 0, -19416, -5753, 0, -20134, -5715, 0, -22732, -6947,
    0, -23030, -7425, 0, -22417, -7994, 0, -21502, -8010, 0,
    -20149, -7666, 0, -6090, -1986, 0, -19842, -6843, 0, -20110,
    -6569, 0, -21396, -6515, 0, -21573, -6183, 0, -22914, -7767,
    0, -22353, -7491, 0, -19169, -5914, 0, -8707, -2815, 0,
    -8582, -2992, 0, -13924, -6394, 0, -13736, -6259, 0, -13136,
    -6212, 0, -13091, -6321, 0, -13510, -6655, 0, -15155, -7222,
    0, -15588, -7171, 0, -15611, -6693, 0, -15165, -6226, 0,
    -14093, -5615, 0, -9, 0, 0, -14477, -6740, 0, -13940,
    -6012, 0, -12665, -5722, 0, -12383, -6276, 0, -12877, -6798,
    0, -15610, -7694, 0, -16155, -7553, 0, -16102, -6718, 0,
    -15450, -6076, 0, -14231, -5394, 0, -126, 169, 0, -13443,
    -5780, 0, -13448, -6163, 0, -14343, -7087, 0, -14243, -7450,
    0, -16306, -7225, -2, -15710, -6975, 0, -12315, -5990, 0,
    -3702, -1240, 0, -3562, -1416, 0,
};

s16 queen_toiki_texcoord0_tbl[88] __attribute__ ((aligned(32))) = {
    -19660, 16384, -19660, 0, -20479, 0, -20479, 16384, -18022, 16384,
    -18022, 0, -18841, 0, -18841, 16384, -17203, 16384, -17203, 0,
    -16384, 16384, -16384, 0, -15564, 16384, -15564, 0, -13926, 16384,
    -13926, 0, -14745, 0, -14745, 16384, -13107, 16384, -13107, 0,
    -12288, 16384, -12288, 0, -11468, 16384, -11468, 0, -4056, 16384,
    -2457, 16384, -2457, 0, -4054, 0, -10649, 0, -10649, 16384,
    -9830, 0, -9011, 0, -9011, 16384, -9830, 16384, -8192, 0,
    -8192, 16384, -7372, 0, -7372, 16384, -6553, 0, -6553, 16384,
    -1661, 0, 0, 0, 0, 16384, -1653, 16384,
};

u8 queen_toiki_dl_0_0[96] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x15, 0x1D, 0x00, 0x00, 0x18, 0x1E, 0x00, 0x00, 0x1B, 0x09,
    0x00, 0x00, 0x16, 0x14, 0x00, 0x00, 0x17, 0x08, 0x00, 0x00, 0x14, 0x13,
    0x00, 0x00, 0x15, 0x07, 0x00, 0x00, 0x12, 0x12, 0x00, 0x00, 0x13, 0x1B,
    0x00, 0x00, 0x12, 0x1A, 0x00, 0x00, 0x13, 0x06, 0x00, 0x00, 0x0E, 0x11,
    0x00, 0x00, 0x0F, 0x05, 0x00, 0x00, 0x11, 0x10, 0x00, 0x00, 0x10, 0x18,
    0x00, 0x00, 0x0C, 0x19, 0x00, 0x00, 0x0D, 0x04, 0x00, 0x00, 0x0A, 0x0F,
    0x00, 0x00, 0x0B, 0x03, 0x00, 0x00, 0x08, 0x0E, 0x00, 0x00, 0x09, 0x1C,
    0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 queen_toiki_dl_0_1[64] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x0B, 0x2D, 0x01, 0x00, 0x15, 0x22, 0x01, 0x00, 0x14, 0x3B,
    0x01, 0x00, 0x15, 0x21, 0x01, 0x00, 0x12, 0x2C, 0x01, 0x00, 0x13, 0x36,
    0x01, 0x00, 0x0E, 0x35, 0x01, 0x00, 0x0F, 0x20, 0x01, 0x00, 0x11, 0x2B,
    0x01, 0x00, 0x10, 0x1F, 0x01, 0x00, 0x0C, 0x2A, 0x01, 0x00, 0x0D, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

u8 queen_toiki_dl_0_2[64] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x0A, 0x03, 0x00, 0x00, 0x08, 0x1C, 0x00, 0x00, 0x09, 0x02,
    0x00, 0x00, 0x04, 0x0D, 0x00, 0x00, 0x05, 0x17, 0x00, 0x00, 0x07, 0x16,
    0x00, 0x00, 0x06, 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x03, 0x0B, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

void* queen_toiki_dl_0_tbl[14] = {
    queen_toiki_dl_0_0, queen_toiki_dl_0_1, queen_toiki_dl_0_2, queen_toiki_dl_0_3,
    queen_toiki_dl_0_4, queen_toiki_dl_0_5, queen_toiki_dl_0_6, queen_toiki_dl_0_7,
    queen_toiki_dl_0_8, queen_toiki_dl_0_9, queen_toiki_dl_0_10, queen_toiki_dl_0_11,
    queen_toiki_dl_0_12, queen_toiki_dl_0_13,
};

__declspec(section ".sdata2") const f32 float_0_80428200 = 0.0f;
__declspec(section ".sdata2") f32 float_deg2rad_80428204 = 0.01745329238474369f;
__declspec(section ".sdata2") f32 float_0p95_80428208 = 0.949999988079071f;
__declspec(section ".sdata2") f32 float_1_8042820c = 1.0f;
__declspec(section ".sdata2") f32 float_4_80428210 = 4.0f;
__declspec(section ".sdata2") f32 float_320_80428214 = 320.0f;
__declspec(section ".sdata2") f32 float_6p2832_80428218 = 6.2831854820251465f;
__declspec(section ".sdata2") f32 float_200_8042821c = 200.0f;
__declspec(section ".sdata2") f32 float_360_80428220 = 360.0f;
__declspec(section ".sdata2") f32 float_0p015_80428224 = 0.014999999664723873f;
__declspec(section ".sdata2") char str_Toiki_80428228[6] = "Toiki";
__declspec(section ".sdata2") f32 float_0p5_80428230 = 0.5f;
__declspec(section ".sdata2") f32 float_255_80428234 = 255.0f;
__declspec(section ".sdata2") f32 float_2p5_80428238 = 2.5f;

__declspec(section ".sdata2") GXColor unk_804298e0;
