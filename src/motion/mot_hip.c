#include "motion/mot_hip.h"

s32 marioChkHipBump(void) {
    extern void* marioGetPtr(void);

    s32 result = 0;
    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x11) {
        result = 1;
    }
    return result;
}

void mot_hip_post(void) {
    extern void* marioGetPtr(void);
    extern void marioOfsRotReset(void);
    extern void psndSFXOff(s32 soundId);

    void* mario = marioGetPtr();
    s32 soundId;

    marioOfsRotReset();
    *(s16*)((s32)mario + 0x50) = 0;
    *(s16*)((s32)mario + 0x52) = 0;

    mario = marioGetPtr();
    soundId = *(s32*)((s32)mario + 0x288);
    if ((u32)(soundId + 0x10000) != 0xFFFF) {
        psndSFXOff(soundId);
        *(s32*)((s32)mario + 0x288) = -1;
    }

    soundId = *(s32*)((s32)mario + 0x28C);
    if ((u32)(soundId + 0x10000) != 0xFFFF) {
        psndSFXOff(soundId);
        *(s32*)((s32)mario + 0x28C) = -1;
    }
}


u8 mot_hip(void) {
    return 0;
}


s32 U_chkground(void) {
    return 0;
}


s32 marioChkHipAttack(void) {
    return 0;
}
