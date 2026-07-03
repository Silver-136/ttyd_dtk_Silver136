#include "effect/eff_levelup.h"

void effLevelupDisp(s32 cameraId, void* effect) {
    typedef f32 MtxRaw[3][4];

    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(void* mtx, double x, double y, double z);
    extern void PSMTXRotRad(void* mtx, s32 axis, double angle);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXSetNumChans(s32 num);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 num);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxFmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern u32 GXGetTexObjWidth(void* texObj);
    extern u32 GXGetTexObjHeight(void* texObj);
    extern void GXBegin(s32 prim, s32 vtxFmt, s32 count);

    extern void* gp;
    extern s32 texid_tbl[];
    extern f32 float_deg2rad_804275d8;
    extern f32 float_1_804275dc;
    extern f32 float_0p9_804275e0;
    extern f32 float_0p5_804275e4;
    extern f32 float_0_804275e8;
    extern u32 dat_804275d0;
    extern u32 dat_804275d4;

    MtxRaw transMtx;
    MtxRaw rotMtx;
    MtxRaw scaleMtx;
    MtxRaw viewMtx;
    u8 texObjLoad[0x20];
    u8 texObjSize[0x20];
    u32 colorA;
    u32 colorB;
    volatile f32* fifo;
    void* cam;
    void* cam2;
    void* workBase;
    void* work;
    s32 count;
    s32 index;
    s32 workOffset;
    s32 texOffset;
    s32 pass;
    s32 xofs;
    s32 language;
    s32 languageOffset;
    s32 texId;
    s32 texBase;
    f32 xAdd;
    f32 yAdd;
    f32 width;
    f32 height;
    f32 left;
    f32 right;
    f32 half;
    f32 angle;
    f32 deg;

    workBase = *(void**)((s32)effect + 0xC);
    cam = camGetPtr(cameraId);
    PSMTXTrans(transMtx,
        (double)*(f32*)((s32)workBase + 0x4),
        (double)*(f32*)((s32)workBase + 0x8),
        (double)*(f32*)((s32)workBase + 0xC));

    cam2 = camGetPtr(cameraId);
    angle = *(f32*)((s32)cam2 + 0x114);
    deg = float_deg2rad_804275d8;
    angle = -angle;
    PSMTXRotRad(rotMtx, 'y', (double)(deg * angle));
    PSMTXConcat(transMtx, rotMtx, transMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), transMtx, viewMtx);

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

    half = float_0p5_804275e4;
    fifo = (volatile f32*)0xCC008000;
    texBase = (s32)texid_tbl;

    pass = 1;
    xofs = 4;
    while (pass >= 0) {
        count = *(s32*)((s32)effect + 0x8);
        index = count - 1;
        workOffset = index * 0x24;
        texOffset = index << 2;
        while (index >= 1) {
            work = (void*)((s32)workBase + workOffset);
            xAdd = (f32)xofs;
            yAdd = (f32)(-pass << 2);

            language = *(s32*)((s32)gp + 0x16C);
            languageOffset = language * 0x18;
            if (pass != 0) {
                texId = *(s32*)(texBase + languageOffset + texOffset + 8);
                effGetTexObj(texId, texObjLoad);
                GXLoadTexObj(texObjLoad, 0);
                colorA = dat_804275d0;
                GXSetChanMatColor(4, &colorA);
            } else {
                texId = *(s32*)(texBase + languageOffset + texOffset - 4);
                effGetTexObj(texId, texObjLoad);
                GXLoadTexObj(texObjLoad, 0);
                colorB = dat_804275d4;
                GXSetChanMatColor(4, &colorB);
            }

            PSMTXTrans(transMtx,
                (double)(xAdd + *(f32*)((s32)work + 0x4) + *(f32*)((s32)work + 0x10)),
                (double)(*(f32*)((s32)work + 0x8) + yAdd),
                (double)*(f32*)((s32)work + 0xC));
            PSMTXConcat(viewMtx, transMtx, transMtx);
            PSMTXScale(scaleMtx,
                *(f32*)((s32)work + 0x1C),
                *(f32*)((s32)work + 0x20),
                float_1_804275dc);
            PSMTXConcat(transMtx, scaleMtx, transMtx);
            GXLoadPosMtxImm(transMtx, 0);
            GXSetCurrentMtx(0);

            language = *(s32*)((s32)gp + 0x16C);
            languageOffset = language * 0x18;
            texId = *(s32*)(texBase + languageOffset + texOffset - 4);
            effGetTexObj(texId, texObjSize);
            width = (f32)(GXGetTexObjWidth(texObjSize) & 0xFFFF);
            height = (f32)(GXGetTexObjHeight(texObjSize) & 0xFFFF);
            if (*(u32*)((s32)gp + 0x16C) == 3U) {
                width *= float_0p9_804275e0;
                height *= float_0p9_804275e0;
            }

            GXBegin(0x80, 0, 4);
            right = width * half;
            left = -width * half;

            fifo[0] = left;
            fifo[0] = height;
            fifo[0] = float_0_804275e8;
            fifo[0] = float_0_804275e8;
            fifo[0] = float_0_804275e8;

            fifo[0] = right;
            fifo[0] = height;
            fifo[0] = float_0_804275e8;
            fifo[0] = float_1_804275dc;
            fifo[0] = float_0_804275e8;

            fifo[0] = right;
            fifo[0] = float_0_804275e8;
            fifo[0] = float_0_804275e8;
            fifo[0] = float_1_804275dc;
            fifo[0] = float_1_804275dc;

            fifo[0] = left;
            fifo[0] = float_0_804275e8;
            fifo[0] = float_0_804275e8;
            fifo[0] = float_0_804275e8;
            fifo[0] = float_1_804275dc;

            workOffset -= 0x24;
            texOffset -= 4;
            index--;
        }
        pass--;
        xofs -= 4;
    }
}



