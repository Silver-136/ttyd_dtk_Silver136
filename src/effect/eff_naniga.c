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
u8 effNanigaDisp(void) {
    return 0;
}

/* stub-fill: effNanigaMain | missing_definition | ghidra_signature */
u8 effNanigaMain(void) {
    return 0;
}

/* stub-fill: effNanigaEntry | missing_definition | ghidra_signature */
u8 effNanigaEntry(void) {
    return 0;
}
