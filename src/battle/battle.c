// Top-level battle entry: runs the battle main loop, checks for the end of the sequence and handles reset.

#include "version.h"
#include "battle/battle.h"
#include "battle/battle_ac_help.h"
#include "battle/battle_break_slot.h"
#include "battle/battle_disp.h"
#include "battle/battle_icon.h"
#include "battle/battle_information.h"
#include "battle/battle_menu_disp.h"
#include "battle/battle_seq.h"
#include "battle/battle_unit.h"
#include "mario/mario_pouch.h"
#include "memory.h"
#include <string.h>
#include "mario/mariost.h"
#include "system.h"
#include "manager/evtmgr.h"
#include "unit/unit_mario.h"
#include "unit/unit_system.h"
#include "driver/animdrv.h"
#include "statuswindow.h"
#include "battle/battle_stage.h"

// Separate capacities: unit-slot indexes versus the after-reaction queue.
enum { BATTLE_UNIT_SLOT_COUNT = 64, BATTLE_AFTER_REACTION_COUNT = 64 };

BattleWork* _battleWorkPointer;

/**
 * Battle entries Btl_UnitSetup makes at the start of every fight: Mario at his home spot (-100, 0, -5),
 * and before him the invisible system unit (unit_system) that runs battle-wide events.
 */
BattleUnitSetup entryunit_mario = { &unitdata_Mario, 0, {0}, BATTLE_PHASE_PLAYER, {-100.0f, 0.0f, -5.0f} };
BattleUnitSetup entryunit_system = { &unit_system, 0, {0}, BATTLE_PHASE_PLAYER };

// forward declarations
BOOL BattleMain(void);
void _EquipItem(BattleUnit* unit, EquipFlags flags, ItemType type);
void BattlePartyInfoWorkInit(BattleWork* wp);
void BattleCheckUnitBroken(BattleWork* wp);

/**
 * Per-frame battle entry called by the battle game sequence;
 * flags BATTLE_SEQ_END once BattleMain reports the battle is over.
 */
void battleMain(void) {
    BattleWork* wp = _battleWorkPointer;

    if (wp && BattleMain() == 0) {
        wp->flags |= BATTLE_SEQ_END;
    }
}

/**
 * Returns TRUE once the battle has finished (BATTLE_SEQ_END, the top flag bit),
 * so the sequence can return to the field.
 */
BOOL battleSeqEndCheck(void) {
    return (_battleWorkPointer->flags & BATTLE_SEQ_END) != 0;
}

/**
 * Starts a battle from the field's FieldBattleInfo:
 * allocates and clears BattleWork (battle allocator; JP uses the map allocator),
 * records the info and initialises the battle subsystems (pad, action-command help, icons, action commands,
 * after-reaction queue, display, units, partner info, stage, audience, Bingo).
 */
void BattleInit(FieldBattleInfo* info) {
    BattleWork* wp;

#if VERSION >= VERSION_US
    wp = _battleWorkPointer = (BattleWork*)BattleAlloc(sizeof(BattleWork));
#else
    wp = _battleWorkPointer = (BattleWork*)_mapAlloc(sizeof(BattleWork));
#endif
    memset(_battleWorkPointer, 0, sizeof(BattleWork));
    wp->curtainSfxId = -1;
    wp->lectureUnitId = -1;
    wp->fbatInfo = info;
    BattlePadInit();
    BattleAcHelpInit();
    BattleIconInit();
    BattleActionCommandManagerInit(wp);
    BattleAfterReactionQueueInit();
    BattleStatusChangeMsgWorkInit();
    btlDispInit();
    BattleSetSeq(wp, SEQ_UNKNOWN, 0);
    BattleSetSeq(wp, SEQ_INIT, 0x1000000);
    wp->seqEndWork = NULL;
    wp->commandMenu.flags = 0;
    wp->commandMenu.cursor[0].absPos = 0;
    wp->commandMenu.cursor[0].relPos = 0;
    wp->commandMenu.cursor[1].absPos = 0;
    wp->commandMenu.cursor[1].relPos = 0;
    wp->commandMenu.cursor[2].absPos = 0;
    wp->commandMenu.cursor[2].relPos = 0;
    wp->commandMenu.cursor[3].absPos = 0;
    wp->commandMenu.cursor[3].relPos = 0;
    wp->commandMenu.cursor[4].absPos = 0;
    wp->commandMenu.cursor[4].relPos = 0;
    wp->commandMenu.cursor[5].absPos = 0;
    wp->commandMenu.cursor[5].relPos = 0;
    wp->commandMenu.cursor[6].absPos = 0;
    wp->commandMenu.cursor[6].relPos = 0;
    wp->commandMenu.cursor[7].absPos = 0;
    wp->commandMenu.cursor[7].relPos = 0;
    wp->commandMenu.cursor[8].absPos = 0;
    wp->commandMenu.cursor[8].relPos = 0;
    wp->commandMenu.cursor[9].absPos = 0;
    wp->commandMenu.cursor[9].relPos = 0;
    wp->commandMenu.subMenu = 0;
    wp->turnCount = 0;
    memset(wp->reserveItems, 0, sizeof(wp->reserveItems));
    wp->flags = 0;
    BtlUnit_Init();
    BattlePartyInfoWorkInit(wp);
    BattleStageInit();
    BattleAudience_Init();
    BattleBreakSlot_Init();
}

