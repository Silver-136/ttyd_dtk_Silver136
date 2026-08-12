#include "effect/n64/eff_kumokumo_n64.h"

/* Target-owned small read-only scalar catalog.  Definitions intentionally live
 * after all functions so MWCC cannot fold earlier consumers back into anonymous
 * duplicate literals. */
extern const u32 dat_804258f0;
extern const u32 dat_804258f4;
extern const f32 float_deg2rad_804258f8;
extern const f32 float_90_804258fc;
extern const f32 float_neg1_80425900;
extern const f32 float_0p4_80425904;
extern const f32 float_0p1_80425908;
extern const f32 float_0_8042590c;
extern const f32 float_360_80425910;
extern const f32 float_5_80425914;
extern const f32 float_1_80425918;
extern const f32 float_2_8042591c;
extern const f32 float_0p5_80425920;
extern const f32 float_3_80425924;
extern const f32 float_8_80425928;
extern const f32 float_1p5_8042592c;
extern const f32 float_30_80425930;
extern const f32 float_6p2832_80425934;
extern const f32 float_0p03125_80425938;
extern const f32 float_200_8042593c;
extern const f32 float_0p018182_80425940;
extern const f32 float_0p04_80425944;

void* effKumokumoN64Entry(f32 x, f32 y, f32 z, f32 scale, f32 unused, f32 angle, s32 type, s32 duration) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effKumokumoMain(void*);
    extern s32 irand(s32);
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_KumokumoN64_802fb3e0[];
    void* entry;
    u8* work;
    u8* part;
    s32 count;
    s32 i;
    s32 delay;
    s32 phase;

    entry = effEntry();
    count = type == 1 ? 1 : 11;
    if (type == 2) scale = float_0_8042590c;
    *(char**)((s32)entry + 0x14) = str_KumokumoN64_802fb3e0;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, count * 100);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effKumokumoMain;
    *(u32*)entry |= 2;
    *(s32*)work = type;
    *(s32*)(work + 0x14) = 0;
    if (duration < 1) {
        *(s32*)(work + 0x10) = 1000;
    } else {
        *(s32*)(work + 0x10) = duration;
    }
    *(s32*)(work + 0x24) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x5C) = angle;
    *(s32*)(work + 0x34) = 0xFF;
    *(f32*)(work + 0x58) = scale;
    *(s32*)(work + 0x60) = 4;
    *(f32*)(work + 0x38) = float_0_8042590c;
    *(f32*)(work + 0x40) = float_0_8042590c;
    *(f32*)(work + 0x48) = float_0_8042590c;
    *(f32*)(work + 0x50) = float_0_8042590c;
    *(f32*)(work + 0x3C) = float_0_8042590c;
    *(f32*)(work + 0x44) = float_0p018182_80425940;
    *(f32*)(work + 0x4C) = float_0_8042590c;
    *(f32*)(work + 0x54) = float_0p04_80425944;
    if (type == 1 || type == 3) {
        *(f32*)(work + 0x3C) *= float_neg1_80425900;
        *(f32*)(work + 0x44) *= float_neg1_80425900;
        *(f32*)(work + 0x4C) *= float_neg1_80425900;
        *(f32*)(work + 0x54) *= float_neg1_80425900;
    }

    switch (type) {
        case 0:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0xFF;
            *(s32*)(work + 0x20) = 0x96;
            *(s32*)(work + 0x28) = 0xD7;
            *(s32*)(work + 0x2C) = 0xD2;
            *(s32*)(work + 0x30) = 10;
            break;
        case 1:
            *(s32*)(work + 0x18) = 0xE6;
            *(s32*)(work + 0x1C) = 0x61;
            *(s32*)(work + 0x20) = 0x2C;
            *(s32*)(work + 0x28) = 0x68;
            *(s32*)(work + 0x2C) = 0x2F;
            *(s32*)(work + 0x30) = 0x95;
            break;
        case 2:
            *(s32*)(work + 0x18) = 0xFF;
            *(s32*)(work + 0x1C) = 0xFF;
            *(s32*)(work + 0x20) = 0xFF;
            *(s32*)(work + 0x28) = 0xFF;
            *(s32*)(work + 0x2C) = 0xFF;
            *(s32*)(work + 0x30) = 0xFF;
            break;
        default:
            *(s32*)(work + 0x18) = 0;
            *(s32*)(work + 0x1C) = 0x99;
            *(s32*)(work + 0x20) = 0xFF;
            *(s32*)(work + 0x28) = 0;
            *(s32*)(work + 0x2C) = 0;
            *(s32*)(work + 0x30) = 0;
            break;
    }

    i = 1;
    delay = 30;
    phase = 15;
    part = work;
    while (i < count) {
        if (type == 3) {
            f32 radians = float_6p2832_80425934 * (f32)irand(360) / float_360_80425910;
            f32 radius = float_200_8042593c + (f32)irand(200);
            *(f32*)(part + 0x68) = radius * (f32)sin(radians);
            *(f32*)(part + 0x6C) = radius * (f32)cos(radians);
            *(f32*)(part + 0x70) = float_0_8042590c;
            *(f32*)(part + 0xC0) = float_2_8042591c;
            *(s32*)(part + 0x88) = 0;
            *(s32*)(part + 0x74) = phase;
            *(f32*)(part + 0xBC) = float_0_8042590c;
        } else if (type == 2) {
            *(f32*)(part + 0x68) = (f32)(200 - irand(400));
            *(f32*)(part + 0x6C) = -(f32)(irand(50) + 200);
            *(f32*)(part + 0x70) = float_0_8042590c;
            *(f32*)(part + 0xC0) = float_1_80425918;
            *(s32*)(part + 0x88) = 0;
            *(s32*)(part + 0x74) = phase;
            *(f32*)(part + 0xBC) = float_0_8042590c;
        } else {
            *(f32*)(part + 0x68) = float_0_8042590c;
            *(f32*)(part + 0x6C) = float_3_80425924;
            *(f32*)(part + 0x70) = float_0_8042590c;
            *(f32*)(part + 0xC0) = float_0_8042590c;
            *(s32*)(part + 0x88) = 0xCC;
            *(s32*)(part + 0x74) = delay;
            *(f32*)(part + 0xBC) = -(f32)irand(20);
            *(f32*)(part + 0x9C) = float_8_80425928;
        }
        delay += 30;
        phase += 15;
        i++;
        part += 100;
    }
    return entry;
}

