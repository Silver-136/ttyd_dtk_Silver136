#include "effect/eff_damage_star.h"


u8 effDamageStarDisp(s32 cameraId, void* effect) {
    extern void effGetTexObj(s32 id, void* obj);
    extern void GXLoadTexObj(void* obj, s32 map);
    extern void GXSetNumTexGens(s32);
    extern void GXSetNumChans(s32);
    extern void GXSetCullMode(s32);
    extern void GXSetBlendMode(s32, s32, s32, s32);
    extern void GXSetZMode(s32, s32, s32);
    extern void GXBegin(s32, s32, s32);
    u8 tex[0x20];
    volatile f32* fifo;
    s32* work;
    s32 count;
    s32 i;

    work = *(s32**)((s32)effect + 0xC);
    if (work == 0) return 0;
    count = *(s32*)((s32)effect + 8);
    fifo = (volatile f32*)0xCC008000;
    for (i = 0; i < 4; i++) {
        effGetTexObj(i, tex);
        GXLoadTexObj(tex, i);
    }
    GXSetNumTexGens(2);
    GXSetNumChans(1);
    GXSetBlendMode(0, 1, 0, 0);
    GXSetZMode(0, 3, 0);
    for (i = 2; i < count; i++) {
        GXSetCullMode(2);
        GXBegin(0x80, 0, 4);
        fifo[0] = -8.0f; fifo[0] = 8.0f; fifo[0] = 0.0f;
        fifo[0] = 0.0f; fifo[0] = 0.0f;
        fifo[0] = 8.0f; fifo[0] = 8.0f; fifo[0] = 0.0f;
        fifo[0] = 2.0f; fifo[0] = 0.0f;
        fifo[0] = 8.0f; fifo[0] = -8.0f; fifo[0] = 0.0f;
        fifo[0] = 2.0f; fifo[0] = 1.0f;
        fifo[0] = -8.0f; fifo[0] = -8.0f; fifo[0] = 0.0f;
        fifo[0] = 0.0f; fifo[0] = 1.0f;
    }
    return 0;
}

void* effDamageStarEntry(f32 x, f32 y, f32 z, f32 speed, f32 baseRot, s32 kind, s32 count) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* ptr, s32 value, u32 size);
    extern void* effMayaAnimAlloc(void* data);
    extern void effPlayMayaAnim(void* anim);
    extern void* gpGlobals;
    extern char str_DamageStar_802f3a80[];
    extern void* damageStar_effAnimDt;
    extern u8 effDamageStarMain(void*);
    extern f32 float_0_80422c80;
    extern f32 float_1_80422c98;
    extern f32 float_30_80422cdc;
    extern f32 float_360_80422ce4;
    extern f32 float_6p2832_80422ce0;
    extern f32 float_1p5_80422cd0;
    extern f32 float_0p2_80422d04;
    extern f32 sin(f32);
    extern f32 cos(f32);
    void* entry;
    void* work;
    void* part;
    s32 effCount;
    s32 size;
    s32 i;
    f32 angle;
    f32 spread;
    f32 dir;
    f32 s;
    f32 c;

    entry = effEntry();
    if (count > 20) {
        count = 20;
    }
    effCount = count + 2;
    *(void**)((s32)entry + 0x14) = str_DamageStar_802f3a80;
    *(s32*)((s32)entry + 0x8) = effCount;

    size = effCount * 0x48;
    work = __memAlloc(3, size);
    *(void**)((s32)entry + 0xC) = work;
    memset(work, 0, size);

    *(void**)((s32)entry + 0x10) = effDamageStarMain;
    *(void**)((s32)work + 0x44) = effMayaAnimAlloc(&damageStar_effAnimDt);
    effPlayMayaAnim(*(void**)((s32)work + 0x44));

    *(s32*)((s32)work + 0x0) = kind;
    *(f32*)((s32)work + 0x4) = x;
    *(f32*)((s32)work + 0x8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x1C) = speed;
    *(f32*)((s32)work + 0x38) = baseRot;
    *(s32*)((s32)work + 0x24) = 99;
    *(s32*)((s32)work + 0x28) = 0;
    *(s32*)((s32)work + 0x3C) = count;
    *(s32*)((s32)work + 0x40) = *(s32*)((s32)gpGlobals + 0x1C);

    spread = float_30_80422cdc - (f32)effCount;
    part = (void*)((s32)work + 0x48);
    for (i = 1; i < effCount; i++, part = (void*)((s32)part + 0x48)) {
        dir = (f32)((i * ((i & 1) != 0 ? 1 : -1)) / 2);
        *(f32*)((s32)part + 0x38) = baseRot + spread * dir;
        *(f32*)((s32)part + 0x4) = float_0_80422c80;
        *(f32*)((s32)part + 0x8) = float_0_80422c80;
        *(f32*)((s32)part + 0xC) = float_0_80422c80;
        angle = (float_6p2832_80422ce0 * *(f32*)((s32)part + 0x38)) / float_360_80422ce4;
        s = sin(angle);
        c = cos(angle);
        *(f32*)((s32)part + 0x10) = -(speed * s) * float_1p5_80422cd0;
        *(f32*)((s32)part + 0x14) = (speed * c) * float_1p5_80422cd0;
        *(f32*)((s32)part + 0x18) = float_0_80422c80;
        *(f32*)((s32)part + 0x1C) = float_0_80422c80;
        *(s32*)((s32)part + 0x20) = 0xFF;
        *(f32*)((s32)part + 0x2C) = float_0p2_80422d04;
        *(f32*)((s32)part + 0x30) = float_0p2_80422d04;
        *(f32*)((s32)part + 0x34) = float_0p2_80422d04;
    }

    return entry;
}

