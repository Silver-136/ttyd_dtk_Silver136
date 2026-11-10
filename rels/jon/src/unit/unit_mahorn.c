// Pit of 100 Trials battle unit: the Wizzerd (`mahorn`): parts, stats, poses and AI scripts.

#include "version.h"
#include "eff/eff_mahorn2.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "system.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "battle/battle_damage.h"
#include "battle/unit/unit_mahorn.h"

/** Absolute value of a double. */
static inline double mahorn_fabs(double x) { return __fabs(x); }

static USER_FUNC(hand_ctrl);
static USER_FUNC(hand_init);
static USER_FUNC(mahorn_get_bunsin);
static USER_FUNC(set_hand_dest);
static USER_FUNC(eff_magic_get_posX);
static USER_FUNC(eff_magic);
static USER_FUNC(hand_end);

BattleUnitKind unit_mahorn;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[5];
static BattleWeapon weapon_mahorn_attack_magic;
static BattleWeapon weapon_mahorn_all_attack_magic;
static BattleWeapon weapon_mahorn_dekadeka_magic;
static BattleWeapon weapon_mahorn_katikati_magic;
static BattleWeapon weapon_mahorn_confuse_magic;
static BattleWeapon weapon_mahorn_recover_magic;
static EvtWord attack_event[152];
static PoseTableEntry pose_table[17];
static BattleDataEntry data_table[3];
static BattleDataEntry data_table_bunsin[3];
static EvtWord init_event[88];
static EvtWord alpha_down_event[98];
static EvtWord alpha_up_event[118];
static EvtWord hand_ctrl_event[23];
static EvtWord damage_event[10];
static EvtWord avoid_miss_event[2];
static EvtWord attack_punch_event[429];
static EvtWord start_avoid[26];
static EvtWord attack_enemy_common_event[299];
static EvtWord attack_enemy_all_common_event[189];
static EvtWord attack_enemy_common_event_sub1[90];
static EvtWord attack_enemy_common_event_sub2[231];
static EvtWord attack_enemy_common_event_sub_no_hit[65];
static EvtWord attack_enemy_common_event_sub_hit[50];
static EvtWord attack_magic_event[54];
static EvtWord confuse_magic_event[41];
static EvtWord all_attack_magic_event[41];
static EvtWord support_friend_common_event[227];
static EvtWord dekadeka_magic_event[75];
static EvtWord katikati_magic_event[75];
static EvtWord trans_magic_event[75];
static EvtWord yokeyoke_magic_event[75];
static EvtWord recover_magic_event[96];
static EvtWord wait_event[386];

