// Battle unit Pokey (`sambo`): tst.rel's copy of rels/eki/src/unit/unit_sambo.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "party/party_motion.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_mario.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "evt/evt_eff.h"

// Unit work slots of the Pokey (btlevtcmd_SetUnitWork / GetUnitWork).
enum {
    UNIT_WORK_BODY_COUNT = 0,       // body segments left (4 at the start); picks the target part and the height
    UNIT_WORK_CALL_DEPTH = 1, // Pokeys called in so far (call_friend_event); a new one inherits it
};

BattleUnitKind unit_sambo;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleWeapon weapon_sambo_head_attack_4;
static BattleWeapon weapon_sambo_head_attack_3;
static BattleWeapon weapon_sambo_head_attack_2;
static BattleWeapon weapon_sambo_head_attack_1;
static BattleWeapon weapon_sambo_spike_attack_4;
static BattleWeapon weapon_sambo_spike_attack_3;
static BattleWeapon weapon_sambo_spike_attack_2;
static PoseTableEntry pose_table_body4[18];
static PoseTableEntry pose_table_body3[20];
static PoseTableEntry pose_table_body2[20];
static PoseTableEntry pose_table_body1[20];
static PoseTableEntry pose_table_parts[3];
static BattleDataEntry data_table[8];
static BattleUnitKindPart parts[5];
static EvtWord init_event[54];
static EvtWord dmg_avoid_counter_thorn_side_event[51];
static EvtWord damage_event[34];
static EvtWord damage_event_bodylost[273];
static EvtWord subset_dmg_sambo_fall[152];
static EvtWord damage_event_bodylost_blow[354];
static EvtWord damage_swallow_body[256];
static EvtWord attack_event[189];
static EvtWord attack_head_2_event[613];
static EvtWord attack_head_1_event[587];
static EvtWord attack_spike_event[824];
static EvtWord call_friend_event[304];
static BattleUnitSetup entry_sambo;
static EvtWord wait_event[36];
static EvtWord sambo_rabbit_move[74];
static EvtWord _body_param_set[49];

/**
 * Unit definition of the Pokey: 4 HP and level 11, with its sizes, damage sounds, status vulnerability,
 * parts and init script.
 */
BattleUnitKind unit_sambo = {
    39, // unitType
    "btl_un_sambo", // name
    4, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    11, // level
    0, // bonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    26, // width
    80, // height
    {14, 80}, // hitOffset
    {0.0f, 40.0f, 0.0f}, // centerOffset
    {0}, // hpGaugeOffset
    {0}, // talkTogeBaseOffset
    {13.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0}, // burnFlameOffset
    0.0f, 0.0f, // vivianOffsetX..vivianOffsetZ
    {13.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 52.0f, 0.0f}, // kissHitOffset
    {0.0f, 40.0f, 0.0f}, // cutBaseOffset
    26.0f, // cutWidth
    80.0f, // cutHeight
    0, 0, 0, // turnOrder..swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0}, // starPointDispOffset
    "SFX_ENM_SAMBO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    5, // numParts
    parts, // parts
    init_event, // initEventCode
    (BattleDataEntry*)data_table, // dataTable
};

/** Defence of the Pokey: none, every hit does its full damage. */
static BattleUnitDefense defence = {0};

/** Element modifiers of the Pokey: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {0};

#if VERSION <= VERSION_DEMO_US
/** Status vulnerability of the Pokey in the demo builds: resists most statuses. */
static StatusVulnerability regist = {90, 80, 90, 100, 90, 100, 100, 50, 100, 90, 100, 90, 100, 90, 90, 100, 90, 100, 90, 100, 100, 100};
#else
/** Status vulnerability of the Pokey in the retail builds: resists most statuses. */
static StatusVulnerability regist = {95, 80, 80, 100, 90, 100, 100, 60, 100, 90, 100, 90, 100, 95, 90, 100, 90, 100, 90, 100, 100, 100};
#endif

/** Attack data of the Pokey's headbutt with 4 segments: 2 damage. */
static BattleWeapon weapon_sambo_head_attack_4 = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    0, // counterResistFlags
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

/** Attack data of the Pokey's headbutt with 3 segments: 2 damage. */
static BattleWeapon weapon_sambo_head_attack_3 = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    0, // counterResistFlags
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

/** Attack data of the Pokey's headbutt with 2 segments: 2 damage. */
static BattleWeapon weapon_sambo_head_attack_2 = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    0, // counterResistFlags
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

