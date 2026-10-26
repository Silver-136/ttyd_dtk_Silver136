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

typedef enum EquipFlags {
    EQUIP_RESET = 1 << 0,   // 0x1, clear/reset equipped badges
    EQUIP_MARIO = 1 << 1,   // 0x2, equip Mario (non-P) badges
    EQUIP_PARTNER = 1 << 2, // 0x4, equip Partner (P) badges
} EquipFlags;

BattleWork* _battleWorkPointer;

BattleUnitSetup entryunit_mario = { &unitdata_Mario, 0, {0}, 0x04000002, {-100.0f, 0.0f, -5.0f} };
BattleUnitSetup entryunit_system = { &unit_system, 0, {0}, 0x04000002 };

// forward declarations
BOOL BattleMain(void);
void BtlUnit_EquipItem(BattleUnit* unit, EquipFlags flags, ItemType type);
void _EquipItem(BattleUnit* unit, EquipFlags flags, ItemType type);
void BattlePartyInfoWorkInit(BattleWork* wp);
void BattleCheckUnitBroken(BattleWork* wp);

void battleMain(void) {
    BattleWork* wp = _battleWorkPointer;

    if (wp && BattleMain() == 0) {
        wp->flags |= BATTLE_SEQ_END;
    }
}

BOOL battleSeqEndCheck() {
    return _battleWorkPointer->flags >> 31;
}

void BattleInit(FieldBattleInfo* info) {
    BattleWork* wp;

#ifdef VERSION_G8ME01
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

    if ((gp->flags & 0x1000) && (gp->flags & 0x2000) && BattleGetSeq(wp, SEQ_UNKNOWN) == 2) {
        wp->flags |= (BATTLE_FLAG_INTERRUPT_STOP | BATTLE_FLAG_DEMO_TIMEOUT);
        BattleSetSeq(wp, SEQ_UNKNOWN, 3);
        BattleSetSeq(wp, SEQ_END, 0x7000000);
        return TRUE;
    }

    return TRUE;
}

void BattleEnd(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    int i;
    void* subMenu;

    for (i = 0; i < 64; i++) {
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
#ifdef VERSION_G8ME01
    BattleFree(wp);
#else
    _mapFree(wp);
#endif
    _battleWorkPointer = NULL;
}

extern BattleUnitSetup entryunit_party[7];

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
            enemy->flags |= 0x40000000u;
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
    for (j = 0; j < 200; j++) {
        switch (pouch->equippedBadges[j]) {
            case ITEM_QUICK_CHANGE:
                wp->badgeEquippedFlags |= 1 << 0;
                break;
            case ITEM_PEEKABOO:
                wp->badgeEquippedFlags |= 1 << 1;
                break;
            case ITEM_TIMING_TUTOR:
                wp->badgeEquippedFlags |= 1 << 2;
                break;
            case ITEM_HEART_FINDER:
                wp->badgeEquippedFlags |= 1 << 3;
                break;
            case ITEM_FLOWER_FINDER:
                wp->badgeEquippedFlags |= 1 << 4;
                break;
            case ITEM_MONEY_MONEY:
                wp->badgeEquippedFlags |= 1 << 7;
                break;
            case ITEM_ATTACK_FX_R:
                wp->badgeEquippedFlags |= 1 << 8;
                break;
            case ITEM_ATTACK_FX_B:
                wp->badgeEquippedFlags |= 1 << 10;
                break;
            case ITEM_ATTACK_FX_G:
                wp->badgeEquippedFlags |= 1 << 11;
                break;
            case ITEM_ATTACK_FX_Y:
                wp->badgeEquippedFlags |= 1 << 9;
                break;
            case ITEM_ATTACK_FX_P:
                wp->badgeEquippedFlags |= 1 << 12;
                break;
            default:
                break;
        }
    }

    return TRUE;
}

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

