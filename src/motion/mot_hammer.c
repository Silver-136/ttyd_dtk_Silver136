#include "motion/mot_hammer.h"
extern void* pouchGetPtr(void);
extern void* marioGetPtr(void);
extern f32 revise360(f32 angle);
extern s32 pouchGetHammerLv(void);
extern void marioChgPose(char* pose);
extern u32 psndSFXOn_3D(s32 id, void* position);
extern s32 marioAnimeId(void);
extern void animPoseSetLocalTime(f32 time, s32 poseId);
extern void marioPaperOff(void);
extern void marioAdjustMoveDir(void);

s32 marioGetHammerLevel(void) {

    return *(s8*)((s32)pouchGetPtr() + 0x99);
}

u8 mot_hammer(void) {
    extern void* hitEstimate(s32* kind);
    extern u32 hitGetAttr(void* hit);
    extern f32 toMovedir(f32 angle);
    extern f32 toMovedir2(f32 angle, f32 offset);
    extern void* evtEntry(void* code, s32 priority, s32 flags);
    extern void marioChgMot(s32 motion);
    extern f32 float_0_80420ab8;
    extern f32 float_90_80420ac0;
    extern f32 float_270_80420ac4;
    extern f32 float_180_80420ac8;
    extern f32 float_30_80420ae4;
    extern const f32 float_neg30_80420b7c;
    extern char str_M_H_1_80420b88;
    extern char str_M_H_2_80420b80;
    extern char str_M_H_4_80420b98;
    extern char str_M_H_5_80420b90;
    extern char str_M_H_7_80420ba8;
    extern char str_M_H_8_80420ba0;
    extern s32 lv1quake_evt;
    extern s32 lv2quake_evt;
    extern s32 lv3quake_evt;

    void* player = marioGetPtr();
    void* hit;
    f32 angle;
    f32 side;
    s32 kind[2];
    s32 hammerLevel;
    s32 frame;
    s32 impact;

#define FLAGS (*(u32*)((s32)player + 0x0))
#define DISP_FLAGS (*(u32*)((s32)player + 0x4))
#define TRIG_FLAGS (*(u32*)((s32)player + 0xC))
#define POSE_TIME (*(u16*)((s32)player + 0x28))
#define SUBMOTION (*(s32*)((s32)player + 0x44))
#define TIMER (*(s32*)((s32)player + 0x48))
#define AIR_TIMER (*(s16*)((s32)player + 0x50))
#define POS ((f32*)((s32)player + 0x8C))
#define BASE_SPEED (*(f32*)((s32)player + 0x180))
#define WORLD_DIR (*(f32*)((s32)player + 0x1A0))
#define VIEW_DIR (*(f32*)((s32)player + 0x1A4))
#define DISP_DIR (*(f32*)((s32)player + 0x1AC))
#define DISP_TARGET (*(f32*)((s32)player + 0x1B0))
#define INTERACT (*(void**)((s32)player + 0x200))
#define HAMMER_HIT (*(void**)((s32)player + 0x204))
#define POSE_IDS ((s32*)((s32)player + 0x22C))
#define HIT_TIME (*(s32*)((s32)player + 0x2B8))
#define HAMMER_ANGLE (*(f32*)((s32)player + 0x2BC))
#define HIT_KIND (*(s32*)((s32)player + 0x2C0))

    if ((TRIG_FLAGS & 1) != 0) {
        TRIG_FLAGS &= ~1;
        FLAGS &= ~0xF0000;
        FLAGS |= 0x80;

        angle = float_0_80420ab8;
        if ((DISP_FLAGS & 0x80000000) != 0) {
            if (DISP_DIR >= float_90_80420ac0 && DISP_DIR <= float_270_80420ac4) {
                angle = float_30_80420ae4;
            } else {
                angle = float_neg30_80420b7c;
            }
        }
        if (DISP_DIR >= float_90_80420ac0 && DISP_DIR <= float_270_80420ac4) {
            angle = revise360(float_180_80420ac8 + angle);
        } else {
            angle = revise360(angle);
        }
        HAMMER_ANGLE = angle;
        TIMER = 0;
        AIR_TIMER = 0;
        SUBMOTION = 0;
        BASE_SPEED = float_0_80420ab8;
        HAMMER_HIT = 0;

        hit = hitEstimate(kind);
        HAMMER_HIT = hit;
        if (hit != 0) {
            side = (DISP_FLAGS & 0x80000000) != 0 ? float_neg30_80420b7c : float_0_80420ab8;
            if (DISP_DIR >= float_90_80420ac0 && DISP_DIR <= float_270_80420ac4) {
                VIEW_DIR = toMovedir2(float_180_80420ac8, side);
            } else {
                VIEW_DIR = toMovedir2(float_0_80420ab8, side);
            }
        }

        hammerLevel = pouchGetHammerLv();
        if (hammerLevel == 1) {
            marioChgPose(kind[0] == 7 ? &str_M_H_2_80420b80 : &str_M_H_1_80420b88);
        } else if (hammerLevel == 2) {
            marioChgPose(kind[0] == 7 ? &str_M_H_5_80420b90 : &str_M_H_4_80420b98);
        } else if (hammerLevel == 3) {
            marioChgPose(kind[0] == 7 ? &str_M_H_8_80420ba0 : &str_M_H_7_80420ba8);
        }

        if (kind[0] == 0) {
            FLAGS &= ~0x800;
        } else {
            HIT_TIME = kind[0] == 7 ? 3 : kind[0];
            FLAGS |= 0x800;
        }
        psndSFXOn_3D(0x15A + hammerLevel, POS);
        HIT_KIND = kind[0];
    }

    if (HAMMER_ANGLE >= float_90_80420ac0 && HAMMER_ANGLE <= float_270_80420ac4) {
        DISP_DIR = float_180_80420ac8;
    } else {
        DISP_DIR = float_0_80420ab8;
    }
    DISP_TARGET = DISP_DIR;
    WORLD_DIR = toMovedir(HAMMER_ANGLE);
    VIEW_DIR = WORLD_DIR;

    impact = 0;
    if ((FLAGS & 0x800) != 0) {
        frame = TIMER <= HIT_TIME ? TIMER : HIT_TIME;
        if (TIMER == HIT_TIME) {
            impact = 1;
        }
    } else {
        frame = TIMER <= 10 ? TIMER : 10;
        if (TIMER == 10) {
            impact = 1;
        }
    }
    POSE_TIME = frame;
    animPoseSetLocalTime((f32)frame, POSE_IDS[marioAnimeId()]);

    if (impact) {
        hammerLevel = *(s8*)((s32)pouchGetPtr() + 0x99);
        if (hammerLevel == 1) {
            evtEntry(&lv1quake_evt, 0, 0);
        } else if (hammerLevel == 2) {
            evtEntry(&lv2quake_evt, 0, 0);
        } else if (hammerLevel == 3) {
            evtEntry(&lv3quake_evt, 0, 0);
        }
        SUBMOTION = 1;
        INTERACT = HAMMER_HIT;
        psndSFXOn_3D(0x15E + hammerLevel, POS);
    }

    TIMER++;
    if (TIMER > 20) {
        FLAGS &= ~0x80;
        marioChgMot(0);
    }

#undef HIT_KIND
#undef HAMMER_ANGLE
#undef HIT_TIME
#undef POSE_IDS
#undef HAMMER_HIT
#undef INTERACT
#undef DISP_TARGET
#undef DISP_DIR
#undef VIEW_DIR
#undef WORLD_DIR
#undef BASE_SPEED
#undef POS
#undef AIR_TIMER
#undef TIMER
#undef SUBMOTION
#undef POSE_TIME
#undef TRIG_FLAGS
#undef DISP_FLAGS
#undef FLAGS
    return 0;
}

