#include "effect/eff_mugi.h"

void* effMugiKemuriEntry(f64 x, f64 y, f64 z, f64 angle) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern f32 reviseAngle(f32 angle);
    extern u8 effMugiKemuriMain(void* effEntry);
    extern char str_MugiKemuri_802f3b10[];
    extern f32 float_0_80422d64;
    extern f32 float_0p7_80422d84;
    extern f32 float_6p2832_80422d98;
    extern f32 float_360_80422d9c;
    extern f32 float_0p3_80422da0;
    extern f32 float_1p75_80422da4;
    extern f32 float_neg0p08_80422da8;
    extern f32 float_neg3p9_80422dac;
    extern double sin(double x);
    extern double cos(double x);
    void* entry;
    void* work;
    s32 i;
    f32 curAngle;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_MugiKemuri_802f3b10;
    *(s32*)((s32)entry + 0x8) = 5;
    work = __memAlloc(3, 500);
    *(void**)((s32)entry + 0xC) = work;
    memset(work, 0, 500);
    *(void**)((s32)entry + 0x10) = effMugiKemuriMain;
    *(u32*)((s32)entry + 0x0) |= 2;

    for (i = 0; i < 5; i++, work = (void*)((s32)work + 100)) {
        *(s32*)((s32)work + 0x0) = 1;
        *(f32*)((s32)work + 0x30) = float_0p3_80422da0;
        *(f32*)((s32)work + 0x34) = float_0_80422d64;
        *(f32*)((s32)work + 0x38) = float_0_80422d64;
        *(f32*)((s32)work + 0x8) = (f32)x;
        *(f32*)((s32)work + 0xC) = (f32)y;
        *(f32*)((s32)work + 0x10) = (f32)z;
        *(f32*)((s32)work + 0x20) = float_0p7_80422d84;
        *(f32*)((s32)work + 0x24) = float_0p7_80422d84;
        *(f32*)((s32)work + 0x28) = float_0p7_80422d84;
        *(s32*)((s32)work + 0x58) = 0xFF;
        *(s32*)((s32)work + 0x54) = 0x3C;
        *(f32*)((s32)work + 0x14) = float_0_80422d64;
        curAngle = (f32)(angle + (f64)(i * 72));
        *(f32*)((s32)work + 0x18) = curAngle;
        *(f32*)((s32)work + 0x1C) = float_0_80422d64;
        *(f32*)((s32)work + 0x3C) = float_1p75_80422da4;
        *(f32*)((s32)work + 0x40) = float_neg0p08_80422da8;
        *(f32*)((s32)work + 0x44) = float_0_80422d64;
        *(f32*)((s32)work + 0x48) = float_0_80422d64;
        *(f32*)((s32)work + 0x2C) = float_neg3p9_80422dac;
        rad = (float_6p2832_80422d98 * reviseAngle(curAngle)) / float_360_80422d9c;
        *(f32*)((s32)work + 0x4C) = (f32)sin(rad);
        *(f32*)((s32)work + 0x50) = (f32)cos(rad);
        *(s32*)((s32)work + 0x60) = -1;
    }

    return entry;
}


