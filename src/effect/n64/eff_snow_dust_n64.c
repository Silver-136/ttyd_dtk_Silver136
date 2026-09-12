#include "effect/n64/eff_snow_dust_n64.h"

extern void* camGetPtr(s32);

void* effSnowDustN64Entry(s32 type, f32 x, f32 y, f32 z, f32 width, f32 height, f32 scale, s32 count, s32 lifetime) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct SnowPart {
        s32 type; Vec3 pos; f32 width, height; f32 velX, velY, unk20, goalX, goalY;
        s32 unk2C, animTime, timeLeft, lifetime;
        s32 innerR, innerG, innerB, innerA;
        s32 glowR, glowG, glowB, glowA;
        f32 scale, scrollX, scrollY; s32 cameraId;
    } SnowPart;
    extern void* effEntry(void);
    extern void* __memAlloc(s32, u32);
    extern s32 rand(void);
    extern void effSnowDustMain(void);
    extern char str_SnowDustN64_802fc2b0[];
    void* effect;
    SnowPart* work;
    SnowPart* part;
    f32 interval;
    s32 i;

    count++;
    effect = effEntry();
    *(char**)((s32)effect + 0x14) = str_SnowDustN64_802fc2b0;
    *(s32*)((s32)effect + 8) = count;
    work = __memAlloc(3, count * sizeof(*work));
    *(SnowPart**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effSnowDustMain;
    *(u32*)effect |= 2;
    work->type = type;
    work->lifetime = 0;
    if (lifetime < 1) work->timeLeft = 1000;
    else work->timeLeft = lifetime;
    work->innerA = 255;
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    work->scale = scale;
    work->width = width;
    work->height = height;
    work->cameraId = 4;
    work->innerR = 255;
    work->innerG = 255;
    work->innerB = 255;

    switch (type) {
        case 0:
            work->glowR = 255; work->glowG = 255; work->glowB = 255; work->glowA = 255;
            break;
        case 1:
            work->glowR = 0; work->glowG = 127; work->glowB = 255; work->glowA = 255;
            break;
        case 2:
        case 3:
            work->glowR = 255; work->glowG = 255; work->glowB = 128; work->glowA = 255;
            break;
        case 6:
            work->innerR = 203; work->innerG = 148; work->innerB = 205;
            work->glowR = 203; work->glowG = 148; work->glowB = 205;
            break;
        default:
            work->innerR = rand() % 127 + 128;
            work->innerG = rand() % (255 - (work->innerR - 128)) + 128;
            work->innerB = rand() % ((255 - (work->innerR - 128)) - (work->innerG - 128)) + 128;
            work->glowR = 127; work->glowG = 127; work->glowB = 127; work->glowA = 127;
            break;
    }
    if (work->timeLeft < 30) interval = (f32)work->timeLeft / (f32)(count - 1);
    else interval = 30.0f / (f32)(count - 1);
    part = work + 1;
    for (i = 1; i < count; i++, part++) {
        part->animTime = (s32)(-(f32)i * interval) - 1;
    }
    return effect;
}

