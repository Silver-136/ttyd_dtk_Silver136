#include "effect/n64/eff_magic2_n64.h"

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMagic2N64Entry(s32 type, s32 arg, f32 x, f32 y, f32 z, f32 scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effMagic2Main(void*);
    extern s32 rand(void);
    extern char str_Magic2N64_802fb460[];
    extern f32 float_10_80425a68;
    extern f32 float_4_80425a6c;
    extern f32 float_0_80425a34;
    extern f32 float_0p2_80425a70;
    void* entry;
    u8* work;
    s32 r;
    s32 angle;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Magic2N64_802fb460;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x60);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMagic2Main;

    *(s32*)work = type;
    *(s32*)(work + 0x28) = arg;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 0x10) = x;
    *(f32*)(work + 0x14) = y;
    *(f32*)(work + 0x18) = z;
    *(s32*)(work + 0x44) = 0;
    *(s32*)(work + 0x40) = 0;
    *(s32*)(work + 0x4C) = 0;
    *(s32*)(work + 0x48) = 0;
    *(s32*)(work + 0x34) = 0;
    *(s32*)(work + 0x30) = 0;
    *(s32*)(work + 0x3C) = 0;
    *(s32*)(work + 0x38) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x1C) = float_10_80425a68;

    r = rand();
    angle = r - ((r / 0x168) * 0x168);
    *(f32*)(work + 0x58) = (f32)angle;
    *(f32*)(work + 0x5C) = float_4_80425a6c;
    *(f32*)(work + 0x50) = float_0_80425a34;
    *(f32*)(work + 0x54) = float_0_80425a34;

    if (type == 1) {
        *(s32*)(work + 0x3C) = 0x60;
        *(f32*)(work + 0x20) = scale;
    } else {
        *(s32*)(work + 0x44) = -2;
        *(s32*)(work + 0x4C) = -4;
        *(f32*)(work + 0x20) = float_0p2_80425a70 * scale;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


void effMagic2Main(void* effect) {
    typedef struct Vec3 { f32 x; f32 y; f32 z; } Vec3;
    extern void effDelete(void*); extern f64 sin(f64); extern void* camGetPtr(s32);
    extern void PSMTXMultVec(void*,Vec3*,Vec3*); extern void PSMTX44MultVec(void*,Vec3*,Vec3*);
    extern f32 dispCalcZ(void*); extern void dispEntry(s32,s32,void*,void*,f32); extern void effMagic2Disp(s32,s32);
    extern f32 vec3_802fb440[3], vec3_802fb44c[3];
    extern f32 float_0p7_80425a48,float_6p2832_80425a4c,float_360_80425a50,float_0p4_80425a54,float_0p1_80425a58,float_0p3_80425a5c;
    extern f32 float_304_80425a60,float_240_80425a64,float_0_80425a34;
    u8* work=*(u8**)((s32)effect+0xC); Vec3 initialPos,displayPos,initialProjected,projected; s32 timer,i; void* camera; f32 wave;
    initialPos.x=vec3_802fb440[0]; initialPos.y=vec3_802fb440[1]; initialPos.z=vec3_802fb440[2];
    initialPos.x=*(f32*)(work+0x10); initialPos.y=*(f32*)(work+0x14); initialPos.z=*(f32*)(work+0x18);
    displayPos=initialPos;
    *(s32*)(work+0x28)-=1; *(s32*)(work+0x2C)+=1; timer=*(s32*)(work+0x28);
    if(timer<0){effDelete(effect);return;}
    if(*(s32*)(work+0x2C)<=10) *(s32*)(work+0x24)=(*(s32*)(work+0x2C)*0xFF)/10;
    if(timer<6) *(s32*)(work+0x24)=(timer*0xFF)/6;
    if(timer<10&&*(s32*)work==0) *(f32*)(work+0x1C)*=float_0p7_80425a48;
    if(*(s32*)work==1){
        wave=(f32)sin((f64)(float_6p2832_80425a4c*(f32)(timer*10)/float_360_80425a50));
        *(f32*)(work+0x1C)=float_0p4_80425a54*((*(f32*)(work+0x20)*float_0p1_80425a58*wave+*(f32*)(work+0x20))-*(f32*)(work+0x1C))+*(f32*)(work+0x1C);
    }else{
        wave=(f32)sin((f64)(float_6p2832_80425a4c*(f32)(timer*10)/float_360_80425a50));
        *(f32*)(work+0x1C)=float_0p3_80425a5c*((*(f32*)(work+0x20)*float_0p1_80425a58*wave+*(f32*)(work+0x20))-*(f32*)(work+0x1C))+*(f32*)(work+0x1C);
    }
    initialProjected.x=vec3_802fb44c[0]; initialProjected.y=vec3_802fb44c[1]; initialProjected.z=vec3_802fb44c[2];
    initialProjected.x=*(f32*)(work+4); initialProjected.y=*(f32*)(work+8); initialProjected.z=*(f32*)(work+0xC);
    projected=initialProjected;
    camera=camGetPtr(4); PSMTXMultVec((u8*)camera+0x11C,&projected,&projected);
    camera=camGetPtr(4); PSMTX44MultVec((u8*)camera+0x15C,&projected,&projected);
    *(f32*)(work+0x10)=float_304_80425a60*projected.x;
    *(f32*)(work+0x14)=float_240_80425a64*projected.y;
    *(f32*)(work+0x18)=float_0_80425a34;
    for(i=0;i<*(s32*)((s32)effect+8);i++){
        *(f32*)(work+0x58)+=*(f32*)(work+0x5C); *(f32*)(work+0x50)+=*(f32*)(work+0x54);
        *(s32*)(work+0x30)+=*(s32*)(work+0x34); *(s32*)(work+0x38)+=*(s32*)(work+0x3C);
        *(s32*)(work+0x40)+=*(s32*)(work+0x44); *(s32*)(work+0x48)+=*(s32*)(work+0x4C);
        if(*(s32*)(work+0x40)<0)*(s32*)(work+0x40)+=0x100; if(*(s32*)(work+0x48)<0)*(s32*)(work+0x48)+=0x100; work+=0x60;
    }
    dispEntry(4,2,effMagic2Disp,effect,dispCalcZ(&displayPos));
}

void effMagic2Disp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef f32 Mtx44[4][4];
    typedef struct GXColorLocal { u8 r, g, b, a; } GXColorLocal;
    extern void* camGetPtr(s32);
    extern void GXGetProjectionv(f32*);
    extern void GXGetViewportv(f32*);
    extern void PSMTX44Trans(f32,f32,f32,Mtx44);
    extern void PSMTX44Scale(f32,f32,f32,Mtx44);
    extern void PSMTX44Concat(Mtx44,Mtx44,Mtx44);
    extern void C_MTXPerspective(Mtx44,f32,f32,f32,f32);
    extern void GXSetProjection(Mtx44,s32);
    extern void GXSetProjectionv(f32*);
    extern void PSMTXTrans(Mtx,f32,f32,f32);
    extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXLoadTexMtxImm(Mtx,s32,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32,void*);
    extern void GXSetScissorBoxOffset(s32,s32);
    extern void magic2_4_dl(void);
    extern void magic2_2_line_dl(void);
    extern u8 color_rotation_data[];
    extern u32 unk_80429758;
    extern u32 unk_8042975c;
    u8* entry = effect;
    u8* work;
    f32 projection[7];
    f32 viewport[6];
    Mtx44 trans44, scale44, perspective;
    Mtx mtx, scale;
    GXColorLocal color1_temp, color1, color2_temp, color2;
    s32 type;
    s32 angle;
    s32 alpha;
    s32 scroll_x1, scroll_y1, scroll_x2, scroll_y2;
    s32 i;

    camGetPtr(cameraId);
    work = *(u8**)(entry + 0xC);
    type = *(s32*)work;
    GXGetProjectionv(projection);
    GXGetViewportv(viewport);
    PSMTX44Trans(*(f32*)(work + 0x10), *(f32*)(work + 0x14), *(f32*)(work + 0x18), trans44);
    PSMTX44Scale(*(f32*)(work + 0x1C), *(f32*)(work + 0x1C), 1.0f, scale44);
    PSMTX44Concat(trans44, scale44, trans44);
    C_MTXPerspective(perspective, type == 1 ? 130.0f : 30.0f, 1.2667f, 1.0f, 32768.0f);
    PSMTX44Concat(perspective, trans44, perspective);
    GXSetProjection(perspective, 0);
    if (type == 1) {
        PSMTXTrans(mtx, 0.0f, 0.0f, -70.0f);
    } else if (type < 1 && type > -1) {
        PSMTXTrans(mtx, 0.0f, 0.0f, -80.0f);
    }
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    alpha = *(s32*)(work + 0x24);
    angle = *(s32*)(work + 0x2C) * 3;
    for (i = 0; i < *(s32*)(entry + 8); i++, work += 0x60) {
        angle = (angle + 30) % 24;
        *(u32*)&color1_temp = unk_80429758;
        color1_temp.a = (u8)alpha;
        color1_temp.r = color_rotation_data[angle];
        color1_temp.b = color_rotation_data[angle + 2];
        color1 = color1_temp;
        GXSetTevColor(1, &color1);
        *(u32*)&color2_temp = unk_8042975c;
        color2_temp.a = (u8)(alpha / 2);
        color2 = color2_temp;
        GXSetTevColor(2, &color2);
        scroll_x1 = *(s32*)(work + 0x30);
        scroll_y1 = *(s32*)(work + 0x40);
        scroll_x2 = *(s32*)(work + 0x38);
        scroll_y2 = *(s32*)(work + 0x48);
        if (type == 0) {
            PSMTXScale(scale, 0.015625f, 0.015625f, 1.0f);
            PSMTXTrans(mtx, (f32)(scroll_x1 / 4), (f32)(scroll_y1 / 4), 0.0f);
            PSMTXConcat(scale, mtx, mtx); GXLoadTexMtxImm(mtx, 0x1E, 1);
            PSMTXScale(scale, 0.015625f, 0.015625f, 1.0f);
            PSMTXTrans(mtx, (f32)(scroll_x2 / 4), (f32)(scroll_y2 / 4), 0.0f);
            PSMTXConcat(scale, mtx, mtx); GXLoadTexMtxImm(mtx, 0x21, 1);
        } else {
            PSMTXScale(scale, 0.015625f, 0.0625f, 0.0f);
            PSMTXTrans(mtx, (f32)(scroll_x1 / 4), (f32)(scroll_y1 / 4), 0.0f);
            PSMTXConcat(scale, mtx, mtx); GXLoadTexMtxImm(mtx, 0x1E, 1);
            PSMTXScale(scale, 0.015625f, 0.0625f, 1.0f);
            PSMTXTrans(mtx, (f32)(scroll_x2 / 4), (f32)(scroll_y2 / 4), 0.0f);
            PSMTXConcat(scale, mtx, mtx); GXLoadTexMtxImm(mtx, 0x21, 1);
        }
        if (type == 1) {
            magic2_4_dl();
        } else if (type < 1 && type > -1) {
            magic2_2_line_dl();
        }
    }
    GXSetProjectionv(projection);
    GXSetScissorBoxOffset(0, 0);
}

