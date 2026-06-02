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
