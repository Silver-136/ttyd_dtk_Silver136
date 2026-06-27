#include "mario/mario_pouch.h"

typedef s16 ItemType16;
extern s32 mpp;
extern u8 itemDataTable[];
extern u8 _party_max_hp_table[];

s32 pouchCheckItem(s32 item);

ItemType16 pouchHaveItem(s32 index) {
    return *(ItemType16*)((s32)mpp + 0x192 + index * 2);
}
PouchData* pouchGetPtr(void) {
    return (PouchData*)mpp;
}
s32 pouchGetHaveItemCnt(void) {
    s32 count;
    s32 loops;
    PouchData* pouch;

    count = 0;
    loops = 2;
    pouch = (PouchData*)mpp;

    for (; loops > 0; loops--) {
        if (pouch->mItems[0] != 0) {
            count++;
        }
        if (pouch->mItems[1] != 0) {
            count++;
        }
        if (pouch->mItems[2] != 0) {
            count++;
        }
        if (pouch->mItems[3] != 0) {
            count++;
        }
        if (pouch->mItems[4] != 0) {
            count++;
        }
        if (pouch->mItems[5] != 0) {
            count++;
        }
        if (pouch->mItems[6] != 0) {
            count++;
        }
        if (pouch->mItems[7] != 0) {
            count++;
        }
        if (pouch->mItems[8] != 0) {
            count++;
        }
        if (pouch->mItems[9] != 0) {
            count++;
        }

        pouch = (PouchData*)((s32)pouch + 0x14);
    }

    return count;
}
s32 pouchGetEmptyHaveItemCnt(void) {
    s32 hasStrangeSack;
    s32 slots;
    s32 empty;
    PouchData* pouch;

    hasStrangeSack = pouchCheckItem(ITEM_STRANGE_SACK);

    slots = 10;
    if (hasStrangeSack != 0) {
        slots = 20;
    }

    empty = 0;
    pouch = (PouchData*)mpp;

    for (; slots > 0; slots--) {
        if (pouch->mItems[0] == 0) {
            empty++;
        }

        pouch = (PouchData*)((s32)pouch + 2);
    }

    return empty;
}
s32 pouchCheckItem(s32 itemId) {
    s32 count;
    s32 loops;
    PouchData* pouch;

    if (((s32)itemId >= 0x79) && ((s32)itemId < 0xEC)) {
        count = 0;
        loops = 2;
        pouch = (PouchData*)mpp;

        for (; loops > 0; loops--) {
            if (itemId == pouch->mItems[0]) {
                count++;
            }
            if (itemId == pouch->mItems[1]) {
                count++;
            }
            if (itemId == pouch->mItems[2]) {
                count++;
            }
            if (itemId == pouch->mItems[3]) {
                count++;
            }
            if (itemId == pouch->mItems[4]) {
                count++;
            }
            if (itemId == pouch->mItems[5]) {
                count++;
            }
            if (itemId == pouch->mItems[6]) {
                count++;
            }
            if (itemId == pouch->mItems[7]) {
                count++;
            }
            if (itemId == pouch->mItems[8]) {
                count++;
            }
            if (itemId == pouch->mItems[9]) {
                count++;
            }

            pouch = (PouchData*)&pouch->partyData[1].currentHP;
        }

        return count;
    }

    if (((s32)itemId >= 1) && ((s32)itemId < 0x79)) {
        count = 0;
        loops = 0x79;
        pouch = (PouchData*)mpp;

        for (; loops > 0; loops--) {
            if (itemId == pouch->mKeyItems[0]) {
                count++;
            }

            pouch = (PouchData*)((s32)pouch + 2);
        }

        return count;
    }

    if (((s32)itemId >= 0xF0) && ((s32)itemId < 0x153)) {
        count = 0;
        loops = 0x14;
        pouch = (PouchData*)mpp;

        for (; loops > 0; loops--) {
            if (itemId == pouch->mBadges[0]) {
                count++;
            }
            if (itemId == pouch->mBadges[1]) {
                count++;
            }
            if (itemId == pouch->mBadges[2]) {
                count++;
            }
            if (itemId == pouch->mBadges[3]) {
                count++;
            }
            if (itemId == pouch->mBadges[4]) {
                count++;
            }
            if (itemId == pouch->mBadges[5]) {
                count++;
            }
            if (itemId == pouch->mBadges[6]) {
                count++;
            }
            if (itemId == pouch->mBadges[7]) {
                count++;
            }
            if (itemId == pouch->mBadges[8]) {
                count++;
            }
            if (itemId == pouch->mBadges[9]) {
                count++;
            }

            pouch = (PouchData*)&pouch->partyData[1].currentHP;
        }

        return count;
    }

    return 0;
}
s32 pouchGetCoin(void) {
    return ((PouchData*)mpp)->mCoins;
}
s32 pouchSetCoin(s16 value) {
    ((PouchData*)mpp)->mCoins = value;

    if (((PouchData*)mpp)->mCoins < 0) {
        ((PouchData*)mpp)->mCoins = 0;
    }

    if (((PouchData*)mpp)->mCoins > 999) {
        ((PouchData*)mpp)->mCoins = 999;
    }

    return ((PouchData*)mpp)->mCoins;
}

