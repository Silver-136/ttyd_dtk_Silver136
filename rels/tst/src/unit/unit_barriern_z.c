// Battle unit Z-Yux (`barriern_z`): tst.rel's copy of rels/aji/src/unit/unit_barriern_z.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "party/party_motion.h"
#include "evt/evt_cmd.h"

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
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "eff/n64/eff_glass_n64.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "tst.h"
#include "unit/unit_barriern.h"
#include "system.h"
#include "driver/mapdrv.h"
#include "battle/battle_damage.h"

// Unit work slots of the Yux family (btlevtcmd_SetUnitWork / GetUnitWork).
enum {
    UNIT_WORK_SATELLITE_COUNT = 0, // Mini-Yux it should have (battle setup), updated when it splits
    UNIT_WORK_WAWAWA_ATTACK = 1,    // 1 during the group ring attack, 0 during the single ring attack
    UNIT_WORK_SHOTS_DONE = 2,    // rings of the group attack that have landed
    UNIT_WORK_BARRIER_EFFECT = 3,  // barrier effect (EffectEntry*) while a Mini-Yux lives; C only
    UNIT_WORK_SATELLITE = 4,      // unit id of the first Mini-Yux, -1 when the slot is empty
    UW_BARRIERN_Z_MINI_YUX_1 = 5,      // unit id of the second Mini-Yux, or -1
    UW_BARRIERN_Z_MINI_YUX_2 = 6,      // unit id of the third Mini-Yux, or -1
    UW_BARRIERN_Z_MINI_YUX_3 = 7,      // unit id of the fourth Mini-Yux, or -1
    UNIT_WORK_SATELLITE_SPLINE = 10,       // orbit spline work of the first Mini-Yux (SatelliteSpline*); C only
    UW_BARRIERN_Z_SPLINE_1 = 11,       // orbit spline work of the second Mini-Yux; C only
    UW_BARRIERN_Z_SPLINE_2 = 12,       // orbit spline work of the third Mini-Yux; C only
    UW_BARRIERN_Z_SPLINE_3 = 13        // orbit spline work of the fourth Mini-Yux; C only
};

s32 irand(s32 scalar);
static u8 barriernSatelliteTo_Init(BattleUnit* unit, s32 n, Vec* table);
static u8 barriernSatelliteTo_Main(BattleUnit* unit, s32 n);

static s32 zanzou_count[5];
static s32 barriern_satellite_zanzou_sub(BOOL isFirstCall, BattleUnit* unit, f32* scales, s32 n, s32 type);
static s32 barriern_zanzou_sub(BOOL isFirstCall, BattleUnit* unit, f32* scales, s32 type);

static USER_FUNC(_check_satellite);
static USER_FUNC(barriern_satellite_zanzou);
static USER_FUNC(barriern_satellite_zanzou2);
static USER_FUNC(battleGetBarriernSatelliteInitPos);
static USER_FUNC(outSplineTableSet);
static void barriern_wawawa_get_next_target(s32* unitIdx, s32* partIdx);
static void barriern_wawawa_get_target(s32* unitIdx, s32* partIdx);
static USER_FUNC(barriern_wawawa_set_target);
static void outSplineTableSet_Sub(BattleUnit* unit, s32 n, Vec* table);
static USER_FUNC(barriernSatelliteToOutside);
static USER_FUNC(barriernSatelliteToInside);
static u8 barriernSatelliteTo_Main(BattleUnit* unit, s32 n);
static u8 barriernSatelliteTo_Init(BattleUnit* unit, s32 n, Vec* table);
static s32 barriern_satellite_zanzou_sub(BOOL isFirstCall, BattleUnit* unit, f32* scales, s32 n, s32 type);
static s32 barriern_zanzou_sub(BOOL isFirstCall, BattleUnit* unit, f32* scales, s32 type);
static USER_FUNC(barriern_zanzou3);
static USER_FUNC(barriern_zanzou2);
static USER_FUNC(barriern_zanzou);

BattleUnitKind unit_barriern_z;
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
static EvtWord unison_phase_event[190];
static EvtWord attack_event[106];
static EvtWord recovery_event[69];
static EvtWord wakka_attack_event[255];
static EvtWord wawawa_attack_event[548];
static EvtWord wawawa_attack_event_sub[87];
static EvtWord wawawa_attack_event_sub_no_last[10];
static EvtWord wawawa_attack_event_sub_last[93];
static EvtWord wait_event[116];

/**
 * Unit kind of Z-Yux: 7 max HP, level 27. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_barriern_z = {
    115,
    "btl_un_barriern_z",
    7,
    0,
    1,
    1,
    27,
    0,
#if VERSION <= VERSION_DEMO_US
    2,
    70,
    2,
#else
    3,
    70,
    0,
#endif
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
    "SFX_ENM_BARRI_DAMAGED2",
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

/** Defence of Z-Yux per element (normal, fire, ice, explosion, electric), subtracted from each hit: none. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of Z-Yux: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Body parts of Z-Yux: 11 parts (model c_baria_z); the first is the body.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 12.0f, 0.0f, 0.0f, 22.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x600009, 0, pose_table},
    {2, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {3, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {4, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {5, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {6, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {7, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {8, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {9, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {10, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
    {11, "btl_un_barriern_z", "c_baria_z", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 10, 10, 255, &defence, &defence_attr, 0x13010000, 0, pose_table},
};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of Z-Yux (the demos), as the percent chance a status takes hold:
 * immune to Poison, Freeze, Fright and Gale Force; always hit by Electric, Burn, Huge, Attack Up,
 * Defense Up, Fast, Dodgy and Invisible; otherwise Sleep 10, Stop 80, Dizzy 70, Confuse 70, Tiny 70,
 * Attack Down 70, Defense Down 70, Allergic 70, Slow 70 and instant KO 95.
 */