/** Attack data of the Pokey's headbutt with 1 segment: 3 damage. */
#if VERSION <= VERSION_DEMO_US
static BattleWeapon weapon_sambo_head_attack_1 = {
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
    weaponGetPowerDefault,
{3, 0, 0, 0, 0, 0, 0, 0},
NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE | SPECIAL_FLIP_SHELLED, // special
    0, // counterResistFlags
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
#else
static BattleWeapon weapon_sambo_head_attack_1 = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE | SPECIAL_FLIP_SHELLED, // special
    0, // counterResistFlags
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
#endif

/** Attack data of the Pokey's segment throw with 4 segments: 3 damage. */
static BattleWeapon weapon_sambo_spike_attack_4 = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

/** Attack data of the Pokey's segment throw with 3 segments: 3 damage. */
static BattleWeapon weapon_sambo_spike_attack_3 = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

/** Attack data of the Pokey's segment throw with 2 segments: 3 damage. */
static BattleWeapon weapon_sambo_spike_attack_2 = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

/** Pose table of the Pokey as a full stack of four segments (the main part 1). */
static PoseTableEntry pose_table_body4[] = {
    {1, "SNB_N_1"},
    {2, "SNB_Y_1"},
    {9, "SNB_Y_1"},
    {5, "SNB_K_1"},
    {4, "SNB_X_1"},
    {3, "SNB_X_1"},
    {BTL_POSE_NORMAL, "SNB_S_1"},
    {29, "SNB_Q_1"},
    {30, "SNB_Q_1"},
    {31, "SNB_S_1"},
    {BTL_POSE_DAMAGE, "SNB_D_1"},
    {47, "SNB_W_1"},
    {POSE_TYPE_RUN, "SNB_W_1"},
    {POSE_TYPE_WALK, "SNB_W_1"},
    {POSE_TYPE_DANGER, "SNB_X_1"},
    {POSE_TYPE_PERIL, "SNB_X_1"},
    {POSE_TYPE_TALK, "SNB_T_1"},
    {POSE_TYPE_ACTION, "SNB_S_1"},
};

/** Pose table of the Pokey once it is down to three segments (part 2). */
static PoseTableEntry pose_table_body3[] = {
    {1, "SNB_N_2"},
    {2, "SNB_Y_2"},
    {9, "SNB_Y_2"},
    {5, "SNB_K_2"},
    {4, "SNB_X_2"},
    {3, "SNB_X_2"},
    {BTL_POSE_NORMAL, "SNB_S_2"},
    {29, "SNB_Q_2"},
    {30, "SNB_Q_2"},
    {31, "SNB_S_2"},
    {BTL_POSE_DAMAGE, "SNB_D_2"},
    {47, "SNB_W_2"},
    {50, "SNB_A_1A"},
    {51, "SNB_A_1B"},
    {POSE_TYPE_RUN, "SNB_W_2"},
    {POSE_TYPE_WALK, "SNB_W_2"},
    {POSE_TYPE_DANGER, "SNB_X_2"},
    {POSE_TYPE_PERIL, "SNB_X_2"},
    {POSE_TYPE_TALK, "SNB_T_2"},
    {POSE_TYPE_ACTION, "SNB_S_2"},
};

/** Pose table of the Pokey once it is down to two segments (part 3). */
static PoseTableEntry pose_table_body2[] = {
    {1, "SNB_N_3"},
    {2, "SNB_Y_3"},
    {9, "SNB_Y_3"},
    {5, "SNB_K_3"},
    {4, "SNB_X_3"},
    {3, "SNB_X_3"},
    {BTL_POSE_NORMAL, "SNB_S_3"},
    {29, "SNB_Q_3"},
    {30, "SNB_Q_3"},
    {31, "SNB_S_3"},
    {BTL_POSE_DAMAGE, "SNB_D_3"},
    {47, "SNB_W_3"},
    {50, "SNB_A_2A"},
    {51, "SNB_A_2B"},
    {POSE_TYPE_RUN, "SNB_W_3"},
    {POSE_TYPE_WALK, "SNB_W_3"},
    {POSE_TYPE_DANGER, "SNB_X_3"},
    {POSE_TYPE_PERIL, "SNB_X_3"},
    {POSE_TYPE_TALK, "SNB_T_3"},
    {POSE_TYPE_ACTION, "SNB_S_3"},
};

/** Pose table of the Pokey once only its head is left (part 4). */
static PoseTableEntry pose_table_body1[] = {
    {1, "SNB_N_4"},
    {2, "SNB_Y_4"},
    {9, "SNB_Y_4"},
    {5, "SNB_K_4"},
    {4, "SNB_X_4"},
    {3, "SNB_X_4"},
    {BTL_POSE_NORMAL, "SNB_S_4"},
    {29, "SNB_Q_4"},
    {30, "SNB_Q_4"},
    {31, "SNB_S_4"},
    {BTL_POSE_DAMAGE, "SNB_D_4"},
    {47, "SNB_W_4"},
    {50, "SNB_A_3A"},
    {51, "SNB_A_3B"},
    {POSE_TYPE_RUN, "SNB_W_4"},
    {POSE_TYPE_WALK, "SNB_W_4"},
    {POSE_TYPE_DANGER, "SNB_X_4"},
    {POSE_TYPE_PERIL, "SNB_X_4"},
    {POSE_TYPE_TALK, "SNB_T_4"},
    {POSE_TYPE_ACTION, "SNB_S_4"},
};

/** Pose table of the loose segment the Pokey throws or sheds (part 5). */
static PoseTableEntry pose_table_parts[] = {
    {BTL_POSE_NORMAL, "SNB_B_1"},
    {BTL_POSE_DAMAGE, "SNB_B_1"},
    {POSE_TYPE_ACTION, "SNB_B_1"},
};

/**
 * Data table of the Pokey: reaction scripts for an attacker hurt by its spikes, losing a body segment,
 * losing a body segment to a hammer blow and being swallowed; a First Strike on it triggers nothing special.
 */
static BattleDataEntry data_table[] = {
    {37, dmg_avoid_counter_thorn_side_event},
    {39, dmg_avoid_counter_thorn_side_event},
    {19, damage_event_bodylost},
    {18, damage_event_bodylost_blow},
    {16, damage_event_bodylost},
    {48, btldefaultevt_Dummy},
    {54, damage_swallow_body},
    {0},
};

/**
 * Parts of the Pokey: part 1 is the full four-segment body,
 * parts 2 to 4 are the shorter stacks it shrinks to (hidden until needed)
 * and part 5 is the loose segment it throws.
 */
static BattleUnitKindPart parts[] = {
    {
        1, // partNum
        "btl_un_sambo", // partName
        "c_sanbo", // modelName
        {0}, // positionOffset
        {0.0f, 84.0f, 0.0f}, // hitBaseOffset
        {0.0f, 90.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        90, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, // attributes
        kCounterTopSpiky | kCounterFrontSpiky, // counterAttributes
        pose_table_body4, // poseTable
    },
    {
        2, // partNum
        "btl_un_sambo", // partName
        "c_sanbo", // modelName
        {0}, // positionOffset
        {0.0f, 64.0f, 0.0f}, // hitBaseOffset
        {0.0f, 70.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        70, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW, // attributes
        kCounterTopSpiky | kCounterFrontSpiky, // counterAttributes
        pose_table_body3, // poseTable
    },
    {
        3, // partNum
        "btl_un_sambo", // partName
        "c_sanbo", // modelName
        {0}, // positionOffset
        {0.0f, 44.0f, 0.0f}, // hitBaseOffset
        {0.0f, 50.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        50, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW, // attributes
        kCounterTopSpiky | kCounterFrontSpiky, // counterAttributes
        pose_table_body2, // poseTable
    },
    {
        4, // partNum
        "btl_un_sambo", // partName
        "c_sanbo", // modelName
        {0}, // positionOffset
        {0.0f, 24.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW, // attributes
        kCounterTopSpiky | kCounterFrontSpiky, // counterAttributes
        pose_table_body1, // poseTable
    },
    {
        5, // partNum
        "btl_un_sambo", // partName
        "c_sanbo", // modelName
        {0}, // positionOffset
        {0}, // hitBaseOffset
        {0}, // hitCursorBaseOffset
        20, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW | PART_ATTR_WORLD_POSITION, // attributes
        0, // counterAttributes
        pose_table_parts, // poseTable
    },
};

/**
 * Battle init script for the Pokey: installs the wait, attack, damage and confusion handlers,
 * sets unit work 0 to 4 (the number of body segments),
 * calls _body_param_set to configure the segments for that count,
 * sets the run and walk sounds and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, 4)
    CALL_CHILD(_body_param_set)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CALL_DEPTH, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 5, POSE_TYPE_ACTION)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE1"), STRING("SFX_ENM_SAMBO_MOVE1"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE1"), STRING("SFX_ENM_SAMBO_MOVE1"), 0, 6, 6)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Data-table events 37 and 39, run by btldefaultevt_Damage for damage codes 31 and 33,
 * which BattleCheckCounter gives the Pokey when its top or front spikes hurt the attacker:
 * it takes pose type 47 and shows a toge_flush spark just above the top of the stack (its height scaled by its
 * size), then waits 60 frames.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Data-table events 37 and 39, run by btldefaultevt_Damage for damage codes 31 and 33,
 * which BattleCheckCounter gives the Pokey when its top or front spikes hurt the attacker:
 * it takes pose type 47 and shows a toge_flush spark just above the top of the stack (its height scaled by its
 * size), then waits 60 frames.
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_side_event)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 47)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, LW(10), LW(3))
    CALL(btlevtcmd_GetStatusMg, LW(10), LW(4))
    MULTIPLY(LW(3), LW(4))
ADD(LW(1), LW(3))
    ADD(LW(1), 5)
    ADD(LW(2), 10)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    RETURN
EVT_END()
#else
/**
 * Data-table events 37 and 39, run by btldefaultevt_Damage for damage codes 31 and 33,
 * which BattleCheckCounter gives the Pokey when its top or front spikes hurt the attacker:
 * it takes pose type 47 and shows a toge_flush spark just above the top of the stack (its height scaled by its
 * size), then waits 60 frames.
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_side_event)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), 47)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, LW(10), LW(3))
    CALL(btlevtcmd_GetStatusMg, LW(10), LW(4))
    MULTIPLY_FLOAT(LW(3), LW(4))
    ADD(LW(1), LW(3))
    ADD(LW(1), 5)
    ADD(LW(2), 10)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    RETURN
EVT_END()
#endif

/**
 * Damage handler: unit work 0 holds the number of body segments left (4 to 1),
 * and the part that takes the hit is the matching segment part (4 down to 1).
 * It then runs the default damage reaction on that part.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        SET_VALUE(LW(11), 4)
        CASE_EQUAL(2)
        SET_VALUE(LW(11), 3)
        CASE_EQUAL(3)
        SET_VALUE(LW(11), 2)
        CASE_DEFAULT
        SET_VALUE(LW(11), 1)
    END_SWITCH
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Data-table events 16 and 19, run by subsetevt_damage for damage pattern 8 (a hit by Koops' shell) and 15
 * or 17 (hit by an enemy knocked into it by the Super or Ultra Hammer): a segment is knocked off.
 * With a single segment left it just plays the hurt reaction (or the no-damage one).
 * Otherwise one segment is lost (unit work 0 down by 1):
 * _body_param_set shrinks the unit's height, the status icon,
 * cut and kiss offsets are lowered to the new top (at one segment Yoshi can swallow it),
 * the old segment part is disabled and the next one becomes the active part,
 * the stack drops (subset_dmg_sambo_fall), and the loose segment (part 5) falls in two hops to the floor behind
 * it, blinking before it disappears.
 */
static EVT_BEGIN(damage_event_bodylost)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetDamage, LW(10), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_FRAMES(30)
            IF_INT_NOT_EQUAL(LW(15), -1)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(15))
                WAIT_FRAMES(30)
            END_IF
        ELSE
            CALL_CHILD(subsetevt_nodamage_motion)
        END_IF
    ELSE
        SET_VALUE(LW(12), LW(11))
        CALL(btlevtcmd_GetUnitWork, LW(10), UNIT_WORK_BODY_COUNT, LW(0))
        SUBTRACT(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, LW(10), UNIT_WORK_BODY_COUNT, LW(0))
        CALL_CHILD(_body_param_set)
        SWITCH(LW(0))
            CASE_EQUAL(1)
            SET_VALUE(LW(11), 4)
            CALL(btlevtcmd_SetSwallowParam, UNIT_ID_SELF, 50)
            CALL(btlevtcmd_SetSwallowAttribute, UNIT_ID_SELF, 2)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 20)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 10, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 16, 0)
            CASE_EQUAL(2)
            SET_VALUE(LW(11), 3)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 40)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 20, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 26, 0)
            CASE_EQUAL(3)
            SET_VALUE(LW(11), 2)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 60)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 30, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 36, 0)
            CASE_DEFAULT
            SET_VALUE(LW(11), 1)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 80)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 40, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 52, 0)
        END_SWITCH
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(12), PART_ATTR_MAIN | PART_ATTR_GUARD_COVER)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(12), PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), PART_ATTR_MAIN | PART_ATTR_GUARD_COVER)
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            CALL_CHILD(subset_dmg_sambo_fall)
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, LW(10), 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_SetPartsFallAccel, LW(10), 5, FLOAT(0.39941406f))
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 25)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 16, -1)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 16)
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 12, -1)
        LOOP(40)
            CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, PART_ATTR_HIDDEN)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, PART_ATTR_HIDDEN)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, PART_ATTR_HIDDEN)
    END_IF
    RETURN
