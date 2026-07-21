#include "effect/eff_hibashira.h"

/* CHATGPT STUB FILL: main/effect/eff_hibashira 20260624_184929 */

/* stub-fill: effHibashiraEntry | missing_definition | ghidra_signature */
void* effHibashiraEntry(
    f32 x,
    f32 y,
    f32 z,
    f32 targetX,
    f32 targetY,
    f32 targetZ,
    f32 scale,
    s32 type,
    s32 countArg,
    s32 timer
) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern double sin(double x);
    extern void effHibashiraMain(void* entry);
    extern char str_hibashira_80302a44[];
    extern double double_to_int_80302a50;
    extern f32 float_3p1416_804288f8;
    extern f32 float_0p5_804288fc;

    void* entry;
    void* work;
    void* child;
    s32 count;
    s32 i;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 denom;
    f32 prev;
    f32 cur;

    entry = effEntry();
    count = 1;
    if (type < 3) {
        if (type >= 0) {
            count = 2;
        }
    } else if (type < 6) {
        count = countArg + 1;
    }

    *(char**)((s32)entry + 0x14) = str_hibashira_80302a44;
    *(s32*)((s32)entry + 8) = count;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) * 0x28));
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effHibashiraMain;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = scale;
    *(s32*)((s32)work + 0x14) = -1;
    *(s32*)((s32)work + 0x18) = 0xFF;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x1C) = 1000;
    } else {
        *(s32*)((s32)work + 0x1C) = timer;
    }
    *(s32*)((s32)work + 0x20) = 0;

    dx = targetX - x;
    dy = targetY - y;
    dz = targetZ - z;
    child = (void*)((s32)work + 0x28);
    i = 1;
    while (i < *(s32*)((s32)entry + 8)) {
        denom = (f32)(*(s32*)((s32)entry + 8) - 1);
        prev = (f32)(i - 1);
        cur = (f32)i;
        *(f32*)((s32)child + 4) = prev * (dx / denom);
        *(f32*)((s32)child + 8) = prev * (dy / denom);
        *(f32*)((s32)child + 0xC) = prev * (dz / denom);
        *(f32*)((s32)child + 0x10) = float_0p5_804288fc + (f32)sin((float_3p1416_804288f8 * cur) / denom);
        *(s32*)((s32)child + 0x14) = -1;
        *(s32*)((s32)child + 0x18) = 0xFF;
        *(s32*)((s32)child + 0x1C) = 0;
        *(s32*)((s32)child + 0x20) = 0;
        *(s32*)((s32)child + 0x24) = (i - 1) << 2;
        i++;
        child = (void*)((s32)child + 0x28);
    }

    return entry;
}

/* CHATGPT FALLBACK MISSING STUBS: main/effect/eff_hibashira 20260624_191429 */

/* fallback stub-fill: map=effHibashiraMain addr=0x80256ff4 size=0x000002c4 */
void effHibashiraMain(void* entry) {
    extern void* gp;
    extern BOOL animGroupBaseAsync(char* name, s32 language, s32 unused);
    extern s32 animPoseEntry(char* name, s32 language);
    extern void animPoseSetAnim(s32 poseId, char* animName, s32 loop);
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, void* param, f32 z);
    extern void effHibashiraDisp(s32 cameraId, void* entry);
    extern u32 vec3_80302a18[];
    extern char str_EFF_hibashira_80302a24[];
    extern char str_EFF_hibashira2_80302a34[];
    extern char str_A_1_804288f0[];
    extern char str_A_2_804288f4[];

    void* work;
    void* child;
    u32 pos[3];
    u32 dispPos[3];
    s32 type;
    s32 languageRaw;
    s32 language;
    s32 i;
    s32 pose;
    char* group;
    char* anim;

    work = *(void**)((s32)entry + 0xC);
    pos[0] = vec3_80302a18[0];
    pos[1] = vec3_80302a18[1];
    pos[2] = vec3_80302a18[2];
    *(f32*)&pos[0] = *(f32*)((s32)work + 4);
    *(f32*)&pos[1] = *(f32*)((s32)work + 8);
    *(f32*)&pos[2] = *(f32*)((s32)work + 0xC);
    dispPos[0] = pos[0];
    dispPos[1] = pos[1];
    dispPos[2] = pos[2];

    type = *(s32*)work;
    languageRaw = *(s32*)((s32)gp + 0x14);
    language = (s32)(((u32)(-languageRaw | languageRaw)) >> 31);

    if (*(u32*)entry & 4) {
        *(u32*)entry &= ~4;
        *(s32*)((s32)work + 0x1C) = 0x10;
    }

    if (type == 2 || type == 5) {
        if (!animGroupBaseAsync(str_EFF_hibashira2_80302a34, language, 0)) {
            return;
        }
    } else if (type >= 0 && type < 5) {
        if (!animGroupBaseAsync(str_EFF_hibashira_80302a24, language, 0)) {
            return;
        }
    }

    if (*(s32*)((s32)work + 0x1C) < 1000) {
        *(s32*)((s32)work + 0x1C) -= 1;
    }
    if (*(s32*)((s32)work + 0x1C) < 0x10) {
        *(s32*)((s32)work + 0x18) = *(s32*)((s32)work + 0x1C) << 4;
    }
    if (*(s32*)((s32)work + 0x1C) < 0) {
        child = work;
        i = 0;
        while (i < *(s32*)((s32)entry + 8)) {
            pose = *(s32*)((s32)child + 0x14);
            if (pose != -1) {
                animPoseRelease(pose);
            }
            child = (void*)((s32)child + 0x28);
            i++;
        }
        effDelete(entry);
        return;
    }

    child = (void*)((s32)work + 0x28);
    i = 1;
    while (i < *(s32*)((s32)entry + 8)) {
        if (*(s32*)((s32)child + 0x24) != 0) {
            *(s32*)((s32)child + 0x24) -= 1;
        } else {
            if (*(s32*)((s32)child + 0x14) == -1) {
                group = str_EFF_hibashira_80302a24;
                anim = str_A_1_804288f0;
                if (type == 1 || type == 4) {
                    anim = str_A_2_804288f4;
                } else if (type == 2 || type == 5) {
                    group = str_EFF_hibashira2_80302a34;
                }
                *(s32*)((s32)child + 0x14) = animPoseEntry(group, language);
                animPoseSetAnim(*(s32*)((s32)child + 0x14), anim, 1);
            }
            *(s32*)((s32)child + 0x20) += 1;
            if (*(s32*)((s32)child + 0x20) > 1000) {
                *(s32*)((s32)child + 0x20) = 1000;
            }
            if (*(s32*)((s32)child + 0x20) > 5) {
                *(s32*)((s32)child + 0x18) -= 10;
            }
            if (*(s32*)((s32)child + 0x18) < 0) {
                *(s32*)((s32)child + 0x18) = 0;
            }
        }
        child = (void*)((s32)child + 0x28);
        i++;
    }

    dispEntry(4, 2, effHibashiraDisp, entry, dispCalcZ(dispPos));
}