void* effMugiKemuri2Entry(f64 x, f64 y, f64 z, f64 angle, f64 dir, s32 arg5) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, u32 size);
    extern void* memset(void* dst, s32 value, u32 size);
    extern s32 rand(void);
    extern f32 reviseAngle(f32 angle);
    extern u8 effMugiKemuri2Main(void* effEntry);
    extern char str_MugiKemuri2_802f3af8[];
    extern f32 float_0_80422d64;
    extern f32 float_1_80422d7c;
    extern f32 float_0p7_80422d84;
    extern f32 float_5p4_80422d88;
    extern f32 float_0p5_80422d8c;
    extern f32 float_neg0p07_80422d90;
    extern f32 float_0p003_80422d94;
    extern f32 float_6p2832_80422d98;
    extern f32 float_360_80422d9c;
    extern double sin(double x);
    extern double cos(double x);
    void* entry;
    void* work;
    s32 i;
    s32 r;
    s32 baseAngle;
    f32 rad;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_MugiKemuri2_802f3af8;
    *(s32*)((s32)entry + 0x8) = 5;
    work = __memAlloc(3, 500);
    *(void**)((s32)entry + 0xC) = work;
    memset(work, 0, 500);
    *(void**)((s32)entry + 0x10) = effMugiKemuri2Main;
    *(u32*)((s32)entry + 0x0) |= 2;

    for (i = 0; i < 5; i++, work = (void*)((s32)work + 100)) {
        *(s32*)((s32)work + 0x0) = 1;
        *(s32*)((s32)work + 0x4) = arg5;
        *(f32*)((s32)work + 0x38) = float_0_80422d64;
        *(f32*)((s32)work + 0x8) = (f32)x;
        *(f32*)((s32)work + 0xC) = (f32)y;
        *(f32*)((s32)work + 0x10) = (f32)z;
        *(f32*)((s32)work + 0x20) = float_0p7_80422d84;
        *(f32*)((s32)work + 0x24) = float_0p7_80422d84;
        *(f32*)((s32)work + 0x28) = float_0p7_80422d84;
        *(s32*)((s32)work + 0x58) = 0xFF;
        *(f32*)((s32)work + 0x34) = float_1_80422d7c;
        *(f32*)((s32)work + 0x30) = float_5p4_80422d88;
        if (dir == (f64)float_0_80422d64) {
            *(f32*)((s32)work + 0x2C) = 10.0f;
            baseAngle = 90;
        } else {
            *(f32*)((s32)work + 0x2C) = -10.0f;
            baseAngle = -90;
        }
        *(f32*)((s32)work + 0x3C) = float_0p5_80422d8c;
        *(f32*)((s32)work + 0x40) = float_neg0p07_80422d90;
        *(f32*)((s32)work + 0x44) = float_0p003_80422d94;
        *(f32*)((s32)work + 0x48) = float_0_80422d64;
        *(s32*)((s32)work + 0x54) = 0xF;
        *(f32*)((s32)work + 0x18) = (f32)angle;
        *(f32*)((s32)work + 0x14) = float_0_80422d64;
        *(f32*)((s32)work + 0x1C) = float_0_80422d64;
        r = rand();
        *(f32*)((s32)work + 0x5C) = (f32)(((r % 100) >> 4) & 1);
        rad = (float_6p2832_80422d98 * reviseAngle((f32)(angle + (f64)baseAngle))) / float_360_80422d9c;
        *(f32*)((s32)work + 0x4C) = (f32)sin(rad);
        *(f32*)((s32)work + 0x50) = (f32)cos(rad);
        *(s32*)((s32)work + 0x60) = -1;
    }

    return entry;
}

