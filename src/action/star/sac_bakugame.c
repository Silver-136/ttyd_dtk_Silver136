#include "action/star/sac_bakugame.h"
#include "event/evt_cmd.h"

extern void* _battleWorkPointer;
extern char str_c_star_bom_802ff6c0[];
const char str_Z_1_80427c20[] = "Z_1";

void* memset(void* dst, int value, u32 size);
void* BattleGetUnitPtr(void* battleWork, s32 unitId);
s32 BtlUnit_CheckStatus(void* unit, s32 status);
s32 BtlUnit_CanActStatus(void* unit);
s32 BtlUnit_GetBodyPartsId(void* unit);
void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
void btlDispPoseAnime(void* part);
s32 animGroupBaseAsync(void* name, s32 mode, s32 flags);
s32 animPoseEntry(void* name, s32 mode);
void animPoseSetAnim(s32 poseId, const void* name, s32 force);
void BtlUnit_SetBodyAnimType(void* unit, s32 type);
void effDelete(void* effect);

typedef struct BakuGameWork {
    u8 pad[0xEC];
    s32 poseId;
    u8 padF0[0x204 - 0xF0];
    s32 starAppear;
} BakuGameWork;

void bakuGameDispStar(void) {
}

s32 bakuGameAudienceCanThrowPos(s32 pos) {
    s32 i;
    s32 base = 0;
    for (i = 0; i < 3; i++) {
        if (pos >= base + 8 && pos <= base + 11) {
            return 1;
        }
        base += 0x14;
    }
    return 0;
}

BakuGameWork* GetBakuGamePtr(void) {
    return (BakuGameWork*)((s32)_battleWorkPointer + 0x1F4C);
}

USER_FUNC(star_stone_appear) {
    BakuGameWork* wp;

    wp = GetBakuGamePtr();
    wp->starAppear = 1;
    return 2;
}

USER_FUNC(init_bakugame) {
    memset(GetBakuGamePtr(), 0, 0x2B8);
    return 2;
}

s32 bakuGamePartyExist(void* unit) {
    if (unit == 0) {
        return 0;
    }
    if (BtlUnit_CheckStatus(unit, 0x1B) != 0) {
        return 0;
    }
    return BtlUnit_CanActStatus(unit) != 0;
}

void bakuGameMarioSurpriseReset(void* mario, void* party) {
    btlDispPoseAnime(BtlUnit_GetPartsPtr(mario, BtlUnit_GetBodyPartsId(mario)));
    if (bakuGamePartyExist(party) != 0) {
        btlDispPoseAnime(BtlUnit_GetPartsPtr(party, BtlUnit_GetBodyPartsId(party)));
    }
}

USER_FUNC(bakuGameBombEntry) {
    BakuGameWork* work = GetBakuGamePtr();
    if (animGroupBaseAsync(str_c_star_bom_802ff6c0, 2, 0) == 0) {
        return 0;
    }
    work->poseId = animPoseEntry(str_c_star_bom_802ff6c0, 2);
    animPoseSetAnim(work->poseId, str_Z_1_80427c20, 1);
    return 2;
}

void bakuGameEnemySurprise(void) {
    s32 i;
    void* battleWork = _battleWorkPointer;
    for (i = 0; i < 0x40; i++) {
        void* unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && *(s8*)((s32)unit + 0xC) == 1 && (*(u32*)((s32)unit + 0x104) & 0x20000) == 0
            && BtlUnit_CheckStatus(unit, 0x1B) == 0) {
            BtlUnit_SetBodyAnimType(unit, 0x27);
        }
    }
}

USER_FUNC(end_bakugame) {
    BakuGameWork* work = GetBakuGamePtr();

    effDelete(*(void**)(*(s32*)(*(s32*)((s32)work + 0x254) + 0xC) + 0x34));
    effDelete(*(void**)((s32)work + 0x254));
    return 2;
}

void bakuGameAudienceSurprise(void) {
    extern void* BattleAudienceGetPtr(s32 id);
    extern s32 BattleAudience_GetWaiting(s32 id);
    extern void BattleAudience_SetAnim(s32 id, s32 anim, s32 pose);
    extern s32 bakuGameAudienceCanThrowPos(s32 pos);
    extern s32 irand(s32 max);
    extern void* memset(void* dst, int value, u32 size);
    extern f32 float_90_80427b7c;
    extern f32 float_270_80427b80;

    s32 i;
    void* audience;

    for (i = 0; i < 0xC8; i++) {
        audience = BattleAudienceGetPtr(i);
        if ((u8)BattleAudience_GetWaiting(i) != 0) {
            *(u32*)audience |= 0x10;
            memset((void*)((s32)audience + 0xF8), 0, 0x34);
            if (bakuGameAudienceCanThrowPos(i) == 0) {
                BattleAudience_SetAnim(i, 7, 0);
                if (irand(2) == 0) {
                    *(f32*)((s32)audience + 0xE8) = float_90_80427b7c;
                } else {
                    *(f32*)((s32)audience + 0xE8) = float_270_80427b80;
                }
            } else {
                BattleAudience_SetAnim(i, 3, 0);
            }
        }
    }
}

