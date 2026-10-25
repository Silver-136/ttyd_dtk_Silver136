#include "system.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "eff/n64/eff_glass_n64.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "eff/eff_syuryou.h"
#include "memory.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"

s32 irand(s32 scalar);
static u8 barriernSatelliteTo_Init(BattleUnit* unit, s32 n, Vec* table);
static u8 barriernSatelliteTo_Main(BattleUnit* unit, s32 n);

typedef struct BarriernSpline { s32 n; Vec* table; f32 a[5]; f32 b[15]; s32 count; } BarriernSpline;
void spline_maketable(s32 n, Vec* table, f32* a, f32* b);
void spline_getvalue(f32 t, Vec* out, s32 n, Vec* table, f32* a, f32* b);
static s32 zanzou_count[5];
void BtlUnit_SetAnim(BattleUnitPart* part, const char* anim);
static s32 barriern_satellite_zanzou_sub(BOOL isFirstCall, BattleUnit* unit, f32* scales, s32 n, s32 type);
static s32 barriern_zanzou_sub(BOOL isFirstCall, BattleUnit* unit, f32* scales, s32 type);

static USER_FUNC(_check_satellite);
static USER_FUNC(barriern_satellite_zanzou);
static USER_FUNC(barriern_satellite_zanzou2);
static USER_FUNC(battleGetBarriernSatelliteInitPos);
USER_FUNC(btlevtcmd_SetEventUnisonPhase);
static USER_FUNC(outSplineTableSet);
static void barriern_wawawa_get_next_target(s32* unitIdx, s32* partIdx);
static void barriern_wawawa_get_target(s32* unitIdx, s32* partIdx);
static USER_FUNC(barriern_wawawa_set_target);
static void outSplineTableSet_Sub(BattleUnit* unit, s32 n, Vec* table);
static USER_FUNC(barriernSatelliteToOutside);
static USER_FUNC(barriernSatelliteToInside);
static USER_FUNC(barriern_zanzou3);
static USER_FUNC(barriern_zanzou2);
static USER_FUNC(barriern_zanzou);
extern BattleUnitKind unit_barriern_custom_satellite;

BattleUnitKind unit_barriern_custom;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitKindPart parts[11];
static StatusVulnerability regist;
static BattleWeapon weapon_wakka_attack;
static BattleWeapon weapon_wawawa_attack;
static BattleUnitSetup entry_barriern_satellite;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static Vec splineTableIn1_1[5];
static Vec splineTableIn2_1[5];
static Vec splineTableIn2_2[5];
static Vec splineTableIn3_1[5];
static Vec splineTableIn3_2[5];
static Vec splineTableIn3_3[5];
static Vec splineTableIn4_1[5];
static Vec splineTableIn4_2[5];
static Vec splineTableIn4_3[5];
static Vec splineTableIn4_4[5];
static Vec splineTableOut1_1[5];
static Vec splineTableOut2_1[5];
static Vec splineTableOut2_2[5];
static Vec splineTableOut3_1[5];
static Vec splineTableOut3_2[5];
static Vec splineTableOut3_3[5];
static Vec splineTableOut4_1[5];
static Vec splineTableOut4_2[5];
static Vec splineTableOut4_3[5];
static Vec splineTableOut4_4[5];
static EvtWord init_event[143];
static EvtWord damage_event[17];
static EvtWord barriern_dmg_first_attack_pos[19];
static EvtWord satellite_blur_on[42];
static EvtWord satellite_blur_off[29];
static EvtWord unison_phase_event[190];
static EvtWord attack_event[106];
static EvtWord recovery_event[69];
static EvtWord start_avoid[26];
static EvtWord wakka_attack_common_event_no_hit[305];
static EvtWord wakka_attack_common_event_hit[211];
static EvtWord damage_check_event_sub[131];
static EvtWord wakka_attack_event[255];
static EvtWord wawawa_attack_event_satellite_blur[10];
static EvtWord wawawa_attack_event[548];
static EvtWord wawawa_attack_event_sub[87];
static EvtWord wawawa_attack_event_sub_no_last[10];
static EvtWord wawawa_attack_event_sub_last[93];
static EvtWord wait_event[116];

