#include "effect/eff_damage_star.h"

void* effDamageStarEntry(f32 x, f32 y, f32 z, f32 speed, f32 baseRot, s32 kind, s32 count) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* ptr, s32 value, u32 size);
    extern void* effMayaAnimAlloc(void* data);
    extern void effPlayMayaAnim(void* anim);
    extern void* gpGlobals;
    extern char str_DamageStar_802f3a80[];
    extern void* damageStar_effAnimDt;
    extern u8 effDamageStarMain(void*);
    extern const f32 float_0_80422c80;
    extern const f32 float_1_80422c98;
    extern const f32 float_30_80422cdc;
    extern const f32 float_360_80422ce4;
    extern const f32 float_6p2832_80422ce0;
    extern const f32 float_1p5_80422cd0;
    extern const f32 float_0p2_80422d04;
    extern const f32 float_0p16605_80422cec;
    extern const f32 float_3p1416_80422cf0;
    extern const f32 float_0p4967_80422d00;
    extern const f32 float_0p00761_80422ce8;
    extern const f32 float_0p03705_80422cfc;
    extern const f32 float_1p5708_80422cf4;
    extern const f32 float_4p7124_80422cf8;
    void* entry;
    u8* work;
    u8* part;
    s32 originalCount;
    s32 size;
    s32 i;
    f32 angle;
    f32 spread;
    f32 dir;
    f32 t;
    f32 t2;
    f32 s;
    f32 c;

    entry = effEntry();
    originalCount = count;
    if (count > 20) count = 20;
    *(void**)((s32)entry + 0x14) = str_DamageStar_802f3a80;
    *(s32*)((s32)entry + 0x8) = count + 2;
    size = (count + 2) * 0x48;
    work = __memAlloc(3, size);
    *(void**)((s32)entry + 0xC) = work;
    memset(work, 0, size);
    *(void**)((s32)entry + 0x10) = effDamageStarMain;
    *(void**)(work + 0x44) = effMayaAnimAlloc(&damageStar_effAnimDt);
    effPlayMayaAnim(*(void**)(work + 0x44));
    *(s32*)(work + 0x0) = kind;
    *(f32*)(work + 0x4) = x;
    *(f32*)(work + 0x8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x1C) = speed;
    *(f32*)(work + 0x38) = baseRot;
    *(s32*)(work + 0x24) = 99;
    *(s32*)(work + 0x28) = 0;
    *(s32*)(work + 0x3C) = originalCount;
    *(s32*)(work + 0x40) = *(s32*)((s32)gpGlobals + 0x1C);

    spread = float_30_80422cdc - (f32)(count + 2);
    part = work + 0x48;
    for (i = 1; i < count + 2; i++, part += 0x48) {
        dir = (f32)((i * (((i % 2) * 2) - 1)) / 2);
        *(f32*)(part + 0x38) = baseRot + spread * dir;
        *(f32*)(part + 0x4) = float_0_80422c80;
        *(f32*)(part + 0x8) = float_0_80422c80;
        *(f32*)(part + 0xC) = float_0_80422c80;
        angle = (float_6p2832_80422ce0 * *(f32*)(part + 0x38)) / float_360_80422ce4;

        if (angle <= float_3p1416_80422cf0) {
            if (angle <= float_1p5708_80422cf4) {
                t = angle;
            } else {
                t = float_1p5708_80422cf4 - (angle - float_1p5708_80422cf4);
            }
            t2 = t * t;
            s = ((float_0p00761_80422ce8 * t2 - float_0p16605_80422cec) * t2 + float_1_80422c98) * t;
        } else {
            if (angle >= float_4p7124_80422cf8) {
                t = float_1p5708_80422cf4 - (angle - float_4p7124_80422cf8);
            } else {
                t = angle - float_3p1416_80422cf0;
            }
            t2 = t * t;
            s = -(((float_0p00761_80422ce8 * t2 - float_0p16605_80422cec) * t2 + float_1_80422c98) * t);
        }
        *(f32*)(part + 0x10) = -(speed * s) * float_1p5_80422cd0;

        if (angle <= float_3p1416_80422cf0) {
            if (angle <= float_1p5708_80422cf4) {
                t = angle;
                t2 = t * t;
                c = (float_0p03705_80422cfc * t2 - float_0p4967_80422d00) * t2 + float_1_80422c98;
            } else {
                t = float_1p5708_80422cf4 - (angle - float_1p5708_80422cf4);
                t2 = t * t;
                c = -((float_0p03705_80422cfc * t2 - float_0p4967_80422d00) * t2 + float_1_80422c98);
            }
        } else {
            if (angle >= float_4p7124_80422cf8) {
                t = float_1p5708_80422cf4 - (angle - float_4p7124_80422cf8);
                t2 = t * t;
                c = (float_0p03705_80422cfc * t2 - float_0p4967_80422d00) * t2 + float_1_80422c98;
            } else {
                t = angle - float_3p1416_80422cf0;
                t2 = t * t;
                c = -((float_0p03705_80422cfc * t2 - float_0p4967_80422d00) * t2 + float_1_80422c98);
            }
        }
        *(f32*)(part + 0x14) = (speed * c) * float_1p5_80422cd0;
        *(f32*)(part + 0x18) = float_0_80422c80;
        *(f32*)(part + 0x1C) = float_0_80422c80;
        *(s32*)(part + 0x20) = 0xFF;
        *(f32*)(part + 0x2C) = float_0p2_80422d04;
        *(f32*)(part + 0x30) = float_0p2_80422d04;
        *(f32*)(part + 0x34) = float_0p2_80422d04;
    }
    return entry;
}

