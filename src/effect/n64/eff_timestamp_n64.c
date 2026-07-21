#include "effect/n64/eff_timestamp_n64.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effTimestampN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effTimestampMain(void*);
    extern char str_TimestampN64_802fc200[];
    extern s32 pos_data[];
    extern f32 float_1_80426454;
    extern f32 float_0_80426438;
    void* entry;
    u8* work;
    s32 i;
    s32* data;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_TimestampN64_802fc200;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x174);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effTimestampMain;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (lifetime <= 0) {
        *(s32*)(work + 0x10) = 1000;
    } else {
        *(s32*)(work + 0x10) = lifetime;
    }
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x28) = scale;
    *(s32*)(work + 0x18) = 0x46;
    *(s32*)(work + 0x1C) = 0xB4;
    *(s32*)(work + 0x20) = 0x78;
    *(s32*)(work + 0x2C) = 0;
    *(f32*)(work + 0x30) = float_1_80426454;
    data = pos_data;
    for (i = 0; i < 5; i++, data += 6, work += 8) {
        *(f32*)(work + 0x34) = (f32)data[0];
        *(f32*)(work + 0x5C) = (f32)data[1];
        *(f32*)(work + 0x84) = float_0_80426438;
        *(f32*)(work + 0xAC) = float_0_80426438;
        *(f32*)(work + 0xD4) = float_0_80426438;
        *(f32*)(work + 0xFC) = float_0_80426438;
        *(s32*)(work + 0x124) = 0;
        *(s32*)(work + 0x14C) = data[2];
        *(f32*)(work + 0x38) = (f32)data[3];
        *(f32*)(work + 0x60) = (f32)data[4];
        *(f32*)(work + 0x88) = float_0_80426438;
        *(f32*)(work + 0xB0) = float_0_80426438;
        *(f32*)(work + 0xD8) = float_0_80426438;
        *(f32*)(work + 0x100) = float_0_80426438;
        *(s32*)(work + 0x128) = 0;
        *(s32*)(work + 0x150) = data[5];
    }
    return entry;
}
#pragma no_register_save_helpers reset
#pragma use_lmw_stmw reset


void effTimestampMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effTimestampDisp(void);
    extern Vec3 vec3_802fc1e0[];
    extern f32 float_0_80426438;
    extern f32 float_0p05_80426450;
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    Vec3* base;
    s32 timer;
    s32 frame;
    s32 i;

    work = *(u8**)((s32)effect + 0xC);
    base = vec3_802fc1e0;
    pos = *base;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x10) = 0x40;
    }
    if (*(s32*)(work + 0x10) < 1000) {
        *(s32*)(work + 0x10) -= 1;
    }
    *(s32*)(work + 0x14) += 1;
    timer = *(s32*)(work + 0x10);
    frame = *(s32*)(work + 0x14);
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    if (timer < 0x40) {
        *(f32*)(work + 0x30) -= float_0p05_80426450;
        if (*(f32*)(work + 0x30) < float_0_80426438) {
            *(f32*)(work + 0x30) = float_0_80426438;
        }
    }
    if (timer < 0x20) {
        *(s32*)(work + 0x2C) += 0x80;
        if (*(s32*)(work + 0x2C) > 0xFF) {
            *(s32*)(work + 0x2C) = 0xFF;
        }
        *(f32*)(work + 0x30) = float_0_80426438;
    }
    if (timer < 0x10) {
        *(s32*)(work + 0x24) = timer << 4;
    }
    if (frame < 0x10) {
        *(s32*)(work + 0x24) = (frame << 4) + 0xF;
    }
    part = work;
    for (i = 0; i < 5; i++, part += 8) {
        if (*(s32*)(part + 0x14C) != 0) {
            *(s32*)(part + 0x14C) -= 1;
            if (*(s32*)(part + 0x14C) != 0) {
                goto second;
            }
        }
        *(s32*)(part + 0x124) += 8;
        if (*(s32*)(part + 0x124) > 0xFF) {
            *(s32*)(part + 0x124) = 0xFF;
        }
second:
        if (*(s32*)(part + 0x150) != 0) {
            *(s32*)(part + 0x150) -= 1;
            if (*(s32*)(part + 0x150) != 0) {
                continue;
            }
        }
        *(s32*)(part + 0x128) += 8;
        if (*(s32*)(part + 0x128) > 0xFF) {
            *(s32*)(part + 0x128) = 0xFF;
        }
    }
    dispEntry(4, 2, effTimestampDisp, effect, dispCalcZ(&dispPos));
}


