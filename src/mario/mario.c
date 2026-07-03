#include "mario/mario.h"

void* marioGetPtr(void) {
    extern void* mp;
    return mp;
}

s32 marioCtrlOffChk(void) {
    extern void* mp;
    return *(s8*)((s32)mp + 0x38);
}

s32 marioGetColor(void) {
    extern void* mp;
    return *(s8*)((s32)mp + 0x3D);
}

s32 marioKeyOffChk(void) {
    extern void* mp;
    return *(s8*)((s32)mp + 0x39);
}

s32 marioItemGetOk(void) {
    extern void* mp;
    return ((*(u32*)mp >> 14) & 1) ^ 1;
}

s32 marioBgmodeChk(void) {
    extern void* mp;
    return (*(u32*)mp >> 25) & 1;
}

s32 marioChkCtrl(void) {
    extern void* mp;
    return *(s8*)((s32)mp + 0x38) == 0;
}

void marioItemGetDisable(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario |= 0x4000;
    *(s16*)((s32)mario + 0x7A) = 20;
}

f32 marioGetScale(void) {
    extern void* mp;
    extern f32 float_1p2_80420000;
    extern f32 float_2_80420004;

    if ((*(u32*)mp & 0x02000000) != 0) {
        return float_1p2_80420000;
    }
    return float_2_80420004;
}

void marioSoundInit(void) {
    extern void* mp;
    void* mario = mp;

    *(s32*)((s32)mario + 0x27C) = -1;
    *(s32*)((s32)mario + 0x280) = -1;
    *(s32*)((s32)mario + 0x284) = -1;
    *(s32*)((s32)mario + 0x288) = -1;
    *(s32*)((s32)mario + 0x28C) = -1;
}

s32 marioCtrlOff(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario |= 2;
    (*(u8*)((s32)mario + 0x38))++;
    return *(s8*)((s32)mario + 0x38);
}

s32 marioKeyOff(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario |= 8;
    (*(u8*)((s32)mario + 0x39))++;
    return *(s8*)((s32)mario + 0x39);
}

void marioPaperLightOff(void) {
    extern void* mp;
    extern void animPoseSetMaterialLightFlagOff(s32 poseId, u32 flag);

    animPoseSetMaterialLightFlagOff(*(s32*)((s32)mp + 0x22C), 1);
}

s32 marioCtrlOff2(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario |= 4;
    *(u32*)mario |= 2;
    (*(u8*)((s32)mario + 0x38))++;
    return *(s8*)((s32)mario + 0x38);
}

s32 marioCtrlOn(void) {
    extern void* mp;
    void* mario = mp;

    (*(u8*)((s32)mario + 0x38))--;
    if (*(s8*)((s32)mario + 0x38) == 0) {
        *(u32*)mario &= ~2;
    }
    return *(s8*)((s32)mario + 0x38);
}

s32 marioKeyOn(void) {
    extern void* mp;
    void* mario = mp;

    (*(u8*)((s32)mario + 0x39))--;
    if (*(s8*)((s32)mario + 0x39) == 0) {
        *(u32*)mario &= ~8;
    }
    return *(s8*)((s32)mario + 0x39);
}

s32 unk_8005ca2c(void) {
    extern void* mp;
    void* mario = mp;

    if (*(u16*)((s32)mario + 0x2E) == 0x16 || (*(u32*)mario & 0x00100000) != 0) {
        return 0;
    }
    return 1;
}

s32 marioChkInScreen(s32 x, s32 y) {
    s32 ret = 0;

    if (x >= 0 && x <= 600 && y >= 0 && y <= 535) {
        ret = 1;
    }
    return ((u32)-ret | (u32)ret) >> 31;
}

s32 marioCaseEventValidChk(void) {
    extern void* mp;
    extern s32 vivianGetStatus(void);

    if (*(u16*)((s32)mp + 0x2E) == 0x18) {
        return 0;
    }
    return vivianGetStatus() == 0;
}

s32 marioSetMutekiTime(s32 msec) {
    extern void* mp;
    extern s32 sysMsec2Frame(s32 msec);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    void* mario = mp;
    void* party;

    *(s16*)((s32)mario + 0x4C) = sysMsec2Frame(msec);
    party = partyGetPtr(marioGetPartyId());
    if (party != NULL) {
        *(s16*)((s32)party + 0x2A) = *(s16*)((s32)mario + 0x4C);
    }
    return *(s16*)((s32)mario + 0x4C);
}

void marioItemGetChk(void) {
    extern void* mp;
    void* mario = mp;
    s32 value = *(s16*)((s32)mario + 0x7A);

    if (value > 0) {
        value--;
        *(s16*)((s32)mario + 0x7A) = value;
        if ((s16)value <= 0) {
            *(s16*)((s32)mario + 0x7A) = 0;
            *(u32*)mp &= ~0x4000;
        }
    }
}