u8 effSnowDustMain(s32 effectAddress) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct SnowPart {
        s32 type;
        Vec3 pos;
        f32 width, height;
        f32 velX, velY;
        f32 unk20;
        f32 goalX, goalY;
        s32 unk2C;
        s32 animTime, timeLeft, lifetime;
        s32 innerR, innerG, innerB, innerA;
        s32 glowR, glowG, glowB, glowA;
        f32 scale, scrollX, scrollY;
        s32 cameraId;
    } SnowPart;
    extern void effDelete(void*);
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
    SnowPart* particle = *(SnowPart**)(effect + 0xC);
    Vec3 position;
    Vec3 renderPosition;
    s32 variation;
    s32 cameraId;
    s32 timeLeft;
    f32 width;
    f32 height;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 tempCos;
    f32 tempSin;
    s32 widthInt;
    s32 heightInt;
    s32 innerR;
    s32 innerG;
    s32 innerB;
    s32 i;

    position = particle->pos;
    renderPosition = position;
    cameraId = particle->cameraId;
    variation = particle->type;
    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        particle->timeLeft = 16;
    }
    if (particle->timeLeft < 1000) particle->timeLeft--;
    particle->lifetime++;
    timeLeft = particle->timeLeft;
    if (timeLeft < 0) {
        effDelete(effect);
        return 0;
    }
    if (timeLeft < 8) particle->innerA = timeLeft << 5;

    width = particle->width;
    height = particle->height;
    posX = particle->pos.x;
    posY = particle->pos.y;
    posZ = particle->pos.z;
    tempCos = (f32)cos((6.2832f * *(f32*)((u8*)camGetPtr(4) + 0x114)) / 360.0f);
    tempSin = (f32)sin((6.2832f * *(f32*)((u8*)camGetPtr(4) + 0x114)) / 360.0f);
    widthInt = (s32)width;
    heightInt = (s32)height;
    innerR = particle->innerR;
    innerG = particle->innerG;
    width *= 0.5f;
    innerB = particle->innerB;

    particle++;
    for (i = 1; i < *(s32*)(effect + 8); i++, particle++) {
        particle->animTime++;
        if (particle->animTime >= 30) {
            if (timeLeft < 30) particle->animTime = 20;
            else particle->animTime = 0;
        }
        if (particle->animTime >= 0) {
            if (particle->animTime == 0) {
                f32 randMag = (f32)(rand() % widthInt) - width;
                particle->pos.x = posX + randMag * tempCos - tempSin;
                particle->pos.y = posY + (f32)(rand() % heightInt);
                particle->pos.z = posZ + randMag * tempSin + tempCos;
                particle->innerR = innerR;
                particle->innerG = innerG;
                particle->innerB = innerB;
            }
            if (variation < 4) {
                if (variation < 2) {
                    if (variation >= 0) {
                        if (particle->animTime == 0) {
                            particle->velX = 0.0f; particle->velY = 0.0f; particle->goalX = 0.0f;
                            particle->goalY = (f32)(-(rand() % 10) - 2) * 0.05f;
                            particle->scrollX = 0.0f; particle->scrollY = 0.0f;
                            particle->scale = (f32)(rand() % 10) * 0.05f + 0.5f;
                        }
                        *(u8*)&particle->innerA = a_data[particle->animTime];
                        particle->pos.x += particle->velX;
                        particle->pos.y += particle->velY;
                        particle->scrollX += 0.2f; particle->scrollY += 0.6f;
                        particle->velX = (particle->goalX - particle->velX) * 0.04f + particle->velX;
                        particle->velY = (particle->goalY - particle->velY) * 0.04f + particle->velY;
                    }
                } else {
                    if (particle->animTime == 0) {
                        if (variation == 2) {
                            particle->velX = (f32)(rand() % 20 - 10) * 0.05f;
                            particle->velY = (f32)(rand() % 20 - 10) * 0.05f;
                        } else {
                            particle->velX = 0.0f; particle->velY = 0.0f;
                        }
                        particle->goalX = 0.0f; particle->goalY = 0.0f;
                        particle->scrollX = (f32)(rand() % 15);
                        particle->scrollY = (f32)(rand() % 15);
                        *(u8*)&particle->innerA = 255;
                    }
                    particle->scale = (f32)scale_data[particle->animTime] * 0.1f;
                    particle->pos.x += particle->velX;
                    particle->pos.y += particle->velY;
                    particle->scrollX += 0.2f; particle->scrollY += 0.6f;
                    particle->velX = (particle->goalX - particle->velX) * 0.04f + particle->velX;
                    particle->velY = (particle->goalY - particle->velY) * 0.04f + particle->velY;
                }
            } else if (variation == 6) {
                if (particle->animTime == 0) {
                    particle->velX = 0.0f; particle->velY = 0.0f; particle->goalX = 0.0f;
                    particle->goalY = (f32)(-(rand() % 10) - 2) * 0.05f;
                    particle->scrollX = 0.0f; particle->scrollY = 0.0f;
                    particle->scale = (f32)(rand() % 10) * 0.05f + 0.5f;
                    particle->unk2C = irand(360);
                }
                *(u8*)&particle->innerA = a_data[particle->animTime];
                *(u8*)&particle->innerA = 255;
                particle->pos.y += particle->velY;
                particle->scrollX += 0.2f; particle->scrollY += 0.6f;
                particle->velY = (particle->goalY - particle->velY) * 0.04f + particle->velY;
                {
                    f32 distance = (f32)distABf(position.x, position.z, particle->pos.x, particle->pos.z);
                    particle->pos.x = distance * 0.97f * (f32)sin((6.2832f * (f32)particle->unk2C) / 360.0f) + position.x;
                    particle->pos.z = distance * 0.97f * (f32)cos((6.2832f * (f32)particle->unk2C) / 360.0f) + position.z;
                    particle->unk2C = (particle->unk2C + 4) % 360;
                }
            } else {
                if (particle->animTime == 0) {
                    particle->scrollX = 0.0f; particle->scrollY = 0.0f;
                    *(u8*)&particle->innerA = 255;
                    particle->innerR = rand() % 127 + 128;
                    particle->innerG = rand() % (255 - (particle->innerR - 128)) + 128;
                    particle->innerB = rand() % ((255 - (particle->innerR - 128)) - (particle->innerG - 128)) + 128;
                }
                if (variation == 4) particle->scale = (f32)scale_data2[particle->animTime] * 0.01f;
                else particle->scale = (f32)scale_data[particle->animTime] * 0.01f;
            }
        }
    }
    dispEntry(cameraId, 2, effSnowDustDisp, effect, dispCalcZ(&renderPosition));
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
    extern void GXSetTevKColorSel(s32, s32);
    extern void GXSetTevKAlphaSel(s32, s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
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
    extern void tri2();
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

    color = (*(s32*)(work + 0x4C) << 24) | (*(s32*)(work + 0x50) << 16) |
            (*(s32*)(work + 0x54) << 8) | *(s32*)(work + 0x58);
    GXSetTevColor(2, &color);
    if (type == 0 || type == 6) {
        GXSetNumChans(0); GXSetNumTevStages(2);
        GXSetTevOrder(0,0,0,0xFF); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
        GXSetTevColorIn(0,0xF,0xF,0xF,0xF); GXSetTevAlphaIn(0,7,4,1,7);
        GXSetTevOrder(1,1,1,0xFF); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0);
        GXSetTevColorIn(1,4,2,8,0xF); GXSetTevAlphaIn(1,7,7,7,0);
        effGetTexObjN64(8,texObj); GXLoadTexObj(texObj,0); effGetTexObjN64(0x65,texObj); GXLoadTexObj(texObj,1);
        GXSetNumTexGens(2); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
        PSMTXScale(texMtx0,0.015625f,0.015625f,0.0f); PSMTXScale(texMtx1,0.015625f,0.015625f,0.0f);
    } else if (type == 1) {
        GXSetNumChans(0); GXSetNumTevStages(2);
        GXSetTevOrder(0,0,0,0xFF); GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
        GXSetTevColorIn(0,0xF,0xF,0xF,0xF); GXSetTevAlphaIn(0,7,4,1,7);
        GXSetTevOrder(1,1,1,0xFF); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0);
        GXSetTevColorIn(1,4,2,8,0xF); GXSetTevAlphaIn(1,7,7,7,0);
        GXSetTevOrder(2,1,1,0xFF); GXSetTevColorOp(2,0,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0);
        GXSetTevColorIn(2,0,0xC,8,0xF); GXSetTevAlphaIn(2,7,7,7,0);
        effGetTexObjN64(0x66,texObj); GXLoadTexObj(texObj,0); effGetTexObjN64(0x65,texObj); GXLoadTexObj(texObj,1);
        GXSetNumTexGens(2); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
        PSMTXScale(texMtx0,0.03125f,0.03125f,0.0f); PSMTXScale(texMtx1,0.03125f,0.03125f,0.0f);
    } else if (type == 2 || type == 3) {
        GXSetNumChans(0); GXSetNumTevStages(1); GXSetTevOrder(0,0,0,0xFF);
        GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
        GXSetTevColorIn(0,4,2,8,0xF); GXSetTevAlphaIn(0,7,4,1,7);
        effGetTexObjN64(0x67,texObj); GXLoadTexObj(texObj,0); GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); PSMTXScale(texMtx0,0.00390625f,0.015625f,0.0f);
    } else if (type == 4 || type == 5) {
        GXSetNumChans(0); GXSetNumTevStages(2); GXSetTevOrder(0,0,0,0xFF);
        GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
        GXSetTevColorIn(0,0xF,8,5,2); GXSetTevAlphaIn(0,7,4,1,7);
        GXSetTevOrder(1,0xFF,0xFF,0xFF); GXSetTevColorOp(1,1,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0);
        GXSetTevColorIn(1,0xF,4,5,0); GXSetTevAlphaIn(1,7,7,7,0);
        effGetTexObjN64(0x68,texObj); GXLoadTexObj(texObj,0); GXSetNumTexGens(1);
        GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); PSMTXScale(texMtx0,0.015625f,0.015625f,0.0f);
    }
    GXSetCullMode(0);
    for (i=1; i<count; i++,part+=0x6C) {
        f32 particleScale;
        if (*(s32*)(part+0x30)<0) continue;
        PSMTXTrans(trans,*(f32*)(part+4),*(f32*)(part+8),*(f32*)(part+0xC));
        PSMTXRotRad(rot,-0.017453292f**(f32*)((u8*)camGetPtr(4)+0x114),'y');
        PSMTXConcat(trans,rot,trans); particleScale=*(f32*)(part+0x5C)*rootScale;
        PSMTXScale(scale,particleScale,particleScale,particleScale); PSMTXConcat(trans,scale,trans);
        PSMTXConcat(camera+0x118,trans,trans); GXLoadPosMtxImm(trans,0); GXSetCurrentMtx(0);
        color=((*(s32*)(part+0x3C)&0xFF)<<24)|((*(s32*)(part+0x40)&0xFF)<<16)|
              ((*(s32*)(part+0x44)&0xFF)<<8)|(s32)(rootScale*(f32)*(u8*)(part+0x48));
        GXSetTevColor(1,&color); kcolor=(0x50-*(s32*)(part+0x30))*0x01010101; GXSetTevKColor(0,&kcolor);
        GXLoadTexMtxImm(texMtx0,0x1E,1); PSMTXTrans(trans,*(f32*)(part+0x60),*(f32*)(part+0x64),0.0f);
        PSMTXConcat(texMtx1,trans,trans); GXLoadTexMtxImm(trans,0x21,1);
        if (type==2) { PSMTXTrans(trans,(f32)((i&0x1F)<<6),64.0f,0.0f); PSMTXConcat(texMtx0,trans,trans); GXLoadTexMtxImm(trans,0x1E,1); }
        if (type < 4 || type == 6) {
            effSetVtxDescN64((void*)(type==1?0x803ABA80:type==2||type==3?0x803ABAF0:0x803ABAB8));
            GXBegin(0x90,0,6); tri2(0,1,2,0,0,2,3,0);
        } else if (type < 6) {
            GXSetNumChans(0); GXSetNumTevStages(3); GXSetTevOrder(0,0,0,0xFF);
            GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
            GXSetTevColorIn(0,0xF,8,5,2); GXSetTevAlphaIn(0,7,4,1,7);
            GXSetTevOrder(1,0xFF,0xFF,0xFF); GXSetTevColorOp(1,1,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0);
            GXSetTevColorIn(1,0xF,4,5,0); GXSetTevAlphaIn(1,7,7,7,0);
            GXSetTevOrder(2,0xFF,0xFF,0xFF); GXSetTevColorOp(2,1,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0);
            GXSetTevColorIn(2,0xE,0xF,0xF,0); GXSetTevAlphaIn(2,7,7,7,0); GXSetTevKColorSel(2,0xC);
            effSetVtxDescN64((void*)0x803ABB28); GXBegin(0x90,0,6); tri2(0,1,2,0,0,2,3,0);
            GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2); GXSetNumTevStages(1); GXSetTevOrder(0,0xFF,0xFF,4);
            GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
            GXSetTevColorIn(0,0xF,0xF,0xF,0xC); GXSetTevAlphaIn(0,7,5,6,7); GXSetTevKAlphaSel(0,0x1C);
            effSetVtxDescN64((void*)0x803ABB60); GXBegin(0x90,0,0xC);
            tri2(0,1,4,0,1,2,4,0); tri2(0,4,3,0,4,2,3,0);
        }
    }
    return 0;
}

