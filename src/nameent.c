#include "nameent.h"

extern void* gp;
extern void* wp;
extern const char* lbl_802FE300[];
extern const char* lbl_802FE318[];
extern const char str_PCTs_w_PCTs_name_tpl_802fe4ac[];
extern char str___80426910[];
extern char str__80426d80[];
extern f32 float_0_80426ddc;

char* getMarioStDvdRoot(void);
s32 fileAsyncf(s32 size, s32 flags, const char* fmt, ...);
void* fileAllocf(s32 size, const char* fmt, ...);
void fileFree(void* file);
void nameMain(void);
void nameEntDisp(void);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, s32 param, f32 priority);
s32 strncmp(const char* s1, const char* s2, u32 n);

void nameEntOff(void) {
    extern void* wp;

    *(s32*)((s32)wp + 4) = 3;
}

s32 nameEntIsCancel(void) {
    extern void* wp;

    return (*(u16*)wp >> 1) & 1;
}

void nameEntReInit(void) {
    extern void* wp;

    *(s32*)((s32)wp + 4) = 3;
}

void nameEntInit(void) {
    extern void* wp;
    extern void* memset(void* ptr, s32 value, u32 size);

    memset(wp, 0, 0x64);
}

s32 nameEntWait(void) {
    extern void* wp;
    s32 value;

    *(u16*)wp |= 1;
    value = *(s32*)((s32)wp + 8);
    return (u32)((0x65 - value) | (value - 0x65)) >> 31;
}

void nameEntMain(void) {
    const char* table[6];
    s32 loaded;

    switch (*(s32*)((s32)wp + 4)) {
        case 0:
            break;
        case 1:
            table[0] = lbl_802FE318[0];
            table[1] = lbl_802FE318[1];
            table[2] = lbl_802FE318[2];
            table[3] = lbl_802FE318[3];
            table[4] = lbl_802FE318[4];
            table[5] = lbl_802FE318[5];
            if ((u32)fileAsyncf(4, 0, str_PCTs_w_PCTs_name_tpl_802fe4ac, getMarioStDvdRoot(), table[*(s32*)((s32)gp + 0x16C)]) != 0) {
                *(void**)((s32)wp + 0x60) = fileAllocf(4, str_PCTs_w_PCTs_name_tpl_802fe4ac, getMarioStDvdRoot(), table[*(s32*)((s32)gp + 0x16C)]);
                loaded = 1;
            } else {
                loaded = 0;
            }
            if (loaded != 0) {
                *(s32*)((s32)wp + 4) = *(s32*)((s32)wp + 4) + 1;
            }
        case 2:
            nameMain();
            break;
        case 3:
            if (*(void**)((s32)wp + 0x60) != NULL) {
                fileFree(*(void**)((s32)wp + 0x60));
            }
            *(void**)((s32)wp + 0x60) = NULL;
            *(s32*)((s32)wp + 4) = 0;
            break;
        default:
            break;
    }

    if (*(s32*)((s32)wp + 4) > 1) {
        dispEntry(4, 7, nameEntDisp, 0, float_0_80426ddc);
    }
}

#pragma use_lmw_stmw off
char* nameEntGetName(void) {
    s32 count;
    s32 offset;
    s32 zero;

    if (*(s32*)((s32)wp + 8) != 0x65) {
        return NULL;
    }
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        count = 7;
        offset = 0xE;
        zero = 0;
        while (strncmp((char*)((s32)wp + offset + 0x3C), str___80426910, 2) == 0) {
            count--;
            *(u8*)((s32)wp + offset + 0x3C) = zero;
            *(u8*)((s32)wp + offset + 0x3D) = zero;
            offset -= 2;
            if (count < 1) {
                break;
            }
        }
    } else {
        offset = 7;
        zero = 0;
        while (strncmp((char*)((s32)wp + offset + 0x3C), str__80426d80, 1) == 0) {
            *(u8*)((s32)wp + offset + 0x3C) = zero;
            offset--;
            if (offset < 1) {
                break;
            }
        }
    }
    return (char*)((s32)wp + 0x3C);
}
#pragma use_lmw_stmw on