s32 pouchAddCoin(s16 coins) {
    ((PouchData*)mpp)->mCoins = ((PouchData*)mpp)->mCoins + coins;

    if (((PouchData*)mpp)->mCoins < 0) {
        ((PouchData*)mpp)->mCoins = 0;
    }

    if (((PouchData*)mpp)->mCoins > 999) {
        ((PouchData*)mpp)->mCoins = 999;
    }

    return ((PouchData*)mpp)->mCoins;
}
s32 pouchAddStarPiece(s16 amount) {
    ((PouchData*)mpp)->mStarPieceCount += amount;

    return ((PouchData*)mpp)->mStarPieceCount;
}
s32 pouchGetStarPiece(void) {
    return ((PouchData*)mpp)->mStarPieceCount;
}
s32 pouchGetSuperCoin(void) {
    return ((PouchData*)mpp)->mShineSpriteCount;
}
s16 pouchSetSuperCoin(s16 newCount) {
    ((PouchData*)mpp)->mShineSpriteCount = newCount;

    return ((PouchData*)mpp)->mShineSpriteCount;
}
s32 pouchGetHP(void) {
    return ((PouchData*)mpp)->currentHP;
}
s32 pouchGetMaxHP(void) {
    return ((PouchData*)mpp)->maxHP;
}
void pouchSetHP(s16 hp) {
    ((PouchData*)mpp)->currentHP = hp;

    if (((PouchData*)mpp)->currentHP <= ((PouchData*)mpp)->maxHP) {
        return;
    }

    ((PouchData*)mpp)->currentHP = ((PouchData*)mpp)->maxHP;
}
s32 pouchAddHP(s32 amount) {
    ((PouchData*)mpp)->currentHP = ((PouchData*)mpp)->currentHP + (s16)amount;

    if (((PouchData*)mpp)->currentHP < 0) {
        ((PouchData*)mpp)->currentHP = 0;
    }

    if (((PouchData*)mpp)->currentHP > ((PouchData*)mpp)->maxHP) {
        ((PouchData*)mpp)->currentHP = ((PouchData*)mpp)->maxHP;
    }

    return ((PouchData*)mpp)->currentHP;
}
void pouchSetMaxHP(s16 maxHP) {
    ((PouchData*)mpp)->maxHP = maxHP;
    ((PouchData*)mpp)->baseMaxHP = ((PouchData*)mpp)->maxHP;
}
void pouchSetMaxFP(s16 maxFP) {
    ((PouchData*)mpp)->maxFP = maxFP;
    ((PouchData*)mpp)->baseMaxFP = ((PouchData*)mpp)->maxFP;
}
void pouchSetFP(s16 fp) {
    ((PouchData*)mpp)->currentFP = fp;

    if (((PouchData*)mpp)->currentFP <= ((PouchData*)mpp)->maxFP) {
        return;
    }

    ((PouchData*)mpp)->currentFP = ((PouchData*)mpp)->maxFP;
}
s32 pouchGetMaxFP(void) {
    return ((PouchData*)mpp)->maxFP;
}
s32 pouchGetFP(void) {
    return ((PouchData*)mpp)->currentFP;
}
void pouchSetPartyHP(s32 partyId, s16 hp) {
    PouchPartyData* party;
    s32 offset;
    s32 base;

    offset = partyId * 0xE;
    base = mpp;
    party = (PouchPartyData*)(base + offset);
    party->currentHP = hp;

    base = mpp;
    party = (PouchPartyData*)(base + offset);

    if (party->currentHP <= party->currentMaxHP) {
        return;
    }

    party->currentHP = party->currentMaxHP;
}

