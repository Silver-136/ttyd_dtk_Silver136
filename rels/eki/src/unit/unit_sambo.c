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


static BattleUnitKind unit_sambo;
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

static BattleUnitKind unit_sambo = {
    39, // mUnitType
    "btl_un_sambo", // name
    4, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    11, // mLevel
    0, // mBonusExp
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
    "SFX_BTL_DAMAGE_BIRIBIRI1", // explosionDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    5, // numParts
    parts, // mParts
    init_event, // initEventCode
    (BattleDataEntry*)data_table, // dataTable
};

static BattleUnitDefense defence = {0};

static BattleUnitDefenseAttr defence_attr = {0};

static StatusVulnerability regist = {95, 80, 80, 100, 90, 100, 100, 60, 100, 90, 100, 90, 100, 95, 90, 100, 90, 100, 90, 100, 100, 100};

static BattleWeapon weapon_sambo_head_attack_4 = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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

static BattleWeapon weapon_sambo_head_attack_3 = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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

static BattleWeapon weapon_sambo_head_attack_2 = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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

static BattleWeapon weapon_sambo_head_attack_1 = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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

static BattleWeapon weapon_sambo_spike_attack_4 = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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

static BattleWeapon weapon_sambo_spike_attack_3 = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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

static BattleWeapon weapon_sambo_spike_attack_2 = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    {1, "SNB_N_1"},
    {2, "SNB_Y_1"},
    {9, "SNB_Y_1"},
    {5, "SNB_K_1"},
    {4, "SNB_X_1"},
    {3, "SNB_X_1"},
    {28, "SNB_S_1"},
    {29, "SNB_Q_1"},
    {30, "SNB_Q_1"},
    {31, "SNB_S_1"},
    {39, "SNB_D_1"},
    {47, "SNB_W_1"},
    {42, "SNB_W_1"},
    {40, "SNB_W_1"},
    {56, "SNB_X_1"},
    {57, "SNB_X_1"},
    {65, "SNB_T_1"},
    {69, "SNB_S_1"},
};

static PoseTableEntry pose_table_body3[] = {
    {1, "SNB_N_2"},
    {2, "SNB_Y_2"},
    {9, "SNB_Y_2"},
    {5, "SNB_K_2"},
    {4, "SNB_X_2"},
    {3, "SNB_X_2"},
    {28, "SNB_S_2"},
    {29, "SNB_Q_2"},
    {30, "SNB_Q_2"},
    {31, "SNB_S_2"},
    {39, "SNB_D_2"},
    {47, "SNB_W_2"},
    {50, "SNB_A_1A"},
    {51, "SNB_A_1B"},
    {42, "SNB_W_2"},
    {40, "SNB_W_2"},
    {56, "SNB_X_2"},
    {57, "SNB_X_2"},
    {65, "SNB_T_2"},
    {69, "SNB_S_2"},
};

static PoseTableEntry pose_table_body2[] = {
    {1, "SNB_N_3"},
    {2, "SNB_Y_3"},
    {9, "SNB_Y_3"},
    {5, "SNB_K_3"},
    {4, "SNB_X_3"},
    {3, "SNB_X_3"},
    {28, "SNB_S_3"},
    {29, "SNB_Q_3"},
    {30, "SNB_Q_3"},
    {31, "SNB_S_3"},
    {39, "SNB_D_3"},
    {47, "SNB_W_3"},
    {50, "SNB_A_2A"},
    {51, "SNB_A_2B"},
    {42, "SNB_W_3"},
    {40, "SNB_W_3"},
    {56, "SNB_X_3"},
    {57, "SNB_X_3"},
    {65, "SNB_T_3"},
    {69, "SNB_S_3"},
};

