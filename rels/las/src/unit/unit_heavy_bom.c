// Palace of Shadow battle unit: the Bulky Bob-omb (`heavy_bom`): parts, stats, poses and AI scripts.
// No formation or script of this REL uses it.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_camera.h"
#include "party/party_motion.h"
#include "battle/battle_status_effect.h"

#include "driver/animdrv.h"
#include "string.h"
#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_FUSE_LIT 0   // 1 once attack_event lit it (with BATTLE_UNIT_FLAG_FUSE_LIT_UNUSED); freeze puts it out; phase_event counts down only while 1; _fire_point_callback draws the flame only while 1
#define UNIT_WORK_COUNTDOWN 1  // countdown 3, 2, 1, 0 (phase_event steps it while lit; at 0 the next phase explodes); picks pose tables, G_1-G_4 poses and the flame locator
#define UNIT_WORK_FUSE_SFX 2   // sound id of the looping SFX_ENM_HEAVYB_WAIT1 started when lit; -1 = none; stopped by freeze, death and explosion
#define UNIT_WORK_GUARD_STEP 3 // which guard weapon the next already-lit turn uses: 0 weapon_heavy_bomb_guard, 1 _guard_2, 2 _guard_3 (cycles)

static USER_FUNC(_set_draw_callback);
static void _fire_point_callback(void* userdata, s32 group, Mtx* modeling);

static BattleUnitKind unit_heavy_bom;
static BattleWeapon weapon_heavy_bomb_super_explosion;
static BattleWeapon weapon_heavy_bomb_guard;
static BattleWeapon weapon_heavy_bomb_guard_2;
static BattleWeapon weapon_heavy_bomb_guard_3;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table_3[16];
static PoseTableEntry pose_table_3_fire[16];
static PoseTableEntry pose_table_2[16];
static PoseTableEntry pose_table_2_fire[16];
static PoseTableEntry pose_table_1[16];
static PoseTableEntry pose_table_1_fire[16];
static PoseTableEntry pose_table_0_fire[16];
static PoseTableEntry pose_table_0[16];
static PoseTableEntry pose_table_3_to_2[13];
static PoseTableEntry pose_table_2_to_1[13];
static PoseTableEntry pose_table_1_to_0[13];
static BattleDataEntry data_table[3];
static PoseSoundTiming pose_sound_timing_table[9];
static BattleUnitKindPart parts[2];
static EvtWord init_event[57];
static EvtWord damage_event[118];
static EvtWord phase_event[126];
static EvtWord attack_event[175];
static EvtWord wait_event[6];
static EvtWord dead_event[52];

#if VERSION >= VERSION_US
static EvtWord explosion_event[254];
#else
static EvtWord explosion_event[247];
#endif

/**
 * Unit data of the Bulky Bob-omb; ties together its sounds, parts, status vulnerability, init event
 * and event table.
 */
static BattleUnitKind unit_heavy_bom = {
    UNIT_BULKY_BOB_OMB, // unitType
    "btl_un_heavy_bom", // name
    6, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    22, // level
    0, // bonusExp
    4, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    40, // runRate
    9999, // pbSoftCap
    54, // width
    70, // height
    {7, 70}, // hitOffset
    {0.0f, 35.0f, 0.0f}, // centerOffset
    {0}, // hpGaugeOffset
    {0}, // talkTogeBaseOffset
    {27.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0}, // burnFlameOffset
    0.0f, 0.0f, // vivianOffsetX..vivianOffsetZ
    {27.0f, 0.0f, 0.0f}, // bintaHitOffset
    {27.0f, 45.5f, 0.0f}, // kissHitOffset
    {0.0f, 35.0f, 0.0f}, // cutBaseOffset
    54.0f, // cutWidth
    70.0f, // cutHeight
    0, 0, 0, // turnOrder..swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    0, // itemStealParam
    {0}, // starPointDispOffset
    "SFX_ENM_HEAVYB_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    2, // numParts
    parts, // parts
    init_event, // initEventCode
    (BattleDataEntry*)data_table, // dataTable
};

/**
 * Weapon of the Bulky Bob-omb (super explosion): targets every unit for 2 damage, explosion element;
 * used by explosion_event.
 */