EVT_END()

/**
 * Drop of the remaining stack when a segment is knocked out from under it:
 * plays the hurt pose in LW(14), lifts the unit 18 units
 * and lets it fall back down with a small bounce over about 15 frames.
 */
static EVT_BEGIN(subset_dmg_sambo_fall)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(1), 18)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(1))
    WAIT_FRAMES(2)
    SUBTRACT(LW(3), 1)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(3)
    SUBTRACT(LW(3), 2)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(2)
    SUBTRACT(LW(3), 3)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(1)
    SUBTRACT(LW(3), 5)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(1)
    SUBTRACT(LW(3), 7)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(1)
    ADD(LW(3), 2)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(1)
    ADD(LW(3), 3)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(1)
    ADD(LW(3), 3)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(1)
    SUBTRACT(LW(3), 3)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(1)
    SUBTRACT(LW(3), 3)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    WAIT_FRAMES(1)
    SUBTRACT(LW(3), 2)
    CALL(btlevtcmd_SetPos, LW(10), LW(0), LW(3), LW(2))
    RETURN
EVT_END()

/**
 * Data-table event 18, run by subsetevt_damage for damage patterns 14
 * and 16 (struck by Mario's Super Hammer or Ultra Hammer):
 * like damage_event_bodylost, but the knocked-off segment (part 5) is sent flying along the Pokey's side
 * and hits each unit behind it in turn with marioWeapon_UltraHammerInvolved (its test for pattern 15,
 * which would pick marioWeapon_KaitenHammerInvolved, never matches since this event only runs for 14 and 16),
 * then flies off the stage edge.
 * For the Ultra Hammer (pattern 16) the Pokey then takes the finishing hit (marioWeapon_UltraHammerFinish).
 * With a single segment left it only plays the knock-back (subsetevt_shot_damage) or no-damage reaction.
 */
