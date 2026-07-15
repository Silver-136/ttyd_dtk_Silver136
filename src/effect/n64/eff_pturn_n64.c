#include "effect/n64/eff_pturn_n64.h"

void effPturnDisp(void* camera, void* effect) {
    extern void pturn_main_dispp(void* camera, void* effect);
    extern void pturn_num_dispp(void* camera, void* effect);

    void* work = *(void**)((s32)effect + 0xC);

    if (*(s32*)((s32)work + 4) != 0) {
        pturn_main_dispp(camera, effect);
        if (*(s32*)work != 1) {
            pturn_num_dispp(camera, effect);
        }
    }
}


void pturn_num_dispp(void* camera, void* effect) {
    extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXScale(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXSetNumChans(s32);extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetTevColor(s32,void*);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void GXLoadTexMtxImm(void*,s32,s32);extern void GXSetCullMode(s32);extern void effSetVtxDescN64(void*);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern f32 float_deg2rad_80425de0,float_1_80425de4,float_0_80425de8,float_0p03125_80425dec,float_0p0026042_80425df0;
    f32 base[3][4],mtx[3][4],scale[3][4],rot[3][4];u8 texObj[0x20];u8* work=*(u8**)((s32)effect+0xC);s32 i;u32 color;
    PSMTXTrans(base,*(f32*)(work+8),*(f32*)(work+0xC),*(f32*)(work+0x10));PSMTXConcat((void*)((s32)camera+0x11C),base,base);
    for(i=1;i<*(s32*)((s32)effect+8);i++,work+=0x48){PSMTXTrans(mtx,*(f32*)(work+0x50),*(f32*)(work+0x54),*(f32*)(work+0x58));PSMTXScale(scale,*(f32*)(work+0x78),*(f32*)(work+0x78),float_1_80425de4);PSMTXRotRad(rot,0x7A,float_deg2rad_80425de0**(f32*)(work+0x88));PSMTXConcat(mtx,scale,mtx);PSMTXConcat(mtx,rot,mtx);PSMTXTrans(scale,float_0_80425de8,*(f32*)(work+0x68),float_0_80425de8);PSMTXConcat(mtx,scale,mtx);PSMTXConcat(base,mtx,mtx);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);if(*(s32*)(work+0x38)>0){GXSetNumChans(1);GXSetChanCtrl(4,0,0,1,0,0,2);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,4);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,8,2,0);GXSetTevAlphaIn(0,0,1,7,7);color=*(u32*)(work+0x6C);GXSetTevColor(1,&color);effGetTexObjN64(0x53,texObj);GXLoadTexObj(texObj,0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(scale,float_0p03125_80425dec,float_0p0026042_80425df0,float_0_80425de8);GXLoadTexMtxImm(scale,0x1E,1);GXSetCullMode(0);effSetVtxDescN64((void*)0x803A77A8);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}}
}

void pturn_main_dispp(void* camera, void* effect) {
    extern void* camGetPtr(s32); extern void PSMTXTrans(void*, f32, f32, f32); extern void PSMTXRotRad(void*, s32, f32); extern void PSMTXScale(void*, f32, f32, f32); extern void PSMTXConcat(void*, void*, void*);
    extern void effGetTexObjN64(s32, void*); extern void GXLoadTexObj(void*, s32); extern void GXLoadPosMtxImm(void*, s32); extern void GXSetCurrentMtx(s32); extern void GXSetTevColor(s32, void*); extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32); extern f32 float_deg2rad_80425de0,float_1_80425de4,float_0_80425de8,float_0p03125_80425dec;
    u8 texObj[0x20]; f32 base[3][4],rot[3][4],mtx[3][4],scale[3][4]; u8* work=*(u8**)((s32)effect+0xC); s32 i; u32 color;
    PSMTXTrans(base,*(f32*)(work+8),*(f32*)(work+0xC),*(f32*)(work+0x10)); PSMTXRotRad(rot,0x79,float_deg2rad_80425de0*-*(f32*)((s32)camGetPtr(4)+0x114)); PSMTXConcat(base,rot,base); PSMTXConcat((void*)((s32)camera+0x11C),base,base);
    for(i=1;i<*(s32*)((s32)effect+8);i++,work+=0x48){if(*(s32*)(work+0x38)<=0&&*(s32*)work!=10)continue;effGetTexObjN64(0x92,texObj);GXLoadTexObj(texObj,0);PSMTXTrans(mtx,*(f32*)(work+0x50),*(f32*)(work+0x54),*(f32*)(work+0x58));PSMTXScale(scale,*(f32*)(work+0x78),*(f32*)(work+0x78),float_1_80425de4);PSMTXRotRad(rot,0x7A,float_deg2rad_80425de0**(f32*)(work+0x88));PSMTXConcat(mtx,scale,mtx);PSMTXConcat(mtx,rot,mtx);PSMTXTrans(scale,float_0_80425de8,*(f32*)(work+0x68),float_0_80425de8);PSMTXConcat(mtx,scale,mtx);PSMTXConcat(base,mtx,mtx);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);color=*(u32*)(work+0x6C);GXSetTevColor(1,&color);GXSetNumChans(1);GXSetChanCtrl(4,0,0,1,0,0,2);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,4);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,8,2,0);GXSetTevAlphaIn(0,0,1,7,7);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(scale,float_0p03125_80425dec,float_0p03125_80425dec,float_0_80425de8);GXLoadTexMtxImm(scale,0x1E,1);GXSetCullMode(0);effSetVtxDescN64((void*)0x803A7770);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
}

