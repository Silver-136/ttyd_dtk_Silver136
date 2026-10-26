#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "libc_decls.h"
#include <math.h>
#include "eff/eff_particle.h"

double log(double);

typedef struct ParticleItem {
    s32 next;  // 0x0
    Vec pos;   // 0x4
    Vec vel;   // 0x10
    s32 life;  // 0x1C
} ParticleItem;

typedef struct ParticleEmitter {
    Mtx mtx;       // 0x0
    Vec pos;       // 0x30
    Vec vecA;      // 0x3C
    Vec vecB;      // 0x48
    f32 accum;     // 0x54
    f32 rate;      // 0x58
    f32 lifeMean;  // 0x5C
    f32 lifeStd;   // 0x60
    f32 pad[2];    // 0x64
} ParticleEmitter;

typedef struct ParticleWork {
    s32 type;             // 0x0
    Vec pos;              // 0x4
    Vec dir;              // 0x10
    f32 angX;             // 0x1C
    f32 angY;             // 0x20
    f32 scale;            // 0x24
    s32 life;             // 0x28
    s32 frame;            // 0x2C
    s32 alpha;            // 0x30
    ParticleEmitter emit; // 0x34
    ParticleItem* items;  // 0xA0
    s32 count;            // 0xA4
    s32 active;           // 0xA8
    f32 speed;            // 0xAC
} ParticleWork;

GXColor hibana_col[32] = {
    {0x5F, 0x28, 0x00, 0x00},
    {0x69, 0x35, 0x00, 0x08},
    {0x73, 0x42, 0x00, 0x10},
    {0x7D, 0x4F, 0x00, 0x18},
    {0x88, 0x5D, 0x00, 0x20},
    {0x92, 0x6A, 0x00, 0x29},
    {0x9D, 0x78, 0x00, 0x31},
    {0xA7, 0x85, 0x00, 0x39},
    {0xB1, 0x92, 0x00, 0x41},
    {0xBC, 0xA0, 0x00, 0x4A},
    {0xC6, 0xAD, 0x00, 0x52},
    {0xD0, 0xBA, 0x00, 0x5A},
    {0xDB, 0xC8, 0x00, 0x62},
    {0xE5, 0xD5, 0x00, 0x6A},
    {0xEF, 0xE2, 0x00, 0x73},
    {0xFA, 0xF0, 0x00, 0x7B},
    {0xFF, 0xF0, 0x00, 0x83},
    {0xFF, 0xF1, 0x10, 0x8B},
    {0xFF, 0xF2, 0x21, 0x94},
    {0xFF, 0xF3, 0x32, 0x9C},
    {0xFF, 0xF4, 0x44, 0xA4},
    {0xFF, 0xF5, 0x55, 0xAC},
    {0xFF, 0xF6, 0x66, 0xB4},
    {0xFF, 0xF7, 0x77, 0xBD},
    {0xFF, 0xF8, 0x88, 0xC5},
    {0xFF, 0xF9, 0x99, 0xCD},
    {0xFF, 0xFA, 0xAA, 0xD5},
    {0xFF, 0xFB, 0xBB, 0xDE},
    {0xFF, 0xFC, 0xCC, 0xE6},
    {0xFF, 0xFD, 0xDD, 0xEE},
    {0xFF, 0xFE, 0xEE, 0xF6},
    {0xFF, 0xFF, 0xFF, 0xFF}
};

s32 seed = 1;

static inline f32 pt_frand(void) {
    seed = seed * 0x5EEDF715 + 0x1B0CB173;
    return (f32)(u32)seed * 2.3283064e-10f;
}

void effParticleDisp(CameraId cameraId, EffectEntry* effect);
void effParticleMain(EffectEntry* effect);
void EmitterEmit(EffectEntry* effect, ParticleEmitter* emitter);
void ParticleEmit(EffectEntry* effect, ParticleItem* p, ParticleEmitter* emitter);

