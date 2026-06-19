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
}


void marioEntry(void) {
}


u8 marioReset(void) {
    return 0;
}


void marioChgPose(void* pose) {
}


u8 marioCtrlOff2Main(void) {
    return 0;
}


u8 marioResetHitObj(char* pObjName) {
    return 0;
}


void* toDotMarioPose(char* param_1) {
    return 0;
}


u8 marioDispBlur(s32 param_1, int param_2) {
    return 0;
}


s32 marioChkKey(void) {
    return 0;
}


s32 marioPaperOn(char* param_1) {
    return 0;
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

u8 marioFBattlePost(void) {
    return 0;
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

u8 marioGetScreenPos(float* param_1, float* param_2, float* param_3, float* param_4) {
    return 0;
}


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

