#include "effect/eff_mario_balloon.h"
u8 effMarioBalloonMain(void* effEntry);
void effMarioBalloonDisp(s32 cameraId, void* effect);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMarioBalloonEntry(int type, void* npc) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern char str_MarioBalloon_802f3a9c[];
    extern f32 float_0_80422d28;
    extern f32 float_0p4_80422d50;
    extern f32 float_0p8_80422d54;
    void* entry;
    void* work;

    entry = effEntry();
    *(void**)((s32)entry + 0x14) = str_MarioBalloon_802f3a9c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, *(s32*)((s32)entry + 8) << 5);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMarioBalloonMain;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 0x10) = float_0_80422d28;
    *(void**)((s32)work + 0x18) = npc;
    *(s32*)((s32)work + 0x1C) = 4;
    switch (type) {
        case 0:
            *(f32*)((s32)work + 0x14) = float_0p4_80422d50;
            break;
        case 1:
            *(f32*)((s32)work + 0x14) = float_0p8_80422d54;
            break;
    }
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effMarioBalloonMain(void* effEntry) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern void* marioGetPtr(void);
    extern f32 float_10_80422d48;
    extern f32 float_4_80422d4c;
    extern Vec3 vec3_802f3a90[];
    extern f32 dispCalcZ(Vec3* pos);
    extern u8 dispEntry();
    void* work = *(void**)((s32)effEntry + 0xC);
    void* npc = *(void**)((s32)work + 0x18);
    Vec3 pos;

    if (npc == 0) {
        void* mario = marioGetPtr();
        f32 x = *(f32*)((s32)mario + 0x8C);
        f32 add = float_10_80422d48;
        *(f32*)((s32)work + 4) = x;
        *(f32*)((s32)work + 8) = *(f32*)((s32)mario + 0x90) + *(f32*)((s32)mario + 0x1BC) + add;
        *(f32*)((s32)work + 0xC) = *(f32*)((s32)mario + 0x94);
    } else {
        f32 x = *(f32*)((s32)npc + 0x8C);
        f32 add = float_10_80422d48;
        *(f32*)((s32)work + 4) = x;
        *(f32*)((s32)work + 8) = *(f32*)((s32)npc + 0x90) + *(f32*)((s32)npc + 0x150) + add;
        *(f32*)((s32)work + 0xC) = *(f32*)((s32)npc + 0x94);
    }

    *(f32*)((s32)work + 0x10) += float_4_80422d4c;
    pos = vec3_802f3a90[0];
    pos.x = *(f32*)((s32)work + 4);
    pos.y = *(f32*)((s32)work + 8);
    pos.z = *(f32*)((s32)work + 0xC);
    return dispEntry(*(s32*)((s32)work + 0x1C), 8, effMarioBalloonDisp, effEntry, dispCalcZ(&pos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



void effMarioBalloonDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct Tex { u32 data[8]; } Tex;
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx,f32,f32,f32);
    extern void PSMTXRotRad(Mtx,f32,char);
    extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void PSMTXInvXpose(Mtx,void*);
    extern void effGetTexObj(s32,void*);
    extern void GXLoadTexObj(void*,s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetBlendMode(s32,s32,s32,s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);
    extern void GXSetZMode(s32,s32,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXLoadNrmMtxImm(void*,s32);
    extern void GXSetCullMode(s32);
    extern void GXBegin(s32,s32,s32);
    extern void GXSetTevColor(s32,void*);
    extern u32 dat_80422d10;
    Mtx trans,rotate,scale,model;
    f32 normal[3][4];
    Tex tex;
    volatile f32* fifo=(volatile f32*)0xCC008000;
    s32* work=*(s32**)((char*)effect+0xC);
    char* camera;
    f32 amount;
    u32 color;

    GXSetNumChans(0); GXSetChanCtrl(4,0,0,0,0,2,2);
    GXSetNumTevStages(1); GXSetTevOrder(0,0,0,-1);
    GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetBlendMode(1,4,5,0); GXSetZCompLoc(1); GXSetAlphaCompare(7,0,0,7,0);
    GXSetZMode(0,3,0); GXSetNumTexGens(1); GXSetTexCoordGen2(0,1,4,0x3C,0,0x7D);
    GXClearVtxDesc(); GXSetVtxDesc(9,1); GXSetVtxAttrFmt(0,9,1,4,0);
    GXSetVtxDesc(10,1); GXSetVtxAttrFmt(0,10,0,4,0);
    GXSetVtxDesc(13,1); GXSetVtxAttrFmt(0,13,1,4,0);
    PSMTXTrans(trans,(f32)work[1],(f32)work[2],(f32)work[3]);
    camera=(char*)camGetPtr(cameraId);
    PSMTXRotRad(rotate,0.017453292f*((f32)work[4]-*(f32*)(camera+0x114)),'y');
    amount=0.7f*(f32)work[5]; PSMTXScale(scale,amount,amount,amount);
    PSMTXConcat(trans,rotate,trans); PSMTXConcat(trans,scale,trans);
    camera=(char*)camGetPtr(cameraId); PSMTXConcat((f32(*)[4])(camera+0x48),trans,trans);
    GXSetCurrentMtx(0);
    if (work[0]==1) {
        effGetTexObj(0x11,&tex); GXLoadTexObj(&tex,0);
        GXSetTevColorIn(0,15,15,15,15); GXSetTevAlphaIn(0,7,7,7,4);
        GXLoadPosMtxImm(trans,0); GXSetCullMode(0); GXBegin(0x80,0,4);
    } else {
        effGetTexObj(0x10,&tex); GXLoadTexObj(&tex,0); color=dat_80422d10;
        GXSetTevColor(1,&color); GXSetTevColorIn(0,15,2,8,15); GXSetTevAlphaIn(0,7,7,7,4);
        GXLoadPosMtxImm(trans,0); PSMTXInvXpose(trans,normal); GXLoadNrmMtxImm(normal,0);
        GXSetCurrentMtx(0); GXSetCullMode(2); GXBegin(0x80,0,4);
    }
    *fifo=-20.0f;*fifo=32.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;*fifo=0.0f;*fifo=0.0f;
    *fifo=20.0f;*fifo=32.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;*fifo=1.0f;*fifo=0.0f;
    *fifo=20.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;*fifo=1.0f;*fifo=1.0f;
    *fifo=-20.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;*fifo=0.0f;*fifo=1.0f;
    if (work[0]==1) {
        effGetTexObj(0x10,&tex); GXLoadTexObj(&tex,0); color=dat_80422d10;
        GXSetTevColor(1,&color); GXSetTevColorIn(0,15,2,8,15); GXSetTevAlphaIn(0,7,7,7,4);
        GXLoadPosMtxImm(trans,0); PSMTXInvXpose(trans,normal); GXLoadNrmMtxImm(normal,0);
        GXSetCurrentMtx(0); GXSetCullMode(2); GXBegin(0x80,0,4);
        *fifo=-20.0f;*fifo=32.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;*fifo=0.0f;*fifo=0.0f;
        *fifo=20.0f;*fifo=32.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;*fifo=1.0f;*fifo=0.0f;
        *fifo=20.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;*fifo=1.0f;*fifo=1.0f;
        *fifo=-20.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;*fifo=0.0f;*fifo=1.0f;
    }
}
