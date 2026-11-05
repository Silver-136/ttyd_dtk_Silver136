// X-Naut Fortress (aji) battle unit: X-Naut PhD (gundan_zako_magician): its parts, defence/status tables, and attack
// events.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"

#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

static USER_FUNC(kanbu_get_id);

BattleUnitKind unit_gundan_zako_magician;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[16];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[2];
static BattleWeapon weapon_gundan_zako_magician_fire_bottle;
static BattleWeapon weapon_gundan_zako_magician_hunyahunya_bottle;
static BattleWeapon weapon_gundan_zako_magician_yokeyoke_bottle;
static BattleWeapon weapon_gundan_zako_magician_minimini_bottle;
static BattleWeapon weapon_gundan_zako_magician_one_recover;
static BattleWeapon weapon_gundan_zako_magician_all_recover;
static BattleWeapon weapon_gundan_zako_magician_ziwaziwa_bottle;
static EvtWord init_event[21];
static EvtWord damage_event[10];
static EvtWord attack_event[235];
static EvtWord attack_common_event1[141];
static EvtWord attack_common_event2[332];
static EvtWord attack_common_event3[251];
static EvtWord fire_bottle_event[56];
static EvtWord hunyahunya_bottle_event[43];
static EvtWord yokeyoke_bottle_event[74];
static EvtWord minimini_bottle_event[314];
static EvtWord one_recover_event[128];
static EvtWord all_recover_event[155];
static EvtWord yokeyoke_bottle_event_with_kanbu[15];
static EvtWord ziwaziwa_bottle_event[15];
static EvtWord wait_event[6];

/**
 * Script function: finds the X-Naut officer (the unit with initialKind 99, the "kanbu") among the 64 battle
 * units and returns its unit ID in args[0] (-1 if absent) and its body part ID in args[1] (0 if absent);
 * the PhD's bottle events use it to aim at the officer.
 */
