#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/effdrv.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr.h"
#include "mario/mariost.h"
#include "memory.h"
#include "system.h"
#include "pmario_sound.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include "eff/n64/eff_water_damage_n64.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "eff/n64/eff_kemuri1_n64.h"

typedef struct EffFallWork {
    s32 type;       // 0x0
    Vec position;   // 0x4
    u8 pad10[0x6C - 0x10];
    Vec pos2;       // 0x6C
    u8 pad78[0x80 - 0x78];
    f32 angle;      // 0x80
    u8 pad84[0x8C - 0x84];
    s32 timer;      // 0x8C
    u8 pad90[0x94 - 0x90];
    f32 scale;      // 0x94
    u8 pad98[0xA0 - 0x98];
    s32 state;      // 0xA0
    u32 alpha;      // 0xA4
    u8 padA8[0xB0 - 0xA8];
    f32 rotation;   // 0xB0
    u8 padB4[0xB8 - 0xB4];
    Vec pos3;       // 0xB8
    s32 pose0;      // 0xC4
    s32 pose1;      // 0xC8
} EffFallWork;

typedef struct EffFallVtx {
    f32 x, y, z;
    f32 u, v;
} EffFallVtx;

typedef struct EffFallElem {
    s32 type;         // 0x0
    Vec pos;          // 0x4
    f32 width;        // 0x10
    f32 drop;         // 0x14
    f32 rotY;         // 0x18
    s32 num;          // 0x1C
    s32 duration;     // 0x20
    s32 timer;        // 0x24
    s32 scroll;       // 0x28
    f32 scale;        // 0x2C
    f32 f30;          // 0x30
    f32 f34;          // 0x34
    s32 state;        // 0x38
    s32 alpha;        // 0x3C
    EffFallVtx** vtx; // 0x40
    f32 f44;          // 0x44
    f32 rot2;         // 0x48
    f32 rot3;         // 0x4C
    Vec vel;          // 0x50
    s32 pose0;        // 0x5C
    s32 pose1;        // 0x60
    s32 sfx;          // 0x64
} EffFallElem;