void magic2_2_line_dl(void) {
    extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void GXSetCullMode(s32); extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32); extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32); u8 tex[0x20];
    GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2); GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    effGetTexObjN64(0x11,tex); GXLoadTexObj(tex,0); GXLoadTexObj(tex,1); GXSetNumTevStages(3);
    GXSetTevOrder(0,0,0,-1); GXSetTevColorOp(0,1,0,0,1,3); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,15,3,8,14); GXSetTevAlphaIn(0,7,6,4,7);
    GXSetTevOrder(1,1,1,4); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0); GXSetTevColorIn(1,15,2,8,2); GXSetTevAlphaIn(1,7,0,5,7);
    GXSetTevOrder(2,1,1,4); GXSetTevColorOp(2,1,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0); GXSetTevColorIn(2,15,8,4,0); GXSetTevAlphaIn(2,7,4,5,0);
    GXSetCullMode(0); effSetVtxDescN64((void*)0x803A5AB8); GXBegin(0x90,0,0x48);
    tri2(0,1,2,0,0,2,3,0); tri2(4,5,1,0,4,1,0,0); tri2(3,2,6,0,3,6,7,0); tri2(8,9,5,0,8,5,4,0);
    tri2(10,11,9,0,10,9,8,0); tri2(12,13,11,0,12,11,10,0); tri2(7,6,14,0,7,14,15,0);
    tri2(16,17,18,0,16,18,19,0); tri2(19,18,20,0,19,20,21,0); tri2(22,23,17,0,22,17,16,0);
    tri2(24,25,23,0,24,23,22,0); tri2(15,14,25,0,15,25,24,0);
}

