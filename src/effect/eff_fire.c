#include "effect/eff_fire.h"

typedef f32 Mtx[3][4];

extern void PSMTXTrans(Mtx, f32, f32, f32);
extern void PSMTXRotRad(Mtx, f32, char);
extern void PSMTXScale(Mtx, f32, f32, f32);
extern void PSMTXConcat(void*, void*, void*);
extern void GXLoadPosMtxImm(Mtx, s32);
extern void GXLoadTexMtxImm(Mtx, s32, s32);
extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
extern void* camGetPtr(s32);
extern void effGetTexObj(s32, void*);
extern void GXLoadTexObj(void*, s32);
extern void GXSetNumChans(u32);
extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
extern void GXSetNumTexGens(u32);
extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
extern void GXClearVtxDesc(void);
extern void GXSetVtxDesc(s32, s32);
extern void GXSetNumTevStages(u32);
extern void GXSetTevOrder(u32, u32, u32, s32);
extern void GXBegin(s32, s32, s16);
extern void* __memAlloc(s32, u32);
extern s32 rand(void);
extern void effDelete(void*);
extern void dispEntry(s32, s32, void*, void*, f32);
extern f32 float_1_80424470;
extern f32 float_0_80424478;
extern void GXSetCurrentMtx(u32);
extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effFireEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 duration) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct EffectEntry {
        s32 flags;
        s32 unk4;
        s32 live;
        void* work;
        void* main;
        char* name;
    } EffectEntry;
    typedef struct FireWork {
        s32 type;
        Vec3 pos;
        u8 alpha;
        u8 pad11;
        u8 pad12;
        u8 pad13;
        s32 duration;
        s32 frame;
        s32 sparkTimer;
        s32 sparkDelay;
        f32 scale;
        s32 texFrame1;
        s32 texFrame2;
        s32 frameCount;
        s32 frameCount2;
        s32 alpha2;
        s32 patternIndex;
        s32 patternTimer;
        f32 scaleMul;
        f32 zero4c;
        f32 zero50;
        f32 zero54;
        f32 zero58;
        f32 zero5c;
        f32 zero60;
        u8 pad64[0x14];
        f32 scaleX;
        f32 scaleY;
        s32 unk80;
        s32 unk84;
        s32 unk88;
    } FireWork;
    extern EffectEntry* effEntry(void);
    extern void effFireMain(void);
    extern char str_Fire_804244d8[];

    EffectEntry* effect;
    FireWork* work;
    s32 animLen;

    effect = effEntry();
    effect->name = str_Fire_804244d8;
    effect->live = 1;
    work = __memAlloc(3, 0x8C);
    effect->work = work;
    effect->main = effFireMain;
    effect->flags |= 2;

    if (duration <= 0) {
        duration = 1000;
    }

    work->type = type;
    work->alpha = 255;
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    work->scale = scale;
    work->duration = duration;
    work->frame = 0;
    work->scaleX = 1.0f;
    work->scaleY = 1.0f;
    work->sparkTimer = 0;
    work->sparkDelay = rand() % 30 + 30;
    work->texFrame1 = 0;
    work->texFrame2 = 0;
    work->zero4c = 0.0f;
    work->zero50 = 0.0f;
    work->zero54 = 0.0f;
    work->zero58 = 0.0f;
    work->zero5c = 0.0f;
    work->zero60 = 0.0f;

    switch (type) {
        case 0:
            work->frameCount2 = 12;
            work->frameCount = work->frameCount2 * 2;
            break;
        case 1:
            work->frameCount2 = 4;
            work->frameCount = work->frameCount2 * 2;
            break;
        case 2:
            work->frameCount2 = 10;
            work->frameCount = work->frameCount2 * 2;
            break;
        case 3:
        case 4:
        case 8:
            work->frameCount2 = 12;
            work->frameCount = work->frameCount2 * 2;
            work->alpha = 204;
            break;
        case 5:
        case 6:
        case 7:
            work->frameCount2 = 12;
            work->frameCount = work->frameCount2 * 2;
            work->alpha = 255;
            break;
    }

    animLen = work->frameCount;
    work->frame = rand() % animLen;
    work->patternIndex = 0;
    work->patternTimer = 0;
    work->scaleMul = 0.0f;
    work->unk80 = 0;
    work->unk84 = 0;
    work->unk88 = 0;

    return effect;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 effFireMain(void* effect) {
    extern void effFireSparkN64Entry(f32, f32, f32, f32, s32, s32, s32);
    extern void* effEntry(void);
    extern f32 dispCalcZ(void*);
    extern void effFireDisp(void);
    extern void effFireDisp2(void);
    extern void effFireDisp3(void);
    extern void effFireSmokeMain(void);
    extern char str_FireSmoke_802f9510[];
    extern f32 float_neg8_80424484;
    extern f32 float_5_8042448c;
    extern f32 float_1p4_804244c8;
    extern f32 float_1p2_804244cc;
    extern f32 float_10_804244d0;
    extern f32 float_30_804244d4;
    extern u8 dat_80424428[];
    extern f32 scale1[];
    extern f32 scale2[];

    s32* work;
    s32 type;
    s32 r;
    s32 idx;
    s32 next;
    s32 start;
    f32 t;
    f32* table;
    void* smoke;
    s32* smokeWork;
    f32 y;

    work = *(s32**)((s32)effect + 0xC);
    type = work[0];
    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        work[5] = 0x10;
    }
    if (work[5] < 1000) {
        work[5]--;
    }
    if (work[5] < 0x10) {
        *(u8*)((s32)work + 0x10) = work[5] << 4;
    }
    if (work[5] < 0) {
        effDelete(effect);
        return 0;
    }

    if (type < 9 && type > 2) {
        work[10] = (s32)(((f32)work[13] * (f32)work[6]) / (f32)work[12]);
        work[11] = (s32)(((f32)work[13] * (f32)(work[6] + 1)) / (f32)work[12]);
        if (work[13] <= work[10]) {
            work[10] = 0;
        }
        if (work[13] <= work[11]) {
            work[11] = 0;
        }
        work[14] = 0x80;
    }

    work[6]++;
    if (work[12] < work[6]) {
        work[6] = 0;
    }

    if (type == 8 || (type < 8 && type > 2 && !(type > 4))) {
        if (type == 4) {
            idx = 4;
            table = scale2;
        } else {
            idx = 0xD;
            table = scale1;
        }
        while (1) {
            next = *(s32*)((s32)table + work[15] * 8);
            if (work[16] < next) {
                break;
            }
            work[15]++;
            if (idx <= work[15]) {
                work[15] = 0;
                work[16] = 0;
            }
        }
        start = *(s32*)((s32)table + work[15] * 8 - 8);
        t = (f32)(next - work[16]) / (f32)(next - start);
        work[17] = (s32)(((float_1_80424470 - t) * *(f32*)((s32)table + work[15] * 8 + 4)) +
                         (t * *(f32*)((s32)table + work[15] * 8 - 4)));
        work[17] = (s32)((f32)work[17] * float_1p4_804244c8);
        work[17] = (s32)((f32)work[17] * float_1p2_804244cc);
        work[16]++;
    } else {
        if (type < 8 && type > 4) {
            work[7]++;
            if (work[8] < work[7]) {
                effFireSparkN64Entry((f32)work[1],
                                     float_10_804244d0 * (f32)work[9] + (f32)work[2],
                                     (f32)work[3],
                                     float_30_804244d4 * (f32)work[30],
                                     type, 2, 200);
                work[7] = 0;
                r = rand();
                work[8] = r % 30 + 30;
            }
        } else {
            work[7]++;
            if (work[8] < work[7]) {
                y = (f32)work[2] + (f32)dat_80424428[type];
                smoke = effEntry();
                *(char**)((s32)smoke + 0x14) = str_FireSmoke_802f9510;
                *(s32*)((s32)smoke + 8) = 1;
                smokeWork = __memAlloc(3, 0x40);
                *(void**)((s32)smoke + 0xC) = smokeWork;
                *(void**)((s32)smoke + 0x10) = effFireSmokeMain;
                *(s32*)smoke |= 2;
                smokeWork[0] = 0;
                *(u8*)((s32)smokeWork + 0x33) = 0x80;
                smokeWork[1] = work[1];
                *(f32*)(smokeWork + 2) = y;
                *(f32*)(smokeWork + 3) = (f32)work[3] - float_5_8042448c;
                smokeWork[15] = work[9];
                smokeWork[13] = 0x3C;
                smokeWork[14] = 0;
                *(f32*)(smokeWork + 4) = float_0_80424478;
                *(f32*)(smokeWork + 5) = float_1_80424470;
                *(f32*)(smokeWork + 6) = float_0_80424478;
                *(f32*)(smokeWork + 11) = float_neg8_80424484;
                *(u8*)(smokeWork + 0xC) = 0xCF;
                *(u8*)((s32)smokeWork + 0x31) = 0x67;
                *(u8*)((s32)smokeWork + 0x32) = 0x38;
                work[7] = 0;
                r = rand();
                work[8] = r % 30 + 30;
            }
        }
    }

    if (type == 8 || (type < 8 && type > 2 && !(type > 4))) {
        dispEntry(4, 2, effFireDisp2, effect, dispCalcZ(work + 1));
    } else if (type < 8 && type > 4) {
        dispEntry(4, 2, effFireDisp3, effect, dispCalcZ(work + 1));
    } else {
        dispEntry(4, 8, effFireDisp, effect, dispCalcZ(work + 1));
    }
}

