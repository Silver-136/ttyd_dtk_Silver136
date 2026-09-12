#include "effect/eff_starstone.h"

extern void* camGetPtr(s32);
extern void mapSetMaterialFog(void);
extern void GXSetArray(s32, void*, s32);
extern void GXSetNumChans(s32);
extern void GXSetChanMatColor(s32, void*);
extern void GXSetNumTexGens(s32);
extern void GXSetNumTevStages(s32);
extern void GXSetCurrentMtx(s32);
extern void GXSetZCompLoc(s32);
extern void GXSetCullMode(s32);
extern void GXClearVtxDesc(void);

extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
extern void GXSetTevOrder(s32, s32, s32, s32);
extern void GXSetTevOp(s32, s32);
extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
extern void GXSetTevColor(s32, void*);
extern void PSMTXConcat(void*, void*, void*);
extern void GXSetBlendMode(s32, s32, s32, s32);
extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
extern void GXSetZMode(s32, s32, s32);
extern void GXSetVtxDesc(s32, s32);
extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
extern void GXCallDisplayList(void*, u32);

typedef struct StarStoneVecCatalog {
    f32 x;
    f32 y;
    f32 z;
} StarStoneVecCatalog;

/* stub-fill: effStarStoneEntry | missing_definition | ghidra_signature */
void* effStarStoneEntry(f64 xPos, f64 yPos, f64 zPos, f64 param_4, s32 param_5) {
    typedef struct GXColorLocal {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    } GXColorLocal;
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern s32 __mulhw(s32, s32);
    extern u8 effStarStoneMain(s32 effEntry);
    extern void* effSnowDustN64Entry(f64 x, f64 y, f64 z, f64 scaleX, f64 scaleY, f64 scaleZ, s32 unk1, s32 unk2, s32 unk3);
    extern void* gpGlobals;
    extern const char str_StarStone_802fe6e0[];
    extern f32 float_0_80427094;
    extern f32 float_1_8042709c;
    extern f32 float_10_804270a4;
    extern f32 float_20_804270b4;
    extern f32 float_255_804270b8;
    extern f32 float_90_804270c0;
    extern f64 float_1p5_804270d4;
    extern f32 edge_color[8][3];
    extern u8* color_tbl[];
    extern u32 dat_80427088;

    void* entry;
    void* work;
    s32 kind;
    s32 quotient;
    f32 zero;
    f32 scale;
    GXColorLocal color;
    u8* tbl;

    entry = effEntry();
    *(const char**)((s32)entry + 0x14) = str_StarStone_802fe6e0;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, *(s32*)((s32)entry + 8) * 0x64);
    *(void**)((s32)entry + 0xC) = work;
    *(void (**)(void))((s32)entry + 0x10) = (void (*)(void))effStarStoneMain;
    *(u32*)entry |= 2;

    quotient = (__mulhw((s32)0x92492493, param_5) + param_5) >> 2;
    quotient += (u32)quotient >> 31;
    kind = param_5 - quotient * 7;
    *(s32*)work = kind;
    *(u16*)((s32)work + 4) = 0;
    *(f32*)((s32)work + 8) = (f32)xPos;
    *(f32*)((s32)work + 0xC) = (f32)yPos;
    *(f32*)((s32)work + 0x10) = (f32)zPos;
    zero = float_0_80427094;
    scale = (f32)(float_1p5_804270d4 * (f64)param_4);
    *(f32*)((s32)work + 0x14) = scale;
    *(f32*)((s32)work + 0x1C) = zero;
    *(f32*)((s32)work + 0x24) = zero;
    *(f32*)((s32)work + 0x28) = zero;
    *(s32*)((s32)work + 0x38) = 4;
    *(s32*)((s32)work + 0x3C) = 0;
    *(f32*)((s32)work + 0x4C) = zero;
    *(f32*)((s32)work + 0x48) = zero;
    *(f32*)((s32)work + 0x54) = zero;
    *(f32*)((s32)work + 0x50) = zero;
    *(f32*)((s32)work + 0x58) = float_90_804270c0;
    *(u8*)((s32)work + 0x5C) = 0xFF;
    *(u8*)((s32)work + 0x5D) = 0xFF;
    *(u8*)((s32)work + 0x5E) = 0xFF;
    *(u8*)((s32)work + 0x5F) = 0xFF;
    *(s32*)((s32)work + 0x60) = 0;

    color.r = (u8)(s32)(float_255_804270b8 * edge_color[kind][0]);
    color.g = (u8)(s32)(float_255_804270b8 * edge_color[kind][1]);
    color.a = dat_80427088;
    color.b = (u8)(s32)(float_255_804270b8 * edge_color[kind][2]);
    *(GXColorLocal*)((s32)work + 0x30) = color;

    if (param_5 > 6) {
        *(u16*)((s32)work + 4) |= 2;
    }
    if ((*(u16*)((s32)work + 4) & 2) == 0) {
        tbl = color_tbl[kind];
        *(void**)((s32)work + 0x34) = effSnowDustN64Entry(
            (f64)xPos,
            -((f64)(float_10_804270a4 * scale) - (f64)yPos),
            (f64)zPos,
            (f64)(float_20_804270b4 * scale),
            (f64)(float_20_804270b4 * scale),
            (f64)float_1_8042709c,
            2, 4, 0);
        *(u32*)((s32)*(void**)((s32)*(void**)((s32)work + 0x34) + 0xC) + 0x4C) = tbl[0];
        *(u32*)((s32)*(void**)((s32)*(void**)((s32)work + 0x34) + 0xC) + 0x50) = tbl[1];
        *(u32*)((s32)*(void**)((s32)*(void**)((s32)work + 0x34) + 0xC) + 0x54) = tbl[2];
    }
    *(u16*)((s32)*(void**)((s32)entry + 0xC) + 4) |= 4;
    if (param_5 > 6) {
        *(u16*)((s32)*(void**)((s32)entry + 0xC) + 4) &= 0xFFFB;
    }
    if (*(s32*)((s32)gpGlobals + 0x14) != 0) {
        *(u16*)((s32)*(void**)((s32)entry + 0xC) + 4) &= 0xFFFB;
    }
    return entry;
}

