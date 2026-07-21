#include "effect/eff_laser.h"
#include <dolphin/mtx.h>

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effLaserMain(void* entry);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx m, s8 axis, f32 rad);
void PSMTXConcat(Mtx a, Mtx b, Mtx out);
void* camGetPtr(s32 cameraId);
void animPoseSetMaterialFlagOn(s32 poseId, u32 flags);
void animPoseSetMaterialEvtColor(s32 poseId, void* color);
void animPoseMain(s32 poseId);
void animPoseDrawMtx(s32 poseId, Mtx matrix, f32 arg2, s32 pass, f32 arg4);

extern char str_Laser_8042896c;
extern u32 dat_8042894c;
extern f32 float_deg2rad_80428950;
extern f32 float_0_80428954;
extern f32 float_10_80428958;
extern f32 float_1000_80428974;
extern f32 float_1_80428978;
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effLaserEntry(s32 type, f32 x, f32 y, f32 z) {
    void* entry = effEntry();
    void* work;
    f32 yOffset;
    f32 scale;
    s32 zero;
    s32 minusOne;
    s32 alpha;

    *(char**)((s32)entry + 0x14) = &str_Laser_8042896c;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, (u32)(*(s32*)((s32)entry + 8) * 0x34));
    *(void**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effLaserMain;
    yOffset = float_1000_80428974;
    zero = 0;
    scale = float_1_80428978;
    minusOne = -1;
    alpha = 0xFF;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 0x10) = x;
    *(f32*)((s32)work + 0x14) = y;
    *(f32*)((s32)work + 0x18) = z;
    *(f32*)((s32)work + 4) = *(f32*)((s32)work + 0x10);
    *(f32*)((s32)work + 8) = yOffset + *(f32*)((s32)work + 0x14);
    *(f32*)((s32)work + 0xC) = *(f32*)((s32)work + 0x18);
    *(f32*)((s32)work + 0x1C) = scale;
    *(s32*)((s32)work + 0x24) = zero;
    *(s32*)((s32)work + 0x28) = zero;
    *(s32*)((s32)work + 0x20) = minusOne;
    *(s32*)((s32)work + 0x30) = alpha;
    *(s32*)((s32)work + 0x2C) = zero;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_laser 20260624_184929 */

