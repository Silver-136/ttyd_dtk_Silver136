#include "effect/n64/eff_booboo_n64.h"

typedef struct EffBoobooWork {
    s32 timer;
    s32 unk_04;
    s32 unk_08[11];
    f32 unk_34[11];
    f32 unk_60[11];
    f32 unk_8c[11];
    f32 unk_b8[11];
    u8 pad_e4[0x16];
    u8 unk_fa[11];
    u8 unk_105[11];
    f32 unk_110[11];
    u8 unk_13c[11];
    u8 pad_147;
} EffBoobooWork;

void* effEntry(void);
void* __memAlloc(s32 heap, s32 size);
void effBoobooMain(void* effect);

extern char str_BoobooN64_802fac08[];
extern f32 float_0_80424dec;
extern f32 float_6p2832_80424ddc;
extern f32 float_360_80424de0;
extern double sin(f64 x);
extern void GXClearVtxDesc(void);
extern void GXSetVtxDesc(s32, s32);
extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
extern void GXSetArray(s32, void*, s32);
extern void GXCallDisplayList(void*, s32);

void boo_polygon_0(void) {
    extern u8 lbl_8039D980[];
    extern u8 boo_color0_tbl[4];
    extern u8 boo_dl_0_4[0x20],boo_dl_0_5[0x20],boo_dl_0_6[0x20],boo_dl_0_7[0x20];
    extern u8 boo_dl_0_8[0x20],boo_dl_0_9[0x20],boo_dl_0_10[0x20],boo_dl_0_11[0x20],boo_dl_0_12[0x20],boo_dl_0_13[0x20],boo_dl_0_14[0x20],boo_dl_0_15[0x20];
    extern u8 boo_dl_0_16[0x20],boo_dl_0_17[0x20],boo_dl_0_18[0x20],boo_dl_0_19[0x20],boo_dl_0_20[0x20],boo_dl_0_21[0x20],boo_dl_0_22[0x20],boo_dl_0_23[0x20];
    extern u8 boo_dl_0_24[0x20],boo_dl_0_25[0x20],boo_dl_0_26[0x20],boo_dl_0_27[0x20],boo_dl_0_28[0x20],boo_dl_0_29[0x20],boo_dl_0_30[0x20],boo_dl_0_31[0x20];
    u8* data = lbl_8039D980;
    GXClearVtxDesc();
    GXSetVtxDesc(9,2); GXSetVtxAttrFmt(0,9,1,3,14); GXSetArray(9,data + 0x40,6);
    GXSetVtxDesc(10,2); GXSetVtxAttrFmt(0,10,0,1,6); GXSetArray(10,data + 0x3E0,3);
    GXSetVtxDesc(11,2); GXSetVtxAttrFmt(0,11,1,5,0); GXSetArray(11,boo_color0_tbl,4);
    GXSetVtxDesc(13,2); GXSetVtxAttrFmt(0,13,1,3,14); GXSetArray(13,data + 0x480,4);
    GXCallDisplayList(data + 0x4E0,0x40); GXCallDisplayList(data + 0x520,0x40);
    GXCallDisplayList(data + 0x560,0x40); GXCallDisplayList(data + 0x5A0,0x40);
    GXCallDisplayList(boo_dl_0_4,0x20); GXCallDisplayList(boo_dl_0_5,0x20);
    GXCallDisplayList(boo_dl_0_6,0x20); GXCallDisplayList(boo_dl_0_7,0x20);
    GXCallDisplayList(boo_dl_0_8,0x20); GXCallDisplayList(boo_dl_0_9,0x20);
    GXCallDisplayList(boo_dl_0_10,0x20); GXCallDisplayList(boo_dl_0_11,0x20);
    GXCallDisplayList(boo_dl_0_12,0x20); GXCallDisplayList(boo_dl_0_13,0x20);
    GXCallDisplayList(boo_dl_0_14,0x20); GXCallDisplayList(boo_dl_0_15,0x20);
    GXCallDisplayList(boo_dl_0_16,0x20); GXCallDisplayList(boo_dl_0_17,0x20);
    GXCallDisplayList(boo_dl_0_18,0x20); GXCallDisplayList(boo_dl_0_19,0x20);
    GXCallDisplayList(boo_dl_0_20,0x20); GXCallDisplayList(boo_dl_0_21,0x20);
    GXCallDisplayList(boo_dl_0_22,0x20); GXCallDisplayList(boo_dl_0_23,0x20);
    GXCallDisplayList(boo_dl_0_24,0x20); GXCallDisplayList(boo_dl_0_25,0x20);
    GXCallDisplayList(boo_dl_0_26,0x20); GXCallDisplayList(boo_dl_0_27,0x20);
    GXCallDisplayList(boo_dl_0_28,0x20); GXCallDisplayList(boo_dl_0_29,0x20);
    GXCallDisplayList(boo_dl_0_30,0x20); GXCallDisplayList(boo_dl_0_31,0x20);
}

