#include "system.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_env.h"
#include "battle/battle_status_effect.h"

#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"

static s32 x_flag_data[2];


USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
static USER_FUNC(x_flag_wait);
static USER_FUNC(x_flag_get);
static USER_FUNC(x_flag_add);
static USER_FUNC(x_flag_set);
static USER_FUNC(wariai_enzan_0f);
static USER_FUNC(wariai_enzan_0);
USER_FUNC(wariai_enzan_1f_intpl_w);
static USER_FUNC(wariai_enzan_1f_intpl);
static USER_FUNC(wariai_enzan_1f);
static USER_FUNC(wariai_enzan_1);

BattleUnitKind unit_boss_macho_gance;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_furafura[18];
static BattleDataEntry data_table[15];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_gance_macho_speed;
BattleWeapon weapon_gance_build_up;
BattleWeapon weapon_gance_body;
BattleWeapon weapon_gance_footwork;
BattleWeapon weapon_gance_attack;
BattleWeapon weapon_gance_posing;
BattleWeapon weapon_gance_rariat_attack;
BattleWeapon weapon_gance_body_attack;
EvtWord macho_gance_zoom[65];
EvtWord macho_gance_blur_zoom[46];
static EvtWord mario_party_zoom[34];
static EvtWord init_event[79];
static EvtWord entry_event[62];
static EvtWord damage_event[97];
EvtWord base_size_change[99];
static EvtWord phase_event[482];
static EvtWord dead_event[64];
EvtWord macho_gance_damage_event[238];
static EvtWord attack_event[151];
EvtWord gance_macho_speed_attack_event[119];
EvtWord gance_build_up_attack_event[119];
EvtWord gance_body_attack_event[126];
EvtWord gance_footwork_attack_event[119];
EvtWord gance_attack_attack_event[541];
EvtWord gance_posing_attack_event[129];
EvtWord gance_rariat_attack_attack_event[446];
EvtWord gance_body_attack_attack_event[515];
static EvtWord wait_event[6];

BattleUnitKind unit_boss_macho_gance = {
    65,
    "btl_un_macho_gance",
    60,
    0,
    1,
    1,
    71,
    0,
    0,
    100,
    0,
    50,
    6,
    116,
    124,
    30,
    100,
    0.0f,
    62.0f,
    0.0f,
    0,
    -3,
    40.0f,
    80.0f,
    0.0f,
    58.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    58.0f,
    0.0f,
    0.0f,
    42.0f,
    80.0f,
    0.0f,
    0.0f,
    62.0f,
    0.0f,
    116.0f,
    124.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_GANSU_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {50, 40, 50, 100, 50, 100, 100, 30, 0, 75, 100, 75, 100, 90, 60, 0, 0, 100, 60, 100, 100, 0};

static PoseTableEntry pose_table[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "K_1"},
    {28, "S_1"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "D_1"},
    {39, "D_1"},
    {50, "E_1A"},
    {42, "R_1"},
    {40, "W_1"},
    {56, "X_1"},
    {57, "X_1"},
    {65, "T_1"},
    {69, "S_1"},
};