void effPturnMain(void* effect) {
    extern void effDelete(void*); extern void* effPturnN64Entry(f32,f32,f32,s32,s32); extern f64 cos(f64); extern f32 dispCalcZ(void*); extern void dispEntry(s32,s32,void*,void*,f32); extern void effPturnDisp(void); extern f32 float_6p2832_80425df4,float_0p05_80425df8,float_360_80425dfc,float_1_80425de4,float_0_80425de8,float_4_80425e00,float_neg2_80425e04,float_neg0p5_80425e08,float_10_80425e0c;
    u8* work=*(u8**)((s32)effect+0xC);s32 type=*(s32*)work,timer,frame,i;
    if(*(s32*)effect&4){*(s32*)effect&=~4;*(s32*)(work+0x28)=0x15;} if(*(s32*)(work+0x28)<100)*(s32*)(work+0x28)-=1; timer=*(s32*)(work+0x28);if(timer<0){effDelete(effect);return;}*(s32*)(work+0x2C)+=1;frame=*(s32*)(work+0x2C);
    for(i=1;i<*(s32*)((s32)effect+8);i++,work+=0x48){*(f32*)(work+0x50)+=*(f32*)(work+0x5C);*(f32*)(work+0x54)+=*(f32*)(work+0x60);*(f32*)(work+0x58)+=*(f32*)(work+0x64);if(timer==0x14){*(f32*)(work+0x60)=float_4_80425e00;*(f32*)(work+0x5C)=float_neg2_80425e04;}if(timer<0x14){*(f32*)(work+0x68)*=float_0p05_80425df8;*(f32*)(work+0x60)+=float_neg0p5_80425e08;*(f32*)(work+0x88)+=float_10_80425e0c;}if(timer<10)*(s32*)(work+0x6C)=timer*0x19;}
    dispEntry(4,2,effPturnDisp,effect,dispCalcZ(work+8));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effPturnN64Entry(s32 type, s32 number, f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effPturnMain(void*);
    extern char str_PturnN64_802fbd90[];
    extern f32 float_1_80425de4;
    extern f32 float_0_80425de8;
    void* entry;
    u8* work;
    f32 one;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_PturnN64_802fbd90;
    *(s32*)((s32)entry + 8) = 2;
    work = __memAlloc(3, 0x90);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effPturnMain;
    one = float_1_80425de4;
    *(s32*)entry |= 2;
    *(s32*)work = type;
    *(f32*)(work + 0x34) = one;
    if (type == 1) {
        *(s32*)(work + 0x38) = 1;
        *(s32*)(work + 0x3C) = 0;
    } else {
        *(s32*)(work + 0x38) = 0;
        *(s32*)(work + 0x3C) = number;
    }
    *(f32*)(work + 8) = x;
    *(f32*)(work + 0xC) = y;
    *(f32*)(work + 0x10) = z;
    if (type == 1) {
        *(s32*)(work + 0x28) = 0x64;
    } else if (type >= 1) {
        if (type == 10) {
            *(s32*)(work + 0x28) = 0x15;
        }
    } else if (type >= 0) {
        *(s32*)(work + 0x28) = 0x64;
    }
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x44) = 0;
    *(s32*)(work + 4) = 1;
    zero = float_0_80425de8;
    *(f32*)(work + 0x58) = zero;
    *(f32*)(work + 0x54) = zero;
    *(f32*)(work + 0x50) = zero;
    *(f32*)(work + 0x68) = zero;
    *(f32*)(work + 0x64) = zero;
    *(f32*)(work + 0x60) = zero;
    *(f32*)(work + 0x5C) = zero;
    *(f32*)(work + 0x88) = zero;
    *(s32*)(work + 0x70) = 0;
    *(s32*)(work + 0x6C) = 0xFF;
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