void effFireDisp(int camId, int effect) {
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void GXSetBlendMode(u32, u32, u32, u32);
    extern void GXSetZCompLoc(u32);
    extern void GXSetAlphaCompare(u32, u32, u32, u32, u32);
    extern void GXSetZMode(u32, u32, u32);
    extern void GXSetTevColorOp(s32, u32, u32, u32, u32, u32);
    extern void GXSetTevAlphaOp(s32, u32, u32, u32, u32, u32);
    extern void GXSetTevKColorSel(u32, u32);
    extern void GXSetTevKAlphaSel(u32, u32);
    extern void GXSetTevKColor(s32, void*);
    extern void GXSetTevColor(s32, void*);
    extern u32 dat_8042442c;
    extern u32 dat_80424430;
    extern u32 dat_80424434;
    extern u32 dat_80424438;
    extern u32 dat_8042443c;
    s32* work = *(s32**)(effect + 0xC);
    char* cam = camGetPtr(camId);
    GXTexObj tex;
    Mtx trans, rot, scale, model, texScale;
    s32 frames = 12;
    s32 width = 32;
    s32 height = 32;
    s32 frame;
    f32 size;
    f32 halfWidth;
    u32 tevColor;

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    if (work[0] == 1) {
        effGetTexObj(0x46, &tex);
        frames = 4;
        height = 40;
    } else if (work[0] == 2) {
        effGetTexObj(0x47, &tex);
        frames = 10;
        width = 24;
        height = 40;
    } else {
        effGetTexObj(0x22, &tex);
    }
    GXLoadTexObj(&tex, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texScale, 1.0f / (f32)frames, 1.0f, 1.0f);
    frame = (work[6] >> 1) % frames;
    PSMTXTrans(trans, (f32)frame, 0.0f, 0.0f);
    PSMTXConcat(texScale, trans, model);
    GXLoadTexMtxImm(model, 0x1E, 1);
    PSMTXTrans(trans, *(f32*)&work[1], *(f32*)&work[2], *(f32*)&work[3]);
    cam = camGetPtr(camId);
    PSMTXRotRad(rot, 0.017453292f * -*(f32*)(cam + 0x114), 'y');
    size = 20.0f * *(f32*)&work[9] * 0.03125f;
    PSMTXScale(scale, size * *(f32*)&work[0x1E], size * *(f32*)&work[0x1F], size);
    PSMTXConcat(trans, rot, model);
    PSMTXConcat(model, scale, model);
    PSMTXConcat(cam + 0x118, model, model);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(11, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    PSMTXTrans(trans, 0.01f, -0.01f, 0.0f);
    PSMTXConcat(model, trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0xE, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 14);
    GXSetTevAlphaIn(0, 4, 5, 6, 7);
    GXSetTevKColorSel(0, 0xC);
    GXSetTevKAlphaSel(0, 0x1C);
    tevColor = dat_8042442c;
    GXSetTevKColor(0, &tevColor);
    tevColor = dat_80424430;
    GXSetTevColor(1, &tevColor);
    GXBegin(0x80, 0, 4);
    halfWidth = (f32)width * 0.5f;
    *(volatile f32*)0xCC008000 = -halfWidth;
    *(volatile f32*)0xCC008000 = (f32)height;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile u32*)0xCC008000 = 0xFFFFFFFF;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = halfWidth;
    *(volatile f32*)0xCC008000 = (f32)height;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile u32*)0xCC008000 = 0xFFFFFFFF;
    *(volatile f32*)0xCC008000 = 1.0f;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = halfWidth;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile u32*)0xCC008000 = 0x000000FF;
    *(volatile f32*)0xCC008000 = 1.0f;
    *(volatile f32*)0xCC008000 = 1.0f;
    *(volatile f32*)0xCC008000 = -halfWidth;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile u32*)0xCC008000 = 0x000000FF;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = 1.0f;

    GXLoadPosMtxImm(model, 0);
    GXSetNumTevStages(4);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 1);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 1);
    GXSetTevColorIn(0, 14, 8, 10, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 7);
    GXSetTevKColorSel(0, 0xC);
    tevColor = dat_80424434;
    GXSetTevKColor(0, &tevColor);
    GXSetTevOrder(1, 0xFF, 0xFF, 4);
    GXSetTevColorOp(1, 0, 0, 0, 1, 2);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 2);
    GXSetTevColorIn(1, 14, 8, 10, 15);
    GXSetTevAlphaIn(1, 7, 7, 7, 7);
    GXSetTevKColorSel(1, 0xD);
    tevColor = dat_80424438;
    GXSetTevKColor(1, &tevColor);
    GXSetTevOrder(2, 0, 0, -1);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0xE, 0, 0, 1, 0);
    GXSetTevColorIn(2, 15, 15, 15, 2);
    GXSetTevAlphaIn(2, 4, 2, 6, 7);
    GXSetTevKAlphaSel(2, 0);
    tevColor = dat_8042443c;
    GXSetTevColor(3, &tevColor);
    GXSetTevOrder(3, 0, 0, -1);
    GXSetTevColorOp(3, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
    GXSetTevColorIn(3, 2, 0, 1, 15);
    GXSetTevAlphaIn(3, 4, 7, 0, 0);
    GXBegin(0x80, 0, 4);
    *(volatile f32*)0xCC008000 = -halfWidth;
    *(volatile f32*)0xCC008000 = (f32)height;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile u32*)0xCC008000 = 0xFFFFFFFF;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = halfWidth;
    *(volatile f32*)0xCC008000 = (f32)height;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile u32*)0xCC008000 = 0xFFFFFFFF;
    *(volatile f32*)0xCC008000 = 1.0f;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = halfWidth;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile u32*)0xCC008000 = 0x000000FF;
    *(volatile f32*)0xCC008000 = 1.0f;
    *(volatile f32*)0xCC008000 = 1.0f;
    *(volatile f32*)0xCC008000 = -halfWidth;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile u32*)0xCC008000 = 0x000000FF;
    *(volatile f32*)0xCC008000 = 0.0f;
    *(volatile f32*)0xCC008000 = 1.0f;
}