BattleUnitKind unit_barriern_custom = {
    117,
    "btl_un_barriern_custom",
    10,
    0,
    1,
    1,
    32,
    0,
    0,
    70,
    5,
    50,
    9999,
    50,
    50,
    0,
    25,
    0.0f,
    0.0f,
    0.0f,
    0,
    -40,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    0.0f,
    10.0f,
    10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    50.0f,
    50.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_BARRI_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x4,
    &regist,
    11,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 12.0f, 0.0f, 0.0f, 22.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x600009, 0, pose_table},
    {2, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {3, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {4, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {5, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {6, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {7, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {8, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {9, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {10, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {11, "btl_un_barriern_custom", "c_baria_c", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
};

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 100, 0, 100, 0, 0, 0, 0, 100, 0, 100, 100, 10};

static BattleWeapon weapon_wakka_attack = {
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
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
    0, 0, // sleepChance..sleepTime
    80, // stopChance
    3, // stopTime
    0, 0, 0, 0, 0, 0, 0, 0, // dizzyChance..electricChance
    0, 0, 0, 0, 0, 0, 0, 0, // electricTime..sizeChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeTime..defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_wawawa_attack = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002000, // targetWeightFlags
    0, 0, // sleepChance..sleepTime
    80, // stopChance
    3, // stopTime
    0, 0, 0, 0, 0, 0, 0, 0, // dizzyChance..electricChance
    0, 0, 0, 0, 0, 0, 0, 0, // electricTime..sizeChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeTime..defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleUnitSetup entry_barriern_satellite = {
    &unit_barriern_custom_satellite, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static PoseTableEntry pose_table[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "X_1"},
    {28, "S_1"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "S_1"},
    {39, "D_1"},
    {42, "R_1"},
    {41, "R_1"},
    {40, "W_1"},
    {56, "X_1"},
    {57, "X_1"},
    {65, "T_1"},
    {69, "S_1"},
};

static BattleDataEntry data_table[] = {
    {48, barriern_dmg_first_attack_pos},
    {0, NULL},
};

static Vec splineTableIn1_1[] = {
    {25.0f, 0.0f, 0.0f},
    {20.0f, -30.0f, -1.0f},
    {12.5f, -40.0f, -1.0f},
    {5.0f, -30.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

static Vec splineTableIn2_1[] = {
    {25.0f, 0.0f, 0.0f},
    {20.0f, -30.0f, -1.0f},
    {12.5f, -40.0f, -1.0f},
    {5.0f, -30.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

static Vec splineTableIn2_2[] = {
    {-25.0f, 0.0f, 0.0f},
    {-20.0f, 30.0f, -1.0f},
    {-12.5f, 30.0f, -1.0f},
    {-5.0f, 30.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

static Vec splineTableIn3_1[] = {
    {0.0f, 25.0f, 0.0f},
    {0.0f, 45.0f, -1.0f},
    {0.0f, 50.0f, -1.0f},
    {0.0f, 25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

static Vec splineTableIn3_2[] = {
    {25.0f, -25.0f, 0.0f},
    {40.0f, -40.0f, -1.0f},
    {30.0f, -35.0f, -1.0f},
    {20.0f, -25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

static Vec splineTableIn3_3[] = {
    {-25.0f, -25.0f, 0.0f},
    {-40.0f, -40.0f, -1.0f},
    {-30.0f, -35.0f, -1.0f},
    {-20.0f, -25.0f, -1.0f},
    {-0.0f, 0.0f, -1.0f},
};

static Vec splineTableIn4_1[] = {
    {25.0f, 25.0f, 0.0f},
    {30.0f, 35.0f, -1.0f},
    {40.0f, 45.0f, -1.0f},
    {20.0f, 25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

static Vec splineTableIn4_2[] = {
    {-25.0f, 25.0f, 0.0f},
    {-30.0f, 35.0f, -1.0f},
    {-40.0f, 45.0f, -1.0f},
    {-20.0f, 25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

static Vec splineTableIn4_3[] = {
    {25.0f, -25.0f, 0.0f},
    {30.0f, -35.0f, -1.0f},
    {40.0f, -45.0f, -1.0f},
    {20.0f, -25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

static Vec splineTableIn4_4[] = {
    {-25.0f, -25.0f, 0.0f},
    {-30.0f, -35.0f, -1.0f},
    {-40.0f, -45.0f, -1.0f},
    {-20.0f, -25.0f, -1.0f},
    {-0.0f, 0.0f, -1.0f},
};

static Vec splineTableOut1_1[] = {
    {0.0f, 0.0f, -1.0f},
    {-35.0f, -35.0f, -1.0f},
    {-40.0f, 0.0f, -1.0f},
    {30.0f, 40.0f, -1.0f},
    {25.0f, 0.0f, 0.0f},
};

static Vec splineTableOut2_1[] = {
    {0.0f, 0.0f, -1.0f},
    {5.0f, 30.0f, -1.0f},
    {12.5f, 40.0f, -1.0f},
    {20.0f, 30.0f, -1.0f},
    {25.0f, 0.0f, 0.0f},
};

static Vec splineTableOut2_2[] = {
    {0.0f, 0.0f, -1.0f},
    {-5.0f, -30.0f, -1.0f},
    {-12.5f, -40.0f, -1.0f},
    {-20.0f, -30.0f, -1.0f},
    {-25.0f, 0.0f, 0.0f},
};

static Vec splineTableOut3_1[] = {
    {0.0f, 0.0f, -1.0f},
    {0.0f, 25.0f, -1.0f},
    {0.0f, 50.0f, -1.0f},
    {0.0f, 45.0f, -1.0f},
    {0.0f, 25.0f, 0.0f},
};

static Vec splineTableOut3_2[] = {
    {0.0f, 0.0f, -1.0f},
    {25.0f, -20.0f, -1.0f},
    {45.0f, -30.0f, -1.0f},
    {30.0f, -25.0f, -1.0f},
    {25.0f, -25.0f, 0.0f},
};

static Vec splineTableOut3_3[] = {
    {0.0f, 0.0f, -1.0f},
    {-25.0f, -20.0f, -1.0f},
    {-45.0f, -30.0f, -1.0f},
    {-30.0f, -25.0f, -1.0f},
    {-25.0f, -25.0f, 0.0f},
};

static Vec splineTableOut4_1[] = {
    {0.0f, 0.0f, -1.0f},
    {25.0f, 20.0f, -1.0f},
    {45.0f, 30.0f, -1.0f},
    {30.0f, 25.0f, -1.0f},
    {25.0f, 25.0f, 0.0f},
};

static Vec splineTableOut4_2[] = {
    {0.0f, 0.0f, -1.0f},
    {-25.0f, 20.0f, -1.0f},
    {-45.0f, 30.0f, -1.0f},
    {-30.0f, 25.0f, -1.0f},
    {-25.0f, 25.0f, 0.0f},
};

static Vec splineTableOut4_3[] = {
    {0.0f, 0.0f, -1.0f},
    {25.0f, -20.0f, -1.0f},
    {45.0f, -30.0f, -1.0f},
    {30.0f, -25.0f, -1.0f},
    {25.0f, -25.0f, 0.0f},
};

static Vec splineTableOut4_4[] = {
    {0.0f, 0.0f, -1.0f},
    {-25.0f, -20.0f, -1.0f},
    {-45.0f, -30.0f, -1.0f},
    {-30.0f, -25.0f, -1.0f},
    {-25.0f, -25.0f, 0.0f},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, -2, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetPartsScale, -2, 1, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_OnUnitFlag, -2, 3145728)
    SET_VALUE(LW(0), 4)
    LOOP(4)
        CALL(btlevtcmd_SetUnitWork, -2, LW(0), -1)
        ADD(LW(0), 1)
    END_LOOP
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(7))
    LABEL(0)
    IF_INT_LESS(LW(6), LW(7))
        CALL(btlevtcmd_GetUnitId, -2, LW(5))
        SET_VALUE(LW(8), LW(6))
        ADD(LW(8), 4)
        CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_barriern_satellite), 0)
        CALL(btlevtcmd_SetUnitWork, -2, LW(8), LW(3))
        CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(5))
        CALL(btlevtcmd_SetUnitWork, LW(3), 1, 1)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(battleGetBarriernSatelliteInitPos, -2, LW(7), LW(6), LW(10), LW(11), LW(12))
        ADD(LW(0), LW(10))
        ADD(LW(1), LW(11))
        ADD(LW(2), LW(12))
        CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetScale, LW(3), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        ADD(LW(6), 1)
        GOTO(0)
    END_IF
    LABEL(10)
    CALL(outSplineTableSet, -2, LW(7))
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetRGB, -2, 1, 255, 255, 255)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(barriern_dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 20)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

static EVT_BEGIN(satellite_blur_on)
    SET_VALUE(LW(2), 4)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(2), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_SetPartsBlur, LW(3), LW(4), 255, 255, 255, 255, 255, 255, 255, 100, 0)
            CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), 67108864)
        END_IF
        ADD(LW(2), 1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(satellite_blur_off)
    SET_VALUE(LW(2), 4)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(2), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_OffPartsAttribute, LW(3), LW(4), 67108864)
        END_IF
        ADD(LW(2), 1)
    END_LOOP
    RETURN
EVT_END()

static USER_FUNC(barriern_zanzou) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    return barriern_zanzou_sub(isFirstCall, unit, scales, 0);
}

static USER_FUNC(barriern_zanzou2) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    return barriern_zanzou_sub(isFirstCall, unit, scales, 1);
}

static USER_FUNC(barriern_zanzou3) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    return barriern_zanzou_sub(isFirstCall, unit, scales, 2);
}

static inline f32 intpl_ff(f32 start, f32 end, s32 mode, s32 cur, s32 total) {
    return intplGetValue(mode, start, end, cur, total);
}

static s32 barriern_zanzou_sub(BOOL isFirstCall, BattleUnit* unit, f32* scales, s32 type) {
    f32 start[5];
    f32 end[5];
    f32 x;
    f32 y;
    f32 z;
    BattleUnitPart* part;
    s32 frames;
    f32 scale;
    f32 ofs;
    u8 alphaStart;
    u8 alphaEnd;
    s32 mode = 4;
    s32 i;
    s32 j;

    switch (type) {
        case 0:
            scale = 0.75f;
            frames = 36;
            for (i = 0; i < 5; i++) {
                start[i] = scales[i];
                end[i] = scale;
            }
            ofs = 0.5f;
            alphaStart = 0;
            alphaEnd = 64;
            mode = 4;
            break;
        case 1:
        case 2:
            scale = 1.0f;
            frames = 8;
            for (i = 0; i < 5; i++) {
                start[i] = scale;
                end[i] = scales[i];
            }
            ofs = -0.5f;
            alphaStart = 64;
            alphaEnd = 0;
            mode = 1;
            break;
    }
    if (type == 2) {
        frames = 36;
    }
    if (isFirstCall) {
        zanzou_count[0] = 0;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetPartsScale(BtlUnit_GetPartsPtr(unit, 1), 1.0f, 1.0f, 1.0f);
        for (i = 2, j = 0; i <= 6; i++, j++) {
            part = BtlUnit_GetPartsPtr(unit, i);
            BtlUnit_SetPartsPos(part, x, y, i * ofs + z);
            BtlUnit_SetPartsScale(part, start[j], start[j], 1.0f);
            part->color.a = alphaEnd;
            part->attributes &= ~kHidden;
        }
    }
    part = BtlUnit_GetPartsPtr(unit, 1);
    {
        f32 s = intplGetValue(mode, 1.0f, scale, zanzou_count[0], frames);
        BtlUnit_SetPartsScale(part, s, s, 1.0f);
    }
    for (i = 2, j = 0; i <= 6; i++, j++) {
        f32 s;

        part = BtlUnit_GetPartsPtr(unit, i);
        s = intpl_ff(start[j], end[j], mode, zanzou_count[0], frames);
        BtlUnit_SetPartsScale(part, s, s, 1.0f);
        part->color.a = intplGetValue(0, alphaStart, alphaEnd, zanzou_count[0], frames);
    }
    if (zanzou_count[0] >= frames) {
        part = BtlUnit_GetPartsPtr(unit, 1);
        BtlUnit_SetPartsScale(part, 1.0f, 1.0f, 1.0f);
        part->color.a = 255;
        for (i = 2; i <= 6; i++) {
            BtlUnit_GetPartsPtr(unit, i)->attributes |= 0x1000000;
        }
        return EVT_RETURN_DONE;
    }
    zanzou_count[0]++;
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(barriern_satellite_zanzou) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 n = evtGetValue(event, args[1]);
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = BattleGetUnitPtr(wp, id);
    BattleUnit* sat;
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    sat = BattleGetUnitPtr(wp, unit->work[4 + n]);
    return barriern_satellite_zanzou_sub(isFirstCall, sat, scales, n, 0);
}

static USER_FUNC(barriern_satellite_zanzou2) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 n = evtGetValue(event, args[1]);
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = BattleGetUnitPtr(wp, id);
    BattleUnit* sat;
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    sat = BattleGetUnitPtr(wp, unit->work[4 + n]);
    return barriern_satellite_zanzou_sub(isFirstCall, sat, scales, n, 1);
}

static s32 barriern_satellite_zanzou_sub(BOOL isFirstCall, BattleUnit* unit, f32* scales, s32 n, s32 type) {
    f32 start[5];
    f32 end[5];
    f32 x;
    f32 y;
    f32 z;
    BattleUnitPart* part;
    s32 frames;
    f32 scale;
    f32 ofs;
    u8 alphaStart;
    u8 alphaEnd;
    s32 mode = 4;
    s32 i;
    s32 j;

    switch (type) {
        case 0:
            scale = 0.75f;
            frames = 36;
            for (i = 0; i < 5; i++) {
                start[i] = scales[i];
                end[i] = scale;
            }
            ofs = 0.5f;
            alphaStart = 0;
            alphaEnd = 64;
            mode = 4;
            break;
        case 1:
            scale = 1.0f;
            frames = 8;
            for (i = 0; i < 5; i++) {
                start[i] = scale;
                end[i] = scales[i];
            }
            ofs = -0.5f;
            alphaStart = 64;
            alphaEnd = 0;
            mode = 1;
            break;
    }
    if (isFirstCall) {
        zanzou_count[(s32)(n + 1)] = 0;
        BtlUnit_GetPos(unit, &x, &y, &z);
        BtlUnit_SetPartsScale(BtlUnit_GetPartsPtr(unit, 1), 1.0f, 1.0f, 1.0f);
        for (i = 2, j = 0; i <= 6; i++, j++) {
            part = BtlUnit_GetPartsPtr(unit, i);
            BtlUnit_SetPartsPos(part, x, y, i * ofs + z);
            BtlUnit_SetPartsScale(part, start[j], start[j], 1.0f);
            BtlUnit_SetPartsRotate(part, 0.0f, 0.0f, 0.0f);
            part->color.a = alphaStart;
            BtlUnit_SetAnim(part, "Z_2");
            part->attributes &= ~kHidden;
        }
    }
    part = BtlUnit_GetPartsPtr(unit, 1);
    {
        f32 s = intplGetValue(4, 1.0f, scale, zanzou_count[0], frames);
        BtlUnit_SetPartsScale(part, s, s, 1.0f);
    }
    for (i = 2, j = 0; i <= 6; i++, j++) {
        f32 s;

        part = BtlUnit_GetPartsPtr(unit, i);
        s = intplGetValue(mode, start[j], end[j], zanzou_count[(s32)(n + 1)], frames);
        BtlUnit_SetPartsScale(part, s, s, 1.0f);
        part->color.a = intplGetValue(0, alphaStart, alphaEnd, zanzou_count[(s32)(n + 1)], frames);
    }
    if (zanzou_count[(s32)(n + 1)] >= frames) {
        part = BtlUnit_GetPartsPtr(unit, 1);
        BtlUnit_SetPartsScale(part, 1.0f, 1.0f, 1.0f);
        part->color.a = 255;
        for (i = 2; i <= 6; i++) {
            BtlUnit_GetPartsPtr(unit, i)->attributes |= 0x1000000;
        }
        return EVT_RETURN_DONE;
    }
    zanzou_count[(s32)(n + 1)]++;
    return EVT_RETURN_BLOCK;
}

static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_StopWaitEvent, -2)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(evt_sub_random, 10, LW(0))
    WAIT_FRAMES(LW(0))
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 4)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            ADD(LW(0), 1)
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    IF_INT_GREATER_OR_EQUAL(LW(0), 4)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK9"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriern_zanzou3, -2)
    CALL_CHILD(satellite_blur_on)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(1))
    IF_INT_GREATER_OR_EQUAL(LW(1), 1)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_SPLIT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(barriernSatelliteToInside, -2, LW(1))
    CALL_CHILD(satellite_blur_off)
    CALL(btlevtcmd_GetUnitId, -2, LW(4))
    SET_VALUE(LW(1), 4)
    SET_VALUE(LW(5), 0)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(2))
        IF_INT_EQUAL(LW(2), -1)
            CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_barriern_satellite), 0)
            CALL(btlevtcmd_AddPos, LW(3), 0, 0, -1)
            CALL(btlevtcmd_SetUnitWork, -2, LW(1), LW(3))
            CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
            ADD(LW(0), 1)
            ADD(LW(5), 1)
            IF_INT_GREATER_OR_EQUAL(LW(5), 2)
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL_CHILD(satellite_blur_on)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_SPLIT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriernSatelliteToOutside, -2, LW(0))
    WAIT_FRAMES(60)
    CALL_CHILD(satellite_blur_off)
    CALL(btlevtcmd_SetUnitWork, -2, 0, LW(0))
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetRGB, -2, 1, 255, 255, 255)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(wakka_attack_event))
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(_check_satellite)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), 4)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, -2, LW(1), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_CheckActStatus, LW(3), LW(2))
            IF_INT_NOT_EQUAL(LW(2), 0)
                CALL(btlevtcmd_CheckStatus, LW(3), 5, LW(2))
                IF_INT_EQUAL(LW(2), 0)
                    ADD(LW(0), 1)
                END_IF
            END_IF
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    IF_INT_EQUAL(LW(0), 4)
        CALL_CHILD(PTR(wawawa_attack_event))
        ELSE
        SET_VALUE(LW(0), 100)
        ADD(LW(0), 0)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 100)
            CALL_CHILD(PTR(wakka_attack_event))
            ELSE
            CALL_CHILD(PTR(recovery_event))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(recovery_event)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_3"))
    WAIT_FRAMES(120)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_RecoverHp, -2, 1, 0)
    CALL(btlevtcmd_GetPos, -2, LW(5), LW(6), LW(7))
    ADD(LW(6), 30)
    ADD(LW(7), 10)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(5), LW(6), LW(7), 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(30)
    RETURN