static EVT_BEGIN(damage_event_bodylost_blow)
    // With only the head left there is nothing to knock off: just the knock-back or no-damage reaction plays.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetDamage, LW(10), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL_CHILD(subsetevt_shot_damage)
        ELSE
            CALL_CHILD(subsetevt_nodamage_motion)
        END_IF
    ELSE
        SET_VALUE(LW(12), LW(11))
        CALL(btlevtcmd_GetUnitWork, LW(10), UNIT_WORK_BODY_COUNT, LW(0))
        SUBTRACT(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, LW(10), UNIT_WORK_BODY_COUNT, LW(0))
        // The Pokey loses a segment: the stack gets one shorter and its sizes and hit positions are adjusted.
        CALL_CHILD(_body_param_set)
        SWITCH(LW(0))
            CASE_EQUAL(1)
            SET_VALUE(LW(11), 4)
            CALL(btlevtcmd_SetSwallowParam, UNIT_ID_SELF, 50)
            CALL(btlevtcmd_SetSwallowAttribute, UNIT_ID_SELF, 2)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 20)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 10, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 16, 0)
            CASE_EQUAL(2)
            SET_VALUE(LW(11), 3)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 40)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 20, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 26, 0)
            CASE_EQUAL(3)
            SET_VALUE(LW(11), 2)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 60)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 30, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 36, 0)
            CASE_DEFAULT
            SET_VALUE(LW(11), 1)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 80)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 40, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 52, 0)
        END_SWITCH
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(12), PART_ATTR_MAIN | PART_ATTR_GUARD_COVER)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(12), PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), PART_ATTR_MAIN | PART_ATTR_GUARD_COVER)
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            // The knocked-off segment (part 5) is thrown aside while the rest of the stack drops into place.
            CALL_CHILD(subset_dmg_sambo_fall)
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, LW(10), 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 15, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(13), PTR(&marioWeapon_KaitenHammerInvolved))
        ELSE
            SET_VALUE(LW(13), PTR(&marioWeapon_UltraHammerInvolved))
        END_IF
        // It flies along the Pokey's side and hits each unit behind it in turn.
        CALL(btlevtcmd_GetFriendBelong, LW(10), LW(0))
        CALL(btlevtcmd_SamplingEnemy, LW(10), LW(0), LW(13))
        CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
        LABEL(0)
        IF_INT_EQUAL(LW(3), -1)
            GOTO(50)
        END_IF
        CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(10))
        IF_INT_NOT_EQUAL(LW(10), LW(3))
            CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
            GOTO(0)
        END_IF
        LABEL(10)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            GOTO(50)
        END_IF
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHomePos, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
        ADD(LW(1), 15)
        CALL(btlevtcmd_JumpPartsSetting, LW(10), 5, 0, FLOAT(5.0f), FLOAT(0.009765625f))
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 0, -1)
        // That unit takes the segment as a hit.
        CALL(btlevtcmd_CheckDamage, LW(10), LW(3), LW(4), LW(13), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(10)
        LABEL(50)
        // The segment keeps going until it flies off the edge of the stage.
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), LW(5))
        CALL(btlevtcmd_GetPos, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_JumpPartsSetting, LW(10), 5, 0, FLOAT(5.0f), FLOAT(0.009765625f))
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 0, -1)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_CheckDamagePattern, LW(10), 16, LW(5))
        IF_INT_NOT_EQUAL(LW(5), 0)
            // After an Ultra Hammer hit the Pokey also takes the finishing blow.
            CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), PTR(marioWeapon_UltraHammerFinish), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Data-table event 54, run by btldefaultevt_Damage for damage pattern 7,
 * which Yoshi's Gulp deals (btlevtcmd_DamageDirect) to a target it can swallow: Yoshi swallows one segment.
 * With a single segment left it only plays the hurt (or no-damage) reaction;
 * otherwise one segment is lost as in damage_event_bodylost, the stack drops,
 * and the loose segment (part 5) slides 25 units forward from the Pokey's base into Yoshi's mouth
 * and disappears.
 */