static inline f32 pt_gauss(void) {
    static f32 y2;
    static s32 use_last;
    f32 x1, x2, w, y1;

    if (use_last) {
        y1 = y2;
        use_last = 0;
    } else {
        do {
            x1 = 2.0f * pt_frand() - 1.0f;
            x2 = 2.0f * pt_frand() - 1.0f;
            w = x1 * x1 + x2 * x2;
        } while (w > 1.0f);
        w = (-2.0f * (f32)log(w)) / w;
        w = sqrtf(w);
        y1 = x1 * w;
        y2 = x2 * w;
        use_last = 1;
    }
    return y1;
}

// Stand-in for a function the original linker dead-stripped; its signed int->float constant is last in .rodata.
void particleStrippedPool(f32* f, s32 i) {
    *f = (f32)i;
}

void ParticleEmit(EffectEntry* effect, ParticleItem* p, ParticleEmitter* emitter) {
    ParticleWork* work = effect->userdata;
    f32 w;
    f32 r;

    p->life = (s32)(emitter->lifeStd * pt_gauss() + emitter->lifeMean);
    do {
        r = 2.0f * pt_frand();
        p->vel.x = r - 1.0f;
        r = 2.0f * pt_frand();
        p->vel.z = r - 1.0f;
        w = p->vel.x * p->vel.x + p->vel.z * p->vel.z;
    } while (w > 1.0f);
    p->vel.x *= 0.5f;
    p->vel.z *= 0.5f;
    w *= 0.25f;
    p->vel.y = sqrtf(1.0f - w);
    PSVECScale(&p->vel, &p->vel, work->speed);
    PSMTXMultVec(emitter->mtx, &p->vel, &p->vel);
    PSVECScale(&p->vel, &p->pos, 2.5f);
    PSVECAdd(&emitter->pos, &p->pos, &p->pos);
}

void EmitterEmit(EffectEntry* effect, ParticleEmitter* emitter) {
    ParticleWork* work = effect->userdata;
    u32 prev, n;
    s32 idx;
    u32 i;
    s32 saved;

    prev = (u32)emitter->accum;
    emitter->accum += emitter->rate;
    n = (u32)emitter->accum - prev;
    work->active += n;
    if ((u32)work->active > (u32)work->count) {
        n -= work->active - work->count;
        work->active = work->count;
    }
    if (n != 0) {
        idx = work->count + 1;
        for (i = 0; i < n; i++) {
            idx = work->items[idx].next;
            ParticleEmit(effect, &work->items[idx], emitter);
        }
        saved = work->items[work->count].next;
        work->items[work->count].next = work->items[work->count + 1].next;
        work->items[work->count + 1].next = work->items[idx].next;
        work->items[idx].next = saved;
    }
}

EffectEntry* effParticleEntry(s32 type, f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 count, s32 life) {
    EffectEntry* effect;
    ParticleWork* work;
    ParticleWork* w2;
    u32 i;
    Mtx mx, mz;

    effect = effEntry();
    effect->type = "Particle";
    effect->count = 1;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(ParticleWork) + (count + 2) * sizeof(ParticleItem));
    effect->userdata = work;
    effect->callback = effParticleMain;
    effect->flags |= EFFECT_FLAG_SOFT_DELETABLE;
    work->type = type;
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    work->dir.x = dx;
    work->dir.y = dy;
    work->dir.z = dz;
    if (life <= 0) {
        work->life = 1000;
    } else {
        work->life = life;
    }
    work->frame = 0;
    work->scale = 1.0f;
    work->alpha = 255;
    work->count = count;
    work->items = (ParticleItem*)((u8*)work + effect->count * sizeof(ParticleWork));
    work->speed = 4.0f;
    w2 = effect->userdata;
    for (i = 1; i < w2->count + 2; i++) {
        w2->items[i - 1].next = i;
    }
    w2->items[i - 1].next = 0;
    w2->active = 0;
    PSMTXIdentity(work->emit.mtx);
    work->emit.vecA.x = work->emit.vecA.y = work->emit.vecA.z = 0.0f;
    work->emit.vecB = work->emit.vecA;
    work->emit.accum = 0.0f;
    work->emit.rate = 5.0f;
    work->emit.lifeMean = 125.0f;
    work->emit.lifeStd = 25.0f;
    {
        Vec zero = {0.0f, 0.0f, 0.0f};
        Vec d;
        Vec t = zero;
        t.x = work->dir.x;
        t.y = work->dir.y;
        t.z = work->dir.z;
        d = t;
        PSVECNormalize(&d, &d);
        work->angX = -angleABf(0.0f, 0.0f, 1000.0f * d.z, 1000.0f * d.y);
        work->angY = -angleABf(0.0f, 0.0f, 1000.0f * d.x, 1000.0f * d.y);
        PSMTXRotRad(mx, 'x', 0.017453292f * work->angX);
        PSMTXRotRad(mz, 'z', 0.017453292f * work->angY);
        PSMTXConcat(mx, mz, work->emit.mtx);
    }
    return effect;
}

