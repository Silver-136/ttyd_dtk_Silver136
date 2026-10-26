#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_mario.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "evt/evt_eff.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"
#include "system.h"


BattleUnitKind unit_sambo_mummy;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleWeapon weapon_sambo_mummy_head_attack_4;
static BattleWeapon weapon_sambo_mummy_head_attack_3;
static BattleWeapon weapon_sambo_mummy_head_attack_2;
static BattleWeapon weapon_sambo_mummy_head_attack_1;
static BattleWeapon weapon_sambo_mummy_spike_attack_4;
static BattleWeapon weapon_sambo_mummy_spike_attack_3;
static BattleWeapon weapon_sambo_mummy_spike_attack_2;
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

BattleUnitKind unit_sambo_mummy = {
    UNIT_POISON_POKEY, // mUnitType
    "btl_un_sambo_mummy", // name
    8, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    23, // mLevel
    0, // mBonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    26, // width
    80, // height
    {14, 80}, // hitOffset
    {0.0f, 40.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {13.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {13.0f, 0.0f, 0.0f}, // bintaHitOffset
    {13.0f, 52.0f, 0.0f}, // kissHitOffset
    {0.0f, 40.0f, 0.0f}, // cutBaseOffset
    26.0f, // cutWidth
    80.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_SAMBO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    5, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    75, // sleep
    60, // stop
    60, // dizzy
    100, // poison
    70, // confuse
    100, // electric
    100, // burn
    40, // freeze
    100, // huge
    85, // tiny
    100, // attackUp
    85, // attackDown
    100, // defenseUp
    90, // defenseDown
    70, // allergic
    80, // fright
    70, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};

static BattleWeapon weapon_sambo_mummy_head_attack_4 = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    0, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
    60, // poisonChance
    10, // poisonTime
    1, // poisonStrength
    0, 0, 0, 0, 0, 0, 0, 0, // confuseChance..burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_sambo_mummy_head_attack_3 = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    0, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
    60, // poisonChance
    10, // poisonTime
    1, // poisonStrength
    0, 0, 0, 0, 0, 0, 0, 0, // confuseChance..burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_sambo_mummy_head_attack_2 = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    0, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
    60, // poisonChance
    10, // poisonTime
    1, // poisonStrength
    0, 0, 0, 0, 0, 0, 0, 0, // confuseChance..burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_sambo_mummy_head_attack_1 = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    0, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
    60, // poisonChance
    10, // poisonTime
    1, // poisonStrength
    0, 0, 0, 0, 0, 0, 0, 0, // confuseChance..burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_sambo_mummy_spike_attack_4 = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_sambo_mummy_spike_attack_3 = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_sambo_mummy_spike_attack_2 = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static PoseTableEntry pose_table_body4[] = {
    {POSE_TYPE_SLEEP, "SNB_N_1"},
    {POSE_TYPE_STOP, "SNB_Y_1"},
    {POSE_TYPE_FREEZE, "SNB_Y_1"},
    {POSE_TYPE_CONFUSE, "SNB_K_1"},
    {POSE_TYPE_POISON, "SNB_X_1"},
    {POSE_TYPE_DIZZY, "SNB_X_1"},
    {POSE_TYPE_NORMAL, "SNB_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "SNB_Q_1"},
    {30, "SNB_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "SNB_S_1"},
    {POSE_TYPE_DAMAGE, "SNB_D_1"},
    {47, "SNB_W_1"},
    {POSE_TYPE_RUN, "SNB_W_1"},
    {POSE_TYPE_WALK, "SNB_W_1"},
    {POSE_TYPE_DANGER, "SNB_X_1"},
    {POSE_TYPE_PERIL, "SNB_X_1"},
    {POSE_TYPE_TALK, "SNB_T_1"},
    {POSE_TYPE_ACTION, "SNB_S_1"},
};

static PoseTableEntry pose_table_body3[] = {
    {POSE_TYPE_SLEEP, "SNB_N_2"},
    {POSE_TYPE_STOP, "SNB_Y_2"},
    {POSE_TYPE_FREEZE, "SNB_Y_2"},
    {POSE_TYPE_CONFUSE, "SNB_K_2"},
    {POSE_TYPE_POISON, "SNB_X_2"},
    {POSE_TYPE_DIZZY, "SNB_X_2"},
    {POSE_TYPE_NORMAL, "SNB_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "SNB_Q_2"},
    {30, "SNB_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "SNB_S_2"},
    {POSE_TYPE_DAMAGE, "SNB_D_2"},
    {47, "SNB_W_2"},
    {POSE_TYPE_ATTACK, "SNB_A_1A"},
    {51, "SNB_A_1B"},
    {POSE_TYPE_RUN, "SNB_W_2"},
    {POSE_TYPE_WALK, "SNB_W_2"},
    {POSE_TYPE_DANGER, "SNB_X_2"},
    {POSE_TYPE_PERIL, "SNB_X_2"},
    {POSE_TYPE_TALK, "SNB_T_2"},
    {POSE_TYPE_ACTION, "SNB_S_2"},
};

static PoseTableEntry pose_table_body2[] = {
    {POSE_TYPE_SLEEP, "SNB_N_3"},
    {POSE_TYPE_STOP, "SNB_Y_3"},
    {POSE_TYPE_FREEZE, "SNB_Y_3"},
    {POSE_TYPE_CONFUSE, "SNB_K_3"},
    {POSE_TYPE_POISON, "SNB_X_3"},
    {POSE_TYPE_DIZZY, "SNB_X_3"},
    {POSE_TYPE_NORMAL, "SNB_S_3"},
    {POSE_TYPE_DAMAGE_FIRE, "SNB_Q_3"},
    {30, "SNB_Q_3"},
    {POSE_TYPE_DAMAGE_GUARDED, "SNB_S_3"},
    {POSE_TYPE_DAMAGE, "SNB_D_3"},
    {47, "SNB_W_3"},
    {POSE_TYPE_ATTACK, "SNB_A_2A"},
    {51, "SNB_A_2B"},
    {POSE_TYPE_RUN, "SNB_W_3"},
    {POSE_TYPE_WALK, "SNB_W_3"},
    {POSE_TYPE_DANGER, "SNB_X_3"},
    {POSE_TYPE_PERIL, "SNB_X_3"},
    {POSE_TYPE_TALK, "SNB_T_3"},
    {POSE_TYPE_ACTION, "SNB_S_3"},
};

static PoseTableEntry pose_table_body1[] = {
    {POSE_TYPE_SLEEP, "SNB_N_4"},
    {POSE_TYPE_STOP, "SNB_Y_4"},
    {POSE_TYPE_FREEZE, "SNB_Y_4"},
    {POSE_TYPE_CONFUSE, "SNB_K_4"},
    {POSE_TYPE_POISON, "SNB_X_4"},
    {POSE_TYPE_DIZZY, "SNB_X_4"},
    {POSE_TYPE_NORMAL, "SNB_S_4"},
    {POSE_TYPE_DAMAGE_FIRE, "SNB_Q_4"},
    {30, "SNB_Q_4"},
    {POSE_TYPE_DAMAGE_GUARDED, "SNB_S_4"},
    {POSE_TYPE_DAMAGE, "SNB_D_4"},
    {47, "SNB_W_4"},
    {POSE_TYPE_ATTACK, "SNB_A_3A"},
    {51, "SNB_A_3B"},
    {POSE_TYPE_RUN, "SNB_W_4"},
    {POSE_TYPE_WALK, "SNB_W_4"},
    {POSE_TYPE_DANGER, "SNB_X_4"},
    {POSE_TYPE_PERIL, "SNB_X_4"},
    {POSE_TYPE_TALK, "SNB_T_4"},
    {POSE_TYPE_ACTION, "SNB_S_4"},
};

static PoseTableEntry pose_table_parts[] = {
    {POSE_TYPE_NORMAL, "SNB_B_1"},
    {POSE_TYPE_DAMAGE, "SNB_B_1"},
    {POSE_TYPE_ACTION, "SNB_B_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_side_event},
    {UNIT_DATA_AVOID_THORN_EVENT, dmg_avoid_counter_thorn_side_event},
    {19, damage_event_bodylost},
    {18, damage_event_bodylost_blow},
    {16, damage_event_bodylost},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_SWALLOW_BODY_EVENT, damage_swallow_body},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_sambo_mummy", "c_sanbo_m", 0.0f, 0.0f, 0.0f, 0.0f, 84.0f, 0.0f, 0.0f, 90.0f, 0.0f, 20, 90, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, kCounterTopSpiky | kCounterFrontSpiky, pose_table_body4},
    {2, "btl_un_sambo_mummy", "c_sanbo_m", 0.0f, 0.0f, 0.0f, 0.0f, 64.0f, 0.0f, 0.0f, 70.0f, 0.0f, 20, 70, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow, kCounterTopSpiky | kCounterFrontSpiky, pose_table_body3},
    {3, "btl_un_sambo_mummy", "c_sanbo_m", 0.0f, 0.0f, 0.0f, 0.0f, 44.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 50, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow, kCounterTopSpiky | kCounterFrontSpiky, pose_table_body2},
    {4, "btl_un_sambo_mummy", "c_sanbo_m", 0.0f, 0.0f, 0.0f, 0.0f, 24.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow, kCounterTopSpiky | kCounterFrontSpiky, pose_table_body1},
    {5, "btl_un_sambo_mummy", "c_sanbo_m", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_parts},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 4)
    CALL_CHILD(PTR(_body_param_set))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 5, POSE_TYPE_ACTION)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE1"), STRING("SFX_ENM_SAMBO_MOVE1"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_MOVE1"), STRING("SFX_ENM_SAMBO_MOVE1"), 0, 6, 6)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_avoid_counter_thorn_side_event)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_AVOID_SPIKY_SIDE)
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

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
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
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_bodylost)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
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
            CALL_CHILD(PTR(subsetevt_nodamage_motion))
        END_IF
        ELSE
        SET_VALUE(LW(12), LW(11))
        CALL(btlevtcmd_GetUnitWork, LW(10), 0, LW(0))
        SUBTRACT(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, LW(10), 0, LW(0))
        CALL_CHILD(PTR(_body_param_set))
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
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(12), (kMainPart | kGuardTargetPart))
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(12), (kPartUntargetable | kHidden | kNoShadow))
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), (kMainPart | kGuardTargetPart))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), (kPartUntargetable | kHidden))
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            CALL_CHILD(PTR(subset_dmg_sambo_fall))
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, LW(10), 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, kHidden)
        CALL(btlevtcmd_SetPartsFallAccel, LW(10), 5, FLOAT(0.39941406f))
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 25)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 16, -1)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 16)
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 12, -1)
        LOOP(40)
            CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, kHidden)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, kHidden)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, kHidden)
    END_IF
    RETURN