static EVT_BEGIN(damage_swallow_body)
    SET_VALUE(LW(14), 39)
    SET_VALUE(LW(15), -1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetDamage, LW(10), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_FRAMES(30)
            IF_INT_NOT_EQUAL(LW(15), -1)
                CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(15))
                WAIT_FRAMES(30)
            END_IF
        ELSE
            CALL_CHILD(subsetevt_nodamage_motion)
        END_IF
    ELSE
        WAIT_FRAMES(3)
        SET_VALUE(LW(12), LW(11))
        CALL(btlevtcmd_GetUnitWork, LW(10), UNIT_WORK_BODY_COUNT, LW(0))
        SUBTRACT(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, LW(10), UNIT_WORK_BODY_COUNT, LW(0))
        CALL_CHILD(_body_param_set)
        SWITCH(LW(0))
            CASE_EQUAL(1)
            SET_VALUE(LW(11), 4)
            CALL(btlevtcmd_SetSwallowParam, UNIT_ID_SELF, 50)
            CALL(btlevtcmd_SetSwallowAttribute, UNIT_ID_SELF, 2)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 20)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 10, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 16, 0)
            CASE_EQUAL(2)
            SET_VALUE(LW(11), 3)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 40)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 20, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 26, 0)
            CASE_EQUAL(3)
            SET_VALUE(LW(11), 2)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 60)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 30, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 36, 0)
            CASE_DEFAULT
            SET_VALUE(LW(11), 1)
            CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 80)
            CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 40, 0)
            CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 52, 0)
        END_SWITCH
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(12), PART_ATTR_MAIN | PART_ATTR_GUARD_COVER)
            CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(12), PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW)
            CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), PART_ATTR_MAIN | PART_ATTR_GUARD_COVER)
            CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_FRAMES(1)
            CALL_CHILD(subset_dmg_sambo_fall)
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, LW(10), 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 25)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 5, 0, 0, 0, -1)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, PART_ATTR_HIDDEN)
        WAIT_FRAMES(20)
    END_IF
    RETURN
EVT_END()

/**
 * Attack AI of the Pokey. It uses an enemy item if it has one.
 * At 4 segments, if it has called friends at most once,
 * it has a 30% chance to call another Pokey (call_friend_event) when there is room beside it.
 * With 2 or more segments it then draws: about 30 in 69 for a head attack (attack_head_2_event,
 * with the weapon matching the segment count), otherwise it fires its spike (attack_spike_event);
 * with a single segment it hops at the target (attack_head_1_event).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(10))
    SWITCH(LW(10))
        CASE_EQUAL(1)
        SET_VALUE(LW(11), 4)
        CASE_EQUAL(2)
        SET_VALUE(LW(11), 3)
        CASE_EQUAL(3)
        SET_VALUE(LW(11), 2)
        CASE_DEFAULT
        SET_VALUE(LW(11), 1)
    END_SWITCH
    IF_INT_EQUAL(LW(10), 4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CALL_DEPTH, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 1)
            CALL(evt_sub_random, 99, LW(0))
            IF_INT_LESS(LW(0), 30)
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
                CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(6), 40)
                SUBTRACT(LW(8), 10)
                CALL(btlevtcmd_CheckSpace, LW(3), LW(6), LW(7), LW(8), 40, 0, 0, 0)
                IF_INT_EQUAL(LW(3), -1)
                    IF_INT_GREATER(LW(6), 0)
                        CALL_CHILD(call_friend_event)
                        GOTO(99)
                    END_IF
                END_IF
            END_IF
        END_IF
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(10), 2)
        CALL(evt_sub_random, 69, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 30)
            SWITCH(LW(10))
                CASE_EQUAL(4)
                CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_head_attack_4))
                CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_4), LW(3), LW(4))
                CASE_EQUAL(3)
                CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_head_attack_3))
                CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_3), LW(3), LW(4))
                CASE_EQUAL(2)
                CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_head_attack_2))
                CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_2), LW(3), LW(4))
            END_SWITCH
            IF_INT_NOT_EQUAL(LW(3), -1)
                CALL_CHILD(attack_head_2_event)
                RETURN
            END_IF
        END_IF
        CALL_CHILD(attack_spike_event)
        RETURN
    END_IF
    CALL_CHILD(attack_head_1_event)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Pokey's headbutt with 2 to 4 segments (weapon_sambo_head_attack_2 to 4, matching its segment count):
 * it walks up to the target (closer for a shorter or Tiny stack),
 * leans back 40 degrees and swings the whole stack forward 90 degrees onto it.
 * The pre-damage check decides between the avoid animation and the damage (with the guard result);
 * the stack then straightens up and walks home.
 */
