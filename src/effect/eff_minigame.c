#include "effect/eff_minigame.h"

void effMiniGameDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct MiniGameTexRow {
        u8* ids;
        s32 count;
    } MiniGameTexRow;

    extern void* camGetPtr(s32 camId);
    extern void GXSetZMode(s32 compareEnable, s32 func, s32 updateEnable);
    extern void PSMTXTrans(Mtx m, f64 x, f64 y, f64 z);
    extern void PSMTXRotRad(Mtx m, char axis, f32 rad);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetNumChans(s32 n);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXInitTexObjLOD(void* texObj, s32 minFilt, s32 magFilt, f32 minLod, f32 maxLod, f32 lodBias, s32 biasClamp, s32 edgeLod, s32 maxAniso);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 count);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);

    extern f32 float_deg2rad_8042780c;
    extern f32 float_1_80427810;
    extern f32 float_40_80427814;
    extern f32 float_0_80427818;
    extern f32 float_20_8042781c;
    extern f32 float_neg20_80427820;
    extern f32 float_3_80427824;
    extern f32 float_neg3_80427828;
    extern f32 float_0p5_8042782c;
    extern u32 unk_804298b0;
    extern u32 dat_80427808;
    extern void* gp;
    extern MiniGameTexRow* type_tbl[];

    u8 texObj[0x20];
    Mtx finalMtx;
    Mtx scaleMtx;
    Mtx rotMtx;
    Mtx transMtx;
    u32 colorA;
    u32 colorB;
    void* cam;
    void* work;
    u8* child;
    s32 type;
    s32 count;
    s32 pass;
    s32 i;
    s32 texIndex;
    f32 baseX;
    f32 baseY;
    f32 negWidth;
    f32 half;
    f32 width;
    f32 height;
    volatile f32* fifo;

    work = *(void**)((s32)effect + 0xC);
    cam = camGetPtr(cameraId);
    type = *(s32*)((s32)work + 0x0);

    GXSetZMode(0, 3, 0);
    PSMTXTrans(transMtx, *(f32*)((s32)work + 0x4), *(f32*)((s32)work + 0x8), *(f32*)((s32)work + 0xC));
    PSMTXRotRad(rotMtx, 'y', float_deg2rad_8042780c * -*(f32*)((s32)camGetPtr(cameraId) + 0x114));
    PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x14), float_1_80427810);
    PSMTXConcat(transMtx, rotMtx, finalMtx);
    PSMTXConcat(finalMtx, scaleMtx, finalMtx);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    width = float_40_80427814;
    negWidth = -width;
    half = float_0p5_8042782c;
    height = float_20_8042781c;
    fifo = (volatile f32*)0xCC008000;
    for (pass = 0; pass < 2; pass++) {
        child = (u8*)work + 0x38;
        i = 1;
        texIndex = 0;
        while (i < *(s32*)((s32)effect + 0x8)) {
            PSMTXTrans(transMtx, float_0_80427818, float_20_8042781c, float_0_80427818);
            PSMTXRotRad(rotMtx, 'z', float_deg2rad_8042780c * *(f32*)(child + 0x1C));
            PSMTXConcat(transMtx, rotMtx, rotMtx);
            PSMTXTrans(transMtx, float_0_80427818, float_neg20_80427820, float_0_80427818);
            PSMTXConcat(rotMtx, transMtx, rotMtx);
            PSMTXTrans(transMtx, *(f32*)(child + 0x4), *(f32*)(child + 0x8), *(f32*)(child + 0xC));
            PSMTXConcat(transMtx, rotMtx, transMtx);
            PSMTXScale(scaleMtx, *(f32*)(child + 0x10), *(f32*)(child + 0x14), float_1_80427810);
            PSMTXRotRad(rotMtx, 'z', float_deg2rad_8042780c * *(f32*)(child + 0x18));
            PSMTXConcat(transMtx, rotMtx, transMtx);
            PSMTXConcat(transMtx, scaleMtx, transMtx);
            PSMTXConcat(finalMtx, transMtx, transMtx);
            if (pass == 0) {
                PSMTXTrans(scaleMtx, float_3_80427824, float_neg3_80427828, float_0_80427818);
                PSMTXConcat(transMtx, scaleMtx, transMtx);
            }
            PSMTXConcat((void*)((s32)cam + 0x11C), transMtx, transMtx);
            GXLoadPosMtxImm(transMtx, 0);
            GXSetCurrentMtx(0);

            effGetTexObj(type_tbl[type][*(s32*)((s32)gp + 0x16C)].ids[texIndex + (1 - pass)], texObj);
            if (pass != 0 && *(f32*)(child + 0x10) <= float_1_80427810) {
                GXInitTexObjLOD(texObj, 0, 0, float_0_80427818, float_0_80427818, float_0_80427818, 0, 0, 0);
            }
            GXLoadTexObj(texObj, 0);

            if (pass == 0) {
                s32 alpha = (*(s32*)(child + 0x34) * 0x8C) / 0xFF;
                colorA = unk_804298b0;
                ((u8*)&colorA)[3] = (u8)alpha;
                GXSetChanMatColor(4, &colorA);
            } else {
                s32 alpha = (*(s32*)(child + 0x34) * 0xFF) / 0xFF;
                colorB = dat_80427808;
                ((u8*)&colorB)[3] = (u8)alpha;
                GXSetChanMatColor(4, &colorB);
            }

            GXBegin(0x80, 0, 4);
            baseY = float_0_80427818;
            baseX = float_0_80427818;
            if (type == 0) {
                baseX = negWidth * float_0p5_8042782c;
            } else if (type == 1) {
                baseX = negWidth * float_0p5_8042782c;
                baseY = float_20_8042781c;
            }

            fifo[0] = negWidth * half + baseY;
            fifo[0] = width + baseX;
            fifo[0] = float_0_80427818;
            fifo[0] = float_0_80427818;
            fifo[0] = float_0_80427818;
            fifo[0] = height + baseY;
            fifo[0] = width + baseX;
            fifo[0] = float_0_80427818;
            fifo[0] = float_1_80427810;
            fifo[0] = float_0_80427818;
            fifo[0] = height + baseY;
            fifo[0] = baseX;
            fifo[0] = float_0_80427818;
            fifo[0] = float_1_80427810;
            fifo[0] = float_1_80427810;
            fifo[0] = negWidth * half + baseY;
            fifo[0] = baseX;
            fifo[0] = float_0_80427818;
            fifo[0] = float_0_80427818;
            fifo[0] = float_1_80427810;

            i++;
            texIndex += 2;
            child += 0x38;
        }
    }
}

