#include "effect/eff_torch.h"

typedef struct TorchVec {
    f32 x;
    f32 y;
    f32 z;
} TorchVec;

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern f32 float_1_804247ac;
extern f32 float_0p5_804247bc;
extern const TorchVec vec3_802f9a88[];

const char str_Torch_804247b4[] = "Torch";

u8 effTorchMain(int param_1);


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effTorchDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObjLocal { u32 dummy[8]; } GXTexObjLocal;

    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXSetNumChans(u32);
    extern void GXInitTexObj(void*, void*, s16, s16, u32, u32, u32, s32);
    extern void GXInitTexObjLOD(void*, s32, s32, f32, f32, f32, s32, s32, u32);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevSwapMode(s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void GXSetCullMode(s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXBegin(s32, s32, s16);
    extern void* torch_obj2_texture;
    extern void* torch_obj22t_texture;
    extern void* gp;
    extern u32 dat_80424790;
    extern f32 float_deg2rad_80424794;
    extern f32 float_0_80424798;
    extern f32 float_neg100_8042479c;
    extern f32 float_neg40_804247a0;
    extern f32 float_neg20_804247a4;
    extern f32 float_40_804247a8;
    extern f32 float_1_804247ac;
    extern f32 float_200_804247b0;

    u8* work;
    void* cam;
    void* cam2;
    GXTexObjLocal texObj;
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx mtx;
    u32 color;
    volatile f32* fifo;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    cam = camGetPtr(cameraId);
    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam2 = camGetPtr(cameraId);
    PSMTXRotRad(rot, float_deg2rad_80424794 * -*(f32*)((s32)cam2 + 0x114), 'y');
    PSMTXScale(scale, *(f32*)(work + 0x10) * *(f32*)(work + 0x14),
               *(f32*)(work + 0x10) * *(f32*)(work + 0x18), *(f32*)(work + 0x10));
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXInitTexObj(&texObj, &torch_obj2_texture, 0x20, 0x40, 0, 0, 0, 0);
    GXInitTexObjLOD(&texObj, 1, 1, float_0_80424798, float_0_80424798, float_0_80424798, 0, 0, 0);
    GXLoadTexObj(&texObj, 0);
    GXInitTexObj(&texObj, &torch_obj22t_texture, 0x20, 0x40, 0, 0, 2, 0);
    GXInitTexObjLOD(&texObj, 1, 1, float_0_80424798, float_0_80424798, float_0_80424798, 0, 0, 0);
    GXLoadTexObj(&texObj, 1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x1E, 0, 0x7D);
    color = dat_80424790;
    GXSetTevColor(1, &color);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 8, 2, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 0, 8, 0xF);
    GXSetTevAlphaIn(1, 7, 0, 4, 7);
    GXSetTevSwapMode(1, 0, 0);
    GXSetBlendMode(1, 4, 1, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetCullMode(0);

    fifo = (volatile f32*)0xCC008000;
    for (i = 0; i < 6; i++) {
        PSMTXTrans(trans, float_0_80424798,
                   (f32)(*(s32*)((s32)gp + 0x1C) + i) / float_neg100_8042479c,
                   float_0_80424798);
        GXLoadTexMtxImm(trans, 0x1E, 1);
        GXBegin(0x80, 0, 4);
        *fifo = float_neg40_804247a0; *fifo = float_neg20_804247a4; *fifo = float_0_80424798; *fifo = float_0_80424798; *fifo = float_0_80424798;
        *fifo = float_40_804247a8; *fifo = float_neg20_804247a4; *fifo = float_0_80424798; *fifo = float_1_804247ac; *fifo = float_0_80424798;
        *fifo = float_40_804247a8; *fifo = float_200_804247b0; *fifo = float_0_80424798; *fifo = float_1_804247ac; *fifo = float_1_804247ac;
        *fifo = float_neg40_804247a0; *fifo = float_200_804247b0; *fifo = float_0_80424798; *fifo = float_0_80424798; *fifo = float_1_804247ac;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTorchEntry(f32 x, f32 y, f32 z, f32 scale, s32 type) {
    void* entry;
    void* work;
    TorchVec pos;
    s32 main;
    f32 size;
    f32 one;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Torch_804247b4;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x20);
    main = (s32)effTorchMain;
    *(void**)((s32)entry + 0xC) = work;
    *(s32*)((s32)entry + 0x10) = main;
    pos = vec3_802f9a88[0];
    size = float_0p5_804247bc * scale;
    one = float_1_804247ac;
    *(s32*)((s32)work + 0) = type;
    *(u8*)((s32)work + 0x1C) = 0;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    *(TorchVec*)((s32)work + 4) = pos;
    *(f32*)((s32)work + 0x10) = size;
    *(f32*)((s32)work + 0x14) = one;
    *(f32*)((s32)work + 0x18) = one;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 effTorchMain(int param_1) {
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, void* param, f32 priority);
    extern void effTorchDisp(void);

    void* effect;
    void* work;

    effect = (void*)param_1;
    work = *(void**)(param_1 + 0xC);
    dispEntry(4, 2, effTorchDisp, effect, dispCalcZ((void*)((s32)work + 4)));
}

