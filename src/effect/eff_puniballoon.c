#include "effect/eff_puniballoon.h"

void effPuniBalloonDisp(void* camera, void* effect) {
    extern void balloon_0(void* camera, void* effect);
    extern void balloon_1(void* camera, void* effect);

    void* work = *(void**)((s32)effect + 0xC);

    if (*(s32*)work == 0) {
        balloon_0(camera, effect);
    } else {
        balloon_1(camera, effect);
    }
}

void balloon_0(void* camera, void* effect) {
    typedef f32 Mtx[3][4];

    extern void* camGetPtr(s32 cameraId);
    extern double sin(double x);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXRotRad(Mtx m, double angle, char axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetNumChans(s32 num);
    extern void GXSetNumTexGens(s32 num);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevKAlphaSel(s32 stage, s32 sel);
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern void GXSetNumIndStages(s32 num);
    extern void GXSetTevDirect(s32 stage);

    extern f64 double_to_int_802fe558;
    extern f32 float_deg2rad_80426f40;
    extern f32 float_10_80426f44;
    extern f32 float_0p1_80426f48;

    void* cam;
    void* cam2;
    s32 work;
    Mtx trans;
    Mtx rotY;
    Mtx rotZ;
    Mtx scaleMtx;
    Mtx concatA;
    f32 drawScale;
    f32 sinValue;
    f32 angle;
    u8 texObj[0x20];
    volatile u16* fifo;
    s32 frame;

    work = *(s32*)((s32)effect + 0xC);
    cam = camGetPtr((s32)camera);

    frame = *(s32*)(work + 0x2C);
    sinValue = (f32)sin((double)((f32)frame / float_10_80426f44));
    drawScale = (float_0p1_80426f48 * sinValue + *(f32*)(work + 0x20)) * *(f32*)(work + 0x1C);

    PSMTXTrans(
        trans,
        (double)*(f32*)(work + 4),
        (double)*(f32*)(work + 8),
        (double)*(f32*)(work + 0xC));

    cam2 = camGetPtr((s32)camera);
    angle = -*(f32*)((s32)cam2 + 0x114);
    PSMTXRotRad(rotY, (double)(float_deg2rad_80426f40 * angle), 'y');

    PSMTXConcat(trans, rotY, concatA);
    PSMTXConcat((void*)((s32)cam + 0x11C), concatA, trans);

    PSMTXRotRad(rotZ, (double)(float_deg2rad_80426f40 * *(f32*)(work + 0x24)), 'z');
    PSMTXScale(scaleMtx, drawScale, drawScale, drawScale);

    PSMTXConcat(trans, rotZ, trans);
    PSMTXConcat(trans, scaleMtx, trans);

    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 0);

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 4, 6, 7);
    GXSetTevKAlphaSel(0, 4);

    effGetTexObj(0x26, texObj);
    GXLoadTexObj(texObj, 0);

    GXBegin(0x80, 0, 4);

    fifo = (volatile u16*)0xCC008000;

    *fifo = (u16)-0xC;
    *fifo = 0xC;
    *fifo = 0;
    *fifo = 0;
    *fifo = 0;

    *fifo = (u16)-0xC;
    *fifo = (u16)-0xC;
    *fifo = 0;
    *fifo = 0;
    *fifo = 1;

    *fifo = 0xC;
    *fifo = (u16)-0xC;
    *fifo = 0;
    *fifo = 2;
    *fifo = 1;

    *fifo = 0xC;
    *fifo = 0xC;
    *fifo = 0;
    *fifo = 2;
    *fifo = 0;

    GXSetNumIndStages(0);
    GXSetTevDirect(0);
}

