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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 U_chkground(void) {
    extern void* marioGetPtr(void);
    extern s32 marioChkLandon(f32* ypos, f32 scale);
    extern void camFollowYOn(void);
    extern s32 hitGetAttr(s32 hit);
    extern void set_damage_root_ypos(f32 ypos);
    extern void marioChgMot(s32 mot);

    void* mario;
    f32 ypos;
    s32 hit;
    s32 attr;

    mario = marioGetPtr();
    hit = marioChkLandon(&ypos, 1.0f);
    if (hit != 0) {
        *(s32*)((s32)mario + 0x1E8) = hit;
        *(s32*)((s32)mario + 0x1EC) = 0;
        *(f32*)((s32)mario + 0x90) = ypos;
        *(f32*)((s32)mario + 0x2B8) = 0.0f;
        *(f32*)((s32)mario + 0xB4) = 0.0f;
        *(f32*)((s32)mario + 0xA8) = 0.0f;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s16*)((s32)mario + 0x52) = 0;
        *(u32*)((s32)mario + 4) &= ~4;
        *(u32*)mario &= ~0x80;
        camFollowYOn();
        attr = hitGetAttr(hit);
        if (attr & 0x200) {
            set_damage_root_ypos(*(f32*)((s32)mario + 0x90));
            *(f32*)((s32)mario + 0x7C) = -0.4f;
            *(f32*)((s32)mario + 0x80) = -0.4f;
            *(f32*)((s32)mario + 0x84) = 0.0f;
            *(f32*)((s32)mario + 0x88) = 0.0f;
            marioChgMot(0x1F);
            return 1;
        }
        if (attr & 0xC00) {
            set_damage_root_ypos(*(f32*)((s32)mario + 0x90));
            marioChgMot(0x20);
            return 1;
        }
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 marioChkHipAttack(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchGetJumpLv(void);

    void* mario;
    s32 result;

    result = 0;
    mario = marioGetPtr();
    if (*(s16*)((s32)mario + 0x52) >= 10 && (*(u16*)((s32)mario + 0x24C) & 0x100) && pouchGetJumpLv() >= 2) {
        result = 1;
    }
    return result;
}