const StarStoneVecCatalog vec3_802fe698 = { 0.0f, 0.0f, 0.0f };
const char str_SFX_EVT_STARSTONE_SH_802fe6a4[] = "SFX_EVT_STARSTONE_SHINE1";
const char str_SFX_EVT_STARSTONE_JU_802fe6c0[] = "SFX_EVT_STARSTONE_JUMP1";
const f64 double_to_int_mask_802fe6d8 = 4503599627370496.0;
const char str_StarStone_802fe6e0[] = "StarStone";

/* stub-fill: effStarStoneMain | missing_definition | ghidra_signature */
u8 effStarStoneMain(s32 effEntry) {
    typedef StarStoneVecCatalog VecLocal;
    typedef union StarStoneU8Double {
        f64 value;
        struct {
            u32 hi;
            u32 lo;
        } words;
    } StarStoneU8Double;
    extern void effDelete(void* effect);
    extern void effSoftDelete(void* effect);
    extern void* camGetPtr(s32 cameraId);
    extern f32 dispCalcZ(VecLocal* pos);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effStarStoneDisp_1(void), effStarStoneDisp_2(void);
    extern u32 psndSFXOn_3D(const char*, VecLocal*);
    extern void psndSFXOff(u32), psndSFX_pos(u32, VecLocal*);
    extern double sin(double), cos(double);
    extern s32 hitCheckFilter(f64,f64,f64,f64,f64,f64,s32,void*,void*,void*,void*,void*,void*,void*);
    extern s32 effStardustN64Entry();
    extern void effStardustN64SetColor(f64,f64,f64,s32);
    extern void effStardustN64SetDrawCam(s32,s32);
    extern s32 shadowEntry(f64,f64,f64,f64);
    extern void U_shadowSetMode(s32,u8);
    extern u8* color_tbl[];
    extern const f32 float_0_80427094, float_0p01_80427098, float_1_8042709c;
    extern const f32 float_0p4_804270a0, float_10_804270a4, float_neg1_804270a8;
    extern const f32 float_neg0p5_804270ac, float_5_804270b0, float_20_804270b4;
    extern const f32 float_255_804270b8, float_6p2832_804270bc, float_90_804270c0;
    extern const f32 float_360_804270c4, float_neg90_804270c8, float_neg1000_804270cc;
    extern const f32 float_8_804270d0;
    u8* effect = (u8*)effEntry;
    u8* work = *(u8**)(effect + 0xC);
    VecLocal basePos = vec3_802fe698;
    VecLocal pos;
    s32 type = *(s32*)work;
    u16 flags = *(u16*)(work + 4);
    s32 phase = *(s32*)(work + 0x3C);
    s32 cameraId = *(s32*)(work + 0x38);
    s32 timer, spawned, shadow;
    f32 angle, sinAngle, cosAngle, hitY, hitDist, hitA, hitB, hitC, hitD, hitE, hitF;
    void* camera;
    u8* color;
    StarStoneU8Double cvt0, cvt1, cvt2;
    f64 conversionMask;

    basePos.x = *(f32*)(work + 8); basePos.y = *(f32*)(work + 0xC); basePos.z = *(f32*)(work + 0x10);
    pos = basePos;
    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        if (*(void**)(work + 0x34) != 0) effSoftDelete(*(void**)(work + 0x34));
        if ((flags & 8) != 0) psndSFXOff(*(u32*)(work + 0x60));
        effDelete(effect);
        return 0;
    }
    if ((flags & 4) != 0) {
        if ((flags & 8) == 0) {
            flags |= 8;
            *(u16*)(work + 4) = flags;
            *(u32*)(work + 0x60) = psndSFXOn_3D(str_SFX_EVT_STARSTONE_SH_802fe6a4, &pos);
        }
        if ((*(u16*)(work + 4) & 8) != 0) {
            psndSFX_pos(*(u32*)(work + 0x60), &pos);
        }
    } else if ((flags & 8) != 0) {
        flags &= ~8;
        *(u16*)(work + 4) = flags;
        psndSFXOff(*(u32*)(work + 0x60));
    }
    if (phase == 1) {
        *(u16*)(work + 4) |= 1;
        *(f32*)(work + 0x44) = float_0_80427094;
        *(f32*)(work + 0x40) = float_0p4_804270a0;
        *(s32*)(work + 0x3C) = phase + 1;
    } else if (phase == 0) {
        if (type == 6) {
            *(f32*)(work + 0x24) -= float_0p01_80427098;
            if (*(f32*)(work + 0x24) <= float_0_80427094) *(f32*)(work + 0x24) += float_1_8042709c;
        } else {
            *(f32*)(work + 0x24) += float_0p01_80427098;
            if (*(f32*)(work + 0x24) >= float_1_8042709c) *(f32*)(work + 0x24) -= float_1_8042709c;
        }
    } else if (phase < 6) {
        s32 bounced = 0;
        if (*(f32*)(work + 0x44) < float_0_80427094) {
            hitDist = float_10_804270a4 + (f32)__fabs(*(f32*)(work + 0x44));
            if (hitCheckFilter(*(f32*)(work+8), *(f32*)(work+0xC), *(f32*)(work+0x10),
                               float_0_80427094, float_neg1_804270a8, float_0_80427094, 0, &hitA,&hitY,&hitB,&hitDist,&hitC,&hitD,&hitE)) {
                bounced = 1;
                *(f32*)(work + 0xC) = hitY + float_10_804270a4;
                *(f32*)(work + 0x44) *= float_neg0p5_804270ac;
                *(s32*)(work + 0x3C) += 1;
                if ((*(u16*)(work + 4) & 8) != 0) {
                    *(u16*)(work + 4) &= ~8;
                    psndSFXOff(*(u32*)(work + 0x60));
                    *(u16*)(work + 4) &= ~4;
                }
                if ((*(u16*)(work + 4) & 0x10) == 0)
                    psndSFXOn_3D(str_SFX_EVT_STARSTONE_JU_802fe6c0, &pos);
                if (((*(u16*)(work + 4) & 2) == 0) && (*(s32*)(work + 0x3C) == 3)) {
                    spawned = effStardustN64Entry(*(f32*)(work+8), *(f32*)(work+0xC) +
                                                   (float_5_804270b0 + float_10_804270a4) * *(f32*)(work+0x14),
                                                   *(f32*)(work+0x10), float_20_804270b4 * *(f32*)(work+0x14), 4);
                    color = color_tbl[type];
                    cvt0.words.hi = 0x43300000;
                    cvt1.words.hi = 0x43300000;
                    cvt2.words.hi = 0x43300000;
                    cvt0.words.lo = (u32)color[0];
                    cvt1.words.lo = (u32)color[1];
                    cvt2.words.lo = (u32)color[2];
                    conversionMask = *(volatile const f64*)&double_to_int_mask_802fe6d8;
                    effStardustN64SetColor(
                        (f32)(cvt0.value - conversionMask) / float_255_804270b8,
                        (f32)(cvt1.value - conversionMask) / float_255_804270b8,
                        (f32)(cvt2.value - conversionMask) / float_255_804270b8,
                        spawned);
                }
            }
        }
        if (!bounced) {
            *(f32*)(work + 0xC) += *(f32*)(work + 0x44);
            *(f32*)(work + 0x44) -= *(f32*)(work + 0x40);
        }
        if (*(s32*)(work + 0x3C) > 2) {
            camera = camGetPtr(cameraId);
            angle = float_6p2832_804270bc * (float_90_804270c0 + *(f32*)((u8*)camera + 0x114)) / float_360_804270c4;
            *(f32*)(work + 0x48) -= (f32)sin(angle);
            camera = camGetPtr(cameraId); angle = float_6p2832_804270bc * (float_90_804270c0 + *(f32*)((u8*)camera + 0x114)) / float_360_804270c4;
            *(f32*)(work + 0x4C) -= (f32)cos(angle);
            camera = camGetPtr(cameraId); angle = float_6p2832_804270bc * (float_90_804270c0 + *(f32*)((u8*)camera + 0x114)) / float_360_804270c4;
            *(f32*)(work + 0x50) += (f32)sin(angle);
            camera = camGetPtr(cameraId); angle = float_6p2832_804270bc * (float_90_804270c0 + *(f32*)((u8*)camera + 0x114)) / float_360_804270c4;
            *(f32*)(work + 0x54) += (f32)cos(angle);
            *(f32*)(work + 0x20) += (float_90_804270c0 - *(f32*)(work + 0x20)) / float_10_804270a4;
        }
        if (*(s32*)(work + 0x3C) > 3) *(f32*)(work + 0x18) += (float_neg90_804270c8 - *(f32*)(work + 0x18)) / float_10_804270a4;
    }
    timer = *(s32*)(work + 0x2C) + 1;
    *(s32*)(work + 0x2C) = timer % 60;
    pos.x = *(f32*)(work + 8); pos.y = *(f32*)(work + 0xC); pos.z = *(f32*)(work + 0x10);
    if ((*(u16*)(work + 4) & 2) == 0) {
        if ((*(s32*)(work + 0x2C) % 10 == 0) && (*(s32*)(work + 0x3C) == 0)) {
            camera = camGetPtr(4);
            angle = float_6p2832_804270bc * *(f32*)((u8*)camera + 0x114) / float_360_804270c4;
            sinAngle = (f32)sin(angle);
            camera = camGetPtr(4);
            angle = float_6p2832_804270bc * *(f32*)((u8*)camera + 0x114) / float_360_804270c4;
            cosAngle = (f32)cos(angle);
            spawned = effStardustN64Entry(
                pos.x + float_10_804270a4 * sinAngle,
                pos.y - float_10_804270a4 * *(f32*)(work + 0x14),
                pos.z - float_10_804270a4 * cosAngle,
                3);
            color = color_tbl[type];
            cvt0.words.hi = 0x43300000;
            cvt1.words.hi = 0x43300000;
            cvt2.words.hi = 0x43300000;
            cvt0.words.lo = (u32)color[0];
            cvt1.words.lo = (u32)color[1];
            cvt2.words.lo = (u32)color[2];
            conversionMask = *(volatile const f64*)&double_to_int_mask_802fe6d8;
            effStardustN64SetColor(
                (f32)(cvt0.value - conversionMask) / float_255_804270b8,
                (f32)(cvt1.value - conversionMask) / float_255_804270b8,
                (f32)(cvt2.value - conversionMask) / float_255_804270b8,
                spawned);
            effStardustN64SetDrawCam(spawned,cameraId);
        }
        {
            u8* dust = *(u8**)(*(u8**)(work + 0x34) + 0xC);
            if (*(s32*)(work + 0x3C) == 0) {
                *(s32*)(dust + 0x68) = cameraId; *(f32*)(dust+4)=pos.x;
                *(f32*)(dust+8)=pos.y-float_10_804270a4* *(f32*)(work+0x14); *(f32*)(dust+0xC)=pos.z;
            } else { *(f32*)(dust+4)=float_0_80427094; *(f32*)(dust+8)=float_neg1000_804270cc; *(f32*)(dust+0xC)=float_0_80427094; }
        }
        if (*(s32*)(work + 0x3C) < 3 && cameraId == 4) {
            shadow = shadowEntry(pos.x,pos.y,pos.z,float_8_804270d0* *(f32*)(work+0x14));
            U_shadowSetMode(shadow,*(u8*)(work+0x5C));
        }
    }
    if ((*(u16*)(work + 4) & 0x20) == 0) {
        if (type == 0 || type == 6) dispEntry(cameraId,2,effStarStoneDisp_1,effect,dispCalcZ(&pos));
        else dispEntry(cameraId,2,effStarStoneDisp_2,effect,dispCalcZ(&pos));
    }
    return 0;
}