static StatusVulnerability regist = {10, 80, 70, 0, 70, 100, 100, 0, 100, 70, 100, 70, 100, 70, 70, 0, 0, 100, 70, 100, 100, 95};
#else
/**
 * Status vulnerability of Z-Yux (the final versions), as the percent chance a status takes hold:
 * immune to Poison, Freeze and Fright; always hit by Electric, Burn, Huge, Attack Up, Defense Up, Fast,
 * Dodgy and Invisible; otherwise Sleep 20, Stop 70, Dizzy 95, Confuse 60, Tiny 85, Attack Down 85,
 * Defense Down 90, Allergic 60, Gale Force 80, Slow 60 and instant KO 95.
 */
static StatusVulnerability regist = {20, 70, 95, 0, 60, 100, 100, 0, 100, 85, 100, 85, 100, 90, 60, 0, 80, 100, 60, 100, 100, 95};
#endif

/** Attack data of Z-Yux's wakka attack: base damage 4, 100% accuracy. */
static BattleWeapon weapon_wakka_attack = {
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_SHATTERS_FREEZE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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

/** Attack data of Z-Yux's wawawa attack: base damage 2, 100% accuracy. */
static BattleWeapon weapon_wawawa_attack = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_SHATTERS_FREEZE, // special
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

/**
 * Spawn setup for a unit that the Z-Yux script adds to the battle mid-fight:
 * unit kind unit_barriern_z_satellite, parked far off screen until its script moves it.
 */
static BattleUnitSetup entry_barriern_satellite = {
    &unit_barriern_z_satellite, // unitKindParams
    1, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Pose table of Z-Yux: maps its battle pose types (stay, walk, hurt, dead, talk and so on) to animation
 * names, 18 entries such as N_1, Y_1, K_1, X_1.
 */
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
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/**
 * Event table of Z-Yux: maps event tags to the scripts that react to them
 * (barriern_dmg_first_attack_pos); tag 0 ends it. Tag 48 is a First Strike.
 */
static BattleDataEntry data_table[] = {
    {48, barriern_dmg_first_attack_pos},
    {0, NULL},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 1 of 1 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn1_1[] = {
    {25.0f, 0.0f, 0.0f},
    {20.0f, -30.0f, -1.0f},
    {12.5f, -40.0f, -1.0f},
    {5.0f, -30.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 1 of 2 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn2_1[] = {
    {25.0f, 0.0f, 0.0f},
    {20.0f, -30.0f, -1.0f},
    {12.5f, -40.0f, -1.0f},
    {5.0f, -30.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 2 of 2 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn2_2[] = {
    {-25.0f, 0.0f, 0.0f},
    {-20.0f, 30.0f, -1.0f},
    {-12.5f, 30.0f, -1.0f},
    {-5.0f, 30.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 1 of 3 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn3_1[] = {
    {0.0f, 25.0f, 0.0f},
    {0.0f, 45.0f, -1.0f},
    {0.0f, 50.0f, -1.0f},
    {0.0f, 25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 2 of 3 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn3_2[] = {
    {25.0f, -25.0f, 0.0f},
    {40.0f, -40.0f, -1.0f},
    {30.0f, -35.0f, -1.0f},
    {20.0f, -25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 3 of 3 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn3_3[] = {
    {-25.0f, -25.0f, 0.0f},
    {-40.0f, -40.0f, -1.0f},
    {-30.0f, -35.0f, -1.0f},
    {-20.0f, -25.0f, -1.0f},
    {-0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 1 of 4 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn4_1[] = {
    {25.0f, 25.0f, 0.0f},
    {30.0f, 35.0f, -1.0f},
    {40.0f, 45.0f, -1.0f},
    {20.0f, 25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 2 of 4 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn4_2[] = {
    {-25.0f, 25.0f, 0.0f},
    {-30.0f, 35.0f, -1.0f},
    {-40.0f, 45.0f, -1.0f},
    {-20.0f, 25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 3 of 4 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn4_3[] = {
    {25.0f, -25.0f, 0.0f},
    {30.0f, -35.0f, -1.0f},
    {40.0f, -45.0f, -1.0f},
    {20.0f, -25.0f, -1.0f},
    {0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 4 of 4 returns to the Yux from its orbit
 * position.
 */
static Vec splineTableIn4_4[] = {
    {-25.0f, -25.0f, 0.0f},
    {-30.0f, -35.0f, -1.0f},
    {-40.0f, -45.0f, -1.0f},
    {-20.0f, -25.0f, -1.0f},
    {-0.0f, 0.0f, -1.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 1 of 1 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut1_1[] = {
    {0.0f, 0.0f, -1.0f},
    {-35.0f, -35.0f, -1.0f},
    {-40.0f, 0.0f, -1.0f},
    {30.0f, 40.0f, -1.0f},
    {25.0f, 0.0f, 0.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 1 of 2 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut2_1[] = {
    {0.0f, 0.0f, -1.0f},
    {5.0f, 30.0f, -1.0f},
    {12.5f, 40.0f, -1.0f},
    {20.0f, 30.0f, -1.0f},
    {25.0f, 0.0f, 0.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 2 of 2 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut2_2[] = {
    {0.0f, 0.0f, -1.0f},
    {-5.0f, -30.0f, -1.0f},
    {-12.5f, -40.0f, -1.0f},
    {-20.0f, -30.0f, -1.0f},
    {-25.0f, 0.0f, 0.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 1 of 3 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut3_1[] = {
    {0.0f, 0.0f, -1.0f},
    {0.0f, 25.0f, -1.0f},
    {0.0f, 50.0f, -1.0f},
    {0.0f, 45.0f, -1.0f},
    {0.0f, 25.0f, 0.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 2 of 3 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut3_2[] = {
    {0.0f, 0.0f, -1.0f},
    {25.0f, -20.0f, -1.0f},
    {45.0f, -30.0f, -1.0f},
    {30.0f, -25.0f, -1.0f},
    {25.0f, -25.0f, 0.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 3 of 3 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut3_3[] = {
    {0.0f, 0.0f, -1.0f},
    {-25.0f, -20.0f, -1.0f},
    {-45.0f, -30.0f, -1.0f},
    {-30.0f, -25.0f, -1.0f},
    {-25.0f, -25.0f, 0.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 1 of 4 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut4_1[] = {
    {0.0f, 0.0f, -1.0f},
    {25.0f, 20.0f, -1.0f},
    {45.0f, 30.0f, -1.0f},
    {30.0f, 25.0f, -1.0f},
    {25.0f, 25.0f, 0.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 2 of 4 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut4_2[] = {
    {0.0f, 0.0f, -1.0f},
    {-25.0f, 20.0f, -1.0f},
    {-45.0f, 30.0f, -1.0f},
    {-30.0f, 25.0f, -1.0f},
    {-25.0f, 25.0f, 0.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 3 of 4 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut4_3[] = {
    {0.0f, 0.0f, -1.0f},
    {25.0f, -20.0f, -1.0f},
    {45.0f, -30.0f, -1.0f},
    {30.0f, -25.0f, -1.0f},
    {25.0f, -25.0f, 0.0f},
};

/**
 * Spline path, as offsets from the Yux, along which satellite 4 of 4 moves out from the Yux to its orbit
 * position.
 */
static Vec splineTableOut4_4[] = {
    {0.0f, 0.0f, -1.0f},
    {-25.0f, -20.0f, -1.0f},
    {-45.0f, -30.0f, -1.0f},
    {-30.0f, -25.0f, -1.0f},
    {-25.0f, -25.0f, 0.0f},
};

/**
 * Init script of a Yux (also used by the X-Yux and Z-Yux):
 * registers its wait, unison-phase, attack, damage and confusion handlers, shrinks its body part to 0.8,
 * empties its four satellite slots (UNIT_WORK_SATELLITE to _3 = -1)
 * and spawns as many Mini-Yux satellites as UNIT_WORK_SATELLITE_COUNT asks for.
 * Each satellite gets the Yux's unit id in its unit work 0 and 1 in its unit work 1,
 * and is placed around the Yux (battleGetBarriernSatelliteInitPos) before the satellite orbit spline is set up.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventUnisonPhase, UNIT_ID_SELF, PTR(unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, 1, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_FLOAT_CAPABLE | BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    SET_VALUE(LW(0), UNIT_WORK_SATELLITE)
    LOOP(4)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(0), -1)
        ADD(LW(0), 1)
    END_LOOP
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SATELLITE_COUNT, LW(7))
    LABEL(0)
    IF_INT_LESS(LW(6), LW(7))
        CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(5))
        SET_VALUE(LW(8), LW(6))
        ADD(LW(8), UNIT_WORK_SATELLITE)
        CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_barriern_satellite), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(8), LW(3))
        CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(5))
        CALL(btlevtcmd_SetUnitWork, LW(3), 1, 1)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(battleGetBarriernSatelliteInitPos, UNIT_ID_SELF, LW(7), LW(6), LW(10), LW(11), LW(12))
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
    CALL(outSplineTableSet, UNIT_ID_SELF, LW(7))
    RETURN
EVT_END()

/** Damage reaction of a Yux: resets its colour to white (SetRGB 255) and runs the default damage handling. */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetRGB, UNIT_ID_SELF, 1, 255, 255, 255)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Position event for a first strike on the Yux: sets its height to 20 and waits for the attack to finish. */
static EVT_BEGIN(barriern_dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 20)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * Turns on the afterimage blur (SetPartsBlur) and kMotionBlur on the body part of each Mini-Yux
 * satellite (UNIT_WORK_SATELLITE to _3) so their movement leaves trails.
 */
static EVT_BEGIN(satellite_blur_on)
    SET_VALUE(LW(2), UNIT_WORK_SATELLITE)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(2), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_SetPartsBlur, LW(3), LW(4), 255, 255, 255, 255, 255, 255, 255, 100, 0)
            CALL(btlevtcmd_OnPartsAttribute, LW(3), LW(4), kMotionBlur)
        END_IF
        ADD(LW(2), 1)
    END_LOOP
    RETURN
EVT_END()

/** Turns kMotionBlur off again on the body part of each Mini-Yux satellite (UNIT_WORK_SATELLITE to _3). */
static EVT_BEGIN(satellite_blur_off)
    SET_VALUE(LW(2), UNIT_WORK_SATELLITE)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(2), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
            CALL(btlevtcmd_OffPartsAttribute, LW(3), LW(4), kMotionBlur)
        END_IF
        ADD(LW(2), 1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Script helper called every frame: the Yux's charge-up afterimage ('zanzou').
 * The five afterimage parts (2-6) start large and faint and shrink onto the body over 36 frames while fading in;
 * returns DONE at the end.
 */
static USER_FUNC(barriern_zanzou) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    return barriern_zanzou_sub(isFirstCall, unit, scales, 0);
}

/**
 * Script helper called every frame: the Yux's burst afterimage ('zanzou').
 * The five afterimage parts (2-6) swell outward from the body over 8 frames while fading out;
 * returns DONE at the end.
 */
static USER_FUNC(barriern_zanzou2) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    return barriern_zanzou_sub(isFirstCall, unit, scales, 1);
}

/** Script helper called every frame: like barriern_zanzou2, but the outward swell and fade lasts 36 frames. */
static USER_FUNC(barriern_zanzou3) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    return barriern_zanzou_sub(isFirstCall, unit, scales, 2);
}

/** Interpolates between two floats with intplGetValue, using the given easing mode, current frame and total frames. */
static inline f32 intpl_ff(f32 start, f32 end, s32 mode, s32 cur, s32 total) {
    return intplGetValue(mode, start, end, cur, total);
}

/**
 * Shared body of the Yux's afterimage effects.
 * type 0: parts 2-6 shrink in from large and transparent to the body over 36 frames;
 * type 1: they swell out and fade over 8 frames; type 2: the same over 36 frames.
 * Frames are counted in zanzou_count; returns BLOCK until finished, then hides the parts and returns DONE.
 */
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
            part->attributes &= ~PART_ATTR_HIDDEN;
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
            BtlUnit_GetPartsPtr(unit, i)->attributes |= PART_ATTR_HIDDEN;
        }
        return EVT_RETURN_DONE;
    }
    zanzou_count[0]++;
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper called every frame: the afterimage charge-up of the Mini-Yux in slot args[1] of the Yux args[0]
 * (looked up in work[UNIT_WORK_SATELLITE + n]); see barriern_satellite_zanzou_sub (type 0).
 */
static USER_FUNC(barriern_satellite_zanzou) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 n = evtGetValue(event, args[1]);
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = BattleGetUnitPtr(wp, id);
    BattleUnit* sat;
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    sat = BattleGetUnitPtr(wp, unit->work[UNIT_WORK_SATELLITE + n]);
    return barriern_satellite_zanzou_sub(isFirstCall, sat, scales, n, 0);
}

/**
 * Script helper called every frame: the outward-burst afterimage of the Mini-Yux in slot args[1] of the Yux
 * args[0]; see barriern_satellite_zanzou_sub (type 1).
 */
static USER_FUNC(barriern_satellite_zanzou2) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 n = evtGetValue(event, args[1]);
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = BattleGetUnitPtr(wp, id);
    BattleUnit* sat;
    f32 scales[5] = {1.7f, 2.4f, 3.1f, 3.8f, 4.5f};

    sat = BattleGetUnitPtr(wp, unit->work[UNIT_WORK_SATELLITE + n]);
    return barriern_satellite_zanzou_sub(isFirstCall, sat, scales, n, 1);
}

/**
 * Afterimage effect of a Mini-Yux (parts 2-6, animation Z_2),
 * like barriern_zanzou_sub but with a separate frame counter per satellite n.
 * type 0 converges over 36 frames, type 1 bursts out over 8 frames.
 */
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
            part->attributes &= ~PART_ATTR_HIDDEN;
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
            BtlUnit_GetPartsPtr(unit, i)->attributes |= PART_ATTR_HIDDEN;
        }
        return EVT_RETURN_DONE;
    }
    zanzou_count[(s32)(n + 1)]++;
    return EVT_RETURN_BLOCK;
}

/**
 * Start-of-turn event of the Yux (not on turn 1 and only while it can act):
 * after a short random delay, if it has fewer than two satellites,
 * the satellites merge into it (SFX_ENM_BARRI_ATTACK9 and SPLIT1) and it spawns one new Mini-Yux,
 * then the satellites spread out again around it (SFX_ENM_BARRI_SPLIT2) with their afterimage blur on;
 * UNIT_WORK_SATELLITE_COUNT is updated to the new satellite count.
 */
static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(evt_sub_random, 10, LW(0))
    WAIT_FRAMES(LW(0))
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), UNIT_WORK_SATELLITE)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            ADD(LW(0), 1)
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    IF_INT_GREATER_OR_EQUAL(LW(0), 4)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK9"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriern_zanzou3, UNIT_ID_SELF)
    CALL_CHILD(satellite_blur_on)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SATELLITE_COUNT, LW(1))
    IF_INT_GREATER_OR_EQUAL(LW(1), 1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_SPLIT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    CALL(barriernSatelliteToInside, UNIT_ID_SELF, LW(1))
    CALL_CHILD(satellite_blur_off)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(4))
    SET_VALUE(LW(1), UNIT_WORK_SATELLITE)
    SET_VALUE(LW(5), 0)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(2))
        IF_INT_EQUAL(LW(2), -1)
            CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_barriern_satellite), 0)
            CALL(btlevtcmd_AddPos, LW(3), 0, 0, -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, LW(1), LW(3))
            CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(4))
            ADD(LW(0), 1)
            ADD(LW(5), 1)
            IF_INT_GREATER_OR_EQUAL(LW(5), 1)
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL_CHILD(satellite_blur_on)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_SPLIT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriernSatelliteToOutside, UNIT_ID_SELF, LW(0))
    WAIT_FRAMES(60)
    CALL_CHILD(satellite_blur_off)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SATELLITE_COUNT, LW(0))
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Chooses the Z-Yux's attack: on a first strike (battle flag 2) it always makes the ring attack (wakka_attack_event);
 * otherwise it uses an item if it has one, then counts its four satellite slots holding a Mini-Z-Yux that can act
 * and is not confused: with four it makes the group attack (wawawa_attack_event), otherwise a 70/30 pick between
 * the single ring attack and healing itself (recovery_event).
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetRGB, UNIT_ID_SELF, 1, 255, 255, 255)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(wakka_attack_event)
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(_check_satellite)
    SET_VALUE(LW(0), 0)
    SET_VALUE(LW(1), UNIT_WORK_SATELLITE)
    LOOP(4)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(1), LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_CheckActStatus, LW(3), LW(2))
            IF_INT_NOT_EQUAL(LW(2), 0)
                CALL(btlevtcmd_CheckStatus, LW(3), STATUS_CONFUSE, LW(2))
                IF_INT_EQUAL(LW(2), 0)
                    ADD(LW(0), 1)
                END_IF
            END_IF
        END_IF
        ADD(LW(1), 1)
    END_LOOP
    IF_INT_EQUAL(LW(0), 4)
        CALL_CHILD(wawawa_attack_event)
    ELSE
        SET_VALUE(LW(0), 70)
        ADD(LW(0), 30)
        SUBTRACT(LW(0), 1)
        CALL(evt_sub_random, LW(0), LW(1))
        IF_INT_LESS(LW(1), 70)
            CALL_CHILD(wakka_attack_event)
        ELSE
            CALL_CHILD(recovery_event)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * The Yux heals itself: zooms the camera onto it, shows the S_3 pose for 120 frames,
 * restores 1 HP (RecoverHp) and plays the recovery effect above it.
 */
static EVT_BEGIN(recovery_event)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_3"))
    WAIT_FRAMES(120)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, 1, 2)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    ADD(LW(6), 30)
    ADD(LW(7), 10)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(5), LW(6), LW(7), 2, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(30)
    RETURN
EVT_END()

/**
 * Starts the target's avoid animation for the result of a pre-damage check in LW(5) (3, 6 or 2 select the avoid
 * types 38, 39 and 40).
 */
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

/**
 * The part of the Yux's ring attack when the shot misses:
 * the attacking part (LW(15)) takes the Z_3 pose
 * and is launched at the target (its PART_ATTR_HIDDEN cleared while it flies),
 * the Yux plays A_5 and the avoid animation starts for the target;
 * afterwards UNIT_WORK_WAWAWA_ATTACK decides whether the part is hidden again.
 * The code name wakka is Japanese for "ring".
 */
static EVT_BEGIN(wakka_attack_common_event_no_hit)
    // The ring part is set up on the Yux: faded out when the satellites are out, then placed on the Yux in its Z_3 pose.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_WAWAWA_ATTACK, LW(6))
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), kMotionBlur)
    ELSE
        CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, LW(15), 255, 255, 255, 255, 255, 255, 255, 100, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), kMotionBlur)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(15), STRING("Z_3"))
    CALL(btlevtcmd_GetPos, LW(12), LW(6), LW(7), LW(8))
    ADD(LW(8), 1)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_CheckPartsAttribute, UNIT_ID_SELF, LW(15), PART_ATTR_HIDDEN, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), PART_ATTR_HIDDEN)
    END_IF

    // The Yux plays its A_5 pose, and a watcher starts the target's dodge as soon as the ring flies past it.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_5"))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_LESS_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            ELSE
                CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(15), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_GREATER_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT

    // Without satellites out, the ring swells and shrinks as it hops to the far end of the stage past the target.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_WAWAWA_ATTACK, LW(6))
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
        CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
        MULTIPLY(LW(6), 1)
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(6), LW(0))
        SUBTRACT(LW(7), 5)
        CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, LW(15), 1, 0, 1)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                LOOP(10)
                    CALL(btlevtcmd_AddPartsScale, UNIT_ID_SELF, LW(15), 0, FLOAT(0.125f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                LOOP(10)
                    CALL(btlevtcmd_AddPartsScale, UNIT_ID_SELF, LW(15), 0, FLOAT(-0.125f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 0, 4, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8), 0, -1)
    ELSE
        // With satellites out, the ring pulses faster, hops past the target and is then hidden below the stage.
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
        CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
        MULTIPLY(LW(6), 1)
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(6), LW(0))
        SUBTRACT(LW(7), 5)
        CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, LW(15), 1, 0, 1)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                LOOP(5)
                    CALL(btlevtcmd_AddPartsScale, UNIT_ID_SELF, LW(15), 0, FLOAT(0.25f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                LOOP(5)
                    CALL(btlevtcmd_AddPartsScale, UNIT_ID_SELF, LW(15), 0, FLOAT(-0.25f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 0, 7, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8), 0, -1)
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), 0, -1000, 0)
    END_IF
    RETURN
EVT_END()

/**
 * The part of the Yux's ring attack when the shot hits:
 * the attacking part (LW(15)) is flung at the target in the Z_3 pose and returns (A_5 pose);
 * UNIT_WORK_WAWAWA_ATTACK decides whether the part stays visible.
 * The code name wakka is Japanese for "ring".
 */
static EVT_BEGIN(wakka_attack_common_event_hit)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_WAWAWA_ATTACK, LW(6))
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), kMotionBlur)
    ELSE
        CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, LW(15), 255, 255, 255, 255, 255, 255, 255, 100, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), kMotionBlur)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, LW(15), STRING("Z_3"))
    CALL(btlevtcmd_GetPos, LW(12), LW(6), LW(7), LW(8))
    ADD(LW(8), 1)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8))
    CALL(btlevtcmd_CheckPartsAttribute, UNIT_ID_SELF, LW(15), PART_ATTR_HIDDEN, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), PART_ATTR_HIDDEN)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_5"))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_WAWAWA_ATTACK, LW(6))
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, LW(15), 1, 0, 1)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                LOOP(10)
                    CALL(btlevtcmd_AddPartsScale, UNIT_ID_SELF, LW(15), 0, FLOAT(0.125f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                LOOP(10)
                    CALL(btlevtcmd_AddPartsScale, UNIT_ID_SELF, LW(15), 0, FLOAT(-0.125f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 0, 4, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8), 0, -1)
    ELSE
        CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, LW(15), 1, 0, 1)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                LOOP(5)
                    CALL(btlevtcmd_AddPartsScale, UNIT_ID_SELF, LW(15), 0, FLOAT(0.25f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
                LOOP(5)
                    CALL(btlevtcmd_AddPartsScale, UNIT_ID_SELF, LW(15), 0, FLOAT(-0.25f), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, LW(15), 0, 7, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(15), LW(6), LW(7), LW(8), 0, -1)
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), 0, -1000, 0)
    END_IF
    RETURN
EVT_END()

/**
 * Resolves one Yux shot against the target in LW(3) and LW(4) with the weapon in LW(9):
 * pre-damage check, a miss runs wakka_attack_common_event_no_hit,
 * a hit runs wakka_attack_common_event_hit and applies the damage (with the action- command defence when LW(10)
 * is 1).
 */
static EVT_BEGIN(damage_check_event_sub)
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
    END_SWITCH
    LABEL(90)
    CALL_CHILD(wakka_attack_common_event_no_hit)
    GOTO(98)
    LABEL(91)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LW(14), 5)
    LOOP(0)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, LW(13), LW(5))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, LW(14), LW(6))
        IF_INT_EQUAL(LW(3), LW(5))
            IF_INT_EQUAL(LW(4), LW(6))
                ADD(LW(13), 10)
                CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, LW(13), 1)
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(13), 1)
        ADD(LW(14), 1)
    END_LOOP
    CALL_CHILD(wakka_attack_common_event_hit)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    IF_INT_EQUAL(LW(10), 1)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 256, LW(5))
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    END_IF
    GOTO(98)
    LABEL(98)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), 0, -1000, 0)
    RETURN
