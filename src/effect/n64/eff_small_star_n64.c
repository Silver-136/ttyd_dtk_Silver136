#include "effect/n64/eff_small_star_n64.h"


void* effSmallStarN64Entry(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ,
                           s32 type, s32 count) {
    typedef f32 Mtx[3][4];
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effSmallStarMain(void*);
    extern void* camGetPtr(s32);
    extern void PSMTXRotAxisRad(Mtx, Vec*, f32);
    extern void PSMTXMultVec(Mtx, Vec*, Vec*);
    extern f64 sqrt(f64);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_SmallStarN64_802fbf68[];
    static f32 rotation;
    void* entry = effEntry();
    u8* part;
    Vec direction;
    Vec axis;
    Vec radial;
    Mtx matrix;
    f32 length;
    f32 angle;
    f32 camAngle;
    s32 i;

    if (count == 0) {
        return 0;
    }
    length = (f32)sqrt((f64)(dirX * dirX + dirY * dirY + dirZ * dirZ));
    if (length == 0.0f) {
        return 0;
    }
    direction.x = -dirX / length;
    direction.y = -dirY / length;
    direction.z = -dirZ / length;
    if (direction.x == 0.0f) {
        axis.x = 1.0f;
        axis.y = direction.y == 0.0f ? 0.0f : -direction.x / direction.y;
        axis.z = direction.y == 0.0f ? -direction.x / direction.z : 0.0f;
    } else {
        axis.x = -direction.y / direction.x;
        axis.y = 1.0f;
        axis.z = 0.0f;
    }
    length = (f32)sqrt((f64)(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z));
    if (length == 0.0f) {
        return 0;
    }
    axis.x = axis.x * 8.0f / length;
    axis.y = axis.y * 8.0f / length;
    axis.z = axis.z * 8.0f / length;
    direction.x *= 8.0f;
    direction.y *= 8.0f;
    direction.z *= 8.0f;

    *(char**)((u8*)entry + 0x14) = str_SmallStarN64_802fbf68;
    *(s32*)((u8*)entry + 8) = count;
    part = __memAlloc(3, count * 0x34);
    *(u8**)((u8*)entry + 0xC) = part;
    *(void**)((u8*)entry + 0x10) = effSmallStarMain;
    for (i = 0; i < count; i++, part += 0x34) {
        camAngle = *(f32*)((u8*)camGetPtr(4) + 0x114);
        *(s32*)part = type;
        *(f32*)(part + 4) = x;
        *(f32*)(part + 8) = y;
        *(f32*)(part + 0xC) = z;
        *(s8*)(part + 0x30) = 4;
        radial = axis;
        if (type == 3) {
            angle = count == 1 ? 0.0f :
                    100.0f * (f32)i / (f32)(count - 1) - 50.0f;
            radial.x = (f32)sin((6.2832f * camAngle) / 360.0f);
            radial.y = 0.0f;
            radial.z = -(f32)cos((6.2832f * camAngle) / 360.0f);
            PSMTXRotAxisRad(matrix, &radial, 0.017453292f * angle);
            PSMTXMultVec(matrix, &direction, &radial);
        } else if (type >= 0 && type < 3) {
            angle = 360.0f * (f32)i / (f32)(count - 1);
            PSMTXRotAxisRad(matrix, &direction, 0.017453292f * angle);
            PSMTXMultVec(matrix, &axis, &radial);
            radial.x += direction.x;
            radial.y += direction.y;
            radial.z += direction.z;
        } else if (type < 5) {
            angle = 360.0f * (f32)i / (f32)(count - 1);
            radial.x = (f32)sin((6.2832f * camAngle) / 360.0f);
            radial.y = 0.0f;
            radial.z = -(f32)cos((6.2832f * camAngle) / 360.0f);
            PSMTXRotAxisRad(matrix, &radial, 0.017453292f * angle);
            PSMTXMultVec(matrix, &direction, &radial);
        }
        *(f32*)(part + 0x10) = radial.x;
        *(f32*)(part + 0x14) = radial.y;
        *(f32*)(part + 0x18) = radial.z;
        if (type == 2) {
            *(f32*)(part + 0x10) *= 0.5f;
            *(f32*)(part + 0x14) *= 0.5f;
            *(f32*)(part + 0x18) *= 0.5f;
            *(f32*)(part + 0x20) = 20.0f;
            *(s32*)(part + 0x28) = 0x28;
        } else {
            *(f32*)(part + 0x20) = type == 0 ? 0.0f : 20.0f;
            *(s32*)(part + 0x28) = 0x18;
        }
        *(f32*)(part + 0x1C) = 0.0f;
        *(s32*)(part + 0x24) = 0xFF;
        *(s32*)(part + 0x2C) = 0;
    }
    rotation += 15.0f;
    if (rotation > 360.0f) {
        rotation = 0.0f;
    }
    return entry;
}