/* stub-fill: effStarStoneDisp_1 | prototype_only | source_prototype */
void effStarStoneDisp_1(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx,f32,f32,f32);
    extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXRotRad(Mtx,f32,char);
    extern void GXLoadPosMtxImm(Mtx,s32);
    extern void* diamond_1_dl_0_tbl[];
    extern void* diamond_1_dl_1_tbl[];
    extern u8 diamond_1_dl_0_size_tbl[];
    extern u8 diamond_1_dl_1_size_tbl[];
    extern void* diamond_7_dl_0_tbl[];
    extern void* diamond_7_dl_1_tbl[];
    extern u8 diamond_7_dl_0_size_tbl[];
    extern u8 diamond_7_dl_1_size_tbl[];
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    u8* work = *(u8**)(effectAddress + 0xC);
    u8* camera = camGetPtr(cameraId);
    u8 texObj[0x20];
    Mtx trans, scale, rot;
    s32 type = *(s32*)work;
    s32 color = *(s32*)(work + 0x5C);
    s32 i;
    f32 size;

    mapSetMaterialFog();
    GXSetArray(11, (void*)(0x803AF400 + type * 4), 4);
    GXSetNumChans(1);
    GXSetChanCtrl(4,0,0,0,0,0,2);
    GXSetChanMatColor(4, work + 0x30);
    GXSetNumTexGens(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(0,0xFF,0xFF,4);
    GXSetTevOp(0,4);
    GXSetTevOrder(1,0xFF,0xFF,0xFF);
    GXSetTevColorOp(1,0,0,0,1,0);
    GXSetTevAlphaOp(1,0,0,0,1,0);
    GXSetTevColorIn(1,0,2,0,0);
    GXSetTevAlphaIn(1,0,5,0,7);
    GXSetTevColor(1,&color);

    PSMTXTrans(trans, *(f32*)(work+8), *(f32*)(work+0xC), *(f32*)(work+0x10));
    size = 1.1f * *(f32*)(work+0x14);
    PSMTXScale(scale,size,size,size);
    PSMTXConcat(trans,scale,trans);
    PSMTXRotRad(rot,0.017453292f * *(f32*)(work+0x1C),'y');
    PSMTXConcat(trans,rot,trans);
    PSMTXConcat(camera+0x11C,trans,trans);
    GXLoadPosMtxImm(trans,0);
    GXSetCurrentMtx(0);
    GXSetBlendMode(1,4,5,0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7,0,0,7,0);
    GXSetZMode(1,3,0);
    GXSetCullMode(2);

    if (type == 0) {
        GXClearVtxDesc();
        GXSetVtxDesc(9,2);
        GXSetVtxAttrFmt(0,9,1,3,11);
        GXSetArray(9,(void*)0x803AF180,6);
        GXSetVtxDesc(10,2);
        GXSetVtxAttrFmt(0,10,0,1,6);
        GXSetArray(10,(void*)0x803AF280,3);
        GXSetVtxDesc(11,2);
        GXSetVtxAttrFmt(0,11,1,5,0);
        GXSetArray(11,(void*)0x803AF3A0,4);
        GXSetVtxDesc(13,2);
        GXSetVtxAttrFmt(0,13,1,3,13);
        GXSetArray(13,(void*)0x803AF300,4);
        for (i=0;i<34;i++) GXCallDisplayList(diamond_1_dl_0_tbl[i], diamond_1_dl_0_size_tbl[i] << 5);

        GXClearVtxDesc();
        GXSetVtxDesc(9,2);
        GXSetVtxAttrFmt(0,9,1,3,11);
        GXSetArray(9,(void*)0x803AF180,6);
        GXSetVtxDesc(10,2);
        GXSetVtxAttrFmt(0,10,0,1,6);
        GXSetArray(10,(void*)0x803AF280,3);
        GXSetVtxDesc(11,2);
        GXSetVtxAttrFmt(0,11,1,5,0);
        GXSetVtxDesc(13,2);
        GXSetVtxAttrFmt(0,13,1,3,13);
        GXSetArray(13,(void*)0x803AF300,4);
        for (i=0;i<34;i++) GXCallDisplayList(diamond_1_dl_1_tbl[i], diamond_1_dl_1_size_tbl[i] << 5);
    } else {
        GXClearVtxDesc();
        GXSetVtxDesc(9,2);
        GXSetVtxAttrFmt(0,9,1,3,11);
        GXSetArray(9,(void*)0x803AF780,6);
        GXSetVtxDesc(10,2);
        GXSetVtxAttrFmt(0,10,0,1,6);
        GXSetArray(10,(void*)0x803AF880,3);
        GXSetVtxDesc(11,2);
        GXSetVtxAttrFmt(0,11,1,5,0);
        GXSetArray(11,(void*)0x803AFA20,4);
        GXSetVtxDesc(13,2);
        GXSetVtxAttrFmt(0,13,1,3,13);
        GXSetArray(13,(void*)0x803AF900,4);
        for (i=0;i<34;i++) GXCallDisplayList(diamond_7_dl_0_tbl[i], diamond_7_dl_0_size_tbl[i] << 5);

        GXClearVtxDesc();
        GXSetVtxDesc(9,2);
        GXSetVtxAttrFmt(0,9,1,3,11);
        GXSetArray(9,(void*)0x803AF780,6);
        GXSetVtxDesc(10,2);
        GXSetVtxAttrFmt(0,10,0,1,6);
        GXSetArray(10,(void*)0x803AF880,3);
        GXSetVtxDesc(11,2);
        GXSetVtxAttrFmt(0,11,1,5,0);
        GXSetVtxDesc(13,2);
        GXSetVtxAttrFmt(0,13,1,3,13);
        GXSetArray(13,(void*)0x803AF900,4);
        for (i=0;i<33;i++) GXCallDisplayList(diamond_7_dl_1_tbl[i], diamond_7_dl_1_size_tbl[i] << 5);
    }

    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXTrans(trans, *(f32*)(work + 0x24), *(f32*)(work + 0x28), 0.0f);
    GXLoadTexMtxImm(trans, 0x1E, 1);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 0);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0, 2, 0, 0);
    GXSetTevAlphaIn(1, 0, 5, 0, 7);
    color = *(s32*)(work + 0x5C);
    GXSetTevColor(1, &color);

    PSMTXTrans(trans, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    size = *(f32*)(work + 0x14);
    PSMTXScale(scale, size, size, size);
    PSMTXConcat(trans, scale, trans);
    PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x1C), 'y');
    PSMTXConcat(trans, rot, trans);
    PSMTXConcat(camera + 0x11C, trans, trans);
    GXLoadPosMtxImm(trans, 0);
    GXSetCurrentMtx(0);
    if (type == 0) effGetTexObj(0x2E, texObj);
    else effGetTexObj(0x2F, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);

    if (type == 0) {
        GXClearVtxDesc(); GXSetVtxDesc(9,2); GXSetVtxAttrFmt(0,9,1,3,11);
        GXSetArray(9,(void*)0x803AF180,6);
        GXSetVtxDesc(10,2); GXSetVtxAttrFmt(0,10,0,1,6);
        GXSetArray(10,(void*)0x803AF280,3);
        GXSetVtxDesc(11,2); GXSetVtxAttrFmt(0,11,1,5,0);
        GXSetArray(11,(void*)0x803AF3A0,4);
        GXSetVtxDesc(13,2); GXSetVtxAttrFmt(0,13,1,3,13);
        GXSetArray(13,(void*)0x803AF300,4);
        for (i = 0; i < 34; i++) GXCallDisplayList(diamond_1_dl_0_tbl[i], diamond_1_dl_0_size_tbl[i] << 5);
    } else {
        GXClearVtxDesc(); GXSetVtxDesc(9,2); GXSetVtxAttrFmt(0,9,1,3,11);
        GXSetArray(9,(void*)0x803AF780,6);
        GXSetVtxDesc(10,2); GXSetVtxAttrFmt(0,10,0,1,6);
        GXSetArray(10,(void*)0x803AF880,3);
        GXSetVtxDesc(11,2); GXSetVtxAttrFmt(0,11,1,5,0);
        GXSetArray(11,(void*)0x803AFA20,4);
        GXSetVtxDesc(13,2); GXSetVtxAttrFmt(0,13,1,3,13);
        GXSetArray(13,(void*)0x803AF900,4);
        for (i = 0; i < 34; i++) GXCallDisplayList(diamond_7_dl_0_tbl[i], diamond_7_dl_0_size_tbl[i] << 5);
    }

    effGetTexObj(0x2D, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
    if (type == 0) {
        GXClearVtxDesc(); GXSetVtxDesc(9,2); GXSetVtxAttrFmt(0,9,1,3,11);
        GXSetArray(9,(void*)0x803AF180,6);
        GXSetVtxDesc(10,2); GXSetVtxAttrFmt(0,10,0,1,6);
        GXSetArray(10,(void*)0x803AF280,3);
        GXSetVtxDesc(11,2); GXSetVtxAttrFmt(0,11,1,5,0);
        GXSetVtxDesc(13,2); GXSetVtxAttrFmt(0,13,1,3,13);
        GXSetArray(13,(void*)0x803AF300,4);
        for (i = 0; i < 34; i++) GXCallDisplayList(diamond_1_dl_1_tbl[i], diamond_1_dl_1_size_tbl[i] << 5);
    } else {
        GXClearVtxDesc(); GXSetVtxDesc(9,2); GXSetVtxAttrFmt(0,9,1,3,11);
        GXSetArray(9,(void*)0x803AF780,6);
        GXSetVtxDesc(10,2); GXSetVtxAttrFmt(0,10,0,1,6);
        GXSetArray(10,(void*)0x803AF880,3);
        GXSetVtxDesc(11,2); GXSetVtxAttrFmt(0,11,1,5,0);
        GXSetArray(11,(void*)0x803AFA20,4);
        GXSetVtxDesc(13,2); GXSetVtxAttrFmt(0,13,1,3,13);
        GXSetArray(13,(void*)0x803AF900,4);
        for (i = 0; i < 33; i++) GXCallDisplayList(diamond_7_dl_1_tbl[i], diamond_7_dl_1_size_tbl[i] << 5);
    }
}