void* effMiniGameEntry(s32 type, f32 x, f32 y, f32 z) {
    typedef struct MiniGameTexRow {
        u8* ids;
        s32 count;
    } MiniGameTexRow;

    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effMiniGameMain(void);
    extern MiniGameTexRow* type_tbl[];
    extern void* gp;
    extern char str_miniGame_802ff020[];
    extern f32 float_1_80427810;
    extern f32 float_0_80427818;
    extern f32 float_26_8042783c;
    extern f32 float_0p5_8042782c;
    extern f32 float_500_80427858;
    extern f32 float_1000_8042785c;

    void* entry;
    u8* work;
    u8* child;
    s32 lang;
    s32 countRaw;
    s32 count;
    s32 i;
    s32 state;
    s32 temp;
    f32 zero;
    f32 one;

    entry = effEntry();
    lang = *(s32*)((s32)gp + 0x16C);
    countRaw = type_tbl[type][lang].count;
    count = ((countRaw + ((u32)countRaw >> 31)) >> 1) + 1;

    *(void**)((s32)entry + 0x14) = str_miniGame_802ff020;
    *(s32*)((s32)entry + 0x8) = count;
    work = __memAlloc(3, count * 0x38);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMiniGameMain;
    *(u32*)((s32)entry + 0x0) |= 2;

    zero = float_0_80427818;
    one = float_1_80427810;

    *(s32*)(work + 0x0) = type;
    *(f32*)(work + 0x4) = x;
    *(f32*)(work + 0x8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x14) = one;
    *(f32*)(work + 0x10) = one;
    *(f32*)(work + 0x1C) = zero;
    *(s32*)(work + 0x28) = 0;
    *(s32*)(work + 0x2C) = 0;

    child = work + 0x38;
    i = 1;
    while (i < count) {
        if (type == 0) {
            if (i < 4) {
                state = 0;
            } else {
                state = 20;
            }
            *(s32*)(child + 0x30) = state;
        } else if (type == 1) {
            *(s32*)(child + 0x30) = 10;
            state = 10;
        } else if (type == 2) {
            *(s32*)(child + 0x30) = 40;
            state = 40;
        } else {
            state = 0xFF;
        }

        *(f32*)(child + 0x1C) = zero;
        *(s32*)(child + 0x34) = 0xFF;
        state = *(s32*)(child + 0x30);

        switch (state) {
            case 0:
                *(f32*)(child + 0x4) = zero;
                temp = i - 1;
                *(s32*)(child + 0x28) = temp * *(s32*)((s32)gp + 0x4);
                *(f32*)(child + 0x8) = zero;
                *(f32*)(child + 0xC) = zero;
                *(f32*)(child + 0x14) = zero;
                *(f32*)(child + 0x10) = zero;
                *(f32*)(child + 0x18) = zero;
                *(s32*)(child + 0x2C) = 0;
                *(s32*)(child + 0x34) = 0;
                break;
            case 40:
            case 30:
                if (state == 40) {
                    f32 total;
                    f32 cur;
                    temp = i - 1;
                    total = (f32)(count - 1);
                    cur = (f32)temp;
                    *(f32*)(child + 0x4) = (float_26_8042783c * cur) - ((float_26_8042783c * total) * float_0p5_8042782c);
                    *(s32*)(child + 0x28) = temp * 10;
                    *(s32*)(child + 0x30) = 50;
                } else {
                    temp = i - 1;
                    *(f32*)(child + 0x4) = zero;
                    *(s32*)(child + 0x28) = temp * *(s32*)((s32)gp + 0x4);
                }
                *(f32*)(child + 0x8) = float_500_80427858;
                *(f32*)(child + 0xC) = zero;
                *(f32*)(child + 0x14) = one;
                *(f32*)(child + 0x10) = one;
                *(f32*)(child + 0x18) = zero;
                *(s32*)(child + 0x2C) = 0;
                *(f32*)(child + 0x20) = zero;
                *(f32*)(child + 0x24) = one;
                break;
            case 10:
            case 20:
                *(f32*)(child + 0x4) = float_1000_8042785c;
                *(f32*)(child + 0x8) = zero;
                *(f32*)(child + 0xC) = zero;
                *(f32*)(child + 0x14) = one;
                *(f32*)(child + 0x10) = one;
                *(f32*)(child + 0x18) = zero;
                if (state == 10) {
                    *(s32*)(child + 0x28) = (i - 1) * 10;
                } else {
                    f32 total;
                    f32 cur;
                    *(s32*)(child + 0x28) = *(s32*)((s32)gp + 0x4) * 3;
                    *(f32*)(child + 0x14) = zero;
                    *(f32*)(child + 0x10) = zero;
                    total = (f32)(count - 3);
                    cur = (f32)(i - 3);
                    *(f32*)(child + 0x4) = (float_26_8042783c * cur) - ((float_26_8042783c * total) * float_0p5_8042782c);
                    *(s32*)(child + 0x34) = 0;
                }
                *(s32*)(child + 0x2C) = 0;
                *(f32*)(child + 0x20) = zero;
                *(f32*)(child + 0x24) = one;
                break;
            default:
                break;
        }

        i++;
        child += 0x38;
    }

    return entry;
}



/* CHATGPT STUB FILL: main/effect/eff_minigame 20260624_184823 */

/* stub-fill: effMiniGameMain | prototype_only | source_prototype */
void effMiniGameMain(void) {
    return;
}
