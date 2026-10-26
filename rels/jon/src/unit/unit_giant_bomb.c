#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_camera.h"
#include "party/party_motion.h"
#include "battle/battle_status_effect.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "driver/animdrv.h"
#include "string.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"

static USER_FUNC(_set_draw_callback);
#ifdef VERSION_G8ME01
#endif
USER_FUNC(btlevtcmd_AfterReactionEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
static void _fire_point_callback(BattleUnitPart* part, s32 group);

BattleUnitKind unit_giant_bomb;
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
#ifdef VERSION_G8ME01
static EvtWord explosion_event[254];
#else
static EvtWord explosion_event[247];
#endif

void animPoseSetDispCallBack(s32 poseId, void (*callback)(BattleUnitPart*, s32), void* userdata);

static USER_FUNC(_set_draw_callback) {
    BattleWork* wp = _battleWorkPointer;
    s32 id = BattleTransID(event, -2);
    BattleUnit* unit = BattleGetUnitPtr(wp, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);

    animPoseSetDispCallBack(part->poseId, _fire_point_callback, part);
    BtlUnit_GetPartsPtr(unit, 2)->attributes |= kHidden;
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_giant_bomb = {
    UNIT_BOB_ULK, // mUnitType
    "btl_un_giant_bomb", // name
    10, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    34, // mLevel
    2, // mBonusExp
    5, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    54, // width
    70, // height
    {7, 70}, // hitOffset
    {0.0f, 35.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {27.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {27.0f, 0.0f, 0.0f}, // bintaHitOffset
    {0.0f, 0.0f, 0.0f}, // kissHitOffset
    {0.0f, 35.0f, 0.0f}, // cutBaseOffset
    54.0f, // cutWidth
    70.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_HEAVYB_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    2, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleWeapon weapon_heavy_bomb_super_explosion = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    4, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 50, 50, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_heavy_bomb_guard = {
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
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_heavy_bomb_guard_2 = {
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
    3, // atkChangeTime
    3, // atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_heavy_bomb_guard_3 = {
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
    9, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleUnitDefense defence = {{2, 2, 2, 2, 2}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    70, // sleep
    55, // stop
    50, // dizzy
    100, // poison
    50, // confuse
    100, // electric
    100, // burn
    90, // freeze
    100, // huge
    70, // tiny
    100, // attackUp
    70, // attackDown
    100, // defenseUp
    85, // defenseDown
    50, // allergic
    60, // fright
    10, // galeForce
    100, // fast
    50, // slow
    100, // dodgy
    100, // invisible
    20, // ohko
};

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

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_EXPLOSION_EVENT, explosion_event},
    {0, NULL},
};

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

static BattleUnitKindPart parts[] = {
    {1, "btl_un_giant_bomb", "c_giant", 0.0f, 0.0f, 0.0f, 15.0f, 50.0f, 0.0f, 0.0f, 65.0f, 0.0f, 65, 70, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table_3},
    {2, /*"ジャイアントボム"*/"\x83\x57\x83\x83\x83\x43\x83\x41\x83\x93\x83\x67\x83\x7B\x83\x80", "c_giant", 0.0f, 80.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 15, 15, 255, &defence, &defence_attr, kNotTargetable | kNoShadow, 0, pose_table_3},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 3)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, -1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("F_1"))
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(_set_draw_callback)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FREEZE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(0))
            CALL(evt_snd_sfxoff, LW(0))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
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
    CALL(btlevtcmd_CheckDamageCode, LW(10), 256, LW(0))
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
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("K_4"))
                CALL(btlevtcmd_AfterReactionEntry, UNIT_ID_SELF, 52)
                RETURN
            END_IF
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HEAVYB_WAIT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
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
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, LW(0))
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
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
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
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HEAVYB_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 1)
        CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, 16)
    ELSE
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
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
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 3, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(12), PTR(&weapon_heavy_bomb_guard))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 1)
            CASE_EQUAL(1)
            SET_VALUE(LW(12), PTR(&weapon_heavy_bomb_guard_2))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 2)
            CASE_EQUAL(2)
            SET_VALUE(LW(12), PTR(&weapon_heavy_bomb_guard_3))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
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
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(0))
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
    SET_VALUE(LW(10), -2)
    CALL_CHILD(PTR(subsetevt_dead_core))
    RETURN
    LABEL(50)
    CALL(btlevtcmd_AfterReactionEntry, UNIT_ID_SELF, 52)
    RETURN
EVT_END()

static EVT_BEGIN(explosion_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(0))
    CALL(evt_snd_sfxoff, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, -1)
#ifdef VERSION_G8ME01
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
    IF_INT_EQUAL(LW(5), 1)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_heavy_bomb_super_explosion), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(99)
    SET_VALUE(LW(10), -2)
    CALL_CHILD(subsetevt_dead_core)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()


static void _fire_point_callback(BattleUnitPart* part, s32 group) {
    BattleUnit* unit = part->mOwner;
    const char* name;
    s32 poseId;

    if (unit->work[0] == 0) {
        return;
    }
    poseId = part->poseId;
    switch (unit->work[1]) {
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