s32 nameEntPrepare(void) {
    const char* table[6];
    s32 result;

    table[0] = lbl_802FE300[0];
    table[1] = lbl_802FE300[1];
    table[2] = lbl_802FE300[2];
    table[3] = lbl_802FE300[3];
    table[4] = lbl_802FE300[4];
    table[5] = lbl_802FE300[5];
    result = fileAsyncf(4, 0, str_PCTs_w_PCTs_name_tpl_802fe4ac, getMarioStDvdRoot(), table[*(s32*)((s32)gp + 0x16C)]);
    return (u32)(-result | result) >> 31;
}

void nameMaskGX(f32 scale) {
    typedef f32 Mtx[3][4];

    extern f32 float_1000_80426de8;
    extern f32 float_0_80426ddc;
    extern f32 float_1_80426de4;
    extern f32 float_0p5_80426dec;
    extern f32 float_neg0p5_80426df0;
    extern u32 dat_80426dd0;

    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZMode(s32 compareEnable, s32 func, s32 updateEnable);
    extern void GXSetNumChans(s32 num);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevColor(s32 reg, void* color);
    extern s32 fadeGetTpl(void);
    extern void TEXGetGXTexObjFromPalette(s32 tpl, void* texObj, s32 index);
    extern void GXInitTexObjLOD(void* texObj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 doEdgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetNumTexGens(s32 num);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void GXLoadTexMtxImm(Mtx m, s32 id, s32 type);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void* camGetPtr(s32 cameraId);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);

    u32 color;
    u8 texObj[0x20];
    Mtx transA;
    Mtx scaleMtx;
    Mtx transB;
    f32 amount;
    f32 zero;
    f32 one;
    volatile u16* fifo16;
    volatile f32* fifoF;
    void* cam;

    amount = float_1000_80426de8 * scale;
    amount = amount * scale;
    amount = scale * amount;
    amount = scale * amount;
    amount = scale * amount;

    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(0, 7, 0);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);

    color = dat_80426dd0;
    GXSetTevColor(1, &color);

    TEXGetGXTexObjFromPalette(fadeGetTpl(), texObj, 1);

    zero = float_0_80426ddc;
    GXInitTexObjLOD(texObj, 1, 1, zero, zero, zero, 0, 0, 0);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);

    PSMTXTrans(transA, (double)float_0p5_80426dec, (double)float_0p5_80426dec, (double)zero);
    PSMTXScale(scaleMtx, amount, amount, zero);
    PSMTXTrans(transB, (double)float_neg0p5_80426df0, (double)float_neg0p5_80426df0, (double)zero);
    PSMTXConcat(transA, scaleMtx, transA);
    PSMTXConcat(transA, transB, transA);

    GXLoadTexMtxImm(transA, 0x1E, 1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);

    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    cam = camGetPtr(8);
    GXLoadPosMtxImm((void*)((s32)cam + 0x11C), 0);
    GXSetCurrentMtx(0);

    GXBegin(0x80, 0, 4);

    fifo16 = (volatile u16*)0xCC008000;
    fifoF = (volatile f32*)0xCC008000;
    one = float_1_80426de4;

    *fifo16 = (u16)-0x130;
    *fifo16 = 0xF0;
    *fifo16 = 0;
    *fifoF = zero;
    *fifoF = zero;

    *fifo16 = 0x130;
    *fifo16 = 0xF0;
    *fifo16 = 0;
    *fifoF = one;
    *fifoF = zero;

    *fifo16 = 0x130;
    *fifo16 = (u16)-0xF0;
    *fifo16 = 0;
    *fifoF = one;
    *fifoF = one;

    *fifo16 = (u16)-0x130;
    *fifo16 = (u16)-0xF0;
    *fifo16 = 0;
    *fifoF = zero;
    *fifoF = one;
}


/* CHATGPT STUB FILL: main/nameent 20260624_184008 */

