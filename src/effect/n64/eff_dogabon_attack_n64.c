#include "effect/n64/eff_dogabon_attack_n64.h"


void effDogabonAttackMain(void* effect) {
    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32,s32,void*,void*,f32);
    extern void effDogabonAttackDisp(s32,void*);
    u8* entry=effect; u8* work=*(u8**)(entry+0xC); u8* p=work;
    f32 pos[3]; s32 i,frame,state,tick;
    pos[0]=*(f32*)(work+4);pos[1]=*(f32*)(work+8);pos[2]=*(f32*)(work+0xC);
    if((*(u32*)entry&4)!=0){*(u32*)entry&=~4;*(s32*)(work+0x10)=64;}
    if(*(s32*)(work+0x10)<1000)(*(s32*)(work+0x10))--;
    frame=++*(s32*)(work+0x14);
    if(*(s32*)(work+0x10)<0){effDelete(effect);return;}
    for(i=0;i<25;i++,p+=4){
        state=*(s32*)(p+0x6D0); tick=*(s32*)(p+0x734);
        *(f32*)(p+0x414)=0.01f*(f32)(100+(frame%7));
        *(f32*)(p+0x3B0)=2.0f-*(f32*)(p+0x414);
        switch(state){
        case 0:
            *(f32*)(p+0x2C)=*(f32*)(p+0x158);
            *(f32*)(p+0x90)=*(f32*)(p+0x1BC);
            *(f32*)(p+0xF4)=*(f32*)(p+0x220);
            if(*(s32*)(p+0x66C)==0){*(s32*)(p+0x6D0)=1;*(s32*)(p+0x734)=0;}else (*(s32*)(p+0x66C))--;
            break;
        case 1:
            if(++*(s32*)(p+0x734)>7){*(s32*)(p+0x734)=0;*(s32*)(p+0x6D0)=(*(s32*)work==0)?2:20;}
            break;
        case 2:
        case 20:{
            f32 t=(state==2?tick/14.0f:tick/19.0f);
            *(f32*)(p+0x2C)+=t*(*(f32*)(p+0x284)-*(f32*)(p+0x2C));
            *(f32*)(p+0x90)+=t*(*(f32*)(p+0x2E8)-*(f32*)(p+0x90));
            *(f32*)(p+0xF4)+=t*(*(f32*)(p+0x34C)-*(f32*)(p+0xF4));
            if(++*(s32*)(p+0x734)>(state==2?14:19)){*(s32*)(p+0x734)=0;*(s32*)(p+0x6D0)=state==2?3:5;}
            break;}
        case 5:
            *(f32*)(p+0x2C)+=*(f32*)(p+0x540);*(f32*)(p+0x90)+=*(f32*)(p+0x5A4);*(f32*)(p+0xF4)+=*(f32*)(p+0x608);
            *(f32*)(p+0x540)*=0.99f;*(f32*)(p+0x5A4)=*(f32*)(p+0x5A4)*0.99f-0.1f;*(f32*)(p+0x608)*=0.99f;
            if(++*(s32*)(p+0x734)>16){*(s32*)(p+0x734)=0;*(s32*)(p+0x6D0)=6;}
            break;
        }
    }
    dispEntry(4,1,effDogabonAttackDisp,effect,dispCalcZ(pos));
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effDogabonAttackDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXRotRad(void* mtx, f32 angle, s32 axis);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetNumChans(s32 count);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 tex, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 out);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 dst, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXLoadTexMtxImm(void* mtx, s32 id, s32 type);
    extern void effGetTexObjN64(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void effSetVtxDescN64(void* vtx);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void tri2(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
    extern u32 unk_80429680;
    extern u32 unk_80429684;
    extern f32 float_0p03125_80425068;
    extern f32 float_0_8042506c;
    extern f32 float_deg2rad_80425070;
    extern f32 float_1_80425074;
    extern char size12x12_tex32x32_vtx[];
    u8 texObj[0x20];
    f32 baseMtx[3][4];
    f32 transMtx[3][4];
    f32 rotMtx[3][4];
    f32 scaleMtx[3][4];
    u8* work;
    u8* part;
    void* cam;
    u32 color;
    s32 i;

    cam = camGetPtr(cameraId);
    work = *(u8**)((s32)effect + 0xC);
    PSMTXTrans(transMtx, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXScale(scaleMtx, *(f32*)(work + 0x28), *(f32*)(work + 0x28), *(f32*)(work + 0x28));
    PSMTXConcat(transMtx, scaleMtx, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);

    color = unk_80429680;
    ((u8*)&color)[0] = *(u8*)(work + 0x18);
    ((u8*)&color)[1] = *(u8*)(work + 0x1C);
    ((u8*)&color)[2] = *(u8*)(work + 0x20);
    ((u8*)&color)[3] = *(u8*)(work + 0x24);
    GXSetTevColor(1, &color);
    color = unk_80429684;
    GXSetTevColor(2, &color);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 8, 2, 4, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 6, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(scaleMtx, float_0p03125_80425068, float_0p03125_80425068, float_0_8042506c);
    GXLoadTexMtxImm(scaleMtx, 0x1E, 1);
    effGetTexObjN64(0x8C, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    effSetVtxDescN64(size12x12_tex32x32_vtx);

    part = work;
    for (i = 0; i < 0x19; i++, part += 4) {
        if ((*(s32*)(work + 0x6D0) != 0) && (*(s32*)(work + 0x6D0) != 7)) {
            PSMTXRotRad(rotMtx, float_deg2rad_80425070 * *(f32*)(part + 0x4DC), 'y');
            PSMTXTrans(transMtx, *(f32*)(part + 0x2C), *(f32*)(part + 0x90), *(f32*)(part + 0xF4));
            PSMTXConcat(rotMtx, transMtx, transMtx);
            PSMTXRotRad(rotMtx, float_deg2rad_80425070 * -*(f32*)(part + 0x4DC), 'y');
            PSMTXConcat(transMtx, transMtx, transMtx);
            PSMTXRotRad(rotMtx, float_deg2rad_80425070 * *(f32*)(part + 0x478), 'z');
            PSMTXConcat(transMtx, rotMtx, transMtx);
            PSMTXScale(scaleMtx, *(f32*)(part + 0x3B0), *(f32*)(part + 0x414), float_1_80425074);
            PSMTXConcat(transMtx, scaleMtx, transMtx);
            PSMTXConcat(baseMtx, transMtx, transMtx);
            GXLoadPosMtxImm(transMtx, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3);
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

u8 effDogabonAttackN64Entry(void) {
    return 0;
}
