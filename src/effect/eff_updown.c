#include "effect/eff_updown.h"

extern void* effEntry(void);
extern void* __memAlloc(s32 heap, u32 size);
extern f32 float_1_80424624;

const char str_Updown_802f9708[] = "Updown";


u8 effUpdownDisp(int param_1, int param_2) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void polygon(s32 type, u8 alpha, f32 x, f32 y, f32 w, f32 h, f32 s, f32 t) {
    extern void GXBegin(s32 primitive, s32 vtxfmt, s32 nverts);

    volatile f32* gxFifoF = (volatile f32*)0xCC008000;
    volatile u8* gxFifoB = (volatile u8*)0xCC008000;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 r2;
    u8 g2;
    u8 b2;
    f32 x2;
    f32 y2;

    switch (type) {
        case 0:
            r1 = 255;
            g1 = 255;
            b1 = 255;
            r2 = 255;
            g2 = 255;
            b2 = 255;
            break;
        case 1:
            r1 = 233;
            g1 = 74;
            b1 = 63;
            r2 = 254;
            g2 = 230;
            b2 = 62;
            break;
        case 2:
            r1 = 63;
            g1 = 63;
            b1 = 233;
            r2 = 116;
            g2 = 201;
            b2 = 255;
            break;
    }

    GXBegin(0x80, 0, 4);
    x2 = x + w;
    y2 = y - h;
    *gxFifoF = x;
    *gxFifoF = y;
    *gxFifoF = 0.0f;
    *gxFifoB = r1;
    *gxFifoB = g1;
    *gxFifoB = b1;
    *gxFifoB = alpha;
    *gxFifoF = 0.0f;
    *gxFifoF = 0.0f;
    *gxFifoF = x2;
    *gxFifoF = y;
    *gxFifoF = 0.0f;
    *gxFifoB = r1;
    *gxFifoB = g1;
    *gxFifoB = b1;
    *gxFifoB = alpha;
    *gxFifoF = s;
    *gxFifoF = 0.0f;
    *gxFifoF = x2;
    *gxFifoF = y2;
    *gxFifoF = 0.0f;
    *gxFifoB = r2;
    *gxFifoB = g2;
    *gxFifoB = b2;
    *gxFifoB = alpha;
    *gxFifoF = s;
    *gxFifoF = t;
    *gxFifoF = x;
    *gxFifoF = y2;
    *gxFifoF = 0.0f;
    *gxFifoB = r2;
    *gxFifoB = g2;
    *gxFifoB = b2;
    *gxFifoB = alpha;
    *gxFifoF = 0.0f;
    *gxFifoF = t;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effUpdownMain(void* effect) {
    typedef struct Vec3 { f32 x, y, z; } Vec3;
    typedef struct UpdownWork {
        s32 type;
        Vec3 pos;
        f32 scaleX;
        f32 scaleY;
        s32 life;
        s32 kind;
        s32 frame;
        u8 alpha;
    } UpdownWork;
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effUpdownDisp(void);
    extern u8 scale_data[];
    extern f64 sin(f64);

    UpdownWork* work;
    s32 drawMode;
    s32 flags;
    s32 type;

    flags = *(s32*)effect;
    work = *(UpdownWork**)((s32)effect + 0xC);
    type = work->type;
    if (flags & 4) {
        *(s32*)effect = flags & ~4;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    if (work->life < 0) {
        effDelete(effect);
        return 0;
    }
    if (work->life < 16) {
        work->alpha = work->life << 4;
    }
    if (type == 0 || type == 1) {
        if (work->frame < 15) {
            work->scaleX = (f32)scale_data[work->frame * 2] / 100.0f;
            work->scaleY = (f32)scale_data[work->frame * 2 + 1] / 100.0f;
        }
        else {
            work->scaleX = 1.0f;
            work->scaleY = 1.0f - (0.1f * (f32)sin((f32)(work->frame - 15) * 0.25f));
        }
    }
    work->frame++;
    drawMode = 1;
    if (work->alpha != 0xFF) {
        drawMode = 2;
    }
    dispEntry(4, drawMode, effUpdownDisp, effect, dispCalcZ(&work->pos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effUpdownEntry(f32 x, s32 type, f32 y, s32 arg1, f32 z, s32 arg2) {
    void* entry;
    void* work;
    s32 main;
    f32 one;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_Updown_802f9708;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x28);
    *(void**)((s32)entry + 0xC) = work;
    main = (s32)effUpdownMain;
    one = float_1_80424624;
    *(s32*)((s32)entry + 0x10) = main;
    *(u32*)entry |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x14) = one;
    *(f32*)((s32)work + 0x10) = one;
    *(s32*)((s32)work + 0x18) = arg1;
    *(s32*)((s32)work + 0x1C) = arg2;
    *(s32*)((s32)work + 0x20) = 0;
    *(u8*)((s32)work + 0x24) = 0xFF;
    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