s32 marioCtrlOn2(void) {
    extern void* mp;
    void* mario = mp;

    *(u32*)mario &= ~4;
    (*(u8*)((s32)mario + 0x38))--;
    if (*(s8*)((s32)mario + 0x38) == 0) {
        *(u32*)mario &= ~2;
    }
    return *(s8*)((s32)mario + 0x38);
}

s32 N_marioChkUseParty(void) {
    extern void* gp;
    extern void* mp;

    if (((u16)*(u32*)((s32)gp + 0x1338) & 0x100F) != 0) {
        return 1;
    }
    return *(s8*)((s32)mp + 0x38);
}

s32 marioAnimeId(void) {
    extern void* mp;
    u32 flags = *(u32*)((s32)mp + 4);
    s32 ret;

    if ((flags & 0x10000000) != 0) {
        ret = 2;
    } else if ((flags & 0x80000000) != 0) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

void marioPaperOff(void) {
    extern void* mp;
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 a, s32 b);
    extern void animPaperPoseRelease(s32 poseId);
    void* mario = mp;

    if ((*(u32*)((s32)mario + 4) & 0x40000000) != 0 && *(void**)((s32)mario + 0x1C) != NULL) {
        animPoseSetPaperAnimGroup(*(s32*)((s32)mario + 0x22C), 0, 0);
        *(s32*)((s32)mario + 0x1C) = 0;
        *(u32*)((s32)mario + 4) &= ~0x40000000;
        mario = mp;
        if (*(s32*)((s32)mario + 0x240) >= 0) {
            animPaperPoseRelease(*(s32*)((s32)mario + 0x240));
            *(s32*)((s32)mario + 0x240) = -1;
        }
    }
}

void marioOfsRotReset(void) {
    typedef struct WordVec {
        u32 x;
        u32 y;
        u32 z;
    } WordVec;
    extern void* mp;
    extern u8 str_a_mario_802c18a0[];
    void* base = str_a_mario_802c18a0;
    void* mario = mp;

    *(WordVec*)((s32)mario + 0xBC) = *(WordVec*)((s32)base + 0x88);
    *(WordVec*)((s32)mario + 0xB0) = *(WordVec*)((s32)base + 0x94);
    *(WordVec*)((s32)mario + 0x98) = *(WordVec*)((s32)base + 0xA0);
    *(WordVec*)((s32)mario + 0xA4) = *(WordVec*)((s32)base + 0xAC);
}


u8 marioMove(void) {
    return 0;
}


u8 marioMain(void) {
    return 0;
}


void marioSetCharMode(s32 mode) {
    ;
}


u8 marioSetFamicomMode(int param_1) {
    return 0;
}


u8 marioRearAnime(void) {
    return 0;
}


u8 marioDispBlurSub(s32 param_1, int param_2) {
    return 0;
}


u8 marioMoveMain(void) {
    return 0;
}


u8 marioMakeDispDir(void) {
    return 0;
}


u8 marioDisp(void) {
    return 0;
}


char* toFrontPose(char* param_1) {
    return 0;
}


char* toRearPose(char* param_1) {
    return 0;
}


u8 marioInit(void) {
    return 0;
}


u8 marioPreDisp(void) {
    return 0;
}


s32 marioCheckMenuDisable(void) {
    return 0;
}


