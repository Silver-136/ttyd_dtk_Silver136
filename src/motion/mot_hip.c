#include "motion/mot_hip.h"

void mot_hip_post(void);

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

s32 marioChkHipBump(void) {
    extern void* marioGetPtr(void);

    s32 result = 0;
    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x11) {
        result = 1;
    }
    return result;
}


void mot_hip(void) {
    extern void* marioGetPtr(void);
    extern void kpa_hip(void);
    extern void marioAdjustMoveDir(void);
    extern f64 toMovedir(f64);
    extern void marioChgPose(char*);
    extern f64 marioGetScale(void);
    extern u32 psndSFXOn_3D(char*, void*);
    extern void psndSFXOff(s32);
    extern s32 U_chkground(void);
    extern void marioChgMotSub(s32, s32);
    extern void marioOfsRotReset(void);
    extern void marioChkLandon(f32*, f32);
    extern char str_M_A_1A_802f94a8[];
    char* mario = marioGetPtr();
    f32 angle;
    f32 value;
    s32 ground;

    if (*(s8*)(mario + 0x3C) == 2) {
        kpa_hip();
        return;
    }
    if (*(u32*)(mario + 0xC) & 2) {
        *(u32*)(mario + 0xC) &= ~2;
        *(u32*)mario &= ~0xF0;
        *(u32*)mario |= 0x280;
        marioAdjustMoveDir();
        angle = 0.0f;
        if (*(f32*)(mario + 0x1B0) >= 90.0f && *(f32*)(mario + 0x1B0) <= 270.0f) {
            angle = 180.0f;
        }
        *(f32*)(mario + 0x1A0) = (f32)toMovedir(angle);
        *(f32*)(mario + 0x1A4) = *(f32*)(mario + 0x1A0);
        marioChgPose(str_M_A_1A_802f94a8);
        *(f32*)(mario + 0x7C) = 5.2f;
        *(f32*)(mario + 0x80) = 0.0f;
        *(f32*)(mario + 0x84) = 0.0f;
        *(f32*)(mario + 0x88) = 0.0f;
        *(f32*)(mario + 0x90) += (f32)(-2.0 * marioGetScale()) * 0.5f;
        *(f32*)(mario + 0xB4) += (f32)(14.0 * marioGetScale()) * 0.5f;
        *(f32*)(mario + 0xA8) = 0.0f;
        *(u32*)(mario + 0x44) = 0;
        *(s32*)(mario + 0x288) = -1;
        *(s32*)(mario + 0x28C) = -1;
    }

    switch (*(u32*)(mario + 0x44)) {
        case 0:
            if (*(s32*)(mario + 0x288) == -1) {
                *(s32*)(mario + 0x288) = psndSFXOn_3D((char*)0x158, mario + 0x8C);
            }
            if (*(s32*)(mario + 0x28C) == -1) {
                *(s32*)(mario + 0x28C) = psndSFXOn_3D((char*)0xDB, mario + 0x8C);
            }
            *(f32*)(mario + 0x64) = 0.0f;
            *(f32*)(mario + 0x180) = 0.0f;
            *(u32*)(mario + 0x44) = 1;
        case 1:
            value = *(f32*)(mario + 0x64) + 20.0f;
            *(f32*)(mario + 0x64) = value;
            if (value >= 360.0f) {
                *(f32*)(mario + 0x64) = 360.0f;
            }
            *(f32*)(mario + 0x7C) -= 0.42f;
            *(f32*)(mario + 0xA8) += *(f32*)(mario + 0x7C);
            if (*(f32*)(mario + 0x64) == 360.0f && *(f32*)(mario + 0x7C) <= 0.0f) {
                *(u32*)(mario + 0x48) = 5;
                *(u32*)(mario + 0x44) = 2;
                *(f32*)(mario + 0x7C) = 2.0f;
            }
            ground = U_chkground();
            break;
        case 2:
            if (*(f32*)(mario + 0x7C) >= 0.0f) {
                *(f32*)(mario + 0x7C) -= 1.4f;
                *(f32*)(mario + 0xA8) += *(f32*)(mario + 0x7C);
            }
            if (--*(u32*)(mario + 0x48) < 1) {
                *(u32*)(mario + 0x44) = 10;
            }
            ground = U_chkground();
            break;
        case 10:
            marioChkLandon(0, 1.0f);
            value = *(f32*)(mario + 0x64) + 22.5f;
            *(f32*)(mario + 0x64) = value;
            if (value >= 360.0f) {
                *(u32*)mario &= ~0x40;
                *(u32*)mario |= 0x80;
                *(f32*)(mario + 0x64) = 0.0f;
                *(u32*)(mario + 0x44) = 0x14;
                *(f32*)(mario + 0x80) = -3.4744f;
                *(f32*)(mario + 0x84) = 0.4704f;
                *(f32*)(mario + 0x88) = -1.1904f;
                *(f32*)(mario + 0x7C) = *(f32*)(mario + 0x80);
            }
            ground = U_chkground();
            break;
        default:
            return;
    }
    if (ground == 2) {
        marioChgMotSub(0, 0);
        mot_hip_post();
    }
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
