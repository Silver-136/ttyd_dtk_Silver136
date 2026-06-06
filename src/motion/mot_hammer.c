#include "motion/mot_hammer.h"

s32 marioGetHammerLevel(void) {
    extern void* pouchGetPtr(void);

    return *(s8*)((s32)pouchGetPtr() + 0x99);
}

f32 marioGetAngleKaitenHammer(void) {
    extern void* marioGetPtr(void);
    extern f32 revise360(f32 angle);
    extern f32 float_neg1_80420b04;
    extern f32 float_neg2_80420b08;
    extern f32 float_270_80420ac4;

    void* mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) != 0x13) {
        return float_neg1_80420b04;
    }
    if (*(s32*)((s32)mario + 0x44) != 0x14) {
        return float_neg2_80420b08;
    }
    return revise360(float_270_80420ac4 - *(f32*)((s32)mario + 0x1AC));
}


u8 mot_hammer2(void) {
    return 0;
}


u8 mot_hammer(void) {
    return 0;
}


u8 hitEstimate(void) {
    return 0;
}


s32 marioChkHammer2(void) {
    return 0;
}


void mot_ham2_post(void) {
    extern void* marioGetPtr(void);
    extern void marioPaperOff(void);
    extern void marioAdjustMoveDir(void);
    extern f32 float_0_80420ab8;
    extern f32 float_0p01_80420b00;
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0x4);
    if (flags & 0x40000000) {
        marioPaperOff();
    }
    {
        void* mario2 = marioGetPtr();
        f32 small = float_0p01_80420b00;
        f32 zero = float_0_80420ab8;
        *(f32*)((s32)mario2 + 0x148) = zero;
        *(f32*)((s32)mario2 + 0x158) = small;
    }
    marioPaperOff();
    *(u32*)((s32)mario + 0x4) &= ~0x100;
    *(u32*)((s32)mario + 0x4) &= ~0x8;
    *(u32*)((s32)mario + 0x4) &= ~0x10;
    *(u32*)((s32)mario + 0x4) &= ~0x4;
    *(u32*)((s32)mario + 0x0) &= ~0x80;
    marioAdjustMoveDir();
}

