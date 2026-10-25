#include "eff/eff_kemu_test.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "eff/eff_confetti.h"
#include "eff/eff_hit.h"
#include "eff/n64/eff_stardust2_n64.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"




typedef struct KemuTestWork {
    s32 type;       // 0x0
    Vec pos;        // 0x4
    Vec vel;        // 0x10
    s32 camera;     // 0x1C
    s32 idx0;       // 0x20
    s32 idx1;       // 0x24
    s32 frame;      // 0x28
    s32 pad2C;      // 0x2C
    s32 life;       // 0x30
    f32 gravity;    // 0x34
    f32 gravityAcc; // 0x38
    f32 gravityMul; // 0x3C
    u8 pad40[4];    // 0x40
    s32 delay;      // 0x44
    f32 scale;      // 0x48
    u8 r;           // 0x4C
    u8 g;           // 0x4D
    u8 b;           // 0x4E
    u8 pad4F;
    f32 rx; // 0x50
    f32 rz; // 0x54
} KemuTestWork;

static void effKemuTestMain(EffectEntry* effect);
void effKemuTestDisp(CameraId cameraId, void* param);

#pragma defer_codegen off

typedef struct { f32 v[3]; } KemuF3;
typedef struct { f32 v[4]; } KemuF4;

static const KemuF3 kemu_c = { { 0.5f, 0.3f, 0.8f } };
static const KemuF3 kemu_a = { { 0.0f, 10.0f, 10.0f } };
static const KemuF3 kemu_b = { { 0.0f, 5.0f, 20.0f } };
static const KemuF4 kemu_d = { { 0.0f, 120.0f, 180.0f, 240.0f } };
static const KemuF3 kemu_e = { { 0.0f, 120.0f, 240.0f } };
static const KemuF3 kemu_f = { { 0.0f, 5.0f, -5.0f } };
static const KemuF3 kemu_g = { { 5.0f, 0.0f, -2.0f } };
static const KemuF3 kemu_h = { { 0.0f, 1.0f, 2.0f } };
static const Vec kemu_pos = { 0.0f, 0.0f, 0.0f };
static const Vec kemu_tmp = { 0.0f, 0.0f, 0.0f };

void effKemuTestSetRxRz(EffectEntry* effect, f32 rx, f32 rz) {
    KemuTestWork* work = effect->userdata;
    work->rx = rx;
    work->rz = rz;
}

void effKemuTestDrawCam(EffectEntry* effect, s32 camera) {
    ((KemuTestWork*)effect->userdata)->camera = camera;
}


