#include "effect/n64/eff_horao_spell_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffHoraoSpellWork {
    s32 type;
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    s32 frame;
    s32 colorR;
    s32 colorG;
    s32 colorB;
    s32 alpha;
    s32 color2R;
    s32 color2G;
    s32 color2B;
    s32 color2A;
    f32 scale;
} EffHoraoSpellWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void effHoraoSpellMain(void* effEntry);
void effHoraoSpellDisp(s32 cameraId, void* effect);

extern const Vec3 vec3_802fb120;
extern char str_HoraoSpellN64_802fb138[];


#pragma use_lmw_stmw off
void* effHoraoSpellN64Entry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale) {
    void* entry = effEntry();
    EffHoraoSpellWork* work;
    s32 flags;

    *(char**)((s32)entry + 0x14) = str_HoraoSpellN64_802fb138;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x3C);
    *(EffHoraoSpellWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHoraoSpellMain;
    flags = *(s32*)entry;
    *(s32*)entry = flags | 2;
    work->type = type;
    work->frame = 0;
    if (duration <= 0) {
        work->timer = 1000;
    } else {
        work->timer = duration;
    }
    work->alpha = 0;
    work->x = x;
    work->y = y;
    work->z = z;
    work->scale = scale;
    work->colorR = 70;
    work->colorG = 180;
    work->colorB = 120;
    work->color2R = 20;
    work->color2G = 230;
    work->color2B = 50;
    work->color2A = 0xFF;

    return entry;
}
#pragma use_lmw_stmw on

void effHoraoSpellMain(void* effEntry) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffHoraoSpellWork* work;
    s32 flags;
    s32 frame;
    s32 timer;

    work = *(EffHoraoSpellWork**)((s32)effEntry + 0xC);
    zero = &vec3_802fb120;
    pos = *zero;
    pos.x = work->x;
    pos.y = work->y;
    pos.z = work->z;
    dispPos = pos;

    flags = *(s32*)effEntry;
    if (flags & 4) {
        *(s32*)effEntry = flags & ~4;
        work->timer = 16;
    }
    if (work->timer < 1000) {
        work->timer--;
    }
    work->frame++;
    timer = work->timer;
    if (timer < 0) {
        effDelete(effEntry);
    } else {
        frame = work->frame;
        if (timer < 16) {
            work->alpha = timer << 4;
        }
        if (frame < 16) {
            work->alpha = (frame << 4) + 15;
        }
        dispEntry(4, 2, effHoraoSpellDisp, effEntry, dispCalcZ(&dispPos));
    }
}


void effHoraoSpellDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef union GXColorWord {
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        } ch;
        u32 word;
    } GXColorWord;
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern f64 sin(f64);

    extern u8 colr[];
    extern u8 colg[];
    extern u8 colb[];
    extern u32 unk_804296f8;
    extern u32 unk_804296fc;
    extern u32 unk_80429700;

    extern const u32 dat_804254d0;
    extern const f32 float_deg2rad_804254d4;
    extern const f32 float_0p0078125_804254d8;
    extern const f32 float_0p03125_804254dc;
    extern const f32 float_0_804254e0;
    extern const f32 float_6p2832_804254e4;
    extern const f32 float_78p75_804254e8;
    extern const f32 float_0p34907_804254ec;
    extern const f32 float_6_804254f0;
    extern const f32 float_360_804254f4;
    extern const f32 float_neg30_804254f8;
    extern const f32 float_8_804254fc;

    u8* w = *(u8**)((u8*)effect + 0xC);
    void* cam = camGetPtr(cameraId);
    Mtx base, m, r, s;
    u8 tex[0x20];
    GXColorWord color1;
    GXColorWord color2;
    GXColorWord color3;
    u32 c;
    s32 i;
    s32 frame = *(s32*)(w + 0x14);
    s32 alpha = *(s32*)(w + 0x24);

    PSMTXTrans(m, *(f32*)(w + 4), *(f32*)(w + 8), *(f32*)(w + 0xC));
    PSMTXRotRad(
        r,
        0x79,
        -*(f32*)((u8*)camGetPtr(4) + 0x114) * float_deg2rad_804254d4);
    PSMTXConcat(m, r, m);
    PSMTXScale(
        s,
        *(f32*)(w + 0x38),
        *(f32*)(w + 0x38),
        *(f32*)(w + 0x38));
    PSMTXConcat(m, s, m);
    PSMTXConcat((u8*)cam + 0x11C, m, base);

    color1.word = unk_804296f8;
    color1.ch.r = (u8)*(s32*)(w + 0x18);
    color1.ch.g = (u8)*(s32*)(w + 0x1C);
    color1.ch.b = (u8)*(s32*)(w + 0x20);
    color1.ch.a = (u8)alpha;
    GXSetTevColor(1, &color1.word);

    color2.word = unk_804296fc;
    color2.ch.r = (u8)*(s32*)(w + 0x28);
    color2.ch.g = (u8)*(s32*)(w + 0x2C);
    color2.ch.b = (u8)*(s32*)(w + 0x30);
    color2.ch.a = (u8)*(s32*)(w + 0x34);
    GXSetTevColor(2, &color2.word);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 1, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 8, 8, 15);
    GXSetTevAlphaIn(0, 7, 4, 5, 5);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 2, 4, 0, 15);
    GXSetTevAlphaIn(1, 7, 0, 1, 7);

    effGetTexObjN64(0x73, tex);
    GXLoadTexObj(tex, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(
        s,
        float_0p0078125_804254d8,
        float_0p03125_804254dc,
        float_0_804254e0);
    GXLoadTexMtxImm(s, 0x1E, 1);
    GXSetCullMode(0);

    /*
     * This is the historically strong A2 two-pass reconstruction, but every
     * target scalar is now referenced through its real .sdata2 identity.
     */
    for (i = 0; i < 2; i++, frame += 4) {
        s32 colorIndex = frame % 17;
        f32 angle = (f32)(-282 + i * 36);

        PSMTXRotRad(r, 0x7A, angle * float_deg2rad_804254d4);
        PSMTXConcat(base, r, m);
        GXLoadPosMtxImm(m, 0);
        GXSetCurrentMtx(0);

        color3.word = unk_80429700;
        color3.ch.r = colr[colorIndex];
        color3.ch.g = colg[colorIndex];
        color3.ch.b = colb[colorIndex];
        color3.ch.a = (u8)alpha;
        GXSetTevColor(1, &color3.word);

        c = dat_804254d0;
        GXSetTevColor(2, &c);

        PSMTXScale(
            s,
            float_0p0078125_804254d8,
            float_0p03125_804254dc,
            float_0_804254e0);
        PSMTXTrans(
            r,
            (f32)(frame * -2),
            float_0_804254e0,
            float_0_804254e0);
        PSMTXConcat(s, r, s);
        GXLoadTexMtxImm(s, 0x1E, 1);

        PSMTXRotRad(r, 0x78, float_0p34907_804254ec);
        PSMTXTrans(
            s,
            float_6_804254f0,
            float_0_804254e0,
            float_0_804254e0);
        PSMTXConcat(r, s, s);

        PSMTXRotRad(
            r,
            0x79,
            float_neg30_804254f8 *
                (f32)sin(
                    (float_6p2832_804254e4 *
                     (float_78p75_804254e8 +
                      frame * float_8_804254fc)) /
                    float_360_804254f4) *
                float_deg2rad_804254d4);
        PSMTXConcat(s, r, s);
        PSMTXConcat(m, s, m);

        effSetVtxDescN64((void*)0x803A4A00);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

u32 horao_spell_vtx[112] = {
    0x0000FFF8, 0x00000000, 0x00000000, 0x00000000, 0x00080000, 0x00000400, 0x00000000, 0x0000FFF8,
    0x00000200, 0x00000000, 0x00320000, 0x00080000, 0x02000400, 0x00000032, 0x0000FFF8, 0x00000400,
    0x00000000, 0x00640000, 0x00080000, 0x04000400, 0x00000064, 0x0000FFF8, 0x00000600, 0x00000000,
    0x00960000, 0x00080000, 0x06000400, 0x00000096, 0x0000FFF8, 0x00000800, 0x00000000, 0x00C80000,
    0x00080000, 0x08000400, 0x000000C8, 0x0000FFF8, 0x00000A00, 0x00000000, 0x00FA0000, 0x00080000,
    0x0A000400, 0x000000FA, 0x0000FFF8, 0x00000C00, 0x00000000, 0x00FF0000, 0x00080000, 0x0C000400,
    0x000000FF, 0x0000FFF8, 0x00000E00, 0x00000000, 0x00FF0000, 0x00080000, 0x0E000400, 0x000000FF,
    0x0000FFF8, 0x00001000, 0x00000000, 0x00FF0000, 0x00080000, 0x10000400, 0x000000FF, 0x0000FFF8,
    0x00001200, 0x00000000, 0x00FF0000, 0x00080000, 0x12000400, 0x000000FF, 0x0000FFF8, 0x00001400,
    0x00000000, 0x00FF0000, 0x00080000, 0x14000400, 0x000000FF, 0x0000FFF8, 0x00001600, 0x00000000,
    0x00FF0000, 0x00080000, 0x16000400, 0x000000FF, 0x0000FFF8, 0x00001800, 0x00000000, 0x00FF0000,
    0x00080000, 0x18000400, 0x000000FF, 0x0000FFF8, 0x00001A00, 0x00000000, 0x00C80000, 0x00080000,
    0x1A000400, 0x000000C8, 0x0000FFF8, 0x00001C00, 0x00000000, 0x00640000, 0x00080000, 0x1C000400,
    0x00000064, 0x0000FFF8, 0x00001E00, 0x00000000, 0x00000000, 0x00080000, 0x1E000400, 0x00000000,
};

u8 colr[17] = { 0x78, 0x82, 0x8C, 0x96, 0xA0, 0xAA, 0xB4, 0xBE, 0xC8, 0xC8, 0xBE, 0xB4, 0xAA, 0xA0, 0x96, 0x8C, 0x82 };
u8 colg[17] = { 0xAA, 0xA0, 0x96, 0x8C, 0x82, 0x78, 0x82, 0x8C, 0x96, 0xA0, 0xAA, 0xB4, 0xBE, 0xC8, 0xC8, 0xBE, 0xB4 };
u8 colb[17] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

const Vec3 vec3_802fb120 = { 0.0f, 0.0f, 0.0f };

const u32 dat_804254d0 = 0xFFFFFF00;
const f32 float_deg2rad_804254d4 = 0.01745329238474369f;
const f32 float_0p0078125_804254d8 = 0.0078125f;
const f32 float_0p03125_804254dc = 0.03125f;
const f32 float_0_804254e0 = 0.0f;
const f32 float_6p2832_804254e4 = 6.2831854820251465f;
const f32 float_78p75_804254e8 = 78.75f;
const f32 float_0p34907_804254ec = 0.3490658402442932f;
const f32 float_6_804254f0 = 6.0f;
const f32 float_360_804254f4 = 360.0f;
const f32 float_neg30_804254f8 = -30.0f;
const f32 float_8_804254fc = 8.0f;
const f32 float_30_80425500 = 30.0f;
const f32 float_0p10472_80425504 = 0.10471975803375244f;
const f32 float_1_80425508 = 1.0f;
const f32 float_0p01_8042550c = 0.009999999776482582f;