static s16 shibuki_vertex_tbl[] __attribute__((aligned(32))) = { -14012, 946, -2560, -7355, 946, -2560, -697, 946, -2560, 5959, 946, -2560, 12617, 946, -2560, -17337, 7417, -2560, -8130, 10717, -2560, -697, 12271, -2560, 6734, 10717, -2560, 15942, 7417, -2560, -23097, 11535, -2560, -11577, 18594, -2560, -697, 20431, -2560, 10822, 18594, -2560, 21702, 11535, -2560, -14777, 3888, -2560, -7591, 3922, -2560, -697, 3922, -2560, 6195, 3922, -2560, 13262, 3922, -2560, -19292, 8815, -2560, -9810, 14556, -2560, -697, 16241, -2560, 8831, 14757, -2560, 17896, 8815, -2560, -12537, 0, -3520, 262, 0, -3520, 13062, 0, -3520, -12537, 1702, -3520, 262, 4006, -3520, 13062, 1702, -3520, -12537, 4940, -3520, 262, 8012, -3520, 13062, 4940, -3520, 10533, 5964, -3520, 10439, 2726, -3520, 10468, 0, -3520, -10037, 5964, -3520, -10021, 2726, -3520, -10006, 0, -3520};
static s16 shibuki_texcoord0_tbl[] __attribute__((aligned(32))) = { 0, 12018, 4096, 12749, 4096, 14745, 0, 14745, 8192, 13023, 8192, 14745, 12288, 12749, 12288, 14745, 16384, 12018, 16384, 14745, 0, 1638, 4096, 1638, 4096, 4997, 0, 5968, 8192, 1638, 8192, 5003, 12288, 1638, 12288, 4830, 16384, 1638, 16384, 5968, 0, 8192, 4096, 8192, 8192, 8192, 12288, 8192, 16384, 8192, -3220, 8192, -16384, 8192, -16384, 16384, -3240, 16384, 13026, 8192, 16384, 16384, 13064, 16384, -3200, 0, -16384, 0, 13147, 0, 16384, 0, 0, 0, 0, 16384};
static u8 shibuki_dl_0_0[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0A, 0x0A, 0x00, 0x00, 0x0A, 0x0B, 0x00, 0x00, 0x0B, 0x14, 0x00, 0x00, 0x0D, 0x15, 0x00, 0x02, 0x0C, 0x05, 0x00, 0x00, 0x14, 0x06, 0x00, 0x03, 0x15, 0x0F, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 shibuki_dl_0_1[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0A, 0x0B, 0x00, 0x00, 0x0B, 0x0C, 0x00, 0x00, 0x0E, 0x15, 0x00, 0x02, 0x0C, 0x16, 0x00, 0x02, 0x0F, 0x06, 0x00, 0x03, 0x15, 0x07, 0x00, 0x03, 0x16, 0x10, 0x00, 0x01, 0x01, 0x11, 0x00, 0x01, 0x04, 0x01, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 shibuki_dl_0_2[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0A, 0x0C, 0x00, 0x00, 0x0E, 0x0D, 0x00, 0x00, 0x10, 0x16, 0x00, 0x02, 0x0F, 0x17, 0x00, 0x02, 0x11, 0x07, 0x00, 0x03, 0x16, 0x08, 0x00, 0x03, 0x17, 0x11, 0x00, 0x01, 0x04, 0x12, 0x00, 0x01, 0x06, 0x02, 0x00, 0x00, 0x05, 0x03, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 shibuki_dl_0_3[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x0A, 0x0D, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x12, 0x17, 0x00, 0x02, 0x11, 0x18, 0x00, 0x00, 0x13, 0x08, 0x00, 0x03, 0x17, 0x09, 0x00, 0x00, 0x18, 0x12, 0x00, 0x01, 0x06, 0x13, 0x00, 0x00, 0x08, 0x03, 0x00, 0x00, 0x07, 0x04, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static s8 shibuki_normal_tbl[] __attribute__((aligned(32))) = { 0, 0, 64};
static u8 shibuki_color0_tbl[] __attribute__((aligned(32))) = { 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0x19, 0xFF, 0xFF, 0xFF, 0x4C, 0xB6, 0xCC, 0xCB, 0x33, 0xB6, 0xCC, 0xCB, 0x4C, 0xB6, 0xCC, 0xCB, 0x99, 0xB6, 0xCC, 0xCB, 0x00 };
static u8 shibuki_dl_1_0[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x1F, 0x00, 0x07, 0x24, 0x25, 0x00, 0x04, 0x20, 0x1C, 0x00, 0x07, 0x14, 0x26, 0x00, 0x04, 0x19, 0x19, 0x00, 0x07, 0x25, 0x27, 0x00, 0x06, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 shibuki_dl_1_1[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x20, 0x00, 0x04, 0x24, 0x22, 0x00, 0x04, 0x22, 0x1D, 0x00, 0x05, 0x14, 0x23, 0x00, 0x04, 0x1D, 0x1A, 0x00, 0x06, 0x25, 0x24, 0x00, 0x06, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 shibuki_dl_1_2[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x22, 0x00, 0x04, 0x22, 0x21, 0x00, 0x07, 0x23, 0x23, 0x00, 0x04, 0x1D, 0x1E, 0x00, 0x07, 0x18, 0x24, 0x00, 0x06, 0x1F, 0x1B, 0x00, 0x07, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00 };
static u8 shibuki_dl_1_3[] __attribute__((aligned(32))) = { 0x98, 0x00, 0x06, 0x25, 0x00, 0x04, 0x20, 0x20, 0x00, 0x04, 0x21, 0x26, 0x00, 0x04, 0x19, 0x1D, 0x00, 0x05, 0x1A, 0x27, 0x00, 0x06, 0x1C, 0x1A, 0x00, 0x06, 0x1B, 0x00, 0x00, 0x00, 0x00, 0x00 };

s32 evt_cam_shake(EventEntry* event, BOOL isFirstCall);

static EVT_BEGIN(evt_shake)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.009765625f), 300)
    RETURN
EVT_END()

void effFallMain(EffectEntry* effect);
void effFallDisp(CameraId cameraId, EffectEntry* effect);
void effFallDispInseki(CameraId cameraId, EffectEntry* effect);
void effFallDispWater(CameraId cameraId, EffectEntry* effect);

static inline f32 fall_sinf(f32 x) {
    f32 a = 0.00761f;
    f32 b = 0.16605f;
    f32 c = 1.0f;
    f32 x2;

    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = a * x;
        }
        else {
            x = 1.5707964f - (x - 1.5707964f);
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = a * x;
        }
    }
    else if (x < 4.712389f) {
        x = x - 3.1415927f;
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
        a = a * x;
        a = -a;
    }
    else {
        x = 1.5707964f - (x - 4.712389f);
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
        a = a * x;
        a = -a;
    }
    return a;
}

static inline f32 fall_cosf(f32 x) {
    f32 a = 0.03705f;
    f32 b = 0.4967f;
    f32 c = 1.0f;
    f32 x2;

    if (x <= 3.1415927f) {
        if (x <= 1.5707964f) {
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
        }
        else {
            x = 1.5707964f - (x - 1.5707964f);
            x2 = x * x;
            a = a * x2 - b;
            a = a * x2 + c;
            a = -a;
        }
    }
    else if (x < 4.712389f) {
        x = x - 3.1415927f;
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
        a = -a;
    }
    else {
        x = 1.5707964f - (x - 4.712389f);
        x2 = x * x;
        a = a * x2 - b;
        a = a * x2 + c;
    }
    return a;
}

EffectEntry* effFallEntry(s32 type, f32 x, f32 y, f32 z, f32 width, f32 drop, f32 dir, s32 num, s32 duration);
void DCFlushRange(void* addr, u32 nBytes);
void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts);
void GXCallDisplayList(void* list, u32 nbytes);
void GXClearVtxDesc(void);
u16 GXGetTexObjHeight(const GXTexObj* to);
void GXLoadPosMtxImm(const f32 mtx[3][4], u32 id);
void GXLoadTexMtxImm(const f32 mtx[][4], u32 id, GXTexMtxType type);
void GXLoadTexObj(GXTexObj* obj, GXTexMapID id);
void GXSetArray(GXAttr attr, void* base_ptr, u8 stride);
void GXSetChanCtrl(GXChannelID chan, GXBool enable, GXColorSrc amb_src, GXColorSrc mat_src, u32 light_mask, GXDiffuseFn diff_fn, GXAttnFn attn_fn);
void GXSetChanMatColor(GXChannelID chan, GXColor mat_color);
void GXSetCullMode(GXCullMode mode);
void GXSetCurrentMtx(u32 id);
void GXSetNumChans(u8 nChans);
void GXSetNumTevStages(u8 nStages);
void GXSetNumTexGens(u8 nTexGens);
void GXSetTevAlphaIn(GXTevStageID stage, GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d);
void GXSetTevAlphaOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale, GXBool clamp, GXTevRegID out_reg);
void GXSetTevColor(GXTevRegID id, GXColor color);
void GXSetTevColorIn(GXTevStageID stage, GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d);
void GXSetTevColorOp(GXTevStageID stage, GXTevOp op, GXTevBias bias, GXTevScale scale, GXBool clamp, GXTevRegID out_reg);
void GXSetTevOp(GXTevStageID id, GXTevMode mode);
void GXSetTevOrder(GXTevStageID stage, GXTexCoordID coord, GXTexMapID map, GXChannelID color);
void GXSetTexCoordGen2(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, u32 mtx, GXBool normalize, u32 pt_texmtx);
void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac);
void GXSetVtxDesc(GXAttr attr, GXAttrType type);
EffectEntry* effFallEntry(s32 type, f32 x, f32 y, f32 z, f32 width, f32 drop, f32 dir, s32 num, s32 duration) {
    const char* sfxName;
    EffectEntry* effect;
    EffFallElem* e;
    s32 i;
    EffFallElem* w;
    s32 count;
    s32 ia;
    f32 a;
    f32 r;

    effect = effEntry();
    count = num + 1;
    switch (type) {
    case 0:
        count = num + 1;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        num = 1;
        count = 2;
        break;
    }
    effect->type = "Fall";
    effect->count = count;
    w = __memAlloc(HEAP_EFFECT, effect->count * sizeof(EffFallElem));
    effect->userdata = w;
    effect->callback = effFallMain;
    effect->flags |= 2;
    // FAKE: comma temps are coloured in assignment order (ia r30 above sfxName r29)
    ia = (0, (s32)width);
    sfxName = (0, "SFX_BTL_STAGE_FALL_WATER1");
    w->type = type;
    w->pos.x = x;
    w->pos.y = y;
    w->pos.z = z;
    w->width = width;
    w->drop = drop;
    w->rotY = dir;
    w->num = num;
    w->timer = 0;
    w->scroll = 0;
    w->duration = duration;
    w->pose0 = -1;
    w->pose1 = -1;
    for (i = 1, e = w + 1; i < effect->count; i++, e++) {
        a = 3.1415927f * irand(360) / 180.0f;
        r = irand(ia);
        e->pos.x = r * (f32)sin(a);
        e->pos.y = 300.0f + irand(effect->count);
        e->pos.z = r * (f32)cos(a);
        e->timer = 0;
        e->scroll = 0;
        e->alpha = 255;
        e->rot2 = 0.0f;
        e->rotY = irand(100) > 50 ? 0 : 180;
        e->state = 0;
        e->pose0 = -1;
        e->pose1 = -1;
        switch (type) {
            case 0:
            case 1:
                e->f34 = 0.0f;
                e->f30 = 0.2f;
                if (i == 1) {
                    e->pos.x = 0.0f;
                    e->pos.z = 0.0f;
                    e->timer = 0;
                } else {
                    e->timer = irand(effect->count) + 60;
                }
                e->scale = type == 0 ? 0.5f : 1.2f;
                break;
            case 2:
                e->f34 = 0.0f;
                e->f30 = 1.6f;
                e->pos.x = 0.0f;
                e->pos.z = 0.0f;
                e->timer = 0;
                e->f44 = 0.0f;
                e->scale = 0.5f;
                break;
            case 3: {
                e->f34 = 0.0f;
                e->f30 = 0.1f;
                e->pos.x = 0.0f;
                e->pos.y = 200.0f;
                e->pos.z = 0.0f;
                e->vel.x = 0.0f;
                e->vel.y = 0.0f;
                e->vel.z = 0.0f;
                e->scale = 1.0f;
                {
                    Vec v = { 0.0f, 0.0f, 0.0f };
                    v.x = x;
                    v.y = y;
                    v.z = z;
                    e->sfx = psndSFXOn_3D(sfxName, &v);
                }
                break;
            }
            case 4:
            case 5:
                e->f34 = 0.0f;
                e->f30 = 0.6f;
                e->pos.x = 0.0f;
                e->pos.z = 0.0f;
                e->timer = 0;
                e->scale = 1.0f;
                break;
            case 6:
                e->pos.x = 0.0f;
                e->pos.y = 200.0f;
                e->pos.z = 0.0f;
                e->vel.x = -e->pos.x / 30.0f;
                e->vel.y = -e->pos.y / 30.0f;
                e->vel.z = -e->pos.z / 30.0f;
                e->scale = 1.3f;
                e->rot2 = 30.0f;
                e->rot3 = 0.0f;
                break;
            case 7:
                e->pos.x = 400.0f;
                e->pos.y = 200.0f;
                e->pos.z = 0.0f;
                e->scale = 0.5f;
                break;
            case 8:
                e->f34 = 0.0f;
                e->f30 = 0.25f;
                e->pos.x = 0.0f;
                e->pos.y = 400.0f;
                e->pos.z = 0.0f;
                e->scale = 1.0f;
                break;
        }
    }
    return effect;
}
#pragma opt_propagation off
#pragma opt_lifetimes off