#pragma opt_loop_invariants off
void effKemuTestDisp(CameraId cameraId, void* param) {
    EffectEntry* effect = param;
    Mtx trans;
    Mtx scl;
    Mtx rot;
    Mtx view;
    GXTexObj tex;
    KemuTestWork* work = effect->userdata;
    CameraEntry* camera = camGetPtr(cameraId);
    s32 type = work->type;
    s32 i;
    s32 start = 0;
    s32 alpha = 0xff;
    f32 ext;
    f32 neg;
    f32 half;
    f32 w;

    PSMTXTrans(trans, work->pos.x, work->pos.y, work->pos.z);
    if (type != 11) {
        PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
    } else {
        PSMTXRotRad(rot, 'x', 0.017453292f * work->rx);
        PSMTXRotRad(scl, 'z', 0.017453292f * work->rz);
        PSMTXConcat(rot, scl, rot);
    }
    PSMTXScale(scl, work->scale, work->scale, work->scale);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scl, trans);
    PSMTXConcat(camera->view, trans, view);
    effGetTexObj(0x28, &tex);
    GXLoadTexObj(&tex, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1e, 0, 0x7d);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7d);
    GXSetNumTevStages(3);
    GXSetTevOrder(0, 0, 0, 0xff);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xf, 0xf, 0xf, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXSetTevOrder(1, 1, 0, 0xff);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0, 8, 2, 0xf);
    GXSetTevAlphaIn(1, 0, 4, 1, 7);
    GXSetTevColor(1, (GXColor){ 0x80, 0x80, 0x80, 0x80 });
    GXSetTevOrder(2, 0xff, 0xff, 4);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 0xf, 0, 0xa, 0xf);
    GXSetTevAlphaIn(2, 7, 0, 5, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xd, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xd, 1, 4, 0);
    switch (type) {
    case 1:
        start = 1;
        alpha = 0x40;
        break;
    case 3:
        alpha = 0xc8;
        break;
    case 10:
    case 12:
        start = 1;
        alpha = 0xc8;
        break;
    }
    w = 32.0f;
    neg = -w;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay == 0 && work->idx0 <= 15) {
            s32 pass;
            PSMTXTrans(trans, 0.0625f * (f32)work->idx0, 0.0f, 0.0f);
            PSMTXScale(scl, 0.0625f, 1.0f, 1.0f);
            PSMTXConcat(trans, scl, trans);
            GXLoadTexMtxImm(trans, 0x1e, 1);
            PSMTXTrans(trans, 0.0625f * (f32)work->idx1, 0.0f, 0.0f);
            PSMTXScale(scl, 0.0625f, 1.0f, 1.0f);
            PSMTXConcat(trans, scl, trans);
            GXLoadTexMtxImm(trans, 0x21, 1);
            ext = 16.0f;
            for (pass = start; pass < 2; pass++) {
                if (pass == 0) {
                    GXSetChanMatColor(4, (GXColor){ 0, 0, 0, 0x60 });
                    PSMTXTrans(trans, 2.0f + work->pos.x, work->pos.y - 2.0f, work->pos.z);
                PSMTXScale(scl, work->scale, work->scale, work->scale);
                PSMTXConcat(view, trans, trans);
                if (type == 11) {
                    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
                    PSMTXConcat(trans, rot, trans);
                }
                PSMTXConcat(trans, scl, trans);
                GXLoadPosMtxImm(trans, 0);
                } else {
                    GXSetChanMatColor(4, (GXColor){ work->r, work->g, work->b, alpha });
                    PSMTXTrans(trans, work->pos.x, work->pos.y, work->pos.z);
                PSMTXScale(scl, work->scale, work->scale, work->scale);
                PSMTXConcat(view, trans, trans);
                if (type == 11) {
                    PSMTXRotRad(rot, 'y', 0.017453292f * -camGetPtr(cameraId)->viewYaw);
                    PSMTXConcat(trans, rot, trans);
                }
                PSMTXConcat(trans, scl, trans);
                GXLoadPosMtxImm(trans, 0);
                }
                half = neg / 2.0f;
                GXBegin(0x80, 0, 4);
                GXPosition3f32(half, ext, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);
                GXPosition3f32(ext, ext, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);
                GXPosition3f32(ext, half, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);
                GXPosition3f32(half, half, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
            }
        }
    }
}

#pragma opt_loop_invariants reset

