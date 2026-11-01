// Battle unit Hyper Cleft (ハイパーシンエモン): moo.rel's copy.
// No formation or script of this REL uses it.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"


static BattleUnitKind unit_hyper_sinemon;
static BattleUnitDefense defence;
static BattleUnitDefense defence_turn;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_sinemon_attack;
static BattleWeapon weapon_sinemon_charge;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_turn[15];
static BattleDataEntry data_table[4];
static EvtWord init_event[42];
static EvtWord dmg_avoid_counter_thorn_head_event[76];
static EvtWord damage_event[58];
static EvtWord dmg_turn_event[96];
static EvtWord wakeup_event[104];
static EvtWord attack_event[595];
static EvtWord wait_event[6];

/**
 * Unit data of the Hyper Cleft; ties together its sounds, parts, status vulnerability, init event
 * and event table.
 */
static BattleUnitKind unit_hyper_sinemon = {
    UNIT_HYPER_CLEFT, // unitType
    "btl_un_hyper_sinemon", // name
    4, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    17, // level
    0, // bonusExp
    3, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    70, // runRate
    9999, // pbSoftCap
    26, // width
    32, // height
    {4, 32}, // hitOffset
    {0.0f, 16.0f, 0.0f}, // centerOffset
    {0}, // hpGaugeOffset
    {0}, // talkTogeBaseOffset
    {13.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0}, // burnFlameOffset
    0.0f, 0.0f, // vivianOffsetX..vivianOffsetZ
    {13.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 24.0f, 0.0f}, // kissHitOffset
    {0.0f, 16.0f, 0.0f}, // cutBaseOffset
    26.0f, // cutWidth
    32.0f, // cutHeight
    0, 0, 0, // turnOrder..swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0}, // starPointDispOffset
    "SFX_ENM_EMON_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitLeader, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    (BattleDataEntry*)data_table, // dataTable
};

#if VERSION <= VERSION_DEMO_US
static BattleUnitDefense defence = {{3, 3, 3, 3, 3}};
#else
static BattleUnitDefense defence = {{3, 99, 3, 3, 3}};
#endif

/**
 * Defence of the Hyper Cleft while it is flipped over onto its back: 0
 * against every kind of hit (set by dmg_turn_event).
 */
static BattleUnitDefense defence_turn = {0};

#if VERSION <= VERSION_DEMO_US
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

static StatusVulnerability regist = {50, 90, 50, 100, 50, 100, 0, 70, 100, 70, 100, 70, 100, 80, 80, 50, 30, 100, 80, 100, 100, 95};
#else
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_IMMUNE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