EVT_END()

/**
 * The Yux's single ring attack with weapon_wakka_attack:
 * it picks a target, spins (SFX_ENM_BARRI_ATTACK1 to 9, poses A_1 to A_4) and sends its ring at the target,
 * resolved by damage_check_event_sub; UNIT_WORK_WAWAWA_ATTACK is 0 for this attack.
 */
static EVT_BEGIN(wakka_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_wakka_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_wakka_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_WAWAWA_ATTACK, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 0, LW(3))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 5, LW(4))
    SET_VALUE(LW(9), PTR(&weapon_wakka_attack))
    SET_VALUE(LW(10), 1)
    SET_VALUE(LW(12), -2)
    SET_VALUE(LW(15), 7)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_wakka_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_wakka_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(14))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(14))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriern_zanzou, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK9"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriern_zanzou2, UNIT_ID_SELF)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(0), 6)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(14))
    MULTIPLY(LW(0), LW(14))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
    END_LOOP
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3"))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK7"), EVT_NULLPTR, 0, LW(0))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 7, 15, LW(0))
    CALL_CHILD(damage_check_event_sub)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 7, 15, LW(0))
    CALL(evt_snd_sfxoff, LW(0))
    SET_VALUE(LW(0), -6)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(14))
    MULTIPLY(LW(0), LW(14))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
    END_LOOP
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Starts the afterimage effects (barriern_satellite_zanzou and zanzou2) for the Yux's satellites during the
 * wawawa attack.
 */
