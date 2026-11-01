// unit_borodo_king: Big Bandit (Bandit king form), battle unit.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "mario/mario_pouch.h"
#include "data/item_data.h"
#include "system.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_message.h"
#include "battle/battle_icon.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"


static USER_FUNC(_steal_item);

BattleUnitKind unit_borodo_king;
static BattleWeapon weapon_borodo_attack;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[20];
static PoseTableEntry pose_table_item_have[18];
#if VERSION >= VERSION_US
static BattleDataEntry data_table[2];
#else
static BattleDataEntry data_table[1];
#endif
static BattleUnitKindPart parts[1];
static EvtWord init_event[57];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static EvtWord attack_event[59];
static EvtWord _borodo_normal_attack[503];
static EvtWord _borodo_steal_attack[610];
static EvtWord escape_event[115];
static EvtWord wait_event[26];

/**
 * Script helper for the Big Bandit's steal attack: arg0 = thief unit, arg1 = victim unit;
 * arg2/arg3/arg4 receive the stolen item id, icon id and count.
 * With a 40% chance it takes up to 10 coins, otherwise it takes a random item or badge from Mario's pouch
 * and equips the victim without it.
 */
static USER_FUNC(_steal_item) {
    EvtWord* args = event->args;
    s32 coins;
    s32 num;
    s32 count;
    PouchData* pouch = pouchGetPtr();
    BattleUnit* unit;
    BattleUnit* target;
    s32 itemVar;
    s32 iconVar;
    s32 numVar;
    s32 id;
    s32 tid;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    tid = evtGetValue(event, args[1]);
    tid = BattleTransID(event, tid);
    target = BattleGetUnitPtr(_battleWorkPointer, tid);
    if (target == NULL) {
        return EVT_RETURN_DONE;
    }
    itemVar = args[2];
    iconVar = args[3];
    numVar = args[4];
    evtSetValue(event, itemVar, 0);
    evtSetValue(event, iconVar, 0);
    evtSetValue(event, numVar, 0);
    if (irand(100) < 40) {
        coins = pouchGetCoin();
        num = 10;
        if (coins < 10) {
            num = coins;
        }
        if (num > 0) {
            evtSetValue(event, itemVar, ITEM_COIN);
#if VERSION >= VERSION_US
            evtSetValue(event, iconVar, 403);
#else
            evtSetValue(event, iconVar, 395);
#endif
            evtSetValue(event, numVar, num);
            unit->heldItem = ITEM_COIN;
            unit->stolenCoins = num;
            pouchSetCoin(coins - num);
        }
        return EVT_RETURN_DONE;
    }
    {
        s32 item = 0;
        s32 r;
        s32 i;
#if VERSION == VERSION_EU
        s32 heldMax = GetHeldItemCount; // PAL scans only the held-item slots the bag has
#endif

        count = 0;
#if VERSION == VERSION_EU
        for (i = 0; i < heldMax; i++) {
#else
        for (i = 0; i < MAX_HELD_ITEMS; i++) {
#endif
            if (pouch->heldItems[i] != 0) {
                count++;
            }
        }
        if (count <= 0) {
            return EVT_RETURN_DONE;
        }
        r = irand(count);
        if (r >= count) {
            s32 j;

            r -= count;
            for (j = 0; j < MAX_BADGES; j++) {
                if (pouch->badges[j] != 0) {
                    if (--r < 0) {
                        item = pouch->badges[j];
                        break;
                    }
                }
            }
        } else {
            s32 k;

#if VERSION == VERSION_EU
            for (k = 0; k < heldMax; k++) {
#else
            for (k = 0; k < MAX_HELD_ITEMS; k++) {
#endif
                if (pouch->heldItems[k] != 0) {
                    if (--r < 0) {
                        item = pouch->heldItems[k];
                        break;
                    }
                }
            }
        }
        if (item != 0) {
            if (!pouchRemoveItem(item)) {
                return EVT_RETURN_DONE;
            }
            unit->heldItem = item;
            if (item >= 240 && item < 339) {
                if (target->currentKind == UNIT_MARIO) {
                    BtlUnit_EquipItem(target, EQUIP_RESET | EQUIP_MARIO, 0);
                } else if (target->currentKind >= TYPE_PARTNER_MIN && target->currentKind < TYPE_PARTNER_MAX) {
                    BtlUnit_EquipItem(target, EQUIP_RESET | EQUIP_PARTNER, 0);
                }
                pouchReviseMarioParam();
                pouchRevisePartyParam();
            }
            evtSetValue(event, itemVar, item);
            evtSetValue(event, iconVar, itemDataTable[item].icon_id);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Unit kind of the Big Bandit (HP 8, level 13):
 * its size, offsets, damage sounds, status vulnerability, parts and init event.
 */
BattleUnitKind unit_borodo_king = {
    UNIT_BIG_BANDIT, // unitType
    "btl_un_borodo_king", // name
    8, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    13, // level
    0, // bonusExp
    3, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    26, // width
    34, // height
    {0, 34}, // hitOffset
    {0.0f, 17.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {13.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {13.0f, 0.0f, 0.0f}, // bintaHitOffset
    {9.0f, 26.0f, 0.0f}, // kissHitOffset
    {0.0f, 17.0f, 0.0f}, // cutBaseOffset
    26.0f, // cutWidth
    34.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_BORODO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Weapon of the Big Bandit's charge: 3 damage, hammer-type contact attack. */
static BattleWeapon weapon_borodo_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Defence of the Big Bandit: no damage reduction. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of the Big Bandit: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Status vulnerability of the Big Bandit (percent chance to take each status): resists most statuses. */
#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {80, 80, 70, 100, 80, 100, 100, 60, 100, 80, 100, 80, 100, 80, 80, 85, 70, 100, 80, 100, 100, 95};
#else
static StatusVulnerability regist = {
    90, // sleep
    80, // stop
    60, // dizzy
    100, // poison
    90, // confuse
    100, // electric
    100, // burn
    60, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    80, // allergic
    85, // fright
    70, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_ATTACK, "A_1"},
    {POSE_TYPE_ATTACK_2, "A_2"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_RUN_HOME, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
#if VERSION <= VERSION_DEMO_US
    {POSE_TYPE_DANGER, "I_1"},
    {POSE_TYPE_PERIL, "I_1"},
#else
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
#endif
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static PoseTableEntry pose_table_item_have[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
#if VERSION <= VERSION_DEMO_US
    {POSE_TYPE_POISON, "I_1"},
    {POSE_TYPE_DIZZY, "I_1"},
#else
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
#endif
    {POSE_TYPE_NORMAL, "S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_2"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_ATTACK, "A_1"},
    {POSE_TYPE_ATTACK_2, "A_2"},
    {POSE_TYPE_RUN, "R_2"},
    {POSE_TYPE_RUN_HOME, "R_2"},
    {POSE_TYPE_WALK, "W_2"},
    {POSE_TYPE_TALK, "S_2"},
    {POSE_TYPE_ACTION, "S_2"},
};

#if VERSION >= VERSION_US
extern EvtWord _borodo_king_emergency_revival_event[];
#endif

/** Data table of the Big Bandit: event handlers for its emergency revival. */
static BattleDataEntry data_table[] = {
#if VERSION >= VERSION_US
    {UNIT_DATA_EMERGENCY_REVIVAL_EVENT, _borodo_king_emergency_revival_event},
#endif
    {0, NULL},
};

/** Body part of the Big Bandit (model c_borodo_g): hit position, defences, attributes and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_borodo_king", "c_borodo_g", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/**
 * Init event of the Big Bandit: registers its wait, attack,
 * damage and confusion events and its run and walk footstep sounds, and picks the pose table:
 * the one with the loot bag (pose_table_item_have)
 * when unit flag 0x40000000 (it already holds stolen goods) is set, the plain one otherwise.
 * Then starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), STRING("SFX_ENM_BORODO_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), STRING("SFX_ENM_BORODO_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_CheckUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_STOLEN_ITEM, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    ELSE
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_item_have))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event: runs the default battle damage reaction on the unit itself (part 1). */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/** Phase event of the Big Bandit: does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/**
 * Attack event of the Big Bandit: same as the Bandit's,
 * but a roll of 0-213 under 75 picks the plain attack (about 35%) and the rest the steal attack.
 * A Big Bandit already carrying loot runs away with escape_event.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_CheckUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_STOLEN_ITEM, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(escape_event))
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(_borodo_normal_attack))
        RETURN
    END_IF
    SET_VALUE(LW(1), 215)
    SUBTRACT(LW(1), 1)
    CALL(evt_sub_random, LW(1), LW(0))
    IF_INT_LESS(LW(0), 75)
        CALL_CHILD(PTR(_borodo_normal_attack))
    ELSE
        CALL_CHILD(PTR(_borodo_steal_attack))
    END_IF
    RETURN
EVT_END()

/**
 * The Big Bandit's plain attack: walks around behind the target, then runs at it for a shoulder charge,
 * stepping with the run sounds.
 * If the pre-check says a pre-emptive spike stops it (result 5) it still charges in and takes the damage check.
 * A miss makes it hop past and trot home; a hit knocks the Big Bandit tumbling head over heels away from the
 * target before it gets up. The weapon is weapon_borodo_attack.
 */
static EVT_BEGIN(_borodo_normal_attack)
    // Choose a target; a confused Bandit with none is flustered, and either way the turn ends.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_borodo_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // The camera frames the Bandit and its target, and the attack is announced.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_borodo_attack))

    // The Bandit walks around to a spot behind the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 60)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // It turns and runs back at the target, stepping with the run sounds.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    LOOP(8)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
    END_LOOP
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)

    // If a spike stops it before it can hit, it still charges in and takes the damage check.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY_FLOAT(LW(6), 40)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(evt_snd_sfxoff, LW(14))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF

    // The Bandit makes its shoulder charge.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_snd_sfxoff, LW(14))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // The target dodged: the Bandit hops past in two hops.
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.39941406f))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    GOTO(98)

    // The charge hits: the damage is dealt and the Bandit goes tumbling head over heels away from the target,
    // then gets up.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.25f))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 40)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("O_1"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(5), 0)
            SET_VALUE(LW(6), 360)
        ELSE
            SET_VALUE(LW(5), 360)
            SET_VALUE(LW(6), 0)
        END_IF
        CALL(btlevtcmd_SetRotateOffsetFromCenterOffset, UNIT_ID_SELF)
        CALL(evt_sub_intpl_init, EASING_SINE_IN, LW(5), LW(6), 24)
        LOOP(25)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("X_1"))
    WAIT_MS(1000)

    // It trots back home.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(_borodo_steal_attack)
    // Choose a target; a confused Bandit with none is flustered, and either way the turn ends.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_borodo_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // The camera frames the Bandit and its target, and the attack is announced.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_borodo_attack))

    // The Bandit walks around to a spot behind the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 60)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // It turns and runs back at the target, stepping with the run sounds.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    LOOP(8)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, LW(14))
        WAIT_FRAMES(3)
    END_LOOP
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)

    // If a spike stops it before it can grab anything, it still charges in and takes the damage check.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY_FLOAT(LW(6), 40)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(evt_snd_sfxoff, LW(14))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF

    // The Bandit lunges at the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ATT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_snd_sfxoff, LW(14))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // The target dodged: the Bandit hops past in two hops.
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.39941406f))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1L"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_MOVE1R"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    GOTO(98)

    // The grab lands: the damage is dealt, and unless Mario guarded well the Bandit tries to snatch an item.
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_borodo_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_borodo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SET_VALUE(LW(8), 0)
    CALL(btlevtcmd_GetResultACDefence, LW(7))
    IF_INT_LESS_OR_EQUAL(LW(7), 3)
        CALL(_steal_item, UNIT_ID_SELF, LW(3), LW(8), LW(9), LW(7))
        IF_INT_NOT_EQUAL(LW(8), 0)
            // A stolen item: the success jingle plays, a message announces what was lost
            // and the item flies off the target into the Bandit's loot.
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_SUCCESS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            BEGIN_INLINE_EVENT
                WAIT_FRAMES(20)
                CALL(btlevtcmd_AnnounceSetParam, 0, LW(8))
                CALL(btlevtcmd_AnnounceMessage, 1, 0, 0, STRING("btl_msg_steal_item_lost"), 90)
            END_INLINE_EVENT
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_BtlIconEntry, LW(9), LW(0), LW(1), LW(2), LW(10))
                CALL(btlevtcmd_BtlIconSetFallAccel, LW(10), FLOAT(0.5f))
                CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
                CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_BtlIconJumpPosition, LW(10), LW(0), LW(1), LW(2), 20)
                CALL(btlevtcmd_BtlIconDelete, LW(10))
                CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_STOLEN_ITEM)
            END_INLINE_EVENT
        END_IF
    END_IF

    // The Bandit hops back and checks its prize.
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("O_1"))
    WAIT_FRAMES(10)

    // Nothing was taken: it falls over, picks itself up, glares and trots home.
    IF_INT_EQUAL(LW(8), 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_INCLINE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("B_1"))
        WAIT_FRAMES(52)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_4"))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        GOTO(99)
    END_IF

    // Something was taken: it cackles and carries the loot bag from now on.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("O_2"))
    WAIT_FRAMES(14)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_LAUGH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_item_have))
    GOTO(98)

    // It walks back home.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Big Bandit runs away with its loot: turns its back, hops,
 * then leaves the screen to x=300 in a blur at speed 9 with the escape sound
 * and removes itself from the battle (KillUnit).
 */