/* CHATGPT STUB FILL: main/effect/eff_hibashira 20260624_184929 */

/* stub-fill: effHibashiraDisp | missing_definition | ghidra_signature */
void effHibashiraDisp(s32 cameraId, void* entry) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx m, s8 axis, f32 rad);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void* camGetPtr(s32 cameraId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, Mtx matrix, f32 arg2, s32 pass, f32 arg4);
    extern u32 dat_804288e0;
    extern f32 float_deg2rad_804288e4;
    extern f32 float_0_804288e8;
    extern f32 float_10_804288ec;

    u32 tempColor;
    u32 color;
    Mtx baseTrans;
    Mtx baseScale;
    Mtx rot;
    Mtx baseMtx;
    void* work;
    void* child;
    void* cam;
    f32 scale;
    f32 angle;
    f32 deg;
    s32 i;
    s32 pose;
    s32 baseAlpha;
    s32 alpha;

    work = *(void**)((s32)entry + 0xC);
    baseAlpha = *(s32*)((s32)work + 0x18);
    PSMTXTrans(baseTrans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
    scale = *(f32*)((s32)work + 0x10);
    PSMTXScale(baseScale, scale, scale, scale);
    cam = camGetPtr(cameraId);
    angle = *(f32*)((s32)cam + 0x114);
    angle = -angle;
    deg = float_deg2rad_804288e4;
    PSMTXRotRad(rot, 0x79, deg * angle);
    PSMTXConcat(baseTrans, rot, baseTrans);
    PSMTXConcat(baseTrans, baseScale, baseMtx);

    i = 1;
    child = (void*)((s32)work + 0x28);
    while (i < *(s32*)((s32)entry + 8)) {
        pose = *(s32*)((s32)child + 0x14);
        if (pose != -1) {
            PSMTXTrans(baseTrans, *(f32*)((s32)child + 4), *(f32*)((s32)child + 8), *(f32*)((s32)child + 0xC));
            scale = *(f32*)((s32)child + 0x10);
            PSMTXScale(baseScale, scale, scale, scale);
            PSMTXConcat(baseTrans, baseScale, baseTrans);
            PSMTXConcat(baseMtx, baseTrans, baseTrans);
            animPoseSetMaterialFlagOn(pose, 0x40);
            alpha = (*(s32*)((s32)child + 0x18) * baseAlpha) / 0xFF;
            tempColor = dat_804288e0;
            ((u8*)&tempColor)[3] = (u8)alpha;
            color = tempColor;
            animPoseSetMaterialEvtColor(pose, &color);
            animPoseMain(pose);
            animPoseDrawMtx(pose, baseTrans, float_0_804288e8, 1, float_10_804288ec);
            animPoseDrawMtx(pose, baseTrans, float_0_804288e8, 2, float_10_804288ec);
            animPoseDrawMtx(pose, baseTrans, float_0_804288e8, 3, float_10_804288ec);
        }
        i++;
        child = (void*)((s32)child + 0x28);
    }
}