static inline void effFallRelease(EffectEntry* effect) {
    s32 i;
    EffFallElem* e;
    for (i = 0, e = effect->userdata; i < effect->count; i++, e++) {
        if (e->pose0 != -1) {
            animPoseRelease(e->pose0);
        }
        if (e->pose1 != -1) {
            animPoseRelease(e->pose1);
        }
        psndSFXOff(e->sfx);
    }
}
// FAKE: the seven sfx names are spelled SFX_NAME_WORD("lit") at each call (no locals) so each becomes a hoisted
// loop-invariant @ temp (UNBLOCK.log lever); plain literals in calls are not hoisted. Same string addresses.
void effFallMain(EffectEntry* effect) {
    Vec sndPos;
    EffFallElem* e;
    s32 i;
    s32 type;
    s32 duration;
    s32 played;
    s32 done;
    s32 group;
    f32 y;
    f32 angle;
    f32 top;
    f32 width;
    f32 ground;
    f32 a;
    s32 j;
    f32 rotY;
    e = effect->userdata;
    {
        Vec v = { 0.0f, 0.0f, 0.0f };
        v.x = e->pos.x;
        v.y = (y = e->pos.y);
        v.z = e->pos.z;
        sndPos = v;
    }
    played = 0;
    ground = y + e->drop;
    type = e->type;
    width = e->width;
    rotY = e->rotY;
    duration = e->duration;
    group = gp->inBattle != 0;
    if (effect->flags & 4) {
        effect->flags &= ~4;
        effFallRelease(effect);
        effDelete(effect);
        return;
    }
    angle = 6.2831855f * rotY;
    top = 30.0f + y;
    done = 0;
    for (i = 1, e++; i < effect->count; i++, e++) {
        switch (type) {
            case 0:
            case 1:
                switch (e->state) {
                    case 0:
                        if (e->timer) {
                            e->timer--;
                        }
                        if (e->timer == 0) {
                            e->state++;
                            if (type == 1) {
                                psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_FALL_BUG1"), &sndPos);
                            } else if (i == 1) {
                                psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_FALL_BUG2"), &sndPos);
                            }
                        }
                        break;
                    case 1:
                        if (type == 0 && i > 1 && !played && gp->retraceCount % 6 == 0) {
                            psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_FALL_BUG2"), &sndPos);
                            played = 1;
                        }
                        e->pos.y += e->f34;
                        e->f34 -= e->f30;
                        if (distABf(e->pos.x, e->pos.z, 0.0f, 0.0f) < width / 2.0f) {
                            if (e->pos.y < ground) {
                                e->pos.y = ground;
                                e->f34 *= -0.3f;
                                e->state++;
                            }
                        } else if (e->pos.y < y) {
                            e->pos.y = y;
                            e->f34 *= -0.3f;
                            e->state++;
                        }
                        break;
                    case 2:
                        e->pos.y += e->f34;
                        e->f34 -= e->f30;
                        a = (6.2831855f * angleABf(0.0f, 0.0f, e->pos.x, e->pos.z)) / 360.0f;
                        if (i == 1) {
                            a = angle / 360.0f;
                        }
                        e->pos.x += 0.5f * (f32)sin(a);
                        { f32 c = (f32)cos(a); e->pos.z += 0.5f * -c; }
                        if (e->pos.y < y) {
                            e->pos.y = y;
                            e->timer = 0;
                            e->state++;
                        }
                        break;
                    case 3:
                        e->timer++;
                        if (e->timer > 30) {
                            e->state++;
                        }
                        break;
                    case 4:
                        e->alpha -= 10;
                        if (e->alpha < 0) {
                            e->alpha = 0;
                            e->state++;
                        }
                        break;
                    case 5:
                        done++;
                        break;
                }
                if (((gp->retraceCount >> 1) >> 1) & 1) {
                    e->scroll++;
                }
                if (e->scroll >= 3) {
                    e->scroll = 0;
                }
                e->rot2 = 35.0f * (f32)sin(gp->retraceCount / 2.0f + i);
                break;
            case 2:
                switch (e->state) {
                    case 0:
                        e->pos.y += e->f34;
                        e->f34 -= e->f30;
                        if (e->pos.y < y) {
                            e->pos.y = y;
                            e->timer = 0;
                            e->state++;
                            psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_DAMAGE_FORK1"), &sndPos);
                        }
                        break;
                    case 1:
                        {
                            f32 s = (f32)sin((62.831856f * e->timer) / 60.0f);
                            e->f44 = ((60.0f - e->timer) * s) / 60.0f;
                        }
                        e->timer++;
                        if (e->timer > 60) {
                            e->state++;
                        }
                        break;
                    case 2:
                        e->timer++;
                        if (e->timer > 30) {
                            e->state++;
                        }
                        break;
                    case 3:
                        e->alpha -= 10;
                        if (e->alpha < 0) {
                            e->alpha = 0;
                            e->state++;
                        }
                        break;
                    case 4:
                        done++;
                        break;
                }
                e->vtx = (EffFallVtx**)smartAlloc(0x280, 3);
                {
                    EffFallVtx* v = *e->vtx;
                    for (j = 0; j < 32; j++, v++) {
                        v->x = (j & 1) ? 16.0f : -16.0f;
                        v->y = 128.0f - 8.533334f * (j / 2);
                        v->z = 0.0f;
                        v->u = (j & 1) ? 2.0f : 0.0f;
                        v->v = 0.06666667f * (j / 2);
                    }
                }
                {
                    EffFallVtx* v = *e->vtx;
                    for (i = 0; i < 32; i++, v++) {
                        v->x += e->f44 * ((v->y * v->y) / 600.0f);
                    }
                }
                DCFlushRange(*e->vtx, 0x280);
                break;
            case 3:
                switch (e->state) {
                    case 0:
                        e->pos.y += e->f34;
                        e->f34 -= e->f30;
                        if (e->pos.y < y) {
                            e->pos.y = y;
                            e->timer = 0;
                            e->state++;
                        }
                        break;
                    case 1:
                        if (e->timer % 10 == 0) {
                            effWaterDamageN64Entry(4, 20, sndPos.x, sndPos.y, sndPos.z, 1.0f);
                        }
                        e->timer++;
                        if (e->timer > duration) {
                            e->state++;
                        }
                        break;
                    case 2:
                        e->alpha *= 0.9f;
                        e->scale *= 0.9f;
                        if (e->scale < 0.01f) {
                            psndSFXOff(e->sfx);
                            effDelete(effect);
                            return;
                        }
                        break;
                }
                e->vel.y -= 0.2f;
                break;
            case 4:
            case 5:
                switch (e->state) {
                    case 0:
                        if (type == 5) {
                            if (!animGroupBaseAsync("EFF_Baketu", group, 0)) {
                                break;
                            }
                            if (e->pose0 == -1) {
                                e->pose0 = animPoseEntry("EFF_Baketu", group);
                                animPoseSetAnim(e->pose0, "Z_1", 1);
                            }
                        } else {
                            if (!animGroupBaseAsync("EFF_Tarai", group, 0)) {
                                break;
                            }
                            if (e->pose0 == -1) {
                                e->pose0 = animPoseEntry("EFF_Tarai", group);
                                animPoseSetAnim(e->pose0, "Z_1", 1);
                            }
                        }
                        if (e->timer) {
                            e->timer--;
                        }
                        if (e->timer == 0) {
                            e->state++;
                        }
                        break;
                    case 1:
                        e->pos.y += e->f34;
                        e->f34 -= e->f30;
                        if (distABf(e->pos.x, e->pos.z, 0.0f, 0.0f) < width / 2.0f) {
                            if (e->pos.y < ground) {
                                e->pos.y = ground;
                                e->f34 *= -0.3f;
                                e->state++;
                            }
                        } else if (e->pos.y < y) {
                            e->pos.y = y;
                            e->f34 *= -0.3f;
                            e->state++;
                        }
                        break;
                    case 2:
                        e->pos.y += e->f34;
                        e->f34 -= e->f30;
                        a = (6.2831855f * angleABf(0.0f, 0.0f, e->pos.x, e->pos.z)) / 360.0f;
                        if (i == 1) {
                            a = angle / 360.0f;
                        }
                        e->pos.x += 2.0f * (f32)sin(a);
                        { f32 c = (f32)cos(a); e->pos.z += 2.0f * -c; }
                        if (e->pos.y < y) {
                            e->pos.y = y;
                            e->f34 *= -0.5f;
                            e->pos.y = y;
                            e->timer = 0;
                            if (e->f34 < 1.0f) {
                                e->state++;
                            }
                        }
                        break;
                    case 3:
                        e->timer++;
                        if (e->timer > 30) {
                            e->state++;
                        }
                        break;
                    case 4:
                        e->alpha -= 10;
                        if (e->alpha < 0) {
                            e->alpha = 0;
                            e->state++;
                            animPoseRelease(e->pose0);
                            e->pose0 = -1;
                        }
                        break;
                    case 5:
                        done++;
                        break;
                }
                switch (type) {
                    case 4:
                        if (e->state > 1) {
                            e->rot2 += (360.0f - e->rot2) / 10.0f;
                        }
                        break;
                    case 5:
                        if (e->state > 1) {
                            e->rot2 += (430.0f - e->rot2) / 10.0f;
                        }
                        break;
                }
                if (e->pose0 != -1) {
                    animPoseMain(e->pose0);
                }
                break;
            case 6:
                switch (e->state) {
                    case 0:
                        if (!animGroupBaseAsync("EFF_koopa", group, 0)) {
                            break;
                        }
                        if (e->pose0 == -1) {
                            e->pose0 = animPoseEntry("EFF_koopa", group);
                            animPoseSetAnim(e->pose0, "Z_1", 1);
                            psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_FALL_KOOPA1"), &sndPos);
                        }
                        e->pos.x += e->vel.x;
                        e->pos.y += e->vel.y;
                        e->pos.z += e->vel.z;
                        e->timer++;
                        if (e->timer > 30) {
                            e->timer = 0;
                            e->state++;
                            effKemuri1N64Entry(4, sndPos.x + e->pos.x, sndPos.y + e->pos.y, sndPos.z + e->pos.z, 0.0f);
                            evtEntry(evt_shake, 0, 0);
                        }
                        break;
                    case 1:
                        e->rot2 = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(30.0f, -27.0f, 11, e->timer, 180);
                        if (e->rot2 < 0.0f) {
                            e->rot3 = e->rot2;
                            e->rot2 = 0.0f;
                        }
                        e->timer++;
                        if (e->timer > 180) {
                            e->timer = 0;
                            e->state++;
                        }
                        if (e->timer == 100) {
                            psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_FALL_KOOPA1"), &sndPos);
                        }
                        break;
                    case 2:
                        e->rot2 = ((f32 (*)(f32, f32, s32, s32, s32))intplGetValue)(-27.0f, 80.0f, 11, e->timer, 200);
                        if (e->rot2 < 0.0f) {
                            e->rot3 = e->rot2;
                            e->rot2 = 0.0f;
                        }
                        e->timer++;
                        if (e->timer > 200) {
                            animPoseRelease(e->pose0);
                            e->pose0 = -1;
                            done++;
                        }
                        if (e->timer == 70) {
                            psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_FALL_KOOPA1"), &sndPos);
                        }
                        break;
                }
                if (e->pose0 != -1) {
                    animPoseMain(e->pose0);
                }
                break;
            case 7:
                switch (e->state) {
                    case 0:
                        if (!animGroupBaseAsync("EFF_inseki", group, 0)) {
                            break;
                        }
                        psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_FALL_METEOR1"), &sndPos);
                        if (e->pose0 == -1) {
                            e->pose0 = animPoseEntry("EFF_inseki", group);
                            animPoseSetAnim(e->pose0, "Z_1", 1);
                        }
                        if (e->pose1 == -1) {
                            e->pose1 = animPoseEntry("EFF_inseki", group);
                            animPoseSetAnim(e->pose1, "Z_2", 1);
                        }
                    case 2:
                    case 4:
                    case 6: {
                        f32 tbl[4][3] = {
                            { 140.0f, 40.0f, -10.0f },
                            { 0.0f, 40.0f, -40.0f },
                            { -140.0f, 40.0f, -10.0f },
                            { -400.0f, 40.0f, -40.0f }
                        };
                        e->vel.x = tbl[e->state / 2][0];
                        e->vel.y = tbl[e->state / 2][1];
                        e->vel.z = tbl[e->state / 2][2];
                        e->f30 = 0.4f;
                        e->timer = distABf(e->vel.x, e->vel.z, e->pos.x, e->pos.z) / 5.0f;
                        e->f34 = ((e->vel.y - e->pos.y) / e->timer) + (e->f30 * e->timer) / 2.0f;
                        e->rotY = angleABf(e->pos.x, e->pos.z, e->vel.x, e->vel.z);
                        e->rot2 = 0.0f;
                        e->state++;
                        break;
                    }
                    case 1:
                    case 3:
                    case 5:
                    case 7:
                        e->pos.y += e->f34;
                        e->f34 -= e->f30;
                        e->pos.x += 5.0f * fall_sinf((6.2831855f * e->rotY) / 360.0f);
                        e->pos.z += 5.0f * -fall_cosf((6.2831855f * e->rotY) / 360.0f);
                        if ((--e->timer) < 0) {
                            e->pos.x = e->vel.x;
                            e->pos.y = e->vel.y;
                            e->pos.z = e->vel.z;
                            e->state++;
                            if (e->state == 2) {
                                e->sfx = psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_DAMAGE_METEOR2"), &sndPos);
                            }
                            if (e->state != 8) {
                                effKemuri1N64Entry(4, sndPos.x + e->pos.x, sndPos.y + e->pos.y - 40.0f, sndPos.z + e->pos.z, 0.0f);
                                evtEntry(evt_shake, 0, 0);
                            }
                        }
                        e->rot2 += (360.0f - e->rot2) / 30.0f;
                        break;
                    case 8:
                        psndSFXOff(e->sfx);
                        animPoseRelease(e->pose0);
                        animPoseRelease(e->pose1);
                        e->pose0 = -1;
                        e->pose1 = -1;
                        e->state++;
                        break;
                    case 9:
                        done++;
                        break;
                }
                if (e->pose0 != -1) {
                    animPoseMain(e->pose0);
                }
                if (e->pose1 != -1) {
                    animPoseMain(e->pose1);
                }
                break;
            case 8:
                switch (e->state) {
                    case 0:
                        if (!animGroupBaseAsync("EFF_light", group, 0)) {
                            break;
                        }
                        e->pose0 = animPoseEntry("EFF_light", group);
                        animPoseSetAnim(e->pose0, "Z_1", 1);
                        e->state++;
                        break;
                    case 1:
                    case 2:
                        e->pos.y += e->f34;
                        e->f34 -= e->f30;
                        if (e->pos.y < y) {
                            e->pos.y = y;
                            e->timer = 0;
                            e->state++;
                        }
                        if (1 == e->state && e->pos.y < top) {
                            effKemuri1N64Entry(4, sndPos.x + e->pos.x, sndPos.y + y, sndPos.z + e->pos.z, 0.0f);
                            evtEntry(evt_shake, 0, 0);
                            animPoseSetAnim(e->pose0, "A_1", 1);
                            e->state++;
                            psndSFXOn_3D(SFX_NAME_WORD("SFX_BTL_STAGE_DAMAGE_LIGHT1"), &sndPos);
                        }
                        break;
                    case 3:
                        if (animPoseGetLoopTimes(e->pose0) < 1.0f) {
                            break;
                        }
                        e->timer++;
                        if (e->timer > e->duration) {
                            e->state++;
                        }
                        break;
                    case 4:
                        e->alpha -= 10;
                        if (e->alpha < 0) {
                            e->alpha = 0;
                            e->state++;
                        }
                        break;
                    case 5:
                        animPoseRelease(e->pose0);
                        e->pose0 = -1;
                        e->state++;
                        break;
                    case 6:
                        done++;
                        break;
                }
                if (e->pose0 != -1) {
                    animPoseMain(e->pose0);
                }
                break;
        }
    }
    if (done >= effect->count - 1) {
        effDelete(effect);
        return;
    }
    switch (type) {
        case 3:
            dispEntry(CAMERA_3D, dispCalcZ(&sndPos), 2, (DispCallback)effFallDispWater, effect);
            break;
        case 4:
        case 5:
            dispEntry(CAMERA_3D, dispCalcZ(&sndPos), 0, (DispCallback)effFallDisp, effect);
            break;
        case 7:
            dispEntry(CAMERA_3D, dispCalcZ(&sndPos), 2, (DispCallback)effFallDispInseki, effect);
            break;
        case 6:
        default:
            dispEntry(CAMERA_3D, dispCalcZ(&sndPos), 2, (DispCallback)effFallDisp, effect);
            break;
    }
}

