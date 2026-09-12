#include "effect/eff_mahorn2.h"

__declspec(section ".rodata") u32 vec3_80302fa8[3] = { 0, 0, 0 };
__declspec(section ".rodata") u32 vec3_80302fb4[3] = { 0, 0, 0 };
__declspec(section ".rodata") u32 vec3_80302fc0[3] = { 0, 0, 0 };
__declspec(section ".rodata") f32 dat_80302fcc[6] = {
    0.0f, 2.0f, 2.0f, 12.0f, 12.0f, 14.0f,
};
__declspec(section ".rodata") f32 dat_80302fe4[6] = {
    0.0f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f,
};
__declspec(section ".rodata") char str_Mahorn2_80302ffc[] = "Mahorn2";
__declspec(section ".rodata") u32 gap_04_80303004_rodata = 0;

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_mahorn2 20260624_191429 */

/* fallback stub-fill: map=effMahorn2Entry addr=0x8025d3f8 size=0x00000344 */
void* effMahorn2Entry(f32 x, f32 y, f32 z, f32 targetX, f32 targetY,
                      f32 targetZ, f32 targetDistance, f32 speed, s32 kind) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern f64 sin(f64 angle);
    extern f64 cos(f64 angle);
    extern void effMahorn2Main(void* entry);
    extern f32 float_0_80428b58;
    extern f32 float_6_80428b7c;
    extern f32 float_6p2832_80428b80;

    void* entry = effEntry();
    u8* work;
    u8* child;
    f32 zero = float_0_80428b58;
    f32 six = float_6_80428b7c;
    f32 twoPi;
    f32 temp;
    s32 i;

    *(char**)((s32)entry + 0x14) = str_Mahorn2_80302ffc;
    *(s32*)((s32)entry + 8) = 14;
    work = __memAlloc(3, 0x348);
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMahorn2Main;

    *(s32*)(work + 0) = kind;
    *(s32*)(work + 0x30) = 0;
    *(s32*)(work + 0x28) = 0;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x10) = targetX;
    *(f32*)(work + 0x14) = targetY;
    *(f32*)(work + 0x18) = targetZ;
    *(f32*)(work + 0x1C) = targetDistance;
    *(f32*)(work + 0x20) = speed;
    *(s32*)(work + 0x24) = 0xFF;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)(work + 0x38) = 0;
    child = work + 0x3C;

    for (i = 1; i < 4; i++, child += 0x3C) {
        memset(child, 0, 0x3C);
        *(s32*)(child + 0x2C) = 1;
        *(f32*)(child + 4) = zero;
        *(f32*)(child + 8) = zero;
        *(f32*)(child + 0xC) = zero;
        *(f32*)(child + 0x1C) = six;
        *(s32*)(child + 0x24) = 0;
        *(s32*)(child + 0x34) = (i - 1) * 15;
    }

    for (i = 4; i < 5; i++, child += 0x3C) {
        memset(child, 0, 0x3C);
        *(s32*)(child + 0x2C) = 2;
        *(f32*)(child + 4) = zero;
        *(f32*)(child + 8) = zero;
        *(f32*)(child + 0xC) = zero;
        *(f32*)(child + 0x1C) = zero;
        *(s32*)(child + 0x24) = 0x99;
        *(s32*)(child + 0x34) = 15;
    }

    for (i = 5; i < 6; i++, child += 0x3C) {
        memset(child, 0, 0x3C);
        *(s32*)(child + 0x2C) = 3;
        *(f32*)(child + 4) = zero;
        *(f32*)(child + 8) = zero;
        *(f32*)(child + 0xC) = zero;
        *(f32*)(child + 0x1C) = zero;
        *(s32*)(child + 0x24) = 0x99;
        *(s32*)(child + 0x34) = 60;
    }

    twoPi = float_6p2832_80428b80;
    for (i = 6; i < *(s32*)((s32)entry + 8); i++, child += 0x3C) {
        memset(child, 0, 0x3C);
        *(s32*)(child + 0x2C) = 4;
        *(f32*)(child + 4) = targetX - x;
        *(f32*)(child + 8) = targetY - y;
        *(f32*)(child + 0xC) = targetZ - z;
        temp = twoPi * (f32)(i - 6);
        *(f32*)(child + 0x10) = six * (f32)sin(temp / (f32)(*(s32*)((s32)entry + 8) - 6));
        *(f32*)(child + 0x14) = six * (f32)cos(temp / (f32)(*(s32*)((s32)entry + 8) - 6));
        *(f32*)(child + 0x18) = zero;
        *(f32*)(child + 0x1C) = zero;
        *(s32*)(child + 0x24) = 0xFF;
        *(s32*)(child + 0x34) = 0;
    }
    return entry;
}