static PoseTableEntry pose_table_body1[] = {
    {1, "SNB_N_4"},
    {2, "SNB_Y_4"},
    {9, "SNB_Y_4"},
    {5, "SNB_K_4"},
    {4, "SNB_X_4"},
    {3, "SNB_X_4"},
    {28, "SNB_S_4"},
    {29, "SNB_Q_4"},
    {30, "SNB_Q_4"},
    {31, "SNB_S_4"},
    {39, "SNB_D_4"},
    {47, "SNB_W_4"},
    {50, "SNB_A_3A"},
    {51, "SNB_A_3B"},
    {42, "SNB_W_4"},
    {40, "SNB_W_4"},
    {56, "SNB_X_4"},
    {57, "SNB_X_4"},
    {65, "SNB_T_4"},
    {69, "SNB_S_4"},
};

static PoseTableEntry pose_table_parts[] = {
    {28, "SNB_B_1"},
    {39, "SNB_B_1"},
    {69, "SNB_B_1"},
};

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

static BattleUnitKindPart parts[] = {
    {
        1, // partNum
        "btl_un_sambo", // partName
        "c_sanbo", // modelName
        {0}, // positionOffset
        {0.0f, 84.0f, 0.0f}, // hitBaseOffset
        {0.0f, 90.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        90, // reserved_32
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        9, // attributes
        5, // counterAttributes
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
        70, // reserved_32
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        0x3010000, // attributes
        5, // counterAttributes
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
        50, // reserved_32
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        0x3010000, // attributes
        5, // counterAttributes
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
        30, // reserved_32
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        0x3010000, // attributes
        5, // counterAttributes
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
        30, // reserved_32
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        0x13010000, // attributes
        0, // counterAttributes
        pose_table_parts, // poseTable
    },
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 4)
    CALL_CHILD(PTR(_body_param_set))
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 5, 69)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_ENM_SAMBO_MOVE1"), STRING("SFX_ENM_SAMBO_MOVE1"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_ENM_SAMBO_MOVE1"), STRING("SFX_ENM_SAMBO_MOVE1"), 0, 6, 6)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

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

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
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
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
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
            CALL(btlevtcmd_SetSwallowParam, -2, 50)
            CALL(btlevtcmd_SetSwallowAttribute, -2, 2)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 20)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 10, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 16, 0)
            CASE_EQUAL(2)
            SET_VALUE(LW(11), 3)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 40)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 20, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 26, 0)
            CASE_EQUAL(3)
            SET_VALUE(LW(11), 2)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 60)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 30, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 36, 0)
            CASE_DEFAULT
            SET_VALUE(LW(11), 1)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 80)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 40, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 52, 0)
        END_SWITCH
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(12), 9)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(12), 50397184)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), 9)
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), 16842752)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            CALL_CHILD(PTR(subset_dmg_sambo_fall))
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, LW(10), 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, 16777216)
        CALL(btlevtcmd_SetPartsFallAccel, LW(10), 5, FLOAT(0.39941406f))
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 25)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 16, -1)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), 16)
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 12, -1)
        LOOP(40)
            CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, 16777216)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, 16777216)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, 16777216)
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
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
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
            CALL(btlevtcmd_SetSwallowParam, -2, 50)
            CALL(btlevtcmd_SetSwallowAttribute, -2, 2)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 20)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 10, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 16, 0)
            CASE_EQUAL(2)
            SET_VALUE(LW(11), 3)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 40)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 20, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 26, 0)
            CASE_EQUAL(3)
            SET_VALUE(LW(11), 2)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 60)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 30, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 36, 0)
            CASE_DEFAULT
            SET_VALUE(LW(11), 1)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 80)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 40, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 52, 0)
        END_SWITCH
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(12), 9)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(12), 50397184)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), 9)
        CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), 16842752)
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(4)
            CALL_CHILD(subset_dmg_sambo_fall)
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, LW(10), 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, 16777216)
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
        CALL(btlevtcmd_GetUnitId, -2, LW(10))
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
        CALL(btlevtcmd_CheckDamage, LW(10), LW(3), LW(4), LW(13), 256, LW(5))
        GOTO(10)
        LABEL(50)
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(0), LW(5))
        CALL(btlevtcmd_GetPos, LW(10), EVT_NULLPTR, LW(1), EVT_NULLPTR)
        CALL(btlevtcmd_JumpPartsSetting, LW(10), 5, 0, FLOAT(5.0f), FLOAT(0.009765625f))
        CALL(btlevtcmd_JumpPartsPosition, LW(10), 5, LW(0), LW(1), LW(2), 0, -1)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, 16777216)
        CALL(btlevtcmd_CheckDamagePattern, LW(10), ATTACKPATTERN_ULTRA_HAMMER, LW(5))
        IF_INT_NOT_EQUAL(LW(5), 0)
            CALL(btlevtcmd_CheckDamage, -2, LW(10), LW(11), PTR(marioWeapon_UltraHammerFinish), 131328, LW(5))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(damage_swallow_body)
    SET_VALUE(LW(14), 39)
    SET_VALUE(LW(15), -1)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
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
            CALL(btlevtcmd_SetSwallowParam, -2, 50)
            CALL(btlevtcmd_SetSwallowAttribute, -2, 2)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 20)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 10, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 16, 0)
            CASE_EQUAL(2)
            SET_VALUE(LW(11), 3)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 40)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 20, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 26, 0)
            CASE_EQUAL(3)
            SET_VALUE(LW(11), 2)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 60)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 30, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 36, 0)
            CASE_DEFAULT
            SET_VALUE(LW(11), 1)
            CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 80)
            CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 40, 0)
            CALL(btlevtcmd_SetKissHitOffset, -2, 10, 52, 0)
        END_SWITCH
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(3)
            CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(12), 9)
            CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(12), 50397184)
            CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), 9)
            CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), 16842752)
            CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
            WAIT_FRAMES(1)
            CALL_CHILD(PTR(subset_dmg_sambo_fall))
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, LW(10), 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, LW(10), 5, 16777216)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 25)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePartsPosition, -2, 5, LW(0), LW(1), LW(2), 5, 0, 0, 0, -1)
        CALL(btlevtcmd_OnPartsAttribute, LW(10), 5, 16777216)
        WAIT_FRAMES(20)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(10))
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
        CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 1)
            CALL(evt_sub_random, 99, LW(0))
            IF_INT_LESS(LW(0), 30)
                CALL(btlevtcmd_GetPos, -2, LW(6), LW(7), LW(8))
                CALL(btlevtcmd_FaceDirectionAdd, -2, LW(6), 40)
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
                CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
                CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_head_attack_4))
                CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_4), LW(3), LW(4))
                CASE_EQUAL(3)
                CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
                CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_head_attack_3))
                CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_3), LW(3), LW(4))
                CASE_EQUAL(2)
                CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
                CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_head_attack_2))
                CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_2), LW(3), LW(4))
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
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_head_2_event)
    SET_VALUE(LW(6), LW(11))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_TINY, LW(5))
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_head_attack_4))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_4), LW(3), LW(4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_head_attack_3))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_3), LW(3), LW(4))
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_head_attack_2))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_2), LW(3), LW(4))
    END_SWITCH
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_sambo_head_attack_4))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_head_attack_4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_sambo_head_attack_3))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_head_attack_3))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_sambo_head_attack_2))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_head_attack_2))
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
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
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), 40)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), 43)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(14), 40)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), -40)
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    CALL(evt_sub_intpl_init, 11, 0, LW(14), 40)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(13), 40)
    SET_VALUE(LW(14), -90)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(13), -40)
        SET_VALUE(LW(14), 90)
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 1, LW(13), LW(14), 10)
    LABEL(3)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(3)
    END_IF
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_head_attack_4), 256, LW(5))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_head_attack_3), 256, LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_head_attack_2), 256, LW(5))
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
    LABEL(90)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(14), -90)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), 90)
    END_IF
    CALL(evt_sub_intpl_init, 0, LW(14), 0, 10)
    LABEL(4)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_head_attack_4))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_head_attack_4), 256, LW(5))
        CASE_EQUAL(3)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_head_attack_3))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_head_attack_3), 256, LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_head_attack_2))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_head_attack_2), 256, LW(5))
    END_SWITCH
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 90)
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), 43)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    SET_VALUE(LW(14), -90)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), 90)
    END_IF
    CALL(evt_sub_intpl_init, 0, LW(14), 0, 20)
    LABEL(5)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), 41)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_head_1_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_head_attack_1))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_head_attack_1), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_head_attack_1))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_sambo_head_attack_1))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(2.0f))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(5), LW(6), LW(7))
    CALL(btlevtcmd_TransStageFloorPosition, LW(5), LW(6), LW(7))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(5), 50)
    SET_VALUE(LW(8), 35)
    CALL_CHILD(sambo_rabbit_move)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_head_attack_1), 256, LW(5))
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
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 5)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(2)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
        SET_VALUE(LW(14), -10)
        IF_INT_EQUAL(LW(15), -1)
            SET_VALUE(LW(14), 10)
        END_IF
        SET_VALUE(LW(0), 0)
        LOOP(15)
            ADD(LW(0), LW(14))
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 44, -1)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 10, 0)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(2)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
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
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 30)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(2)
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
        SET_VALUE(LW(14), -10)
        IF_INT_EQUAL(LW(15), -1)
            SET_VALUE(LW(14), 10)
        END_IF
        SET_VALUE(LW(0), 0)
        LOOP(15)
            ADD(LW(0), LW(14))
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 36, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_ATTACK6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_sambo_head_attack_1))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_head_attack_1), 256, LW(5))
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.89941406f))
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 40)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 10)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 4, 43)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 4, 41)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5f))
    CALL(btlevtcmd_GetHomePos, -2, LW(5), LW(6), LW(7))
    SET_VALUE(LW(8), 35)
    CALL_CHILD(sambo_rabbit_move)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_spike_event)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(10))
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_spike_attack_4))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_spike_attack_4), LW(3), LW(4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_spike_attack_3))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_spike_attack_3), LW(3), LW(4))
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_sambo_spike_attack_2))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_sambo_spike_attack_2), LW(3), LW(4))
    END_SWITCH
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    SWITCH(LW(10))
        CASE_EQUAL(4)
        CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_sambo_spike_attack_4))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_spike_attack_4))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_sambo_spike_attack_3))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_spike_attack_3))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_sambo_spike_attack_2))
        CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_sambo_spike_attack_2))
    END_SWITCH
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_TINY, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetPartsScale, -2, 5, FLOAT(0.69921875f), FLOAT(0.69921875f), FLOAT(1.0f))
    ELSE
        CALL(btlevtcmd_SetPartsScale, -2, 5, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(10))
    SUBTRACT(LW(10), 1)
    CALL(btlevtcmd_SetUnitWork, -2, 0, LW(10))
    CALL_CHILD(_body_param_set)
    SWITCH(LW(10))
        CASE_EQUAL(3)
        SET_VALUE(LW(7), 1)
        SET_VALUE(LW(6), 2)
        CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 60)
        CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 30, 0)
        CALL(btlevtcmd_SetKissHitOffset, -2, 10, 36, 0)
        CASE_EQUAL(2)
        SET_VALUE(LW(7), 2)
        SET_VALUE(LW(6), 3)
        CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 40)
        CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 20, 0)
        CALL(btlevtcmd_SetKissHitOffset, -2, 10, 26, 0)
        CASE_EQUAL(1)
        SET_VALUE(LW(7), 3)
        SET_VALUE(LW(6), 4)
        CALL(btlevtcmd_SetSwallowParam, -2, 50)
        CALL(btlevtcmd_SetSwallowAttribute, -2, 2)
        CALL(btlevtcmd_SetStatusIconOffset, -2, 14, 20)
        CALL(btlevtcmd_SetCutBaseOffset, -2, 0, 10, 0)
        CALL(btlevtcmd_SetKissHitOffset, -2, 10, 16, 0)
    END_SWITCH
    CALL(btlevtcmd_OffPartsAttribute, -2, LW(7), 9)
    CALL(btlevtcmd_OnPartsAttribute, -2, LW(7), 50397184)
    CALL(btlevtcmd_OnPartsAttribute, -2, LW(6), 9)
    CALL(btlevtcmd_OffPartsAttribute, -2, LW(6), 16842752)
    CALL(btlevtcmd_CheckStatus, -2, STATUS_TINY, LW(5))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
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
        CALL(btlevtcmd_SetPartsPos, -2, 5, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, -2, 5, kHidden)
        CALL(btlevtcmd_SetPartsFallAccel, -2, 5, FLOAT(0.5996094f))
        CALL(btlevtcmd_AnimeChangePose, -2, 5, STRING("SNB_B_1"))
        CALL(btlevtcmd_JumpPartsPosition, -2, 5, LW(0), LW(1), LW(2), 30, -1)
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), 50)
    WAIT_FRAMES(24)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_ATTACK5"), EVT_NULLPTR, 0, LW(14))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), 51)
    WAIT_FRAMES(2)
    WAIT_FRAMES(5)
    SWITCH(LW(10))
        CASE_EQUAL(3)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_spike_attack_4), 256, LW(5))
        CASE_EQUAL(2)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_spike_attack_3), 256, LW(5))
        CASE_EQUAL(1)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_sambo_spike_attack_2), 256, LW(5))
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
    LABEL(90)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(9))
    MULTIPLY(LW(0), LW(9))
    SUBTRACT(LW(1), 5)
    CALL(btlevtcmd_SetPartsMoveSpeed, -2, 5, FLOAT(10.0f))
    CALL(btlevtcmd_SetPartsFallAccel, -2, 5, FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPartsPosition, -2, 5, LW(0), LW(1), LW(2), 0, -1)
    CALL(evt_snd_sfxoff, LW(14))
    CALL(btlevtcmd_OnPartsAttribute, -2, 5, kHidden)
    GOTO(99)
    LABEL(91)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsMoveSpeed, -2, 5, FLOAT(10.0f))
    CALL(btlevtcmd_SetPartsFallAccel, -2, 5, FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPartsPosition, -2, 5, LW(0), LW(1), LW(2), 0, -1)
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
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    CALL(btlevtcmd_AnimeChangePoseType, -2, LW(6), 43)
    IF_INT_EQUAL(LW(5), 21)
        CALL(evt_snd_sfxoff, LW(14))
        CALL(btlevtcmd_GetBodyId, -2, LW(11))
        CALL(btlevtcmd_GetHitPos, -2, LW(11), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsMoveSpeed, -2, 5, FLOAT(4.5f))
        CALL(btlevtcmd_SetPartsFallAccel, -2, 5, FLOAT(0.39941406f))
        CALL(btlevtcmd_JumpPartsPosition, -2, 5, LW(0), LW(1), LW(2), 40, -1)
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_GetPartsPos, -2, 5, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 55)
            CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetPartsMoveSpeed, -2, 5, FLOAT(3.0f))
            CALL(btlevtcmd_SetPartsFallAccel, -2, 5, FLOAT(0.099609375f))
            CALL(btlevtcmd_DivePartsPosition, -2, 5, LW(0), LW(1), LW(2), 0, 25, 0, 0, -1)
            CALL(btlevtcmd_SetPartsMoveSpeed, -2, 5, FLOAT(2.0f))
            CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 24)
            CALL(btlevtcmd_DivePartsPosition, -2, 5, LW(0), LW(1), LW(2), 0, 21, 0, 0, -1)
            CALL(btlevtcmd_SetPartsMoveSpeed, -2, 5, FLOAT(1.5f))
            CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
            CALL(btlevtcmd_DivePartsPosition, -2, 5, LW(0), LW(1), LW(2), 0, 20, 0, 0, -1)
            CALL(btlevtcmd_OnPartsAttribute, -2, 5, kHidden)
        END_INLINE_EVENT
        CALL(btlevtcmd_CheckDamage, -2, -2, LW(11), LW(12), 256, LW(5))
        RETURN
    END_IF
    CALL(evt_snd_sfxoff, LW(14))
    CALL(btlevtcmd_GetPartsPos, -2, 5, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 55)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsMoveSpeed, -2, 5, FLOAT(3.0f))
    CALL(btlevtcmd_SetPartsFallAccel, -2, 5, FLOAT(0.099609375f))
    CALL(btlevtcmd_DivePartsPosition, -2, 5, LW(0), LW(1), LW(2), 0, 25, 0, 0, -1)
    CALL(btlevtcmd_SetPartsMoveSpeed, -2, 5, FLOAT(2.0f))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 24)
    CALL(btlevtcmd_DivePartsPosition, -2, 5, LW(0), LW(1), LW(2), 0, 21, 0, 0, -1)
    CALL(btlevtcmd_SetPartsMoveSpeed, -2, 5, FLOAT(1.5f))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 20)
    CALL(btlevtcmd_DivePartsPosition, -2, 5, LW(0), LW(1), LW(2), 0, 20, 0, 0, -1)
    CALL(btlevtcmd_OnPartsAttribute, -2, 5, kHidden)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(call_friend_event)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("SNB_S_1"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_SEND_GROUP1_3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 11, 0, -20, 40)
    LABEL(0)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_SEND_GROUP1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 11, -20, 30, 40)
    LABEL(1)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(1)
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_SEND_GROUP1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 11, 30, -40, 40)
    LABEL(2)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(2)
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_SEND_GROUP2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_sub_intpl_init, 11, -40, 90, 20)
    LABEL(3)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
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
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_SEND_GROUP3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, LW(3), 1, STRING("SNB_D_1"))
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, LW(3), FLOAT(0.69921875f))
        CALL(btlevtcmd_JumpPosition, LW(3), LW(6), LW(7), LW(8), 20, -1)
        CALL(btlevtcmd_SetHomePos, LW(3), LW(6), LW(7), LW(8))
        WAIT_FRAMES(40)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(4))
    ADD(LW(4), 1)
    CALL(btlevtcmd_SetUnitWork, LW(3), 1, LW(4))
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_sub_intpl_init, 4, 90, -30, 14)
    LABEL(4)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(4)
    END_IF
    WAIT_FRAMES(40)
    CALL(evt_sub_intpl_init, 11, -30, 0, 20)
    LABEL(5)
    CALL(evt_sub_intpl_get_value)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(0))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(5)
    END_IF
    WAIT_FRAMES(40)
    RETURN
EVT_END()

static BattleUnitSetup entry_sambo = {
    &unit_sambo, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(wait_event)
    SET_VALUE(LW(10), -2)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
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
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
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
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(6), LW(2), 0, -1)
        GOTO(0)
    ELSE
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_SAMBO_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpPosition, -2, LW(5), LW(6), LW(7), 0, -1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(_body_param_set)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetHeight, -2, 20)
        CALL(btlevtcmd_SetCutHeight, -2, 20)
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetHeight, -2, 40)
        CALL(btlevtcmd_SetCutHeight, -2, 40)
        CASE_EQUAL(3)
        CALL(btlevtcmd_SetHeight, -2, 60)
        CALL(btlevtcmd_SetCutHeight, -2, 60)
        CASE_DEFAULT
        CALL(btlevtcmd_SetHeight, -2, 80)
        CALL(btlevtcmd_SetCutHeight, -2, 80)
    END_SWITCH
    RETURN
EVT_END()

