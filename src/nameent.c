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
u8 nameWinGX(void) {
    return 0;
}

/* stub-fill: nameKirinukiGX | missing_definition | ghidra_signature */
u8 nameKirinukiGX(void) {
    return 0;
}

/* stub-fill: nameBG | missing_definition | ghidra_signature */
u8 nameBG(void) {
    return 0;
}

/* stub-fill: nameEntDisp | prototype_only | source_prototype */
void nameEntDisp(void) {
    return;
}

/* stub-fill: nameMain | prototype_only | source_prototype */
void nameMain(void) {
    return;
}

/* stub-fill: nameEntOn | missing_definition | ghidra_signature */
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