BOOL BattleMain(void) {
    BattleWork* wp = _battleWorkPointer;

    BattleCheckUnitBroken(wp);
    BattlePadManager();
    BattleSequenceManager();
    BattleActionCommandManager(wp);
    BattleStageObjectMain();
    BattleIconMain();
    BattleStageMain();
    BattleAcHelpMain();
    BattleAudience_Main();
    BattleBreakSlot_Main();
    btlDispMain();

    if (BattleGetSeq(wp, SEQ_UNKNOWN) == 4) {
        return FALSE;
    }

    if ((gp->flags & GW_FLAG_E3) && (gp->flags & GW_FLAG_E3_TIMEOUT) && BattleGetSeq(wp, SEQ_UNKNOWN) == 2) {
        wp->flags |= (BATTLE_FLAG_INTERRUPT_STOP | BATTLE_FLAG_DEMO_TIMEOUT);
        BattleSetSeq(wp, SEQ_UNKNOWN, 3);
        BattleSetSeq(wp, SEQ_END, 0x7000000);
        return TRUE;
    }

    return TRUE;
}

/**
 * Tears a battle down: deletes every unit, ends the icon, menu, stage, audience and Bingo subsystems,
 * frees the command submenu and BattleWork, and clears _battleWorkPointer.
 */
void BattleEnd(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    int i;
    void* subMenu;

    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            BtlUnit_Delete(unit);
        }
    }
    BattleIconEnd();
    battleMenuDispEnd();
    BattleStageEnd();
    BattleAudience_End();
    BattleBreakSlot_End();
    subMenu = wp->commandMenu.subMenu;
    if (subMenu) {
        BattleFree(subMenu);
    }
#if VERSION >= VERSION_US
    BattleFree(wp);
#else
    _mapFree(wp);
#endif
    _battleWorkPointer = NULL;
}

extern BattleUnitSetup entryunit_party[7];

/**
 * Spawns the battle's units: the system unit, Mario and his current partner with their stats from the pouch
 * and the equipped badges, then each enemy of the selected group with its held item (an item it stole in the
 * field takes priority) and its drop table.
 */
BOOL Btl_UnitSetup(BattleWork* wp) {
    int i;
    BattleUnitSetup* enemyData;
    NpcBattleInfo* setup;
    BattleGroupSetup* groupSetup;
    PouchData* pouch;
    BattleUnit* system;
    BattleUnit* mario;
    BattleUnitType partyId;
    BattleUnit* party;
    BattleUnit* enemy;
    ItemType heldItem;
    int j;
    f32 x, y, z;

    setup = wp->fbatInfo->setup;
    groupSetup = setup->groupSetup;
    pouch = pouchGetPtr();
    wp->alliances[0].allianceId = 2;
    wp->alliances[1].allianceId = 1;
    wp->alliances[2].allianceId = 0;
    wp->alliances[0].attackDirection = 1;
    wp->alliances[1].attackDirection = -1;
    wp->alliances[2].attackDirection = 1;
    wp->alliances[0].clearConditionMet = 0;
    wp->alliances[1].clearConditionMet = 0;
    wp->alliances[2].clearConditionMet = 0;

    system = BtlUnit_Entry(&entryunit_system);
    BtlUnit_EquipItem(system, EQUIP_MARIO | EQUIP_RESET, ITEM_NULL);
    mario = BtlUnit_Entry(&entryunit_mario);
    BtlUnit_EquipItem(mario, EQUIP_MARIO | EQUIP_RESET, ITEM_NULL);
    BtlUnit_SetParamFromPouch(mario);

    partyId = wp->fbatInfo->party;
    if (partyId) {
        party = BtlUnit_Entry(&entryunit_party[partyId - TYPE_PARTNER_MIN]);
        BtlUnit_GetHomePos(party, &x, &y, &z);
        BtlUnit_SetHomePos(party, -140.0, y, 5.0);
        BtlUnit_SetPos(party, -140.0, y, 5.0);
        BtlUnit_EquipItem(party, EQUIP_PARTNER | EQUIP_RESET, ITEM_NULL);
        BtlUnit_SetParamFromPouch(party);
    }

    enemyData = groupSetup->enemyData;
    for (i = 0; i < groupSetup->numEnemies; enemyData++, i++) {
        enemy = BtlUnit_Entry(enemyData);
        enemy->groupSetupId = i;
        enemy->heldItem = ITEM_NULL;
        heldItem = setup->stolenItems[enemy->groupSetupId];
        if (heldItem) {
            enemy->heldItem = heldItem;
            enemy->flags |= BATTLE_UNIT_FLAG_STOLEN_ITEM;
        } else {
            enemy->heldItem = setup->heldItems[i];
            BtlUnit_EquipItem(enemy, EQUIP_RESET, setup->heldItems[i]);
        }
        enemy->dropTable = setup->itemDropLists[i];
        enemy->currentHp = enemy->maxHp;
        enemy->currentFp = enemy->maxFp;
    }

    wp->activeUnitId = -1;
    wp->badgeEquippedFlags = 0;
    for (j = 0; j < MAX_BADGES; j++) {
        switch (pouch->equippedBadges[j]) {
            case ITEM_QUICK_CHANGE:
                wp->badgeEquippedFlags |= BATTLE_BADGE_QUICK_CHANGE;
                break;
            case ITEM_PEEKABOO:
                wp->badgeEquippedFlags |= BATTLE_BADGE_PEEKABOO;
                break;
            case ITEM_TIMING_TUTOR:
                wp->badgeEquippedFlags |= BATTLE_BADGE_TIMING_TUTOR;
                break;
            case ITEM_HEART_FINDER:
                wp->badgeEquippedFlags |= BATTLE_BADGE_HEART_FINDER;
                break;
            case ITEM_FLOWER_FINDER:
                wp->badgeEquippedFlags |= BATTLE_BADGE_FLOWER_FINDER;
                break;
            case ITEM_MONEY_MONEY:
                wp->badgeEquippedFlags |= BATTLE_BADGE_MONEY_MONEY;
                break;
            case ITEM_ATTACK_FX_R:
                wp->badgeEquippedFlags |= BATTLE_BADGE_ATTACK_FX_R;
                break;
            case ITEM_ATTACK_FX_B:
                wp->badgeEquippedFlags |= BATTLE_BADGE_ATTACK_FX_B;
                break;
            case ITEM_ATTACK_FX_G:
                wp->badgeEquippedFlags |= BATTLE_BADGE_ATTACK_FX_G;
                break;
            case ITEM_ATTACK_FX_Y:
                wp->badgeEquippedFlags |= BATTLE_BADGE_ATTACK_FX_Y;
                break;
            case ITEM_ATTACK_FX_P:
                wp->badgeEquippedFlags |= BATTLE_BADGE_ATTACK_FX_P;
                break;
            default:
                break;
        }
    }

    return TRUE;
}

