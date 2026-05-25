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
    s32 value;

    marioGetPtr();
    marioGetPtr();
    value = *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x30);
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
    f32 zero1;
    f32 zero2;

    marioGetPtr();
    zero1 = float_0_80420eb8;
    marioGetPtr();
    zero2 = float_0_80420eb8;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x14) = zero1;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x18) = zero2;
}

void marioSetSwirlPower(s32 msec, f32 a, f32 b) {
    marioGetPtr();
    *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x30) = sysMsec2Frame(msec);
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x2C) = a;
    *(f32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x28) = b;
}