/* stub-fill: nameWinGX | missing_definition | ghidra_signature */
u8 nameWinGX(f64 x, f64 y, f64 width, f64 height, s32 palette, s32 texture) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void TEXGetGXTexObjFromPalette(s32, void*, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);
    extern u16 GXGetTexObjWidth(void*);
    extern u16 GXGetTexObjHeight(void*);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetFog(s32, f32, f32, f32, f32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevKAlphaSel(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXBegin(s32, s32, s32);
    u8 texObj[0x20];
    Mtx texMtx;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 right = (f32)(x + width - 16.0);
    f32 bottom = (f32)(y - height + 16.0);
    s32 ids[4] = { 4, 7, 6, 3 };
    u32 fogColor = 0xFFFFFFFF;
    u32 tevColor = 0x00000080;
    s32 i;

    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetBlendMode(1, 4, 5, 7);
    GXSetZMode(0, 7, 0);
    GXSetFog(0, 0.0f, 0.0f, 0.0f, 0.0f, &fogColor);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0, 0, 0, 8);
    GXSetTevAlphaIn(0, 0, 0, 0, 7);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0, 0xF, 10, 0);
    GXSetTevAlphaIn(1, 0, 4, 4, 7);
    GXSetTevColor(1, &tevColor);
    GXSetTevSwapMode(1, 0, 0);
    GXSetTevOrder(2, 1, 2, 0xFF);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0xF, 8, 10, 0);
    GXSetTevAlphaIn(2, 7, 6, 4, 7);
    GXSetTevKAlphaSel(2, 0);
    GXSetTevSwapMode(2, 0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(1) + 0x118, 0);
    GXSetCurrentMtx(0);

#pragma unroll 4
    for (i = 0; i < 4; i++) {
        f32 left = i == 1 || i == 3 ? right : (f32)x;
        f32 top = i >= 2 ? bottom + 16.0f : (f32)y;
        f32 quadRight = left + (i == 1 || i == 3 ? 16.0f : (f32)width - 16.0f);
        f32 quadBottom = top - (i >= 2 ? 16.0f : (f32)height - 16.0f);
        TEXGetGXTexObjFromPalette(palette, texObj, texture + ids[i]);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(2);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   __fabs(quadRight - left) / (f32)GXGetTexObjWidth(texObj),
                   __fabs(top - quadBottom) / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        *fifo = left; *fifo = top; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
        *fifo = left; *fifo = quadBottom; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
        *fifo = quadRight; *fifo = quadBottom; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
        *fifo = quadRight; *fifo = top; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
    }
    return 0;
}