/** Writes Mario's and the partner's battle stats (HP, FP, ...) back to the pouch when the battle ends. */
void BattleSetMarioParamToFieldBattle(BattleWork* wp) {
    BattleUnit* mario;
    BattleUnit* party;

    mario = BattleGetMarioPtr(wp);
    BtlUnit_SetParamToPouch(mario);
    party = BattleGetPartyPtr(wp);
    if (party) {
        BtlUnit_SetParamToPouch(party);
    }
}

/** Sets the counter of one battle sequence level (init, first act, turn, phase, move, act, end, ...) to value. */
void BattleSetSeq(BattleWork* wp, BattleSequence seq, u32 value) {
    switch (seq) {
        case SEQ_UNKNOWN:
            wp->seqUnknownValue = value;
            break;
        case SEQ_INIT:
            wp->seqInitValue = value;
            break;
        case SEQ_FIRST_ACT:
            wp->seqFirstActValue = value;
            break;
        case SEQ_TURN:
            wp->seqTurnValue = value;
            break;
        case SEQ_PHASE:
            wp->seqPhaseValue = value;
            break;
        case SEQ_MOVE:
            wp->seqMoveValue = value;
            break;
        case SEQ_ACT:
            wp->seqActValue = value;
            break;
        case SEQ_END:
            wp->seqEndValue = value;
            break;
    }
}

/** Returns the counter of one battle sequence level. */
s32 BattleGetSeq(BattleWork* wp, BattleSequence seq) {
    switch (seq) {
        case SEQ_UNKNOWN:
            return wp->seqUnknownValue;
        case SEQ_INIT:
            return wp->seqInitValue;
        case SEQ_FIRST_ACT:
            return wp->seqFirstActValue;
        case SEQ_TURN:
            return wp->seqTurnValue;
        case SEQ_PHASE:
            return wp->seqPhaseValue;
        case SEQ_MOVE:
            return wp->seqMoveValue;
        case SEQ_ACT:
            return wp->seqActValue;
        case SEQ_END:
            return wp->seqEndValue;
        default:
            return 0;
    }
}

/** Advances one battle sequence level's counter by one. */
void BattleIncSeq(BattleWork* wp, BattleSequence seq) {
    s32 value = BattleGetSeq(wp, seq);
    BattleSetSeq(wp, seq, value + 1);
}

void* BattleAlloc(u32 size) {
#if VERSION >= VERSION_US
    return (_mapAlloc)(R_battlemapalloc_base_ptr, size);
#else
    return __memAlloc(HEAP_BATTLE, size);
#endif
}

/** Frees memory from BattleAlloc; NULL is ignored. */
void BattleFree(void* ptr) {
    if (ptr) {
#if VERSION >= VERSION_US
        (_mapFree)(R_battlemapalloc_base_ptr, ptr);
#else
        __memFree(HEAP_BATTLE, ptr);
#endif
    }
}

/** Returns the unit in battle slot index, or NULL for negative or out-of-range slots. */
BattleUnit* BattleGetUnitPtr(BattleWork* wp, s32 index) {
    if (index == -1) {
        return NULL;
    }
    if (index < -1) {
        return NULL;
    }
    if (index < BATTLE_UNIT_SLOT_COUNT) {
        return wp->units[index];
    }

    return NULL;
}

/** Stores unit in battle slot index (ignored past BATTLE_UNIT_MAX). */
void BattleSetUnitPtr(BattleWork* wp, s32 index, BattleUnit* unit) {
    if (index < BATTLE_UNIT_SLOT_COUNT) {
        wp->units[index] = unit;
    }
}

/** Returns part partNum of the unit in slot index. */
BattleUnitPart* BattleGetUnitPartsPtr(s32 index, s32 partNum) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, index);
    return BtlUnit_GetPartsPtr(unit, partNum);
}

/** Returns the invisible system unit that owns battle-wide scripts (stage hazards, audience items), or NULL. */
BattleUnit* BattleGetSystemPtr(BattleWork* wp) {
    s32 i;
    BattleUnit* unit;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->currentKind == UNIT_SYSTEM) {
            return unit;
        }
    }
    return NULL;
}