static void effKemuTestMain(EffectEntry* effect) {
    KemuTestWork* work = effect->userdata;
    Vec pos;
    Vec tmp = kemu_tmp;
    s32 type;
    s32 done = 0;
    s32 i;
    s32 play = 0;
    s32 camera;

    tmp.x = work->pos.x;
    tmp.y = work->pos.y;
    tmp.z = work->pos.z;
    pos = tmp;
    type = work->type;
    camera = work->camera;
    work++;
    for (i = 1; i < effect->count; i++, work++) {
        if (work->delay) {
            work->delay--;
            continue;
        }
        work->idx0 = (s32)(16.0f * (f32)work->frame / (f32)work->life);
        work->idx1 = (s32)(16.0f * (f32)(work->frame + 1) / (f32)work->life);
        if (work->idx1 > 15) {
            work->idx1 = 15;
        }
        work->frame++;
        switch (type) {
        case 1:
        case 3:
        case 19:
            work->pos.y += work->gravity;
            work->gravity += work->gravityAcc;
            work->gravityAcc *= work->gravityMul;
            break;
        case 10:
        case 14: {
            f32 a = 6.2831855f * (90.0f + 180.0f * (f32)(i - 1) / (f32)(effect->count - 2)) / 360.0f;
            work->pos.x += work->gravity * (f32)sin(a);
            work->pos.y += -work->gravity * (f32)cos(a);
            work->gravity += work->gravityAcc;
            work->gravityAcc *= work->gravityMul;
            break;
        }
        case 4:
        case 9:
        case 17:
        case 20:
            work->pos.x += work->vel.x;
            work->pos.y += work->vel.y;
            work->vel.x *= 0.9f;
            work->vel.y *= 0.9f;
            break;
        case 5:
        case 21:
            work->pos.x += work->vel.x;
            work->pos.y += work->vel.y;
            work->vel.x *= 0.9f;
            work->vel.y *= 0.9f;
            break;
        case 6:
            work->pos.x += work->vel.x;
            work->vel.x *= 0.95f;
            work->pos.y += work->gravity;
            work->gravity += work->gravityAcc;
            work->gravityAcc *= work->gravityMul;
            break;
        case 7:
        case 8:
        case 16:
            work->pos.x += work->vel.x;
            work->pos.y += work->vel.y;
            work->vel.x *= 0.9f;
            work->vel.y *= 0.9f;
            break;
        case 11:
            work->pos.x += work->vel.x;
            work->pos.y += work->vel.y;
            work->pos.z += work->vel.z;
            work->vel.x *= 0.9f;
            work->vel.y *= 0.9f;
            work->vel.z *= 0.9f;
            break;
        case 12: {
            f32 a = 6.2831855f * (90.0f + 360.0f * (f32)(i - 1) / (f32)(effect->count - 2)) / 360.0f;
            work->pos.x += work->gravity * (f32)sin(a);
            work->pos.y += -work->gravity * (f32)cos(a);
            work->gravity += work->gravityAcc;
            work->gravityAcc *= work->gravityMul;
            work->scale += 0.05f;
            break;
        }
        case 13:
            work->pos.x += work->vel.x;
            work->pos.y += work->vel.y;
            work->vel.x *= 0.9f;
            work->vel.y *= 0.9f;
            work->pos.y += work->gravity;
            work->gravity += work->gravityAcc;
            work->gravityAcc *= work->gravityMul;
            break;
        }
        switch (type) {
        case 3:
        case 6:
        case 11:
        case 16:
        case 18: {
            s32 r, g, b;
            g = 0xff;
            r = 0xff;
            b = 0xff - work->frame * 60;
            if (b < 0) {
                g = (s32)(0.8f * (f32)b) + 0xff;
                b = 0;
                if (g < 0) {
                    r = (s32)(0.4f * (f32)g) + 0xff;
                    g = 0;
                    if (r < 0) {
                        r = 0;
                    }
                }
            }
            work->r = r;
            work->g = g;
            work->b = b;
            if (type == 3 && i == 1 && work->frame == 0x1e) {
                play = 1;
            }
            break;
        }
        case 7:
            work->r += (s32)((f32)(0xbd - work->r) / 10.0f);
            work->g += (s32)((f32)(0x10 - work->g) / 10.0f);
            work->b += (s32)((f32)(0xaa - work->b) / 10.0f);
            break;
        case 12:
            work->r += (s32)((f32)(0x82 - work->r) / 64.0f);
            work->g += (s32)((f32)(0x82 - work->g) / 64.0f);
            work->b += (s32)((f32)(0x82 - work->b) / 64.0f);
            break;
        }
        if (work->frame >= work->life) {
            done++;
        }
    }
    if (play) {
        psndSFXOn_3D("SFX_BTL_STAGE_BURST1", &pos);
        effConfettiEntry(6, 0x3c, pos.x, pos.y, pos.z);
    }
    if (done >= effect->count - 1) {
        effDelete(effect);
    } else {
        dispEntry(camera, dispCalcZ(&pos), 2, effKemuTestDisp, effect);
    }
}