EVT_END()

static EVT_BEGIN(start_avoid)
    IF_INT_EQUAL(LW(5), 3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
    END_IF
    IF_INT_EQUAL(LW(5), 6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
    END_IF
    IF_INT_EQUAL(LW(5), 2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(wakka_attack_common_event_no_hit)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(6))
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_OffPartsAttribute, -2, LW(15), 67108864)
    ELSE
        CALL(btlevtcmd_SetPartsBlur, -2, LW(15), 255, 255, 255, 255, 255, 255, 255, 100, 0)
        CALL(btlevtcmd_OnPartsAttribute, -2, LW(15), 67108864)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, LW(15), STRING("Z_3"))
    CALL(btlevtcmd_GetPos, LW(12), LW(6), LW(7), LW(8))
    ADD(LW(8), 1)
    CALL(btlevtcmd_SetPartsPos, -2, LW(15), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_CheckPartsAttribute, -2, LW(15), 16777216, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_OffPartsAttribute, -2, LW(15), 16777216)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_5"))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_GetPartsPos, -2, LW(15), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_LESS_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            ELSE
                CALL(btlevtcmd_GetPartsPos, -2, LW(15), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_GREATER_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(6))
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
        CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
        MULTIPLY(LW(6), 1)
        CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
        MULTIPLY(LW(6), LW(0))
        SUBTRACT(LW(7), 5)
        CALL(btlevtcmd_SetPartsScale, -2, LW(15), 1, 0, 1)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                LOOP(10)
                    CALL(btlevtcmd_AddPartsScale, -2, LW(15), 0, FLOAT(0.125f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                LOOP(10)
                    CALL(btlevtcmd_AddPartsScale, -2, LW(15), 0, FLOAT(-0.125f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPartsSetting, -2, LW(15), 0, 4, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(15), LW(6), LW(7), LW(8), 0, -1)
    ELSE
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
        CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
        MULTIPLY(LW(6), 1)
        CALL(btlevtcmd_GetFaceDirection, -2, LW(0))
        MULTIPLY(LW(6), LW(0))
        SUBTRACT(LW(7), 5)
        CALL(btlevtcmd_SetPartsScale, -2, LW(15), 1, 0, 1)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                LOOP(5)
                    CALL(btlevtcmd_AddPartsScale, -2, LW(15), 0, FLOAT(0.25f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                LOOP(5)
                    CALL(btlevtcmd_AddPartsScale, -2, LW(15), 0, FLOAT(-0.25f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPartsSetting, -2, LW(15), 0, 7, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(15), LW(6), LW(7), LW(8), 0, -1)
        CALL(btlevtcmd_SetPartsPos, -2, LW(15), 0, -1000, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(wakka_attack_common_event_hit)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(6))
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_OffPartsAttribute, -2, LW(15), 67108864)
        ELSE
        CALL(btlevtcmd_SetPartsBlur, -2, LW(15), 255, 255, 255, 255, 255, 255, 255, 100, 0)
        CALL(btlevtcmd_OnPartsAttribute, -2, LW(15), 67108864)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, LW(15), STRING("Z_3"))
    CALL(btlevtcmd_GetPos, LW(12), LW(6), LW(7), LW(8))
    ADD(LW(8), 1)
    CALL(btlevtcmd_SetPartsPos, -2, LW(15), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_CheckPartsAttribute, -2, LW(15), 16777216, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_OffPartsAttribute, -2, LW(15), 16777216)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_5"))
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(6))
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_SetPartsScale, -2, LW(15), 1, 0, 1)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                LOOP(10)
                    CALL(btlevtcmd_AddPartsScale, -2, LW(15), 0, FLOAT(0.125f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                LOOP(10)
                    CALL(btlevtcmd_AddPartsScale, -2, LW(15), 0, FLOAT(-0.125f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
        CALL(btlevtcmd_JumpPartsSetting, -2, LW(15), 0, 4, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(15), LW(6), LW(7), LW(8), 0, -1)
        ELSE
        CALL(btlevtcmd_SetPartsScale, -2, LW(15), 1, 0, 1)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                LOOP(5)
                    CALL(btlevtcmd_AddPartsScale, -2, LW(15), 0, FLOAT(0.25f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                LOOP(5)
                    CALL(btlevtcmd_AddPartsScale, -2, LW(15), 0, FLOAT(-0.25f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
        CALL(btlevtcmd_JumpPartsSetting, -2, LW(15), 0, 7, 0)
        CALL(btlevtcmd_JumpPartsPosition, -2, LW(15), LW(6), LW(7), LW(8), 0, -1)
        CALL(btlevtcmd_SetPartsPos, -2, LW(15), 0, -1000, 0)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(damage_check_event_sub)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        GOTO(90)
        CASE_EQUAL(2)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    CALL_CHILD(PTR(wakka_attack_common_event_no_hit))
    GOTO(98)
    LABEL(91)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LW(14), 5)
    LOOP(0)
        CALL(btlevtcmd_GetPartsWork, -2, 1, LW(13), LW(5))
        CALL(btlevtcmd_GetPartsWork, -2, 1, LW(14), LW(6))
        IF_INT_EQUAL(LW(3), LW(5))
            IF_INT_EQUAL(LW(4), LW(6))
                ADD(LW(13), 10)
                CALL(btlevtcmd_SetPartsWork, -2, 1, LW(13), 1)
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(13), 1)
        ADD(LW(14), 1)
    END_LOOP
    CALL_CHILD(PTR(wakka_attack_common_event_hit))
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    IF_INT_EQUAL(LW(10), 1)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
        ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 0, LW(5))
    END_IF
    GOTO(98)
    LABEL(98)
    CALL(btlevtcmd_SetPartsPos, -2, LW(15), 0, -1000, 0)
    RETURN
EVT_END()

static EVT_BEGIN(wakka_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_wakka_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_wakka_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 0, LW(3))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 5, LW(4))
    SET_VALUE(LW(9), PTR(&weapon_wakka_attack))
    SET_VALUE(LW(10), 1)
    SET_VALUE(LW(12), -2)
    SET_VALUE(LW(15), 7)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_wakka_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_wakka_attack))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(14))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(14))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriern_zanzou, -2)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK9"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriern_zanzou2, -2)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    SET_VALUE(LW(0), 6)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(14))
    MULTIPLY(LW(0), LW(14))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
    END_LOOP
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3"))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_4"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK7"), EVT_NULLPTR, 0, LW(0))
    CALL(btlevtcmd_SetPartsWork, -2, 7, 15, LW(0))
    CALL_CHILD(damage_check_event_sub)
    CALL(btlevtcmd_GetPartsWork, -2, 7, 15, LW(0))
    CALL(evt_snd_sfxoff, LW(0))
    SET_VALUE(LW(0), -6)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(14))
    MULTIPLY(LW(0), LW(14))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
    END_LOOP
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wawawa_attack_event_satellite_blur)
    CALL(barriern_satellite_zanzou, -2, LW(0))
    CALL(barriern_satellite_zanzou2, -2, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(wawawa_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_wawawa_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_wawawa_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 1, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(barriern_wawawa_set_target, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 0, LW(0))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 1, LW(2))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 2, LW(4))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 3, LW(6))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 4, LW(8))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 5, LW(1))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 6, LW(3))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 7, LW(5))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 8, LW(7))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 9, LW(9))
    CALL(btlevtcmd_SetPartsWork, -2, 1, 10, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 11, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 12, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 13, 0)
    CALL(btlevtcmd_SetPartsWork, -2, 1, 14, 0)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_wawawa_attack))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_wawawa_attack))
    CALL(btlevtcmd_GetPartsWork, -2, 1, 0, LW(3))
    CALL(btlevtcmd_GetPartsWork, -2, 1, 5, LW(4))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(0), 0)
    LOOP(4)
        SPAWN_EVENT(wawawa_attack_event_satellite_blur)
        ADD(LW(0), 1)
        WAIT_FRAMES(10)
    END_LOOP
    CALL(barriern_zanzou, -2)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK10"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriern_zanzou2, -2)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("F_1"))
    WAIT_FRAMES(20)
    SET_VALUE(LW(11), 4)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, LW(11), LW(12))
        CALL(btlevtcmd_GetBodyId, LW(12), LW(13))
        CALL(btlevtcmd_AnimeChangePose, LW(12), LW(13), STRING("F_2"))
        ADD(LW(11), 1)
    END_LOOP
    WAIT_FRAMES(20)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    SET_VALUE(LW(11), 4)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, LW(11), LW(12))
        CALL(btlevtcmd_SetRotate, LW(12), 0, 0, 0)
        ADD(LW(11), 1)
    END_LOOP
    SET_VALUE(LW(0), 6)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(14))
    MULTIPLY(LW(0), LW(14))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
        SET_VALUE(LW(11), 4)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(11), 8)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, -2, LW(11), LW(12))
            CALL(btlevtcmd_AddRotate, LW(12), 0, LW(0), 0)
            ADD(LW(11), 1)
        END_LOOP
    END_LOOP
    SET_VALUE(LW(11), 4)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, LW(11), LW(12))
        CALL(btlevtcmd_GetBodyId, LW(12), LW(13))
        CALL(btlevtcmd_AnimeChangePose, LW(12), LW(13), STRING("S_2"))
        ADD(LW(11), 1)
    END_LOOP
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_3"))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_4"))
    SET_VALUE(LW(12), -2)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LW(14), 5)
    SET_VALUE(LW(15), 7)
    SPAWN_EVENT(wawawa_attack_event_sub)
    WAIT_FRAMES(30)
    SET_VALUE(LW(11), 4)
    ADD(LW(13), 1)
    ADD(LW(14), 1)
    ADD(LW(15), 1)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, LW(11), LW(12))
        SPAWN_EVENT(wawawa_attack_event_sub)
        WAIT_FRAMES(20)
        ADD(LW(11), 1)
        ADD(LW(13), 1)
        ADD(LW(14), 1)
        ADD(LW(15), 1)
    END_LOOP
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 5)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(10)
    CALL(_check_satellite)
    SET_VALUE(LW(0), -6)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(14))
    MULTIPLY(LW(0), LW(14))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, -2, 0, LW(0), 0)
        SET_VALUE(LW(11), 4)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(11), 8)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, -2, LW(11), LW(12))
            CALL(btlevtcmd_AddRotate, LW(12), 0, LW(0), 0)
            ADD(LW(11), 1)
        END_LOOP
    END_LOOP
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wawawa_attack_event_sub)
    CALL(btlevtcmd_GetPartsWork, -2, 1, LW(13), LW(3))
    CALL(btlevtcmd_GetPartsWork, -2, 1, LW(14), LW(4))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_BARRI_ATTACK8"), EVT_NULLPTR, 0, LW(0))
    CALL(btlevtcmd_SetPartsWork, -2, LW(15), 15, LW(0))
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(13), 3)
            CALL_CHILD(wawawa_attack_event_sub_last)
            BREAK_LOOP
        END_IF
        ADD(LW(13), 1)
        ADD(LW(14), 1)
        CALL(btlevtcmd_GetPartsWork, -2, 1, LW(13), LW(5))
        CALL(btlevtcmd_GetPartsWork, -2, 1, LW(14), LW(6))
        IF_INT_EQUAL(LW(3), LW(5))
            IF_INT_EQUAL(LW(4), LW(6))
                CALL_CHILD(wawawa_attack_event_sub_no_last)
                BREAK_LOOP
            END_IF
        END_IF
    END_LOOP
    CALL(btlevtcmd_AddUnitWork, -2, 2, 1)
    CALL(btlevtcmd_GetPartsWork, -2, LW(15), 15, LW(0))
    CALL(evt_snd_sfxoff, LW(0))
    RETURN
