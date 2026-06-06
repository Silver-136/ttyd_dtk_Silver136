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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_ship_post(void) {
    extern void* marioGetPtr(void);
    extern void allPartyRideOff(void);
    extern void marioPaperOff(void);
    extern void psndSFXOff(s32 id);
    extern void marioOfsRotReset(void);
    extern void __memFree(s32 heap, void* ptr);
    extern f32 float_20_80420eb4;
    extern f32 float_37_80420f7c;
    void* mario = marioGetPtr();
    allPartyRideOff();
    if (*(u32*)((s32)mario + 0x4) & 0x40000000) {
        marioPaperOff();
    }
    {
        f32 z = float_37_80420f7c;
        f32 y = float_20_80420eb4;
        *(f32*)((s32)mario + 0x1B8) = y;
        *(f32*)((s32)mario + 0x1BC) = z;
    }
    marioGetPtr();
    if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
        s32 sound = *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x38);
        if ((u32)(sound + 0x10000) != 0xFFFF) {
            psndSFXOff(*(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x38));
            *(s32*)(*(s32*)((s32)marioGetPtr() + 0x294) + 0x38) = -1;
        }
    }
    *(u32*)((s32)mario + 0x4) &= ~0x4;
    *(u32*)((s32)mario + 0x4) &= ~0x100;
    *(u32*)((s32)mario + 0x4) &= ~0x8;
    marioOfsRotReset();
    if (*(void**)((s32)marioGetPtr() + 0x294) != 0) {
        __memFree(0, *(void**)((s32)marioGetPtr() + 0x294));
        *(void**)((s32)marioGetPtr() + 0x294) = 0;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 marioShipChanging(void) {
    extern void* marioGetPtr(void);
    void* mario = marioGetPtr();
    s32 state;
    if (*(u16*)((s32)mario + 0x2E) != 0x19) {
        return 0;
    }
    state = *(s32*)((s32)mario + 0x44);
    if (state == 10) {
        return 2;
    }
    if ((state >= 0 && state <= 12) || (state >= 0x1E && state <= 0x30)) {
        return 1;
    }
    if (state == 0x32 || state == 0x34) {
        return 3;
    }
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