s32 marioChkHammer2(void) {
    extern s32 strcmp(char*, char*);
    extern void marioChgPose(char*);
    extern void marioVoiceGlareOff(void);
    extern void marioVoiceGlareOn(void);
    extern s32 marioGetRub(s32, void*, void*, void*);
    extern char str_M_I_Y_80420b74;
    extern char str_M_S_1_80420b6c;
    extern f32 float_0_80420ab8;

    void* mario = marioGetPtr();
    void* pouch = pouchGetPtr();
    s32 count;
    s32 dir;
    s32 stack8;

    if (*(s8*)((s32)pouch + 0x99) < 2) {
        return 0;
    }
    if ((*(u16*)((s32)mario + 0x24A) & 0x200) == 0) {
        if ((*(u16*)((s32)mario + 0x24A) & 0x100) == 0) {
            if (strcmp(*(char**)((s32)mario + 0x18), &str_M_I_Y_80420b74) == 0 &&
                *(u16*)((s32)mario + 0x2E) == 0) {
                marioChgPose(&str_M_S_1_80420b6c);
                marioVoiceGlareOff();
            }
            *(u8*)((s32)mario + 0x40) = 0;
        }
        return 0;
    }
    if ((*(u32*)((s32)mario + 0x0) & 0x02000000) != 0) {
        *(u8*)((s32)mario + 0x40) = 0;
    } else {
        *(u8*)((s32)mario + 0x40) = *(u8*)((s32)mario + 0x40) + 1;
    }
    if (*(f32*)((s32)mario + 0x194) != float_0_80420ab8 &&
        *(s8*)((s32)mario + 0x40) > 10) {
        if (*(u16*)((s32)mario + 0x2E) == 0 && (*(u32*)((s32)mario + 0x4) & 0x40000000) == 0) {
            marioChgPose(&str_M_I_Y_80420b74);
            marioVoiceGlareOn();
        }
    }
    if (*(s8*)((s32)mario + 0x40) >= 0x32) {
        *(u8*)((s32)mario + 0x40) = 0x32;
        if (*(u16*)((s32)mario + 0x2E) == 0 && (*(u32*)((s32)mario + 0x4) & 0x40000000) == 0) {
            marioChgPose(&str_M_I_Y_80420b74);
            marioVoiceGlareOn();
        }
    }
    if (marioGetRub(0x200, &dir, &count, &stack8) != 1) {
        return 0;
    }
    *(f32*)((s32)mario + 0x180) = float_0_80420ab8;
    if (count < 1) {
        return 0;
    }
    *(u8*)((s32)mario + 0x43) = dir;
    return 1;
}