#pragma opt_propagation reset
#pragma opt_lifetimes reset
void effFallDisp(CameraId cameraId, EffectEntry* effect) {
    Mtx trans;
    Mtx scl;
    Mtx rot;
    Mtx mtx;
    GXTexObj tex;
    Vec axis;
    CameraEntry* cam;
    EffFallElem* e;
    s32 i;
    s32 type;
    f32 angle;
    s32 j;
    f32 hw, hh, s;

    e = effect->userdata;
    cam = camGetPtr(cameraId);
    type = e->type;
    angle = e->rotY;
    PSMTXTrans(trans, e->pos.x, e->pos.y, e->pos.z);
    PSMTXRotRad(rot, 'y', -camGetPtr(cameraId)->viewYaw * 0.017453292f);
    PSMTXConcat(trans, rot, mtx);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    switch (type) {
        case 0:
        case 1:
            effGetTexObj(0x32, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            break;
        case 2:
            effGetTexObj(0x33, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            break;
    }
    for (i = 1, e++; i < effect->count; i++, e++) {
        switch (type) {
            case 0:
            case 1:
                hw = 64.0f;
                hh = 42.666668f;
                hw /= 2.0f;
                hh /= 2.0f;
                PSMTXTrans(trans, e->pos.x, e->pos.y, e->pos.z);
                PSMTXRotRad(rot, 'y', e->rotY * 0.017453292f);
                PSMTXConcat(trans, rot, trans);
                PSMTXTrans(rot, 0.0f, hh / 2.0f * e->scale, 0.0f);
                PSMTXConcat(trans, rot, trans);
                PSMTXRotRad(rot, 'z', e->rot2 * 0.017453292f);
                PSMTXConcat(trans, rot, trans);
                PSMTXTrans(rot, 0.0f, -hh / 2.0f * e->scale, 0.0f);
                PSMTXConcat(trans, rot, trans);
                PSMTXScale(scl, e->scale, e->scale, e->scale);
                PSMTXConcat(trans, scl, trans);
                PSMTXConcat(mtx, trans, trans);
                PSMTXConcat(cam->view, trans, trans);
                GXLoadPosMtxImm(trans, 0);
                GXSetCurrentMtx(0);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 255, 255, 255, e->alpha });
                PSMTXScale(scl, 1.0f, 0.33333334f, 1.0f);
                PSMTXTrans(trans, 0.0f, (f32)e->scroll, 0.0f);
                PSMTXConcat(scl, trans, trans);
                GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition3f32(-hw / 2.0f, hh, 0.0f);
                GXTexCoord2f32(0.0f, 0.0f);
                GXPosition3f32(hw / 2.0f, hh, 0.0f);
                GXTexCoord2f32(1.0f, 0.0f);
                GXPosition3f32(hw / 2.0f, 0.0f, 0.0f);
                GXTexCoord2f32(1.0f, 1.0f);
                GXPosition3f32(-hw / 2.0f, 0.0f, 0.0f);
                GXTexCoord2f32(0.0f, 1.0f);
                break;
            case 2:
                PSMTXTrans(trans, e->pos.x, e->pos.y, e->pos.z);
                PSMTXScale(scl, e->scale, e->scale, e->scale);
                PSMTXConcat(trans, scl, trans);
                PSMTXConcat(mtx, trans, trans);
                PSMTXConcat(cam->view, trans, trans);
                GXLoadPosMtxImm(trans, 0);
                GXSetCurrentMtx(0);
                GXSetChanMatColor(GX_COLOR0A0, (GXColor){ 255, 255, 255, e->alpha });
                PSMTXIdentity(trans);
                GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
                for (j = 0; j < 30; j += 2) {
                    EffFallVtx* v = &(*e->vtx)[j];
                    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                    GXPosition3f32(v[0].x, v[0].y, v[0].z);
                    GXTexCoord2f32(v[0].u, v[0].v);
                    GXPosition3f32(v[2].x, v[2].y, v[2].z);
                    GXTexCoord2f32(v[2].u, v[2].v);
                    GXPosition3f32(v[3].x, v[3].y, v[3].z);
                    GXTexCoord2f32(v[3].u, v[3].v);
                    GXPosition3f32(v[1].x, v[1].y, v[1].z);
                    GXTexCoord2f32(v[1].u, v[1].v);
                }
                break;
            case 4:
            case 5: {
                Vec v = { 0.0f, 0.0f, 0.0f };
                v.x = (f32)cos(6.2831855f * angle / 360.0f);
                v.z = (f32)sin(6.2831855f * angle / 360.0f);
                axis = v;
                PSMTXTrans(trans, e->pos.x, e->pos.y, e->pos.z);
                PSMTXScale(scl, e->scale, e->scale, e->scale);
                PSMTXConcat(trans, scl, trans);
                PSMTXTrans(scl, 0.0f, (f32)(type == 4 ? 8 : 16), 0.0f);
                PSMTXConcat(trans, scl, trans);
                PSMTXRotAxisRad(rot, &axis, -e->rot2 * 0.017453292f);
                PSMTXConcat(trans, rot, trans);
                PSMTXTrans(scl, 0.0f, (f32)-(type == 4 ? 8 : 16), 0.0f);
                PSMTXConcat(trans, scl, trans);
                PSMTXConcat(mtx, trans, trans);
                if (e->pose0 != -1) {
                    animPoseDrawMtx(e->pose0, trans, 1, 0.0f, 10.0f);
                    animPoseDrawMtx(e->pose0, trans, 2, 0.0f, 10.0f);
                    animPoseDrawMtx(e->pose0, trans, 3, 0.0f, 10.0f);
                }
                break;
            }
            case 6:
                if (e->pose0 == -1) {
                    break;
                }
                PSMTXTrans(trans, e->pos.x, e->pos.y, e->pos.z);
                if (angle < 180.0f) {
                    PSMTXScale(scl, e->scale, e->scale, e->scale);
                } else {
                    PSMTXScale(scl, -e->scale, e->scale, e->scale);
                }
                PSMTXConcat(trans, scl, trans);
                if (e->rot2) {
                    PSMTXRotRad(rot, 'z', -e->rot2 * 0.017453292f);
                    PSMTXConcat(trans, rot, trans);
                } else {
                    PSMTXTrans(scl, -200.0f, 0.0f, 0.0f);
                    PSMTXConcat(trans, scl, trans);
                    PSMTXRotRad(rot, 'z', -e->rot3 * 0.017453292f);
                    PSMTXConcat(trans, rot, trans);
                    PSMTXTrans(scl, 200.0f, 0.0f, 0.0f);
                    PSMTXConcat(trans, scl, trans);
                }
                PSMTXConcat(mtx, trans, trans);
                PSMTXTrans(scl, -82.655f, 88.8f, 0.0f);
                PSMTXConcat(trans, scl, scl);
                PSMTXScale(rot, 1.03f, 1.03f, 1.03f);
                PSMTXConcat(scl, rot, rot);
                PSMTXTrans(scl, 82.655f, -88.8f, 0.0f);
                PSMTXConcat(rot, scl, scl);
                animPoseSetMaterialFlagOn(e->pose0, 0x40);
                animPoseSetMaterialEvtColor(e->pose0, (GXColor){ 0, 0, 0, 255 });
                animPoseDrawMtx(e->pose0, scl, 1, 0.0f, 10.0f);
                animPoseDrawMtx(e->pose0, scl, 2, 0.0f, 10.0f);
                animPoseDrawMtx(e->pose0, scl, 3, 0.0f, 10.0f);
                animPoseSetMaterialFlagOn(e->pose0, 0x40);
                animPoseSetMaterialEvtColor(e->pose0, (GXColor){ 255, 255, 255, 255 });
                animPoseDrawMtx(e->pose0, trans, 1, 0.0f, 10.0f);
                animPoseDrawMtx(e->pose0, trans, 2, 0.0f, 10.0f);
                animPoseDrawMtx(e->pose0, trans, 3, 0.0f, 10.0f);
                break;
            case 8:
                if (e->pose0 == -1) {
                    break;
                }
                PSMTXTrans(trans, e->pos.x, e->pos.y, e->pos.z);
                PSMTXScale(scl, e->scale, e->scale, e->scale);
                PSMTXConcat(trans, scl, trans);
                PSMTXConcat(mtx, trans, trans);
                animPoseSetMaterialFlagOn(e->pose0, 0x40);
                animPoseSetMaterialEvtColor(e->pose0, (GXColor){ 255, 255, 255, e->alpha });
                animPoseDrawMtx(e->pose0, trans, 1, 0.0f, 10.0f);
                animPoseDrawMtx(e->pose0, trans, 2, 0.0f, 10.0f);
                animPoseDrawMtx(e->pose0, trans, 3, 0.0f, 10.0f);
                break;
        }
    }
}