void effFireDisp2(s32 camId, s32 effect) {
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevKAlphaSel(u32, u32);
    extern void GXSetTevKColor(s32, void*);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXSetTevOp(s32, s32);
    extern f32 float_deg2rad_80424468;
    extern f32 float_0p5_80424474;
    extern f32 float_32_80424488;
    extern f32 float_16_8042449c;
    extern f32 float_40_804244a0;
    extern f32 float_24_804244a4;
    extern f32 float_12_804244a8;
    extern f32 float_20_804244ac;
    extern f32 float_25_804244b0;
    extern f32 float_0p7854_804244b4;
    extern f32 float_2_804244b8;
    extern const u32 dat_80424440, dat_80424444, dat_80424448, dat_8042444c;
    extern const u32 dat_80424450, dat_80424454, dat_80424458;
    extern u32 unk_80429604;
    s32* work = *(s32**)(effect + 0xC);
    char* cam = camGetPtr(camId);
    GXTexObj tex;
    Mtx trans, rot, scale, model;
    f32 baseScale;
    f32 texScale;
    f32 halfWidth;
    f32 texBottom;
    u32 color;
    u8 alpha;
    s32 type = work[0];

    effGetTexObj(0x48, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    texScale = float_1_80424470 / (f32)work[0xD];
    PSMTXScale(scale, texScale, float_1_80424470, float_1_80424470);
    PSMTXTrans(trans, (f32)work[10], float_0_80424478, float_0_80424478);
    PSMTXConcat(scale, trans, trans);
    GXLoadTexMtxImm(trans, 0x1E, 1);
    PSMTXScale(scale, texScale, float_1_80424470, float_1_80424470);
    PSMTXTrans(trans, (f32)work[11], float_0_80424478, float_0_80424478);
    PSMTXConcat(scale, trans, trans);
    GXLoadTexMtxImm(trans, 0x21, 1);
    PSMTXTrans(trans, *(f32*)&work[1], *(f32*)&work[2], *(f32*)&work[3]);
    PSMTXRotRad(rot, float_deg2rad_80424468 * -*(f32*)((char*)camGetPtr(camId) + 0x114), 'y');
    baseScale = *(f32*)&work[9];
    PSMTXScale(scale, baseScale * *(f32*)&work[0x1E],
               baseScale * *(f32*)&work[0x1F], baseScale);
    PSMTXConcat(trans, rot, model);
    PSMTXConcat(model, scale, model);
    PSMTXConcat(cam + 0x118, model, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(11, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 0, 10, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    if (type == 8) {
        color = dat_80424448;
        GXSetTevColor(1, &color);
        color = dat_8042444c;
        GXSetTevColor(2, &color);
    } else if (type == 3 || type == 4) {
        color = dat_80424440;
        GXSetTevColor(1, &color);
        color = dat_80424444;
        GXSetTevColor(2, &color);
    }
    GXSetTevOrder(1, 1, 0, -1);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 15, 15, 0);
    GXSetTevAlphaIn(1, 0, 4, 2, 7);
    alpha = (u8)work[0xE];
    color = alpha | alpha << 8 | alpha << 16 | alpha << 24;
    GXSetTevColor(3, &color);
    GXSetTevOrder(2, 0xFF, 0xFF, -1);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 15, 15, 15, 0);
    GXSetTevAlphaIn(2, 7, 0, 6, 7);
    GXSetTevKAlphaSel(2, 0x1C);
    color = (unk_80429604 & 0xFFFFFF00) | *(u8*)&work[4];
    GXSetTevKColor(0, &color);

    GXBegin(0x80, 0, 4);
    halfWidth = -float_24_804244a4 * float_0p5_80424474;
    *(volatile f32*)0xCC008000 = halfWidth;
    *(volatile f32*)0xCC008000 = float_40_804244a0;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile u8*)0xCC008000 = 0xFF; *(volatile u8*)0xCC008000 = 0xFF;
    *(volatile u8*)0xCC008000 = 0xFF; *(volatile u8*)0xCC008000 = 0xFF;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_12_804244a8;
    *(volatile f32*)0xCC008000 = float_40_804244a0;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile u8*)0xCC008000 = 0xFF; *(volatile u8*)0xCC008000 = 0xFF;
    *(volatile u8*)0xCC008000 = 0xFF; *(volatile u8*)0xCC008000 = 0xFF;
    *(volatile f32*)0xCC008000 = float_1_80424470;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_12_804244a8;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile u8*)0xCC008000 = 0; *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0; *(volatile u8*)0xCC008000 = 0xFF;
    *(volatile f32*)0xCC008000 = float_1_80424470;
    *(volatile f32*)0xCC008000 = float_1_80424470;
    *(volatile f32*)0xCC008000 = halfWidth;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile u8*)0xCC008000 = 0; *(volatile u8*)0xCC008000 = 0;
    *(volatile u8*)0xCC008000 = 0; *(volatile u8*)0xCC008000 = 0xFF;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_1_80424470;

    if (type == 4) {
        PSMTXTrans(trans, float_0_80424478, float_25_804244b0, float_0_80424478);
    } else if (type == 3 || type == 8) {
        PSMTXTrans(trans, float_0_80424478, float_20_804244ac, float_0_80424478);
    }
    baseScale = *(f32*)&work[0x11];
    PSMTXScale(scale, baseScale, baseScale, baseScale);
    PSMTXConcat(trans, scale, trans);
    if (type == 4) {
        PSMTXRotRad(rot, float_0p7854_804244b4, 'z');
        PSMTXConcat(trans, rot, trans);
    }
    PSMTXConcat(model, trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    if (type == 4) {
        color = dat_80424454;
        GXSetChanMatColor(4, &color);
    } else if (type == 3) {
        color = dat_80424450;
        GXSetChanMatColor(4, &color);
    } else if (type == 8) {
        color = dat_80424458;
        GXSetChanMatColor(4, &color);
    }
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    if (type == 4) {
        effGetTexObj(0x60, &tex);
        GXLoadTexObj(&tex, 0);
    } else if (type == 3 || type == 8) {
        effGetTexObj(0x5F, &tex);
        GXLoadTexObj(&tex, 0);
    }
    GXBegin(0x80, 0, 4);
    texBottom = (type == 3 || type == 8) ? float_1_80424470 : float_2_804244b8;
    halfWidth = -float_32_80424488 * float_0p5_80424474;
    *(volatile f32*)0xCC008000 = halfWidth; *(volatile f32*)0xCC008000 = float_16_8042449c;
    *(volatile f32*)0xCC008000 = float_0_80424478; *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_16_8042449c; *(volatile f32*)0xCC008000 = float_16_8042449c;
    *(volatile f32*)0xCC008000 = float_0_80424478; *(volatile f32*)0xCC008000 = float_2_804244b8;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_16_8042449c; *(volatile f32*)0xCC008000 = halfWidth;
    *(volatile f32*)0xCC008000 = float_0_80424478; *(volatile f32*)0xCC008000 = float_2_804244b8;
    *(volatile f32*)0xCC008000 = texBottom;
    *(volatile f32*)0xCC008000 = halfWidth; *(volatile f32*)0xCC008000 = halfWidth;
    *(volatile f32*)0xCC008000 = float_0_80424478; *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = texBottom;
}

