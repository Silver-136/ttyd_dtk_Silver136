#include "effect/n64/eff_sstar_n64.h"
#include "dolphin/os/OSFastCast.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effSstarN64Entry(f32 x, f32 y, f32 z, f32 dstX, f32 dstY, f32 dstZ, f32 speed, s32 type) {
    typedef union FloatBits { f32 value; s32 bits; } FloatBits;
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effSstarMain(void*);
    extern u8 vec3_802fbfe0[];
    extern s32 rand(void);
    extern f64 __frsqrte(f64);
    extern f64 cos(f64);
    extern f64 sin(f64);
    extern f32 angleABf(f32, f32, f32, f32);
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern f32 float_0_8042610c, float_0p2_80426144, float_0p5_80426148;
    extern f32 float_6p2832_8042614c, float_360_80426150;
    extern f32 float_neg20_80426154, float_20_80426158;
    extern f32 __float_nan[];
    u8* constants = vec3_802fbfe0;
    void* entry;
    u8* work;
    u8* camera;
    f32 dx, dy, dz, len, invLen;
    f32 radians, cs, sn, projectedX, projectedZ, projectedLen;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = (char*)constants + 0x28;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x25C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effSstarMain;
    *(s32*)work = 1;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x38) = type;
    *(f32*)(work + 0x1C) = float_0_8042610c;
    *(f32*)(work + 0x24) = float_0_8042610c;
    *(f32*)(work + 0x28) = float_0_8042610c;
    if (type == 3) {
        s32 first;
        s32 second;
        *(f32*)(work + 0x1C0) = float_0p2_80426144;
        first = rand() % 256;
        second = rand() % (255 - first);
        *(s32*)(work + 0x1C4) = first;
        *(s32*)(work + 0x1C8) = second;
        *(s32*)(work + 0x1CC) = 255 - first - second;
    } else {
        *(f32*)(work + 0x1C0) = float_0p5_80426148;
        *(s32*)(work + 0x1C4) = 0xD2;
        *(s32*)(work + 0x1C8) = 0xBA;
        *(s32*)(work + 0x1CC) = 0x5A;
    }

    dx = dstX - x;
    dy = dstY - y;
    dz = dstZ - z;
    len = dx * dx + dy * dy + dz * dz;
    {
        FloatBits classify;
        register f64 value, estimate;
        s32 category, exponent;
        value = (f64)len;
        invLen = (f32)value;
        if (value != (f64)float_0_8042610c) {
            if (value > (f64)float_0_8042610c) {
#ifdef __MWERKS__
                asm { frsqrte estimate, value }
#else
                estimate = 1.0 / value;
#endif
                estimate = *(f64*)(constants + 0x10) * estimate *
                           (*(f64*)(constants + 0x18) - value * estimate * estimate);
                estimate = *(f64*)(constants + 0x10) * estimate *
                           (*(f64*)(constants + 0x18) - value * estimate * estimate);
                len = (f32)(value * *(f64*)(constants + 0x10) * estimate *
                            (*(f64*)(constants + 0x18) - value * estimate * estimate));
            } else if (value < *(f64*)(constants + 0x20)) {
                len = __float_nan[0];
            } else {
                classify.value = len;
                exponent = classify.bits & 0x7F800000;
                if (exponent == 0x7F800000) {
                    if ((classify.bits & 0x7FFFFF) == 0) {
                        category = 2;
                    } else {
                        category = 1;
                    }
                } else if (exponent >= 0x7F800000) {
                    category = 4;
                } else if (exponent == 0) {
                    if ((classify.bits & 0x7FFFFF) == 0) {
                        category = 3;
                    } else {
                        category = 5;
                    }
                } else {
                    category = 4;
                }
                if (category == 1) {
                    len = __float_nan[0];
                }
            }
            invLen = speed / len;
        }
    }
    *(f32*)(work + 0x10) = dx * invLen;
    *(f32*)(work + 0x14) = dy * invLen;
    *(f32*)(work + 0x18) = dz * invLen;

    camera = camGetPtr(4);
    radians = float_6p2832_8042614c * *(f32*)(camera + 0x114) / float_360_80426150;
    cs = (f32)cos(radians);
    sn = (f32)sin(radians);
    projectedX = -cs * *(f32*)(work + 0x10);
    projectedZ = -sn * *(f32*)(work + 0x18);
    projectedLen = projectedX * projectedX + projectedZ * projectedZ;
    {
        FloatBits classify;
        register f64 value, estimate;
        s32 category, exponent;
        value = (f64)projectedLen;
        if (value != (f64)float_0_8042610c) {
            if (value > (f64)float_0_8042610c) {
#ifdef __MWERKS__
                asm { frsqrte estimate, value }
#else
                estimate = 1.0 / value;
#endif
                estimate = *(f64*)(constants + 0x10) * estimate *
                           (*(f64*)(constants + 0x18) - value * estimate * estimate);
                estimate = *(f64*)(constants + 0x10) * estimate *
                           (*(f64*)(constants + 0x18) - value * estimate * estimate);
                projectedLen = (f32)(value * *(f64*)(constants + 0x10) * estimate *
                                     (*(f64*)(constants + 0x18) - value * estimate * estimate));
            } else if (value < *(f64*)(constants + 0x20)) {
                projectedLen = __float_nan[0];
            } else {
                classify.value = projectedLen;
                exponent = classify.bits & 0x7F800000;
                if (exponent == 0x7F800000) {
                    if ((classify.bits & 0x7FFFFF) == 0) {
                        category = 2;
                    } else {
                        category = 1;
                    }
                } else if (exponent >= 0x7F800000) {
                    category = 4;
                } else if (exponent == 0) {
                    if ((classify.bits & 0x7FFFFF) == 0) {
                        category = 3;
                    } else {
                        category = 5;
                    }
                } else {
                    category = 4;
                }
                if (category == 1) {
                    projectedLen = __float_nan[0];
                }
            }
        }
    }
    if (projectedX + projectedZ >= float_0_8042610c) {
        *(f32*)(work + 0x2C) = float_20_80426158;
    } else {
        projectedLen = -projectedLen;
        *(f32*)(work + 0x2C) = float_neg20_80426154;
    }
    *(f32*)(work + 0x24) = angleABf(float_0_8042610c, float_0_8042610c,
                                     -*(f32*)(work + 0x14), -projectedLen);
    *(f32*)(work + 0x20) = *(f32*)(work + 0x24);
    *(f32*)(work + 0x34) = -projectedLen;
    *(s32*)(work + 0x30) = (s32)(len / speed);
    for (i = 0; i < 8; i++) {
        PSMTXTrans(work + 0x40 + i * 0x30, x, y, z);
    }
    *(s32*)(work + 0x3C) = -1;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

