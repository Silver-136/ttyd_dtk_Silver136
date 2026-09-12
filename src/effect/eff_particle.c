#include "effect/eff_particle.h"

extern f64 __frsqrte(f64 value);

/* CHATGPT STUB FILL: main/effect/eff_particle 20260624_184929 */

/* stub-fill: ParticleEmit | prototype_only | source_prototype */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void ParticleEmit(void* emitter, void* particle, f32 matrix[3][4]) {
    extern s32 seed;
    extern f64 log(f64);
    extern f32 __float_nan;
    extern const f32 vec3_80300c00[];
    extern const f32 float_0p5_80428284;
    extern const f32 float_0_80428288;
    extern const f32 float_2_8042828c;
    extern const f32 float_1_80428290;
    extern const u32 dat_804282a8;
    extern const f32 float_neg2_804282ac;
    extern const f32 float_0p25_804282b0;
    extern const f32 float_2p5_804282b4;
    extern void PSVECScale(f32*, f32*, f32);
    extern void PSMTXMultVec(f32[3][4], f32*, f32*);
    extern void PSVECAdd(f32*, f32*, f32*);

    static s32 use_last_431;
    static f32 y2_430;

    void* work = *(void**)((s32)emitter + 0xC);
    f32 gaussian;
    f32 x;
    f32 z;
    f32 length;
    const u8* constPool = (const u8*)vec3_80300c00;

    if (use_last_431 == 0) {
        f32 y;
        f32 factor;
        do {
            seed = seed * 0x5EEDF715 + 0x1B0CB173;
            x = float_2_8042828c * (*(const f32*)&dat_804282a8 * (f32)(u32)seed) - float_1_80428290;
            seed = seed * 0x5EEDF715 + 0x1B0CB173;
            y = float_2_8042828c * (*(const f32*)&dat_804282a8 * (f32)(u32)seed) - float_1_80428290;
            length = x * x + y * y;
        } while (length > float_1_80428290);
        factor = (float_neg2_804282ac * (f32)log(length)) / length;
        {
            f64 value = factor;
            if (value > *(const f64*)(constPool + 0x40)) {
                f64 half = *(const f64*)(constPool + 0x30);
                f64 three = *(const f64*)(constPool + 0x38);
                f64 inv = __frsqrte(value);
                inv = half * inv * (three - value * inv * inv);
                inv = half * inv * (three - value * inv * inv);
                factor = (f32)(value * half * inv * (three - value * inv * inv));
            } else if (value < *(const f64*)(constPool + 0x40)) {
                factor = __float_nan;
            } else {
                u32 exponent = *(u32*)&factor & 0x7F800000;
                s32 kind;
                if (exponent == 0x7F800000) {
                    if ((*(u32*)&factor & 0x7FFFFF) == 0) {
                        kind = 2;
                    } else {
                        kind = 1;
                    }
                } else if (exponent < 0x7F800000 && exponent == 0) {
                    if ((*(u32*)&factor & 0x7FFFFF) == 0) {
                        kind = 3;
                    } else {
                        kind = 5;
                    }
                } else {
                    kind = 4;
                }
                if (kind == 1) {
                    factor = __float_nan;
                }
            }
        }
        y2_430 = y * factor;
        gaussian = x * factor;
        use_last_431 = 1;
    } else {
        use_last_431 = 0;
        gaussian = y2_430;
    }

    *(s32*)((s32)particle + 0x1C) =
        (s32)(((f32*)matrix)[0x18] * gaussian + ((f32*)matrix)[0x17]);
    do {
        seed = seed * 0x5EEDF715 + 0x1B0CB173;
        x = float_2_8042828c * (*(const f32*)&dat_804282a8 * (f32)(u32)seed) - float_1_80428290;
        seed = seed * 0x5EEDF715 + 0x1B0CB173;
        z = float_2_8042828c * (*(const f32*)&dat_804282a8 * (f32)(u32)seed) - float_1_80428290;
        length = x * x + z * z;
    } while (length > float_1_80428290);

    x *= float_0p5_80428284;
    z *= float_0p5_80428284;
    *(f32*)((s32)particle + 0x10) = x;
    {
        f32 root = float_1_80428290 - length * float_0p25_804282b0;
        f64 value = root;
        if (value > *(const f64*)(constPool + 0x40)) {
            f64 half = *(const f64*)(constPool + 0x30);
            f64 three = *(const f64*)(constPool + 0x38);
            f64 inv = __frsqrte(value);
            inv = half * inv * (three - value * inv * inv);
            inv = half * inv * (three - value * inv * inv);
            root = (f32)(value * half * inv * (three - value * inv * inv));
        } else if (value < *(const f64*)(constPool + 0x40)) {
            root = __float_nan;
        } else {
            u32 exponent = *(u32*)&root & 0x7F800000;
            s32 kind;
            if (exponent == 0x7F800000) {
                if ((*(u32*)&root & 0x7FFFFF) == 0) {
                    kind = 2;
                } else {
                    kind = 1;
                }
            } else if (exponent < 0x7F800000 && exponent == 0) {
                if ((*(u32*)&root & 0x7FFFFF) == 0) {
                    kind = 3;
                } else {
                    kind = 5;
                }
            } else {
                kind = 4;
            }
            if (kind == 1) {
                root = __float_nan;
            }
        }
        *(f32*)((s32)particle + 0x14) = root;
    }
    *(f32*)((s32)particle + 0x18) = z;
    PSVECScale((f32*)((s32)particle + 0x10),
               (f32*)((s32)particle + 0x10), *(f32*)((s32)work + 0xAC));
    PSMTXMultVec(matrix, (f32*)((s32)particle + 0x10),
                 (f32*)((s32)particle + 0x10));
    PSVECScale((f32*)((s32)particle + 0x10),
               (f32*)((s32)particle + 4), float_2p5_804282b4);
    PSVECAdd((f32*)((s32)matrix + 0x30),
             (f32*)((s32)particle + 4), (f32*)((s32)particle + 4));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void EmitterEmit(void* emitter, void* data) {
    extern void ParticleEmit(void* emitter, void* particle, void* data);
    extern u32 __cvt_fp2unsigned(f32 value);

    void* work;
    u32 oldCount;
    u32 count;
    u32 overflow;
    u32 max;
    u32 index;
    u32 i;
    void* particles;
    u32 temp;
    u32 next;

    work = *(void**)((s32)emitter + 0xC);
    oldCount = __cvt_fp2unsigned(*(f32*)((s32)data + 0x54));
    *(f32*)((s32)data + 0x54) = *(f32*)((s32)data + 0x54) + *(f32*)((s32)data + 0x58);
    count = __cvt_fp2unsigned(*(f32*)((s32)data + 0x54)) - oldCount;
    *(u32*)((s32)work + 0xA8) += count;
    if (*(u32*)((s32)work + 0xA8) > *(u32*)((s32)work + 0xA4)) {
        overflow = *(u32*)((s32)work + 0xA8) - *(u32*)((s32)work + 0xA4);
        *(u32*)((s32)work + 0xA8) = *(u32*)((s32)work + 0xA4);
        count -= overflow;
    }
    if (count != 0) {
        max = *(u32*)((s32)work + 0xA4);
        i = 0;
        index = max + 1;
        while (i < count) {
            particles = *(void**)((s32)work + 0xA0);
            index = *(u32*)((s32)particles + (index << 5));
            ParticleEmit(emitter, (void*)((s32)particles + (index << 5)), data);
            i++;
        }
        particles = *(void**)((s32)work + 0xA0);
        temp = *(u32*)((s32)particles + (*(u32*)((s32)work + 0xA4) << 5));
        *(u32*)((s32)particles + (*(u32*)((s32)work + 0xA4) << 5)) =
            *(u32*)((s32)particles + (*(u32*)((s32)work + 0xA4) << 5) + 0x20);
        particles = *(void**)((s32)work + 0xA0);
        next = *(u32*)((s32)particles + (index << 5));
        max = *(u32*)((s32)work + 0xA4);
        particles = (void*)((s32)particles + (max << 5));
        *(u32*)((s32)particles + 0x20) = next;
        particles = *(void**)((s32)work + 0xA0);
        *(u32*)((s32)particles + (index << 5)) = temp;
    }
}

/* CHATGPT STUB FILL: main/effect/eff_particle 20260624_184929 */

/* stub-fill: effParticleEntry | missing_definition | ghidra_signature */
void* effParticleEntry(s32 type, s32 count, s32 duration, f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz) {
    extern void* effEntry(s32 type);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void effParticleMain(void);
    extern void PSMTXIdentity(f32 mtx[3][4]);
    extern void PSVECNormalize(f32* src, f32* dst);
    extern f32 angleABf(f32 ax, f32 ay, f32 bx, f32 by);
    extern void PSMTXRotRad(f32 mtx[3][4], s32 axis, f32 radians);
    extern void PSMTXConcat(f32 a[3][4], f32 b[3][4], f32 out[3][4]);
    extern const char str_Particle_80300c20[];
    extern const f32 float_1_80428290;
    extern const f32 float_4_80428278;
    extern const f32 float_0_80428288;
    extern const f32 float_5_80428298;
    extern const f32 float_125_8042829c;
    extern const f32 float_25_804282a0;
    extern const f32 float_1000_804282a4;
    extern const f32 float_deg2rad_80428280;

    void* effect;
    void* work;
    f32 dir[3];
    f32 rotX[3][4];
    f32 rotZ[3][4];
    u32 i;

    effect = effEntry(type);
    *(const char**)((s32)effect + 0x14) = str_Particle_80300c20;
    *(s32*)((s32)effect + 8) = 1;
    work = __memAlloc(3, (*(s32*)((s32)effect + 8) * 0xB0) + ((count + 2) << 5));
    *(void**)((s32)effect + 0xC) = work;
    *(void**)((s32)effect + 0x10) = effParticleMain;
    *(u32*)effect |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = vx;
    *(f32*)((s32)work + 0x14) = vy;
    *(f32*)((s32)work + 0x18) = vz;
    *(s32*)((s32)work + 0x28) = (duration > 0) ? duration : 1000;
    *(s32*)((s32)work + 0x2C) = 0;
    *(f32*)((s32)work + 0x24) = float_1_80428290;
    *(s32*)((s32)work + 0x30) = 0xFF;
    *(s32*)((s32)work + 0xA4) = count;
    *(void**)((s32)work + 0xA0) = (void*)((s32)work + (*(s32*)((s32)effect + 8) * 0xB0));
    *(f32*)((s32)work + 0xAC) = float_4_80428278;

    for (i = 1; i < (u32)count + 2; i++) {
        *(u32*)((s32)*(void**)((s32)work + 0xA0) + ((i - 1) << 5)) = i;
    }
    *(u32*)((s32)*(void**)((s32)work + 0xA0) + ((i - 1) << 5)) = 0;
    *(u32*)((s32)work + 0xA8) = 0;

    PSMTXIdentity((f32(*)[4])((s32)work + 0x34));
    *(f32*)((s32)work + 0x70) = float_0_80428288;
    *(f32*)((s32)work + 0x74) = float_0_80428288;
    *(f32*)((s32)work + 0x78) = float_0_80428288;
    *(f32*)((s32)work + 0x7C) = float_0_80428288;
    *(f32*)((s32)work + 0x80) = float_0_80428288;
    *(f32*)((s32)work + 0x84) = float_0_80428288;
    *(f32*)((s32)work + 0x88) = float_0_80428288;
    *(f32*)((s32)work + 0x8C) = float_5_80428298;
    *(f32*)((s32)work + 0x90) = float_125_8042829c;
    *(f32*)((s32)work + 0x94) = float_25_804282a0;

    dir[0] = vx;
    dir[1] = vy;
    dir[2] = vz;
    PSVECNormalize(dir, dir);
    *(f32*)((s32)work + 0x1C) = -angleABf(float_0_80428288, float_0_80428288, float_1000_804282a4 * dir[2], float_1000_804282a4 * dir[1]);
    *(f32*)((s32)work + 0x20) = -angleABf(float_0_80428288, float_0_80428288, float_1000_804282a4 * dir[0], float_1000_804282a4 * dir[1]);
    PSMTXRotRad(rotX, 0x78, float_deg2rad_80428280 * *(f32*)((s32)work + 0x1C));
    PSMTXRotRad(rotZ, 0x7A, float_deg2rad_80428280 * *(f32*)((s32)work + 0x20));
    PSMTXConcat(rotX, rotZ, (f32(*)[4])((s32)work + 0x34));
    return effect;
}

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_particle 20260624_191429 */

/* fallback stub-fill: map=effParticleMain addr=0x80244f78 size=0x000001d8 */
void effParticleMain(void* effect) {
    typedef struct Vec {
        f32 x, y, z;
    } Vec;
    extern void effDelete(void* effect);
    extern void EmitterEmit(void* effect, void* data);
    extern void PSVECAdd(f32* a, f32* b, f32* out);
    extern f32 dispCalcZ(f32* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
    extern void effParticleDisp(s32 cameraId, void* effect);
    extern f32 float_neg0p03_80428294;
    extern Vec vec3_80300c0c;

    s32* work;
    s32* link;
    s32* particle;
    Vec pos;
    s32 idx;

    pos = vec3_80300c0c;
    work = *(s32**)((s32)effect + 0xC);
    pos.x = *(f32*)&work[1];
    pos.y = *(f32*)&work[2];
    pos.z = *(f32*)&work[3];

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4u;
        work[10] = 0x10;
    }
    if (work[10] < 1000) {
        work[10]--;
    }
    if (work[10] < 0) {
        effDelete(effect);
    } else {
        if (work[10] < 0x10) {
            work[12] = work[10] << 4;
        }
        work[11]++;
        EmitterEmit(effect, work + 13);

        work = *(s32**)((s32)effect + 0xC);
        link = (s32*)(work[40] + (work[41] << 5));
        idx = *link;
        while (idx != work[41] + 1) {
            particle = (s32*)(work[40] + (idx << 5));
            particle[7]--;
            if (particle[7] < 0) {
                *link = *particle;
                *particle = *(s32*)(work[40] + (work[41] << 5) + 0x20);
                *(s32*)(work[40] + (work[41] << 5) + 0x20) = idx;
                work[42]--;
                particle = link;
            } else {
                PSVECAdd((f32*)(particle + 1), (f32*)(particle + 4), (f32*)(particle + 1));
                *(f32*)&particle[5] += float_neg0p03_80428294;
            }
            link = particle;
            idx = *particle;
        }
        dispEntry(4, 2, effParticleDisp, dispCalcZ((f32*)&pos), effect);
    }
}


/* CHATGPT STUB FILL: main/effect/eff_particle 20260624_184929 */

/* stub-fill: effParticleDisp | missing_definition | ghidra_signature */
void effParticleDisp(s32 cameraId, void* effect) {
    extern void* camGetPtr(s32);
    extern void effGetTexObj(s32, void*);
    extern void GXLoadTexObj(void*, s32);
    extern void GXSetNumChans(s32);
    extern void GXSetChanCtrl(s32, s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTexGens(s32);
    extern void GXSetTexCoordGen2(s32, s32, s32, s32, s32, s32);
    extern void GXSetNumTevStages(s32);
    extern void GXSetTevOrder(s32, s32, s32, s32);
    extern void GXSetTevColorOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaOp(s32, s32, s32, s32, s32, s32);
    extern void GXSetTevColorIn(s32, s32, s32, s32, s32);
    extern void GXSetTevAlphaIn(s32, s32, s32, s32, s32);
    extern void GXSetCullMode(s32);
    extern void GXClearVtxDesc(void);
    extern void GXSetVtxDesc(s32, s32);
    extern void GXSetVtxAttrFmt(s32, s32, s32, s32, s32);
    extern void PSMTXTrans(f32[3][4], f32, f32, f32);
    extern void PSMTXScale(f32[3][4], f32, f32, f32);
    extern void PSMTXRotRad(f32[3][4], s32, f32);
    extern void PSMTXConcat(f32[3][4], f32[3][4], f32[3][4]);
    extern void GXSetChanMatColor(s32, u32*);
    extern void GXLoadPosMtxImm(f32[3][4], s32);
    extern void GXSetCurrentMtx(s32);
    extern void GXBegin(s32, s32, s32);
    extern const f32 float_deg2rad_80428280;
    extern u32 hibana_col[];
    extern void* gpGlobals;

    u8* work = *(u8**)((u8*)effect + 0xC);
    void* camera = camGetPtr(cameraId);
    u8* particles = *(u8**)(work + 0xA0);
    s32 max = *(s32*)(work + 0xA4);
    s32 index = *(s32*)(particles + max * 0x20);
    s32 baseAlpha = *(s32*)(work + 0x30);
    f32 base[3][4];
    f32 trans[3][4];
    f32 rot[3][4];
    f32 scaleMtx[3][4];
    u8 texObj[0x20];
    volatile f32* fifo = (volatile f32*)0xCC008000;

    effGetTexObj(0x61, texObj);
    GXLoadTexObj(texObj, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 0xA);
    GXSetTevAlphaIn(0, 7, 1, 4, 7);
    GXSetCullMode(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    PSMTXTrans(trans, *(f32*)(work + 4), *(f32*)(work + 8), *(f32*)(work + 0xC));
    PSMTXScale(scaleMtx, *(f32*)(work + 0x24), *(f32*)(work + 0x24), *(f32*)(work + 0x24));
    PSMTXConcat(trans, scaleMtx, base);

    while (index != max + 1) {
        u8* particle = particles + index * 0x20;
        u32 source = hibana_col[(*(s32*)(particle + 0x1C) >> 3)];
        u32 alpha = baseAlpha * (source & 0xFF) / 255;
        u32 color = (source & 0xFFFFFF00) | (alpha & 0xFF);
        PSMTXTrans(trans, *(f32*)(particle + 4), *(f32*)(particle + 8), *(f32*)(particle + 0xC));
        PSMTXConcat(base, trans, trans);
        PSMTXConcat((f32(*)[4])((s32)camera + 0x34), trans, trans);
        PSMTXRotRad(rot, 0x79, float_deg2rad_80428280 * -*(f32*)((s32)camGetPtr(cameraId) + 0x114));
        PSMTXConcat(trans, rot, trans);
        PSMTXRotRad(rot, 0x78,
                    float_deg2rad_80428280 * (f32)(((*(u32*)((s32)gpGlobals + 0x1C) + index) % 360) * 10));
        PSMTXConcat(trans, rot, trans);
        GXSetChanMatColor(4, &color);
        GXLoadPosMtxImm(trans, 0);
        GXSetCurrentMtx(0);
        GXBegin(0x80, 0, 4);
        fifo[0] = -2.0f; fifo[0] = 4.0f; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 0.0f;
        fifo[0] = 2.0f; fifo[0] = 4.0f; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 0.0f;
        fifo[0] = 2.0f; fifo[0] = -4.0f; fifo[0] = 0.0f; fifo[0] = 1.0f; fifo[0] = 1.0f;
        fifo[0] = -2.0f; fifo[0] = -4.0f; fifo[0] = 0.0f; fifo[0] = 0.0f; fifo[0] = 1.0f;
        index = *(s32*)particle;
    }
}

const f32 vec3_80300c00[3] = { 0.0f, 0.0f, 0.0f };
const f32 vec3_80300c0c[3] = { 0.0f, 0.0f, 0.0f };
const f64 double_to_int_mask_80300c18 = 4503599627370496.0;
const char str_Particle_80300c20[] = "Particle";
const f64 double_0p5_80300c30 = 0.5;
const f64 double_3_80300c38 = 3.0;
const f64 double_0_80300c40 = 0.0;
const f64 double_to_int_80300c48 = 4503601774854144.0;

u32 hibana_col[] = {
    0x5F280000, 0x69350008, 0x73420010, 0x7D4F0018,
    0x885D0020, 0x926A0029, 0x9D780031, 0xA7850039,
    0xB1920041, 0xBCA0004A, 0xC6AD0052, 0xD0BA005A,
    0xDBC80062, 0xE5D5006A, 0xEFE20073, 0xFAF0007B,
    0xFFF00083, 0xFFF1108B, 0xFFF22194, 0xFFF3329C,
    0xFFF444A4, 0xFFF555AC, 0xFFF666B4, 0xFFF777BD,
    0xFFF888C5, 0xFFF999CD, 0xFFFAAAD5, 0xFFFBBBDE,
    0xFFFCCCE6, 0xFFFDDDEE, 0xFFFEEEF6, 0xFFFFFFFF,
};

s32 seed = 1;

const f32 float_4_80428278 = 4.0f;
const f32 float_8_8042827c = 8.0f;
const f32 float_deg2rad_80428280 = 0.017453292f;
const f32 float_0p5_80428284 = 0.5f;
const f32 float_0_80428288 = 0.0f;
const f32 float_2_8042828c = 2.0f;
const f32 float_1_80428290 = 1.0f;
const f32 float_neg0p03_80428294 = -0.03f;
const f32 float_5_80428298 = 5.0f;
const f32 float_125_8042829c = 125.0f;
const f32 float_25_804282a0 = 25.0f;
const f32 float_1000_804282a4 = 1000.0f;
const u32 dat_804282a8 = 0x2F800000;
const f32 float_neg2_804282ac = -2.0f;
const f32 float_0p25_804282b0 = 0.25f;
const f32 float_2p5_804282b4 = 2.5f;
