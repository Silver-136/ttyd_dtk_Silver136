#include "effect/eff_sandars.h"

/* CHATGPT STUB FILL: main/effect/eff_sandars 20260624_184929 */

/* stub-fill: effSandarsEntry | missing_definition | ghidra_signature */
void* effSandarsEntry(s32 type, f32 x, f32 y, f32 z, f32 angle) {
    extern void* gp;
    extern void* effEntry(s32 type);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 animGroupBaseAsync(const char* name, s32 heap, s32 flags);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern void effSandarsMain(void);
    extern char str_Sandars_80300a88[];
    extern char str_EFF_bakuhatu_80300a6c[];
    extern f32 float_1_8042814c;
    extern f32 float_4_80428150;
    extern f32 float_6p2832_8042815c;
    extern f32 float_360_80428160;
    extern f32 float_0_80428148;
    extern f32 float_0p2_80428164;
    extern f32 float_51_80428168;
    extern f32 float_1p1_8042816c;
    extern f32 float_15_80428170;

    void* effect;
    void* work;
    void* part;
    s32 count;
    s32 heap;
    s32 i;
    s32 radius;
    f32 radians;

    effect = effEntry(type);
    count = (type == 5) ? 1 : 6;
    heap = (*(s32*)((s32)gp + 0x14) != 0);
    *(char**)((s32)effect + 0x14) = str_Sandars_80300a88;
    *(s32*)((s32)effect + 8) = count;
    work = __memAlloc(3, count * 0x44);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effSandarsMain;
    *(u32*)effect |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = float_1_8042814c;
    *(s32*)((s32)work + 0x14) = 0;
    *(s32*)((s32)work + 0x18) = 0;
    *(s32*)((s32)work + 0x1C) = -1;
    *(s32*)((s32)work + 0x40) = 4;

    switch (type) {
        case 0:
        case 4:
            *(s32*)((s32)work + 0x24) = 1;
            *(f32*)((s32)work + 0x28) = float_1_8042814c;
            break;
        case 1:
        case 2:
            *(s32*)((s32)work + 0x24) = 2;
            *(f32*)((s32)work + 0x28) = float_4_80428150;
            break;
        case 5:
            *(s32*)((s32)work + 0x18) = 0x64;
            *(f32*)((s32)work + 0x30) = angle;
            break;
    }

    if (type == 3 || type == 4) {
        animGroupBaseAsync(str_EFF_bakuhatu_80300a6c, heap, 0);
        *(s32*)((s32)work + 0x1C) = -1;
        *(s32*)((s32)work + 0x20) = 0xFF;
    }

    radians = (float_6p2832_8042815c * angle) / float_360_80428160;
    part = (void*)((s32)work + 0x44);
    radius = 10;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, part = (void*)((s32)part + 0x44), radius += 10) {
        *(f32*)((s32)part + 4) = (f32)radius * (f32)cos(radians);
        *(f32*)((s32)part + 8) = (f32)radius * (f32)sin(radians);
        *(f32*)((s32)part + 0xC) = float_0_80428148;
        *(f32*)((s32)part + 0x2C) = float_0p2_80428164 * (f32)i;
        *(f32*)((s32)part + 0x10) = float_0_80428148;
        *(s32*)((s32)part + 0x20) = (s32)(float_51_80428168 * (f32)i);
        *(s32*)((s32)part + 0x34) = (i - 1) * 5;
        *(s32*)((s32)part + 0x18) = 0;
        *(f32*)((s32)part + 0x38) = float_0_80428148;
        *(f32*)((s32)part + 0x3C) = float_0_80428148;
        if (i == *(s32*)((s32)effect + 8) - 1) {
            *(f32*)((s32)part + 0x2C) = float_1p1_8042816c;
            *(s32*)((s32)part + 0x20) = 0xCC;
            *(f32*)((s32)part + 0x3C) = float_15_80428170;
        }
    }

    return effect;
}

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_sandars 20260624_191429 */