EVT_END()

static EVT_BEGIN(wawawa_attack_event_sub_no_last)
    SET_VALUE(LW(9), PTR(&weapon_wawawa_attack))
    SET_VALUE(LW(10), 0)
    CALL_CHILD(PTR(damage_check_event_sub))
    RETURN
EVT_END()

static EVT_BEGIN(wawawa_attack_event_sub_last)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LW(14), 5)
    LOOP(0)
        CALL(btlevtcmd_GetPartsWork, -2, 1, LW(13), LW(5))
        CALL(btlevtcmd_GetPartsWork, -2, 1, LW(14), LW(6))
        IF_INT_EQUAL(LW(3), LW(5))
            IF_INT_EQUAL(LW(4), LW(6))
                ADD(LW(13), 10)
                CALL(btlevtcmd_GetPartsWork, -2, 1, LW(13), LW(0))
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(13), 1)
        ADD(LW(14), 1)
    END_LOOP
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(wakka_attack_common_event_hit))
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_wawawa_attack))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_wawawa_attack), 256, LW(5))
        CALL(btlevtcmd_SetPartsPos, -2, LW(15), 0, -1000, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_OffPartsAttribute, -2, LW(15), 67108864)
        CALL(btlevtcmd_OnPartsAttribute, -2, LW(15), 16777216)
        ELSE
        SET_VALUE(LW(9), PTR(&weapon_wawawa_attack))
        SET_VALUE(LW(10), 1)
        CALL_CHILD(PTR(damage_check_event_sub))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(_check_satellite)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    SET_VALUE(LW(11), 4)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, LW(11), LW(12))
        IF_INT_NOT_EQUAL(LW(12), -1)
            CALL(btlevtcmd_SetUnitWork, LW(12), 1, 1)
        END_IF
        ADD(LW(11), 1)
    END_LOOP
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(1), 255)
        SET_VALUE(LW(2), 255)
        SET_VALUE(LW(3), 255)
        SET_VALUE(LW(4), -8)
        LABEL(0)
        CALL(_check_satellite, LW(0))
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    LABEL(1)
    CALL(evt_sub_random, 120, LW(0))
    ADD(LW(0), 30)
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_CheckActStatus, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckStatus, -2, 5, LW(0))
        CALL(btlevtcmd_CheckStatus, -2, 3, LW(1))
        CALL(btlevtcmd_CheckStatus, -2, 4, LW(2))
        OR(LW(0), LW(1))
        OR(LW(0), LW(2))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_3"))
            WAIT_FRAMES(180)
            CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
        END_IF
    END_IF
    GOTO(1)
    RETURN
