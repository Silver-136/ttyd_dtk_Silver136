#include "effect/eff_itemget.h"

void effItemUseDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];

    extern void* camGetPtr(s32 cameraId);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 compareEnable, s32 func, s32 updateEnable);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXRotRad(Mtx m, double angle, char axis);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetNumTexGens(s32 num);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetNumChans(s32 num);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);

    extern f32 float_deg2rad_804270e4;
    extern f32 float_0p4_804270e8;
    extern f32 float_21p333_804270ec;
    extern f32 float_42p667_804270f0;
    extern f32 float_2_804270f4;
    extern f32 float_0p5_804270f8;
    extern f32 float_0_804270fc;
    extern f32 float_10p667_80427100;
    extern f32 float_1_80427104;
    extern u32 unk_804298a0;
    extern u32 dat_804270e0;

    void* cam;
    void* cam2;
    s32 work;
    s32 child;
    s32 i;
    s32 pass;
    u32 color;
    s32 alpha;
    u8 texObj[0x20];
    Mtx baseTrans;
    Mtx rotY;
    Mtx scaleMtx;
    Mtx baseMtx;
    Mtx localMtx;
    f32 left;
    f32 bottom;
    f32 top;
    f32 zero;
    f32 one;
    f32 two;
    volatile f32* fifo;

    work = *(s32*)((s32)effect + 0xC);
    cam = camGetPtr(cameraId);

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);

    PSMTXTrans(
        baseTrans,
        (double)*(f32*)(work + 4),
        (double)*(f32*)(work + 8),
        (double)*(f32*)(work + 0xC));

    cam2 = camGetPtr(4);
    PSMTXRotRad(rotY, (double)(float_deg2rad_804270e4 * -*(f32*)((s32)cam2 + 0x114)), 'y');
    PSMTXScale(scaleMtx, float_0p4_804270e8, float_0p4_804270e8, float_0p4_804270e8);

    PSMTXConcat(baseTrans, scaleMtx, baseMtx);
    PSMTXConcat(baseMtx, rotY, baseMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), baseMtx, baseMtx);

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    effGetTexObj(0x2B, texObj);
    GXLoadTexObj(texObj, 0);

    two = float_2_804270f4;
    zero = float_0_804270fc;
    one = float_1_80427104;
    top = float_10p667_80427100;
    bottom = -float_42p667_804270f0;
    left = -float_21p333_804270ec * float_0p5_804270f8;
    fifo = (volatile f32*)0xCC008000;

    child = work + 0x3C;
    i = 1;
    while (i < *(s32*)((s32)effect + 8)) {
        pass = 0;
        do {
            if (pass == 0) {
                PSMTXTrans(
                    localMtx,
                    (double)(two + *(f32*)(child + 4)),
                    (double)(*(f32*)(child + 8) - two),
                    (double)*(f32*)(child + 0xC));

                PSMTXRotRad(rotY, (double)*(f32*)(child + 0x30), 'z');
                PSMTXConcat(localMtx, rotY, localMtx);
                PSMTXConcat(baseMtx, localMtx, localMtx);

                color = unk_804298a0;
                alpha = (*(s32*)(child + 0x34) << 6) / 0xFF;
                ((u8*)&color)[3] = (u8)alpha;
                GXSetChanMatColor(4, &color);
            } else {
                PSMTXTrans(
                    localMtx,
                    (double)*(f32*)(child + 4),
                    (double)*(f32*)(child + 8),
                    (double)*(f32*)(child + 0xC));

                PSMTXRotRad(rotY, (double)*(f32*)(child + 0x30), 'z');
                PSMTXConcat(localMtx, rotY, localMtx);
                PSMTXConcat(baseMtx, localMtx, localMtx);

                color = dat_804270e0;
                alpha = (*(s32*)(child + 0x34) << 7) / 0xFF;
                ((u8*)&color)[3] = (u8)alpha;
                GXSetChanMatColor(4, &color);
            }

            GXLoadPosMtxImm(localMtx, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x80, 0, 4);

            *fifo = left;
            *fifo = zero;
            *fifo = zero;
            *fifo = zero;
            *fifo = zero;

            *fifo = top;
            *fifo = zero;
            *fifo = zero;
            *fifo = two;
            *fifo = zero;

            *fifo = top;
            *fifo = bottom;
            *fifo = zero;
            *fifo = two;
            *fifo = one;

            *fifo = left;
            *fifo = bottom;
            *fifo = zero;
            *fifo = zero;
            *fifo = one;

            pass++;
        } while (pass < 2);

        i++;
        child += 0x3C;
    }
}

