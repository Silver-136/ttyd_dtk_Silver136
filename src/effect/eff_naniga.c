#include "effect/eff_naniga.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effNanigaDisp3(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void iconDispGxCol(f32 mtx[3][4], s32 unk, s32 iconId, u32* color);
    extern u8 itemDataTable[];
    extern f32 float_16_804280ec;
    extern f32 float_deg2rad_804280f0;
    extern u32 dat_804280e4;
    extern u32 dat_804280e8;

    u32 color2;
    u32 color1;
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    f32 mtx[3][4];
    void* work;
    void* item;
    void* cam;
    f32 angle;
    f32 deg;

    work = *(void**)((s32)effect + 0xC);
    camGetPtr(cameraId);
    item = itemDataTable + *(s32*)((s32)work + 0x30) * 0x28;
    PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8) - float_16_804280ec, *(f32*)((s32)work + 0xC));
    cam = camGetPtr(cameraId);
    angle = *(f32*)((s32)cam + 0x114);
    angle = -angle;
    deg = float_deg2rad_804280f0;
    PSMTXRotRad(rotMtx, 0x79, deg * angle);
    PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x1C), *(f32*)((s32)work + 0x1C), *(f32*)((s32)work + 0x1C));
    PSMTXConcat(transMtx, rotMtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, mtx);
    if (*(s32*)((s32)work + 0x20) < 4) {
        color2 = dat_804280e4;
        iconDispGxCol(mtx, 0, 0x18C, &color2);
    } else {
        color1 = dat_804280e8;
        iconDispGxCol(mtx, 0, *(u16*)((s32)item + 0x20), &color1);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effNanigaDisp2(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void animPoseSetMaterialFlagOn(s32 poseId, s32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, u32* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, f32 mtx[3][4], s32 mode, f32 a, f32 b);
    extern f32 float_20_804280f4;
    extern f32 float_deg2rad_804280f0;
    extern f32 float_0_804280f8;
    extern f32 float_10_804280fc;
    extern u32 dat_804280e0;

    u32 evtColor;
    u32 color;
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    f32 mtx[3][4];
    void* work;
    void* cam;
    f32 angle;
    f32 deg;

    work = *(void**)((s32)effect + 0xC);
    camGetPtr(cameraId);
    PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8) - float_20_804280f4, *(f32*)((s32)work + 0xC));
    cam = camGetPtr(cameraId);
    angle = *(f32*)((s32)cam + 0x114);
    angle = -angle;
    deg = float_deg2rad_804280f0;
    PSMTXRotRad(rotMtx, 0x79, deg * angle);
    PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x1C), *(f32*)((s32)work + 0x1C), *(f32*)((s32)work + 0x1C));
    PSMTXConcat(transMtx, rotMtx, transMtx);
    PSMTXConcat(transMtx, scaleMtx, mtx);
    animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x40), 0x40);
    color = dat_804280e0;
    ((u8*)&color)[3] = *(s32*)((s32)work + 0x44);
    evtColor = color;
    animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x40), &evtColor);
    animPoseMain(*(s32*)((s32)work + 0x40));
    animPoseDrawMtx(*(s32*)((s32)work + 0x40), mtx, 1, float_0_804280f8, float_10_804280fc);
    animPoseDrawMtx(*(s32*)((s32)work + 0x40), mtx, 2, float_0_804280f8, float_10_804280fc);
    animPoseDrawMtx(*(s32*)((s32)work + 0x40), mtx, 3, float_0_804280f8, float_10_804280fc);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_naniga 20260624_184929 */