u8 effDamageStarMain(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effCalcMayaAnim(void* anim);
    extern void effDeleteMayaAnim(void* anim);
    extern void effDelete(void* effect);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 cur, s32 steps);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effDamageStarDisp(void);
    extern void* gpGlobals;
    extern const LocalVec3 vec3_802f3a60;
    extern u8 scale_data[];
    extern f32 float_0_80422c80;
    extern f32 float_1_80422c98;
    extern f32 float_10_80422ca4;
    extern f32 float_0p5_80422cc4;
    extern f32 float_0p01_80422cc8;
    extern f32 float_720_80422ccc;
    extern f32 float_1p5_80422cd0;
    extern f32 float_0p1_80422cd4;
    extern f32 float_0p75_80422cd8;
    LocalVec3 pos;
    LocalVec3 dispPos;
    void* work;
    void* part;
    u8* scaleTable;
    u32 delta;
    s32 timer;
    s32 i;
    f32 scale;
    f32 velScale;

    work = *(void**)((s32)effEntry + 0xC);

    pos = vec3_802f3a60;
    pos.x = *(f32*)((s32)work + 0x4);
    pos.y = *(f32*)((s32)work + 0x8);
    pos.z = *(f32*)((s32)work + 0xC);
    dispPos = pos;

    effCalcMayaAnim(*(void**)((s32)work + 0x44));

    delta = 0;
    if (*(u32*)((s32)work + 0x40) < *(u32*)((s32)gpGlobals + 0x1C)) {
        delta = *(u32*)((s32)gpGlobals + 0x1C) - *(u32*)((s32)work + 0x40);
    }
    if (delta < 1) {
        delta = 1;
    }

    velScale = float_0p75_80422cd8;

    while (delta-- != 0) {
        work = *(void**)((s32)effEntry + 0xC);

        if (*(s32*)((s32)work + 0x24) < 100) {
            *(s32*)((s32)work + 0x24) -= 1;
        }
        *(s32*)((s32)work + 0x28) += 1;

        if (*(s32*)((s32)work + 0x24) < 0) {
            effDeleteMayaAnim(*(void**)((s32)work + 0x44));
            effDelete(effEntry);
            goto exit;
        }

        timer = *(s32*)((s32)work + 0x28);
        i = 2;
        scaleTable = scale_data;
        part = (void*)((s32)work + 0x90);

        for (; i < *(s32*)((s32)effEntry + 0x8);
             i++, part = (void*)((s32)part + 0x48)) {
            if (timer < 20) {
                scale = float_0p01_80422cc8 * (f32)scaleTable[timer];
                *(f32*)((s32)part + 0x34) = scale;
                *(f32*)((s32)part + 0x30) = scale;
                *(f32*)((s32)part + 0x2C) = scale;
            } else if (timer < 40) {
                *(f32*)((s32)part + 0x1C) =
                    intplGetValue(float_0_80422c80, float_720_80422ccc, 0xB, timer - 20, 19);
            } else if (timer < 50) {
                *(f32*)((s32)part + 0x2C) =
                    intplGetValue(float_1_80422c98, float_1p5_80422cd0, 4, timer - 40, 9);
                *(f32*)((s32)part + 0x30) =
                    intplGetValue(float_1_80422c98, float_0p5_80422cc4, 4, timer - 40, 9);
            } else if (timer < 60) {
                *(f32*)((s32)part + 0x2C) =
                    intplGetValue(float_1p5_80422cd0, float_0p1_80422cd4, 1, timer - 50, 9);
                *(f32*)((s32)part + 0x30) =
                    intplGetValue(float_0p5_80422cc4, float_1_80422c98, 1, timer - 50, 9);
                *(f32*)((s32)part + 0x8) =
                    intplGetValue(*(f32*)((s32)part + 0x8),
                                  *(f32*)((s32)part + 0x8) + float_10_80422ca4,
                                  1, timer - 50, 9);
            } else {
                scale = float_0_80422c80;
                *(f32*)((s32)part + 0x34) = scale;
                *(f32*)((s32)part + 0x30) = scale;
                *(f32*)((s32)part + 0x2C) = scale;
            }

            *(f32*)((s32)part + 0x10) *= velScale;
            *(f32*)((s32)part + 0x14) *= velScale;
            *(f32*)((s32)part + 0x18) *= velScale;
            *(f32*)((s32)part + 0x4) += *(f32*)((s32)part + 0x10);
            *(f32*)((s32)part + 0x8) += *(f32*)((s32)part + 0x14);
            *(f32*)((s32)part + 0xC) += *(f32*)((s32)part + 0x18);
        }
    }

    work = *(void**)((s32)effEntry + 0xC);
    *(s32*)((s32)work + 0x40) = *(s32*)((s32)gpGlobals + 0x1C);
    dispEntry(4, 8, effDamageStarDisp, effEntry, dispCalcZ(&dispPos));