s32 pouchGetPartyHP(s32 partyId) {
    s32 offset;
    s32 addr;

    offset = partyId * 0xE;
    addr = mpp;
    addr += offset;

    return *(s16*)(addr + 0x6);
}
void pouchSetAP(s16 sp) {
    ((PouchData*)mpp)->currentSP = sp;

    if (((PouchData*)mpp)->currentSP < 0) {
        ((PouchData*)mpp)->currentSP = 0;
    }

    if (((PouchData*)mpp)->currentSP <= ((PouchData*)mpp)->maxSP) {
        return;
    }

    ((PouchData*)mpp)->currentSP = ((PouchData*)mpp)->maxSP;
}

void pouchAddAP(s16 amount) {
    ((PouchData*)mpp)->currentSP = ((PouchData*)mpp)->currentSP + amount;

    if (((PouchData*)mpp)->currentSP < 0) {
        ((PouchData*)mpp)->currentSP = 0;
    }

    if (((PouchData*)mpp)->currentSP <= ((PouchData*)mpp)->maxSP) {
        return;
    }

    ((PouchData*)mpp)->currentSP = ((PouchData*)mpp)->maxSP;
}
s32 pouchGetMaxAP(void) {
    return ((PouchData*)mpp)->maxSP;
}
s32 pouchGetAP(void) {
    return ((PouchData*)mpp)->currentSP;
}
void pouchSetAudienceNum(f32 audienceNum) {
    ((PouchData*)mpp)->lastAudienceCount = audienceNum;
}
f32 pouchGetAudienceNum(void) {
    return ((PouchData*)mpp)->lastAudienceCount;
}
s32 pouchGetPartyAttackLv(s32 partyId) {
    return ((PouchData*)mpp)->partyData[partyId].attackLevel;
}
s32 pouchGetHammerLv(void) {
    s32 level;

    level = 0;

    if (pouchCheckItem(0xB) == 0) {
        goto check_super;
    }

    level = 3;
    goto done;

check_super:
    if (pouchCheckItem(0xA) == 0) {
        goto check_normal;
    }

    level = 2;
    goto done;

check_normal:
    if (pouchCheckItem(0x9) == 0) {
        goto done;
    }

    level = 1;

done:
    ((PouchData*)mpp)->hammerLevel = level;
    return ((PouchData*)mpp)->hammerLevel;
}
s32 pouchGetJumpLv(void) {
    s32 level;

    level = 0;

    if (pouchCheckItem(0x8) == 0) {
        goto check_super;
    }

    level = 3;
    goto done;

check_super:
    if (pouchCheckItem(0x7) == 0) {
        goto check_normal;
    }

    level = 2;
    goto done;

check_normal:
    if (pouchCheckItem(0x6) == 0) {
        goto done;
    }

    level = 1;

done:
    ((PouchData*)mpp)->jumpLevel = level;
    return ((PouchData*)mpp)->jumpLevel;
}
s32 pouchGetStarPoint(void) {
    return ((PouchData*)mpp)->starPoints;
}
u32 pouchEquipCheckBadgeIndex(s32 index) {
    s32 offset;
    s32 badge;
    s32 negBadge;

    offset = index * 2;
    index = mpp + offset;

    badge = *(s16*)(index + 0x38A);
    negBadge = -badge;

    return (u32)(negBadge | badge) >> 31;
}
s32 pouchEquipBadgeIndex(s32 index) {
    ItemType16 badge;

    badge = ((PouchData*)mpp)->mBadges[index];

    if (badge == 0) {
        return 0;
    }

    ((PouchData*)mpp)->mEquippedBadges[index] = badge;
    return 1;
}
s32 pouchUnEquipBadgeIndex(s32 index) {
    s32 offset;
    s32 addr;
    s32 badge;

    offset = index * 2;
    addr = mpp;
    addr += offset;

    badge = *(s16*)(addr + 0x38A);

    if (badge == 0) {
        return 0;
    }

    *(s16*)(addr + 0x38A) = 0;
    return 1;
}
s32 pouchEquipCheckBadge(s32 itemId) {
    PouchData* pouch;
    s32 count;
    s32 i;

    pouch = (PouchData*)mpp;
    count = 0;

    for (i = 0; i != 0xB4; i += 9) {
        if (itemId == pouch->mEquippedBadges[0]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[1]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[2]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[3]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[4]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[5]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[6]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[7]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[8]) {
            count++;
        }
        if (itemId == pouch->mEquippedBadges[9]) {
            count++;
        }

        pouch = (PouchData*)((s32)pouch + 0x14);
    }

    return count;
}
s32 pouchEquipBadgeID(s32 itemId) {
    PouchData* scan;
    PouchData* pouch;
    s32 index;
    s32 count;
    s16 badge;

    pouch = (PouchData*)mpp;
    count = 200;
    index = 0;
    scan = pouch;

    for (; count > 0; count--) {
        if ((s32)scan->mBadges[0] == itemId) {
            badge = pouch->mBadges[index];

            if (badge != 0) {
                pouch->mEquippedBadges[index] = badge;
            }

            return 1;
        }

        scan = (PouchData*)((s32)scan + 2);
        index++;
    }

    return 0;
}
s32 N_pouchUnEquipBadgeID(s32 badgeID) {
    PouchData* scan;
    PouchData* pouch;
    s32 index;
    s32 count;
    s16 badge;

    pouch = (PouchData*)mpp;
    count = 200;
    index = 0;
    scan = pouch;

    for (; count > 0; count--) {
        if ((s32)scan->mEquippedBadges[0] == badgeID) {
            badge = pouch->mEquippedBadges[index];

            if (badge != 0) {
                pouch->mEquippedBadges[index] = 0;
            }

            return 1;
        }

        scan = (PouchData*)((s32)scan + 2);
        index++;
    }

    return 0;
}
s32 pouchGetHaveBadgeCnt(void) {
    PouchData* pouch;
    s32 count;
    s32 loops;

    pouch = (PouchData*)mpp;
    count = 0;
    loops = 0x14;

    for (; loops > 0; loops--) {
        if (pouch->mBadges[0] != 0) {
            count++;
        }
        if (pouch->mBadges[1] != 0) {
            count++;
        }
        if (pouch->mBadges[2] != 0) {
            count++;
        }
        if (pouch->mBadges[3] != 0) {
            count++;
        }
        if (pouch->mBadges[4] != 0) {
            count++;
        }
        if (pouch->mBadges[5] != 0) {
            count++;
        }
        if (pouch->mBadges[6] != 0) {
            count++;
        }
        if (pouch->mBadges[7] != 0) {
            count++;
        }
        if (pouch->mBadges[8] != 0) {
            count++;
        }
        if (pouch->mBadges[9] != 0) {
            count++;
        }

        pouch = (PouchData*)((s32)pouch + 0x14);
    }

    return count;
}
s32 pouchGetEquipBadgeCnt(void) {
    PouchData* pouch;
    s32 count;
    s32 loops;

    pouch = (PouchData*)mpp;
    count = 0;
    loops = 0x14;

    for (; loops > 0; loops--) {
        if (pouch->mEquippedBadges[0] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[1] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[2] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[3] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[4] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[5] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[6] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[7] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[8] != 0) {
            count++;
        }
        if (pouch->mEquippedBadges[9] != 0) {
            count++;
        }

        pouch = (PouchData*)((s32)pouch + 0x14);
    }

    return count;
}
void pouchReviseMarioParam(void) {
    PouchData* pouch;
    s32 offset;
    s32 count;
    s32 item_base;
    s32 item_offset;
    s32 item_addr;
    s16 badge;
    s8 bpCost;
    s32 index;

    pouch = (PouchData*)mpp;
    pouch->maxHP = pouch->baseMaxHP;

    pouch = (PouchData*)mpp;
    pouch->maxFP = pouch->baseMaxFP;

    offset = 0;
index = 0;
count = 100;

for (; count > 0; count--) {
    pouch = (PouchData*)mpp;
    badge = *(s16*)((s32)pouch + offset + 0x38A);

    if (badge == 0) {
        goto badge0_done;
    }
    if (badge == 0x117) {
        goto badge0_done;
    }
    if (badge >= 0x117) {
        goto badge0_ge_117;
    }
    if (badge >= 0x116) {
        goto badge0_add_hp;
    }
    goto badge0_done;

badge0_ge_117:
    if (badge >= 0x119) {
        goto badge0_done;
    }
    goto badge0_add_fp;

badge0_add_hp:
    pouch->maxHP = pouch->maxHP + 5;
    goto badge0_done;

badge0_add_fp:
    pouch->maxFP = pouch->maxFP + 5;

badge0_done:
    pouch = (PouchData*)mpp;
    badge = *(s16*)((s32)pouch + offset + 0x38C);

    if (badge == 0) {
        goto badge1_done;
    }
    if (badge == 0x117) {
        goto badge1_done;
    }
    if (badge >= 0x117) {
        goto badge1_ge_117;
    }
    if (badge >= 0x116) {
        goto badge1_add_hp;
    }
    goto badge1_done;

badge1_ge_117:
    if (badge >= 0x119) {
        goto badge1_done;
    }
    goto badge1_add_fp;

badge1_add_hp:
    pouch->maxHP = pouch->maxHP + 5;
    goto badge1_done;

badge1_add_fp:
    pouch->maxFP = pouch->maxFP + 5;

badge1_done:
    offset += 4;
    index++;
}

    pouch = (PouchData*)mpp;
    if (pouch->currentHP > pouch->maxHP) {
        pouch->currentHP = pouch->maxHP;
    }

    pouch = (PouchData*)mpp;
    if (pouch->currentFP > pouch->maxFP) {
        pouch->currentFP = pouch->maxFP;
    }

    pouch = (PouchData*)mpp;
    pouch->availableBP = pouch->totalBP;

    item_base = (s32)itemDataTable;
    offset = 0;
    count = 0x19;

    for (; count > 0; count--) {
        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x38A);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x38C);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x38E);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x390);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x392);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x394);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x396);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        pouch = (PouchData*)mpp;
        badge = *(s16*)((s32)pouch + offset + 0x398);
        item_offset = badge * 0x28;
        item_addr = item_base + item_offset;
        bpCost = *(s8*)(item_addr + 0x1C);
        pouch->availableBP = pouch->availableBP - bpCost;

        offset += 0x10;
    }
}
void pouchRevisePartyParam(void) {
    PouchData* pouch;
    s32 loops;
    s32 hpPlusPCount;
    s32 pairOffset;
    s32 tableOffset;
    s32 partyCheck;
    s16 hpLevel;
    s16 maxHP;
    u8* table;

    hpPlusPCount = 0;
    loops = 0x14;
    pouch = (PouchData*)mpp;

    for (; loops > 0; loops--) {
        if (pouch->mEquippedBadges[0] == 0x117) {
            hpPlusPCount++;
        }
        if (pouch->mEquippedBadges[1] == 0x117) {
            hpPlusPCount++;
        }
        if (pouch->mEquippedBadges[2] == 0x117) {
            hpPlusPCount++;
        }
        if (pouch->mEquippedBadges[3] == 0x117) {
            hpPlusPCount++;
        }
        if (pouch->mEquippedBadges[4] == 0x117) {
            hpPlusPCount++;
        }
        if (pouch->mEquippedBadges[5] == 0x117) {
            hpPlusPCount++;
        }
        if (pouch->mEquippedBadges[6] == 0x117) {
            hpPlusPCount++;
        }
        if (pouch->mEquippedBadges[7] == 0x117) {
            hpPlusPCount++;
        }
        if (pouch->mEquippedBadges[8] == 0x117) {
            hpPlusPCount++;
        }
        if (pouch->mEquippedBadges[9] == 0x117) {
            hpPlusPCount++;
        }

        pouch = (PouchData*)((s32)pouch + 0x14);
    }

    table = _party_max_hp_table;
    loops = 4;
    partyCheck = 0;
    hpPlusPCount = hpPlusPCount * 5;
    pairOffset = 0;

    for (; loops > 0; loops--) {
        if (partyCheck != 0) {
            pouch = (PouchData*)mpp;

            hpLevel = *(s16*)((s32)pouch + pairOffset + 0x8);
            tableOffset = hpLevel * 2;

            *(s16*)((s32)pouch + pairOffset + 0x4) =
                *(s16*)((s32)table + tableOffset);

            *(s16*)((s32)pouch + pairOffset + 0x2) =
                *(s16*)((s32)pouch + pairOffset + 0x4) + hpPlusPCount;

            maxHP = *(s16*)((s32)pouch + pairOffset + 0x2);

            if (*(s16*)((s32)pouch + pairOffset + 0x6) > maxHP) {
                *(s16*)((s32)pouch + pairOffset + 0x6) = maxHP;
            }
        }

        partyCheck++;
        table += 8;
        pairOffset += 0xE;

        if (partyCheck != 0) {
            pouch = (PouchData*)mpp;

            hpLevel = *(s16*)((s32)pouch + pairOffset + 0x8);
            tableOffset = hpLevel * 2;

            *(s16*)((s32)pouch + pairOffset + 0x4) =
                *(s16*)((s32)table + tableOffset);

            *(s16*)((s32)pouch + pairOffset + 0x2) =
                *(s16*)((s32)pouch + pairOffset + 0x4) + hpPlusPCount;

            maxHP = *(s16*)((s32)pouch + pairOffset + 0x2);

            if (*(s16*)((s32)pouch + pairOffset + 0x6) > maxHP) {
                *(s16*)((s32)pouch + pairOffset + 0x6) = maxHP;
            }
        }

        pairOffset += 0xE;
        table += 8;
        partyCheck++;
    }
}
s32 pouchAddKeepItem(s16 item) {
   s16* scan;
    s32 scanCount;
    u32 index;
    u32 remainder;
    s32 offset;
    u32 chunks;
    s32 addr;

    scan = &((PouchData*)mpp)->partyData[4].currentHP;
    scanCount = 0x20;
    index = 0x1F;
    for (; scanCount > 0; scanCount--) {
        if (scan[0xDD] != 0) {
            goto scan_next;
        }

        offset = index * 2;
        remainder = index;

        if ((s32)index > 0) {
            chunks = index >> 3;

            if (chunks != 0) {
                for (; chunks != 0; chunks--) {
                    addr = (s32)mpp + offset;
                    offset -= 2;
                    *(s16*)(addr + 0x1BA) = *(s16*)(addr + 0x1B8);

                    addr = (s32)mpp + offset;
                    offset -= 2;
                    *(s16*)(addr + 0x1BA) = *(s16*)(addr + 0x1B8);

                    addr = (s32)mpp + offset;
                    offset -= 2;
                    *(s16*)(addr + 0x1BA) = *(s16*)(addr + 0x1B8);

                    addr = (s32)mpp + offset;
                    offset -= 2;
                    *(s16*)(addr + 0x1BA) = *(s16*)(addr + 0x1B8);

                    addr = (s32)mpp + offset;
                    offset -= 2;
                    *(s16*)(addr + 0x1BA) = *(s16*)(addr + 0x1B8);

                    addr = (s32)mpp + offset;
                    offset -= 2;
                    *(s16*)(addr + 0x1BA) = *(s16*)(addr + 0x1B8);

                    addr = (s32)mpp + offset;
                    offset -= 2;
                    *(s16*)(addr + 0x1BA) = *(s16*)(addr + 0x1B8);

                    addr = (s32)mpp + offset;
                    offset -= 2;
                    *(s16*)(addr + 0x1BA) = *(s16*)(addr + 0x1B8);
                }

                remainder &= 7;

                if (remainder == 0) {
                    goto done_shift;
                }
            }

            do {
    addr = (s32)mpp + offset;
    offset -= 2;
    *(s16*)(addr + 0x1BA) = *(s16*)(addr + 0x1B8);
    remainder--;
} while (remainder != 0);
        }

done_shift:
        *(s16*)((s32)mpp + 0x1BA) = item;
        return 1;

scan_next:
        scan--;
        index--;
    }

    return 0;
}
s32 pouchRemoveKeepItem(s32 item, s32 index) {
    s16* scan;
    s32 count;
    s32 offset;
    s32 nextOffset;
    u32 remaining;
    u32 chunks;

    if ((s16)((PouchData*)mpp)->mStoredItems[index] != item) {
        return 0;
    }

    count = 0x20 - index;
    scan = &((PouchData*)mpp)->partyData[0].flags + index;

    if (index < 0x20) {
        do {
            if ((s16)scan[0xDD] == item) {
                offset = index * 2;

                ((PouchData*)mpp)->mStoredItems[index] = 0;

                remaining = 0x1F - index;

                if (index < 0x1F) {
                    chunks = remaining >> 2;

                    if (chunks != 0) {
                        do {
                            *(s16*)((s32)mpp + offset + 0x1BA) =
                                *(s16*)((s32)mpp + offset + 0x1BC);
                            *(s16*)((s32)mpp + offset + 0x1BC) = 0;

                            *(s16*)((s32)mpp + offset + 0x1BC) =
                                *(s16*)((s32)mpp + offset + 0x1BE);
                            *(s16*)((s32)mpp + offset + 0x1BE) = 0;

                            *(s16*)((s32)mpp + offset + 0x1BE) =
                                *(s16*)((s32)mpp + offset + 0x1C0);
                            *(s16*)((s32)mpp + offset + 0x1C0) = 0;

                            nextOffset = offset + 8;

                            *(s16*)((s32)mpp + offset + 0x1C0) =
                                *(s16*)((s32)mpp + offset + 0x1C2);
                            *(s16*)((s32)mpp + offset + 0x1C2) = 0;

                            chunks--;
                            offset = nextOffset;
                        } while (chunks != 0);

                        remaining = remaining & 3;

                        if (remaining == 0) {
                            return 1;
                        }
                    }

                    do {
                        *(s16*)((s32)mpp + offset + 0x1BA) =
                            *(s16*)((s32)mpp + offset + 0x1BC);
                        *(s16*)((s32)mpp + offset + 0x1BC) = 0;

                        offset += 2;
                        remaining--;
                    } while (remaining != 0);
                }

                return 1;
            }

            scan++;
            index++;
            count--;
        } while (count != 0);
    }

    return 0;
}

