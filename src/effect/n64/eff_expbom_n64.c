#include "effect/n64/eff_expbom_n64.h"

typedef struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct EffExpBomWork {
    s16 unk_00;
    s16 life;
    s16 unk_04;
    s16 divTimer;
    s32 timer;
    f32 baseX;
    f32 baseY;
    f32 baseZ;
    f32 x;
    f32 y;
    f32 vx;
    f32 vy;
    u8 pad_28[4];
} EffExpBomWork;

void effDelete(void* effect);
f32 dispCalcZ(Vec3* pos);
void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
void effExpBomDisp(int cameraId, int effect);

extern const Vec3 vec3_802fae10;
extern f32 float_1p25_804250d0;

void* effExpBomN64Entry(f32 x, f32 y, f32 z) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effExpBomMain(void*);
    extern s32 rand(void);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_ExpBomN64_802fae28[];
    extern const f32 float_0_804250ac;
    extern const f32 float_2_804250d4;
    extern const f32 float_6p2832_804250b4;
    extern const f32 float_90_804250b8;
    extern const f32 float_360_804250c0;
    extern f32 bom_dir[];
    extern f32 bom_spd[];
    void* entry;
    EffExpBomWork* work;
    s16 random;
    f32* speed;
    f32* direction;
    f32 spd;
    f32 angle;
    f32 sine;
    f32 cosine;
    s32 i;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_ExpBomN64_802fae28;
    *(s32*)((s32)entry + 8) = 10;
    work = __memAlloc(3, 0x1B8);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effExpBomMain;
    direction = bom_dir;
    speed = bom_spd;

    for (i = 0; i < *(s32*)((s32)entry + 8);
         i++, work++, speed++, direction++) {
        work->unk_00 = 0;
        random = (s16)(rand() % 6);
        random += 16;
        work->unk_04 = random;
        work->life = random;
        work->divTimer = 4;
        work->timer = 0;
        work->baseX = x;
        work->baseY = y;
        work->baseZ = z;
        work->x = float_0_804250ac;
        work->y = float_0_804250ac;
        spd = float_2_804250d4 * *speed;
        angle = (float_6p2832_804250b4 *
                 (float_90_804250b8 + *direction)) /
                float_360_804250c0;
        sine = (f32)sin(angle);
        cosine = (f32)cos(angle);
        work->vx = spd * sine;
        work->vy = -spd * cosine;
    }
    return entry;
}

void effExpBomMain(void* effect) {
    Vec3 dispPos;
    Vec3 pos;
    const Vec3* zero;
    register EffExpBomWork* work;
    register s32 i;
    register s32 anyAlive;
    f32 div;

    anyAlive = 0;
    i = 0;
    work = *(EffExpBomWork**)((s32)effect + 0xC);
    zero = &vec3_802fae10;
    pos = *zero;
    pos.x = work->baseX;
    pos.y = work->baseY;
    pos.z = work->baseZ;
    dispPos = pos;

    while (i < *(s32*)((s32)effect + 0x8)) {
        if (work->life >= 0) {
            work->timer--;
            if (work->timer < 0) {
                anyAlive = 1;
                work->x += work->vx;
                work->y += work->vy;
                work->unk_00++;
                work->life--;
                if (work->life >= 0) {
                    work->divTimer--;
                    if (work->divTimer < 0) {
                        div = float_1p25_804250d0;
                        work->vx /= div;
                        work->vy /= div;
                    }
                }
            }
        }
        i++;
        work++;
    }

    if (anyAlive == 0) {
        effDelete(effect);
    } else {
        dispEntry(4, 2, effExpBomDisp, effect, dispCalcZ(&dispPos));
    }
}