void mot_hammer2(void) {
    extern void* marioGetPtr(void);
    extern s32 pouchGetHammerLv(void);
    extern void marioChgPose(char* pose);
    extern void marioPaperOn(char* pose);
    extern void marioPaperOff(void);
    extern void marioChgPaper(char* pose);
    extern void marioPaperLightOff(void);
    extern u32 psndSFXOn_3D(s32 id, void* position);
    extern void psndSFXOff(u32 id);
    extern s32 marioAnimeId(void);
    extern void animPoseSetLocalTime(f32 time, s32 poseId);
    extern s32 marioGetRub(s32 type, void* dir, void* count, void* scratch);
    extern f64 revise360(f64 angle);
    extern void marioAdjustMoveDir(void);
    extern void marioChgMotSub(s32 motion, s32 param);
    extern s32 sysMsec2Frame(s32 msec);
    extern void* effUltraHammerEntry(f32, f32, f32, f32, s32, s32);
    extern char str_M_H_3_80420b18;
    extern char str_M_H_6_80420b20;
    extern char str_M_H_9_80420b28;
    extern char str_P_H_1A_802c3f8c;
    extern char str_P_H_1B_802c3f84;
    extern char str_P_H_1C_802c3f7c;
    extern char str_p_kaiten_h_802c3f70;
    extern char str_M_H_3A_802c3f94, str_M_H_6A_802c3f9c, str_M_H_9A_802c3fa4;
    extern char str_M_H_3B_802c3fac, str_M_H_6B_802c3fb4, str_M_H_9B_802c3fbc;
    extern char str_M_S_1_80420b6c;
    extern f32 float_0_80420ab8;
    extern f32 float_1_80420b48;
    extern f32 float_6_80420b30;
    extern f32 float_18_80420b58;
    extern f32 float_24_80420b64;
    extern f32 float_30_80420ae4;
    extern f32 float_36_80420aec;
    extern f32 float_42_80420b4c;
    extern f32 float_54_80420b44;
    extern f32 float_72_80420b34;
    extern f32 float_180_80420ac8;
    extern f32 float_270_80420ac4;
    extern f32 float_38_80420b0c;
    extern f32 float_15_80420b10;
    extern f32 float_10_80420b14;
    extern f32 float_20_80420af0, float_22_80420b68;
    extern f32 float_0p01_80420b00;
    extern f32 float_0p005_80420b3c, float_0p2_80420b5c, float_0p3_80420b50;
    extern f32 float_0p4_80420b38, float_1p5_80420b60, float_1p6_80420b54;
    extern f32 float_1p7_80420b40;
    extern f32 vec3_802c3ec8[];

    void* player = marioGetPtr();
    u8 dir[4];
    u8 count[4];
    u8 scratch[4];
    f32 low;
    f32 high;
    f32 threshold;
    f32 direction;
    s32 level;
    s32 rub;

#define FLAGS (*(u32*)((s32)player + 0x0))
#define DISP_FLAGS (*(u32*)((s32)player + 0x4))
#define TRIG_FLAGS (*(u32*)((s32)player + 0xC))
#define BUTTONS (*(u32*)((s32)player + 0x14))
#define SUBMOTION (*(s32*)((s32)player + 0x44))
#define TIMER (*(s32*)((s32)player + 0x48))
#define POS ((f32*)((s32)player + 0x8C))
#define BASE_SPEED (*(f32*)((s32)player + 0x180))
#define DISP_DIR (*(f32*)((s32)player + 0x1AC))
#define DISP_TARGET (*(f32*)((s32)player + 0x1B0))
#define POSE_IDS ((s32*)((s32)player + 0x22C))
#define MOTION_COUNT (*(s8*)((s32)player + 0x41))
#define CHARGE (*(s8*)((s32)player + 0x42))
#define AIR_TIMER (*(s16*)((s32)player + 0x50))
#define HAMMER_HIT (*(void**)((s32)player + 0x204))
#define CHARGE_RATE (*(f32*)((s32)player + 0x2C0))
#define CHARGE_ANGLE (*(f32*)((s32)player + 0x2C8))
#define CHARGE_FLAGS (*(u16*)((s32)player + 0x2CC))
#define SOUND_ID (*(s32*)((s32)player + 0x2D0))
#define HIT_COUNT (*(s16*)((s32)player + 0x2C4))

    if ((TRIG_FLAGS & 1) != 0) {
        TRIG_FLAGS &= ~1;
        FLAGS &= ~0xF0000;
        CHARGE_FLAGS = 0;
        SUBMOTION = 0;
        SOUND_ID = -1;
        *(f32*)((s32)player + 0x1B4) = DISP_DIR;
    }

    switch (SUBMOTION) {
        case 0:
            FLAGS |= 0x80;
            DISP_FLAGS |= 4;
            MOTION_COUNT = 0;
            CHARGE = 0;
            BASE_SPEED = float_0_80420ab8;
            AIR_TIMER = 0;
            SUBMOTION = 1;
            TIMER = 30;
            if (CHARGE == 3) {
                CHARGE_RATE = float_38_80420b0c;
            } else if (CHARGE <= 2) {
                CHARGE_RATE = float_15_80420b10;
            } else {
                CHARGE_RATE = float_10_80420b14;
            }
            level = pouchGetHammerLv();
            if (level == 1) marioChgPose(&str_M_H_3_80420b18);
            else if (level == 2) marioChgPose(&str_M_H_6_80420b20);
            else if (level == 3) marioChgPose(&str_M_H_9_80420b28);
            HAMMER_HIT = 0;
            SUBMOTION = 1;
            TIMER = 1;
            break;
        case 1:
            TIMER--;
            if (TIMER > 0) return;
            if (DISP_TARGET == float_180_80420ac8 && *(s8*)((s32)player + 0x43) == 1)
                DISP_FLAGS |= 0x80000000;
            if (DISP_TARGET == float_0_80420ab8 && *(s8*)((s32)player + 0x43) == 0)
                DISP_FLAGS |= 0x20000000;
            marioPaperOn(&str_p_kaiten_h_802c3f70);
            marioChgPaper(&str_P_H_1C_802c3f7c);
            marioPaperLightOff();
            SOUND_ID = psndSFXOn_3D(0x162, POS);
            CHARGE_ANGLE = float_0_80420ab8;
            SUBMOTION = 10;
            TIMER = 30;
        case 10:
            rub = marioGetRub(0x200, dir, count, scratch);
            if (rub == 1 && ++MOTION_COUNT > 20) MOTION_COUNT = 20;
            if (MOTION_COUNT < 2) {
                low = float_18_80420b58;
                high = float_24_80420b64;
                CHARGE = 1;
            } else if (MOTION_COUNT < 3) {
                low = float_30_80420ae4;
                high = float_36_80420aec;
                CHARGE = 2;
            } else if (MOTION_COUNT < 4) {
                low = float_42_80420b4c;
                high = float_54_80420b44;
                CHARGE = 3;
            } else {
                low = float_72_80420b34;
                high = float_72_80420b34;
                SUBMOTION = 11;
                CHARGE = 3;
                marioChgPaper(&str_P_H_1B_802c3f84);
            }
            marioChgPaper(&str_P_H_1A_802c3f8c);
            threshold = MOTION_COUNT < 2 ? low : high;
            if ((CHARGE_FLAGS & 1) == 0) {
                CHARGE_ANGLE += float_6_80420b30;
                if (CHARGE_ANGLE >= threshold) {
                    CHARGE_ANGLE = threshold;
                    CHARGE_FLAGS |= 1;
                    CHARGE_FLAGS |= 0x1000;
                }
            } else if ((CHARGE_FLAGS & 0x10) == 0) {
                CHARGE_ANGLE += float_1_80420b48;
                if (CHARGE_ANGLE >= high) {
                    CHARGE_ANGLE = high;
                    CHARGE_FLAGS |= 0x10;
                }
            } else {
                CHARGE_ANGLE -= float_1_80420b48;
                if (CHARGE_ANGLE <= low) {
                    CHARGE_ANGLE = low;
                    CHARGE_FLAGS &= ~0x10;
                }
            }
            if (CHARGE_FLAGS & 0x1000) {
                CHARGE_FLAGS &= ~0x1000;
                *(f32*)((s32)player + 0x154) =
                    CHARGE == 1 ? float_0p2_80420b5c :
                    (CHARGE == 2 ? float_0p3_80420b50 : float_0p4_80420b38);
                *(f32*)((s32)player + 0x158) = float_0p005_80420b3c;
                *(f32*)((s32)player + 0x68) =
                    CHARGE == 1 ? float_1p5_80420b60 :
                    (CHARGE == 2 ? float_1p6_80420b54 : float_1p7_80420b40);
                *(f32*)((s32)player + 0x6C) = vec3_802c3ec8[0];
                *(f32*)((s32)player + 0x70) = vec3_802c3ec8[1];
                *(f32*)((s32)player + 0x74) = vec3_802c3ec8[2];
                if (!(CHARGE_FLAGS & 0x2000)) {
                    CHARGE_FLAGS |= 0x2000;
                    level = pouchGetHammerLv();
                    if (level == 1) marioChgPose(&str_M_H_3A_802c3f94);
                    else if (level == 2) marioChgPose(&str_M_H_6A_802c3f9c);
                    else if (level == 3) marioChgPose(&str_M_H_9A_802c3fa4);
                }
            }
            animPoseSetLocalTime(CHARGE_ANGLE / float_6_80420b30, POSE_IDS[marioAnimeId()]);
        case 11:
            if ((BUTTONS & 0x200) == 0) {
                if (SOUND_ID != -1) {
                    psndSFXOff(SOUND_ID);
                    SOUND_ID = -1;
                }
                *(f32*)((s32)player + 0x154) = float_0_80420ab8;
                *(f32*)((s32)player + 0x158) = float_0p01_80420b00;
                level = pouchGetHammerLv();
                if (level == 1) marioChgPose(&str_M_H_3B_802c3fac);
                else if (level == 2) marioChgPose(&str_M_H_6B_802c3fb4);
                else if (level == 3) marioChgPose(&str_M_H_9B_802c3fbc);
                marioChgPaper(&str_P_H_1C_802c3f7c);
                SUBMOTION = 20;
                TIMER = CHARGE == 3 ? 48 : (CHARGE == 2 ? 66 : 72);
                HIT_COUNT = 0;
                CHARGE_RATE = CHARGE == 3 ? float_38_80420b0c :
                              (CHARGE == 2 ? float_22_80420b68 : float_15_80420b10);
                effUltraHammerEntry(POS[0], POS[1] + float_20_80420af0, POS[2],
                                    float_1_80420b48, *(s8*)((s32)player + 0x43) != 0,
                                    TIMER);
            }
            break;
        case 20:
            direction = DISP_DIR;
            HIT_COUNT = 10;
            DISP_FLAGS |= 0x20000000;
            DISP_DIR += CHARGE_RATE * (*(s8*)((s32)player + 0x43) ? -1.0f : 1.0f);
            if ((s32)(direction / 360.0f) != (s32)(DISP_DIR / 360.0f)) {
                psndSFXOn_3D(0x163, POS);
            }
            if (HIT_COUNT == 1) psndSFXOn_3D(0x163, POS);
            TIMER--;
            if (CHARGE > 2 && TIMER == 10) {
                marioPaperOff();
                marioChgPose(&str_M_S_1_80420b6c);
                DISP_FLAGS &= ~0xA0000004;
            }
            if (TIMER <= 0) {
                if (CHARGE < 3) {
                    TIMER = sysMsec2Frame(300);
                } else {
                    DISP_DIR = (f32)revise360(DISP_DIR);
                    marioAdjustMoveDir();
                    TIMER = 30;
                }
                SUBMOTION = 22;
            }
            break;
        case 22:
            if (--TIMER <= 0) {
                if (CHARGE < 3) {
                    marioPaperOff();
                    marioChgPose(&str_M_S_1_80420b6c);
                    DISP_FLAGS &= ~0xA0000004;
                    DISP_DIR = (f32)revise360(DISP_DIR);
                    marioAdjustMoveDir();
                }
                DISP_FLAGS &= ~0x20000000;
                SUBMOTION = 23;
                TIMER = 1;
                TIMER--;
                if (TIMER <= 0) {
                    TIMER = 1;
                    TIMER--;
                    if (TIMER <= 0) {
                        DISP_FLAGS &= ~4;
                        FLAGS &= ~0x80;
                        marioAdjustMoveDir();
                        marioChgMotSub(0, 0);
                    }
                }
            }
            break;
    }
    DISP_TARGET = DISP_DIR;

#undef CHARGE_FLAGS
#undef SOUND_ID
#undef HIT_COUNT
#undef CHARGE_ANGLE
#undef CHARGE_RATE
#undef HAMMER_HIT
#undef AIR_TIMER
#undef CHARGE
#undef MOTION_COUNT
#undef POSE_IDS
#undef DISP_TARGET
#undef DISP_DIR
#undef BASE_SPEED
#undef POS
#undef TIMER
#undef SUBMOTION
#undef BUTTONS
#undef TRIG_FLAGS
#undef DISP_FLAGS
#undef FLAGS
}