static EVT_BEGIN(wawawa_attack_event_satellite_blur)
    CALL(barriern_satellite_zanzou, UNIT_ID_SELF, LW(0))
    CALL(barriern_satellite_zanzou2, UNIT_ID_SELF, LW(0))
    RETURN
EVT_END()

/**
 * The Yux's group attack with weapon_wawawa_attack:
 * it and its satellites strike a pose (F_1/F_2),
 * spin in unison (SFX_ENM_BARRI_ATTACK1 to 10) and every satellite sends its ring at the target in turn,
 * handing each shot to wawawa_attack_event_sub (UNIT_WORK_SHOTS_DONE counts the shots);
 * afterwards the satellites go back to their orbit.
 */
static EVT_BEGIN(wawawa_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // The Yux picks its targets; with nothing to hit it ends the attack (or flusters when confused).
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_wawawa_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_wawawa_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_WAWAWA_ATTACK, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHOTS_DONE, 0)

    // Five targets are picked at random from the target list, one per ring, and stored.
    CALL(barriern_wawawa_set_target, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 0, LW(0))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 1, LW(2))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 2, LW(4))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 3, LW(6))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 4, LW(8))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 5, LW(1))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 6, LW(3))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 7, LW(5))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 8, LW(7))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 9, LW(9))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 10, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 11, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 12, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 13, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 1, 14, 0)

    // The camera zooms in on the Yux, the attack is declared on every target and its cost is paid.
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_wawawa_attack))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_wawawa_attack))
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, 0, LW(3))
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, 5, LW(4))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // The Yux and its four satellites start to spin with afterimages, one satellite after another.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(0), 0)
    LOOP(4)
        SPAWN_EVENT(wawawa_attack_event_satellite_blur)
        ADD(LW(0), 1)
        WAIT_FRAMES(10)
    END_LOOP
    CALL(barriern_zanzou, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK10"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(barriern_zanzou2, UNIT_ID_SELF)

    // The Yux and the satellites strike their F_1 and F_2 poses.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("F_1"))
    WAIT_FRAMES(20)
    SET_VALUE(LW(11), UNIT_WORK_SATELLITE)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(11), LW(12))
        CALL(btlevtcmd_GetBodyId, LW(12), LW(13))
        CALL(btlevtcmd_AnimeChangePose, LW(12), LW(13), STRING("F_2"))
        ADD(LW(11), 1)
    END_LOOP
    WAIT_FRAMES(20)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(11), UNIT_WORK_SATELLITE)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(11), LW(12))
        CALL(btlevtcmd_SetRotate, LW(12), 0, 0, 0)
        ADD(LW(11), 1)
    END_LOOP
    SET_VALUE(LW(0), 6)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(14))
    MULTIPLY(LW(0), LW(14))

    // The Yux and its satellites all turn together for 10 frames.
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
        SET_VALUE(LW(11), UNIT_WORK_SATELLITE)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(11), 8)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(11), LW(12))
            CALL(btlevtcmd_AddRotate, LW(12), 0, LW(0), 0)
            ADD(LW(11), 1)
        END_LOOP
    END_LOOP
    SET_VALUE(LW(11), UNIT_WORK_SATELLITE)

    // The satellites settle into their S_2 poses.
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(11), LW(12))
        CALL(btlevtcmd_GetBodyId, LW(12), LW(13))
        CALL(btlevtcmd_AnimeChangePose, LW(12), LW(13), STRING("S_2"))
        ADD(LW(11), 1)
    END_LOOP

    // The Yux winds up through its A_1 to A_4 poses while the camera pulls back.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3"))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4"))
    SET_VALUE(LW(12), -2)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LW(14), 5)
    SET_VALUE(LW(15), 7)

    // The Yux fires its ring first, then each satellite in turn.
    SPAWN_EVENT(wawawa_attack_event_sub)
    WAIT_FRAMES(30)
    SET_VALUE(LW(11), UNIT_WORK_SATELLITE)
    ADD(LW(13), 1)
    ADD(LW(14), 1)
    ADD(LW(15), 1)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(11), LW(12))
        SPAWN_EVENT(wawawa_attack_event_sub)
        WAIT_FRAMES(20)
        ADD(LW(11), 1)
        ADD(LW(13), 1)
        ADD(LW(14), 1)
        ADD(LW(15), 1)
    END_LOOP

    // Waits until every ring has landed.
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SHOTS_DONE, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 5)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(10)

    // The barrier is rechecked, then the Yux and its satellites turn back the other way.
    CALL(_check_satellite)
    SET_VALUE(LW(0), -6)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(14))
    MULTIPLY(LW(0), LW(14))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
        SET_VALUE(LW(11), UNIT_WORK_SATELLITE)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(11), 8)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(11), LW(12))
            CALL(btlevtcmd_AddRotate, LW(12), 0, LW(0), 0)
            ADD(LW(11), 1)
        END_LOOP
    END_LOOP
    LABEL(99)

    // The Yux faces forward again and goes back to waiting.
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * One ring of the wawawa attack: records the target, plays SFX_ENM_BARRI_ATTACK4 and 8,
 * and walks through the stored targets calling the no-last variant for every target
 * but the last (the last one gets wawawa_attack_event_sub_last), then counts the shot in UNIT_WORK_SHOTS_DONE.
 */
