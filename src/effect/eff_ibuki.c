#include "effect/eff_ibuki.h"

/* stub-fill: effIbukiEntry | missing_definition | ghidra_signature */
void* effIbukiEntry(s32 type, s32 duration, f32 x, f32 y, f32 z) {
    extern void* effEntry(s32 type);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effIbukiMain(void);
    extern s32 irand(s32 max);
    extern f64 cos(f64 x);
    extern f64 sin(f64 x);
    extern char str_Ibuki_804284e0[];
    extern f32 float_1_804284c8;
    extern f32 float_0_804284a0;
    extern f32 float_6p2832_804284d8;
    extern f32 float_360_804284dc;

    void* effect;
    void* work;
    void* part;
    s32 i;
    s32 angle;
    f32 radians;

    effect = effEntry(type);
    *(char**)((s32)effect + 0x14) = str_Ibuki_804284e0;
    *(s32*)((s32)effect + 8) = 0x21;
    work = __memAlloc(3, 0xAD4);
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effIbukiMain;
    *(u32*)effect |= 2;
    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(s32*)((s32)work + 0x30) = (duration > 0) ? duration : 1000;
    *(s32*)((s32)work + 0x34) = 0;
    *(f32*)((s32)work + 0x2C) = float_1_804284c8;
    *(s32*)((s32)work + 0x38) = 0;
    *(s32*)((s32)work + 0x3C) = 0;
    *(f32*)((s32)work + 0x40) = float_0_804284a0;
    *(f32*)((s32)work + 0x44) = float_0_804284a0;
    *(f32*)((s32)work + 0x48) = float_0_804284a0;
    *(f32*)((s32)work + 0x4C) = float_0_804284a0;

    part = (void*)((s32)work + 0x54);
    for (i = 1, angle = 10; i < *(s32*)((s32)effect + 8); i++, angle += 10, part = (void*)((s32)part + 0x54)) {
        angle = 0x1E - irand(0x28);
        *(f32*)((s32)part + 4) = float_0_804284a0;
        *(f32*)((s32)part + 8) = (f32)irand(0x1E);
        *(f32*)((s32)part + 0xC) = float_0_804284a0;
        radians = (float_6p2832_804284d8 * (f32)angle) / float_360_804284dc;
        *(f32*)((s32)part + 0x10) = (f32)cos(radians);
        *(f32*)((s32)part + 0x14) = (f32)sin(radians);
        *(f32*)((s32)part + 0x18) = float_0_804284a0;
        *(f32*)((s32)part + 0x2C) = float_1_804284c8;
        *(s32*)((s32)part + 0x38) = 0;
        *(s32*)((s32)part + 0x50) = i * 10;
        *(f32*)((s32)part + 0x1C) = float_0_804284a0;
        *(f32*)((s32)part + 0x20) = float_0_804284a0;
        if (irand(1000) < 500) {
            *(f32*)((s32)part + 0x24) = (f32)(irand(10) + 10);
            *(f32*)((s32)part + 0x28) = float_0_804284a0;
        } else {
            *(f32*)((s32)part + 0x24) = float_0_804284a0;
            *(f32*)((s32)part + 0x28) = (f32)(irand(10) + 10);
        }
    }

    return effect;
}