f32 marioGetAngleKaitenHammer(void) {
    extern f32 float_270_80420ac4;
    extern const f32 float_neg1_80420b04;
    extern const f32 float_neg2_80420b08;

    void* mario = marioGetPtr();
    if (*(u16*)((s32)mario + 0x2E) != 0x13) {
        return float_neg1_80420b04;
    }
    if (*(s32*)((s32)mario + 0x44) != 0x14) {
        return float_neg2_80420b08;
    }
    return revise360(float_270_80420ac4 - *(f32*)((s32)mario + 0x1AC));
}

void mot_ham2_post(void) {
    extern f32 float_0_80420ab8;
    extern const f32 float_0p01_80420b00;
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0x4);
    if (flags & 0x40000000) {
        marioPaperOff();
    }
    {
        void* mario2 = marioGetPtr();
        f32 small = float_0p01_80420b00;
        f32 zero = float_0_80420ab8;
        *(f32*)((s32)mario2 + 0x148) = zero;
        *(f32*)((s32)mario2 + 0x158) = small;
    }
    marioPaperOff();
    *(u32*)((s32)mario + 0x4) &= ~0x100;
    *(u32*)((s32)mario + 0x4) &= ~0x8;
    *(u32*)((s32)mario + 0x4) &= ~0x10;
    *(u32*)((s32)mario + 0x4) &= ~0x4;
    *(u32*)((s32)mario + 0x0) &= ~0x80;
    marioAdjustMoveDir();
}