static EVT_BEGIN(wawawa_attack_event_sub)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, LW(13), LW(3))
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, LW(14), LW(4))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BARRI_ATTACK8"), EVT_NULLPTR, 0, LW(0))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(0))
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(13), 3)
            CALL_CHILD(wawawa_attack_event_sub_last)
            BREAK_LOOP
        END_IF
        ADD(LW(13), 1)
        ADD(LW(14), 1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, LW(13), LW(5))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, LW(14), LW(6))
        IF_INT_EQUAL(LW(3), LW(5))
            IF_INT_EQUAL(LW(4), LW(6))
                CALL_CHILD(wawawa_attack_event_sub_no_last)
                BREAK_LOOP
            END_IF
        END_IF
    END_LOOP
    CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, UNIT_WORK_SHOTS_DONE, 1)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(15), 15, LW(0))
    CALL(evt_snd_sfxoff, LW(0))
    RETURN
EVT_END()

/**
 * A wawawa ring that is not the last one: resolves its shot with weapon_wawawa_attack through
 * damage_check_event_sub (LW(10) = 0).
 */
static EVT_BEGIN(wawawa_attack_event_sub_no_last)
    SET_VALUE(LW(9), PTR(&weapon_wawawa_attack))
    SET_VALUE(LW(10), 0)
    CALL_CHILD(damage_check_event_sub)
    RETURN
