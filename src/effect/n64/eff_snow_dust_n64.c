#include "effect/n64/eff_snow_dust_n64.h"

void* effSnowDustN64Entry(s32 type, f32 x, f32 y, f32 z, f32 arg4, f32 arg5, f32 scale, s32 count, s32 lifetime) {
    extern void* effEntry(void); extern void* __memAlloc(s32,u32); extern s32 rand(void); extern void effSnowDustMain(void);
    void* effect=effEntry(); u8* work; s32 i; f32 step;
    *(s32*)((s32)effect+8)=count+1; work=__memAlloc(3,(count+1)*0x6C); *(u8**)((s32)effect+0xC)=work; *(void**)((s32)effect+0x10)=effSnowDustMain; *(u32*)effect|=2;
    *(s32*)work=type;*(f32*)(work+4)=x;*(f32*)(work+8)=y;*(f32*)(work+0xC)=z;*(f32*)(work+0x10)=arg4;*(f32*)(work+0x14)=arg5;*(s32*)(work+0x34)=lifetime>0?lifetime:1000;*(s32*)(work+0x38)=0;*(s32*)(work+0x3C)=255;*(s32*)(work+0x40)=255;*(s32*)(work+0x44)=255;*(f32*)(work+0x5C)=scale;*(s32*)(work+0x68)=4;
    if(type==0){*(s32*)(work+0x4C)=255;*(s32*)(work+0x50)=255;*(s32*)(work+0x54)=255;*(s32*)(work+0x58)=255;}else if(type==1){*(s32*)(work+0x4C)=0;*(s32*)(work+0x50)=127;*(s32*)(work+0x54)=255;*(s32*)(work+0x58)=255;}else{*(s32*)(work+0x4C)=127;*(s32*)(work+0x50)=127;*(s32*)(work+0x54)=127;*(s32*)(work+0x58)=127;}
    step=(lifetime<30?(f32)lifetime:30.0f)/(f32)count;for(i=1;i<=count;i++){u8* p=work+i*0x6C;*(s32*)(p+0x30)=-(s32)((f32)i*step)-1;}return effect;
}