static EVT_BEGIN(attack_head_2_event)
    SET_VALUE(LW(6), LW(11))
    // A Tiny Pokey stands closer to its target.
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(5))
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        // A target is picked using the weapon for the stack height (4, 3 or 2 segments).
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_head_attack_4))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_4), LW(3), LW(4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_head_attack_3))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_3), LW(3), LW(4))
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_head_attack_2))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_2), LW(3), LW(4))
    END_SWITCH
    // No target to attack: a confused Pokey just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    // The headbutt is declared.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_head_attack_4))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_head_attack_4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_head_attack_3))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_head_attack_3))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_head_attack_2))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_head_attack_2))
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SWITCH(LW(10))
        CASE_EQUAL(4)
        IF_INT_EQUAL(LW(5), 0)
            CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 60)
        ELSE
            CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 42)
        END_IF
        CASE_EQUAL(3)
        IF_INT_EQUAL(LW(5), 0)
            CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 40)
        ELSE
            CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 28)
        END_IF
        CASE_EQUAL(2)
        IF_INT_EQUAL(LW(5), 0)
            CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
        ELSE
            CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 14)
        END_IF
    END_SWITCH
    // The whole stack walks up to the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_WALK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), BTL_POSE_STAY)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(14), 40)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), -40)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    // It leans back 40 degrees to wind up.
    CALL(evt_sub_intpl_init, 11, 0, LW(14), 40)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(13), 40)
    SET_VALUE(LW(14), -90)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(13), -40)
        SET_VALUE(LW(14), 90)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    // Then it swings the whole stack forward onto the target.
    CALL(evt_sub_intpl_init, 1, LW(13), LW(14), 10)
    LABEL(3)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    SWITCH(LW(10))
        CASE_EQUAL(4)
        // Check whether the attack will connect; a miss sends the target into its dodge.
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_head_attack_4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_head_attack_3), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_head_attack_2), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    // A miss: the target dodges and the stack swings back upright.
    LABEL(90)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(14), -90)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), 90)
    END_IF
    CALL(evt_sub_intpl_init, 0, LW(14), 0, 10)
    LABEL(4)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    GOTO(98)
    // A hit: the target takes the damage and the stack straightens up.
    LABEL(91)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_head_attack_4))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_head_attack_4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(3)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_head_attack_3))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_head_attack_3), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_head_attack_2))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_head_attack_2), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 90)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), BTL_POSE_STAY)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(14), -90)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), 90)
    END_IF
    CALL(evt_sub_intpl_init, 0, LW(14), 0, 20)
    LABEL(5)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    // The headbutt is over: the stack walks back to its place.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), 41)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    // Turn finished: the Pokey goes back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The one-segment Pokey's attack (weapon_sambo_head_attack_1):
 * it hops to 50 units in front of the target in 35-unit hops (sambo_rabbit_move),
 * then leaps onto it while tilting forward; a hit shakes the camera,
 * applies the guard result and the damage and bounces it back in four hops, a miss carries it past.
 * It then hops home.
 */
static EVT_BEGIN(attack_head_1_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_head_attack_1))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_1), LW(3), LW(4))
    // No target to attack: a confused Pokey just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_head_attack_1))
    // The lone segment announces its attack.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_head_attack_1))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_TransStageFloorPosition, LW(5), LW(6), LW(7))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(5), 50)
    SET_VALUE(LW(8), 35)
    // It hops up to 50 units in front of the target, 35 units at a time.
    CALL_CHILD(sambo_rabbit_move)
    WAIT_FRAMES(10)
    // Check whether the attack will connect; a miss sends the target into its dodge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_head_attack_1), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    // A miss: it leaps right over the target, tilting forward, and lands beyond it.
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 5)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(2)
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
        SET_VALUE(LW(14), -10)
        IF_INT_EQUAL(LW(15), -1)
            SET_VALUE(LW(14), 10)
        END_IF
        SET_VALUE(LW(0), 0)
        LOOP(15)
            ADD(LW(0), LW(14))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 44, -1)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 10, 0)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(2)
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
        SET_VALUE(LW(0), 180)
        LOOP(12)
            IF_INT_EQUAL(LW(15), -1)
                ADD(LW(0), 15)
                IF_INT_EQUAL(LW(0), 360)
                    SET_VALUE(LW(0), 0)
                END_IF
            ELSE
                SUBTRACT(LW(0), 15)
                IF_INT_EQUAL(LW(0), 0)
                    SET_VALUE(LW(0), 360)
                END_IF
            END_IF
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 30)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    GOTO(98)
    // A hit: it leaps onto the target, tilting forward, and the target takes the damage.
    LABEL(91)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(2)
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
        SET_VALUE(LW(14), -10)
        IF_INT_EQUAL(LW(15), -1)
            SET_VALUE(LW(14), 10)
        END_IF
        SET_VALUE(LW(0), 0)
        LOOP(15)
            ADD(LW(0), LW(14))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 36, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_head_attack_1))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_head_attack_1), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.89941406f))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    // It bounces off the target in four shrinking hops.
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 4, BTL_POSE_STAY)
    // It hops back to its place.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 4, 41)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    SET_VALUE(LW(8), 35)
    CALL_CHILD(sambo_rabbit_move)
    // Turn finished: the Pokey goes back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Pokey throws one of its own segments (weapon_sambo_spike_attack_4 to 2 by segment count):
 * it loses a segment as in damage_event_bodylost,
 * and the loose segment (part 5, smaller when Tiny) pops up and flies at the target.
 * A miss lets it fly off stage while the target dodges;
 * a hit applies the guard result and the damage and the segment bounces off to the floor and disappears.
 * If the target Superguards (CheckDamage result 21), the segment flies back and hits the Pokey itself.
 */