/** Unit data of the Wizzerd: stats, sounds, attributes, parts, init script and data table. */
BattleUnitKind unit_mahorn = {
    UNIT_WIZZERD, // unitType
    "btl_un_mahorn", // name
    10, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    30, // level
    1, // bonusExp
    4, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    48, // width
    44, // height
    {0, 44}, // hitOffset
    {0.0f, 22.0f, 0.0f}, // centerOffset
    {0, -9}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {24.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {24.0f, 0.0f, 0.0f}, // bintaHitOffset
    {16.0f, 28.6f, 10.0f}, // kissHitOffset
    {0.0f, 22.0f, 0.0f}, // cutBaseOffset
    48.0f, // cutWidth
    44.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    0, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_MAHO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating, // attributeFlags
    &regist, // statusVulnerability
    5, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Damage reduction of the Wizzerd: defence 3 against every attack type. */
static BattleUnitDefense defence = {{3, 3, 3, 3, 3}};

/** Element response of the Wizzerd: none, every attack type is treated normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {10, 50, 50, 100, 40, 100, 100, 40, 100, 40, 100, 40, 100, 40, 40, 30, 75, 100, 40, 100, 100, 40};
#else
static StatusVulnerability regist = {
    10, // sleep
    50, // stop
    90, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    50, // freeze
    100, // huge
    85, // tiny
    100, // attackUp
    85, // attackDown
    100, // defenseUp
    90, // defenseDown
    40, // allergic
    30, // fright
    85, // galeForce
    100, // fast
    40, // slow
    100, // dodgy
    100, // invisible
    40, // ohko
};
#endif

/** Body parts of the Wizzerd: hit position, size, defence tables and attributes. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_mahorn", "c_maho", 0.0f, 0.0f, 0.0f, 0.0f, 30.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_mahorn", "c_maho", 0.0f, 0.0f, 0.0f, 5.0f, 30.0f, 0.0f, 0.0f, 35.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kNotTargetable | kNoShadow, 0, pose_table},
    {3, "btl_un_mahorn", "c_maho", 0.0f, 0.0f, 0.0f, 5.0f, 30.0f, 0.0f, 0.0f, 35.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kNotTargetable | kNoShadow, 0, pose_table},
    {4, "btl_un_mahorn", "c_maho", 0.0f, 0.0f, 0.0f, 5.0f, 30.0f, 0.0f, 0.0f, 35.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kNotTargetable | kNoShadow, 0, pose_table},
    {5, "btl_un_mahorn", "c_maho", 0.0f, 0.0f, 0.0f, 5.0f, 30.0f, 0.0f, 0.0f, 35.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kNotTargetable | kNoShadow, 0, pose_table},
};

/** Attack data for the Wizzerd's single-target magic: 6 damage. */
static BattleWeapon weapon_mahorn_attack_magic = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
#if VERSION <= VERSION_DEMO_JP
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_IGNORE_VULNERABILITY,
#else
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH, // special
#endif
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/** Attack data for the Wizzerd's magic on all enemies: 6 damage. */
static BattleWeapon weapon_mahorn_all_attack_magic = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#if VERSION >= VERSION_US
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ELECTRIC, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/** Attack data for the Wizzerd's size-up spell: no damage, 100% size change. */
static BattleWeapon weapon_mahorn_dekadeka_magic = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, // burnTime..freezeTime
    100, // sizeChangeChance
    3, // sizeChangeTime
    2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Attack data for the Wizzerd's defence-up spell: no damage, 100% defence change. */
static BattleWeapon weapon_mahorn_katikati_magic = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    3, // defChangeTime
    3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Attack data for the Wizzerd's confusion spell: no damage, 50% Confusion. */
static BattleWeapon weapon_mahorn_confuse_magic = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
#if VERSION <= VERSION_DEMO_JP
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_IGNORE_VULNERABILITY,
#else
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
#endif
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, // poisonStrength
    50, // confuseChance
    3, // confuseTime
    0, 0, 0, 0, 0, 0, 0, 0, // electricChance..freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Attack data for the Wizzerd's healing spell: no damage. */
static BattleWeapon weapon_mahorn_recover_magic = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, 0, // acHelpMsg..special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP | TARGET_WEIGHT_LOW_HP_RATIO, // targetWeightFlags
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

/**
 * Attack script of the Wizzerd. On a first strike it casts its single-target magic.
 * Otherwise, after an item check, it draws a spell:
 * single-target magic (50 in 120), magic on all enemies (30),
 * and 10 each for the size-up spell on an ally (dekadeka), the defence-up spell (katikati),
 * the confusion spell and the heal (recover).
 * The invisibility (trans) and dodgy (yokeyoke) spells are never picked here.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(9), PTR(&weapon_mahorn_attack_magic))
        CALL_CHILD(attack_magic_event)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    LABEL(10)
    SET_VALUE(LW(0), 0)
    ADD(LW(0), 50)
    ADD(LW(0), 30)
    ADD(LW(0), 10)
    ADD(LW(0), 10)
    ADD(LW(0), 10)
    ADD(LW(0), 10)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 50)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(9), PTR(&weapon_mahorn_attack_magic))
        CALL_CHILD(attack_magic_event)
        GOTO(99)
    END_IF
    ADD(LW(0), 30)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(9), PTR(&weapon_mahorn_all_attack_magic))
        CALL_CHILD(all_attack_magic_event)
        GOTO(99)
    END_IF
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(9), PTR(&weapon_mahorn_dekadeka_magic))
        CALL_CHILD(dekadeka_magic_event)
        GOTO(99)
    END_IF
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(9), PTR(&weapon_mahorn_katikati_magic))
        CALL_CHILD(katikati_magic_event)
        GOTO(99)
    END_IF
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(9), PTR(&weapon_mahorn_confuse_magic))
        CALL_CHILD(confuse_magic_event)
        GOTO(99)
    END_IF
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        SET_VALUE(LW(9), PTR(&weapon_mahorn_recover_magic))
        CALL_CHILD(recover_magic_event)
        GOTO(99)
    END_IF
    GOTO(10)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Pose table of the Wizzerd: battle pose types mapped to animation names. */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "MAH_N_1"},
    {POSE_TYPE_STOP, "MAH_Y_1"},
    {POSE_TYPE_FREEZE, "MAH_Y_1"},
    {POSE_TYPE_CONFUSE, "MAH_K_1"},
    {POSE_TYPE_POISON, "MAH_X_1"},
    {POSE_TYPE_DIZZY, "MAH_X_1"},
    {POSE_TYPE_NORMAL, "MAH_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "MAH_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "MAH_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "MAH_A_1"},
    {POSE_TYPE_DAMAGE, "MAH_D_1"},
    {POSE_TYPE_WALK, "MAH_W_1"},
    {POSE_TYPE_RUN, "MAH_R_1"},
    {POSE_TYPE_DANGER, "MAH_X_1"},
    {POSE_TYPE_PERIL, "MAH_X_1"},
    {POSE_TYPE_TALK, "MAH_T_1"},
    {POSE_TYPE_ACTION, "MAH_S_1"},
};

/** Data table of the Wizzerd: event handlers keyed by tag (avoid_miss_event). */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/** Data table of the Wizzerd, clone: event handlers keyed by tag (avoid_miss_event). */
static BattleDataEntry data_table_bunsin[] = {
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/**
 * Init script of the Wizzerd: registers its wait, attack, damage and confusion handlers,
 * lifts its home position to y 20, sets up its four floating hands (hand_init) with their resting offsets,
 * starts hand_ctrl_event to animate them, and clears UNIT_WORK_BUNSIN (set on its copies) and UNIT_WORK_FADING.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 20, LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(hand_init, UNIT_ID_SELF)
    CALL(set_hand_dest, UNIT_ID_SELF, 2, FLOAT(-25.0f), FLOAT(30.0f), FLOAT(-10.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 3, FLOAT(25.0f), FLOAT(30.0f), FLOAT(10.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 4, FLOAT(-25.0f), FLOAT(10.0f), FLOAT(-10.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 5, FLOAT(25.0f), FLOAT(10.0f), FLOAT(10.0f))
    SPAWN_EVENT(hand_ctrl_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FADING, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Fades the unit in LW(0) out over 30 frames (all five body parts) and then hides all its parts.
 * A unit id of -1 does nothing. Used when the Wizzerd vanishes to make or remove copies.
 */
static EVT_BEGIN(alpha_down_event)
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 254)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, PART_ANIM_CHANGED, 1)
    WAIT_FRAMES(1)
    CALL(evt_sub_intpl_init, EASING_LINEAR, 255, 0, 30)
    LOOP(30)
        CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
        CALL(btlevtcmd_SetAlpha, LW(0), 1, LW(1))
        CALL(btlevtcmd_SetAlpha, LW(0), 2, LW(1))
        CALL(btlevtcmd_SetAlpha, LW(0), 3, LW(1))
        CALL(btlevtcmd_SetAlpha, LW(0), 4, LW(1))
        CALL(btlevtcmd_SetAlpha, LW(0), 5, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_OnPartsAttribute, LW(0), 1, kNoShadow)
    CALL(btlevtcmd_OnPartsAttribute, LW(0), 2, kNoShadow)
    CALL(btlevtcmd_OnPartsAttribute, LW(0), 3, kNoShadow)
    CALL(btlevtcmd_OnPartsAttribute, LW(0), 4, kNoShadow)
    CALL(btlevtcmd_OnPartsAttribute, LW(0), 5, kNoShadow)
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 0)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, PART_ANIM_CHANGED, 1)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

