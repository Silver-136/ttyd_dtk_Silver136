#include "battle/battle_information.h"

void BattleInformationSetResult(void* info, s32 result);

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
    extern s32 N_item_hog_table[];
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

    percent = (*(s16*)((s32)mario + 0x10C) * 100) / *(s16*)((s32)mario + 0x108);
    if (percent > 100) {
        percent = 100;
    }
    *(s32*)((s32)info + 0x10) = 0;
    while (hpTable[0] != 0) {
        if (percent <= hpTable[0]) {
            if (irand(100) < hpTable[1]) {
                count = hpTable[2];
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
    while (fpTable[0] != 0) {
        if (percent <= fpTable[0]) {
            if (irand(100) < fpTable[1]) {
                count = fpTable[2];
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
        itemWeight = 0;
        coinWeight = 0;
        noDropWeight = 100;
    }
    count = pouchEquipCheckBadge(0x13A);
    if (count >= 1) {
        itemWeight *= count + 1;
        coinWeight *= count + 1;
    }

    roll = irand(itemWeight + coinWeight + noDropWeight);
    if (roll < coinWeight) {
        dropKind = 1;
    } else if (roll - coinWeight < itemWeight) {
        dropKind = 2;
    }

    *(s32*)((s32)info + 0xC) = 0;
    if (dropKind == 1) {
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
    } else if (dropKind == 2) {
        count = 0;
        for (i = 0; i < 8; i++) {
            if (*(s32*)((s32)info + 0x1C + i * 4) != 0) {
                count++;
            }
        }
        if (count > 0) {
            roll = irand(count);
            for (i = 0; i < 8; i++) {
                s32 item = *(s32*)((s32)info + 0x1C + i * 4);
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

    if (*(s32*)((s32)info + 0xC) == 0 && pouchEquipCheckBadge(0x13A) >= 1) {
        s32 hogItems[25];
        for (i = 0; i < 25; i++) {
            hogItems[i] = N_item_hog_table[i];
        }
        *(s32*)((s32)info + 0xC) = hogItems[irand(25)];
    }
}