void effKumokumoMain(void* effect) {
    extern void effDelete(void*); extern s32 irand(s32); extern f64 sin(f64); extern f64 cos(f64); extern f32 dispCalcZ(void*); extern void dispEntry(s32,s32,void*,void*,f32); extern void effKumokumoDisp(s32,void*);
    u8* e=effect; u8* w=*(u8**)(e+0xC); u8* p=w; f32 pos[3]; s32 type=*(s32*)w,timer,i,camera=*(s32*)(w+0x60),delay=30;
    pos[0]=*(f32*)(w+4);pos[1]=*(f32*)(w+8);pos[2]=*(f32*)(w+0xC);if((*(u32*)e&4)!=0){*(u32*)e&=~4;*(s32*)(w+0x10)=16;}if(*(s32*)(w+0x10)<1000)(*(s32*)(w+0x10))--;(*(s32*)(w+0x14))++;timer=*(s32*)(w+0x10);if(timer<0){effDelete(effect);return;}if(timer<16)*(s32*)(w+0x24)=timer<<4;if(*(s32*)(w+0x14)<16)*(s32*)(w+0x24)=*(s32*)(w+0x14)*16+15;
    *(f32*)(w+0x38)+=*(f32*)(w+0x3C);*(f32*)(w+0x40)+=*(f32*)(w+0x44);*(f32*)(w+0x48)+=*(f32*)(w+0x4C);*(f32*)(w+0x50)+=*(f32*)(w+0x54);if(type==3)*(f32*)(w+0x58)-=float_2_8042591c;
    for(i=1;i<*(s32*)(e+8);i++,p+=0x64,delay+=30){if(*(s32*)(p+0x74)==0){if(type==0||type==4){(*(s32*)(p+0x88))-=2;if(*(s32*)(p+0x88)<0){*(f32*)(p+0x68)=float_0_8042590c;*(f32*)(p+0x6C)=float_3_80425924;*(f32*)(p+0x70)=float_0_8042590c;*(f32*)(p+0xC0)=float_0_8042590c;*(s32*)(p+0x88)=0xCC;*(s32*)(p+0x74)=delay;*(f32*)(p+0xBC)=-(f32)irand(20);*(f32*)(p+0x9C)=float_8_80425928;}else{*(f32*)(p+0xC0)+=(float_1p5_8042592c-*(f32*)(p+0xC0))/float_30_80425930;*(f32*)(p+0xBC)+=(float_30_80425930-*(f32*)(p+0xBC))/float_30_80425930;*(f32*)(p+0x68)+=*(f32*)(p+0x9C)*(f32)cos(float_6p2832_80425934**(f32*)(p+0xBC)/float_360_80425910);*(f32*)(p+0x6C)+=float_2_8042591c**(f32*)(p+0x9C)*(f32)sin(float_6p2832_80425934**(f32*)(p+0xBC)/float_360_80425910);*(f32*)(p+0x9C)+=(float_2_8042591c-*(f32*)(p+0x9C))/float_30_80425930;}}else{*(s32*)(p+0x88)+=(0xC0-*(s32*)(p+0x88))/4;*(f32*)(p+0x68) += -*(f32*)(p+0x68) * float_0p03125_80425938;*(f32*)(p+0x6C) += -*(f32*)(p+0x6C) * float_0p03125_80425938;*(f32*)(p+0x70) += -*(f32*)(p+0x70) * float_0p03125_80425938;}}else (*(s32*)(p+0x74))--;}
    dispEntry(camera,2,effKumokumoDisp,effect,dispCalcZ(pos));
}