void boo_polygon_1(void) {
    extern u8 lbl_8039D980[];
    extern u8 boo_color0_tbl[4];
    extern u8 boo_dl_1_4[0x20], boo_dl_1_5[0x20], boo_dl_1_6[0x20], boo_dl_1_7[0x20];
    extern u8 boo_dl_1_8[0x20], boo_dl_1_9[0x20], boo_dl_1_10[0x20], boo_dl_1_11[0x20];
    extern u8 boo_dl_1_12[0x20], boo_dl_1_13[0x20], boo_dl_1_14[0x20], boo_dl_1_15[0x20];
    extern u8 boo_dl_1_16[0x20], boo_dl_1_17[0x20], boo_dl_1_18[0x20], boo_dl_1_19[0x20];
    extern u8 boo_dl_1_20[0x20], boo_dl_1_21[0x20], boo_dl_1_22[0x20], boo_dl_1_23[0x20];
    extern u8 boo_dl_1_24[0x20], boo_dl_1_25[0x20], boo_dl_1_26[0x20], boo_dl_1_27[0x20];
    extern u8 boo_dl_1_28[0x20], boo_dl_1_29[0x20], boo_dl_1_30[0x20], boo_dl_1_31[0x20];
    u8* data = lbl_8039D980;

    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxAttrFmt(0, 9, 1, 3, 14);
    GXSetArray(9, data + 0x40, 6);
    GXSetVtxDesc(10, 2);
    GXSetVtxAttrFmt(0, 10, 0, 1, 6);
    GXSetArray(10, data + 0x3E0, 3);
    GXSetVtxDesc(11, 2);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetArray(11, boo_color0_tbl, 4);
    GXSetVtxDesc(13, 2);
    GXSetVtxAttrFmt(0, 13, 1, 3, 14);
    GXSetArray(13, data + 0x480, 4);
    GXCallDisplayList(data + 0x5E0, 0x40);
    GXCallDisplayList(data + 0x620, 0x40);
    GXCallDisplayList(data + 0x660, 0x40);
    GXCallDisplayList(data + 0x6A0, 0x40);
    GXCallDisplayList(boo_dl_1_4, 0x20);
    GXCallDisplayList(boo_dl_1_5, 0x20);
    GXCallDisplayList(boo_dl_1_6, 0x20);
    GXCallDisplayList(boo_dl_1_7, 0x20);
    GXCallDisplayList(boo_dl_1_8, 0x20);
    GXCallDisplayList(boo_dl_1_9, 0x20);
    GXCallDisplayList(boo_dl_1_10, 0x20);
    GXCallDisplayList(boo_dl_1_11, 0x20);
    GXCallDisplayList(boo_dl_1_12, 0x20);
    GXCallDisplayList(boo_dl_1_13, 0x20);
    GXCallDisplayList(boo_dl_1_14, 0x20);
    GXCallDisplayList(boo_dl_1_15, 0x20);
    GXCallDisplayList(boo_dl_1_16, 0x20);
    GXCallDisplayList(boo_dl_1_17, 0x20);
    GXCallDisplayList(boo_dl_1_18, 0x20);
    GXCallDisplayList(boo_dl_1_19, 0x20);
    GXCallDisplayList(boo_dl_1_20, 0x20);
    GXCallDisplayList(boo_dl_1_21, 0x20);
    GXCallDisplayList(boo_dl_1_22, 0x20);
    GXCallDisplayList(boo_dl_1_23, 0x20);
    GXCallDisplayList(boo_dl_1_24, 0x20);
    GXCallDisplayList(boo_dl_1_25, 0x20);
    GXCallDisplayList(boo_dl_1_26, 0x20);
    GXCallDisplayList(boo_dl_1_27, 0x20);
    GXCallDisplayList(boo_dl_1_28, 0x20);
    GXCallDisplayList(boo_dl_1_29, 0x20);
    GXCallDisplayList(boo_dl_1_30, 0x20);
    GXCallDisplayList(boo_dl_1_31, 0x20);
}

