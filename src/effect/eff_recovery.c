#include "effect/eff_recovery.h"


void effRecoveryMain(void* effect) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void effCalcMayaAnim(void* anim);
    extern void effDeleteMayaAnim(void* anim);
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 camera,s32 layer,void* callback,void* param,f32 z);
    extern void effRecoveryDisp(void);
    extern void effRecoveryDisp2(void);
    extern void effRecoveryDisp3(void);
    extern f32 intplGetValue(f32 start,f32 end,s32 mode,s32 current,s32 total);
    extern f32 distABf(f32,f32,f32,f32);
    extern double sin(double);
    extern double cos(double);
    extern s32 rand(void);
    extern void* gpGlobals;
    char* work;
    char* part;
    Vec pos;
    s32 type;
    s32 delta;
    s32 i;
    s32 finished;
    s32 count;
    f32 angle;
    f32 radius;

    work = *(char**)((char*)effect + 0x0C);
    type = *(s8*)work;
    effCalcMayaAnim(*(void**)(work + 0x38));
    delta = 0;
    if (*(u32*)(work + 0x34) < *(u32*)((char*)gpGlobals + 4)) {
        delta = *(u32*)((char*)gpGlobals + 4) - *(u32*)(work + 0x34);
    }
    if (delta == 0) delta = 1;
    finished = 0;

    while (delta-- > 0) {
        if (++*(s16*)(work + 0x2C) > 10) {
            finished = 1;
        }
        count = (type == 4 || type == 5) ? 10 : *(u8*)(work + 1);
        part = work;
        for (i = 1; i < *(s32*)((char*)effect + 8); i++, part += 0x44) {
            switch (*(s8*)(part + 0x46)) {
                case 0:
                    if (finished) {
                        angle = 0.017453292f * (90.0f + 360.0f * (f32)(i - 1) / (f32)count);
                        *(f32*)(part + 0x58) = 12.0f * (f32)sin(angle);
                        *(f32*)(part + 0x5C) = 0.0f;
                        *(f32*)(part + 0x60) = -12.0f * (f32)cos(angle);
                        *(s16*)(part + 0x70) = 0;
                        *(s8*)(part + 0x46) += 1;
                    }
                    break;
                case 1:
                    if (*(s16*)(part + 0x70) < 10) {
                        *(f32*)(part + 0x58) *= 0.7f;
                        *(f32*)(part + 0x5C) *= 0.7f;
                        *(f32*)(part + 0x60) *= 0.7f;
                        *(f32*)(part + 0x4C) += *(f32*)(part + 0x58);
                        *(f32*)(part + 0x50) += *(f32*)(part + 0x5C);
                        *(f32*)(part + 0x54) += *(f32*)(part + 0x60);
                        *(f32*)(part + 0x64) = 1.0f;
                        *(f32*)(part + 0x68) = 1.0f;
                        *(f32*)(part + 0x6C) = 1.0f;
                        *(s16*)(part + 0x70) += 1;
                    } else {
                        *(s16*)(part + 0x70) = 10;
                        *(s8*)(part + 0x46) += 1;
                    }
                    break;
                case 2:
                    if (--*(s16*)(part + 0x70) < 1) {
                        *(s16*)(part + 0x70) = 0;
                        *(s8*)(part + 0x46) += 1;
                    }
                    break;
                case 3:
                    if (*(s16*)(part + 0x70) < 40) {
                        *(s16*)(part + 0x70) += 1;
                        angle = intplGetValue(0.0f,360.0f,11,*(s16*)(part+0x70),40);
                        radius = distABf(0.0f,0.0f,*(f32*)(part+0x4C),*(f32*)(part+0x54)) * 0.99f;
                        angle = 0.017453292f * (90.0f + angle + 360.0f * (f32)(i-2)/(f32)count);
                        *(f32*)(part+0x4C)=radius*(f32)sin(angle);
                        *(f32*)(part+0x54)=-radius*(f32)cos(angle);
                    } else {
                        *(s16*)(part+0x70)=rand()%20;
                        *(s8*)(part+0x46)+=1;
                    }
                    break;
                case 4:
                    if (--*(s16*)(part+0x70)<1) {
                        *(s16*)(part+0x70)=0;
                        *(s8*)(part+0x46)+=1;
                    }
                    break;
                case 5:
                    if (*(s16*)(part+0x70)<16) {
                        *(s16*)(part+0x70)+=1;
                        *(f32*)(part+0x4C)=intplGetValue(*(f32*)(part+0x4C),0.0f,1,*(s16*)(part+0x70),16);
                        *(f32*)(part+0x50)=intplGetValue(*(f32*)(part+0x50),*(f32*)(work+0x40),1,*(s16*)(part+0x70),16);
                        *(f32*)(part+0x54)=intplGetValue(*(f32*)(part+0x54),0.0f,1,*(s16*)(part+0x70),20);
                        *(f32*)(part+0x64)=intplGetValue(1.0f,0.0f,1,*(s16*)(part+0x70),16);
                        *(f32*)(part+0x68)=*(f32*)(part+0x64);
                        *(f32*)(part+0x6C)=*(f32*)(part+0x64);
                    } else {
                        *(s8*)(part+0x46)+=1;
                    }
                    break;
            }
        }
    }

    *(u32*)(work + 0x34) = *(u32*)((char*)gpGlobals + 4);
    pos.x=*(f32*)(work+8); pos.y=*(f32*)(work+0xC); pos.z=*(f32*)(work+0x10);
    dispEntry(4,1,effRecoveryDisp,effect,dispCalcZ(&pos));
    if (type<2 || type==7 || type==8) {
        dispEntry(4,2,effRecoveryDisp2,effect,dispCalcZ(&pos));
    }
    if (type==4 || type==5) {
        dispEntry(4,2,effRecoveryDisp3,effect,dispCalcZ(&pos));
    }
}