EVT_END()

/**
 * The last ring of the wawawa attack: a hit runs wakka_attack_common_event_hit and applies ResultACDefence
 * and CheckDamage with weapon_wawawa_attack, then hides the ring part and makes it targetable again;
 * otherwise it resolves the shot with damage_check_event_sub (LW(10) = 1).
 */
static EVT_BEGIN(wawawa_attack_event_sub_last)
    SET_VALUE(LW(13), 0)
    SET_VALUE(LW(14), 5)
    LOOP(0)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, LW(13), LW(5))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, LW(14), LW(6))
        IF_INT_EQUAL(LW(3), LW(5))
            IF_INT_EQUAL(LW(4), LW(6))
                ADD(LW(13), 10)
                CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, 1, LW(13), LW(0))
                BREAK_LOOP
            END_IF
        END_IF
        ADD(LW(13), 1)
        ADD(LW(14), 1)
    END_LOOP
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(wakka_attack_common_event_hit)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_wawawa_attack))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_wawawa_attack), 256, LW(5))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(15), 0, -1000, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(15), kMotionBlur)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(15), PART_ATTR_HIDDEN)
    ELSE
        SET_VALUE(LW(9), PTR(&weapon_wawawa_attack))
        SET_VALUE(LW(10), 1)
        CALL_CHILD(damage_check_event_sub)
    END_IF
    RETURN