/* fallback stub-fill: map=effSandarsMain addr=0x80241d68 size=0x000002dc */
void effSandarsMain(void* effect) {
    extern void* gp;
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* effect);
    extern void* effBombN64Entry(s32 type);
    extern void effBombN64SetCamId(void* eff, s32 camId);
    extern s32 animGroupBaseAsync(const char* name, s32 heap, s32 flags);
    extern s32 animPoseEntry(const char* name, s32 heap);
    extern void animPoseSetAnim(s32 poseId, const char* name, s32 flags);
    extern f32 dispCalcZ(f32* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
    extern void effSandarsDisp(s32 cameraId, void* effect);
    extern void effSandarsDisp2(s32 cameraId, void* effect);
    extern char str_EFF_bakuhatu_80300a6c[];
    extern char str_Z_1_80428158[];
    extern u8 scale_data[];
    extern f32 float_100_80428154;

    void* work;
    void* part;
    f32 pos[3];
    s32 heap;
    s32 type;
    s32 camId;
    s32 i;

    work = *(void**)((s32)effect + 0xC);
    pos[0] = *(f32*)((s32)work + 4);
    pos[1] = *(f32*)((s32)work + 8);
    pos[2] = *(f32*)((s32)work + 0xC);
    heap = (*(s32*)((s32)gp + 0x14) != 0);
    type = *(s32*)((s32)work + 0);
    camId = *(s32*)((s32)work + 0x40);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4u;
        if (*(s32*)((s32)work + 0x1C) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x1C));
        }
        effDelete(effect);
        return;
    }

    if (type == 5) {
        *(s32*)((s32)work + 0x18) -= 1;
        if (*(s32*)((s32)work + 0x18) < 0) {
            effDelete(effect);
            return;
        }
        part = (void*)((s32)work + 0x44);
        for (i = 1; i < *(s32*)((s32)effect + 8); i++, part = (void*)((s32)part + 0x44)) {
            if (*(s32*)((s32)part + 0x34) != 0) {
                *(s32*)((s32)part + 0x34) -= 1;
            } else {
                if (*(u32*)((s32)part + 0x18) < 0xF) {
                    *(f32*)((s32)part + 0x10) =
                        (f32)scale_data[*(s32*)((s32)part + 0x18)] / float_100_80428154;
                }
                *(s32*)((s32)part + 0x18) += 1;
                *(f32*)((s32)part + 0x38) += *(f32*)((s32)part + 0x3C);
            }
        }
        dispEntry(camId, 2, effSandarsDisp2, dispCalcZ(pos), effect);
        return;
    }

    switch (*(s32*)((s32)work + 0x14)) {
        case 0:
            effBombN64SetCamId(effBombN64Entry(*(s32*)((s32)work + 0x24)), camId);
            *(s32*)((s32)work + 0x18) = 0;
            *(s32*)((s32)work + 0x14) += 1;
            break;
        case 1:
            if (type < 3) {
                effDelete(effect);
                return;
            }
            if (*(s32*)((s32)work + 0x18) != 0) {
                *(s32*)((s32)work + 0x18) -= 1;
                break;
            }
            if (animGroupBaseAsync(str_EFF_bakuhatu_80300a6c, heap, 0) != 0) {
                *(s32*)((s32)work + 0x1C) = animPoseEntry(str_EFF_bakuhatu_80300a6c, heap);
                animPoseSetAnim(*(s32*)((s32)work + 0x1C), str_Z_1_80428158, 1);
                *(s32*)((s32)work + 0x14) += 1;
            }
            break;
        case 2:
            *(s32*)((s32)work + 0x20) -= 8;
            if (*(s32*)((s32)work + 0x20) < 0) {
                animPoseRelease(*(s32*)((s32)work + 0x1C));
                *(s32*)((s32)work + 0x1C) = -1;
                effDelete(effect);
                return;
            }
            break;
    }

    dispEntry(camId, 2, effSandarsDisp, dispCalcZ(pos), effect);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effSandarsDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXScale(f32 mtx[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 ab[3][4]);
    extern void animPoseSetMaterialFlagOn(s32 poseId, s32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, u32* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, f32 mtx[3][4], s32 mode, f32 a, f32 b);
    extern f32 float_deg2rad_80428138;
    extern f32 float_0_80428148;
    extern f32 float_4_80428150;
    extern u32 dat_80428130;

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
    if (*(s32*)((s32)work + 0x1C) != -1) {
        PSMTXTrans(transMtx, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        deg = float_deg2rad_80428138;
        PSMTXRotRad(rotMtx, 0x79, deg * angle);
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10));
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXConcat(transMtx, scaleMtx, mtx);
        PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x28), *(f32*)((s32)work + 0x28), *(f32*)((s32)work + 0x28));
        PSMTXConcat(mtx, scaleMtx, mtx);
        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x1C), 0x40);
        color = dat_80428130;
        ((u8*)&color)[3] = *(s32*)((s32)work + 0x20);
        evtColor = color;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x1C), &evtColor);
        animPoseMain(*(s32*)((s32)work + 0x1C));
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), mtx, 1, float_0_80428148, float_4_80428150);
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), mtx, 2, float_0_80428148, float_4_80428150);
        animPoseDrawMtx(*(s32*)((s32)work + 0x1C), mtx, 3, float_0_80428148, float_4_80428150);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_sandars 20260624_184929 */

/* stub-fill: effSandarsDisp2 | missing_definition | ghidra_signature */
void effSandarsDisp2(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetChanMatColor(s32, u32*);
    extern void GXBegin(s32, s32, s32);
    extern f32 float_deg2rad_80428138;
    extern u32 dat_80428134;

    void* work = *(void**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 base[3][4];
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scaleMtx[3][4];
    f32 draw[3][4];
    u8 texObj[0x20];
    volatile f32* fifo = (volatile f32*)0xCC008000;
    f32 half = -16.0f;
    s32 i;

    PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_80428138 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114));
    PSMTXScale(scaleMtx, *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10), *(f32*)((s32)work + 0x10));
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scaleMtx, trans);
    PSMTXConcat((f32(*)[4])((s32)camera + 0x34), trans, base);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 4, 4, 7);
    effGetTexObj(0x44, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        void* part = (void*)((s32)work + i * 0x44);
        f32 size = *(f32*)((s32)part + 0x10) * *(f32*)((s32)part + 0x2C);
        u32 color = (dat_80428134 & 0xFFFFFF00) | (*(u32*)((s32)part + 0x20) & 0xFF);
        PSMTXTrans(trans, *(f32*)((s32)part + 4), *(f32*)((s32)part + 8), *(f32*)((s32)part + 0xC));
        PSMTXRotRad(rot, 0x7A, float_deg2rad_80428138 * *(f32*)((s32)part + 0x38));
        PSMTXScale(scaleMtx, size, size, size);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);
        PSMTXConcat(base, trans, draw);
        GXLoadPosMtxImm(draw, 0);
        GXSetCurrentMtx(0);
        GXSetChanMatColor(4, &color);
        GXBegin(0x80, 0, 4);
        fifo[0] = half; fifo[0] = 16.0f; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 0.0f;
        fifo[0] = 16.0f; fifo[0] = 16.0f; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 0.0f;
        fifo[0] = 16.0f; fifo[0] = half; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 1.0f;
        fifo[0] = half; fifo[0] = half; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 1.0f;
    }
}