/* stub-fill: nameKirinukiGX | missing_definition | ghidra_signature */
u8 nameKirinukiGX(f64 x, f64 y, f64 width, f64 height) {
    typedef f32 Mtx[3][4];
    extern void* wp;
    extern void* camGetPtr(s32);
    extern void TEXGetGXTexObjFromPalette(s32, void*, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, s32);
    extern u16 GXGetTexObjWidth(void*);
    extern u16 GXGetTexObjHeight(void*);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXBegin(s32, s32, s32);
    u8 texObj[0x20];
    Mtx texMtx;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    s32 textureIds[8] = { 0x2F, 0x2D, 0x31, 0x2E, 0x32, 0x30, 0x34, 0x33 };
    s32 palette = ***(s32***)((u8*)wp + 0x60);
    s32 i;

    GXSetBlendMode(0, 1, 0, 0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6, 0x80, 1, 0, 0);
    GXSetZMode(0, 7, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXLoadPosMtxImm((u8*)camGetPtr(1) + 0x118, 0);
    GXSetCurrentMtx(0);

    for (i = 0; i < 8; i++) {
        f32 left = (f32)x + ((i & 1) ? (f32)width - 8.0f : 0.0f);
        f32 top = (f32)y - ((i & 2) ? (f32)height - 8.0f : 0.0f);
        f32 right = left + ((i & 1) ? 8.0f : (f32)width - 8.0f);
        f32 bottom = top - ((i & 2) ? 8.0f : (f32)height - 8.0f);
        TEXGetGXTexObjFromPalette(palette, texObj, textureIds[i]);
        GXInitTexObjLOD(texObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx,
                   __fabs(right - left) / (f32)GXGetTexObjWidth(texObj),
                   __fabs(top - bottom) / (f32)GXGetTexObjHeight(texObj), 1.0f);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        *fifo = left; *fifo = top; *fifo = 0.0f; *fifo = 0.0f; *fifo = 0.0f;
        *fifo = left; *fifo = bottom; *fifo = 0.0f; *fifo = 0.0f; *fifo = 1.0f;
        *fifo = right; *fifo = bottom; *fifo = 0.0f; *fifo = 1.0f; *fifo = 1.0f;
        *fifo = right; *fifo = top; *fifo = 0.0f; *fifo = 1.0f; *fifo = 0.0f;
    }
    return 0;
}

/* stub-fill: nameBG | missing_definition | ghidra_signature */
void nameBG(void)  {
    extern void* wp;
    extern void* camGetPtr(s32);
    extern void GXSetBlendMode(s32,s32,s32,s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);
    extern void GXSetZMode(s32,s32,s32);
    extern void GXSetFog(s32,f32,f32,f32,f32,void*);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevOp(s32,s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void GXLoadPosMtxImm(void*,s32);
    extern void GXSetCurrentMtx(s32);
    extern void TEXGetGXTexObjFromPalette(void*,void*,s32);
    extern void GXInitTexObjLOD(void*,s32,s32,f32,f32,f32,s32,s32,s32);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern u32 GXGetTexObjHeight(void*);
    extern u32 GXGetTexObjWidth(void*);
    extern void PSMTXScale(void*,f32,f32,f32);
    extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void GXBegin(s32,s32,s32);
    f32 texMtx[3][4];
    u8 texObj[0x20];
    u32 fog=0;
    void* camera;
    void* palette;
    volatile f32* fifo=(volatile f32*)0xCC008000;
    f32 left[3]= {
        -304.0f,-272.0f,272.0f
    }
    ;
    f32 right[3]= {
        -272.0f,272.0f,304.0f
    }
    ;
    s32 indices[3]= {
        0x22,0x21,0x23
    }
    ;
    f32 widths[3]= {
        32.0f,544.0f,32.0f
    }
    ;
    s32 i;
    f32 sx,sy;
    GXSetBlendMode(0,1,0,0);
    GXSetZCompLoc(0);
    GXSetAlphaCompare(6,0x80,1,0,0);
    GXSetZMode(0,7,0);
    GXSetFog(0,0.0f,0.0f,0.0f,0.0f,&fog);
    GXSetNumChans(0);
    GXSetChanCtrl(4,0,0,0,0,0,2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0,0,0,0xFF);
    GXSetTevOp(0,3);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9,1);
    GXSetVtxDesc(13,1);
    GXSetVtxAttrFmt(0,9,1,4,0);
    GXSetVtxAttrFmt(0,13,1,4,0);
    camera=camGetPtr(1);
    GXLoadPosMtxImm((u8*)camera+0x11C,0);
    GXSetCurrentMtx(0);
    palette=*(void**)(*(s32*)((s32)wp+0x60));
    for(i=0;i<3;i++) {
        TEXGetGXTexObjFromPalette(palette,texObj,indices[i]);
        GXInitTexObjLOD(texObj,1,1,0.0f,0.0f,0.0f,0,0,0);
        GXLoadTexObj(texObj,0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
        sx=widths[i]/(f32)(GXGetTexObjWidth(texObj)&0xFFFF);
        sy=480.0f/(f32)(GXGetTexObjHeight(texObj)&0xFFFF);
        PSMTXScale(texMtx,sx,sy,1.0f);
        GXLoadTexMtxImm(texMtx,0x1E,1);
        GXBegin(0x80,0,4);
        *fifo=left[i];
        *fifo=240.0f;
        *fifo=0.0f;
        *fifo=0.0f;
        *fifo=0.0f;
        *fifo=left[i];
        *fifo=-240.0f;
        *fifo=0.0f;
        *fifo=0.0f;
        *fifo=1.0f;
        *fifo=right[i];
        *fifo=-240.0f;
        *fifo=0.0f;
        *fifo=1.0f;
        *fifo=1.0f;
        *fifo=right[i];
        *fifo=240.0f;
        *fifo=0.0f;
        *fifo=1.0f;
        *fifo=0.0f;
    }
}

/* stub-fill: nameEntDisp | prototype_only | source_prototype */
void nameEntDisp(void) {
    typedef f32 Mtx[3][4];
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* wp;
    extern void* camGetCurPtr(void);
    extern void* camGetPtr(s32);
    extern void GXGetProjectionv(f32*);
    extern void GXGetViewportv(f32*);
    extern void GXSetProjection(void*, s32);
    extern void GXSetProjectionv(f32*);
    extern void GXSetViewport(f32, f32, f32, f32, f32, f32);
    extern void winTexInit(s32);
    extern void winTexSet(s32, Vec*, Vec*, void*);
    extern void nameBG(void);
    extern void nameKirinukiGX(f64, f64, f64, f64);
    extern void nameWinGX(f64, f64, f64, f64, s32, s32);
    extern void FontDrawStart(void);
    extern void FontDrawString(f32, f32, const char*);
    extern void FontDrawColor(void*);
    extern void FontDrawEdge(void);
    extern void FontDrawEdgeOff(void);
    extern u16 FontGetMessageWidth(const char*);
    extern void* memset(void*, s32, u32);
    extern char* strncpy(char*, const char*, u32);
    extern u32 strlen(const char*);
    extern char* msgSearch(const char*);
    extern void windowDispGX_Waku_col(f64, f64, f64, f64, f64, s32, void*);
    extern void nameMaskGX(f64);
    extern char* msg_tbl[];
    extern char str_msg_nameent_ok_802fe450[];
    extern char str_msg_nameent_yes_802fe460[];
    extern char str_msg_nameent_no_802fe470[];
    extern char str_msg_nameent_ok_01_802fe480[];
    u8* work = wp;
    f32 projection[7];
    f32 viewport[6];
    Mtx saved;
    Vec pos, scale;
    u32 color = 0xFFFFFFFF;
    u32 messageColor = 0xFFFFFFFF;
    char glyph[16];
    s32 file = ***(s32***)(work + 0x60);
    s32 i;

    GXGetProjectionv(projection);
    GXGetViewportv(viewport);
    for (i = 0; i < 12; i++) {
        ((f32*)saved)[i] = *(f32*)((u8*)camGetCurPtr() + 0x118 + i * 4);
    }
    GXSetProjection((u8*)camGetPtr(1) + 0x160, *(s32*)((u8*)camGetPtr(1) + 0x1A0));

    nameBG();
    winTexInit(file);
    pos.x = -240.0f; pos.y = 120.0f; pos.z = 0.0f;
    scale.x = 480.0f; scale.y = 270.0f; scale.z = 1.0f;
    winTexSet(0x2A, &pos, &scale, &color);
    for (i = 0; i < 8; i++) {
        pos.x = -250.0f + 40.0f * (f32)i;
        pos.y = 20.0f;
        scale.x = 28.0f; scale.y = 28.0f; scale.z = 1.0f;
        winTexSet(0x29, &pos, &scale, &color);
    }
    pos.x = -160.0f; pos.y = -160.0f;
    scale.x = 500.0f; scale.y = 40.0f;
    winTexSet(0x2B, &pos, &scale, &color);
    nameKirinukiGX(-240.0, 120.0, 480.0, 270.0);

    FontDrawStart();
    for (i = 0; i < 8; i++) {
        memset(glyph, 0, sizeof(glyph));
        if (*(u8*)(work + 0x3C + i) >= 0x80) {
            strncpy(glyph, (char*)work + 0x3C + i, 2);
            i++;
        } else {
            strncpy(glyph, (char*)work + 0x3C + i, 1);
        }
        if (strlen(glyph) != 0) {
            FontDrawString(-220.0f + 40.0f * (f32)i, -100.0f, glyph);
        }
    }

    {
        const char* message = msgSearch(msg_tbl[*(s32*)(work + 0x58)]);
        f32 messageWidth = (f32)FontGetMessageWidth(message);
        windowDispGX_Waku_col(-250.0, -160.0, 500.0, 40.0, 20.0, 0, &messageColor);
        FontDrawStart();
        FontDrawString((240.0f - messageWidth) * 0.5f - 120.0f, -168.0f, message);
        nameMaskGX((f64)*(f32*)(work + 0x54));
    }
    if (*(s32*)(work + 8) == 100) {
        const char* ok = msgSearch(str_msg_nameent_ok_802fe450);
        const char* yes = msgSearch(str_msg_nameent_yes_802fe460);
        const char* no = msgSearch(str_msg_nameent_no_802fe470);
        u32 textColor = 0xFFFFFFFF;
        if (*(s32*)(work + 0x58) == 4) {
            ok = msgSearch(str_msg_nameent_ok_01_802fe480);
        }
        nameWinGX(-180.0, 130.0, 360.0, 100.0, file, 1);
        nameWinGX(-80.0, 10.0, 160.0, 120.0, file, 1);
        FontDrawStart();
        FontDrawColor(&textColor);
        FontDrawEdge();
        FontDrawString(-(f32)FontGetMessageWidth(ok) * 0.5f, 110.0f, ok);
        FontDrawString(-(f32)FontGetMessageWidth((char*)work + 0x3C) * 0.5f,
                       70.0f, (char*)work + 0x3C);
        FontDrawString(-(f32)FontGetMessageWidth(no) * 0.5f, -20.0f, yes);
        FontDrawString(-(f32)FontGetMessageWidth(no) * 0.5f, -60.0f, no);
        FontDrawEdgeOff();
    }
    winTexInit(file);
    pos.x = *(f32*)(work + 0x1C) + 2.0f;
    pos.y = *(f32*)(work + 0x20) - 2.0f;
    pos.z = 0.0f;
    scale.x = 32.0f;
    scale.y = 32.0f;
    scale.z = 1.0f;
    winTexSet(0, &pos, &scale, &color);
    pos.x = *(f32*)(work + 0x1C);
    pos.y = *(f32*)(work + 0x20);
    winTexSet(0, &pos, &scale, &color);
    GXSetProjectionv(projection);
    GXSetViewport(viewport[0], viewport[1], viewport[2], viewport[3], viewport[4], viewport[5]);
}

/* stub-fill: nameMain | prototype_only | source_prototype */
void nameMain(void) {
    extern void* wp;
    extern void* gp;
    extern f64 distABf(f64, f64, f64, f64);
    extern u32 keyGetButtonTrg(s32);
    extern u32 keyGetDirRep(s32);
    extern void psndSFXOn(void*);
    u8* work = wp;
    f32 dist = (f32)distABf(*(f32*)(work + 0x24), *(f32*)(work + 0x28),
                            *(f32*)(work + 0x1C), *(f32*)(work + 0x20));
    u32 buttons;
    u32 direction;
    s32 state = *(s32*)(work + 8);

    if (state == 100) {
        buttons = keyGetButtonTrg(0);
        if ((buttons & 0x100) != 0) {
            if (*(s32*)(work + 0x5C) == 0) {
                *(s32*)(work + 8) = 101;
                psndSFXOn((void*)0x12);
            } else {
                *(s32*)(work + 8) = 0;
                psndSFXOn((void*)0x13);
            }
        } else if ((buttons & 0x200) != 0) {
            *(s32*)(work + 8) = 0;
            psndSFXOn((void*)0x13);
        } else {
            direction = keyGetDirRep(0);
            if ((direction & 0x3000) != 0) {
                *(s32*)(work + 0x5C) = 1 - *(s32*)(work + 0x5C);
                psndSFXOn((void*)5);
            }
        }
        *(f32*)(work + 0x24) = -60.0f;
        *(f32*)(work + 0x28) = 30.0f - 40.0f * (f32)*(s32*)(work + 0x5C);
        *(f32*)(work + 0x54) += 0.05f;
        if (*(f32*)(work + 0x54) > 1.0f) *(f32*)(work + 0x54) = 1.0f;
    } else if (state == 0 && (*(u16*)work & 1) != 0 && *(s32*)((u8*)gp + 8) == 0) {
        buttons = keyGetButtonTrg(0);
        direction = keyGetDirRep(0);
        if ((buttons & 0x100) != 0 || (buttons & 0x1000) != 0) {
            *(s32*)(work + 8) = 100;
            *(s32*)(work + 0x5C) = 0;
            psndSFXOn((void*)0x12);
        } else if ((buttons & 0x200) != 0) {
            *(s32*)(work + 8) = 100;
            *(s32*)(work + 0x5C) = 1;
            psndSFXOn((void*)0x13);
        } else if ((direction & 0x4000) != 0 && dist < 10.0f) {
            (*(s32*)(work + 0x14))--;
            if (*(s32*)(work + 0x14) < 0) *(s32*)(work + 0x14) = 8;
            psndSFXOn((void*)5);
        } else if ((direction & 0x8000) != 0 && dist < 10.0f) {
            (*(s32*)(work + 0x14))++;
            if (*(s32*)(work + 0x14) > 8) *(s32*)(work + 0x14) = 0;
            psndSFXOn((void*)5);
        } else if ((direction & 0x1000) != 0 && dist < 10.0f) {
            (*(s32*)(work + 0x18))--;
            if (*(s32*)(work + 0x18) < 0) *(s32*)(work + 0x18) = 13;
            psndSFXOn((void*)5);
        } else if ((direction & 0x2000) != 0 && dist < 10.0f) {
            (*(s32*)(work + 0x18))++;
            if (*(s32*)(work + 0x18) > 13) *(s32*)(work + 0x18) = 0;
            psndSFXOn((void*)5);
        }
    }
    *(f32*)(work + 0x24) += (*(f32*)(work + 0x1C) - *(f32*)(work + 0x24)) * 0.5f;
    *(f32*)(work + 0x28) += (*(f32*)(work + 0x20) - *(f32*)(work + 0x28)) * 0.5f;
}

/* stub-fill: nameEntOn | missing_definition | ghidra_signature */
#pragma optimize_for_size off
void nameEntOn(int param_1) {
    extern char* strcpy(char* dst, const char* src);
    extern char* strcat(char* dst, const char* src);
    extern char lbl_803AE790[];
    extern char str___80426910[];
    extern char str__80426d80[];
    extern char str_p_80426c8c[];
    extern char float_0_80426e94[];
    extern f32 float_neg106_80426e90;
    extern f32 float_135_80426e8c;
    s32 i;
    s32 col;
    s32 row;
    f32 x;
    f32 y;
    char* fill;

    *(u16*)wp = 0;
    *(s32*)((s32)wp + 0xC) = param_1;
    *(s32*)((s32)wp + 4) = 1;
    *(s32*)((s32)wp + 8) = 0;
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        *(void**)((s32)wp + 0x50) = lbl_803AE790 + 0x40;
    } else {
        *(void**)((s32)wp + 0x50) = lbl_803AE790 + 0x430;
    }

    *(s32*)((s32)wp + 0x18) = 0;
    *(s32*)((s32)wp + 0x10) = 0;
    *(s32*)((s32)wp + 0x14) = 0;
    col = *(s32*)((s32)wp + 0x10);
    x = (f32)(col * 0x18 + (col / 5) * 0x1E - 0xBE);
    *(f32*)((s32)wp + 0x24) = x;
    *(f32*)((s32)wp + 0x1C) = x;
    row = *(s32*)((s32)wp + 0x14);
    y = (f32)(0x70 - row * 0x1C);
    *(f32*)((s32)wp + 0x28) = y;
    *(f32*)((s32)wp + 0x20) = y;
    *(f32*)((s32)wp + 0x34) = float_neg106_80426e90;
    *(f32*)((s32)wp + 0x2C) = float_neg106_80426e90;
    *(f32*)((s32)wp + 0x38) = float_135_80426e8c;
    *(f32*)((s32)wp + 0x30) = float_135_80426e8c;

    strcpy((char*)((s32)wp + 0x3C), float_0_80426e94);
    for (i = 0; i < 8; i++) {
        fill = str__80426d80;
        if (*(u32*)((s32)gp + 0x16C) == 0) {
            fill = str___80426910;
        }
        strcat((char*)((s32)wp + 0x3C), fill);
    }
    *(f32*)((s32)wp + 0x54) = float_0_80426ddc;

    if (*(u32*)((s32)gp + 0x16C) == 0) {
        if (param_1 == 2) {
            *(char**)(lbl_803AE790 + 0x64) = str___80426910;
            *(char**)(lbl_803AE790 + 0x25C) = str___80426910;
        } else {
            *(char**)(lbl_803AE790 + 0x64) = (char*)0x80426918;
            *(char**)(lbl_803AE790 + 0x25C) = (char*)0x80426AA8;
        }
    } else if (param_1 == 2) {
        *(char**)(lbl_803AE790 + 0x4EC) = str__80426d80;
    } else {
        *(char**)(lbl_803AE790 + 0x4EC) = str_p_80426c8c;
    }
}
#pragma optimize_for_size on

