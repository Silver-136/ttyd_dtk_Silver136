#include "motion/mot_roll.h"

void* marioGetPtr(void);
void camFollowYOff(void);

void clrRollEvtFlag(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x2BC) &= ~0x100;
}

void setRollEvtFlag(void) {
    void* mario = marioGetPtr();
    *(u32*)((s32)mario + 0x2BC) |= 0x100;
}

s32 getRollEvtFlag(void) {
    s32 ret = 0;
    void* mario = marioGetPtr();

    if ((*(u32*)mario & 0x1000000) && (*(u32*)((s32)mario + 0x2BC) & 0x100)) {
        ret = 1;
    }
    return ret;
}

void rollEvtJumpSetup(void) {
    void* mario = marioGetPtr();

    camFollowYOff();
    *(u32*)mario |= 0x10000;
    *(u16*)((s32)mario + 0x50) = 1;
}

s32 marioRollKeyDisable(void) {
    void* mario = marioGetPtr();
    s32 value;

    if (*(u16*)((s32)mario + 0x2E) != 0x16) {
        return 1;
    }
    value = *(s32*)((s32)mario + 0x44);
    if (value >= 0 && value <= 1) {
        return 0;
    }
    if (value >= 0x14 && value <= 0x16) {
        return 0;
    }
    return 1;
}

void marioRollFixOn(void) {
    void* mario = marioGetPtr();

    if (*(u16*)((s32)mario + 0x2E) == 0x16) {
        *(u32*)((s32)mario + 0x2BC) |= 0x200;
    }
}