void balloon_1(void* camera, void* effect) {
    typedef f32 Mtx[3][4];

    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXRotRad(Mtx m, double angle, char axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXSetNumChans(s32 num);
    extern void GXSetNumTexGens(s32 num);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevColorOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevAlphaOp(s32 stage, s32 op, s32 bias, s32 scale, s32 clamp, s32 outReg);
    extern void GXSetTevColorIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevAlphaIn(s32 stage, s32 a, s32 b, s32 c, s32 d);
    extern void GXSetTevKAlphaSel(s32 stage, s32 sel);
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);

    extern f32 float_deg2rad_80426f40;

    s32 work;
    s32 child;
    s32 i;
    void* cam;
    void* cam2;
    Mtx trans;
    Mtx rotY;
    Mtx viewMtx;
    Mtx scaleMtx;
    u8 texObj[0x20];
    f32 scale;
    f32 angle;
    volatile u16* fifo;

    work = *(s32*)((s32)effect + 0xC);
    cam = camGetPtr((s32)camera);

    PSMTXTrans(
        trans,
        (double)*(f32*)(work + 4),
        (double)*(f32*)(work + 8),
        (double)*(f32*)(work + 0xC));

    cam2 = camGetPtr((s32)camera);
    angle = -*(f32*)((s32)cam2 + 0x114);
    PSMTXRotRad(rotY, (double)(float_deg2rad_80426f40 * angle), 'y');

    PSMTXConcat(trans, rotY, viewMtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), viewMtx, viewMtx);

    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 4, 6, 7);
    GXSetTevKAlphaSel(0, 4);

    effGetTexObj(0x26, texObj);
    GXLoadTexObj(texObj, 0);

    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 3, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 3, 0);

    i = 1;
    child = work + 0x30;
    while (i < *(s32*)((s32)effect + 8)) {
        PSMTXTrans(
            trans,
            (double)*(f32*)(child + 4),
            (double)*(f32*)(child + 8),
            (double)*(f32*)(child + 0xC));

        scale = *(f32*)(child + 0x20);
        PSMTXScale(scaleMtx, scale, scale, scale);
        PSMTXConcat(trans, scaleMtx, trans);
        PSMTXConcat(viewMtx, trans, trans);

        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 4);

        fifo = (volatile u16*)0xCC008000;

        *fifo = (u16)-0xC;
        *fifo = 0xC;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;

        *fifo = (u16)-0xC;
        *fifo = (u16)-0xC;
        *fifo = 0;
        *fifo = 0;
        *fifo = 1;

        *fifo = 0xC;
        *fifo = (u16)-0xC;
        *fifo = 0;
        *fifo = 2;
        *fifo = 1;

        *fifo = 0xC;
        *fifo = 0xC;
        *fifo = 0;
        *fifo = 2;
        *fifo = 0;

        i++;
        child += 0x30;
    }
}

void* effPuniBalloonEntry(s32 type, double x, double y, double z, double scale) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 rand(void);
    extern const char str_PuniBalloon_802fe560[];
    extern void effPuniBalloonMain(void);
    extern f32 float_0_80426f60;
    extern f64 double_to_int_802fe558;

    void* entry;
    s32 work;
    s32 child;
    s32 count;
    s32 i;
    s32 r;
    s32 q;
    s32 rem;
    f32 zero;
    f64 conv;

    entry = effEntry();

    count = 1;
    if (type == 1) {
        count = 0xB;
    } else if (type < 1) {
        if (type >= 0) {
            count = 1;
        }
    }

    *(const char**)((s32)entry + 0x14) = str_PuniBalloon_802fe560;
    *(s32*)((s32)entry + 8) = count;

    work = (s32)__memAlloc(3, *(s32*)((s32)entry + 8) * 0x30);
    *(s32*)((s32)entry + 0xC) = work;
    *(void (**)(void))((s32)entry + 0x10) = effPuniBalloonMain;

    zero = float_0_80426f60;
    *(u32*)entry = *(u32*)entry | 2;

    *(s32*)work = type;
    *(f32*)(work + 4) = (f32)x;
    *(f32*)(work + 8) = (f32)y;
    *(f32*)(work + 0xC) = (f32)z;
    *(f32*)(work + 0x24) = zero;

    if (type == 0) {
        *(s32*)(work + 0x28) = 1000;
    } else {
        *(s32*)(work + 0x28) = 0x1E;
    }

    *(s32*)(work + 0x2C) = 0;
    *(f32*)(work + 0x20) = zero;
    *(f32*)(work + 0x1C) = (f32)scale;

    conv = double_to_int_802fe558;
    child = work + 0x30;
    i = 1;

    while (i < *(s32*)((s32)entry + 8)) {
        *(f32*)(child + 4) = zero;
        *(f32*)(child + 8) = zero;
        *(f32*)(child + 0xC) = zero;
        *(f32*)(child + 0x20) = (f32)scale;

        r = rand();
        q = r / 11;
        if (r < 0 && (r - q * 11) != 0) {
            q--;
        }
        rem = r - q * 11;
        *(f32*)(child + 0x10) = (f32)(rem - 5);

        r = rand();
        q = r / 11;
        if (r < 0 && (r - q * 11) != 0) {
            q--;
        }
        rem = r - q * 11;
        *(f32*)(child + 0x14) = (f32)(rem - 5);

        r = rand();
        q = r / 11;
        if (r < 0 && (r - q * 11) != 0) {
            q--;
        }
        rem = r - q * 11;
        *(f32*)(child + 0x18) = (f32)(rem - 5);

        i++;
        child += 0x30;
    }

    return entry;
}