void marioReInit(void) {
    ;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioEntry(void) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern void* dotMarioPose[][6];
    extern void marioSlitForceCancel(void);
    extern void marioChgMot(s32 motion);
    extern void marioReInit_ship(void);
    extern void marioReInit_roll(void);
    extern void marioReInit_slit(void);
    extern void marioRideYoshi(void);
    void* player;
    void* m;
    char* dotPose;
    u16 motion;

    player = mp;
    *(u32*)player |= 1;
    m = mp;
    if ((*(u32*)((s32)m + 4) & 2) == 0) {
        if (*(char**)((s32)m + 0x18) != NULL && strcmp(*(char**)((s32)m + 0x18), str_M_S_1_8041ff98) == 0) {
        } else if ((*(u32*)m & 0x80000000) == 0) {
            *(const char**)((s32)m + 0x18) = str_M_S_1_8041ff98;
            *(s16*)((s32)m + 0x28) = 0;
            *(u32*)((s32)m + 4) &= ~0x30000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        } else {
            dotPose = NULL;
            if (strcmp(str_M_Z_1_8041ff90, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
            } else if (strcmp(str_M_S_1_8041ff98, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
            } else if (strcmp(str_M_W_1_8041ffa0, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
            } else if (strcmp(str_M_R_1_8041ffa8, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
            } else if (strcmp(str_M_J_1B_802c1b24, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            } else if (strcmp(str_M_J_1C_802c1b34, str_M_S_1_8041ff98) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            }
            if (dotPose != NULL) {
                *(char**)((s32)m + 0x18) = dotPose;
                *(s16*)((s32)m + 0x28) = 0;
                *(u32*)((s32)m + 4) &= ~0x30000000;
                *(u32*)((s32)m + 0xC) |= 0x1000;
            }
        }
    }

    if ((*(u32*)player & 0x02000000) == 0) {
        motion = *(u16*)((s32)player + 0x2E);
        if (motion == 0x19) {
            marioReInit_ship();
        } else if (motion == 0x16) {
            marioReInit_roll();
        } else if (motion == 0x14) {
            marioReInit_slit();
        } else if (motion == 0x1A) {
            marioRideYoshi();
        } else {
            marioChgMot(1);
        }
    } else {
        if (*(u16*)((s32)player + 0x2E) == 0x14) {
            *(u32*)player |= 0x40000000;
            marioSlitForceCancel();
        }
        marioChgMot(1);
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioReset(void) {
    typedef struct WordVec {
        u32 x;
        u32 y;
        u32 z;
    } WordVec;
    extern void* mp;
    extern void animPoseSetPaperAnimGroup(s32 poseId, s32 a, s32 b);
    extern void animPaperPoseRelease(s32 poseId);
    extern s32 strcmp(const char* a, const char* b);
    extern void marioChgMot(s32 motion);
    extern u8 str_a_mario_802c18a0[];
    extern const f32 float_20_80420088;
    extern const f32 float_47_804200bc;
    extern const f32 float_1_80420008;
    extern const f32 float_2_80420004;
    extern const f32 float_2p25_804200b8;
    extern const f32 float_37_804200c0;
    extern const f32 float_10_80420078;
    extern const f32 float_25p9_804200c4;
    extern const f32 float_0_80420020;
    void* player;
    void* p;
    void* base;
    s32 spec;

    player = mp;
    spec = *(s8*)((s32)mp + 0x3C);
    if (spec == 1) {
        *(f32*)((s32)mp + 0x1B8) = float_20_80420088;
        *(f32*)((s32)player + 0x1BC) = float_47_804200bc;
        *(f32*)((s32)player + 0x184) = float_1_80420008;
        *(f32*)((s32)player + 0x188) = float_2_80420004;
    } else if (spec == 2) {
        *(f32*)((s32)mp + 0x184) = float_1_80420008;
        *(f32*)((s32)player + 0x188) = float_2p25_804200b8;
    } else {
        if ((*(u32*)mp & 0x02000000) == 0) {
            *(f32*)((s32)mp + 0x1B8) = float_20_80420088;
            *(f32*)((s32)player + 0x1BC) = float_37_804200c0;
        } else {
            *(f32*)((s32)mp + 0x1B8) = float_10_80420078;
            *(f32*)((s32)player + 0x1BC) = float_25p9_804200c4;
        }
        *(f32*)((s32)player + 0x184) = float_1_80420008;
        *(f32*)((s32)player + 0x188) = float_2p25_804200b8;
    }

    *(u32*)player &= ~0x60000000;
    p = mp;
    if ((*(u32*)((s32)mp + 4) & 0x40000000) != 0 && *(void**)((s32)mp + 0x1C) != NULL) {
        animPoseSetPaperAnimGroup(*(s32*)((s32)mp + 0x22C), 0, 0);
        *(s32*)((s32)p + 0x1C) = 0;
        *(u32*)((s32)p + 4) &= ~0x40000000;
        p = mp;
        if (*(s32*)((s32)mp + 0x240) >= 0) {
            animPaperPoseRelease(*(s32*)((s32)mp + 0x240));
            *(s32*)((s32)p + 0x240) = -1;
        }
    }

    p = mp;
    if ((*(u32*)((s32)mp + 4) & 0x40000000) != 0) {
        if ((*(u32*)((s32)mp + 4) & 2) == 0 && (*(u32*)mp & 0x80000000) == 0 &&
            (*(char**)((s32)mp + 0x1C) == NULL || strcmp(*(char**)((s32)mp + 0x1C), NULL) != 0)) {
            *(s32*)((s32)p + 0x1C) = 0;
            *(s16*)((s32)p + 0x28) = 0;
            *(u32*)((s32)p + 0xC) |= 0x4000;
        }
    }

    *(u32*)((s32)player + 4) &= ~0x08000000;
    *(u32*)((s32)player + 4) &= ~0x00000040;
    p = mp;
    base = str_a_mario_802c18a0;
    *(WordVec*)((s32)p + 0xBC) = *(WordVec*)((s32)base + 0x88);
    *(WordVec*)((s32)p + 0xB0) = *(WordVec*)((s32)base + 0x94);
    *(WordVec*)((s32)p + 0x98) = *(WordVec*)((s32)base + 0xA0);
    *(WordVec*)((s32)p + 0xA4) = *(WordVec*)((s32)base + 0xAC);
    *(f32*)((s32)player + 0x180) = float_0_80420020;
    marioChgMot(1);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgPose(char* pose) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern void* dotMarioPose[][6];
    void* m;
    char* dotPose;

    m = mp;
    if ((*(u32*)((s32)m + 4) & 2) == 0) {
        if (*(char**)((s32)m + 0x18) != NULL && strcmp(*(char**)((s32)m + 0x18), pose) == 0) {
            return;
        }
        if ((*(u32*)m & 0x80000000) == 0) {
            *(char**)((s32)m + 0x18) = pose;
            *(s16*)((s32)m + 0x28) = 0;
            *(u32*)((s32)m + 4) &= ~0x30000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        } else {
            dotPose = NULL;
            if (strcmp(str_M_Z_1_8041ff90, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
            } else if (strcmp(str_M_S_1_8041ff98, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
            } else if (strcmp(str_M_W_1_8041ffa0, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
            } else if (strcmp(str_M_R_1_8041ffa8, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
            } else if (strcmp(str_M_J_1B_802c1b24, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            } else if (strcmp(str_M_J_1C_802c1b34, pose) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            }
            if (dotPose != NULL) {
                *(char**)((s32)m + 0x18) = dotPose;
                *(s16*)((s32)m + 0x28) = 0;
                *(u32*)((s32)m + 4) &= ~0x30000000;
                *(u32*)((s32)m + 0xC) |= 0x1000;
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioCtrlOff2Main(void) {
    extern void* mp;
    extern s32 vivianGetStatus(void);
    extern void marioMotion(void);
    extern void marioMove(void);
    extern f32 marioChkOverhead(void);
    extern void marioChgMot(s32 motion);
    extern void marioSetFallPara(void);
    extern void marioJump(void);
    extern void marioChkGnd2(void);
    extern void marioFall(void);
    extern s32 marioChkWallAround(void* pos, s32 arg, f32 a, f32 b, f32 c, f32 d);
    extern void marioChkGnd(void);
    extern f32 float_0_80420020;
    void* mario;
    u16 motion;
    u32 flags;

    mario = mp;
    motion = *(u16*)((s32)mario + 0x2E);
    if (motion == 0x18) {
        return;
    }
    if (motion == 0x19) {
        return;
    }
    if ((u16)(motion - 0x1F) <= 2) {
        return;
    }
    if (motion == 0x14) {
        return;
    }
    if (motion == 0x1A) {
        return;
    }
    if (motion == 8) {
        return;
    }
    if (motion == 6) {
        return;
    }
    if (vivianGetStatus() != 0) {
        return;
    }

    marioMotion();
    flags = *(u32*)mario;
    if (flags & 0x40000) {
        marioMove();
        marioChkOverhead();
        if (*(s16*)((s32)mario + 0x50) > 100) {
            marioChgMot(0xA);
            marioSetFallPara();
        }
    } else if (flags & 0x10000) {
        marioJump();
        marioChkOverhead();
        marioMove();
        if (*(u16*)((s32)mp + 0x2E) != 0x16 && *(s16*)((s32)mario + 0x50) > 5) {
            marioChkGnd2();
        }
    } else if (flags & 0x20000) {
        if (*(u16*)((s32)mario + 0x2E) != 0x19) {
            marioMove();
            if (*(u16*)((s32)mario + 0x2E) == 0x10) {
                f32 y = marioChkOverhead();
                if (*(void**)((s32)mario + 0x1F8) != NULL) {
                    *(f32*)((s32)mario + 0x90) = y;
                }
            } else {
                marioChkOverhead();
            }
            marioFall();
            motion = *(u16*)((s32)mario + 0x2E);
            if (motion == 0x10 || motion == 0x11) {
                marioChkGnd2();
            } else if (*(u16*)((s32)mp + 0x2E) != 0x16 && *(s16*)((s32)mario + 0x50) > 5) {
                marioChkGnd2();
            }
            marioChkWallAround((void*)((s32)mario + 0x8C), 0, float_0_80420020, float_0_80420020, *(f32*)((s32)mario + 0x1C0), *(f32*)((s32)mario + 0x1BC));
        }
    } else {
        marioChkGnd();
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioResetHitObj(char* name) {
    extern void* mp;
    extern char* hitGetName(void* hit);
    extern s32 strcmp(const char* a, const char* b);
    void* m;
    void* hit;

    m = mp;
    hit = *(void**)((s32)m + 0x1E0);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1E0) = NULL;
    }
    hit = *(void**)((s32)m + 0x1E4);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1E4) = NULL;
    }
    hit = *(void**)((s32)m + 0x1E8);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1E8) = NULL;
    }
    hit = *(void**)((s32)m + 0x1EC);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1EC) = NULL;
    }
    hit = *(void**)((s32)m + 0x1F0);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1F0) = NULL;
    }
    hit = *(void**)((s32)m + 0x1F4);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1F4) = NULL;
    }
    hit = *(void**)((s32)m + 0x1F8);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1F8) = NULL;
    }
    hit = *(void**)((s32)m + 0x1FC);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x1FC) = NULL;
    }
    hit = *(void**)((s32)m + 0x200);
    if (hit != NULL && strcmp(hitGetName(hit), name) == 0) {
        *(void**)((s32)m + 0x200) = NULL;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void* toDotMarioPose(char* pose) {
    extern s32 strcmp(const char* a, const char* b);
    extern void* mp;
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern void* dotMarioPose[][6];
    void* result;

    result = NULL;
    if (strcmp(str_M_Z_1_8041ff90, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
    } else if (strcmp(str_M_S_1_8041ff98, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
    } else if (strcmp(str_M_W_1_8041ffa0, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
    } else if (strcmp(str_M_R_1_8041ffa8, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
    } else if (strcmp(str_M_J_1B_802c1b24, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
    } else if (strcmp(str_M_J_1C_802c1b34, pose) == 0) {
        result = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
    }
    return result;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioDispBlur(s32 param_1, void* mario) {
    extern void* mp;
    extern u32 animPoseGetMaterialFlag(s32 poseId);
    extern void marioDispBlurSub(void* data, s32 index);
    extern void animPoseSetMaterialFlagOff(s32 poseId, u32 flag);
    extern void animPoseSetMaterialFlagOn(s32 poseId, u32 flag);
    extern f32 float_6p2832_80420028;
    extern f32 float_360_8042002c;
    extern f32 float_3p1416_80420030;
    extern f32 float_1p5708_80420034;
    extern f32 float_4p7124_80420038;
    u8 data[60];
    u32 flags;
    s32 index;
    u32 materialFlags;
    s32 i;
    f32 angle;

    flags = *(u32*)((s32)mp + 4);
    if (flags & 0x10000000) {
        index = 2;
    } else if (flags & 0x80000000) {
        index = 1;
    } else {
        index = 0;
    }
    materialFlags = animPoseGetMaterialFlag(*(s32*)((s32)mario + 0x22C + index * 4));

    angle = (float_6p2832_80420028 * *(f32*)((s32)mario + 0x19C)) / float_360_8042002c;
    if (angle <= float_3p1416_80420030) {
        if (angle >= float_1p5708_80420034) {
        }
    } else {
        (void)(angle == float_4p7124_80420038);
    }
    if (angle <= float_3p1416_80420030) {
        if (angle >= float_1p5708_80420034) {
        }
    } else {
        (void)(angle == float_4p7124_80420038);
    }

    for (i = 7; i > 0; i--) {
        marioDispBlurSub(data, i);
    }

    flags = *(u32*)((s32)mp + 4);
    if (flags & 0x10000000) {
        index = 2;
    } else if (flags & 0x80000000) {
        index = 1;
    } else {
        index = 0;
    }
    animPoseSetMaterialFlagOff(*(s32*)((s32)mario + 0x22C + index * 4), 0x40);

    flags = *(u32*)((s32)mp + 4);
    if (flags & 0x10000000) {
        index = 2;
    } else if (flags & 0x80000000) {
        index = 1;
    } else {
        index = 0;
    }
    animPoseSetMaterialFlagOn(*(s32*)((s32)mario + 0x22C + index * 4), materialFlags);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 marioChkKey(void) {
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern s32 seqGetSeq(void);
    extern void* mp;
    extern s32 marioShipChanging(void);
    extern s32 marioRollKeyDisable(void);
    extern s32 marioSlitKeyDisable(void);
    extern s32 christineGetStatus(void);
    extern s32 mario_bomhei_keychk2(void);
    extern s32 yoshiGetStatus(void);
    extern s32 nokonoko_holdItem(void);
    void* party;

    party = partyGetPtr(marioGetPartyId());
    if (seqGetSeq() != 2) {
        return 0;
    }
    if (*(s8*)((s32)mp + 0x39) != 0) {
        return 0;
    }
    if (marioShipChanging() == 1 || marioShipChanging() == 2) {
        return 0;
    }
    if (marioRollKeyDisable() == 0) {
        return 0;
    }
    if (marioSlitKeyDisable() == 0) {
        return 0;
    }
    if (party == 0) {
        return 1;
    }
    if (christineGetStatus() == 1) {
        return 0;
    }
    if (mario_bomhei_keychk2() == 0) {
        return 0;
    }
    if (yoshiGetStatus() != 0) {
        return 0;
    }
    if ((*(u32*)party & 0x100) != 0) {
        s32 mode = *(s8*)((s32)party + 0x31);
        if (mode == 7 || mode == 5 || mode == 6) {
            return 0;
        }
    }
    if (nokonoko_holdItem() != 0) {
        return 0;
    }
    return 1;
}

s32 marioPaperOn(char* name) {
    extern void* mp;
    extern s32 strcmp(const char*, const char*);
    extern void animPaperPoseRelease(s32);
    extern s32 animPaperPoseEntry(char*, s32);
    extern void animPoseSetPaperAnimGroup(s32, char*, s32);
    extern char str_a_mario_802c18a0[];
    void* mario;
    char* base;
    s32 special;

    base = str_a_mario_802c18a0;
    mario = mp;
    if (*(s32*)((s32)mario + 0x240) >= 0) {
        animPaperPoseRelease(*(s32*)((s32)mario + 0x240));
        *(s32*)((s32)mario + 0x240) = -1;
    }
    *(u32*)((s32)mario + 4) |= 0x40000000;
    *(s32*)((s32)mario + 0x240) = animPaperPoseEntry(name, 2);

    special = 0;
    if (strcmp(name, base + 0x300) == 0 || strcmp(name, base + 0x308) == 0 ||
        strcmp(name, base + 0x310) == 0 || strcmp(name, base + 0x318) == 0 ||
        strcmp(name, base + 0x324) == 0 || strcmp(name, base + 0x330) == 0) {
        special = 1;
    }
    animPoseSetPaperAnimGroup(*(s32*)((s32)mario + 0x22C), name, special == 0);
    return 1;
}

void marioBgmodeOff(void) {
    extern void* mp;
    extern void camFollowYOn(void);
    extern const f32 float_20_80420088;
    extern const f32 float_47_804200bc;
    extern const f32 float_1_80420008;
    extern const f32 float_2_80420004;
    extern const f32 float_2p25_804200b8;
    extern const f32 float_37_804200c0;
    extern const f32 float_10_80420078;
    extern const f32 float_25p9_804200c4;
    void* m = mp;
    s32 spec;
    *(u32*)m &= ~0x02000000;
    m = mp;
    spec = *(s8*)((s32)m + 0x3C);
    if (spec == 1) {
        *(f32*)((s32)m + 0x1B8) = float_20_80420088;
        *(f32*)((s32)m + 0x1BC) = float_47_804200bc;
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2_80420004;
    } else if (spec == 2) {
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
    } else {
        if ((*(u32*)m & 0x02000000) == 0) {
            *(f32*)((s32)m + 0x1B8) = float_20_80420088;
            *(f32*)((s32)m + 0x1BC) = float_37_804200c0;
        } else {
            *(f32*)((s32)m + 0x1B8) = float_10_80420078;
            *(f32*)((s32)m + 0x1BC) = float_25p9_804200c4;
        }
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
    }
    camFollowYOn();
}

void marioBgmodeOn(void) {
    extern void* mp;
    extern void camFollowYOff(void);
    extern const f32 float_20_80420088;
    extern const f32 float_47_804200bc;
    extern const f32 float_1_80420008;
    extern const f32 float_2_80420004;
    extern const f32 float_2p25_804200b8;
    extern const f32 float_37_804200c0;
    extern const f32 float_10_80420078;
    extern const f32 float_25p9_804200c4;
    void* m = mp;
    s32 spec;
    *(u32*)m |= 0x02000000;
    m = mp;
    spec = *(s8*)((s32)m + 0x3C);
    if (spec == 1) {
        *(f32*)((s32)m + 0x1B8) = float_20_80420088;
        *(f32*)((s32)m + 0x1BC) = float_47_804200bc;
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2_80420004;
    } else if (spec == 2) {
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
    } else {
        if ((*(u32*)m & 0x02000000) == 0) {
            *(f32*)((s32)m + 0x1B8) = float_20_80420088;
            *(f32*)((s32)m + 0x1BC) = float_37_804200c0;
        } else {
            *(f32*)((s32)m + 0x1B8) = float_10_80420078;
            *(f32*)((s32)m + 0x1BC) = float_25p9_804200c4;
        }
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
    }
    camFollowYOff();
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioSetPaperAnimeLocalTime(s32 time) {
    extern void* mp;
    extern void animPoseSetLocalTime(s32 poseId, f32 time);
    void* m = mp;
    s32 index;
    *(s16*)((s32)m + 0x28) = time;
    animPoseSetLocalTime(*(s32*)((s32)m + 0x240), (f32)time);
    m = mp;
    if ((*(u32*)((s32)m + 4) & 0x10000000) != 0) {
        index = 2;
    } else if ((*(u32*)((s32)m + 4) & 0x80000000) != 0) {
        index = 1;
    } else {
        index = 0;
    }
    animPoseSetLocalTime(*(s32*)((s32)m + 0x22C + index * 4), (f32)time);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

void marioFBattlePost(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* mp;
    extern Vec R_last_position;
    extern void marioYoshiForceCancel(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern void partyGetAppearPos(void*, Vec*);
    extern void partyCtrlOn(void);
    extern void marioChgMot(s32);
    void* mario;
    void* party;
    Vec pos;

    mario = mp;
    *(u32*)mario &= ~0x1000;
    if (*(u16*)((s32)mario + 0x2E) == 0x14) {
        *(Vec*)((s32)mario + 0x8C) = R_last_position;
    }
    if (*(u16*)((s32)mario + 0x2E) == 0x1A) {
        marioYoshiForceCancel();
    }
    party = partyGetPtr(marioGetPartyId());
    if (party != 0) {
        partyGetAppearPos(party, &pos);
        *(Vec*)((s32)party + 0x58) = pos;
    }
    partyCtrlOn();
    if (*(u16*)((s32)mario + 0x2E) == 0x12 || *(u16*)((s32)mario + 0x2E) == 0x14) {
        marioChgMot(0);
    }
}

s32 marioChkSts(u32 flags) {
    extern void* mp;
    void* m = mp;
    s32 ret = 0;

    if ((flags & 1) && (*(u32*)m & 0x10000)) {
        ret = 1;
    }
    if ((flags & 2) && (*(u32*)m & 0x20000)) {
        ret = 1;
    }
    if ((flags & 4) && (*(u32*)m & 0x100000)) {
        ret = 1;
    }
    if ((flags & 0x20) && *(u16*)((s32)m + 0x2E) == 0x17) {
        ret = 1;
    }
    if ((flags & 0x10) && *(u16*)((s32)m + 0x2E) == 0x16) {
        ret = 1;
    }
    if ((flags & 8) && *(u16*)((s32)m + 0x2E) == 0x14) {
        ret = 1;
    }
    return ret;
}

void marioPoseInit(void) {
    extern void* mp;
    extern void* marioAnimeGroupData[];
    extern s32 animPoseEntry(void* name, s32 flags);
    void* m = mp;
    s32 color = *(s8*)((s32)m + 0x3D);
    *(s32*)((s32)m + 0x22C) = animPoseEntry(marioAnimeGroupData[color * 3], 2);
    m = mp;
    color = *(s8*)((s32)m + 0x3D);
    *(s32*)((s32)mp + 0x230) = animPoseEntry(marioAnimeGroupData[color * 3 + 1], 2);
    m = mp;
    color = *(s8*)((s32)m + 0x3D);
    *(s32*)((s32)mp + 0x234) = animPoseEntry(marioAnimeGroupData[color * 3 + 2], 2);
}

void marioSetSpec(void) {
    extern void* mp;
    extern const f32 float_20_80420088;
    extern const f32 float_47_804200bc;
    extern const f32 float_1_80420008;
    extern const f32 float_2_80420004;
    extern const f32 float_2p25_804200b8;
    extern const f32 float_37_804200c0;
    extern const f32 float_10_80420078;
    extern const f32 float_25p9_804200c4;
    void* m = mp;
    s32 spec = *(s8*)((s32)m + 0x3C);

    if (spec == 1) {
        *(f32*)((s32)m + 0x1B8) = float_20_80420088;
        *(f32*)((s32)m + 0x1BC) = float_47_804200bc;
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2_80420004;
        return;
    }
    if (spec == 2) {
        *(f32*)((s32)m + 0x184) = float_1_80420008;
        *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
        return;
    }
    if ((*(u32*)m & 0x02000000) == 0) {
        *(f32*)((s32)m + 0x1B8) = float_20_80420088;
        *(f32*)((s32)m + 0x1BC) = float_37_804200c0;
    } else {
        *(f32*)((s32)m + 0x1B8) = float_10_80420078;
        *(f32*)((s32)m + 0x1BC) = float_25p9_804200c4;
    }
    *(f32*)((s32)m + 0x184) = float_1_80420008;
    *(f32*)((s32)m + 0x188) = float_2p25_804200b8;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgPoseTime(char* name, s32 time) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    void* m = mp;
    if (!(*(u32*)((s32)m + 4) & 2) && !(*(u32*)m & 0x80000000)) {
        char* old = *(char**)((s32)m + 0x18);
        if (old == 0 || strcmp(old, name) != 0) {
            *(char**)((s32)m + 0x18) = name;
            *(s16*)((s32)m + 0x28) = time;
            *(u32*)((s32)m + 4) &= ~0x30000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgEvtPose(char* name) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    void* m = mp;
    if (!(*(u32*)((s32)m + 4) & 2) && !(*(u32*)m & 0x80000000)) {
        char* old = *(char**)((s32)m + 0x18);
        if (old == 0 || strcmp(old, name) != 0) {
            *(char**)((s32)m + 0x18) = name;
            *(u32*)((s32)m + 4) &= ~0x20000000;
            *(u32*)((s32)m + 4) |= 0x10000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioChgPaper(char* name) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    void* m = mp;
    if ((*(u32*)((s32)m + 4) & 0x40000000) && !(*(u32*)((s32)m + 4) & 2) && !(*(u32*)m & 0x80000000)) {
        char* old = *(char**)((s32)m + 0x1C);
        if (old == 0 || strcmp(old, name) != 0) {
            *(char**)((s32)m + 0x1C) = name;
            *(s16*)((s32)m + 0x28) = 0;
            *(u32*)((s32)m + 0xC) |= 0x4000;
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void marioGetScreenPos(f32* pos, f32* x, f32* y, f32* z) {
    extern void* camGetPtr(s32 id);
    extern void GXSetProjection(void* proj, s32 type);
    extern void GXGetProjectionv(f32* proj);
    extern void GXGetViewportv(f32* viewport);
    extern void GXProject(f32 x, f32 y, f32 z, void* model, f32* proj, f32* viewport, f32* outX, f32* outY, f32* outZ);
    f32 proj[7];
    f32 viewport[6];
    void* cam;

    cam = camGetPtr(4);
    GXSetProjection((void*)((s32)cam + 0x15C), *(s32*)((s32)cam + 0x19C));
    GXGetProjectionv(proj);
    GXGetViewportv(viewport);
    GXProject(pos[0], pos[1], pos[2], (void*)((s32)cam + 0x11C), proj, viewport, x, y, z);
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 marioChkPushAnime(void) {
    extern void* mp;
    extern s32 strcmp(const char* s1, const char* s2);
    extern char str_M_O_1_8041ffe0;
    extern char str_M_O_2_8042009c;
    extern char str_M_O_2R_802c1b6c[];
    void* mario;
    s32 result;

    result = 0;
    mario = mp;
    if (strcmp(*(char**)((s32)mario + 0x18), &str_M_O_1_8041ffe0) == 0 ||
        strcmp(*(char**)((s32)mario + 0x18), &str_M_O_2_8042009c) == 0 ||
        strcmp(*(char**)((s32)mario + 0x18), str_M_O_2R_802c1b6c) == 0) {
        result = 1;
    }
    return result;
}

u8 marioFBattlePrepare(void) {
    typedef struct Vec {
        f32 x;
        f32 y;
        f32 z;
    } Vec;
    extern void* mp;
    extern Vec R_last_position;
    extern void marioAdjustMoveDir(void);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 partyId);
    extern void partyUsePost(void* party);
    extern void partyCtrlOff(void);
    void* mario = mp;

    R_last_position = *(Vec*)((s32)mario + 0x8C);
    marioAdjustMoveDir();
    if (*(u16*)((s32)mario + 0x2E) != 0x14) {
        *(u32*)mario |= 0x1000;
    }
    partyUsePost(partyGetPtr(marioGetPartyId()));
    partyCtrlOff();
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_800591b4(void) {
    extern void* mp;
    extern s32 strcmp(const char* a, const char* b);
    extern const char str_M_D_1_80420094[6];
    extern const char str_M_Z_1_8041ff90[6];
    extern const char str_M_S_1_8041ff98[6];
    extern const char str_M_W_1_8041ffa0[6];
    extern const char str_M_R_1_8041ffa8[6];
    extern const char str_M_J_1B_802c1b24[7];
    extern const char str_M_J_1C_802c1b34[7];
    extern void* dotMarioPose[][6];
    void* m;
    char* dotPose;
    u16 motion;

    m = mp;
    motion = *(u16*)((s32)m + 0x2E);
    if (motion != 0x19 && motion != 0x16 && motion != 0x18 && motion != 0x14 && motion != 0x1A &&
        (*(u32*)((s32)m + 4) & 0x01000000) == 0 && (*(u32*)((s32)m + 4) & 2) == 0 &&
        (*(char**)((s32)m + 0x18) == NULL || strcmp(*(char**)((s32)m + 0x18), str_M_D_1_80420094) != 0)) {
        if ((*(u32*)m & 0x80000000) == 0) {
            *(const char**)((s32)m + 0x18) = str_M_D_1_80420094;
            *(s16*)((s32)m + 0x28) = 0;
            *(u32*)((s32)m + 4) &= ~0x30000000;
            *(u32*)((s32)m + 0xC) |= 0x1000;
        } else {
            dotPose = NULL;
            if (strcmp(str_M_Z_1_8041ff90, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][0];
            } else if (strcmp(str_M_S_1_8041ff98, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][1];
            } else if (strcmp(str_M_W_1_8041ffa0, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][2];
            } else if (strcmp(str_M_R_1_8041ffa8, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][3];
            } else if (strcmp(str_M_J_1B_802c1b24, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            } else if (strcmp(str_M_J_1C_802c1b34, str_M_D_1_80420094) == 0) {
                dotPose = dotMarioPose[*(s8*)((s32)mp + 0x3D)][5];
            }
            if (dotPose != NULL) {
                *(char**)((s32)m + 0x18) = dotPose;
                *(s16*)((s32)m + 0x28) = 0;
                *(u32*)((s32)m + 4) &= ~0x30000000;
                *(u32*)((s32)m + 0xC) |= 0x1000;
            }
        }
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