void bakuGameAudienceSurpriseReset(void) {
    extern void* BattleAudienceGetPtr(s32 id);
    extern s32 BattleAudience_GetExist(s32 id);
    extern s32 bakuGameAudienceCanThrowPos(s32 pos);
    extern f32 float_90_80427b7c;

    s32 i;
    void* audience;
    f32 angle;

    i = 0;
    angle = float_90_80427b7c;
    while (i < 0xC8) {
        if ((u8)BattleAudience_GetExist(i) != 0) {
            audience = BattleAudienceGetPtr(i);
            *(f32*)((s32)audience + 0xE8) = angle;
            if (bakuGameAudienceCanThrowPos(i) == 0) {
                *(u32*)audience &= ~0x10;
            }
        }
        i++;
    }
}

void bakuGameEnemySurpriseReset(void) {
    extern void* _battleWorkPointer;
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern s32 BtlUnit_CheckStatus(void* unit, s32 status);
    extern s32 BtlUnit_GetBodyPartsId(void* unit);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
    extern void btlDispPoseAnime(void* part);

    s32 i;
    void* battleWork;
    void* unit;

    battleWork = _battleWorkPointer;
    for (i = 0; i < 0x40; i++) {
        unit = BattleGetUnitPtr(battleWork, i);
        if (unit != 0 && *(s8*)((s32)unit + 0xC) == 1 && (*(u32*)((s32)unit + 0x104) & 0x20000) == 0
            && BtlUnit_CheckStatus(unit, 0x1B) == 0) {
            btlDispPoseAnime(BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit)));
        }
    }
}

void bakuGameMarioSurprise(void* mario, void* party) {
    extern void BtlUnit_SetBodyAnim(void* unit, const char* name);
    extern s32 bakuGamePartyExist(void* unit);
    extern const char str_btl_wn_sac_bakugame_802ff3a0[];
    extern const char str_M_N_7_80427b84[6];

    const char* base;

    base = str_btl_wn_sac_bakugame_802ff3a0;
    BtlUnit_SetBodyAnim(mario, str_M_N_7_80427b84);
    if (bakuGamePartyExist(party) != 0) {
        switch (*(s32*)((s32)party + 4) - 0xE0) {
            case 0:
                BtlUnit_SetBodyAnim(party, base + 0x24C);
                break;
            case 1:
                BtlUnit_SetBodyAnim(party, base + 0x254);
                break;
            case 2:
                BtlUnit_SetBodyAnim(party, base + 0x25C);
                break;
            case 3:
                BtlUnit_SetBodyAnim(party, base + 0x264);
                break;
            case 4:
                BtlUnit_SetBodyAnim(party, base + 0x26C);
                break;
            case 5:
                BtlUnit_SetBodyAnim(party, base + 0x274);
                break;
            case 6:
                BtlUnit_SetBodyAnim(party, base + 0x27C);
                break;
        }
    }
}

void bakuGameDecideButton(s32 flag) {
    typedef struct ButtonTable {
        s32 values[9];
    } ButtonTable;
    typedef struct PatternTable {
        s32 values[18];
    } PatternTable;
    extern void* GetBakuGamePtr(void);
    extern const ButtonTable dat_802ff504;
    extern const PatternTable dat_802ff528;
    extern s32 irand(s32 max);

    void* work;
    ButtonTable buttons;
    PatternTable patterns;
    s32 row;
    s32* rowBase;
    s32* buttonsBase;
    s32 workOffset;
    s32 rowOffset;
    s32 i;
    s32 entryOffset;
    s32 buttonOffset;
    s32* src;
    s32* dst;

    work = GetBakuGamePtr();
    buttons = dat_802ff504;
    patterns = dat_802ff528;
    row = irand(6) * 0xC;
    rowBase = (s32*)((s32)&patterns.values[0] + row);
    buttonsBase = buttons.values;
    workOffset = 0;
    rowOffset = 0;
    for (i = 0; i < 3; i++) {
        entryOffset = workOffset + 0xC;
        dst = (s32*)((s32)work + entryOffset);
        workOffset += 0x24;
        *(s32*)((s32)dst + 0xC) = *(s32*)((s32)rowBase + rowOffset);
        buttonOffset = *(s32*)((s32)rowBase + rowOffset) * 0xC;
        rowOffset += 4;
        src = (s32*)((s32)buttonsBase + buttonOffset);
        *(s32*)((s32)dst + 0x14) = src[0];
        *(s32*)((s32)dst + 0x18) = src[1];
        *(s32*)((s32)dst + 0x1C) = src[2];
    }
    if (flag == 0) {
        *(s32*)((s32)work + 0xF4) = 0x3C;
    }
}

USER_FUNC(bakuGameDecideWeapon) {
    extern void* GetBakuGamePtr(void);
    extern void* memcpy(void* dst, const void* src, u32 size);
    extern u8 weapon[0xC0];
    extern f32 float_100_80427b78;

    s32* args;
    u8* weaponWork;
    s32 value;
    void* work;

    args = event->args;
    evtGetFloat(event, args[0]);
    value = evtGetValue(event, args[1]);
    work = GetBakuGamePtr();
    weaponWork = (u8*)((s32)work + 0x178);
    memcpy((void*)((s32)work + 0xF8), weapon, 0xC0);
    weaponWork[2] = 0x64;
    if (value != -1) {
        if (*(f32*)((s32)work + 0xD0) >= float_100_80427b78) {
            weaponWork[2] = 0x7F;
        } else {
            weaponWork[2] = 0x64;
        }
        weaponWork[3] = value - 1;
    } else {
        weaponWork[2] = 0;
        weaponWork[3] = 0;
    }
    evtSetValue(event, args[2], (s32)work + 0xF8);
    return 2;
}