static EVT_BEGIN(escape_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_WAIT1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_3"))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 180, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_JUMP1"), 0, 0, -1, -1, 0, 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(20.0f), MOVE_SOUND_KEEP)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 255, 255, 255, 255, 100, 100, 100, 50, 0)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BORODO_ESCAPE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(9.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, 300, LW(1), LW(2), 0, MOVE_SOUND_NONE, 0)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(40)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/**
 * Wait event of the Big Bandit: picks the plain or loot-bag pose table by unit flag 0x40000000
 * and sets the idle pose from it.
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_CheckUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_STOLEN_ITEM, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    ELSE
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_item_have))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
#if VERSION >= VERSION_US
/**
 * Revival when the Big Bandit holds a Life Shroom (US and later):
 * the knocked-out unit spins, uses the item, is cured of all status, recovers HP and gets back up.
 * LW(10) is the unit, LW(11) the part.
 */
EVT_BEGIN(_borodo_king_emergency_revival_event)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)
    SET_VALUE(LW(0), 48)
    LABEL(5)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(5)
    END_IF
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
    SET_VALUE(LW(0), 30)
    LABEL(6)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(6)
    END_IF
    CALL(btlevtcmd_ClearAllStatus, LW(10))
    CALL(btlevtcmd_ConsumeItemReserve, LW(10), ITEM_LIFE_SHROOM)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(btlevtcmd_DispItemIcon, LW(0), LW(1), LW(2), ITEM_LIFE_SHROOM, 120)
    END_SIBLING_EVENT
    WAIT_FRAMES(90)
    SET_VALUE(LW(0), 30)
    LABEL(7)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), -3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(7)
    END_IF
    CALL(btlevtcmd_OnOffStatus, LW(10), STATUS_INSTANT_KILL, 0, 0, 0)
    CALL(btlevtcmd_SetHp, LW(10), 0)
    CALL(btlevtcmd_RecoverHp, LW(10), LW(11), 10)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(2000)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_RUN)
    CALL(btlevtcmd_JumpSetting, LW(10), 0, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_OffUnitFlag, LW(10), BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)
    CALL(btlevtcmd_OffUnitFlag, LW(10), BATTLE_UNIT_FLAG_STOLEN_ITEM)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#endif

