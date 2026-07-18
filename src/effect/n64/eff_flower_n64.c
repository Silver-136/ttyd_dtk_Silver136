#include "effect/n64/eff_flower_n64.h"
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effFlowerDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, f32 angle, char axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetCullMode(s32 mode);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 tex, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern u32 dat_80425340;
    extern f32 float_deg2rad_80425344;
    extern f32 float_0p0625_80425348;
    extern f32 float_0_8042534c;
    extern char flowers_v[];

    u8 texObj[0x20];
    f32 mtxA[3][4];
    f32 mtxB[3][4];
    f32 mtxC[3][4];
    void* cam;
    u8* work;
    u32 color;

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);

    PSMTXTrans(mtxA, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(mtxC, float_deg2rad_80425344 * *(f32*)(work + 0x28), 'y');
    PSMTXConcat(mtxA, mtxC, mtxA);
    PSMTXRotRad(mtxC, float_deg2rad_80425344 * *(f32*)(work + 0x24), 'x');
    PSMTXConcat(mtxA, mtxC, mtxA);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtxA, mtxA);
    GXLoadPosMtxImm(mtxA, 0);

    color = dat_80425340;
    GXSetTevColor(1, &color);
    GXSetCullMode(0);
    effGetTexObjN64(0x4C, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(mtxB, float_0p0625_80425348, float_0p0625_80425348, float_0_8042534c);
    GXLoadTexMtxImm(mtxB, 0x1E, 1);

    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 8, 3, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 1, 7);
    effSetVtxDescN64(flowers_v);
    GXBegin(0x90, 0, 0x18);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(4, 2, 5, 0, 4, 5, 6, 0);
    tri2(7, 8, 2, 0, 7, 2, 9, 0);
    tri2(10, 2, 0xB, 0, 10, 0xB, 0xC, 0);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xA, 3, 0xF);
    GXSetTevAlphaIn(0, 7, 5, 1, 7);
    effSetVtxDescN64(flowers_v + 0xB6);
    GXBegin(0x90, 0, 0x18);
    tri2(0, 1, 2, 0, 0, 2, 3, 0);
    tri2(2, 4, 5, 0, 2, 5, 3, 0);
    tri2(6, 7, 8, 0, 6, 8, 9, 0);
    tri2(8, 10, 0xB, 0, 8, 0xB, 9, 0);
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

void* effFlowerN64Entry(float x, float y, float z, int type, int timer) {
    extern void* effEntry(void);
    extern void* __memAlloc(int, int);
    extern void effFlowerMain(void*);
    extern int rand(void);
    extern char str_FlowerN64_802fafbc[];
    extern int kaiten_412;
    extern float float_10_80425354;
    extern float float_0_8042534c;
    extern float float_0p12_80425368;
    extern float float_neg0p0152_8042536c;
    extern float float_3_80425370;
    unsigned char* entry = (unsigned char*)effEntry();
    unsigned char* work = (unsigned char*)__memAlloc(3, 0x48);

    *(char**)(entry + 0x14) = str_FlowerN64_802fafbc;
    *(int*)(entry + 8) = 1;
    *(unsigned char**)(entry + 0xC) = work;
    *(void**)(entry + 0x10) = effFlowerMain;
    *(int*)work = type;
    *(float*)(work + 4) = x;
    *(float*)(work + 8) = y + float_10_80425354;
    *(float*)(work + 0xC) = z;
    *(float*)(work + 0x10) = float_0_8042534c;
    *(float*)(work + 0x14) = float_0_8042534c;
    *(float*)(work + 0x18) = float_0_8042534c;
    *(float*)(work + 0x1C) = float_0p12_80425368;
    *(float*)(work + 0x20) = float_neg0p0152_8042536c;
    if (type == 0) {
        *(float*)(work + 0x24) = float_0_8042534c;
        *(float*)(work + 0x28) = float_0_8042534c;
        *(float*)(work + 0x2C) = float_3_80425370;
        *(float*)(work + 0x30) = kaiten_412 != 0 ? -10.0f : 10.0f;
        *(int*)(work + 0x34) = timer;
    } else {
        *(float*)(work + 0x24) = (float)((rand() & 1) * 30 - 15);
        *(float*)(work + 0x28) = (float)(rand() % 361);
        *(float*)(work + 0x2C) = float_0_8042534c;
        *(float*)(work + 0x30) = (float)((rand() & 1) * 8 - 4);
        *(int*)(work + 0x34) = timer;
        *(int*)(work + 0x38) = 0;
        *(int*)(work + 0x44) = rand() % 11;
        *(int*)(work + 0x40) = rand() % 21;
        *(int*)(work + 0x3C) = (rand() & 1) * 2 - 1;
    }
    kaiten_412++;
    if (kaiten_412 > 1) kaiten_412 = 0;
    return entry;
}

void effFlowerMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effFlowerDisp(s32, s32);
    extern Vec3 vec3_802fafb0[];
    extern f32 float_0p001_80425350;
    extern f32 float_10_80425354;
    extern f32 float_45_80425358;
    extern f32 float_1p4_8042535c;
    extern f32 float_1_80425360;
    extern f32 float_0p0007_80425364;
    u8* work;
    Vec3* base;
    Vec3 pos;
    Vec3 dispPos;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fafb0;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;

    if (*(s32*)(work + 0x34) < 0x96) {
        *(f32*)(work + 0x10) -= float_0p001_80425350;
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        *(f32*)(work + 0x24) += *(f32*)(work + 0x2C);
        if (*(s32*)work == 0) {
            if (*(f32*)(work + 0x24) < float_10_80425354) {
                *(f32*)(work + 0x24) = float_10_80425354;
                *(f32*)(work + 0x2C) = -*(f32*)(work + 0x2C);
            }
            if (*(f32*)(work + 0x24) > float_45_80425358) {
                *(f32*)(work + 0x24) = float_45_80425358;
                *(f32*)(work + 0x2C) = -*(f32*)(work + 0x2C);
            }
        }
        *(f32*)(work + 0x28) += *(f32*)(work + 0x30);
        if (*(s32*)work == 0) {
            *(f32*)(work + 0x14) = float_1p4_8042535c;
        } else {
            *(f32*)(work + 0x14) = float_1_80425360;
        }
    } else {
        *(f32*)(work + 0x20) += float_0p0007_80425364;
        *(f32*)(work + 0x1C) += *(f32*)(work + 0x20);
        *(f32*)(work + 0x14) += *(f32*)(work + 0x1C);
        if (*(f32*)(work + 0x14) > float_1_80425360) {
            *(f32*)(work + 0x14) = float_1_80425360;
        }
    }

    if (*(s32*)work == 1) {
        *(s32*)(work + 0x40) += *(s32*)(work + 0x3C);
        if (*(s32*)(work + 0x40) >= 0x14) {
            *(s32*)(work + 0x40) = 0x14;
            *(s32*)(work + 0x3C) = -1;
        } else if (*(s32*)(work + 0x40) <= 0) {
            *(s32*)(work + 0x40) = 0;
            *(s32*)(work + 0x3C) = 1;
        }
    }

    *(f32*)(work + 8) += *(f32*)(work + 0x14);
    *(s32*)(work + 0x34) -= 1;
    *(s32*)(work + 0x38) += 1;
    if (*(s32*)(work + 0x34) < 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effFlowerDisp, effect, dispCalcZ(&dispPos));
    }
}

