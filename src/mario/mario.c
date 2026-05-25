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