/* CHATGPT STUB FILL: main/effect/eff_levelup 20260624_184823 */

/* stub-fill: effLevelupMain | missing_definition | ghidra_signature */
void effLevelupMain(void* effect) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;

    extern s32 evtEntry(void* evt, s32 flags, s32 prio);
    extern void psndSFXOn(s32 soundId);
    extern f32 intplGetValue(s32 type, f32 start, f32 end, s32 frame, s32 maxFrame);
    extern f32 dispCalcZ(VecLocal* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* dispFunc, void* data, f32 z);
    extern void effDelete(void* effect);
    extern void effLevelupDisp(s32 cameraId, void* effect);
    extern VecLocal vec3_802feef8;
    extern s16 y_trans[];
    extern u8 y_scale[];
    extern void* evt_shake;
    extern f32 float_0_804275e8;
    extern f32 float_100_804275ec;
    extern f32 float_400_804275f0;
    extern f32 float_neg400_804275f4;

    void* base = *(void**)((s32)effect + 0xC);
    void* work = (void*)((s32)base + 0x24);
    VecLocal pos = vec3_802feef8;
    s32 done = 0;
    s32 i;

    pos.x = *(f32*)((s32)base + 4);
    pos.y = *(f32*)((s32)base + 8);
    pos.z = *(f32*)((s32)base + 0xC);

    for (i = 1; i < *(s32*)((s32)effect + 8); i++, work = (void*)((s32)work + 0x24)) {
        switch (*(s32*)((s32)work + 0x18)) {
            case 0:
                *(s32*)((s32)work + 0x14) += 1;
                if (*(u32*)((s32)work + 0x14) < 0x2EU) {
                    *(f32*)((s32)work + 8) = (f32)(u16)y_trans[*(s32*)((s32)work + 0x14)];
                    if (i == 1 && *(f32*)((s32)work + 8) == float_0_804275e8) {
                        evtEntry(evt_shake, 0, 0);
                        psndSFXOn(0xA57);
                    }
                } else {
                    *(s32*)((s32)work + 0x18) += 1;
                    *(s32*)((s32)work + 0x14) = 0;
                }
                break;
            case 1:
                *(s32*)((s32)work + 0x14) += 1;
                if (*(u32*)((s32)work + 0x14) < 0x13U) {
                    if (i > 2) {
                        *(f32*)((s32)work + 0x20) =
                            (f32)y_scale[*(s32*)((s32)work + 0x14)] / float_100_804275ec;
                    }
                } else {
                    *(s32*)((s32)work + 0x18) += 1;
                    *(s32*)((s32)work + 0x14) = 0;
                }
                break;
            case 2:
                *(s32*)((s32)work + 0x14) += 1;
                if (*(s32*)((s32)work + 0x14) > *(s32*)((s32)base + 0x14)) {
                    *(s32*)((s32)work + 0x14) = 0;
                    *(s32*)((s32)work + 0x18) += 1;
                }
                break;
            case 3:
                if (i < 2) {
                    *(f32*)((s32)work + 0x10) = intplGetValue(1, float_0_804275e8, float_400_804275f0,
                                                               *(s32*)((s32)work + 0x14), 0x1E);
                } else {
                    *(f32*)((s32)work + 0x10) = intplGetValue(1, float_0_804275e8, float_neg400_804275f4,
                                                               *(s32*)((s32)work + 0x14), 0x1E);
                }
                *(s32*)((s32)work + 0x14) += 1;
                if (*(s32*)((s32)work + 0x14) > 0x1E) {
                    *(s32*)((s32)work + 0x18) += 1;
                    *(s32*)((s32)work + 0x14) = 0;
                }
                break;
            case 4:
                done++;
                break;
        }
    }

    if (done >= *(s32*)((s32)effect + 8) - 1) {
        effDelete(effect);
    } else {
        dispEntry(8, 2, effLevelupDisp, effect, dispCalcZ(&pos));
    }
}

