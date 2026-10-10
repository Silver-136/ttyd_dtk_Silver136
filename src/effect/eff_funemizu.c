#include "effect/eff_funemizu.h"

typedef struct FunemizuColorEntry {
    const char* name;
    u32 color;
} FunemizuColorEntry;

typedef struct FunemizuColorTable {
    FunemizuColorEntry entries[32];
    const char* terminator;
} FunemizuColorTable;

extern f32 float_0_804279d0;
extern f32 float_0p5_804279d4;
extern f32 float_1_804279dc;

u32 unk_802294a4(void) {
    extern FunemizuColorTable unk_803b3b00;
    extern void* gp;
    extern u32 dat_804279c0;
    extern s32 strcmp(const char* a, const char* b);
    extern s32 strncmp(const char* a, const char* b, s32 n);

    FunemizuColorEntry* entry = unk_803b3b00.entries;

    while (entry != 0) {
        if (entry->name == 0) {
            break;
        }
        if (strcmp(entry->name, (const char*)((s32)gp + 0x12C)) == 0) {
            return entry->color;
        }
        entry++;
    }

    entry = unk_803b3b00.entries;
    while (entry != 0) {
        if (entry->name == 0) {
            break;
        }
        if (strncmp(entry->name, (const char*)((s32)gp + 0x12C), 3) == 0) {
            return entry->color;
        }
        entry++;
    }
    return dat_804279c0;
}

/* stub-fill: effFunemizuEntry | prototype_only | source_prototype */
void* effFunemizuEntry(double x, double y, double z, double scale, double angle, s32 type) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 irand(s32 range);
    extern void effFunemizuMain(void* effect);
    extern const char str_funemizu_802ff1c0[];
    extern f32 float_0p1_804279f0;
    extern f32 float_100_80427a00;

    void* effect = effEntry();
    s32 count = 1;
    void* work;
    void* child;
    s32 i;
    f32 halfScale;
    f32 zero;

    if (type == 1) {
        count = ((s32)scale * 3) + 1;
    }
    if (count > 10) {
        count = 10;
    }

    *(const char**)((s32)effect + 0x14) = str_funemizu_802ff1c0;
    *(s32*)((s32)effect + 8) = count;
    work = __memAlloc(3, count * 0x4C);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effFunemizuMain;

    zero = float_0_804279d0;
    halfScale = (f32)scale * float_0p5_804279d4;
    *(s32*)work = type;
    *(f32*)((s32)work + 4) = (f32)x;
    *(f32*)((s32)work + 8) = (f32)y;
    *(f32*)((s32)work + 0xC) = (f32)z;
    *(f32*)((s32)work + 0x18) = zero;
    *(f32*)((s32)work + 0x1C) = zero;
    *(f32*)((s32)work + 0x20) = zero;
    *(f32*)((s32)work + 0x24) = zero;
    *(f32*)((s32)work + 0x2C) = zero;
    *(f32*)((s32)work + 0x28) = zero;
    *(s32*)((s32)work + 0x30) = 0;
    *(f32*)((s32)work + 0x44) = float_1_804279dc;
    *(s32*)((s32)work + 0x48) = 4;

    child = (void*)((s32)work + 0x4C);
    for (i = 1; i < *(s32*)((s32)effect + 8); i++, child = (void*)((s32)child + 0x4C)) {
        *(f32*)((s32)child + 4) = zero;
        *(f32*)((s32)child + 8) = zero;
        *(f32*)((s32)child + 0xC) = zero;
        *(f32*)((s32)child + 0x10) = halfScale;
        *(f32*)((s32)child + 0x14) = (f32)angle;
        *(f32*)((s32)child + 0x34) = halfScale + (f32)irand(2);
        *(f32*)((s32)child + 0x38) = float_0p1_804279f0 + ((f32)irand(10) / float_100_80427a00);
        *(s32*)((s32)child + 0x3C) = irand(5);
        *(s32*)((s32)child + 0x40) = 0xFF;
    }

    return effect;
}
void effFunemizuMain(void* effect) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct FunemizuWork {
        s32 mode;
        f32 x;
        f32 y;
        f32 z;
        f32 scale;
        f32 angle;
        f32 rate0;
        f32 rate1;
        f32 rate2;
        f32 rate3;
        f32 scroll0;
        f32 scroll1;
        s32 frame;
        f32 vy;
        f32 gravity;
        s32 delay;
        s32 alpha;
        s32 unk44;
        s32 cameraId;
    } FunemizuWork;
    typedef struct EffectEntryLocal {
        char pad0[8];
        s32 effCount;
        void* work;
    } EffectEntryLocal;

    extern const VecLocal vec3_802ff1a8;
    extern f32 float_10_804279e8;
    extern f32 float_0p05_804279ec;
    extern f32 float_0p1_804279f0;
    extern f32 float_6p2832_804279f4;
    extern f32 float_360_804279f8;
    extern f32 float_14_804279fc;
    extern void* camGetPtr(s32 cameraId);
    extern double sin(double x);
    extern double cos(double x);
    extern void effFunemizuEntry(double x, double y, double z, double scale, double angle, s32 type);
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 renderMode, void* callback, void* param, f32 order);
    extern void effFunemizuDisp(s32 cameraId, void* effect);
    extern void effFunemizuShibukiDisp(s32 cameraId, void* effect);

    EffectEntryLocal* eff;
    FunemizuWork* work;
    FunemizuWork* child;
    VecLocal basePos;
    VecLocal dispPos;
    s32 deadCount;
    s32 mode;
    s32 cameraId;
    s32 i;
    s32 frame;
    f32 scale;
    f32 curr;
    f32 next;
    f32 zero;
    f32 one;
    f32 ten;
    f32 half;
    f32 t;
    f32 angleRad;
    f32 sinv;
    f32 cosv;
    f32 tmp;
    f32 order;
    void* cam;

    eff = (EffectEntryLocal*)effect;
    work = (FunemizuWork*)eff->work;
    deadCount = 0;

    basePos = vec3_802ff1a8;
    basePos.x = work->x;
    basePos.y = work->y;
    basePos.z = work->z;
    dispPos = basePos;

    mode = work->mode;
    cameraId = work->cameraId;

    work->frame++;
    if (work->frame > 1000) {
        work->frame -= 1000;
    }

    if (mode == 0 || mode == 2) {
        zero = float_0_804279d0;
        one = float_1_804279dc;
        ten = float_10_804279e8;
        scale = work->scale;
        curr = scale;
        next = scale - one;
        if (curr < zero) {
            curr = zero;
        }
        if (curr > one) {
            curr = one;
        }
        if (next < zero) {
            next = zero;
        }
        if (next > one) {
            next = one;
        }

        work->rate0 += (curr - work->rate0) / ten;
        work->rate1 += (next - work->rate1) / ten;
        if (work->scale > one) {
            work->rate2 += (curr - work->rate2) / ten;
            work->rate3 += (next - work->rate3) / ten;
        } else {
            half = float_0p5_804279d4;
            work->rate2 += (curr - work->rate2) * half;
            work->rate3 += (next - work->rate3) * half;
        }

        work->scroll0 += float_0p05_804279ec * work->rate0;
        work->scroll1 += float_0p1_804279f0 * work->rate1;

        if (work->scale > float_0p1_804279f0) {
            frame = work->frame;
            if ((frame % 10) == 0) {
                cam = camGetPtr(4);
                angleRad = (float_6p2832_804279f4 * (work->angle - *(f32*)((s32)cam + 0x114))) / float_360_804279f8;
                sinv = (f32)sin((double)angleRad);
                cosv = (f32)cos((double)angleRad);
                effFunemizuEntry(
                    (double)(work->x - float_14_804279fc * cosv),
                    (double)work->y,
                    (double)(work->z - float_14_804279fc * sinv),
                    (double)work->scale,
                    (double)work->angle,
                    1);
            }
        }
    } else {
        zero = float_0_804279d0;
        work->rate0 = float_1_804279dc;
        child = (FunemizuWork*)((s32)work + 0x4C);
        for (i = 1; i < eff->effCount; i++) {
            if (child->y < zero) {
                deadCount++;
            } else if (child->delay != 0) {
                child->delay--;
            } else {
                t = (float_6p2832_804279f4 * child->angle) / float_360_804279f8;
                cosv = (f32)cos((double)t);
                child->x += child->scale * cosv;
                t = (float_6p2832_804279f4 * child->angle) / float_360_804279f8;
                sinv = (f32)sin((double)t);
                child->z += child->scale * -sinv;
                child->y += child->vy;
                child->vy -= child->gravity;
                child->alpha -= 8;
                if (child->alpha < 0) {
                    child->alpha = 0;
                }
            }
            child++;
        }
        if (deadCount >= eff->effCount - 1) {
            effDelete(effect);
            return;
        }
    }

    order = dispCalcZ(&dispPos);
    if (mode == 0 || mode == 2) {
        dispEntry(cameraId, 2, effFunemizuDisp, effect, order);
    } else {
        dispEntry(cameraId, 2, effFunemizuShibukiDisp, effect, order);
    }
}