static EVT_BEGIN(attack_spike_event)
    // The weapon depends on how many segments the stack has (4, 3 or 2).
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(10))
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_spike_attack_4))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_spike_attack_4), LW(3), LW(4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_spike_attack_3))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_spike_attack_3), LW(3), LW(4))
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_spike_attack_2))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_spike_attack_2), LW(3), LW(4))
    END_SWITCH
    // No target to attack: a confused Pokey just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    // The throw is declared.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_spike_attack_4))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_spike_attack_4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_spike_attack_3))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_spike_attack_3))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_spike_attack_2))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_spike_attack_2))
    END_SWITCH
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        // The segment about to be thrown (part 5) is smaller when the Pokey is Tiny.
        CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, 5, FLOAT(0.69921875f), FLOAT(0.69921875f), FLOAT(1.0f))
    ELSE
        CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, 5, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(10))
    SUBTRACT(LW(10), 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(10))
    // The stack loses a segment, as when it is knocked off.
    CALL_CHILD(_body_param_set)
    SWITCH(LW(10))
        CASE_EQUAL(3)
        SET_VALUE(LW(7), 1)
        SET_VALUE(LW(6), 2)
        CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 60)
        CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 30, 0)
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 36, 0)
        CASE_EQUAL(2)
        SET_VALUE(LW(7), 2)
        SET_VALUE(LW(6), 3)
        CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 40)
        CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 20, 0)
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 26, 0)
        CASE_EQUAL(1)
        SET_VALUE(LW(7), 3)
        SET_VALUE(LW(6), 4)
        CALL(btlevtcmd_SetSwallowParam, UNIT_ID_SELF, 50)
        CALL(btlevtcmd_SetSwallowAttribute, UNIT_ID_SELF, 2)
        CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 14, 20)
        CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 10, 0)
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 10, 16, 0)
    END_SWITCH
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(7), PART_ATTR_MAIN | PART_ATTR_GUARD_COVER)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(7), PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(6), PART_ATTR_MAIN | PART_ATTR_GUARD_COVER)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(6), PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(5))
    // The loose segment pops up and flies toward the target.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        IF_INT_EQUAL(LW(5), 0)
            SET_VALUE(LW(9), 20)
            SET_VALUE(LW(11), 20)
        ELSE
            SET_VALUE(LW(9), 14)
            SET_VALUE(LW(11), 14)
        END_IF
        MULTIPLY(LW(9), LW(10))
        SUBTRACT(LW(9), LW(11))
        ADD(LW(1), LW(9))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 5, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 5, FLOAT(0.5996094f))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("SNB_B_1"))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 30, -1)
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    // The Pokey rears back and throws it.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), 50)
    WAIT_FRAMES(24)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK5"), EVT_NULLPTR, 0, LW(14))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), 51)
    WAIT_FRAMES(2)
    WAIT_FRAMES(5)
    SWITCH(LW(10))
        CASE_EQUAL(3)
        // Check whether the attack will connect; a miss sends the target into its dodge.
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_spike_attack_4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_spike_attack_3), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(1)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_spike_attack_2), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    // A miss: the segment flies off the stage while the target dodges.
    LABEL(90)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(9))
    MULTIPLY(LW(0), LW(9))
    SUBTRACT(LW(1), 5)
    CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, 5, FLOAT(10.0f))
    CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 5, FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 0, -1)
    CALL(evt_snd_sfxoff, LW(14))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 5, PART_ATTR_HIDDEN)
    GOTO(99)
    // A hit: the target takes the damage and the segment bounces away across the floor and vanishes.
    LABEL(91)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, 5, FLOAT(10.0f))
    CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 5, FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 0, -1)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    SWITCH(LW(10))
        CASE_EQUAL(3)
        SET_VALUE(LW(12), PTR(&weapon_sambo_spike_attack_4))
        CASE_EQUAL(2)
        SET_VALUE(LW(12), PTR(&weapon_sambo_spike_attack_3))
        CASE_EQUAL(1)
        SET_VALUE(LW(12), PTR(&weapon_sambo_spike_attack_2))
    END_SWITCH
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), BTL_POSE_STAY)
    // Superguard: the segment flies back and hits the Pokey itself.
    IF_INT_EQUAL(LW(5), 21)
        CALL(evt_snd_sfxoff, LW(14))
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(11))
        CALL(btlevtcmd_GetHitPos, UNIT_ID_SELF, LW(11), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, 5, FLOAT(4.5f))
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 5, FLOAT(0.39941406f))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 40, -1)
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 55)
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, 5, FLOAT(3.0f))
            CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 5, FLOAT(0.099609375f))
            CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 0, 25, 0, 0, -1)
            CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, 5, FLOAT(2.0f))
            CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 24)
            CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 0, 21, 0, 0, -1)
            CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, 5, FLOAT(1.5f))
            CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
            CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 0, 20, 0, 0, -1)
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 5, PART_ATTR_HIDDEN)
        END_INLINE_EVENT
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, LW(11), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    CALL(evt_snd_sfxoff, LW(14))
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 55)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, 5, FLOAT(3.0f))
    CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 5, FLOAT(0.099609375f))
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 0, 25, 0, 0, -1)
    CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, 5, FLOAT(2.0f))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 24)
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 0, 21, 0, 0, -1)
    CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, 5, FLOAT(1.5f))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 20)
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 0, 20, 0, 0, -1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 5, PART_ATTR_HIDDEN)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    // Turn finished: the Pokey goes back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Pokey calls another Pokey: it sways left and right in widening arcs,
 * then a new Pokey (entry_sambo) rises out of the floor at the free spot beside it with a puff of smoke
 * and a camera shake, and that spot becomes its home.
 * The new unit's unit work 1 is set one higher than the caller's (attack_event only lets Pokeys with unit work 1
 * at most 1 call).
 */