void* hitEstimate(s32* outKind) {
    extern f32 toMovedir2(f32, f32);
    extern void sincosf(f32, f32*, f32*);
    extern f64 cos(f64);
    extern void* hitCheckVecFilter(void*, s32);
    extern u32 hitGetAttr(void*);
    extern f32 float_0_80420ab8;
    extern f32 float_90_80420ac0;
    extern f32 float_270_80420ac4;
    extern f32 float_180_80420ac8;
    extern f32 float_30_80420ae4;
    extern const f32 float_neg20_80420abc;
    extern const f32 float_3p1416_80420acc;
    extern const f32 float_4_80420ad0;
    extern const f32 float_14_80420ad4;
    extern const f32 float_1p2217_80420ad8;
    extern const f32 float_5_80420adc;
    extern const f32 float_26_80420ae0;
    extern const f32 float_0p7854_80420ae8;
    extern const f32 float_20_80420af0;
    extern const f32 float_34_80420af4;
    extern const f32 float_37_80420af8;
    extern const f32 float_13_80420afc;

    typedef struct Vec { f32 x, y, z; } Vec;
    typedef struct Probe {
        Vec base;
        Vec pos;
        Vec dir;
        u8 pad[0x18];
        f32 radius;
    } Probe;

    void* mario;
    void* hit;
    Probe probe;
    f32 sinv;
    f32 cosv;
    f32 angle;
    f32 side;
    f32 height;
    f32 dist;
    s32 i;

    mario = marioGetPtr();
    *outKind = 0;

    side = float_0_80420ab8;
    if ((*(u32*)((s32)marioGetPtr() + 0x4) & 1) != 0) {
        side = float_neg20_80420abc;
    }

    if (*(f32*)((s32)mario + 0x1AC) >= float_90_80420ac0 &&
        *(f32*)((s32)mario + 0x1AC) <= float_270_80420ac4) {
        angle = toMovedir2(float_180_80420ac8, side);
    } else {
        angle = toMovedir2(float_0_80420ab8, side);
    }
    sincosf(angle, &cosv, &sinv);

    height = *(f32*)((s32)mario + 0x1BC);
    probe.base.x = float_0_80420ab8;
    probe.base.y = -(f32)cos(float_3p1416_80420acc);
    probe.base.z = float_0_80420ab8;
    probe.pos.x = *(f32*)((s32)mario + 0x8C);
    probe.pos.y = *(f32*)((s32)mario + 0x90) + height + float_4_80420ad0;
    probe.pos.z = *(f32*)((s32)mario + 0x94);
    probe.dir.x = cosv;
    probe.dir.y = probe.base.y;
    probe.dir.z = sinv;
    probe.radius = float_14_80420ad4;
    hit = hitCheckVecFilter(&probe, 0);
    if (hit != 0 && (hitGetAttr(hit) & 0x800005) == 0) {
        *outKind = 7;
        return hit;
    }

    probe.base.y = -(f32)cos(float_1p2217_80420ad8);
    probe.pos.x = cosv * float_5_80420adc + *(f32*)((s32)mario + 0x8C);
    probe.pos.y = *(f32*)((s32)mario + 0x90) + height;
    probe.pos.z = sinv * float_5_80420adc + *(f32*)((s32)mario + 0x94);
    probe.dir.x = cosv;
    probe.dir.y = probe.base.y;
    probe.dir.z = sinv;
    probe.radius = float_26_80420ae0;
    hit = hitCheckVecFilter(&probe, 0);
    if (hit != 0 && (hitGetAttr(hit) & 0x800005) == 0) {
        if (*(f32*)((s32)&probe + 0x28) - *(f32*)((s32)mario + 0x90) <= float_30_80420ae4) {
            *outKind = 5;
        } else {
            *outKind = 4;
        }
        return hit;
    }

    probe.base.y = -(f32)cos(float_0p7854_80420ae8);
    probe.pos.x = cosv * float_5_80420adc + *(f32*)((s32)mario + 0x8C);
    probe.pos.y = *(f32*)((s32)mario + 0x90) + height;
    probe.pos.z = sinv * float_5_80420adc + *(f32*)((s32)mario + 0x94);
    probe.dir.x = cosv;
    probe.dir.y = probe.base.y;
    probe.dir.z = sinv;
    hit = hitCheckVecFilter(&probe, 0);
    if (hit != 0 && (hitGetAttr(hit) & 0x800005) == 0) {
        if (*(f32*)((s32)&probe + 0x28) - *(f32*)((s32)mario + 0x90) <= float_30_80420ae4) {
            *outKind = 5;
        } else {
            *outKind = 4;
        }
        return hit;
    }

    probe.base.x = float_0_80420ab8;
    probe.base.y = -(f32)cos(float_0_80420ab8);
    probe.base.z = float_0_80420ab8;
    probe.dir.x = float_0_80420ab8;
    probe.dir.y = probe.base.y;
    probe.dir.z = float_0_80420ab8;
    for (i = 0; i < 2; i++) {
        dist = float_20_80420af0;
        if (i != 0) {
            dist = float_34_80420af4;
            if ((*(u32*)((s32)mario + 0x4) & 1) != 0) {
                dist = float_37_80420af8;
            }
        }
        probe.pos.x = cosv * dist + *(f32*)((s32)mario + 0x8C);
        probe.pos.y = *(f32*)((s32)mario + 0x90) + height;
        probe.pos.z = sinv * dist + *(f32*)((s32)mario + 0x94);
        hit = hitCheckVecFilter(&probe, 0);
        if (hit != 0 && (hitGetAttr(hit) & 0x800005) == 0) {
            if (*(f32*)((s32)&probe + 0x28) - *(f32*)((s32)mario + 0x90) <= float_13_80420afc) {
                *outKind = 6;
            } else {
                *outKind = 5;
            }
            return hit;
        }
    }
    return hit;
}

