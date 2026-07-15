#include "effect/eff_kiss.h"

void effKissDisp2(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct KissWork {
        s32 type;
        f32 x;
        f32 y;
        f32 z;
        f32 vx;
        f32 vy;
        f32 angleRate;
        f32 rot;
        f32 scaleX;
        f32 scaleY;
        s32 timer;
        s32 frame;
        s32 alpha;
        s32 unk34;
    } KissWork;

    extern void* camGetPtr(s32 camId);
    extern void PSMTXTrans(Mtx m, f64 x, f64 y, f64 z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx m, char axis, f32 rad);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetNumChans(s32 n);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 n);
    extern void GXSetTexCoordGen2(s32 coord, s32 func, s32 src, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 n);
    extern void GXSetTevOrder(s32 stage, s32 coord, s32 map, s32 color);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void effGetTexObj(s32 texId, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 count);

    extern u32 dat_80427950;
    extern f32 float_deg2rad_80427954;
    extern f32 float_8_80427958;
    extern f32 float_0p5_80427960;
    extern f32 float_1_8042795c;
    extern f32 float_0_80427964;
    extern f32 float_4_80427968;
    extern f32 float_2_8042796c;

    u8 texObj[0x20];
    Mtx baseMtx;
    Mtx childMtx;
    Mtx rotMtx;
    Mtx scaleMtx;
    KissWork* work;
    KissWork* child;
    void* cam;
    u32 color;
    s32 i;
    f32 height;
    f32 halfWidth;
    f32 width;
    f32 deg;
    volatile f32* fifo;

    work = *(KissWork**)((s32)effect + 0xC);
    cam = camGetPtr(cameraId);

    PSMTXTrans(baseMtx, work->x, work->y, work->z);
    PSMTXScale(scaleMtx, work->scaleX, work->scaleY, work->scaleY);
    PSMTXRotRad(rotMtx, 'y', float_deg2rad_80427954 * -*(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(baseMtx, rotMtx, baseMtx);
    PSMTXConcat(baseMtx, scaleMtx, childMtx);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    effGetTexObj(0x14, texObj);
    GXLoadTexObj(texObj, 0);

    color = dat_80427950;
    *(u8*)((s32)&color + 3) = (u8)work->alpha;
    GXSetChanMatColor(4, &color);

    height = float_8_80427958;
    halfWidth = -height * float_0p5_80427960;
    width = float_4_80427968;
    deg = float_deg2rad_80427954;
    fifo = (volatile f32*)0xCC008000;

    i = 1;
    child = work + 1;
    while (i < *(s32*)((s32)effect + 0x8)) {
        PSMTXTrans(baseMtx, child->x, child->y, child->z);
        PSMTXScale(scaleMtx, child->scaleX, child->scaleY, float_1_8042795c);
        PSMTXRotRad(rotMtx, 'z', deg * child->rot);
        PSMTXConcat(baseMtx, scaleMtx, baseMtx);
        PSMTXConcat(baseMtx, rotMtx, baseMtx);
        PSMTXConcat(childMtx, baseMtx, baseMtx);
        PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);
        GXLoadPosMtxImm(baseMtx, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 4);

        i++;
        child++;

        fifo[0] = halfWidth;
        fifo[0] = height;
        fifo[0] = float_0_80427964;
        fifo[0] = float_0_80427964;
        fifo[0] = float_0_80427964;

        fifo[0] = width;
        fifo[0] = height;
        fifo[0] = float_0_80427964;
        fifo[0] = float_2_8042796c;
        fifo[0] = float_0_80427964;

        fifo[0] = width;
        fifo[0] = float_0_80427964;
        fifo[0] = float_0_80427964;
        fifo[0] = float_2_8042796c;
        fifo[0] = float_1_8042795c;

        fifo[0] = halfWidth;
        fifo[0] = float_0_80427964;
        fifo[0] = float_0_80427964;
        fifo[0] = float_0_80427964;
        fifo[0] = float_1_8042795c;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effKissMain(void* effect) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    typedef struct KissWork {
        s32 type;
        f32 x;
        f32 y;
        f32 z;
        f32 vx;
        f32 vy;
        f32 angleRate;
        f32 rot;
        f32 scaleX;
        f32 scaleY;
        s32 timer;
        s32 frame;
        s32 alpha;
        s32 unk34;
    } KissWork;

    extern Vec vec3_802ff088;
    extern void* effStardustN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale);
    extern void effStardustN64SetScale(void* effect, f32 scale);
    extern void effDelete(void* effect);
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f32 __fabsf(f32 x);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void effKissDisp(s32 cameraId, void* effect);
    extern void effKissDisp2(s32 cameraId, void* effect);

    extern u8 kuri_kiss[];
    extern u8 kura_kiss[];
    extern s16 kuri_kiss_r[];
    extern f32 float_5_80427980;
    extern f32 float_10_80427988;
    extern f32 float_0p5_80427960;
    extern f32 float_0p9_8042798c;
    extern f32 float_6p2832_80427990;
    extern f32 float_0p03125_80427994;
    extern f32 float_1p2_80427998;
    extern f32 float_360_8042799c;
    extern f32 float_0p2_804279a0;
    extern f32 float_9p4248_804279a4;
    extern f32 float_60_804279a8;
    extern f32 float_2_8042796c;
    extern f32 float_12_804279ac;

    KissWork* work;
    KissWork* child;
    Vec pos;
    Vec zpos;
    s32 type;
    s32 timer;
    s32 i;
    f32 angleRate;
    f32 phase;
    f32 tableScale;
    f32 damp;
    void* dust;

    work = *(KissWork**)((s32)effect + 0xC);

    pos = vec3_802ff088;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    zpos = pos;

    type = work->type;
    work->frame++;
    work->timer--;
    angleRate = work->angleRate;

    if (type == 3) {
        work->frame %= 12;
    }

    timer = work->timer;
    if (timer < 0) {
        if (type == 4) {
            child = work + 1;
            dust = effStardustN64Entry(1,
                                       work->x + child->x,
                                       float_5_80427980 + work->y + child->y,
                                       work->z + child->z,
                                       float_10_80427988);
            effStardustN64SetScale(dust, float_0p5_80427960);
        }
        effDelete(effect);
        return;
    }

    if (type != 4 && timer < 0x10) {
        work->alpha = timer * 15;
    }

    child = work + 1;
    switch (type) {
        case 0:
        case 3:
            damp = float_0p9_8042798c;
            for (i = 1; i < *(s32*)((s32)effect + 0x8); i++) {
                child->x += child->vx;
                child->y += child->vy;
                child->vx *= damp;
                child->vy *= damp;
                child++;
            }
            break;

        case 1:
            phase = float_6p2832_80427990 * angleRate;
            for (i = 1; i < *(s32*)((s32)effect + 0x8); i++) {
                if ((u32)child->frame < 0x18) {
                    s32 idx = child->frame;
                    f32 angle = phase / float_360_8042799c;
                    tableScale = ((f32)kuri_kiss[idx * 3] * float_0p03125_80427994) * float_1p2_80427998;
                    child->x = tableScale * (f32)sin(angle);
                    child->y = tableScale * (f32)cos(angle);
                    child->scaleX = (f32)kuri_kiss[idx * 3 + 1] * float_0p03125_80427994;
                    child->scaleY = (f32)kuri_kiss[idx * 3 + 2] * float_0p03125_80427994;
                    child->rot = (f32)kuri_kiss_r[idx];
                    child->frame++;
                }
                child++;
            }
            break;

        case 2:
            phase = float_6p2832_80427990 * angleRate;
            for (i = 1; i < *(s32*)((s32)effect + 0x8); i++) {
                if ((u32)child->frame < 0x27) {
                    s32 idx = child->frame;
                    f32 angle = phase / float_360_8042799c;
                    tableScale = ((f32)kura_kiss[idx * 3] * float_0p03125_80427994) * float_1p2_80427998;
                    child->x = tableScale * (f32)sin(angle);
                    child->y = tableScale * (f32)cos(angle);
                    child->scaleX = (f32)kura_kiss[idx * 3 + 1] * float_0p03125_80427994;
                    child->scaleY = (f32)kura_kiss[idx * 3 + 2] * float_0p03125_80427994;
                    child->frame++;
                }
                child++;
            }
            break;

        case 4:
            child->x += float_0p2_804279a0;
            child->y = float_2_8042796c * __fabsf((f32)sin((float_9p4248_804279a4 * (f32)child->frame) / float_60_804279a8));
            child->rot = float_12_804279ac * (f32)sin((float_9p4248_804279a4 * (f32)child->frame) / float_60_804279a8);
            child->frame++;
            break;

        case 5:
            damp = float_0p9_8042798c;
            for (i = 1; i < *(s32*)((s32)effect + 0x8); i++) {
                child->x += child->vx;
                child->y += child->vy;
                child->vx *= damp;
                child->vy *= damp;
                child++;
            }
            break;
    }

    if (type == 4 || type == 5) {
        dispEntry(4, 2, effKissDisp2, effect, dispCalcZ(&zpos));
    } else {
        dispEntry(4, 2, effKissDisp, effect, dispCalcZ(&zpos));
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



/* CHATGPT STUB FILL: main/effect/eff_kiss 20260624_184823 */

/* stub-fill: effKissDisp | prototype_only | source_prototype */
void effKissDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4]; typedef u8 TexObj[0x20]; typedef struct Color{u8 r,g,b,a;}Color;
    extern void* camGetPtr(s32); extern void PSMTXTrans(Mtx,f64,f64,f64); extern void PSMTXScale(Mtx,f32,f32,f32); extern void PSMTXRotRad(Mtx,f64,char); extern void PSMTXConcat(void*,void*,void*);
    extern void GXSetNumChans(s32);extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetCullMode(s32);extern void GXClearVtxDesc(void);extern void GXSetVtxDesc(s32,s32);extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);extern void GXSetChanMatColor(s32,Color*);
    extern void effGetTexObj(s32,void*);extern void GXLoadTexObj(void*,s32);extern void GXLoadPosMtxImm(void*,s32);extern void GXSetCurrentMtx(s32);extern void GXBegin(s32,s32,s32);extern volatile f32 DAT_cc008000;
    u8* base=*(u8**)((u8*)effect+0xC);void* cam=camGetPtr(cameraId);Mtx root,trans,rot,scale,model;TexObj tex;Color color;s32 type=*(s32*)base;s32 pass;s32 i;u8* child;f32 halfW;f32 top;
    PSMTXTrans(trans,*(f32*)(base+4),*(f32*)(base+8),*(f32*)(base+0xC));PSMTXScale(scale,*(f32*)(base+0x20),*(f32*)(base+0x24),*(f32*)(base+0x24));PSMTXRotRad(rot,-0.017453292f**(f32*)((u8*)camGetPtr(4)+0x114),'y');PSMTXConcat(trans,rot,trans);PSMTXConcat(trans,scale,root);
    GXSetNumChans(1);GXSetChanCtrl(4,0,0,0,0,0,2);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x3C,0,0x7D);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,4);GXSetCullMode(0);GXClearVtxDesc();GXSetVtxDesc(9,1);GXSetVtxDesc(13,1);GXSetVtxAttrFmt(0,9,1,4,0);GXSetVtxAttrFmt(0,13,1,4,0);
    for(pass=0;pass<2;pass++){
        child=base+0x38;
        for(i=1;i<*(s32*)((u8*)effect+8);i++,child+=0x38){
            if(type==3){effGetTexObj(pass?0x58:0x59,tex);halfW=pass?10.0f:16.0f;top=pass?16.0f:20.0f;}else{effGetTexObj(0x14,tex);halfW=4.0f;top=8.0f;}
            GXLoadTexObj(tex,0);color.r=color.g=color.b=0xFF;color.a=(u8)*(s32*)(base+0x30);GXSetChanMatColor(4,&color);
            PSMTXTrans(trans,*(f32*)(child+4),*(f32*)(child+8),*(f32*)(child+0xC));PSMTXRotRad(rot,-0.017453292f**(f32*)(base+0x18),'z');PSMTXConcat(trans,rot,trans);PSMTXRotRad(rot,0.017453292f**(f32*)(child+0x1C),'y');PSMTXConcat(trans,rot,trans);PSMTXScale(scale,*(f32*)(child+0x20),*(f32*)(child+0x24),*(f32*)(child+0x24));PSMTXConcat(trans,scale,trans);PSMTXConcat(root,trans,model);PSMTXConcat((u8*)cam+0x11C,model,model);GXLoadPosMtxImm(model,0);GXSetCurrentMtx(0);GXBegin(0x80,0,4);
            DAT_cc008000=-halfW;DAT_cc008000=top;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=halfW;DAT_cc008000=top;DAT_cc008000=0;DAT_cc008000=1;DAT_cc008000=0;DAT_cc008000=halfW;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=1;DAT_cc008000=1;DAT_cc008000=-halfW;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=0;DAT_cc008000=1;
        }
    }
}

