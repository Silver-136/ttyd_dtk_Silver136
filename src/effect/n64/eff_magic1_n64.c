#include "effect/n64/eff_magic1_n64.h"

typedef f32 Mtx[3][4];

void* camGetPtr(s32 cameraId);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx mtx, s32 axis, f32 rad);
void PSMTXConcat(void* a, void* b, void* ab);

extern f32 float_0_804259cc;
extern f32 float_deg2rad_804259d0;
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMagic1N64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 targetX, f32 targetY, f32 targetZ) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effMagic1Main(void*);
    extern char str_Magic1N64_802fb430[];
    extern f32 float_0_804259cc;
    extern f32 float_1_80425a04;
    extern f32 float_29_80425a1c;
    extern f32 float_6p2_80425a20;
    void* entry;
    u8* work;
    f32 invTime;
    f32 one;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Magic1N64_802fb430;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x38);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMagic1Main;
    *(s32*)entry |= 2;

    one = float_1_80425a04;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)work = type;
    *(s32*)(work + 0x34) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x28) = one;

    if (type == 0) {
        zero = float_0_804259cc;
        invTime = one / (f32)time;
        *(f32*)(work + 0x10) = zero;
        *(f32*)(work + 0x14) = zero;
        *(f32*)(work + 0x18) = zero;
        *(f32*)(work + 0x1C) = invTime * (targetX - x);
        *(f32*)(work + 0x20) = invTime * (targetY - y);
        *(f32*)(work + 0x24) = invTime * (targetZ - z);
        *(s32*)(work + 0x30) = time;
    } else {
        zero = float_0_804259cc;
        *(f32*)(work + 0x10) = float_29_80425a1c;
        *(f32*)(work + 0x14) = zero;
        *(f32*)(work + 0x18) = zero;
        *(f32*)(work + 0x1C) = float_6p2_80425a20;
        *(f32*)(work + 0x20) = zero;
        *(f32*)(work + 0x24) = zero;
        *(s32*)(work + 0x30) = 0x18;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagic1Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern s32 marioStGetSystemLevel(void);
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effMagic1Disp(s32, void*);
    extern void* effMagic1N64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 targetX, f32 targetY, f32 targetZ);
    extern const Vec3 vec3_802fb418;
    extern f32 float_0_804259cc;
    extern f32 float_0p83_80425a14;
    extern f32 float_0p9_80425a18;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 type;
    u32 systemLevel;
    void* spawned;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb418;
    pos.x = *(f32*)(work + 0x10);
    pos.y = *(f32*)(work + 0x14);
    pos.z = *(f32*)(work + 0x18);
    dispPos = pos;

    systemLevel = (u32)marioStGetSystemLevel();
    if (systemLevel != 0U) {
        dispEntry(4, 2, effMagic1Disp, effect, dispCalcZ(&dispPos));
        return;
    }

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x30) = 0;
    }
    *(s32*)(work + 0x30) -= 1;
    *(s32*)(work + 0x2C) += 1;
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effect);
        return;
    }

    type = *(s32*)work;
    if (type == 0 && *(s32*)(work + 0x30) == 0) {
        spawned = effMagic1N64Entry(1, 0x30,
                                    *(f32*)(work + 4) + *(f32*)(work + 0x10),
                                    *(f32*)(work + 8) + *(f32*)(work + 0x14),
                                    *(f32*)(work + 0xC) + *(f32*)(work + 0x18),
                                    float_0_804259cc, float_0_804259cc, float_0_804259cc);
        *(f32*)((s32)*(void**)((s32)spawned + 0xC) + 0x28) = *(f32*)(work + 0x28);
    }

    if (type == 1) {
        *(f32*)(work + 0x1C) *= float_0p83_80425a14;
        *(s32*)(work + 0x34) = (s32)((f32)*(s32*)(work + 0x34) * float_0p9_80425a18);
    }

    *(f32*)(work + 0x10) += *(f32*)(work + 0x1C);
    *(f32*)(work + 0x14) += *(f32*)(work + 0x20);
    *(f32*)(work + 0x18) += *(f32*)(work + 0x24);
    dispEntry(4, 2, effMagic1Disp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off



void main_dl(void* effect, f32 mtx[3][4]) {
    typedef f32 Mtx[3][4]; extern void* camGetPtr(s32); extern f64 sin(f64); extern f64 cos(f64); extern void PSMTXTrans(void*,f32,f32,f32); extern void PSMTXRotRad(void*,s32,f32); extern void PSMTXScale(void*,f32,f32,f32); extern void PSMTXConcat(void*,void*,void*); extern void GXSetNumChans(s32); extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32); extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32); extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32); extern void GXSetCullMode(s32); extern void GXClearVtxDesc(void); extern void GXSetVtxDesc(s32,s32); extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32); extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32); extern void GXLoadTexMtxImm(void*,s32,s32); extern void GXLoadPosMtxImm(void*,s32); extern void GXSetCurrentMtx(s32); extern void GXSetTevColor(s32,void*); extern void GXBegin(s32,s32,s32);
    u8* w=*(u8**)((u8*)effect+0xC);Mtx trans,rot,scale,out,texMtx;u8 tex[0x20];u32 color;s32 type=*(s32*)w,frame=*(s32*)(w+0x30),alpha=*(s32*)(w+0x34),i,j;f32 baseAngle=(f32)(frame*(type?25:35));f32 yaw=type?0.0f:-*(f32*)((u8*)camGetPtr(4)+0x114);f32 step=type?6.0f:9.0f;f32 px=*(f32*)(w+0x10),py=*(f32*)(w+0x14),pz=*(f32*)(w+0x18),size=*(f32*)(w+0x28);
    GXSetNumChans(0);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,0xFF);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,2,4,8,15);GXSetTevAlphaIn(0,0,1,4,7);effGetTexObjN64(0x10,tex);GXLoadTexObj(tex,0);GXSetCullMode(0);GXClearVtxDesc();GXSetVtxDesc(9,1);GXSetVtxDesc(0xD,1);GXSetVtxAttrFmt(0,9,1,3,0);GXSetVtxAttrFmt(0,0xD,1,4,0);
    for(i=0;i<2;i++){if(i>0){px=-(2.0f**(f32*)(w+0x1C)-px);py=-(2.0f**(f32*)(w+0x20)-py);pz=-(2.0f**(f32*)(w+0x24)-pz);baseAngle-=type?50.0f:70.0f;alpha=(100*alpha)/255;}GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);PSMTXTrans(trans,px,py,pz);PSMTXScale(scale,size,size,size);PSMTXRotRad(rot,0x79,yaw*0.0174533f);PSMTXConcat(rot,trans,trans);PSMTXConcat(trans,scale,out);for(j=0;j<8;j++){f32 a=6.2832f*(baseAngle+j*120.0f)/360.0f;f32 c=(f32)cos(a),sn=(f32)sin(a);PSMTXTrans(trans,c*step,sn*step,0.0f);PSMTXRotRad(rot,0x7A,a);PSMTXConcat(trans,rot,trans);PSMTXConcat(out,trans,trans);PSMTXConcat(mtx,trans,trans);GXLoadPosMtxImm(trans,0);GXSetCurrentMtx(0);PSMTXScale(texMtx,0.00390625f,0.00390625f,0.0f);GXLoadTexMtxImm(texMtx,0x1E,1);color=0xFFFFFF00|(u8)alpha;GXSetTevColor(1,&color);GXBegin(0x80,0,4);}}
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagic1Disp(s32 cameraId, void* effEntry) {
    Mtx trans;
    Mtx rot;
    Mtx mtx;
    Mtx loopRot;
    register s32* work;
    register void* cam;
    register s32 type;
    f32 angle;
    s32 i;
    s32 rotAngle;

    cam = camGetPtr(cameraId);
    work = *(s32**)((s32)effEntry + 0xC);
    type = work[0];

    switch (type) {
        case 0:
            angle = float_0_804259cc;
            break;
        default:
            angle = -*(f32*)((s32)camGetPtr(4) + 0x114);
            break;
    }

    PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_804259d0 * angle);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);

    switch (type) {
        case 0:
            main_dl(effEntry, mtx);
            break;
        case 1:
            angle = float_deg2rad_804259d0;
            i = 0;
            rotAngle = 0;
            while (i < 12) {
                PSMTXRotRad(loopRot, 0x7A, angle * (f32)rotAngle);
                PSMTXConcat(mtx, loopRot, loopRot);
                main_dl(effEntry, loopRot);
                i++;
                rotAngle += 30;
            }
            break;
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
