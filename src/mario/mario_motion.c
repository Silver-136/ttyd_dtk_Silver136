#include "mario/mario_motion.h"
#include "bowser/koopa_hit.h"
#include "bowser/koopa_motion.h"

extern const f32 float_0_804209a0;
extern char str_M_I_U_80420a14[];
extern void mot_stay(void);
extern void mot_stay_post(void);
extern void mot_walk(void);
extern void mot_dash(void);
extern void mot_jump(void);
extern void mot_jumpNpc(void);
extern void N_mot_jumpNpc_post(void);
extern void mot_jumpSw(void);
extern void mot_jumpStand(void);
extern void mot_jumpSmall(void);
extern void mot_fall(void);
extern void mot_fall_post(void);
extern void mot_upstairs(void);
extern void mot_land(void);
extern void mot_talk(void);
extern void mot_getItem(void);
extern void mot_hip(void);
extern void mot_hip_post(void);
extern void mot_hammer(void);
extern void mot_hammer2(void);
extern void mot_ham2_post(void);
extern void mot_jabara(void);
extern void mot_jabara_post(void);
extern void mot_slit(void);
extern void mot_slit_post(void);
extern void mot_roll(void);
extern void mot_roll_post(void);
extern void mot_kaze(void);
extern void mot_plane(void);
extern void mot_plane_post(void);
extern void mot_ship(void);
extern void mot_ship_post(void);
extern void mot_yoshi(void);
extern void mot_yoshi_post(void);
extern void mot_cloud(void);
extern void cloudResetAt(void);
extern void mot_vivian(void);
extern void mot_vivian_post(void);
extern void mot_dokan(void);
extern void mot_grasp(void);
extern void mot_damage(void);
extern void mot_damageToge(void);
extern void mot_bottomless(void);
extern void mot_forceReset(void);
extern void mot_shadow(void);
extern void mot_partyUse(void);
extern void kpa_swim(void);
extern void kpa_powup(void);
extern void kpa_powdown(void);
extern void mot_dummy(void);

__declspec(section ".data") void (*marioMotTbl[41][3])(void) = {
    { mot_stay, 0, mot_stay_post },
    { mot_walk, 0, 0 },
    { mot_dash, 0, 0 },
    { mot_jump, 0, 0 },
    { mot_jumpNpc, 0, N_mot_jumpNpc_post },
    { mot_jumpSw, 0, 0 },
    { mot_jumpStand, 0, 0 },
    { mot_jump, 0, 0 },
    { mot_jump, 0, 0 },
    { mot_jumpSmall, 0, 0 },
    { mot_fall, 0, mot_fall_post },
    { mot_fall, 0, 0 },
    { mot_upstairs, 0, 0 },
    { mot_land, 0, 0 },
    { mot_talk, 0, 0 },
    { mot_getItem, 0, 0 },
    { mot_hip, 0, mot_hip_post },
    { mot_hip, 0, mot_hip_post },
    { mot_hammer, 0, 0 },
    { mot_hammer2, 0, mot_ham2_post },
    { mot_jabara, 0, mot_jabara_post },
    { mot_slit, 0, mot_slit_post },
    { mot_roll, 0, mot_roll_post },
    { mot_kaze, 0, 0 },
    { mot_plane, 0, mot_plane_post },
    { mot_ship, 0, mot_ship_post },
    { mot_yoshi, 0, mot_yoshi_post },
    { mot_cloud, 0, cloudResetAt },
    { mot_vivian, 0, mot_vivian_post },
    { mot_dokan, 0, 0 },
    { mot_grasp, 0, 0 },
    { mot_damage, 0, 0 },
    { mot_damageToge, 0, 0 },
    { mot_bottomless, 0, 0 },
    { mot_forceReset, 0, 0 },
    { mot_shadow, 0, 0 },
    { mot_partyUse, 0, 0 },
    { kpa_swim, 0, 0 },
    { kpa_powup, 0, 0 },
    { kpa_powdown, 0, 0 },
    { mot_dummy, 0, 0 },
};

__declspec(section ".sdata") f32 marioJumpData[10] = {
    8.0f,
    -0.84f,
    0.08f,
    -0.02f,
    -0.1f,
    -0.2f,
    0.024f,
    -0.004f,
    32.0f,
    -18.0f,
};

void* marioGetPtr(void);
void marioReInit_ship(void);
void marioAdjustMoveDir(void);
void marioChgPose(const void* pose);
void marioLandOn(void);
extern void camFollowYOn(void);
extern s32 marioChkSts(u32);
extern const f32 float_neg0p5_80420a30;
extern const f32 float_6_80420a10;
extern s32 marioGetPartyId(void);
extern s32 marioBgmodeChk(void);
extern f32 marioJumpSwData[];
extern const f32 float_0p5_80420a1c;
extern const f32 float_1_804209b4;
extern const f32 float_neg0p01_804209d4;
extern const f32 float_neg0p002_804209d0;
extern const f32 float_0p34_804209cc;
extern const f32 float_1p7_80420a34;
extern s32 getRollEvtFlag(void);
extern void* marioChkLandon(f32, void*);
extern s32 marioChkInScreen(s32, s32);
extern s32 marioPriCheckJabara(void);
extern void* gp;
extern char str_M_S_1_804209ec[];
extern const f32 float_10_804209c4;
extern const f32 float_90_804209f4;
extern const f32 float_180_80420a08;
extern const f32 float_270_80420a0c;
extern s32 pouchCheckItem(s32);

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


void marioChgShipMotion(void) {
    marioReInit_ship();
}