static USER_FUNC(kanbu_get_id) {
    EvtWord* args = event->args;
    s32 id = -1;
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    // Select the first occupied slot with this initial kind, without checking its current HP or alliance.
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == UNIT_LORD_CRUMP_CHAPTER5) {
            id = i;
            break;
        }
    }
    evtSetValue(event, args[0], id);
    if (id != -1) {
        evtSetValue(event, args[1], BtlUnit_GetBodyPartsId(unit));
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

/** Unit data of the X-Naut PhD: 9 HP, level 27; the battle starts it from init_event. */
BattleUnitKind unit_gundan_zako_magician = {
    UNIT_X_NAUT_PHD, // unitType
    "btl_un_gundan_zako_magician", // name
    9, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    27, // level
    0, // bonusExp
    3, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    30, // width
    36, // height
    {0, 36}, // hitOffset
    {0.0f, 18.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {15.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {15.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 27.0f, 0.0f}, // kissHitOffset
    {0.0f, 18.0f, 0.0f}, // cutBaseOffset
    30.0f, // cutWidth
    36.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_3RD2_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    2, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the X-Naut PhD: none against any element. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Elemental reactions of the X-Naut PhD: none, every element hurts it normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {70, 80, 70, 100, 80, 100, 100, 70, 100, 75, 100, 75, 100, 70, 70, 90, 80, 100, 70, 100, 100, 95};
#else
static StatusVulnerability regist = {
    80, // sleep
    80, // stop
    80, // dizzy
    100, // poison
    80, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    90, // defenseDown
    70, // allergic
    90, // fright
    70, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/**
 * Pose table of the X-Naut PhD: the animation each listed battle pose (idle, hurt, walk, talk and so on) plays.
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_KO, "Z_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "D_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "D_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/** Event table of the X-Naut PhD: empty, it has no special reactions. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** Parts of the X-Naut PhD: part 1 is his body (the target) and part 2 is the bottle he throws. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_gundan_zako_magician", "c_zako_m", 0.0f, 0.0f, 0.0f, 0.0f, 28.0f, 0.0f, 0.0f, 38.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
#if VERSION <= VERSION_DEMO_JP
    {2, "btl_un_gundan_zako_magician", "c_zako_m", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW | PART_ATTR_WORLD_POSITION, 0, pose_table},
#else
    {2, "btl_un_gundan_zako_magician", "c_zako_m", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13018000, 0, pose_table},
#endif
};

/**
 * Weapon data for the X-Naut PhD's fire bottle: 4 damage, fire element, a jump-type hit, one target, may
 * inflict burn (75%).
 */
static BattleWeapon weapon_gundan_zako_magician_fire_bottle = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
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
    0, 0, 0, 0, 0, 0, 0, // poisonStrength..dodgyTime
    75, // burnChance
    3, // burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Weapon data for the X-Naut PhD's Defense-down bottle: 4 damage, a jump-type hit, one target, may inflict
 * defense change (75%).
 */
static BattleWeapon weapon_gundan_zako_magician_hunyahunya_bottle = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
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
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    75, // defChangeChance
    3, // defChangeTime
    -3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon data for the X-Naut PhD's Dodgy bottle: no damage, one target, may inflict Dodgy (100%). */
static BattleWeapon weapon_gundan_zako_magician_yokeyoke_bottle = {
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
    0, 0, 0, 0, 0, 0, // poisonTime..electricTime
    100, // dodgyChance
    3, // dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon data for the X-Naut PhD's shrink bottle: 3 damage, every target, may inflict size change (75%). */
static BattleWeapon weapon_gundan_zako_magician_minimini_bottle = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
#if VERSION <= VERSION_DEMO_JP
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_IGNORE_VULNERABILITY,
#else
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
#endif
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, // burnTime..freezeTime
    75, // sizeChangeChance
    3, // sizeChangeTime
    -2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon data for the X-Naut PhD's heal-one bottle: no damage, one target. */
static BattleWeapon weapon_gundan_zako_magician_one_recover = {
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
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP | TARGET_WEIGHT_LOW_HP_RATIO, // targetWeightFlags
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

/** Weapon data for the X-Naut PhD's heal-all bottle: no damage, every target. */
static BattleWeapon weapon_gundan_zako_magician_all_recover = {
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, 0, // acHelpMsg..special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon data for the X-Naut PhD's regen bottle: no damage, one target. */
static BattleWeapon weapon_gundan_zako_magician_ziwaziwa_bottle = {
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
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP | TARGET_WEIGHT_LOW_HP_RATIO, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, // invisibleChance..invisibleTime
    5, // hpRegenTime
    2, // hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Battle init for the X-Naut PhD: registers its wait, attack,
 * damage and confusion events and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event for the X-Naut PhD: runs the default damage handler (btldefaultevt_Damage). */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Attack script of the X-Naut PhD: if battle flag 2 is set it throws the fire bottle;
 * otherwise it lets an enemy item be used if one is ready and picks a bottle by weight:
 * fire 20, defense-down 10, dodgy 10 (skipped if already dodgy), shrink 20, heal one ally 10, heal all 5.
 * When an X-Naut officer (kanbu_get_id) is in the battle a different mix is used that includes giving the
 * officer the dodgy bottle or the regen bottle.
 */
static EVT_BEGIN(attack_event)
    // During a first strike it always throws the fire bottle.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(fire_bottle_event))
        GOTO(99)
    END_IF

    // It uses an item instead of a bottle when one is ready.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF

    // Look for an X-Naut officer in the battle: with one present it picks from a different mix of bottles.
    CALL(kanbu_get_id, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(10)
    ELSE
        GOTO(20)
    END_IF
    LABEL(10)
    SET_VALUE(LW(0), 0)
    ADD(LW(0), 20)
    ADD(LW(0), 10)
    ADD(LW(0), 10)
    ADD(LW(0), 20)
    ADD(LW(0), 10)
    ADD(LW(0), 5)
    SUBTRACT(LW(0), 1)

    // No officer: a random number picks a bottle by weight (fire, Defense-down, Dodgy, shrink, heal one ally,
    // heal all).
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 20)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(fire_bottle_event))
        GOTO(99)
    END_IF
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(hunyahunya_bottle_event))
        GOTO(99)
    END_IF
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DODGY, LW(2))
        IF_INT_EQUAL(LW(2), 0)
            CALL_CHILD(PTR(yokeyoke_bottle_event))
            GOTO(99)
        END_IF
    END_IF
    ADD(LW(0), 20)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(minimini_bottle_event))
        GOTO(99)
    END_IF
    ADD(LW(0), 10)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(one_recover_event))
        GOTO(99)
    END_IF
    ADD(LW(0), 5)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(all_recover_event))
        GOTO(99)
    END_IF
    GOTO(10)
    LABEL(20)
    SET_VALUE(LW(0), 0)
    ADD(LW(0), 60)
    ADD(LW(0), 20)
    ADD(LW(0), 20)
    SUBTRACT(LW(0), 1)

    // With an officer: a random number picks the fire bottle, the Dodgy bottle for the officer or the regen
    // bottle.
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 60)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(PTR(fire_bottle_event))
        GOTO(99)
    END_IF
    ADD(LW(0), 20)
    IF_INT_LESS(LW(1), LW(0))
        CALL(kanbu_get_id, LW(3), LW(4))
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_DODGY, LW(2))
        IF_INT_EQUAL(LW(2), 0)
            CALL_CHILD(PTR(yokeyoke_bottle_event_with_kanbu))
            GOTO(99)
        END_IF
    END_IF
    ADD(LW(0), 20)
    IF_INT_LESS(LW(1), LW(0))
        CALL(kanbu_get_id, LW(3), LW(4))
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_HP_REGEN, LW(2))
        IF_INT_EQUAL(LW(2), 0)
            CALL_CHILD(PTR(ziwaziwa_bottle_event))
            GOTO(99)
        END_IF
    END_IF
    GOTO(20)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Shared start of the X-Naut PhD's bottle throws:
 * pays for the attack, faces the target (or the target side),
 * plays the throw wind-up sounds and poses A_1A to A_5A.
 */
