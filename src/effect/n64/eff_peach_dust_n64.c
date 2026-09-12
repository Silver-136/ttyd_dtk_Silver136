#include "effect/n64/eff_peach_dust_n64.h"


void* effPeachDustN64Entry(f32 x, f32 y, f32 z, f32 unused, f32 angle, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effPeachDustMain(void*);
    extern char str_PeachDustN64_802fbc80[];
    extern u8 data[];
    extern s32 rand(void);
    extern f64 sin(f64), cos(f64);
    extern f32 float_6p2832_80425cf0, float_360_80425cf4;
    extern f32 float_0p1_80425cec, float_0p04_80425cf8;
    extern f32 float_0p001_80425cfc, float_0_80425cd4;
    void* entry;
    u8* work;
    u8* part;
    u8* pattern;
    u8* patternBase;
    s32 row;
    s32 column;
    s32 count;
    s32 random;
    s32 rowBase;
    s32 remaining;
    s32 groups;
    f32 baseX;
    f32 radians;
    f32 angleScale;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_PeachDustN64_802fbc80;
    *(s32*)((s32)entry + 8) = 0x65;
    work = __memAlloc(3, 0x115C);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effPeachDustMain;
    rowBase = 0;
    angleScale = float_6p2832_80425cf0 * angle;
    *(f32*)(work + 4) = x;
    patternBase = data + type * 0x100;
    part = work + 0x2C;
    count = 1;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    row = 0;
    *(s32*)(work + 0x20) = 0;
    *(s32*)work = type;
    *(s32*)(work + 0x1C) = 0x50;

    do {
        column = 0;
        pattern = patternBase + (15 - row) * 16;
        do {
            if (*pattern != '.') {
                random = rand() % 20 - 10;
                radians = angleScale / float_360_80425cf4;
                baseX = float_0p1_80425cec * (f32)random + (f32)((column - 8) * 2);
                *(f32*)(part + 4) = baseX * (f32)cos((f64)radians);
                random = rand() % 20 - 10;
                *(f32*)(part + 8) = float_0p1_80425cec * (f32)random + (f32)rowBase;
                *(f32*)(part + 0xC) = baseX * (f32)sin((f64)radians);
                *(f32*)(part + 0x10) = float_0p04_80425cf8 * *(f32*)(part + 4);
                *(f32*)(part + 0x14) = float_0p04_80425cf8 * *(f32*)(part + 8);
                *(f32*)(part + 0x18) = float_0p001_80425cfc * (f32)(rand() % 10);
                *(s32*)(part + 0x1C) = rand() % 7 + 0x1E;
                *(s32*)(part + 0x20) = 0;
                part += 0x2C;
                count++;
                if (count >= 0x65) {
                    break;
                }
            }
            column++;
            pattern++;
        } while (column < 16);
        if (count >= 0x65) {
            break;
        }
        row++;
        rowBase += 2;
    } while (row < 16);

    remaining = 0x65 - count;
    zero = float_0_80425cd4;
    if (count < 0x65) {
        groups = remaining >> 2;
        if (groups != 0) {
            do {
                *(f32*)(part + 0x24) = zero;
                *(f32*)(part + 0x18) = zero;
                *(f32*)(part + 0x14) = zero;
                *(f32*)(part + 0x10) = zero;
                *(f32*)(part + 0xC) = zero;
                *(f32*)(part + 8) = zero;
                *(f32*)(part + 4) = zero;
                *(s32*)(part + 0x1C) = -1;
                *(s32*)(part + 0x20) = 0;
                *(f32*)(part + 0x50) = zero;
                *(f32*)(part + 0x44) = zero;
                *(f32*)(part + 0x40) = zero;
                *(f32*)(part + 0x3C) = zero;
                *(f32*)(part + 0x38) = zero;
                *(f32*)(part + 0x34) = zero;
                *(f32*)(part + 0x30) = zero;
                *(s32*)(part + 0x48) = -1;
                *(s32*)(part + 0x4C) = 0;
                *(f32*)(part + 0x7C) = zero;
                *(f32*)(part + 0x70) = zero;
                *(f32*)(part + 0x6C) = zero;
                *(f32*)(part + 0x68) = zero;
                *(f32*)(part + 0x64) = zero;
                *(f32*)(part + 0x60) = zero;
                *(f32*)(part + 0x5C) = zero;
                *(s32*)(part + 0x74) = -1;
                *(s32*)(part + 0x78) = 0;
                *(f32*)(part + 0xA8) = zero;
                *(f32*)(part + 0x9C) = zero;
                *(f32*)(part + 0x98) = zero;
                *(f32*)(part + 0x94) = zero;
                *(f32*)(part + 0x90) = zero;
                *(f32*)(part + 0x8C) = zero;
                *(f32*)(part + 0x88) = zero;
                *(s32*)(part + 0xA0) = -1;
                *(s32*)(part + 0xA4) = 0;
                part += 0xB0;
            } while (--groups != 0);
            remaining &= 3;
            if (remaining == 0) {
                return entry;
            }
        }
        do {
            *(f32*)(part + 0x24) = zero;
            *(f32*)(part + 0x18) = zero;
            *(f32*)(part + 0x14) = zero;
            *(f32*)(part + 0x10) = zero;
            *(f32*)(part + 0xC) = zero;
            *(f32*)(part + 8) = zero;
            *(f32*)(part + 4) = zero;
            *(s32*)(part + 0x1C) = -1;
            *(s32*)(part + 0x20) = 0;
            part += 0x2C;
        } while (--remaining != 0);
    }
    return entry;
}
void effPeachDustMain(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effPeachDustDisp(s32, s32);
    extern const Vec3 vec3_802fbc68;
    extern u8 scale_data[];
    extern f32 float_0p01_80425ce8;
    extern f32 float_0p4_80425ce4;
    extern f32 float_0p1_80425cec;
    u8* work;
    u8* part;
    Vec3 dispPos;
    Vec3 pos;
    s32 i;
    s32 dead;
    s32 frame;
    s32 count;
    f32 scale;

    dead = 0;
    i = 0;
    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fbc68;
    pos.x = *(f32*)(work + 4);
    pos.y = *(f32*)(work + 8);
    pos.z = *(f32*)(work + 0xC);
    dispPos = pos;
    *(s32*)(work + 0x1C) -= 1;
    *(s32*)(work + 0x20) += 1;

    frame = *(s32*)(work + 0x20);
    part = work + 0x2C;
    while (i < *(s32*)((s32)effect + 8) - 1) {
        *(s32*)(part + 0x28) = (frame + i + i) & 7;
        if (*(s32*)(part + 0x1C) < 0) {
            dead++;
        }
        *(s32*)(part + 0x1C) -= 1;
        count = *(s32*)(part + 0x1C);
        if (count > 0x1E || count < 0) {
            *(s32*)(part + 0x28) = -1;
        } else {
            *(s32*)(part + 0x20) += 1;
            if (*(s32*)(part + 0x20) - 1 < 0x19) {
                *(f32*)(part + 0x24) =
                    float_0p4_80425ce4 * (float_0p01_80425ce8 * (f32)scale_data[*(s32*)(part + 0x20) - 1]);
            } else {
                scale = *(f32*)(part + 0x24);
                *(f32*)(part + 0x24) = scale + float_0p1_80425cec * (float_0p1_80425cec - scale);
            }
            *(f32*)(part + 4) += *(f32*)(part + 0x10);
            *(f32*)(part + 8) += *(f32*)(part + 0x14);
            *(f32*)(part + 0x14) += *(f32*)(part + 0x18);
            *(f32*)(part + 0xC) += *(f32*)(part + 0x10);
        }
        i++;
        part += 0x2C;
    }
    if (dead >= *(s32*)((s32)effect + 8) - 1) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effPeachDustDisp, effect, dispCalcZ(&dispPos));
    }
}

void effPeachDustDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void* camGetPtr(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void GXLoadTexMtxImm(Mtx,s32,s32);
    extern void effGetTexObjN64(s32,void*);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void PSMTXTrans(Mtx,f32,f32,f32);
    extern void PSMTXRotRad(Mtx,s32,f32);
    extern void PSMTXConcat(void*,void*,void*);
    extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32,s32,s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    extern void effSetVtxDescN64(void*);
    extern f32 float_0p0056818_80425ccc;
    extern f32 float_0p045455_80425cd0;
    extern f32 float_0_80425cd4;
    extern f32 float_deg2rad_80425cd8;
    extern f32 float_1p4_80425cdc;
    extern f32 float_0p7_80425ce0;
    extern u8 dat_80425cc8;
    extern u8 color_rotation_data[];
    extern u8 size8x8_tex22x22_vtx[];
    Mtx trans, rot, model, local;
    GXTexObj texObj;
    u8* work;
    char* camera;
    s32 i;
    s32 colorIndex;

    camera = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    PSMTXScale(local,float_0p0056818_80425ccc,float_0p045455_80425cd0,float_0_80425cd4);
    GXLoadTexMtxImm(local,0x1E,1);
    effGetTexObjN64(0x20,&texObj);
    GXLoadTexObj(&texObj,0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0,0,0,0xFF);
    GXSetTevColorOp(0,0,0,0,1,0);
    GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,2,12,8,15);
    GXSetTevAlphaIn(0,7,5,4,7);
    GXSetCullMode(0);

    colorIndex = (*(s32*)(work + 0x20) - 1) * 3;
    PSMTXTrans(local,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));
    PSMTXRotRad(rot,0x79,float_deg2rad_80425cd8 * -*(f32*)((char*)camGetPtr(4)+0x114));
    PSMTXConcat(local,rot,model);
    PSMTXConcat(camera+0x11C,model,model);
    for (i=0; i<*(s32*)((s32)effect+8)-1; i++,work+=0x2C) {
        u32 frame = *(u32*)(work+0x54);
        u8* vtx;
        u32 color;
        s32 r,g,b;
        if ((s32)frame < 0) continue;
        PSMTXTrans(local,*(f32*)(work+0x30),*(f32*)(work+0x34),*(f32*)(work+0x38));
        local[0][0]=*(f32*)(work+0x50);
        local[1][1]=local[0][0];
        local[2][2]=local[0][0];
        PSMTXConcat(model,local,local);
        GXLoadPosMtxImm(local,0);
        GXSetCurrentMtx(0);
        if (colorIndex > 0x23) colorIndex=0;
        r=(s32)((f32)color_rotation_data[colorIndex] * float_1p4_80425cdc);
        g=(s32)((f32)color_rotation_data[colorIndex+1] * float_0p7_80425ce0);
        b=(s32)((f32)color_rotation_data[colorIndex+2] * float_0p7_80425ce0);
        colorIndex += 3;
        if (r>0xFF) r=0xFF;
        if (g>0xFF) g=0xFF;
        if (b>0xFF) b=0xFF;
        color=((u8)r<<24)|((u8)g<<16)|((u8)b<<8)|dat_80425cc8;
        GXSetTevColor(1,&color);
        switch(frame & 7) {
            case 0: vtx=size8x8_tex22x22_vtx+0xA8; break;
            case 1: vtx=size8x8_tex22x22_vtx+0xE0; break;
            case 2: vtx=size8x8_tex22x22_vtx+0x118; break;
            case 3: vtx=size8x8_tex22x22_vtx+0x150; break;
            case 4: vtx=size8x8_tex22x22_vtx+0x188; break;
            case 5: vtx=size8x8_tex22x22_vtx; break;
            case 6: vtx=size8x8_tex22x22_vtx+0x38; break;
            case 7: vtx=size8x8_tex22x22_vtx+0x70; break;
        }
        effSetVtxDescN64(vtx);
        GXBegin(0x90,0,6);
        tri2(0,1,2,0,0,2,3,0);
    }
}