typedef struct VecData {
    f32 x;
    f32 y;
    f32 z;
} VecData;

const VecData vec3_802c3ec8 = { 0.0f, 40.0f, 0.0f };
const VecData vec3_802c3ed4 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3ee0 = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3eec = { 0.0f, 0.0f, 0.0f };
const VecData vec3_802c3ef8 = { 0.0f, 0.0f, 0.0f };

const char str_MOBJ_Lv1Block_802c3f04[] = "MOBJ_Lv1Block";
const char str_MOBJ_Lv1BigBlock_802c3f14[] = "MOBJ_Lv1BigBlock";
const char str_MOBJ_Lv2Block_802c3f28[] = "MOBJ_Lv2Block";
const char str_MOBJ_Lv2BigBlock_802c3f38[] = "MOBJ_Lv2BigBlock";
const char str_MOBJ_Lv3Block_802c3f4c[] = "MOBJ_Lv3Block";
const char str_MOBJ_Lv3BigBlock_802c3f5c[] = "MOBJ_Lv3BigBlock";
const char str_p_kaiten_h_802c3f70[] = "p_kaiten_h";
const char str_P_H_1C_802c3f7c[] = "P_H_1C";
const char str_P_H_1B_802c3f84[] = "P_H_1B";
const char str_P_H_1A_802c3f8c[] = "P_H_1A";
const char str_M_H_3A_802c3f94[] = "M_H_3A";
const char str_M_H_6A_802c3f9c[] = "M_H_6A";
const char str_M_H_9A_802c3fa4[] = "M_H_9A";
const char str_M_H_3B_802c3fac[] = "M_H_3B";
const char str_M_H_6B_802c3fb4[] = "M_H_6B";
const char str_M_H_9B_802c3fbc[] = "M_H_9B";

