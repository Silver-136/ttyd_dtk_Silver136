#include "effect/n64/eff_magic1_n64.h"

typedef f32 Mtx[3][4];

void* camGetPtr(s32 cameraId);
void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);
void PSMTXRotRad(Mtx mtx, s32 axis, f32 rad);
void PSMTXConcat(void* a, void* b, void* ab);

extern f32 float_0_804259cc;
extern f32 float_deg2rad_804259d0;


u8 main_dl(void* effEntry, Mtx mtx) {
    return 0;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagic1Main(void* effect) {
    typedef struct Vec3 {
        f32 x;
        f32 y;
        f32 z;
    } Vec3;
    extern s32 marioStGetSystemLevel(void);
    extern void effDelete(void*);
    extern f32 dispCalcZ(Vec3*);
    extern void dispEntry(s32 camera, s32 layer, void* callback, void* param, f32 z);
    extern void effMagic1Disp(s32, void*);
    extern void* effMagic1N64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 targetX, f32 targetY, f32 targetZ);
    extern const Vec3 vec3_802fb418;
    extern f32 float_0_804259cc;
    extern f32 float_0p83_80425a14;
    extern f32 float_0p9_80425a18;
    u8* work;
    Vec3 dispPos;
    Vec3 pos;
    s32 type;
    u32 systemLevel;
    void* spawned;

    work = *(u8**)((s32)effect + 0xC);
    pos = vec3_802fb418;
    pos.x = *(f32*)(work + 0x10);
    pos.y = *(f32*)(work + 0x14);
    pos.z = *(f32*)(work + 0x18);
    dispPos = pos;

    systemLevel = (u32)marioStGetSystemLevel();
    if (systemLevel != 0U) {
        dispEntry(4, 2, effMagic1Disp, effect, dispCalcZ(&dispPos));
        return;
    }

    if (*(s32*)effect & 4) {
        *(s32*)effect &= ~4;
        *(s32*)(work + 0x30) = 0;
    }
    *(s32*)(work + 0x30) -= 1;
    *(s32*)(work + 0x2C) += 1;
    if (*(s32*)(work + 0x30) < 0) {
        effDelete(effect);
        return;
    }

    type = *(s32*)work;
    if (type == 0 && *(s32*)(work + 0x30) == 0) {
        spawned = effMagic1N64Entry(1, 0x30,
                                    *(f32*)(work + 4) + *(f32*)(work + 0x10),
                                    *(f32*)(work + 8) + *(f32*)(work + 0x14),
                                    *(f32*)(work + 0xC) + *(f32*)(work + 0x18),
                                    float_0_804259cc, float_0_804259cc, float_0_804259cc);
        *(f32*)((s32)*(void**)((s32)spawned + 0xC) + 0x28) = *(f32*)(work + 0x28);
    }

    if (type == 1) {
        *(f32*)(work + 0x1C) *= float_0p83_80425a14;
        *(s32*)(work + 0x34) = (s32)((f32)*(s32*)(work + 0x34) * float_0p9_80425a18);
    }

    *(f32*)(work + 0x10) += *(f32*)(work + 0x1C);
    *(f32*)(work + 0x14) += *(f32*)(work + 0x20);
    *(f32*)(work + 0x18) += *(f32*)(work + 0x24);
    dispEntry(4, 2, effMagic1Disp, effect, dispCalcZ(&dispPos));
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* effMagic1N64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 targetX, f32 targetY, f32 targetZ) {
    extern void* effEntry(void);
    extern void* __memAlloc(s32 heap, s32 size);
    extern void effMagic1Main(void*);
    extern char str_Magic1N64_802fb430[];
    extern f32 float_0_804259cc;
    extern f32 float_1_80425a04;
    extern f32 float_29_80425a1c;
    extern f32 float_6p2_80425a20;
    void* entry;
    u8* work;
    f32 invTime;
    f32 one;
    f32 zero;

    entry = effEntry();
    *(char**)((s32)entry + 0x14) = str_Magic1N64_802fb430;
    *(s32*)((s32)entry + 8) = 1;
    work = __memAlloc(3, 0x38);
    *(u8**)((s32)entry + 0xC) = work;
    *(void**)((s32)entry + 0x10) = effMagic1Main;
    *(s32*)entry |= 2;

    one = float_1_80425a04;
    *(s32*)(work + 0x2C) = 0;
    *(s32*)work = type;
    *(s32*)(work + 0x34) = 0xFF;
    *(f32*)(work + 4) = x;
    *(f32*)(work + 8) = y;
    *(f32*)(work + 0xC) = z;
    *(f32*)(work + 0x28) = one;

    if (type == 0) {
        zero = float_0_804259cc;
        invTime = one / (f32)time;
        *(f32*)(work + 0x10) = zero;
        *(f32*)(work + 0x14) = zero;
        *(f32*)(work + 0x18) = zero;
        *(f32*)(work + 0x1C) = invTime * (targetX - x);
        *(f32*)(work + 0x20) = invTime * (targetY - y);
        *(f32*)(work + 0x24) = invTime * (targetZ - z);
        *(s32*)(work + 0x30) = time;
    } else {
        zero = float_0_804259cc;
        *(f32*)(work + 0x10) = float_29_80425a1c;
        *(f32*)(work + 0x14) = zero;
        *(f32*)(work + 0x18) = zero;
        *(f32*)(work + 0x1C) = float_6p2_80425a20;
        *(f32*)(work + 0x20) = zero;
        *(f32*)(work + 0x24) = zero;
        *(s32*)(work + 0x30) = 0x18;
    }
    return entry;
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void effMagic1Disp(s32 cameraId, void* effEntry) {
    Mtx trans;
    Mtx rot;
    Mtx mtx;
    Mtx loopRot;
    register s32* work;
    register void* cam;
    register s32 type;
    f32 angle;
    s32 i;
    s32 rotAngle;

    cam = camGetPtr(cameraId);
    work = *(s32**)((s32)effEntry + 0xC);
    type = work[0];

    switch (type) {
        case 0:
            angle = float_0_804259cc;
            break;
        default:
            angle = -*(f32*)((s32)camGetPtr(4) + 0x114);
            break;
    }

    PSMTXTrans(trans, *(f32*)((s32)work + 4), *(f32*)((s32)work + 8), *(f32*)((s32)work + 0xC));
    PSMTXRotRad(rot, 0x79, float_deg2rad_804259d0 * angle);
    PSMTXConcat(trans, rot, mtx);
    PSMTXConcat((void*)((s32)cam + 0x11C), mtx, mtx);

    switch (type) {
        case 0:
            main_dl(effEntry, mtx);
            break;
        case 1:
            angle = float_deg2rad_804259d0;
            i = 0;
            rotAngle = 0;
            while (i < 12) {
                PSMTXRotRad(loopRot, 0x7A, angle * (f32)rotAngle);
                PSMTXConcat(mtx, loopRot, loopRot);
                main_dl(effEntry, loopRot);
                i++;
                rotAngle += 30;
            }
            break;
    }
}
#pragma use_lmw_stmw on
#pragma no_register_save_helpers off