void effItemGetMain(void* effect) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern u32 vec3_802fe6f0[];
    extern u8 scale_data[];
    extern f32 float_100_80427124;
    extern f32 float_1p1_80427128;
    extern f32 float_0p9_8042712c;
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* dispFunc, void* effect, f32 z);
    extern void effItemUseDisp(s32 cameraId, void* effect);
    extern void effItemGetDisp(s32 cameraId, void* effect);

    s32 work;
    s32 child;
    s32 i;
    s32 goneCount;
    s32 timer;
    s32 alpha;
    s32 type;
    Vec base;
    Vec pos;
    f32 z;

    work = *(s32*)((s32)effect + 0xC);

    base.x = ((f32*)vec3_802fe6f0)[0];
    base.y = ((f32*)vec3_802fe6f0)[1];
    base.z = ((f32*)vec3_802fe6f0)[2];

    base.x = *(f32*)(work + 4);
    base.y = *(f32*)(work + 8);
    base.z = *(f32*)(work + 0xC);

    pos.x = base.x;
    pos.y = base.y;
    pos.z = base.z;

    type = *(s32*)work;

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4U;
        *(s32*)(work + 0x18) = 0x10;
    }

    timer = *(s32*)(work + 0x18);
    if (timer < 1000) {
        timer--;
        *(s32*)(work + 0x18) = timer;
    }

    timer = *(s32*)(work + 0x18);
    if (timer < 0x10) {
        *(s32*)(work + 0x34) = timer << 4;
    }

    if (*(s32*)(work + 0x18) < 0) {
        effDelete(effect);
        return;
    }

    *(s32*)(work + 0x1C) = *(s32*)(work + 0x1C) + 1;

    if (*(u32*)(work + 0x1C) < 0xC) {
        *(f32*)(work + 0x10) = (f32)scale_data[*(s32*)(work + 0x1C)] / float_100_80427124;
    }

    if (*(s32*)(work + 0x1C) > 1000) {
        *(s32*)(work + 0x1C) = 1000;
    }

    if (*(s32*)(work + 0x20) == 1) {
        if (*(f32*)(work + 0x10) < float_100_80427124) {
            *(f32*)(work + 0x10) = *(f32*)(work + 0x10) * float_1p1_80427128;
        }
    } else if (*(s32*)(work + 0x20) == 2) {
        *(f32*)(work + 0x10) = *(f32*)(work + 0x10) * float_0p9_8042712c;
    }

    goneCount = 0;
    i = 1;
    child = work + 0x3C;

    while (i < *(s32*)((s32)effect + 8)) {
        *(s32*)(child + 0x1C) = *(s32*)(child + 0x1C) + 1;

        if (*(s32*)(child + 0x1C) < 0x10) {
            alpha = *(s32*)(child + 0x34) + 0x10;
            *(s32*)(child + 0x34) = alpha;
            if (*(s32*)(child + 0x34) > 0xFF) {
                *(s32*)(child + 0x34) = 0xFF;
            }
        } else {
            alpha = *(s32*)(child + 0x34) - 8;
            *(s32*)(child + 0x34) = alpha;
            if (*(s32*)(child + 0x34) < 0) {
                *(s32*)(child + 0x34) = 0;
            }
        }

        if (*(s32*)(child + 0x1C) > 1000) {
            *(s32*)(child + 0x1C) = 1000;
        }

        if (*(s32*)(child + 0x34) == 0) {
            goneCount++;
        } else {
            *(f32*)(child + 4) = *(f32*)(child + 4) + *(f32*)(child + 0x24);
            *(f32*)(child + 8) = *(f32*)(child + 8) + *(f32*)(child + 0x28);
            *(f32*)(child + 0xC) = *(f32*)(child + 0xC) + *(f32*)(child + 0x2C);

            if (type == 0xA) {
                *(f32*)(child + 0x24) = *(f32*)(child + 0x24) * float_0p9_8042712c;
                *(f32*)(child + 0x28) = *(f32*)(child + 0x28) * float_0p9_8042712c;
                *(f32*)(child + 0x2C) = *(f32*)(child + 0x2C) * float_0p9_8042712c;
            }
        }

        i++;
        child += 0x3C;
    }

    if (type == 0xA) {
        if (goneCount >= (*(s32*)((s32)effect + 8) - 1)) {
            effDelete(effect);
            return;
        }

        z = dispCalcZ(&pos);
        dispEntry(*(s32*)(work + 0x38), 2, effItemUseDisp, effect, z);
    } else {
        z = dispCalcZ(&pos);
        dispEntry(*(s32*)(work + 0x38), 2, effItemGetDisp, effect, z);
    }
}

