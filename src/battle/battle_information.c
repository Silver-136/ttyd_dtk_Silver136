#include "battle/battle_information.h"
#include "battle/battle.h"
#include "evt/evt_cmd.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_unit.h"

void BattleInformationSetMode(FieldBattleInfo* info, u32 mode) {
    info->mode = mode;
}

void BattleInformationSetParty(FieldBattleInfo* info, BattleUnitType party) {
    info->party = party;
}

void BattleInformationSetFirstAttack(FieldBattleInfo* info, u32 type) {
    info->firstAttack = type;
}

void BattleInformationInit(FieldBattleInfo* info) {
    BattleInformationSetMode(info, 0);
    BattleInformationSetParty(info, UNIT_NULL);
    BattleInformationSetFirstAttack(info, FIRST_ATTACK_NONE);
    BattleInformationSetResult(info, BATTLE_RESULT_NONE);
    info->setup = NULL;
    info->coins = 0;
    info->ruleType = 0;
    info->ruleKeepResult = 0;
    info->ruleParam = 0;
    info->ruleParam2 = 0;
}

void BattleInfomationSetBattleSetupInfo(FieldBattleInfo* info, NpcBattleInfo* setup) {
    info->setup = setup;
}

void BattleInformationSetResult(FieldBattleInfo* info, u32 result) {
    info->result = result;
}

s32 BattleInformationGetResult(FieldBattleInfo* info) {
    return info->result;
}

typedef struct ItemHogTable {
#ifdef VERSION_G8ME01
    ItemType items[25];
#else
    ItemType items[20];
#endif
} ItemHogTable;

// Descriptive name (not in the symbol maps).
const ItemHogTable item_hog_table = {
    {0x98, 0x98, 0x98, 0xA2, 0x94, 0x9A, 0x99, 0x84, 0x87, 0x8B}
};

void BattleInformationSetDropMaterial(FieldBattleInfo* info) {
    NpcBattleInfo* setup = info->setup;
    BattleGroupSetup* group = setup->groupSetup;
    PointDropData* hp = group->hpDropTable;
    PointDropData* fp = group->fpDropTable;
    BattleUnit* mario = BattleGetMarioPtr(_battleWorkPointer);
    s32 pct;
    s32 n, n2;
    s32 held, random, none;
    s32 type;
    s32 r;
    s32 sum;
    s32 i;
    ItemDropData* drop;
    ItemDropData* p;
    ItemType item;
    ItemHogTable hog;

    pct = mario->currentHp * 100 / mario->maxHp;
    if (pct > 100) {
        pct = 100;
    }
    setup->heartsDropped = 0;
    for (; hp->maxStatPercent; hp++) {
        if (pct <= hp->maxStatPercent) {
            r = irand(100);
            n = hp->numDrops;
            if (r < hp->overallDropChance) {
                for (; n != 0; n--) {
                    if (irand(100) < hp->dropChance) {
                        setup->heartsDropped++;
                    }
                }
            }
            break;
        }
    }

    pct = mario->currentFp * 100 / mario->maxFp;
    if (pct > 100) {
        pct = 100;
    }
    setup->flowersDropped = 0;
    for (; fp->maxStatPercent; fp++) {
        if (pct <= fp->maxStatPercent) {
            r = irand(100);
            n2 = fp->numDrops;
            if (r < fp->overallDropChance) {
                for (; n2 != 0; n2--) {
                    if (irand(100) < fp->dropChance) {
                        setup->flowersDropped++;
                    }
                }
            }
            break;
        }
    }

    held = group->heldItemWeight;
    random = group->randomItemWeight;
    none = group->noItemWeight;
    type = 0;
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) <= STG0_16_FRANKLYS_HOUSE) {
        random = 0;
        held = 0;
        none = 100;
    }
    n = pouchEquipCheckBadge(ITEM_ITEM_HOG);
    if (n >= 1) {
        held *= n + 1;
        random *= n + 1;
    }
    r = irand(random + held + none);
    if ((r -= random) < 0) {
        type = 1;
    } else if ((r -= held) < 0) {
        type = 2;
    }
    setup->itemDropped = ITEM_NULL;
    switch (type) {
        case 0:
            break;
        case 1:
            drop = group->enemyData->itemDropTable;
            p = drop;
            if (drop) {
                sum = 0;
                for (; p->dropWeight > 0 || p->type != 0; p++) {
                    sum += p->dropWeight;
                }
                if (sum > 0) {
                    r = irand(sum);
                    for (;; drop++) {
                        if ((r -= drop->dropWeight) < 0) {
                            setup->itemDropped = drop->type;
                            break;
                        }
                    }
                }
            }
            break;
        case 2:
            for (n = i = 0; i < 8; i++) {
                if (setup->heldItems[i]) {
                    n++;
                }
            }
            if (n > 0) {
                r = irand(n);
                for (i = 0; i < 8; i++) {
                    item = setup->heldItems[i];
                    if (item && --r < 0) {
                        setup->itemDropped = item;
                        break;
                    }
                }
                if (i >= 8) {
                    setup->itemDropped = ITEM_NULL;
                }
            }
#ifndef VERSION_G8ME01
            else if (pouchEquipCheckBadge(ITEM_ITEM_HOG) >= 1) {
                hog = item_hog_table;
                setup->itemDropped = hog.items[irand(20)];
            }
#endif
            break;
    }
#ifdef VERSION_G8ME01
    if (setup->itemDropped == ITEM_NULL && pouchEquipCheckBadge(ITEM_ITEM_HOG) >= 1) {
        hog = item_hog_table;
        setup->itemDropped = hog.items[irand(25)];
    }
#endif
}
