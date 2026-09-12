#include "effect/n64/eff_pturn_n64.h"

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


void effPturnMain(void* effect) {
    extern void effDelete(void*);
    extern void* effPturnN64Entry(f32, f32, f32, s32, s32);
    extern f64 cos(f64);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effPturnDisp(void);
    extern f32 scale_dat[];
    extern f32 float_6p2832_80425df4, float_0p05_80425df8;
    extern f32 float_360_80425dfc, float_1_80425de4, float_0_80425de8;
    extern f32 float_4_80425e00, float_neg2_80425e04;
    extern f32 float_neg0p5_80425e08, float_10_80425e0c;
    extern f64 double_to_int_802fbd88;
    u8* work = *(u8**)((s32)effect + 0xC);
    f32 position[3];
    s32 type;
    s32 timer;
    s32 frame;
    s32 current;
    s32 target;
    f32 baseScale;
    f32 angle;
    f32 fade;
    u32 conversion[4];
    s32 i;

    position[0] = *(f32*)(work + 8);
    position[1] = *(f32*)(work + 0xC);
    position[2] = *(f32*)(work + 0x10);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x28) = 0x15;
    }
    if (*(s32*)(work + 0x28) < 100) {
        *(s32*)(work + 0x28) -= 1;
    }
    if (*(s32*)(work + 0x28) < 0) {
        effDelete(effect);
        return;
    }

    *(s32*)(work + 0x2C) += 1;
    if (*(s32*)(work + 0x2C) > 0x4F1A0) {
        *(s32*)(work + 0x2C) = 0x100;
    }
    type = *(s32*)work;
    timer = *(s32*)(work + 0x28);
    frame = *(s32*)(work + 0x2C);
    current = *(s32*)(work + 0x38);
    target = *(s32*)(work + 0x3C);
    baseScale = *(f32*)(work + 0x34);

    if (type == 0) {
        if (*(s32*)(work + 0x44) > 0) {
            *(s32*)(work + 0x44) -= 1;
        } else if (current != target) {
            if (current < 0) {
                current = 0;
            }
            if (current > target) {
                *(s32*)(work + 0x28) = type != 0 ? 0x3C : 0x64;
                *(s32*)(work + 0x2C) = 1;
                *(s32*)(work + 0x38) += 1;
                *(s32*)(work + 0x70) = 0;
            } else if (current < target) {
                effPturnN64Entry(*(f32*)(work + 8),
                                 *(f32*)(work + 0xC) + *(f32*)(work + 0x68),
                                 *(f32*)(work + 0x10), 10, 0);
                *(s32*)(work + 0x38) -= 1;
            }
            *(s32*)(work + 0x44) = 8;
        }
    }

    conversion[0] = 0x43300000;
    conversion[1] = (frame * 12) ^ 0x80000000;
    conversion[2] = 0x43300000;
    conversion[3] = timer ^ 0x80000000;
    angle = float_6p2832_80425df4 * (f32)(*(f64*)&conversion[0] - double_to_int_802fbd88);
    fade = float_0p05_80425df8 * (f32)(*(f64*)&conversion[2] - double_to_int_802fbd88);
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, work += 0x48) {
        s32 settled = 0;
        f32 scale;
        if (*(s32*)(work + 0x70) > 8 || type == 10) {
            settled = 1;
        }
        scale = float_1_80425de4;
        if (!settled) {
            scale = scale_dat[*(s32*)(work + 0x70)];
        }
        *(f32*)(work + 0x78) = scale * baseScale;
        *(f32*)(work + 0x68) = float_0_80425de8;
        if (type & 1) {
            *(f32*)(work + 0x68) = float_4_80425e00 * (f32)cos((f64)(angle / float_360_80425dfc));
        }

        *(f32*)(work + 0x50) += *(f32*)(work + 0x5C);
        *(f32*)(work + 0x54) += *(f32*)(work + 0x60);
        *(f32*)(work + 0x58) += *(f32*)(work + 0x64);
        if (timer == 0x14) {
            *(f32*)(work + 0x60) = float_4_80425e00;
            *(f32*)(work + 0x5C) = float_neg2_80425e04;
        }
        if (timer < 0x14) {
            *(f32*)(work + 0x68) *= fade;
            *(f32*)(work + 0x60) += float_neg0p5_80425e08;
            *(f32*)(work + 0x88) += float_10_80425e0c;
        }
        if (timer < 10) {
            *(s32*)(work + 0x6C) = timer * 0x19;
        }
        *(s32*)(work + 0x70) += 1;
    }
    dispEntry(4, 2, effPturnDisp, effect, dispCalcZ(position));
}

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

