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


u8 mot_stay(void) {
    return 0;
}


void mot_stay_post(void) {
    extern void* marioGetPtr(void);
    extern void psndSFXOff(s32 soundId);
    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);
    if (motion != 0x13 && motion != 0x14) {
        mario = marioGetPtr();
        if ((u32)(*(s32*)((s32)mario + 0x27C) + 0x10000) != 0xFFFF) {
            psndSFXOff(*(s32*)((s32)mario + 0x27C));
            *(s32*)((s32)mario + 0x27C) = -1;
        }
        mario = marioGetPtr();
        if ((u32)(*(s32*)((s32)mario + 0x280) + 0x10000) != 0xFFFF) {
            psndSFXOff(*(s32*)((s32)mario + 0x280));
            *(s32*)((s32)mario + 0x280) = -1;
        }
    } else {
        mario = marioGetPtr();
        if ((u32)(*(s32*)((s32)mario + 0x27C) + 0x10000) != 0xFFFF) {
            psndSFXOff(*(s32*)((s32)mario + 0x27C));
            *(s32*)((s32)mario + 0x27C) = -1;
        }
    }
}

void marioChgMotAuto(void) {
    extern void* marioGetPtr(void);
    extern void marioChgMot(s32 motion);
    extern s32 pouchEquipCheckBadge(s32 badge);
    void* mario = marioGetPtr();
    u8 ux = *(u8*)((s32)mario + 0x252);
    s32 x = ux;
    s32 y;
    if ((s8)x == 0 && *(s8*)((s32)mario + 0x253) == 0) {
        marioChgMot(0);
    } else {
        x = (s8)x;
        y = *(s8*)((s32)mario + 0x253);
        if (x * x + y * y > 0xBD1) {
            if (pouchEquipCheckBadge(0x143) == 0) {
                marioChgMot(2);
            } else {
                marioChgMot(1);
            }
        } else {
            marioChgMot(1);
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkDeepSleep(void) {
    extern void* marioGetPtr(void);
    extern s32 sysMsec2Frame(s32 msec);
    register s32 result = 0;
    void* mario = marioGetPtr();

    if (*(u16*)((s32)mario + 0x2E) == 0) {
        if (*(s32*)((s32)mario + 0x2B4) >= sysMsec2Frame(10000)) {
            result = 1;
        }
    }
    return result;
}
#pragma no_register_save_helpers off

