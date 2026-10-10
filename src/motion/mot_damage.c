#include "motion/mot_damage.h"

extern s32 quake_bgmode_evt[8];
extern s32 quake_evt[8];
extern f32 float_1_80422b78;
extern void marioClearSlitFloor(void);
extern void marioAdjustMoveDir(void);
extern void marioSlitForceCancel(void);
extern void bomhei_use_cancel2(void);
extern void marioNoUpdateCamPos(void);
extern void marioSetFallPara(void);
extern f32 marioGetFallSpd(void);
extern void mario_simple_kemuri(void);
extern s32 pouchGetHP(void);
extern void marioUpdateCamPos(void);
extern char str_M_D_2_80422b90[8];
extern char str_M_U_3_80422b98[8];

void* marioGetPtr(void);
s32 marioBgmodeChk(void);
void* evtEntry(void* script, s32 priority, s32 flags);
void psndSFXOn_3D(s32 id, void* pos);
void effSplashEntry(s32 type, f32 x, f32 y, f32 z, f32 scale);
void marioGetScreenPos(void* pos, f32* x, f32* y, f32* z);
s32 marioChkInScreen(s32 x, s32 y);

typedef struct DamageVec {
    f32 x;
    f32 y;
    f32 z;
} DamageVec;

const DamageVec vec3_802f3838[1] = {{0.0f, 0.0f, 0.0f}};
const DamageVec vec3_802f3844[1] = {{0.0f, 0.0f, 0.0f}};
const DamageVec vec3_802f3850[1] = {{0.0f, 0.0f, 0.0f}};
const DamageVec vec3_802f385c[1] = {{0.0f, 0.0f, 0.0f}};
const DamageVec vec3_802f3868[1] = {{400.0f, 35.0f, -100.0f}};
const DamageVec vec3_802f3874[1] = {{0.0f, 0.0f, -200.0f}};
const DamageVec vec3_802f3880[1] = {{146.0f, 0.0f, 146.0f}};
const DamageVec vec3_802f388c[1] = {{0.0f, 0.0f, 0.0f}};
const DamageVec vec3_802f3898[1] = {{0.0f, 0.0f, 0.0f}};
const DamageVec vec3_802f38a4[1] = {{0.0f, 0.0f, 0.0f}};