/**
 * Fades the unit in LW(0) back in over 30 frames (all five body parts) after showing its parts again;
 * a unit id of -1 does nothing.
 */
static EVT_BEGIN(alpha_up_event)
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_OffPartsAttribute, LW(0), 1, kNoShadow)
    CALL(btlevtcmd_OffPartsAttribute, LW(0), 2, kNoShadow)
    CALL(btlevtcmd_OffPartsAttribute, LW(0), 3, kNoShadow)
    CALL(btlevtcmd_OffPartsAttribute, LW(0), 4, kNoShadow)
    CALL(btlevtcmd_OffPartsAttribute, LW(0), 5, kNoShadow)
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 0)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, PART_ANIM_CHANGED, 1)
    WAIT_FRAMES(1)
    CALL(evt_sub_intpl_init, EASING_LINEAR, 0, 254, 30)
    LOOP(30)
        CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
        CALL(btlevtcmd_SetAlpha, LW(0), 1, LW(1))
        CALL(btlevtcmd_SetAlpha, LW(0), 2, LW(1))
        CALL(btlevtcmd_SetAlpha, LW(0), 3, LW(1))
        CALL(btlevtcmd_SetAlpha, LW(0), 4, LW(1))
        CALL(btlevtcmd_SetAlpha, LW(0), 5, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 255)
    CALL(btlevtcmd_SetAlpha, LW(0), 2, 255)
    CALL(btlevtcmd_SetAlpha, LW(0), 3, 255)
    CALL(btlevtcmd_SetAlpha, LW(0), 4, 255)
    CALL(btlevtcmd_SetAlpha, LW(0), 5, 255)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, PART_ANIM_CHANGED, 1)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

/**
 * Background loop of the Wizzerd, run for as long as it exists:
 * each frame it moves the floating hands towards their targets (hand_ctrl).
 */
static EVT_BEGIN(hand_ctrl_event)
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(hand_ctrl, UNIT_ID_SELF)
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

/** Damage script of the Wizzerd: runs the default damage handling on itself. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Empty script (just returns). */
static EVT_BEGIN(avoid_miss_event)
    RETURN
EVT_END()

/**
 * The Wizzerd's punch flurry: it moves next to the target (35 units to the side), spreads its four hands,
 * which blur (SFX_ENM_MAHO_ATTACK2), and has them dart to random spots around the target for 25 rounds,
 * then pulls them back. The target is checked and takes the damage. It then returns home.
 * After a first strike it uses the first-strike target.
 */