/* CHATGPT STUB FILL: main/effect/eff_mahorn2 20260624_185035 */

/* stub-fill: effMahorn2Main | missing_definition | ghidra_signature */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMahorn2Main(void* entry) {
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 step, s32 duration);
    extern f32 PSVECDistance(void* a, void* b);
    extern f32 angleABf(f32 x1, f32 y1, f32 x2, f32 y2);
    extern f64 sin(f64 angle);
    extern f64 cos(f64 angle);
    extern void effHitEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale);
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(void* position);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* effect, f32 z);
    extern void effMahorn2Disp(s32 camera, void* effect);
    extern f32 float_0_80428b58;
    extern f32 float_0p0625_80428b5c;
    extern f32 float_3_80428b60;
    extern f32 float_6p2832_80428b64;
    extern f32 float_360_80428b68;
    extern f32 float_4_80428b6c;
    extern f32 float_0p125_80428b70;
    extern f32 float_0p88_80428b74;
    extern f32 float_1_80428b78;

    void* work = *(void**)((s32)entry + 0xC);
    f32 target[3];
    f32 targetCopy[3];
    f32 current[3];
    f32 zpos[3];
    f32 zposCopy[3];
    f32 speed = *(f32*)((s32)work + 0x20);
    f32 hitScale = *(f32*)((s32)work + 0x1C);
    s32 i;

    ((u32*)zpos)[0] = vec3_80302fa8[0];
    ((u32*)zpos)[1] = vec3_80302fa8[1];
    ((u32*)zpos)[2] = vec3_80302fa8[2];
    zpos[0] = *(f32*)((s32)work + 4);
    zpos[1] = *(f32*)((s32)work + 8);
    zpos[2] = *(f32*)((s32)work + 0xC);
    ((u32*)zposCopy)[0] = ((u32*)zpos)[0];
    ((u32*)zposCopy)[1] = ((u32*)zpos)[1];
    ((u32*)zposCopy)[2] = ((u32*)zpos)[2];
    ((u32*)target)[0] = vec3_80302fa8[3];
    ((u32*)target)[1] = vec3_80302fa8[4];
    ((u32*)target)[2] = vec3_80302fa8[5];
    target[0] = *(f32*)((s32)work + 0x10);
    target[1] = *(f32*)((s32)work + 0x14);
    target[2] = *(f32*)((s32)work + 0x18);
    ((u32*)targetCopy)[0] = ((u32*)target)[0];
    ((u32*)targetCopy)[1] = ((u32*)target)[1];
    ((u32*)targetCopy)[2] = ((u32*)target)[2];

    for (i = 0; i < *(s32*)((s32)entry + 8); i++, work = (void*)((s32)work + 0x3C)) {
        if (*(s32*)((s32)work + 0x34) != 0) {
            *(s32*)((s32)work + 0x34) -= 1;
            continue;
        }

        switch (*(s32*)((s32)work + 0x2C)) {
            case 1:
                *(s32*)((s32)work + 0x24) += 4;
                if (*(s32*)((s32)work + 0x24) > 0xFF) {
                    *(s32*)((s32)work + 0x24) = 0xFF;
                }
                *(f32*)((s32)work + 0x1C) -=
                    *(f32*)((s32)work + 0x1C) * float_0p0625_80428b5c;
                break;

            case 2:
                if (*(s32*)((s32)work + 0x28) < 0x5A) {
                    s32 timer = *(s32*)((s32)work + 0x28) + 1;
                    *(s32*)((s32)work + 0x28) = timer;
                    *(f32*)((s32)work + 0x1C) = intplGetValue(
                        float_0_80428b58, float_3_80428b60, 11,
                        timer, 0x2D);
                }
                break;

            case 3: {
                f32 distance;
                f32 angle;
                f32 radians;

                ((u32*)current)[0] = vec3_80302fa8[6];
                ((u32*)current)[1] = vec3_80302fa8[7];
                ((u32*)current)[2] = vec3_80302fa8[8];
                current[0] = zposCopy[0] + *(f32*)((s32)work + 4);
                current[1] = zposCopy[1] + *(f32*)((s32)work + 8);
                current[2] = zposCopy[2] + *(f32*)((s32)work + 0xC);
                distance = PSVECDistance(targetCopy, current);
                if (distance > speed) {
                    angle = angleABf(current[0], current[1], target[0], target[1]);
                    radians = (float_6p2832_80428b64 * angle) / float_360_80428b68;
                    *(f32*)((s32)work + 4) += speed * (f32)sin(radians);
                    *(f32*)((s32)work + 8) += speed * (f32)cos(radians);
                    *(f32*)((s32)work + 0x1C) +=
                        (float_4_80428b6c - *(f32*)((s32)work + 0x1C)) *
                        float_0p125_80428b70;
                } else {
                    *(f32*)((s32)work + 0x1C) -=
                        *(f32*)((s32)work + 0x1C) * float_0p125_80428b70;
                    if (*(s32*)((s32)*(void**)((s32)entry + 0xC) + 0x38) == 0) {
                        *(s32*)((s32)*(void**)((s32)entry + 0xC) + 0x38) = 1;
                        effHitEntry(0, 4, targetCopy[0], targetCopy[1], targetCopy[2], hitScale);
                    }
                }
                break;
            }

            case 4:
                if (*(s32*)((s32)*(void**)((s32)entry + 0xC) + 0x38) != 0) {
                    *(f32*)((s32)work + 4) += *(f32*)((s32)work + 0x10);
                    *(f32*)((s32)work + 8) += *(f32*)((s32)work + 0x14);
                    *(f32*)((s32)work + 0xC) += *(f32*)((s32)work + 0x18);
                    *(f32*)((s32)work + 0x10) *= float_0p88_80428b74;
                    *(f32*)((s32)work + 0x14) *= float_0p88_80428b74;
                    *(f32*)((s32)work + 0x18) *= float_0p88_80428b74;
                    *(f32*)((s32)work + 0x1C) = float_1_80428b78;
                    *(s32*)((s32)work + 0x24) -= 4;
                    if (*(s32*)((s32)work + 0x24) < 0) {
                        *(s32*)((s32)work + 0x24) = 0;
                        effDelete(entry);
                        return;
                    }
                }
                break;
        }
    }

    dispEntry(4, 2, effMahorn2Disp, entry, dispCalcZ(zposCopy));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_mahorn2 20260624_185035 */