EVT_END()

static u8 barriernSatelliteTo_Init(BattleUnit* unit, s32 n, Vec* table) {
    BarriernSpline* sp;

    if (unit->work[4 + n] == -1) {
        return FALSE;
    }
    sp = BTL_ALLOC(sizeof(BarriernSpline));
    unit->work[10 + n] = (EvtWord)sp;
    sp->n = 5;
    sp->table = table;
    sp->count = 0;
    spline_maketable(5, sp->table, sp->a, sp->b);
    BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[4 + n]), 1)->work[2] = (EvtWord)table;
    return TRUE;
}

static u8 barriernSatelliteTo_Main(BattleUnit* unit, s32 n) {
    BarriernSpline* sp;
    BattleUnit* sat;
    Vec upos;
    Vec pos;

    if (unit->work[4 + n] == -1) {
        return FALSE;
    }
    sp = (BarriernSpline*)unit->work[10 + n];
    if (sp == NULL) {
        return FALSE;
    }
    if (sp->count >= 60) {
        if (sp != NULL) {
            BTL_FREE(sp);
            unit->work[10 + n] = 0;
        }
        return FALSE;
    }
    spline_getvalue(intplGetValue(4, 0.0f, 1.0f, sp->count, 60), &pos, sp->n, sp->table, sp->a, sp->b);
    sat = BattleGetUnitPtr(_battleWorkPointer, unit->work[4 + n]);
    if (sat == NULL) {
        return FALSE;
    }
    BtlUnit_GetPos(unit, &upos.x, &upos.y, &upos.z);
    BtlUnit_SetPos(sat, upos.x + pos.x * unit->faceDirection, upos.y + pos.y, upos.z + pos.z);
    BtlUnit_SetHomePos(sat, upos.x + pos.x * unit->faceDirection, upos.y + pos.y, upos.z + pos.z);
    sp->count++;
    return TRUE;
}