/* stub-fill: effIbukiMain | missing_definition | ghidra_signature */
void effIbukiMain(void* effect) {
    extern void effDelete(void* effect);
    extern s32 irand(s32 max);
    extern void* camGetPtr(s32 cameraId);
    extern void PSMTXRotRad(f32 matrix[3][4], s32 axis, f32 radians);
    extern void PSMTXMultVec(f32 matrix[3][4], const f32* src, f32* dst);
    extern void effVaporN64Entry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 scale);
    extern f64 cos(f64 angle);
    extern f64 sin(f64 angle);
    extern f64 dispCalcZ(const f32* position);
    extern void dispEntry(s32 camera, s32 order, void* callback, void* param, f32 z);
    extern void effIbukiDisp(s32 camera, void* effect);

    u8* work = *(u8**)((s32)effect + 0xC);
    f32 displayPosition[3];
    s32 i;
    s32 delay;

    displayPosition[0] = *(f32*)(work + 4);
    displayPosition[1] = *(f32*)(work + 8);
    displayPosition[2] = *(f32*)(work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4;
        *(s32*)(work + 0x30) = 0x10;
    }
    if (*(s32*)(work + 0x30) < 1000) {
        *(s32*)(work + 0x30) -= 1;
    }
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effect);
        return;
    }

    if (*(s32*)(work + 0x34) < 0x10) {
        *(s32*)(work + 0x38) = *(s32*)(work + 0x34) * 0x10 + 0xF;
    }
    if (*(s32*)(work + 0x30) < 0x10) {
        *(s32*)(work + 0x38) = *(s32*)(work + 0x30) << 4;
    }
    if (*(s32*)(work + 0x34) > 0x3B && *(s32*)(work + 0x34) < 0x5C) {
        *(s32*)(work + 0x3C) = (*(s32*)(work + 0x34) - 0x3C) * 8 + 7;
    }

    *(s32*)(work + 0x34) += 1;
    *(f32*)(work + 0x40) -= 0.0125f;
    *(f32*)(work + 0x44) += 0.0125f;
    *(f32*)(work + 0x48) -= 0.0125f;
    *(f32*)(work + 0x4C) = 0.0f;

    if (*(s32*)(work + 0x34) > 0x78 && (*(s32*)(work + 0x34) % 0x10) == 0) {
        f32 position[3];
        f32 rotation[3][4];
        void* camera;

        position[0] = *(f32*)(work + 4) + (f32)irand(300);
        position[1] = 30.0f + *(f32*)(work + 8) + (f32)irand(100);
        position[2] = *(f32*)(work + 0xC);
        camera = camGetPtr(4);
        PSMTXRotRad(rotation, 'y', 0.017453292f * -*(f32*)((s32)camera + 0x114));
        PSMTXMultVec(rotation, position, position);
        effVaporN64Entry(2, 0x1E, position[0], position[1], position[2],
                         1.0f + (f32)irand(10) / 10.0f);
    }

    delay = 10;
    for (i = 1; i < *(s32*)((s32)effect + 8); i++) {
        u8* particle = work + (i - 1) * 0x54;

        if (*(s32*)(particle + 0xA4) == 0) {
            *(f32*)(particle + 0x58) += *(f32*)(particle + 0x64);
            *(f32*)(particle + 0x5C) += *(f32*)(particle + 0x68);
            *(f32*)(particle + 0x60) += *(f32*)(particle + 0x6C);
            *(f32*)(particle + 0x70) += *(f32*)(particle + 0x78);
            *(f32*)(particle + 0x74) += *(f32*)(particle + 0x7C);
            *(f32*)(particle + 0x64) *= 1.01f;
            *(f32*)(particle + 0x68) *= 1.01f;
            *(f32*)(particle + 0x6C) *= 1.01f;
            *(f32*)(particle + 0x78) *= 1.01f;
            *(f32*)(particle + 0x7C) *= 1.01f;

            *(s32*)(particle + 0x8C) += 5;
            if (*(s32*)(particle + 0x8C) > 0xFF) {
                *(s32*)(particle + 0x8C) = 0xFF;
            }

            if (*(f32*)(particle + 0x58) > 300.0f) {
                s32 angle = 0x1E - irand(0x28);
                f32 radians = (6.2832f * (f32)angle) / 360.0f;

                *(f32*)(particle + 0x58) = 0.0f;
                *(f32*)(particle + 0x5C) = (f32)irand(0x1E);
                *(f32*)(particle + 0x60) = 0.0f;
                *(f32*)(particle + 0x64) = (f32)cos((f64)radians);
                *(f32*)(particle + 0x68) = (f32)sin((f64)radians);
                *(f32*)(particle + 0x6C) = 0.0f;
                *(f32*)(particle + 0x80) = 1.0f;
                *(s32*)(particle + 0x8C) = 0;
                *(s32*)(particle + 0xA4) = delay;
                *(f32*)(particle + 0x70) = 0.0f;
                *(f32*)(particle + 0x74) = 0.0f;

                if (irand(1000) < 500) {
                    *(f32*)(particle + 0x78) = (f32)(irand(10) + 10);
                    *(f32*)(particle + 0x7C) = 0.0f;
                } else {
                    *(f32*)(particle + 0x78) = 0.0f;
                    *(f32*)(particle + 0x7C) = (f32)(irand(10) + 10);
                }
            }
        } else {
            *(s32*)(particle + 0xA4) -= 1;
        }
        delay += 10;
    }

    dispEntry(4, 2, effIbukiDisp, effect, (f32)dispCalcZ(displayPosition));
}


/* CHATGPT STUB FILL: main/effect/eff_ibuki 20260624_184929 */