EffectEntry* effKemuTestEntry(s32 type, f32 x, f32 y, f32 z, f32 scale) {
    static f32 bom_dir[10] = { 10.0f, 40.0f, 80.0f, 170.0f, 140.0f, 100.0f, 25.0f, 155.0f, 60.0f, 120.0f };
    static f32 bom_spd[10] = { 2.2f, 2.7f, 3.0f, 2.2f, 2.7f, 3.0f, 1.9f, 1.9f, 1.5f, 1.5f };
    EffectEntry* effect;
    KemuTestWork* work;
    s32 i;
    s32 count;
    s32 n;

    effect = effEntry();
    count = 1;
    switch (type) {
    case 0:
    case 1:
    case 15:
    case 18:
        count = 2;
        break;
    case 2:
    case 16:
        count = 5;
        break;
    case 3:
        count = 0x21;
        break;
    case 4:
        count = 0xb;
        break;
    case 5:
        count = 4;
        break;
    case 6:
        count = 7;
        break;
    case 7:
        count = 7;
        break;
    case 8:
        count = 0x41;
        break;
    case 9:
        count = 0x15;
        break;
    case 10:
        count = 7;
        break;
    case 11:
        count = 9;
        break;
    case 12:
        count = irand(3) + 4;
        break;
    case 13:
        count = 0xb;
        break;
    case 14:
        count = 5;
        break;
    case 17:
        count = 0xb;
        scale *= 0.5f;
        break;
    case 19:
        count = 4;
        break;
    case 20:
        count = 0xb;
        scale *= 0.5f;
        break;
    case 21:
        count = 3;
        break;
    case 22:
        count = 4;
        break;
    }
    effect->type = "KemuTest";
    effect->count = count;
    work = __memAlloc(HEAP_EFFECT, effect->count * sizeof(KemuTestWork));
    effect->userdata = work;
    effect->callback = effKemuTestMain;
    work->type = type;
    work->pos.x = x;
    work->pos.y = y;
    work->pos.z = z;
    work->frame = 0;
    work->delay = 0;
    work->scale = scale;
    work->camera = 4;
    work->rx = 0.0f;
    work->rz = 0.0f;
    work++;
    for (i = 1; i < (count = effect->count); i++, work++) {
        switch (type) {
        case 0:
        case 15:
        case 18:
            work->pos.x = 0.0f;
            work->pos.y = 0.0f;
            work->pos.z = 0.0f;
            work->life = 0x20;
            work->frame = 0;
            work->delay = 0;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            work->scale = 1.0f;
            if (type == 15) {
                work->r = 0x50;
                work->g = 0x4b;
                work->b = 0x4b;
            }
            break;
        case 1:
        case 19:
            work->pos.x = 0.0f;
            work->pos.y = 0.0f;
            work->pos.z = 0.0f;
            work->life = 0x80;
            work->frame = 0;
            work->gravity = 0.95f;
            work->gravityAcc = -0.04f;
            work->gravityMul = 0.94f;
            work->delay = 0;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            work->scale = 1.0f;
            if (type == 19) {
                KemuF3 c = kemu_c;
                KemuF3 a = kemu_a;
                KemuF3 b = kemu_b;
                work->pos.x = a.v[i - 1];
                work->pos.y = b.v[i - 1];
                work->scale = c.v[i - 1];
                work->delay = i * 4;
                work->r = 0x78;
                work->g = 0x78;
                work->b = 0x78;
                work->gravity *= 0.8f;
            }
            break;
        case 2:
        case 16: {
            f32 ang = 6.2831855f * (f32)irand(0x168) / 360.0f;
            work->pos.x = 20.0f * (f32)sin(ang);
            work->pos.y = 20.0f * (f32)cos(ang);
            work->pos.z = 0.0f;
            work->life = 0x20;
            work->frame = 0;
            work->delay = (i - 1) * 6;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            work->scale = 1.0f;
            break;
        }
        case 3: {
            f32 ang = 6.2831855f * (f32)irand(0x168) / 360.0f;
            work->pos.x = 20.0f * (f32)sin(ang);
            work->pos.y = 20.0f * (f32)cos(ang);
            work->pos.z = 0.0f;
            work->life = 0x20;
            work->frame = 0;
            work->delay = (i - 1) * 3;
            work->gravity = 0.95f;
            work->gravityAcc = -0.04f;
            work->gravityMul = 0.94f;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            work->scale = 1.0f;
            break;
        }
        case 4:
        case 5:
        case 9:
        case 10:
        case 13:
        case 14:
        case 17:
        case 20:
        case 21:
            work->pos.x = 0.0f;
            work->pos.y = 0.0f;
            work->pos.z = 0.0f;
            work->life = 0x20;
            work->frame = 0;
            work->delay = 0;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            work->delay = 0;
            if (type == 4 || type == 13) {
                f32 ang;
                f32 spd;
                f32 d;
                d = bom_dir[i - 1];
                spd = 2.0f * bom_spd[i - 1];
                ang = d;
                ang = 6.2831855f * (90.0f + ang) / 360.0f;
                work->vel.x = spd * (f32)sin(ang);
                work->vel.y = -spd * (f32)cos(ang);
                work->scale = 1.0f;
                work->delay = irand(0x1e);
                if (type == 13) {
                    work->gravity = 0.59999996f;
                    work->gravityAcc = -0.08f;
                    work->gravityMul = 0.94f;
                    work->r = 0xaa;
                    work->g = 0x82;
                    work->b = 0xe6;
                    work->vel.x *= 0.8f;
                    work->vel.y *= 0.8f;
                    work->life *= 2;
                }
            } else if (type == 5) {
                KemuF4 d = kemu_d;
                f32 spd = 3.0f;
                f32 s = (f32)sin(6.2831855f * d.v[i] / 360.0f);
                work->vel.x = spd * s;
                work->vel.y = -spd * (f32)cos(6.2831855f * d.v[i] / 360.0f);
                work->scale = 0.5f;
            } else if (type == 21) {
                KemuF3 e = kemu_e;
                f32 spd = 3.0f;
                f32 s = (f32)sin(6.2831855f * e.v[i] / 360.0f);
                work->vel.x = spd * s;
                work->vel.y = -spd * (f32)cos(6.2831855f * e.v[i] / 360.0f);
                work->scale = 0.5f;
            } else if (type == 9 || type == 17 || type == 20) {
                f32 ang = (f32)irand(0x168);
                f32 a2;
                f32 spd;
                f32 d;
                if (i < (effect->count - 1) / 2 + 1) {
                    spd = 4.5f;
                } else {
                    spd = 3.0f;
                }
                a2 = 6.2831855f * ang / 360.0f;
                work->vel.x = spd * (f32)sin(a2);
                work->vel.y = -spd * (f32)cos(a2);
                work->scale = 1.0f;
                work->delay = (i < (effect->count - 1) / 2 + 1) ? 0 : 8;
                if (type == 9) {
                    work->r = 0xf8;
                    work->g = 0xc0;
                    work->b = 0xde;
                } else if (type == 17) {
                    work->r = 0xff;
                    work->g = 0xff;
                    work->b = 0xff;
                } else if (type == 20) {
                    work->r = 0xff;
                    work->g = 0x97;
                    work->b = 0x10;
                }
            } else if (type == 10 || type == 14) {
                work->gravity = 0.95f;
                work->gravityAcc = -0.04f;
                work->gravityMul = 0.94f;
                work->life = 0x40;
                work->scale = 1.0f;
                work->delay = 0;
                if (type == 10) {
                    work->r = 0xaf;
                    work->g = 0xa5;
                    work->b = 0x9b;
                }
            }
            break;
        case 6:
            work->pos.x = (f32)(irand(0x28) - 0x14);
            work->pos.y = (f32)(irand(0x14) - 0xa);
            work->pos.z = 0.0f;
            work->life = 0x40;
            work->frame = 0;
            work->delay = irand(10);
            work->gravity = 0.59999996f;
            work->gravityAcc = -0.04f;
            work->gravityMul = 0.94f;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            work->scale = 0.6f;
            {
                f32 spd = 1.0f;
                work->vel.x = spd * (f32)sin(6.2831855f * angleABf(0.0f, 0.0f, work->pos.x, work->pos.y) / 360.0f);
            }
            break;
        case 7: {
            f32 a, s;
            s = 1.0f + (f32)irand(0x28) / 10.0f;
            work->pos.x = 0.0f;
            work->pos.y = 0.0f;
            work->pos.z = 0.0f;
            work->life = 0x3c;
            work->frame = 0;
            a = 6.2831855f * (135.0f - 90.0f * ((f32)(i - 1) / (f32)(count - 2))) / 360.0f;
            work->vel.x = s * (f32)cos(a);
            work->vel.y = s * (f32)sin(a);
            work->delay = irand(0x10);
            work->r = 0xdc;
            work->g = 0xc3;
            work->b = 0xe9;
            work->scale = 1.0f;
            break;
        }
        case 8: {
            f32 ang = (f32)irand(0x168);
            f32 a;
            f32 s;
            s = 2.0f * ((f32)irand(0x28) / 10.0f) + 1.0f;
            a = 6.2831855f * ang / 360.0f;
            work->pos.x = 0.0f;
            work->pos.y = 0.0f;
            work->pos.z = 0.0f;
            work->life = 0x20;
            work->frame = 0;
            work->vel.x = s * (f32)cos(a);
            work->vel.y = s * (f32)sin(a);
            work->delay = i % 32;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            work->scale = 1.0f;
            break;
        }
        case 11: {
            f32 a = 6.2831855f * (360.0f * (f32)(i - 1) / (f32)(count - 2)) / 360.0f;
            work->pos.x = 0.0f;
            work->pos.y = 0.0f;
            work->pos.z = 0.0f;
            work->vel.x = 5.0f * (f32)sin(a);
            work->vel.y = 0.0f;
            work->vel.z = 5.0f * (f32)cos(a);
            work->life = 0x40;
            work->frame = 0;
            work->delay = 0;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            work->scale = 1.0f;
            break;
        }
        case 12:
            work->gravity = 0.95f;
            work->gravityAcc = -0.04f;
            work->gravityMul = 0.94f;
            work->life = 0x60;
            work->scale = 0.0f;
            work->delay = 0;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            break;
        case 22: {
            KemuF3 f = kemu_f;
            KemuF3 g = kemu_g;
            KemuF3 h = kemu_h;
            work->pos.x = f.v[i - 1];
            work->pos.y = g.v[i - 1];
            work->pos.z = h.v[i - 1];
            work->vel.x = 0.0f;
            work->vel.y = 0.0f;
            work->vel.z = 0.0f;
            work->life = 0x20;
            work->frame = 0;
            work->r = 0xff;
            work->g = 0xff;
            work->b = 0xff;
            work->delay = 0;
            work->scale = 0.5f;
            break;
        }
        }
    }
    if (type == 3) {
        Vec pos = kemu_pos;
        pos.x = x;
        pos.y = y;
        pos.z = z;
        psndSFXOn_3D("SFX_BTL_STAGE_BURST1", &pos);
        effConfettiEntry(6, 0x3c, x, y, z);
    }
    if (type == 16) {
        effHitEntry(1, 3, x, y, z, scale);
    }
    if (type == 20) {
        effStardust2N64Entry(0, 8, 0x1e, x, y, z, 50.0f * scale, 50.0f * scale);
    }
    return effect;
}


static __declspec(section ".rodata") const u32 gap_04_802FDD34_rodata[1] = {0};
#ifndef VERSION_G8ME01
__declspec(section ".data") u8 gap_05_803AC064_data[28] = {0};
#endif