static USER_FUNC(barriernSatelliteToInside) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 num = evtGetValue(event, args[1]);
    BattleUnit* unit;
    u8 moving;
    s32 i;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        for (i = 0; i < 4; i++) {
            if (unit->work[4 + i] != -1) {
                EvtWord table = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[4 + i]), 1)->work[2];

                if (table == (EvtWord)splineTableOut1_1) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn1_1);
                }
                if (table == (EvtWord)splineTableOut2_1) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn2_1);
                }
                if (table == (EvtWord)splineTableOut2_2) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn2_2);
                }
                if (table == (EvtWord)splineTableOut3_1) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn3_1);
                }
                if (table == (EvtWord)splineTableOut3_2) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn3_2);
                }
                if (table == (EvtWord)splineTableOut3_3) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn3_3);
                }
                if (table == (EvtWord)splineTableOut4_1) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn4_1);
                }
                if (table == (EvtWord)splineTableOut4_2) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn4_2);
                }
                if (table == (EvtWord)splineTableOut4_3) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn4_3);
                }
                if (table == (EvtWord)splineTableOut4_4) {
                    barriernSatelliteTo_Init(unit, i, splineTableIn4_4);
                }
            }
        }
    }
    moving = FALSE;
    for (i = 0; i < 4; i++) {
        if (barriernSatelliteTo_Main(unit, i) == TRUE) {
            moving = TRUE;
        }
    }
    return moving == FALSE ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