EVT_END()

/**
 * Idle event of the Yux: checks its satellites (_check_satellite)
 * and keeps checking them every frame in a sibling event, sets its idle pose,
 * tells each satellite to start idling (unit work 1 = 1),
 * and at random intervals of 30 to 150 frames holds the S_3 pose for 180 frames,
 * as long as it can act and is not asleep, stopped or dizzy.
 */
static EVT_BEGIN(wait_event)
    CALL(_check_satellite)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    SET_VALUE(LW(11), UNIT_WORK_SATELLITE)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(11), 8)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(11), LW(12))
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
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(0))
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DIZZY, LW(1))
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_POISON, LW(2))
        OR(LW(0), LW(1))
        OR(LW(0), LW(2))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_3"))
            WAIT_FRAMES(180)
            CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
        END_IF
    END_IF
    GOTO(1)
    RETURN
EVT_END()

/**
 * Starts a satellite's flight along a spline: allocates the SatelliteSpline (5 points, 60 frames) in the Yux's
 * work[UNIT_WORK_SATELLITE_SPLINE + n], builds its tables and remembers the table in the satellite's body part.
 * Returns FALSE when slot n has no satellite.
 */
static u8 barriernSatelliteTo_Init(BattleUnit* unit, s32 n, Vec* table) {
    SatelliteSpline* sp;

    if (unit->work[UNIT_WORK_SATELLITE + n] == -1) {
        return FALSE;
    }
    sp = BTL_ALLOC(sizeof(SatelliteSpline));
    unit->work[UNIT_WORK_SATELLITE_SPLINE + n] = (EvtWord)sp;
    sp->numPoints = 5;
    sp->points = table;
    sp->frame = 0;
    spline_maketable(5, sp->points, sp->knots, sp->curvature);
    BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE + n]), 1)->work[2] = (EvtWord)table;
    return TRUE;
}

/**
 * Moves satellite n one frame along its spline:
 * samples it at frame / 60, places the satellite (and its home position) relative to the Yux with x mirrored by
 * facing direction, and frees the spline after 60 frames. Returns TRUE while the satellite is still moving.
 */
static u8 barriernSatelliteTo_Main(BattleUnit* unit, s32 n) {
    SatelliteSpline* sp;
    BattleUnit* sat;
    Vec upos;
    Vec pos;

    if (unit->work[UNIT_WORK_SATELLITE + n] == -1) {
        return FALSE;
    }
    sp = (SatelliteSpline*)unit->work[UNIT_WORK_SATELLITE_SPLINE + n];
    if (sp == NULL) {
        return FALSE;
    }
    if (sp->frame >= 60) {
        if (sp != NULL) {
            BTL_FREE(sp);
            unit->work[UNIT_WORK_SATELLITE_SPLINE + n] = 0;
        }
        return FALSE;
    }
    spline_getvalue(&pos, intplGetValue(4, 0.0f, 1.0f, sp->frame, 60), sp->numPoints, sp->points, sp->knots, sp->curvature);
    sat = BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE + n]);
    if (sat == NULL) {
        return FALSE;
    }
    BtlUnit_GetPos(unit, &upos.x, &upos.y, &upos.z);
    BtlUnit_SetPos(sat, upos.x + pos.x * unit->faceDirection, upos.y + pos.y, upos.z + pos.z);
    BtlUnit_SetHomePos(sat, upos.x + pos.x * unit->faceDirection, upos.y + pos.y, upos.z + pos.z);
    sp->frame++;
    return TRUE;
}