void effExpBomDisp(int cameraId, int effect) {
    typedef f32 Mtx[3][4];
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXBegin(s32, s32, s32);
    extern f64 sin(f64);
    extern f32 bom_scale[];
    extern f32 bom_ry[];
    extern f32 float_255_804250c4;
    extern f32 float_neg16_804250c8;
    extern f32 float_16_804250cc;
    extern f64 double_to_int_802fae20;
    extern u32 unk_80429688;
    extern u32 unk_8042968c;
    s16* work;
    u8* camera;
    u8* camera2;
    f32* scalePtr;
    f32* ryPtr;
    Mtx base;
    Mtx rot;
    Mtx scale;
    Mtx model;
    u8 texObj[0x20];
    u32 color1;
    u32 color2;
    s32 i;
    s32 frame;
    s32 tex;
    s32 alpha;
    f32 life;
    f32 total;
    f32 wave;
    volatile f32* fifo = (volatile f32*)0xCC008000;

    camera = camGetPtr(cameraId);
    work = *(s16**)(effect + 0xC);
    PSMTXTrans(base, *(f32*)(work + 6), *(f32*)(work + 8), *(f32*)(work + 10));
    camera2 = camGetPtr(cameraId);
    PSMTXRotRad(rot, 0x79, 0.0174533f * -*(f32*)(camera2 + 0x114));
    PSMTXConcat(base, rot, model);
    PSMTXConcat(camera + 0x11C, model, model);
    scalePtr = bom_scale;
    ryPtr = bom_ry;

    for (i = 0; i < *(s32*)(effect + 8); i++, scalePtr++, ryPtr++, work += 0x16) {
        frame = work[1];
        if (work[1] >= 0 && *(s32*)(work + 4) < 0) {
            if (frame > 16) {
                frame = 16;
            }
            PSMTXTrans(base, *(f32*)(work + 0xC), *(f32*)(work + 0xE), 0.0f);
            PSMTXScale(scale, *scalePtr, *scalePtr, *scalePtr);
            PSMTXRotRad(rot, 0x7A, 0.0174533f * *ryPtr);
            PSMTXConcat(base, scale, base);
            PSMTXConcat(base, rot, rot);
            PSMTXConcat(model, rot, rot);
            GXLoadPosMtxImm(rot, 0);

            life = (f32)work[0] - 1.0f;
            total = (f32)work[2];
            if (life < total) {
                wave = (f32)sin((f64)(6.2832f * 90.0f *
                                      ((7.0f * life) / total) /
                                      7.0f / 360.0f));
                tex = (s32)(7.0f * wave);
                alpha = (s32)(float_255_804250c4 * (7.0f * wave));
            } else {
                alpha = 0xFF;
                tex = 6;
            }

            color1 = unk_80429688;
            ((u8*)&color1)[3] = (u8)alpha;
            GXSetTevColor(1, &color1);
            color2 = unk_8042968c;
            ((u8*)&color2)[3] = (u8)((frame * 210) >> 4);
            GXSetTevColor(2, &color2);
            GXSetNumChans(0);
            GXSetNumTexGens(2);
            GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
            GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
            GXSetNumTevStages(3);
            GXSetTevOrder(0, 0, 0, 0xFF);
            GXSetTevColorOp(0, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
            GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
            GXSetTevAlphaIn(0, 7, 7, 7, 4);
            GXSetTevOrder(1, 1, 1, 0xFF);
            GXSetTevColorOp(1, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
            GXSetTevColorIn(1, 0, 8, 3, 0xF);
            GXSetTevAlphaIn(1, 0, 4, 1, 7);
            GXSetTevOrder(2, 0xFF, 0xFF, 0xFF);
            GXSetTevColorOp(2, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
            GXSetTevColorIn(2, 0xF, 0xF, 0xF, 0);
            GXSetTevAlphaIn(2, 7, 0, 2, 7);
            GXSetCullMode(0);
            GXClearVtxDesc();
            GXSetVtxDesc(9, 1);
            GXSetVtxDesc(0xD, 1);
            GXSetVtxAttrFmt(0, 9, 1, 4, 0);
            GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

            if ((u32)tex <= 6) {
                effGetTexObjN64(tex, texObj);
                GXLoadTexObj(texObj, 0);
                if (tex != 6) {
                    tex++;
                }
                effGetTexObjN64(tex, texObj);
                GXLoadTexObj(texObj, 1);
            }

            GXBegin(0x80, 0, 4);
            *fifo = float_neg16_804250c8; *fifo = float_16_804250cc;
            *fifo = 0.0f; *fifo = 0.0f;
            *fifo = 0.0f; *fifo = float_16_804250cc;
            *fifo = float_16_804250cc; *fifo = 0.0f;
            *fifo = 1.0f; *fifo = 0.0f;
            *fifo = float_16_804250cc; *fifo = float_neg16_804250c8;
            *fifo = 0.0f; *fifo = 1.0f;
            *fifo = 1.0f; *fifo = float_neg16_804250c8;
            *fifo = float_neg16_804250c8; *fifo = 0.0f;
            *fifo = 0.0f; *fifo = 1.0f;
        }
    }
}

f32 bom_dir[10] = { 10.0f, 40.0f, 80.0f, 170.0f, 140.0f, 100.0f, 25.0f, 155.0f, 60.0f, 120.0f };
f32 bom_spd[10] = { 2.2f, 2.7f, 3.0f, 2.2f, 2.7f, 3.0f, 1.9f, 1.9f, 1.5f, 1.5f };
f32 bom_scale[10] = { 1.4f, 1.3f, 1.2f, 1.3f, 1.4f, 1.3f, 1.6f, 1.6f, 1.6f, 1.6f };
f32 bom_ry[10] = { 0.0f, 234.0f, 468.0f, 702.0f, 936.0f, 1260.0f, 1404.0f, 1638.0f, 1902.0f, 1976.0f };

const Vec3 vec3_802fae10 = { 0.0f, 0.0f, 0.0f };
const char str_ExpBomN64_802fae28[] = "ExpBomN64";