const u32 dat_80424440 = 0xFFCD15FF;
const u32 dat_80424444 = 0xFF5100FF;
const u32 dat_80424448 = 0x593359FF;
const u32 dat_8042444c = 0x000000FF;
const u32 dat_80424450 = 0xFFEA5D4C;
const u32 dat_80424454 = 0xFFEA5D26;
const u32 dat_80424458 = 0x0000004C;


static u8 col_tbl[0x60] = {
    0xAC, 0x2E, 0x00, 0xFF, 0xE0, 0x22, 0x00, 0xFF,
    0xFD, 0x5F, 0x00, 0xFF, 0xFD, 0x5F, 0x04, 0xFF,
    0xFF, 0x69, 0x00, 0xFF, 0xFF, 0x8A, 0x00, 0xFF,
    0xFF, 0xB7, 0x00, 0xFF, 0xFF, 0xD7, 0x00, 0xFF,
    0x0A, 0xE0, 0xFF, 0xFF, 0x10, 0xC3, 0xFF, 0xFF,
    0x09, 0x61, 0xFF, 0xFF, 0x09, 0x61, 0xFF, 0xFF,
    0xD1, 0xF9, 0xFF, 0xFF, 0xD1, 0xF9, 0xFF, 0xFF,
    0xA3, 0xE5, 0xFF, 0xFF, 0x3A, 0xCA, 0xFF, 0xFF,
    0x5B, 0xF6, 0x3D, 0xFF, 0x3B, 0xDE, 0x1C, 0xFF,
    0x3B, 0xDE, 0x1C, 0xFF, 0x1D, 0xB9, 0x00, 0xFF,
    0xDE, 0xF6, 0x18, 0xFF, 0xA0, 0xF6, 0x06, 0xFF,
    0xA0, 0xF6, 0x06, 0xFF, 0x92, 0xE4, 0x00, 0xFF,
};

