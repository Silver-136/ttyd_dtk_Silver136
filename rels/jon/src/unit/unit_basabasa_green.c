#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_START_FLYING 0 // 1 (from the setup, all jon setups): init_event runs ceil_sky_event so it starts flying instead of hanging from the ceiling


BattleUnitKind unit_basabasa_green;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleWeapon weapon_basabasa_drain_attack_biribiri_counterd;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_ceil[18];
static BattleDataEntry data_table[14];
static BattleUnitKindPart parts[1];
static StatusVulnerability regist;
static BattleWeapon weapon_basabasa_attack;
static PoseSoundTiming pose_sound_timing_table[9];
static EvtWord ceil_sky_event[98];
static EvtWord init_event[78];
static EvtWord dmg_first_attack_pos[12];
static EvtWord damage_event[10];
static EvtWord damage_event_ceil[51];
static EvtWord counter_damage_event[16];
static EvtWord ceil_fall_ready_event[23];
static EvtWord attack_event[683];
static EvtWord wait_event[6];

BattleUnitKind unit_basabasa_green = {
    UNIT_SWAMPIRE, // mUnitType
    "btl_un_basabasa_green", // name
    20, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    35, // mLevel
    2, // mBonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    2, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    32, // width
    32, // height
    {6, 32}, // hitOffset
    {0.0f, 16.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {16.0f, 0.0f, -15.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {16.0f, 0.0f, 0.0f}, // bintaHitOffset
    {16.0f, 20.8f, 0.0f}, // kissHitOffset
    {0.0f, 16.0f, 0.0f}, // cutBaseOffset
    32.0f, // cutWidth
    32.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_BASA_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kCeiling | kUnitLeader | kUnitFaceCamera, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static BattleWeapon weapon_basabasa_drain_attack_biribiri_counterd = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, 0, 0, // acHelpMsg..counterResistFlags
    0x2000, // targetWeightFlags
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

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "BSA_N_1"},
    {POSE_TYPE_STOP, "BSA_Y_1"},
    {POSE_TYPE_FREEZE, "BSA_Y_1"},
    {POSE_TYPE_CONFUSE, "BSA_K_1"},
    {POSE_TYPE_POISON, "BSA_X_1"},
    {POSE_TYPE_DIZZY, "BSA_X_1"},
    {POSE_TYPE_NORMAL, "BSA_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "BSA_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "BSA_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "BSA_S_1"},
    {POSE_TYPE_DAMAGE, "BSA_D_1"},
    {POSE_TYPE_ATTACK, "BSA_A_1"},
    {POSE_TYPE_RUN, "BSA_R_1"},
    {POSE_TYPE_WALK, "BSA_W_1"},
    {POSE_TYPE_DANGER, "BSA_X_1"},
    {POSE_TYPE_PERIL, "BSA_X_1"},
    {POSE_TYPE_TALK, "BSA_T_1"},
    {POSE_TYPE_ACTION, "BSA_S_1"},
};

static PoseTableEntry pose_table_ceil[] = {
    {POSE_TYPE_SLEEP, "BSA_N_2"},
    {POSE_TYPE_STOP, "BSA_Y_2"},
    {POSE_TYPE_FREEZE, "BSA_Y_2"},
    {POSE_TYPE_CONFUSE, "BSA_K_2"},
    {POSE_TYPE_POISON, "BSA_X_2"},
    {POSE_TYPE_DIZZY, "BSA_X_2"},
    {POSE_TYPE_NORMAL, "BSA_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "BSA_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "BSA_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "BSA_S_2"},
    {POSE_TYPE_DAMAGE, "BSA_D_2"},
    {POSE_TYPE_ATTACK, "BSA_A_1"},
    {POSE_TYPE_RUN, "BSA_S_1"},
    {POSE_TYPE_WALK, "BSA_S_1"},
    {POSE_TYPE_DANGER, "BSA_X_2"},
    {POSE_TYPE_PERIL, "BSA_X_2"},
    {POSE_TYPE_TALK, "BSA_T_2"},
    {POSE_TYPE_ACTION, "BSA_S_2"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, counter_damage_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_basabasa_green", "c_basabasa_g", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table_ceil},
};

static StatusVulnerability regist = {
    50, // sleep
    55, // stop
    90, // dizzy
    100, // poison
    50, // confuse
    100, // electric
    100, // burn
    50, // freeze
    100, // huge
    75, // tiny
    100, // attackUp
    75, // attackDown
    100, // defenseUp
    80, // defenseDown
    50, // allergic
    50, // fright
    75, // galeForce
    100, // fast
    50, // slow
    100, // dodgy
    100, // invisible
    20, // ohko
};

static BattleWeapon weapon_basabasa_attack = {
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
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    0, // counterResistFlags
    0x80002000, // targetWeightFlags
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

static PoseSoundTiming pose_sound_timing_table[] = {
    {"BSA_S_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_W_1", 0.01f, NULL, "SFX_ENM_BASA_MOVE1", 0, 1},
    {"BSA_R_1", 0.01f, NULL, "SFX_ENM_BASA_MOVE1", 0, 1},
    {"BSA_T_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_A_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_N_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_K_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {"BSA_X_1", 0.01f, NULL, "SFX_ENM_BASA_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

static EVT_BEGIN(ceil_sky_event)
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kCeiling)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kFloating)
    CALL(btlevtcmd_SetPossessionItemOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetEventCeilFall, UNIT_ID_SELF, 0)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 0, -30, 0)
    CALL(btlevtcmd_AddHomePos, UNIT_ID_SELF, 0, -30, 0)
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, 30, 0)
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 16, 21, 0)
    CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 6, 32)
    CALL(btlevtcmd_AddHomePos, UNIT_ID_SELF, 0, -40, 0)
    LABEL(99)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_ceil))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventCeilFall, UNIT_ID_SELF, PTR(ceil_fall_ready_event))
    CALL(btlevtcmd_SetPossessionItemOffset, UNIT_ID_SELF, 0, -20, 0)
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, -50, 0)
    CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 0, -30, 0)
    CALL(btlevtcmd_SetStatusIconOffset, UNIT_ID_SELF, 6, 10)
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 16, -18, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_START_FLYING, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(ceil_sky_event))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_first_attack_pos)
    CALL_CHILD(PTR(ceil_sky_event))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 10, LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_ceil)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetTotalDamage, LW(10), LW(12))
    IF_INT_EQUAL(LW(12), 0)
        RETURN
    END_IF
    CALL_CHILD(PTR(ceil_sky_event))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