void* effItemGetEntry(s32 type, double x, double y, double z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern double sin(double x);
    extern double cos(double x);
    extern const char str_ItemGet_802fe728[];
    extern void effItemGetMain(void* effect);
    extern u8 size_tbl[];
    extern f64 double_to_int_mask_802fe720;
    extern f64 double_to_int_802fe730;
    extern f32 float_0_804270fc;
    extern f32 float_10_80427134;
    extern f32 float_0p75_80427130;
    extern f32 float_6p2832_80427108;
    extern f32 float_6_80427110;
    extern f32 float_1p0472_80427138;
    extern f32 float_5_8042713c;

    void* entry;
    s32 work;
    s32 child;
    s32 count;
    s32 i;
    s32 typeValue;
    f32 zero;
    f32 angle;
    f32 size;
    f32 speed;

    typeValue = type;
    entry = effEntry();

    count = 1;
    if (typeValue == 0 || typeValue == 3 || typeValue == 9) {
        count = 8;
    } else if (typeValue == 0xA) {
        count = 6;
    }

    *(const char**)((s32)entry + 0x14) = str_ItemGet_802fe728;
    *(s32*)((s32)entry + 8) = count;

    work = (s32)__memAlloc(3, *(s32*)((s32)entry + 8) * 0x3C);
    *(s32*)((s32)entry + 0xC) = work;
    *(void (**)(void))((s32)entry + 0x10) = (void (*)(void))effItemGetMain;

    *(u32*)entry = *(u32*)entry | 2U;

    zero = float_0_804270fc;

    *(s32*)(work + 0x00) = typeValue;
    *(f32*)(work + 0x04) = (f32)x;
    *(f32*)(work + 0x08) = (f32)y;
    *(f32*)(work + 0x0C) = (f32)z;
    *(f32*)(work + 0x10) = zero;

    size = (f32)size_tbl[*(s32*)work] / float_10_80427134;
    *(f32*)(work + 0x14) = float_0p75_80427130 * size;

    *(s32*)(work + 0x1C) = 0;
    *(s32*)(work + 0x20) = 0;
    *(s32*)(work + 0x38) = 4;
    *(s32*)(work + 0x34) = 0xFF;
    *(s32*)(work + 0x18) = 1000;

    child = work + 0x3C;
    i = 1;
    while (i < *(s32*)((s32)entry + 8)) {
        *(f32*)(child + 0x04) = zero;
        *(f32*)(child + 0x08) = zero;
        *(f32*)(child + 0x0C) = zero;

        if (typeValue == 0xA) {
            angle = float_1p0472_80427138 +
                (float_6p2832_80427108 * (f32)(i - 1)) / float_6_80427110;

            *(f32*)(child + 0x30) = angle;

            speed = float_5_8042713c;
            *(f32*)(child + 0x24) = speed * (f32)sin((double)*(f32*)(child + 0x30));
            *(f32*)(child + 0x28) = -(speed * (f32)cos((double)*(f32*)(child + 0x30)));
            *(f32*)(child + 0x2C) = zero;
        } else {
            angle = (float_6p2832_80427108 * (f32)(i - 1)) /
                (f32)(*(s32*)((s32)entry + 8) - 1);

            *(f32*)(child + 0x30) = angle;
            *(f32*)(child + 0x24) = (f32)sin((double)*(f32*)(child + 0x30));
            *(f32*)(child + 0x28) = -(f32)cos((double)*(f32*)(child + 0x30));
            *(f32*)(child + 0x2C) = zero;
        }

        *(s32*)(child + 0x34) = 0;
        *(s32*)(child + 0x1C) = 0;

        i++;
        child += 0x3C;
    }

    return entry;
}


/* CHATGPT STUB FILL: main/effect/eff_itemget 20260624_184128 */