EVT_END()

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

static EVT_BEGIN(damage_event_bodylost_blow)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetDamage, LW(10), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL_CHILD(subsetevt_shot_damage)
        ELSE
            CALL_CHILD(subsetevt_nodamage_motion)
        END_IF
    ELSE
        SET_VALUE(LW(12), LW(11))
        CALL(btlevtcmd_GetUnitWork, LW(10), 0, LW(0))
        SUBTRACT(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, LW(10), 0, LW(0))
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
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(12), (kMainPart | kGuardTargetPart))
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(12), (kPartUntargetable | kHidden | kNoShadow))
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), (kMainPart | kGuardTargetPart))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), (kPartUntargetable | kHidden))
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            CALL_CHILD(subset_dmg_sambo_fall)
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, LW(10), 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, kHidden)
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_KAITEN_HAMMER_INVOLVED, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(13), PTR(&marioWeapon_KaitenHammerInvolved))
        ELSE
            SET_VALUE(LW(13), PTR(&marioWeapon_UltraHammerInvolved))
        END_IF
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
        CALL(btlevtcmd_CheckDamage, LW(10), LW(3), LW(4), LW(13), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(10)
        LABEL(50)
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), LW(5))
        CALL(btlevtcmd_GetPos, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_JumpPartsSetting, LW(10), 5, 0, FLOAT(5.0f), FLOAT(0.009765625f))
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 0, -1)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, kHidden)
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ULTRA_HAMMER, LW(5))
        IF_INT_NOT_EQUAL(LW(5), 0)
            CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), PTR(marioWeapon_UltraHammerFinish), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(damage_swallow_body)
    SET_VALUE(LW(14), POSE_TYPE_DAMAGE)
    SET_VALUE(LW(15), -1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
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
            CALL_CHILD(PTR(subsetevt_nodamage_motion))
        END_IF
        ELSE
        WAIT_FRAMES(3)
        SET_VALUE(LW(12), LW(11))
        CALL(btlevtcmd_GetUnitWork, LW(10), 0, LW(0))
        SUBTRACT(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, LW(10), 0, LW(0))
        CALL_CHILD(PTR(_body_param_set))
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
            CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(12), (kMainPart | kGuardTargetPart))
            CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(12), (kPartUntargetable | kHidden | kNoShadow))
            CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), (kMainPart | kGuardTargetPart))
            CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), (kPartUntargetable | kHidden))
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_FRAMES(1)
            CALL_CHILD(PTR(subset_dmg_sambo_fall))
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, LW(10), 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, kHidden)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 25)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 5, 0, 0, 0, -1)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, kHidden)
        WAIT_FRAMES(20)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(10))
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
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 1)
            CALL(evt_sub_random, 99, LW(0))
            IF_INT_LESS(LW(0), 20)
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
                CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(6), 40)
                SUBTRACT(LW(8), 10)
                CALL(btlevtcmd_CheckSpace, LW(3), LW(6), LW(7), LW(8), 40, 0, 0, 0)
                IF_INT_EQUAL(LW(3), -1)
                    IF_INT_GREATER(LW(6), 0)
                        CALL_CHILD(PTR(call_friend_event))
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
                CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_head_attack_4))
                CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_head_attack_4), LW(3), LW(4))
                CASE_EQUAL(3)
                CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_head_attack_3))
                CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_head_attack_3), LW(3), LW(4))
                CASE_EQUAL(2)
                CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_head_attack_2))
                CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_head_attack_2), LW(3), LW(4))
            END_SWITCH
            IF_INT_NOT_EQUAL(LW(3), -1)
                CALL_CHILD(PTR(attack_head_2_event))
                RETURN
            END_IF
        END_IF
        CALL_CHILD(PTR(attack_spike_event))
        RETURN
    END_IF
    CALL_CHILD(PTR(attack_head_1_event))
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(attack_head_2_event)
    SET_VALUE(LW(6), LW(11))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(5))
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_head_attack_4))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_head_attack_4), LW(3), LW(4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_head_attack_3))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_head_attack_3), LW(3), LW(4))
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_head_attack_2))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_head_attack_2), LW(3), LW(4))
    END_SWITCH
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_mummy_head_attack_4))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_mummy_head_attack_4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_mummy_head_attack_3))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_mummy_head_attack_3))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_mummy_head_attack_2))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_mummy_head_attack_2))
    END_SWITCH
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
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
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_WALK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_NEUTRAL)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(14), 40)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), -40)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, LW(14), 40)
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
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_IN, LW(13), LW(14), 10)
    LABEL(3)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_head_attack_4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_head_attack_3), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_head_attack_2), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
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
    LABEL(90)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(14), -90)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), 90)
    END_IF
    CALL(evt_sub_intpl_init, EASING_LINEAR, LW(14), 0, 10)
    LABEL(4)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_mummy_head_attack_4))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_head_attack_4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(3)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_mummy_head_attack_3))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_head_attack_3), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_mummy_head_attack_2))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_head_attack_2), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 90)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_NEUTRAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(14), -90)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), 90)
    END_IF
    CALL(evt_sub_intpl_init, EASING_LINEAR, LW(14), 0, 20)
    LABEL(5)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_RUN_HOME)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(attack_head_1_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_head_attack_1))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_head_attack_1), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_mummy_head_attack_1))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_mummy_head_attack_1))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_TransStageFloorPosition, LW(5), LW(6), LW(7))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(5), 50)
    SET_VALUE(LW(8), 35)
    CALL_CHILD(sambo_rabbit_move)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_head_attack_1), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    GOTO(98)
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
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_mummy_head_attack_1))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_head_attack_1), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.89941406f))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
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
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 4, POSE_TYPE_NEUTRAL)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 4, POSE_TYPE_RUN_HOME)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    SET_VALUE(LW(8), 35)
    CALL_CHILD(sambo_rabbit_move)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(attack_spike_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(10))
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_spike_attack_4))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_spike_attack_4), LW(3), LW(4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_spike_attack_3))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_spike_attack_3), LW(3), LW(4))
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_sambo_mummy_spike_attack_2))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_mummy_spike_attack_2), LW(3), LW(4))
    END_SWITCH
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_mummy_spike_attack_4))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_mummy_spike_attack_4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_mummy_spike_attack_3))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_mummy_spike_attack_3))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_sambo_mummy_spike_attack_2))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_mummy_spike_attack_2))
    END_SWITCH
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, 5, FLOAT(0.69921875f), FLOAT(0.69921875f), FLOAT(1.0f))
    ELSE
        CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, 5, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(10))
    SUBTRACT(LW(10), 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, LW(10))
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
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(7), (kMainPart | kGuardTargetPart))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(7), (kPartUntargetable | kHidden | kNoShadow))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(6), (kMainPart | kGuardTargetPart))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(6), (kPartUntargetable | kHidden))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(5))
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
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 5, kHidden)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 5, FLOAT(0.5996094f))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("SNB_B_1"))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 5, LW(0), LW(1), LW(2), 30, -1)
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_ATTACK)
    WAIT_FRAMES(24)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_ATTACK5"), EVT_NULLPTR, 0, LW(14))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_ATTACK_2)
    WAIT_FRAMES(2)
    WAIT_FRAMES(5)
    SWITCH(LW(10))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_spike_attack_4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_spike_attack_3), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(1)
        CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_sambo_mummy_spike_attack_2), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_SWITCH
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
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 5, kHidden)
    GOTO(99)
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
        SET_VALUE(LW(12), PTR(&weapon_sambo_mummy_spike_attack_4))
        CASE_EQUAL(2)
        SET_VALUE(LW(12), PTR(&weapon_sambo_mummy_spike_attack_3))
        CASE_EQUAL(1)
        SET_VALUE(LW(12), PTR(&weapon_sambo_mummy_spike_attack_2))
    END_SWITCH
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_NEUTRAL)
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
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 5, kHidden)
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
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 5, kHidden)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(call_friend_event)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("SNB_S_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_SEND_GROUP1_3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, -20, 40)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_SEND_GROUP1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -20, 30, 40)
    LABEL(1)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_SEND_GROUP1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 30, -40, 40)
    LABEL(2)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_SAMBO_SEND_GROUP2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -40, 90, 20)
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
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_sambo), 0)
    SET_VALUE(LW(0), LW(6))
    SET_VALUE(LW(1), LW(7))
    SET_VALUE(LW(2), LW(8))
    SUBTRACT(LW(1), 80)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
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
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(4))
    ADD(LW(4), 1)
    CALL(btlevtcmd_SetUnitWork, LW(3), 1, LW(4))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 90, -30, 14)
    LABEL(4)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    WAIT_FRAMES(40)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, -30, 0, 20)
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

static BattleUnitSetup entry_sambo = {
    &unit_sambo_mummy, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(wait_event)
    SET_VALUE(LW(10), -2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
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

static EVT_BEGIN(_body_param_set)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
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