static PoseTableEntry pose_table_furafura[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "K_1"},
    {28, "X_1"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "D_1"},
    {39, "D_1"},
    {50, "E_1A"},
    {42, "R_1"},
    {40, "W_1"},
    {56, "X_1"},
    {57, "X_1"},
    {65, "T_1"},
    {69, "X_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {36, macho_gance_damage_event},
    {24, macho_gance_damage_event},
    {25, macho_gance_damage_event},
    {26, macho_gance_damage_event},
    {27, macho_gance_damage_event},
    {28, macho_gance_damage_event},
    {29, macho_gance_damage_event},
    {30, macho_gance_damage_event},
    {31, macho_gance_damage_event},
    {32, macho_gance_damage_event},
    {33, macho_gance_damage_event},
    {34, macho_gance_damage_event},
    {35, macho_gance_damage_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_macho_gance", "c_m_gansu", 0.0f, 0.0f, 0.0f, 26.0f, 77.0f, 0.0f, 25.0f, 81.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

BattleWeapon weapon_gance_macho_speed = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
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
    100, // fastChance
    4, // fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_gance_build_up = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    0, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, // burnTime..sizeChangeStrength
    100, // atkChangeChance
    1, // atkChangeTime
    3, // atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_gance_body = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
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
    0, // atkChangeStrength
    100, // defChangeChance
    2, // defChangeTime
    3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_gance_footwork = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    0, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, // poisonStrength..electricTime
    100, // dodgyChance
    2, // dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_gance_attack = {
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
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
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

BattleWeapon weapon_gance_posing = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_FREEZE_CRUSH, // special
    0, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..ohkoChance
    4, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_gance_rariat_attack = {
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
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

BattleWeapon weapon_gance_body_attack = {
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
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_PRESS, // damagePattern
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
    {0, 25, 25, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

EVT_BEGIN(macho_gance_zoom)
    CALL(btlevtcmd_GetStatusMg, -2, LW(2))
    MULTIPLY_FLOAT(LW(2), FLOAT(10.0f))
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(1))
    MULTIPLY_FLOAT(LW(2), LW(1))
    CALL(evt_btl_camera_set_mode, 0, 3)
    IF_INT_NOT_EQUAL(LW(0), 2)
        SET_VALUE(LW(1), 30)
        ELSE
        SET_VALUE(LW(1), 1)
    END_IF
    IF_INT_LESS_OR_EQUAL(LW(2), FLOAT(10.0f))
        CALL(evt_btl_camera_set_moveto, 0, 83, 124, 512, 83, 89, 0, LW(1), 11)
        ELSE
        CALL(evt_btl_camera_set_moveto, 0, 71, 159, 609, 71, 118, 0, LW(1), 11)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(600)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(macho_gance_blur_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
    IF_INT_NOT_EQUAL(LW(0), 2)
        SET_VALUE(LW(1), 48)
        SET_VALUE(LW(2), 800)
        ELSE
        SET_VALUE(LW(1), 1)
        SET_VALUE(LW(2), 16)
    END_IF
    CALL(evt_btl_camera_set_moveto, 0, 83, 137, 701, 83, 89, 0, LW(1), 11)
    BEGIN_INLINE_EVENT
        CALL(evt_env_blur_on, 0, LW(2))
    END_INLINE_EVENT
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(900)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(mario_party_zoom)
    CALL(evt_btl_camera_set_mode, 0, 3)
    IF_INT_NOT_EQUAL(LW(0), 2)
        SET_VALUE(LW(1), 30)
        ELSE
        SET_VALUE(LW(1), 1)
    END_IF
    CALL(evt_btl_camera_set_moveto, 0, -111, 67, 275, -111, 49, 0, LW(1), 11)
    IF_INT_EQUAL(LW(0), 1)
        WAIT_MS(600)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_BOSS_GANSU_MOVE1L"), STRING("SFX_BOSS_GANSU_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_BOSS_GANSU_MOVE1L"), STRING("SFX_BOSS_GANSU_MOVE1R"), 0, 20, 20)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(entry_event))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_SetUnitWorkFloat, -2, 3, FLOAT(1.0f))
    CALL(btlevtcmd_SetHitOffset, -2, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    SET_VALUE(LW(0), 2)
    CALL_CHILD(macho_gance_zoom)
    WAIT_MS(1000)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_msg_print, 2, STRING("stg3_tou_469"), 0, -2)
    WAIT_MS(300)
    SET_VALUE(LW(0), 2)
    CALL_CHILD(mario_party_zoom)
    CALL(evt_msg_print_battle_party, STRING("stg3_tou_470"))
    WAIT_MS(300)
    CALL(btlevtcmd_GetBodyId, -4, LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, -4, LW(0))
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SWITCH(LW(0))
            CASE_EQUAL(1)
            WAIT_MS(250)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            WAIT_MS(300)
            CASE_EQUAL(2)
            WAIT_MS(250)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            WAIT_MS(300)
            CASE_EQUAL(3)
            WAIT_MS(250)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            WAIT_MS(300)
            CASE_EQUAL(4)
            WAIT_MS(250)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            WAIT_MS(300)
            CASE_EQUAL(6)
            WAIT_MS(250)
            CALL(evt_btl_camera_set_mode, 0, 0)
            CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
            WAIT_MS(300)
        END_SWITCH
        CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(base_size_change)
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(1))
    CALL(btlevtcmd_SetUnitWorkFloat, -2, 3, LW(0))
    DIVIDE_FLOAT(LW(0), LW(1))
    SET_FLOAT_VALUE(LW(5), LW(0))
    MULTIPLY_FLOAT(LW(5), FLOAT(26.0f))
    SUBTRACT_FLOAT(LW(5), FLOAT(26.0f))
    SET_FLOAT_VALUE(LW(6), LW(0))
    MULTIPLY_FLOAT(LW(6), FLOAT(77.0f))
    SUBTRACT_FLOAT(LW(6), FLOAT(77.0f))
    SET_FLOAT_VALUE(LW(7), LW(0))
    MULTIPLY_FLOAT(LW(7), FLOAT(0.0f))
    SUBTRACT_FLOAT(LW(7), FLOAT(0.0f))
    CALL(btlevtcmd_SetHitOffset, -2, 1, LW(5), LW(6), LW(7))
    SET_FLOAT_VALUE(LW(5), LW(0))
    MULTIPLY_FLOAT(LW(5), FLOAT(25.0f))
    SUBTRACT_FLOAT(LW(5), FLOAT(25.0f))
    SET_FLOAT_VALUE(LW(6), LW(0))
    MULTIPLY_FLOAT(LW(6), FLOAT(81.0f))
    SUBTRACT_FLOAT(LW(6), FLOAT(81.0f))
    SET_FLOAT_VALUE(LW(7), LW(0))
    MULTIPLY_FLOAT(LW(7), FLOAT(0.0f))
    SUBTRACT_FLOAT(LW(7), FLOAT(0.0f))
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, LW(5), LW(6), LW(7))
    CALL(btlevtcmd_GetHpGaugeOffset, -2, LW(5), LW(6))
    MULTIPLY_FLOAT(LW(5), LW(0))
    MULTIPLY_FLOAT(LW(6), LW(0))
    CALL(btlevtcmd_SetHpGaugeOffset, -2, LW(5), LW(6))
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(0)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetMaxHp, -2, LW(0))
        MULTIPLY(LW(0), 75)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, -2, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
            CALL(btlevtcmd_StopWaitEvent, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            SET_VALUE(LW(0), 0)
            CALL_CHILD(macho_gance_zoom)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
            WAIT_MS(1500)
            CALL(evt_snd_sfxoff, LW(15))
            SET_VALUE(LW(0), -900)
            IF_INT_GREATER(LW(0), 0)
                WAIT_MS(LW(0))
            END_IF
            CALL(evt_msg_print, 2, STRING("stg3_tou_471_17"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
            WAIT_MS(1000)
        END_IF
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetMaxHp, -2, LW(0))
        MULTIPLY(LW(0), 50)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, -2, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, -2, 1, 2)
            CALL(btlevtcmd_StopWaitEvent, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            SET_VALUE(LW(0), 1)
            CALL_CHILD(macho_gance_zoom)
            CALL(evt_msg_print, 2, STRING("stg3_tou_471_18"), 0, -2)
            WAIT_MS(300)
            SET_VALUE(LW(0), 0)
            CALL_CHILD(macho_gance_blur_zoom)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_BIG3"), EVT_NULLPTR, 0, LW(15))
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_1"))
            WAIT_MS(800)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_BIG1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetRotate, -2, LW(0), LW(1), LW(2))
            SET_VALUE(LW(5), 36)
            LOOP(LW(5))
                CALL(wariai_enzan_1f, LW(5), 36, FLOAT(0.0f), FLOAT(180.0f), LW(6))
                CALL(wariai_enzan_1f, LW(5), 36, FLOAT(1.0f), FLOAT(1.1494141f), LW(7))
                CALL(btlevtcmd_SetBaseScale, -2, LW(7), LW(7), 1)
                ADD(LW(6), LW(1))
                CALL(btlevtcmd_SetRotate, -2, 0, LW(6), 0)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_ChangeFaceDirection, -2, 1)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            WAIT_MS(400)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_BIG2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetRotate, -2, LW(0), LW(1), LW(2))
            SET_VALUE(LW(5), 36)
            LOOP(LW(5))
                CALL(wariai_enzan_1f, LW(5), 36, FLOAT(0.0f), FLOAT(-180.0f), LW(6))
                CALL(wariai_enzan_1f, LW(5), 36, FLOAT(1.1494141f), FLOAT(1.2998047f), LW(7))
                CALL(btlevtcmd_SetBaseScale, -2, LW(7), LW(7), 1)
                ADD(LW(6), LW(1))
                CALL(btlevtcmd_SetRotate, -2, 0, LW(6), 0)
                WAIT_FRAMES(1)
            END_LOOP
            CALL(btlevtcmd_ResetFaceDirection, -2)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
            SET_FLOAT_VALUE(LW(0), FLOAT(1.2998047f))
            CALL_CHILD(base_size_change)
            WAIT_MS(400)
            CALL(evt_snd_sfxoff, LW(15))
            CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.5996094f))
            SET_VALUE(LW(8), 2)
            LOOP(LW(8))
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1A"))
                    CALL(btlevtcmd_GetPosFloat, -2, LW(5), LW(6), LW(7))
                    LOOP(0)
                        CALL(btlevtcmd_GetPosFloat, -2, LW(9), LW(10), LW(11))
                        SUBTRACT_FLOAT(LW(6), LW(10))
                        IF_INT_GREATER(LW(6), FLOAT(0.0f))
                            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1B"))
                            BREAK_LOOP
                        END_IF
                        SET_FLOAT_VALUE(LW(6), LW(10))
                        WAIT_FRAMES(1)
                    END_LOOP
                END_SIBLING_EVENT
                CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 36, -1)
                BEGIN_SIBLING_EVENT
                    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_HIPDROP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                    CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 800)
                END_SIBLING_EVENT
                WAIT_MS(200)
            END_LOOP
            CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
            WAIT_MS(400)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
            WAIT_MS(1000)
        END_IF
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetMaxHp, -2, LW(0))
        MULTIPLY(LW(0), 25)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, -2, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, -2, 1, 3)
            CALL(btlevtcmd_StopWaitEvent, -2)
            CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_furafura))
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            SET_VALUE(LW(0), 1)
            CALL_CHILD(macho_gance_zoom)
            CALL(btlevtcmd_SetTalkPoseType, -2, 65)
            CALL(btlevtcmd_SetStayPoseType, -2, 43)
            CALL(evt_msg_print, 2, STRING("stg3_tou_471_19"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            CALL(evt_btl_camera_set_mode, 0, 0)
            WAIT_MS(1000)
        END_IF
    END_SWITCH
    LABEL(0)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    SET_VALUE(LW(0), 1)
    CALL_CHILD(macho_gance_zoom)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("D_2"))
    WAIT_MS(170)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_cam_shake, 4, FLOAT(0.01953125f), FLOAT(0.0f), 500)
    WAIT_MS(200)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("T_2"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("S_2"))
    CALL(evt_msg_print, 2, STRING("stg3_tou_471_20"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(500)
    SET_VALUE(LW(10), -2)
    CALL_CHILD(subsetevt_dead_core_nospin_norotate)
    RETURN
EVT_END()

EVT_BEGIN(macho_gance_damage_event)
    CALL(btlevtcmd_ResetFaceDirection, LW(10))
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
    BEGIN_SIBLING_EVENT
        SWITCH(LW(0))
            CASE_OR_EQUAL(43)
            CASE_OR_EQUAL(44)
            CASE_OR_EQUAL(45)
            CALL(btlevtcmd_GetUnitKind, LW(10), LW(2))
            IF_INT_EQUAL(LW(2), 222)
                CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
                CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_DAMAGED1"), LW(2), LW(3), LW(4), 0)
            END_IF
            CASE_END
        END_SWITCH
    END_SIBLING_EVENT
    SWITCH(LW(0))
        CASE_EQUAL(43)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS3"), 0)
        SET_VALUE(LW(14), 36)
        CASE_EQUAL(46)
        CALL(btlevtcmd_GetUnitKind, LW(10), LW(2))
        IF_INT_EQUAL(LW(2), 222)
            CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_VOICE_MARIO_DAMAGED1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_IF
        SET_VALUE(LW(14), 29)
        CASE_OR_EQUAL(49)
        CASE_OR_EQUAL(50)
        CASE_OR_EQUAL(54)
        SET_VALUE(LW(14), 29)
        CASE_END
        CASE_DEFAULT
        SET_VALUE(LW(14), 39)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 10, 0, 0, 0, -1)
    WAIT_MS(500)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_GetHomePos, LW(10), LW(5), LW(6), LW(7))
    MULTIPLY(LW(2), 3)
    MULTIPLY(LW(5), 4)
    ADD(LW(2), LW(5))
    DIVIDE(LW(2), 7)
    CALL(btlevtcmd_TransStageFloorPosition, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 40, -1)
    BEGIN_SIBLING_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 250)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHomePos, LW(10), LW(5), LW(6), LW(7))
    MULTIPLY(LW(5), 2)
    ADD(LW(2), LW(5))
    DIVIDE(LW(2), 3)
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 20, -1)
    BEGIN_SIBLING_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 166)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHomePos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(2), LW(3), LW(4), 10, -1)
    BEGIN_SIBLING_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 116)
    END_SIBLING_EVENT
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(1))
    ADD(LW(1), 1)
    CALL(btlevtcmd_SetUnitWork, -2, 0, LW(1))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_FAST, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(9), PTR(&weapon_gance_macho_speed))
        CALL_CHILD(PTR(gance_macho_speed_attack_event))
        ELSE
        CALL(btlevtcmd_GetUnitWork, -2, 1, LW(2))
        IF_INT_LESS(LW(2), 2)
            IF_INT_LESS_OR_EQUAL(LW(1), 1)
                CALL(btlevtcmd_DrawLots, LW(0), 3, 30, 20, 10)
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    SET_VALUE(LW(9), PTR(&weapon_gance_build_up))
                    CALL_CHILD(PTR(gance_build_up_attack_event))
                    CASE_EQUAL(1)
                    SET_VALUE(LW(9), PTR(&weapon_gance_body))
                    CALL_CHILD(PTR(gance_body_attack_event))
                    CASE_EQUAL(2)
                    SET_VALUE(LW(9), PTR(&weapon_gance_footwork))
                    CALL_CHILD(PTR(gance_footwork_attack_event))
                END_SWITCH
                ELSE
                SET_VALUE(LW(9), PTR(&weapon_gance_attack))
                CALL_CHILD(PTR(gance_attack_attack_event))
            END_IF
            ELSE
            IF_INT_LESS_OR_EQUAL(LW(1), 1)
                CALL(btlevtcmd_DrawLots, LW(0), 4, 25, 20, 10, 10)
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    SET_VALUE(LW(9), PTR(&weapon_gance_build_up))
                    CALL_CHILD(PTR(gance_build_up_attack_event))
                    CASE_EQUAL(1)
                    SET_VALUE(LW(9), PTR(&weapon_gance_body))
                    CALL_CHILD(PTR(gance_body_attack_event))
                    CASE_EQUAL(2)
                    SET_VALUE(LW(9), PTR(&weapon_gance_footwork))
                    CALL_CHILD(PTR(gance_footwork_attack_event))
                    CASE_EQUAL(3)
                    SET_VALUE(LW(9), PTR(&weapon_gance_posing))
                    CALL_CHILD(PTR(gance_posing_attack_event))
                END_SWITCH
                ELSE
                CALL(btlevtcmd_DrawLots, LW(0), 2, 10, 10)
                SWITCH(LW(0))
                    CASE_EQUAL(0)
                    SET_VALUE(LW(9), PTR(&weapon_gance_rariat_attack))
                    CALL_CHILD(PTR(gance_rariat_attack_attack_event))
                    CASE_EQUAL(1)
                    SET_VALUE(LW(9), PTR(&weapon_gance_body_attack))
                    CALL_CHILD(PTR(gance_body_attack_attack_event))
                END_SWITCH
            END_IF
        END_IF
    END_IF
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(gance_macho_speed_attack_event)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(60.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SUBTRACT_FLOAT(LW(2), FLOAT(40.0f))
    CALL(evt_eff64, STRING("hikari"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_3"))
    WAIT_MS(1000)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_eff_delete, STRING("hikari"))
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_SetUnitWork, -2, 2, 1)
    CALL(btlevtcmd_CheckDamage, -2, -2, 1, LW(9), 256, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(gance_build_up_attack_event)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(50.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SUBTRACT_FLOAT(LW(2), FLOAT(40.0f))
    CALL(evt_eff64, STRING("hikari"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING3"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_5"))
    WAIT_MS(1000)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_eff_delete, STRING("hikari"))
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_SetUnitWork, -2, 2, 2)
    CALL(btlevtcmd_CheckDamage, -2, -2, 1, LW(9), 256, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(gance_body_attack_event)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(50.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SUBTRACT_FLOAT(LW(2), FLOAT(40.0f))
    CALL(evt_eff64, STRING("hikari"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING4"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_4"))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_7"))
    WAIT_MS(500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_eff_delete, STRING("hikari"))
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_SetUnitWork, -2, 2, 3)
    CALL(btlevtcmd_CheckDamage, -2, -2, 1, LW(9), 256, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(gance_footwork_attack_event)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(50.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SUBTRACT_FLOAT(LW(2), FLOAT(40.0f))
    CALL(evt_eff64, STRING("hikari"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING5"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_6"))
    WAIT_MS(1000)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_eff_delete, STRING("hikari"))
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_SetUnitWork, -2, 2, 4)
    CALL(btlevtcmd_CheckDamage, -2, -2, 1, LW(9), 256, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(gance_attack_attack_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(50.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SUBTRACT_FLOAT(LW(2), FLOAT(40.0f))
    CALL(evt_eff64, STRING("hikari"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING7"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1B"))
    WAIT_MS(1000)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_eff_delete, STRING("hikari"))
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_PartsFaceDirectionSub, -2, 1, LW(0), 250)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(x_flag_set, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.00390625f), FLOAT(0.0f), 1166)
        CALL(x_flag_set, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 70, -1, 1)
    CALL(x_flag_wait, 0, 0)
    WAIT_MS(500)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    BEGIN_SIBLING_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.00390625f), FLOAT(0.0f), 500)
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 30, -1, 0)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1A"))
        CALL(btlevtcmd_GetPosFloat, -2, LW(5), LW(6), LW(7))
        LOOP(0)
            CALL(btlevtcmd_GetPosFloat, -2, LW(9), LW(10), LW(11))
            SUBTRACT_FLOAT(LW(6), LW(10))
            IF_INT_GREATER(LW(6), FLOAT(0.0f))
                CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("J_1B"))
                BREAK_LOOP
            END_IF
            SET_FLOAT_VALUE(LW(6), LW(10))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 36, -1)
    CALL(x_flag_set, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
    CALL(btlevtcmd_JumpContinue, -2)
    CALL(x_flag_set, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_HIPDROP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 800)
        CALL(x_flag_set, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MoveDirectionAdd, -2, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_MoveDirectionAdd, -2, LW(0), 18)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 9, -1)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
    WAIT_MS(300)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(x_flag_set, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_HIPDROP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 800)
        CALL(x_flag_set, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 48)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 24)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 12)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 6, -1)
    LABEL(98)
    CALL(x_flag_wait, 0, 0)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(x_flag_set, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.00390625f), FLOAT(0.0f), 666)
        CALL(x_flag_set, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 40, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(x_flag_wait, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(gance_posing_attack_event)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(50.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SUBTRACT_FLOAT(LW(2), FLOAT(40.0f))
    CALL(evt_eff64, STRING("hikari"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_SIBLING_EVENT
        WAIT_MS(50)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_FIST1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(180)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_FIST1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_8"))
    WAIT_MS(1000)
    CALL(evt_eff_delete, STRING("hikari"))
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_SetUnitWork, -2, 2, 6)
    CALL(btlevtcmd_CheckDamage, -2, -2, 1, LW(9), 256, LW(5))
    RETURN
EVT_END()

EVT_BEGIN(gance_rariat_attack_attack_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(90.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(20.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(0), LW(6))
    SUBTRACT_FLOAT(LW(2), FLOAT(40.0f))
    CALL(evt_eff64, STRING("hikari"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING8"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_2"))
    WAIT_MS(1000)
    CALL(evt_eff_delete, STRING("hikari"))
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2A"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(8.0f))
    BEGIN_SIBLING_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.00390625f), FLOAT(0.0f), 200)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(11), 0)
    CALL(x_flag_set, 0, 0)
    CALL(x_flag_set, 1, 0)
    LOOP(0)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStatusMg, -2, LW(7))
        MULTIPLY_FLOAT(LW(7), 35)
        CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(6))
        MULTIPLY_FLOAT(LW(7), LW(6))
        ADD_FLOAT(LW(7), 15)
        ADD(LW(0), LW(7))
        ADD(LW(2), 6)
        IF_INT_EQUAL(LW(11), 0)
            CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2B"))
            CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
        ELSE
            CALL(x_flag_set, 1, 1)
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
                CALL(x_flag_set, 1, 0)
            END_SIBLING_EVENT
        END_IF
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        SWITCH(LW(5))
            CASE_OR_EQUAL(4)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
            SET_VALUE(LW(6), 90)
            CASE_END
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
            SET_VALUE(LW(6), 90)
            CASE_EQUAL(1)
            SET_VALUE(LW(6), 91)
            CASE_END
            CASE_DEFAULT
            SET_VALUE(LW(6), 97)
            CASE_END
        END_SWITCH
        GOTO(LW(6))
        LABEL(90)
        GOTO(97)
        LABEL(91)
        IF_INT_EQUAL(LW(11), 0)
            CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        END_IF
        IF_INT_EQUAL(LW(10), 0)
            SET_VALUE(LW(10), 1)
            CALL(x_flag_set, 0, 1)
            BEGIN_SIBLING_EVENT
                CALL(evt_cam_shake, 4, FLOAT(0.01953125f), FLOAT(0.0f), 800)
                CALL(x_flag_set, 0, 0)
            END_SIBLING_EVENT
        END_IF
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        LABEL(97)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        ADD(LW(11), 1)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_EQUAL(LW(3), -1)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(x_flag_wait, 1, 0)
    SUBTRACT(LW(0), LW(7))
    ADD(LW(2), 6)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    WAIT_MS(500)
    CALL(x_flag_wait, 0, 0)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(x_flag_set, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.00390625f), FLOAT(0.0f), 666)
        CALL(x_flag_set, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 40, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(x_flag_wait, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(gance_body_attack_attack_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("E_1A"))
    WAIT_MS(1500)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetPosFloat, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(5))
    CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(50.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SUBTRACT_FLOAT(LW(2), FLOAT(40.0f))
    CALL(evt_eff64, STRING("hikari"), STRING("radiation_n64"), 2, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_PAUSING9"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("M_1"))
    WAIT_MS(1000)
    CALL(evt_eff_delete, STRING("hikari"))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetRotate, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(5), 12)
    LOOP(LW(5))
        CALL(wariai_enzan_1f, LW(5), 12, FLOAT(0.0f), FLOAT(180.0f), LW(6))
        ADD(LW(6), LW(1))
        CALL(btlevtcmd_SetRotate, -2, 0, LW(6), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_ChangeFaceDirection, -2, 1)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    WAIT_MS(500)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1A"))
    WAIT_MS(300)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1B"))
        CALL(btlevtcmd_GetStatusMg, -2, LW(0))
        MULTIPLY_FLOAT(LW(0), FLOAT(70.0f))
        CALL(btlevtcmd_GetUnitWorkFloat, -2, 3, LW(1))
        MULTIPLY_FLOAT(LW(0), LW(1))
        CALL(btlevtcmd_SetRotateOffset, -2, FLOAT(0.0f), LW(0), FLOAT(0.0f))
        SET_VALUE(LW(5), FLOAT(28.799805f))
        LOOP(LW(5))
            CALL(wariai_enzan_1f_intpl, LW(5), FLOAT(28.799805f), FLOAT(0.0f), FLOAT(320.0f), LW(6), 5)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(6))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1C"))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_SetRotateOffset, -2, FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 36, -1)
    CALL(x_flag_set, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
    CALL(btlevtcmd_JumpContinue, -2)
    CALL(x_flag_set, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_HIPDROP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 800)
        CALL(x_flag_set, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1D"))
    WAIT_MS(300)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(x_flag_set, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GANSU_HIPDROP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.01953125f), 800)
        CALL(x_flag_set, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1D"))
    CALL(btlevtcmd_JumpContinue, -2)
    LABEL(98)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MoveDirectionAdd, -2, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_MoveDirectionAdd, -2, LW(0), 18)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 9, -1)
    CALL(x_flag_wait, 0, 0)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(x_flag_set, 0, 1)
    BEGIN_SIBLING_EVENT
        CALL(evt_cam_shake, 4, FLOAT(0.00390625f), FLOAT(0.0f), 666)
        CALL(x_flag_set, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 40, -1, 0)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(x_flag_wait, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static USER_FUNC(x_flag_wait) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);

    if (value == x_flag_data[index]) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(x_flag_get) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], x_flag_data[index]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(x_flag_add) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] += evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(x_flag_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_0f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_0) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