/** Returns Mario's unit on the player side, or NULL. */
BattleUnit* BattleGetMarioPtr(BattleWork* wp) {
    s32 i;
    BattleUnit* unit;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->currentKind == UNIT_MARIO && unit->alliance == ALLIANCE_PLAYER) {
            return unit;
        }
    }
    return NULL;
}

/** Returns the active partner's unit on the player side, or NULL when Mario fights alone. */
BattleUnit* BattleGetPartyPtr(BattleWork* wp) {
    s32 i;
    BattleUnit* unit;
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && !(unit->flags & BATTLE_UNIT_FLAG_SWAPPED_OUT) && unit->currentKind >= TYPE_PARTNER_MIN &&
            unit->currentKind < TYPE_PARTNER_MAX && unit->alliance == ALLIANCE_PLAYER) {
            return unit;
        }
    }
    return NULL;
}

/** Returns the other member of the player's pair: the partner for Mario, Mario for the partner. */
BattleUnit* BattleGetPartnerPtr(BattleWork* wp, BattleUnit* unit) {
    if (unit->currentKind == UNIT_MARIO) {
        return BattleGetPartyPtr(wp);
    }
    return BattleGetMarioPtr(wp);
}

/**
 * Starts loading a partner's animation data in the background (partyId is the unit kind 0xE0-0xE6)
 * so a partner switch is quick.
 */
BOOL BattlePartyAnimeLoad(s32 partyId) {
    BattleUnitSetup* setup = &entryunit_party[partyId - TYPE_PARTNER_MIN];

    return animEffectAsync(setup->unitKindParams->parts->modelName, 1);
}

/**
 * Swaps in the partner picked in the command menu (commandMenu.partyKind):
 * spawns the new partner unit, gives it the old one's place in the turn order and position,
 * and returns it (NULL for an invalid choice).
 */
BattleUnit* BattleChangeParty(BattleWork* wp) {
    s32 kind = wp->commandMenu.partyKind;
    BattleUnit* party = BattleGetPartyPtr(wp);
    s32 oldId = party->unitId;
    BattleUnit* unit;
    s32 i;
    f32 dummy, x, y, z;

    if (kind < TYPE_PARTNER_MIN || kind >= TYPE_PARTNER_MAX) {
        return NULL;
    }
    unit = BtlUnit_Spawn(&entryunit_party[kind - TYPE_PARTNER_MIN], 1);
    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        s32 id = wp->movePriorityQueue[i];
        if (id != -1) {
            if (id == oldId) {
                wp->movePriorityQueue[i] = unit->unitId;
            } else if (id == unit->unitId) {
                wp->movePriorityQueue[i] = oldId;
            }
        }
    }
    BtlUnit_GetHomePos(unit, &x, &y, &z);
    BtlUnit_GetHomePos(party, &x, &dummy, &z);
    BtlUnit_EquipItem(unit, EQUIP_RESET | EQUIP_PARTNER, ITEM_NULL);
    y += BattleGetFloorHeight(_battleWorkPointer, x, y, z);
    BtlUnit_SetHomePos(unit, x, y, z);
    if (wp->activeUnitId == oldId) {
        wp->activeUnitId = unit->unitId;
    }
    unit->moveState = party->moveState;
    unit->movesRemaining = party->movesRemaining;
    unit->maxMovesThisTurn = party->maxMovesThisTurn;
    unit->activeTurns = party->activeTurns;
    BtlUnit_SetParamToPouch(party);
    party->flags |= BATTLE_UNIT_FLAG_SWAPPED_OUT;
    unit->moveColorLv = party->moveColorLv;
    unit->moveColorLvDisp = party->moveColorLvDisp;
    return unit;
}

/**
 * Converts a partner unit kind (battle, field-NPC or enemy variants) to the party id used by the pouch (1
 * Goombella, 2 Koops, 3 Bobbery, 4 Yoshi, 5 Flurrie, 6 Vivian, 7 Ms. Mowz).
 */
s32 BattleTransPartyId(BattleUnitType kind) {
    switch (kind) {
        case UNIT_GOOMBELLA_CHAPTER4:
        case UNIT_DOOPLISS_CHAPTER8_GOOMBELLA:
        case UNIT_GOOMBELLA:
            return PARTNER_GOOMBELLA;
        case UNIT_KOOPS_CHAPTER4:
        case UNIT_DOOPLISS_CHAPTER8_KOOPS:
        case UNIT_KOOPS:
            return PARTNER_KOOPS;
        case UNIT_YOSHI_CHAPTER4:
        case UNIT_DOOPLISS_CHAPTER8_YOSHI:
        case UNIT_YOSHI:
            return PARTNER_YOSHI;
        case UNIT_FLURRIE_CHAPTER4:
        case UNIT_DOOPLISS_CHAPTER8_FLURRIE:
        case UNIT_FLURRIE:
            return PARTNER_FLURRIE;
        case UNIT_DOOPLISS_CHAPTER8_VIVIAN:
        case UNIT_VIVIAN:
            return PARTNER_VIVIAN;
        case UNIT_DOOPLISS_CHAPTER8_BOBBERY:
        case UNIT_BOBBERY:
            return PARTNER_BOBBERY;
        case UNIT_DOOPLISS_CHAPTER8_MS_MOWZ:
        case UNIT_MS_MOWZ:
            return PARTNER_MSMOWZ;
        case UNIT_NULL:
            return PARTNER_NONE;
        default:
            return PARTNER_NONE;
    }
}