/* stub-fill: effLevelupEntry | missing_definition | ghidra_signature */
void* effLevelupEntry(s32 type, f32 x, f32 y, f32 z, s32 wait) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effGetTexObj(s32 texId, void* texObj);
    extern u32 GXGetTexObjWidth(void* texObj);
    extern void effLevelupMain(void* effect);
    extern void* gp;
    extern s32 texid_tbl[];
    extern s16 y_trans[];
    extern const char str_Levelup_802fef18[];
    extern f32 float_0p5_804275e4;
    extern f32 float_0p9_804275e0;
    extern f32 float_1_804275dc;
    extern f32 float_0_804275e8;

    void* effect = effEntry();
    void* work;
    void* item;
    u8 texObj[0x20];
    s32 i;
    s32 j;
    s32 lang;
    f32 scale;
    f32 left;
    f32 total;

    *(const char**)((s32)effect + 0x14) = str_Levelup_802fef18;
    *(s32*)((s32)effect + 8) = 4;
    work = __memAlloc(3, *(s32*)((s32)effect + 8) * 0x24);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effLevelupMain;

    *(s32*)work = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x14) = wait;
    *(s32*)((s32)work + 0x18) = 0;

    item = (void*)((s32)work + 0x24);
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, item = (void*)((s32)item + 0x24)) {
        lang = *(s32*)((s32)gp + 0x16C);
        scale = (lang == 3) ? float_0p9_804275e0 : float_1_804275dc;
        left = float_0_804275e8;
        for (j = 0; j < i - 1; j++) {
            effGetTexObj(*(s32*)((s32)texid_tbl + (lang * 0x18) + (j * 4)), texObj);
            left += scale * (f32)(GXGetTexObjWidth(texObj) & 0xFFFF);
        }
        effGetTexObj(*(s32*)((s32)texid_tbl + (lang * 0x18) + (j * 4)), texObj);
        left += scale * ((f32)(GXGetTexObjWidth(texObj) & 0xFFFF) * float_0p5_804275e4);

        total = float_0_804275e8;
        for (j = 0; j < 3; j++) {
            effGetTexObj(*(s32*)((s32)texid_tbl + (lang * 0x18) + (j * 4)), texObj);
            total += scale * ((f32)(GXGetTexObjWidth(texObj) & 0xFFFF) * float_0p5_804275e4);
        }

        *(f32*)((s32)item + 4) = left - total;
        *(f32*)((s32)item + 8) = (f32)(u16)y_trans[0];
        *(f32*)((s32)item + 0xC) = float_0_804275e8;
        *(f32*)((s32)item + 0x10) = float_0_804275e8;
        *(s32*)((s32)item + 0x18) = 0;
        *(f32*)((s32)item + 0x1C) = float_1_804275dc;
        *(f32*)((s32)item + 0x20) = (i < 3) ? float_1_804275dc : float_0_804275e8;
    }

    return effect;
}