static BattleWeapon weapon_heavy_bomb_super_explosion = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_EXPLOSION, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_BOMB_FLIPPABLE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 50, 50, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Weapon of the Bulky Bob-omb (guard): targets every unit, raises Defense (3 turns); used by attack_event.
 */
static BattleWeapon weapon_heavy_bomb_guard = {
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, // atkChangeTime..atkChangeStrength
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

/**
 * Weapon of the Bulky Bob-omb (guard 2): targets every unit, raises Attack (2 turns in the demos, 3 in retail); used by attack_event.
 */
static BattleWeapon weapon_heavy_bomb_guard_2 = {
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, // burnChance..sizeChangeStrength
    100, // atkChangeChance
#if VERSION <= VERSION_DEMO_US
    2,
#else
    3, // atkChangeTime
#endif
    3, // atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of the Bulky Bob-omb (guard 3): targets every unit; used by attack_event. */
static BattleWeapon weapon_heavy_bomb_guard_3 = {
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
#if VERSION <= VERSION_DEMO_US
    6,
#else
    3, // chargeStrength
#endif
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Defence of the Bulky Bob-omb: 1 against every kind of hit. */
static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

/** Defence attributes of the Bulky Bob-omb: none, every element hits it normally. */
static BattleUnitDefenseAttr defence_attr = {0};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {70, 50, 70, 0, 70, 150, 100, 70, 100, 70, 100, 70, 100, 70, 70, 80, 20, 100, 70, 100, 100, 95};
#else
static StatusVulnerability regist = {
    90, // sleep
    75, // stop
    70, // dizzy
    0, // poison
    70, // confuse
    150, // electric
    100, // burn
    100, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    95, // defenseDown
    70, // allergic
    80, // fright
    20, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};
#endif

/** Poses of the Bulky Bob-omb with its fuse at length 3 and not lit (the full length it starts with). */
static PoseTableEntry pose_table_3[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_DIZZY, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_FREEZE, "Z_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "D_1"},
    {POSE_TYPE_DAMAGE_STATUS, "S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "S_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/** Poses of the Bulky Bob-omb with its fuse at length 3, burning (lit). */
static PoseTableEntry pose_table_3_fire[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_DIZZY, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_FREEZE, "Z_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "D_1"},
    {POSE_TYPE_DAMAGE_STATUS, "S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "S_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/** Poses of the Bulky Bob-omb with its fuse at length 2 and not lit. */
static PoseTableEntry pose_table_2[] = {
    {POSE_TYPE_SLEEP, "N_2"},
    {POSE_TYPE_FREEZE, "Y_2"},
    {POSE_TYPE_STOP, "Y_2"},
    {POSE_TYPE_DIZZY, "K_2"},
    {POSE_TYPE_POISON, "X_2"},
    {POSE_TYPE_CONFUSE, "K_2"},
    {POSE_TYPE_FREEZE, "Z_2B"},
    {POSE_TYPE_NORMAL, "S_2B"},
    {POSE_TYPE_DAMAGE_FIRE, "D_2"},
    {POSE_TYPE_DAMAGE_STATUS, "S_2B"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_2B"},
    {POSE_TYPE_DAMAGE, "D_2"},
    {POSE_TYPE_DANGER, "X_2"},
    {POSE_TYPE_PERIL, "X_2"},
    {POSE_TYPE_TALK, "S_2B"},
    {POSE_TYPE_ACTION, "S_2B"},
};

/** Poses of the Bulky Bob-omb with its fuse at length 2, burning (lit). */
static PoseTableEntry pose_table_2_fire[] = {
    {POSE_TYPE_SLEEP, "N_2"},
    {POSE_TYPE_FREEZE, "Y_2"},
    {POSE_TYPE_STOP, "Y_2"},
    {POSE_TYPE_DIZZY, "K_2"},
    {POSE_TYPE_POISON, "X_2"},
    {POSE_TYPE_CONFUSE, "K_2"},
    {POSE_TYPE_FREEZE, "Z_2A"},
    {POSE_TYPE_NORMAL, "S_2A"},
    {POSE_TYPE_DAMAGE_FIRE, "D_2"},
    {POSE_TYPE_DAMAGE_STATUS, "S_2A"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_2A"},
    {POSE_TYPE_DAMAGE, "D_2"},
    {POSE_TYPE_DANGER, "X_2"},
    {POSE_TYPE_PERIL, "X_2"},
    {POSE_TYPE_TALK, "S_2A"},
    {POSE_TYPE_ACTION, "S_2A"},
};

/** Poses of the Bulky Bob-omb with its fuse at length 1 and not lit. */
static PoseTableEntry pose_table_1[] = {
    {POSE_TYPE_SLEEP, "N_3"},
    {POSE_TYPE_FREEZE, "Y_3"},
    {POSE_TYPE_STOP, "Y_3"},
    {POSE_TYPE_DIZZY, "K_3"},
    {POSE_TYPE_POISON, "X_3"},
    {POSE_TYPE_CONFUSE, "K_3"},
    {POSE_TYPE_FREEZE, "Z_3B"},
    {POSE_TYPE_NORMAL, "S_3B"},
    {POSE_TYPE_DAMAGE_FIRE, "D_3"},
    {POSE_TYPE_DAMAGE_STATUS, "S_3B"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_3B"},
    {POSE_TYPE_DAMAGE, "D_3"},
    {POSE_TYPE_DANGER, "X_3"},
    {POSE_TYPE_PERIL, "X_3"},
    {POSE_TYPE_TALK, "S_3B"},
    {POSE_TYPE_ACTION, "S_3B"},
};

/** Poses of the Bulky Bob-omb with its fuse at length 1, burning (lit). */
static PoseTableEntry pose_table_1_fire[] = {
    {POSE_TYPE_SLEEP, "N_3"},
    {POSE_TYPE_FREEZE, "Y_3"},
    {POSE_TYPE_STOP, "Y_3"},
    {POSE_TYPE_DIZZY, "K_3"},
    {POSE_TYPE_POISON, "X_3"},
    {POSE_TYPE_CONFUSE, "K_3"},
    {POSE_TYPE_FREEZE, "Z_3A"},
    {POSE_TYPE_NORMAL, "S_3A"},
    {POSE_TYPE_DAMAGE_FIRE, "D_3"},
    {POSE_TYPE_DAMAGE_STATUS, "S_3A"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_3A"},
    {POSE_TYPE_DAMAGE, "D_3"},
    {POSE_TYPE_DANGER, "X_3"},
    {POSE_TYPE_PERIL, "X_3"},
    {POSE_TYPE_TALK, "S_3A"},
    {POSE_TYPE_ACTION, "S_3A"},
};

/** Poses of the Bulky Bob-omb with its fuse at length 0, burning (lit). */
static PoseTableEntry pose_table_0_fire[] = {
    {POSE_TYPE_SLEEP, "N_4"},
    {POSE_TYPE_FREEZE, "Y_4"},
    {POSE_TYPE_STOP, "Y_4"},
    {POSE_TYPE_DIZZY, "K_4"},
    {POSE_TYPE_POISON, "X_4"},
    {POSE_TYPE_CONFUSE, "K_4"},
    {POSE_TYPE_FREEZE, "Z_4A"},
    {POSE_TYPE_NORMAL, "S_4A"},
    {POSE_TYPE_DAMAGE_FIRE, "D_4"},
    {POSE_TYPE_DAMAGE_STATUS, "S_4A"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_4A"},
    {POSE_TYPE_DAMAGE, "D_4"},
    {POSE_TYPE_DANGER, "X_4"},
    {POSE_TYPE_PERIL, "X_4"},
    {POSE_TYPE_TALK, "S_4A"},
    {POSE_TYPE_ACTION, "S_4A"},
};

/** Poses of the Bulky Bob-omb with its fuse at length 0 and not lit. */
static PoseTableEntry pose_table_0[] = {
    {POSE_TYPE_SLEEP, "N_4"},
    {POSE_TYPE_FREEZE, "Y_4"},
    {POSE_TYPE_STOP, "Y_4"},
    {POSE_TYPE_DIZZY, "K_4"},
    {POSE_TYPE_POISON, "X_4"},
    {POSE_TYPE_CONFUSE, "K_4"},
    {POSE_TYPE_FREEZE, "Z_4B"},
    {POSE_TYPE_NORMAL, "S_4B"},
    {POSE_TYPE_DAMAGE_FIRE, "D_4"},
    {POSE_TYPE_DAMAGE_STATUS, "S_4B"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_4B"},
    {POSE_TYPE_DAMAGE, "D_4"},
    {POSE_TYPE_DANGER, "X_4"},
    {POSE_TYPE_PERIL, "X_4"},
    {POSE_TYPE_TALK, "S_4B"},
    {POSE_TYPE_ACTION, "S_4B"},
};

/** Poses played while the burning fuse of the Bulky Bob-omb shortens from length 3 to 2. */
static PoseTableEntry pose_table_3_to_2[] = {
    {POSE_TYPE_SLEEP, "B_4"},
    {POSE_TYPE_FREEZE, "B_1"},
    {POSE_TYPE_STOP, "B_1"},
    {POSE_TYPE_DIZZY, "B_7"},
    {POSE_TYPE_POISON, "B_10"},
    {POSE_TYPE_CONFUSE, "B_7"},
    {POSE_TYPE_NORMAL, "B_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "B_1"},
    {POSE_TYPE_DAMAGE, "B_1"},
    {POSE_TYPE_DANGER, "B_10"},
    {POSE_TYPE_PERIL, "B_10"},
    {POSE_TYPE_TALK, "B_1"},
    {POSE_TYPE_ACTION, "B_1"},
};

/** Poses played while the burning fuse of the Bulky Bob-omb shortens from length 2 to 1. */
static PoseTableEntry pose_table_2_to_1[] = {
    {POSE_TYPE_SLEEP, "B_5"},
    {POSE_TYPE_FREEZE, "B_2"},
    {POSE_TYPE_STOP, "B_2"},
    {POSE_TYPE_DIZZY, "B_8"},
    {POSE_TYPE_POISON, "B_11"},
    {POSE_TYPE_CONFUSE, "B_8"},
    {POSE_TYPE_NORMAL, "B_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "B_2"},
    {POSE_TYPE_DAMAGE, "B_2"},
    {POSE_TYPE_DANGER, "B_11"},
    {POSE_TYPE_PERIL, "B_11"},
    {POSE_TYPE_TALK, "B_2"},
    {POSE_TYPE_ACTION, "B_2"},
};

/** Poses played while the burning fuse of the Bulky Bob-omb shortens from length 1 to 0. */
static PoseTableEntry pose_table_1_to_0[] = {
    {POSE_TYPE_SLEEP, "B_6"},
    {POSE_TYPE_FREEZE, "B_3"},
    {POSE_TYPE_STOP, "B_3"},
    {POSE_TYPE_DIZZY, "B_9"},
    {POSE_TYPE_POISON, "B_12"},
    {POSE_TYPE_CONFUSE, "B_9"},
    {POSE_TYPE_NORMAL, "B_3"},
    {POSE_TYPE_DAMAGE_GUARDED, "B_3"},
    {POSE_TYPE_DAMAGE, "B_3"},
    {POSE_TYPE_DANGER, "B_12"},
    {POSE_TYPE_PERIL, "B_12"},
    {POSE_TYPE_TALK, "B_3"},
    {POSE_TYPE_ACTION, "B_3"},
};

/** Event table of the Bulky Bob-omb: dead_event, explosion_event. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_EXPLOSION_EVENT, explosion_event},
    {0},
};

/**
 * Sound cues of the Bulky Bob-omb: SFX_ENM_HEAVYB_WAIT3 plays at fixed points of the animations S_3A, S_4A.
 */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"S_3A", 0.01f, NULL, "SFX_ENM_HEAVYB_WAIT3", 0, 1},
    {"S_3A", 0.26666668f, NULL, "SFX_ENM_HEAVYB_WAIT3", 0, 1},
    {"S_3A", 0.53333336f, NULL, "SFX_ENM_HEAVYB_WAIT3", 0, 1},
    {"S_3A", 0.8f, NULL, "SFX_ENM_HEAVYB_WAIT3", 0, 1},
    {"S_4A", 0.01f, NULL, "SFX_ENM_HEAVYB_WAIT3", 0, 1},
    {"S_4A", 0.16666667f, NULL, "SFX_ENM_HEAVYB_WAIT3", 0, 1},
    {"S_4A", 0.33333334f, NULL, "SFX_ENM_HEAVYB_WAIT3", 0, 1},
    {"S_4A", 0.5f, NULL, "SFX_ENM_HEAVYB_WAIT3", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/** Parts of the Bulky Bob-omb: 2 parts (model c_heavy). */
static BattleUnitKindPart parts[] = {
    {
        1, // partNum
        "btl_un_heavy_bom", // partName
        "c_heavy", // modelName
        {0}, // positionOffset
        {15.0f, 50.0f, 0.0f}, // hitBaseOffset
        {0.0f, 65.0f, 0.0f}, // hitCursorBaseOffset
        65, // shadowSize
        70, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table_3, // poseTable
    },
    {
        2, // partNum
        "ヘビーボム", // partName
        "c_heavy", // modelName
        {0.0f, 80.0f, 0.0f}, // positionOffset
        {0}, // hitBaseOffset
        {0}, // hitCursorBaseOffset
        15, // shadowSize
        15, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        kNotTargetable | kNoShadow, // attributes
        0, // counterAttributes
        pose_table_3, // poseTable
    },
};

/**
 * Init event of the Bulky Bob-omb: installs its wait, phase, attack,
 * damage and confusion events and sets its unit work:
 * 0 = fuse lit, 1 = fuse length left (starts at 3), 2 = the fuse sound handle (-1 for none),
 * 3 = which guard weapon is next. Part 2 gets the fuse flame pose F_1 and the draw callback is set up.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_LIT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_COUNTDOWN, 3)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_SFX, -1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD_STEP, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("F_1"))
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(_set_draw_callback)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FREEZE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_LIT, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_SFX, LW(0))
            CALL(evt_snd_sfxoff, LW(0))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_SFX, -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_LIT, 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_COUNTDOWN, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(2)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_2))
                CASE_EQUAL(1)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_1))
                CASE_EQUAL(0)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_0))
            END_SWITCH
        END_IF
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
        SWITCH(LW(0))
            CASE_OR_EQUAL(27)
            CASE_OR_EQUAL(24)
            CASE_OR_EQUAL(25)
            CASE_OR_EQUAL(52)
            CASE_OR_EQUAL(46)
            CASE_OR_EQUAL(50)
            CASE_OR_EQUAL(54)
            GOTO(50)
            CASE_END
        END_SWITCH
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
    LABEL(50)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE_FIRE)
    CALL_CHILD(PTR(dead_event))
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_AFTER_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_LIT, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_COUNTDOWN, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("K_4"))
                CALL(btlevtcmd_AfterReactionEntry, UNIT_ID_SELF, UNIT_DATA_EXPLOSION_EVENT)
                RETURN
            END_IF
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HEAVYB_WAIT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_COUNTDOWN, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(3)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_3_to_2))
                CASE_EQUAL(2)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_2_to_1))
                CASE_EQUAL(1)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_1_to_0))
            END_SWITCH
            CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
            WAIT_FRAMES(40)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_COUNTDOWN, LW(0))
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_COUNTDOWN, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(2)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_2_fire))
                CASE_EQUAL(1)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_1_fire))
                CASE_EQUAL(0)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_0_fire))
            END_SWITCH
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
    END_IF
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HEAVYB_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_LIT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_COUNTDOWN, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(3)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_1"))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_2"))
            CASE_EQUAL(1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_3"))
            CASE_EQUAL(0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_4"))
        END_SWITCH
        WAIT_FRAMES(40)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HEAVYB_WAIT1"), EVT_NULLPTR, 0, LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_SFX, LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HEAVYB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_LIT, 1)
        CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_FUSE_LIT_UNUSED)
    ELSE
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_COUNTDOWN, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(3)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_1"))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_2"))
            CASE_EQUAL(1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_3"))
            CASE_EQUAL(0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("G_4"))
        END_SWITCH
        WAIT_FRAMES(40)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD_STEP, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(12), PTR(&weapon_heavy_bomb_guard))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD_STEP, 1)
            CASE_EQUAL(1)
            SET_VALUE(LW(12), PTR(&weapon_heavy_bomb_guard_2))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD_STEP, 2)
            CASE_EQUAL(2)
            SET_VALUE(LW(12), PTR(&weapon_heavy_bomb_guard_3))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD_STEP, 0)
        END_SWITCH
        CALL(btlevtcmd_WeaponAftereffect, LW(12))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_SFX, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(evt_snd_sfxoff, LW(0))
    END_IF
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    SWITCH(LW(0))
        CASE_OR_EQUAL(27)
        CASE_OR_EQUAL(24)
        CASE_OR_EQUAL(25)
        CASE_OR_EQUAL(52)
        CASE_OR_EQUAL(46)
        CASE_OR_EQUAL(50)
        CASE_OR_EQUAL(54)
        GOTO(50)
        CASE_END
    END_SWITCH
    LABEL(10)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    CALL_CHILD(PTR(subsetevt_dead_core))
    RETURN
    LABEL(50)
    CALL(btlevtcmd_AfterReactionEntry, UNIT_ID_SELF, UNIT_DATA_EXPLOSION_EVENT)
    RETURN
EVT_END()

static EVT_BEGIN(explosion_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_SFX, LW(0))
    CALL(evt_snd_sfxoff, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FUSE_SFX, -1)
#if VERSION >= VERSION_US
    CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 4)
#endif
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HEAVYB_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
    DIVIDE(LW(5), 2)
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    ADD(LW(1), LW(5))
    CALL(evt_eff, 0, STRING("bomb"), 5, LW(0), LW(1), LW(2), FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_StageDispellFog)
    WAIT_MS(300)
    CALL(evt_eff, 0, STRING("bomb"), 4, -40, 30, 20, FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(125)
    CALL(evt_eff, 0, STRING("bomb"), 4, 40, 90, 20, FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(125)
    CALL(evt_eff, 0, STRING("bomb"), 4, 40, 30, 20, FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(125)
    CALL(evt_eff, 0, STRING("bomb"), 4, -40, 90, 20, FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(125)
    CALL(evt_eff, 0, STRING("bomb"), 4, -100, 60, 20, FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(125)
    CALL(evt_eff, 0, STRING("bomb"), 4, 100, 60, 20, FLOAT(2.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_heavy_bomb_super_explosion))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_heavy_bomb_super_explosion), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_heavy_bomb_super_explosion))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_heavy_bomb_super_explosion))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_heavy_bomb_super_explosion), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), PRECHECK_HIT)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_heavy_bomb_super_explosion), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(99)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    CALL_CHILD(subsetevt_dead_core)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()


static void _fire_point_callback(void* userdata, s32 group, Mtx* modeling) {
    BattleUnitPart* part = userdata;
    BattleUnit* unit = part->owner;
    const char* name;
    s32 poseId;

    if (unit->work[UNIT_WORK_FUSE_LIT] == 0) {
        return;
    }
    poseId = part->poseId;
    switch (unit->work[UNIT_WORK_COUNTDOWN]) {
        case 0:
            name = "locator11";
            break;
        case 1:
            name = "locator10";
            break;
        case 2:
            name = "locator9";
            break;
        case 3:
        default:
            name = "locator8";
            break;
    }
    if (strcmp(name, animPoseGetGroupName(poseId, group)) == 0) {
        s32 shapePose = BtlUnit_GetPartsPtr(unit, 2)->poseId;
        s32 shape = animPoseGetShapeIdx(shapePose, "pPlaneShape3");
        animPoseDrawShape(shapePose, shape);
    }
}


static USER_FUNC(_set_draw_callback) {
    BattleWork* wp = _battleWorkPointer;
    s32 id = BattleTransID(event, UNIT_ID_SELF);
    BattleUnit* unit = BattleGetUnitPtr(wp, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);

    animPoseSetDispCallBack(part->poseId, _fire_point_callback, part);
    BtlUnit_GetPartsPtr(unit, 2)->attributes |= kHidden;
    return EVT_RETURN_DONE;
}