void effKumokumoDisp(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    typedef struct GXTexObj {
        u32 data[8];
    } GXTexObj;
    typedef union F64Conv {
        f64 value;
        struct {
            u32 hi;
            u32 lo;
        } words;
    } F64Conv;

    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetArray(s32, void*, s32);
    extern void GXCallDisplayList(void*, u32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXBegin(s32, s32, s32);
    extern u32 GXGetTexObjWidth(void*);
    extern u32 GXGetTexObjHeight(void*);

    extern u8 lbl_803A5220[];
    extern u8 cloud_normal_tbl[];
    extern u8 cloud_color0_tbl[];
    extern void* cloud_dl_0_tbl[];
    extern u8 cloud_dl_0_size_tbl[];
    extern u8 batten_normal_tbl[];
    extern u8 batten_color0_tbl[];
    extern void* batten_dl_0_tbl[];
    extern u8 batten_dl_0_size_tbl[];

    extern u32 unk_80429748;
    extern u32 unk_8042974c;
    extern const u32 dat_804258f0;
    extern const u32 dat_804258f4;

    extern const f32 float_deg2rad_804258f8;
    extern const f32 float_90_804258fc;
    extern const f32 float_neg1_80425900;
    extern const f32 float_0p4_80425904;
    extern const f32 float_0p1_80425908;
    extern const f32 float_0_8042590c;
    extern const f32 float_360_80425910;
    extern const f32 float_5_80425914;
    extern const f32 float_1_80425918;
    extern const f32 float_2_8042591c;
    extern const f32 float_0p5_80425920;
    extern const f64 double_to_int_802fb3c8;
    extern const f64 double_to_int_mask_802fb3d0;

    u8* effect;
    u8* work;
    u8* part;
    u8* dataBase;
    u8* camera;
    GXTexObj tex;
    Mtx model;
    Mtx rot;
    Mtx scale;
    Mtx trans;
    u32 colorASeed;
    u32 colorA;
    u32 colorBSeed;
    u32 colorB;
    u32 partColorSeed;
    u32 partColor;
    u32 widthWord;
    u32 heightWord;
    volatile f32* fifo;
    s32 baseAlpha;
    s32 type;
    s32 alpha;
    s32 i;
    u32 j;
    f32 size;
    f32 deg;
    f32 full;
    f32 five;
    f32 converted;
    f32 width;
    f32 height;
    f32 texScale;
    f32 halfHeight;
    f32 negHalfWidth;
    f32 halfWidth;
    f32 negHalfHeight;
    f64 signedBias;

    effect = (u8*)effectAddress;
    work = *(u8**)(effect + 0xC);
    dataBase = lbl_803A5220;
    camera = camGetPtr(cameraId);
    baseAlpha = *(s32*)(work + 0x24);
    type = *(s32*)work;

    PSMTXTrans(trans,
               *(f32*)(work + 4),
               *(f32*)(work + 8),
               *(f32*)(work + 0xC));

    PSMTXRotRad(rot,
                float_deg2rad_804258f8 *
                    -*(f32*)((u8*)camGetPtr(cameraId) + 0x114),
                'y');
    PSMTXConcat(trans, rot, trans);

    PSMTXRotRad(rot,
                float_deg2rad_804258f8 * *(f32*)(work + 0x58),
                'z');
    PSMTXConcat(trans, rot, trans);

    if (type != 3) {
        if (*(f32*)(work + 0x58) < float_90_804258fc) {
            size = *(f32*)(work + 0x5C);
            PSMTXScale(scale, size, size, size);
        } else {
            size = *(f32*)(work + 0x5C);
            PSMTXScale(scale, size, float_neg1_80425900 * size, size);
        }
        PSMTXConcat(trans, scale, trans);
        PSMTXScale(scale,
                   float_0p4_80425904,
                   float_0p4_80425904,
                   float_0p4_80425904);
        PSMTXConcat(trans, scale, trans);
    } else {
        size = *(f32*)(work + 0x5C);
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(trans, scale, trans);
        PSMTXScale(scale,
                   float_0p1_80425908,
                   float_0p1_80425908,
                   float_0p1_80425908);
        PSMTXConcat(trans, scale, trans);
    }

    PSMTXConcat(camera + 0x11C, trans, model);
    GXLoadPosMtxImm(model, 0);
    GXSetCurrentMtx(0);

    colorASeed = unk_80429748;
    ((u8*)&colorASeed)[0] = *(s32*)(work + 0x18);
    ((u8*)&colorASeed)[1] = *(s32*)(work + 0x1C);
    ((u8*)&colorASeed)[2] = *(s32*)(work + 0x20);
    ((u8*)&colorASeed)[3] = baseAlpha;
    colorA = colorASeed;
    GXSetTevColor(1, &colorA);

    colorBSeed = unk_8042974c;
    ((u8*)&colorBSeed)[0] = *(s32*)(work + 0x28);
    ((u8*)&colorBSeed)[1] = *(s32*)(work + 0x2C);
    ((u8*)&colorBSeed)[2] = *(s32*)(work + 0x30);
    ((u8*)&colorBSeed)[3] = *(s32*)(work + 0x34);
    colorB = colorBSeed;
    GXSetTevColor(2, &colorB);

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTevStages(3);

    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 4, 2, 9, 15);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);

    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 0, 9, 15);
    GXSetTevAlphaIn(1, 7, 0, 4, 7);

    GXSetTevOrder(2, 0xFF, 0xFF, 4);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevColorIn(2, 15, 0, 10, 15);
    GXSetTevAlphaIn(2, 7, 0, 5, 7);

    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 5, 0x21, 0, 0x7D);

    effGetTexObj(0x68, &tex);
    GXLoadTexObj(&tex, 0);
    effGetTexObj(0x69, &tex);
    GXLoadTexObj(&tex, 1);

    PSMTXTrans(trans,
               *(f32*)(work + 0x38),
               *(f32*)(work + 0x40),
               float_0_8042590c);
    GXLoadTexMtxImm(trans, 0x1E, 1);

    PSMTXTrans(trans,
               *(f32*)(work + 0x48),
               *(f32*)(work + 0x50),
               float_0_8042590c);
    GXLoadTexMtxImm(trans, 0x21, 1);

    GXSetCullMode(0);

    if (type == 3) {
        F64Conv conv;

        deg = float_deg2rad_804258f8;
        full = float_360_80425910;
        signedBias = double_to_int_802fb3c8;
        five = float_5_80425914;
        i = 0;
        do {
            conv.words.hi = 0x43300000;
            conv.words.lo = ((u32)i) ^ 0x80000000;
            converted = (f32)(conv.value - signedBias);

            PSMTXRotRad(rot, deg * ((full * converted) / five), 'z');
            PSMTXConcat(model, rot, rot);
            GXLoadPosMtxImm(rot, 0);
            GXSetCurrentMtx(0);

            GXClearVtxDesc();
            GXSetVtxDesc(9, 2);
            GXSetVtxAttrFmt(0, 9, 1, 3, 6);
            GXSetArray(9, dataBase + 0x40, 6);
            GXSetVtxDesc(10, 2);
            GXSetVtxAttrFmt(0, 10, 0, 1, 6);
            GXSetArray(10, cloud_normal_tbl, 3);
            GXSetVtxDesc(11, 2);
            GXSetVtxAttrFmt(0, 11, 1, 5, 0);
            GXSetArray(11, cloud_color0_tbl, 4);
            GXSetVtxDesc(13, 2);
            GXSetVtxAttrFmt(0, 13, 1, 3, 14);
            GXSetArray(13, dataBase + 0x120, 4);
            GXSetVtxDesc(14, 2);
            GXSetVtxAttrFmt(0, 14, 1, 3, 14);
            GXSetArray(14, dataBase + 0x1C0, 4);

            j = 0;
            do {
                GXCallDisplayList(cloud_dl_0_tbl[j],
                                  (u32)cloud_dl_0_size_tbl[j] << 5);
                j++;
            } while (j < 4);

            i++;
        } while (i < 5);
        goto second_pass;
    }

    if (type < 3) {
        if (type > 1) {
            GXClearVtxDesc();
            GXSetVtxDesc(9, 2);
            GXSetVtxAttrFmt(0, 9, 1, 3, 6);
            GXSetArray(9, dataBase + 0x3E0, 6);
            GXSetVtxDesc(10, 2);
            GXSetVtxAttrFmt(0, 10, 0, 1, 6);
            GXSetArray(10, batten_normal_tbl, 3);
            GXSetVtxDesc(11, 2);
            GXSetVtxAttrFmt(0, 11, 1, 5, 0);
            GXSetArray(11, batten_color0_tbl, 4);
            GXSetVtxDesc(13, 2);
            GXSetVtxAttrFmt(0, 13, 1, 3, 14);
            GXSetArray(13, dataBase + 0x4C0, 4);
            GXSetVtxDesc(14, 2);
            GXSetVtxAttrFmt(0, 14, 1, 3, 14);
            GXSetArray(14, dataBase + 0x560, 4);

            j = 0;
            do {
                GXCallDisplayList(batten_dl_0_tbl[j],
                                  (u32)batten_dl_0_size_tbl[j] << 5);
                j++;
            } while (j < 4);
            goto second_pass;
        }

        if (type > -1) {
            GXClearVtxDesc();
            GXSetVtxDesc(9, 2);
            GXSetVtxAttrFmt(0, 9, 1, 3, 6);
            GXSetArray(9, dataBase + 0x40, 6);
            GXSetVtxDesc(10, 2);
            GXSetVtxAttrFmt(0, 10, 0, 1, 6);
            GXSetArray(10, cloud_normal_tbl, 3);
            GXSetVtxDesc(11, 2);
            GXSetVtxAttrFmt(0, 11, 1, 5, 0);
            GXSetArray(11, cloud_color0_tbl, 4);
            GXSetVtxDesc(13, 2);
            GXSetVtxAttrFmt(0, 13, 1, 3, 14);
            GXSetArray(13, dataBase + 0x120, 4);
            GXSetVtxDesc(14, 2);
            GXSetVtxAttrFmt(0, 14, 1, 3, 14);
            GXSetArray(14, dataBase + 0x1C0, 4);

            j = 0;
            do {
                GXCallDisplayList(cloud_dl_0_tbl[j],
                                  (u32)cloud_dl_0_size_tbl[j] << 5);
                j++;
            } while (j < 4);
            goto second_pass;
        }
    }

    GXClearVtxDesc();
    GXSetVtxDesc(9, 2);
    GXSetVtxAttrFmt(0, 9, 1, 3, 6);
    GXSetArray(9, dataBase + 0x40, 6);
    GXSetVtxDesc(10, 2);
    GXSetVtxAttrFmt(0, 10, 0, 1, 6);
    GXSetArray(10, cloud_normal_tbl, 3);
    GXSetVtxDesc(11, 2);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetArray(11, cloud_color0_tbl, 4);
    GXSetVtxDesc(13, 2);
    GXSetVtxAttrFmt(0, 13, 1, 3, 14);
    GXSetArray(13, dataBase + 0x120, 4);
    GXSetVtxDesc(14, 2);
    GXSetVtxAttrFmt(0, 14, 1, 3, 14);
    GXSetArray(14, dataBase + 0x1C0, 4);

    j = 0;
    do {
        GXCallDisplayList(cloud_dl_0_tbl[j],
                          (u32)cloud_dl_0_size_tbl[j] << 5);
        j++;
    } while (j < 4);