void marioDmgRetQuakeOn(void) {
    void* script;

    if (marioBgmodeChk() == 0) {
        script = quake_evt;
    } else {
        script = quake_bgmode_evt;
    }
    evtEntry(script, 0, 0);
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioSetSplash(s32 type, f32* pos) {
    f32* p = pos;

    if (type == 0) {
        psndSFXOn_3D(0x194, p);
    }
    if (marioBgmodeChk() != 1) {
        effSplashEntry(((u32)(-type) | (u32)type) >> 31, p[0], p[1], p[2], float_1_80422b78);
    }
    if (type == 1) {
        psndSFXOn_3D(0xBA, p);
    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



s32 gazigazi(void* pEvt) {
    struct MarioPositionView {
        char before_position[0x8C];
        f32 x;
        f32 y;
        f32 z;
        char before_damage_y[0x2B8 - 0x98];
        f32 damage_y;
    };
    struct MarioPositionView* mario = marioGetPtr();

    *(s32*)((s32)pEvt + 0x9C) = (s32)mario->x;
    *(s32*)((s32)pEvt + 0xA0) = (s32)mario->damage_y;
    *(s32*)((s32)pEvt + 0xA4) = (s32)mario->z;
    return 2;
}


s32 marioChkDamageSink(void) {
    void* mario = marioGetPtr();
    s32 state;

    if (*(u16*)((s32)mario + 0x2E) == 0x1F) {
        state = *(s32*)((s32)mario + 0x44);
        if (state >= 0 && state < 10) {
            return 1;
        }
    }
    return 0;
}

void set_damage_root_ypos(f32 y) {
    f32 temp = y;
    void* mario = marioGetPtr();
    *(f32*)((s32)mario + 0x2B8) = temp;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma opt_propagation off
#pragma opt_common_subs off
void mot_damage(void) {
    typedef DamageVec Vec;
    extern void* marioGetPtr(void);
    extern void marioClearSlitFloor(void);
    extern void marioOfsRotReset(void);
    extern void marioAdjustMoveDir(void);
    extern void marioSlitForceCancel(void);
    extern void bomhei_use_cancel2(void);
    extern void marioNoUpdateCamPos(void);
    extern void marioChgPose(char*);
    extern void marioChgEvtPose(char*);
    extern void marioMakeJumpPara(void);
    extern void marioSetFallPara(void);
    extern f32 marioGetFallSpd(void);
    extern void* marioChkLandon(f32, f32*);
    extern void mario_simple_kemuri(void);
    extern void npcStopForEvent(void);
    extern void npcStartForEvent(void);
    extern void* evtEntry(void*, s32, s32);
    extern void psndSFXOn_3D(s32, void*);
    extern Vec* hitGetDamageReturnPos(void*);
    extern f32 distABf(f32, f32, f32, f32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern void movePos(f32, f32, f32*, f32*);
    extern s32 marioBgmodeChk(void);
    extern s32 pouchAddHP(s16);
    extern s32 pouchGetHP(void);
    extern void marioUpdateCamPos(void);
    extern void marioChgMot(s32);
    extern s32 quake_evt[8];
    extern s32 quake_bgmode_evt[8];
    extern s32 evt_gazigazi[8];
    extern char str_KPA_D_1_802f38b8[];
    extern char str_M_F_1_80422ba4[8];
    extern char str_KPA_D_2_802f38d8[];
    extern char str_M_D_2_80422b90[8];
    extern char str_M_U_3_80422b98[8];
    extern char str_muj_00_802f38c0[];
    extern char str_gor_00_802f38c8[];
    extern char str_gor_04_802f38d0[];
    extern void* gp;
    extern f32 float_50_80422bb8;
    extern s32 strcmp(char*, char*);
    void* mario;
    void* hit;
    void* code;
    Vec target;
    Vec splash;
    Vec hitPos;
    Vec bgPos;
    Vec* returnPos;
    f32 landY;
    f32 fall;
    f32 oldY;
    f32 rise;
    f32 returnY;
    s32 timer;

    mario = marioGetPtr();
    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        if (*(f32*)((s32)mario + 0x7C) == 0.0f) {
            *(f32*)((s32)mario + 0x7C) = -6.0f;
            *(f32*)((s32)mario + 0x80) = 0.0f;
            *(f32*)((s32)mario + 0x84) = 0.0f;
            *(f32*)((s32)mario + 0x88) = 0.0f;
        }
        marioClearSlitFloor();
        marioOfsRotReset();
        *(u32*)((s32)mario + 4) &= ~0x100;
        marioAdjustMoveDir();
        marioSlitForceCancel();
        bomhei_use_cancel2();
        marioNoUpdateCamPos();
        if (*(s8*)((s32)mario + 0x3C) == 2) {
            marioChgPose(str_KPA_D_1_802f38b8);
        } else {
            marioChgEvtPose(str_M_F_1_80422ba4);
        }
        *(s32*)((s32)mario + 0x44) = 0;
        *(s32*)((s32)mario + 0x48) = 0;
        *(f32*)((s32)mario + 0x180) = 0.0f;
        *(s32*)((s32)mario + 0x48) = 20;
        if (*(u16*)((s32)mario + 0x30) != 0x1A && *(u16*)((s32)mario + 0x30) != 0x18) {
            splash.x = *(f32*)((s32)mario + 0x8C);
            splash.y = *(f32*)((s32)mario + 0x90);
            splash.z = *(f32*)((s32)mario + 0x94);
            psndSFXOn_3D(0x194, &splash);
            if (marioBgmodeChk() != 1) {
                effSplashEntry(0, splash.x, splash.y, splash.z, float_1_80422b78);
            }
        }
        *(Vec*)((s32)mario + 0x98) = vec3_802f3838[0];
        *(Vec*)((s32)mario + 0xA4) = vec3_802f3844[0];
        npcStopForEvent();
    }

    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            if (*(f32*)((s32)mario + 0x90) < *(f32*)((s32)mario + 0x2B8) - 160.0f) {
                *(f32*)((s32)mario + 0x7C) = 0.3f;
                *(s32*)((s32)mario + 0x44) = 1;
                *(s32*)((s32)mario + 0x48) = 40;
            } else {
                marioMakeJumpPara();
                if (*(f32*)((s32)mario + 0x7C) <= -6.0f) {
                    *(f32*)((s32)mario + 0x7C) = -6.0f;
                    *(f32*)((s32)mario + 0x80) = 0.0f;
                    *(f32*)((s32)mario + 0x84) = 0.0f;
                    *(f32*)((s32)mario + 0x88) = 0.0f;
                }
                *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
            }
            break;
        case 1:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                *(s32*)((s32)mario + 0x48) = 0;
                *(s32*)((s32)mario + 0x44) = 10;
                *(f32*)((s32)mario + 0x2C4) = 20.0f;
            }
            break;
        case 10:
            timer = ++*(s32*)((s32)mario + 0x48);
            if (timer == 2) {
                *(s32*)((s32)mario + 0x44) = 11;
                psndSFXOn_3D(0x195, (void*)((s32)mario + 0x8C));
                if (*(void**)((s32)mario + 0x208) != 0) {
                    evtEntry(evt_gazigazi, 0, 0);
                }
            }
        case 11:
            oldY = *(f32*)((s32)mario + 0x90);
            rise = *(f32*)((s32)mario + 0x2C4);
            *(f32*)((s32)mario + 0x90) = oldY + rise;
            rise -= 1.0f;
            if (rise <= 00.0f) rise = 10.0f;
            *(f32*)((s32)mario + 0x2C4) = rise;
            if ((*(f32*)((s32)mario + 0x90) >= *(f32*)((s32)mario + 0x2B8)) &&
                (oldY < *(f32*)((s32)mario + 0x2B8))) {
                splash = *(Vec*)((s32)mario + 0x8C);
                if (marioBgmodeChk() != 1) {
                    effSplashEntry(1, splash.x, splash.y, splash.z, float_1_80422b78);
                }
                psndSFXOn_3D(0xBA, &splash);
            }
            if (*(f32*)((s32)mario + 0x90) < 500.0f + *(f32*)((s32)mario + 0x2B8)) break;
            {
                if (*(void**)((s32)mario + 0x208) == 0) {
                    target = vec3_802f385c[0];
                    if (strcmp((char*)gp + 0x12C, str_muj_00_802f38c0) == 0) target = vec3_802f3868[0];
                    if (strcmp((char*)gp + 0x12C, str_gor_00_802f38c8) == 0) target = vec3_802f3874[0];
                    if (strcmp((char*)gp + 0x12C, str_gor_04_802f38d0) == 0) target = vec3_802f3880[0];
                } else {
                    returnPos = hitGetDamageReturnPos(*(void**)((s32)mario + 0x208));
                    hitPos = vec3_802f3850[0];
                    hitPos.x = returnPos->x;
                    hitPos.y = returnPos->y;
                    hitPos.z = returnPos->z;
                    target = hitPos;
                }
                if ((*(u32*)mario & 0x02000000) != 0) {
                    bgPos = vec3_802f388c[0];
                    bgPos.x = *(f32*)((s32)mario + 0xE0);
                    bgPos.y = *(f32*)((s32)mario + 0xE4);
                    bgPos.z = *(f32*)((s32)mario + 0xE8);
                    target = bgPos;
                }
                *(f32*)((s32)mario + 0x180) = 8.0f;
                timer = (s16)(s32)(distABf(target.x, target.z, *(f32*)((s32)mario + 0x8C),
                                           *(f32*)((s32)mario + 0x94)) * 0.125f);
                *(s32*)((s32)mario + 0x48) = timer;
                *(f32*)((s32)mario + 0x1A4) = angleABf(*(f32*)((s32)mario + 0x8C),
                    *(f32*)((s32)mario + 0x94), target.x, target.z);
                *(f32*)((s32)mario + 0x2C0) = (target.y - *(f32*)((s32)mario + 0x2B8)) / (f32)*(s32*)((s32)mario + 0x48);
                *(s32*)((s32)mario + 0x44) = 12;
                if (*(s32*)((s32)mario + 0x48) == 0) {
                    *(s32*)((s32)mario + 0x48) = 10;
                    *(f32*)((s32)mario + 0x2C0) = (target.y - *(f32*)((s32)mario + 0x2B8)) / (f32)*(s32*)((s32)mario + 0x48);
                    *(s32*)((s32)mario + 0x44) = 14;
                }
                *(u32*)((s32)mario + 4) |= 1;
            }
            break;
        case 12:
            *(f32*)((s32)mario + 0xF0) += *(f32*)((s32)mario + 0x2C0);
            movePos(*(f32*)((s32)mario + 0x180), *(f32*)((s32)mario + 0x1A4),
                    (f32*)((s32)mario + 0x8C), (f32*)((s32)mario + 0x94));
            *(f32*)((s32)mario + 0xEC) = *(f32*)((s32)mario + 0x8C);
            *(f32*)((s32)mario + 0xF4) = *(f32*)((s32)mario + 0x94);
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                *(s32*)((s32)mario + 0x44) = 13;
                *(u32*)mario |= 0x80;
                marioSetFallPara();
                *(u32*)((s32)mario + 4) &= ~1;
                *(s32*)((s32)mario + 0x48) = 1;
            }
            break;
        case 13:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                *(s32*)((s32)mario + 0x44) = 20;
            }
            break;
        case 14:
            *(f32*)((s32)mario + 0xF0) += *(f32*)((s32)mario + 0x2C0);
            *(f32*)((s32)mario + 0xEC) = *(f32*)((s32)mario + 0x8C);
            *(f32*)((s32)mario + 0xF4) = *(f32*)((s32)mario + 0x94);
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                *(u32*)mario |= 0x80;
                marioSetFallPara();
                *(u32*)((s32)mario + 4) &= ~1;
                *(s32*)((s32)mario + 0x44) = 20;
            }
            break;
        case 20:
            *(void**)((s32)mario + 0x1E8) = 0;
            fall = marioGetFallSpd();
            if (fall <= -10.0f) fall = -10.0f;
            if (*(void**)((s32)mario + 0x208) == 0) returnY = *(f32*)((s32)mario + 0x90);
            else returnY = hitGetDamageReturnPos(*(void**)((s32)mario + 0x208))->y;
            if (marioBgmodeChk() == 1 || *(f32*)((s32)mario + 0x90) - returnY <= float_50_80422bb8) hit = marioChkLandon(fall, &landY);
            else hit = 0;
            if (hit == 0) {
                *(f32*)((s32)mario + 0x90) += fall;
            } else {
                *(void**)((s32)mario + 0x1E8) = hit;
                *(void**)((s32)mario + 0x1EC) = 0;
                *(f32*)((s32)mario + 0x90) = landY;
                if (*(s8*)((s32)mario + 0x3C) == 2) marioChgPose(str_KPA_D_2_802f38d8);
                else marioChgPose(str_M_D_2_80422b90);
                code = (marioBgmodeChk() == 0) ? quake_evt : quake_bgmode_evt;
                evtEntry(code, 0, 0);
                psndSFXOn_3D(0xCB, (void*)((s32)mario + 0x8C));
                psndSFXOn_3D(0x197, (void*)((s32)mario + 0x8C));
                mario_simple_kemuri();
            }
            if (*(void**)((s32)mario + 0x1E8) == 0) return;
        case 30:
            *(s32*)((s32)mario + 0x48) = 50;
            *(s32*)((s32)mario + 0x44) = 31;
        case 31:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                if (*(s8*)((s32)mario + 0x3C) == 0) pouchAddHP(-1);
                if (pouchGetHP() <= 0) {
                    *(s32*)((s32)mario + 0x44) = 33;
                    marioUpdateCamPos();
                } else {
                    if (*(s8*)((s32)mario + 0x3C) != 2) marioChgPose(str_M_U_3_80422b98);
                    *(s32*)((s32)mario + 0x48) = 12;
                    *(s32*)((s32)mario + 0x44) = 32;
                    goto wait_recovery;
                }
            }
            break;
        case 32:
wait_recovery:
            if (--*(s32*)((s32)mario + 0x48) <= 0) {
                marioChgMot(0);
                marioUpdateCamPos();
                npcStartForEvent();
            }
            break;
    }
    return;
}

#pragma opt_common_subs reset
#pragma opt_propagation reset
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 marioWaitDamageReturnFall(void) {
    s32 mario = (s32)marioGetPtr();
    DamageVec pos;
    f32 x;
    f32 y;
    f32 z;
    u32 px;
    u32 py;

    if (*(u16*)(mario + 0x2E) != 0x1F) {
        return -1;
    }
    if (*(s32*)(mario + 0x44) >= 20) {
        px = *(u32*)(mario + 0x8C);
        py = *(u32*)(mario + 0x90);
        *(u32*)&pos.x = px;
        *(u32*)&pos.y = py;
        *(u32*)&pos.z = *(u32*)(mario + 0x94);
        marioGetScreenPos(&pos, &x, &y, &z);
        if (marioChkInScreen((s32)x, (s32)y) != 0) {
            return 1;
        }
    }
    return 0;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma opt_propagation off
#pragma opt_common_subs off
u8 mot_damageToge(void) {
    typedef DamageVec Vec;

    extern void marioYoshiForceCancel(void);
    extern void unk_800d058c(void);
    extern void marioChgEvtPose(char* pose);
    extern void marioChgPose(char* pose);
    extern void psndSFXOn_3D(s32 id, void* pos);
    extern Vec* hitGetDamageReturnPos(void* hit);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern void movePos(f32 speed, f32 angle, void* pos, f32* z);
    extern void* marioChkLandon(f32 velocityY, f32* y);
    extern void* evtEntry(void* script, s32 priority, s32 flags);
    extern s32 pouchAddHP(s16 amount);
    extern void marioChgMot(s32 mot);
    extern f32 float_0_80422b60;
    extern f32 float_4_80422b64;
    extern char str_M_D_7_80422b68[8];
    extern char str_M_D_6_80422b70[8];
    extern f32 float_1_80422b78;
    extern f32 float_10_80422b7c;
    extern f32 float_500_80422b80;
    extern f32 float_8_80422b84;
    extern f32 float_0p125_80422b88;
    extern f32 float_neg10_80422b8c;

    void* mario;
    void* hit;
    void* evtCode;
    Vec target;
    Vec temporary;
    Vec* hitPos;
    f32 landY;
    f32 fall;
    f32 rise;
    s32 timer;

    mario = marioGetPtr();
    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        marioClearSlitFloor();
        *(u32*)((s32)mario + 4) &= ~0x100;
        marioAdjustMoveDir();
        marioSlitForceCancel();
        marioYoshiForceCancel();
        unk_800d058c();
        marioNoUpdateCamPos();
        bomhei_use_cancel2();
        *(s32*)((s32)mario + 0x44) = 0;
        *(s16*)((s32)mario + 0x50) = 0;
        *(f32*)((s32)mario + 0x180) = float_0_80422b60;
        *(f32*)((s32)mario + 0x2B8) = *(f32*)((s32)mario + 0x90);
        *(s32*)((s32)mario + 0x48) = 5;
        *(f32*)((s32)mario + 0x2C4) = float_4_80422b64;
    }

    switch (*(s32*)((s32)mario + 0x44)) {
        case 0:
            if (--*(s32*)((s32)mario + 0x48) > 0) {
                break;
            }
            if ((*(u16*)((s32)mario + 0x30) == 0x10) || (*(u16*)((s32)mario + 0x30) == 0x11)) {
                marioChgEvtPose(str_M_D_7_80422b68);
            } else {
                marioChgPose(str_M_D_6_80422b70);
            }
            psndSFXOn_3D(0xBA, (void*)((s32)mario + 0x8C));
            mario_simple_kemuri();
            *(s32*)((s32)mario + 0x44) = 0xB;
        case 0xB:
            rise = *(f32*)((s32)mario + 0x2C4);
            *(f32*)((s32)mario + 0x90) += rise;
            rise -= float_1_80422b78;
            if (rise < float_10_80422b7c) {
                rise = float_10_80422b7c;
            }
            *(f32*)((s32)mario + 0x2C4) = rise;
            if (*(f32*)((s32)mario + 0x90) < float_500_80422b80 + *(f32*)((s32)mario + 0x2B8)) {
                break;
            }
            {
                if (*(void**)((s32)mario + 0x208) != 0) {
                    hitPos = hitGetDamageReturnPos(*(void**)((s32)mario + 0x208));
                    temporary = vec3_802f3898[0];
                    temporary.x = hitPos->x;
                    temporary.y = hitPos->y;
                    temporary.z = hitPos->z;
                    target = temporary;
                } else {
                    target = vec3_802f38a4[0];
                }
                *(f32*)((s32)mario + 0x180) = float_8_80422b84;
                *(s32*)((s32)mario + 0x48) =
                    (s16)(s32)(distABf(target.x, target.z, *(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94)) *
                                float_0p125_80422b88);
                *(f32*)((s32)mario + 0x1A4) =
                    angleABf(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x94), target.x, target.z);
                *(f32*)((s32)mario + 0x2C0) =
                    (target.y - *(f32*)((s32)mario + 0x2B8)) / (f32)*(s32*)((s32)mario + 0x48);
                *(s32*)((s32)mario + 0x44) = 0xC;
            }
            break;
        case 0xC:
            *(f32*)((s32)mario + 0xF0) += *(f32*)((s32)mario + 0x2C0);
            movePos(*(f32*)((s32)mario + 0x180), *(f32*)((s32)mario + 0x1A4),
                    (void*)((s32)mario + 0x8C), (f32*)((s32)mario + 0x94));
            *(f32*)((s32)mario + 0xEC) = *(f32*)((s32)mario + 0x8C);
            *(f32*)((s32)mario + 0xF4) = *(f32*)((s32)mario + 0x94);
            timer = *(s32*)((s32)mario + 0x48) - 1;
            *(s32*)((s32)mario + 0x48) = timer;
            if (timer <= 0) {
                *(s32*)((s32)mario + 0x44) = 0xD;
                *(u32*)mario |= 0x80;
                marioSetFallPara();
            }
            break;
        case 0xD:
            timer = *(s32*)((s32)mario + 0x48) - 1;
            *(s32*)((s32)mario + 0x48) = timer;
            if (timer <= 0) {
                *(s32*)((s32)mario + 0x44) = 0x14;
            }
            break;
        case 0x14:
            *(void**)((s32)mario + 0x1E8) = 0;
            fall = marioGetFallSpd();
            if (fall <= float_neg10_80422b8c) {
                fall = float_neg10_80422b8c;
            }
            hit = marioChkLandon(fall, &landY);
            if (hit == 0) {
                *(f32*)((s32)mario + 0x90) += fall;
            } else {
                marioChgPose(str_M_D_2_80422b90);
                if (marioBgmodeChk() == 0) {
                    evtCode = quake_evt;
                } else {
                    evtCode = quake_bgmode_evt;
                }
                evtEntry(evtCode, 0, 0);
                psndSFXOn_3D(0xCB, (void*)((s32)mario + 0x8C));
                psndSFXOn_3D(0x197, (void*)((s32)mario + 0x8C));
                mario_simple_kemuri();
                *(void**)((s32)mario + 0x1E8) = hit;
                *(void**)((s32)mario + 0x1EC) = 0;
                *(f32*)((s32)mario + 0x90) = landY;
            }
            if (*(void**)((s32)mario + 0x1E8) == 0) {
                return 0;
            }
        case 0x1E:
            *(s32*)((s32)mario + 0x48) = 0x32;
            *(s32*)((s32)mario + 0x44) = 0x1F;
        case 0x1F:
            timer = *(s32*)((s32)mario + 0x48) - 1;
            *(s32*)((s32)mario + 0x48) = timer;
            if (timer <= 0) {
                pouchAddHP(-1);
                if (pouchGetHP() <= 0) {
                    *(s32*)((s32)mario + 0x44) = 0x21;
                    marioUpdateCamPos();
                } else {
                    marioChgPose(str_M_U_3_80422b98);
                    *(s32*)((s32)mario + 0x48) = 0xC;
                    *(s32*)((s32)mario + 0x44) = 0x20;
                    goto wait_recovery;
                }
            }
            break;
        case 0x20:
        wait_recovery:
            timer = *(s32*)((s32)mario + 0x48) - 1;
            *(s32*)((s32)mario + 0x48) = timer;
            if (timer <= 0) {
                marioChgMot(0);
                marioUpdateCamPos();
            }
            break;
    }
    return 0;
}
#pragma opt_common_subs reset
#pragma opt_propagation reset

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 marioWaitDamageTogeReturnFall(void) {
    s32 mario = (s32)marioGetPtr();
    DamageVec pos;
    f32 x;
    f32 y;
    f32 z;
    u32 px;
    u32 py;

    if (*(u16*)(mario + 0x2E) != 0x20) {
        return -1;
    }
    if (*(s32*)(mario + 0x44) >= 20) {
        px = *(u32*)(mario + 0x8C);
        py = *(u32*)(mario + 0x90);
        *(u32*)&pos.x = px;
        *(u32*)&pos.y = py;
        *(u32*)&pos.z = *(u32*)(mario + 0x94);
        marioGetScreenPos(&pos, &x, &y, &z);
        if (marioChkInScreen((s32)x, (s32)y) != 0) {
            return 1;
        }
    }
    return 0;
}