/* CHATGPT STUB FILL: main/effect/eff_starstone 20260624_184128 */

/* stub-fill: effStarStoneDisp_2 | prototype_only | source_prototype */
void effStarStoneDisp_2(s32 cameraId, s32 effectAddress) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(Mtx, s32, s32);
    extern void* diamond_2_2_dl_0_tbl[];
    extern void* diamond_2_2_dl_1_tbl[];
    extern u8 diamond_2_2_dl_0_size_tbl[];
    extern u8 diamond_2_2_dl_1_size_tbl[];
    u8* work = *(u8**)(effectAddress + 0xC);
    u8* camera = camGetPtr(cameraId);
    u8 texObj[0x20];
    Mtx trans, scale, rot;
    s32 color = *(s32*)(work + 0x5C);
    s32 i;

    mapSetMaterialFog();
    GXSetArray(11, (void*)(0x803AF400 + *(s32*)work * 4), 4);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetChanMatColor(4, work + 0x30);
    GXSetNumTexGens(0);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0xFF, 0xFF, 4);
    GXSetTevOp(0, 4);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 2, 0, 15);
    GXSetTevAlphaIn(1, 7, 1, 0, 7);
    GXSetTevColor(1, &color);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);
    GXSetCullMode(2);

    {
        f32 size;
        PSMTXTrans(trans, *(f32*)(work + 8) + *(f32*)(work + 0x48),
                   *(f32*)(work + 0xC),
                   *(f32*)(work + 0x10) + *(f32*)(work + 0x4C));
        size = 1.1f * *(f32*)(work + 0x14);
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(trans, scale, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x1C), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x20), 'z');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x18), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(camera + 0x118, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxAttrFmt(0, 9, 1, 3, 11);
        GXSetArray(9, (void*)0x803AF500, 6);
        GXSetVtxDesc(10, 2);
        GXSetVtxAttrFmt(0, 10, 0, 1, 6);
        GXSetArray(10, (void*)0x803AF5C0, 3);
        GXSetVtxDesc(11, 2);
        GXSetVtxAttrFmt(0, 11, 1, 5, 0);
        GXSetVtxDesc(13, 2);
        GXSetVtxAttrFmt(0, 13, 1, 3, 14);
        GXSetArray(13, (void*)0x803AF660, 4);
        for (i = 0; i < 19; i++) {
            if ((*(u16*)(work + 4) & 1) || i != 0) {
                GXCallDisplayList(diamond_2_2_dl_0_tbl[i],
                                  diamond_2_2_dl_0_size_tbl[i] << 5);
            }
        }
    }

    {
        f32 size;
        PSMTXTrans(trans, *(f32*)(work + 8) + *(f32*)(work + 0x50),
                   *(f32*)(work + 0xC),
                   *(f32*)(work + 0x10) + *(f32*)(work + 0x54));
        size = 1.1f * *(f32*)(work + 0x14);
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(trans, scale, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x1C), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * -*(f32*)(work + 0x20), 'z');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * -*(f32*)(work + 0x18), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(camera + 0x118, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxAttrFmt(0, 9, 1, 3, 11);
        GXSetArray(9, (void*)0x803AF500, 6);
        GXSetVtxDesc(10, 2);
        GXSetVtxAttrFmt(0, 10, 0, 1, 6);
        GXSetArray(10, (void*)0x803AF5C0, 3);
        GXSetVtxDesc(11, 2);
        GXSetVtxAttrFmt(0, 11, 1, 5, 0);
        GXSetVtxDesc(13, 2);
        GXSetVtxAttrFmt(0, 13, 1, 3, 14);
        GXSetArray(13, (void*)0x803AF660, 4);
        for (i = 0; i < 19; i++) {
            if ((*(u16*)(work + 4) & 1) || i != 0) {
                GXCallDisplayList(diamond_2_2_dl_1_tbl[i],
                                  diamond_2_2_dl_1_size_tbl[i] << 5);
            }
        }
    }
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXTrans(trans, *(f32*)(work + 0x24), *(f32*)(work + 0x28), 0.0f);
    GXLoadTexMtxImm(trans, 0x1E, 1);
    GXSetNumTevStages(2);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOp(0, 0);
    GXSetTevOrder(1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 15, 2, 0, 15);
    GXSetTevAlphaIn(1, 7, 1, 0, 7);
    GXSetTevColor(1, &color);
    if (*(s32*)work == 2) {
        effGetTexObj(0x2E, texObj);
        GXLoadTexObj(texObj, 0);
    } else {
        effGetTexObj(0x29, texObj);
        GXLoadTexObj(texObj, 0);
    }
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(1, 3, 0);

    {
        f32 size;
        PSMTXTrans(trans, *(f32*)(work + 8) + *(f32*)(work + 0x48),
                   *(f32*)(work + 0xC),
                   *(f32*)(work + 0x10) + *(f32*)(work + 0x4C));
        size = *(f32*)(work + 0x14);
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(trans, scale, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x1C), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x20), 'z');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x18), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(camera + 0x118, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxAttrFmt(0, 9, 1, 3, 11);
        GXSetArray(9, (void*)0x803AF500, 6);
        GXSetVtxDesc(10, 2);
        GXSetVtxAttrFmt(0, 10, 0, 1, 6);
        GXSetArray(10, (void*)0x803AF5C0, 3);
        GXSetVtxDesc(11, 2);
        GXSetVtxAttrFmt(0, 11, 1, 5, 0);
        GXSetVtxDesc(13, 2);
        GXSetVtxAttrFmt(0, 13, 1, 3, 14);
        GXSetArray(13, (void*)0x803AF660, 4);
        for (i = 0; i < 19; i++) {
            if ((*(u16*)(work + 4) & 1) || i != 0) {
                GXCallDisplayList(diamond_2_2_dl_0_tbl[i],
                                  diamond_2_2_dl_0_size_tbl[i] << 5);
            }
        }
    }

    {
        f32 size;
        PSMTXTrans(trans, *(f32*)(work + 8) + *(f32*)(work + 0x50),
                   *(f32*)(work + 0xC),
                   *(f32*)(work + 0x10) + *(f32*)(work + 0x54));
        size = *(f32*)(work + 0x14);
        PSMTXScale(scale, size, size, size);
        PSMTXConcat(trans, scale, trans);
        PSMTXRotRad(rot, 0.017453292f * *(f32*)(work + 0x1C), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * -*(f32*)(work + 0x20), 'z');
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0.017453292f * -*(f32*)(work + 0x18), 'y');
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(camera + 0x118, trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(9, 2);
        GXSetVtxAttrFmt(0, 9, 1, 3, 11);
        GXSetArray(9, (void*)0x803AF500, 6);
        GXSetVtxDesc(10, 2);
        GXSetVtxAttrFmt(0, 10, 0, 1, 6);
        GXSetArray(10, (void*)0x803AF5C0, 3);
        GXSetVtxDesc(11, 2);
        GXSetVtxAttrFmt(0, 11, 1, 5, 0);
        GXSetVtxDesc(13, 2);
        GXSetVtxAttrFmt(0, 13, 1, 3, 14);
        GXSetArray(13, (void*)0x803AF660, 4);
        for (i = 0; i < 19; i++) {
            if ((*(u16*)(work + 4) & 1) || i != 0) {
                GXCallDisplayList(diamond_2_2_dl_1_tbl[i],
                                  diamond_2_2_dl_1_size_tbl[i] << 5);
            }
        }
    }
}

void effStarStoneDraw(void* camera, void* effect) {
    extern void effStarStoneDisp_1(void* camera, void* effect);
    extern void effStarStoneDisp_2(void* camera, void* effect);

    void* work = *(void**)((s32)effect + 0xC);

    switch (*(s32*)work) {
        case 0:
        case 6:
            effStarStoneDisp_1(camera, effect);
            break;
        default:
            effStarStoneDisp_2(camera, effect);
            break;
    }
}

const f32 float_0p01_80427098 = 0.01f;
const f32 float_0p4_804270a0 = 0.4f;
const f32 float_neg1_804270a8 = -1.0f;
const f32 float_neg0p5_804270ac = -0.5f;
const f32 float_5_804270b0 = 5.0f;
const f32 float_6p2832_804270bc = 6.2831855f;
const f32 float_360_804270c4 = 360.0f;
const f32 float_neg90_804270c8 = -90.0f;
const f32 float_neg1000_804270cc = -1000.0f;
const f32 float_8_804270d0 = 8.0f;