void effFallDispInseki(CameraId cameraId, EffectEntry* effect) {
    Mtx trans;
    Mtx scl;
    Mtx rot;
    Mtx mtx;
    Vec axis;
    EffFallWork* w;
    s32 i;

    w = effect->userdata;
    camGetPtr(cameraId);
    PSMTXTrans(trans, w->position.x, w->position.y, w->position.z);
    PSMTXRotRad(rot, 'y', -camGetPtr(cameraId)->viewYaw * 0.017453292f);
    PSMTXConcat(trans, rot, mtx);
    if (w->pose0 != -1 && w->pose1 != -1) {
        Vec v = { 0.0f, 0.0f, 0.0f };
        v.x = (f32)cos(6.2831855f * w->angle / 360.0f);
        v.z = (f32)sin(6.2831855f * w->angle / 360.0f);
        axis = v;
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                PSMTXTrans(trans, w->pos2.x, w->pos2.y, w->pos2.z);
                PSMTXRotAxisRad(rot, &axis, -w->rotation * 0.017453292f);
                PSMTXScale(scl, 2.05f * w->scale, 2.05f * w->scale, 2.05f * w->scale);
                PSMTXConcat(mtx, trans, trans);
                PSMTXConcat(trans, rot, trans);
                PSMTXConcat(trans, scl, trans);
                animPoseSetMaterialFlagOn(w->pose0, 0x40);
                animPoseSetMaterialEvtColor(w->pose0, (GXColor){ 0, 0, 0, 255 });
            } else {
                PSMTXTrans(trans, w->pos2.x, w->pos2.y, w->pos2.z);
                PSMTXRotAxisRad(rot, &axis, -w->rotation * 0.017453292f);
                PSMTXScale(scl, 2.0f * w->scale, 2.0f * w->scale, 2.0f * w->scale);
                PSMTXConcat(mtx, trans, trans);
                PSMTXConcat(trans, rot, trans);
                PSMTXConcat(trans, scl, trans);
                animPoseSetMaterialFlagOn(w->pose0, 0x40);
                animPoseSetMaterialEvtColor(w->pose0, (GXColor){ 255, 255, 255, 255 });
            }
            animPoseDrawMtx(w->pose0, trans, 1, 0.0f, 10.0f);
            animPoseDrawMtx(w->pose0, trans, 2, 0.0f, 10.0f);
            animPoseDrawMtx(w->pose0, trans, 3, 0.0f, 10.0f);
        }
        PSMTXTrans(trans, w->pos2.x, w->pos2.y, w->pos2.z);
        PSMTXScale(scl, 2.1f * w->scale, 2.1f * w->scale, 2.1f * w->scale);
        PSMTXConcat(mtx, trans, trans);
        PSMTXConcat(trans, scl, trans);
        animPoseDrawMtx(w->pose1, trans, 1, 0.0f, 10.0f);
        animPoseDrawMtx(w->pose1, trans, 2, 0.0f, 10.0f);
        animPoseDrawMtx(w->pose1, trans, 3, 0.0f, 10.0f);
    }
}

