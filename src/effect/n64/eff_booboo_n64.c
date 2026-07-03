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

extern char str_BoobooN64_802fac08[];
extern f32 float_0_80424dec;


s32 texture_dl(int param_1, int param_2, s32 param_3) {
    return 0;
}


u8 effBoobooMain(int param_1) {
    return 0;
}


u8 boo_polygon_2(void) {
    return 0;
}


u8 boo_polygon_1(void) {
    return 0;
}


u8 boo_polygon_0(void) {
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
    extern double sin(f64 x);
    extern f32 float_deg2rad_80424dd0;
    extern f32 float_180_80424dd4;
    extern f32 float_10_80424dd8;
    extern f32 float_6p2832_80424ddc;
    extern f32 float_360_80424de0;
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