/* stub-fill: effNanigaDisp | missing_definition | ghidra_signature */
void effNanigaDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void GXGetScissor(s32*, s32*, s32*, s32*);
    extern void GXSetScissor(s32, s32, s32, s32);
    extern void effGetTexObj(s32, void*);
    extern void iconGetTexObj(void*, s32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern f32 float_deg2rad_804280f0;
    extern void* gpGlobals;
    extern u8 itemDataTable[];

    void* work = *(void**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scaleMtx[3][4];
    f32 model[3][4];
    f32 draw[3][4];
    u8 texObj[0x20];
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 oldScale = *(f32*)((s32)work + 0x1C);
    f32 size;
    f32 half = -20.0f;
    s32 oldX, oldY, oldW, oldH;
    s32 scissorSize;
    s32 count;
    s32 i;

    *(f32*)((s32)work + 0x1C) = oldScale * 1.5f;
    size = *(f32*)((s32)work + 0x1C);
    PSMTXTrans(trans, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x14), *(f32*)((s32)work + 0x18));
    PSMTXRotRad(rot, 0x79, float_deg2rad_804280f0 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114));
    PSMTXScale(scaleMtx, size, size, size);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scaleMtx, model);

    GXGetScissor(&oldX, &oldY, &oldW, &oldH);
    scissorSize = (s32)(40.0f * size);
    GXSetScissor(
        (s32)((f32)*(u16*)((s32)gpGlobals + 0x170) * 0.5f + *(f32*)((s32)work + 0x10) - 20.0f * size),
        (s32)((f32)*(u16*)((s32)gpGlobals + 0x172) * 0.5f - *(f32*)((s32)work + 0x14) - 20.0f * size),
        scissorSize, scissorSize);

    effGetTexObj(0x57, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetNumChans(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    PSMTXConcat((f32(*)[4])((s32)camera + 0x34), model, draw);
    GXLoadPosMtxImm(draw, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);
    fifo[0] = half; fifo[0] = 20.0f; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 0.0f;
    fifo[0] = 20.0f; fifo[0] = 20.0f; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 0.0f;
    fifo[0] = 20.0f; fifo[0] = half; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 1.0f;
    fifo[0] = half; fifo[0] = half; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 1.0f;

    count = *(s32*)((s32)work + 0x2C);
    for (i = 0; i < count + 5; i++) {
        s32 item = *(s32*)((s32)*(void**)((s32)work + 0x28) + (i % count) * 4);
        iconGetTexObj(texObj, *(u16*)(itemDataTable + item * 0x28 + 0x20));
        GXLoadTexObj(texObj, 0);
        PSMTXTrans(trans, 0.0f, -40.0f * (f32)i + *(f32*)((s32)work + 0x34), 0.0f);
        PSMTXScale(scaleMtx, 0.9f, 0.9f, 0.9f);
        PSMTXConcat(model, trans, trans);
        PSMTXConcat(trans, scaleMtx, trans);
        PSMTXConcat((f32(*)[4])((s32)camera + 0x34), trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 4);
        fifo[0] = half; fifo[0] = 20.0f; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 0.0f;
        fifo[0] = 20.0f; fifo[0] = 20.0f; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 0.0f;
        fifo[0] = 20.0f; fifo[0] = half; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 1.0f;
        fifo[0] = half; fifo[0] = half; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 1.0f;
    }
    GXSetScissor(oldX, oldY, oldW, oldH);
    *(f32*)((s32)work + 0x1C) = oldScale;
}

/* stub-fill: effNanigaMain | missing_definition | ghidra_signature */
void effNanigaMain(void* effect) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void effDelete(void*);
    extern void animPoseRelease(s32);
    extern s32 irand(s32);
    extern void* effKemuTestEntry(f32, f32, f32, f32, s32);
    extern void effKemuTestDrawCam(void*, s32);
    extern u32 psndSFXOn_3D(char*, Vec*);
    extern void psndSFXOff(s32);
    extern f64 intplGetValue(f64, f64, s32, s32, s32);
    extern u32 animGroupBaseAsync(char*, s32, s32);
    extern s32 animPoseEntry(char*, s32);
    extern void animPoseSetAnim(s32, char*, s32);
    extern f64 animPoseGetLoopTimes(s32);
    extern void effItemGetEntry(f32, f32, f32, s32);
    extern f32 dispCalcZ(Vec*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effNanigaDisp(void*);
    extern void effNanigaDisp2(void*);
    extern void effNanigaDisp3(void*);
    extern char str_SFX_ITEM_WHAT1_80300a28[];
    extern char str_SFX_ITEM_WHAT2_80300a38[];
    extern char str_EFF_naniga_80300a48[];
    extern char str_A_1_80428120[];
    u8* eff = (u8*)effect;
    u8* work = *(u8**)(eff + 0xC);
    Vec pos;
    s32 state;
    s32 i;

    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    if ((*(u32*)eff & 4) != 0) {
        *(u32*)eff &= ~4;
        if (*(s32*)(work + 0x40) != -1) animPoseRelease(*(s32*)(work + 0x40));
        effDelete(effect);
        return;
    }
    state = *(s32*)(work + 0x20);
    switch (state) {
        case 0:
            (*(s32*)(work + 0x20))++;
            *(s32*)(work + 0x24) = 0x1E;
        case 1:
            if (*(s32*)(work + 0x24) == 0x1E) {
                for (i = 0; i < 4; i++) {
                    void* smoke = effKemuTestEntry(*(f32*)(work + 0x10), *(f32*)(work + 0x14),
                                                   *(f32*)(work + 0x18),
                                                   0.5f + (f32)irand(200) / 100.0f, 2);
                    effKemuTestDrawCam(smoke, 8);
                }
                psndSFXOn_3D(str_SFX_ITEM_WHAT1_80300a28, &pos);
            }
            if (--*(s32*)(work + 0x24) < 0) {
                *(s32*)(work + 0x24) = 0x3C;
                (*(s32*)(work + 0x20))++;
            }
            break;
        case 2:
            if (--*(s32*)(work + 0x24) < 0) {
                *(s32*)(work + 0x24) = 0;
                (*(s32*)(work + 0x20))++;
                *(u32*)(work + 0x4C) = psndSFXOn_3D(str_SFX_ITEM_WHAT2_80300a38, &pos);
            }
            break;
        case 3:
            *(f32*)(work + 0x34) = (f32)intplGetValue(*(f32*)(work + 0x38),
                *(f32*)(work + 0x3C), 5, *(s32*)(work + 0x24), 300);
            if (++*(s32*)(work + 0x24) > 300) {
                psndSFXOff(*(s32*)(work + 0x4C));
                if (*(s32*)(work + 0x48) == 0) {
                    *(s32*)(work + 0x24) = 0x3C;
                    *(s32*)(work + 0x20) = 10;
                } else if (*(s32*)(work + 0x48) == 1) {
                    *(s32*)(work + 0x24) = 0;
                    *(s32*)(work + 0x20) = 4;
                } else if (*(s32*)(work + 0x48) == 2) {
                    *(s32*)(work + 0x24) = 0;
                    *(s32*)(work + 0x20) = 5;
                }
            }
            break;
        case 4:
            if (*(s32*)(work + 0x24) < 10) (*(s32*)(work + 0x24))++;
            else if (*(s32*)(work + 0x24) < 20) {
                *(f32*)(work + 0x34) -= 4.0f;
                (*(s32*)(work + 0x24))++;
            } else {
                *(f32*)(work + 0x34) = *(f32*)(work + 0x3C) - 40.0f;
                *(s32*)(work + 0x24) = 0x3C;
                *(s32*)(work + 0x20) = 10;
            }
            break;
        case 5:
            if (*(s32*)(work + 0x24) < 10) (*(s32*)(work + 0x24))++;
            else if (*(s32*)(work + 0x24) < 20) {
                *(f32*)(work + 0x34) += 4.0f;
                (*(s32*)(work + 0x24))++;
            } else {
                *(f32*)(work + 0x34) = *(f32*)(work + 0x3C) + 40.0f;
                *(s32*)(work + 0x24) = 0x3C;
                *(s32*)(work + 0x20) = 10;
            }
            break;
        case 10:
            if (*(s32*)(work + 0x24) != 0) (*(s32*)(work + 0x24))--;
            else if (animGroupBaseAsync(str_EFF_naniga_80300a48, 0, 0) != 0) {
                *(s32*)(work + 0x40) = animPoseEntry(str_EFF_naniga_80300a48, 0);
                animPoseSetAnim(*(s32*)(work + 0x40), str_A_1_80428120, 1);
                (*(s32*)(work + 0x20))++;
            }
            break;
        case 11:
            if (animPoseGetLoopTimes(*(s32*)(work + 0x40)) > 0.8) {
                effItemGetEntry(pos.x, pos.y, pos.z, 10);
                (*(s32*)(work + 0x20))++;
            }
            break;
        case 12:
            *(s32*)(work + 0x44) -= 4;
            if (*(s32*)(work + 0x44) < 0) {
                animPoseRelease(*(s32*)(work + 0x40));
                *(s32*)(work + 0x40) = -1;
                *(s32*)(work + 0x24) = 0x1E;
                (*(s32*)(work + 0x20))++;
            }
            break;
        case 13:
            if (*(s32*)(work + 0x24) == 0) { effDelete(effect); return; }
            (*(s32*)(work + 0x24))--;
            break;
    }
    while (*(f32*)(work + 0x34) < 0.0f)
        *(f32*)(work + 0x34) += 40.0f * (f32)*(s32*)(work + 0x2C);
    while (*(f32*)(work + 0x34) >= 40.0f * (f32)(*(s32*)(work + 0x2C) + 2))
        *(f32*)(work + 0x34) = -(40.0f * (f32)*(s32*)(work + 0x2C) - *(f32*)(work + 0x34));
    state = *(s32*)(work + 0x20);
    if ((state >= 2 && state < 6) || state == 10)
        dispEntry(8, 1, effNanigaDisp, effect, dispCalcZ(&pos));
    else if (state != 11 || animPoseGetLoopTimes(*(s32*)(work + 0x40)) <= 0.8)
        dispEntry(4, 1, effNanigaDisp3, effect, dispCalcZ(&pos));
    if (*(s32*)(work + 0x40) != -1)
        dispEntry(4, 2, effNanigaDisp2, effect, dispCalcZ(&pos));
}

/* stub-fill: effNanigaEntry | missing_definition | ghidra_signature */
void* effNanigaEntry(f32 x, f32 y, f32 z, s32 type, s32* items, s32 item) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern void effNanigaMain(void*);
    extern s32 rand(void);
    extern void animGroupBaseAsync(char*, s32, s32);
    extern void* camGetPtr(s32);
    extern void PSMTXMultVec(f32[3][4], f32*, f32*);
    extern void PSMTX44MultVec(f32[4][4], f32*, f32*);
    extern char str_Naniga_80300a54[];
    extern char str_EFF_naniga_80300a48[];
    extern void* gpGlobals;

    void* effect = effEntry();
    s32* work = __memAlloc(3, 0x50);
    void* camera;
    s32 index = 0;
    s32 random;

    *(char**)((s32)effect + 0x14) = str_Naniga_80300a54;
    *(s32*)((s32)effect + 8) = 1;
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effNanigaMain;
    *(u32*)effect |= 2;
    work[0] = type;
    *(f32*)&work[1] = x;
    *(f32*)&work[2] = y + 20.0f;
    *(f32*)&work[3] = z;
    *(f32*)&work[7] = 1.0f;
    work[8] = 0;
    work[9] = 0;
    work[10] = (s32)items;
    work[11] = 0;
    while (items[work[11]] != 0) {
        work[11]++;
    }
    work[12] = item;
    *(f32*)&work[13] = 0.0f;
    *(f32*)&work[14] = 0.0f;
    while (index < work[11] && items[index] != item) {
        index++;
    }
    *(f32*)&work[15] = (f32)index * 40.0f + (f32)work[11] * 120.0f;
    random = rand();
    if (random % 3 == 0) {
        work[18] = 1;
        *(f32*)&work[15] += 40.0f;
    } else if (random % 3 == 1) {
        work[18] = 2;
        *(f32*)&work[15] -= 40.0f;
    } else {
        work[18] = 0;
    }

    animGroupBaseAsync(str_EFF_naniga_80300a48,
                       *(s32*)((s32)gpGlobals + 0x14) != 0, 0);
    work[16] = -1;
    work[17] = 0xFF;
    *(f32*)&work[4] = *(f32*)&work[1];
    *(f32*)&work[5] = *(f32*)&work[2];
    *(f32*)&work[6] = *(f32*)&work[3];
    camera = camGetPtr(4);
    PSMTXMultVec((f32(*)[4])((s32)camera + 0x11C), (f32*)&work[4], (f32*)&work[4]);
    camera = camGetPtr(4);
    PSMTX44MultVec((f32(*)[4])((s32)camera + 0x15C), (f32*)&work[4], (f32*)&work[4]);
    *(f32*)&work[4] *= (f32)*(u16*)((s32)gpGlobals + 0x170) * 0.5f;
    *(f32*)&work[5] *= (f32)*(u16*)((s32)gpGlobals + 0x172) * 0.5f;
    return effect;
}