static EVT_BEGIN(attack_punch_event)
    // Pick the target (after a First Strike the one already chosen, otherwise sampled from the enemies); a
    // confused Wizzerd with nobody to hit gets flustered.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))

    // Declare the attack.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(set_hand_dest, UNIT_ID_SELF, 2, FLOAT(-16.0f), FLOAT(29.0f), FLOAT(-5.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 3, FLOAT(16.0f), FLOAT(29.0f), FLOAT(5.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 4, FLOAT(-16.0f), FLOAT(11.0f), FLOAT(-5.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 5, FLOAT(16.0f), FLOAT(11.0f), FLOAT(5.0f))

    // It floats next to the target and spreads its four hands.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    IF_INT_EQUAL(LW(15), -1)
        ADD(LW(0), 35)
    ELSE
        SUBTRACT(LW(0), 35)
    END_IF
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 4, FLOAT(0.0009765625f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 20, LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MAH_S_1"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("MAH_Z_2"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("MAH_Z_2"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("MAH_Z_2"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("MAH_Z_2"))
    SET_VALUE(LW(10), 2)
    LOOP(4)
        CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, LW(10), 255, 255, 255, 255, 255, 255, 255, 100, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(10), kMotionBlur)
        ADD(LW(10), 1)
    END_LOOP

    // Its hands blur and dart to random spots around the target in a flurry of punches.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(25)
        SET_VALUE(LW(10), 2)
        LOOP(4)
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(10), LW(0), LW(1), LW(2))
            IF_INT_EQUAL(LW(15), -1)
                CALL(evt_sub_random, 120, LW(11))
                ADD(LW(11), -90)
            ELSE
                CALL(evt_sub_random, 120, LW(11))
                MULTIPLY(LW(11), -1)
                ADD(LW(11), 90)
            END_IF
            CALL(evt_sub_random, 60, LW(12))
            ADD(LW(12), -20)
            CALL(set_hand_dest, UNIT_ID_SELF, LW(10), LW(11), LW(12), LW(2))
            ADD(LW(10), 1)
        END_LOOP
        WAIT_FRAMES(2)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MAH_S_1"))
    CALL(set_hand_dest, UNIT_ID_SELF, 2, FLOAT(-16.0f), FLOAT(29.0f), FLOAT(-5.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 3, FLOAT(16.0f), FLOAT(29.0f), FLOAT(5.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 4, FLOAT(-16.0f), FLOAT(11.0f), FLOAT(-5.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 5, FLOAT(16.0f), FLOAT(11.0f), FLOAT(5.0f))
    SET_VALUE(LW(10), 2)
    LOOP(4)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(10), kMotionBlur)
        ADD(LW(10), 1)
    END_LOOP

    // The hands pull back; find out whether the flurry connects.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
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
        LABEL(90)
        WAIT_FRAMES(30)
        GOTO(98)

        // Hit: the damage is dealt.
        LABEL(91)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        WAIT_FRAMES(30)
        GOTO(98)

        // It floats back home.
        LABEL(98)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_ATTACK1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 6, FLOAT(0.0009765625f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(45)
        LABEL(99)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
        RETURN
EVT_END()

/**
 * Plays the avoid reaction on the target in LW(3) (damage code 38, 39 or 40) according to the pre-damage result in
 * LW(5): 3, 6 or 2.
 */
static EVT_BEGIN(start_avoid)
    IF_INT_EQUAL(LW(5), 3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
    END_IF
    IF_INT_EQUAL(LW(5), 6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
    END_IF
    IF_INT_EQUAL(LW(5), 2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
    END_IF
    RETURN
EVT_END()

/**
 * The Wizzerd's single-target spell: it and its copies (found by mahorn_get_bunsin) all charge up
 * (SFX_ENM_MAHO_CHARGE1, attack_enemy_common_event_sub1) and cast (SFX_ENM_MAHO_MAGIC2);
 * on a miss, all their orbs fly past the target to the edge of the stage (attack_enemy_common_event_sub_no_hit).
 * On a hit, all their orbs fly towards the target's x position (attack_enemy_common_event_sub_hit)
 * and the damage is dealt. On a miss, any avoid reaction is timed to the orb reaching the target.
 */
static EVT_BEGIN(attack_enemy_common_event)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))

    // Declare the attack.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // The Wizzerd (and each of its copies) charges up its magic.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(mahorn_get_bunsin, LW(11), LW(12), LW(13), LW(14), LW(0))
    SET_VALUE(LW(15), LW(11))
    SPAWN_EVENT(attack_enemy_common_event_sub1)
    SET_VALUE(LW(15), LW(12))
    SPAWN_EVENT(attack_enemy_common_event_sub1)
    SET_VALUE(LW(15), LW(13))
    SPAWN_EVENT(attack_enemy_common_event_sub1)
    SET_VALUE(LW(15), LW(14))
    SPAWN_EVENT(attack_enemy_common_event_sub1)
    SET_VALUE(LW(15), -2)
    CALL_CHILD(attack_enemy_common_event_sub1)

    // Then it casts: a magic orb heads for the target.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(25)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT

    // Find out whether the spell reaches the target.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
        // Miss or dodge: any avoid reaction is timed to the orb's arrival as the orbs fly on past.
        LABEL(90)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(60)
            LOOP(0)
                CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
                IF_INT_EQUAL(LW(0), -1)
                    CALL(eff_magic_get_posX, UNIT_ID_SELF, LW(0))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                    IF_INT_LESS_OR_EQUAL(LW(0), LW(6))
                        CALL_CHILD(start_avoid)
                        BREAK_LOOP
                    END_IF
                ELSE
                    CALL(eff_magic_get_posX, UNIT_ID_SELF, LW(0))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                    IF_INT_GREATER_OR_EQUAL(LW(0), LW(6))
                        CALL_CHILD(start_avoid)
                        BREAK_LOOP
                    END_IF
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(mahorn_get_bunsin, LW(11), LW(12), LW(13), LW(14), LW(0))
        SET_VALUE(LW(15), LW(11))
        SPAWN_EVENT(attack_enemy_common_event_sub_no_hit)
        SET_VALUE(LW(15), LW(12))
        SPAWN_EVENT(attack_enemy_common_event_sub_no_hit)
        SET_VALUE(LW(15), LW(13))
        SPAWN_EVENT(attack_enemy_common_event_sub_no_hit)
        SET_VALUE(LW(15), LW(14))
        SPAWN_EVENT(attack_enemy_common_event_sub_no_hit)
        SET_VALUE(LW(15), -2)
        CALL_CHILD(attack_enemy_common_event_sub_no_hit)
        GOTO(98)

        // Hit: the real orb and the copies' orbs head towards the target's x position, and the damage is dealt.
        LABEL(91)
        CALL(mahorn_get_bunsin, LW(11), LW(12), LW(13), LW(14), LW(0))
        SET_VALUE(LW(15), LW(11))
        SPAWN_EVENT(attack_enemy_common_event_sub_hit)
        SET_VALUE(LW(15), LW(12))
        SPAWN_EVENT(attack_enemy_common_event_sub_hit)
        SET_VALUE(LW(15), LW(13))
        SPAWN_EVENT(attack_enemy_common_event_sub_hit)
        SET_VALUE(LW(15), LW(14))
        SPAWN_EVENT(attack_enemy_common_event_sub_hit)
        SET_VALUE(LW(15), -2)
        CALL_CHILD(attack_enemy_common_event_sub_hit)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(98)

        // The copies go back to waiting.
        LABEL(98)
        CALL(mahorn_get_bunsin, LW(11), LW(12), LW(13), LW(14), LW(0))
        IF_INT_NOT_EQUAL(LW(11), -1)
            CALL(btlevtcmd_StartWaitEvent, LW(11))
        END_IF
        IF_INT_NOT_EQUAL(LW(12), -1)
            CALL(btlevtcmd_StartWaitEvent, LW(12))
        END_IF
        IF_INT_NOT_EQUAL(LW(13), -1)
            CALL(btlevtcmd_StartWaitEvent, LW(13))
        END_IF
        IF_INT_NOT_EQUAL(LW(14), -1)
            CALL(btlevtcmd_StartWaitEvent, LW(14))
        END_IF
        WAIT_FRAMES(30)
        RETURN
EVT_END()

/**
 * The Wizzerd's spell on all enemies: it charges up (SFX_ENM_MAHO_CHARGE1, attack_enemy_common_event_sub1),
 * casts (attack_enemy_common_event_sub2) and then a "thunder_n64" bolt strikes each target in turn,
 * damaging those hit (the defence action command result is taken once, at the first target hit).
 */
static EVT_BEGIN(attack_enemy_all_common_event)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(15), -2)
    CALL_CHILD(attack_enemy_common_event_sub1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(25)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    SET_VALUE(LW(15), -2)
    CALL_CHILD(attack_enemy_common_event_sub2)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(6), 0)
    LABEL(10)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(evt_eff64, STRING(""), STRING("thunder_n64"), 4, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
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
        LABEL(90)
        GOTO(97)
        LABEL(91)
        IF_INT_EQUAL(LW(6), 0)
            CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
            SET_VALUE(LW(6), 1)
        END_IF
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(97)
        LABEL(97)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(98)
        END_IF
        GOTO(10)
        LABEL(98)
        WAIT_FRAMES(30)
        RETURN
EVT_END()

/**
 * Charge-up of the Wizzerd in LW(15) (-2 for itself):
 * it stops its wait script, an "akari_charge_n64" glow appears over it,
 * it takes the MAH_A_4 pose and raises its four hands (MAH_S_5A), then waits 50 frames.
 */
static EVT_BEGIN(attack_enemy_common_event_sub1)
    IF_INT_EQUAL(LW(15), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_StopWaitEvent, LW(15))
    CALL(btlevtcmd_GetPos, LW(15), LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    CALL(evt_eff64, STRING(""), STRING("akari_charge_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 70, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(15), 1, STRING("MAH_A_4"))
    CALL(btlevtcmd_AnimeChangePose, LW(15), 2, STRING("MAH_S_5A"))
    CALL(btlevtcmd_AnimeChangePose, LW(15), 3, STRING("MAH_S_5A"))
    CALL(btlevtcmd_AnimeChangePose, LW(15), 4, STRING("MAH_S_5A"))
    CALL(btlevtcmd_AnimeChangePose, LW(15), 5, STRING("MAH_S_5A"))
    CALL(set_hand_dest, LW(15), 2, FLOAT(-16.0f), FLOAT(29.0f), FLOAT(-5.0f))
    CALL(set_hand_dest, LW(15), 3, FLOAT(16.0f), FLOAT(29.0f), FLOAT(5.0f))
    CALL(set_hand_dest, LW(15), 4, FLOAT(-16.0f), FLOAT(11.0f), FLOAT(-5.0f))
    CALL(set_hand_dest, LW(15), 5, FLOAT(16.0f), FLOAT(11.0f), FLOAT(5.0f))
    WAIT_FRAMES(50)
    RETURN
EVT_END()

/**
 * Casting gesture of the Wizzerd in LW(15) (-2 for itself):
 * in pose MAH_A_1 its four hands sweep to and fro in turn, 20 units each way, offset by 7,
 * 10 and 15 frames from each other, while the main script waits 55 frames.
 */
static EVT_BEGIN(attack_enemy_common_event_sub2)
    IF_INT_EQUAL(LW(15), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_StopWaitEvent, LW(15))
    CALL(btlevtcmd_AnimeChangePose, LW(15), 1, STRING("MAH_A_1"))
    BEGIN_INLINE_EVENT
        LOOP(2)
            SET_FLOAT_VALUE(LW(0), FLOAT(-16.0f))
            SET_FLOAT_VALUE(LW(1), FLOAT(20.0f))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(2))
            MULTIPLY_FLOAT(LW(1), LW(2))
            CALL(btlevtcmd_AnimeChangePose, LW(15), 2, STRING("MAH_S_2B"))
            ADD(LW(0), LW(1))
            CALL(set_hand_dest, LW(15), 2, LW(0), FLOAT(29.0f), FLOAT(-5.0f))
            WAIT_FRAMES(20)
            CALL(btlevtcmd_AnimeChangePose, LW(15), 2, STRING("MAH_S_2A"))
            SUBTRACT(LW(0), LW(1))
            CALL(set_hand_dest, LW(15), 2, LW(0), FLOAT(29.0f), FLOAT(-5.0f))
            WAIT_FRAMES(20)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(7)
        LOOP(2)
            SET_FLOAT_VALUE(LW(0), FLOAT(16.0f))
            SET_FLOAT_VALUE(LW(1), FLOAT(20.0f))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(2))
            MULTIPLY_FLOAT(LW(1), LW(2))
            CALL(btlevtcmd_AnimeChangePose, LW(15), 5, STRING("MAH_S_2B"))
            ADD(LW(0), LW(1))
            CALL(set_hand_dest, LW(15), 5, LW(0), FLOAT(11.0f), FLOAT(5.0f))
            WAIT_FRAMES(20)
            CALL(btlevtcmd_AnimeChangePose, LW(15), 5, STRING("MAH_S_2A"))
            SUBTRACT(LW(0), LW(1))
            CALL(set_hand_dest, LW(15), 5, LW(0), FLOAT(11.0f), FLOAT(5.0f))
            WAIT_FRAMES(20)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(10)
        LOOP(2)
            SET_FLOAT_VALUE(LW(0), FLOAT(-16.0f))
            SET_FLOAT_VALUE(LW(1), FLOAT(20.0f))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(2))
            MULTIPLY_FLOAT(LW(1), LW(2))
            CALL(btlevtcmd_AnimeChangePose, LW(15), 4, STRING("MAH_S_2B"))
            ADD(LW(0), LW(1))
            CALL(set_hand_dest, LW(15), 4, LW(0), FLOAT(11.0f), FLOAT(-5.0f))
            WAIT_FRAMES(20)
            CALL(btlevtcmd_AnimeChangePose, LW(15), 4, STRING("MAH_S_2A"))
            SUBTRACT(LW(0), LW(1))
            CALL(set_hand_dest, LW(15), 4, LW(0), FLOAT(11.0f), FLOAT(-5.0f))
            WAIT_FRAMES(20)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(15)
        LOOP(2)
            SET_FLOAT_VALUE(LW(0), FLOAT(16.0f))
            SET_FLOAT_VALUE(LW(1), FLOAT(20.0f))
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(2))
            MULTIPLY_FLOAT(LW(1), LW(2))
            CALL(btlevtcmd_AnimeChangePose, LW(15), 3, STRING("MAH_S_2B"))
            ADD(LW(0), LW(1))
            CALL(set_hand_dest, LW(15), 3, LW(0), FLOAT(29.0f), FLOAT(5.0f))
            WAIT_FRAMES(20)
            CALL(btlevtcmd_AnimeChangePose, LW(15), 3, STRING("MAH_S_2A"))
            SUBTRACT(LW(0), LW(1))
            CALL(set_hand_dest, LW(15), 3, LW(0), FLOAT(29.0f), FLOAT(5.0f))
            WAIT_FRAMES(20)
        END_LOOP
    END_INLINE_EVENT
    WAIT_FRAMES(55)
    RETURN
EVT_END()

/**
 * A missed spell: the Wizzerd in LW(15), original or copy, sends its magic orb (eff_magic, 40 frames) to the far edge of the stage
 * and does the casting gesture (attack_enemy_common_event_sub2) with SFX_ENM_MAHO_MAGIC1 and MAGIC3,
 * so it looks like a real attack.
 */
static EVT_BEGIN(attack_enemy_common_event_sub_no_hit)
    IF_INT_EQUAL(LW(15), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetPos, LW(15), LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_GetStageSize, LW(10), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, LW(15), LW(13))
    MULTIPLY(LW(10), LW(13))
    SUBTRACT(LW(11), 5)
    CALL(eff_magic, LW(15), LW(10), LW(11), LW(12), 40)
    CALL_CHILD(attack_enemy_common_event_sub2)
    WAIT_FRAMES(5)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(40)
    RETURN
EVT_END()

/**
 * A hit spell: the Wizzerd in LW(15), original or copy, sends its magic orb (eff_magic, 20 frames) towards the target's hit-position x
 * and does the casting gesture (attack_enemy_common_event_sub2) with SFX_ENM_MAHO_MAGIC1 and MAGIC3.
 */
static EVT_BEGIN(attack_enemy_common_event_sub_hit)
    IF_INT_EQUAL(LW(15), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetPos, LW(15), LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(eff_magic, LW(15), LW(10), LW(11), LW(12), 20)
    CALL_CHILD(attack_enemy_common_event_sub2)
    WAIT_FRAMES(5)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    RETURN
EVT_END()

/**
 * The Wizzerd's single-target magic attack: picks a target (the first-strike target on a first strike),
 * casts the spell (attack_enemy_common_event) and resets its facing.
 * When confused with no target it is flustered.
 */
static EVT_BEGIN(attack_magic_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL_CHILD(attack_enemy_common_event)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Wizzerd's confusion spell: picks a target,
 * casts the single-target spell (attack_enemy_common_event) with the confuse weapon in LW(9) (50 percent chance,
 * 3 turns) and resets its facing. When confused with no target it is flustered.
 */
static EVT_BEGIN(confuse_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL_CHILD(attack_enemy_common_event)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Wizzerd's magic attack on all enemies: picks the targets,
 * casts the area spell (attack_enemy_all_common_event) and resets its facing.
 * When confused with no target it is flustered.
 */
static EVT_BEGIN(all_attack_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL_CHILD(attack_enemy_all_common_event)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Support spell animation of the Wizzerd: it charges up (SFX_ENM_MAHO_CHARGE1, "akari_charge_n64" glow,
 * hands raised), casts (MAH_A_2A, SFX_ENM_MAHO_MAGIC4) and sparkles with the "mahorn" effect first over itself
 * and then over the target in LW(3).
 */
static EVT_BEGIN(support_friend_common_event)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    CALL(evt_eff64, STRING(""), STRING("akari_charge_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 70, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MAH_A_4"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("MAH_S_5A"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("MAH_S_5A"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("MAH_S_5A"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("MAH_S_5A"))
    CALL(set_hand_dest, UNIT_ID_SELF, 2, FLOAT(-16.0f), FLOAT(29.0f), FLOAT(-5.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 3, FLOAT(16.0f), FLOAT(29.0f), FLOAT(5.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 4, FLOAT(-16.0f), FLOAT(11.0f), FLOAT(-5.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 5, FLOAT(16.0f), FLOAT(11.0f), FLOAT(5.0f))
    WAIT_FRAMES(50)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MAH_A_2A"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("MAH_S_4A"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("MAH_S_4A"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("MAH_S_4A"))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("MAH_S_4A"))
    CALL(set_hand_dest, UNIT_ID_SELF, 2, FLOAT(-20.0f), FLOAT(40.0f), FLOAT(-15.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 3, FLOAT(20.0f), FLOAT(40.0f), FLOAT(15.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 4, FLOAT(-20.0f), FLOAT(0.0f), FLOAT(-15.0f))
    CALL(set_hand_dest, UNIT_ID_SELF, 5, FLOAT(20.0f), FLOAT(0.0f), FLOAT(15.0f))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 15)
    CALL(evt_eff, STRING(""), STRING("mahorn"), 0, LW(0), LW(1), LW(2), FLOAT(1.3994141f), 2, 30, 0, 0, 0, 0, 0)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_MAHO_MAGIC4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 15)
    CALL(evt_eff, STRING(""), STRING("mahorn"), 0, LW(0), LW(1), LW(2), FLOAT(1.3994141f), 2, 30, 0, 0, 0, 0, 0)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MAH_A_2C"))
    RETURN
EVT_END()

/**
 * The Wizzerd's size-up spell: it picks an ally, declares the cast,
 * casts the support spell (support_friend_common_event)
 * and the target grows for 3 turns (the weapon's size change, strength 2).
 */
static EVT_BEGIN(dekadeka_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL_CHILD(support_friend_common_event)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_FRAMES(10)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Wizzerd's defence-up spell: it picks an ally,
 * casts the support spell (support_friend_common_event) and the target's defence rises by 3 for 3 turns.
 */
static EVT_BEGIN(katikati_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL_CHILD(support_friend_common_event)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_FRAMES(10)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Wizzerd's invisibility spell: it casts the support spell (support_friend_common_event) on an ally.
 * Never chosen by attack_event, and there is no weapon for it in this file.
 */
static EVT_BEGIN(trans_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL_CHILD(support_friend_common_event)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_FRAMES(10)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Wizzerd's dodgy spell: it casts the support spell (support_friend_common_event) on an ally.
 * Never chosen by attack_event, and there is no weapon for it in this file.
 */
static EVT_BEGIN(yokeyoke_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL_CHILD(support_friend_common_event)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_FRAMES(10)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Wizzerd's heal: it picks an ally, casts the support spell (support_friend_common_event)
 * and the ally recovers 6 HP with the "recovery" effect.
 */
static EVT_BEGIN(recover_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL_CHILD(support_friend_common_event)
    CALL(btlevtcmd_RecoverHp, LW(3), LW(4), 6)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    ADD(LW(2), 10)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(0), LW(1), LW(2), 6, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(10)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Wait script of the Wizzerd: unless it is a plain copy (UNIT_WORK_BUNSIN set) its copies also play their idle pose,
 * and each of the four hands bobs between two positions and poses at random intervals of 15 to 30 frames.
 * Under statuses 1, 2 or 9 the hands hold still (MAH_Z_2).
 */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(mahorn_get_bunsin, LW(1), LW(2), LW(3), LW(4), LW(0))

        // The Wizzerd and any copies play their idle poses.
        CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
        IF_INT_NOT_EQUAL(LW(1), -1)
            CALL(btlevtcmd_AnimeChangePoseFromTable, LW(1), 1)
        END_IF
        IF_INT_NOT_EQUAL(LW(2), -1)
            CALL(btlevtcmd_AnimeChangePoseFromTable, LW(2), 1)
        END_IF
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), 1)
        END_IF
        IF_INT_NOT_EQUAL(LW(4), -1)
            CALL(btlevtcmd_AnimeChangePoseFromTable, LW(4), 1)
        END_IF
    END_IF

    // Under Sleep, Stop or Freeze the four hands hold still (MAH_Z_2).
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FREEZE, LW(13))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_SLEEP, LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_STOP, LW(15))
    OR(LW(15), LW(13))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("MAH_Z_2"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("MAH_Z_2"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("MAH_Z_2"))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("MAH_Z_2"))
    ELSE
        BEGIN_SIBLING_EVENT
            // Each of the four hands bobs between two poses (MAH_S_4A and 4B) on its own random timer.
            LABEL(10)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("MAH_S_4A"))
            CALL(evt_sub_random, 15, LW(0))
            ADD(LW(0), 15)
            WAIT_FRAMES(LW(0))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("MAH_S_4B"))
            CALL(evt_sub_random, 15, LW(0))
            ADD(LW(0), 15)
            WAIT_FRAMES(LW(0))
            GOTO(10)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            LABEL(11)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("MAH_S_4A"))
            CALL(evt_sub_random, 15, LW(0))
            ADD(LW(0), 15)
            WAIT_FRAMES(LW(0))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("MAH_S_4B"))
            CALL(evt_sub_random, 15, LW(0))
            ADD(LW(0), 15)
            WAIT_FRAMES(LW(0))
            GOTO(11)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            LABEL(12)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("MAH_S_4A"))
            CALL(evt_sub_random, 15, LW(0))
            ADD(LW(0), 15)
            WAIT_FRAMES(LW(0))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("MAH_S_4B"))
            CALL(evt_sub_random, 15, LW(0))
            ADD(LW(0), 15)
            WAIT_FRAMES(LW(0))
            GOTO(12)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            LABEL(13)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("MAH_S_4A"))
            CALL(evt_sub_random, 15, LW(0))
            ADD(LW(0), 15)
            WAIT_FRAMES(LW(0))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("MAH_S_4B"))
            CALL(evt_sub_random, 15, LW(0))
            ADD(LW(0), 15)
            WAIT_FRAMES(LW(0))
            GOTO(13)
        END_SIBLING_EVENT
    END_IF
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(set_hand_dest, UNIT_ID_SELF, 2, FLOAT(-25.0f), FLOAT(30.0f), FLOAT(-10.0f))
        CALL(evt_sub_random, 15, LW(0))
        ADD(LW(0), 15)
        WAIT_FRAMES(LW(0))
        CALL(set_hand_dest, UNIT_ID_SELF, 2, FLOAT(-15.0f), FLOAT(40.0f), FLOAT(-10.0f))
        CALL(evt_sub_random, 15, LW(0))
        ADD(LW(0), 15)
        WAIT_FRAMES(LW(0))
        GOTO(0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LABEL(1)
        CALL(set_hand_dest, UNIT_ID_SELF, 3, FLOAT(25.0f), FLOAT(30.0f), FLOAT(10.0f))
        CALL(evt_sub_random, 15, LW(0))
        ADD(LW(0), 15)
        WAIT_FRAMES(LW(0))
        CALL(set_hand_dest, UNIT_ID_SELF, 3, FLOAT(15.0f), FLOAT(40.0f), FLOAT(10.0f))
        CALL(evt_sub_random, 15, LW(0))
        ADD(LW(0), 15)
        WAIT_FRAMES(LW(0))
        GOTO(1)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        LABEL(2)
        CALL(set_hand_dest, UNIT_ID_SELF, 4, FLOAT(-25.0f), FLOAT(10.0f), FLOAT(-10.0f))
        CALL(evt_sub_random, 15, LW(0))
        ADD(LW(0), 15)
        WAIT_FRAMES(LW(0))
        CALL(set_hand_dest, UNIT_ID_SELF, 4, FLOAT(-15.0f), FLOAT(0.0f), FLOAT(-10.0f))
        CALL(evt_sub_random, 15, LW(0))
        ADD(LW(0), 15)
        WAIT_FRAMES(LW(0))
        GOTO(2)
    END_SIBLING_EVENT
    LABEL(3)
    CALL(set_hand_dest, UNIT_ID_SELF, 5, FLOAT(25.0f), FLOAT(10.0f), FLOAT(10.0f))
    CALL(evt_sub_random, 15, LW(0))
    ADD(LW(0), 15)
    WAIT_FRAMES(LW(0))
    CALL(set_hand_dest, UNIT_ID_SELF, 5, FLOAT(15.0f), FLOAT(0.0f), FLOAT(10.0f))
    CALL(evt_sub_random, 15, LW(0))
    ADD(LW(0), 15)
    WAIT_FRAMES(LW(0))
    GOTO(3)
    RETURN
EVT_END()

/**
 * Script helper: args[0] = unit; returns in args[1] the x position of the magic effect's type-3 particle (the
 * moving orb), or 0 if none exists.
 */
static USER_FUNC(eff_magic_get_posX) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, *args++));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    EffectEntry* eff = (EffectEntry*)unit->work[UNIT_WORK_MAGIC_EFFECT];
    effMahorn2Data* work = eff->userdata;
    s32 i;

    for (i = 0; i < eff->count; i++, work++) {
        if (work->type == 3) {
            evtSetFloat(event, *args++, work->position.x);
            break;
        }
    }
    if (i == eff->count) {
        evtSetFloat(event, *args++, 0.0f);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] = unit, args[1] = target x, args[2..3] are unused, args[4] = travel time in frames.
 * Starts the Wizzerd's magic effect from its wand towards the target x at its starting y and z and stores it in work[7];
 * the speed is derived from the distance and time.
 */
static USER_FUNC(eff_magic) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 a = evtGetFloat(event, args[1]);
    f32 b = evtGetFloat(event, args[2]);
    f32 c = evtGetFloat(event, args[3]);
    s32 time = evtGetValue(event, args[4]);
    f32 x;
    f32 y;
    f32 z;

    BtlUnit_GetPos(unit, &x, &y, &z);
    x += unit->sizeMultiplier * unit->faceDirection;
    y += 18.0f * unit->sizeMultiplier;
    z += 5.0f;
    unit->work[UNIT_WORK_MAGIC_EFFECT] = (EvtWord)effMahorn2Entry(0, x, y, z, a, y, z, unit->sizeMultiplier, mahorn_fabs((a - x) / time));
    return EVT_RETURN_DONE;
}

/**
 * Script helper: finds the Wizzerd's copies on the field (kinds 0x81 and 0xAA)
 * and returns up to four of their unit indices in args[0..3] (-1 for none) and the count in args[4].
 */
static USER_FUNC(mahorn_get_bunsin) {
    EvtWord* args = event->args;
    s32 copies[4];
    s32 n = 0;
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    // indices of the wizzerd's copies
    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && (unit->currentKind == UNIT_DARK_WIZZERD_CLONE || unit->currentKind == UNIT_ELITE_WIZZERD_CLONE)) {
            copies[n] = i;
            n++;
        }
    }
    for (i = 0; i < 4; i++) {
        if (i < n) {
            evtSetValue(event, *args++, copies[i]);
        } else {
            evtSetValue(event, *args++, -1);
        }
    }
    evtSetValue(event, *args, n);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] = unit, args[1] = hand (2-5), args[2..4] = x, y, z.
 * Sets the target offset of one floating hand, mirrored in x and scaled by 1.25 or 0.75 when the Wizzerd is big
 * or small.
 */
static USER_FUNC(set_hand_dest) {
    s32 hand;
    BattleUnit* unit;
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    f32* dest;
    f32 x;
    f32 y;
    f32 z;

    id = evtGetValue(event, args[0]);
    hand = evtGetValue(event, args[1]);
    x = evtGetFloat(event, args[2]);
    y = evtGetFloat(event, args[3]);
    z = evtGetFloat(event, args[4]);
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    if (unit == NULL) {
        return EVT_RETURN_BLOCK;
    }
    hand -= 2;
    dest = (f32*)unit->work[hand];
    if (BtlUnit_CheckStatus(unit, STATUS_HUGE)) {
        dest[0] = 1.25f * -x;
        dest[1] = 1.25f * y;
        dest[2] = 1.25f * z;
    } else if (BtlUnit_CheckStatus(unit, STATUS_TINY)) {
        dest[0] = 0.75f * -x;
        dest[1] = 0.75f * y;
        dest[2] = 0.75f * z;
    } else {
        dest[0] = -x;
        dest[1] = y;
        dest[2] = z;
    }
    return EVT_RETURN_DONE;
}

/**
 * Per-frame script helper: args[0] = unit.
 * Eases each of the four floating hand parts (2-5) a tenth of the way towards the destination set by
 * set_hand_dest; does nothing while the Wizzerd is under certain statuses (ids 1, 2 and 9).
 */
static USER_FUNC(hand_ctrl) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    s8 turns;
    s8 strength;
    BattleUnitPart* part;
    f32* dest;
    Vec* scale;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    s32 j;

    if (unit == NULL) {
        return EVT_RETURN_BLOCK;
    }
    BtlUnit_GetStatus(unit, STATUS_STOP, &turns, &strength);
    if (turns > 0) {
        return EVT_RETURN_DONE;
    }
    BtlUnit_GetStatus(unit, STATUS_FREEZE, &turns, &strength);
    if (turns > 0) {
        return EVT_RETURN_DONE;
    }
    BtlUnit_GetStatus(unit, STATUS_SLEEP, &turns, &strength);
    if (turns > 0) {
        return EVT_RETURN_DONE;
    }
    for (j = 0, i = 2; i <= 5; i++, j++) {
        part = BtlUnit_GetPartsPtr(unit, i);
        dest = (f32*)unit->work[j];
        scale = &part->baseScale;
        x = part->positionOffset.x * scale->x;
        y = part->positionOffset.y * scale->y;
        z = part->positionOffset.z * scale->z;
        x += (dest[0] - x) / 10.0f;
        y += (dest[1] - y) / 10.0f;
        z += (dest[2] - z) / 10.0f;
        part->positionOffset.x = x;
        part->positionOffset.y = y;
        part->positionOffset.z = z;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: args[0] = unit. Frees the four hand destination blocks held in work[0..3]. */
static USER_FUNC(hand_end) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    s32 i;
    s32 j;

    for (i = 2, j = 0; i <= 5; i++, j++) {
        if ((void*)unit->work[j] != NULL) {
            BattleFree((void*)unit->work[j]);
            unit->work[j] = 0;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] = unit.
 * Gives each of the four hand parts (2-5) the idle animation MAH_Z_2
 * and a zeroed destination block (work[0..3]).
 */
static USER_FUNC(hand_init) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    f32 x;
    f32 y;
    f32 z;
    f32* dest;
    s32 i;
    s32 j;

    BtlUnit_GetPos(unit, &x, &y, &z);
    for (j = 0, i = 2; i <= 5; i++, j++) {
        BtlUnit_SetAnim(BtlUnit_GetPartsPtr(unit, i), "MAH_Z_2");
        dest = BTL_ALLOC(12);
        unit->work[j] = (EvtWord)dest;
        dest[0] = 0.0f;
        dest[1] = 0.0f;
        dest[2] = 0.0f;
    }
    return EVT_RETURN_DONE;
}