void effTimestampDisp(s32 cameraId, void* effect)  {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*,f32,f32,f32);
    extern void PSMTXRotRad(void*,s32,f32);
    extern void PSMTXScale(void*,f32,f32,f32);
    extern void PSMTXConcat(void*,void*,void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetTevColor(s32,void*);
    extern void effGetTexObjN64(s32,void*);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXLoadTexMtxImm(void*,s32,s32);
    extern void GXSetCullMode(s32);
    extern void GXLoadPosMtxImm(void*,s32);
    extern void GXSetCurrentMtx(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    extern void* smartAlloc(s32,s32);
    extern void DCFlushRange(void*,s32);
    extern void GXInvalidateVtxCache(void);
    extern f64 sin(f64);
    u8* work=*(u8**)((s32)effect+0xC);
    void* cam=camGetPtr(cameraId);
    f32 base[3][4],m[3][4],r[3][4],s[3][4];
    u8 texObj[0x20];
    s16* v;
    u32 color;
    s32 i,j;
    f32 wave;
    PSMTXTrans(base,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));
    PSMTXScale(s,*(f32*)(work+0x28),*(f32*)(work+0x28),*(f32*)(work+0x28));
    PSMTXConcat(base,s,base);
    PSMTXRotRad(r,0x79,0.017453292f*-*(f32*)((u8*)camGetPtr(4)+0x114));
    PSMTXConcat(base,r,base);
    PSMTXConcat((u8*)cam+0x11C,base,base);
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(0,0,0,4);
    GXSetTevColorOp(0,0,0,0,1,0);
    GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,2,12,8,0);
    GXSetTevAlphaIn(0,0,0,0,7);
    GXSetTevOrder(1,1,1,4);
    GXSetTevColorOp(1,0,0,0,1,0);
    GXSetTevAlphaOp(1,0,0,0,1,0);
    GXSetTevColorIn(1,0,12,7,0);
    GXSetTevAlphaIn(1,0,4,7,7);
    effGetTexObjN64(0x5A,texObj);
    GXLoadTexObj(texObj,0);
    effGetTexObjN64(0x5B,texObj);
    GXLoadTexObj(texObj,1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    GXSetTexCoordGen2(1,1,4,0x1E,0,0x7D);
    PSMTXScale(s,0.015625f,0.015625f,0.0f);
    GXLoadTexMtxImm(s,0x1E,1);
    color=0xFFFFFF00|(*(s32*)(work+0x2C)&0xFF);
    GXSetTevColor(2,&color);
    v=smartAlloc(0x1C0,3);
    for(i=0;i<16;i++) {
        wave=*(f32*)(work+0x30);
        v[i*14]=(s16)(wave*500.0f*(f32)sin((6.2832f*(f32)(*(s32*)(work+0x14)*10+i*60))/360.0f))-0xC80;
        v[i*14+1]=(s16)(wave*200.0f*(f32)sin((6.2832f*(f32)(*(s32*)(work+0x14)*10+i*6))/360.0f)+i*400-3000);
        v[i*14+2]=0;
        v[i*14+3]=0;
        v[i*14+4]=(s16)(i*0x80);
        v[i*14+7]=v[i*14]+0x1900;
        v[i*14+8]=v[i*14+1];
        v[i*14+9]=0;
        v[i*14+10]=0x800;
        v[i*14+11]=(s16)(i*0x80);
    }
    DCFlushRange(v,0x1C0);
    GXInvalidateVtxCache();
    GXSetCullMode(0);
    effSetVtxDescN64(v);
    for(i=0;i<10;i++) {
        PSMTXTrans(m,*(f32*)(work+0x34+i*4),*(f32*)(work+0x5C+i*4),*(f32*)(work+0x84+i*4));
        PSMTXScale(s,0.01f,0.01f,0.01f);
        PSMTXConcat(m,s,m);
        PSMTXConcat(base,m,m);
        GXLoadPosMtxImm(m,0);
        GXSetCurrentMtx(0);
        color=0xFFFFFF00|((*(s32*)(work+0x24) * *(s32*)(work+0x124+i*4))/0xFF&0xFF);
        GXSetTevColor(1,&color);
        for(j=0;j<15;j++) {
            GXBegin(0x90,0,6);
            tri2(j*2,j*2+2,j*2+1,j*2,j*2+1,j*2+2,j*2+3,0);
        }
    }
}