void pturn_main_dispp(void* camera, void* effect) {
    extern void* camGetPtr(s32); extern void PSMTXTrans(void*, f32, f32, f32); extern void PSMTXRotRad(void*, s32, f32); extern void PSMTXScale(void*, f32, f32, f32); extern void PSMTXConcat(void*, void*, void*);
    extern void effGetTexObjN64(s32, void*); extern void GXLoadTexObj(void*, s32); extern void GXLoadPosMtxImm(void*, s32); extern void GXSetCurrentMtx(s32); extern void GXSetTevColor(s32, void*); extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32); extern f32 float_deg2rad_80425de0,float_1_80425de4,float_0_80425de8,float_0p03125_80425dec;
    extern u8 size48x48_tex32x32_pturn_vtx[];
    u8 texObj[0x20]; f32 base[3][4],rot[3][4],mtx[3][4],scale[3][4]; u8* work=*(u8**)((s32)effect+0xC); s32 i; u32 color;
    PSMTXTrans(base,*(f32*)(work+8),*(f32*)(work+0xC),*(f32*)(work+0x10)); PSMTXRotRad(rot,0x79,float_deg2rad_80425de0*-*(f32*)((s32)camGetPtr(4)+0x114)); PSMTXConcat(base,rot,base); PSMTXConcat((void*)((s32)camera+0x11C),base,base);
    for(i=1;i<*(s32*)((s32)effect+8);i++,work+=0x48){if(*(s32*)(work+0x38)<=0&&*(s32*)work!=10)continue;effGetTexObjN64(0x92,texObj);GXLoadTexObj(texObj,0);PSMTXTrans(mtx,*(f32*)(work+0x50),*(f32*)(work+0x54),*(f32*)(work+0x58));PSMTXScale(scale,*(f32*)(work+0x78),*(f32*)(work+0x78),float_1_80425de4);PSMTXRotRad(rot,0x7A,float_deg2rad_80425de0**(f32*)(work+0x88));PSMTXConcat(mtx,scale,mtx);PSMTXConcat(mtx,rot,mtx);PSMTXTrans(scale,float_0_80425de8,*(f32*)(work+0x68),float_0_80425de8);PSMTXConcat(mtx,scale,mtx);PSMTXConcat(base,mtx,mtx);GXLoadPosMtxImm(mtx,0);GXSetCurrentMtx(0);color=*(u32*)(work+0x6C);GXSetTevColor(1,&color);GXSetNumChans(1);GXSetChanCtrl(4,0,0,1,0,0,2);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,4);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,8,2,0);GXSetTevAlphaIn(0,0,1,7,7);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXScale(scale,float_0p03125_80425dec,float_0p03125_80425dec,float_0_80425de8);GXLoadTexMtxImm(scale,0x1E,1);GXSetCullMode(0);effSetVtxDescN64(size48x48_tex32x32_pturn_vtx);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
}


