#include "motion/mot_hip.h"
#include "bowser/koopa_motion.h"

void mot_hip_post(void);
extern void* marioGetPtr(void);
extern void marioOfsRotReset(void);
extern void psndSFXOff(s32 soundId);

s32 marioChkHipAttack(void) {
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

    s32 result = 0;
    if (*(u16*)((s32)marioGetPtr() + 0x2E) == 0x11) {
        result = 1;
    }
    return result;
}


void mot_hip(void) {
    extern void marioAdjustMoveDir(void);
    extern f64 toMovedir(f64);
    extern void marioChgPose(char*);
    extern f64 marioGetScale(void);
    extern u32 psndSFXOn_3D(char*, void*);
    extern s32 U_chkground(void);
    extern void marioChgMotSub(s32, s32);
    extern void marioChkLandon(f32*, f32);
    extern s32 sysMsec2Frame(s32);
    extern f32 marioGetFallSpd(void);
    extern void* marioSearchGround(f64, f64, f32*, void*, void*);
    extern s32 hitGetAttr(void*);
    extern void set_damage_root_ypos(f64);
    extern void marioChgMot(s32);
    extern f32 float_neg0p4_804243d8;
    extern s32 marioBgmodeChk(void);
    extern void camFollowYOn(void);
    extern char str_M_A_1A_802f94a8[];
    extern char str_M_A_1B_802f94b0[];
    char* mario;
    f32 angle;
    f32 value;
    f32 fall;
    f32 groundY;
    u32 outA;
    u32 outB;
    void* hit;
    s32 ground;
    s32 attr;
    u32 state;

    mario = marioGetPtr();
    if (*(s8*)(mario + 0x3C) == 2) {
        kpa_hip();
        return;
    }
    if (*(u32*)(mario + 0xC) & 2) {
        *(u32*)(mario + 0xC) &= ~2;
        *(u32*)mario &= ~0xF0;
        *(u32*)mario |= 0x20000;
        *(u32*)mario |= 0x80;
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
        mario = marioGetPtr();
        *(s32*)(mario + 0x288) = -1;
        *(s32*)(mario + 0x28C) = -1;
    }

    state = *(u32*)(mario + 0x44);
    if (state == 0) {
        if (*(s32*)(mario + 0x288) == -1) {
            *(s32*)(mario + 0x288) = psndSFXOn_3D((char*)0x158, mario + 0x8C);
        }
        if (*(s32*)(mario + 0x28C) == -1) {
            *(s32*)(mario + 0x28C) = psndSFXOn_3D((char*)0xDB, mario + 0x8C);
        }
        *(f32*)(mario + 0x2B8) = 0.0f;
        *(f32*)(mario + 0x180) = 0.0f;
        *(u32*)(mario + 0x44) = 1;
    }
    if (state <= 1) {
        value = *(f32*)(mario + 0x2B8) + 20.0f;
        *(f32*)(mario + 0x2B8) = value;
        if (value >= 360.0f) {
            *(f32*)(mario + 0x2B8) = 360.0f;
        }
        *(f32*)(mario + 0x7C) -= 0.42f;
        if (*(void**)(mario + 0x1F8) == 0) {
            *(f32*)(mario + 0xA8) += *(f32*)(mario + 0x7C);
        }
        if (*(f32*)(mario + 0x2B8) == 360.0f && *(f32*)(mario + 0x7C) <= 0.0f) {
            *(u32*)(mario + 0x48) = 5;
            *(u32*)(mario + 0x44) = 2;
            *(f32*)(mario + 0x7C) = 2.0f;
        }
        ground = U_chkground();
    } else if (state == 2) {
        if (*(f32*)(mario + 0x7C) >= 0.0f) {
            *(f32*)(mario + 0x7C) -= 1.4f;
            if (*(void**)(mario + 0x1F8) == 0) {
                *(f32*)(mario + 0xA8) += *(f32*)(mario + 0x7C);
            }
        }
        if (--*(u32*)(mario + 0x48) < 1) {
            *(u32*)(mario + 0x44) = 10;
        }
        ground = U_chkground();
    } else if (state == 10) {
        marioChkLandon(&groundY, 1.0f);
        value = *(f32*)(mario + 0x2B8) + 22.5f;
        *(f32*)(mario + 0x2B8) = value;
        if (value >= 360.0f) {
            *(u32*)mario &= ~0x40;
            *(u32*)mario |= 0x80;
            *(f32*)(mario + 0x2B8) = 0.0f;
            *(u32*)(mario + 0x44) = 0x14;
            *(f32*)(mario + 0x80) = -3.4744f;
            *(f32*)(mario + 0x84) = 0.4704f;
            *(f32*)(mario + 0x88) = -1.1904f;
            *(f32*)(mario + 0x7C) = *(f32*)(mario + 0x80);
        }
        ground = U_chkground();
    } else if (state == 20 || state == 21) {
        if (state == 20) {
            *(s32*)(mario + 0x48) = sysMsec2Frame(200);
            *(s32*)(mario + 0x44) = 21;
        }
        if (--*(s32*)(mario + 0x48) < 1) {
            fall = marioGetFallSpd();
            hit = marioSearchGround((f64)fall, (f64)fall, &groundY, &outA, &outB);
            if (hit == 0) {
                *(f32*)(mario + 0x90) += fall;
            } else {
                *(s16*)(mario + 0x2E) = 0x11;
                *(s32*)(mario + 0x44) = 22;
                *(s32*)(mario + 0x48) = 20;
                *(void**)(mario + 0x1E8) = hit;
                *(void**)(mario + 0x1EC) = 0;
                *(f32*)(mario + 0x90) = groundY;
                *(f32*)(mario + 0x2B8) = 0.0f;
                *(f32*)(mario + 0xB4) = 0.0f;
                *(s16*)(mario + 0x50) = 0;
                *(s16*)(mario + 0x52) = 0;
                *(f32*)(mario + 0xA8) = 0.0f;
                marioChgPose(str_M_A_1B_802f94b0);
                attr = hitGetAttr(hit);
                if (attr & 0x200) {
                    set_damage_root_ypos((f64)*(f32*)(mario + 0x90));
                    *(f32*)(mario + 0x7C) = float_neg0p4_804243d8;
                    *(f32*)(mario + 0x80) = float_neg0p4_804243d8;
                    *(f32*)(mario + 0x84) = 0.0f;
                    *(f32*)(mario + 0x88) = 0.0f;
                    marioChgMot(0x1F);
                    return;
                }
                if (attr & 0xC00) {
                    set_damage_root_ypos((f64)*(f32*)(mario + 0x90));
                    marioChgMot(0x20);
                    return;
                }
                psndSFXOn_3D((char*)0x159, mario + 0x8C);
            }
            if (*(s32*)(mario + 0x2D8) > 400 && marioBgmodeChk() == 0) {
                camFollowYOn();
            }
        }
        goto final_update;
    } else if (state == 22) {
        ground = U_chkground();
        if (ground == 1) {
            return;
        }
        if (--*(s32*)(mario + 0x48) < 1) {
            *(u32*)(mario + 4) &= 0xF7FFFFFF;
            if (marioBgmodeChk() == 0) {
                camFollowYOn();
            }
            *(u32*)(mario + 4) &= ~0x10000000;
            *(u32*)mario &= ~0x80;
            marioChgMotSub(0, 0);
            mot_hip_post();
        }
        goto final_update;
    } else {
        goto final_update;
    }
    if (ground == 2) {
        marioChgMotSub(0, 0);
        mot_hip_post();
        return;
    }
    if (ground == 1) {
        return;
    }
final_update:
    *(f32*)(mario + 0x188) = *(f32*)(mario + 0x2B8);
    if (*(s16*)(mario + 0x50) != 0) {
        *(f32*)(mario + 0xAC) = 0.0f;
        *(f32*)(mario + 0xB0) = 0.0f;
        *(f32*)(mario + 0xA8) = 0.0f;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 U_chkground(void) {
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

const char str_M_A_1A_802f94a8[] = "M_A_1A";
const char str_M_A_1B_802f94b0[] = "M_A_1B";
