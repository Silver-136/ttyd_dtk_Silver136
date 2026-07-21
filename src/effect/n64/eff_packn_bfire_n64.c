#include "effect/n64/eff_packn_bfire_n64.h"
void* effPacknBfireN64Entry(f32 x, f32 y, f32 z, f32 dstX, f32 dstY, f32 dstZ, f32 scale, s32 type, s32 duration, s32 time) {
    extern void* effEntry(void); extern void* __memAlloc(s32,s32); extern void effPacknBfireMain(void*); extern char str_PacknBfireN64_802fbc4c[];
    extern f32 float_0_80425c6c,float_1_80425c80; void* entry; u8* work; s32 count,i;
    entry=effEntry(); count=type==2?8:1; *(char**)((s32)entry+0x14)=str_PacknBfireN64_802fbc4c; *(s32*)((s32)entry+8)=count; work=__memAlloc(3,count*0x7C); *(u8**)((s32)entry+0xC)=work; *(void**)((s32)entry+0x10)=effPacknBfireMain; *(s32*)entry|=2;
    for(i=0;i<count;i++,work+=0x7C){ *(s32*)work=type; *(f32*)(work+4)=x;*(f32*)(work+8)=y;*(f32*)(work+0xC)=z;*(f32*)(work+0x10)=dstX;*(f32*)(work+0x14)=dstY;*(f32*)(work+0x18)=dstZ;*(s32*)(work+0x1C)=duration;*(s32*)(work+0x20)=time<1?1000:time;*(s32*)(work+0x24)=0;*(f32*)(work+0x60)=(dstX-x)/(f32)duration;*(f32*)(work+0x64)=(dstY-y)/(f32)duration;*(f32*)(work+0x68)=(dstZ-z)/(f32)duration;*(f32*)(work+0x48)=scale;*(f32*)(work+0x6C)=float_0_80425c6c; }
    return entry;
}

void effPacknBfireMain(void* effect) {
    extern void effDelete(void*); extern void* effPacknBfireN64Entry(f32,f32,f32,f32,f32,f32,f32,s32,s32,s32);
    extern f32 dispCalcZ(void*); extern void dispEntry(s32,s32,void*,void*,f32); extern void effPacknBfireDisp(void);
    extern f32 float_1_80425c80,float_0p97_80425c8c,float_0p02_80425c90,float_0p05_80425c94,float_0p1_80425c98,float_0p9_80425c9c,float_0p2_80425ca0,float_neg0p6_80425ca4,float_0_80425c6c;
    u8* work=*(u8**)((s32)effect+0xC); s32 type=*(s32*)work,i;
    if(*(s32*)effect&4){*(s32*)effect&=~4;*(s32*)(work+0x20)=0x10;} if(*(s32*)(work+0x20)<1000)*(s32*)(work+0x20)-=1;*(s32*)(work+0x24)+=1;
    if(*(s32*)(work+0x20)<0){effDelete(effect);return;}
    if(type==1){*(f32*)(work+0x60)*=float_0p97_80425c8c;*(f32*)(work+0x68)*=float_0p97_80425c8c;*(f32*)(work+0x64)+=float_0p02_80425c90*(float_1_80425c80-*(f32*)(work+0x64));}
    else if(type<1 && (*(s32*)(work+0x24)&3)==0) effPacknBfireN64Entry(*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC),*(f32*)(work+0x10),*(f32*)(work+0x14),*(f32*)(work+0x18),float_1_80425c80,1,*(s32*)(work+0x1C),*(s32*)(work+0x1C));
    else if(type<3) for(i=0;i<*(s32*)((s32)effect+8);i++,work+=0x7C){*(f32*)(work+0x60)*=float_0p9_80425c9c;*(f32*)(work+0x64)-=float_0p2_80425ca0;*(f32*)(work+4)+=*(f32*)(work+0x60);*(f32*)(work+8)+=*(f32*)(work+0x64);if(*(f32*)(work+8)<float_0_80425c6c){*(f32*)(work+8)=float_0_80425c6c;*(f32*)(work+0x64)*=float_neg0p6_80425ca4;}}
    dispEntry(4,2,effPacknBfireDisp,effect,dispCalcZ((u8*)*(void**)((s32)effect+0xC)+4));
}


void effPacknBfireDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_deg2rad_80425c68;
    u8* part = *(u8**)((s32)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    f32 trans[3][4], rot[3][4], scale[3][4], model[3][4], texMtx[3][4];
    u8 texObj[0x20];
    u32 color;
    s32 i;

    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0, 2, 8, 0);
    GXSetTevAlphaIn(0, 0, 1, 7, 7);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetCullMode(0);
    effGetTexObjN64(0x71, texObj); GXLoadTexObj(texObj, 0);
    effGetTexObjN64(0x72, texObj); GXLoadTexObj(texObj, 1);
    effSetVtxDescN64((void*)0x803A6AC8);

    for (i = 0; i < *(s32*)((s32)effect + 8); i++, part += 0x7C) {
        *(f32*)(part + 0x4C) += *(f32*)(part + 0x54);
        *(f32*)(part + 0x50) += *(f32*)(part + 0x58);
        color = *(u32*)(part + 0x28);
        GXSetTevColor(1, &color);
        PSMTXTrans(trans, *(f32*)(part + 4), *(f32*)(part + 8), *(f32*)(part + 0xC));
        PSMTXRotRad(rot, 0x79, float_deg2rad_80425c68 * -*(f32*)((u8*)camGetPtr(4) + 0x114));
        PSMTXConcat(trans, rot, model);
        PSMTXScale(scale, *(f32*)(part + 0x48), *(f32*)(part + 0x48), *(f32*)(part + 0x48));
        PSMTXConcat(model, scale, model);
        PSMTXRotRad(rot, 0x7A, float_deg2rad_80425c68 * *(f32*)(part + 0x74));
        PSMTXScale(scale, *(f32*)(part + 0x70), *(f32*)(part + 0x6C), 1.0f);
        PSMTXConcat(rot, scale, scale);
        PSMTXConcat(model, scale, model);
        PSMTXConcat((u8*)camera + 0x11C, model, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        PSMTXScale(texMtx, 0.00390625f, 0.015625f, 0.0f);
        PSMTXTrans(trans, *(f32*)(part + 0x4C), *(f32*)(part + 0x50), 0.0f);
        PSMTXConcat(texMtx, trans, texMtx);
        GXLoadTexMtxImm(texMtx, 0x1E, 1);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}