/**
 * Script helper: sends each satellite of the Yux args[0] from its outer orbit spline to the matching inner one
 * (each outer table has an In counterpart). Blocks until every satellite has arrived.
 */
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
            if (unit->work[UNIT_WORK_SATELLITE + i] != -1) {
                EvtWord table = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE + i]), 1)->work[2];

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

/**
 * Script helper: sends the Yux's args[1] satellites (1-4) from their inner positions out to their outer orbit
 * splines, picking the spline set by the satellite count. Blocks until every satellite has arrived.
 */
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

/**
 * Script helper: tells each of the Yux's args[1] satellites (1-4) which outer orbit spline it sits on,
 * by storing the table in the satellite's body part; no movement.
 */
static USER_FUNC(outSplineTableSet) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part;

    switch (evtGetValue(event, args[1])) {
        case 1:
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE]), 1);
            part->work[2] = (EvtWord)splineTableOut1_1;
            break;
        case 2:
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE]), 1);
            part->work[2] = (EvtWord)splineTableOut2_1;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UW_BARRIERN_Z_MINI_YUX_1]), 1);
            part->work[2] = (EvtWord)splineTableOut2_2;
            break;
        case 3:
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE]), 1);
            part->work[2] = (EvtWord)splineTableOut3_1;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UW_BARRIERN_Z_MINI_YUX_1]), 1);
            part->work[2] = (EvtWord)splineTableOut3_2;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UW_BARRIERN_Z_MINI_YUX_2]), 1);
            part->work[2] = (EvtWord)splineTableOut3_3;
            break;
        case 4:
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE]), 1);
            part->work[2] = (EvtWord)splineTableOut4_1;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UW_BARRIERN_Z_MINI_YUX_1]), 1);
            part->work[2] = (EvtWord)splineTableOut4_2;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UW_BARRIERN_Z_MINI_YUX_2]), 1);
            part->work[2] = (EvtWord)splineTableOut4_3;
            part = BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UW_BARRIERN_Z_MINI_YUX_3]), 1);
            part->work[2] = (EvtWord)splineTableOut4_4;
            break;
    }
    return EVT_RETURN_DONE;
}

/** Stores an orbit spline table pointer in the work[2] of the body part of satellite n. */
static void outSplineTableSet_Sub(BattleUnit* unit, s32 n, Vec* table) {
    BtlUnit_GetPartsPtr(BattleGetUnitPtr(_battleWorkPointer, unit->work[UNIT_WORK_SATELLITE + n]), 1)->work[2] = (EvtWord)table;
}

/**
 * Script helper: picks five random targets from the weapon's current target list
 * and writes them as (unit, part) pairs to args, one for each hit of the Yux's multi-hit attack.
 */
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

/** Returns the unit and part index of the weapon target currently selected in weaponTargets. */
static void barriern_wawawa_get_target(s32* unitIdx, s32* partIdx) {
    BattleWorkWeaponTargets* wt = &_battleWorkPointer->weaponTargets;
    BattleWorkTarget* target = &wt->targets[wt->targetIndices[wt->currentTarget]];

    *unitIdx = target->unitIdx;
    *partIdx = target->partIdx;
}

/** Advances to the next weapon target and returns its unit and part index, or (-1, 0) after the last one. */
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

/**
 * Script helper run from the Yux's wait event.
 * Drops satellites (UNIT_WORK_SATELLITE to _3) that no longer exist;
 * with none left the barrier breaks (the body loses its damage immunity,
 * the barrier effect in UNIT_WORK_BARRIER_EFFECT is deleted and glass shards fly) and args[0] receives 0.
 * While satellites live it raises the barrier (immune body part, barrier effect kept under the Yux)
 * and args[0] receives 1.
 */
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
#if VERSION >= VERSION_JP
        TST_ASSERTMSG("unit_barriern_main.c", 1927, 0, "起動中の待機イベントの主はすでに存在しない\n");
#endif
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
            effDelete((EffectEntry*)unit->work[UNIT_WORK_BARRIER_EFFECT]);
            unit->work[UNIT_WORK_BARRIER_EFFECT] = 0;
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
            unit->work[UNIT_WORK_BARRIER_EFFECT] = (EvtWord)effSyuryouEntry(1, 1000, 0.0f, -1000.0f, 0.0f);
        }
        part->attributes |= kImmuneToDamageAndStatus;
        evtSetValue(event, args[0], 1);
        if (unit->work[UNIT_WORK_BARRIER_EFFECT] != 0) {
            f32* ud = ((EffectEntry*)unit->work[UNIT_WORK_BARRIER_EFFECT])->userdata;

            BtlUnit_GetPos(unit, &x, &y, &z);
            ud[1] = x;
            ud[2] = y - 32.0f * unit->sizeMultiplier;
            ud[3] = 10.0f + z;
            ud[4] = 1.8f * unit->sizeMultiplier;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] is the Yux, args[1] the number of satellites (1-4),
 * args[2] the index of this satellite; args[3]-args[5] receive its starting offset from the Yux (a fixed
 * formation of +-25 units, x mirrored by facing direction).
 */
static USER_FUNC(battleGetBarriernSatelliteInitPos) {
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 max;
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
    max = evtGetValue(event, args[1]);
    index = evtGetValue(event, args[2]);
    x = args[3];
    y = args[4];
    z = args[5];
#if VERSION >= VERSION_JP
    TST_ASSERTMSG("unit_barriern_common_main.c", 44, max<5, "衛星の数が多すぎ\n");
#endif
    p = VecData;
    for (i = 0; i < max; i++) {
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
