// Battle information: the data passed in and out of a battle (mode, party, first strike, result, item drops).

#include "version.h"
#include "battle/battle_information.h"
#include "battle/battle.h"
#include "evt/evt_cmd.h"
#include "mario/mario_pouch.h"
#include "system.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_unit.h"

/** Stores the battle mode in the info passed from the field to the battle. */
void BattleInformationSetMode(FieldBattleInfo* info, u32 mode) {
    info->mode = mode;
}

/**
 * Stores which partner (as a battle unit kind) joins Mario in the battle;
 * set by seq_battle from the active party member.
 */
void BattleInformationSetParty(FieldBattleInfo* info, BattleUnitType party) {
    info->party = party;
}

/**
 * Stores how the battle started: seq_battle passes 0 when no first strike happened, else 1, 2, 3, ...
 * from the field hit kind (fbat->hitKind) that began it.
 * The firstAttack field holds this first-strike kind.
 */
void BattleInformationSetFirstAttack(FieldBattleInfo* info, u32 type) {
    info->firstAttack = type;
}

/**
 * Resets the field-to-battle info: mode, partner, first strike and result 0, no setup,
 * no coins and no special rule.
 */
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

/** Links the field NPC's battle setup (enemy group, held items, drop results) to the battle info. */
void BattleInfomationSetBattleSetupInfo(FieldBattleInfo* info, NpcBattleInfo* setup) {
    info->setup = setup;
}

/** Stores the battle result; battle_seq_end sets 1 for a win, 4 for running away, and so on. */
void BattleInformationSetResult(FieldBattleInfo* info, u32 result) {
    info->result = result;
}

/** Returns the battle result read back by battle_seq_end, npcdrv and evt_npc after the fight (1 = won). */
s32 BattleInformationGetResult(FieldBattleInfo* info) {
    return info->result;
}

typedef struct ItemHogTable {
#if VERSION >= VERSION_US
    ItemType items[25];
#else
    ItemType items[20];
#endif
} ItemHogTable;

// Descriptive name (not in the symbol maps).
const ItemHogTable item_hog_table = {
    {ITEM_DRIED_SHROOM, ITEM_DRIED_SHROOM, ITEM_DRIED_SHROOM, ITEM_FRIGHT_MASK, ITEM_MUSHROOM, ITEM_HONEY_SYRUP, ITEM_TASTY_TONIC, ITEM_FIRE_FLOWER, ITEM_VOLT_SHROOM, ITEM_POW_BLOCK}
};

/**
 * Rolls the drops after a won battle into the NPC's battle setup:
 * hearts and flowers from the group's HP and FP drop tables according to Mario's remaining HP and FP percentage,
 * then one item, chosen by weight between none,
 * a random pick from the enemy's drop table and one of the enemy's held items.
 * Item Hog multiplies the item weights (by 2n+2 for n badges in the demos, n+1 later)
 * and adds a pick from item_hog_table: in the demos and JP only when the held-item roll finds nothing to drop,
 * from US on whenever no item dropped. From JP on, before Frankly's house the normal held/random item roll
 * is disabled, but hearts and flowers are not.
 */
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
#if VERSION <= VERSION_DEMO_JP
    n = pouchEquipCheckBadge(ITEM_ITEM_HOG);
    if (n >= 1) {
        held *= n * 2 + 2;
        random *= n * 2 + 2;
#else
    if (evtGetValue(NULL, GSW_STORY_PROGRESS) <= STG0_16_FRANKLYS_HOUSE) {
        random = 0;
        held = 0;
        none = 100;
    }
    n = pouchEquipCheckBadge(ITEM_ITEM_HOG);
    if (n >= 1) {
        held *= n + 1;
        random *= n + 1;
#endif
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
#if VERSION < VERSION_US
            else if (pouchEquipCheckBadge(ITEM_ITEM_HOG) >= 1) {
                hog = item_hog_table;
                setup->itemDropped = hog.items[irand(20)];
            }
#endif
            break;
    }
#if VERSION >= VERSION_US
    if (setup->itemDropped == ITEM_NULL && pouchEquipCheckBadge(ITEM_ITEM_HOG) >= 1) {
        hog = item_hog_table;
        setup->itemDropped = hog.items[irand(25)];
    }
#endif
}