void marioChgRollMotion(void) {
    typedef struct MarioRollWork {
        u8 pad_00[0x2E];
        s16 motion;
        u8 pad_30[0x14];
        s32 mode;
    } MarioRollWork;
    MarioRollWork* mario = marioGetPtr();

    mario->mode = 0x32;
    mario->motion = 0x16;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioChgJumpStandMotion(double param_1) {
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
void marioChgSmallJumpMotion(void) {
    void* mario = marioGetPtr();
    void* wp;
    f32 jumpSpeed;
    f32 jumpAccel;
    f32 zero;

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
    jumpSpeed = float_6_80420a10;
    jumpAccel = float_neg0p5_80420a30;
    zero = float_0_804209a0;
    *(u32*)mario |= 0x80;
    *(void**)((s32)mario + 0x1EC) = *(void**)((s32)mario + 0x1E8);
    *(void**)((s32)mario + 0x1E8) = 0;
    *(f32*)((s32)mario + 0x7C) = jumpSpeed;
    *(f32*)((s32)mario + 0x80) = jumpAccel;
    *(f32*)((s32)mario + 0x84) = zero;
    *(f32*)((s32)mario + 0x88) = zero;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkTalkable(void) {
    extern void* partyGetPtr(s32 id);
    extern s32 vivianGetStatus(void);
    void* mario = marioGetPtr();
    void* party;
    s32 result = 0;
    s16 motion;

    party = partyGetPtr(marioGetPartyId());
    if (party != 0 && *(s8*)((s32)party + 0x31) == 4 && (*(u32*)party & 0x100) != 0) {
        return 0;
    }
    motion = *(s16*)((s32)mario + 0x2E);
    if (motion >= 0 && motion < 3 && ((*(u32*)((s32)mario + 0xC) & 1) == 0) &&
        marioBgmodeChk() != 1 && ((*(u32*)((s32)mario + 4) & 0x01000000) == 0)) {
        result = 1;
    }
    if (vivianGetStatus() != 0) {
        result = 0;
    }
    return result;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioBoots(void) {
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
void marioMotion(void) {
    void* mario = marioGetPtr();
    *(s32*)((s32)mario + 0x200) = 0;
    do {
        marioMotTbl[*(u16*)((s32)mario + 0x2E)][0]();
    } while (*(u32*)((s32)mario + 0xC) & 1);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


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

void marioClearJumpPara(void) {
    void* mario = marioGetPtr();
    f32 zero = float_0_804209a0;
    *(f32*)((s32)mario + 0x7C) = zero;
    *(f32*)((s32)mario + 0x80) = zero;
    *(f32*)((s32)mario + 0x84) = zero;
    *(f32*)((s32)mario + 0x88) = zero;
}
void marioSetJumpPara(void) {
    extern const f32 float_2_80420a44;
    extern const f32 float_neg0p003_80420a48;
    extern const f32 float_0p029_80420a4c;
    extern const f32 float_neg0p04_80420a50;
    extern const f32 float_2p7_80420a54;
    extern const f32 float_0p016_80420a58;
    extern void kpaSetJumpPara(void);

    void* mario = marioGetPtr();
    f32 value1;
    f32 value2;
    u16 motion = *(u16*)((s32)mario + 0x2E);

    switch (motion) {
        case 5:
            *(f32*)((s32)mario + 0x7C) = marioJumpSwData[0];
            *(f32*)((s32)mario + 0x80) = marioJumpSwData[1];
            *(f32*)((s32)mario + 0x84) = marioJumpSwData[2];
            *(f32*)((s32)mario + 0x88) = marioJumpSwData[3];
            break;
        case 3:
        case 6:
        case 7:
        case 0x18:
        case 0x19:
            if ((*(u32*)((s32)mario + 0xC) & 0x10000) != 0) {
                u32 flags = *(u32*)mario;
                f32* data = marioJumpData;
                if ((flags & 0x1000) != 0) {
                    value1 = float_0p5_80420a1c;
                    *(f32*)((s32)mario + 0x7C) = value1 * data[0];
                    *(f32*)((s32)mario + 0x80) = value1 * data[1];
                    *(f32*)((s32)mario + 0x84) = value1 * data[2];
                    *(f32*)((s32)mario + 0x88) = value1 * data[3];
                }
            } else {
                u32 flags = *(u32*)mario;
                f32* data = marioJumpData;
                if ((flags & 0x1000) == 0) {
                    *(f32*)((s32)mario + 0x7C) = data[0];
                    *(f32*)((s32)mario + 0x80) = data[1];
                    *(f32*)((s32)mario + 0x84) = data[2];
                    *(f32*)((s32)mario + 0x88) = data[3];
                } else {
                    value1 = float_0p5_80420a1c;
                    *(f32*)((s32)mario + 0x7C) = value1 * data[0];
                    *(f32*)((s32)mario + 0x80) = value1 * data[1];
                    *(f32*)((s32)mario + 0x84) = value1 * data[2];
                    *(f32*)((s32)mario + 0x88) = value1 * data[3];
                }
                if (*(f32*)((s32)mario + 0x228) != float_1_804209b4) {
                    value1 = float_2_80420a44;
                    value2 = float_neg0p003_80420a48;
                    *(f32*)((s32)mario + 0x7C) = value1;
                    value1 = float_0p029_80420a4c;
                    *(f32*)((s32)mario + 0x80) = value2;
                    *(f32*)((s32)mario + 0x84) = value1;
                    *(f32*)((s32)mario + 0x88) = value2;
                }
            }
            break;
        case 4:
            value1 = float_2_80420a44;
            value2 = float_neg0p04_80420a50;
            *(f32*)((s32)mario + 0x7C) = value1;
            value1 = float_neg0p01_804209d4;
            *(f32*)((s32)mario + 0x80) = value2;
            value2 = float_neg0p003_80420a48;
            *(f32*)((s32)mario + 0x84) = value1;
            *(f32*)((s32)mario + 0x88) = value2;
            break;
        case 0x16:
            *(f32*)((s32)mario + 0x7C) = marioJumpData[0];
            *(f32*)((s32)mario + 0x80) = marioJumpData[1];
            *(f32*)((s32)mario + 0x84) = marioJumpData[2];
            *(f32*)((s32)mario + 0x88) = marioJumpData[3];
            if (*(f32*)((s32)mario + 0x228) != float_1_804209b4) {
                value1 = float_2p7_80420a54;
                value2 = float_neg0p002_804209d0;
                *(f32*)((s32)mario + 0x7C) = value1;
                value1 = float_0p016_80420a58;
                *(f32*)((s32)mario + 0x80) = value2;
                *(f32*)((s32)mario + 0x84) = value1;
                *(f32*)((s32)mario + 0x88) = value2;
            }
            break;
    }
    if (*(u16*)((s32)mario + 0x2E) != 8 && *(s8*)((s32)mario + 0x3C) == 2) {
        kpaSetJumpPara();
    }
}

void marioSetFallPara(void) {
    void* mario = marioGetPtr();
    f32* data = &marioJumpData[4];
    f32 x;
    f32 y;

    if (*(u16*)((s32)mario + 0x2E) == 5) {
        data = &marioJumpSwData[4];
    }
    *(f32*)((s32)mario + 0x7C) = data[0];
    *(f32*)((s32)mario + 0x80) = data[1];
    *(f32*)((s32)mario + 0x84) = data[2];
    *(f32*)((s32)mario + 0x88) = data[3];
    if (*(f32*)((s32)mario + 0x228) != float_1_804209b4) {
        x = float_0p34_804209cc;
        y = float_neg0p002_804209d0;
        *(f32*)((s32)mario + 0x7C) = x;
        x = float_neg0p01_804209d4;
        *(f32*)((s32)mario + 0x80) = y;
        y = float_0_804209a0;
        *(f32*)((s32)mario + 0x84) = x;
        *(f32*)((s32)mario + 0x88) = y;
    }
}
f32 marioGetFallSpd(void) {
    void* mario = marioGetPtr();
    f32 zero = float_0_804209a0;

    *(f32*)((s32)mario + 0x84) += *(f32*)((s32)mario + 0x88);
    if (*(f32*)((s32)mario + 0x84) > zero) {
        *(f32*)((s32)mario + 0x84) = zero;
    }
    *(f32*)((s32)mario + 0x80) += *(f32*)((s32)mario + 0x84);
    *(f32*)((s32)mario + 0x7C) += *(f32*)((s32)mario + 0x80);
    if (*(f32*)((s32)mario + 0x7C) <= marioJumpData[9]) {
        *(f32*)((s32)mario + 0x7C) = marioJumpData[9];
        zero = float_0_804209a0;
        *(f32*)((s32)mario + 0x80) = zero;
        *(f32*)((s32)mario + 0x84) = zero;
        *(f32*)((s32)mario + 0x88) = zero;
    }
    return *(f32*)((s32)mario + 0x7C);
}

double marioMakeJumpPara(void) {
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

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioJump(void) {
    extern void marioSetPeakYpos(void);
    extern const f32 float_neg0p526_80420a38;
    extern const f32 float_neg0p03_80420a3c;
    extern const f32 float_neg0p02_80420a40;
    u8* player = marioGetPtr();
    u8* current;
    f32* data;
    f32 oldSpeed;
    f32 scale;
    s16 motion;

    if (getRollEvtFlag() != 0) {
        return;
    }
    motion = *(u16*)(player + 0x2E);
    if (motion >= 4 && motion < 9) {
        return;
    }
    if (*(s32*)(player + 0x44) < 10) {
        return;
    }
    oldSpeed = *(f32*)(player + 0x7C);
    current = marioGetPtr();
    scale = (*(u32*)current & 0x00001000) != 0 ? float_1p7_80420a34 : float_1_804209b4;
    *(f32*)(current + 0x84) += *(f32*)(current + 0x88) / scale;
    *(f32*)(current + 0x80) += *(f32*)(current + 0x84) / scale;
    *(f32*)(current + 0x7C) += *(f32*)(current + 0x80) / scale;
    if (*(f32*)(player + 0x7C) > float_0_804209a0 &&
        *(s16*)(player + 0x50) >= 4 && (*(u16*)(player + 0x24A) & 0x100) == 0) {
        *(f32*)(player + 0x7C) += float_neg0p526_80420a38;
    }
    if (*(f32*)(player + 0x7C) <= float_0_804209a0) {
        if (oldSpeed > float_0_804209a0) {
            *(s16*)(player + 0x52) = *(s16*)(player + 0x50);
        }
        marioSetPeakYpos();
        current = marioGetPtr();
        data = *(u16*)(current + 0x2E) == 5 ? marioJumpSwData + 4 : marioJumpData + 4;
        *(f32*)(current + 0x7C) = data[0];
        *(f32*)(current + 0x80) = data[1];
        *(f32*)(current + 0x84) = data[2];
        *(f32*)(current + 0x88) = data[3];
        if (*(f32*)(current + 0x228) != float_1_804209b4) {
            *(f32*)(current + 0x7C) = float_0p34_804209cc;
            *(f32*)(current + 0x80) = float_neg0p002_804209d0;
            *(f32*)(current + 0x84) = float_neg0p01_804209d4;
            *(f32*)(current + 0x88) = float_0_804209a0;
        }
        current = marioGetPtr();
        scale = (*(u32*)current & 0x00001000) != 0 ? float_1p7_80420a34 : float_1_804209b4;
        *(f32*)(current + 0x84) += *(f32*)(current + 0x88) / scale;
        *(f32*)(current + 0x80) += *(f32*)(current + 0x84) / scale;
        *(f32*)(current + 0x7C) += *(f32*)(current + 0x80) / scale;
        if (marioChkSts(0x10) == 0) {
            current = marioGetPtr();
            if ((*(u32*)current & 0x10000000) == 0) {
                if (*(u16*)(current + 0x30) == 0x10 || *(u16*)(current + 0x30) == 0x11) {
                    if ((*(u32*)(current + 4) & 0x08000000) != 0) {
                        *(u32*)(current + 4) &= ~0x08000000;
                    }
                    if ((*(u32*)(current + 4) & 4) != 0) {
                        *(u32*)(current + 4) &= ~4;
                    }
                }
                *(u16*)(current + 0x30) = *(u16*)(current + 0x2E);
                *(u16*)(current + 0x2E) = 10;
                if (marioMotTbl[*(u16*)(current + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)(current + 0x30)][2]();
                }
                *(u32*)(current + 0xC) |= 1;
            }
        } else {
            *(f32*)(player + 0x7C) = float_neg0p03_80420a3c;
            *(f32*)(player + 0x80) = float_neg0p02_80420a40;
            *(f32*)(player + 0x84) = float_neg0p02_80420a40;
            *(f32*)(player + 0x88) = float_neg0p002_804209d0;
            *(u32*)player &= ~0x00010000;
            *(u32*)player |= 0x00020000;
        }
    }
    *(f32*)(player + 0x90) += *(f32*)(player + 0x7C);
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioFall(void) {
    extern s32 strcmp(char*, char*);
    extern s32 marioKeyOffChk(void);
    extern s32 marioCtrlOffChk(void);
    extern void mot_fall_roll(void);
    extern s32 marioGetRub(s32, void*, void*, void*);
    extern u32 hitGetAttr(void*);
    extern s32 kpaChkHitobjRide(void);
    extern char str_kpa_03_802c3e9c[];
    extern const f32 float_0p01_80420a2c;
    extern const f32 float_0p1_80420a28;

    void* player;
    void* current;
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
        {
        f32 zero;
        current = marioGetPtr();
        zero = float_0_804209a0;
        *(f32*)((s32)current + 0x84) += *(f32*)((s32)current + 0x88);
        if (*(f32*)((s32)current + 0x84) > zero) {
            *(f32*)((s32)current + 0x84) = zero;
        }
        *(f32*)((s32)current + 0x80) += *(f32*)((s32)current + 0x84);
        *(f32*)((s32)current + 0x7C) += *(f32*)((s32)current + 0x80);
        if (*(f32*)((s32)current + 0x7C) <= marioJumpData[9]) {
            *(f32*)((s32)current + 0x7C) = marioJumpData[9];
            zero = float_0_804209a0;
            *(f32*)((s32)current + 0x80) = zero;
            *(f32*)((s32)current + 0x84) = zero;
            *(f32*)((s32)current + 0x88) = zero;
        }
        speed = *(f32*)((s32)current + 0x7C);
        hit = marioChkLandon(speed, &landY);
        if (hit != 0) {
            *(void**)((s32)player + 0x1E8) = hit;
            *(s32*)((s32)player + 0x1EC) = 0;
            *(f32*)((s32)player + 0x90) = landY;
        } else {
            *(f32*)((s32)player + 0x90) += speed;
        }
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

    {
    f32 zero;
    current = marioGetPtr();
    zero = float_0_804209a0;
    *(f32*)((s32)current + 0x84) += *(f32*)((s32)current + 0x88);
    if (*(f32*)((s32)current + 0x84) > zero) {
        *(f32*)((s32)current + 0x84) = zero;
    }
    *(f32*)((s32)current + 0x80) += *(f32*)((s32)current + 0x84);
    *(f32*)((s32)current + 0x7C) += *(f32*)((s32)current + 0x80);
    if (*(f32*)((s32)current + 0x7C) <= marioJumpData[9]) {
        *(f32*)((s32)current + 0x7C) = marioJumpData[9];
        zero = float_0_804209a0;
        *(f32*)((s32)current + 0x80) = zero;
        *(f32*)((s32)current + 0x84) = zero;
        *(f32*)((s32)current + 0x88) = zero;
    }

    speed = *(f32*)((s32)current + 0x7C);
    hit = marioChkLandon(speed, &landY);
    if (hit != 0) {
        *(void**)((s32)player + 0x1E8) = hit;
        *(s32*)((s32)player + 0x1EC) = 0;
        attr = hitGetAttr(hit);
        if ((attr & 0x200) != 0) {
            *(f32*)((s32)player + 0x90) += speed;
        } else {
            *(f32*)((s32)player + 0x90) = landY;
            if ((*(u32*)((s32)player + 0xC) & 0x200000) == 0 &&
                (*(u32*)((s32)player + 0xC) & 0x100000) != 0 &&
                marioPriCheckJabara() != 0 &&
                rubDir != -1 && rub >= 0) {
                camFollowYOn();
                player = marioGetPtr();
                if ((*(u32*)player & 0x10000000) != 0) {
                    return;
                }
                if (*(u16*)((s32)player + 0x30) == 0x10 || *(u16*)((s32)player + 0x30) == 0x11) {
                    if ((*(u32*)((s32)player + 4) & 0x08000000) != 0) {
                        *(u32*)((s32)player + 4) &= ~0x08000000;
                    }
                    if ((*(u32*)((s32)player + 4) & 4) != 0) {
                        *(u32*)((s32)player + 4) &= ~4;
                    }
                }
                *(u16*)((s32)player + 0x30) = *(u16*)((s32)player + 0x2E);
                *(u16*)((s32)player + 0x2E) = 0x14;
                if (marioMotTbl[*(u16*)((s32)player + 0x30)][2] != 0) {
                    marioMotTbl[*(u16*)((s32)player + 0x30)][2]();
                }
                *(u32*)((s32)player + 0xC) |= 1;
                return;
            }
        }
    } else {
        *(f32*)((s32)player + 0x90) += speed;
    }
    }

    if (*(s8*)((s32)player + 0x3C) != 2) {
        if (*(void**)((s32)player + 0x1E8) != 0) {
            marioLandOn();
        }
    } else {
        if (kpaChkHitobjRide() != 0) {
            marioLandOn();
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioLandOn(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern u32 hitGetAttr(void* hit);
    extern void kpaCamFollow(void);
    extern u32 psndSFXOn_3D(s32 id, void* position);
    extern f32 distABf(f32 x1, f32 z1, f32 x2, f32 z2);
    extern f32 __fabsf(f32 value);
    extern s32 kpaFireAttackCheck(void);
    extern void marioChgMot(s32 motion);
    extern void set_damage_root_ypos(f32 y);
    extern void N_marioSlitSetAfterLand(void);
    extern s32 marioChkSlitEnd(void);
    extern void motSlitCancel(void);
    extern void unk_JP_US_EU_06_800a6ac8(void);
    extern void* mobjHitObjPtrToPtr(void* hitObj);
    extern s32 strcmp(const char* lhs, const char* rhs);
    extern void effKemuriEntry(double x, double y, double z, double unk, s32 type);
    extern void yoshi_jumpStand(void);
    extern const f32 float_100_804209b8;
    extern char str_MOBJ_RedSwitch_802c3e78[];
    extern char str_MOBJ_BlueJumpStand_802c3e88[];

    void* player = marioGetPtr();
    void* current;
    void* hit;
    Vec pos;
    void* mobj;
    u32 attr;
    s32 sound;
    s32 motion;

#define CHANGE_MOTION(next) do { \
        player = marioGetPtr(); \
        if ((*(u32*)player & 0x10000000) == 0) { \
            if (*(u16*)((s32)player + 0x30) == 0x10 || \
                *(u16*)((s32)player + 0x30) == 0x11) { \
                if ((*(u32*)((s32)player + 4) & 0x08000000) != 0) { \
                    *(u32*)((s32)player + 4) &= ~0x08000000; \
                } \
                if ((*(u32*)((s32)player + 4) & 4) != 0) { \
                    *(u32*)((s32)player + 4) &= ~4; \
                } \
            } \
            *(u16*)((s32)player + 0x30) = *(u16*)((s32)player + 0x2E); \
            *(u16*)((s32)player + 0x2E) = (next); \
            if (marioMotTbl[*(u16*)((s32)player + 0x30)][2] != 0) { \
                marioMotTbl[*(u16*)((s32)player + 0x30)][2](); \
            } \
            *(u32*)((s32)player + 0xC) |= 1; \
        } \
    } while (0)

    if ((hitGetAttr(*(void**)((s32)player + 0x1E8)) & 0x200) == 0) {
        f32 zero;
        current = marioGetPtr();
        zero = float_0_804209a0;
        *(f32*)((s32)current + 0x7C) = zero;
        *(f32*)((s32)current + 0x80) = zero;
        *(f32*)((s32)current + 0x84) = zero;
        *(f32*)((s32)current + 0x88) = zero;
    }
    *(volatile u32*)player &= ~0x80;
    *(volatile u32*)player &= ~0x40000000;
    *(s16*)((s32)player + 0x50) = 0;
    *(s16*)((s32)player + 0x52) = 0;

    if (*(s8*)((s32)player + 0x3C) != 2) {
        if (marioBgmodeChk() == 0) {
            camFollowYOn();
        }
    } else {
        kpaCamFollow();
    }

    hit = *(void**)((s32)player + 0x1E8);
    pos = *(Vec*)((s32)player + 0x8C);
    if (hit != 0) {
        attr = hitGetAttr(hit);
        if ((attr & 0x100) != 0) {
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
        *(void**)((s32)player + 0x1EC) = 0;
        return;
    }

    *(f32*)((s32)player + 0x2C8) = __fabsf(distABf(
        *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
        *(f32*)((s32)player + 0x11C), *(f32*)((s32)player + 0x124)));

    attr = hitGetAttr(*(void**)((s32)player + 0x1E8));
    if (*(u16*)((s32)player + 0x2E) != 0x1F &&
        (attr & 0x200) != 0) {
        set_damage_root_ypos(*(f32*)((s32)player + 0x90));
        CHANGE_MOTION(0x1F);
        return;
    }
    if (*(u16*)((s32)player + 0x2E) != 0x20 &&
        (attr & 0x800) != 0) {
        set_damage_root_ypos(*(f32*)((s32)player + 0x90));
        CHANGE_MOTION(0x20);
        return;
    }

    if ((hitGetAttr(*(void**)((s32)player + 0x1E8)) & 0x80000000) != 0) {
        mobj = mobjHitObjPtrToPtr(*(void**)((s32)player + 0x1E8));
        if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_RedSwitch_802c3e78) == 0) {
            CHANGE_MOTION(5);
        } else if (strcmp((char*)((s32)mobj + 0x15), str_MOBJ_BlueJumpStand_802c3e88) == 0) {
            player = marioGetPtr();
            camFollowYOn();
            if (marioChkSts(0x10) == 0) {
                if (*(u16*)((s32)player + 0x2E) == 0x1A) {
                    *(f32*)((s32)player + 0x2D0) = float_100_804209b8;
                    yoshi_jumpStand();
                } else {
                    *(f32*)((s32)player + 0x2D0) = float_100_804209b8;
                    CHANGE_MOTION(6);
                }
            } else {
                *(f32*)((s32)player + 0x2D0) = float_100_804209b8;
                effKemuriEntry(*(f32*)((s32)player + 0x8C),
                               *(f32*)((s32)player + 0x90),
                               *(f32*)((s32)player + 0x94),
                               float_0_804209a0, 0);
                CHANGE_MOTION(6);
            }
        } else if (*(s8*)((s32)player + 0x3C) == 2) {
            CHANGE_MOTION(0);
            quake_kpaLandOn();
        } else if (*(u16*)((s32)player + 0x2E) == 0x15) {
            if ((*(u32*)player & 0x00100000) == 0) {
                if ((*(u32*)((s32)player + 4) & 0x00100000) != 0 &&
                    marioChkSlitEnd() == 0) {
                    motSlitCancel();
                }
            } else {
                unk_JP_US_EU_06_800a6ac8();
            }
        } else {
            CHANGE_MOTION(0);
        }
    } else {
        if (*(f32*)((s32)player + 0x180) > float_0_804209a0 &&
            (*(u32*)((s32)player + 0xC) & 0x100) == 0 &&
            (*(u32*)player & 0x2000000) == 0) {
            if (*(s8*)((s32)player + 0x3C) == 2) {
                if (kpaFireAttackCheck() == 0) {
                    CHANGE_MOTION(1);
                } else {
                    CHANGE_MOTION(0);
                }
                quake_kpaLandOn();
                return;
            }
            if ((*(u32*)((s32)player + 4) & 0x00100000) != 0) {
                N_marioSlitSetAfterLand();
                return;
            }
            if (marioPriCheckJabara() == 0) {
                CHANGE_MOTION(1);
                return;
            }
            if ((*(u32*)((s32)player + 0x14) & 0x100) != 0) {
                CHANGE_MOTION(0);
                return;
            }
            CHANGE_MOTION(1);
            return;
        }

        if ((*(u32*)player & 0x01000000) == 0) {
            if (*(s8*)((s32)player + 0x3C) == 2) {
                CHANGE_MOTION(0);
                quake_kpaLandOn();
            } else if (*(u16*)((s32)player + 0x2E) == 0x15) {
                if ((*(u32*)player & 0x00100000) == 0) {
                    if ((*(u32*)((s32)player + 4) & 0x00100000) != 0 &&
                        marioChkSlitEnd() == 0) {
                        motSlitCancel();
                    }
                } else {
                    unk_JP_US_EU_06_800a6ac8();
                }
            } else if ((*(u32*)player & 0x02000000) == 0) {
                CHANGE_MOTION(0);
            } else {
                motion = *(f32*)((s32)player + 0x180) <= *(f32*)((s32)player + 0x184) ? 1 : 2;
                CHANGE_MOTION(motion);
            }
        } else {
            *(s32*)((s32)player + 0x44) = 0x32;
            *(u16*)((s32)player + 0x2E) = 0x0B;
        }
    }

#undef CHANGE_MOTION
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_talk(void) {
    extern void peach_talk(void);
    extern void* marioGetTouchNpcPtr(void);
    extern void marioAdjustMoveDir(void);
    extern void* camGetPtr(s32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern f32 revise360(f32);
    extern f32 toMovedir(f32);
    extern void kpaChgPose(char*, char*);
    extern void* fbatGetPointer(void);
    extern void marioChgPose(char*);
    extern s32 strcmp(const char*, const char*);
    extern char str_KPA_S_1_802c3e5c[];
    extern char str_KPA2_S_1_802c3e64[];
    extern char str_M_I_2_80420a20[];
    u8* player = marioGetPtr();
    u8* npc;
    u8* fbat;
    f32 cameraAngle;
    f32 direction;

    if (*(s8*)(player + 0x3C) == 1) {
        peach_talk();
        return;
    }
    if ((*(u32*)(player + 0xC) & 1) != 0) {
        f32 zero;
        *(u32*)(player + 0xC) &= ~1;
        zero = float_0_804209a0;
        *(s32*)(player + 0x48) = 0;
        *(s16*)(player + 0x50) = 0;
        *(s32*)(player + 0x44) = 0;
        *(f32*)(player + 0x180) = zero;
        npc = marioGetTouchNpcPtr();
        if (npc == 0) {
            marioAdjustMoveDir();
        } else {
            cameraAngle = *(f32*)((u8*)camGetPtr(4) + 0x114);
            direction = angleABf(*(f32*)(player + 0x8C), *(f32*)(player + 0x94),
                                 *(f32*)(npc + 0x8C), *(f32*)(npc + 0x94));
            direction = float_270_80420a0c -
                        (f32)(s32)(float_10_804209c4 * (direction - cameraAngle) +
                                  float_0p5_80420a1c) / float_10_804209c4;
            direction = revise360(direction);
            if (direction >= float_90_804209f4 && direction <= float_270_80420a0c) {
                *(f32*)(player + 0x1A0) = toMovedir(float_90_804209f4);
                *(f32*)(player + 0x1B0) = float_180_80420a08;
                *(f32*)(player + 0x1A4) = *(f32*)(player + 0x1A0);
            } else {
                *(f32*)(player + 0x1A0) = toMovedir(float_90_804209f4);
                *(f32*)(player + 0x1B0) = float_0_804209a0;
                *(f32*)(player + 0x1A4) = *(f32*)(player + 0x1A0);
            }
        }
        if (*(s8*)(player + 0x3C) == 2) {
            kpaChgPose(str_KPA_S_1_802c3e5c, str_KPA2_S_1_802c3e64);
            return;
        }
        fbat = fbatGetPointer();
        npc = *(u8**)(fbat + 4);
        if (npc != 0) {
            marioChgPose(*(char**)(npc + 0x10C));
        }
        if (strcmp(*(char**)(player + 0x18), str_M_I_2_80420a20) == 0) {
            *(s32*)(player + 0x48) = 60;
            *(s32*)(player + 0x44) = 10;
        }
    }
    switch (*(s32*)(player + 0x44)) {
        case 0:
            break;
        case 10:
            if (--*(s32*)(player + 0x48) <= 0) {
                marioChgPose(str_M_S_1_804209ec);
                *(s32*)(player + 0x44) = 11;
            }
            break;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

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


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioChkTransform(void) {
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


s32 L_marioChkRub(void) {
    return 0;
}
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 marioRollChgChk(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern s32 marioGetRub(s32, void*, s32*, void*);
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
void mot_kaze(void) {
    extern const f32 float_5_80420a00;
    extern const f32 float_60_80420a04;
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

    switch (*(s32*)((s32)mario + 0x44)) {
    case 0:
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
        break;

    case 100:
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
        break;

    }
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


void mot_shadow(void) {
    extern void marioChgPose(char*);
    u8* player = marioGetPtr();
    f32 angle;
    f32 zero;

    if ((*(u32*)(player + 0xC) & 1) != 0) {
        *(u32*)(player + 0xC) &= ~1;
        *(u32*)player &= ~0x0000F000;
        *(u32*)(player + 4) &= ~0x01000000;
        *(s32*)(player + 0x48) = 0;
        *(s32*)(player + 0x44) = 0;
        marioChgPose(str_M_S_1_804209ec);
        angle = float_90_804209f4;
        zero = float_0_804209a0;
        *(f32*)(player + 0xBC) = angle;
        *(f32*)(player + 0xC0) = zero;
        *(f32*)(player + 0xC4) = zero;
    }
    if (*(s32*)(player + 0x44) == 0 && (*(u16*)(player + 0x24C) & 0x200) != 0) {
        *(f32*)(player + 0xBC) = float_0_804209a0;
        *(f32*)(player + 0xC4) = float_0_804209a0;
        *(s32*)(player + 0x48) = 0;
        player = marioGetPtr();
        if ((*(u32*)player & 0x10000000) == 0) {
            if (*(u16*)(player + 0x30) == 0x10 || *(u16*)(player + 0x30) == 0x11) {
                if ((*(u32*)(player + 4) & 0x08000000) != 0) {
                    *(u32*)(player + 4) &= ~0x08000000;
                }
                if ((*(u32*)(player + 4) & 4) != 0) {
                    *(u32*)(player + 4) &= ~4;
                }
            }
            *(u16*)(player + 0x30) = *(u16*)(player + 0x2E);
            *(u16*)(player + 0x2E) = 0;
            if (marioMotTbl[*(u16*)(player + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)(player + 0x30)][2]();
            }
            *(u32*)(player + 0xC) |= 1;
        }
    }
}

void mot_partyUse(void) {
    marioGetPtr();
}

void mot_dummy(void) {
    marioGetPtr();
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma opt_common_subs off
void mot_bottomless(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;

    extern s32 strcmp(char*, char*);
    extern void marioGetScreenPos(Vec, f32*, f32*, f32*);
    extern f32 distABf(f32, f32, f32, f32);
    extern f32 angleABf(f32, f32, f32, f32);
    extern void movePos(f32*, f32*, f32, f32);
    extern void psndSFXOn_3D(s32, void*);
    extern void marioChgPose(const void*);
    extern void marioDmgRetQuakeOn(void);
    extern s32 pouchAddHP(s16);
    extern s32 pouchGetHP(void);
    extern void marioUpdateCamPos(void);
    extern const Vec vec3_802c3e38;
    extern char str_M_D_2_804209dc[];
    extern char str_M_U_3_804209e4[];
    extern const f32 float_8_804209c8;
    extern const f32 float_13_804209ac;
    extern const f32 float_100_804209b8;
    extern const f32 float_120_804209bc;
    extern const f32 float_220_804209b0;
    extern const f32 float_250_804209a8;
    extern const f32 float_300_804209c0;
    extern const f32 float_neg2000_804209a4;
    extern const f32 float_neg10_804209d8;

    void* player;
    void* current;
    Vec screenPos;
    Vec targetPos;
    f32 sx;
    f32 sy;
    f32 sz;
    f32 y;
    f32 speed;
    f32 zero;
    void* hit;
    s32 timer;
    const u8* catalog;
    f32* data;
    f32 targetX;
    f32 targetZ;

    catalog = (const u8*)&vec3_802c3e38;
    player = marioGetPtr();
    if ((*(u32*)((s32)player + 0xC) & 1) != 0) {
        *(u32*)((s32)player + 0xC) &= ~1;
        *(f32*)((s32)player + 0x180) = float_0_804209a0;
        *(s16*)((s32)player + 0x50) = 0;
        *(s32*)((s32)player + 0x44) = 0;

        y = *(f32*)((s32)player + 0xE4);
        if (y < float_neg2000_804209a4) {
            if (strcmp((char*)((s32)gp + 0x12C), (char*)catalog + 0xC) == 0) {
                targetX = float_250_804209a8;
                y = float_13_804209ac;
                targetZ = *(f32*)((s32)player + 0x94);
            } else if (strcmp((char*)((s32)gp + 0x12C), (char*)catalog + 0x14) == 0) {
                targetX = float_220_804209b0;
                y = float_1_804209b4;
                targetZ = *(f32*)((s32)player + 0x94);
            } else if (strcmp((char*)((s32)gp + 0x12C), (char*)catalog + 0x1C) == 0) {
                targetX = float_100_804209b8;
                y = float_120_804209bc;
                targetZ = *(f32*)((s32)player + 0x94);
            }
        } else {
            targetX = *(f32*)((s32)player + 0xE0);
            targetZ = *(f32*)((s32)player + 0xE8);
        }

        targetPos = *(const Vec*)catalog;
        targetPos.x = targetX;
        targetPos.y = y + float_300_804209c0;
        targetPos.z = targetZ;
        screenPos = targetPos;
        speed = float_10_804209c4;
        do {
            marioGetScreenPos(screenPos, &sx, &sy, &sz);
            if (marioChkInScreen((s32)sx, (s32)sy) == 0) {
                break;
            }
            screenPos.y += speed;
        } while (1);

        *(f32*)((s32)player + 0x90) = screenPos.y;
        *(f32*)((s32)player + 0x2C4) = float_8_804209c8;
        *(s32*)((s32)player + 0x48) = (s16)((s32)(distABf(targetX, targetZ,
            *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94)) /
            *(f32*)((s32)player + 0x2C4)));
        *(f32*)((s32)player + 0x2C8) = angleABf(*(f32*)((s32)player + 0x8C),
            *(f32*)((s32)player + 0x94), targetX, targetZ);
    }

    switch (*(s32*)((s32)player + 0x44)) {
        case 0:
            movePos((f32*)((s32)player + 0x8C), (f32*)((s32)player + 0x94),
                    *(f32*)((s32)player + 0x2C4), *(f32*)((s32)player + 0x2C8));
            *(f32*)((s32)player + 0xEC) = *(f32*)((s32)player + 0x8C);
            *(f32*)((s32)player + 0xF4) = *(f32*)((s32)player + 0x94);
            if (--*(s32*)((s32)player + 0x48) < 1) {
                *(s32*)((s32)player + 0x44) = 2;
                *(u32*)player |= 0x80;
                current = marioGetPtr();
                data = &marioJumpData[4];
                if (*(u16*)((s32)current + 0x2E) == 5) {
                    data = &marioJumpSwData[4];
                }
                *(f32*)((s32)current + 0x7C) = data[0];
                *(f32*)((s32)current + 0x80) = data[1];
                *(f32*)((s32)current + 0x84) = data[2];
                *(f32*)((s32)current + 0x88) = data[3];
                if (*(f32*)((s32)current + 0x228) != float_1_804209b4) {
                    *(f32*)((s32)current + 0x7C) = float_0p34_804209cc;
                    *(f32*)((s32)current + 0x80) = float_neg0p002_804209d0;
                    *(f32*)((s32)current + 0x84) = float_neg0p01_804209d4;
                    *(f32*)((s32)current + 0x88) = float_0_804209a0;
                }
            }
            break;

        case 2:
            *(s32*)((s32)player + 0x1E8) = 0;
            current = marioGetPtr();
            zero = float_0_804209a0;
            *(f32*)((s32)current + 0x84) += *(f32*)((s32)current + 0x88);
            if (*(f32*)((s32)current + 0x84) > zero) {
                *(f32*)((s32)current + 0x84) = zero;
            }
            data = marioJumpData;
            *(f32*)((s32)current + 0x80) += *(f32*)((s32)current + 0x84);
            *(f32*)((s32)current + 0x7C) += *(f32*)((s32)current + 0x80);
            if (*(f32*)((s32)current + 0x7C) <= data[9]) {
                *(f32*)((s32)current + 0x7C) = data[9];
                *(f32*)((s32)current + 0x80) = zero;
                *(f32*)((s32)current + 0x84) = zero;
                *(f32*)((s32)current + 0x88) = zero;
            }
            speed = *(f32*)((s32)current + 0x7C);
            if (speed < float_neg10_804209d8) {
                speed = float_neg10_804209d8;
            }
            hit = marioChkLandon(speed, &y);
            if (hit == 0) {
                *(f32*)((s32)player + 0x90) += speed;
                return;
            }
            psndSFXOn_3D(0xB9, (void*)((s32)player + 0x8C));
            marioChgPose(str_M_D_2_804209dc);
            psndSFXOn_3D(0x197, (void*)((s32)player + 0x8C));
            marioDmgRetQuakeOn();
            *(void**)((s32)player + 0x1E8) = hit;
            *(s32*)((s32)player + 0x1EC) = 0;
            *(f32*)((s32)player + 0x90) = y;
            /* fall through */

        case 10:
            *(s32*)((s32)player + 0x48) = 0x32;
            *(s32*)((s32)player + 0x44) = 0xB;
            /* fall through */

        case 0xB:
            if (--*(s32*)((s32)player + 0x48) < 1) {
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
#pragma opt_common_subs reset
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void mot_forceReset(void) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern void marioSlitForceCancel(void);
    extern void marioReset(void);
    extern s32 marioGetExtraPartyId(void);
    extern void* partyGetPtr(s32);
    u8* player = marioGetPtr();
    u8* current;
    u8* party;

    if ((*(u32*)(player + 0xC) & 1) != 0) {
        *(u32*)(player + 0xC) &= ~1;
        *(s32*)(player + 0x44) = 0;
    }
    switch (*(s32*)(player + 0x44)) {
    case 0:
        marioSlitForceCancel();
        current = marioGetPtr();
        if ((*(u32*)current & 0x10000000) == 0) {
            if (*(u16*)(current + 0x30) == 0x10 || *(u16*)(current + 0x30) == 0x11) {
                if ((*(u32*)(current + 4) & 0x08000000) != 0) {
                    *(u32*)(current + 4) &= ~0x08000000;
                }
                if ((*(u32*)(current + 4) & 4) != 0) {
                    *(u32*)(current + 4) &= ~4;
                }
            }
            *(u16*)(current + 0x30) = *(u16*)(current + 0x2E);
            *(u16*)(current + 0x2E) = 0;
            if (marioMotTbl[*(u16*)(current + 0x30)][2] != 0) {
                marioMotTbl[*(u16*)(current + 0x30)][2]();
            }
            *(u32*)(current + 0xC) |= 1;
        }
        *(s32*)(player + 0x44) = 1;
        break;
    case 1:
        marioReset();
        party = partyGetPtr(marioGetPartyId());
        if (party != 0) {
            *(Vec*)(party + 0x58) = *(Vec*)(player + 0x8C);
        }
        party = partyGetPtr(marioGetExtraPartyId());
        if (party != 0) {
            *(Vec*)(party + 0x58) = *(Vec*)(player + 0x8C);
        }
        break;
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

typedef struct VecData {
    f32 x;
    f32 y;
    f32 z;
} VecData;

const VecData vec3_802c3e38 = { 0.0f, 0.0f, 0.0f };
const char str_win_03_802c3e44[] = "win_03";
const char str_win_01_802c3e4c[] = "win_01";
const char str_tik_01_802c3e54[] = "tik_01";
const char str_KPA_S_1_802c3e5c[] = "KPA_S_1";
const char str_KPA2_S_1_802c3e64[] = "KPA2_S_1";
const char str_MOBJ_RedSwitch_802c3e78[] = "MOBJ_RedSwitch";
const char str_MOBJ_BlueJumpStand_802c3e88[] = "MOBJ_BlueJumpStand";
const char str_kpa_03_802c3e9c[] = "kpa_03";

/* Exact US mario_motion small-data catalog.
 * Late definitions preserve declaration-only codegen for executable consumers. */
__declspec(section ".sdata2") const f32 float_0_804209a0 = 0.0f;
__declspec(section ".sdata2") const f32 float_neg2000_804209a4 = -2000.0f;
__declspec(section ".sdata2") const f32 float_250_804209a8 = 250.0f;
__declspec(section ".sdata2") const f32 float_13_804209ac = 13.0f;
__declspec(section ".sdata2") const f32 float_220_804209b0 = 220.0f;
__declspec(section ".sdata2") const f32 float_1_804209b4 = 1.0f;
__declspec(section ".sdata2") const f32 float_100_804209b8 = 100.0f;
__declspec(section ".sdata2") const f32 float_120_804209bc = 120.0f;
__declspec(section ".sdata2") const f32 float_300_804209c0 = 300.0f;
__declspec(section ".sdata2") const f32 float_10_804209c4 = 10.0f;
__declspec(section ".sdata2") const f32 float_8_804209c8 = 8.0f;
__declspec(section ".sdata2") const f32 float_0p34_804209cc = 0.34f;
__declspec(section ".sdata2") const f32 float_neg0p002_804209d0 = -0.002f;
__declspec(section ".sdata2") const f32 float_neg0p01_804209d4 = -0.01f;
__declspec(section ".sdata2") const f32 float_neg10_804209d8 = -10.0f;
__declspec(section ".sdata2") char str_M_D_2_804209dc[] = "M_D_2";
__declspec(section ".sdata2") char str_M_U_3_804209e4[] = "M_U_3";
__declspec(section ".sdata2") char str_M_S_1_804209ec[] = "M_S_1";
__declspec(section ".sdata2") const f32 float_90_804209f4 = 90.0f;
__declspec(section ".sdata2") char str_M_P_2_804209f8[] = "M_P_2";
__declspec(section ".sdata2") const f32 float_5_80420a00 = 5.0f;
__declspec(section ".sdata2") const f32 float_60_80420a04 = 60.0f;
__declspec(section ".sdata2") const f32 float_180_80420a08 = 180.0f;
__declspec(section ".sdata2") const f32 float_270_80420a0c = 270.0f;
__declspec(section ".sdata2") const f32 float_6_80420a10 = 6.0f;
__declspec(section ".sdata2") char str_M_I_U_80420a14[] = "M_I_U";
__declspec(section ".sdata2") const f32 float_0p5_80420a1c = 0.5f;
__declspec(section ".sdata2") char str_M_I_2_80420a20[] = "M_I_2";
__declspec(section ".sdata2") const f32 float_0p1_80420a28 = 0.1f;
__declspec(section ".sdata2") const f32 float_0p01_80420a2c = 0.01f;
__declspec(section ".sdata2") const f32 float_neg0p5_80420a30 = -0.5f;
__declspec(section ".sdata2") const f32 float_1p7_80420a34 = 1.7f;
__declspec(section ".sdata2") const f32 float_neg0p526_80420a38 = -0.526f;
__declspec(section ".sdata2") const f32 float_neg0p03_80420a3c = -0.03f;
__declspec(section ".sdata2") const f32 float_neg0p02_80420a40 = -0.02f;
__declspec(section ".sdata2") const f32 float_2_80420a44 = 2.0f;
__declspec(section ".sdata2") const f32 float_neg0p003_80420a48 = -0.003f;
__declspec(section ".sdata2") const f32 float_0p029_80420a4c = 0.029f;
__declspec(section ".sdata2") const f32 float_neg0p04_80420a50 = -0.04f;
__declspec(section ".sdata2") const f32 float_2p7_80420a54 = 2.7f;
__declspec(section ".sdata2") const f32 float_0p016_80420a58 = 0.016f;