u8 effSnowDustMain(s32 effectAddress) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effDelete(void*);
    extern void* camGetPtr(s32);
    extern s32 rand(void);
    extern s32 irand(s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern f64 distABf(f64, f64, f64, f64);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effSnowDustDisp(void);
    extern u8 a_data[];
    extern u8 scale_data[];
    extern u8 scale_data2[];
    u8* effect = (u8*)effectAddress;
    u8* work = *(u8**)(effect + 0xC);
    u8* part = work + 0x6C;
    LocalVec3 position;
    s32 cameraId = *(s32*)(work + 0x68);
    s32 type = *(s32*)work;
    s32 timer;
    s32 frame;
    s32 i;
    s32 width;
    s32 height;
    f32 halfWidth;
    f32 angle;
    f32 cs;
    f32 sn;

    position.x = *(f32*)(work + 4);
    position.y = *(f32*)(work + 8);
    position.z = *(f32*)(work + 0xC);
    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x34) = 16;
    }
    if (*(s32*)(work + 0x34) < 1000) {
        (*(s32*)(work + 0x34))--;
    }
    (*(s32*)(work + 0x38))++;
    timer = *(s32*)(work + 0x34);
    if (timer < 0) {
        effDelete(effect);
        return 0;
    }
    if (timer < 8) {
        *(s32*)(work + 0x48) = timer << 5;
    }

    width = (s32)*(f32*)(work + 0x10);
    height = (s32)*(f32*)(work + 0x14);
    halfWidth = *(f32*)(work + 0x10) * 0.5f;
    angle = 6.2832f * *(f32*)((u8*)camGetPtr(4) + 0x114) / 360.0f;
    cs = (f32)cos(angle);
    sn = (f32)sin(angle);

    for (i = 1; i < *(s32*)(effect + 8); i++, part += 0x6C) {
        frame = ++*(s32*)(part + 0x30);
        if (frame > 29) {
            *(s32*)(part + 0x30) = timer < 30 ? 20 : 0;
            frame = *(s32*)(part + 0x30);
        }
        if (frame < 0) {
            continue;
        }
        if (frame == 0) {
            f32 offset = (f32)(rand() % width) - halfWidth;
            *(f32*)(part + 4) = position.x + offset * cs - sn;
            *(f32*)(part + 8) = position.y + (f32)(rand() % height);
            *(f32*)(part + 0xC) = position.z + offset * sn + cs;
            *(s32*)(part + 0x3C) = *(s32*)(work + 0x3C);
            *(s32*)(part + 0x40) = *(s32*)(work + 0x40);
            *(s32*)(part + 0x44) = *(s32*)(work + 0x44);
        }

        if (type == 0 || type == 1 || type == 6) {
            if (frame == 0) {
                *(f32*)(part + 0x18) = 0.0f;
                *(f32*)(part + 0x1C) = 0.0f;
                *(f32*)(part + 0x24) = 0.0f;
                *(f32*)(part + 0x28) = 0.05f * (f32)(-(rand() % 10) - 2);
                *(f32*)(part + 0x60) = 0.0f;
                *(f32*)(part + 0x64) = 0.0f;
                *(f32*)(part + 0x5C) = 0.5f + 0.05f * (f32)(rand() % 10);
                if (type == 6) {
                    *(s32*)(part + 0x38) = irand(360);
                }
            }
            *(u8*)(part + 0x48) = type == 6 ? 255 : a_data[frame];
            *(f32*)(part + 8) += *(f32*)(part + 0x1C);
            *(f32*)(part + 0x60) += 0.2f;
            *(f32*)(part + 0x64) += 0.6f;
            *(f32*)(part + 0x1C) += 0.04f * (*(f32*)(part + 0x28) - *(f32*)(part + 0x1C));
            if (type != 6) {
                *(f32*)(part + 4) += *(f32*)(part + 0x18);
                *(f32*)(part + 0x18) += 0.04f * (*(f32*)(part + 0x24) - *(f32*)(part + 0x18));
            } else {
                f32 distance = (f32)distABf(position.x, position.z,
                                            *(f32*)(part + 4), *(f32*)(part + 0xC));
                f32 orbit = 6.2832f * (f32)*(s32*)(part + 0x38) / 360.0f;
                *(f32*)(part + 4) = position.x + distance * 0.97f * (f32)sin(orbit);
                *(f32*)(part + 0xC) = position.z + distance * 0.97f * (f32)cos(orbit);
                *(s32*)(part + 0x38) = (*(s32*)(part + 0x38) + 4) % 360;
            }
        } else if (type == 2 || type == 3) {
            if (frame == 0) {
                if (type == 2) {
                    *(f32*)(part + 0x18) = 0.05f * (f32)((rand() % 20) - 10);
                    *(f32*)(part + 0x1C) = 0.05f * (f32)((rand() % 20) - 10);
                } else {
                    *(f32*)(part + 0x18) = 0.0f;
                    *(f32*)(part + 0x1C) = 0.0f;
                }
                *(f32*)(part + 0x24) = 0.0f;
                *(f32*)(part + 0x28) = 0.0f;
                *(f32*)(part + 0x60) = (f32)(rand() % 15);
                *(f32*)(part + 0x64) = (f32)(rand() % 15);
                *(u8*)(part + 0x48) = 255;
            }
            *(f32*)(part + 0x5C) = 0.1f * (f32)scale_data[frame];
            *(f32*)(part + 4) += *(f32*)(part + 0x18);
            *(f32*)(part + 8) += *(f32*)(part + 0x1C);
            *(f32*)(part + 0x60) += 0.2f;
            *(f32*)(part + 0x64) += 0.6f;
            *(f32*)(part + 0x18) += 0.04f * (*(f32*)(part + 0x24) - *(f32*)(part + 0x18));
            *(f32*)(part + 0x1C) += 0.04f * (*(f32*)(part + 0x28) - *(f32*)(part + 0x1C));
        } else {
            if (frame == 0) {
                s32 red = rand() % 127 + 128;
                s32 green = rand() % (255 - (red - 128)) + 128;
                s32 blue = rand() % ((255 - (red - 128)) - (green - 128)) + 128;
                *(f32*)(part + 0x60) = 0.0f;
                *(f32*)(part + 0x64) = 0.0f;
                *(u8*)(part + 0x48) = 255;
                *(s32*)(part + 0x3C) = red;
                *(s32*)(part + 0x40) = green;
                *(s32*)(part + 0x44) = blue;
            }
            *(f32*)(part + 0x5C) = 0.01f * (f32)(type == 4 ? scale_data2[frame] : scale_data[frame]);
        }
    }
    dispEntry(cameraId, 2, effSnowDustDisp, effect, dispCalcZ(&position));
    return 0;
}