void boo_polygon_2(void) {
    extern u8 lbl_8039D980[];
    extern u8 boo_color0_tbl[4];
    extern u8 boo_dl_2_4[0x20], boo_dl_2_5[0x20], boo_dl_2_6[0x20], boo_dl_2_7[0x20];
    extern u8 boo_dl_2_8[0x20], boo_dl_2_9[0x20], boo_dl_2_10[0x20], boo_dl_2_11[0x20];
    extern u8 boo_dl_2_12[0x20], boo_dl_2_13[0x20], boo_dl_2_14[0x20], boo_dl_2_15[0x20];
    extern u8 boo_dl_2_16[0x20], boo_dl_2_17[0x20], boo_dl_2_18[0x20], boo_dl_2_19[0x20];
    extern u8 boo_dl_2_20[0x20], boo_dl_2_21[0x20], boo_dl_2_22[0x20], boo_dl_2_23[0x20];
    extern u8 boo_dl_2_24[0x20], boo_dl_2_25[0x20], boo_dl_2_26[0x20], boo_dl_2_27[0x20];
    extern u8 boo_dl_2_28[0x20], boo_dl_2_29[0x20], boo_dl_2_30[0x20], boo_dl_2_31[0x20];
    u8* data = lbl_8039D980;

    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxAttrFmt(0, 9, 1, 3, 14);
    GXSetArray(9, data + 0x40, 6);
    GXSetVtxDesc(10, 2);
    GXSetVtxAttrFmt(0, 10, 0, 1, 6);
    GXSetArray(10, data + 0x3E0, 3);
    GXSetVtxDesc(11, 2);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetArray(11, boo_color0_tbl, 4);
    GXSetVtxDesc(13, 2);
    GXSetVtxAttrFmt(0, 13, 1, 3, 14);
    GXSetArray(13, data + 0x480, 4);
    GXCallDisplayList(data + 0x6E0, 0x40);
    GXCallDisplayList(data + 0x720, 0x40);
    GXCallDisplayList(data + 0x760, 0x40);
    GXCallDisplayList(data + 0x7A0, 0x40);
    GXCallDisplayList(boo_dl_2_4, 0x20);
    GXCallDisplayList(boo_dl_2_5, 0x20);
    GXCallDisplayList(boo_dl_2_6, 0x20);
    GXCallDisplayList(boo_dl_2_7, 0x20);
    GXCallDisplayList(boo_dl_2_8, 0x20);
    GXCallDisplayList(boo_dl_2_9, 0x20);
    GXCallDisplayList(boo_dl_2_10, 0x20);
    GXCallDisplayList(boo_dl_2_11, 0x20);
    GXCallDisplayList(boo_dl_2_12, 0x20);
    GXCallDisplayList(boo_dl_2_13, 0x20);
    GXCallDisplayList(boo_dl_2_14, 0x20);
    GXCallDisplayList(boo_dl_2_15, 0x20);
    GXCallDisplayList(boo_dl_2_16, 0x20);
    GXCallDisplayList(boo_dl_2_17, 0x20);
    GXCallDisplayList(boo_dl_2_18, 0x20);
    GXCallDisplayList(boo_dl_2_19, 0x20);
    GXCallDisplayList(boo_dl_2_20, 0x20);
    GXCallDisplayList(boo_dl_2_21, 0x20);
    GXCallDisplayList(boo_dl_2_22, 0x20);
    GXCallDisplayList(boo_dl_2_23, 0x20);
    GXCallDisplayList(boo_dl_2_24, 0x20);
    GXCallDisplayList(boo_dl_2_25, 0x20);
    GXCallDisplayList(boo_dl_2_26, 0x20);
    GXCallDisplayList(boo_dl_2_27, 0x20);
    GXCallDisplayList(boo_dl_2_28, 0x20);
    GXCallDisplayList(boo_dl_2_29, 0x20);
    GXCallDisplayList(boo_dl_2_30, 0x20);
    GXCallDisplayList(boo_dl_2_31, 0x20);
}

