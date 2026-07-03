#include "bowser/koopa_motion.h"

void quake_kpaHipAttack(void) {
    extern void* evtEntry(void* script, s32 type, s32 flags);
    extern s32 quake_kpaHipAttack_evt[8];
    evtEntry(quake_kpaHipAttack_evt, 0, 0);
}

void quake_kpaLandOn(void) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetStageType(void);
    extern void psndSFXOn_3D(s32 id, void* pos);
    extern s32 kpaGetLevel(void);
    extern void* evtEntry(void* script, s32 type, s32 flags);
    extern s32 quake_kpaLandOn_evt[8];

    void* mario = marioGetPtr();
    if (kpaGetStageType() != 1) {
        psndSFXOn_3D(0x81D, (void*)((u8*)mario + 0x8C));
        if (kpaGetLevel() >= 2) {
            evtEntry(quake_kpaLandOn_evt, 0, 0);
        }
    }
}

s32 kpaPowDown(void) {
    extern void* marioGetPtr(void);
    extern void kpaCancelAttackMode(void);
    extern s32 kpaGetLevel(void);
    extern s32 sysMsec2Frame(s32 msec);
    extern void marioChgMot(s32 mot);

    void* mario = marioGetPtr();
    void* work;

    kpaCancelAttackMode();
    if (kpaGetLevel() > 0) {
        work = *(void**)((s32)mario + 0x298);
        *(s32*)((s32)work + 4) = *(s32*)((s32)work + 4) - 1;
        *(s16*)((s32)mario + 0x78) = sysMsec2Frame(3000);
        marioChgMot(0x27);
        return 1;
    }
    work = *(void**)((s32)mario + 0x298);
    *(u32*)work |= 0x10000000;
    return 0;
}

s32 kpaPowUp(void) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetLevel(void);
    extern void kpaCancelAttackMode(void);
    extern void marioChgMot(s32 mot);
    extern void kpaMSetMutekiTimer(void);
    extern void kpaMutekiOn(void);

    void* mario = marioGetPtr();
    void* work;

    if (kpaGetLevel() < 3) {
        work = *(void**)((s32)mario + 0x298);
        *(s32*)((s32)work + 4) = *(s32*)((s32)work + 4) + 1;
        kpaCancelAttackMode();
        marioChgMot(0x26);
        return 1;
    }
    kpaMSetMutekiTimer();
    kpaMutekiOn();
    return 0;
}

s32 kpaHipAttackOk(void) {
    extern void* marioGetPtr(void);

    void* mario = marioGetPtr();

    if (*(u16*)((s32)mario + 0x2E) == 0x10) {
        return 0;
    }
    return ((*(u32*)*(void**)((s32)mario + 0x298) >> 4) & 1) ^ 1;
}

void kpaSwimMove(void) {
    extern void* marioGetPtr(void);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 float_0_804272e0;
    extern f32 float_0p01_80427354;
    extern f32 float_0p06_80427328;
    extern f32 float_2_80427320;
    extern f32 float_40_8042735c;
    extern f32 float_1225_80427358;

    void* mario = marioGetPtr();
    f32 dir = *(s8*)((s32)mario + 0x252);
    f32 speed = *(f32*)((s32)mario + 0x2DC);
    f32 move;
    f32 angle;

    if (dir != float_0_804272e0) {
        if (dir * speed < float_0_804272e0) {
            speed = float_0_804272e0;
        }
        if (speed * speed <= float_1225_80427358) {
            speed += dir * float_0p01_80427354;
        } else {
            speed += dir * float_0p01_80427354;
            if (speed < -float_40_8042735c) {
                speed = -float_40_8042735c;
            }
            if (speed > float_40_8042735c) {
                speed = float_40_8042735c;
            }
        }
    }

    *(f32*)((s32)mario + 0x2DC) = speed;
    move = float_0p06_80427328 * __fabs(speed);
    if (move >= float_2_80427320) {
        move = float_2_80427320;
    }
    angle = angleABf(float_0_804272e0, float_0_804272e0, speed, float_0_804272e0);
    if (move < float_0p01_80427354) {
        move = float_0_804272e0;
    }
    *(f32*)((s32)mario + 0x180) = move;
    if (move != float_0_804272e0) {
        *(f32*)((s32)mario + 0x1A4) = angle;
    }
    *(f32*)((s32)mario + 0x18C) = move;
}