__declspec(section ".data") f32 negone_one_374[2] = { -1.0f, 1.0f };
__declspec(section ".data") f32 gap_05_803ABA50_data[2] = { -1.0f, 1.0f };
__declspec(section ".data") f32 one_negone_373[2] = { 1.0f, -1.0f };
__declspec(section ".data") f32 gap_05_803ABA60_data[2] = { 1.0f, -1.0f };
__declspec(section ".data") f32 negone_one_360[2] = { -1.0f, 1.0f };
__declspec(section ".data") f32 gap_05_803ABA70_data[2] = { -1.0f, 1.0f };
__declspec(section ".data") f32 one_negone_359[2] = { 1.0f, -1.0f };

typedef struct EffN64Vertex {
    s16 x;
    s16 y;
    s16 z;
    s16 s;
    s16 t;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} EffN64Vertex;

EffN64Vertex size8x8_tex64x64_vtx[4] = {
    { -4, -4, 0, 0, 0, 0x00, 0x00, 0x00, 0xFF },
    { 4, -4, 0, 2048, 0, 0x00, 0x00, 0x00, 0xFF },
    { 4, 4, 0, 2048, 2048, 0x00, 0x00, 0x00, 0xFF },
    { -4, 4, 0, 0, 2048, 0x00, 0x00, 0x00, 0xFF },
};

