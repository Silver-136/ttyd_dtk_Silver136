#include "effect/n64/eff_stardust2_n64.h"
extern f64 sin(f64);
extern f64 cos(f64);
extern f32 float_1_8042621c;
extern f32 float_360_80426240;
extern f32 float_6p2832_80426248;
extern f32 float_0p1_80426224;
extern f32 float_0p01_80426238;

static u8 speed_data[14] = {
    0x1E, 0x3C, 0x5A, 0x64, 0x68, 0x6A, 0x6C,
    0x6E, 0x70, 0x71, 0x71, 0x6E, 0x6B, 0x67,
};
static u8 scale_data[14] = {
    0x1E, 0x3C, 0x5A, 0x64, 0x68, 0x6A, 0x6C,
    0x6E, 0x70, 0x71, 0x71, 0x6E, 0x6B, 0x67,
};



void* effStardust2N64Entry(
    f32 x, f32 y, f32 z, f32 size, f32 spread, s32 type, s32 count, s32 time) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32, s32);
    extern void effStardust2Main(void*);
    extern char str_Stardust2N64_802fc088[];
    extern s32 rand(void);
    extern f32 float_0_80426218;
    extern f32 float_0p4_80426254;
    extern f32 float_10_80426258;
    extern f32 float_90_8042625c;
    extern f32 float_0p03_80426260;
    extern f32 float_0p04_80426264;
    extern f32 float_0p005_80426268;

    void* entry = effEntry();
    u8* work;
    u8* particle;
    f32 countf;
    f32 step;
    f32 one;
    f32 fullCircle;
    f32 tenth;
    f32 fourTenths;
    f32 ten;
    f32 ninety;
    f32 zero;
    s32 i;

    *(char**)((s32)entry + 0x14) = str_Stardust2N64_802fc088;
    *(s32*)((s32)entry + 8) = count + 1;
    work = __memAlloc(3, (count + 1) * 0x30);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effStardust2Main;

    if (type == 6) {
        *(u16*)work = 1;
        type = 3;
    } else {
        *(u16*)work = 0;
    }

    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(s32*)(work + 0x20) = 0;
    *(s16*)(work + 2) = type;
    *(s32*)(work + 0x24) = time;
    *(s32*)(work + 0x2C) = 4;

    one = float_1_8042621c;
    fullCircle = float_360_80426240;
    tenth = float_0p1_80426224;
    fourTenths = float_0p4_80426254;
    ten = float_10_80426258;
    ninety = float_90_8042625c;
    zero = float_0_80426218;
    countf = (f32)(count + 1) - one;
    step = fullCircle / countf;
    particle = work + 0x30;
    for (i = 1; i < count + 1; i++, particle += 0x30) {
        f32 radialAngle = step * (f32)i;
        f32 randomAngle;
        f32 angle;
        f32 sine;
        f32 cosine;
        f32 vertical;
        f32 radius;

        if (type < 2) {
            randomAngle = zero;
        } else {
            randomAngle = (f32)(rand() % 360);
        }
        angle = float_6p2832_80426248 * (radialAngle + randomAngle) / fullCircle;
        sine = (f32)sin(angle);
        cosine = (f32)cos(angle);
        vertical = (f32)sin(float_6p2832_80426248 *
                            (ten * radialAngle - ninety) / fullCircle);
        radius = -(tenth * size * vertical - fourTenths * size);

        *(s32*)(particle + 0x24) = i + 0x1E;
        *(s32*)(particle + 0x20) = 0;
        *(f32*)(particle + 0x1C) = one;

        if (type == 3) {
            goto type3_particle;
        }
        if (type < 3) {
            if (type < 2) {
                if (type < 0) {
                    goto default_particle;
                }
                goto type01_particle;
            }
            goto type2_or5_particle;
        }
        if (type == 5) {
            goto type2_or5_particle;
        }
        goto default_particle;

type01_particle:
        *(f32*)(particle + 0x10) = sine * radius;
        *(f32*)(particle + 0x14) = cosine *
            -(tenth * spread * vertical - fourTenths * spread);
        *(f32*)(particle + 0x18) = zero;
        *(f32*)(particle + 0xC) = zero;
        goto particle_done;

type2_or5_particle:
        *(f32*)(particle + 0x10) = sine * radius;
        *(f32*)(particle + 8) = ((f32)i - one) * spread / countf;
        *(f32*)(particle + 0xC) = cosine * radius;
        *(f32*)(particle + 0x14) =
            float_0p03_80426260 * (f32)-(rand() % 10);
        *(f32*)(particle + 0x18) =
            float_0p04_80426264 * (f32)(rand() % 10) +
            float_0p01_80426238;
        goto particle_done;

type3_particle:
        *(f32*)(particle + 4) =
            float_0p005_80426268 * sine * size * (f32)(rand() % 100);
        *(f32*)(particle + 8) =
            float_0p005_80426268 * cosine * spread * (f32)(rand() % 100);
        *(f32*)(particle + 0xC) = zero;
        *(f32*)(particle + 0x10) = zero;
        *(f32*)(particle + 0x14) = zero;
        *(f32*)(particle + 0x18) = zero;
        *(s32*)(particle + 0x24) =
            *(s32*)(particle + 0x24) + i + i;
        goto particle_done;

default_particle:
        *(f32*)(particle + 0x10) = sine * radius;
        *(f32*)(particle + 8) = ((f32)i - one) * spread / countf;
        *(f32*)(particle + 0xC) = cosine * radius;
        *(f32*)(particle + 0x14) =
            float_0p03_80426260 * (f32)-(rand() % 10);
        *(f32*)(particle + 0x18) =
            float_0p04_80426264 * (f32)(rand() % 10) +
            float_0p01_80426238;

particle_done:
        ;
    }
    return entry;
}

