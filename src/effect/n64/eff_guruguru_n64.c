#include "effect/n64/eff_guruguru_n64.h"


void effGuruguruDisp(int cameraId, int effect) {
    typedef float Mtx[3][4];
    extern void* camGetPtr(int);
    extern void GXSetNumChans(int);
    extern void GXSetNumTexGens(int);
    extern void GXSetTexCoordGen2(int, int, int, int, int, int);
    extern void effGetTexObjN64(int, void*);
    extern void GXLoadTexObj(void*, int);
    extern void GXSetNumTevStages(int);
    extern void GXSetTevOrder(int, int, int, int);
    extern void GXSetTevColorOp(int, int, int, int, int, int);
    extern void GXSetTevAlphaOp(int, int, int, int, int, int);
    extern void GXSetTevColorIn(int, int, int, int, int);
    extern void GXSetTevAlphaIn(int, int, int, int, int);
    extern void GXSetCullMode(int);
    extern void effSetVtxDescN64(void*);
    extern void PSMTXTrans(Mtx, double, double, double);
    extern void PSMTXRotRad(Mtx, double, char);
    extern void PSMTXScale(Mtx, float, float, float);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXSetTevColor(int, void*);
    extern void GXLoadPosMtxImm(Mtx, int);
    extern void GXSetCurrentMtx(int);
    extern void GXBegin(int, int, int);
    extern void tri2(int, int, int, int, int, int, int);
    extern float float_deg2rad_80425424;
    extern unsigned int dat_80425420;
    unsigned char texObj[0x20];
    Mtx model;
    Mtx scale;
    Mtx rotation;
    Mtx partMtx;
    unsigned char* entry = (unsigned char*)effect;
    unsigned char* work = *(unsigned char**)(entry + 0xC);
    unsigned char* part;
    unsigned char* camera;
    unsigned char* camera3d;
    unsigned int color = dat_80425420;
    int i;

    if (*(int*)(work + 0x28) == 0) return;
    camera = (unsigned char*)camGetPtr(cameraId);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    effGetTexObjN64(0x1D, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 2);
    GXSetTevAlphaIn(0, 7, 4, 4, 7);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x803A03E8);
    PSMTXTrans(model, *(float*)(work + 4), *(float*)(work + 8), *(float*)(work + 0xC));
    camera3d = (unsigned char*)camGetPtr(4);
    PSMTXRotRad(rotation, float_deg2rad_80425424 * -*(float*)(camera3d + 0x114), 'y');
    PSMTXScale(scale, *(float*)(work + 0x2C), *(float*)(work + 0x2C), *(float*)(work + 0x2C));
    PSMTXConcat(model, rotation, model);
    PSMTXConcat(model, scale, model);
    PSMTXConcat((float (*)[4])(camera + 0x11C), model, model);
    GXSetTevColor(1, &color);
    part = work + 0x30;
    for (i = 1; i < *(int*)(entry + 8); i++, part += 0x30) {
        PSMTXTrans(partMtx, *(float*)(part + 4), *(float*)(part + 8), *(float*)(part + 0xC));
        PSMTXRotRad(rotation, float_deg2rad_80425424 * *(float*)(part + 0x18), 'z');
        PSMTXConcat(partMtx, rotation, partMtx);
        PSMTXConcat(model, partMtx, partMtx);
        GXLoadPosMtxImm(partMtx, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effGuruguruN64Entry(s32 type, s32 count, f32 x, f32 y, f32 z, f32 radius) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effGuruguruMain(void*);
    extern char str_GuruguruN64_802fb060[];
    extern f32 float_0_80425434;
    extern f32 float_1_80425438;
    extern f32 float_20_8042543c;
    void* entry;
    u8* work;
    u8* part;
    s32 i;
    s32 angle;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_GuruguruN64_802fb060;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x30);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effGuruguruMain;

    *(s32*)work = type;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = float_0_80425434;
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 0x14) = radius;
    *(s32*)(work + 0x28) = 1;
    *(f32*)(work + 0x2C) = float_1_80425438;

    part = work + 0x30;
    for (i = 1; i < count + 1; i++) {
        angle = ((i - 1) * 0x168) / count;
        *(f32*)(part + 4) = float_0_80425434;
        *(f32*)(part + 8) = float_0_80425434;
        *(f32*)(part + 0xC) = float_0_80425434;
        *(f32*)(part + 0x18) = (f32)angle;
        *(f32*)(part + 0x1C) = float_20_8042543c;
        *(f32*)(part + 0x20) = float_0_80425434;
        *(s32*)(part + 0x24) = angle;
        part += 0x30;
    }

    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

void effGuruguruMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effGuruguruDisp(void);
    extern Vec3 vec3_802fb048[];
    extern f32 float_0p1_80425428;
    extern f32 float_6p2832_8042542c;
    extern f32 float_360_80425430;
    extern f32 float_0_80425434;
    u8* work;
    u8* part;
    Vec3* base;
    Vec3 pos;
    Vec3 dispPos;
    s32 i;
    f32 radius;
    f32 angle0;
    f32 angle1;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fb048;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)(work + 0x28) != 0) {
        *(s32*)(work + 0x24) += 1;
        *(f32*)(work + 0x10) += float_0p1_80425428 * (*(f32*)(work + 0x14) - *(f32*)(work + 0x10));
        radius = *(f32*)(work + 0x10);
        part = work + 0x30;
        for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
            angle0 = float_6p2832_8042542c * (f32)*(s32*)(part + 0x24) / float_360_80425430;
            *(f32*)(part + 4) = radius * (f32)sin(angle0);
            angle1 = float_6p2832_8042542c * *(f32*)(part + 0x20) / float_360_80425430;
            *(f32*)(part + 0xC) = radius * (f32)cos(angle0) * (f32)cos(angle1);
            *(f32*)(part + 8) = radius * (f32)cos(angle0) * (f32)sin(angle1);
            *(f32*)(part + 0x18) += *(f32*)(part + 0x1C);
            *(f32*)(part + 0x20) = float_0_80425434;
            *(s32*)(part + 0x24) += 0x10;
            if (*(s32*)(part + 0x24) > 0x168) {
                *(s32*)(part + 0x24) -= 0x168;
            }
            part += 0x30;
        }
        dispEntry(4, 2, effGuruguruDisp, effect, dispCalcZ(&dispPos));
    }
}

