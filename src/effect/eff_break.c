#include "effect/eff_break.h"

void effBreakDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];

    extern void* camGetPtr(s32 camId);
    extern void PSMTXTrans(Mtx m, f64 x, f64 y, f64 z);
    extern void PSMTXRotRad(Mtx m, char axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void GXSetNumChans(s32 n);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetTexCoordGen2(s32 coord, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void effGetTexObj(s32 texId, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 count);

    extern f32 float_deg2rad_804275f8;
    extern f32 float_48_804275fc;
    extern f32 float_0p5_80427604;
    extern f32 float_24_80427608;
    extern f32 float_0_80427600;
    extern f32 float_1_8042760c;
    extern void* gp;
    extern s32 texid_tbl;

    u8 texObj[0x20];
    Mtx transMtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    Mtx viewMtx;
    void* cam;
    void* work;
    u8* entry;
    f32 scale;
    f32 height;
    f32 halfWidth;
    s32 i;
    s32 texOffset;
    volatile f32* fifo;

    work = *(void**)((s32)effect + 0xC);
    cam = camGetPtr(cameraId);
    scale = *(f32*)((s32)work + 0x20);

    PSMTXTrans(transMtx,
               *(f32*)((s32)work + 0x4),
               *(f32*)((s32)work + 0x8),
               *(f32*)((s32)work + 0xC));

    PSMTXRotRad(rotMtx, 'y', float_deg2rad_804275f8 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114));
    PSMTXConcat(transMtx, rotMtx, transMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), transMtx, viewMtx);

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    i = 1;
    texOffset = 2;
    entry = (u8*)work + 0x24;
    fifo = (volatile f32*)0xCC008000;
    halfWidth = -float_48_804275fc * float_0p5_80427604;
    height = float_24_80427608;

    while (i < *(s32*)((s32)effect + 0x8)) {
        s32 language;
        s16* texTable;

        language = *(s32*)((s32)gp + 0x16C);
        texTable = *(s16**)((s32)&texid_tbl + (language << 3));
        effGetTexObj(*(s16*)((s32)texTable + (texOffset - 2)), texObj);
        GXLoadTexObj(texObj, 0);

        PSMTXTrans(transMtx, float_0_80427600, *(f32*)(entry + 0x14), float_0_80427600);
        GXLoadTexMtxImm(transMtx, 0x1E, 1);

        PSMTXTrans(transMtx, *(f32*)(entry + 0x4), *(f32*)(entry + 0x8), *(f32*)(entry + 0xC));
        PSMTXScale(scaleMtx, scale, scale, scale);
        PSMTXConcat(viewMtx, transMtx, transMtx);
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        GXLoadPosMtxImm(transMtx, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 4);

        fifo[0] = halfWidth;
        texOffset += 2;
        i++;
        entry += 0x24;
        fifo[0] = height;
        fifo[0] = float_0_80427600;
        fifo[0] = float_0_80427600;
        fifo[0] = float_0_80427600;
        fifo[0] = height;
        fifo[0] = height;
        fifo[0] = float_0_80427600;
        fifo[0] = float_1_8042760c;
        fifo[0] = float_0_80427600;
        fifo[0] = height;
        fifo[0] = halfWidth;
        fifo[0] = float_0_80427600;
        fifo[0] = float_1_8042760c;
        fifo[0] = float_1_8042760c;
        fifo[0] = halfWidth;
        fifo[0] = halfWidth;
        fifo[0] = float_0_80427600;
        fifo[0] = float_0_80427600;
        fifo[0] = float_1_8042760c;
    }
}



/* CHATGPT STUB FILL: main/effect/eff_break 20260624_184823 */

/* stub-fill: effBreakMain | missing_definition | ghidra_signature */
u8 effBreakMain(void) {
    return 0;
}

/* stub-fill: effBreakEntry | missing_definition | ghidra_signature */
u8 effBreakEntry(void) {
    return 0;
}