s16 pouchHaveBadge(s32 index) {
    return *(s16*)(mpp + 0x1FA + index * 2);
}

s16 pouchKeepItem(s32 index) {
    return *(s16*)(mpp + 0x1BA + index * 2);
}

s16 pouchKeyItem(s32 index) {
    return *(s16*)(mpp + 0xA0 + index * 2);
}

s32 pouchGetKpaCoin(void) {
    return *(u8*)(mpp + 0x5BB);
}

s32 pouchGetKpaScore(void) {
    return *(s32*)(mpp + 0x5BC);
}

s32 pouchGetPartyColor(s32 partyId) {
    return *(u16*)(mpp + partyId * 0xE) >> 13;
}

void pouchMajinaiInit(s32 value) {
    if (*(u8*)(mpp + 0x5B8) < value) {
        *(u8*)(mpp + 0x5B8) = value;
        *(s8*)(mpp + 0x5B9) = -1;
        *(u8*)(mpp + 0x5BA) = 0;
    }
}

void pouchSetYoshiName(char* name) {
    extern char* strcpy(char* dest, const char* src);
    strcpy((char*)(mpp + 0x5C0), name);
}

void pouchAddKpaScore(s32 value) {
    *(u32*)(mpp + 0x5BC) += value;
    if (*(u32*)(mpp + 0x5BC) > 999999) {
        *(u32*)(mpp + 0x5BC) = 999999;
    }
}