/** Converts a pouch party id back to the partner's battle unit kind (0xE0-0xE6). */
BattleUnitType BattleTransPartyIdToUnitKind(s32 partyId) {
    switch (partyId) {
        case PARTNER_GOOMBELLA:
            return UNIT_GOOMBELLA;
        case PARTNER_KOOPS:
            return UNIT_KOOPS;
        case PARTNER_YOSHI:
            return UNIT_YOSHI;
        case PARTNER_FLURRIE:
            return UNIT_FLURRIE;
        case PARTNER_VIVIAN:
            return UNIT_VIVIAN;
        case PARTNER_BOBBERY:
            return UNIT_BOBBERY;
        case PARTNER_MSMOWZ:
            return UNIT_MS_MOWZ;
        default:
            return UNIT_NULL;
    }
}

/**
 * Recounts the badges a unit wears: optionally clears the counts,
 * then adds every badge in the pouch's equipped list for Mario
 * or the partner (or the single item type for an enemy) via _EquipItem, and re-derives max HP and FP.
 */
void BtlUnit_EquipItem(BattleUnit* unit, EquipFlags flags, ItemType type) {
    PouchData* pouch = pouchGetPtr();
    int i;

    if (unit) {
        if (flags & EQUIP_RESET) {
            for (i = 0; i < 40; i++) {
                unit->badgeCounts[i] = FALSE;
            }
        }

        if (unit->currentKind != UNIT_SYSTEM) {
            if (flags & (EQUIP_MARIO | EQUIP_PARTNER)) {
                for (i = 0; i < MAX_BADGES; i++) {
                    _EquipItem(unit, flags & ~EQUIP_RESET, pouch->equippedBadges[i]);
                }
            } else {
                _EquipItem(unit, flags, type);
            }
            BtlUnit_ReviseHpFp(unit);
        }
    }
}

/**
 * Adds one badge to the unit's badgesEquipped counts.
 * Refund, Simplifier and Unsimplifier count for anyone;
 * the other badges count for Mario or for partners (P badges) according to the flags.
 */