u8 effSstarMain(int* effect) {
    typedef union FloatBits { f32 value; s32 bits; } FloatBits;
    extern void* marioGetPtr(void); extern void* gpGlobals;
    extern s32 hitCheckFilter(f64,f64,f64,f64,f64,f64,s32,f32*,f32*,f32*,f32*,f32*,f32*,f32*);
    extern void effDelete(void*); extern f64 dispCalcZ(void*); extern void dispEntry(s32,s32,void*,void*,f32);
    extern void effSstarDisp(void); extern f64 angleABf(f64,f64,f64,f64);
    extern u8 vec3_802fbfe0[]; extern f32 __float_nan[];
    extern f32 float_0p01_80426118,float_1_80426114,float_32_8042611c,FLOAT_80426120;
    extern f32 float_42_80426128,float_0p6_8042612c,float_0p7_80426130,float_10_80426134;
    extern f32 float_0_8042610c,float_neg1_80426124,float_neg0p5_80426138,float_300_8042613c,float_neg300_80426140;
    u8* constants=vec3_802fbfe0; u8* work=*(u8**)((u8*)effect+0xC); u8* mario=marioGetPtr();
    volatile f32 oldPos[3]; f32 displayPos[3],length,inverse,nx,ny,nz;
    f32 outX,outY,outZ,distance,outNX,outNY,outNZ;
    oldPos[0]=*(f32*)(constants); oldPos[1]=*(f32*)(constants+4); oldPos[2]=*(f32*)(constants+8);
    oldPos[0]=*(f32*)(work+4); oldPos[1]=*(f32*)(work+8); oldPos[2]=*(f32*)(work+0xC);
    displayPos[0]=oldPos[0]; displayPos[1]=oldPos[1]; displayPos[2]=oldPos[2];
    if(*(s32*)(work+0x38)>1){
        FloatBits classify; register f64 value,estimate; s32 category,exponent;
        nx=*(f32*)(work+0x10); ny=*(f32*)(work+0x14); nz=*(f32*)(work+0x18);
        length=nx*nx+ny*ny+nz*nz; value=(f64)length;
        if(value>(f64)float_0p01_80426118){
            if(value>(f64)float_0_8042610c){
#ifdef __MWERKS__
                asm { frsqrte estimate, value }
#else
                estimate=1.0/value;
#endif
                estimate=*(f64*)(constants+0x10)*estimate*(*(f64*)(constants+0x18)-value*estimate*estimate);
                estimate=*(f64*)(constants+0x10)*estimate*(*(f64*)(constants+0x18)-value*estimate*estimate);
                length=(f32)(value**(f64*)(constants+0x10)*estimate*(*(f64*)(constants+0x18)-value*estimate*estimate));
            }else if(value<*(f64*)(constants+0x20)) length=__float_nan[0];
            else { classify.value=length; exponent=classify.bits&0x7F800000;
                if(exponent==0x7F800000){if((classify.bits&0x7FFFFF)==0)category=2;else category=1;}
                else if(exponent>=0x7F800000)category=4;
                else if(exponent==0){if((classify.bits&0x7FFFFF)==0)category=3;else category=5;}
                else category=4; if(category==1)length=__float_nan[0]; }
            inverse=float_1_80426114/length;
        }else inverse=length;
        nx*=inverse;ny*=inverse;nz*=inverse;distance=FLOAT_80426120;
        if(*(s32*)work!=0&&*(f32*)(work+0x14)<float_0_8042610c&&
          hitCheckFilter(*(f32*)(work+4)-float_32_8042611c*nx,*(f32*)(work+8)-float_32_8042611c*ny,
          *(f32*)(work+0xC)-float_32_8042611c*nz,float_0_8042610c,float_neg1_80426124,float_0_8042610c,0,
          &outX,&outY,&outZ,&distance,&outNX,&outNY,&outNZ)!=0&&distance<float_42_80426128){
            *(f32*)(work+0x14)=-*(f32*)(work+0x14)*float_0p6_8042612c;*(f32*)(work+0x10)*=float_0p7_80426130;
            *(f32*)(work+0x34)*=float_0p7_80426130;*(f32*)(work+0x2C)*=float_0p7_80426130;
            *(f32*)(work+0x1C)+=float_1_80426114;*(s32*)work=0;
            if(*(f32*)(work+0x1C)>=float_10_80426134)*(s32*)(work+0x30)=-1;}
        if(*(f32*)(work+0x1C)!=float_0_8042610c){*(f32*)(work+0x14)+=float_neg0p5_80426138;
          *(f32*)(work+0x24)+=*(f32*)(work+0x2C);*(f32*)(work+0x20)=(f32)angleABf(float_0_8042610c,
          float_0_8042610c,-*(f32*)(work+0x14),*(f32*)(work+0x34));}
    }
    if(*(s32*)((u8*)gpGlobals+0x14)==0){if(*(f32*)(mario+0x90)-*(f32*)(work+8)>float_300_8042613c)*(s32*)(work+0x30)=-1;}
    else if(*(f32*)(work+8)<float_neg300_80426140)*(s32*)(work+0x30)=-1;
    *(f32*)(work+4)+=*(f32*)(work+0x10);*(f32*)(work+8)+=*(f32*)(work+0x14);*(f32*)(work+0xC)+=*(f32*)(work+0x18);
    if(*(s32*)(work+0x30)<0)effDelete(effect);else dispEntry(4,2,effSstarDisp,effect,(f32)dispCalcZ(displayPos));return 0;
}