void magic2_4_dl(void) {
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32,void*);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32,s32,s32);
    extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);
    u8 tex[0x20];
    GXSetNumChans(1); GXSetChanCtrl(4,0,0,1,0,0,2);
    GXSetNumTexGens(2); GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D); GXSetTexCoordGen2(1,1,4,0x21,0,0x7D);
    effGetTexObjN64(0x12,tex); GXLoadTexObj(tex,0); effGetTexObjN64(0x13,tex); GXLoadTexObj(tex,1);
    GXSetNumTevStages(3);
    GXSetTevOrder(0,0,0,-1); GXSetTevColorOp(0,1,0,0,1,3); GXSetTevAlphaOp(0,0,0,0,1,0); GXSetTevColorIn(0,15,3,8,14); GXSetTevAlphaIn(0,7,6,4,7);
    GXSetTevOrder(1,1,1,-1); GXSetTevColorOp(1,0,0,0,1,0); GXSetTevAlphaOp(1,0,0,0,1,0); GXSetTevColorIn(1,15,2,8,2); GXSetTevAlphaIn(1,7,0,5,7);
    GXSetTevOrder(2,1,1,-1); GXSetTevColorOp(2,1,0,0,1,0); GXSetTevAlphaOp(2,0,0,0,1,0); GXSetTevColorIn(2,15,8,4,0); GXSetTevAlphaIn(2,7,4,5,0);
    GXSetCullMode(0); effSetVtxDescN64((void*)0x803A5C24); GXBegin(0x90,0,0x48);
    tri2(0,1,2,0,0,2,3,0); tri2(4,5,1,0,4,1,0,0); tri2(3,2,6,0,3,6,7,0); tri2(8,9,5,0,8,5,4,0);
    tri2(10,11,9,0,10,9,8,0); tri2(12,13,11,0,12,11,10,0); tri2(7,6,14,0,7,14,15,0);
    tri2(16,17,18,0,16,18,19,0); tri2(19,18,20,0,19,20,21,0); tri2(22,23,17,0,22,17,16,0);
    tri2(24,25,23,0,24,23,22,0); tri2(15,14,25,0,15,25,24,0);
}