second_pass:
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);

    if (type == 2) {
        effGetTexObj(0xB, &tex);
        GXLoadTexObj(&tex, 0);
    } else if (type < 2 && type > -1) {
        effGetTexObj(0x34, &tex);
        GXLoadTexObj(&tex, 0);
    } else {
        effGetTexObjN64(7, &tex);
        GXLoadTexObj(&tex, 0);
    }

    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    i = 1;
    part = work + 0x64;
    while (i < *(s32*)(effect + 8)) {
        if (*(s32*)(part + 0x10) == 0) {
            alpha = (*(s32*)(part + 0x24) * baseAlpha) / 0xFF;

            if (type == 3 || type == 4) {
                partColorSeed = dat_804258f4;
            } else {
                partColorSeed = dat_804258f0;
            }
            ((u8*)&partColorSeed)[3] = alpha;
            partColor = partColorSeed;
            GXSetChanMatColor(4, &partColor);

            PSMTXTrans(trans,
                       *(f32*)(part + 4),
                       *(f32*)(part + 8),
                       *(f32*)(part + 0xC));
            PSMTXRotRad(rot,
                        float_deg2rad_804258f8 * *(f32*)(part + 0x58),
                        'z');
            size = *(f32*)(part + 0x5C);
            PSMTXScale(scale, size, size, size);
            PSMTXConcat(model, trans, trans);
            PSMTXConcat(trans, rot, trans);
            PSMTXConcat(trans, scale, trans);
            GXLoadPosMtxImm(trans, 0);
            GXSetCurrentMtx(0);

            GXBegin(0x80, 0, 4);

            {
                F64Conv conv;

                widthWord = GXGetTexObjWidth(&tex) & 0xFFFF;
                conv.words.hi = 0x43300000;
                conv.words.lo = widthWord;
                width = (f32)(conv.value - double_to_int_mask_802fb3d0);

                heightWord = GXGetTexObjHeight(&tex) & 0xFFFF;
                conv.words.hi = 0x43300000;
                conv.words.lo = heightWord;
                height = (f32)(conv.value - double_to_int_mask_802fb3d0);
            }

            if (type == 3 || type == 4) {
                texScale = float_2_8042591c;
            } else {
                texScale = float_1_80425918;
            }

            halfHeight = height * float_0p5_80425920;
            negHalfWidth = -width * float_0p5_80425920;
            halfWidth = width * float_0p5_80425920;
            negHalfHeight = -height * float_0p5_80425920;

            fifo = (volatile f32*)0xCC008000;
            *fifo = negHalfWidth;
            *fifo = halfHeight;
            *fifo = float_0_8042590c;
            *fifo = float_0_8042590c;
            *fifo = float_0_8042590c;

            *fifo = halfWidth;
            *fifo = halfHeight;
            *fifo = float_0_8042590c;
            *fifo = texScale;
            *fifo = float_0_8042590c;

            *fifo = halfWidth;
            *fifo = negHalfHeight;
            *fifo = float_0_8042590c;
            *fifo = texScale;
            *fifo = texScale;

            *fifo = negHalfWidth;
            *fifo = negHalfHeight;
            *fifo = float_0_8042590c;
            *fifo = float_0_8042590c;
            *fifo = texScale;
        }

        i++;
        part += 0x64;
    }
}

