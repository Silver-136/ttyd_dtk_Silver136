#include "effect/eff_nice.h"


u8 acrobatMain(int* param_1) {
    return 0;
}


u8 effAcrobatDisp(void) {
    return 0;
}


u8 effNiceMain(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern void* animPoseGetAnimPosePtr(s32 poseId);
    extern void* animPoseGetAnimDataPtr(s32 poseId);
    extern void animPoseMain(s32 poseId);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern f32 animPoseGetLoopTimes(s32 poseId);
    extern void animPoseRelease(s32 poseId);
    extern void effDelete(void* effect);
    extern u8 acrobatMain(void* effEntry);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effNiceDisp(s32 cameraId, s32 entry);
    extern void* gpGlobals;
    extern u32 dat_80422e80;
    extern f32 float_0_80422e94;
    extern f32 float_1_80422e9c;
    extern f32 float_255_80422ec0;
    LocalVec3 pos;
    void* work;
    void* pose;
    void* animData;
    f32 endFrame;
    f32 fade;
    u32 color;
    s32 kind;
    s32 poseId;

    work = *(void**)((s32)effEntry + 0xC);
    kind = *(s32*)((s32)work + 0x0);
    pos.x = *(f32*)((s32)work + 0x4);
    pos.y = *(f32*)((s32)work + 0x8);
    pos.z = *(f32*)((s32)work + 0xC);
    poseId = *(s32*)((s32)work + 0x30);

    if ((*(u32*)((s32)effEntry + 0x0) & 4) != 0) {
        *(u32*)((s32)effEntry + 0x0) &= ~4;
        if (poseId != -1) {
            animPoseRelease(poseId);
        }
        effDelete(effEntry);
        return 0;
    }

    if (poseId != -1) {
        pose = animPoseGetAnimPosePtr(poseId);
        animData = animPoseGetAnimDataPtr(poseId);
        animPoseMain(poseId);
        if (kind >= 0 && kind < 7) {
            endFrame = *(f32*)(*(s32*)((s32)animData + 0x24) + 0x8);
            if (kind == *(s32*)((s32)work + 0x0)) {
                fade = endFrame - 5.0f;
                if (fade < *(f32*)((s32)pose + 0x34)) {
                    fade = float_1_80422e9c - ((*(f32*)((s32)pose + 0x34) - fade) / (endFrame - fade));
                    if (fade < float_0_80422e94) {
                        fade = float_0_80422e94;
                    }
                    animPoseSetMaterialFlagOn(poseId, 0x40);
                    color = (dat_80422e80 & 0xFFFFFF00) | ((s32)(float_255_80422ec0 * fade) & 0xFF);
                    animPoseSetMaterialEvtColor(poseId, &color);
                }
            }
        }
        if (animPoseGetLoopTimes(poseId) >= float_1_80422e9c) {
            animPoseRelease(poseId);
            effDelete(effEntry);
            return 0;
        }
    }

    if (kind == 7) {
        acrobatMain(effEntry);
    } else {
        dispEntry(4, 8, effNiceDisp, effEntry, dispCalcZ(&pos));
    }
    return 0;
}

