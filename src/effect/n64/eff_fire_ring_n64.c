#include "effect/n64/eff_fire_ring_n64.h"

typedef struct EffFireRingWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    u8 pad_10[0xC];
    f32 unk_1c;
    f32 scaleStep;
    s32 alpha;
    s32 duration;
    s32 frame;
    f32 unk_30;
    f32 unk_34;
    f32 unk_38;
    f32 unk_3c;
    f32 unk_40;
    f32 unk_44;
    f32 unk_48;
    f32 unk_4c;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 pad_53;
} EffFireRingWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);

extern char str_FireRingN64_802faec8[];
extern f32 float_0_804251e4;
extern f32 float_neg1p1_804251fc;
extern f32 float_0p4_80425200;
extern f32 float_1_80425204;
extern f32 float_0p1_80425208;


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effFireRingDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 tex, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern u32 dat_804251d8;
    extern u32 dat_804251dc;
    extern f32 float_0p015873_804251e0;
    extern f32 float_0_804251e4;
    extern f32 float_0p032258_804251e8;
    extern char stg__s_v[];
    u8 texObj[0x20];
    f32 mtxA[3][4];
    f32 mtxB[3][4];
    u8* work;
    void* cam;
    u32 color;

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);

    PSMTXScale(mtxA, float_0p015873_804251e0, float_0p015873_804251e0, float_0_804251e4);
    PSMTXTrans(mtxB, *(f32*)(work + 0x30), *(f32*)(work + 0x34), float_0_804251e4);
    PSMTXConcat(mtxA, mtxB, mtxB);
    GXLoadTexMtxImm(mtxB, 0x1E, 1);

    PSMTXScale(mtxA, float_0p032258_804251e8, float_0p032258_804251e8, float_0_804251e4);
    PSMTXTrans(mtxB, *(f32*)(work + 0x38), *(f32*)(work + 0x3C), float_0_804251e4);
    PSMTXConcat(mtxA, mtxB, mtxB);
    GXLoadTexMtxImm(mtxB, 0x21, 1);

    PSMTXTrans(mtxB, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXScale(mtxA, *(f32*)(work + 0x1C), *(f32*)(work + 0x1C), *(f32*)(work + 0x1C));
    PSMTXConcat(mtxB, mtxA, mtxB);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtxB, mtxB);
    GXLoadPosMtxImm(mtxB, 0);
    GXSetCurrentMtx(0);

    effGetTexObjN64(0x2D, texObj);
    GXLoadTexObj(texObj, 0);
    effGetTexObjN64(0x2E, texObj);
    GXLoadTexObj(texObj, 1);

    color = dat_804251d8;
    ((u8*)&color)[3] = *(u8*)(work + 0x24);
    GXSetTevColor(1, &color);
    color = (*(u8*)(work + 0x50) << 24) | (*(u8*)(work + 0x51) << 16) |
            (*(u8*)(work + 0x52) << 8) | (dat_804251dc & 0xFF);
    GXSetTevColor(2, &color);

    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 6, 5, 4);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0, 8, 2);
    GXSetTevAlphaIn(1, 7, 0, 4, 7);
    GXSetTevOrder(2, 1, 1, 0xFF);
    GXSetTevColorOp(2, 1, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0xF, 8, 8, 0);
    GXSetTevAlphaIn(2, 7, 7, 7, 0);
    GXSetCullMode(0);
    effSetVtxDescN64(stg__s_v);
    GXBegin(0x90, 0, 0x5A);
    tri2(0, 1, 2, 0, 0, 2, 3);
    tri2(4, 5, 6, 0, 4, 6, 7);
    tri2(8, 9, 10, 0, 8, 10, 0xB);
    tri2(0xC, 0xD, 0xE, 0, 0xC, 0xE, 0xF);
    tri2(0x10, 0x11, 0x12, 0, 0x10, 0x12, 0x13);
    tri2(0x14, 0x15, 0x16, 0, 0x14, 0x16, 0x17);
    tri2(0x18, 0x19, 0x1A, 0, 0x18, 0x1A, 0x1B);
    tri2(0x1C, 0x1D, 0x1E, 0, 0x1C, 0x1E, 0x1F);
    tri2(0x20, 0x27, 0x28, 0, 0x20, 0x28, 0x29);
    tri2(0x21, 0x2A, 0x2B, 0, 0x21, 0x2B, 0x2C);
    tri2(0x22, 0x2D, 0x2E, 0, 0x22, 0x2E, 0x2F);
    tri2(0x23, 0x30, 0x31, 0, 0x23, 0x31, 0x32);
    tri2(0x24, 0x33, 0x34, 0, 0x24, 0x34, 0x35);
    tri2(0x25, 0x36, 0x37, 0, 0x25, 0x37, 0x38);
    tri2(0x26, 0x39, 0x3A, 0, 0x26, 0x3A, 0x3B);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 effFireRingMain(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFireRingN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffFireRingWork* work;
    f32 zero;
    f32 scaleStep;

    *(char**)((s32)entry + 0x14) = str_FireRingN64_802faec8;
    *(s32*)((s32)entry + 0x8) = 0x20;
    work = __memAlloc(3, 0xA80);
    *(EffFireRingWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effFireRingMain;
    scaleStep = float_0p1_80425208 * scale;
    zero = float_0_804251e4;

    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->alpha = 0xFF;
    work->duration = duration;
    work->frame = 0;
    work->unk_30 = zero;
    work->unk_34 = zero;
    work->unk_38 = zero;
    work->unk_3c = zero;
    work->unk_40 = float_neg1p1_804251fc;
    work->unk_44 = zero;
    work->unk_48 = float_0p4_80425200;
    work->unk_4c = float_1_80425204;
    work->unk_1c = zero;
    work->scaleStep = scaleStep;
    work->colorR = 0xFF;
    work->colorG = 0xFF;
    work->colorB = 0x40;

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
