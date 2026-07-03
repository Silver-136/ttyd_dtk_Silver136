#include "effect/eff_nokotarou.h"

void* effEntry(void);
void* __memAlloc(s32 heap, u32 size);
void effNokotarouMain(void* entry);

extern char str_Nokotarou_80302b18[];
extern f32 float_0_80428a34;

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effNokotarouEntry(s32 type, s32 timer, f32 x, f32 y, f32 z, f32 field1C) {
    void* entry = effEntry();
    void* work;
    f32 zero;
    s32 alpha;

    *(char**)((s32)entry + 0x14) = str_Nokotarou_80302b18;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x38);
    *(void**)((s32)entry + 0xC) = work;
    zero = float_0_80428a34;
    *(void**)((s32)entry + 0x10) = effNokotarouMain;
    alpha = 0xFF;
    *(u32*)entry |= 2;

    *(s32*)((s32)work + 0) = type;
    *(f32*)((s32)work + 4) = x;
    *(f32*)((s32)work + 8) = y;
    *(f32*)((s32)work + 0xC) = z;
    *(f32*)((s32)work + 0x10) = *(f32*)((s32)work + 4);
    *(f32*)((s32)work + 0x14) = *(f32*)((s32)work + 8);
    *(f32*)((s32)work + 0x18) = *(f32*)((s32)work + 0xC);
    *(f32*)((s32)work + 0x1C) = field1C;
    *(f32*)((s32)work + 0x20) = zero;
    *(s32*)((s32)work + 0x24) = alpha;
    if (timer <= 0) {
        *(s32*)((s32)work + 0x28) = 1000;
    } else {
        *(s32*)((s32)work + 0x28) = timer;
    }
    *(s32*)((s32)work + 0x30) = -1;
    *(s32*)((s32)work + 0x34) = -1;

    return entry;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


/* CHATGPT STUB FILL: main/effect/eff_nokotarou 20260624_185035 */

/* stub-fill: effNokotarouDisp | missing_definition | ghidra_signature */
u8 effNokotarouDisp(void) {
    return 0;
}

/* stub-fill: effNokotarouMain | prototype_only | source_prototype */
void effNokotarouMain(void* entry) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* gp;
    extern BOOL animGroupBaseAsync(char* name, s32 language, s32 unused);
    extern s32 animPoseEntry(char* name, s32 language);
    extern void animPoseSetAnim(s32 poseId, const char* animName, s32 loop);
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* entry);
    extern f32 dispCalcZ(Vec* pos);
    extern void dispEntry(s32 cameraId, s32 order, void* callback, f32 priority, void* param);
    extern void effNokotarouDisp(s32 cameraId, void* entry);
    extern char str_MOBJ_EFF_nokotarou3_80302b04[];
    extern const char str_S_1_80428a44[];
    extern const char str_A_1_80428a48[];
    extern f32 float_1_80428a40;
    extern f32 float_0p03125_80428a4c;

    void* work;
    Vec pos;
    s32 language;

    work = *(void**)((s32)entry + 0xC);
    pos.x = *(f32*)((s32)work + 0x10);
    pos.y = *(f32*)((s32)work + 0x14);
    pos.z = *(f32*)((s32)work + 0x18);
    language = *(s32*)((s32)gp + 0x14) != 0;

    if (!animGroupBaseAsync(str_MOBJ_EFF_nokotarou3_80302b04, language, 0)) {
        return;
    }
    if (*(s32*)((s32)work + 0x30) == -1) {
        *(s32*)((s32)work + 0x30) = animPoseEntry(str_MOBJ_EFF_nokotarou3_80302b04, language);
        animPoseSetAnim(*(s32*)((s32)work + 0x30), str_S_1_80428a44, 1);
        *(s32*)((s32)work + 0x34) = animPoseEntry(str_MOBJ_EFF_nokotarou3_80302b04, language);
        animPoseSetAnim(*(s32*)((s32)work + 0x34), str_A_1_80428a48, 1);
    }
    if (*(u32*)entry & 4) {
        *(u32*)entry &= ~4;
        *(s32*)((s32)work + 0x28) = 0x20;
    }
    if (*(s32*)((s32)work + 0x28) < 1000) {
        *(s32*)((s32)work + 0x28) -= 1;
    }
    if (*(s32*)((s32)work + 0x28) < 0) {
        if (*(s32*)((s32)work + 0x30) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x30));
        }
        if (*(s32*)((s32)work + 0x34) != -1) {
            animPoseRelease(*(s32*)((s32)work + 0x34));
        }
        effDelete(entry);
        return;
    }
    if (*(s32*)((s32)work + 0x28) < 0x20) {
        *(s32*)((s32)work + 0x24) = *(s32*)((s32)work + 0x28) << 3;
        *(f32*)((s32)work + 0x20) += -*(f32*)((s32)work + 0x20) * float_0p03125_80428a4c;
    } else {
        *(f32*)((s32)work + 0x20) += (float_1_80428a40 - *(f32*)((s32)work + 0x20)) * float_0p03125_80428a4c;
    }
    dispCalcZ(&pos);
    dispEntry(4, 2, effNokotarouDisp, 0.0f, entry);
}