static USER_FUNC(barriernSatelliteToOutside) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 num = evtGetValue(event, args[1]);
    BattleUnit* unit;
    u8 moving;
    s32 i;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        switch (num) {
        case 1:
            i = 0;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut1_1) == TRUE) {
                    break;
                }
                i++;
            }
            break;
        case 2:
            i = 0;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut2_1) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut2_2) == TRUE) {
                    break;
                }
                i++;
            }
            break;
        case 3:
            i = 0;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut3_1) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut3_2) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut3_3) == TRUE) {
                    break;
                }
                i++;
            }
            break;
        case 4:
            i = 0;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut4_1) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut4_2) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut4_3) == TRUE) {
                    break;
                }
                i++;
            }
            i++;
            while (TRUE) {
                if (barriernSatelliteTo_Init(unit, i, splineTableOut4_4) == TRUE) {
                    break;
                }
                i++;
            }
            break;
        default:
            return EVT_RETURN_DONE;
        }
    }
    moving = FALSE;
    for (i = 0; i < 4; i++) {
        if (barriernSatelliteTo_Main(unit, i) == TRUE) {
            moving = TRUE;
        }
    }
    return moving == FALSE ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

static USER_FUNC(outSplineTableSet) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part;

    switch (evtGetValue(event, args[1])) {
        case 1:
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[4]), 1);
            part->work[2] = (EvtWord)splineTableOut1_1;
            break;
        case 2:
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[4]), 1);
            part->work[2] = (EvtWord)splineTableOut2_1;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[5]), 1);
            part->work[2] = (EvtWord)splineTableOut2_2;
            break;
        case 3:
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[4]), 1);
            part->work[2] = (EvtWord)splineTableOut3_1;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[5]), 1);
            part->work[2] = (EvtWord)splineTableOut3_2;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[6]), 1);
            part->work[2] = (EvtWord)splineTableOut3_3;
            break;
        case 4:
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[4]), 1);
            part->work[2] = (EvtWord)splineTableOut4_1;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[5]), 1);
            part->work[2] = (EvtWord)splineTableOut4_2;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[6]), 1);
            part->work[2] = (EvtWord)splineTableOut4_3;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[7]), 1);
            part->work[2] = (EvtWord)splineTableOut4_4;
            break;
    }
    return EVT_RETURN_DONE;
}