const f32 vec3_802fbfe0[3] = { 0.0f, 0.0f, 0.0f };
const f64 double_0p5_802fbff0 = 0.5;
const f64 double_3_802fbff8 = 3.0;
const f64 double_0_802fc000 = 0.0;
const char str_SstarN64_802fc008[] = "SstarN64";


void effSstarDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void effSetVtxDescN64(void*);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32, s32, s16);
    extern void PSMTXMultVec(Mtx, Vec3*, Vec3*);
    extern void GXInvalidateVtxCache(void);
    extern void DCFlushRange(void*, u32);
    extern void* memcpy(void*, const void*, u32);
    extern u8 shooting_dust_vtx[];
    extern f32 float_1_80426114;
    u8* work = *(u8**)(effectAddress + 0xC);
    char* camera = camGetPtr(cameraId);
    GXTexObj tex;
    Mtx trans, rot, scale, model;
    u32 color;
    s32 type = *(s32*)(work + 0x38);

    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, -0.017453292f * *(f32*)(camera + 0x114), 'y');
    PSMTXConcat(trans, rot, model);
    PSMTXScale(scale, *(f32*)(work + 0x1C0), *(f32*)(work + 0x1C0),
               *(f32*)(work + 0x1C0));
    PSMTXConcat(model, scale, model);
    PSMTXRotRad(rot, *(f32*)(work + 0x24) * 0.017453292f, 'z');
    PSMTXConcat(model, rot, model);
    GXSetCullMode(0);
    color = (*(u8*)(work + 0x1C4) << 24) | (*(u8*)(work + 0x1C8) << 16) |
            (*(u8*)(work + 0x1CC) << 8) | 0xFF;
    GXSetTevColor(1, &color);
    color = 0x7F7F7F7F;
    GXSetTevColor(2, &color);
    color = 0x50505050;
    GXSetTevColor(3, &color);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, 0.015625f, 0.015625f, 0.0f);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effGetTexObj(9, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 8, 6, 2);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    GXSetTevOrder(1, 0xFF, 0xFF, -1);
    GXSetTevColorOp(1, 1, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 4, 6, 0);
    GXSetTevAlphaIn(1, 7, 7, 7, 0);
    GXSetTevOrder(2, 0xFF, 0xFF, 4);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 3, 15, 15, 0);
    GXSetTevAlphaIn(2, 7, 0, 10, 7);
    effSetVtxDescN64((void*)0x803A8C70);
    PSMTXConcat(camera + 0x11C, model, trans);
    GXLoadPosMtxImm(trans, 0);
    GXBegin(0x90, 0, 6);
    tri2(0,1,2,0,0,2,3,0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 1);
    GXSetTevAlphaIn(0, 7, 10, 6, 7);
    effSetVtxDescN64((void*)0x803A8CA8);
    GXBegin(0x90, 0, 0xC);
    tri2(0,1,4,0,1,2,4,0);
    tri2(0,4,3,0,4,2,3,0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 1);
    GXSetTevAlphaIn(0, 7, 10, 4, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scale, 0.03125f, 0.03125f, 0.0f);
    GXLoadTexMtxImm(scale, 0x1E, 1);
    effGetTexObj(type == 3 ? 0xB : 0xA, &tex);
    GXLoadTexObj(&tex, 0);
    *(s32*)(work + 0x3C) += 1;
    if (*(s32*)(work + 0x3C) > 7) {
        *(s32*)(work + 0x3C) = 0;
    }
    if (*(f32*)(work + 0x1C) <= float_1_80426114) {
        s32 frame = *(s32*)(work + 0x3C);
        s32 i;
        PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8),
                   *(f32*)(work + 0xC));
        camera = camGetPtr(cameraId);
        PSMTXRotRad(rot, -0.017453292f * *(f32*)(camera + 0x114), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXScale(scale, *(f32*)(work + 0x1C0), *(f32*)(work + 0x1C0),
                   *(f32*)(work + 0x1C0));
        PSMTXConcat(trans, scale, trans);
        PSMTXRotRad(rot, *(f32*)(work + 0x20) * 0.017453292f, 'z');
        PSMTXConcat(trans, rot, work + 0x40 + *(s32*)(work + 0x3C) * 0x30);
        for (i = 0; i < 5; i++) {
            u8* dst = work + 0x1D0 + i * 0x1C;
            const u8* src = shooting_dust_vtx + i * 0x1C;
            Mtx* history = (Mtx*)(work + 0x40 + ((frame - i + 13) & 7) * 0x30);
            Vec3 v;
            s16* vertex;
            memcpy(dst, src, 0xE);
            vertex = (s16*)dst;
            v.x = __OSs16tof32(&vertex[0]);
            v.y = __OSs16tof32(&vertex[1]);
            v.z = __OSs16tof32(&vertex[2]);
            PSMTXMultVec(*history, &v, &v);
            vertex[0] = __OSf32tos16(v.x);
            vertex[1] = __OSf32tos16(v.y);
            vertex[2] = __OSf32tos16(v.z);
            memcpy(dst + 0xE, src + 0xE, 0xE);
            vertex = (s16*)(dst + 0xE);
            v.x = __OSs16tof32(&vertex[0]);
            v.y = __OSs16tof32(&vertex[1]);
            v.z = __OSs16tof32(&vertex[2]);
            PSMTXMultVec(*history, &v, &v);
            vertex[0] = __OSf32tos16(v.x);
            vertex[1] = __OSf32tos16(v.y);
            vertex[2] = __OSf32tos16(v.z);
        }
        GXInvalidateVtxCache();
        DCFlushRange(work + 0x1D0, 0x8C);
        effSetVtxDescN64(work + 0x1D0);
        GXLoadPosMtxImm((f32(*)[4])(camera + 0x11C), 0);
        for (i = 0; i < 4; i++) {
            s16 vertex = (s16)(i * 2);
            GXBegin(0x90, 0, 6);
            tri2(vertex, vertex + 1, vertex + 2, i * 2,
                 vertex + 1, vertex + 3, vertex + 2, i * 2);
        }
    }
}