static EVT_BEGIN(attack_common_event1)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_minimini_bottle))
        CALL(btlevtcmd_GetEnemyBelong, LW(3), LW(10))
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LW(10), 125)
        ELSE
            SET_VALUE(LW(10), -125)
        END_IF
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(10), 0, FACE_DIR_TOWARD_X, LW(14))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(14))
    ELSE
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(14))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(14))
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_fire_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_hunyahunya_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2A"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_minimini_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3A"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_yokeyoke_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_5A"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_one_recover))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4A"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_all_recover))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4A"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_ziwaziwa_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4A"))
    END_IF
    WAIT_FRAMES(80)
    RETURN
EVT_END()

/**
 * Shared bottle throw at an opposing target: lobs the bottle part (part 2) in a high arc to the target's hit
 * position, then either it shatters on a dodged or missed target (glass shards, SFX_ENM_3RD2_CRASH1)
 * or applies the damage and status to the target with the 256 final-hit flag;
 * ends by resetting the camera and facing.
 */
static EVT_BEGIN(attack_common_event2)
    // Pick the bottle's pose for the bottle being thrown (fire or Defense-down).
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_fire_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("B_1A"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_hunyahunya_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("B_1B"))
    END_IF

    // The bottle (part 2) appears in its hand as it winds up.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
    ADD(LW(1), 20)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_fire_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_hunyahunya_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2B"))
    END_IF
    WAIT_FRAMES(24)

    // It hurls the bottle in a high arc toward the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(24)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(6.0f), FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)

    // Hit check: a miss or dodge shatters the bottle, a hit goes on to the damage and status.
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

        // Miss: the bottle smashes on the ground beside the target in a shower of glass.
        CALL(btlevtcmd_JumpPartsContinue, UNIT_ID_SELF, 2)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
        LOOP(12)
            CALL(evt_eff64, STRING(""), STRING("glass_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 30, 0, 0, 0, 0, 0, 0)
        END_LOOP
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_CRASH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("B_2"))
        WAIT_FRAMES(8)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
        GOTO(98)
        LABEL(91)
        IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_fire_bottle))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(evt_eff, STRING(""), STRING("kemuri_test"), 16, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        END_IF
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        LOOP(12)
            CALL(evt_eff64, STRING(""), STRING("glass_n64"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 30, 0, 0, 0, 0, 0, 0)
        END_LOOP

        // Hit: the bottle smashes on the target and the damage and status effect are applied.
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_CRASH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("B_2"))
        WAIT_FRAMES(8)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
        GOTO(98)
        LABEL(98)

        // The camera and facing return to normal.
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_FRAMES(30)
        LABEL(99)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
        RETURN
EVT_END()

/**
 * Shared bottle throw at an ally: the bottle part (part 2) is thrown in a short low arc to ground level at the target's
 * X and Z, shatters in glass shards and applies the weapon's effect to the target.
 */
static EVT_BEGIN(attack_common_event3)
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_yokeyoke_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("B_1D"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_ziwaziwa_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("B_1C"))
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
    ADD(LW(1), 20)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_yokeyoke_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_5C"))
    END_IF
    IF_INT_EQUAL(LW(9), PTR(&weapon_gundan_zako_magician_ziwaziwa_bottle))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4C"))
    END_IF
    WAIT_FRAMES(24)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(24)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(1.0f), FLOAT(0.5f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    LOOP(12)
        CALL(evt_eff64, STRING(""), STRING("glass_n64"), 0, LW(0), 0, LW(2), FLOAT(1.0f), 30, 0, 0, 0, 0, 0, 0)
    END_LOOP
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
        GOTO(98)
        LABEL(91)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(98)
        LABEL(98)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_CRASH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("B_2"))
        WAIT_FRAMES(8)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_FRAMES(30)
        LABEL(99)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
        RETURN
EVT_END()

/**
 * Fire bottle: picks a target (the first attack target if battle flag 2 is set) and throws a bottle that damages
 * and may burn it (75% for 3 turns).
 */
static EVT_BEGIN(fire_bottle_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_magician_fire_bottle))
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
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL_CHILD(PTR(attack_common_event1))
    CALL_CHILD(PTR(attack_common_event2))
    LABEL(99)
    RETURN