u8 effStardust2Main(void* effect) {
    extern void effDelete(void*);
    extern f32 dispCalcZ(void*);
    extern void dispEntry(s32, s32, void*, void*, f32);
    extern void effStardust2Disp(void);
    extern f32 float_0p96_80426250;
    extern f32 float_neg0p02_80426234;
    extern f32 float_60_8042623c;
    extern f32 float_120_80426244;
    extern f32 float_neg0p05_8042624c;
    extern f64 double_to_int_mask_802fc078;

    u8* work = *(u8**)((s32)effect + 0xC);
    u8* particle = work + 0x30;
    f32 position[3];
    s32 type;
    s32 frame;
    s32 dead = 0;
    s32 count;
    s32 i;
    union {
        f64 value;
        struct { u32 hi, lo; } words;
    } cvt;

    position[0] = *(f32*)(work + 4);
    position[1] = *(f32*)(work + 8);
    position[2] = *(f32*)(work + 0xC);
    *(s32*)(work + 0x24) -= 1;
    *(s32*)(work + 0x20) += 1;
    type = *(s16*)(work + 2);
    frame = *(s32*)(work + 0x20);

    if ((type < 2 || type == 3 || type > 9) && *(s32*)(work + 0x24) < 0) {
        effDelete(effect);
        return 0;
    }

    count = *(s32*)((s32)effect + 8) - 1;
    for (i = 0; i < count; i++, particle += 0x30) {
        if (type < 6) {
            if (type >= 0 && type < 2) {
                f32 angle;
                *(s32*)(particle + 0x28) = (frame + i) & 7;
                *(f32*)(particle + 0x18) += float_neg0p02_80426234;
                if ((u32)(frame - 1) < 14) {
                    f32 scale;
                    cvt.words.hi = 0x43300000;
                    cvt.words.lo = speed_data[frame - 1];
                    scale = float_0p01_80426238 *
                            (f32)(cvt.value - double_to_int_mask_802fc078);
                    *(f32*)(particle + 4) = *(f32*)(particle + 0x10) * scale;
                    *(f32*)(particle + 8) =
                        *(f32*)(particle + 0x14) * scale + *(f32*)(particle + 0x18);
                    cvt.words.lo = scale_data[frame - 1];
                    *(f32*)(particle + 0x1C) =
                        float_0p01_80426238 *
                        (f32)(cvt.value - double_to_int_mask_802fc078);
                } else {
                    angle = (float_360_80426240 / (f32)count) * (f32)i + float_60_8042623c;
                    if (i & 1) {
                        angle -= float_120_80426244;
                    }
                    angle = float_6p2832_80426248 * angle / float_360_80426240;
                    *(s16*)(particle + 2) = 10;
                    *(f32*)(particle + 0x10) = float_1_8042621c * (f32)sin(angle);
                    *(f32*)(particle + 0x14) = float_1_8042621c * (f32)cos(angle);
                    *(f32*)(particle + 0x18) = float_neg0p05_8042624c;
                }
                continue;
            }
            if (type == 2 || type == 3) {
                *(s32*)(particle + 0x28) = (frame + i * 2) & 7;
                if ((u32)(frame - 1) < 14) {
                    cvt.words.hi = 0x43300000;
                    cvt.words.lo = scale_data[frame - 1];
                    *(f32*)(particle + 0x1C) =
                        float_0p01_80426238 *
                        (f32)(cvt.value - double_to_int_mask_802fc078);
                } else {
                    *(s16*)(particle + 2) = 30;
                }
                continue;
            }
        } else if (type == 10 || type == 30) {
            *(s32*)(particle + 0x28) = (frame + i) & 7;
            *(f32*)(particle + 0x10) *= float_0p96_80426250;
            *(f32*)(particle + 0x14) *= float_0p96_80426250;
            *(f32*)(particle + 0x14) += *(f32*)(particle + 0x18);
            *(f32*)(particle + 4) += *(f32*)(particle + 0x10);
            *(f32*)(particle + 8) += *(f32*)(particle + 0x14);
            *(f32*)(particle + 0x1C) +=
                float_0p1_80426224 * (float_0p1_80426224 - *(f32*)(particle + 0x1C));
            continue;
        }

        *(s32*)(particle + 0x28) = (frame + i * 2) & 7;
        if (*(s32*)(particle + 0x24) < 0) {
            dead++;
        }
        *(s32*)(particle + 0x24) -= 1;
        if (*(s32*)(particle + 0x24) >= 0 && *(s32*)(particle + 0x24) < 31) {
            *(s32*)(particle + 0x20) += 1;
            if ((u32)(*(s32*)(particle + 0x20) - 1) < 14) {
                cvt.words.hi = 0x43300000;
                cvt.words.lo = scale_data[*(s32*)(particle + 0x20) - 1];
                *(f32*)(particle + 0x1C) =
                    float_0p01_80426238 *
                    (f32)(cvt.value - double_to_int_mask_802fc078);
                if (type == 5) {
                    *(f32*)(particle + 0x14) += *(f32*)(particle + 0x18);
                    *(f32*)(particle + 8) += *(f32*)(particle + 0x14);
                }
            } else {
                *(f32*)(particle + 0x1C) +=
                    float_0p1_80426224 * (float_0p1_80426224 - *(f32*)(particle + 0x1C));
                *(f32*)(particle + 0x14) += *(f32*)(particle + 0x18);
                *(f32*)(particle + 8) += *(f32*)(particle + 0x14);
            }
            if (type == 4) {
                f32 angle = float_6p2832_80426248 * (f32)(*(s32*)(particle + 0x20) * 12) /
                            float_360_80426240;
                *(f32*)(particle + 4) = *(f32*)(particle + 0x10) * (f32)sin(angle);
                *(f32*)(particle + 0xC) = *(f32*)(particle + 0x10) * (f32)cos(angle);
            } else {
                *(f32*)(particle + 4) = *(f32*)(particle + 0x10);
            }
            *(f32*)(particle + 0x14) *= float_0p96_80426250;
        } else {
            *(s32*)(particle + 0x28) = -1;
        }
    }

    if (type < 2 || type == 3 || type > 9 || dead < count) {
        dispEntry(*(s32*)(work + 0x2C), 2, effStardust2Disp, effect, dispCalcZ(position));
    } else {
        effDelete(effect);
    }
    return 0;
}