/* stub-fill: effIbukiDisp | missing_definition | ghidra_signature */
void effIbukiDisp(s32 cameraId, void* effect) {
    typedef f32 Mtx[3][4];
    typedef u32 GXTexObj[8];
    extern u8* camGetPtr(s32 id);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32,u8,s32,s32,u32,s32,s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32,s32,s32,s32,u8,s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32,s32,s32,s32);
    extern void GXSetTevColorOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaOp(s32,s32,s32,s32,s32,s32);
    extern void GXSetTevColorIn(s32,s32,s32,s32,s32);
    extern void GXSetTevAlphaIn(s32,s32,s32,s32,s32);
    extern void GXSetTevColor(s32,u32*);
    extern void PSMTXTrans(Mtx,f32,f32,f32);
    extern void PSMTXRotRad(Mtx,s32,f32);
    extern void PSMTXScale(Mtx,f32,f32,f32);
    extern void PSMTXConcat(Mtx,Mtx,Mtx);
    extern void GXLoadTexMtxImm(Mtx,s32,s32);
    extern void GXSetCullMode(s32);
    extern void GXLoadPosMtxImm(Mtx,s32);
    extern void GXSetCurrentMtx(s32);
    extern void effGetTexObj(s32,GXTexObj);
    extern void GXLoadTexObj(GXTexObj,s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32,s32);
    extern void GXSetVtxAttrFmt(s32,s32,s32,s32,s32);
    extern void GXSetArray(s32,void*,s32);
    extern void GXCallDisplayList(void*,u32);
    extern u8 ibuki_vertex_tbl[];
    extern u8 ibuki_texcoord0_tbl[];
    extern u8 ibuki_texcoord1_tbl[];
    extern u8 ibuki_dl_0_tbl[];
    extern u8 ibuki_dl_0_size_tbl[];
    extern u8 dat_8041de80[];
    extern u8 dat_8041dea0[];

    s32* work = *(s32**)((u8*)effect + 0xC);
    u8* camera = camGetPtr(cameraId);
    Mtx trans, rotate, scale, model;
    GXTexObj texture;
    u32 color;
    s32 alpha = work[0xE] * work[0xF];
    s32 i;

    GXSetNumChans(1);
    GXSetChanCtrl(4,0,0,1,0,0,2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0,1,4,0x1E,0,0x7D);
    GXSetTexCoordGen2(1,1,5,0x21,0,0x7D);
    GXSetNumTevStages(4);
    GXSetTevOrder(0,1,1,0xFF);
    GXSetTevColorOp(0,0,0,0,1,0);
    GXSetTevAlphaOp(0,0,0,0,1,0);
    GXSetTevColorIn(0,15,15,15,6);
    GXSetTevAlphaIn(0,7,7,7,4);
    GXSetTevOrder(1,0,0,0xFF);
    GXSetTevColorOp(1,0,0,0,1,0);
    GXSetTevAlphaOp(1,0,0,0,1,0);
    GXSetTevColorIn(1,15,0,8,15);
    GXSetTevAlphaIn(1,7,0,4,7);
    GXSetTevOrder(2,0xFF,0xFF,4);
    GXSetTevColorOp(2,0,0,0,1,0);
    GXSetTevAlphaOp(2,0,0,0,1,0);
    GXSetTevColorIn(2,15,0,10,15);
    GXSetTevAlphaIn(2,7,0,5,7);
    GXSetTevOrder(3,0xFF,0xFF,0xFF);
    GXSetTevColorOp(3,0,0,0,1,0);
    GXSetTevAlphaOp(3,0,0,0,1,0);
    GXSetTevColorIn(3,15,0,2,15);
    GXSetTevAlphaIn(3,7,0,1,7);
    alpha = alpha / 255 + (alpha >> 31);
    color = 0xFFFFFF00 | (u8)(alpha - (alpha >> 31));
    GXSetTevColor(1,&color);
    PSMTXTrans(trans,(f32)work[0x10],(f32)work[0x11],0.0f);
    GXLoadTexMtxImm(trans,0x1E,1);
    PSMTXTrans(trans,(f32)work[0x12],0.02f,0.0f);
    GXLoadTexMtxImm(trans,0x21,1);
    GXSetCullMode(0);
    PSMTXTrans(trans,(f32)work[1],(f32)work[2],(f32)work[3]);
    PSMTXRotRad(rotate,'y',-0.017453292f * *(f32*)(camera + 0x114));
    if (work[0] == 0) {
        PSMTXScale(scale,(f32)work[0xB],(f32)work[0xB],(f32)work[0xB]);
    } else {
        PSMTXScale(scale,-(f32)work[0xB],(f32)work[0xB],(f32)work[0xB]);
    }
    PSMTXConcat(trans,rotate,model);
    PSMTXConcat(model,scale,model);
    PSMTXConcat((f32(*)[4])(camera+0x11C),model,model);
    GXLoadPosMtxImm(model,0);
    GXSetCurrentMtx(0);
    effGetTexObj(0x3C,texture);
    GXLoadTexObj(texture,0);
    effGetTexObj(0x62,texture);
    GXLoadTexObj(texture,1);
    GXClearVtxDesc();
    GXSetVtxDesc(9,2);
    GXSetVtxAttrFmt(0,9,1,3,6);
    GXSetArray(9,ibuki_vertex_tbl,6);
    GXSetVtxDesc(10,2);
    GXSetVtxAttrFmt(0,10,0,1,6);
    GXSetArray(10,dat_8041de80,3);
    GXSetVtxDesc(11,2);
    GXSetVtxAttrFmt(0,11,1,5,0);
    GXSetArray(11,dat_8041dea0,4);
    GXSetVtxDesc(13,2);
    GXSetVtxAttrFmt(0,13,1,3,0xE);
    GXSetArray(13,ibuki_texcoord0_tbl,4);
    GXSetVtxDesc(14,2);
    GXSetVtxAttrFmt(0,14,1,3,0xE);
    GXSetArray(14,ibuki_texcoord1_tbl,4);
    for (i=0;i<4;i++) {
        GXCallDisplayList(*(void**)(ibuki_dl_0_tbl+i*4),
                          (u32)ibuki_dl_0_size_tbl[i] << 5);
    }
}