/* stub-fill: effKissEntry | missing_definition | ghidra_signature */
void* effKissEntry(s32 type, f32 x, f32 y, f32 z, f32 angleRate) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* effStampN64Entry(s32 type, f32 x, f32 y, f32 z);
    extern void effKissMain(void* effect);
    extern const char str_kiss_804279b0[];
    extern f64 sin(f64 x);
    extern f64 cos(f64 x);
    extern f32 float_1_8042795c;
    extern f32 float_0_80427964;
    extern f32 float_6p2832_80427990;
    extern f32 float_360_8042799c;
    extern f32 float_3_804279b8;
    extern f32 float_1p5_80427970;
    extern f32 float_neg1_804279bc;

    void* effect = effEntry();
    void* work;
    void* child;
    s32 count = 2;
    s32 i;
    f32 phase = float_6p2832_80427990 * angleRate;

    if (type == 5) {
        effStampN64Entry(2, x, y, z);
        count = 7;
    }

    *(const char**)((s32)effect + 0x14) = str_kiss_804279b0;
    *(s32*)((s32)effect + 8) = count;
    work = __memAlloc(3, count * 0x38);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effKissMain;

    *(s32*)work = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x20) = float_1_8042795c;
    *(f32*)((s32)work + 0x24) = float_1_8042795c;
    *(s32*)((s32)work + 0x28) = (type == 4) ? 0x3C : 0x28;
    *(s32*)((s32)work + 0x2C) = 0;
    *(s32*)((s32)work + 0x30) = 0xFF;
    *(f32*)((s32)work + 0x18) = angleRate;
    *(f32*)((s32)work + 0x1C) = float_0_80427964;

    child = (void*)((s32)work + 0x38);
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, child = (void*)((s32)child + 0x38)) {
        *(f32*)((s32)child + 4) = float_0_80427964;
        *(f32*)((s32)child + 8) = float_0_80427964;
        *(f32*)((s32)child + 0xC) = float_0_80427964;
        *(f32*)((s32)child + 0x1C) = float_0_80427964;
        *(s32*)((s32)child + 0x28) = 0;
        *(s32*)((s32)child + 0x2C) = 0;

        switch (type) {
            case 0:
            case 3: {
                f32 angle = phase / float_360_8042799c;
                *(f32*)((s32)child + 0x20) = float_1_8042795c;
                *(f32*)((s32)child + 0x24) = float_1_8042795c;
                *(f32*)((s32)child + 0x10) = float_3_804279b8 * (f32)sin(angle);
                *(f32*)((s32)child + 0x14) = float_3_804279b8 * (f32)cos(angle);
                break;
            }
            case 1:
            case 2: {
                f32 angle = phase / float_360_8042799c;
                *(f32*)((s32)child + 0x20) = float_0_80427964;
                *(f32*)((s32)child + 0x24) = float_0_80427964;
                *(f32*)((s32)child + 0x10) = float_3_804279b8 * (f32)sin(angle);
                *(f32*)((s32)child + 0x14) = float_3_804279b8 * (f32)cos(angle);
                break;
            }
            case 4:
                *(f32*)((s32)child + 0x20) = float_1_8042795c;
                *(f32*)((s32)child + 0x24) = float_1_8042795c;
                break;
            case 5: {
                f32 span = float_360_8042799c * (f32)i / (f32)(*(s32*)((s32)effect + 8) - 1);
                f32 angle = (float_6p2832_80427990 * span) / float_360_8042799c;
                *(f32*)((s32)child + 0x20) = float_1p5_80427970;
                *(f32*)((s32)child + 0x24) = float_1p5_80427970;
                *(f32*)((s32)child + 0x1C) = span;
                *(f32*)((s32)child + 0x10) = float_3_804279b8 * (f32)sin(angle);
                *(f32*)((s32)child + 0x14) = float_3_804279b8 * (f32)cos(angle);
                *(f32*)((s32)child + 0x1C) *= float_neg1_804279bc;
                break;
            }
        }
    }

    return effect;
}

