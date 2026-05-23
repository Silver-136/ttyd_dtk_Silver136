#include "motion/mot_stay.h"

void marioVoiceGlareOff(void) {
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32 soundId);

    void* mario = marioGetPtr();
    s32 soundId = *(s32*)((s32)mario + 0x280);

    if ((u32)(soundId + 0x10000) != 0xFFFF) {
        psndSFXOff(soundId);
        *(s32*)((s32)mario + 0x280) = -1;
    }
}

void marioVoiceGlareOn(void) {
    extern void* marioGetPtr(void);
    extern u32 psndSFXOn_3D(s32 soundId, void* position);

    void* mario = marioGetPtr();
    s32 soundId = *(s32*)((s32)mario + 0x280);

    if ((u32)(soundId + 0x10000) == 0xFFFF) {
        void* position = (void*)((s32)mario + 0x8C);
        *(u32*)((s32)mario + 0x280) = psndSFXOn_3D(0xDC, position);
    }
}
