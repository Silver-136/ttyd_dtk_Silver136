#include "mario/mario_motion.h"

extern f32 float_0_804209a0;
const char str_M_I_U_80420a14[] = "M_I_U";
extern void (*marioMotTbl[][3])(void);

void* marioGetPtr(void);
void marioReInit_ship(void);
void marioAdjustMoveDir(void);
void marioChgPose(const void* pose);

s32 L_marioChkRub(void) {
    return 0;
}

void marioChgShipMotion(void) {
    marioReInit_ship();
}

void mot_dummy(void) {
    marioGetPtr();
}

void mot_partyUse(void) {
    marioGetPtr();
}

void marioChgRollMotion(void) {
    void* mario = marioGetPtr();
    s32 mode = 0x32;
    s32 pose = 0x16;
    *(s32*)((s32)mario + 0x44) = mode;
    *(s16*)((s32)mario + 0x2E) = pose;
}

void marioClearJumpPara(void) {
    void* mario = marioGetPtr();
    f32 zero = float_0_804209a0;
    *(f32*)((s32)mario + 0x7C) = zero;
    *(f32*)((s32)mario + 0x80) = zero;
    *(f32*)((s32)mario + 0x84) = zero;
    *(f32*)((s32)mario + 0x88) = zero;
}

void mot_getItem(void) {
    void* mario = marioGetPtr();
    u32 flags = *(u32*)((s32)mario + 0xC);
    if (flags & 1) {
        f32 zero;
        *(u32*)((s32)mario + 0xC) = flags & ~1;
        zero = float_0_804209a0;
        *(s32*)((s32)mario + 0x48) = 0;
        *(s16*)((s32)mario + 0x50) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        marioAdjustMoveDir();
        marioChgPose(str_M_I_U_80420a14);
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioMotion(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x200) = 0;
    do {
        marioMotTbl[*(u16*)((s32)mario + 0x2E)][0]();
    } while (*(u32*)((s32)mario + 0xC) & 1);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void marioLandOn(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* marioGetPtr(void);
    extern u32 hitGetAttr(void* hit);
    extern void kpaCamFollow(void);
    extern s32 marioBgmodeChk(void);
    extern void camFollowYOn(void);
    extern u32 psndSFXOn_3D(s32 id, void* position);
    extern f64 distABf(f64 x1, f64 z1, f64 x2, f64 z2);
    extern s32 kpaFireAttackCheck(void);
    extern void quake_kpaLandOn(void);
    extern s32 marioPriCheckJabara(void);
    extern void marioChgMot(s32 motion);
    extern void set_damage_root_ypos(f32 y);
    extern f32 float_0_804209a0;

    void* player = marioGetPtr();
    void* hit = *(void**)((s32)player + 0x1E8);
    u32 attr = hitGetAttr(hit);
    Vec pos;
    s32 sound;
    s32 motion;

    if ((attr & 0x20) == 0) {
        *(f32*)((s32)player + 0x7C) = float_0_804209a0;
        *(f32*)((s32)player + 0x80) = float_0_804209a0;
        *(f32*)((s32)player + 0x84) = float_0_804209a0;
        *(f32*)((s32)player + 0x88) = float_0_804209a0;
    }
    *(u32*)player &= ~0x40000080;
    *(s16*)((s32)player + 0x50) = 0;
    *(s16*)((s32)player + 0x52) = 0;

    if (*(s8*)((s32)player + 0x3C) == 2) {
        kpaCamFollow();
    } else if (marioBgmodeChk() == 0) {
        camFollowYOn();
    }

    pos = *(Vec*)((s32)player + 0x8C);
    if (hit != 0) {
        if ((attr & 0x2000) != 0) {
            sound = 0x150;
        } else if ((attr & 0x200000) != 0) {
            sound = 0x151;
        } else if ((attr & 0x100000) != 0) {
            sound = 0x152;
        } else if ((attr & 0x1000) != 0) {
            sound = 0x153;
        } else if ((attr & 0x400000) != 0) {
            sound = 0x154;
        } else {
            sound = 0x14F;
        }
        psndSFXOn_3D(sound, &pos);
    }

    if ((*(u32*)player & 0x01000000) != 0) {
        *(u32*)player &= ~0x60000;
        *(void**)((s32)player + 0x1F8) = 0;
        return;
    }

    *(f32*)((s32)player + 0x178) = (f32)distABf(
        *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
        *(f32*)((s32)player + 0x164), *(f32*)((s32)player + 0x16C));

    if ((attr & 0x20) != 0 && *(u16*)((s32)player + 0x2E) != 3) {
        set_damage_root_ypos(*(f32*)((s32)player + 0x90));
        marioChgMot(3);
        return;
    }
    if ((attr & 0x40) != 0 && *(u16*)((s32)player + 0x2E) != 6) {
        set_damage_root_ypos(*(f32*)((s32)player + 0x90));
        marioChgMot(6);
        return;
    }

    if ((attr & 0x40000000) != 0) {
        marioChgMot(0x0B);
        return;
    }

    if (*(s8*)((s32)player + 0x3C) == 2) {
        if (kpaFireAttackCheck() == 0) {
            motion = 1;
        } else {
            motion = 0;
        }
        marioChgMot(motion);
        quake_kpaLandOn();
        return;
    }

    if (*(f32*)((s32)player + 0x180) > float_0_804209a0 &&
        (*(u32*)((s32)player + 0xC) & 0x100) == 0 &&
        (*(u32*)player & 0x2000000) == 0) {
        if (marioPriCheckJabara() == 0) {
            marioChgMot(1);
        } else if ((*(u32*)((s32)player + 0x14) & 0x100) != 0) {
            marioChgMot(0);
        } else {
            marioChgMot(1);
        }
    } else if ((*(u32*)player & 0x2000000) != 0) {
        motion = *(f32*)((s32)player + 0x180) <= *(f32*)((s32)player + 0x184) ? 1 : 2;
        marioChgMot(motion);
    } else {
        marioChgMot(0);
    }
}

void mot_bottomless(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern void* marioGetPtr(void);
    extern s32 strcmp(char*, char*);
    extern void marioGetScreenPos(Vec*, f32*, f32*, f32*);
    extern s32 marioChkInScreen(s32, s32);
    extern f64 distABf(f64, f64, f64, f64);
    extern f64 angleABf(f64, f64, f64, f64);
    extern void movePos(f32*, f32*, f32, f32);
    extern void* marioChkLandon(f32, void*);
    extern void psndSFXOn_3D(s32, void*);
    extern void marioChgPose(const void*);
    extern void marioDmgRetQuakeOn(void);
    extern s32 pouchAddHP(s16);
    extern s32 pouchGetHP(void);
    extern void marioUpdateCamPos(void);
    extern void* gp;
    extern f32 marioJumpData[];
    extern f32 marioJumpSwData[];
    extern void (*marioMotTbl[][3])(void);
    extern char str_win_03_802c3e44[];
    extern char str_win_01_802c3e4c[];
    extern char str_tik_01_802c3e54[];
    extern char str_M_D_2_804209dc[];
    extern char str_M_U_3_804209e4[];
    extern f32 float_0_804209a0;
    extern f32 float_1_804209b4;
    extern f32 float_8_804209c8;
    extern f32 float_10_804209c4;
    extern f32 float_13_804209ac;
    extern f32 float_100_804209b8;
    extern f32 float_120_804209bc;
    extern f32 float_220_804209b0;
    extern f32 float_250_804209a8;
    extern f32 float_300_804209c0;
    extern f32 float_neg2000_804209a4;
    extern f32 float_0p34_804209cc;
    extern f32 float_neg0p002_804209d0;
    extern f32 float_neg0p01_804209d4;
    extern f32 float_neg10_804209d8;

    void* player;
    Vec target;
    Vec screenPos;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 landY;
    f32 step;
    f32 fallSpeed;
    void* hit;
    s32 timer;

    player = marioGetPtr();
    if ((*(u32*)((s32)player + 0xC) & 1) != 0) {
        *(u32*)((s32)player + 0xC) &= ~1;
        *(f32*)((s32)player + 0x180) = float_0_804209a0;
        *(s16*)((s32)player + 0x50) = 0;
        *(s32*)((s32)player + 0x44) = 0;

        target.x = *(f32*)((s32)player + 0xE0);
        target.y = *(f32*)((s32)player + 0xE4);
        target.z = *(f32*)((s32)player + 0xE8);
        if (target.y < float_neg2000_804209a4) {
            target.x = *(f32*)((s32)player + 0x8C);
            target.y = *(f32*)((s32)player + 0x90);
            target.z = *(f32*)((s32)player + 0x94);
            if (strcmp((char*)((s32)gp + 0x12C), str_win_03_802c3e44) == 0) {
                target.x = float_250_804209a8;
                target.y = float_13_804209ac;
            } else if (strcmp((char*)((s32)gp + 0x12C), str_win_01_802c3e4c) == 0) {
                target.x = float_220_804209b0;
                target.y = float_1_804209b4;
            } else if (strcmp((char*)((s32)gp + 0x12C), str_tik_01_802c3e54) == 0) {
                target.x = float_100_804209b8;
                target.y = float_120_804209bc;
            }
        }

        screenPos.x = target.x;
        screenPos.y = target.y + float_300_804209c0;
        screenPos.z = target.z;
        step = float_10_804209c4;
        do {
            Vec temp = screenPos;
            marioGetScreenPos(&temp, &sx, &sy, &sz);
            if (marioChkInScreen((s32)sx, (s32)sy) == 0) {
                break;
            }
            screenPos.y += step;
        } while (1);

        *(f32*)((s32)player + 0x90) = screenPos.y;
        *(f32*)((s32)player + 0x2C4) = float_8_804209c8;
        *(s32*)((s32)player + 0x48) = (s16)((s32)(distABf(target.x, target.z,
            *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94)) /
            *(f32*)((s32)player + 0x2C4)));
        *(f32*)((s32)player + 0x2C8) = (f32)angleABf(*(f32*)((s32)player + 0x8C),
            *(f32*)((s32)player + 0x94), target.x, target.z);
    }

    switch (*(s32*)((s32)player + 0x44)) {
        case 0:
            movePos((f32*)((s32)player + 0x8C), (f32*)((s32)player + 0x94),
                    *(f32*)((s32)player + 0x2C4), *(f32*)((s32)player + 0x2C8));
            *(f32*)((s32)player + 0xEC) = *(f32*)((s32)player + 0x8C);
            *(f32*)((s32)player + 0xF4) = *(f32*)((s32)player + 0x94);
            *(s32*)((s32)player + 0x48) -= 1;
            if (*(s32*)((s32)player + 0x48) < 1) {
                *(s32*)((s32)player + 0x44) = 2;
                *(u32*)player |= 0x80;
                player = marioGetPtr();
                if (*(u16*)((s32)player + 0x2E) == 5) {
                    *(f32*)((s32)player + 0x7C) = marioJumpSwData[4];
                    *(f32*)((s32)player + 0x80) = marioJumpSwData[5];
                    *(f32*)((s32)player + 0x84) = marioJumpSwData[6];
                    *(f32*)((s32)player + 0x88) = marioJumpSwData[7];
                } else {
                    *(f32*)((s32)player + 0x7C) = marioJumpData[4];
                    *(f32*)((s32)player + 0x80) = marioJumpData[5];
                    *(f32*)((s32)player + 0x84) = marioJumpData[6];
                    *(f32*)((s32)player + 0x88) = marioJumpData[7];
                }
                if (*(f32*)((s32)player + 0x228) != float_1_804209b4) {
                    *(f32*)((s32)player + 0x7C) = float_0p34_804209cc;
                    *(f32*)((s32)player + 0x80) = float_neg0p002_804209d0;
                    *(f32*)((s32)player + 0x84) = float_neg0p01_804209d4;
                    *(f32*)((s32)player + 0x88) = float_0_804209a0;
                }
            }
            break;

        case 2:
            *(s32*)((s32)player + 0x1E8) = 0;
            player = marioGetPtr();
            *(f32*)((s32)player + 0x84) += *(f32*)((s32)player + 0x88);
            if (*(f32*)((s32)player + 0x84) > float_0_804209a0) {
                *(f32*)((s32)player + 0x84) = float_0_804209a0;
            }
            *(f32*)((s32)player + 0x80) += *(f32*)((s32)player + 0x84);
            *(f32*)((s32)player + 0x7C) += *(f32*)((s32)player + 0x80);
            if (*(f32*)((s32)player + 0x7C) <= marioJumpData[9]) {
                *(f32*)((s32)player + 0x7C) = marioJumpData[9];
                *(f32*)((s32)player + 0x80) = float_0_804209a0;
                *(f32*)((s32)player + 0x84) = float_0_804209a0;
                *(f32*)((s32)player + 0x88) = float_0_804209a0;
            }
            fallSpeed = *(f32*)((s32)player + 0x7C);
            if (fallSpeed < float_neg10_804209d8) {
                fallSpeed = float_neg10_804209d8;
            }
            hit = marioChkLandon(fallSpeed, &landY);
            if (hit == 0) {
                *(f32*)((s32)player + 0x90) += fallSpeed;
                return;
            }
            psndSFXOn_3D(0xB9, (void*)((s32)player + 0x8C));
            marioChgPose(str_M_D_2_804209dc);
            psndSFXOn_3D(0x197, (void*)((s32)player + 0x8C));
            marioDmgRetQuakeOn();
            *(void**)((s32)player + 0x1E8) = hit;
            *(s32*)((s32)player + 0x1EC) = 0;
            *(f32*)((s32)player + 0x90) = landY;
            /* fall through */

        case 10:
            *(s32*)((s32)player + 0x48) = 0x32;
            *(s32*)((s32)player + 0x44) = 0xB;
            /* fall through */

        case 0xB:
            *(s32*)((s32)player + 0x48) -= 1;
            if (*(s32*)((s32)player + 0x48) < 1) {
                pouchAddHP(-1);
                if (pouchGetHP() < 1) {
                    *(s32*)((s32)player + 0x44) = 0xD;
                    marioUpdateCamPos();
                } else {
                    marioChgPose(str_M_U_3_804209e4);
                    *(s32*)((s32)player + 0x48) = 0xC;
                    *(s32*)((s32)player + 0x44) = 0xC;
                    timer = 1;
                    goto case_c_wait;
                }
            }
            break;

        case 0xC:
            timer = 0;
case_c_wait:
            if (timer == 0) {
                *(s32*)((s32)player + 0x48) -= 1;
            }
            if (*(s32*)((s32)player + 0x48) < 1) {
                player = marioGetPtr();
                if ((*(u32*)player & 0x10000000) == 0) {
                    if (*(u16*)((s32)player + 0x30) == 0x10 || *(u16*)((s32)player + 0x30) == 0x11) {
                        *(u32*)((s32)player + 4) &= ~0x08000000;
                        *(u32*)((s32)player + 4) &= ~4;
                    }
                    *(u16*)((s32)player + 0x30) = *(u16*)((s32)player + 0x2E);
                    *(u16*)((s32)player + 0x2E) = 0;
                    if (marioMotTbl[*(u16*)((s32)player + 0x30)][2] != 0) {
                        marioMotTbl[*(u16*)((s32)player + 0x30)][2]();
                    }
                    *(u32*)((s32)player + 0xC) |= 1;
                }
                marioUpdateCamPos();
            }
            break;
    }
}

void marioFall(void) {
    extern void* marioGetPtr(void);
    extern s32 strcmp(char*, char*);
    extern void kpaClearHitobjRide(void);
    extern void* marioChkLandon(f32, void*);
    extern s32 marioChkInScreen(s32, s32);
    extern s32 getRollEvtFlag(void);
    extern s32 marioKeyOffChk(void);
    extern s32 marioCtrlOffChk(void);
    extern s32 marioChkSts(u32);
    extern void mot_fall_roll(void);
    extern s32 marioGetRub(s32, void*, void*, void*);
    extern u32 hitGetAttr(void*);
    extern s32 marioPriCheckJabara(void);
    extern void camFollowYOn(void);
    extern s32 kpaChkHitobjRide(void);
    extern void* gp;
    extern f32 marioJumpData[];
    extern void (*marioMotTbl[][3])(void);
    extern char str_kpa_03_802c3e9c[];
    extern f32 float_0_804209a0;
    extern f32 float_0p01_80420a2c;
    extern f32 float_0p1_80420a28;
    extern f32 float_neg0p5_80420a30;

    void* player;
    void* hit;
    f32 landY;
    f32 speed;
    s32 rubDir;
    s32 rubCount;
    s32 rubOther;
    s32 rub;
    u32 attr;

    player = marioGetPtr();

    if (strcmp((char*)((s32)gp + 0x12C), str_kpa_03_802c3e9c) == 0 &&
        *(s8*)((s32)player + 0x3C) == 2) {
        *(s32*)((s32)player + 0x1E8) = 0;
        kpaClearHitobjRide();
        player = marioGetPtr();
        *(f32*)((s32)player + 0x84) += *(f32*)((s32)player + 0x88);
        if (*(f32*)((s32)player + 0x84) > float_0_804209a0) {
            *(f32*)((s32)player + 0x84) = float_0_804209a0;
        }
        *(f32*)((s32)player + 0x80) += *(f32*)((s32)player + 0x84);
        *(f32*)((s32)player + 0x7C) += *(f32*)((s32)player + 0x80);
        if (*(f32*)((s32)player + 0x7C) <= marioJumpData[9]) {
            *(f32*)((s32)player + 0x7C) = marioJumpData[9];
            *(f32*)((s32)player + 0x80) = float_0_804209a0;
            *(f32*)((s32)player + 0x84) = float_0_804209a0;
            *(f32*)((s32)player + 0x88) = float_0_804209a0;
        }
        speed = *(f32*)((s32)player + 0x7C);
        hit = marioChkLandon(speed, &landY);
        if (hit == 0) {
            *(f32*)((s32)player + 0x90) += speed;
        } else {
            *(void**)((s32)player + 0x1E8) = hit;
            *(s32*)((s32)player + 0x1EC) = 0;
            *(f32*)((s32)player + 0x90) = landY;
        }
        if (*(void**)((s32)player + 0x1E8) != 0) {
            marioLandOn();
        }
        return;
    }

    if ((*(u32*)player & 0x20) == 0 &&
        marioChkInScreen(*(s32*)((s32)player + 0x168), *(s32*)((s32)player + 0x16C)) == 0) {
        *(u32*)player |= 0x80;
        *(f32*)((s32)player + 0x180) =
            *(f32*)((s32)player + 0x180) + float_0p1_80420a28 * -*(f32*)((s32)player + 0x180);
    }

    if (getRollEvtFlag() != 0) {
        return;
    }
    if (*(u16*)((s32)player + 0x2E) == 0x10 || *(u16*)((s32)player + 0x2E) == 0x11) {
        return;
    }

    if ((marioKeyOffChk() == 0 || marioCtrlOffChk() == 0) && (*(u32*)player & 0x20) == 0) {
        speed = *(f32*)((s32)player + 0x180) +
            float_0p01_80420a2c * (float_neg0p5_80420a30 - *(f32*)((s32)player + 0x180));
        if (speed < float_0_804209a0) {
            speed = float_0_804209a0;
        }
        *(f32*)((s32)player + 0x180) = speed;
    }

    *(s32*)((s32)player + 0x1E8) = 0;
    if (*(s8*)((s32)player + 0x3C) == 2) {
        kpaClearHitobjRide();
    }

    if (marioChkSts(0x10) == 1) {
        mot_fall_roll();
        return;
    }

    rub = -1;
    rubDir = -1;
    if ((*(u32*)((s32)player + 0xC) & 0x200000) == 0 &&
        (*(u32*)((s32)player + 0xC) & 0x100000) != 0) {
        rub = marioGetRub(0x100, &rubCount, &rubDir, &rubOther);
        if ((*(u16*)((s32)player + 0x24A) & 0x100) != 0) {
            *(u32*)((s32)player + 0xC) |= 0x400000;
        }
    }

    player = marioGetPtr();
    *(f32*)((s32)player + 0x84) += *(f32*)((s32)player + 0x88);
    if (*(f32*)((s32)player + 0x84) > float_0_804209a0) {
        *(f32*)((s32)player + 0x84) = float_0_804209a0;
    }
    *(f32*)((s32)player + 0x80) += *(f32*)((s32)player + 0x84);
    *(f32*)((s32)player + 0x7C) += *(f32*)((s32)player + 0x80);
    if (*(f32*)((s32)player + 0x7C) <= marioJumpData[9]) {
        *(f32*)((s32)player + 0x7C) = marioJumpData[9];
        *(f32*)((s32)player + 0x80) = float_0_804209a0;
        *(f32*)((s32)player + 0x84) = float_0_804209a0;
        *(f32*)((s32)player + 0x88) = float_0_804209a0;
    }

    speed = *(f32*)((s32)player + 0x7C);
    hit = marioChkLandon(speed, &landY);
    if (hit == 0) {
        *(f32*)((s32)player + 0x90) += speed;
    } else {
        *(void**)((s32)player + 0x1E8) = hit;
        *(s32*)((s32)player + 0x1EC) = 0;
        attr = hitGetAttr(hit);
        if ((attr & 0x200) == 0) {
            *(f32*)((s32)player + 0x90) = landY;
            if ((*(u32*)((s32)player + 0xC) & 0x200000) == 0 &&
                (*(u32*)((s32)player + 0xC) & 0x100000) != 0 &&
                marioPriCheckJabara() != 0 &&
                rubDir != -1 && rub > -1) {
                camFollowYOn();
                player = marioGetPtr();
                if ((*(u32*)player & 0x10000000) != 0) {
                    return;
                }
                if (*(u16*)((s32)player + 0x30) == 0x10 || *(u16*)((s32)player + 0x30) == 0x11) {
                    *(u32*)((s32)player + 4) &= ~0x08000000;
                    *(u32*)((s32)player + 4) &= ~4;
                }
                *(u16*)((s32)player + 0x30) = *(u16*)((s32)player + 0x2E);
                *(u16*)((s32)player + 0x2E) = 0x14;
                if (marioMotTbl[*(u16*)((s32)player + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)((s32)player + 0x30)][2]();
                }
                *(u32*)((s32)player + 0xC) |= 1;
                return;
            }
        } else {
            *(f32*)((s32)player + 0x90) += speed;
        }
    }

    if (*(s8*)((s32)player + 0x3C) == 2) {
        if (kpaChkHitobjRide() != 0) {
            marioLandOn();
        }
    } else if (*(void**)((s32)player + 0x1E8) != 0) {
        marioLandOn();
    }
}

u8 marioJump(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgMoveMotion(void) {
    extern void mario_plane_cancel(void);
    extern void allPartyForceRideOn(void);

    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);

    if (motion == 0x18) {
        mario_plane_cancel();
    } else if (motion == 0x16) {
        allPartyForceRideOn();
    } else if ((u16)(motion - 0x1F) > 1 && motion != 0x1A) {
        if (*(f32*)((s32)mario + 0x180) == float_0_804209a0) {
            void* wp = marioGetPtr();
            u16 m = *(u16*)((s32)wp + 0x2E);
            if (m != 0x19 && m != 0x1A && m != 0x15 && m != 0x16) {
                wp = marioGetPtr();
                if ((*(u32*)wp & 0x10000000) == 0) {
                    if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                        if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                            *(u32*)((s32)wp + 4) &= ~0x08000000;
                        }
                        if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                            *(u32*)((s32)wp + 4) &= ~4;
                        }
                    }
                    *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                    *(u16*)((s32)wp + 0x2E) = 0;
                    if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                        marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                    }
                    *(u32*)((s32)wp + 0xC) |= 1;
                }
            }
        }
        if (*(f32*)((s32)mario + 0x180) > *(f32*)((s32)mario + 0x184)) {
            void* wp = marioGetPtr();
            if ((*(u32*)wp & 0x10000000) == 0) {
                if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                    if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                        *(u32*)((s32)wp + 4) &= ~0x08000000;
                    }
                    if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                        *(u32*)((s32)wp + 4) &= ~4;
                    }
                }
                *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                *(u16*)((s32)wp + 0x2E) = 2;
                if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                }
                *(u32*)((s32)wp + 0xC) |= 1;
            }
        } else {
            void* wp = marioGetPtr();
            if ((*(u32*)wp & 0x10000000) == 0) {
                if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                    if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                        *(u32*)((s32)wp + 4) &= ~0x08000000;
                    }
                    if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                        *(u32*)((s32)wp + 4) &= ~4;
                    }
                }
                *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                *(u16*)((s32)wp + 0x2E) = 1;
                if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                }
                *(u32*)((s32)wp + 0xC) |= 1;
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_kaze(void) {
    extern f32 float_5_80420a00;
    extern f32 float_60_80420a04;
    extern f32 float_180_80420a08;
    extern f32 float_270_80420a0c;
    extern f32 float_6_80420a10;
    extern f32 float_90_804209f4;
    extern char str_M_P_2_804209f8[];
    extern f32 revise360(double);

    void* mario = marioGetPtr();

    if ((*(u32*)((s32)mario + 0xC) & 1) != 0) {
        f32 zero;
        *(u32*)((s32)mario + 0xC) &= ~1;
        zero = float_0_804209a0;
        *(u32*)mario &= ~0xF0000;
        *(u32*)((s32)mario + 4) |= 0x100;
        *(s32*)((s32)mario + 0x48) = 0;
        *(s32*)((s32)mario + 0x44) = 0;
        *(f32*)((s32)mario + 0x180) = zero;
        *(f32*)((s32)mario + 0x2B8) = zero;
        marioChgPose(str_M_P_2_804209f8);
    }

    if (*(s32*)((s32)mario + 0x44) == 100) {
        *(f32*)((s32)mario + 0x2B8) -= float_6_80420a10;
        if (*(f32*)((s32)mario + 0x2B8) < float_0_804209a0) {
            void* wp;
            *(f32*)((s32)mario + 0x2B8) = float_0_804209a0;
            *(u32*)((s32)mario + 4) &= ~0x100;
            wp = marioGetPtr();
            if ((*(u32*)wp & 0x10000000) == 0) {
                if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                    if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                        *(u32*)((s32)wp + 4) &= ~0x08000000;
                    }
                    if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                        *(u32*)((s32)wp + 4) &= ~4;
                    }
                }
                *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                *(u16*)((s32)wp + 0x2E) = 0;
                if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                }
                *(u32*)((s32)wp + 0xC) |= 1;
            }
        }
        *(f32*)((s32)mario + 0x1AC) = revise360((double)(*(f32*)((s32)mario + 0x1AC) + *(f32*)((s32)mario + 0x2B8)));
        *(f32*)((s32)mario + 0x1B0) = (*(f32*)((s32)mario + 0x1AC) > float_90_804209f4 && *(f32*)((s32)mario + 0x1AC) < float_270_80420a0c) ? float_180_80420a08 : float_0_804209a0;
    } else if (*(s32*)((s32)mario + 0x44) == 0) {
        *(f32*)((s32)mario + 0x2B8) += float_5_80420a00;
        if (*(f32*)((s32)mario + 0x2B8) > float_60_80420a04) {
            *(f32*)((s32)mario + 0x2B8) = float_60_80420a04;
        }
        *(f32*)((s32)mario + 0x1AC) = revise360((double)(*(f32*)((s32)mario + 0x1AC) + *(f32*)((s32)mario + 0x2B8)));
        if ((*(u16*)((s32)mario + 0x24A) & 0x10) == 0) {
            *(f32*)((s32)mario + 0x1B0) = (*(f32*)((s32)mario + 0x1AC) > float_90_804209f4 && *(f32*)((s32)mario + 0x1AC) < float_270_80420a0c) ? float_180_80420a08 : float_0_804209a0;
            *(s32*)((s32)mario + 0x44) = 100;
            *(s32*)((s32)mario + 0x48) = 10;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkJump(void) {
    extern u32 hitGetAttr(void*);
    extern void* mobjHitObjPtrToPtr(s32);
    extern s32 mobjGetHint(void*);
    extern void* caseCheckHitObj(void*);
    extern void* fbatNpcTalkCheck(void);

    void* mario = marioGetPtr();
    void* hit;
    void* c;

    if ((*(u16*)((s32)mario + 0x24C) & 0x100) == 0) {
        return 0;
    }
    if (*(u16*)((s32)mario + 0x2E) == 0x1A) {
        return 0;
    }

    hit = *(void**)((s32)mario + 0x1E0);
    if (hit != 0) {
        if ((hitGetAttr(hit) & 0x80000000) != 0) {
            if (mobjGetHint(mobjHitObjPtrToPtr((s32)hit)) == 1) {
                return 0;
            }
        }
        c = caseCheckHitObj(hit);
        if (c != 0 && (*(s32*)((s32)c + 4) == 9 || *(s32*)((s32)c + 4) == 4 || *(s32*)((s32)c + 4) == 0xC)) {
            return 0;
        }
    }

    hit = *(void**)((s32)mario + 0x1E4);
    if (hit != 0) {
        if ((hitGetAttr(hit) & 0x80000000) != 0) {
            if (mobjGetHint(mobjHitObjPtrToPtr((s32)hit)) == 1) {
                return 0;
            }
        }
        c = caseCheckHitObj(hit);
        if (c != 0 && (*(s32*)((s32)c + 4) == 9 || *(s32*)((s32)c + 4) == 4 || *(s32*)((s32)c + 4) == 0xC)) {
            return 0;
        }
    }

    hit = *(void**)((s32)mario + 0x1E8);
    if (hit != 0) {
        if ((hitGetAttr(hit) & 0x80000000) != 0) {
            if (mobjGetHint(mobjHitObjPtrToPtr((s32)hit)) == 1) {
                return 0;
            }
        }
        c = caseCheckHitObj(hit);
        if (c != 0 && *(s32*)((s32)c + 4) == 5) {
            return 0;
        }
    }

    if (fbatNpcTalkCheck() != 0) {
        return 0;
    }

    mario = marioGetPtr();
    if ((*(u32*)mario & 0x10000000) == 0) {
        if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
            if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                *(u32*)((s32)mario + 4) &= ~0x08000000;
            }
            if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                *(u32*)((s32)mario + 4) &= ~4;
            }
        }
        *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
        *(u16*)((s32)mario + 0x2E) = 3;
        if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
            marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
        }
        *(u32*)((s32)mario + 0xC) |= 1;
    }
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 mot_talk(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkTransform(void) {
    extern s32 pouchCheckItem(s32);
    extern s32 marioChkShipPlace(s32);
    extern s32 marioChkPlanePlace(s32);

    void* mario = marioGetPtr();

    if ((*(u16*)((s32)mario + 0x24C) & 0x800) == 0) {
        return 0;
    }
    if ((*(u32*)((s32)mario + 0xC) & 4) == 0 && *(s8*)((s32)mario + 0x3B) != 0) {
        return 0;
    }
    if (*(u16*)((s32)mario + 0x2E) == 0x1A) {
        return 0;
    }
    if (pouchCheckItem(5) > 0 && marioChkShipPlace(*(s32*)((s32)mario + 0x1E8)) != 0) {
        void* wp = marioGetPtr();
        if ((*(u32*)wp & 0x10000000) == 0) {
            if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)wp + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                    *(u32*)((s32)wp + 4) &= ~4;
                }
            }
            *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
            *(u16*)((s32)wp + 0x2E) = 0x19;
            if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
            }
            *(u32*)((s32)wp + 0xC) |= 1;
        }
        return 0;
    }
    if (pouchCheckItem(4) > 0 && marioChkPlanePlace(*(s32*)((s32)mario + 0x1E8)) != 0) {
        void* wp = marioGetPtr();
        if ((*(u32*)wp & 0x10000000) == 0) {
            if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)wp + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                    *(u32*)((s32)wp + 4) &= ~4;
                }
            }
            *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
            *(u16*)((s32)wp + 0x2E) = 0x18;
            if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
            }
            *(u32*)((s32)wp + 0xC) |= 1;
        }
        return 0;
    }
    return 1;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioSetJumpPara(void) {
    extern f32 marioJumpData[];
    extern f32 marioJumpSwData[];
    extern f32 float_0p5_80420a1c;
    extern f32 float_1_804209b4;
    extern f32 float_2_80420a44;
    extern f32 float_neg0p003_80420a48;
    extern f32 float_0p029_80420a4c;
    extern f32 float_neg0p04_80420a50;
    extern f32 float_neg0p01_804209d4;
    extern f32 float_2p7_80420a54;
    extern f32 float_neg0p002_804209d0;
    extern f32 float_0p016_80420a58;
    extern void kpaSetJumpPara(void);

    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);

    switch (motion) {
        case 3:
        case 6:
        case 7:
        case 0x18:
        case 0x19:
            if ((*(u32*)((s32)mario + 0xC) & 0x10000) == 0) {
                if ((*(u32*)mario & 0x1000) == 0) {
                    *(f32*)((s32)mario + 0x7C) = marioJumpData[0];
                    *(f32*)((s32)mario + 0x80) = marioJumpData[1];
                    *(f32*)((s32)mario + 0x84) = marioJumpData[2];
                    *(f32*)((s32)mario + 0x88) = marioJumpData[3];
                } else {
                    *(f32*)((s32)mario + 0x7C) = float_0p5_80420a1c * marioJumpData[0];
                    *(f32*)((s32)mario + 0x80) = float_0p5_80420a1c * marioJumpData[1];
                    *(f32*)((s32)mario + 0x84) = float_0p5_80420a1c * marioJumpData[2];
                    *(f32*)((s32)mario + 0x88) = float_0p5_80420a1c * marioJumpData[3];
                }
                if (*(f32*)((s32)mario + 0x228) != float_1_804209b4) {
                    *(f32*)((s32)mario + 0x7C) = float_2_80420a44;
                    *(f32*)((s32)mario + 0x80) = float_neg0p003_80420a48;
                    *(f32*)((s32)mario + 0x84) = float_0p029_80420a4c;
                    *(f32*)((s32)mario + 0x88) = float_neg0p003_80420a48;
                }
            } else if ((*(u32*)mario & 0x1000) != 0) {
                *(f32*)((s32)mario + 0x7C) = float_0p5_80420a1c * marioJumpData[0];
                *(f32*)((s32)mario + 0x80) = float_0p5_80420a1c * marioJumpData[1];
                *(f32*)((s32)mario + 0x84) = float_0p5_80420a1c * marioJumpData[2];
                *(f32*)((s32)mario + 0x88) = float_0p5_80420a1c * marioJumpData[3];
            }
            break;
        case 4:
            *(f32*)((s32)mario + 0x7C) = float_2_80420a44;
            *(f32*)((s32)mario + 0x80) = float_neg0p04_80420a50;
            *(f32*)((s32)mario + 0x84) = float_neg0p01_804209d4;
            *(f32*)((s32)mario + 0x88) = float_neg0p003_80420a48;
            break;
        case 5:
            *(f32*)((s32)mario + 0x7C) = marioJumpSwData[0];
            *(f32*)((s32)mario + 0x80) = marioJumpSwData[1];
            *(f32*)((s32)mario + 0x84) = marioJumpSwData[2];
            *(f32*)((s32)mario + 0x88) = marioJumpSwData[3];
            break;
        case 0x16:
            *(f32*)((s32)mario + 0x7C) = marioJumpData[0];
            *(f32*)((s32)mario + 0x80) = marioJumpData[1];
            *(f32*)((s32)mario + 0x84) = marioJumpData[2];
            *(f32*)((s32)mario + 0x88) = marioJumpData[3];
            if (*(f32*)((s32)mario + 0x228) != float_1_804209b4) {
                *(f32*)((s32)mario + 0x7C) = float_2p7_80420a54;
                *(f32*)((s32)mario + 0x80) = float_neg0p002_804209d0;
                *(f32*)((s32)mario + 0x84) = float_0p016_80420a58;
                *(f32*)((s32)mario + 0x88) = float_neg0p002_804209d0;
            }
            break;
    }
    if (*(u16*)((s32)mario + 0x2E) != 8 && *(s8*)((s32)mario + 0x3C) == 2) {
        kpaSetJumpPara();
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkItemMotion(void) {
    extern s32 marioGetHammerLevel(void);
    extern s32 marioChkHammer2(void);

    void* mario = marioGetPtr();

    if ((*(u32*)mario & 0x80000000) != 0) {
        return 0;
    }
    if (marioGetHammerLevel() == 0) {
        return 0;
    }
    if (marioChkHammer2() != 0) {
        void* wp = marioGetPtr();
        if ((*(u32*)wp & 0x10000000) == 0) {
            if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)wp + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                    *(u32*)((s32)wp + 4) &= ~4;
                }
            }
            *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
            *(u16*)((s32)wp + 0x2E) = 0x13;
            if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
            }
            *(u32*)((s32)wp + 0xC) |= 1;
        }
        return 1;
    } else {
        u32 pressed = *(u16*)((s32)mario + 0x24C) & 0x200;
        if (pressed == 0) {
            return 0;
        }
        if ((*(u32*)mario & 0x01100000) != 0) {
            return 0;
        }
        if (pressed == 0) {
            return 0;
        }
        mario = marioGetPtr();
        if ((*(u32*)mario & 0x10000000) == 0) {
            if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
                if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)mario + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                    *(u32*)((s32)mario + 4) &= ~4;
                }
            }
            *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
            *(u16*)((s32)mario + 0x2E) = 0x12;
            if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
            }
            *(u32*)((s32)mario + 0xC) |= 1;
        }
        return 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioChgJumpStandMotion(double param_1) {
    extern void camFollowYOn(void);
    extern s32 marioChkSts(u32);
    extern void effKemuriEntry(double, double, double, double, s32);
    extern void yoshi_jumpStand(void);

    void* mario = marioGetPtr();
    f32 value = (f32)param_1;

    camFollowYOn();
    if (marioChkSts(0x10) != 0) {
        *(f32*)((s32)mario + 0x2D0) = value;
        effKemuriEntry(*(f32*)((s32)mario + 0x8C), *(f32*)((s32)mario + 0x90), *(f32*)((s32)mario + 0x94), float_0_804209a0, 0);
        mario = marioGetPtr();
        if ((*(u32*)mario & 0x10000000) == 0) {
            if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
                if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)mario + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                    *(u32*)((s32)mario + 4) &= ~4;
                }
            }
            *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
            *(u16*)((s32)mario + 0x2E) = 6;
            if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
            }
            *(u32*)((s32)mario + 0xC) |= 1;
        }
    } else if (*(u16*)((s32)mario + 0x2E) == 0x1A) {
        *(f32*)((s32)mario + 0x2D0) = value;
        yoshi_jumpStand();
    } else {
        *(f32*)((s32)mario + 0x2D0) = value;
        mario = marioGetPtr();
        if ((*(u32*)mario & 0x10000000) == 0) {
            if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
                if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)mario + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                    *(u32*)((s32)mario + 4) &= ~4;
                }
            }
            *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
            *(u16*)((s32)mario + 0x2E) = 6;
            if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
            }
            *(u32*)((s32)mario + 0xC) |= 1;
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioBoots(void) {
    extern s32 kpaHipAttackOk(void);
    extern s32 marioChkHipAttack(void);

    void* mario = marioGetPtr();
    u32 flags = *(u32*)mario;

    if ((flags & 0x80000000) == 0 && (flags & 0x00100000) == 0) {
        if (*(s8*)((s32)mario + 0x3C) == 2) {
            if (*(s16*)((s32)mario + 0x52) >= 6 && kpaHipAttackOk() != 0 && (*(u16*)((s32)mario + 0x24C) & 0x100) != 0) {
                void* wp = marioGetPtr();
                if ((*(u32*)wp & 0x10000000) == 0) {
                    if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                        if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                            *(u32*)((s32)wp + 4) &= ~0x08000000;
                        }
                        if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                            *(u32*)((s32)wp + 4) &= ~4;
                        }
                    }
                    *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                    *(u16*)((s32)wp + 0x2E) = 0x10;
                    if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                        marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                    }
                    *(u32*)((s32)wp + 0xC) |= 1;
                }
            }
        } else if ((flags & 0x00080000) != 0) {
            if (marioChkHipAttack() != 0) {
                void* wp = marioGetPtr();
                if ((*(u32*)wp & 0x10000000) == 0) {
                    if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                        if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                            *(u32*)((s32)wp + 4) &= ~0x08000000;
                        }
                        if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                            *(u32*)((s32)wp + 4) &= ~4;
                        }
                    }
                    *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
                    *(u16*)((s32)wp + 0x2E) = 0x10;
                    if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                        marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
                    }
                    *(u32*)((s32)wp + 0xC) |= 1;
                }
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioRollChgChk(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern s32 marioGetRub(s32, void*, s32*, void*);
    extern s32 pouchCheckItem(s32);
    extern s32 marioSlitChkWallAround(void);
    extern s32 marioCreviceWallChk(Vec*);

    void* mario = marioGetPtr();
    s32 rub;
    s32 dir;
    s32 out1;
    s32 out2;
    Vec pos;

    if ((*(u32*)mario & 0x80000000) != 0) {
        return 0;
    }
    if ((*(u16*)((s32)mario + 0x24A) & 0x20) == 0) {
        return 0;
    }
    rub = marioGetRub(0x20, &out1, &dir, &out2);
    if (rub != 1) {
        return 0;
    }
    if (dir < 3) {
        return 0;
    }
    if (pouchCheckItem(3) == 0) {
        return 0;
    }
    if (marioSlitChkWallAround() == 0) {
        pos.x = *(f32*)((s32)mario + 0x8C);
        pos.y = *(f32*)((s32)mario + 0x90);
        pos.z = *(f32*)((s32)mario + 0x94);
        if (marioCreviceWallChk(&pos) != 0) {
            return 0;
        }
    } else {
        return 0;
    }

    if ((*(u16*)((s32)mario + 0x24A) & 0x20) != 0) {
        void* wp = marioGetPtr();
        if ((*(u32*)wp & 0x10000000) == 0) {
            if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
                if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)wp + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                    *(u32*)((s32)wp + 4) &= ~4;
                }
            }
            *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
            *(u16*)((s32)wp + 0x2E) = 0x16;
            if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
            }
            *(u32*)((s32)wp + 0xC) |= 1;
        }
        return 0x16;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