static StatusVulnerability regist = {
    40, // sleep
    90, // stop
    40, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    0, // burn
    60, // freeze
    100, // huge
    65, // tiny
    100, // attackUp
    65, // attackDown
    100, // defenseUp
    100, // defenseDown
    60, // allergic
    40, // fright
    30, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Parts of the Hyper Cleft: 1 part (model c_sinemon_h). */
static BattleUnitKindPart parts[] = {
    {
        1, // partNum
        "btl_un_hyper_sinemon", // partName
        "c_sinemon_h", // modelName
        {0}, // positionOffset
        {0.0f, 25.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        kMainPart | kGuardTargetPart | kIsBombFlippable, // attributes
        kCounterTopSpiky, // counterAttributes
        pose_table, // poseTable
    },
};

/** Weapon of the Hyper Cleft (attack): targets one unit for 3 damage; used by attack_event. */
static BattleWeapon weapon_sinemon_attack = {
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
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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

/** Weapon of the Hyper Cleft (charge): targets one unit; used by attack_event. */
static BattleWeapon weapon_sinemon_charge = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    6, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "SIN_N_1"},
    {POSE_TYPE_STOP, "SIN_Y_1"},
    {POSE_TYPE_FREEZE, "SIN_Y_1"},
    {POSE_TYPE_CONFUSE, "SIN_K_1"},
    {POSE_TYPE_POISON, "SIN_I_1"},
    {POSE_TYPE_DIZZY, "SIN_I_1"},
    {POSE_TYPE_NORMAL, "SIN_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "SIN_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "SIN_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "SIN_A_1a"},
    {POSE_TYPE_DAMAGE, "SIN_D_1"},
    {POSE_TYPE_ATTACK, "SIN_A_1a"},
    {POSE_TYPE_WALK, "SIN_W_1"},
    {POSE_TYPE_RUN, "SIN_R_1"},
    {POSE_TYPE_DANGER, "SIN_I_1"},
    {POSE_TYPE_PERIL, "SIN_I_1"},
    {POSE_TYPE_TALK, "SIN_T_1"},
    {POSE_TYPE_ACTION, "SIN_S_1"},
};

/** Poses of the Hyper Cleft while it lies flipped over on its back (set by dmg_turn_event). */
static PoseTableEntry pose_table_turn[] = {
    {POSE_TYPE_SLEEP, "SIN_N_2"},
    {POSE_TYPE_STOP, "SIN_Y_2"},
    {POSE_TYPE_FREEZE, "SIN_Y_2"},
    {POSE_TYPE_CONFUSE, "SIN_K_2"},
    {POSE_TYPE_POISON, "SIN_K_2"},
    {POSE_TYPE_DIZZY, "SIN_K_2"},
    {POSE_TYPE_NORMAL, "SIN_D_2"},
    {POSE_TYPE_DAMAGE_FIRE, "SIN_D_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "SIN_D_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "SIN_D_2"},
    {POSE_TYPE_DAMAGE, "SIN_D_2"},
    {POSE_TYPE_DANGER, "SIN_K_2"},
    {POSE_TYPE_PERIL, "SIN_K_2"},
    {POSE_TYPE_TALK, "SIN_D_2"},
    {POSE_TYPE_ACTION, "SIN_D_2"},
};

/** Event table of the Hyper Cleft: dmg_turn_event, dmg_avoid_counter_thorn_head_event. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_head_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), STRING("SFX_ENM_EMON_MOVE2"), 0, 10, 10)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), STRING("SFX_ENM_EMON_MOVE2"), 0, 7, 7)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_EMON_JUMP1"), STRING("SFX_ENM_EMON_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Spark effect (toge_flush) shown when an attack on the Hyper Cleft's spiked head is turned away:
 * it waits 30 frames, then places the flash above the head at a height that depends on whether the unit has
 * STATUS_HUGE or STATUS_TINY.
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
        ADD(LW(1), 45)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 7)
        ADD(LW(1), 25)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
    ADD(LW(1), 35)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

/**
 * Damage event of the Hyper Cleft: shows the hurt pose (SIN_D_1) when damage was dealt;
 * while it lies flipped over (over-turn count) it shows SIN_D_2 and hops twice.
 * Then it runs the default damage handling.
 */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_GetDamage, UNIT_ID_SELF, LW(15))
    IF_INT_GREATER_OR_EQUAL(LW(15), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SIN_D_1"))
    END_IF
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SIN_D_2"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 5, MOVE_SOUND_FROM_TABLE)
    END_IF
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Reaction when the Hyper Cleft is flipped over:
 * switches to the flipped defence and pose tables, sets the over-turn count to 2 (SFX_ENM_INSIDE1), hops,
 * and rests on its back at y = 7 with the hit offset lowered and its counter attribute turned off.
 */
static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("SIN_D_2"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 7, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 7, LW(2))
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, -10, 0)
    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
    RETURN
EVT_END()

/**
 * Recovery of the Hyper Cleft at the start of its turn:
 * counts the over-turn count down and, when it reaches 0, restores the normal defence and pose tables,
 * ground position and counter attribute with SFX_ENM_INSIDE2 and a hop;
 * while still flipped it just goes back to waiting.
 */
static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("SIN_S_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, 0, 0)
    CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Attack event of the Hyper Cleft: gets up first if it is flipped over;
 * on a field first strike it goes straight for the first attack target, otherwise,
 * unless it is already charged (STATUS_CHARGE), it may use an enemy item
 * or draw lots (25% on turn 1, 25/25 later) to spend the turn charging (weapon_sinemon_charge, crystal effect).
 * Otherwise it dashes at a target (SFX_ENM_EMON_DASH1);
 * the pre-damage check decides between a dodge and a hit, then it hops back home.
 */
static EVT_BEGIN(attack_event)
    // If the Hyper Cleft is lying flipped on its back, it spends the turn getting back up (wakeup_event).
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF

    // In a first strike it goes straight for the first-strike target.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
        WAIT_MS(800)
    ELSE
        // Unless it is already charged, it may use a held item.
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CHARGE, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            GOTO(10)
        END_IF
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF

        // Then it draws lots (25% on the first turn, 25% after that) to spend the turn charging up instead of
        // attacking.
        CALL(btlevtcmd_get_turn, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_DrawLots, LW(0), 2, 25, 75)
        ELSE
            CALL(btlevtcmd_DrawLots, LW(0), 2, 25, 25)
        END_IF
        IF_INT_EQUAL(LW(0), 1)
            SET_VALUE(LW(9), PTR(&weapon_sinemon_charge))
            CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
            CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            CALL(evt_btl_camera_set_zoom, 0, 300)
            WAIT_FRAMES(40)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            ADD(LW(1), 15)
            ADD(LW(2), 10)

            // It charges up with a crystal-like glow for a second, then hops on the spot; the turn is over.
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(evt_eff64, STRING(""), STRING("crystal_n64"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 60, 0, 0, 0, 0, 0, 0)
            WAIT_MS(1000)
            BEGIN_INLINE_EVENT
                CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, 0, FLOAT(1.0f))
                CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
                WAIT_MS(500)
                CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            END_INLINE_EVENT
            CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
            RETURN
        END_IF

        // Otherwise it picks a target for its dash attack.
        LABEL(10)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sinemon_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sinemon_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sinemon_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sinemon_attack))

    // It lines up with the target's row, walking with a footstep sound.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 100)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
        END_LOOP
    END_SIBLING_EVENT

    // It crouches to wind up while the camera pulls in.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(750)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)

    // The dash: it rushes at the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_DASH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(5), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sinemon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // The dash misses or is blocked: it rushes past the target, then stops after a short pause.
    LABEL(90)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    WAIT_MS(500)
    GOTO(98)

    // The dash hits: the camera shakes, the Action Command defence result is applied, damage is dealt, and it
    // bounces back in two hops.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sinemon_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sinemon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(1.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 75)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.5f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 100)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_EMON_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)

    // It walks back to its home position.
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

/** Wait (idle) event of the Hyper Cleft: shows the idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