void effSmallStarMain(void* effect) {
    extern void effDelete(void*);extern f32 dispCalcZ(void*);extern void dispEntry(s32,s32,void*,void*,f32);extern void effSmallStarDisp(void);extern f32 float_0p8_80426030,float_0p94_80426028,float_0_8042602c,float_0p7_80426038,float_0p6_80426034,float_1_80426014;
    u8* work=*(u8**)((s32)effect+0xC);s32 timer,i,type=*(s32*)work;timer=*(s32*)(work+0x28)-1;*(s32*)(work+0x28)=timer;*(s32*)(work+0x2C)+=1;if(timer<0){effDelete(effect);return;}
    for(i=0;i<*(s32*)((s32)effect+8);i++,work+=0x34){if(type==2){*(f32*)(work+0x14)=float_1_80426014;*(f32*)(work+0x10)*=float_0p94_80426028;*(f32*)(work+0x18)*=float_0p94_80426028;if(*(f32*)(work+0x14)<float_0_8042602c){*(f32*)(work+0x20)*=float_0p8_80426030;*(s32*)(work+0x24)=(s32)((f32)*(s32*)(work+0x24)*float_0p6_80426034);}}else{if(timer>5){*(f32*)(work+0x10)*=float_0p8_80426030;*(f32*)(work+0x14)*=float_0p8_80426030;*(f32*)(work+0x18)*=float_0p8_80426030;}if(timer<5){*(f32*)(work+0x20)*=float_0p8_80426030;*(s32*)(work+0x24)=(s32)((f32)*(s32*)(work+0x24)*float_0p6_80426034);*(f32*)(work+0x10)*=float_0p7_80426038;*(f32*)(work+0x14)*=float_0p7_80426038;*(f32*)(work+0x18)*=float_0p7_80426038;}}*(f32*)(work+4)+=*(f32*)(work+0x10);*(f32*)(work+8)+=*(f32*)(work+0x14);*(f32*)(work+0xC)+=*(f32*)(work+0x18);*(f32*)(work+0x1C)+=*(f32*)(work+0x20);}dispEntry(*(u8*)(*(u8**)((s32)effect+0xC)+0x30),2,effSmallStarDisp,effect,dispCalcZ(*(u8**)((s32)effect+0xC)+4));
}

void effSmallStarDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);extern void GXSetZMode(s32,s32,s32);extern void GXSetNumChans(s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void GXSetCullMode(s32);extern void effSetVtxDescN64(void*);extern void GXSetTevColor(s32,void*);extern void PSMTXTrans(void*,f32,f32,f32);extern void PSMTXRotRad(void*,s32,f32);extern void PSMTXConcat(void*,void*,void*);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXBegin(s32,s32,s32);extern void tri2(s32,s32,s32,s32,s32,s32,s32,s32);extern u8 color_rotation_data[];
    u8* w=*(u8**)((u8*)effect+0xC);u8* cam=camGetPtr(cameraId);f32 a[3][4],b[3][4],c[3][4];u8 tex[0x20];u32 col;s32 i,idx=(*(s32*)(w+0x2C)-1)*3;
    GXSetZMode(0,7,0);GXSetNumChans(0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x3C,0,0x7D);effGetTexObjN64(0x1D,tex);GXLoadTexObj(tex,0);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,-1);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,15,15,15,2);GXSetTevAlphaIn(0,7,5,4,7);GXSetCullMode(0);effSetVtxDescN64((void*)0x803A89A0);
    for(i=0;i<*(s32*)((u8*)effect+8);i++,w+=0x34,idx+=3){col=(color_rotation_data[idx%36]<<24)|(color_rotation_data[(idx+1)%36]<<16)|(color_rotation_data[(idx+2)%36]<<8)|*(u8*)(w+0x24);GXSetTevColor(1,&col);PSMTXTrans(a,*(f32*)(w+4),*(f32*)(w+8),*(f32*)(w+0xC));PSMTXRotRad(b,0x79,-0.017453292f**(f32*)((u8*)camGetPtr(4)+0x114));PSMTXRotRad(c,0x7A,0.017453292f**(f32*)(w+0x1C));PSMTXConcat(a,b,a);PSMTXConcat(a,c,a);PSMTXConcat(cam+0x11C,a,a);GXLoadPosMtxImm(a,0);GXSetCurrentMtx(0);GXBegin(0x90,0,6);tri2(0,1,2,0,0,2,3,0);}
}

void effSmallStarN64CamChg(void* effect, s8 camId) {
    void* work = *(void**)((s32)effect + 0xC);
    s32 i;

    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work = (void*)((s32)work + 0x34)) {
        *(s8*)((s32)work + 0x30) = camId;
    }
}