/* stub-fill: effMahorn2Disp | missing_definition | ghidra_signature */
void effMahorn2Disp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32 cameraId);
    extern void effGetTexObj(s32 textureId, void* textureObject);
    extern void GXLoadTexObj(void* textureObject, s32 mapId);
    extern void GXSetNumChans(s32 count);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32 count);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32 count);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32 mode);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32 attr, s32 type);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void GXSetChanMatColor(s32 channel, void* color);
    extern void GXLoadPosMtxImm(f32 matrix[3][4], s32 id);
    extern void GXSetCurrentMtx(s32 id);
    extern void GXBegin(s32 primitive, s32 format, s32 count);
    extern void PSMTXTrans(f32 matrix[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXScale(f32 matrix[3][4], f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(f32 matrix[3][4], s32 axis, f32 angle);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern u32 dat_80428b40;
    extern f32 float_deg2rad_80428b44;
    extern f32 float_16_80428b48;
    extern f32 float_2_80428b4c;
    extern f32 float_8_80428b50;
    extern f32 float_0p5_80428b54;
    extern f32 float_0_80428b58;

    u8 textureObject[0x20];
    f32 base[3][4];
    f32 transform[3][4];
    f32 scaleMtx[3][4];
    f32 rotation[3][4];
    u8* work = *(u8**)((s32)effect + 0xC);
    u8* particle = work + 0x3C;
    void* camera = camGetPtr(cameraId);
    volatile f32* fifo = (volatile f32*)0xCC008000;
    s32 baseAlpha = *(s32*)(work + 0x24);
    f32 baseScale = *(f32*)(work + 0x1C);
    f32 deg2rad = float_deg2rad_80428b44;
    f32 size16 = float_16_80428b48;
    f32 size2 = float_2_80428b4c;
    f32 size8 = float_8_80428b50;
    f32 half = float_0p5_80428b54;
    f32 zero = float_0_80428b58;
    s32 i;

    PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 10);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(13, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    for (i = 1; i < *(s32*)((s32)effect + 8); i++, particle += 0x3C) {
        s32 mode;
        f32 width = size2;
        f32 height = size16;

        if (*(s32*)(particle + 0x34) != 0) {
            continue;
        }
        mode = *(s32*)(particle + 0x2C);
        if (mode == 1) {
            effGetTexObj(0x73, textureObject);
            GXLoadTexObj(textureObject, 0);
        } else if (mode > 0 && mode < 5) {
            effGetTexObj(0x74, textureObject);
            GXLoadTexObj(textureObject, 0);
        }

        {
            u32 color = dat_80428b40;
            s32 alpha = (*(s32*)(particle + 0x24) * baseAlpha) / 0xFF;
            ((u8*)&color)[3] = (u8)alpha;
            GXSetChanMatColor(4, &color);
        }

        PSMTXTrans(transform, *(f32*)(particle + 4), *(f32*)(particle + 8),
                    *(f32*)(particle + 0xC));
        width = *(f32*)(particle + 0x1C) * baseScale;
        PSMTXScale(scaleMtx, width, width, width);
        PSMTXConcat(transform, scaleMtx, transform);
        PSMTXRotRad(rotation, 'y', deg2rad *
                    -*(f32*)((u8*)camGetPtr(cameraId) + 0x114));
        PSMTXConcat(transform, rotation, transform);
        PSMTXConcat(base, transform, transform);
        PSMTXConcat((f32(*)[4])((u8*)camera + 0x11C), transform, transform);
        GXLoadPosMtxImm(transform, 0);
        GXSetCurrentMtx(0);

        if (mode == 2) {
            width = size2;
            height = size16;
        } else if (mode == 3) {
            width = size2;
            height = size2;
        } else if (mode == 4) {
            width = size2;
            height = size8;
        }

        GXBegin(0x80, 0, 4);
        *fifo = -width * half;
        *fifo = height * half;
        *fifo = zero; *fifo = 0.0f; *fifo = 0.0f;
        *fifo = width * half;
        *fifo = height * half;
        *fifo = zero; *fifo = 2.0f; *fifo = 0.0f;
        *fifo = width * half;
        *fifo = -height * half;
        *fifo = zero; *fifo = 2.0f; *fifo = 2.0f;
        *fifo = -width * half;
        *fifo = -height * half;
        *fifo = zero; *fifo = 0.0f; *fifo = 2.0f;
    }
}