void* effNiceEntry(f64 x, f64 y, f64 z, s32 kind) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern s32 animPoseEntry(char* name, s32 battle);
    extern void animPoseSetAnim(s32 poseId, char* anim, s32 force);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetGXFunc(s32 poseId, void* func, s32 arg);
    extern u8 effNiceMain(void* effEntry);
    extern s32 rendermodeFunc(s32 param);
    extern void* gpGlobals;
    extern char str_Nice_80422ec4[];
    extern char str_A_1_80422ed0[];
    extern char** anim_tbl[];
    extern f32 float_0_80422e94;
    extern f32 float_1_80422e9c;
    extern f32 float_1000_80422ecc;
    void* entry;
    void* work;
    s32 count;
    s32 poseId;
    s32 i;
    s32 language;

    entry = effEntry();
    if (kind == 5) {
        kind = 0;
    }
    language = *(s32*)((s32)gpGlobals + 0x128);
    count = 1;
    if (kind == 7) {
        count = 2;
    }

    *(char**)((s32)entry + 0x14) = str_Nice_80422ec4;
    *(s32*)((s32)entry + 0x8) = count;
    work = __memAlloc(3, count * 100);
    *(void**)((s32)entry + 0xC) = work;
    memset(work, 0, count * 100);
    *(void**)((s32)entry + 0x10) = effNiceMain;
    *(u32*)((s32)entry + 0x0) |= 2;

    *(s32*)((s32)work + 0x0) = kind;
    *(f32*)((s32)work + 0x4) = (f32)x;
    *(f32*)((s32)work + 0x8) = (f32)y;
    *(f32*)((s32)work + 0xC) = (f32)z;
    *(f32*)((s32)work + 0x10) = float_1_80422e9c;
    *(f32*)((s32)work + 0x14) = float_1_80422e9c;
    *(f32*)((s32)work + 0x18) = float_1_80422e9c;
    *(f32*)((s32)work + 0x1C) = float_1_80422e9c;
    *(s32*)((s32)work + 0x30) = -1;

    if (kind == 7) {
        for (i = 1, work = (void*)((s32)work + 100); i < count; i++, work = (void*)((s32)work + 100)) {
            *(f32*)((s32)work + 0x4) = float_1000_80422ecc;
            *(f32*)((s32)work + 0x8) = float_0_80422e94;
            *(f32*)((s32)work + 0xC) = float_0_80422e94;
            *(f32*)((s32)work + 0x10) = float_1_80422e9c;
            *(f32*)((s32)work + 0x14) = float_1_80422e9c;
            *(f32*)((s32)work + 0x18) = float_1_80422e9c;
            *(s32*)((s32)work + 0x2C) = 0;
            *(s32*)((s32)work + 0x24) = 0;
        }
    } else {
        poseId = animPoseEntry(anim_tbl[language][kind], (*(s32*)((s32)gpGlobals + 0x14) != 0));
        *(s32*)((s32)*(void**)((s32)entry + 0xC) + 0x30) = poseId;
        animPoseSetAnim(poseId, str_A_1_80422ed0, 1);
        animPoseSetMaterialFlagOn(poseId, 0x1800);
        animPoseSetGXFunc(poseId, rendermodeFunc, 0);
    }

    return entry;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effNiceDisp(int cameraId, int entry) {
    typedef f32 Mtx[3][4];
    extern void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXRotRad(Mtx mtx, s32 axis, f32 rad);
    extern void PSMTXScale(Mtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(Mtx a, Mtx b, Mtx out);
    extern void* camGetPtr(s32 cameraId);
    extern void* gp;
    extern f32 float_deg2rad_80422e88;
    extern f32 float_8_80422eb8;
    extern f32 float_0p7_80422ebc;
    extern f32 float_0_80422e94;
    extern f32 float_1_80422e9c;
    extern u8 animPoseDrawMtx();
    Mtx trans;
    Mtx rot;
    Mtx scale;
    Mtx mtx;
    void* work = *(void**)((s32)entry + 0xC);
    u32 renderMode;

    PSMTXTrans(trans,
               *(f32*)((s32)work + 4),
               *(f32*)((s32)work + 8),
               *(f32*)((s32)work + 0xC));
    PSMTXRotRad(rot, 0x79, -*(f32*)((s32)camGetPtr(cameraId) + 0x114) * float_deg2rad_80422e88);
    PSMTXScale(scale,
               *(f32*)((s32)work + 0x1C),
               *(f32*)((s32)work + 0x1C),
               *(f32*)((s32)work + 0x1C));
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat(mtx, scale, mtx);
    PSMTXScale(scale, float_8_80422eb8, float_8_80422eb8, float_8_80422eb8);
    PSMTXConcat(mtx, scale, mtx);

    renderMode = *(s32*)((s32)gp + 0x16C);
    if ((renderMode == 1 && *(s32*)work == 3) ||
        (renderMode == 1 && *(s32*)work == 4) ||
        (renderMode == 5 && *(s32*)work == 6)) {
        PSMTXScale(scale, float_0p7_80422ebc, float_0p7_80422ebc, float_0p7_80422ebc);
        PSMTXConcat(mtx, scale, mtx);
    }

    animPoseDrawMtx(*(s32*)((s32)work + 0x30), mtx, 1, float_0_80422e94, float_1_80422e9c);
    animPoseDrawMtx(*(s32*)((s32)work + 0x30), mtx, 2, float_0_80422e94, float_1_80422e9c);
    return animPoseDrawMtx(*(s32*)((s32)work + 0x30), mtx, 3, float_0_80422e94, float_1_80422e9c);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void rendermodeFunc(s32 param) {
    extern void* animGetPtr(s32 id);
    extern void GXSetBlendMode(s32 type, s32 src, s32 dst, s32 op);
    extern void GXSetZCompLoc(s32 beforeTex);
    extern void GXSetAlphaCompare(s32 comp0, s32 ref0, s32 op, s32 comp1, s32 ref1);
    extern void GXSetZMode(s32 enable, s32 func, s32 update);
    void* anim;

    anim = animGetPtr(param);
    GXSetBlendMode(1, 4, 5, 0);
    GXSetZCompLoc(1);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZMode(0, 3, 0);
    *(s32*)((s32)anim + 0xE0) = param;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 effNiceAsync(char* name) {
    extern void* gp;
    extern char** anim_tbl[];
    extern s32 animGroupBaseAsync(char* animName, char* groupName, s32 unused);
    s32 result;
    s32 i;
    s32 offset;
    char*** tbl;
    char* animName;

    result = 1;
    i = 0;
    offset = 0;
    tbl = anim_tbl;
    do {
        animName = tbl[*(s32*)((s32)gp + 0x16C)][offset / 4];
        if (animName != 0) {
            if (animGroupBaseAsync(animName, name, 0) == 0) {
                result = 0;
            }
        }
        i++;
        offset += 4;
    } while (i < 7);
    return result;
}