u8 effMugiKemuriMain(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern s32 strncmp(char* s1, char* s2, u32 n);
    extern u32 animGroupBaseAsync(char* name, s32 battle, s32 unused);
    extern s32 animPoseEntry(char* name, s32 battle);
    extern void animPoseSetAnim(s32 pose, char* anim, s32 force);
    extern void animPoseRelease(s32 pose);
    extern void effDelete(void* effect);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effMugiKemuriDisp(s32 camera, void* effEntry);
    extern void* gpGlobals;
    extern LocalVec3 vec3_802f3abc;
    extern char str_win_80422d6c[];
    extern char str_Z_1_80422d70[];
    extern char str_EFF_Hana_802f3ad4[];
    extern char str_EFF_Mugi_802f3ae0[];
    extern f32 float_0_80422d64;
    extern f32 float_0p85_80422d74;
    extern f32 float_0p004_80422d78;
    extern f32 float_1_80422d7c;
    extern f32 float_neg100_80422d80;
    LocalVec3 pos;
    void* work;
    void* cur;
    char* name;
    s32 battle;
    s32 i;
    s32 alive;

    work = *(void**)((s32)effEntry + 0xC);
    pos = vec3_802f3abc;
    pos.x = *(f32*)((s32)work + 0x8);
    pos.y = *(f32*)((s32)work + 0xC);
    pos.z = *(f32*)((s32)work + 0x10);
    battle = (*(s32*)((s32)gpGlobals + 0x14) != 0);

    if ((*(u32*)((s32)effEntry + 0x0) & 4) != 0) {
        *(u32*)((s32)effEntry + 0x0) &= ~4;
        cur = work;
        for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, cur = (void*)((s32)cur + 100)) {
            if (*(s32*)((s32)cur + 0x60) != -1) {
                animPoseRelease(*(s32*)((s32)cur + 0x60));
            }
        }
        effDelete(effEntry);
        return 0;
    }

    cur = work;
    for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, cur = (void*)((s32)cur + 100)) {
        if (*(s32*)((s32)cur + 0x60) == -1) {
            name = str_EFF_Mugi_802f3ae0;
            if (strncmp((char*)((s32)gpGlobals + 0xA4), str_win_80422d6c, 3) == 0) {
                name = str_EFF_Hana_802f3ad4;
            }
            if (animGroupBaseAsync(name, battle, 0) == 0) {
                return 0;
            }
            *(s32*)((s32)cur + 0x60) = animPoseEntry(name, battle);
            animPoseSetAnim(*(s32*)((s32)cur + 0x60), str_Z_1_80422d70, 1);
        }
    }

    alive = 0;
    cur = work;
    for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, cur = (void*)((s32)cur + 100)) {
        if (*(s32*)((s32)cur + 0x0) != 0) {
            *(s32*)((s32)cur + 0x54) -= 1;
            if (*(s32*)((s32)cur + 0x54) < 1) {
                *(s32*)((s32)cur + 0x0) = 0;
            } else {
                alive = 1;
                *(f32*)((s32)cur + 0x2C) *= float_0p85_80422d74;
                *(f32*)((s32)cur + 0x8) += *(f32*)((s32)cur + 0x2C) * *(f32*)((s32)cur + 0x4C);
                *(f32*)((s32)cur + 0x10) += *(f32*)((s32)cur + 0x2C) * *(f32*)((s32)cur + 0x50);
                *(f32*)((s32)cur + 0x44) += *(f32*)((s32)cur + 0x48);
                *(f32*)((s32)cur + 0x40) += *(f32*)((s32)cur + 0x44);
                *(f32*)((s32)cur + 0x3C) += *(f32*)((s32)cur + 0x40);
                *(f32*)((s32)cur + 0xC) += *(f32*)((s32)cur + 0x3C);
                if (*(f32*)((s32)cur + 0x3C) < float_0_80422d64) {
                    *(f32*)((s32)cur + 0x44) = float_0p004_80422d78;
                }
                *(f32*)((s32)cur + 0x14) -= float_1_80422d7c;
                if (*(f32*)((s32)cur + 0x14) < float_neg100_80422d80) {
                    *(f32*)((s32)cur + 0x14) = float_neg100_80422d80;
                }
            }
        }
    }

    if (alive != 0) {
        dispEntry(4, 2, effMugiKemuriDisp, effEntry, dispCalcZ(&pos));
    } else {
        cur = work;
        for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, cur = (void*)((s32)cur + 100)) {
            if (*(s32*)((s32)cur + 0x60) != -1) {
                animPoseRelease(*(s32*)((s32)cur + 0x60));
            }
        }
        effDelete(effEntry);
    }
    return 0;
}
u8 effMugiKemuri2Main(void* effEntry) {
    typedef struct LocalVec3 {
        f32 x;
        f32 y;
        f32 z;
    } LocalVec3;
    extern s32 strncmp(char* s1, char* s2, u32 n);
    extern u32 animGroupBaseAsync(char* name, s32 battle, s32 unused);
    extern s32 animPoseEntry(char* name, s32 battle);
    extern void animPoseSetAnim(s32 pose, char* anim, s32 force);
    extern void animPoseRelease(s32 pose);
    extern void effDelete(void* effect);
    extern void* marioGetPtr(void);
    extern f32 dispCalcZ(LocalVec3* pos);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern u8 effMugiKemuri2Disp(s32 camera, void* effEntry);
    extern void* gpGlobals;
    extern LocalVec3 vec3_802f3ab0;
    extern char str_win_80422d6c[];
    extern char str_Z_1_80422d70[];
    extern char str_EFF_Hana_802f3ad4[];
    extern char str_EFF_Mugi_802f3ae0[];
    extern f32 float_0_80422d64;
    LocalVec3 pos;
    void* work;
    void* cur;
    char* name;
    s32 battle;
    s32 i;
    s32 alive;
    s32 fading;

    work = *(void**)((s32)effEntry + 0xC);
    pos = vec3_802f3ab0;
    pos.x = *(f32*)((s32)work + 0x8);
    pos.y = *(f32*)((s32)work + 0xC);
    pos.z = *(f32*)((s32)work + 0x10);
    battle = (*(s32*)((s32)gpGlobals + 0x14) != 0);

    if ((*(u32*)((s32)effEntry + 0x0) & 4) != 0) {
        *(u32*)((s32)effEntry + 0x0) &= ~4;
        cur = work;
        for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, cur = (void*)((s32)cur + 100)) {
            if (*(s32*)((s32)cur + 0x60) != -1) {
                animPoseRelease(*(s32*)((s32)cur + 0x60));
            }
        }
        effDelete(effEntry);
        return 0;
    }

    cur = work;
    for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, cur = (void*)((s32)cur + 100)) {
        if (*(s32*)((s32)cur + 0x60) == -1) {
            name = str_EFF_Mugi_802f3ae0;
            if (strncmp((char*)((s32)gpGlobals + 0x12C), str_win_80422d6c, 3) == 0) {
                name = str_EFF_Hana_802f3ad4;
            }
            if (animGroupBaseAsync(name, battle, 0) == 0) {
                return 0;
            }
            *(s32*)((s32)cur + 0x60) = animPoseEntry(name, battle);
            animPoseSetAnim(*(s32*)((s32)cur + 0x60), str_Z_1_80422d70, 1);
        }
    }

    alive = 0;
    cur = work;
    for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, cur = (void*)((s32)cur + 100)) {
        if (*(s32*)((s32)cur + 0x0) != 0) {
            *(s32*)((s32)cur + 0x54) -= 1;
            if (*(s32*)((s32)cur + 0x54) < 1) {
                *(s32*)((s32)cur + 0x0) = 0;
            } else {
                alive = 1;
                if (*(s32*)((s32)cur + 0x4) == 0 && *(f32*)((s32)cur + 0x34) != float_0_80422d64) {
                    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0) {
                        *(s32*)((s32)cur + 0x54) += 1;
                        continue;
                    }
                }
                *(f32*)((s32)cur + 0x44) += *(f32*)((s32)cur + 0x48);
                *(f32*)((s32)cur + 0x40) += *(f32*)((s32)cur + 0x44);
                *(f32*)((s32)cur + 0x3C) += *(f32*)((s32)cur + 0x40);
                *(f32*)((s32)cur + 0x1C) += *(f32*)((s32)cur + 0x3C) * *(f32*)((s32)cur + 0x2C);
                *(f32*)((s32)cur + 0x8) -= *(f32*)((s32)cur + 0x4C) * (*(f32*)((s32)cur + 0x3C) * *(f32*)((s32)cur + 0x30));
                *(f32*)((s32)cur + 0x10) -= *(f32*)((s32)cur + 0x50) * (*(f32*)((s32)cur + 0x3C) * *(f32*)((s32)cur + 0x30));
                *(s32*)((s32)cur + 0x58) -= 12;
                if (*(f32*)((s32)cur + 0x3C) < float_0_80422d64) {
                    *(f32*)((s32)cur + 0x34) = float_0_80422d64;
                }
            }
        }
    }

    if (alive == 0) {
        fading = 0;
        cur = work;
        for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, cur = (void*)((s32)cur + 100)) {
            *(s32*)((s32)cur + 0x58) -= 10;
            if (*(s32*)((s32)cur + 0x58) < 0) {
                *(s32*)((s32)cur + 0x58) = 0;
            } else {
                fading = 1;
            }
        }
        if (fading == 0) {
            cur = work;
            for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, cur = (void*)((s32)cur + 100)) {
                if (*(s32*)((s32)cur + 0x60) != -1) {
                    animPoseRelease(*(s32*)((s32)cur + 0x60));
                }
            }
            effDelete(effEntry);
            return 0;
        }
    }

    dispEntry(4, 2, effMugiKemuri2Disp, effEntry, dispCalcZ(&pos));
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effMugiKemuriDisp(s32 cameraId, void* effEntry) {
    typedef f32 LocalMtx[3][4];
    extern void PSMTXRotRad(LocalMtx mtx, f32 rad, s32 axis);
    extern void PSMTXTrans(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(LocalMtx a, LocalMtx b, LocalMtx ab);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, LocalMtx mtx, s32 mode, f32 rot, f32 scale);
    extern u32 dat_80422d58;
    extern f32 float_deg2rad_80422d60;
    extern f32 float_0_80422d64;
    extern f32 float_15_80422d68;
    LocalMtx rotX;
    LocalMtx rotY;
    LocalMtx rotZ;
    LocalMtx trans;
    LocalMtx scale;
    void* work;
    u32 color;
    s32 i;

    work = *(void**)((s32)effEntry + 0xC);
    for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, work = (void*)((s32)work + 100)) {
        if (*(s32*)((s32)work + 0x0) != 0 || *(s32*)((s32)work + 0x58) != 0) {
            PSMTXRotRad(rotX, float_deg2rad_80422d60 * *(f32*)((s32)work + 0x14), 'x');
            PSMTXRotRad(rotY, float_deg2rad_80422d60 * *(f32*)((s32)work + 0x18), 'y');
            PSMTXRotRad(rotZ, float_deg2rad_80422d60 * *(f32*)((s32)work + 0x1C), 'z');
            PSMTXTrans(trans, *(f32*)((s32)work + 0x8), *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10));
            PSMTXScale(scale, *(f32*)((s32)work + 0x20), *(f32*)((s32)work + 0x24), *(f32*)((s32)work + 0x28));
            PSMTXConcat(rotX, rotZ, rotZ);
            PSMTXConcat(rotY, rotZ, rotZ);
            PSMTXConcat(rotZ, scale, scale);
            PSMTXConcat(trans, scale, trans);
            if (*(s32*)((s32)work + 0x60) != -1) {
                animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x60), 0x40);
                color = (dat_80422d58 & 0xFFFFFF00) | (*(u32*)((s32)work + 0x58) & 0xFF);
                animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x60), &color);
                animPoseMain(*(s32*)((s32)work + 0x60));
                animPoseDrawMtx(*(s32*)((s32)work + 0x60), trans, 1, float_0_80422d64, float_15_80422d68);
                animPoseDrawMtx(*(s32*)((s32)work + 0x60), trans, 2, float_0_80422d64, float_15_80422d68);
                animPoseDrawMtx(*(s32*)((s32)work + 0x60), trans, 3, float_0_80422d64, float_15_80422d68);
            }
        }
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
u8 effMugiKemuri2Disp(s32 cameraId, void* effEntry) {
    typedef f32 LocalMtx[3][4];
    extern void PSMTXRotRad(LocalMtx mtx, f32 rad, s32 axis);
    extern void PSMTXTrans(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXScale(LocalMtx mtx, f32 x, f32 y, f32 z);
    extern void PSMTXConcat(LocalMtx a, LocalMtx b, LocalMtx ab);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern void animPoseSetMaterialEvtColor(s32 poseId, void* color);
    extern void animPoseMain(s32 poseId);
    extern void animPoseDrawMtx(s32 poseId, LocalMtx mtx, s32 mode, f32 rot, f32 scale);
    extern u32 dat_80422d5c;
    extern f32 float_deg2rad_80422d60;
    extern f32 float_0_80422d64;
    extern f32 float_15_80422d68;
    LocalMtx rotX;
    LocalMtx rotY;
    LocalMtx rotZ;
    LocalMtx trans;
    LocalMtx scale;
    void* work;
    u32 color;
    s32 i;

    work = *(void**)((s32)effEntry + 0xC);
    for (i = 0; i < *(s32*)((s32)effEntry + 0x8); i++, work = (void*)((s32)work + 100)) {
        if (*(s32*)((s32)work + 0x0) != 0 || *(s32*)((s32)work + 0x58) != 0) {
            PSMTXRotRad(rotX, float_deg2rad_80422d60 * *(f32*)((s32)work + 0x14), 'x');
            PSMTXRotRad(rotY, float_deg2rad_80422d60 * *(f32*)((s32)work + 0x18), 'y');
            PSMTXRotRad(rotZ, float_deg2rad_80422d60 * *(f32*)((s32)work + 0x1C), 'z');
            PSMTXTrans(trans, *(f32*)((s32)work + 0x8), *(f32*)((s32)work + 0xC), *(f32*)((s32)work + 0x10));
            PSMTXScale(scale, *(f32*)((s32)work + 0x20), *(f32*)((s32)work + 0x24), *(f32*)((s32)work + 0x28));
            PSMTXConcat(rotX, rotZ, rotZ);
            PSMTXConcat(rotY, rotZ, rotZ);
            PSMTXConcat(rotZ, scale, scale);
            PSMTXConcat(trans, scale, trans);
            if (*(s32*)((s32)work + 0x60) != -1) {
                animPoseSetMaterialFlagOn(*(s32*)((s32)work + 0x60), 0x40);
                color = (dat_80422d5c & 0xFFFFFF00) | (*(u32*)((s32)work + 0x58) & 0xFF);
                animPoseSetMaterialEvtColor(*(s32*)((s32)work + 0x60), &color);
                animPoseMain(*(s32*)((s32)work + 0x60));
                animPoseDrawMtx(*(s32*)((s32)work + 0x60), trans, 1, float_0_80422d64, float_15_80422d68);
                animPoseDrawMtx(*(s32*)((s32)work + 0x60), trans, 2, float_0_80422d64, float_15_80422d68);
                animPoseDrawMtx(*(s32*)((s32)work + 0x60), trans, 3, float_0_80422d64, float_15_80422d68);
            }
        }
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