u8 mot_forceReset(void) {
    return 0;
}


u8 mot_shadow(void) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgSmallJumpMotion(void) {
    extern f32 float_neg0p5_80420a30;
    extern f32 float_6_80420a10;
    void* mario = marioGetPtr();
    void* wp;

    *(u32*)((s32)mario + 0xC) |= 0x10000;
    wp = marioGetPtr();
    if ((*(u32*)wp & 0x10000000) == 0) {
        if (*(u16*)((s32)wp + 0x30) == 0x10 || *(u16*)((s32)wp + 0x30) == 0x11) {
            if ((*(u32*)((s32)wp + 4) & 0x08000000) != 0) {
                *(u32*)((s32)wp + 4) &= ~0x08000000;
            }
            if ((*(u32*)((s32)wp + 4) & 4) != 0) {
                *(u32*)((s32)wp + 4) &= ~4;
            }
        }
        *(u16*)((s32)wp + 0x30) = *(u16*)((s32)wp + 0x2E);
        *(u16*)((s32)wp + 0x2E) = 3;
        if (marioMotTbl[*(u16*)((s32)wp + 0x30)][2] != 0) {
            marioMotTbl[*(u16*)((s32)wp + 0x30)][2]();
        }
        *(u32*)((s32)wp + 0xC) |= 1;
    }
    *(u32*)mario |= 0x80;
    *(void**)((s32)mario + 0x1EC) = *(void**)((s32)mario + 0x1E8);
    *(void**)((s32)mario + 0x1E8) = 0;
    *(f32*)((s32)mario + 0x7C) = float_6_80420a10;
    *(f32*)((s32)mario + 0x80) = float_neg0p5_80420a30;
    *(f32*)((s32)mario + 0x84) = float_0_804209a0;
    *(f32*)((s32)mario + 0x88) = float_0_804209a0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgMotSub(s16 newMotionId, int param_2) {
    void* mario = marioGetPtr();

    if ((*(u32*)mario & 0x10000000) == 0) {
        if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
            if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                *(u32*)((s32)mario + 4) &= ~0x08000000;
            }
            if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                *(u32*)((s32)mario + 4) &= ~4;
            }
        }
        *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
        *(u16*)((s32)mario + 0x2E) = newMotionId;
        if (param_2 != 0) {
            if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
            }
        } else {
            if (*(u16*)((s32)mario + 0x30) == 0x14) {
                if (marioMotTbl[0x14][2] != 0) {
                    marioMotTbl[0x14][2]();
                }
            }
        }
        *(u32*)((s32)mario + 0xC) |= 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgGetItemMotion(void) {
    extern void marioResetRollSpd(void);
    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);

    if (motion == 0x16) {
        marioResetRollSpd();
    } else if (motion == 0x15) {
        *(f32*)((s32)mario + 0x180) = float_0_804209a0;
    } else if (motion == 0x19) {
        *(f32*)((s32)mario + 0x180) = float_0_804209a0;
    } else if (motion != 0x1A) {
        mario = marioGetPtr();
        if ((*(u32*)mario & 0x10000000) == 0) {
            if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
                if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)mario + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                    *(u32*)((s32)mario + 4) &= ~4;
                }
            }
            *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
            *(u16*)((s32)mario + 0x2E) = 0xF;
            if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
            }
            *(u32*)((s32)mario + 0xC) |= 1;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgStayMotion(void) {
    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);

    if (motion != 0x19 && motion != 0x1A && motion != 0x15 && motion != 0x16) {
        mario = marioGetPtr();
        if ((*(u32*)mario & 0x10000000) == 0) {
            if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
                if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                    *(u32*)((s32)mario + 4) &= ~0x08000000;
                }
                if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                    *(u32*)((s32)mario + 4) &= ~4;
                }
            }
            *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
            *(u16*)((s32)mario + 0x2E) = 0;
            if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
            }
            *(u32*)((s32)mario + 0xC) |= 1;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioChgMotJump2(void) {
    extern s32 kpaGetBodyStatus(void);
    void* mario = marioGetPtr();

    if (kpaGetBodyStatus() != 2) {
        if (*(u16*)((s32)mario + 0x2E) != 0x16) {
            mario = marioGetPtr();
            if ((*(u32*)mario & 0x10000000) == 0) {
                if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
                    if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                        *(u32*)((s32)mario + 4) &= ~0x08000000;
                    }
                    if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                        *(u32*)((s32)mario + 4) &= ~4;
                    }
                }
                *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
                *(u16*)((s32)mario + 0x2E) = 4;
                if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
                }
                *(u32*)((s32)mario + 0xC) |= 1;
            }
        }
    }
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgMot(s32 motion) {
    void* mario = marioGetPtr();

    if ((*(u32*)mario & 0x10000000) == 0) {
        if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
            if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                *(u32*)((s32)mario + 4) &= ~0x08000000;
            }
            if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                *(u32*)((s32)mario + 4) &= ~4;
            }
        }
        *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
        *(u16*)((s32)mario + 0x2E) = motion;
        if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
            marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
        }
        *(u32*)((s32)mario + 0xC) |= 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 marioChkTalkable(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern s32 marioBgmodeChk(void);
    extern s32 vivianGetStatus(void);
    void* mario = marioGetPtr();
    void* party;
    s32 result = 0;

    party = partyGetPtr(marioGetPartyId());
    if (party == 0 || *(s8*)((s32)party + 0x31) != 0x1A || (*(u32*)party & 1) == 0) {
        if (*(u16*)((s32)mario + 0x2E) < 3 && ((*(u32*)((s32)mario + 0xC) & 1) == 0) &&
            marioBgmodeChk() != 1 && ((*(u32*)((s32)mario + 4) & 0x10000000) == 0)) {
            result = 1;
        }
        if (vivianGetStatus() != 0) {
            result = 0;
        }
    }
    return result;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgTalkMotion(void) {
    void* mario = marioGetPtr();

    if ((*(u32*)mario & 0x10000000) == 0) {
        if (*(u16*)((s32)mario + 0x30) == 0x10 || *(u16*)((s32)mario + 0x30) == 0x11) {
            if ((*(u32*)((s32)mario + 4) & 0x08000000) != 0) {
                *(u32*)((s32)mario + 4) &= ~0x08000000;
            }
            if ((*(u32*)((s32)mario + 4) & 4) != 0) {
                *(u32*)((s32)mario + 4) &= ~4;
            }
        }
        *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
        *(u16*)((s32)mario + 0x2E) = 0xE;
        if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
            marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
        }
        *(u32*)((s32)mario + 0xC) |= 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

f32 marioGetFallSpd(void) {
    extern void* marioGetPtr(void);
    extern f32 marioJumpData[];
    extern f32 float_0_804209a0;
    void* mario = marioGetPtr();

    *(f32*)((s32)mario + 0x84) += *(f32*)((s32)mario + 0x88);
    if (*(f32*)((s32)mario + 0x84) > float_0_804209a0) {
        *(f32*)((s32)mario + 0x84) = float_0_804209a0;
    }
    *(f32*)((s32)mario + 0x80) += *(f32*)((s32)mario + 0x84);
    *(f32*)((s32)mario + 0x7C) += *(f32*)((s32)mario + 0x80);
    if (*(f32*)((s32)mario + 0x7C) <= marioJumpData[9]) {
        *(f32*)((s32)mario + 0x7C) = marioJumpData[9];
        *(f32*)((s32)mario + 0x80) = float_0_804209a0;
        *(f32*)((s32)mario + 0x84) = float_0_804209a0;
        *(f32*)((s32)mario + 0x88) = float_0_804209a0;
    }
    return *(f32*)((s32)mario + 0x7C);
}

void marioSetFallPara(void) {
    extern void* marioGetPtr(void);
    extern f32 marioJumpData[];
    extern f32 marioJumpSwData[];
    extern f32 float_1_804209b4;
    extern f32 float_0p34_804209cc;
    extern f32 float_neg0p002_804209d0;
    extern f32 float_neg0p01_804209d4;
    extern f32 float_0_804209a0;
    void* mario = marioGetPtr();
    f32* data = &marioJumpData[4];

    if (*(u16*)((s32)mario + 0x2E) == 5) {
        data = &marioJumpSwData[4];
    }
    *(f32*)((s32)mario + 0x7C) = data[0];
    *(f32*)((s32)mario + 0x80) = data[1];
    *(f32*)((s32)mario + 0x84) = data[2];
    *(f32*)((s32)mario + 0x88) = data[3];
    if (*(f32*)((s32)mario + 0x228) != float_1_804209b4) {
        *(f32*)((s32)mario + 0x7C) = float_0p34_804209cc;
        *(f32*)((s32)mario + 0x80) = float_neg0p002_804209d0;
        *(f32*)((s32)mario + 0x84) = float_neg0p01_804209d4;
        *(f32*)((s32)mario + 0x88) = float_0_804209a0;
    }
}

u32 marioChkItemGetMotion(void) {
    extern s32 marioShipChanging(void);
    void* mario = marioGetPtr();
    u16 motion = *(u16*)((s32)mario + 0x2E);
    u32 result = 0;

    if (motion == 0 || (u16)(motion - 1) <= 1 || motion == 0x1A || motion == 0x16 || motion == 0x15) {
        if ((*(u32*)((s32)mario + 0xC) & 1) == 0) {
            result = 1;
        }
    } else if (motion == 0x19) {
        if (marioShipChanging() == 3) {
            result = 1;
        }
    }
    return result;
}

double marioMakeJumpPara(void) {
    extern f32 float_1_804209b4;
    extern f32 float_1p7_80420a34;
    void* mario = marioGetPtr();
    f32 div;

    if ((*(u32*)mario & 0x1000) != 0) {
        div = float_1p7_80420a34;
    } else {
        div = float_1_804209b4;
    }
    *(f32*)((s32)mario + 0x84) += *(f32*)((s32)mario + 0x88) / div;
    *(f32*)((s32)mario + 0x80) += *(f32*)((s32)mario + 0x84) / div;
    *(f32*)((s32)mario + 0x7C) += *(f32*)((s32)mario + 0x80) / div;
    return *(f32*)((s32)mario + 0x7C);
}

void marioChgMot2(s16 motionId) {
    void* mario = marioGetPtr();

    if ((*(u32*)mario & 0x10000000) == 0) {
        *(u16*)((s32)mario + 0x30) = *(u16*)((s32)mario + 0x2E);
        *(u16*)((s32)mario + 0x2E) = motionId;
        *(u32*)((s32)mario + 0xC) &= ~1;
        if (marioMotTbl[*(u16*)((s32)mario + 0x30)][2] != 0) {
            marioMotTbl[*(u16*)((s32)mario + 0x30)][2]();
        }
    }
}