void _EquipItem(BattleUnit* unit, EquipFlags flags, ItemType type) {
    if (type == ITEM_NULL) {
        return;
    }

    if (type == ITEM_REFUND) {
        unit->badgesEquipped.Refund++;
        return;
    } else if (type == ITEM_SIMPLIFIER) {
        unit->badgesEquipped.Simplifier++;
        return;
    } else if (type == ITEM_UNSIMPLIFIER) {
        unit->badgesEquipped.Unsimplifier++;
        return;
    }

    if ((flags & EQUIP_PARTNER) == 0) {
        if (type == ITEM_SPIKE_SHIELD) {
            unit->badgesEquipped.SpikeShield++;
            return;
        } else if (type == ITEM_ICE_POWER) {
            unit->badgesEquipped.IcePower++;
            return;
        } else if (type == ITEM_CLOSE_CALL) {
            unit->badgesEquipped.CloseCall++;
            return;
        } else if (type == ITEM_PRETTY_LUCKY) {
            unit->badgesEquipped.PrettyLucky++;
            return;
        } else if (type == ITEM_LUCKY_DAY) {
            unit->badgesEquipped.LuckyDay++;
            return;
        } else if (type == ITEM_DAMAGE_DODGE) {
            unit->badgesEquipped.DamageDodge++;
            return;
        } else if (type == ITEM_POWER_PLUS) {
            unit->badgesEquipped.PowerPlus++;
            return;
        } else if (type == ITEM_P_UP_D_DOWN) {
            unit->badgesEquipped.PUpDDown++;
            return;
        } else if (type == ITEM_ALL_OR_NOTHING) {
            unit->badgesEquipped.AllOrNothing++;
            return;
        } else if (type == ITEM_MEGA_RUSH) {
            unit->badgesEquipped.MegaRush++;
            return;
        } else if (type == ITEM_POWER_RUSH) {
            unit->badgesEquipped.PowerRush++;
            return;
        } else if (type == ITEM_P_DOWN_D_UP) {
            unit->badgesEquipped.PDownDUp++;
            return;
        } else if (type == ITEM_DOUBLE_PAIN) {
            unit->badgesEquipped.DoublePain++;
            return;
        } else if (type == ITEM_LAST_STAND) {
            unit->badgesEquipped.LastStand++;
            return;
        } else if (type == ITEM_DEFEND_PLUS) {
            unit->badgesEquipped.DefendPlus++;
            return;
        } else if (type == ITEM_HAPPY_HEART) {
            unit->badgesEquipped.HappyHeart++;
            return;
        } else if (type == ITEM_HAPPY_FLOWER) {
            unit->badgesEquipped.HappyFlower++;
            return;
        } else if (type == ITEM_RETURN_POSTAGE) {
            unit->badgesEquipped.ReturnPostage++;
            return;
        } else if (type == ITEM_HP_PLUS) {
            unit->badgesEquipped.HpPlus++;
            return;
        } else if (type == ITEM_FP_PLUS) {
            unit->badgesEquipped.FpPlus++;
            return;
        } else if (type == ITEM_FEELING_FINE) {
            unit->badgesEquipped.FeelingFine++;
            return;
        } else if (type == ITEM_ZAP_TAP) {
            unit->badgesEquipped.ZapTap++;
            return;
        } else if (type == ITEM_PITY_FLOWER) {
            unit->badgesEquipped.PityFlower++;
            return;
        } else if (type == ITEM_HP_DRAIN_BADGE) {
            unit->badgesEquipped.HpDrain++;
            return;
        } else if (type == ITEM_FP_DRAIN) {
            unit->badgesEquipped.FpDrain++;
            return;
        } else if (type == ITEM_UNUSED_DEFEND_BADGE) {
            unit->badgesEquipped.DefendUnused++;
            return;
        } else if (type == ITEM_CHARGE) {
            unit->badgesEquipped.Charge++;
            return;
        } else if (type == ITEM_SUPER_CHARGE) {
            unit->badgesEquipped.SuperCharge++;
            return;
        } else if (type == ITEM_FLOWER_SAVER) {
            unit->badgesEquipped.FlowerSaver++;
            return;
        } else if (type == ITEM_SUPER_APPEAL) {
            unit->badgesEquipped.SuperAppeal++;
            return;
        } else if (type == ITEM_LUCKY_START) {
            unit->badgesEquipped.LuckyStart++;
            return;
        }
    }

    if ((flags & EQUIP_MARIO) == 0) {
        if (type == ITEM_CLOSE_CALL_P) {
            unit->badgesEquipped.CloseCall++;
            return;
        } else if (type == ITEM_PRETTY_LUCKY_P) {
            unit->badgesEquipped.PrettyLucky++;
            return;
        } else if (type == ITEM_LUCKY_DAY_P) {
            unit->badgesEquipped.LuckyDay++;
            return;
        } else if (type == ITEM_DAMAGE_DODGE_P) {
            unit->badgesEquipped.DamageDodge++;
            return;
        } else if (type == ITEM_ALL_OR_NOTHING_P) {
            unit->badgesEquipped.AllOrNothing++;
            return;
        } else if (type == ITEM_POWER_PLUS_P) {
            unit->badgesEquipped.PowerPlus++;
            return;
        } else if (type == ITEM_POWER_RUSH_P) {
            unit->badgesEquipped.PowerRush++;
            return;
        } else if (type == ITEM_DEFEND_PLUS_P) {
            unit->badgesEquipped.DefendPlus++;
            return;
        } else if (type == ITEM_UNUSED_DEFEND_BADGE_P) {
            unit->badgesEquipped.DefendUnused++;
            return;
        } else if (type == ITEM_P_DOWN_D_UP_P) {
            unit->badgesEquipped.PDownDUp++;
            return;
        } else if (type == ITEM_PITY_FLOWER_P) {
            unit->badgesEquipped.PityFlower++;
            return;
        } else if (type == ITEM_HP_DRAIN_P) {
            unit->badgesEquipped.HpDrain++;
            return;
        } else if (type == ITEM_FP_DRAIN_P) {
            unit->badgesEquipped.FpDrain++;
            return;
        } else if (type == ITEM_CHARGE_P) {
            unit->badgesEquipped.Charge++;
            return;
        } else if (type == ITEM_SUPER_CHARGE_P) {
            unit->badgesEquipped.SuperCharge++;
            return;
        } else if (type == ITEM_HAPPY_HEART_P) {
            unit->badgesEquipped.HappyHeart++;
            return;
        } else if (type == ITEM_HP_PLUS_P) {
            unit->badgesEquipped.HpPlus++;
            return;
        } else if (type == ITEM_LAST_STAND_P) {
            unit->badgesEquipped.LastStand++;
            return;
        } else if (type == ITEM_MEGA_RUSH_P) {
            unit->badgesEquipped.MegaRush++;
            return;
        } else if (type == ITEM_P_UP_D_DOWN_P) {
            unit->badgesEquipped.PUpDDown++;
            return;
        } else if (type == ITEM_FLOWER_SAVER_P) {
            unit->badgesEquipped.FlowerSaver++;
            return;
        } else if (type == ITEM_FEELING_FINE_P) {
            unit->badgesEquipped.FeelingFine++;
            return;
        } else if (type == ITEM_SUPER_APPEAL_P) {
            unit->badgesEquipped.SuperAppeal++;
            return;
        } else if (type == ITEM_LUCKY_START_P) {
            unit->badgesEquipped.LuckyStart++;
            return;
        }
    }

    if ((flags & EQUIP_MARIO) != 0) {
        if (type == ITEM_DOUBLE_DIP) {
            unit->badgesEquipped.DoubleDip++;
            if (unit->badgesEquipped.DoubleDip >= 2 && !unit->badgesEquipped.TripleDip) {
                unit->badgesEquipped.TripleDip++;
            }
            return;
        } else if (type == ITEM_TRIPLE_DIP) {
            unit->badgesEquipped.TripleDip++;
            return;
        } else if (type == ITEM_JUMPMAN) {
            unit->badgesEquipped.Jumpman++;
            return;
        } else if (type == ITEM_HAMMERMAN) {
            unit->badgesEquipped.Hammerman++;
            return;
        }
    }

    if (flags & EQUIP_PARTNER && type == ITEM_DOUBLE_DIP_P) {
        unit->badgesEquipped.DoubleDip++;
        if (unit->badgesEquipped.DoubleDip >= 2 && !unit->badgesEquipped.TripleDip) {
            unit->badgesEquipped.TripleDip++;
        }
        return;
    }

    if ((flags & EQUIP_MARIO || flags & EQUIP_PARTNER) && type == ITEM_AUTO_COMMAND_BADGE) {
        unit->badgesEquipped.AutoCommand++;
        return;
    }
}

