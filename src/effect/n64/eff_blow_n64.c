#include "effect/n64/eff_blow_n64.h"


void* effBlowN64Entry(f32 x,f32 y,f32 z,f32 dx,f32 dy,f32 dz,s32 type,s32 count) {
    extern void* effEntry(void);extern void* __memAlloc(s32,s32);extern f64 sqrt(f64);extern s32 rand(void);extern void effBlowMain(void*);extern char str_BlowN64_802fabb0[];
    void* entry=effEntry();u8* work;u8* p;f32 len,inv,nx,ny,nz,px,py,pz,plen; s32 i;
    if(count==0)return 0;len=(f32)sqrt((f64)(dx*dx+dy*dy+dz*dz));if(len==0.0f)return 0;inv=-1.0f/len;nx=dx*inv;ny=dy*inv;nz=dz*inv;
    if(nx==0.0f){px=0.0f;if(ny==0.0f){py=1.0f;pz=-nx/nz;}else{py=-nx/ny;pz=1.0f;}}else{px=1.0f;py=-ny/nx;pz=0.0f;}
    plen=(f32)sqrt((f64)(px*px+py*py+pz*pz));if(plen==0.0f)return 0;px/=plen;py/=plen;pz/=plen;
    *(char**)((u8*)entry+0x14)=str_BlowN64_802fabb0;*(s32*)((u8*)entry+8)=count+1;work=__memAlloc(3,(count+1)*0x68);*(u8**)((u8*)entry+0xC)=work;*(void**)((u8*)entry+0x10)=effBlowMain;
    *(s32*)(work+4)=type;*(f32*)(work+8)=x;*(f32*)(work+0xC)=y;*(f32*)(work+0x10)=z;*(s32*)(work+0x5C)=10;*(s32*)(work+0x60)=0;
    p=work+0x68;for(i=1;i<count+1;i++,p+=0x68){f32 a=(f32)((rand()%201)-100)*0.01f;f32 b=(f32)((rand()%201)-100)*0.01f;*(f32*)(p+8)=0.0f;*(f32*)(p+0xC)=0.0f;*(f32*)(p+0x10)=0.0f;*(f32*)(p+0x14)=nx*8.0f+px*a+pz*b;*(f32*)(p+0x18)=ny*8.0f+py*a;*(f32*)(p+0x1C)=nz*8.0f+pz*a-px*b;*(f32*)(p+0x20)=*(f32*)(p+0x14);*(f32*)(p+0x24)=*(f32*)(p+0x18);*(f32*)(p+0x28)=*(f32*)(p+0x1C);*(f32*)(p+0x44)=0.5f;*(f32*)(p+0x48)=4.0f;*(f32*)(p+0x50)=20.0f;*(s32*)(p+0x58)=255;*(s32*)(p+0x5C)=10;*(s32*)(p+0x60)=0;*(s32*)(p+0x64)=i;}
    return entry;
}