void BattleIncSeq(BattleWork* wp, BattleSequence seq) {
    s32 value = BattleGetSeq(wp, seq);
    BattleSetSeq(wp, seq, value + 1);
}

void* BattleAlloc(u32 size) {
#ifdef VERSION_G8ME01
    return (_mapAlloc)(R_battlemapalloc_base_ptr, size);
#else
    return __memAlloc(HEAP_BATTLE, size);
#endif
}

void BattleFree(void* ptr) {
    if (ptr)
#ifdef VERSION_G8ME01
        (_mapFree)(R_battlemapalloc_base_ptr, ptr);
#else
        __memFree(HEAP_BATTLE, ptr);
#endif
}

BattleUnit* BattleGetUnitPtr(BattleWork* wp, s32 index) {
    if (index == -1) {
        return NULL;
    }
    if (index < -1) {
        return NULL;
    }
    if (index < 64) {
        return wp->units[index];
    }

    return NULL;
}

void BattleSetUnitPtr(BattleWork* wp, s32 index, BattleUnit* unit) {
    if (index < 64) {
        wp->units[index] = unit;
    }
}

BattleUnitPart* BattleGetUnitPartsPtr(s32 index, s32 partNum) {
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, index);
    return BtlUnit_GetPartsPtr(unit, partNum);
}

BattleUnit* BattleGetSystemPtr(BattleWork* wp) {
    s32 i;
    BattleUnit* unit;
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->currentKind == UNIT_SYSTEM) {
            return unit;
        }
    }
    return NULL;
}

BattleUnit* BattleGetMarioPtr(BattleWork* wp) {
    s32 i;
    BattleUnit* unit;
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->currentKind == UNIT_MARIO && unit->mAlliance == ALLIANCE_PLAYER) {
            return unit;
        }
    }
    return NULL;
}

BattleUnit* BattleGetPartyPtr(BattleWork* wp) {
    s32 i;
    BattleUnit* unit;
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && !(unit->flags & BATTLE_UNIT_FLAG_SWAPPED_OUT) && unit->currentKind >= TYPE_PARTNER_MIN &&
            unit->currentKind < TYPE_PARTNER_MAX && unit->mAlliance == ALLIANCE_PLAYER) {
            return unit;
        }
    }
    return NULL;
}

BattleUnit* BattleGetPartnerPtr(BattleWork* wp, BattleUnit* unit) {
    if (unit->currentKind == UNIT_MARIO) {
        return BattleGetPartyPtr(wp);
    }
    return BattleGetMarioPtr(wp);
}

BOOL BattlePartyAnimeLoad(s32 partyId) {
    BattleUnitSetup* setup = &entryunit_party[partyId - TYPE_PARTNER_MIN];

    return animEffectAsync(setup->mUnitKindParams->mParts->modelName, 1);
}