static void outSplineTableSet_Sub(BattleUnit* unit, s32 n, Vec* table) {
    BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[4 + n]), 1)->work[2] = (EvtWord)table;
}

static USER_FUNC(barriern_wawawa_set_target) {
    EvtWord* args = event->args;
    s32 unitIdx[10];
    s32 partIdx[10];
    s32 i;
    s32 num;

    i = 0;
    _battleWorkPointer->weaponTargets.currentTarget = 0;
    barriern_wawawa_get_target(&unitIdx[0], &partIdx[0]);
    while (TRUE) {
        if (unitIdx[i] == -1) {
            num = i;
            break;
        }
        i++;
        barriern_wawawa_get_next_target(&unitIdx[i], &partIdx[i]);
    }
    for (i = 0; i < 5; i++) {
        s32 r = irand(num);
        evtSetValue(event, *args++, unitIdx[r]);
        evtSetValue(event, *args++, partIdx[r]);
    }
    return EVT_RETURN_DONE;
}

static void barriern_wawawa_get_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;
    BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];

    *unitIdx = target->unitIdx;
    *partIdx = target->partIdx;
}

static void barriern_wawawa_get_next_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;

    wt->currentTarget++;
    if (wt->currentTarget < wt->numTargets) {
        BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];
        *unitIdx = target->unitIdx;
        *partIdx = target->partIdx;
    } else {
        *unitIdx = -1;
        *partIdx = 0;
    }
}

static USER_FUNC(_check_satellite) {
    s32 id = BattleTransID(event, -2);
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    EvtWord* args = event->args;
    BattleUnitPart* part;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    BOOL alive;
    s32 j;

    alive = FALSE;
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    for (i = 4; i < 8; i++) {
        if (unit->work[i] != -1) {
            if (BattleGetUnitPtr(_battleWorkPointer, unit->work[i]) != NULL) {
                alive = TRUE;
            } else {
                unit->work[i] = -1;
            }
        }
    }
    if (!alive) {
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        if (part->attributes & kImmuneToDamageAndStatus) {
            effDelete((EffectEntry*)unit->work[3]);
            unit->work[3] = 0;
            BtlUnit_snd_se(unit, "SFX_ENM_BARRI_BARRIER_BREAK1", 0xF1194D80, 0);
            BtlUnit_GetPos(unit, &x, &y, &z);
            for (j = 0; j < 20; j++) {
                s32 w = BtlUnit_GetWidth(unit) + 15;
                s32 h = BtlUnit_GetHeight(unit) + 15;
                s32 dx = irand(w) - w / 2;
                s32 dy = irand(h) - h / 2;

                effGlassN64Entry(2, 60, x + dx, y + dy, z, 1.0f);
            }
        }
        part->attributes &= ~kImmuneToDamageAndStatus;
        evtSetValue(event, args[0], 0);
    } else {
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        if (!(part->attributes & kImmuneToDamageAndStatus)) {
            f32 px;
            f32 py;
            f32 pz;

            BtlUnit_snd_se(unit, "SFX_ENM_BARRI_BARRIER1", 0xF1194D80, 0);
            BtlUnit_GetPos(unit, &px, &py, &pz);
            unit->work[3] = (EvtWord)effSyuryouEntry(1, 1000, 0.0f, -1000.0f, 0.0f);
        }
        part->attributes |= kImmuneToDamageAndStatus;
        evtSetValue(event, args[0], 1);
        if (unit->work[3] != 0) {
            f32* ud = ((EffectEntry*)unit->work[3])->userdata;

            BtlUnit_GetPos(unit, &x, &y, &z);
            ud[1] = x;
            ud[2] = y - 32.0f * unit->mSizeMultiplier;
            ud[3] = 10.0f + z;
            ud[4] = 1.8f * unit->mSizeMultiplier;
        }
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(battleGetBarriernSatelliteInitPos) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 num;
    s32 index;
    s32 id;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 j;
    static const Vec VecData[] = {
        // 1 satellite
        {25.0f, 0.0f, 0.0f},
        // 2 satellites
        {25.0f, 0.0f, 0.0f},
        {-25.0f, 0.0f, 0.0f},
        // 3 satellites
        {0.0f, 25.0f, 0.0f},
        {25.0f, -25.0f, 0.0f},
        {-25.0f, -25.0f, 0.0f},
        // 4 satellites
        {25.0f, 25.0f, 0.0f},
        {-25.0f, 25.0f, 0.0f},
        {25.0f, -25.0f, 0.0f},
        {-25.0f, -25.0f, 0.0f},
    };
    const Vec* p;
    BattleWork* wp = _battleWorkPointer;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    num = evtGetValue(event, args[1]);
    index = evtGetValue(event, args[2]);
    x = args[3];
    y = args[4];
    z = args[5];
    p = VecData;
    for (i = 0; i < num; i++) {
        for (j = 0; j < i; j++) {
            p++;
        }
    }
    for (j = 0; j < index; j++) {
        p++;
    }
    evtSetValue(event, x, (s32)(p->x * unit->faceDirection));
    evtSetValue(event, y, (s32)p->y);
    evtSetValue(event, z, (s32)p->z);
    return EVT_RETURN_DONE;
}