s32 pouchAddKpaCoin(s32 value) {
    s32 ret = 0;
    *(u8*)(mpp + 0x5BB) += value;
    if (*(u8*)(mpp + 0x5BB) >= 100) {
        *(u8*)(mpp + 0x5BB) -= 100;
        ret = 1;
    }
    return ret;
}

void pouchArriveBadge(s32 badge) {
    extern void* bdsw;
    extern void badgeShop_add(void* work, s16 badge, s32 flag);
    badgeShop_add(bdsw, badge, 1);
}

void pouchOpenMail(s32 mail) {
    s32 word = mail / 32;
    s32 bit = mail % 32;
    *(u32*)(mpp + 0x590 + word * 4) |= 1 << bit;
}

char* pouchGetYoshiName(void) {
    extern u32 strlen(const char* str);
    extern char* msgSearch(const char* msg);
    extern const char str_name_party3_802cc728[];
    if (strlen((char*)(mpp + 0x5C0)) < 1) {
        return msgSearch(str_name_party3_802cc728);
    }
    return (char*)(mpp + 0x5C0);
}


u8 pouchSortItem(u32 sortType) {
    return 0;
}


void pouchInit(void* pouch) {
    ;
}


void pouchGetItem(s32 itemNo) {
    ;
}


s32 pouchRemoveItemIndex(u32 itemType, int itemIndex) {
    return 0;
}


