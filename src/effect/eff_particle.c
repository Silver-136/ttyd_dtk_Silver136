#include "effect/eff_particle.h"

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
    extern f32 float_deg2rad_80428280;
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
    extern char str_Particle_80300c20[];
    extern f32 float_1_80428290;
    extern f32 float_4_80428278;
    extern f32 float_0_80428288;
    extern f32 float_5_80428298;
    extern f32 float_125_8042829c;
    extern f32 float_25_804282a0;
    extern f32 float_1000_804282a4;
    extern f32 float_deg2rad_80428280;

    void* effect;
    void* work;
    f32 dir[3];
    f32 rotX[3][4];
    f32 rotZ[3][4];
    u32 i;

    effect = effEntry(type);
    *(char**)((s32)effect + 0x14) = str_Particle_80300c20;
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

/* CHATGPT STUB FILL: main/effect/eff_particle 20260624_184929 */

/* stub-fill: ParticleEmit | prototype_only | source_prototype */
void ParticleEmit(void* emitter, void* particle, f32 matrix[3][4]) {
    extern s32 seed;
    extern f64 log(f64);
    extern f64 sqrt(f64);
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

    if (use_last_431 == 0) {
        f32 y;
        f32 factor;
        do {
            seed = seed * 0x5EEDF715 + 0x1B0CB173;
            x = (f32)seed / 2147483648.0f;
            seed = seed * 0x5EEDF715 + 0x1B0CB173;
            y = (f32)seed / 2147483648.0f;
            length = x * x + y * y;
        } while (length > 1.0f);
        factor = (f32)sqrt((-2.0f * (f32)log(length)) / length);
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
        x = (f32)seed / 2147483648.0f;
        seed = seed * 0x5EEDF715 + 0x1B0CB173;
        z = (f32)seed / 2147483648.0f;
        length = x * x + z * z;
    } while (length > 1.0f);

    x *= 0.5f;
    z *= 0.5f;
    *(f32*)((s32)particle + 0x10) = x;
    *(f32*)((s32)particle + 0x14) = (f32)sqrt(1.0f - length * 0.25f);
    *(f32*)((s32)particle + 0x18) = z;
    PSVECScale((f32*)((s32)particle + 0x10),
               (f32*)((s32)particle + 0x10), *(f32*)((s32)work + 0xAC));
    PSMTXMultVec(matrix, (f32*)((s32)particle + 0x10),
                 (f32*)((s32)particle + 0x10));
    PSVECScale((f32*)((s32)particle + 0x10),
               (f32*)((s32)particle + 4), 2.5f);
    PSVECAdd((f32*)((s32)matrix + 0x30),
             (f32*)((s32)particle + 4), (f32*)((s32)particle + 4));
}

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_particle 20260624_191429 */

/* fallback stub-fill: map=effParticleMain addr=0x80244f78 size=0x000001d8 */
void effParticleMain(void* effect) {
    extern void effDelete(void* effect);
    extern void EmitterEmit(void* effect, void* data);
    extern void PSVECAdd(f32* a, f32* b, f32* out);
    extern f32 dispCalcZ(f32* pos);
    extern void dispEntry(s32 cameraId, s32 layer, void* callback, f32 z, void* param);
    extern void effParticleDisp(s32 cameraId, void* effect);
    extern f32 float_neg0p03_80428294;

    void* work;
    void* particle;
    void* particles;
    f32 pos[3];
    u32 idx;
    u32 max;

    work = *(void**)((s32)effect + 0xC);
    pos[0] = *(f32*)((s32)work + 4);
    pos[1] = *(f32*)((s32)work + 8);
    pos[2] = *(f32*)((s32)work + 0xC);

    if ((*(u32*)effect & 4) != 0) {
        *(u32*)effect &= ~4u;
        *(s32*)((s32)work + 0x28) = 0x10;
    }
    if (*(s32*)((s32)work + 0x28) < 1000) {
        *(s32*)((s32)work + 0x28) -= 1;
    }
    if (*(s32*)((s32)work + 0x28) < 0) {
        effDelete(effect);
        return;
    }

    if (*(s32*)((s32)work + 0x28) < 0x10) {
        *(s32*)((s32)work + 0x30) = *(s32*)((s32)work + 0x28) << 4;
    }

    *(s32*)((s32)work + 0x2C) += 1;
    EmitterEmit(effect, (void*)((s32)work + 0x34));

    max = *(u32*)((s32)work + 0xA4);
    particles = *(void**)((s32)work + 0xA0);
    idx = *(u32*)((s32)particles + (max << 5));
    while (idx != max + 1) {
        particle = (void*)((s32)particles + (idx << 5));
        *(s32*)((s32)particle + 0x1C) -= 1;
        if (*(s32*)((s32)particle + 0x1C) < 0) {
            *(u32*)((s32)particles + (max << 5)) = *(u32*)particle;
            *(u32*)particle = *(u32*)((s32)particles + (max << 5) + 0x20);
            *(u32*)((s32)particles + (max << 5) + 0x20) = idx;
            *(u32*)((s32)work + 0xA8) -= 1;
        } else {
            PSVECAdd((f32*)((s32)particle + 4), (f32*)((s32)particle + 0x10), (f32*)((s32)particle + 4));
            *(f32*)((s32)particle + 0x14) += float_neg0p03_80428294;
        }
        idx = *(u32*)((s32)particles + (max << 5));
    }

    dispEntry(4, 2, effParticleDisp, dispCalcZ(pos), effect);
}