/* stub-fill: effItemGetDisp | prototype_only | source_prototype */
void effItemGetDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct VecLocal { f32 x, y, z; } VecLocal;
    typedef struct ColorLocal { u8 r, g, b, a; } ColorLocal;
    typedef u8 TexObj[0x20];
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(Mtx m, f64 x, f64 y, f64 z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx m, f64 angle, char axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetBlendMode(s32,s32,s32,s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32,s32,s32,s32,s32);
    extern void GXSetZMode(s32,s32,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevOp(s32,s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetChanMatColor(s32, ColorLocal*);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*,s32);
    extern void GXLoadPosMtxImm(void*,s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32,s32,s32);
    extern volatile f32 DAT_cc008000;
    u8* work = *(u8**)((u8*)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    Mtx trans, rot, scale, model;
    TexObj tex;
    ColorLocal color;
    f32 size = *(f32*)(work + 0x14) * *(f32*)(work + 0x18);
    f32 halfW;
    f32 top = 32.0f;
    s32 pass;
    s32 i;

    GXSetBlendMode(1,4,5,0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7,0,0,7,0);
    GXSetZMode(1,3,0);
    PSMTXTrans(trans, *(f32*)(work+4), *(f32*)(work+8), *(f32*)(work+0xC));
    PSMTXRotRad(rot, -0.017453292f * *(f32*)((u8*)camGetPtr(cameraId)+0x114), 'y');
    PSMTXScale(scale, size, size, size);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scale, model);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,0,0x1E,0,0x7D);
    GXSetTexCoordGen2(1,1,4,0x3C,0,0x7D);
    GXSetNumTevStages(2);
    GXSetTevOrder(0,0,0,0xFF);
    GXSetTevOrder(1,1,1,4);
    effGetTexObj(0x2C, tex); GXLoadTexObj(tex,0);
    effGetTexObj(0x2A, tex); GXLoadTexObj(tex,1);
    GXSetNumChans(1);
    GXSetChanCtrl(4,0,0,0,0,0,2);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9,1); GXSetVtxDesc(13,1);
    GXSetVtxAttrFmt(0,9,1,4,0); GXSetVtxAttrFmt(0,13,1,4,0);
    halfW = -32.0f;
    for (pass=0; pass<2; pass++) {
        PSMTXConcat((u8*)camera+0x11C, model, trans);
        color.r = color.g = color.b = 0xFF;
        color.a = (u8)((*(u32*)(work+0x30) * (pass ? 255 : 128)) / 255);
        GXSetChanMatColor(4,&color);
        GXLoadPosMtxImm(trans,0); GXSetCurrentMtx(0); GXBegin(0x80,0,4);
        DAT_cc008000=halfW; DAT_cc008000=top; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
        DAT_cc008000=top; DAT_cc008000=top; DAT_cc008000=0.0f; DAT_cc008000=2.0f; DAT_cc008000=0.0f;
        DAT_cc008000=top; DAT_cc008000=halfW; DAT_cc008000=0.0f; DAT_cc008000=2.0f; DAT_cc008000=2.0f;
        DAT_cc008000=halfW; DAT_cc008000=halfW; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=2.0f;
    }
    effGetTexObj(0x2B, tex); GXLoadTexObj(tex,1);
    for (i=1; i<*(s32*)((u8*)effect+8); i++) {
        u8* child = work + i*0x38;
        for (pass=0; pass<2; pass++) {
            PSMTXTrans(trans, *(f32*)(child+4), *(f32*)(child+8), *(f32*)(child+0xC));
            PSMTXConcat(model,trans,trans);
            PSMTXRotRad(rot,*(f32*)(child+0x1C),'z');
            PSMTXConcat(trans,rot,trans);
            color.r=color.g=color.b=0xFF; color.a=(u8)*(s32*)(child+0x30);
            GXSetChanMatColor(4,&color);
            PSMTXConcat((u8*)camera+0x11C,trans,trans);
            GXLoadPosMtxImm(trans,0); GXSetCurrentMtx(0); GXBegin(0x80,0,4);
            DAT_cc008000=-5.3333f; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=0.0f;
            DAT_cc008000=5.3333f; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=2.0f; DAT_cc008000=0.0f;
            DAT_cc008000=5.3333f; DAT_cc008000=-42.667f; DAT_cc008000=0.0f; DAT_cc008000=2.0f; DAT_cc008000=1.0f;
            DAT_cc008000=-5.3333f; DAT_cc008000=-42.667f; DAT_cc008000=0.0f; DAT_cc008000=0.0f; DAT_cc008000=1.0f;
        }
    }
}

