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