void kpaSetSpec(void) {
    extern void* marioGetPtr(void);
    extern f32 kpaGetWidth(s32 level);
    extern f32 float_24_80427360;
    extern f32 float_74_80427364;
    extern f32 float_103p6_80427368;
    extern f32 float_222_8042736c;

    void* mario = marioGetPtr();
    void* work = *(void**)((s32)mario + 0x298);

    *(f32*)((s32)mario + 0x1B8) = kpaGetWidth(*(s32*)((s32)work + 4));
    work = *(void**)((s32)mario + 0x298);
    if (*(s32*)((s32)work + 4) == 0) {
        *(f32*)((s32)mario + 0x1BC) = float_24_80427360;
    } else if (*(s32*)((s32)work + 4) == 1) {
        *(f32*)((s32)mario + 0x1BC) = float_74_80427364;
    } else if (*(s32*)((s32)work + 4) == 2) {
        *(f32*)((s32)mario + 0x1BC) = float_103p6_80427368;
    } else if (*(s32*)((s32)work + 4) == 3) {
        *(f32*)((s32)mario + 0x1BC) = float_222_8042736c;
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void kpa_jump(void) {
    extern void* marioGetPtr(void);
    extern void marioPreJump(void);
    extern void camFollowYOff(void);
    extern void kpaChgPose(const char* pose, const char* pose2);
    extern void psndSFXOn_3D(s32 id, void* pos);
    extern void marioMakeJumpPara(void);
    extern void marioSetPeakYpos(void);
    extern void kpaSetFallPara(void);
    extern void marioChgMot(s32 mot);
    extern f32 float_0_804272e0;
    extern f32 float_neg0p526_80427388;
    extern char str_KPA_J_1A_802feb94[];
    extern char str_KPA2_J_1A_802feba0[];
    typedef struct {
        s32 x;
        s32 y;
        s32 z;
    } VecCopy;

    void* mario = marioGetPtr();
    f32 oldY;
    f32 y;

    if (*(u32*)((s32)mario + 0xC) & 1) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        *(u32*)mario |= 0x80000;
        *(s32*)((s32)mario + 0x48) = 2;
        *(s32*)((s32)mario + 0x44) = 0;
        marioPreJump();
        if (*(u16*)((s32)mario + 0x2E) != 8) {
            camFollowYOff();
        }
        kpaChgPose(str_KPA_J_1A_802feb94, str_KPA2_J_1A_802feba0);
        psndSFXOn_3D(0x81E, (void*)((u8*)mario + 0x8C));
        if (*(s16*)((s32)mario + 0x50) == 0) {
            *(VecCopy*)((s32)mario + 0xD4) = *(VecCopy*)((s32)mario + 0x8C);
        }
        *(s16*)((s32)mario + 0x2B8) = 0;
    }

    *(s16*)((s32)mario + 0x50) = *(s16*)((s32)mario + 0x50) + 1;
    if (*(u16*)((s32)mario + 0x2E) != 8) {
        oldY = *(f32*)((s32)mario + 0x7C);
        marioMakeJumpPara();
        if (*(f32*)((s32)mario + 0x7C) > float_0_804272e0 &&
            *(s16*)((s32)mario + 0x50) >= 4 &&
            !(*(u16*)((s32)mario + 0x24A) & 0x100)) {
            *(f32*)((s32)mario + 0x7C) += float_neg0p526_80427388;
        }
        y = *(f32*)((s32)mario + 0x7C);
        if (y <= float_0_804272e0) {
            if (oldY > float_0_804272e0) {
                *(s16*)((s32)mario + 0x52) = *(s16*)((s32)mario + 0x50);
            }
            marioSetPeakYpos();
            kpaSetFallPara();
            marioMakeJumpPara();
            marioChgMot(0xA);
        }
        *(f32*)((s32)mario + 0x90) += *(f32*)((s32)mario + 0x7C);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void kpa_walk(void) {
    extern void* marioGetPtr(void);
    extern void kpaChgPose(char* pose3d, char* pose2d);
    extern s32 kpaGetStageViewType(void);
    extern s32 kpaGetBodyStatus(void);
    extern void marioChgMot(s32 mot);
    extern void kpaAttackStart(void);
    extern s32 marioAnimeId(void);
    extern void animPoseSetLocalTime(f64 time, s32 poseId);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern f32 kpaStickSpd(s32 stick);
    extern s32 kpaGetLevel(void);
    extern s32 marioKeyOffChk(void);
    extern void marioCamZoomOffReq(void);
    extern char str_SFX_KUPPA_SWIM1_802feb70[];
    extern f32 float_0p02_80427314;
    extern f32 float_neg0p02_8042738c;
    extern f32 float_0_804272e0;
    extern f32 _walkSpdTbl[4];
    extern s32 _rateTbl[4];

    char* base = str_SFX_KUPPA_SWIM1_802feb70;
    void* mario = marioGetPtr();
    void* work;
    void* mario2;
    s32 changed;
    s32 status;
    s32 timer;
    s32 level;
    s32 animeId;
    s32 stickX;
    s32 stickY;
    u16 walkTimer;
    f32 walkSpd;
    f32 initSpeed;
    f32 initAccel;

    if (*(u32*)((s32)mario + 0xC) & 1) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        *(u32*)mario &= ~0xF0000;
        *(s32*)((s32)mario + 0x44) = 0;
        *(s32*)((s32)mario + 0x48) = 0;
        work = *(void**)((s32)mario + 0x298);
        if ((*(u32*)work & 0x01000000) == 0) {
            kpaChgPose(base + 0x10, base + 0xD8);
        } else {
            kpaChgPose(base + 0xE4, base + 0xD8);
        }
        initSpeed = float_0p02_80427314;
        initAccel = float_neg0p02_8042738c;
        *(u16*)((s32)mario + 0x2B8) = 0;
        *(u16*)((s32)mario + 0x2BA) = 0;
        work = *(void**)((s32)mario + 0x298);
        *(f32*)((s32)work + 0x10) = initSpeed;
        work = *(void**)((s32)mario + 0x298);
        *(f32*)((s32)work + 0x14) = initAccel;
    }

    mario2 = marioGetPtr();
    if (kpaGetStageViewType() == 1) {
        changed = 0;
    } else if ((*(u16*)((s32)mario2 + 0x24C) & 0x100) == 0) {
        changed = 0;
    } else {
        if (kpaGetBodyStatus() == 2) {
            marioChgMot(0x25);
            changed = 1;
        } else {
            marioChgMot(3);
            changed = 1;
        }
    }

    if (changed == 0) {
        if (*(u16*)((s32)mario + 0x24C) & 0x200) {
            kpaAttackStart();
            marioChgMot(0);
        } else {
            status = kpaGetBodyStatus();
            if (status == 2) {
                walkTimer = *(u16*)((s32)mario + 0x2B8) + 1;
                *(u16*)((s32)mario + 0x2B8) = walkTimer;
                if (*(u16*)((s32)mario + 0x2B8) > 0x50) {
                    *(u16*)((s32)mario + 0x2B8) = 0;
                }
                *(u16*)((s32)mario + 0x28) = (u32)*(u16*)((s32)mario + 0x2B8) >> 1;
                animeId = marioAnimeId();
                animPoseSetLocalTime((f64)*(u16*)((s32)mario + 0x28), *(s32*)((s32)mario + 0x22C + animeId * 4));
                if (kpaGetBodyStatus() == 0) {
                    timer = *(s32*)((s32)mario + 0x48);
                    if (timer % 0x28 == 0) {
                        psndSFXOn_3D(0x819, (void*)((s32)mario + 0x8C));
                    } else if (timer % 0x14 == 0) {
                        psndSFXOn_3D(0x818, (void*)((s32)mario + 0x8C));
                    }
                    *(s32*)((s32)mario + 0x48) = *(s32*)((s32)mario + 0x48) + 1;
                }
            } else {
                if (kpaGetBodyStatus() == 0) {
                    timer = *(s32*)((s32)mario + 0x48);
                    if (timer % 0x28 == 0) {
                        psndSFXOn_3D(0x819, (void*)((s32)mario + 0x8C));
                    } else if (timer % 0x14 == 0) {
                        psndSFXOn_3D(0x818, (void*)((s32)mario + 0x8C));
                    }
                    *(s32*)((s32)mario + 0x48) = *(s32*)((s32)mario + 0x48) + 1;
                }
            }

            if (*(s16*)((s32)mario + 0x4E) == 0) {
                if (kpaGetStageViewType() == 0) {
                    *(f32*)((s32)mario + 0x180) = kpaStickSpd(0);
                } else {
                    mario2 = marioGetPtr();
                    level = kpaGetLevel();
                    walkSpd = _walkSpdTbl[level];
                    work = *(void**)((s32)mario2 + 0x298);
                    *(s32*)((s32)work + 0xD0) = _rateTbl[level];
                    *(f32*)((s32)mario + 0x180) = walkSpd;
                }
            }

            if (kpaGetStageViewType() == 0) {
                if (*(f32*)((s32)mario + 0x180) == float_0_804272e0 && !(*(u32*)mario & 0x20)) {
                    marioChgMot(0);
                    return;
                }
            } else if (*(f32*)((s32)mario + 0x194) == float_0_804272e0 && !(*(u32*)mario & 0x20)) {
                marioChgMot(0);
                return;
            }

            if (marioKeyOffChk() == 0) {
                marioCamZoomOffReq();
                if (kpaGetStageViewType() == 1) {
                    *(f32*)((s32)mario + 0x1A4) = *(f32*)((s32)mario + 0x198);
                }
                stickX = *(s8*)((s32)mario + 0x252);
                stickY = *(s8*)((s32)mario + 0x253);
                if (stickX * stickX + stickY * stickY > 0xBD1) {
                    marioChgMot(2);
                }
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on



/* CHATGPT STUB FILL: main/bowser/koopa_motion 20260624_184128 */

/* stub-fill: kpa_swim | missing_definition | ghidra_signature */
u8 kpa_swim(void) {
    return 0;
}

/* stub-fill: kpa_powdown | missing_definition | ghidra_signature */
u8 kpa_powdown(void) {
    return 0;
}

/* stub-fill: kpa_powup | missing_definition | ghidra_signature */
u8 kpa_powup(void) {
    return 0;
}

/* stub-fill: kpa_hip | missing_definition | ghidra_signature */
u8 kpa_hip(void) {
    return 0;
}

/* stub-fill: kpa_fall | missing_definition | ghidra_signature */
u8 kpa_fall(void) {
    return 0;
}

/* stub-fill: kpa_dash | missing_definition | ghidra_signature */
u8 kpa_dash(void) {
    return 0;
}

/* stub-fill: kpa_stay | missing_definition | ghidra_signature */
void kpa_stay(void) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetAttackMode(void);
    extern s32 kpaGetStageViewType(void);
    extern s32 kpaGetBodyStatus(void);
    extern void kpaChgPose(char* pose3d, char* pose2d);
    extern void marioChgMot(s32 mot);
    extern void kpaAttackStart(void);
    extern f32 kpaStickSpd(s32 stick);
    extern char str_SFX_KUPPA_SWIM1_802feb70[];
    extern f32 float_0_804272e0;
    char* base;
    void* mario;
    void* work;
    s32 changed;
    f32 speed;

    base = str_SFX_KUPPA_SWIM1_802feb70;
    mario = marioGetPtr();
    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        *(u32*)((s32)mario + 0xC) &= ~1;
        *(u32*)mario &= ~0xF0000;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s32*)((s32)mario + 0x48) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        if (kpaGetAttackMode() == 0) {
            work = *(void**)((s32)mario + 0x298);
            if ((*(u32*)work & 0x01000000) == 0) {
                kpaChgPose(base + 0xEC, base + 0xF4);
            } else {
                kpaChgPose(base + 0x100, base + 0xF4);
            }
        }
        if ((*(u32*)mario & 0x20) == 0) {
            *(f32*)((s32)mario + 0x180) = float_0_804272e0;
        }
    }

    if (kpaGetAttackMode() == 0) {
        mario = marioGetPtr();
        if (kpaGetStageViewType() == 1) {
            changed = 0;
        } else if ((*(u16*)((s32)mario + 0x24C) & 0x100) == 0) {
            changed = 0;
        } else if (kpaGetBodyStatus() == 2) {
            marioChgMot(0x25);
            changed = 1;
        } else {
            marioChgMot(3);
            changed = 1;
        }

        if (changed == 0) {
            if ((*(u16*)((s32)mario + 0x24C) & 0x200) != 0) {
                kpaAttackStart();
            } else {
                if (*(s16*)((s32)mario + 0x4E) == 0) {
                    speed = kpaStickSpd(0);
                } else {
                    speed = *(f32*)((s32)mario + 0x180);
                }
                if (kpaGetStageViewType() == 0) {
                    *(f32*)((s32)mario + 0x180) = speed;
                    if (speed > float_0_804272e0) {
                        marioChgMot(1);
                    }
                } else if (*(f32*)((s32)mario + 0x194) > float_0_804272e0) {
                    marioChgMot(1);
                }
            }
        }
    }
}

/* stub-fill: kpaStickSpd | prototype_only | source_prototype */
f32 kpaStickSpd(s32 stick) {
    extern void* marioGetPtr(void);
    extern s32 kpaGetBodyStatus(void);
    extern f32 angleABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 float_70_80427390;
    extern f32 float_0_804272e0;
    extern f32 float_neg0p2_80427394;
    extern f32 float_neg0p7_80427398;
    extern f32 float_0p01_80427354;
    extern f32 float_0p2_8042739c;
    extern f32 float_1p3_804273a0;
    extern f32 float_0p633_804273a4;
    extern f32 float_2p3_804273a8;
    extern f32 float_0p002_804273ac;
    void* mario;
    f32 dir;
    f32 speed;
    f32 limit;
    f32 negLimit;

    mario = marioGetPtr();
    dir = (f32)*(s8*)((s32)mario + 0x252);
    if (dir >= float_70_80427390) {
        dir = float_70_80427390;
    }
    if (dir <= -float_70_80427390) {
        dir = -float_70_80427390;
    }

    speed = *(f32*)((s32)mario + 0x2DC);
    if (dir == float_0_804272e0) {
        if (*(u32*)((s32)mario + 0x1F4) != 0) {
            speed += speed * float_neg0p7_80427398;
        } else {
            speed += speed * float_neg0p2_80427394;
        }
        if (__fabs(speed) < float_0p01_80427354) {
            speed = float_0_804272e0;
        }
    } else {
        if (stick == 0) {
            if (kpaGetBodyStatus() == 2) {
                limit = float_0p2_8042739c;
            } else {
                limit = float_1p3_804273a0;
            }
        } else {
            if (kpaGetBodyStatus() == 2) {
                limit = float_0p633_804273a4;
            } else {
                limit = float_2p3_804273a8;
            }
        }
        speed += float_0p002_804273ac * dir;
        if (speed >= limit) {
            speed = limit;
        }
        negLimit = -limit;
        if (speed <= negLimit) {
            speed = negLimit;
        }
    }

    *(f32*)((s32)mario + 0x2DC) = speed;
    *(f32*)((s32)mario + 0x180) = __fabs(speed);
    dir = angleABf(float_0_804272e0, float_0_804272e0, speed, float_0_804272e0);
    if (*(f32*)((s32)mario + 0x180) != float_0_804272e0) {
        *(f32*)((s32)mario + 0x1A4) = dir;
    }
    return *(f32*)((s32)mario + 0x180);
}