BattleUnit* BattleChangeParty(BattleWork* wp) {
    s32 kind = wp->commandMenu.partyKind;
    BattleUnit* party = BattleGetPartyPtr(wp);
    s32 oldId = *(s32*)party;
    BattleUnit* unit;
    s32 i;
    f32 dummy, x, y, z;

    if (kind < TYPE_PARTNER_MIN || kind >= TYPE_PARTNER_MAX) {
        return NULL;
    }
    unit = BtlUnit_Spawn(&entryunit_party[kind - TYPE_PARTNER_MIN], 1);
    for (i = 0; i < 64; i++) {
        s32 id = wp->movePriorityQueue[i];
        if (id != -1) {
            if (id == oldId) {
                wp->movePriorityQueue[i] = *(s32*)unit;
            } else if (id == *(s32*)unit) {
                wp->movePriorityQueue[i] = oldId;
            }
        }
    }
    BtlUnit_GetHomePos(unit, &x, &y, &z);
    BtlUnit_GetHomePos(party, &x, &dummy, &z);
    BtlUnit_EquipItem(unit, EQUIP_RESET | EQUIP_PARTNER, 0);
    y += BattleGetFloorHeight(_battleWorkPointer, x, y, z);
    BtlUnit_SetHomePos(unit, x, y, z);
    if (wp->activeUnitId == oldId) {
        wp->activeUnitId = *(s32*)unit;
    }
    unit->moveState = party->moveState;
    unit->movesRemaining = party->movesRemaining;
    unit->maxMovesThisTurn = party->maxMovesThisTurn;
    unit->activeTurns = party->activeTurns;
    BtlUnit_SetParamToPouch(party);
    party->flags |= BATTLE_UNIT_FLAG_SWAPPED_OUT;
    unit->mMoveColorLv = party->mMoveColorLv;
    unit->moveColorLvDisp = party->moveColorLvDisp;
    return unit;
}

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
    case 0:
        return PARTNER_NONE;
    default:
        return PARTNER_NONE;
    }
}

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
        return 0;
    }
}

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
                for (i = 0; i < 200; i++) {
                    _EquipItem(unit, flags & ~EQUIP_RESET, pouch->equippedBadges[i]);
                }
            } else {
                _EquipItem(unit, flags, type);
            }
            BtlUnit_ReviseHpFp(unit);
        }
    }
}

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
#ifdef VERSION_G8ME01
        partyInfo->usCommandDisabled[0] = 0;
        partyInfo->usCommandDisabled[1] = 0;
#endif
    }
}

void BattleStoreCoin(BattleWork* wp, s32 coin) {
    wp->fbatInfo->coins += coin;
}

void BattleStoreExp(BattleWork* wp, s32 exp) {
    wp->stockExp += exp;
}

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

f32 BattleGetFloorHeight(BattleWork* wp, f32 x, f32 y, f32 z) {
    BattleStage* stage = BattleStageGetPtr();
    if (z <= stage->stageSizeZ) {
        return 0.0f;
    }
    return stage->audienceSizeY;
}

void BattleCheckUnitBroken(BattleWork* wp) {
    s32 i;

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(wp, i);
        if (unit && unit->flags & BATTLE_UNIT_FLAG_BROKEN) {
            BtlUnit_Delete(unit);
        }
    }
}

void BattleAfterReactionQueueInit(void) {
    BattleAfterReaction* base = _battleWorkPointer->afterReactions;
    s32 i;
    for (i = 0; i < 64; i++) {
        base[i].unitId = -1;
        base[i].tag = 0;
    }
}

void BattleAfterReactionEntry(s32 a, s32 b) {
    BattleAfterReaction* base = _battleWorkPointer->afterReactions;
    s32 i;
    for (i = 0; i < 64; i++) {
        if (base[i].unitId != -1 && a == base[i].unitId) {
            return;
        }
    }
    for (i = 0; i < 64; i++) {
        if (base[i].unitId == -1) {
            break;
        }
    }
    if (i < 64) {
        base[i].unitId = a;
        base[i].tag = b;
    }
}

void BattleAfterReactionRelease(s32 a, s32 b) {
    s32 i;
    s32 j;
    BattleAfterReaction* base = _battleWorkPointer->afterReactions;
    for (i = 0; i < 64; i++) {
        if (base[i].unitId != -1 && a == base[i].unitId && b == base[i].tag) {
            for (j = i; j < 63; j++) {
                base[j].unitId = base[j + 1].unitId;
                base[j].tag = base[j + 1].tag;
            }
            base[63].unitId = -1;
            base[63].tag = 0;
            i--;
        }
    }
}

BOOL BattleAfterReactionMain(void) {
    s32 id;
    BattleWork* wp = _battleWorkPointer;
    BattleAfterReaction* base = wp->afterReactions;
    s32 i;
    for (i = 0; i < 64; i++) {
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
                evt->unitId = *(s32*)unit;
            }
            return TRUE;
        }
    }
    return FALSE;
}

