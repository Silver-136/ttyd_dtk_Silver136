#include "motion/mot_stay.h"

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
#pragma use_lmw_stmw on


u8 mot_stay(void) {
    extern void* marioGetPtr(void);
    extern void kpa_stay(void);
    extern void peach_stay(void);
    extern void marioClearJumpPara(void);
    extern s32 marioPriCheckJabara(void);
    extern void marioSoundInit(void);
    extern void marioResetCamFollowRate(void);
    extern s32 pouchGetHP(void);
    extern s32 pouchEquipCheckBadge(s32 item);
    extern void marioChgPose(char* pose);
    extern void marioChgEvtPose(char* pose);
    extern void marioChgPoseTime(char* pose, s32 time);
    extern s32 marioChkKey(void);
    extern s32 marioChkItemMotion(void);
    extern s32 marioGetHammerLevel(void);
    extern s32 marioSlitAbilityChk(void);
    extern s32 marioSlitButton(void);
    extern void motSlitCancel(void);
    extern void motSlitContinue(void);
    extern s32 marioChkJabara(void);
    extern s32 marioChkJump(void);
    extern s32 marioChkTransform(void);
    extern void marioAdjustMoveDir(void);
    extern void marioChgMot(s32 motion);
    extern void marioChgMot2(s32 motion);
    extern s32 sysMsec2Frame(s32 msec);
    extern u32 psndSFXOn_3D(s32 id, void* position);
    extern s32 strcmp(char* left, char* right);
    extern f32 float_0_80421010;
    extern char str_M_S_1_80421008;
    extern char str_M_I_S_80421014;
    extern char str_M_S_3_8042101c;
    extern char str_M_I_Y_80421024;
    extern char str_M_U_2_8042102c;
    extern char str_M_I_5_80421034;
    extern char str_M_S_1R_802c4398;
    extern char str_M_N_5A_802c43a0;

    void* player = marioGetPtr();
    s32 hp;
    s32 moving;
    s32 submotion;

#define FLAGS (*(u32*)((s32)player + 0x0))
#define DISP_FLAGS (*(u32*)((s32)player + 0x4))
#define TRIG_FLAGS (*(u32*)((s32)player + 0xC))
#define ANIM_NAME (*(char**)((s32)player + 0x18))
#define POSE_TIME (*(u16*)((s32)player + 0x28))
#define CHARACTER (*(s8*)((s32)player + 0x3C))
#define MOTION_TIMER (*(u8*)((s32)player + 0x40))
#define SUBMOTION (*(s32*)((s32)player + 0x44))
#define TIMER (*(s32*)((s32)player + 0x48))
#define AIR_TIMER (*(s16*)((s32)player + 0x50))
#define LOW_HP_TIMER (*(s16*)((s32)player + 0x58))
#define STAY_TIMER (*(s16*)((s32)player + 0x5A))
#define POS ((f32*)((s32)player + 0x8C))
#define JUMP_POS ((f32*)((s32)player + 0x11C))
#define BASE_SPEED (*(f32*)((s32)player + 0x180))
#define STICK_SENS (*(f32*)((s32)player + 0x194))
#define PAUSE_BUTTONS (*(u16*)((s32)player + 0x24A))
#define PRESSED_BUTTONS (*(u16*)((s32)player + 0x24C))
#define STICK_X (*(s8*)((s32)player + 0x252))
#define STICK_Y (*(s8*)((s32)player + 0x253))
#define STAY_SFX (*(s32*)((s32)player + 0x27C))
#define JABARA_TIMER (*(s32*)((s32)player + 0x2AC))
#define STAY_VAR (*(s32*)((s32)player + 0x2B0))
#define SLEEP_TIMER (*(s32*)((s32)player + 0x2B4))

    if (CHARACTER == 2) {
        kpa_stay();
        return 0;
    }
    if (CHARACTER == 1) {
        peach_stay();
        return 0;
    }

    if ((PAUSE_BUTTONS & 0x100) == 0) {
        TRIG_FLAGS &= ~0x00300000;
    }

    if ((TRIG_FLAGS & 1) != 0) {
        TRIG_FLAGS &= ~1;
        FLAGS &= ~0xF0000;
        marioClearJumpPara();
        AIR_TIMER = 0;
        TIMER = 0;
        SUBMOTION = 0;
        if (marioPriCheckJabara() == 0) {
            MOTION_TIMER = 0;
        } else {
            MOTION_TIMER = ((TRIG_FLAGS & 0x100000) != 0) ? 10 : 0;
        }
        TRIG_FLAGS &= ~0x80000;
        JUMP_POS[0] = POS[0];
        JUMP_POS[1] = POS[1];
        JUMP_POS[2] = POS[2];
        JABARA_TIMER = 0;
        STAY_VAR = 0;
        SLEEP_TIMER = 0;
        STAY_TIMER = 0;
        marioSoundInit();
        marioResetCamFollowRate();

        if ((FLAGS & 0x1000000) == 0) {
            if ((FLAGS & 0x800000) == 0) {
                if ((DISP_FLAGS & 0x20000000) != 0) {
                    marioChgMot2(0x15);
                    motSlitCancel();
                }
            } else {
                motSlitContinue();
            }

            if ((FLAGS & 0x400000) == 0) {
                hp = pouchGetHP();
                if (hp == 0) {
                    marioChgPose(&str_M_I_S_80421014);
                    psndSFXOn_3D(0x173, POS);
                } else if (hp >= 6 && marioChkKey() != 0) {
                    DISP_FLAGS |= 0x4000000;
                    marioChgPose(&str_M_S_3_8042101c);
                    LOW_HP_TIMER = 0;
                } else if (strcmp(ANIM_NAME, &str_M_I_Y_80421024) != 0) {
                    marioChgPose(&str_M_S_1_80421008);
                }
            } else {
                marioChgPoseTime(&str_M_U_2_8042102c, 10);
            }
        } else {
            marioChgPose(&str_M_S_1_80421008);
        }
        BASE_SPEED = float_0_80421010;
        FLAGS &= ~0x200000;
    }

    if ((FLAGS & 0x1000000) != 0) {
        marioChkJump();
        if (STICK_SENS > float_0_80421010) {
            if (STICK_X * STICK_X + STICK_Y * STICK_Y < 0xBD2 ||
                pouchEquipCheckBadge(0x143) != 0) {
                marioChgMot(1);
            } else {
                marioChgMot(2);
            }
        }
        return 0;
    }

    STAY_TIMER++;
    if (STAY_TIMER > sysMsec2Frame(1000) && (FLAGS & 0x400000) == 0 && marioChkKey() != 0) {
        hp = pouchGetHP();
        if (hp >= 6) {
            DISP_FLAGS |= 0x4000000;
            marioChgPose(&str_M_S_3_8042101c);
        } else if (SUBMOTION == 0 && strcmp(ANIM_NAME, &str_M_I_Y_80421024) != 0) {
            STAY_TIMER = 0;
            marioChgPose(&str_M_S_1_80421008);
        }
    }

    if ((FLAGS & 0x400000) != 0) {
        if (STICK_SENS > float_0_80421010) {
            marioChgMot(1);
        }
        return 0;
    }

    if (marioChkItemMotion() == 0) {
        if (marioGetHammerLevel() < 2) {
            moving = PRESSED_BUTTONS & 0x200;
        } else {
            moving = PAUSE_BUTTONS & 0x200;
        }
        if (moving == 0) {
            if (marioSlitAbilityChk() != 0 && marioSlitButton() == 1) {
                marioChgMot(0x15);
                return 0;
            }

            JABARA_TIMER++;
            if (JABARA_TIMER > 9) {
                JABARA_TIMER = 10;
                if (marioChkJabara() != 0) {
                    marioChgMot(0x14);
                    return 0;
                }
            }
            marioChkJump();
            marioChkTransform();

            if (STICK_SENS != float_0_80421010 && (TRIG_FLAGS & 0x100000) == 0) {
                if (STICK_X * STICK_X + STICK_Y * STICK_Y < 0xBD2 ||
                    pouchEquipCheckBadge(0x143) != 0) {
                    marioChgMot(1);
                } else {
                    marioChgMot(2);
                }
            }

            if ((FLAGS & 0x20000) == 0 && (DISP_FLAGS & 0x10000000) == 0 && pouchGetHP() < 6) {
                submotion = SUBMOTION;
                if (submotion == 0) {
                    TIMER++;
                    if (TIMER >= sysMsec2Frame(50000)) {
                        SUBMOTION = 1;
                    }
                } else if (submotion == 1) {
                    marioChgEvtPose(&str_M_I_5_80421034);
                    marioAdjustMoveDir();
                    STAY_SFX = psndSFXOn_3D(0xEF, POS);
                    TIMER = 50;
                    SUBMOTION = 2;
                } else if (submotion == 2) {
                    TIMER--;
                    if (TIMER < 0) {
                        marioChgPose(&str_M_S_1_80421008);
                        TIMER = 100;
                        SUBMOTION = 3;
                    }
                } else if (submotion == 3) {
                    TIMER--;
                    if (TIMER < 0) {
                        marioChgEvtPose(&str_M_N_5A_802c43a0);
                        POSE_TIME = 0;
                        TIMER = 0;
                        SUBMOTION = 4;
                        SLEEP_TIMER = 0;
                        STAY_SFX = psndSFXOn_3D(0xE4, POS);
                    }
                } else if (submotion == 4) {
                    if (SLEEP_TIMER < sysMsec2Frame(10000)) {
                        SLEEP_TIMER++;
                    }
                    TIMER++;
                    if (TIMER >= 120) {
                        TIMER = 0;
                        STAY_SFX = psndSFXOn_3D(0xE4, POS);
                    }
                }
            } else if (SUBMOTION > 0 && strcmp(ANIM_NAME, &str_M_S_1_80421008) != 0 &&
                       strcmp(ANIM_NAME, &str_M_S_1R_802c4398) != 0) {
                marioChgPose(&str_M_S_1_80421008);
                SUBMOTION = 0;
            }
        }
    }

#undef SLEEP_TIMER
#undef STAY_VAR
#undef JABARA_TIMER
#undef STAY_SFX
#undef STICK_Y
#undef STICK_X
#undef PRESSED_BUTTONS
#undef PAUSE_BUTTONS
#undef STICK_SENS
#undef BASE_SPEED
#undef JUMP_POS
#undef POS
#undef STAY_TIMER
#undef LOW_HP_TIMER
#undef AIR_TIMER
#undef TIMER
#undef SUBMOTION
#undef MOTION_TIMER
#undef CHARACTER
#undef POSE_TIME
#undef ANIM_NAME
#undef TRIG_FLAGS
#undef DISP_FLAGS
#undef FLAGS
    return 0;
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
        y = *(u8*)((s32)mario + 0x253);
        x = (s8)x;
        y = (s8)y;
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