void effStardust2Disp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void PSMTXTrans(void*, f32, f32, f32);
    extern void PSMTXRotRad(void*, s32, f32);
    extern void PSMTXConcat(void*, void*, void*);
    extern void PSMTXScale(void*, f32, f32, f32);
    extern void GXSetNumChans(s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXLoadTexMtxImm(void*, s32, s32);
    extern void effGetTexObjN64(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXSetTevColor(s32, void*);
    extern void GXLoadPosMtxImm(void*, s32);
    extern void GXSetCurrentMtx(s32);
    extern void effSetVtxDescN64(void*);
    extern void GXBegin(s32, s32, s32);
    extern void tri2(s32, s32, s32, s32, s32, s32, s32, s32);
    extern f32 float_deg2rad_8042620c;
    extern f32 float_0p0056818_80426210;
    extern f32 float_0p045455_80426214;
    extern f32 float_0_80426218;
    extern f32 float_1_8042621c;
    extern f32 float_0p5_80426220;
    extern f32 float_0p1_80426224;
    extern f32 float_0p8_80426228;
    extern f32 float_0p7_8042622c;
    extern f32 float_1p4_80426230;
    extern u8 color_rotation_data[];
    f32 base[3][4], rot[3][4], texMtx[3][4], mtx[3][4];
    u8 texObj[0x20];
    u8* work = *(u8**)((s32)effect + 0xC);
    u8* p = work + 0x30;
    void* camera = camGetPtr(cameraId);
    void* vertices;
    f32 redScale, greenScale, blueScale;
    s32 colorIndex = (*(s32*)(work + 0x20) - 1) * 3;
    s32 i;
    s32 selector;
    u32 color;

    PSMTXTrans(base, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_8042620c * -*(f32*)((s32)camGetPtr(4) + 0x114));
    PSMTXConcat(base, rot, base);
    PSMTXConcat((void*)((s32)camera + 0x11C), base, base);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    PSMTXScale(texMtx, float_0p0056818_80426210, float_0p045455_80426214, float_0_80426218);
    GXLoadTexMtxImm(texMtx, 0x1E, 1);
    effGetTexObjN64(0x20, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 2, 12, 8, 15);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetCullMode(0);

    switch (*(s16*)(work + 2)) {
        case 0:
        case 1:
        case 2:
        case 10:
            redScale = float_1_8042621c;
            greenScale = float_0p5_80426220;
            blueScale = float_0p1_80426224;
            break;
        case 3:
        case 30:
            redScale = float_1_8042621c;
            greenScale = float_0p8_80426228;
            blueScale = float_0p7_8042622c;
            break;
        case 5:
            redScale = float_1p4_80426230;
            greenScale = float_1p4_80426230;
            blueScale = float_0p7_8042622c;
            break;
        default:
            redScale = float_1p4_80426230;
            greenScale = float_0p7_8042622c;
            blueScale = float_0p7_8042622c;
            break;
    }

    for (i = 0; i < *(s32*)((s32)effect + 8) - 1; i++, p += 0x30) {
        selector = *(s32*)(p + 0x28);
        if (selector >= 0) {
            PSMTXTrans(mtx, *(f32*)(p + 4), *(f32*)(p + 8), *(f32*)(p + 0xC));
            mtx[0][0] = *(f32*)(p + 0x1C);
            mtx[1][1] = mtx[0][0];
            mtx[2][2] = mtx[0][0];
            PSMTXConcat(base, mtx, mtx);
            GXLoadPosMtxImm(mtx, 0);
            GXSetCurrentMtx(0);
            if (colorIndex >= 36) colorIndex = 0;
            color = ((u8)((f32)color_rotation_data[colorIndex++] * redScale) << 24) |
                    ((u8)((f32)color_rotation_data[colorIndex++] * greenScale) << 16) |
                    ((u8)((f32)color_rotation_data[colorIndex++] * blueScale) << 8) | 0xFF;
            GXSetTevColor(1, &color);
            switch (selector & 7) {
                case 0: vertices = (void*)0x803A9090; break;
                case 1: vertices = (void*)0x803A90C8; break;
                case 2: vertices = (void*)0x803A9100; break;
                case 3: vertices = (void*)0x803A9138; break;
                case 4: vertices = (void*)0x803A9170; break;
                case 5: vertices = (void*)0x803A8FE8; break;
                case 6: vertices = (void*)0x803A9020; break;
                case 7: vertices = (void*)0x803A9058; break;
                default: vertices = (void*)0x803A8FE8; break;
            }
            effSetVtxDescN64(vertices);
            GXBegin(0x90, 0, 6);
            tri2(0, 1, 2, 0, 0, 2, 3, 0);
        }
    }
}

void effStardust2N64SetDrawCam(void* effect, s32 camId) {
    *(s32*)((s32)*(void**)((s32)effect + 0xC) + 0x2C) = camId;
}

typedef struct EffN64Vertex {
    s16 x;
    s16 y;
    s16 z;
    s16 s;
    s16 t;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} EffN64Vertex;

EffN64Vertex size8x8_tex22x22_vtx[32] = {
    {     -8,     -8,      0,      0,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,     -8,      0,    704,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,      8,      0,    704,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,      8,      0,      0,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,     -8,      0,    704,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,     -8,      0,   1408,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,      8,      0,   1408,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,      8,      0,    704,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,     -8,      0,   1408,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,     -8,      0,   2112,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,      8,      0,   2112,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,      8,      0,   1408,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,     -8,      0,   2112,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,     -8,      0,   2816,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,      8,      0,   2816,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,      8,      0,   2112,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,     -8,      0,   2816,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,     -8,      0,   3520,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,      8,      0,   3520,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,      8,      0,   2816,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,     -8,      0,   3520,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,     -8,      0,   4224,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,      8,      0,   4224,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,      8,      0,   3520,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,     -8,      0,   4224,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,     -8,      0,   4928,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,      8,      0,   4928,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,      8,      0,   4224,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,     -8,      0,   4928,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,     -8,      0,   5632,      0, 0x00, 0x00, 0x00, 0xFF },
    {      8,      8,      0,   5632,    704, 0x00, 0x00, 0x00, 0xFF },
    {     -8,      8,      0,   4928,    704, 0x00, 0x00, 0x00, 0xFF },
};