u8 effFireDisp3(s32 camId, s32 effect) {
    typedef struct GXTexObj { u32 data[8]; } GXTexObj;
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    extern void GXSetCullMode(s32);
    extern void PSMTXIdentity(Mtx);
    extern void PSMTXMultVec(Mtx, Vec3*, Vec3*);
    extern f32 getScreenPoint(f32*, f32*);
    extern f32 float_deg2rad_80424468;
    extern f32 float_32_80424488;
    extern f32 float_5_8042448c;
    extern f32 float_0p6_80424490;
    extern f32 float_13p333_80424494;
    extern f32 float_0p33333_80424498;
    extern f32 float_16_8042449c;
    extern f32 float_40_804244a0;
    extern f32 float_0p5_80424474;
    s32* work = *(s32**)(effect + 0xC);
    char* cam = camGetPtr(camId);
    GXTexObj tex;
    Mtx trans, rot, scale, model, local, draw;
    f32 baseScale = *(f32*)&work[9];
    f32 sx = baseScale * *(f32*)&work[0x1E];
    f32 sy = baseScale * *(f32*)&work[0x1F];
    f32 texScale = float_1_80424470 / (f32)work[0xD];
    s32 i, pass;
    u8* colors;
    f32 y0, y1, v0, v1;
    Vec3 point;
    f32 screen[2];

    effGetTexObj(0x49, &tex);
    GXLoadTexObj(&tex, 0);
    effGetTexObj(0x49, &tex);
    GXLoadTexObj(&tex, 1);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    PSMTXScale(scale, texScale, float_1_80424470, float_1_80424470);
    PSMTXTrans(trans, (f32)work[10], float_0_80424478, float_0_80424478);
    PSMTXConcat(scale, trans, trans);
    GXLoadTexMtxImm(trans, 0x1E, 1);
    PSMTXScale(scale, texScale, float_1_80424470, float_1_80424470);
    PSMTXTrans(trans, (f32)work[11], float_0_80424478, float_0_80424478);
    PSMTXConcat(scale, trans, trans);
    GXLoadTexMtxImm(trans, 0x21, 1);
    PSMTXTrans(trans, *(f32*)&work[1], *(f32*)&work[2], *(f32*)&work[3]);
    PSMTXRotRad(rot, -float_deg2rad_80424468 * *(f32*)(cam + 0x114), 'y');
    PSMTXScale(scale, sx, sy, baseScale);
    PSMTXConcat(trans, rot, model);
    PSMTXConcat(model, scale, model);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(11, 1);
    GXSetVtxDesc(13, 1);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 15);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 1, 1, 4);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 3, 1, 0);
    GXSetTevColorIn(1, 15, 15, 15, 10);
    GXSetTevAlphaIn(1, 0, 7, 7, 4);
    GXSetTevOrder(2, 0xFF, 0xFF, -1);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 15, 15, 15, 0);
    GXSetTevAlphaIn(2, 7, 0, 1, 7);
    GXSetCullMode(0);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    colors = col_tbl + (work[0] - 5) * 0x20;
    work[0x1A] = 10000;
    work[0x1B] = -10000;
    work[0x1C] = -10000;
    work[0x1D] = 10000;
    for (pass = 0; pass < 2; pass++, colors += 0x10) {
        if (pass == 0) {
            PSMTXIdentity(local);
        } else {
            PSMTXTrans(trans, float_0_80424478, float_5_8042448c, float_1_80424470);
            PSMTXScale(scale, float_0p6_80424490, float_0p6_80424490,
                       float_0p6_80424490);
            PSMTXConcat(trans, scale, local);
        }
        PSMTXConcat(model, local, draw);
        PSMTXConcat(cam + 0x118, draw, local);
        GXLoadPosMtxImm(local, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 12);
        for (i = 0; i < 3; i++) {
            y0 = float_13p333_80424494 * (f32)(i + 1);
            y1 = float_13p333_80424494 * (f32)i;
            v0 = float_0p33333_80424498 * (f32)(2 - i);
            v1 = float_0p33333_80424498 * (f32)(3 - i);

            *(volatile f32*)0xCC008000 = -float_32_80424488 * float_0p5_80424474;
            *(volatile f32*)0xCC008000 = y0;
            *(volatile f32*)0xCC008000 = float_0_80424478;
            *(volatile u32*)0xCC008000 = *(u32*)(colors + (i + 1) * 4);
            *(volatile f32*)0xCC008000 = float_0_80424478;
            *(volatile f32*)0xCC008000 = v0;

            *(volatile f32*)0xCC008000 = float_16_8042449c;
            *(volatile f32*)0xCC008000 = y0;
            *(volatile f32*)0xCC008000 = float_0_80424478;
            *(volatile u32*)0xCC008000 = *(u32*)(colors + (i + 1) * 4);
            *(volatile f32*)0xCC008000 = float_1_80424470;
            *(volatile f32*)0xCC008000 = v0;

            *(volatile f32*)0xCC008000 = float_16_8042449c;
            *(volatile f32*)0xCC008000 = y1;
            *(volatile f32*)0xCC008000 = float_0_80424478;
            *(volatile u32*)0xCC008000 = *(u32*)(colors + i * 4);
            *(volatile f32*)0xCC008000 = float_1_80424470;
            *(volatile f32*)0xCC008000 = v1;

            *(volatile f32*)0xCC008000 = -float_32_80424488 * float_0p5_80424474;
            *(volatile f32*)0xCC008000 = y1;
            *(volatile f32*)0xCC008000 = float_0_80424478;
            *(volatile u32*)0xCC008000 = *(u32*)(colors + i * 4);
            *(volatile f32*)0xCC008000 = float_0_80424478;
            *(volatile f32*)0xCC008000 = v1;
        }

#define UPDATE_FIRE_BOUND(px, py) \
        point.x = (px); \
        point.y = (py); \
        point.z = float_0_80424478; \
        PSMTXMultVec(draw, &point, &point); \
        getScreenPoint(&point.x, screen); \
        if (screen[0] < (f32)work[0x1A]) work[0x1A] = (s32)screen[0]; \
        if ((f32)work[0x1B] < screen[0]) work[0x1B] = (s32)screen[0]; \
        if ((f32)work[0x1C] < screen[1]) work[0x1C] = (s32)screen[1]; \
        if (screen[1] < (f32)work[0x1D]) work[0x1D] = (s32)screen[1]

        UPDATE_FIRE_BOUND(-float_32_80424488 * float_0p5_80424474, float_40_804244a0);
        UPDATE_FIRE_BOUND(float_16_8042449c, float_40_804244a0);
        UPDATE_FIRE_BOUND(float_16_8042449c, float_0_80424478);
        UPDATE_FIRE_BOUND(-float_32_80424488 * float_0p5_80424474, float_0_80424478);
#undef UPDATE_FIRE_BOUND
    }
    work[0x1A] += 0x130;
    work[0x1B] += 0x130;
    work[0x1C] = 0xF0 - work[0x1C];
    work[0x1D] = 0xF0 - work[0x1D];
    if (work[0x1A] < 0) work[0x1A] = 0;
    if (work[0x1B] > 0x260) work[0x1B] = 0x260;
    if (work[0x1C] < 0) work[0x1C] = 0;
    if (work[0x1D] > 0x1E0) work[0x1D] = 0x1E0;
    work[0x19] = 1;
    return 0;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effFireSmokeMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct FireSmokeWork {
        s32 type;
        Vec3 pos;
        Vec3 vel;
        s32 unk1c;
        s32 unk20;
        f32 scale;
        f32 rot;
        f32 rotStep;
        u8 r, g, b, a;
        s32 life;
        s32 timer;
        f32 drawZ;
    } FireSmokeWork;
    extern f32 dispCalcZ(Vec3*);
    extern void effFireSmokeDisp(void);

    FireSmokeWork* work = *(FireSmokeWork**)((s32)effect + 0xC);
    s32 flags = *(s32*)effect;

    if (flags & 4) {
        *(s32*)effect = flags & ~4;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    if (work->life < 16) {
        work->a = work->life << 3;
    }
    if (work->life < 0) {
        effDelete(effect);
    }
    else {
        work->timer++;
        work->vel.x *= 0.95f;
        work->vel.y *= 0.95f;
        work->vel.z *= 0.95f;
        work->pos.x += work->vel.x;
        work->pos.y += work->vel.y;
        work->pos.z += work->vel.z;
        work->rot += work->rotStep;
        {
            f32 fade = 0.99f;
            work->r = (u8)((f32)work->r * fade);
            work->g = (u8)((f32)work->g * fade);
            work->b = (u8)((f32)work->b * fade);
        }
        dispEntry(4, 2, effFireSmokeDisp, effect, dispCalcZ(&work->pos));
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 effFireSmokeDisp(s32 camId, void* effect) {
    typedef u8 GXColor[4];
    typedef struct GXTexObj { u32 dummy[8]; } GXTexObj;
    extern void GXSetChanMatColor(s32, void*);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void GXSetTevOp(s32, s32);
    extern u8 scale_dt[];
    extern f32 float_0p0078125_8042445c;
    extern f32 float_1p5_80424460;
    extern f32 float_0p8_80424464;
    extern f32 float_deg2rad_80424468;
    extern f32 float_8_8042446c;
    extern f32 float_0p5_80424474;

    char* work;
    void* cam;
    void* cam2;
    u32 index;
    f32 sx;
    f32 sy;
    f32 size;
    f32 negHalf;
    GXColor color;
    GXTexObj texObj;
    Mtx mtx;
    Mtx rot;
    Mtx scale;
    Mtx trans;

    work = *(char**)((s32)effect + 0xC);
    cam = camGetPtr(camId);
    index = *(s32*)(work + 0x38) * 2;
    sx = float_1p5_80424460;
    sy = float_0p8_80424464;
    if (index < 0x3C) {
        sx = (f32)scale_dt[index] * float_0p0078125_8042445c;
        sy = (f32)scale_dt[index + 1] * float_0p0078125_8042445c;
    }

    effGetTexObj(0x21, &texObj);
    GXLoadTexObj(&texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    *(u32*)color = *(u32*)(work + 0x30);
    GXSetChanMatColor(4, color);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    cam2 = camGetPtr(camId);
    PSMTXRotRad(rot, float_deg2rad_80424468 * -*(f32*)((s32)cam2 + 0x114), 'y');
    size = float_8_8042446c * *(f32*)(work + 0x3C);
    PSMTXScale(scale, size * sx, size * sy, size);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXRotRad(rot, float_deg2rad_80424468 * *(f32*)(work + 0x28), 'z');
    PSMTXConcat(mtx, rot, mtx);
    PSMTXConcat((void*)((s32)cam + 0x118), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXBegin(0x80, 0, 4);
    negHalf = -float_1_80424470 * float_0p5_80424474;
    *(volatile f32*)0xCC008000 = negHalf;
    *(volatile f32*)0xCC008000 = float_0p5_80424474;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_0p5_80424474;
    *(volatile f32*)0xCC008000 = float_0p5_80424474;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_1_80424470;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_0p5_80424474;
    *(volatile f32*)0xCC008000 = negHalf;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_1_80424470;
    *(volatile f32*)0xCC008000 = float_1_80424470;
    *(volatile f32*)0xCC008000 = negHalf;
    *(volatile f32*)0xCC008000 = negHalf;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_0_80424478;
    *(volatile f32*)0xCC008000 = float_1_80424470;
}

const f32 float_0p95_8042447c = 0.95f;
const f32 float_0p99_80424480 = 0.99f;
const f32 float_24_804244a4 = 24.0f;
const f32 float_12_804244a8 = 12.0f;
const f32 float_20_804244ac = 20.0f;
const f32 float_25_804244b0 = 25.0f;
const f32 float_0p7854_804244b4 = 0.7853982f;
const f32 float_2_804244b8 = 2.0f;
const f32 float_0p03125_804244bc = 0.03125f;
const f32 float_0p01_804244c0 = 0.01f;
const f32 float_neg0p01_804244c4 = -0.01f;