static const GXColor white255 = { 255, 255, 255, 255 };
void effFallDispWater(CameraId cameraId, EffectEntry *effect) {
    Mtx trans;
    Mtx scl;
    Mtx rot;
    Mtx mtx;
    GXTexObj tex;
    GXColor color;
    CameraEntry *cam;
    EffFallWork *w;
    f32 hw;
    f32 hh;
    f32 s;
    w = effect->userdata;
    cam = camGetPtr(cameraId);
    PSMTXTrans(trans, w->position.x, w->position.y, w->position.z);
    PSMTXRotRad(rot, 'y', (-camGetPtr(cameraId)->viewYaw) * 0.017453292f);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(cam->view, mtx, mtx);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_C0, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_A0, GX_CA_APREV, GX_CA_ZERO);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetCullMode(GX_CULL_BACK);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    effGetTexObj(0x35, &tex);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    hw = 64.0f;
    hh = 704.0f;
    hw /= 2.0f;
    hh /= 2.0f;
    PSMTXTrans(trans, w->pos3.x, w->pos3.y, w->pos3.z);
    PSMTXScale(scl, 1.0f, hh / ((f32) GXGetTexObjHeight(&tex)), 1.0f);
    PSMTXConcat(trans, scl, trans);
    GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
    PSMTXTrans(trans, w->pos2.x, w->pos2.y, w->pos2.z);
    PSMTXScale(scl, w->scale, 1.0f, 1.0f);
    PSMTXConcat(trans, scl, trans);
    PSMTXConcat(mtx, trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetTevColor(GX_TEVREG0, white255);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32((-hw) / 2.0f, hh, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(hw / 2.0f, hh, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(2.0f, 0.0f);
    GXPosition3f32(hw / 2.0f, 0.0f, 0.0f);
    GXColor4u8(255, 255, 255, 0);
    GXTexCoord2f32(2.0f, 1.0f);
    GXPosition3f32((-hw) / 2.0f, 0.0f, 0.0f);
    GXColor4u8(255, 255, 255, 0);
    GXTexCoord2f32(0.0f, 1.0f);
    {
        EffFallWork *w2;
        w = effect->userdata;
        w2 = w;
        if (w2->state > 0) {
            PSMTXTrans(trans, w2->pos2.x, w2->pos2.y, w2->pos2.z);
            PSMTXScale(scl, 0.56f, 0.4f, 0.8f);
            PSMTXConcat(mtx, trans, trans);
            PSMTXConcat(trans, scl, trans);
            GXLoadPosMtxImm(trans, 0);
            PSMTXTrans(trans, w2->pos3.y / 8.0f, 0.0f, 0.0f);
            GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
            GXSetTevColor(GX_TEVREG0, (GXColor){255, 255, 255, w2->alpha});
            effGetTexObj(0x37, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
            GXSetArray(GX_VA_POS, shibuki_vertex_tbl, 6);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
            GXSetArray(GX_VA_NRM, shibuki_normal_tbl, 3);
            GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            GXSetArray(GX_VA_CLR0, shibuki_color0_tbl, 4);
            GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 14);
            GXSetArray(GX_VA_TEX0, shibuki_texcoord0_tbl, 4);
            GXCallDisplayList(shibuki_dl_1_0, 0x20);
            GXCallDisplayList(shibuki_dl_1_1, 0x20);
            GXCallDisplayList(shibuki_dl_1_2, 0x20);
            GXCallDisplayList(shibuki_dl_1_3, 0x20);
            PSMTXTrans(trans, w2->pos2.x, w2->pos2.y, w2->pos2.z);
            // FAKE: constant parked in s first; target loads scale before 0.8f (natural `s = w2->scale * 0.8f` swaps the loads)
            s = 0.8f;
            s = w2->scale * s;
            PSMTXScale(scl, 0.7f * s, 0.5f * s, s);
            PSMTXConcat(mtx, trans, trans);
            PSMTXConcat(trans, scl, trans);
            GXLoadPosMtxImm(trans, 0);
            PSMTXTrans(trans, 0.0f, (-w2->pos3.y) / 4.0f, 0.0f);
            GXLoadTexMtxImm(trans, GX_TEXMTX0, GX_MTX2x4);
            {
                static const GXColor white255b = {255, 255, 255, 255};
                GXSetTevColor(GX_TEVREG0, white255b);
            }
            effGetTexObj(0x36, &tex);
            GXLoadTexObj(&tex, GX_TEXMAP0);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 8);
            GXSetArray(GX_VA_POS, shibuki_vertex_tbl, 6);
            GXSetVtxDesc(GX_VA_NRM, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_S8, 6);
            GXSetArray(GX_VA_NRM, shibuki_normal_tbl, 3);
            GXSetVtxDesc(GX_VA_CLR0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            GXSetArray(GX_VA_CLR0, shibuki_color0_tbl, 4);
            GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_S16, 14);
            GXSetArray(GX_VA_TEX0, shibuki_texcoord0_tbl, 4);
            GXCallDisplayList(shibuki_dl_0_0, 0x40);
            GXCallDisplayList(shibuki_dl_0_1, 0x40);
            GXCallDisplayList(shibuki_dl_0_2, 0x40);
            GXCallDisplayList(shibuki_dl_0_3, 0x40);
        }
    }
}

s32 effFallCheckHit(EffectEntry* effect) {
    EffFallWork* w = effect->userdata;
    switch (w->type) {
        case 0:
        case 1:
        case 4:
        case 5:
            if (w->state >= 2) return 1;
            break;
        case 2:
        case 3:
            if (w->state >= 1) return 1;
            break;
        case 6:
            if (w->state >= 2 || (w->state == 1 && w->timer > 90)) return 1;
            break;
        case 7:
            if (w->state >= 4) return 1;
            break;
        case 8:
            if (w->state >= 3) return 1;
            break;
    }
    return 0;
}