void effBlowMain(int* entry) {
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void effBlowDisp(void);
    extern f32 float_0_80424d00;
    extern f32 float_0p5_80424d08;
    extern f32 float_4_80424d0c;
    extern f32 float_20_80424d10;
    extern f32 float_8_80424d14;
    extern f32 float_0p03_80424d18;
    extern f32 FLOAT_80424d1c;
    extern f32 float_6_80424d20;
    extern f32 float_0p2_80424d24;
    extern f32 float_0p225_80424d28;
    extern f32 float_0p45_80424d2c;
    extern f32 float_0p8_80424d30;
    u8* work = (u8*)entry[3];
    u8* item;
    f32 pos[3];
    s32 timer;
    s32 type;
    s32 i;

    pos[0] = *(f32*)(work + 8);
    pos[1] = *(f32*)(work + 0xC);
    pos[2] = *(f32*)(work + 0x10);

    *(s32*)(work + 0x5C) -= 1;
    *(s32*)(work + 0x60) += 1;
    timer = *(s32*)(work + 0x5C);
    type = *(s32*)(work + 4);
    if ((timer < 0) && (type == 2)) {
        effDelete(entry);
        return;
    }

    item = work + 0x68;
    for (i = 1; i < entry[2]; i++, item += 0x68) {
        s32 subTimer;

        if (*(s32*)(item + 0x64) > 0) {
            *(s32*)(item + 0x64) -= 1;
            if (*(s32*)(item + 0x64) > 0) {
                continue;
            }
        }

        *(s32*)(item + 0x5C) -= 1;
        subTimer = *(s32*)(item + 0x5C);
        if (subTimer < 0) {
            if (type == 0) {
                *(f32*)(item + 8) = float_0_80424d00;
                *(f32*)(item + 0xC) = float_0_80424d00;
                *(f32*)(item + 0x10) = float_0_80424d00;
                *(f32*)(item + 0x14) = *(f32*)(item + 0x20);
                *(f32*)(item + 0x18) = *(f32*)(item + 0x24);
                *(f32*)(item + 0x1C) = *(f32*)(item + 0x28);
                *(f32*)(item + 0x44) = float_0p5_80424d08;
                *(f32*)(item + 0x48) = float_4_80424d0c;
                *(f32*)(item + 0x50) = float_20_80424d10;
                *(f32*)(item + 0x4C) = *(f32*)(item + 0x54);
                *(s32*)(item + 0x58) = 0xFF;
                *(s32*)(item + 0x5C) = 10;
                *(s32*)(item + 0x5C) -= 1;
                subTimer = *(s32*)(item + 0x5C);
            } else {
                *(f32*)(item + 8) = float_0_80424d00;
                *(f32*)(item + 0xC) = float_0_80424d00;
                *(f32*)(item + 0x10) = float_0_80424d00;
                *(f32*)(item + 0x14) = float_0_80424d00;
                *(f32*)(item + 0x18) = float_0_80424d00;
                *(f32*)(item + 0x1C) = float_0_80424d00;
                *(s32*)(item + 0x58) = 0;
                *(s32*)(item + 0x5C) = 0;
            }
        }

        if (((u32)(subTimer - 5) < 2) || (subTimer == 4)) {
            *(f32*)(item + 0x14) += *(f32*)(item + 0x2C);
            *(f32*)(item + 0x18) += *(f32*)(item + 0x30);
            *(f32*)(item + 0x1C) += *(f32*)(item + 0x34);
        }

        if (subTimer < 8) {
            *(f32*)(item + 0x44) += float_8_80424d14 * float_0p03_80424d18 * (FLOAT_80424d1c - *(f32*)(item + 0x44));
            *(f32*)(item + 0x48) += float_6_80424d20 * float_0p03_80424d18 * (float_0p5_80424d08 - *(f32*)(item + 0x48));
        }

        *(f32*)(item + 0x14) += *(f32*)(item + 0x38);
        *(f32*)(item + 0x18) += *(f32*)(item + 0x3C);
        *(f32*)(item + 0x1C) += *(f32*)(item + 0x40);
        if (*(f32*)(item + 0x18) < float_0_80424d00) {
            *(f32*)(item + 0x50) *= float_0p2_80424d24;
            *(f32*)(item + 0x14) *= float_0p225_80424d28;
            *(f32*)(item + 0x18) *= float_0p45_80424d2c;
            *(f32*)(item + 0x1C) *= float_0p225_80424d28;
            *(s32*)(item + 0x58) = (s32)((f32)*(s32*)(item + 0x58) * float_0p8_80424d30);
        }

        *(f32*)(item + 8) += *(f32*)(item + 0x14);
        *(f32*)(item + 0xC) += *(f32*)(item + 0x18);
        *(f32*)(item + 0x10) += *(f32*)(item + 0x1C);
        *(f32*)(item + 0x4C) += *(f32*)(item + 0x50);
    }

    if (timer < 0) {
        work = (u8*)entry[3];
        if (*(s32*)(work + 4) == 1) {
            *(s32*)(work + 4) = 2;
        }
        *(s32*)(work + 0x5C) = 10;
        *(s32*)(work + 0x60) = 0;
    }

    dispEntry(4, 2, effBlowDisp, entry, dispCalcZ(pos));
}

void effBlowDisp(int param_1, void* param_2) {
    extern void* camGetPtr(s32 id);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 coord, s32 type, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 count);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
    extern f32 float_0p03125_80424cfc;
    extern f32 float_0_80424d00;
    extern f32 float_deg2rad_80424d04;
    extern u32 dat_80424cf8;
    extern u8 size16x16_tex32x32_vtx[];
    u8* work;
    u8* item;
    void* cam;
    f32 baseMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    u8 texObj[0x20];
    u32 color;
    s32 i;

    cam = camGetPtr(param_1);
    work = *(u8**)((s32)param_2 + 0xC);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xA, 2, 8, 0xF);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    effGetTexObjN64(0x34, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p03125_80424cfc, float_0p03125_80424cfc, float_0_80424d00);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    GXSetCullMode(0);
    effSetVtxDescN64(size16x16_tex32x32_vtx);
    PSMTXTrans(baseMtx, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));

    item = work + 0x68;
    for (i = 1; i < *(s32*)((s32)param_2 + 8); i++, item += 0x68) {
        color = dat_80424cf8;
        ((u8*)&color)[3] = *(s32*)(item + 0x58);
        GXSetTevColor(1, &color);

        PSMTXTrans(transMtx, *(f32*)(item + 8), *(f32*)(item + 0xC), *(f32*)(item + 0x10));
        PSMTXRotRad(rotMtx, 0x79, -(*(f32*)((s32)camGetPtr(4) + 0x114) * float_deg2rad_80424d04));
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXScale(scaleMtx, *(f32*)(item + 0x44), *(f32*)(item + 0x48), *(f32*)(item + 0x44));
        PSMTXConcat(transMtx, scaleMtx, transMtx);
        PSMTXRotRad(rotMtx, 0x7A, *(f32*)(item + 0x4C) * float_deg2rad_80424d04);
        PSMTXConcat(transMtx, rotMtx, transMtx);
        PSMTXConcat(baseMtx, transMtx, transMtx);
        PSMTXConcat((void*)((s32)cam + 0x11C), transMtx, transMtx);
        GXLoadPosMtxImm(transMtx, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