void* effBoobooN64Entry(void) {
    void* entry = effEntry();
    EffBoobooWork* work;
    register void* iter;
    register s32 i;
    f32 zero;

    *(char**)((s32)entry + 0x14) = str_BoobooN64_802fac08;
    *(s32*)((s32)entry + 0x8) = 1;
    work = __memAlloc(3, 0x148);
    *(EffBoobooWork**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effBoobooMain;
    work->unk_04 = 0;
    iter = work;
    i = 0;
    zero = float_0_80424dec;
    work->timer = 1000;
    while (i < 11) {
        *(f32*)((s32)iter + 0x34) = zero;
        *(f32*)((s32)iter + 0x60) = zero;
        *(f32*)((s32)iter + 0x8C) = zero;
        *(f32*)((s32)iter + 0xB8) = zero;
        *(s32*)((s32)iter + 0x8) = 0;
        *((u8*)work + i + 0xFA) = 1;
        *((u8*)work + i + 0x105) = 1;
        *(f32*)((s32)iter + 0x110) = zero;
        iter = (void*)((s32)iter + 4);
        *((u8*)work + i + 0x13C) = 0;
        i++;
    }

    return entry;
}

void effBoobooMain(void* effect) {
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effBoobooDisp(s32, void*);
    extern f32 float_2_80424de8, float_0_80424dec, float_4_80424df0, float_12_80424df4;
    extern u8 anime_data_walk[8];
    register s32* wordSlot;
    register u8* slot;
    register EffBoobooWork* work;
    register s32 i;

    work = *(EffBoobooWork**)((u8*)effect + 0xC);
    work->unk_04++;
    wordSlot = (s32*)work;
    for (i = 0; i < 11; i++, wordSlot++) {
        u8 state;
        register s32 done = 0;
        register s32 frame;
        slot = (u8*)work + i;
        state = *(u8*)(slot + 0xFA);
        frame = *(s8*)(slot + 0x13C);
        switch (state) {
            case 0: done = 1; break;
            case 1: *(u8*)(slot + 0x105) = 1; done = 1; break;
            case 3:
                *(u8*)(slot + 0x105) = anime_data_walk[frame % 8];
                *(f32*)((u8*)wordSlot + 0xB8) += (f32)sin((float_6p2832_80424ddc * (f32)(frame * 10)) / float_360_80424de0);
                if (frame >= 36) { done = 1; *(u8*)(slot + 0x13C) = 0; }
                break;
            case 2:
                *(u8*)(slot + 0x105) = anime_data_walk[frame % 8];
                *(f32*)((u8*)wordSlot + 0x110) = float_2_80424de8 * (f32)sin((float_6p2832_80424ddc * (f32)(frame * 20)) / float_360_80424de0);
                done = 1;
                if (frame >= 9) { *(f32*)((u8*)wordSlot + 0x110) = float_0_80424dec; *(u8*)(slot + 0x13C) = 0; }
                break;
            case 4:
                *(u8*)(slot + 0x105) = anime_data_walk[frame % 8];
                *(f32*)((u8*)wordSlot + 0x110) = float_4_80424df0 * (f32)sin((float_6p2832_80424ddc * (f32)(frame * 20)) / float_360_80424de0);
                if (frame >= 9) { done = 1; *(f32*)((u8*)wordSlot + 0x110) = float_0_80424dec; *(u8*)(slot + 0x13C) = 0; }
                break;
            case 5:
            default:
                *(u8*)(slot + 0x105) = anime_data_walk[frame % 8];
                *(f32*)((u8*)wordSlot + 0x110) = float_12_80424df4 * (f32)sin((float_6p2832_80424ddc * (f32)(frame * 5)) / float_360_80424de0);
                if (frame >= 36) { done = 1; *(f32*)((u8*)wordSlot + 0x110) = float_0_80424dec; *(u8*)(slot + 0x13C) = 0; }
                break;
        }
        (*(s8*)(slot + 0x13C))++;
        if (done && ((*wordSlot & 2) != 0)) {
            *wordSlot &= ~2;
            *(u8*)(slot + 0xFA) = *(u8*)(slot + 0xEF);
            *(u8*)(slot + 0x13C) = 0;
        }
    }
    dispEntry(4, 0, effBoobooDisp, effect, float_0_80424dec);
}

s32 texture_dl(int effect, int index, s32 texture) {
    extern void GXSetNumChans(s32); extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTexGens(s32); extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);
    extern void GXSetNumTevStages(s32); extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32); extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32); extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void effGetTexObjN64(s32,void*); extern void GXLoadTexObj(void*,s32);
    u8 obj[0x20];