exit:
    ;
}


void effDamageStarDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct Tex { u32 data[8]; } Tex;
    extern void* camGetPtr(s32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXIdentity(Mtx);
    extern void PSMTXScaleApply(Mtx, Mtx, f32, f32, f32);
    extern void PSMTXTransApply(Mtx, Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetChanAmbColor(s32, void*);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void GXSetBlendMode(s32,s32,s32,s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);
    extern void GXSetZMode(s32,s32,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevOp(s32, s32);
    extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32,s32,s32);
    extern u32 dat_80422c78;
    extern u32 dat_80422c7c;
    extern u8 color_rotation_data[];
    extern f32 float_neg12_80422ca0, float_12_80422c9c, float_10_80422ca4;
    extern f32 float_neg1p5_80422ca8, float_neg2_80422cb4, float_2_80422c94;
    extern f32 float_0_80422c80, float_1_80422c98;
    Mtx rotate, trans, scale, base, model;
    Tex tex;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    char* work = *(char**)((char*)effect + 0xC);
    char* part;
    char* camera;
    u32 color;
    s32 i;
    s32 frame;
    char* anim;

    camera = (char*)camGetPtr(cameraId);
    PSMTXRotRad(rotate, -0.017453292f * *(f32*)(camera + 0x114), 'y');
    PSMTXTrans(trans, (f32)*(s32*)(work+4), (f32)*(s32*)(work+8), (f32)*(s32*)(work+12));
    PSMTXScale(scale, 0.8f, 0.8f, 0.8f);
    PSMTXConcat(trans, rotate, base);
    PSMTXConcat(base, scale, base);
    for (i=0; i<4; i++) { effGetTexObj(i,&tex); GXLoadTexObj(&tex,i); }
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    GXSetTexCoordGen2(1,1,4,0x3C,0,0x7D);
    GXSetNumChans(1); GXSetChanCtrl(4,0,0,0,0,0,2);
    color = dat_80422c78; GXSetChanAmbColor(4,&color);
    GXSetCullMode(0); GXClearVtxDesc();
    GXSetVtxDesc(9,1); GXSetVtxAttrFmt(0,9,1,4,0);
    GXSetVtxDesc(13,1); GXSetVtxAttrFmt(0,13,1,4,0);
    GXSetBlendMode(0,1,0,0); GXSetZCompLoc(0); GXSetAlphaCompare(6,0x80,1,0,0);
    GXSetZMode(0,3,0); GXSetNumTevStages(1); GXSetTevOrder(0,1,2,4);
    GXSetTevColorOp(0,0,0,0,1,0); GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,15,10,8,15); GXSetTevAlphaIn(0,7,5,4,7);
    if (*(s32*)work == 1) { effGetTexObj(0x15,&tex); GXLoadTexObj(&tex,2); }
    part = work + 0x90;
    for (i=2; i<*(s32*)((char*)effect+8); i++,part+=0x48) {
        frame = (i + *(s32*)(work+0x28)) % 12;
        color = ((u32)color_rotation_data[frame*3]<<24) |
                ((u32)color_rotation_data[frame*3+1]<<16) |
                ((u32)color_rotation_data[frame*3+2]<<8) | (u8)part[0x20];
        GXSetChanMatColor(4,&color);
        PSMTXIdentity(model);
        PSMTXScaleApply(model,model,*(f32*)(part+0x2C),*(f32*)(part+0x30),*(f32*)(part+0x34));
        PSMTXRotRad(rotate,0.017453292f**(f32*)(part+0x1C),'y');
        PSMTXConcat(model,rotate,model); PSMTXScale(scale,0.8f,0.8f,0.8f);
        PSMTXConcat(model,scale,model); PSMTXConcat(base,model,model);
        camera=(char*)camGetPtr(cameraId); PSMTXConcat((f32(*)[4])(camera+0x48),model,model);
        GXLoadPosMtxImm(model,0); GXSetCurrentMtx(0); GXSetCullMode(2);
        GXBegin(0x80,0,4);
        *fifo=-8.0f;*fifo=8.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;
        *fifo=8.0f;*fifo=8.0f;*fifo=0.0f;*fifo=2.0f;*fifo=0.0f;
        *fifo=8.0f;*fifo=-8.0f;*fifo=0.0f;*fifo=2.0f;*fifo=1.0f;
        *fifo=-8.0f;*fifo=-8.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;
        GXSetCullMode(1);
        GXSetTevColorOp(0,0,0,3,1,0);
        GXBegin(0x80,0,4);
        *fifo=-8.0f;*fifo=8.0f;*fifo=0.0f;*fifo=0.0f;*fifo=0.0f;
        *fifo=8.0f;*fifo=8.0f;*fifo=0.0f;*fifo=2.0f;*fifo=0.0f;
        *fifo=8.0f;*fifo=-8.0f;*fifo=0.0f;*fifo=2.0f;*fifo=1.0f;
        *fifo=-8.0f;*fifo=-8.0f;*fifo=0.0f;*fifo=0.0f;*fifo=1.0f;
    }

    camera = (char*)camGetPtr(cameraId);
    PSMTXRotRad(rotate, -0.017453292f * *(f32*)(camera + 0x114), 'y');
    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXConcat(trans, rotate, base);
    if (*(f32*)(work + 0x38) > float_0_80422c80) {
        PSMTXScale(scale, float_neg12_80422ca0, float_12_80422c9c, float_12_80422c9c);
    } else {
        PSMTXScale(scale, float_12_80422c9c, float_12_80422c9c, float_12_80422c9c);
    }
    PSMTXConcat(base, scale, base);
    PSMTXTrans(trans, float_10_80422ca4, float_neg1p5_80422ca8, float_0_80422c80);
    PSMTXConcat(base, trans, base);
    anim = *(char**)(work + 0x44);
    PSMTXConcat(base, (f32(*)[4])(anim + 0x14), base);
    PSMTXConcat(base, (f32(*)[4])(anim + 0x48), base);
    PSMTXTransApply(base, model, float_0_80422c80, float_0_80422c80, float_0_80422c80);
    PSMTXConcat((f32(*)[4])(camera + 0x48), model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 1, 3, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetCullMode(0);
    if (*(s32*)work == 1) {
        effGetTexObj(0x27, &tex);
        GXLoadTexObj(&tex, 3);
        GXBegin(0x80, 0, 4);
        *fifo=float_neg2_80422cb4;*fifo=float_2_80422c94;*fifo=float_0_80422c80;*fifo=float_0_80422c80;*fifo=float_0_80422c80;
        *fifo=float_2_80422c94;*fifo=float_2_80422c94;*fifo=float_0_80422c80;*fifo=float_2_80422c94;*fifo=float_0_80422c80;
        *fifo=float_2_80422c94;*fifo=float_neg2_80422cb4;*fifo=float_0_80422c80;*fifo=float_2_80422c94;*fifo=float_1_80422c98;
        *fifo=float_neg2_80422cb4;*fifo=float_neg2_80422cb4;*fifo=float_0_80422c80;*fifo=float_0_80422c80;*fifo=float_1_80422c98;
    } else {
        GXBegin(0x80, 0, 4);
        *fifo=float_neg2_80422cb4;*fifo=float_2_80422c94;*fifo=float_0_80422c80;*fifo=float_0_80422c80;*fifo=float_0_80422c80;
        *fifo=float_2_80422c94;*fifo=float_2_80422c94;*fifo=float_0_80422c80;*fifo=float_2_80422c94;*fifo=float_0_80422c80;
        *fifo=float_2_80422c94;*fifo=float_neg2_80422cb4;*fifo=float_0_80422c80;*fifo=float_2_80422c94;*fifo=float_1_80422c98;
        *fifo=float_neg2_80422cb4;*fifo=float_neg2_80422cb4;*fifo=float_0_80422c80;*fifo=float_0_80422c80;*fifo=float_1_80422c98;
    }

    PSMTXConcat((f32(*)[4])(camera + 0x48), base, model);
    GXLoadPosMtxImm(model, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 1, 2, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 2, 8, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    color = dat_80422c7c;
    GXSetTevColor(1, &color);
    if (*(s32*)work == 1) {
        effGetTexObj(0x13, &tex);
        GXLoadTexObj(&tex, 2);
        GXSetTevOp(0, 3);
        GXBegin(0x80, 0, 4);
        *fifo=float_neg2_80422cb4;*fifo=float_2_80422c94;*fifo=float_0_80422c80;*fifo=float_0_80422c80;*fifo=float_0_80422c80;
        *fifo=float_2_80422c94;*fifo=float_2_80422c94;*fifo=float_0_80422c80;*fifo=float_2_80422c94;*fifo=float_0_80422c80;
        *fifo=float_2_80422c94;*fifo=float_neg2_80422cb4;*fifo=float_0_80422c80;*fifo=float_1_80422c98;*fifo=float_1_80422c98;
        *fifo=float_neg2_80422cb4;*fifo=float_neg2_80422cb4;*fifo=float_0_80422c80;*fifo=float_0_80422c80;*fifo=float_1_80422c98;
    } else {
        GXBegin(0x80, 0, 4);
        *fifo=float_neg2_80422cb4;*fifo=float_2_80422c94;*fifo=float_0_80422c80;*fifo=float_0_80422c80;*fifo=float_0_80422c80;
        *fifo=float_2_80422c94;*fifo=float_2_80422c94;*fifo=float_0_80422c80;*fifo=float_2_80422c94;*fifo=float_0_80422c80;
        *fifo=float_2_80422c94;*fifo=float_neg2_80422cb4;*fifo=float_0_80422c80;*fifo=float_1_80422c98;*fifo=float_1_80422c98;
        *fifo=float_neg2_80422cb4;*fifo=float_neg2_80422cb4;*fifo=float_0_80422c80;*fifo=float_0_80422c80;*fifo=float_1_80422c98;
    }
}

