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
void effBreakMain(void* effect) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern void* effStardustN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern void effStardustN64SetDrawCam(void* effect, s32 cam);
    extern f32 dispCalcZ(VecLocal* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* dispFunc, void* data, f32 z);
    extern void effDelete(void* effect);
    extern void effBreakDisp(s32 cameraId, void* effect);
    extern VecLocal vec3_802fef20;
    extern f32 ty_data[];
    extern f32 ty_data2[];
    extern f32 float_20_80427610;
    extern f32 float_0_80427600;

    void* workBase = *(void**)((s32)effect + 0xC);
    void* work = (void*)((s32)workBase + 0x24);
    VecLocal pos = vec3_802fef20;
    s32 done = 0;
    s32 i;
    f32 starScale;

    pos.x = *(f32*)((s32)workBase + 4);
    pos.y = *(f32*)((s32)workBase + 8);
    pos.z = *(f32*)((s32)workBase + 0xC);
    starScale = float_20_80427610 * *(f32*)((s32)workBase + 0x20);

    for (i = 1; i < *(s32*)((s32)effect + 8); i++, work = (void*)((s32)work + 0x24)) {
        switch (*(s32*)((s32)work + 0x1C)) {
            case 0:
            case 2:
                *(s32*)((s32)work + 0x10) -= 1;
                if (*(s32*)((s32)work + 0x10) <= 0) {
                    *(s32*)((s32)work + 0x10) = 0;
                    *(s32*)((s32)work + 0x1C) += 1;
                }
                break;
            case 1:
                if (*(u32*)((s32)work + 0x10) < 0x15U) {
                    *(f32*)((s32)work + 0x14) = ty_data[*(s32*)((s32)work + 0x10)];
                    *(s32*)((s32)work + 0x10) += 1;
                    if (*(f32*)((s32)work + 0x14) == float_0_80427600) {
                        void* dust = effStardustN64Entry(1,
                            pos.x + *(f32*)((s32)work + 4),
                            pos.y + *(f32*)((s32)work + 8),
                            pos.z + *(f32*)((s32)work + 0xC),
                            starScale);
                        effStardustN64SetDrawCam(dust, 8);
                    }
                } else {
                    *(s32*)((s32)work + 0x10) = ((*(s32*)((s32)effect + 8) - 1) - i) * 0x15;
                    *(s32*)((s32)work + 0x1C) += 1;
                }
                break;
            case 3:
                *(s32*)((s32)work + 0x10) += 1;
                if (*(s32*)((s32)work + 0x10) > *(s32*)((s32)workBase + 0x10)) {
                    *(s32*)((s32)work + 0x10) = 0;
                    *(s32*)((s32)work + 0x1C) += 1;
                }
                break;
            case 4:
                if (*(u32*)((s32)work + 0x10) < 0x15U) {
                    *(f32*)((s32)work + 0x14) = ty_data2[*(s32*)((s32)work + 0x10)];
                    if ((i & 1) == 0) {
                        *(f32*)((s32)work + 0x14) = -*(f32*)((s32)work + 0x14);
                    }
                    *(s32*)((s32)work + 0x10) += 1;
                } else {
                    *(s32*)((s32)work + 0x1C) += 1;
                }
                break;
            case 5:
                done++;
                break;
        }
    }

    if (done >= *(s32*)((s32)effect + 8) - 1) {
        effDelete(effect);
    } else {
        dispEntry(8, 2, effBreakDisp, effect, dispCalcZ(&pos));
    }
}

/* stub-fill: effBreakEntry | missing_definition | ghidra_signature */
void* effBreakEntry(s32 kind, f32 x, f32 y, f32 z, s32 timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effGetTexObj(s32 texId, void* texObj);
    extern void effBreakMain(void* effect);
    extern void* gp;
    extern s16* texid_tbl[];
    extern const char str_Break_80427614[];
    extern f32 float_2_8042761c;
    extern f32 float_36_80427624;
    extern f32 float_72_80427620;
    extern f32 float_0_80427600;
    extern f32 float_10_80427628;

    void* effect = effEntry();
    s32 language = *(s32*)((s32)gp + 0x16C);
    s32 count = texid_tbl[language][2] + 1;
    void* work;
    void* item;
    u8 texObj[0x20];
    s32 i;
    s32 j;
    f32 left;
    f32 total;

    *(const char**)((s32)effect + 0x14) = str_Break_80427614;
    *(s32*)((s32)effect + 8) = count;
    work = __memAlloc(3, count * 0x24);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effBreakMain;

    *(s32*)work = kind;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x10) = timer;
    *(f32*)((s32)work + 0x20) = float_2_8042761c;

    item = (void*)((s32)work + 0x24);
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, item = (void*)((s32)item + 0x24)) {
        left = float_0_80427600;
        for (j = 0; j < i - 1; j++) {
            effGetTexObj(texid_tbl[*(s32*)((s32)gp + 0x16C)][j], texObj);
            left += float_72_80427620;
        }
        effGetTexObj(texid_tbl[*(s32*)((s32)gp + 0x16C)][j], texObj);
        left += float_36_80427624;

        total = float_0_80427600;
        for (j = 0; j < *(s32*)((s32)effect + 8) - 1; j++) {
            effGetTexObj(texid_tbl[*(s32*)((s32)gp + 0x16C)][j], texObj);
            total += float_36_80427624;
        }

        *(f32*)((s32)item + 4) = left - total;
        *(f32*)((s32)item + 8) = float_0_80427600;
        *(f32*)((s32)item + 0xC) = float_0_80427600;
        *(f32*)((s32)item + 0x14) = float_10_80427628;
        *(s32*)((s32)item + 0x18) = 0;
        *(s32*)((s32)item + 0x10) = (i - 1) * 0x15;
        *(s32*)((s32)item + 0x1C) = 0;
    }

    return effect;
}