/* CHATGPT STUB FILL: main/effect/eff_funemizu 20260624_184823 */

/* stub-fill: effFunemizuDisp | prototype_only | source_prototype */
void effFunemizuDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef u8 TexObj[0x20];
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f64, f64, f64);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f64, char);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetArray(s32, void*, s32);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void GXCallDisplayList(void*, u32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern FunemizuColorTable unk_803b3b00;
    extern void* gp;
    extern u32 dat_804279c0;
    extern s32 strcmp(const char*, const char*);
    extern s32 strncmp(const char*, const char*, s32);
    extern u8 funemizu_vertex_tbl[];
    extern u8 funemizu_normal_tbl[];
    extern u8 funemizu_color0_tbl[];
    extern u8 funemizu_texcoord0_tbl[];
    extern void* funemizu_dl_0_tbl[];
    extern u8 funemizu_dl_0_size_tbl[];
    extern void* funemizu_dl_1_tbl[];
    extern u8 funemizu_dl_1_size_tbl[];
    extern u8 gor_fune_vertex_tbl[];
    extern u8 gor_fune_normal_tbl[];
    extern u8 gor_fune_color0_tbl[];
    extern u8 gor_fune_texcoord0_tbl[];
    extern void* gor_fune_dl_0_tbl[];
    extern u8 gor_fune_dl_0_size_tbl[];
    extern void* gor_fune_dl_1_tbl[];
    extern u8 gor_fune_dl_1_size_tbl[];
    u8* w = *(u8**)((u8*)effect + 0xC);
    void* cam = camGetPtr(cameraId);
    Mtx root, trans, rot, scale, model;
    TexObj tex;
    FunemizuColorEntry* entry;
    void** lists;
    u8* sizes;
    u8* colors;
    u32 color;
    s32 mode = *(s32*)w;
    s32 side;
    s32 i;
    s32 count;
    f32 direction;

    entry = unk_803b3b00.entries;
    while (entry != 0 && entry->name != 0) {
        if (strcmp(entry->name, (const char*)((s32)gp + 0x12C)) == 0) {
            color = entry->color;
            goto color_found;
        }
        entry++;
    }
    entry = unk_803b3b00.entries;
    while (entry != 0 && entry->name != 0) {
        if (strncmp(entry->name, (const char*)((s32)gp + 0x12C), 3) == 0) {
            color = entry->color;
            goto color_found;
        }
        entry++;
    }
    color = dat_804279c0;
color_found:
    colors = mode == 0 ? funemizu_color0_tbl : gor_fune_color0_tbl;
    colors[0] = color >> 24;
    colors[1] = color >> 16;
    colors[2] = color >> 8;

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
    PSMTXTrans(trans, *(f32*)(w + 4), *(f32*)(w + 8), *(f32*)(w + 0xC));
    PSMTXRotRad(rot, 0.017453292f * (*(f32*)(w + 0x14) - *(f32*)((u8*)camGetPtr(cameraId) + 0x114)), 'y');
    PSMTXScale(scale, *(f32*)(w + 0x44), *(f32*)(w + 0x44), *(f32*)(w + 0x44));
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(trans, scale, trans);
    PSMTXTrans(rot, -15.0f, 0.0f, 0.0f);
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat((u8*)cam + 0x11C, trans, root);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 0);
    effGetTexObj(0x43, tex);
    GXLoadTexObj(tex, 0);

    for (side = 0; side < 2; side++) {
        GXSetCullMode(side == 0 ? 2 : 1);
        direction = -(2.0f * (f32)side - 1.0f);
        PSMTXTrans(trans, 0.0f, 0.0f, direction);
        PSMTXConcat(root, trans, trans);
        PSMTXScale(scale, 1.0f, 1.0f, direction);
        PSMTXConcat(trans, scale, trans);
        PSMTXScale(scale, *(f32*)(w + 0x18), *(f32*)(w + 0x20), *(f32*)(w + 0x18));
        PSMTXConcat(trans, scale, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        PSMTXTrans(scale, *(f32*)(w + 0x28), 0.0f, 0.0f);
        GXLoadTexMtxImm(scale, 0x1E, 1);

        if (mode == 0) {
            GXClearVtxDesc();
            GXSetVtxDesc(9, 2);
            GXSetVtxAttrFmt(0, 9, 1, 3, 9);
            GXSetArray(9, funemizu_vertex_tbl, 6);
            GXSetVtxDesc(10, 2);
            GXSetVtxAttrFmt(0, 10, 0, 1, 6);
            GXSetArray(10, funemizu_normal_tbl, 3);
            GXSetVtxDesc(11, 2);
            GXSetVtxAttrFmt(0, 11, 1, 5, 0);
            GXSetArray(11, funemizu_color0_tbl, 4);
            GXSetVtxDesc(13, 2);
            GXSetVtxAttrFmt(0, 13, 1, 3, 12);
            GXSetArray(13, funemizu_texcoord0_tbl, 4);
            sizes = funemizu_dl_0_size_tbl;
            lists = funemizu_dl_0_tbl;
            for (i = 0; i < 3; i++) {
                GXCallDisplayList(lists[i], (u32)sizes[i] << 5);
            }
        } else {
            GXClearVtxDesc();
            GXSetVtxDesc(9, 2);
            GXSetVtxAttrFmt(0, 9, 1, 3, 8);
            GXSetArray(9, gor_fune_vertex_tbl, 6);
            GXSetVtxDesc(10, 2);
            GXSetVtxAttrFmt(0, 10, 0, 1, 6);
            GXSetArray(10, gor_fune_normal_tbl, 3);
            GXSetVtxDesc(11, 2);
            GXSetVtxAttrFmt(0, 11, 1, 5, 0);
            GXSetArray(11, gor_fune_color0_tbl, 4);
            GXSetVtxDesc(13, 2);
            GXSetVtxAttrFmt(0, 13, 1, 3, 14);
            GXSetArray(13, gor_fune_texcoord0_tbl, 4);
            sizes = gor_fune_dl_0_size_tbl;
            lists = gor_fune_dl_0_tbl;
            for (i = 0; i < 2; i++) {
                GXCallDisplayList(lists[i], (u32)sizes[i] << 5);
            }
        }

        PSMTXScale(scale, *(f32*)(w + 0x1C), *(f32*)(w + 0x24), *(f32*)(w + 0x1C));
        PSMTXConcat(trans, scale, model);
        GXLoadPosMtxImm(model, 0);
        GXSetCurrentMtx(0);
        PSMTXTrans(scale, *(f32*)(w + 0x2C), 0.0f, 0.0f);
        GXLoadTexMtxImm(scale, 0x1E, 1);
        if (mode == 0) {
            GXClearVtxDesc();
            GXSetVtxDesc(9, 2);
            GXSetVtxAttrFmt(0, 9, 1, 3, 9);
            GXSetArray(9, funemizu_vertex_tbl, 6);
            GXSetVtxDesc(10, 2);
            GXSetVtxAttrFmt(0, 10, 0, 1, 6);
            GXSetArray(10, funemizu_normal_tbl, 3);
            GXSetVtxDesc(11, 2);
            GXSetVtxAttrFmt(0, 11, 1, 5, 0);
            GXSetArray(11, funemizu_color0_tbl, 4);
            GXSetVtxDesc(13, 2);
            GXSetVtxAttrFmt(0, 13, 1, 3, 12);
            GXSetArray(13, funemizu_texcoord0_tbl, 4);
            sizes = funemizu_dl_1_size_tbl;
            lists = funemizu_dl_1_tbl;
            for (i = 0; i < 4; i++) {
                GXCallDisplayList(lists[i], (u32)sizes[i] << 5);
            }
        } else {
            GXClearVtxDesc();
            GXSetVtxDesc(9, 2);
            GXSetVtxAttrFmt(0, 9, 1, 3, 8);
            GXSetArray(9, gor_fune_vertex_tbl, 6);
            GXSetVtxDesc(10, 2);
            GXSetVtxAttrFmt(0, 10, 0, 1, 6);
            GXSetArray(10, gor_fune_normal_tbl, 3);
            GXSetVtxDesc(11, 2);
            GXSetVtxAttrFmt(0, 11, 1, 5, 0);
            GXSetArray(11, gor_fune_color0_tbl, 4);
            GXSetVtxDesc(13, 2);
            GXSetVtxAttrFmt(0, 13, 1, 3, 14);
            GXSetArray(13, gor_fune_texcoord0_tbl, 4);
            sizes = gor_fune_dl_1_size_tbl;
            lists = gor_fune_dl_1_tbl;
            for (i = 0; i < 2; i++) {
                GXCallDisplayList(lists[i], (u32)sizes[i] << 5);
            }
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effFunemizuShibukiDisp(s32 cameraId, void* effect) {
    typedef struct FunemizuChild {
        s32 mode;
        f32 x;
        f32 y;
        f32 z;
        f32 speed;
        f32 angle;
        f32 a;
        f32 b;
        f32 c;
        f32 d;
        f32 e;
        f32 f;
        s32 counter;
        f32 vy;
        f32 gravity;
        s32 delay;
        s32 alpha;
        s32 unk44;
        s32 cameraId;
    } FunemizuChild;
    typedef struct EffectEntryLocal {
        char pad0[8];
        s32 effCount;
        void* work;
    } EffectEntryLocal;

    extern void* camGetPtr(s32 cameraId);
    extern void GXSetBlendMode(s32 type, s32 srcFactor, s32 dstFactor, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 updateEnable);
    extern void PSMTXTrans(f32 mtx[3][4], double x, double y, double z);
    extern void PSMTXRotRad(f32 mtx[3][4], double angle, char axis);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32 chan, s32 enable, s32 ambSrc, s32 matSrc, s32 lightMask, s32 diffFn, s32 attnFn);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32 texCoord, s32 genType, s32 texGenSrc, s32 mtx, s32 normalize, s32 postMtx);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32 stage, s32 texCoord, s32 texMap, s32 colorChan);
    extern void GXSetTevOp(s32 stage, s32 mode);
    extern void effGetTexObj(s32 id, void* texObj);
    extern void GXLoadTexObj(void* texObj, s32 mapId);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32 vtxfmt, s32 attr, s32 cnt, s32 type, s32 frac);
    extern void GXSetChanMatColor(s32 chan, void* color);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 prim, s32 vtxfmt, s32 nverts);
    extern f32 float_deg2rad_804279c8;
    extern f32 float_3_804279cc;
    extern f32 float_1p5_804279d8;
    extern u32 dat_804279c4;

    EffectEntryLocal* eff;
    FunemizuChild* work;
    FunemizuChild* child;
    void* cam;
    void* cam2;
    f32 trans[3][4];
    f32 rot[3][4];
    f32 viewTrans[3][4];
    u32 texObj[8];
    u32 color;
    volatile f32* fifo;
    f32 negThree;
    f32 halfNeg;
    f32 oneHalf;
    f32 zero;
    f32 one;
    s32 i;

    eff = (EffectEntryLocal*)effect;
    work = (FunemizuChild*)eff->work;
    cam = camGetPtr(cameraId);

    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);

    PSMTXTrans(trans, (double)work->x, (double)work->y, (double)work->z);
    cam2 = camGetPtr(cameraId);
    PSMTXRotRad(rot, (double)(float_deg2rad_804279c8 * (work->angle - *(f32*)((s32)cam2 + 0x114))), 'y');
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat((void*)((s32)cam + 0x11C), trans, viewTrans);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    effGetTexObj(0x40, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);

    negThree = -float_3_804279cc;
    i = 1;
    child = (FunemizuChild*)((s32)work + 0x4C);
    while (i < eff->effCount) {
        if (child->delay != 0) {
            goto next_child;
        }
        if (child->y < float_0_804279d0) {
            goto next_child;
        }

        color = dat_804279c4;
            ((u8*)&color)[3] = (u8)child->alpha;
            GXSetChanMatColor(4, &color);

            PSMTXTrans(trans, (double)child->x, (double)child->y, (double)child->z);
            PSMTXConcat(viewTrans, trans, trans);
            GXLoadPosMtxImm(trans, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x80, 0, 4);

            zero = float_0_804279d0;
            one = float_1_804279dc;
            oneHalf = float_1p5_804279d8;
            halfNeg = negThree * float_0p5_804279d4;
            fifo = (volatile f32*)0xCC008000;
            *fifo = halfNeg;
            *fifo = oneHalf;
            *fifo = zero;
            *fifo = zero;
            *fifo = zero;
            *fifo = oneHalf;
            *fifo = oneHalf;
            *fifo = zero;
            *fifo = one;
            *fifo = zero;
            *fifo = oneHalf;
            *fifo = halfNeg;
            *fifo = zero;
            *fifo = one;
            *fifo = one;
            *fifo = halfNeg;
            *fifo = halfNeg;
            *fifo = zero;
            *fifo = zero;
            *fifo = one;

next_child:
        child++;
        i++;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

/*
 * Late-defined target-backed water/spout geometry/display-list data.
 *
 * The unresolved compiler/static-local +/-1 prefix is intentionally not
 * represented here.  The room-color lookup table at the end of target .data
 * is also left for a separate source-shape hypothesis.  Natural 32-byte
 * alignment reproduces the target's internal geometry gaps.
 */
u8 funemizu_vertex_tbl[162] __attribute__ ((aligned(32))) = {
    0x4E, 0x6F, 0xFD, 0x84, 0x00, 0x00, 0x35, 0xC6, 0x00, 0x00, 0x00, 0x00,
    0x22, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x15, 0xC6, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0xC6, 0x06, 0x00, 0x00, 0x00,
    0x22, 0xC6, 0x0A, 0x00, 0x00, 0x00, 0x15, 0xC6, 0x0A, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0xEA, 0x03, 0x83, 0x00, 0x00,
    0x35, 0xC6, 0x0D, 0x00, 0x00, 0x00, 0x22, 0xC6, 0x15, 0x00, 0x00, 0x00,
    0x15, 0xC6, 0x15, 0x70, 0x00, 0x00, 0x08, 0xC6, 0x0B, 0xA6, 0x00, 0x00,
    0x38, 0xCE, 0xFE, 0x74, 0x04, 0xE1, 0x27, 0x20, 0x00, 0x00, 0x04, 0xE1,
    0x19, 0x80, 0x00, 0x00, 0x04, 0xE1, 0x10, 0x2E, 0x00, 0x00, 0x04, 0xE1,
    0x02, 0x00, 0x00, 0x00, 0x04, 0xE1, 0x27, 0x20, 0x03, 0xBD, 0x04, 0xE1,
    0x19, 0x80, 0x06, 0x3C, 0x04, 0xE1, 0x10, 0x2E, 0x06, 0x3C, 0x04, 0xE1,
    0x33, 0x69, 0x02, 0x30, 0x04, 0xE1, 0x27, 0x20, 0x08, 0x1A, 0x04, 0xE1,
    0x19, 0x80, 0x0D, 0x17, 0x04, 0xE1, 0x10, 0x2E, 0x0D, 0x5E, 0x04, 0xE1,
    0x06, 0xDB, 0x07, 0x43, 0x04, 0xE1,
};

u8 funemizu_texcoord0_tbl[108] __attribute__ ((aligned(32))) = {
    0x04, 0x00, 0x10, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0C, 0x00,
    0x08, 0x00, 0x10, 0x00, 0x08, 0x00, 0x08, 0x00, 0x0C, 0x00, 0x10, 0x00,
    0x0C, 0x00, 0x08, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x08, 0x00,
    0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x01, 0x00,
    0x0C, 0x00, 0x01, 0x00, 0x10, 0x00, 0x01, 0x00, 0xB4, 0x00, 0x10, 0x00,
    0xB4, 0x00, 0x08, 0x00, 0xB0, 0x00, 0x0C, 0x00, 0xB8, 0x00, 0x10, 0x00,
    0xB8, 0x00, 0x08, 0x00, 0xBC, 0x00, 0x10, 0x00, 0xBC, 0x00, 0x08, 0x00,
    0xC0, 0x00, 0x0C, 0x00, 0xB4, 0x00, 0x01, 0x00, 0xB0, 0x00, 0x01, 0x00,
    0xB8, 0x00, 0x01, 0x00, 0xBC, 0x00, 0x01, 0x00, 0xC0, 0x00, 0x01, 0x00,
};

u8 funemizu_dl_0_0[64] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x0A, 0x09, 0x00, 0x01, 0x0A, 0x00, 0x00, 0x00, 0x02, 0x0A,
    0x00, 0x01, 0x09, 0x05, 0x00, 0x01, 0x01, 0x0B, 0x00, 0x01, 0x0B, 0x06,
    0x00, 0x01, 0x04, 0x0C, 0x00, 0x01, 0x0C, 0x07, 0x00, 0x01, 0x06, 0x0D,
    0x00, 0x01, 0x0D, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

u8 funemizu_dl_1_0[64] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x0A, 0x16, 0x00, 0x01, 0x17, 0x0E, 0x00, 0x00, 0x10, 0x17,
    0x00, 0x01, 0x16, 0x13, 0x00, 0x01, 0x0F, 0x18, 0x00, 0x01, 0x18, 0x14,
    0x00, 0x01, 0x12, 0x19, 0x00, 0x01, 0x19, 0x15, 0x00, 0x01, 0x14, 0x1A,
    0x00, 0x01, 0x1A, 0x12, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

u8 gor_fune_vertex_tbl[216] __attribute__ ((aligned(32))) = {
    0x5C, 0x35, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00,
    0x2F, 0x31, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00,
    0x10, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x5E, 0x86, 0x01, 0x4D, 0x00, 0x00, 0x45, 0x5A, 0x0C, 0xDB, 0x00, 0x00,
    0x36, 0x3E, 0x19, 0xA5, 0x00, 0x00, 0x28, 0x53, 0x23, 0x40, 0x00, 0x00,
    0x18, 0xFE, 0x24, 0x62, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x5E, 0x4B, 0x08, 0x00, 0x00, 0x00, 0x47, 0x0E, 0x12, 0x39, 0x00, 0x00,
    0x38, 0x83, 0x20, 0xCD, 0x00, 0x00, 0x2A, 0xE1, 0x2B, 0x4C, 0x00, 0x00,
    0x1B, 0xD4, 0x2D, 0x53, 0x00, 0x00, 0x08, 0xAF, 0x26, 0x2E, 0x00, 0x00,
    0x5C, 0x35, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00,
    0x2F, 0x31, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00,
    0x10, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x5E, 0x86, 0x00, 0xC8, 0x00, 0x00, 0x45, 0x5A, 0x07, 0xB6, 0x00, 0x00,
    0x36, 0x3E, 0x11, 0xC9, 0x00, 0x00, 0x28, 0x53, 0x19, 0x26, 0x00, 0x00,
    0x18, 0xFE, 0x19, 0xD4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x5E, 0x4B, 0x04, 0xCD, 0x00, 0x00, 0x47, 0x0E, 0x0A, 0xEF, 0x00, 0x00,
    0x38, 0x83, 0x16, 0x14, 0x00, 0x00, 0x2A, 0xE1, 0x1D, 0xFA, 0x00, 0x00,
    0x1B, 0xD4, 0x1F, 0x32, 0x00, 0x00, 0x0B, 0x96, 0x19, 0x16, 0x00, 0x00,
};

u8 gor_fune_texcoord0_tbl[96] __attribute__ ((aligned(32))) = {
    0xF3, 0x03, 0x40, 0x00, 0xF3, 0x03, 0x18, 0x84, 0xD5, 0x04, 0x18, 0x84,
    0xD5, 0x04, 0x40, 0x00, 0x11, 0x00, 0x40, 0x00, 0x11, 0x00, 0x18, 0x84,
    0x2E, 0xFF, 0x40, 0x00, 0x2E, 0xFF, 0x18, 0x84, 0x4C, 0xFD, 0x40, 0x00,
    0x4C, 0xFD, 0x18, 0x84, 0x6A, 0xFC, 0x40, 0x00, 0x6A, 0xFC, 0x18, 0x84,
    0xF3, 0x03, 0x04, 0x00, 0xD5, 0x04, 0x02, 0x00, 0x11, 0x00, 0x04, 0x00,
    0x2E, 0xFF, 0x04, 0x00, 0x4C, 0xFD, 0x04, 0x00, 0x6A, 0xFC, 0x02, 0x00,
    0xF3, 0x03, 0x3C, 0xCC, 0xD5, 0x04, 0x3C, 0xCC, 0x11, 0x00, 0x3C, 0xCC,
    0x2E, 0xFF, 0x3C, 0xCC, 0x4C, 0xFD, 0x3C, 0xCC, 0x6A, 0xFC, 0x3C, 0xCC,
};

u8 gor_fune_dl_0_0[64] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x0C, 0x0C, 0x00, 0x01, 0x0D, 0x06, 0x00, 0x00, 0x02, 0x0D,
    0x00, 0x01, 0x0C, 0x07, 0x00, 0x01, 0x01, 0x0E, 0x00, 0x01, 0x0E, 0x08,
    0x00, 0x01, 0x05, 0x0F, 0x00, 0x01, 0x0F, 0x09, 0x00, 0x01, 0x07, 0x10,
    0x00, 0x01, 0x10, 0x0A, 0x00, 0x01, 0x09, 0x11, 0x00, 0x01, 0x11, 0x0B,
    0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

u8 gor_fune_dl_0_1[64] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x0C, 0x05, 0x00, 0x00, 0x0A, 0x0B, 0x00, 0x00, 0x0B, 0x04,
    0x00, 0x00, 0x08, 0x0A, 0x00, 0x01, 0x09, 0x03, 0x00, 0x00, 0x06, 0x09,
    0x00, 0x01, 0x07, 0x02, 0x00, 0x00, 0x04, 0x08, 0x00, 0x01, 0x05, 0x01,
    0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x03, 0x06,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

u8 gor_fune_dl_1_0[64] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x0C, 0x1E, 0x00, 0x01, 0x0D, 0x18, 0x00, 0x00, 0x02, 0x1F,
    0x00, 0x01, 0x0C, 0x19, 0x00, 0x01, 0x01, 0x20, 0x00, 0x01, 0x0E, 0x1A,
    0x00, 0x01, 0x05, 0x21, 0x00, 0x01, 0x0F, 0x1B, 0x00, 0x01, 0x07, 0x22,
    0x00, 0x01, 0x10, 0x1C, 0x00, 0x01, 0x09, 0x23, 0x00, 0x01, 0x11, 0x1D,
    0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

u8 gor_fune_dl_1_1[64] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x0C, 0x17, 0x00, 0x00, 0x17, 0x1D, 0x00, 0x00, 0x0B, 0x16,
    0x00, 0x00, 0x16, 0x1C, 0x00, 0x01, 0x09, 0x15, 0x00, 0x00, 0x15, 0x1B,
    0x00, 0x01, 0x07, 0x14, 0x00, 0x00, 0x14, 0x1A, 0x00, 0x01, 0x05, 0x13,
    0x00, 0x00, 0x12, 0x19, 0x00, 0x01, 0x01, 0x12, 0x00, 0x00, 0x13, 0x18,
    0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

__declspec(section ".sdata") u8 funemizu_normal_tbl[3] = {
    0x00, 0x00, 0x40
};

__declspec(section ".sdata") u8 funemizu_color0_tbl[8] __attribute__ ((aligned(32))) = {
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

__declspec(section ".sdata") u8 funemizu_dl_0_1[32] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x07, 0x04, 0x00, 0x00, 0x07, 0x08, 0x00, 0x00, 0x08, 0x03, 0x00, 0x00, 0x05, 0x07, 0x00, 0x01, 0x06, 0x02, 0x00, 0x00, 0x03, 0x06, 0x00, 0x01, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 funemizu_dl_0_2[32] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x01, 0x01, 0x06, 0x00, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

extern u8 funemizu_dl_0_0[];
extern u8 funemizu_dl_1_0[];
extern u8 gor_fune_dl_0_0[];
extern u8 gor_fune_dl_0_1[];
extern u8 gor_fune_dl_1_0[];
extern u8 gor_fune_dl_1_1[];

__declspec(section ".sdata") void* funemizu_dl_0_tbl[3] = {
    (void*)funemizu_dl_0_0,
    (void*)funemizu_dl_0_1,
    (void*)funemizu_dl_0_2
};

__declspec(section ".sdata") u8 funemizu_dl_0_size_tbl[3] = {
    0x02, 0x01, 0x01
};

__declspec(section ".sdata") u8 funemizu_dl_1_1[32] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x04, 0x12, 0x00, 0x00, 0x15, 0x15, 0x00, 0x01, 0x14, 0x11, 0x00, 0x00, 0x13, 0x10, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 funemizu_dl_1_2[32] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x04, 0x0E, 0x00, 0x00, 0x10, 0x0F, 0x00, 0x00, 0x0E, 0x13, 0x00, 0x01, 0x0F, 0x14, 0x00, 0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") u8 funemizu_dl_1_3[32] __attribute__ ((aligned(32))) = {
    0x98, 0x00, 0x04, 0x15, 0x00, 0x01, 0x14, 0x14, 0x00, 0x01, 0x12, 0x10, 0x00, 0x00, 0x11, 0x0F, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

__declspec(section ".sdata") void* funemizu_dl_1_tbl[4] = {
    (void*)funemizu_dl_1_0,
    (void*)funemizu_dl_1_1,
    (void*)funemizu_dl_1_2,
    (void*)funemizu_dl_1_3
};

__declspec(section ".sdata") u8 funemizu_dl_1_size_tbl[4] = {
    0x02, 0x01, 0x01, 0x01
};

__declspec(section ".sdata") u8 gor_fune_normal_tbl[3] __attribute__ ((aligned(32))) = {
    0x00, 0x00, 0x40
};

__declspec(section ".sdata") u8 gor_fune_color0_tbl[8] __attribute__ ((aligned(32))) = {
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

__declspec(section ".sdata") void* gor_fune_dl_0_tbl[2] = {
    (void*)gor_fune_dl_0_0,
    (void*)gor_fune_dl_0_1
};

__declspec(section ".sdata") u8 gor_fune_dl_0_size_tbl[2] = {
    0x02, 0x02
};

__declspec(section ".sdata") void* gor_fune_dl_1_tbl[2] = {
    (void*)gor_fune_dl_1_0,
    (void*)gor_fune_dl_1_1
};

__declspec(section ".sdata") u8 gor_fune_dl_1_size_tbl[2] = {
    0x02, 0x02
};

__declspec(section ".rodata") const char str_dou_02_802ff0a8[7] __attribute__ ((aligned(8))) = "dou_02";
__declspec(section ".rodata") const char str_dou_03_802ff0b0[7] __attribute__ ((aligned(8))) = "dou_03";
__declspec(section ".rodata") const char str_dou_04_802ff0b8[7] __attribute__ ((aligned(8))) = "dou_04";
__declspec(section ".rodata") const char str_dou_05_802ff0c0[7] __attribute__ ((aligned(8))) = "dou_05";
__declspec(section ".rodata") const char str_dou_07_802ff0c8[7] __attribute__ ((aligned(8))) = "dou_07";
__declspec(section ".rodata") const char str_dou_08_802ff0d0[7] __attribute__ ((aligned(8))) = "dou_08";
__declspec(section ".rodata") const char str_dou_09_802ff0d8[7] __attribute__ ((aligned(8))) = "dou_09";
__declspec(section ".rodata") const char str_dou_10_802ff0e0[7] __attribute__ ((aligned(8))) = "dou_10";
__declspec(section ".rodata") const char str_dou_11_802ff0e8[7] __attribute__ ((aligned(8))) = "dou_11";
__declspec(section ".rodata") const char str_dou_12_802ff0f0[7] __attribute__ ((aligned(8))) = "dou_12";
__declspec(section ".rodata") const char str_gon_01_802ff0f8[7] __attribute__ ((aligned(8))) = "gon_01";
__declspec(section ".rodata") const char str_gor_00_802ff100[7] __attribute__ ((aligned(8))) = "gor_00";
__declspec(section ".rodata") const char str_gor_02_802ff108[7] __attribute__ ((aligned(8))) = "gor_02";
__declspec(section ".rodata") const char str_hei_01_802ff110[7] __attribute__ ((aligned(8))) = "hei_01";
__declspec(section ".rodata") const char str_las_04_802ff118[7] __attribute__ ((aligned(8))) = "las_04";
__declspec(section ".rodata") const char str_las_09_802ff120[7] __attribute__ ((aligned(8))) = "las_09";
__declspec(section ".rodata") const char str_mri_16_802ff128[7] __attribute__ ((aligned(8))) = "mri_16";
__declspec(section ".rodata") const char str_muj_00_802ff130[7] __attribute__ ((aligned(8))) = "muj_00";
__declspec(section ".rodata") const char str_muj_02_802ff138[7] __attribute__ ((aligned(8))) = "muj_02";
__declspec(section ".rodata") const char str_muj_04_802ff140[7] __attribute__ ((aligned(8))) = "muj_04";
__declspec(section ".rodata") const char str_muj_10_802ff148[7] __attribute__ ((aligned(8))) = "muj_10";
__declspec(section ".rodata") const char str_nok_00_802ff150[7] __attribute__ ((aligned(8))) = "nok_00";
__declspec(section ".rodata") const char str_nok_01_802ff158[7] __attribute__ ((aligned(8))) = "nok_01";
__declspec(section ".rodata") const char str_pik_00_802ff160[7] __attribute__ ((aligned(8))) = "pik_00";
__declspec(section ".rodata") const char str_pik_01_802ff168[7] __attribute__ ((aligned(8))) = "pik_01";
__declspec(section ".rodata") const char str_pik_04_802ff170[7] __attribute__ ((aligned(8))) = "pik_04";
__declspec(section ".rodata") const char str_tik_02_802ff178[7] __attribute__ ((aligned(8))) = "tik_02";
__declspec(section ".rodata") const char str_tik_15_802ff180[7] __attribute__ ((aligned(8))) = "tik_15";
__declspec(section ".rodata") const char str_tik_16_802ff188[7] __attribute__ ((aligned(8))) = "tik_16";
__declspec(section ".rodata") const char str_tik_17_802ff190[7] __attribute__ ((aligned(8))) = "tik_17";
__declspec(section ".rodata") const char str_tik_18_802ff198[7] __attribute__ ((aligned(8))) = "tik_18";
__declspec(section ".rodata") const char str_tik_20_802ff1a0[7] __attribute__ ((aligned(8))) = "tik_20";

/*
 * Target-backed room/area water-color lookup.
 *
 * Retail stores 32 complete {name,color} entries followed by only the
 * four-byte null name sentinel, giving the target symbol its exact 260-byte
 * extent rather than a padded 33-entry array.
 */
FunemizuColorTable unk_803b3b00 = {
    {
        { str_dou_02_802ff0a8, 0x0E2CA200 },
        { str_dou_03_802ff0b0, 0x0E2CA200 },
        { str_dou_04_802ff0b8, 0x0E2CA200 },
        { str_dou_05_802ff0c0, 0x0E2CA200 },
        { str_dou_07_802ff0c8, 0x0E2CA200 },
        { str_dou_08_802ff0d0, 0x0E2CA200 },
        { str_dou_09_802ff0d8, 0x0E2CA200 },
        { str_dou_10_802ff0e0, 0x0E2CA200 },
        { str_dou_11_802ff0e8, 0x0E2CA200 },
        { str_dou_12_802ff0f0, 0x0E2CA200 },
        { str_gon_01_802ff0f8, 0x00614900 },
        { str_gor_00_802ff100, 0x00306500 },
        { str_gor_02_802ff108, 0x00306500 },
        { str_hei_01_802ff110, 0x51C3CB00 },
        { str_las_04_802ff118, 0x081C2C00 },
        { str_las_09_802ff120, 0x45798E00 },
        { str_mri_16_802ff128, 0x00A6FF00 },
        { str_muj_00_802ff130, 0x0E2CA200 },
        { str_muj_02_802ff138, 0x20305D00 },
        { str_muj_04_802ff140, 0x0E2CA200 },
        { str_muj_10_802ff148, 0x0E2CA200 },
        { str_nok_00_802ff150, 0x69C3D700 },
        { str_nok_01_802ff158, 0x69C3D700 },
        { str_pik_00_802ff160, 0x82E3DF00 },
        { str_pik_01_802ff168, 0x6DCBFF00 },
        { str_pik_04_802ff170, 0x6DCBFF00 },
        { str_tik_02_802ff178, 0x042C5100 },
        { str_tik_15_802ff180, 0x304D5900 },
        { str_tik_16_802ff188, 0x304D5900 },
        { str_tik_17_802ff190, 0x304D5900 },
        { str_tik_18_802ff198, 0x304D5900 },
        { str_tik_20_802ff1a0, 0x304D5900 },
    },
    0
};