static EVT_BEGIN(counter_damage_event)
    CALL_CHILD(PTR(ceil_sky_event))
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(PTR(subsetevt_counter_damage))
    RETURN
EVT_END()

static EVT_BEGIN(ceil_fall_ready_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL_CHILD(PTR(ceil_sky_event))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_basabasa_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 0, -30, 0)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.5f))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        SUBTRACT(LW(1), 40)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), -6)
    LOOP(2)
        ADD(LW(1), 13)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, 10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
        ADD(LW(1), -13)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, -10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    END_LOOP
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    SUBTRACT(LW(1), 10)
    SUBTRACT(LW(2), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.5f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    ADD(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(200)
    GOTO(98)
    LABEL(91)
    SET_VALUE(LF(0), 0)
    CALL(btlevtcmd_PreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), 256, LW(5))
    IF_INT_EQUAL(LW(5), 17)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LW(5), 14)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_basabasa_attack))
        SET_VALUE(LW(9), PTR(&weapon_basabasa_drain_attack_biribiri_counterd))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, -45)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_BASA_SUCK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BSA_A_2"))
    LOOP(2)
        LOOP(10)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, 0, FLOAT(0.049804688f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(10)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, 0, FLOAT(-0.049804688f), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_LOOP
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_basabasa_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_basabasa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(2), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_LINEAR, 0, MOVE_SOUND_NONE)
    CALL(btlevtcmd_GetDamage, LW(3), LW(10))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BSA_N_1"))
        WAIT_MS(300)
        CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, 1, LW(10))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        ADD(LW(6), 20)
        ADD(LW(7), 10)
        CALL(evt_eff, STRING(""), STRING("recovery"), 7, LW(5), LW(6), LW(7), LW(10), 0, 0, 0, 0, 0, 0, 0)
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(0), 5)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.099609375f), FLOAT(0.099609375f), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(0), 10)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.099609375f), FLOAT(-0.099609375f), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(0), 0)
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(0), 5)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.099609375f), FLOAT(0.099609375f), 0)
            ADD(LW(0), 1)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, 1, 1, 1)
        WAIT_MS(1000)
    END_IF
    WAIT_MS(500)
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(7))
    IF_INT_EQUAL(LW(7), 1)
        SUBTRACT(LW(1), 30)
    END_IF
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    LABEL(99)
    CALL(btlevtcmd_CheckAttribute, UNIT_ID_SELF, kCeiling, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BSA_S_2"))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPos, UNIT_ID_SELF, 0, 30, 0)
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("BSA_S_1"))
        WAIT_FRAMES(1)
    END_IF
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

