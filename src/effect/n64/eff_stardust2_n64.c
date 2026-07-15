#include "effect/n64/eff_stardust2_n64.h"

void effStardust2N64SetDrawCam(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x2C) = camId;
}


void* effStardust2N64Entry(f32 x, f32 y, f32 z, f32 size, f32 unused, s32 type, s32 count, s32 time) {
    extern void* effEntry(void);extern void* __memAlloc(s32,s32);extern void effStardust2Main(void*);extern char str_Stardust2N64_802fc088[];extern s32 rand(void);extern f64 sin(f64),cos(f64);extern f32 float_1_8042621c,float_360_80426240,float_6p2832_80426248,float_0p1_80426224,float_0p4_80426254;
    void* entry;u8* work;u8* p;s32 i;f32 angle;entry=effEntry();*(char**)((s32)entry+0x14)=str_Stardust2N64_802fc088;*(s32*)((s32)entry+8)=count+1;work=__memAlloc(3,(count+1)*0x30);*(u8**)((s32)entry+0xC)=work;*(void**)((s32)entry+0x10)=effStardust2Main;*(s32*)work=type==6?1:0;*(s16*)(work+2)=type==6?3:type;*(f32*)(work+4)=x;*(f32*)(work+8)=y;*(f32*)(work+0xC)=z;*(s32*)(work+0x10)=0;*(s32*)(work+0x14)=time;*(s32*)(work+0x2C)=4;p=work+0x30;
    for(i=1;i<=count;i++,p+=0x30){angle=float_6p2832_80426248*((float_360_80426240/(f32)count*(f32)i+(type<2?0:(f32)(rand()%360)))/float_360_80426240);*(f32*)(p+4)=x+(f32)cos(angle)*size;*(f32*)(p+8)=y;*(f32*)(p+0xC)=z+(f32)sin(angle)*size;*(s32*)(p+0x10)=i+0x1E;*(s32*)(p+0x14)=0;*(f32*)(p+0x1C)=float_1_8042621c;*(f32*)(p+0x20)=float_0p1_80426224*size;*(f32*)(p+0x24)=float_0p4_80426254*size;}
    return entry;
}

u8 effStardust2Main(void* effect) {
    extern void effDelete(void*);extern f32 dispCalcZ(void*);extern void dispEntry(s32,s32,void*,void*,f32);extern void effStardust2Disp(void);extern f32 float_0p96_80426250,float_neg0p02_80426234;
    u8* work=*(u8**)((s32)effect+0xC);u8* p=work+0x30;s32 type=*(s16*)(work+2),i,timer=*(s32*)(work+0x14)-1;*(s32*)(work+0x14)=timer;*(s32*)(work+0x10)+=1;if((type<2||type==3||type>9)&&timer<0){effDelete(effect);return 0;}
    for(i=0;i<*(s32*)((s32)effect+8)-1;i++,p+=0x30){*(s32*)(p+0x28)=(*(s32*)(work+0x10)+i)&7;*(f32*)(p+0x14)*=float_0p96_80426250;*(f32*)(p+0x18)*=float_0p96_80426250;*(f32*)(p+0x18)+=float_neg0p02_80426234;*(f32*)(p+4)+=*(f32*)(p+0x14);*(f32*)(p+8)+=*(f32*)(p+0x18);*(f32*)(p+0xC)+=*(f32*)(p+0x1C);}dispEntry(*(s32*)(work+0x2C),2,effStardust2Disp,effect,dispCalcZ(work+4));return 0;
}

void effStardust2Disp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXConcat(void*,void*,void*);extern void PSMTXScale(void*,f32,f32,f32);extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void GXLoadTexMtxImm(void*,s32,s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetCullMode(s32);extern void GXSetTevColor(s32,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void effSetVtxDescN64(void*);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern f32 float_deg2rad_8042620c,float_0p0056818_80426210,float_0p045455_80426214,float_0_80426218,float_1_8042621c;
    f32 base[3][4],rot[3][4],scale[3][4],mtx[3][4];u8 texObj[0x20];u8* work=*(u8**)((s32)effect+0xC);u8* p=work+0x30;s32 i;u32 color;
    PSMTXTrans(base,*(f32*)(work+4),*(f32*)(work+8),*(f32*)(work+0xC));PSMTXRotRad(rot,0x79,float_deg2rad_8042620c*-*(f32*)((s32)camGetPtr(4)+0x114));PSMTXConcat(base,rot,base);PSMTXConcat((void*)((s32)camGetPtr(cameraId)+0x11C),base,base);GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(scale,float_0p0056818_80426210,float_0p045455_80426214,float_0_80426218);GXLoadTexMtxImm(scale,0x1E,1);effGetTexObjN64(0x20,texObj);GXLoadTexObj(texObj,0);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,2,1,8,0);GXSetTevAlphaIn(0,0,1,7,7);GXSetCullMode(0);
    for(i=1;i<*(s32*)((s32)effect+8);i++,p+=0x30){PSMTXTrans(mtx,*(f32*)(p+4),*(f32*)(p+8),*(f32*)(p+0xC));PSMTXScale(scale,*(f32*)(p+0x1C),*(f32*)(p+0x1C),float_1_8042621c);PSMTXConcat(mtx,scale,mtx);PSMTXConcat(base,mtx,mtx);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);color=*(u32*)(p+0x28);GXSetTevColor(1,&color);effSetVtxDescN64((void*)0x803A8EF0);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
}