BOOL battleDisableHResetCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    if (gp->inBattle == 0) {
        return FALSE;
    }
    if (wp == NULL) {
        return FALSE;
    }
    return (wp->flags >> 29) & 1;
}

u8 BattleMajinaiCheck(void) {
    PouchData* pouch = pouchGetPtr();
    BattleWork* wp = _battleWorkPointer;
    s32 r;
    if (pouch->mMerleeCurseUsesRemaining == 0) {
        return 0;
    }
    if (pouch->mTurnsUntilMerleeActivation < 0) {
        pouch->mMerleeNextCurseType = 0;
        pouch->mTurnsUntilMerleeActivation = irand(5) + 6;
        (pouch->mTurnsUntilMerleeActivation)--;
        return 0;
    }
    if (pouch->mMerleeNextCurseType != 0) {
        return pouch->mMerleeNextCurseType;
    }
    (pouch->mTurnsUntilMerleeActivation)--;
    if (pouch->mTurnsUntilMerleeActivation > 0) {
        return 0;
    }
    pouch->mTurnsUntilMerleeActivation = 0;
    if ((u32)wp->fbatInfo->setup->battleFlags & NPC_BATTLE_MERLEE_SAFE_CURSE_UNUSED) {
        r = irand(100);
        if (r < 45) pouch->mMerleeNextCurseType = 1;
        else if (r < 90) pouch->mMerleeNextCurseType = 2;
        else pouch->mMerleeNextCurseType = 3;
    } else {
        r = irand(100);
        if (r < 30) pouch->mMerleeNextCurseType = 1;
        else if (r < 60) pouch->mMerleeNextCurseType = 2;
        else if (r < 80) pouch->mMerleeNextCurseType = 3;
        else pouch->mMerleeNextCurseType = 4;
    }
    return pouch->mMerleeNextCurseType;
}

void BattleMajinaiDone(void) {
    PouchData* pouch = pouchGetPtr();
    pouch->mMerleeCurseUsesRemaining--;
    if (pouch->mMerleeCurseUsesRemaining != 0) {
        pouch->mTurnsUntilMerleeActivation = -1;
        pouch->mMerleeNextCurseType = 0;
    } else {
        pouch->mTurnsUntilMerleeActivation = -1;
        pouch->mMerleeNextCurseType = 0;
    }
}

void BattleMajinaiEndCheck(void) {
    BattleWork* wp = _battleWorkPointer;
    if (pouchGetPtr()->mMerleeCurseUsesRemaining == 0) {
        wp->flags |= BATTLE_FLAG_MAJINAI_END;
    }
}

void BattleStatusWindowAPRecoveryOn(void) {
    _battleWorkPointer->statusWindowAPRecoveryOn = 1;
    BattleStatusWindowCheck();
}

void BattleStatusWindowAPRecoveryOff(void) {
    _battleWorkPointer->statusWindowAPRecoveryOn = 0;
    BattleStatusWindowCheck();
}

void BattleStatusWindowEventOn(void) {
    _battleWorkPointer->statusWindowEventOn = 1;
    BattleStatusWindowCheck();
}

void BattleStatusWindowSystemOn(void) {
    _battleWorkPointer->statusWindowSystemOn = 1;
    BattleStatusWindowCheck();
}

void BattleStatusWindowEventOff(void) {
    _battleWorkPointer->statusWindowEventOn = 0;
    BattleStatusWindowCheck();
}

void BattleStatusWindowSystemOff(void) {
    _battleWorkPointer->statusWindowSystemOn = 0;
    BattleStatusWindowCheck();
}

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

void BattleConsumeReserveItem(void) {
    BattleWork* wp;
    s32 i;
    wp = _battleWorkPointer;
    for (i = 0; i < 4; i++) {
        if ((s32)wp->reserveItems[i] != 0) {
            pouchRemoveItem(wp->reserveItems[i]);
        }
        wp->reserveItems[i] = 0;
    }
}