/* Keep these definitions after every function.  Their order and payloads match
 * target .sdata2 0x804258F0..0x80425948 exactly. */
const u32 dat_804258f0 = 0xDBDFE700;
const u32 dat_804258f4 = 0xFFFFFF00;
const f32 float_deg2rad_804258f8 = 0.01745329238474369f;
const f32 float_90_804258fc = 90.0f;
const f32 float_neg1_80425900 = -1.0f;
const f32 float_0p4_80425904 = 0.4000000059604645f;
const f32 float_0p1_80425908 = 0.10000000149011612f;
const f32 float_0_8042590c = 0.0f;
const f32 float_360_80425910 = 360.0f;
const f32 float_5_80425914 = 5.0f;
const f32 float_1_80425918 = 1.0f;
const f32 float_2_8042591c = 2.0f;
const f32 float_0p5_80425920 = 0.5f;
const f32 float_3_80425924 = 3.0f;
const f32 float_8_80425928 = 8.0f;
const f32 float_1p5_8042592c = 1.5f;
const f32 float_30_80425930 = 30.0f;
const f32 float_6p2832_80425934 = 6.2831854820251465f;
const f32 float_0p03125_80425938 = 0.03125f;
const f32 float_200_8042593c = 200.0f;
const f32 float_0p018182_80425940 = 0.0181818176060915f;
const f32 float_0p04_80425944 = 0.03999999910593033f;