void effPuniBalloonMain(void* effect) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern u32 vec3_802fe548[];
    extern f32 float_1_80426f4c;
    extern f32 float_10_80426f44;
    extern f32 float_20_80426f50;
    extern f32 float_neg0p1_80426f54;
    extern f32 float_0p98_80426f58;
    extern f32 float_0p9_80426f5c;
    extern f64 double_to_int_802fe558;
    extern double sin(double x);
    extern void* effPuniBalloonEntry(s32 type, double x, double y, double z, double scale);
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* dispFunc, void* effect, f32 z);
    extern void effPuniBalloonDisp(void* camera, void* effect);

    s32 work;
    s32 child;
    s32 i;
    Vec base;
    Vec pos;
    f32 scale;
    f32 target;
    f32 frame;
    f32 wave;
    f32 one;
    f32 twenty;
    f32 neg;
    f32 damp;
    f32 shrink;
    f32 zOrder;

    work = *(s32*)((s32)effect + 0xC);

    base.x = ((f32*)vec3_802fe548)[0];
    base.y = ((f32*)vec3_802fe548)[1];
    base.z = ((f32*)vec3_802fe548)[2];

    base.x = *(f32*)(work + 4);
    base.y = *(f32*)(work + 8);
    base.z = *(f32*)(work + 0xC);

    pos.x = base.x;
    pos.y = base.y;
    pos.z = base.z;

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect = *(u32*)effect & ~4;
        *(s32*)(work + 0x28) = 0x10;
    }

    if (*(s32*)(work + 0x28) < 0x3E8) {
        *(s32*)(work + 0x28) = *(s32*)(work + 0x28) - 1;
    }

    *(s32*)(work + 0x2C) = *(s32*)(work + 0x2C) + 1;

    if (*(s32*)(work + 0x28) < 0) {
        if (*(s32*)work == 0) {
            one = float_1_80426f4c;
            effPuniBalloonEntry(
                1,
                (double)*(f32*)(work + 4),
                (double)*(f32*)(work + 8),
                (double)*(f32*)(work + 0xC),
                (double)one);
        }

        effDelete(effect);
        return;
    }

    one = float_1_80426f4c;
    scale = *(f32*)(work + 0x20);
    target = one - scale;
    scale = scale + target / float_10_80426f44;
    *(f32*)(work + 0x20) = scale;

    twenty = float_20_80426f50;
    frame = (f32)*(s32*)(work + 0x2C);
    wave = twenty * (f32)sin((double)(frame / twenty));
    *(f32*)(work + 0x24) = wave;

    neg = float_neg0p1_80426f54;
    damp = float_0p98_80426f58;
    shrink = float_0p9_80426f5c;

    i = 1;
    child = work + 0x30;

    while (i < *(s32*)((s32)effect + 8)) {
        *(f32*)(child + 4) = *(f32*)(child + 4) + *(f32*)(child + 0x10);
        *(f32*)(child + 8) = *(f32*)(child + 8) + *(f32*)(child + 0x14);
        *(f32*)(child + 0xC) = *(f32*)(child + 0xC) + *(f32*)(child + 0x18);

        *(f32*)(child + 0x14) = *(f32*)(child + 0x14) + neg;

        *(f32*)(child + 0x10) = *(f32*)(child + 0x10) * damp;
        *(f32*)(child + 0x14) = *(f32*)(child + 0x14) * damp;
        *(f32*)(child + 0x18) = *(f32*)(child + 0x18) * damp;
        *(f32*)(child + 0x20) = *(f32*)(child + 0x20) * shrink;

        i++;
        child += 0x30;
    }

    zOrder = dispCalcZ(&pos);
    dispEntry(4, 2, effPuniBalloonDisp, effect, zOrder);
}