void pouchRemoveItem(s32 item) {
    ;
}


u8 comp_aiueo_r(short* param_1, short* param_2) {
    return 0;
}


u8 comp_aiueo(short* param_1, short* param_2) {
    return 0;
}


void unk_800d48b0(u16* src, u16* dst) {
    extern s32 strlen(const char* str);
    extern char* strcpy(char* dst, const char* src);
    extern void* gp;
    s32 count;
    s32 i;
    u16 value;

    count = strlen((const char*)src) >> 1;
    if (*(u32*)((s32)gp + 0x16C) == 0) {
        for (i = 0; i < count; i++) {
            value = src[i];
            if (value >= 0x829F && value < 0x82DE) {
                value += 0xA1;
            } else if (value >= 0x82DE && value <= 0x82F1) {
                value += 0xA2;
            }
            dst[i] = value;
        }
        *(u8*)((s32)dst + i * 2) = 0;
    } else {
        strcpy((char*)dst, (const char*)src);
    }
}


void pouchReceiveMail(s32 mail) {
    s32 i;
    s32 count;
    s32 base;

    count = 0;
    base = mpp;
    for (i = 0; i < 99; i++) {
        if (*(u32*)(base + 0x580 + (i / 32) * 4) & (1 << (i % 32))) {
            count++;
        }
    }

    *(u32*)(base + 0x580 + (mail / 32) * 4) |= 1 << (mail % 32);
    *(u8*)(mpp + 0x51A + count) = mail;
}