#define LOAD_BOO_TEXTURE(tex_id, final_color) \
    do { \
        effGetTexObjN64((tex_id), obj); \
        GXLoadTexObj(obj, 0); \
        GXSetNumTexGens(1); \
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D); \
        GXSetNumTevStages(1); \
        GXSetTevOrder(0, 0, 0, 4); \
        GXSetTevColorOp(0, 0, 0, 0, 1, 0); \
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0); \
        GXSetTevColorIn(0, 15, 8, 10, (final_color)); \
        GXSetTevAlphaIn(0, 7, 7, 7, 5); \
    } while (0)

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    switch (texture) {
        case 0: LOAD_BOO_TEXTURE(0x38, 15); break;
        case 1:
            GXSetNumTexGens(0);
            GXSetNumTevStages(1);
            GXSetTevOrder(0, 0, 0, 4);
            GXSetTevColorOp(0, 0, 0, 0, 1, 0);
            GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
            GXSetTevColorIn(0, 15, 15, 15, 10);
            GXSetTevAlphaIn(0, 7, 7, 7, 5);
            break;
        case 2: LOAD_BOO_TEXTURE(0x39, 15); break;
        case 3: LOAD_BOO_TEXTURE(0x3A, 15); break;
        case 4: LOAD_BOO_TEXTURE(0x3B, 15); break;
        case 5: LOAD_BOO_TEXTURE(0x3C, 15); break;
        case 6: LOAD_BOO_TEXTURE(0x3D, 15); break;
        case 7: LOAD_BOO_TEXTURE(0x3E, 15); break;
        case 8: LOAD_BOO_TEXTURE(0x3F, 2); break;
        case 9: LOAD_BOO_TEXTURE(0x40, 2); break;
        default: return 1;
    }
#undef LOAD_BOO_TEXTURE
    return 0;
}

void effBoobooDisp(int param_1, void* param_2) {
    extern void* camGetPtr(s32 id);
    extern void PSMTXTrans(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(void* mtx, s32 axis, f32 rad);
    extern void PSMTXScale(void* mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(void* a, void* b, void* out);
    extern void GXLoadPosMtxImm(void* mtx, s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXSetTevColor(s32 id, void* color);
    extern void GXSetCullMode(s32 mode);
    extern f32 float_deg2rad_80424dd0;
    extern f32 float_180_80424dd4;
    extern f32 float_10_80424dd8;
    extern f32 float_63_80424de4;
    extern u32 unk_80429650;
    f32 mtxA[3][4];
    f32 mtxB[3][4];
    f32 mtxC[3][4];
    EffBoobooWork* work;
    void* cam;
    s32 i;
    s32 angle;
    u32 color;
    u8 poly;
    u8 tex;

    cam = camGetPtr(param_1);
    work = *(EffBoobooWork**)((s32)param_2 + 0xC);
    angle = work->unk_04 * 0x1E;

    for (i = 0; i < 11; i++, angle += 0x14) {
        if ((work->unk_08[i] & 1) != 0) {
            PSMTXTrans(mtxA, work->unk_34[i], work->unk_60[i] + work->unk_110[i], work->unk_8c[i]);
            PSMTXRotRad(mtxB, 0x79, (float_180_80424dd4 - work->unk_b8[i]) * float_deg2rad_80424dd0);
            PSMTXConcat(mtxA, mtxB, mtxA);
            PSMTXScale(mtxC, float_10_80424dd8, float_10_80424dd8, float_10_80424dd8);
            PSMTXConcat(mtxA, mtxC, mtxA);
            PSMTXConcat((void*)((s32)cam + 0x11C), mtxA, mtxA);
            GXLoadPosMtxImm(mtxA, 0);
            GXSetCurrentMtx(0);

            tex = *((u8*)work + i + 0xE4);
            if (texture_dl((s32)param_2, i, tex) == 0) {
                if (tex == 8 || tex == 9) {
                    s32 alpha = (s32)((f32)sin(((f32)angle * float_6p2832_80424ddc) / float_360_80424de0) *
                                      float_63_80424de4 + float_63_80424de4);
                    color = unk_80429650;
                    ((u8*)&color)[0] = alpha;
                    ((u8*)&color)[1] = alpha;
                    ((u8*)&color)[2] = alpha;
                    GXSetTevColor(1, &color);
                }

                poly = work->unk_105[i];
                GXSetCullMode(2);
                switch (poly) {
                    case 0:
                        boo_polygon_0();
                        break;
                    case 1:
                        boo_polygon_1();
                        break;
                    case 2:
                        boo_polygon_2();
                        break;
                }
            }
        }
    }
}