void pturn_num_dispp(void* camera, void* effect) {
    extern void* camGetPtr(s32); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXScale(void*,f32,f32,f32); extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXConcat(void*,void*,void*); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetTevColor(s32,void*); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void effSetVtxDescN64(void*); extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    extern f32 float_deg2rad_80425de0,float_1_80425de4,float_0_80425de8,float_0p03125_80425dec,float_0p0026042_80425df0; extern f64 double_to_int_802fbd88; extern u32 unk_804297d4; extern u8 size24x24_tex32x32_pturn_vtx[],size16x24_tex32x32_l_pturn_vtx[],size16x24_tex32x32_r_pturn_vtx[];
    f32 base[3][4], mtx[3][4], scale[3][4], rot[3][4]; u8 texObj[0x20]; u8* work=*(u8**)((s32)effect+0xC); s32 i,count=*(s32*)(work+0x38); s32 q=count/10+(count>>31); s32 first=0x160-(count<<5); s32 rem=0x160-((count-q*10)<<5); s32 tens=0x160-(q<<5); f32 deg=float_deg2rad_80425de0; u32 color; union { f64 d; u32 w[2]; } cv;
    PSMTXTrans(base,*(f32*)(work+8),*(f32*)(work+0xC),*(f32*)(work+0x10)); PSMTXRotRad(rot,0x79,deg*-*(f32*)((s32)camGetPtr(4)+0x114)); PSMTXConcat(base,rot,mtx); PSMTXConcat((u8*)camera+0x11C,mtx,mtx);
    for(i=1,work+=0x48;i<*(s32*)((s32)effect+8);i++,work+=0x48){
        PSMTXTrans(base,*(f32*)(work+8),*(f32*)(work+0xC),*(f32*)(work+0x10)); PSMTXScale(scale,*(f32*)(work+0x30),*(f32*)(work+0x30),float_1_80425de4); PSMTXRotRad(rot,0x7A,deg**(f32*)(work+0x40)); PSMTXConcat(base,scale,base); PSMTXConcat(base,rot,base); PSMTXTrans(scale,float_0_80425de8,*(f32*)(work+0x20),float_0_80425de8); PSMTXConcat(base,scale,base); PSMTXConcat(mtx,base,base); GXLoadPosMtxImm(base,0); GXSetCurrentMtx(0);
        if(count>0){ GXSetNumChans(1);GXSetChanCtrl(4,0,0,1,0,0,2);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,4);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,8,10,15);GXSetTevAlphaIn(0,7,1,4,7); color=(unk_804297d4&0xFFFFFF00)|*(u8*)(work+0x24); GXSetTevColor(1,&color); effGetTexObjN64(0x53,texObj);GXLoadTexObj(texObj,0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
            if(count<10){
                PSMTXScale(scale,float_0p03125_80425dec,float_0p0026042_80425df0,float_0_80425de8);cv.w[0]=0x43300000;cv.w[1]=first^0x80000000;PSMTXTrans(base,float_0_80425de8,(f32)(cv.d-double_to_int_802fbd88),float_0_80425de8);PSMTXConcat(scale,base,scale);GXLoadTexMtxImm(scale,0x1E,1);effSetVtxDescN64(size24x24_tex32x32_pturn_vtx);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);
            }else{
                PSMTXScale(scale,float_0p03125_80425dec,float_0p0026042_80425df0,float_0_80425de8);cv.w[0]=0x43300000;cv.w[1]=rem^0x80000000;PSMTXTrans(base,float_0_80425de8,(f32)(cv.d-double_to_int_802fbd88),float_0_80425de8);PSMTXConcat(scale,base,scale);GXLoadTexMtxImm(scale,0x1E,1);effSetVtxDescN64(size16x24_tex32x32_r_pturn_vtx);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);
                PSMTXScale(scale,float_0p03125_80425dec,float_0p0026042_80425df0,float_0_80425de8);cv.w[0]=0x43300000;cv.w[1]=tens^0x80000000;PSMTXTrans(base,float_0_80425de8,(f32)(cv.d-double_to_int_802fbd88),float_0_80425de8);PSMTXConcat(scale,base,scale);GXLoadTexMtxImm(scale,0x1E,1);effSetVtxDescN64(size16x24_tex32x32_l_pturn_vtx);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
        }
    }
}

u8 size48x48_tex32x32_pturn_vtx[56] = {
    0xFF, 0xE8, 0xFF, 0xE8, 0x00, 0x00, 0xFE, 0x00,
    0x02, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x18,
    0xFF, 0xE8, 0x00, 0x00, 0x02, 0x00, 0xFE, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x18, 0x00, 0x18,
    0x00, 0x00, 0x06, 0x00, 0x02, 0x00, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xE8, 0x00, 0x18, 0x00, 0x00,
    0x02, 0x00, 0x06, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
};

u8 size24x24_tex32x32_pturn_vtx[56] = {
    0xFF, 0xF4, 0xFF, 0xF4, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFA, 0xFA, 0x00, 0xFF, 0x00, 0x0C,
    0xFF, 0xF4, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0xFA, 0xFA, 0x00, 0xFF, 0x00, 0x0C, 0x00, 0x0C,
    0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFA, 0xFA,
    0x00, 0xFF, 0xFF, 0xF4, 0x00, 0x0C, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00, 0xFA, 0xFA, 0x00, 0xFF,
};

const f32 vec3_802fbd78[3] = { 0.0f, 0.0f, 0.0f };
const char str_PturnN64_802fbd90[] = "PturnN64";

u8 size16x24_tex32x32_l_pturn_vtx[56] = {
    0xFF, 0xF0, 0xFF, 0xF4, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFA, 0xFA, 0x00, 0xFF, 0x00, 0x04,
    0xFF, 0xF4, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0xFA, 0xFA, 0x00, 0xFF, 0x00, 0x04, 0x00, 0x0C,
    0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFA, 0xFA,
    0x00, 0xFF, 0xFF, 0xF0, 0x00, 0x0C, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00, 0xFA, 0xFA, 0x00, 0xFF,
};

u8 size16x24_tex32x32_r_pturn_vtx[56] = {
    0xFF, 0xFC, 0xFF, 0xF4, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xFA, 0xFA, 0x00, 0xFF, 0x00, 0x10,
    0xFF, 0xF4, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0xFA, 0xFA, 0x00, 0xFF, 0x00, 0x10, 0x00, 0x0C,
    0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFA, 0xFA,
    0x00, 0xFF, 0xFF, 0xFC, 0x00, 0x0C, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00, 0xFA, 0xFA, 0x00, 0xFF,
};