EVT_END()

/** Hunyahunya bottle: throws a bottle at a chosen opponent that lowers its defense by 3 for 3 turns (75% chance). */
static EVT_BEGIN(hunyahunya_bottle_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_magician_hunyahunya_bottle))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL_CHILD(PTR(attack_common_event1))
    CALL_CHILD(PTR(attack_common_event2))
    LABEL(99)
    RETURN
EVT_END()

/** Yokeyoke bottle: the X-Naut PhD drinks a bottle that makes it dodgy (always hits itself, 3 turns). */
static EVT_BEGIN(yokeyoke_bottle_event)
    SET_VALUE(LW(3), UNIT_ID_SELF)
    SET_VALUE(LW(4), 1)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_magician_yokeyoke_bottle))
    CALL_CHILD(attack_common_event1)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_BOTTLE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(30)
        LOOP(3)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_BOTTLE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(20)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_5B"))
    WAIT_FRAMES(120)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(5)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_BOTTLE5"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(55)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    END_INLINE_EVENT
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

/**
 * Minimini bottle: throws a bottle that shrinks every opposing target in turn (size change of -2 for 3 turns,
 * 75% chance), with the bottle shattering over the opposing side before the target checks.
 */
static EVT_BEGIN(minimini_bottle_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_magician_minimini_bottle))

    // It samples a target, but the bottle reaches every opposing unit.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))

    // No valid target: a confused PhD is flustered, otherwise the turn is skipped.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // Wind-up: it prepares the bottle throw (attack_common_event1).
    CALL_CHILD(attack_common_event1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("B_1E"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
    ADD(LW(1), 20)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))

    // It lifts the bottle (part 2) and throws it high over the opposing side.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3C"))
    WAIT_FRAMES(24)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(24)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
    CALL(btlevtcmd_GetEnemyBelong, LW(3), LW(10))
    IF_INT_EQUAL(LW(10), 0)
        SET_VALUE(LW(10), 125)
    ELSE
        SET_VALUE(LW(10), -125)
    END_IF
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(6.0f), FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(10), 0, 0, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 20, LW(10), 50, 0, FLOAT(2.399414f), 0, 0, 0, 0, 0, 0, 0)
    LOOP(12)
        CALL(evt_eff64, STRING(""), STRING("glass_n64"), 0, LW(10), 0, 0, FLOAT(1.0f), 30, 0, 0, 0, 0, 0, 0)
    END_LOOP

    // The bottle bursts over the opposing side in a shower of shrinking mist.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_CRASH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("B_2"))
        WAIT_FRAMES(8)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kNoShadow)
    END_INLINE_EVENT
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(0)

    // Each target gets a hit check; only a hit reaches CheckDamage. Defence is requested only for target
    // ordinal zero, if that target reaches the hit branch.
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
        GOTO(97)
        LABEL(91)

        // Defence is requested only for the first selected target, when it reaches the hit branch.
        IF_INT_EQUAL(LW(10), 0)
            CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        END_IF
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(97)
        LABEL(97)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_NOT_EQUAL(LW(3), -1)
            ADD(LW(10), 1)
            GOTO(0)
        END_IF
        LABEL(98)

        // The camera returns to normal.
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_FRAMES(60)
        LABEL(99)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
        RETURN