const f32 float_2_80420ab0 = 2.0f;
const f32 float_0p5_80420ab4 = 0.5f;
const f32 float_0_80420ab8 = 0.0f;
const f32 float_neg20_80420abc = -20.0f;
const f32 float_90_80420ac0 = 90.0f;
const f32 float_270_80420ac4 = 270.0f;
const f32 float_180_80420ac8 = 180.0f;
const f32 float_3p1416_80420acc = 3.1415927f;
const f32 float_4_80420ad0 = 4.0f;
const f32 float_14_80420ad4 = 14.0f;
const f32 float_1p2217_80420ad8 = 1.22173059f;
const f32 float_5_80420adc = 5.0f;
const f32 float_26_80420ae0 = 26.0f;
const f32 float_30_80420ae4 = 30.0f;
const f32 float_0p7854_80420ae8 = 0.7853982f;
const f32 float_36_80420aec = 36.0f;
const f32 float_20_80420af0 = 20.0f;
const f32 float_34_80420af4 = 34.0f;
const f32 float_37_80420af8 = 37.0f;
const f32 float_13_80420afc = 13.0f;
const f32 float_0p01_80420b00 = 0.01f;
const f32 float_neg1_80420b04 = -1.0f;
const f32 float_neg2_80420b08 = -2.0f;
const f32 float_38_80420b0c = 38.0f;
const f32 float_15_80420b10 = 15.0f;
const f32 float_10_80420b14 = 10.0f;
const char str_M_H_3_80420b18[] = "M_H_3";
const char str_M_H_6_80420b20[] = "M_H_6";
const char str_M_H_9_80420b28[] = "M_H_9";
const f32 float_6_80420b30 = 6.0f;
const f32 float_72_80420b34 = 72.0f;
const f32 float_0p4_80420b38 = 0.4f;
const f32 float_0p005_80420b3c = 0.005f;
const f32 float_1p7_80420b40 = 1.7f;
const f32 float_54_80420b44 = 54.0f;
const f32 float_1_80420b48 = 1.0f;
const f32 float_42_80420b4c = 42.0f;
const f32 float_0p3_80420b50 = 0.3f;
const f32 float_1p6_80420b54 = 1.6f;
const f32 float_18_80420b58 = 18.0f;
const f32 float_0p2_80420b5c = 0.2f;
const f32 float_1p5_80420b60 = 1.5f;
const f32 float_24_80420b64 = 24.0f;
const f32 float_22_80420b68 = 22.0f;
const char str_M_S_1_80420b6c[] = "M_S_1";
const char str_M_I_Y_80420b74[] = "M_I_Y";
const f32 float_neg30_80420b7c = -30.0f;
const char str_M_H_2_80420b80[] = "M_H_2";
const char str_M_H_1_80420b88[] = "M_H_1";
const char str_M_H_5_80420b90[] = "M_H_5";
const char str_M_H_4_80420b98[] = "M_H_4";
const char str_M_H_8_80420ba0[] = "M_H_8";
const char str_M_H_7_80420ba8[] = "M_H_7";
const f32 float_16_80420bb0 = 16.0f;
const f32 float_53_80420bb4 = 53.0f;
const f32 float_25_80420bb8 = 25.0f;
const f32 float_neg3_80420bbc = -3.0f;
const f32 float_31_80420bc0 = 31.0f;
const f32 float_neg6_80420bc4 = -6.0f;
const f32 float_28_80420bc8 = 28.0f;

extern u8 evt_cam_shake(s32 evt, s32 init);

s32 se_data[] = {
    0x13B, 0x165,
    0x13C, 0x167,
    0x13D, 0x166,
    0x13E, 0x168,
    0x13F, 0x169,
};

s32 lv1quake_evt[] = {
    0x0005005B, (s32)evt_cam_shake, 0x00000004, 0xF24A7A80,
    0xF24A7A8A, 0x0000005A, 0x00000002, 0x00000001,
};

s32 lv2quake_evt[] = {
    0x0005005B, (s32)evt_cam_shake, 0x00000004, 0xF24A7A80,
    0xF24A7A94, 0x0000006E, 0x00000002, 0x00000001,
};

s32 lv3quake_evt[] = {
    0x0005005B, (s32)evt_cam_shake, 0x00000004, 0xF24A7A80,
    0xF24A7A9E, 0x0000008C, 0x00000002, 0x00000001,
};