USER_FUNC(wariai_enzan_1f_intpl_w) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);
    s32 out = args[4];
    f32 t1 = evtGetFloat(event, args[5]);
    f32 t2 = evtGetFloat(event, args[6]);
    f32 r1;
    f32 r2;
    f32 w;

    if (t1 >= 14.0f) {
        t1 -= 14.0f;
        if (t1 >= 14.0f) {
            evtSetFloat(event, out, c);
            return EVT_RETURN_DONE;
        }
        r1 = 1.0f - intplGetValue(t1, 1.0f, 0.0f, b - a, b);
    } else {
        r1 = intplGetValue(t1, 0.0f, 1.0f, b - a, b);
    }
    if (t2 >= 14.0f) {
        t2 = t1 - 14.0f;
        if (t2 >= 14.0f) {
            evtSetFloat(event, out, c);
            return EVT_RETURN_DONE;
        }
        r2 = 1.0f - intplGetValue(t2, 1.0f, 0.0f, b - a, b);
    } else {
        r2 = intplGetValue(t2, 0.0f, 1.0f, b - a, b);
    }
    w = r1 * r2;
    evtSetFloat(event, out, c * (1.0f - w) + d * w);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_1f_intpl) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);
    s32 out = args[4];
    f32 t = evtGetFloat(event, args[5]);
    f32 r;

    if (t >= 14.0f) {
        t -= 14.0f;
        if (t >= 14.0f) {
            evtSetFloat(event, out, c);
            return EVT_RETURN_DONE;
        }
        r = 1.0f - intplGetValue(t, 1.0f, 0.0f, b - a, b);
    } else {
        r = intplGetValue(t, 0.0f, 1.0f, b - a, b);
    }
    evtSetFloat(event, out, (1.0f - r) * c + r * d);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_1f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

static USER_FUNC(wariai_enzan_1) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}