/* stub-fill: effLaserMain | prototype_only | source_prototype */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effLaserMain(void* entry) {
    extern void* gp;
    extern BOOL animGroupBaseAsync(char* name, s32 language, s32 unused);
    extern s32 animPoseEntry(char* name, s32 language);
    extern void animPoseSetAnim(s32 poseId, char* animName, s32 loop);
    extern void animPoseSetLocalTimeRate(s32 poseId, f32 rate);
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* entry);
    extern f32 intplGetValue(s32 type, s32 current, f32 start, f32 end, s32 max);
    extern BOOL fadeIsFinish(void);
    extern void fadeEntry(s32 type, s32 time, void* color);
    extern f32 dispCalcZ(void* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, void* param, f32 priority);
    extern void effLaserDisp(s32 cameraId, void* entry);
    extern u32 vec3_80302a60[];
    extern char str_MOBJ_EFF_laser_80302a6c[];
    extern char str_S_1_8042895c;
    extern char str_A_1_80428968;
    extern f32 float_0p5_80428960;
    extern f32 float_500_80428964;
    extern u32 dat_80428948;

    void* work;
    u32 dispPos[3];
    u32 pos[3];
    s32 rawLanguage;
    s32 language;
    u32 color;
    f32 addY;

    work = *(void**)((s32)entry + 0xC);
    pos[0] = vec3_80302a60[0];
    pos[1] = vec3_80302a60[1];
    pos[2] = vec3_80302a60[2];
    *(f32*)&pos[0] = *(f32*)((s32)work + 4);
    *(f32*)&pos[1] = *(f32*)((s32)work + 8);
    *(f32*)&pos[2] = *(f32*)((s32)work + 0xC);
    dispPos[0] = pos[0];
    dispPos[1] = pos[1];
    dispPos[2] = pos[2];
    rawLanguage = *(s32*)((s32)gp + 0x14);
    language = (s32)(((u32)(-rawLanguage | rawLanguage)) >> 0x1F);

    if (*(u32*)entry & 4) {
        *(u32*)entry &= ~4;
        if (*(s32*)((s32)work + 0x20) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x20));
        }
        effDelete(entry);
        return;
    }
    if (!animGroupBaseAsync(str_MOBJ_EFF_laser_80302a6c, language, 0)) {
        return;
    }

    switch (*(s32*)((s32)work + 0x2C)) {
        case 0:
            if (*(s32*)((s32)work + 0x20) == -1) {
                *(s32*)((s32)work + 0x20) = animPoseEntry(str_MOBJ_EFF_laser_80302a6c, language);
                animPoseSetAnim(*(s32*)((s32)work + 0x20), &str_S_1_8042895c, 1);
                animPoseSetLocalTimeRate(*(s32*)((s32)work + 0x20), float_0p5_80428960);
            }
            *(s32*)((s32)work + 0x2C) += 1;
            break;
        case 1:
            addY = float_500_80428964;
            *(s32*)((s32)work + 0x28) += 1;
            *(f32*)((s32)work + 8) = intplGetValue(1, *(s32*)((s32)work + 0x28),
                                                   addY + *(f32*)((s32)work + 0x14),
                                                   *(f32*)((s32)work + 0x14), 0x3C);
            if (*(s32*)((s32)work + 0x28) >= 0x3C) {
                animPoseSetAnim(*(s32*)((s32)work + 0x20), &str_A_1_80428968, 1);
                animPoseSetLocalTimeRate(*(s32*)((s32)work + 0x20), float_0p5_80428960);
                *(s32*)((s32)work + 0x2C) += 2;
            }
            break;
        case 2:
            if (fadeIsFinish()) {
                color = dat_80428948;
                fadeEntry(0x10, 0x64, &color);
                animPoseSetAnim(*(s32*)((s32)work + 0x20), &str_A_1_80428968, 1);
                animPoseSetLocalTimeRate(*(s32*)((s32)work + 0x20), float_0p5_80428960);
                *(s32*)((s32)work + 0x2C) += 1;
            }
            break;
        case 3:
            *(s32*)((s32)work + 0x30) -= 1;
            if (*(s32*)((s32)work + 0x30) < 0) {
                animPoseRelease(*(s32*)((s32)work + 0x20));
                effDelete(entry);
                return;
            }
            break;
    }

    dispEntry(4, 2, effLaserDisp, entry, dispCalcZ(dispPos));
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effLaserDisp(s32 cameraId, void* entry) {
    u32 color;
    u32 tempColor;
    Mtx trans;
    Mtx scaleMtx;
    Mtx rot;
    void* work = *(void**)((s32)entry + 0xC);
    s32 poseId = *(s32*)((s32)work + 0x20);
    void* cam;
    f32 scale;
    f32 angle;
    f32 deg;

    if (poseId != -1) {
        PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
        scale = *(f32*)((s32)work + 0x1C);
        PSMTXScale(scaleMtx, scale, scale, scale);
        cam = camGetPtr(cameraId);
        angle = *(f32*)((s32)cam + 0x114);
        angle = -angle;
        deg = float_deg2rad_80428950;
        PSMTXRotRad(rot, 0x79, deg * angle);
        PSMTXConcat(trans, rot, trans);
        PSMTXConcat(trans, scaleMtx, trans);

        animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x20), 0x40);
        tempColor = dat_8042894c;
        ((u8*)&tempColor)[3] = (u8)*(s32*)((s32)work + 0x30);
        color = tempColor;
        animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x20), &color);
        animPoseMain(*(s32*)((s32)work + 0x20));
        animPoseDrawMtx(*(s32*)((s32)work + 0x20), trans, float_0_80428954, 1, float_10_80428958);
        animPoseDrawMtx(*(s32*)((s32)work + 0x20), trans, float_0_80428954, 2, float_10_80428958);
        animPoseDrawMtx(*(s32*)((s32)work + 0x20), trans, float_0_80428954, 3, float_10_80428958);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

