#include "effect/eff_kemuri.h"


u8 effKemuri2Entry(f32 x, f32 y, f32 z, f32 vx, f32 vz) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void* camGetPtr(s32 cameraId);
    extern f32 reviseAngle(f32 angle);
    extern u8 effKemuri2Main(void* effEntry);
    extern char str_Kemuri2_802f3720[];
    extern f32 float_1000_804229f8;
    extern f32 float_0_804229a4;
    extern f32 float_360_804229f4;
    extern f32 float_6p2832_804229fc;
    extern f32 float_5_80422a00;
    extern f32 float_1_80422998;
    extern f32 float_0p7_80422a04;
    extern f32 float_neg20_80422a08;
    extern double sin(double x);
    extern double cos(double x);
    void* entry;
    void* work;
    f32 sx;
    f32 sz;
    f32 angle;
    f32 camAngle;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Kemuri2_802f3720;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x8C);
    *(void**)((s32)entry + 0xC) = work;
    memset(work, 0, 0x8C);
    *(void**)((s32)entry + 0x10) = effKemuri2Main;

    sx = float_1000_804229f8 * vx;
    sz = float_1000_804229f8 * vz;
    angle = (float_360_804229f4 * angleABf(float_0_804229a4, float_0_804229a4, sx, sz)) / float_6p2832_804229fc;
    camAngle = *(f32*)((s32)camGetPtr(4) + 0x114);
    *(f32*)((s32)work + 0x78) = reviseAngle(((float_360_804229f4 * angleABf(float_0_804229a4, float_0_804229a4, sx, sz)) / float_6p2832_804229fc) + camAngle);
    *(s32*)((s32)work + 0x3C) = 0;
    rad = (float_6p2832_804229fc * (f32)(s32)angle) / float_360_804229f4;
    *(f32*)((s32)work + 0x30) = (f32)sin(rad);
    *(f32*)((s32)work + 0x34) = (f32)cos(rad);
    *(f32*)((s32)work + 0x4) = x;
    *(f32*)((s32)work + 0x8) = y;
    *(f32*)((s32)work + 0xC) = float_5_80422a00 + z;
    *(f32*)((s32)work + 0x10) = float_1_80422998;
    *(f32*)((s32)work + 0x20) = float_0p7_80422a04;
    *(f32*)((s32)work + 0x64) = float_neg20_80422a08;
    *(f32*)((s32)work + 0x68) = float_0_804229a4;
    *(s32*)((s32)work + 0x6C) = 0xFF;
    *(f32*)((s32)work + 0x70) = float_1_80422998;
    *(f32*)((s32)work + 0x74) = float_1_80422998;
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effKemuriEntry(s32 kind, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern char str_Kemuri_802f3728[];
    extern f32 float_0_804229a4;
    extern f32 float_1_80422998;
    extern f32 float_3_80422a0c;
    extern f32 float_neg3_80422a10;
    extern void* effKemuriMain(void* effEntry);
    void* entry;
    void* work;
    void* child;
    s32 count;
    s32 i;

    entry = effEntry();
    count = 1;
    if (kind == 0) {
        count = 3;
    } else if (kind == 1) {
        count = 2;
    } else if (kind == 2) {
        count = 2;
    }

    *(char**)((s32)entry + 0x14) = str_Kemuri_802f3728;
    *(s32*)((s32)entry + 0x8) = count;
    work = __memAlloc(3, count * 0x8C);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKemuriMain;

    *(s32*)((s32)work + 0x0) = kind;
    *(f32*)((s32)work + 0x4) = x;
    *(f32*)((s32)work + 0x8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x40) = 0;
    *(f32*)((s32)work + 0x44) = float_1_80422998;
    *(f32*)((s32)work + 0x48) = float_1_80422998;
    *(f32*)((s32)work + 0x74) = float_1_80422998;

    child = (void*)((s32)work + 0x8C);
    i = 1;
    while (i < *(s32*)((s32)entry + 0x8)) {
        *(f32*)((s32)child + 0x4) = float_0_804229a4;
        *(f32*)((s32)child + 0x8) = float_0_804229a4;
        *(f32*)((s32)child + 0xC) = float_0_804229a4;
        *(f32*)((s32)child + 0x44) = float_1_80422998;
        *(f32*)((s32)child + 0x48) = float_1_80422998;
        *(f32*)((s32)child + 0x74) = float_1_80422998;
        *(f32*)((s32)child + 0x58) = float_0_804229a4;
        *(f32*)((s32)child + 0x5C) = float_0_804229a4;
        *(s32*)((s32)child + 0x40) = 0;
        *(s32*)((s32)child + 0x80) = 0;
        *(s32*)((s32)child + 0x84) = 0;

        if (kind == 0) {
            if ((i & 1) != 0) {
                *(f32*)((s32)child + 0x4C) = float_3_80422a0c;
            } else {
                *(f32*)((s32)child + 0x4C) = float_neg3_80422a10;
            }
            *(s32*)((s32)child + 0x88) = 0x20;
        } else if (kind == 1) {
            *(f32*)((s32)child + 0x4C) = float_0_804229a4;
            *(s32*)((s32)child + 0x88) = 0x20;
        } else if (kind == 2) {
            *(f32*)((s32)child + 0x4C) = float_0_804229a4;
            *(s32*)((s32)child + 0x88) = 0x40;
        }

        i++;
        child = (void*)((s32)child + 0x8C);
    }

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 effKemuri2Disp(s32 cameraId, void* effEntry) {
    typedef f32 Mtx[3][4];
    typedef struct Tex { u32 data[8]; } Tex;
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumChans(u32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetCullMode(s32);
    extern void mapSetMaterialFog(void);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXBegin(s32, s32, s16);
    extern u32 unk_804295b8;
    extern u32 dat_80422968;
    extern u32 unk_804295bc;
    extern u32 dat_8042296c;
    u8* work;
    void* cam;
    Tex tex;
    Mtx trans, camRot, zRot, scale1, scale2, local;
    volatile f32* fifo;
    u32 color;
    f32 offset;
    f32 angle;
    s32 i;

    work = *(u8**)((s32)effEntry + 0xC);
    cam = camGetPtr(cameraId);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0,0xF,2,8,0xF);
    GXSetTevAlphaIn(0,7,0,4,7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0,1,4,0x3C,0,0x7D);
    GXSetCullMode(0);
    mapSetMaterialFog();
    GXClearVtxDesc();
    GXSetVtxDesc(9,1);
    GXSetVtxDesc(0xD,1);
    GXSetVtxAttrFmt(0,9,1,4,0);
    GXSetVtxAttrFmt(0,0xD,1,4,0);
    fifo = (volatile f32*)0xCC008000;
    angle = *(f32*)(work + 0x78);

    for (i = 0; i < 2; i++) {
        offset = (f32)(1 - i);
        if (((angle > 20.0f) && (angle < 160.0f)) || ((angle > 180.0f) && (angle < 340.0f))) {
            PSMTXTrans(trans, *(f32*)(work+4)+offset, *(f32*)(work+8)-offset, *(f32*)(work+0xC));
            PSMTXRotRad(camRot, 0.017453292f * -*(f32*)((s32)cam+0x114), 'y');
            PSMTXRotRad(zRot, 0.17453f, 'z');
            PSMTXScale(scale1, *(f32*)(work+0x70), *(f32*)(work+0x70), *(f32*)(work+0x70));
            PSMTXConcat(trans, camRot, local);
            if (angle < 180.0f) {
                PSMTXScale(scale2,-1.0f,1.0f,-1.0f);
                PSMTXConcat(local,scale2,local);
            }
            PSMTXTrans(scale2,10.0f,-6.0f,0.0f); PSMTXConcat(local,scale2,local);
            PSMTXScale(scale2,*(f32*)(work+0x74),*(f32*)(work+0x74),*(f32*)(work+0x74)); PSMTXConcat(local,scale2,local);
            PSMTXTrans(scale2,-10.0f,6.0f,0.0f); PSMTXConcat(local,scale2,local);
            PSMTXConcat(local,zRot,local); PSMTXConcat(local,scale1,local);
            PSMTXConcat((void*)((s32)cam+0x11C),local,local);
            PSMTXTrans(trans,6.0f,-4.0f,0.0f); PSMTXConcat(local,trans,local);
            GXLoadPosMtxImm(local,0); GXSetCurrentMtx(0);
            effGetTexObj(0xD,&tex); GXLoadTexObj(&tex,0);
            color = (i == 0 ? unk_804295b8 : dat_80422968) & 0xFFFFFF00;
            color |= *(u8*)(work+0x6F); GXSetTevColor(1,&color);
            GXBegin(0x80,0,4);
            *fifo=-6.0f; *fifo=4.0f; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f;
            *fifo=6.0f; *fifo=4.0f; *fifo=0.0f; *fifo=1.0f; *fifo=0.0f;
            *fifo=6.0f; *fifo=-4.0f; *fifo=0.0f; *fifo=1.0f; *fifo=1.0f;
            *fifo=-6.0f; *fifo=-4.0f; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f;
        }
        PSMTXTrans(trans, *(f32*)(work+4)+offset, *(f32*)(work+8)-offset, *(f32*)(work+0xC));
        PSMTXRotRad(camRot,0.017453292f * -*(f32*)((s32)cam+0x114),'y');
        PSMTXRotRad(zRot,0.017453292f * *(f32*)(work+0x68),'z');
        PSMTXScale(scale1,*(f32*)(work+0x70),*(f32*)(work+0x70),*(f32*)(work+0x70));
        PSMTXConcat(trans,camRot,local);
        if (angle < 180.0f) { PSMTXScale(scale2,-1.0f,1.0f,-1.0f); PSMTXConcat(local,scale2,local); }
        PSMTXTrans(scale2,10.0f,-6.0f,0.0f); PSMTXConcat(local,scale2,local);
        PSMTXScale(scale2,*(f32*)(work+0x74),*(f32*)(work+0x74),*(f32*)(work+0x74)); PSMTXConcat(local,scale2,local);
        PSMTXTrans(scale2,-10.0f,6.0f,0.0f); PSMTXConcat(local,scale2,local);
        PSMTXConcat(local,zRot,local); PSMTXConcat(local,scale1,local);
        PSMTXConcat((void*)((s32)cam+0x11C),local,local);
        GXLoadPosMtxImm(local,0); GXSetCurrentMtx(0);
        effGetTexObj(i == 0 ? 0xC : 0xB,&tex); GXLoadTexObj(&tex,0);
        color = (i == 0 ? unk_804295bc : dat_8042296c) & 0xFFFFFF00;
        color |= *(u8*)(work+0x6F); GXSetTevColor(1,&color);
        GXBegin(0x80,0,4);
        *fifo=-6.4f; *fifo=6.4f; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f;
        *fifo=6.4f; *fifo=6.4f; *fifo=0.0f; *fifo=1.0f; *fifo=0.0f;
        *fifo=6.4f; *fifo=-6.4f; *fifo=0.0f; *fifo=1.0f; *fifo=1.0f;
        *fifo=-6.4f; *fifo=-6.4f; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f;
    }
    return 0;
}

u8 effKemuriDisp(s32 cameraId, void* effEntry) {
    typedef f32 Mtx[3][4];
    typedef struct Tex { u32 data[8]; } Tex;
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXSetNumChans(u32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32,u32,u32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32,s32,s32,u32,u32,s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(u32,s32,u32,u32,u32);
    extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXLoadTexMtxImm(Mtx,s32,s32);
    extern void GXBegin(s32,s32,s16);
    extern u32 dat_80422958;
    extern u32 dat_8042295c;
    extern u32 dat_80422960;
    extern u32 dat_80422964;
    u8* work;
    u8* part;
    void* cam;
    Tex tex;
    Mtx trans, rot, scale, base, local;
    volatile f32* fifo;
    u32 color;
    f32 offset;
    s32 type;
    s32 i;
    s32 side;

    work = *(u8**)((s32)effEntry + 0xC);
    type = *(s32*)work;
    cam = camGetPtr(cameraId);
    if (type == 1) { effGetTexObj(0x24,&tex); GXLoadTexObj(&tex,0); }
    else if ((type >= 0) && (type < 3)) { effGetTexObj(0x23,&tex); GXLoadTexObj(&tex,0); }
    PSMTXTrans(trans,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));
    PSMTXRotRad(rot,0.017453292f * -*(f32*)((s32)cam+0x114),'y');
    PSMTXScale(scale,*(f32*)(work+0x74),*(f32*)(work+0x74),1.0f);
    PSMTXConcat(trans,rot,base);
    PSMTXConcat(base,scale,base);
    PSMTXConcat((void*)((s32)cam+0x11C),base,base);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(4,0,0,0,0,0,2);
    color=dat_80422958; GXSetChanMatColor(4,&color);
    GXSetNumTevStages(3);
    GXSetTevOrder(0,0,0,0xFF); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,0xF,0xF,0xF,8); GXSetTevAlphaIn(0,7,7,7,4);
    GXSetTevOrder(1,1,0,0xFF); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0);
    GXSetTevColorIn(1,0,8,2,0xF); GXSetTevAlphaIn(1,0,4,0,7);
    color=dat_8042295c; GXSetTevColor(1,&color);
    GXSetTevOrder(2,0xFF,0xFF,4); GXSetTevColorOp(2,0,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0);
    GXSetTevColorIn(2,0xF,0,10,0xF); GXSetTevAlphaIn(2,7,0,5,7);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    GXClearVtxDesc(); GXSetVtxDesc(9,1); GXSetVtxDesc(0xD,1);
    GXSetVtxAttrFmt(0,9,1,4,0); GXSetVtxAttrFmt(0,0xD,1,4,0);
    fifo=(volatile f32*)0xCC008000;
    part=work+0x8C;
    for(i=1;i<*(s32*)((s32)effEntry+8);i++,part+=0x8C){
        for(side=0;side<2;side++){
            color=(side==0)?dat_80422960:dat_80422964;
            GXSetChanMatColor(4,&color);
            offset=(f32)(1-side);
            PSMTXTrans(trans,*(f32*)(part+0x58)+offset,-offset,0.0f);
            PSMTXScale(scale,20.0f**(f32*)(part+0x44),20.0f**(f32*)(part+0x48),20.0f);
            PSMTXConcat(base,trans,local); PSMTXConcat(local,scale,local);
            GXLoadPosMtxImm(local,0);
            PSMTXScale(scale,0.0625f,1.0f,1.0f);
            PSMTXTrans(trans,(f32)*(s32*)(part+0x80),-0.2f,0.0f);
            PSMTXConcat(scale,trans,trans); GXLoadTexMtxImm(trans,0x1E,1);
            PSMTXScale(scale,0.0625f,1.0f,1.0f);
            PSMTXTrans(trans,(f32)*(s32*)(part+0x84),-0.2f,0.0f);
            PSMTXConcat(scale,trans,trans); GXLoadTexMtxImm(trans,0x21,1);
            GXBegin(0x80,0,4);
            *fifo=-0.5f; *fifo=1.0f; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f;
            *fifo=0.5f; *fifo=1.0f; *fifo=0.0f; *fifo=1.0f; *fifo=0.0f;
            *fifo=0.5f; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f; *fifo=1.0f;
            *fifo=-0.5f; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f;
        }
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effKemuriMain(void* effEntry) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void effDelete(void* effect);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effKemuriDisp(s32 cameraId, void* effEntry);
    extern Vec vec3_802f36f8;
    extern u8 touchdown_scale[];
    extern u8 heavy_scale[];
    extern f32 float_16_804229dc;
    extern f32 float_0p8_804229d8;
    extern f32 float_0p0078125_804229e0;
    extern f32 float_1p5_804229e4;
    extern f32 float_0p85_804229e8;
    extern f32 float_2_804229ec;
    extern f32 float_1p2_804229f0;

    void* work;
    void* cur;
    Vec dispPos;
    s32 type;
    s32 frame;
    s32 duration;
    s32 i;

    work = *(void**)((s32)effEntry + 0xC);
    dispPos = vec3_802f36f8;
    dispPos.x = *(f32*)((s32)work + 4);
    dispPos.y = *(f32*)((s32)work + 8);
    dispPos.z = *(f32*)((s32)work + 0xC);

    type = *(s32*)work;
    cur = (void*)((s32)work + 0x8C);
    i = 1;
    while (i < *(s32*)((s32)effEntry + 8)) {
        frame = *(s32*)((s32)cur + 0x40);
        duration = *(s32*)((s32)cur + 0x88);

        *(s32*)((s32)cur + 0x80) = (s32)((float_16_804229dc * (f32)frame) / (f32)duration);
        *(s32*)((s32)cur + 0x84) = (s32)((float_16_804229dc * (f32)(frame + 1)) / (f32)duration);
        if (*(s32*)((s32)cur + 0x84) > 0xF) {
            *(s32*)((s32)cur + 0x84) = 0xF;
        }

        if (type == 1) {
            *(s32*)((s32)cur + 0x40) = frame + 1;
        } else if (type == 0) {
            if (frame < 0x1E) {
                *(f32*)((s32)cur + 0x44) = (f32)touchdown_scale[frame * 2] * float_0p0078125_804229e0;
                *(f32*)((s32)cur + 0x48) = (f32)touchdown_scale[frame * 2 + 1] * float_0p0078125_804229e0;
            } else {
                *(f32*)((s32)cur + 0x44) = float_1p5_804229e4;
                *(f32*)((s32)cur + 0x48) = float_0p8_804229d8;
            }
            *(f32*)((s32)cur + 0x4C) *= float_0p85_804229e8;
            *(f32*)((s32)cur + 0x58) += *(f32*)((s32)cur + 0x4C);
            *(s32*)((s32)cur + 0x40) = frame + 1;
        } else if (type < 3) {
            if (frame < 0x1E) {
                *(f32*)((s32)cur + 0x44) = (f32)heavy_scale[frame * 2] * float_0p0078125_804229e0;
                *(f32*)((s32)cur + 0x48) = (f32)heavy_scale[frame * 2 + 1] * float_0p0078125_804229e0;
            } else {
                *(f32*)((s32)cur + 0x44) = float_2_804229ec;
                *(f32*)((s32)cur + 0x48) = float_1p2_804229f0;
            }
            *(s32*)((s32)cur + 0x40) = frame + 1;
        }

        if (*(s32*)((s32)cur + 0x88) <= *(s32*)((s32)cur + 0x40)) {
            effDelete(effEntry);
            return 0;
        }
        i++;
        cur = (void*)((s32)cur + 0x8C);
    }

    dispEntry(4, 2, effKemuriDisp, effEntry, dispCalcZ(&dispPos));
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effKemuri2Main(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effKemuri2Disp(s32 cameraId, void* effEntry);
    extern LocalVec3 vec3_802f3704;
    extern f32 float_3p1416_804229c8;
    extern f32 float_30_804229cc;
    extern f32 float_0p9_804229d0;
    extern f32 float_0p95_804229d4;
    extern f32 float_0p8_804229d8;
    extern double sin(double x);
    LocalVec3 dispPos;
    LocalVec3 pos;
    void* work;
    void* cur;
    s32 i;
    f32 wave;

    work = *(void**)((s32)effEntry + 0xC);
    pos = vec3_802f3704;
    pos.x = *(f32*)((s32)work + 0x4);
    pos.y = *(f32*)((s32)work + 0x8);
    pos.z = *(f32*)((s32)work + 0xC);
    dispPos = pos;

    wave = (f32)sin((float_3p1416_804229c8 * (f32)*(s32*)((s32)work + 0x3C)) / float_30_804229cc);
    *(s32*)((s32)work + 0x3C) += 1;
    if (*(s32*)((s32)work + 0x3C) > 30) {
        effDelete(effEntry);
        return 0;
    }

    cur = work;
    i = 0;
    while (i < *(s32*)((s32)effEntry + 0x8)) {
        *(f32*)((s32)cur + 0x4) += *(f32*)((s32)cur + 0x10) * *(f32*)((s32)cur + 0x30);
        *(f32*)((s32)cur + 0xC) += *(f32*)((s32)cur + 0x10) * *(f32*)((s32)cur + 0x34);
        *(f32*)((s32)cur + 0x20) *= float_0p9_804229d0;
        *(f32*)((s32)cur + 0x8) += *(f32*)((s32)cur + 0x20);
        *(f32*)((s32)cur + 0x64) *= float_0p95_804229d4;
        *(f32*)((s32)cur + 0x68) += *(f32*)((s32)cur + 0x64);
        *(f32*)((s32)cur + 0x70) = wave;
        if (*(s32*)((s32)cur + 0x3C) > 15) {
            *(s32*)((s32)cur + 0x6C) = (s32)((f32)*(s32*)((s32)cur + 0x6C) * float_0p8_804229d8);
        }
        i++;
        cur = (void*)((s32)cur + 0x8C);
    }
    dispEntry(4, 2, effKemuri2Disp, effEntry, dispCalcZ(&dispPos));
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

