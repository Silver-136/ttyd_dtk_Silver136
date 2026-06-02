#include "motion/mot_ship.h"

extern f32 float_0_80420eb8;

u32 hitGetAttr(void* hit);
void* marioGetPtr(void);
s32 sysMsec2Frame(s32 msec);

s32 marioChkShipPlace(void* hit) {
    u32 attr = hitGetAttr(hit) & 0x3C000;
    return (u32)(-attr | attr) >> 31;
}

s32 marioChkSwirlPower(void) {
    void* mario;
    s32 value;

    marioGetPtr();
    mario = marioGetPtr();
    value = *(s32*)(*(s32*)((s32)mario + 0x294) + 0x30);
    return (u32)(-value | value) >> 31;
}

s32 N_marioShipGetDir(void) {
    marioGetPtr();
    if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
        return (*(u32*)*(s32*)((s32)marioGetPtr() + 0x294) >> 1) & 1;
    }
    return 0;
}

void marioShipSetDispDir(void) {
    f32 zero2;
    f32 zero1;
    void* mario;

    marioGetPtr();
    zero1 = float_0_80420eb8;
    mario = marioGetPtr();
    zero2 = float_0_80420eb8;
    *(f32*)(*(s32*)((s32)mario + 0x294) + 0x14) = zero1;
    mario = marioGetPtr();
    *(f32*)(*(s32*)((s32)mario + 0x294) + 0x18) = zero2;
}

void marioSetSwirlPower(s32 msec, f32 a, f32 b) {
    marioGetPtr();
    *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x30) = sysMsec2Frame(msec);
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x2C) = a;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x28) = b;
}


u8 mot_ship(void) {
    return 0;
}


u8 shipMove(void) {
    return 0;
}


s32 checkHarbor(float* param_1) {
    return 0;
}


void marioReInit_ship(void) {
}


u8 mot_ship_post(void) {
    return 0;
}


s32 marioShipChanging(void) {
    return 0;
}


u8 marioForceShipAnime(void) {
    return 0;
}


u8 marioShipForceStop(void) {
    return 0;
}


s32 marioChkShipMoveMode(void) {
    return 0;
}


u8 marioShipChgFall(void) {
    return 0;
}