EffN64Vertex size4x4_tex64x64_vtx[4] = {
    { -2, -2, 0, 0, 0, 0x00, 0x00, 0x00, 0xFF },
    { 2, -2, 0, 2048, 0, 0x00, 0x00, 0x00, 0xFF },
    { 2, 2, 0, 2048, 2048, 0x00, 0x00, 0x00, 0xFF },
    { -2, 2, 0, 0, 2048, 0x00, 0x00, 0x00, 0xFF },
};

EffN64Vertex size12x12_tex64x64_vtx[4] = {
    { -6, -6, 0, 0, 0, 0x00, 0x00, 0x00, 0xFF },
    { 6, -6, 0, 2048, 0, 0x00, 0x00, 0x00, 0xFF },
    { 6, 6, 0, 2048, 2048, 0x00, 0x00, 0x00, 0xFF },
    { -6, 6, 0, 0, 2048, 0x00, 0x00, 0x00, 0xFF },
};

EffN64Vertex size16x16_tex64x64_vtx[4] = {
    { -8, -8, 0, 0, 0, 0x00, 0x00, 0x00, 0xFF },
    { 8, -8, 0, 2048, 0, 0x00, 0x00, 0x00, 0xFF },
    { 8, 8, 0, 2048, 2048, 0x00, 0x00, 0x00, 0xFF },
    { -8, 8, 0, 0, 2048, 0x00, 0x00, 0x00, 0xFF },
};

EffN64Vertex size32x32_tex32x32_vtx[5] = {
    { -32, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0x00 },
    { 0, -32, 0, 1024, 0, 0xFF, 0xFF, 0xFF, 0x00 },
    { 32, 0, 0, 1024, 1024, 0xFF, 0xFF, 0xFF, 0x00 },
    { 0, 32, 0, 0, 1024, 0xFF, 0xFF, 0xFF, 0x00 },
    { 0, 0, 0, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF },
};
