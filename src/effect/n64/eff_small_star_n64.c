#include "effect/n64/eff_small_star_n64.h"

extern f64 __frsqrte(f64);
extern f32 __float_nan[];

void* effSmallStarN64Entry(f32 x, f32 y, f32 z, f32 dirX, f32 dirY, f32 dirZ, s32 type, s32 count) {
    typedef f32 Mtx[3][4];
    typedef struct Vec {
        f32 x, y, z;
    } Vec;
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effSmallStarMain(void*);
    extern void* camGetPtr(s32);
    extern void PSMTXRotAxisRad(Mtx, Vec*, f32);
    extern void PSMTXMultVec(Mtx, Vec*, Vec*);
    extern f64 sqrt(f64);
    extern const f64 double_0p5_802fbf50;
    extern const f64 double_3_802fbf58;
    extern const f64 double_0_802fbf60;
    extern const f32 float_neg1_8042603c;
    extern f64 sin(f64);
    extern f64 cos(f64);
    extern char str_SmallStarN64_802fbf68[];
    extern const f32 vec3_802fbf18[3];
    extern const f32 vec3_802fbf24[3];
    extern const f32 vec3_802fbf30[3];
    static f32 rotation;
    void* entry = effEntry();
    u8* part;
    Vec cameraAxis4;
    Vec cameraAxis3;
    Vec directionVec;
    Vec rotAxis;
    Mtx matrix;
    f32 axisX;
    f32 axisY;
    f32 axisZ;
    f32 length;
    f32 lengthSq;
    f32 scale;
    f64 value64;
    f64 inv;
    f64 square;
    f32 classifyValue;
    u32 bits;
    u32 exp;
    s32 kind;
    f32 angle;
    f32 camAngle;
    s32 i;

    if (count == 0) {
        return 0;
    }
    lengthSq = dirX * dirX + dirY * dirY + dirZ * dirZ;
    if (lengthSq == 0.0f) {
        return 0;
    }
    value64 = (f64)lengthSq;
    if (lengthSq > 0.0f) {
        inv = __frsqrte(value64);

        square = inv * inv;
        inv = double_0p5_802fbf50 * inv *
              (double_3_802fbf58 - value64 * square);

        square = inv * inv;
        inv = double_0p5_802fbf50 * inv *
              (double_3_802fbf58 - value64 * square);

        square = inv * inv;
        inv = double_0p5_802fbf50 * inv *
              (double_3_802fbf58 - value64 * square);

        length = (f32)(value64 * inv);
    } else if (value64 < double_0_802fbf60) {
        length = __float_nan[0];
    } else {
        classifyValue = lengthSq;
        bits = *(u32*)&classifyValue;
        exp = bits;
        exp &= 0x7F800000;

        if (exp >= 0x7F800000) {
            if (exp == 0x7F800000) {
                if ((bits & 0x7FFFFF) != 0) {
                    kind = 1;
                } else {
                    kind = 2;
                }
            } else {
                kind = 4;
            }
        } else if (exp == 0) {
            if ((bits & 0x7FFFFF) != 0) {
                kind = 5;
            } else {
                kind = 3;
            }
        } else {
            kind = 4;
        }

        if (kind == 1) {
            length = __float_nan[0];
        } else {
            length = lengthSq;
        }
    }
    scale = float_neg1_8042603c / length;
    dirX *= scale;
    dirY *= scale;
    dirZ *= scale;

    if (dirX == 0.0f) {
        axisX = 1.0f;
        if (dirY == 0.0f) {
            axisY = 0.0f;
            axisZ = -dirX / dirZ;
        } else {
            axisY = -dirX / dirY;
            axisZ = 0.0f;
        }
    } else {
        axisX = -dirY / dirX;
        axisY = 1.0f;
        axisZ = 0.0f;
    }

    lengthSq = axisY * axisY + axisX * axisX + axisZ * axisZ;
    if (lengthSq == 0.0f) {
        return 0;
    }
    value64 = (f64)lengthSq;
    if (lengthSq > 0.0f) {
        inv = __frsqrte(value64);

        square = inv * inv;
        inv = double_0p5_802fbf50 * inv *
              (double_3_802fbf58 - value64 * square);

        square = inv * inv;
        inv = double_0p5_802fbf50 * inv *
              (double_3_802fbf58 - value64 * square);

        square = inv * inv;
        inv = double_0p5_802fbf50 * inv *
              (double_3_802fbf58 - value64 * square);

        length = (f32)(value64 * inv);
    } else if (value64 < double_0_802fbf60) {
        length = __float_nan[0];
    } else {
        classifyValue = lengthSq;
        bits = *(u32*)&classifyValue;
        exp = bits;
        exp &= 0x7F800000;

        if (exp >= 0x7F800000) {
            if (exp == 0x7F800000) {
                if ((bits & 0x7FFFFF) != 0) {
                    kind = 1;
                } else {
                    kind = 2;
                }
            } else {
                kind = 4;
            }
        } else if (exp == 0) {
            if ((bits & 0x7FFFFF) != 0) {
                kind = 5;
            } else {
                kind = 3;
            }
        } else {
            kind = 4;
        }

        if (kind == 1) {
            length = __float_nan[0];
        } else {
            length = lengthSq;
        }
    }
    scale = 1.0f / length;
    axisX *= scale;
    axisY *= scale;
    axisZ *= scale;
    dirX *= 8.0f;
    dirY *= 8.0f;
    dirZ *= 8.0f;
    axisX *= 8.0f;
    axisY *= 8.0f;
    axisZ *= 8.0f;

    *(char**)((u8*)entry + 0x14) = str_SmallStarN64_802fbf68;
    *(s32*)((u8*)entry + 8) = count;
    part = __memAlloc(3, count * 0x34);
    *(u8**)((u8*)entry + 0xC) = part;
    *(void**)((u8*)entry + 0x10) = effSmallStarMain;
    for (i = 0; i < count; i++, part += 0x34) {
        camAngle = *(f32*)((u8*)camGetPtr(4) + 0x114);
        *(s32*)part = type;
        *(f32*)(part + 4) = x;
        *(f32*)(part + 8) = y;
        *(f32*)(part + 0xC) = z;
        *(s8*)(part + 0x30) = 4;

        if (type == 3) {
            angle = count == 1 ? 0.0f : 100.0f * (f32)i / (f32)(count - 1) - 50.0f;

            cameraAxis3 = *(const Vec*)vec3_802fbf24;
            cameraAxis3.x = (f32)sin((6.2832f * camAngle) / 360.0f);
            cameraAxis3.z = -(f32)cos((6.2832f * camAngle) / 360.0f);
            rotAxis = cameraAxis3;

            PSMTXRotAxisRad(matrix, &rotAxis, 0.017453292f * angle);
            *(f32*)(part + 0x10) =
                matrix[0][0] * dirX + matrix[0][1] * dirY + matrix[0][2] * dirZ;
            *(f32*)(part + 0x14) =
                matrix[1][0] * dirX + matrix[1][1] * dirY + matrix[1][2] * dirZ;
            *(f32*)(part + 0x18) =
                matrix[2][0] * dirX + matrix[2][1] * dirY + matrix[2][2] * dirZ;
        } else if (type >= 3) {
            if (type < 5) {
                angle = 360.0f * (f32)i / (f32)(count - 1);

                cameraAxis4 = *(const Vec*)vec3_802fbf30;
                cameraAxis4.x = (f32)sin((6.2832f * camAngle) / 360.0f);
                cameraAxis4.z = -(f32)cos((6.2832f * camAngle) / 360.0f);
                rotAxis = cameraAxis4;

                PSMTXRotAxisRad(matrix, &rotAxis, 0.017453292f * angle);
                *(f32*)(part + 0x10) =
                    matrix[0][0] * dirX + matrix[0][1] * dirY + matrix[0][2] * dirZ;
                *(f32*)(part + 0x14) =
                    matrix[1][0] * dirX + matrix[1][1] * dirY + matrix[1][2] * dirZ;
                *(f32*)(part + 0x18) =
                    matrix[2][0] * dirX + matrix[2][1] * dirY + matrix[2][2] * dirZ;
            }
        } else if (type >= 0) {
            angle = 360.0f * (f32)i / (f32)(count - 1);

            directionVec = *(const Vec*)vec3_802fbf18;
            directionVec.x = dirX;
            directionVec.y = dirY;
            directionVec.z = dirZ;
            rotAxis = directionVec;

            PSMTXRotAxisRad(matrix, &rotAxis, 0.017453292f * angle);
            *(f32*)(part + 0x10) =
                matrix[0][0] * axisX + matrix[0][1] * axisY +
                matrix[0][2] * axisZ + dirX;
            *(f32*)(part + 0x14) =
                matrix[1][0] * axisX + matrix[1][1] * axisY +
                matrix[1][2] * axisZ + dirY;
            *(f32*)(part + 0x18) =
                matrix[2][0] * axisX + matrix[2][1] * axisY +
                matrix[2][2] * axisZ + dirZ;
        }

        if (type == 2) {
            *(f32*)(part + 0x20) = 20.0f;
            *(f32*)(part + 0x10) *= 0.5f;
            *(f32*)(part + 0x14) *= 0.5f;
            *(f32*)(part + 0x18) *= 0.5f;
            *(s32*)(part + 0x28) = 0x28;
        } else if (type >= 2) {
            if (type < 5) {
                *(f32*)(part + 0x20) = 20.0f;
                *(s32*)(part + 0x28) = 0x18;
            }
        } else if (type == 0) {
            *(f32*)(part + 0x20) = 0.0f;
            *(s32*)(part + 0x28) = 0x18;
        } else if (type >= 0) {
            *(f32*)(part + 0x20) = 20.0f;
            *(s32*)(part + 0x28) = 0x18;
        }
        *(f32*)(part + 0x1C) = 0.0f;
        *(s32*)(part + 0x24) = 0xFF;
        *(s32*)(part + 0x2C) = 0;
    }
    rotation += 15.0f;
    if (rotation > 360.0f) {
        rotation = 0.0f;
    }
    return entry;
}