int pouchReceiveMailCount(void) {
    s32 i;
    s32 count;
    s32 base;

    count = 0;
    base = mpp;
    for (i = 0; i < 99; i++) {
        if (*(u32*)(base + 0x580 + (i / 32) * 4) & (1 << (i % 32))) {
            count++;
        }
    }
    return count;
}


int pouchGetEmptyKeepItemCnt(void) {
    s16* items;
    s32 count;
    s32 i;

    count = 0;
    items = (s16*)(mpp + 0x1BA);
    for (i = 0; i < 32; i++) {
        if (items[i] == 0) {
            count++;
        }
    }
    return count;
}


int pouchGetKeepItemCnt(void) {
    s16* items;
    s32 count;
    s32 i;

    count = 0;
    items = (s16*)(mpp + 0x1BA);
    for (i = 0; i < 32; i++) {
        if (items[i] != 0) {
            count++;
        }
    }
    return count;
}


void pouchGetStarStone(s32 id) {
    extern f32 float_2_80421878;

    if (*(u16*)(mpp + 0x8C) == 0) {
        *(f32*)(mpp + 0x84) = float_2_80421878;
    }

    *(u16*)(mpp + 0x8C) |= 1 << id;
    *(s16*)(mpp + 0x7C) = (id + 1) * 100;

    if (*(s16*)(mpp + 0x7C) < 0) {
        *(s16*)(mpp + 0x7C) = 0;
    }

    if (*(s16*)(mpp + 0x7C) > 0x320) {
        *(s16*)(mpp + 0x7C) = 0x320;
    }
}

s32 pouchCheckMail(s32 mail) {
    register s32 word = mail / 32;
    s32 bit = mail % 32;
    u32 mask = 1 << bit;
    u32 value;

    if (*(u32*)(mpp + 0x590 + word * 4) & mask) {
        return 2;
    }
    value = *(u32*)(mpp + 0x580 + word * 4) & mask;
    return ((u32)(-value) | value) >> 31;
}


int comp_kind_r(short* param_1, short* param_2) {
    return 0;
}


int comp_kind(short* param_1, short* param_2) {
    return 0;
}


u8 pouchSetPartyColor(int param_1, int param_2) {
    return 0;
}


s16 L_pouchEquipBadge(int param_1) {
    return *(s16*)(mpp + 0x38A + param_1 * 2);
}