void BattlePartyInfoWorkInit(BattleWork* wp) {
    BattleWorkPartyInfo* partyInfo;
    int i;

    for (i = TYPE_PARTNER_MIN; i < TYPE_PARTNER_MAX; i++) {
        partyInfo = &wp->partyInfo[i - TYPE_PARTNER_MIN];
        partyInfo->sizeMultiplier = 1.0;
        partyInfo->statusFlags = 0;
        memset(&partyInfo->statusTurns, 0, sizeof(partyInfo->statusTurns));
#if VERSION >= VERSION_US
        partyInfo->usCommandDisabled[0] = 0;
        partyInfo->usCommandDisabled[1] = 0;
#endif
    }
}

/** Adds coins won in battle to the field battle info, paid out when the battle ends. */
void BattleStoreCoin(BattleWork* wp, s32 coin) {
    wp->fbatInfo->coins += coin;
}

/** Adds Star Points earned from a defeated enemy to the battle's running total. */
void BattleStoreExp(BattleWork* wp, s32 exp) {
    wp->stockExp += exp;
}

/** Returns the Star Points to award at the end of the battle, clamped to 1-100. */
s32 BattleGetStockExp(BattleWork* wp) {
    s32 exp = wp->stockExp;
    if (exp <= 0) {
        return 1;
    }
    if (exp >= 100) {
        return 100;
    }
    return exp;
}

/** Returns the floor height at a point: 0 on the stage, the audience floor height beyond the stage's depth. */
f32 BattleGetFloorHeight(BattleWork* wp, f32 x, f32 y, f32 z) {
    BattleStage* stage = BattleStageGetPtr();
    if (z <= stage->stageSizeZ) {
        return 0.0f;
    }
    return stage->audienceSizeY;
}

void BattleCheckUnitBroken(BattleWork* wp) {
    s32 i;

    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        BattleUnit* unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->flags & BATTLE_UNIT_FLAG_BROKEN) {
            BtlUnit_Delete(unit);
        }
    }
}

/** Empties the after-reaction queue (64 entries of unit id and data tag). */
void BattleAfterReactionQueueInit(void) {
    BattleAfterReaction* base = _battleWorkPointer->afterReactions;
    s32 i;
    for (i = 0; i < BATTLE_AFTER_REACTION_COUNT; i++) {
        base[i].unitId = -1;
        base[i].tag = 0;
    }
}

/**
 * Queues an after-reaction for unit a: a script from its unit data entry b to run once the current action
 * finishes. A unit already queued is not added again.
 */
void BattleAfterReactionEntry(s32 unitId, s32 tag) {
    BattleAfterReaction* base = _battleWorkPointer->afterReactions;
    s32 i;
    for (i = 0; i < BATTLE_AFTER_REACTION_COUNT; i++) {
        if (base[i].unitId != -1 && unitId == base[i].unitId) {
            return;
        }
    }
    for (i = 0; i < BATTLE_AFTER_REACTION_COUNT; i++) {
        if (base[i].unitId == -1) {
            break;
        }
    }
    if (i < BATTLE_AFTER_REACTION_COUNT) {
        base[i].unitId = unitId;
        base[i].tag = tag;
    }
}

/** Removes every queued after-reaction of unit a with tag b, closing the gaps. */
void BattleAfterReactionRelease(s32 unitId, s32 tag) {
    s32 i;
    s32 j;
    BattleAfterReaction* base = _battleWorkPointer->afterReactions;
    for (i = 0; i < BATTLE_AFTER_REACTION_COUNT; i++) {
        if (base[i].unitId != -1 && unitId == base[i].unitId && tag == base[i].tag) {
            for (j = i; j < BATTLE_AFTER_REACTION_COUNT - 1; j++) {
                base[j].unitId = base[j + 1].unitId;
                base[j].tag = base[j + 1].tag;
            }
            base[BATTLE_AFTER_REACTION_COUNT - 1].unitId = -1;
            base[BATTLE_AFTER_REACTION_COUNT - 1].tag = 0;
            i--;
        }
    }
}

/**
 * Starts the next queued after-reaction script (dropping entries whose unit is gone)
 * after consuming reserved items; returns TRUE while one was started,
 * FALSE when the queue is empty or the battle is decided.
 */
BOOL BattleAfterReactionMain(void) {
    s32 id;
    BattleWork* wp = _battleWorkPointer;
    BattleAfterReaction* base = wp->afterReactions;
    s32 i;
    for (i = 0; i < BATTLE_AFTER_REACTION_COUNT; i++) {
        id = base[i].unitId;
        if (id != -1) {
            s32 tag = base[i].tag;
            BattleUnit* unit = BattleGetUnitPtr(wp, id);
            EventEntry* evt;
            if (unit == NULL) {
                BattleAfterReactionRelease(id, tag);
                return BattleAfterReactionMain();
            }
            BattleConsumeReserveItem();
            if (BattleCheckConcluded(wp)) {
                return FALSE;
            }
            BattleAfterReactionRelease(id, tag);
            evt = evtEntry(BtlUnit_GetData(unit, tag), 10, 0);
            if (evt != NULL) {
                unit->attackEventId = evt->eventId;
                evt->unitId = unit->unitId;
            }
            return TRUE;
        }
    }
    return FALSE;
}

