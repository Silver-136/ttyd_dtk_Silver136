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


s32 texture_dl(int effect,int index,s32 texture) {
    extern void GXSetNumChans(s32);extern void GXSetChanCtrl(s32,s32,s32,s32,s32,s32,s32);extern void GXSetNumTexGens(s32);extern void GXSetTexCoordGen2(s32,s32,s32,s32,s32,s32);extern void GXSetNumTevStages(s32);extern void GXSetTevOrder(s32,s32,s32,s32);extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);extern void GXSetTevColorIn(s32,s32,s32,s32,s32);extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);extern void effGetTexObjN64(s32,void*);extern void GXLoadTexObj(void*,s32);u8 obj[0x20];s32 id;
    GXSetNumChans(1);GXSetChanCtrl(4,0,0,1,0,0,2);
    switch(texture){case 0:id=0x38;break;case 1:GXSetNumTexGens(0);GXSetNumTevStages(1);GXSetTevOrder(0,0xFF,0xFF,4);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);GXSetTevColorIn(0,0,0,0,4);GXSetTevAlphaIn(0,0,0,0,2);return 0;case 2:id=0x39;break;case 3:id=0x3A;break;case 4:id=0x3B;break;case 5:id=0x3C;break;case 6:id=0x3D;break;case 7:id=0x3E;break;case 8:id=0x3F;break;case 9:id=0x40;break;default:return 1;}
    effGetTexObjN64(id,obj);GXLoadTexObj(obj,0);GXSetNumTexGens(1);GXSetTexCoordGen2(0,1,4,0x3C,0,0x7D);GXSetNumTevStages(1);GXSetTevOrder(0,0,0,4);GXSetTevColorOp(0,0,0,0,1,0);GXSetTevAlphaOp(0,0,0,0,1,0);if(texture>=8)GXSetTevColorIn(0,0,8,2,0);else GXSetTevColorIn(0,0,0,0,8);GXSetTevAlphaIn(0,0,0,0,1);return 0;
}

void effBoobooMain(void* effect) {
    extern double sin(f64 x);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, void* param, f32 z);
    extern void effBoobooDisp(s32 cameraId, void* effect);
    extern f32 float_6p2832_80424ddc;
    extern f32 float_360_80424de0;
    extern f32 float_2_80424de8;
    extern f32 float_0_80424dec;
    extern f32 float_4_80424df0;
    extern f32 float_12_80424df4;
    extern u8 anime_data_walk[];
    EffBoobooWork* work;
    s32 i;
    u8* byteWork;

    work = *(EffBoobooWork**)((s32)effect + 0xC);
    work->unk_04++;
    byteWork = (u8*)work;

    for (i = 0; i < 11; i++) {
        s8 frame = *(s8*)(byteWork + i + 0x13C);
        u8 state = *(u8*)(byteWork + i + 0xFA);
        s32 done = 0;

        switch (state) {
            case 0:
                done = 1;
                break;
            case 1:
                done = 1;
                *(u8*)(byteWork + i + 0x105) = 1;
                break;
            case 2:
                *(u8*)(byteWork + i + 0x105) = anime_data_walk[frame % 8];
                work->unk_110[i] = float_2_80424de8 *
                    (f32)sin((float_6p2832_80424ddc * (f32)(frame * 20)) / float_360_80424de0);
                done = 1;
                if (frame > 8) {
                    done = 1;
                    work->unk_110[i] = float_0_80424dec;
                    *(u8*)(byteWork + i + 0x13C) = 0;
                }
                break;
            case 3:
                *(u8*)(byteWork + i + 0x105) = anime_data_walk[frame % 8];
                work->unk_b8[i] +=
                    (f32)sin((float_6p2832_80424ddc * (f32)(frame * 10)) / float_360_80424de0);
                if (frame > 0x23) {
                    done = 1;
                    *(u8*)(byteWork + i + 0x13C) = 0;
                }
                break;
            case 4:
                *(u8*)(byteWork + i + 0x105) = anime_data_walk[frame % 8];
                work->unk_110[i] = float_4_80424df0 *
                    (f32)sin((float_6p2832_80424ddc * (f32)(frame * 20)) / float_360_80424de0);
                if (frame > 8) {
                    done = 1;
                    work->unk_110[i] = float_0_80424dec;
                    *(u8*)(byteWork + i + 0x13C) = 0;
                }
                break;
            default:
                *(u8*)(byteWork + i + 0x105) = anime_data_walk[frame % 8];
                work->unk_110[i] = float_12_80424df4 *
                    (f32)sin((float_6p2832_80424ddc * (f32)(frame * 5)) / float_360_80424de0);
                if (frame > 0x23) {
                    done = 1;
                    work->unk_110[i] = float_0_80424dec;
                    *(u8*)(byteWork + i + 0x13C) = 0;
                }
                break;
        }

        (*(s8*)(byteWork + i + 0x13C))++;
        if (done && ((work->unk_08[i] & 2) != 0)) {
            work->unk_08[i] &= ~2;
            *(u8*)(byteWork + i + 0xFA) = *(u8*)(byteWork + i + 0xEF);
            *(u8*)(byteWork + i + 0x13C) = 0;
        }
    }

    dispEntry(4, 0, effBoobooDisp, effect, float_0_80424dec);
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