u8 effRecoveryDisp(int cameraId, int effect) {
    typedef f32 Mtx[3][4];
    typedef struct Tex { u32 data[8]; } Tex;
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetNumChans(u32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZCompLoc(s32);
    extern void GXSetAlphaCompare(s32, s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void GXBegin(s32, s32, s16);
    u8* work;
    u8* part;
    void* cam;
    Tex tex;
    Mtx billboard, rot, trans, scale, local;
    volatile f32* fifo;
    s32 type;
    s32 i;

    work = *(u8**)(effect + 0xC);
    type = *work;
    cam = camGetPtr(cameraId);
    PSMTXTrans(billboard, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    PSMTXRotRad(rot, 0.017453292f * -*(f32*)((s32)cam + 0x114), 'y');
    PSMTXConcat(billboard, rot, billboard);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x1E, 0, 0x7D);
    GXSetNumChans(0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    effGetTexObj((type & 1) ? 0x13 : 0x12, &tex);
    GXLoadTexObj(&tex, 0);
    effGetTexObj((type & 1) ? 0x15 : 0x14, &tex);
    GXLoadTexObj(&tex, 1);

    PSMTXTrans(local, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    PSMTXRotRad(rot, 0.017453292f * -*(f32*)((s32)cam + 0x114), 'y');
    PSMTXConcat(local, rot, local);
    PSMTXScale(scale, 10.0f, 10.0f, 10.0f);
    PSMTXConcat(local, scale, local);
    PSMTXTrans(trans, 0.0f, 2.0f, 0.0f);
    PSMTXConcat(local, trans, local);
    if ((type < 2) || (type == 4) || (type == 5) || (type == 7) || (type == 8)) {
        PSMTXConcat(local, (void*)(*(s32*)(work + 0x38) + 0x14), local);
    }
    PSMTXConcat((void*)((s32)cam + 0x11C), local, local);
    PSMTXScale(scale, 0.1f, 0.1f, 0.1f);
    PSMTXConcat(local, scale, local);
    GXLoadPosMtxImm(local, 0);
    GXSetCurrentMtx(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevAlphaIn(0, 7, 7, 7, 4);
    GXBegin(0x80, 0, 4);
    fifo = (volatile f32*)0xCC008000;
    *fifo=-16.0f; *fifo=16.0f; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f;
    *fifo=16.0f; *fifo=16.0f; *fifo=0.0f; *fifo=1.0f; *fifo=0.0f;
    *fifo=16.0f; *fifo=-16.0f; *fifo=0.0f; *fifo=1.0f; *fifo=1.0f;
    *fifo=-16.0f; *fifo=-16.0f; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f;

    if ((type != 2) && (type != 3)) {
        GXSetBlendMode(0, 1, 0, 0);
        GXSetZCompLoc(0);
        GXSetAlphaCompare(6, 0x80, 1, 0, 0);
        GXSetZMode(1, 3, 1);
        GXSetNumTevStages(1);
        GXSetTevOrder(0, 0, 1, 0xFF);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        GXSetTevColorIn(0, 0xF, 2, 8, 0xF);
        GXSetTevAlphaIn(0, 7, 0, 4, 7);
        part = work + 0x44;
        for (i = 1; i < *(s32*)(effect + 8); i++, part += 0x44) {
            PSMTXTrans(trans, *(f32*)(part + 8), *(f32*)(part + 0xC), *(f32*)(part + 0x10));
            PSMTXScale(scale, *(f32*)(part + 0x20), *(f32*)(part + 0x24), *(f32*)(part + 0x28));
            PSMTXConcat(trans, scale, trans);
            PSMTXConcat(billboard, trans, local);
            PSMTXConcat((void*)((s32)cam + 0x11C), local, local);
            GXLoadPosMtxImm(local, 0);
            GXSetCurrentMtx(0);
            GXBegin(0x80, 0, 4);
            *fifo=-6.0f; *fifo=6.0f; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f;
            *fifo=6.0f; *fifo=6.0f; *fifo=0.0f; *fifo=2.0f; *fifo=0.0f;
            *fifo=6.0f; *fifo=-6.0f; *fifo=0.0f; *fifo=2.0f; *fifo=1.0f;
            *fifo=-6.0f; *fifo=-6.0f; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f;
        }
    }
    return 0;
}

u8 effRecoveryDisp3(void) {
    return 0;
}


void effRecoveryDisp4(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef struct Tex { u32 data[8]; } Tex;
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(Mtx, f32, f32, f32);
    extern void PSMTXRotRad(Mtx, f32, char);
    extern void PSMTXScale(Mtx, f32, f32, f32);
    extern void PSMTXConcat(Mtx, Mtx, Mtx);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXLoadPosMtxImm(Mtx, s32);
    extern void GXSetCurrentMtx(u32);
    extern void GXSetNumTexGens(u32);
    extern void GXSetTexCoordGen2(s32, s32, s32, u32, u32, s32);
    extern void GXSetNumChans(u32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(u32);
    extern void GXSetTevOrder(u32, u32, u32, s32);
    extern void GXSetTevOp(s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(u32, s32, u32, u32, u32);
    extern void GXSetChanMatColor(s32, void*);
    extern void GXBegin(s32, s32, s16);
    extern void* gpGlobals;
    extern u32 dat_80422dbc;
    extern u8 color_rotation_data[];
    Mtx trans;
    Mtx rotate;
    Mtx scale;
    Mtx model;
    Tex tex;
    volatile f32* fifo = (volatile f32*)0xCC008000;
    char* work = *(char**)((char*)effect + 0x0C);
    char* part = work;
    char* camera = (char*)camGetPtr(cameraId);
    u32 color;
    s32 alpha;
    s32 i;
    f32 u;
    f32 yoff;
    f32 left;

    PSMTXTrans(trans, *(f32*)(work + 8), *(f32*)(work + 0xC), *(f32*)(work + 0x10));
    PSMTXRotRad(rotate, -0.017453292f * *(f32*)(camera + 0x114), 'y');
    PSMTXConcat(trans, rotate, model);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    left = -32.0f;
    for (i = 1; i < *(s32*)((char*)effect + 8); i++, part += 0x44) {
        if (*(s32*)(part + 0x80) != 0) {
            continue;
        }
        alpha = *(s32*)(work + 4) * *(s32*)(part + 0x48) / 255;
        if (i == 1) {
            effGetTexObj(0x12, &tex);
            color = (dat_80422dbc & 0xFFFFFF00) | (alpha & 0xFF);
        } else {
            effGetTexObj(0x14, &tex);
            color = ((u32)color_rotation_data[((i + *(u32*)((char*)gpGlobals + 4)) % 12) * 3] << 24) |
                    ((u32)color_rotation_data[((i + *(u32*)((char*)gpGlobals + 4)) % 12) * 3 + 1] << 16) |
                    ((u32)color_rotation_data[((i + *(u32*)((char*)gpGlobals + 4)) % 12) * 3 + 2] << 8) |
                    (alpha & 0xFF);
        }
        GXLoadTexObj(&tex, 0);
        GXSetChanMatColor(4, &color);
        PSMTXTrans(trans, *(f32*)(part + 0x4C), *(f32*)(part + 0x50), *(f32*)(part + 0x54));
        PSMTXScale(scale, *(f32*)(part + 0x64), *(f32*)(part + 0x68), *(f32*)(part + 0x6C));
        PSMTXRotRad(rotate, i == 1 ? 0.0f : 0.017453292f * (*(f32*)(part + 0x74) - 90.0f), 'z');
        PSMTXConcat(model, trans, trans);
        PSMTXConcat(trans, scale, trans);
        PSMTXConcat(trans, rotate, trans);
        PSMTXConcat((f32 (*)[4])(camera + 0x48), trans, trans);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 4);
        u = i == 1 ? 1.0f : 2.0f;
        yoff = i == 1 ? 8.0f : 0.0f;
        *fifo=left*0.5f; *fifo=16.0f+yoff; *fifo=0.0f; *fifo=0.0f; *fifo=0.0f;
        *fifo=16.0f; *fifo=16.0f+yoff; *fifo=0.0f; *fifo=u; *fifo=0.0f;
        *fifo=16.0f; *fifo=left*0.5f+yoff; *fifo=0.0f; *fifo=u; *fifo=1.0f;
        *fifo=left*0.5f; *fifo=left*0.5f+yoff; *fifo=0.0f; *fifo=0.0f; *fifo=1.0f;
    }
}

void* effRecoveryEntry(s32 type, s32 amount, f32 x, f32 y, f32 z) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* effMayaAnimAlloc(void* data);
    extern void effPlayMayaAnim(void* anim);
    extern void effDeleteMayaAnim(void* anim);
    extern void psndSFXOn_3D(char* sound, LocalVec3* pos);
    extern s32 irand(s32 max);
    extern double cos(double);
    extern double sin(double);
    extern void* gpGlobals;
    extern u8 effRecoveryMain(void);
    extern u8 effRecoveryMain2(void*);
    extern char str_Recovery_802f3c00[];
    extern char str_SFX_HP_RECOVER_SMALL_802f3c0c[];
    extern char str_SFX_HP_RECOVER_MID1_802f3c24[];
    extern char str_SFX_HP_RECOVER_BIG1_802f3c38[];
    extern char str_SFX_FP_RECOVER_SMALL_802f3c4c[];
    extern char str_SFX_FP_RECOVER_MID1_802f3c64[];
    extern char str_SFX_FP_RECOVER_BIG1_802f3c78[];
    extern void* recovery_effAnimDt;
    extern f32 float_0_80422dcc;
    extern f32 float_6p2832_80422e20;
    extern f32 float_360_80422e24;
    extern f32 float_neg30_80422e38;
    extern f32 float_neg15_80422e3c;
    extern f32 float_1p5_80422e40;
    extern f32 float_0p3_80422e44;
    extern f32 float_1p8_80422e48;
    void* entry;
    void* work;
    void* part;
    LocalVec3 pos;
    s32 count;
    s32 i;
    s32 angle;
    f32 rad;

    entry = effEntry();
    if (type == 6) {
        amount = 0xB;
    } else if (type < 6) {
        if (type < 4) {
            if (type > 1) {
                amount = 0;
            }
        } else {
            amount = 0x14;
        }
    }

    count = amount + 1;
    *(void**)((s32)entry + 0x14) = str_Recovery_802f3c00;
    *(s32*)((s32)entry + 0x8) = count;
    work = __memAlloc(3, count * 0x44);
    *(void**)((s32)entry + 0xC) = work;
    if (type == 6) {
        *(void**)((s32)entry + 0x10) = effRecoveryMain2;
    } else {
        *(void**)((s32)entry + 0x10) = effRecoveryMain;
    }
    *(void**)((s32)work + 0x38) = effMayaAnimAlloc(&recovery_effAnimDt);
    effPlayMayaAnim(*(void**)((s32)work + 0x38));

    *(u8*)((s32)work + 0x0) = type;
    *(u8*)((s32)work + 0x1) = amount;
    *(f32*)((s32)work + 0x8) = x;
    *(f32*)((s32)work + 0xC) = y;
    *(f32*)((s32)work + 0x10) = z;
    *(u16*)((s32)work + 0x2C) = 0;
    *(s32*)((s32)work + 0x34) = *(s32*)((s32)gpGlobals + 0x1C);
    *(s32*)((s32)work + 0x4) = 0xFF;
    *(f32*)((s32)work + 0x40) = float_neg30_80422e38;

    pos.x = x;
    pos.y = y;
    pos.z = z;
    switch (type) {
        case 0:
        case 7:
            if ((s8)amount < 6) {
                psndSFXOn_3D(str_SFX_HP_RECOVER_SMALL_802f3c0c, &pos);
            } else if ((s8)amount < 11) {
                psndSFXOn_3D(str_SFX_HP_RECOVER_MID1_802f3c24, &pos);
            } else {
                psndSFXOn_3D(str_SFX_HP_RECOVER_BIG1_802f3c38, &pos);
            }
            break;
        case 1:
        case 8:
            if ((s8)amount < 6) {
                psndSFXOn_3D(str_SFX_FP_RECOVER_SMALL_802f3c4c, &pos);
            } else if ((s8)amount < 11) {
                psndSFXOn_3D(str_SFX_FP_RECOVER_MID1_802f3c64, &pos);
            } else {
                psndSFXOn_3D(str_SFX_FP_RECOVER_BIG1_802f3c78, &pos);
            }
            break;
        case 4:
            psndSFXOn_3D(str_SFX_HP_RECOVER_BIG1_802f3c38, &pos);
            break;
        case 5:
            psndSFXOn_3D(str_SFX_FP_RECOVER_BIG1_802f3c78, &pos);
            break;
    }

    if (type == 6) {
        effDeleteMayaAnim(*(void**)((s32)work + 0x38));
        *(u16*)((s32)work + 0x2C) = 0xB4;
    }

    part = (void*)((s32)work + 0x44);
    for (i = 1; i < count; i++, part = (void*)((s32)part + 0x44)) {
        *(f32*)((s32)part + 0x8) = float_0_80422dcc;
        if (type < 6 && type > 3) {
            if (i < 10) {
                *(f32*)((s32)part + 0xC) = float_0_80422dcc;
            } else {
                *(f32*)((s32)part + 0xC) = float_neg15_80422e3c;
            }
        } else {
            *(f32*)((s32)part + 0xC) = float_0_80422dcc;
        }
        *(f32*)((s32)part + 0x10) = float_0_80422dcc;
        *(f32*)((s32)part + 0x20) = float_0_80422dcc;
        *(f32*)((s32)part + 0x24) = float_0_80422dcc;
        *(f32*)((s32)part + 0x28) = float_0_80422dcc;
        *(f32*)((s32)part + 0x14) = float_0_80422dcc;
        *(f32*)((s32)part + 0x18) = float_0_80422dcc;
        *(f32*)((s32)part + 0x1C) = float_0_80422dcc;
        *(u16*)((s32)part + 0x2C) = 0;
        *(u8*)((s32)part + 0x2) = 0;
        *(s32*)((s32)part + 0x4) = 0xFF;
        *(f32*)((s32)part + 0x30) = float_0_80422dcc;

        if (type == 6) {
            if (i == 1) {
                *(f32*)((s32)part + 0x20) = float_1p5_80422e40;
                *(f32*)((s32)part + 0x24) = float_1p5_80422e40;
                *(f32*)((s32)part + 0x28) = float_1p5_80422e40;
                *(s32*)((s32)part + 0x4) = 0;
                *(s32*)((s32)part + 0x3C) = 0;
            } else {
                *(f32*)((s32)part + 0x20) = float_0p3_80422e44;
                *(f32*)((s32)part + 0x24) = float_0p3_80422e44;
                *(f32*)((s32)part + 0x28) = float_0p3_80422e44;
                if ((i & 1) == 0) {
                    angle = -irand(0x50);
                } else {
                    angle = irand(0x50);
                }
                angle += 0x5A;
                *(f32*)((s32)part + 0x30) = (f32)angle;
                rad = (float_6p2832_80422e20 * *(f32*)((s32)part + 0x30)) / float_360_80422e24;
                *(f32*)((s32)part + 0x14) = float_1p8_80422e48 * (f32)cos(rad);
                *(f32*)((s32)part + 0x18) = float_1p8_80422e48 * (f32)sin(rad);
                *(s32*)((s32)part + 0x4) = 0xFF;
                *(s32*)((s32)part + 0x3C) = i * 8;
            }
        }
    }

    return entry;
}

void effRecoveryDisp2(s32 cameraId, void* effEntry) {
    typedef f32 LocalMtx[3][4];
    extern void* camGetPtr(s32 cameraId);
    extern s32 sprintf(char* str, char* fmt, ...);
    extern u32 strlen(char* str);
    extern void PSMTXTrans(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(LocalMtx mtx, f32 rad, s32 axis);
    extern void PSMTXConcat(LocalMtx a, LocalMtx b, LocalMtx ab);
    extern void PSMTXScale(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void effNumberGX(LocalMtx mtx, s32 colorType, s32 value, s32 alpha, s32 flag);
    extern char str_PCTd_80422df8[];
    extern f32 float_deg2rad_80422dc0;
    extern f32 float_0_80422dcc;
    extern f32 float_1_80422dd0;
    extern f32 float_2_80422dd8;
    extern f32 float_0p5_80422ddc;
    extern f32 float_10_80422de4;
    extern f32 float_0p035_80422dfc;
    extern f32 float_24_80422e00;
    LocalMtx textMtx;
    LocalMtx rotMtx;
    LocalMtx tmpMtx;
    void* work;
    char buf[32];
    s32 type;
    s32 value;
    u32 len;

    work = *(void**)((s32)effEntry + 0xC);
    camGetPtr(cameraId);
    value = *(s8*)((s32)work + 0x1);
    type = *(s8*)((s32)work + 0x0);

    sprintf(buf, str_PCTd_80422df8, value);
    len = strlen(buf);

    PSMTXTrans(textMtx, *(f32*)((s32)work + 0x8), *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10));
    PSMTXRotRad(rotMtx, float_deg2rad_80422dc0 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114), 'y');
    PSMTXConcat(textMtx, rotMtx, textMtx);
    PSMTXTrans(tmpMtx, float_0_80422dcc, float_0_80422dcc, float_1_80422dd0);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXScale(tmpMtx, float_10_80422de4, float_10_80422de4, float_10_80422de4);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXTrans(tmpMtx, float_0_80422dcc, float_2_80422dd8, float_0_80422dcc);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXConcat(textMtx, (void*)(*(s32*)((s32)work + 0x38) + 0x14), textMtx);
    PSMTXScale(tmpMtx, float_0p035_80422dfc, float_0p035_80422dfc, float_0p035_80422dfc);
    PSMTXConcat(textMtx, tmpMtx, textMtx);
    PSMTXTrans(tmpMtx, float_24_80422e00 * (f32)-(s32)(len - 1) * float_0p5_80422ddc, float_0_80422dcc, float_0_80422dcc);
    PSMTXConcat(textMtx, tmpMtx, textMtx);

    switch (type) {
        case 0:
        case 2:
        case 7:
            effNumberGX(textMtx, 0, value, 0xFF, 1);
            break;
        case 1:
        case 3:
        case 8:
            effNumberGX(textMtx, 1, value, 0xFF, 1);
            break;
    }
}

u8 effRecoveryMain2(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void* effStardustEntry(f64 x, f64 y, f64 z, f64 a, f64 b, s32 c, s32 d, s32 e);
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effRecoveryDisp4(void);
    extern f32 float_0_80422dcc;
    extern f32 float_2_80422dd8;
    extern f32 float_40_80422e14;
    extern f32 float_45_80422e18;
    extern f32 float_0p95_80422e1c;
    extern f32 float_6p2832_80422e20;
    extern f32 float_360_80422e24;
    extern double sin(double x);
    extern double cos(double x);
    LocalVec3 pos;
    void* work;
    void* part;
    s32 i;
    f32 rad;

    work = *(void**)((s32)effEntry + 0xC);
    pos.x = *(f32*)((s32)work + 0x8);
    pos.y = *(f32*)((s32)work + 0xC);
    pos.z = *(f32*)((s32)work + 0x10);

    if (*(s16*)((s32)work + 0x2) < 1000) {
        *(s16*)((s32)work + 0x2) -= 1;
    }
    if (*(s16*)((s32)work + 0x2) < 0x20) {
        *(s32*)((s32)work + 0x4) = *(s16*)((s32)work + 0x2) << 3;
    }
    if (*(s16*)((s32)work + 0x2) < 0) {
        effStardustEntry(*(f32*)((s32)work + 0x8), *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10),
                         float_40_80422e14, float_45_80422e18, 2, 6, 0x1E);
        effDelete(effEntry);
        return 0;
    }

    part = (void*)((s32)work + 0x44);
    for (i = 1; i < *(s32*)((s32)effEntry + 0x8); i++, part = (void*)((s32)part + 0x44)) {
        if (*(s32*)((s32)part + 0x3C) == 0) {
            *(f32*)((s32)part + 0x8) += *(f32*)((s32)part + 0x14);
            *(f32*)((s32)part + 0xC) += *(f32*)((s32)part + 0x18);
            *(f32*)((s32)part + 0x10) += *(f32*)((s32)part + 0x1C);
            *(f32*)((s32)part + 0x14) *= float_0p95_80422e1c;
            *(f32*)((s32)part + 0x18) *= float_0p95_80422e1c;
            *(f32*)((s32)part + 0x1C) *= float_0p95_80422e1c;
            if (i == 1) {
                *(s32*)((s32)part + 0x4) += 5;
                if (*(s32*)((s32)part + 0x4) > 0x80) {
                    *(s32*)((s32)part + 0x4) = 0x80;
                }
            } else {
                *(s32*)((s32)part + 0x4) -= 8;
                if (*(s32*)((s32)part + 0x4) < 0) {
                    *(f32*)((s32)part + 0x8) = float_0_80422dcc;
                    *(f32*)((s32)part + 0xC) = float_0_80422dcc;
                    *(f32*)((s32)part + 0x10) = float_0_80422dcc;
                    rad = (float_6p2832_80422e20 * *(f32*)((s32)part + 0x30)) / float_360_80422e24;
                    *(f32*)((s32)part + 0x14) = float_2_80422dd8 * (f32)cos(rad);
                    *(f32*)((s32)part + 0x18) = float_2_80422dd8 * (f32)sin(rad);
                    *(s32*)((s32)part + 0x4) = 0xFF;
                }
            }
        } else {
            *(s32*)((s32)part + 0x3C) -= 1;
        }
    }

    dispEntry(4, 2, effRecoveryDisp4, effEntry, dispCalcZ(&pos));
    return 0;
}

