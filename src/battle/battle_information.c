#include "battle/battle_information.h"
#include "mario/mario_pouch.h"

void BattleInformationSetMode(void* info, s32 mode) {
    *(s32*)((s32)info + 0x0) = mode;
}

void BattleInformationSetParty(void* info, s32 party) {
    *(s32*)((s32)info + 0x4) = party;
}

void BattleInformationSetFirstAttack(void* info, s32 firstAttack) {
    *(s32*)((s32)info + 0x8) = firstAttack;
}

void BattleInformationInit(void* info) {
    BattleInformationSetMode(info, 0);
    BattleInformationSetParty(info, 0);
    BattleInformationSetFirstAttack(info, 0);
    BattleInformationSetResult(info, 0);

    *(s32*)((s32)info + 0xC) = 0;
    *(s32*)((s32)info + 0x14) = 0;
    *(u8*)((s32)info + 0x18) = 0;
    *(u8*)((s32)info + 0x19) = 0;
    *(u8*)((s32)info + 0x1A) = 0;
    *(u8*)((s32)info + 0x1B) = 0;
}

void BattleInfomationSetBattleSetupInfo(void* info, s32 battleSetupInfo) {
    *(s32*)((s32)info + 0xC) = battleSetupInfo;
}

void BattleInformationSetResult(void* info, s32 result) {
    *(s32*)((s32)info + 0x10) = result;
}
s32 BattleInformationGetResult(void* info) {
    return *(s32*)((s32)info + 0x10);
}


void BattleInformationSetDropMaterial(void* fbatBattleInfo) {
    extern void* BattleGetMarioPtr(void* battleWork);
    extern s32 evtGetValue(void* event, s32 value);
    extern s32 irand(s32 max);
    extern s32 pouchEquipCheckBadge(s32 badgeId);
    extern const s32 N_item_hog_table[];
    extern void* _battleWorkPointer;
    void* info = *(void**)((s32)fbatBattleInfo + 0xC);
    void* setup = *(void**)((s32)info + 0x18);
    s32* hpTable = *(s32**)((s32)setup + 0x14);
    s32* fpTable = *(s32**)((s32)setup + 0x18);
    void* mario = BattleGetMarioPtr(_battleWorkPointer);
    s32 percent;
    s32 count;
    s32 itemWeight;
    s32 coinWeight;
    s32 noDropWeight;
    s32 dropKind;
    s32 roll;
    s32 i;
    s32 threshold;
    typedef struct DropItems { u8 prefix[0x1C]; s32 items[8]; } DropItems;
    typedef struct HogItems { s32 items[25]; } HogItems;

    percent = (*(s16*)((s32)mario + 0x10C) * 100) / *(s16*)((s32)mario + 0x108);
    if (percent > 100) {
        percent = 100;
    }
    *(s32*)((s32)info + 0x10) = 0;
    while ((threshold = hpTable[0]) != 0) {
        if (percent <= threshold) {
            roll = irand(100);
            count = hpTable[2];
            if (roll < hpTable[1]) {
                while (count != 0) {
                    if (irand(100) < hpTable[3]) {
                        *(s32*)((s32)info + 0x10) = *(s32*)((s32)info + 0x10) + 1;
                    }
                    count--;
                }
            }
            break;
        }
        hpTable += 4;
    }

    percent = (*(s16*)((s32)mario + 0x112) * 100) / *(s16*)((s32)mario + 0x10E);
    if (percent > 100) {
        percent = 100;
    }
    *(s32*)((s32)info + 0x14) = 0;
    while ((threshold = fpTable[0]) != 0) {
        if (percent <= threshold) {
            roll = irand(100);
            count = fpTable[2];
            if (roll < fpTable[1]) {
                while (count != 0) {
                    if (irand(100) < fpTable[3]) {
                        *(s32*)((s32)info + 0x14) = *(s32*)((s32)info + 0x14) + 1;
                    }
                    count--;
                }
            }
            break;
        }
        fpTable += 4;
    }

    itemWeight = *(s32*)((s32)setup + 0x8);
    coinWeight = *(s32*)((s32)setup + 0xC);
    noDropWeight = *(s32*)((s32)setup + 0x10);
    dropKind = 0;
    if (evtGetValue(NULL, -170000000) <= 0x13) {
        coinWeight = 0;
        itemWeight = 0;
        noDropWeight = 100;
    }
    count = pouchEquipCheckBadge(0x13A);
    if (count >= 1) {
        itemWeight *= count + 1;
        coinWeight *= count + 1;
    }

    roll = irand(coinWeight + (itemWeight + noDropWeight));
    if ((roll -= coinWeight) < 0) {
        dropKind = 1;
    } else if ((roll -= itemWeight) < 0) {
        dropKind = 2;
    }

    *(s32*)((s32)info + 0xC) = 0;
    switch (dropKind) {
    case 1: {
        s32* table = *(s32**)((s32)*(void**)((s32)setup + 0x4) + 0x2C);
        if (table != 0) {
            s32 total = 0;
            s32* iter = table;
            while (*(s16*)((s32)iter + 6) > 0 || iter[0] != 0) {
                total += *(s16*)((s32)iter + 6);
                iter += 2;
            }
            if (total > 0) {
                roll = irand(total);
                while (1) {
                    roll -= *(s16*)((s32)table + 6);
                    if (roll < 0) {
                        *(s32*)((s32)info + 0xC) = table[0];
                        break;
                    }
                    table += 2;
                }
            }
        }
        break;
    }
    case 2: {
        count = 0;
        for (i = 0; i < 8; i++) {
            if (((DropItems*)info)->items[i] != 0) {
                count++;
            }
        }
        if (count > 0) {
            roll = irand(count);
            for (i = 0; i < 8; i++) {
                s32 item = ((DropItems*)info)->items[i];
                if (item != 0) {
                    roll--;
                    if (roll < 0) {
                        *(s32*)((s32)info + 0xC) = item;
                        break;
                    }
                }
            }
            if (i >= 8) {
                *(s32*)((s32)info + 0xC) = 0;
            }
        }
    }

    }

    if (*(s32*)((s32)info + 0xC) == 0 && pouchEquipCheckBadge(0x13A) >= 1) {
        HogItems hogItems = *(const HogItems*)N_item_hog_table;
        *(s32*)((s32)info + 0xC) = hogItems.items[irand(25)];
    }
}

const s32 N_item_hog_table[25] = {
    ITEM_DRIED_SHROOM,
    ITEM_DRIED_SHROOM,
    ITEM_DRIED_SHROOM,
    ITEM_FRIGHT_MASK,
    ITEM_MUSHROOM,
    ITEM_HONEY_SYRUP,
    ITEM_TASTY_TONIC,
    ITEM_FIRE_FLOWER,
    ITEM_VOLT_SHROOM,
    ITEM_POW_BLOCK,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
    ITEM_NULL,
};
