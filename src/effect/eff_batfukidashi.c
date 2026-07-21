#include "effect/eff_batfukidashi.h"

typedef struct Vec {
    f32 x;
    f32 y;
    f32 z;
} Vec;

typedef struct EffCoinFukidashiWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    f32 scale;
    s32 item;
    f32 unk18;
    s32 alpha;
    s32 timer;
    s32 counter;
} EffCoinFukidashiWork;

extern const Vec vec3_802fddb0;
extern u8 offset_tbl[];
extern const char str_CoinFukidashi_802fddc8[];
extern f32 float_0_804267d8;
extern f32 float_1_804267dc;
extern f32 float_100_804267f4;

void effCoinFukidashiDisp(s32 cameraId, void* effect);
void effCoinFukidashiMain(void* effect);
void* effEntry(s32 type, s32 item, f32 x, f32 y, f32 z, s32 timer);
void effDelete(void* effect);
void* __memAlloc(s32 heap, u32 size);
f32 dispCalcZ(Vec* pos);
void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 priority);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effCoinFukidashiEntry(s32 type, s32 item, f32 x, f32 y, f32 z, s32 timer) {
    void* effect = effEntry(type, item, x, y, z, timer);
    EffCoinFukidashiWork* work;
    f32 zero;
    f32 one;

    *(const char**)((s32)effect + 0x14) = str_CoinFukidashi_802fddc8;
    *(s32*)((s32)effect + 0x8) = 1;
    work = __memAlloc(3, *(s32*)((s32)effect + 0x8) * 0x28);
    *(void**)((s32)effect + 0xC) = work;
    zero = float_0_804267d8;
    *(void**)((s32)effect + 0x10) = effCoinFukidashiMain;
    one = float_1_804267dc;
    *(u32*)effect |= 2;

    work->type = type;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = zero;
    work->unk18 = one;
    work->item = item;
    if (timer <= 0) {
        work->timer = 1000;
    } else {
        work->timer = timer;
    }
    work->counter = 0;
    work->alpha = 0;
    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void effCoinFukidashiMain(void* effect) {
    Vec dispPos;
    Vec pos = vec3_802fddb0;
    EffCoinFukidashiWork* work = *(EffCoinFukidashiWork**)((s32)effect + 0xC);
    u32 flags;
    s32 timer;
    s32 alpha;

    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(u32*)effect;
    if ((flags & 4) != 0) {
        *(u32*)effect = flags & ~4;
        work->timer = 0x10;
    }

    timer = work->timer;
    if (timer < 1000) {
        work->timer = timer - 1;
    }

    if (work->timer < 0x10) {
        work->alpha = work->timer << 4;
    } else {
        alpha = work->alpha + 0x10;
        work->alpha = alpha;
        if (work->alpha > 0xFF) {
            work->alpha = 0xFF;
        }
    }

    if (work->timer < 0) {
        effDelete(effect);
        return;
    }

    work->scale = (f32)offset_tbl[work->counter] / float_100_804267f4;
    work->counter++;
    if ((u32)work->counter >= 0x3D) {
        work->counter = 0;
    }
    dispEntry(4, 2, effCoinFukidashiDisp, effect, dispCalcZ(&dispPos));
}
void effCoinFukidashiDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];

    extern void* camGetPtr(s32 cameraId);
    extern void GXSetNumTexGens(s32 num);
    extern void GXSetTexCoordGen2(s32 dstCoord, s32 func, s32 srcParam, s32 mtx, s32 normalize, s32 postMtx);
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetNumChans(s32 num);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTevStages(s32 num);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void PSMTXTrans(Mtx m, double x, double y, double z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx m, double angle, char axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 compCnt, s32 compType, s32 frac);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern u16 GXGetTexObjWidth(void* texObj);
    extern u16 GXGetTexObjHeight(void* texObj);
    extern int sprintf(char* dst, const char* fmt, ...);
    extern u16 FontGetMessageWidth(char* msg);
    extern void FontDrawStart_alpha(u8 alpha);
    extern void GXSetZMode(s32 compareEnable, s32 func, s32 updateEnable);
    extern void FontDrawStringMtx(void* mtx, char* str);

    extern u32 dat_804267c8;
    extern f32 float_deg2rad_804267cc;
    extern f32 float_0p4_804267d0;
    extern f32 float_0p5_804267d4;
    extern f32 float_0_804267d8;
    extern f32 float_1_804267dc;
    extern const char str_PCTd_804267e0[];
    extern f32 float_3_804267e4;
    extern f32 float_neg28_804267e8;
    extern f32 float_28_804267ec;
    extern f32 float_34_804267f0;

    void* cam;
    void* cam2;
    s32 work;
    u32 colorA;
    u32 colorB;
    u8 texObj[0x20];
    char text[0x40];
    Mtx trans;
    Mtx scaleMtx;
    Mtx rotY;
    f32 scale;
    f32 width;
    f32 height;
    f32 halfWidth;
    f32 negHalfWidth;
    f32 textWidth;
    f32 textScale;
    f32 zero;
    f32 one;
    volatile f32* fifo;

    cam = camGetPtr(cameraId);
    work = *(s32*)((s32)effect + 0xC);

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    effGetTexObj(0x25, texObj);
    GXLoadTexObj(texObj, 0);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);

    colorA = dat_804267c8;
    ((u8*)&colorA)[3] = *(u8*)(work + 0x1C);
    colorB = colorA;
    GXSetChanMatColor(4, &colorB);

    PSMTXTrans(
        trans,
        (double)*(f32*)(work + 4),
        (double)(*(f32*)(work + 8) + *(f32*)(work + 0x10)),
        (double)*(f32*)(work + 0xC));

    scale = *(f32*)(work + 0x18);
    PSMTXScale(scaleMtx, scale, scale, scale);

    cam2 = camGetPtr(cameraId);
    PSMTXRotRad(rotY, (double)(float_deg2rad_804267cc * -*(f32*)((s32)cam2 + 0x114)), 'y');

    PSMTXConcat(trans, scaleMtx, trans);
    PSMTXConcat(trans, rotY, trans);
    PSMTXConcat((void*)((s32)cam + 0x11C), trans, trans);

    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    GXBegin(0x80, 0, 4);

    width = float_0p4_804267d0 * (f32)GXGetTexObjWidth(texObj);
    height = float_0p4_804267d0 * (f32)GXGetTexObjHeight(texObj);
    halfWidth = width * float_0p5_804267d4;
    negHalfWidth = -width * float_0p5_804267d4;
    zero = float_0_804267d8;
    one = float_1_804267dc;

    fifo = (volatile f32*)0xCC008000;

    *fifo = negHalfWidth;
    *fifo = height;
    *fifo = zero;
    *fifo = zero;
    *fifo = zero;

    *fifo = halfWidth;
    *fifo = height;
    *fifo = zero;
    *fifo = one;
    *fifo = zero;

    *fifo = halfWidth;
    *fifo = zero;
    *fifo = zero;
    *fifo = one;
    *fifo = one;

    *fifo = negHalfWidth;
    *fifo = zero;
    *fifo = zero;
    *fifo = zero;
    *fifo = one;

    sprintf(text, str_PCTd_804267e0, *(s32*)(work + 0x14));

    textWidth = (*(f32*)(work + 0x18) * (f32)FontGetMessageWidth(text)) / float_3_804267e4;

    PSMTXTrans(
        trans,
        (double)(-textWidth * float_0p5_804267d4),
        (double)float_neg28_804267e8,
        (double)zero);

    textScale = *(f32*)(work + 0x18) / float_3_804267e4;
    PSMTXScale(scaleMtx, textScale, textScale, textScale);
    PSMTXConcat(trans, scaleMtx, scaleMtx);

    PSMTXTrans(
        trans,
        (double)(textWidth * float_0p5_804267d4),
        (double)float_28_804267ec,
        (double)zero);
    PSMTXConcat(scaleMtx, trans, scaleMtx);

    PSMTXTrans(
        trans,
        (double)*(f32*)(work + 4),
        (double)*(f32*)(work + 8),
        (double)*(f32*)(work + 0xC));

    cam2 = camGetPtr(cameraId);
    PSMTXRotRad(rotY, (double)(float_deg2rad_804267cc * -*(f32*)((s32)cam2 + 0x114)), 'y');
    PSMTXConcat(trans, rotY, rotY);

    PSMTXTrans(
        trans,
        (double)float_3_804267e4,
        (double)(float_34_804267f0 + *(f32*)(work + 0x10)),
        (double)zero);

    PSMTXConcat(rotY, trans, trans);
    PSMTXConcat(trans, scaleMtx, trans);

    FontDrawStart_alpha(*(u8*)(work + 0x1C));
    GXSetZMode(1, 3, 0);
    FontDrawStringMtx(trans, text);
}