EVT_END()

/**
 * Heals one ally: picks the ally to help, shakes the bottle (SFX_ENM_3RD2_SHAKE1)
 * and restores 10 HP to it with a recovery effect above its head.
 */
static EVT_BEGIN(one_recover_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_magician_one_recover))
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
    CALL_CHILD(attack_common_event1)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4B"))
    WAIT_FRAMES(80)
    CALL(btlevtcmd_RecoverHp, LW(3), LW(4), 10)
    CALL(btlevtcmd_GetPartsPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, LW(3), LW(5))
    CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    ADD(LW(1), LW(5))
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Heals the whole team: shakes the bottle and restores 5 HP to every ally in turn,
 * with a recovery effect above each.
 */
static EVT_BEGIN(all_recover_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_magician_all_recover))
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
    CALL_CHILD(attack_common_event1)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_3RD2_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4B"))
    WAIT_FRAMES(80)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(0)
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_random, 10, LW(10))
        ADD(LW(10), 5)
        WAIT_FRAMES(LW(10))
        CALL(btlevtcmd_RecoverHp, LW(3), LW(4), 5)
        CALL(btlevtcmd_GetPartsPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHeight, LW(3), LW(5))
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY_FLOAT(LW(5), LW(6))
        ADD(LW(1), LW(5))
        CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(0), LW(1), LW(2), 5, 0, 0, 0, 0, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(0)
    END_IF
    WAIT_FRAMES(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Yokeyoke bottle given to the X-Naut officer (kanbu) instead of itself:
 * throws the bottle at the officer through attack_common_event3 to make it dodgy.
 */
static EVT_BEGIN(yokeyoke_bottle_event_with_kanbu)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_magician_yokeyoke_bottle))
    CALL(kanbu_get_id, LW(3), LW(4))
    CALL_CHILD(PTR(attack_common_event1))
    CALL_CHILD(PTR(attack_common_event3))
    LABEL(99)
    RETURN
EVT_END()

/**
 * Ziwaziwa bottle given to the X-Naut officer (kanbu):
 * throws the bottle at the officer through attack_common_event3,
 * giving it HP regeneration (2 HP per turn for 5 turns).
 */
static EVT_BEGIN(ziwaziwa_bottle_event)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_magician_ziwaziwa_bottle))
    CALL(kanbu_get_id, LW(3), LW(4))
    CALL_CHILD(PTR(attack_common_event1))
    CALL_CHILD(PTR(attack_common_event3))
    LABEL(99)
    RETURN
EVT_END()

/** Idle script of the X-Naut PhD: sets the idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