/** Returns TRUE while a battle forbids the soft reset (battle flag bit 29); mariost checks it before resetting. */
BOOL battleDisableHResetCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    if (gp->inBattle == 0) {
        return FALSE;
    }
    if (wp == NULL) {
        return FALSE;
    }
    return (wp->flags & BATTLE_FLAG_DISABLE_HRESET) != 0;
}

u8 BattleMajinaiCheck(void) {
    PouchData* pouch = pouchGetPtr();
    BattleWork* wp = _battleWorkPointer;
    s32 r;
    if (pouch->merleeCurseUsesRemaining == 0) {
        return 0;
    }
    if (pouch->turnsUntilMerleeActivation < 0) {
        pouch->merleeNextCurseType = 0;
        pouch->turnsUntilMerleeActivation = irand(5) + 6;
        pouch->turnsUntilMerleeActivation--;
        return 0;
    }
    if (pouch->merleeNextCurseType != 0) {
        return pouch->merleeNextCurseType;
    }
    pouch->turnsUntilMerleeActivation--;
    if (pouch->turnsUntilMerleeActivation > 0) {
        return 0;
    }
    pouch->turnsUntilMerleeActivation = 0;
    if (wp->fbatInfo->setup->battleFlags & NPC_BATTLE_MERLEE_SAFE_CURSE_UNUSED) {
        r = irand(100);
        if (r < 45) {
            pouch->merleeNextCurseType = 1;
        } else if (r < 90) {
            pouch->merleeNextCurseType = 2;
        } else {
            pouch->merleeNextCurseType = 3;
        }
    } else {
        r = irand(100);
        if (r < 30) {
            pouch->merleeNextCurseType = 1;
        } else if (r < 60) {
            pouch->merleeNextCurseType = 2;
        } else if (r < 80) {
            pouch->merleeNextCurseType = 3;
        } else {
            pouch->merleeNextCurseType = 4;
        }
    }
    return pouch->merleeNextCurseType;
}

/** Uses up one of Merlee's charms and resets the countdown for the next. */
void BattleMajinaiDone(void) {
    PouchData* pouch = pouchGetPtr();
    pouch->merleeCurseUsesRemaining--;
    if (pouch->merleeCurseUsesRemaining != 0) {
        pouch->turnsUntilMerleeActivation = -1;
        pouch->merleeNextCurseType = 0;
    } else {
        pouch->turnsUntilMerleeActivation = -1;
        pouch->merleeNextCurseType = 0;
    }
}

/** Flags the battle when Merlee's last charm has been used, so the end-of-charm notice can show. */
void BattleMajinaiEndCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    if (pouchGetPtr()->merleeCurseUsesRemaining == 0) {
        wp->flags |= BATTLE_FLAG_MAJINAI_END;
    }
}

/** Keeps the HP/FP/Star Power status window open while Star Power sparkles are flying to the gauge. */
void BattleStatusWindowAPRecoveryOn(void) {
    _battleWorkPointer->statusWindowAPRecoveryOn = 1;
    BattleStatusWindowCheck();
}

/** Releases the Star Power hold on the status window. */
void BattleStatusWindowAPRecoveryOff(void) {
    _battleWorkPointer->statusWindowAPRecoveryOn = 0;
    BattleStatusWindowCheck();
}

/** Sets the script side's request to show the status window (it opens when the system also allows it). */
void BattleStatusWindowEventOn(void) {
    _battleWorkPointer->statusWindowEventOn = 1;
    BattleStatusWindowCheck();
}

/** Sets the battle system's permission to show the status window. */
void BattleStatusWindowSystemOn(void) {
    _battleWorkPointer->statusWindowSystemOn = 1;
    BattleStatusWindowCheck();
}

/** Clears the script side's request to show the status window. */
void BattleStatusWindowEventOff(void) {
    _battleWorkPointer->statusWindowEventOn = 0;
    BattleStatusWindowCheck();
}

/** Clears the battle system's permission to show the status window. */
void BattleStatusWindowSystemOff(void) {
    _battleWorkPointer->statusWindowSystemOn = 0;
    BattleStatusWindowCheck();
}

/**
 * Opens or closes the status window from the current requests:
 * never while forced off or during a tutorial lecture, always while Star Power sparkles fly,
 * otherwise when both script and system want it. Returns whether it is open.
 */
BOOL BattleStatusWindowCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    BOOL open;
    if (wp->statusWindowOff) {
        open = FALSE;
    } else if (wp->lectureMode) {
        open = FALSE;
    } else if (wp->statusWindowAPRecoveryOn) {
        open = TRUE;
    } else if (wp->statusWindowEventOn && wp->statusWindowSystemOn) {
        open = TRUE;
    } else {
        open = FALSE;
    }
    if (open) {
        if (!statusWinCheck()) {
            statusWinOpen();
        }
    } else if (statusWinCheck()) {
        statusWinClose();
    }
    return open;
}

/**
 * Removes from the pouch the items reserved by actions this turn (up to four),
 * so items are only spent once their use has played out.
 */
void BattleConsumeReserveItem(void) {
    BattleWork* wp;
    s32 i;
    wp = _battleWorkPointer;
    for (i = 0; i < 4; i++) {
        // FAKE: (s32) cast before the zero test; the plain test changes the object
        if ((s32)wp->reserveItems[i] != 0) {
            pouchRemoveItem(wp->reserveItems[i]);
        }
        wp->reserveItems[i] = 0;
    }
}