void effSmallStarMain(void* effect) {
    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effSmallStarDisp(void);
    extern f32 float_0p8_80426030, float_0p94_80426028, float_0_8042602c, float_0p7_80426038,
        float_0p6_80426034, float_1_80426014;
    u8* work = *(u8**)((s32)effect + 0xC);
    s32 timer, i, type = *(s32*)work;
    timer = *(s32*)(work + 0x28) - 1;
    *(s32*)(work + 0x28) = timer;
    *(s32*)(work + 0x2C) += 1;
    if (timer < 0) {
        effDelete(effect);
        return;
    }
    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work += 0x34) {
        if (type == 2) {
            *(f32*)(work + 0x14) = float_1_80426014;
            *(f32*)(work + 0x10) *= float_0p94_80426028;
            *(f32*)(work + 0x18) *= float_0p94_80426028;
            if (*(f32*)(work + 0x14) < float_0_8042602c) {
                *(f32*)(work + 0x20) *= float_0p8_80426030;
                *(s32*)(work + 0x24) = (s32)((f32) * (s32*)(work + 0x24) * float_0p6_80426034);
            }
        }
        else {
            if (timer > 5) {
                *(f32*)(work + 0x10) *= float_0p8_80426030;
                *(f32*)(work + 0x14) *= float_0p8_80426030;
                *(f32*)(work + 0x18) *= float_0p8_80426030;
            }
            if (timer < 5) {
                *(f32*)(work + 0x20) *= float_0p8_80426030;
                *(s32*)(work + 0x24) = (s32)((f32) * (s32*)(work + 0x24) * float_0p6_80426034);
                *(f32*)(work + 0x10) *= float_0p7_80426038;
                *(f32*)(work + 0x14) *= float_0p7_80426038;
                *(f32*)(work + 0x18) *= float_0p7_80426038;
            }
        }
        *(f32*)(work + 4) += *(f32*)(work + 0x10);
        *(f32*)(work + 8) += *(f32*)(work + 0x14);
        *(f32*)(work + 0xC) += *(f32*)(work + 0x18);
        *(f32*)(work + 0x1C) += *(f32*)(work + 0x20);
    }
    dispEntry(*(u8*)(*(u8**)((s32)effect + 0xC) + 0x30), 2, effSmallStarDisp, effect,
              dispCalcZ(*(u8**)((s32)effect + 0xC) + 4));
}

void effSmallStarDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXSetTevColor(s32, void*);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern u8 color_rotation_data[];
    u8* w = *(u8**)((u8*)effect + 0xC);
    u8* cam = camGetPtr(cameraId);
    f32 a[3][4], b[3][4], c[3][4];
    u8 tex[0x20];
    u32 col;
    s32 i, idx = (*(s32*)(w + 0x2C) - 1) * 3;
    GXSetZMode(0, 7, 0);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    effGetTexObjN64(0x1D, tex);
    GXLoadTexObj(tex, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, -1);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 15, 15, 15, 2);
    GXSetTevAlphaIn(0, 7, 5, 4, 7);
    GXSetCullMode(0);
    effSetVtxDescN64((void*)0x803A89A0);
    for (i = 0; i < *(s32*)((u8*)effect + 8); i++, w += 0x34, idx += 3) {
        col = (color_rotation_data[idx % 36] << 24) | (color_rotation_data[(idx + 1) % 36] << 16)
            | (color_rotation_data[(idx + 2) % 36] << 8) | *(u8*)(w + 0x24);
        GXSetTevColor(1, &col);
        PSMTXTrans(a, *(f32*)(w + 4), *(f32*)(w + 8), *(f32*)(w + 0xC));
        PSMTXRotRad(b, 0x79, -0.017453292f * *(f32*)((u8*)camGetPtr(4) + 0x114));
        PSMTXRotRad(c, 0x7A, 0.017453292f * *(f32*)(w + 0x1C));
        PSMTXConcat(a, b, a);
        PSMTXConcat(a, c, a);
        PSMTXConcat(cam + 0x11C, a, a);
        GXLoadPosMtxImm(a, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x90, 0, 6);
        tri2(0, 1, 2, 0, 0, 2, 3, 0);
    }
}

void effSmallStarN64CamChg(void* effect, s8 camId) {
    void* work = *(void**)((s32)effect + 0xC);
    s32 i;

    for (i = 0; i < *(s32*)((s32)effect + 8); i++, work = (void*)((s32)work + 0x34)) {
        *(s8*)((s32)work + 0x30) = camId;
    }
}

u8 size16x16_tex32x32_vtx[] = {
    0xFF, 0xF8, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x08,
    0xFF, 0xF8, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x08, 0x00, 0x08,
    0x00, 0x00, 0x00, 0x40, 0x00, 0x20, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xF8, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0xFF,
};

const f32 vec3_802fbf18[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802fbf24[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802fbf30[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_802fbf3c[3] = { 0.0f, 0.0f, 0.0f };
const f64 double_0p5_802fbf50 = 0.5;
const f64 double_3_802fbf58 = 3.0;
const f64 double_0_802fbf60 = 0.0;
const char str_SmallStarN64_802fbf68[] = "SmallStarN64";