static EVT_BEGIN(call_friend_event)
    // The Pokey sways left and right in widening arcs.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SNB_S_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_SEND_GROUP1_3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 11, 0, -20, 40)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_SEND_GROUP1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 11, -20, 30, 40)
    LABEL(1)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_SEND_GROUP1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 11, 30, -40, 40)
    LABEL(2)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_SEND_GROUP2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 11, -40, 90, 20)
    LABEL(3)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    WAIT_FRAMES(40)
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    END_INLINE_EVENT
    // A new Pokey is spawned beneath the floor beside it.
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_sambo), 0)
    SET_VALUE(LW(0), LW(6))
    SET_VALUE(LW(1), LW(7))
    SET_VALUE(LW(2), LW(8))
    SUBTRACT(LW(1), 80)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        // A puff of smoke goes up as the new one rises out of the floor.
        CALL(evt_eff, 0, STRING("kemuri_test"), 14, LW(0), LW(1), LW(2), FLOAT(1.5f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_SEND_GROUP3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("SNB_D_1"))
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, LW(3), FLOAT(0.69921875f))
        CALL(btlevtcmd_JumpPosition, LW(3), LW(6), LW(7), LW(8), 20, -1)
        CALL(btlevtcmd_SetHomePos, LW(3), LW(6), LW(7), LW(8))
        WAIT_FRAMES(40)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    // The new one counts one call deeper than the caller, which caps how many may call friends.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CALL_DEPTH, LW(4))
    ADD(LW(4), 1)
    CALL(btlevtcmd_SetUnitWork, LW(3), UNIT_WORK_CALL_DEPTH, LW(4))
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_sub_intpl_init, 4, 90, -30, 14)
    LABEL(4)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    WAIT_FRAMES(40)
    // The caller settles back upright.
    CALL(evt_sub_intpl_init, 11, -30, 0, 20)
    LABEL(5)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    WAIT_FRAMES(40)
    RETURN
EVT_END()

/**
 * Spawn setup for another Pokey, which the Pokey calls into the battle:
 * enemy side, acting in the enemy phase, placed far off-screen until the calling script moves it into position.
 */
static BattleUnitSetup entry_sambo = {
    &unit_sambo, // unitKindParams
    1, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Idle script: picks the body part that matches the number of segments left (unit work 0, 4 to 1)
 * and sets the pose from the pose table for it.
 */
static EVT_BEGIN(wait_event)
    SET_VALUE(LW(10), -2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        SET_VALUE(LW(11), 4)
        CASE_EQUAL(2)
        SET_VALUE(LW(11), 3)
        CASE_EQUAL(3)
        SET_VALUE(LW(11), 2)
        CASE_DEFAULT
        SET_VALUE(LW(11), 1)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseFromTable, LW(10), LW(11))
    RETURN
EVT_END()

/**
 * Hopping movement for the Pokey from its position to the target position in LW(5) to LW(7),
 * in jumps of at most LW(8) units in x, with a hop sound each time and a final hop onto the target.
 */
static EVT_BEGIN(sambo_rabbit_move)
    LABEL(0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(11), LW(5))
    SUBTRACT(LW(11), LW(0))
    SET_VALUE(LW(12), LW(11))
    IF_INT_LESS(LW(12), 0)
        MULTIPLY(LW(12), -1)
    END_IF
    IF_INT_GREATER(LW(12), LW(8))
        IF_INT_GREATER_OR_EQUAL(LW(11), 0)
            ADD(LW(0), LW(8))
        ELSE
            SUBTRACT(LW(0), LW(8))
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(6), LW(2), 0, -1)
        GOTO(0)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(5), LW(6), LW(7), 0, -1)
    END_IF
    RETURN
EVT_END()

/** Sets the Pokey's height and cut height to 20 units per body segment left (unit work 0): 20, 40, 60 or 80. */
static EVT_BEGIN(_body_param_set)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BODY_COUNT, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetHeight, UNIT_ID_SELF, 20)
        CALL(btlevtcmd_SetCutHeight, UNIT_ID_SELF, 20)
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetHeight, UNIT_ID_SELF, 40)
        CALL(btlevtcmd_SetCutHeight, UNIT_ID_SELF, 40)
        CASE_EQUAL(3)
        CALL(btlevtcmd_SetHeight, UNIT_ID_SELF, 60)
        CALL(btlevtcmd_SetCutHeight, UNIT_ID_SELF, 60)
        CASE_DEFAULT
        CALL(btlevtcmd_SetHeight, UNIT_ID_SELF, 80)
        CALL(btlevtcmd_SetCutHeight, UNIT_ID_SELF, 80)
    END_SWITCH
    RETURN
EVT_END()