void effParticleMain(EffectEntry* effect) {
    ParticleWork* work = effect->userdata;
    Vec zero = {0.0f, 0.0f, 0.0f};
    Vec pos;
    Vec t;
    ParticleItem* p;
    ParticleItem* prev;
    ParticleWork* w;
    u32 idx;

    t = zero;
    t.x = work->pos.x;
    t.y = work->pos.y;
    t.z = work->pos.z;
    pos = t;
    if (effect->flags & EFFECT_FLAG_SOFT_DELETE) {
        effect->flags &= ~EFFECT_FLAG_SOFT_DELETE;
        work->life = 16;
    }
    if (work->life < 1000) {
        work->life--;
    }
    if (work->life < 0) {
        effDelete(effect);
        return;
    }
    if (work->life < 16) {
        work->alpha = work->life * 16;
    }
    work->frame++;
    EmitterEmit(effect, &work->emit);
    w = effect->userdata;
    prev = &w->items[w->count];
    idx = prev->next;
    while (idx != w->count + 1) {
        p = &w->items[idx];
        p->life--;
        if (p->life < 0) {
            prev->next = p->next;
            p->next = w->items[w->count + 1].next;
            w->items[w->count + 1].next = idx;
            w->active--;
        } else {
            PSVECAdd(&p->pos, &p->vel, &p->pos);
            p->vel.y += -0.03f;
            prev = p;
        }
        idx = prev->next;
    }
    dispEntry(CAMERA_3D, dispCalcZ(&pos), 2, (DispCallback)effParticleDisp, effect);
}

void effParticleDisp(CameraId cameraId, EffectEntry* effect) {
    ParticleWork* work = effect->userdata;
    CameraEntry* cam = camGetPtr(cameraId);
    s32 alpha = work->alpha;
    GXTexObj texObj;
    Mtx mX, mT, mS, mR;
    f32 hx, hy, ox, oy, ex;
    u32 idx;
    ParticleItem* p;
    GXColor col;
    f32 d2r;

    effGetTexObj(0x61, &texObj);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    PSMTXTrans(mX, work->pos.x, work->pos.y, work->pos.z);
    PSMTXScale(mS, work->scale, work->scale, work->scale);
    PSMTXConcat(mX, mS, mR);
    hx = 4.0f;
    hy = 8.0f;
    d2r = 0.017453292f;
    for (idx = work->items[work->count].next; idx != work->count + 1; idx = p->next) {
        p = &work->items[idx];
        col = hibana_col[p->life >> 3];
        col.a = alpha * col.a / 255;
        GXSetChanMatColor(GX_COLOR0A0, col);
        PSMTXTrans(mX, p->pos.x, p->pos.y, p->pos.z);
        PSMTXConcat(mR, mX, mX);
        PSMTXConcat(cam->view, mX, mX);
        PSMTXRotRad(mT, 'y', d2r * -camGetPtr(cameraId)->viewYaw);
        PSMTXConcat(mX, mT, mX);
        PSMTXRotRad(mT, 'x', d2r * (f32)((gp->retraceCount + idx) % 360 * 10));
        PSMTXConcat(mX, mT, mX);
        GXLoadPosMtxImm(mX, 0);
        GXSetCurrentMtx(0);
        ex = 2.0f;
        ox = -hx / 2.0f;
        oy = -hy / 2.0f;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(ox, hx, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(ex, hx, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(ex, oy, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(ox, oy, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
    }
}