u8 effSnowDustDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevKColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32);
    u8 texObj[0x20];
    Mtx texMtx0, texMtx1, trans, rot, scale, model;
    u8* work = *(u8**)(effectAddress + 0xC);
    u8* part = work + 0x6C;
    u8* camera = camGetPtr(cameraId);
    s32 type = *(s32*)work;
    s32 count = *(s32*)(effectAddress + 8);
    f32 rootScale = ((f32)*(s32*)(work + 0x48) / 255.0f) * *(f32*)(work + 0x5C);
    u32 color;
    u32 kcolor;
    s32 i;

    color = (*(s32*)(work + 0x4C) << 24) |
            (*(s32*)(work + 0x50) << 16) |
            (*(s32*)(work + 0x54) << 8) |
            *(s32*)(work + 0x58);
    GXSetTevColor(2, &color);

    GXSetNumChans(0);
    if (type == 2 || type == 3) {
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 2, 1, 8, 0);
        GXSetTevAlphaIn(0, 0, 4, 5, 7);
        effGetTexObjN64(0x67, texObj);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx0, 0.00390625f, 0.015625f, 0.0f);
    } else if (type == 4 || type == 5) {
        GXSetNumTevStages(2);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0, 8, 7, 2);
        GXSetTevAlphaIn(0, 0, 4, 5, 7);
        GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
        GXSetTevColorOp(1, 1, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 0, 1, 7, 0);
        GXSetTevAlphaIn(1, 0, 0, 0, 0);
        effGetTexObjN64(0x68, texObj);
        GXLoadTexObj(texObj, 0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        PSMTXScale(texMtx0, 0.015625f, 0.015625f, 0.0f);
    } else {
        GXSetNumTevStages(2);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0, 0, 0, 0);
        GXSetTevAlphaIn(0, 0, 4, 5, 7);
        GXSetTevOrder(1, 1, 1, 0xFF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
        GXSetTevColorIn(1, 2, 1, 8, 0);
        GXSetTevAlphaIn(1, 0, 0, 0, 0);
        effGetTexObjN64(type == 1 ? 0x66 : type == 6 ? 8 : 0x68, texObj);
        GXLoadTexObj(texObj, 0);
        if (type == 1 || type == 6) {
            effGetTexObjN64(0x65, texObj);
            GXLoadTexObj(texObj, 1);
            GXSetNumTexGens(2);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
            PSMTXScale(texMtx0, type == 1 ? 0.03125f : 0.015625f,
                       type == 1 ? 0.03125f : 0.015625f, 0.0f);
            PSMTXScale(texMtx1, type == 1 ? 0.03125f : 0.015625f,
                       type == 1 ? 0.03125f : 0.015625f, 0.0f);
        } else {
            GXSetNumTexGens(1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
            PSMTXScale(texMtx0, 0.015625f, 0.015625f, 0.0f);
        }
    }
    GXSetCullMode(0);

    for (i = 1; i < count; i++, part += 0x6C) {
        f32 particleScale;
        if (*(s32*)(part + 0x30) < 0) {
            continue;
        }
        PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXRotRad(rot, -0.017453292f * *(f32*)((u8*)camGetPtr(4) + 0x114), 'y');
        PSMTXConcat(trans, rot, trans);
        particleScale = *(f32*)(part + 0x5C) * rootScale;
        PSMTXScale(scale, particleScale, particleScale, particleScale);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(camera + 0x118, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);

        color = ((*(s32*)(part + 0x3C) & 0xFF) << 24) |
                ((*(s32*)(part + 0x40) & 0xFF) << 16) |
                ((*(s32*)(part + 0x44) & 0xFF) << 8) |
                (s32)(rootScale * (f32)*(u8*)(part + 0x48));
        GXSetTevColor(1, &color);
        kcolor = (0x50 - *(s32*)(part + 0x30)) * 0x01010101;
        GXSetTevKColor(0, &kcolor);
        GXLoadTexMtxImm(texMtx0, 0x1E, 1);
        PSMTXTrans(trans, *(f32*)(part + 0x60), *(f32*)(part + 0x64), 0.0f);
        PSMTXConcat(texMtx1, trans, trans);
        GXLoadTexMtxImm(trans, 0x21, 1);

        if (type == 2) {
            PSMTXTrans(trans, (f32)((i & 0x1F) << 6), 64.0f, 0.0f);
            PSMTXConcat(texMtx0, trans, trans);
            GXLoadTexMtxImm(trans, 0x1E, 1);
        }
        effSetVtxDescN64((void*)(type == 1 ? 0x803ABA80 :
                                  type == 2 || type == 3 ? 0x803ABAF0 : 0x803ABAB8));
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3);
    }
    return 0;
}