u8 effDamageStarMain(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void effCalcMayaAnim(void* anim);
    extern void effDeleteMayaAnim(void* anim);
    extern void effDelete(void* effect);
    extern f32 intplGetValue(f32 start, f32 end, s32 mode, s32 cur, s32 steps);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effDamageStarDisp(void);
    extern void* gpGlobals;
    extern LocalVec3 vec3_802f3a60;
    extern u8 scale_data[];
    extern f32 float_0_80422c80;
    extern f32 float_1_80422c98;
    extern f32 float_10_80422ca4;
    extern f32 float_0p5_80422cc4;
    extern f32 float_0p01_80422cc8;
    extern f32 float_720_80422ccc;
    extern f32 float_1p5_80422cd0;
    extern f32 float_0p1_80422cd4;
    extern f32 float_0p75_80422cd8;
    LocalVec3 dispPos;
    void* work;
    void* part;
    s32 delta;
    s32 timer;
    s32 i;
    f32 scale;
    f32 velScale = float_0p75_80422cd8;

    work = *(void**)((s32)effEntry + 0xC);
    dispPos = vec3_802f3a60;
    dispPos.x = *(f32*)((s32)work + 0x4);
    dispPos.y = *(f32*)((s32)work + 0x8);
    dispPos.z = *(f32*)((s32)work + 0xC);

    effCalcMayaAnim(*(void**)((s32)work + 0x44));

    delta = 0;
    if (*(u32*)((s32)work + 0x40) < *(u32*)((s32)gpGlobals + 0x1C)) {
        delta = *(u32*)((s32)gpGlobals + 0x1C) - *(u32*)((s32)work + 0x40);
    }
    if (delta < 1) {
        delta = 1;
    }

    while (delta-- > 0) {
        if (*(s32*)((s32)work + 0x24) < 100) {
            *(s32*)((s32)work + 0x24) -= 1;
        }
        *(s32*)((s32)work + 0x28) += 1;

        if (*(s32*)((s32)work + 0x24) < 0) {
            effDeleteMayaAnim(*(void**)((s32)work + 0x44));
            effDelete(effEntry);
            return 0;
        }

        timer = *(s32*)((s32)work + 0x28);
        part = (void*)((s32)work + 0x90);
        for (i = 2; i < *(s32*)((s32)effEntry + 0x8); i++, part = (void*)((s32)part + 0x48)) {
            if (timer < 20) {
                scale = float_0p01_80422cc8 * (f32)scale_data[timer];
                *(f32*)((s32)part + 0x2C) = scale;
                *(f32*)((s32)part + 0x30) = scale;
                *(f32*)((s32)part + 0x34) = scale;
            } else if (timer < 40) {
                *(f32*)((s32)part + 0x1C) = intplGetValue(float_0_80422c80, float_720_80422ccc, 0xB, timer - 20, 19);
            } else if (timer < 50) {
                *(f32*)((s32)part + 0x2C) = intplGetValue(float_1_80422c98, float_1p5_80422cd0, 4, timer - 40, 9);
                *(f32*)((s32)part + 0x30) = intplGetValue(float_1_80422c98, float_0p5_80422cc4, 4, timer - 40, 9);
            } else if (timer < 60) {
                *(f32*)((s32)part + 0x2C) = intplGetValue(float_1p5_80422cd0, float_0p1_80422cd4, 1, timer - 50, 9);
                *(f32*)((s32)part + 0x30) = intplGetValue(float_0p5_80422cc4, float_1_80422c98, 1, timer - 50, 9);
                *(f32*)((s32)part + 0x8) = intplGetValue(*(f32*)((s32)part + 0x8), *(f32*)((s32)part + 0x8) + float_10_80422ca4, 1, timer - 50, 9);
            } else {
                *(f32*)((s32)part + 0x2C) = float_0_80422c80;
                *(f32*)((s32)part + 0x30) = float_0_80422c80;
                *(f32*)((s32)part + 0x34) = float_0_80422c80;
            }

            *(f32*)((s32)part + 0x10) *= velScale;
            *(f32*)((s32)part + 0x14) *= velScale;
            *(f32*)((s32)part + 0x18) *= velScale;
            *(f32*)((s32)part + 0x4) += *(f32*)((s32)part + 0x10);
            *(f32*)((s32)part + 0x8) += *(f32*)((s32)part + 0x14);
            *(f32*)((s32)part + 0xC) += *(f32*)((s32)part + 0x18);
        }
    }

    *(s32*)((s32)work + 0x40) = *(s32*)((s32)gpGlobals + 0x1C);
    dispEntry(4, 8, effDamageStarDisp, effEntry, dispCalcZ(&dispPos));
    return 0;
}

