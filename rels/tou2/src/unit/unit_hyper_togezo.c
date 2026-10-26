#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "party/party_motion.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_GUARD 0      // 1 while in the guard stance (guard_event and the end of normal_attack_event set it; guard_cancel_event clears it); attack_event then cancels the guard
#define UNIT_WORK_SPINNING 1   // 1 keeps the sibling loop that rolls it during normal_attack_event turning; 0 ends it
#define UNIT_WORK_SPIN_SPEED 2 // degrees per frame of that roll (+-36, or +-18 slowing by 1 every 2-3 frames; times the face direction)
#define UNIT_WORK_CHARGED 3    // 1 after charge_event; the next attack_event clears it and always uses normal_attack_event

USER_FUNC(btlevtcmd_SetJumpSound);

BattleUnitKind unit_hyper_togezo;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitDefense defence_turn;
static BattleUnitDefenseAttr defence_attr_turn;
static StatusVulnerability regist_turn;
static BattleUnitDefense defence_guard;
static BattleUnitDefenseAttr defence_attr_guard;
static StatusVulnerability regist_guard;
static PoseTableEntry pose_table[15];
static PoseTableEntry pose_table_turn[14];
static PoseTableEntry pose_table_guard[12];
static BattleDataEntry data_table[5];
static BattleDataEntry data_table_turn[3];
static BattleDataEntry data_table_guard[4];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_togezo_attack;
static BattleWeapon weapon_togezo_charge;
static EvtWord init_event[52];
static EvtWord dmg_avoid_counter_thorn_event[61];
static EvtWord damage_event[10];
static EvtWord dmg_turn_event[84];
static EvtWord wakeup_event[90];
static EvtWord attack_event[109];
static EvtWord normal_attack_event[813];
static EvtWord guard_event[95];
static EvtWord guard_cancel_event[89];
static EvtWord charge_event[102];
static EvtWord wait_event[6];

BattleUnitKind unit_hyper_togezo = {
    UNIT_SKY_BLUE_SPINY, // mUnitType
    "btl_un_hyper_togezo", // name
    6, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    22, // mLevel
    0, // mBonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    32, // width
    30, // height
    {4, 30}, // hitOffset
    {0.0f, 15.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {16.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {16.0f, 0.0f, 0.0f}, // bintaHitOffset
    {12.0f, 16.0f, 0.0f}, // kissHitOffset
    {0.0f, 15.0f, 0.0f}, // cutBaseOffset
    32.0f, // cutWidth
    30.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_TOGEZO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{4, 4, 4, 4, 4}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    60, // sleep
    70, // stop
    60, // dizzy
    100, // poison
    60, // confuse
    100, // electric
    100, // burn
    50, // freeze
    100, // huge
    85, // tiny
    100, // attackUp
    85, // attackDown
    100, // defenseUp
    90, // defenseDown
    70, // allergic
    80, // fright
    10, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    50, // ohko
};

static BattleUnitDefense defence_turn = {0};

static BattleUnitDefenseAttr defence_attr_turn = {0};

static StatusVulnerability regist_turn = {
    100, // sleep
    100, // stop
    100, // dizzy
    100, // poison
    100, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    100, // tiny
    100, // attackUp
    100, // attackDown
    100, // defenseUp
    100, // defenseDown
    100, // allergic
    100, // fright
    100, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    100, // ohko
};

static BattleUnitDefense defence_guard = {{99, 99, 99, 99, 99}};

static BattleUnitDefenseAttr defence_attr_guard = {{2, 2, 2, 2, 2}};

static StatusVulnerability regist_guard = {
    100, // sleep
    100, // stop
    100, // dizzy
    100, // poison
    100, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    100, // tiny
    100, // attackUp
    100, // attackDown
    100, // defenseUp
    100, // defenseDown
    100, // allergic
    100, // fright
    100, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    100, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "TGZ_N_1"},
    {POSE_TYPE_STOP, "TGZ_Y_1"},
    {POSE_TYPE_FREEZE, "TGZ_Y_1"},
    {POSE_TYPE_CONFUSE, "TGZ_K_1"},
    {POSE_TYPE_POISON, "TGZ_X_1"},
    {POSE_TYPE_DIZZY, "TGZ_X_1"},
    {POSE_TYPE_NORMAL, "TGZ_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "TGZ_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "TGZ_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "TGZ_A_1"},
    {POSE_TYPE_DAMAGE, "TGZ_D_1"},
    {POSE_TYPE_WALK, "TGZ_W_1"},
    {POSE_TYPE_RUN, "TGZ_R_1"},
    {POSE_TYPE_TALK, "TGZ_T_1"},
    {POSE_TYPE_ACTION, "TGZ_S_1"},
};

static PoseTableEntry pose_table_turn[] = {
    {POSE_TYPE_SLEEP, "TGZ_N_2"},
    {POSE_TYPE_STOP, "TGZ_Y_2"},
    {POSE_TYPE_FREEZE, "TGZ_Y_2"},
    {POSE_TYPE_CONFUSE, "TGZ_K_2"},
    {POSE_TYPE_POISON, "TGZ_X_2"},
    {POSE_TYPE_DIZZY, "TGZ_X_2"},
    {POSE_TYPE_NORMAL, "TGZ_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "TGZ_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "TGZ_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "TGZ_S_1"},
    {POSE_TYPE_DAMAGE, "TGZ_D_3"},
    {POSE_TYPE_WALK, "TGZ_W_1"},
    {POSE_TYPE_RUN, "TGZ_R_1"},
    {POSE_TYPE_ACTION, "TGZ_S_2"},
};

static PoseTableEntry pose_table_guard[] = {
    {POSE_TYPE_SLEEP, "TGZ_P_1"},
    {POSE_TYPE_STOP, "TGZ_P_1"},
    {POSE_TYPE_FREEZE, "TGZ_P_1"},
    {POSE_TYPE_CONFUSE, "TGZ_P_1"},
    {POSE_TYPE_POISON, "TGZ_P_1"},
    {POSE_TYPE_DIZZY, "TGZ_P_1"},
    {POSE_TYPE_NORMAL, "TGZ_P_1"},
    {POSE_TYPE_DAMAGE_FIRE, "TGZ_P_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "TGZ_P_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "TGZ_P_1"},
    {POSE_TYPE_DAMAGE, "TGZ_P_1"},
    {POSE_TYPE_ACTION, "TGZ_P_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_event},
    {UNIT_DATA_AVOID_THORN_EVENT, dmg_avoid_counter_thorn_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleDataEntry data_table_turn[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

static BattleDataEntry data_table_guard[] = {
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_event},
    {UNIT_DATA_AVOID_THORN_EVENT, dmg_avoid_counter_thorn_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_hyper_togezo", "c_togezo_b", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kIsShelled, kCounterTopSpiky | kCounterFrontSpiky, pose_table},
};

static BattleWeapon weapon_togezo_attack = {
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
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
    0x2004, // targetWeightFlags
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

static BattleWeapon weapon_togezo_charge = {
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
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    6, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGED, 0)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_MOVE1L"), STRING("SFX_ENM_TOGEZO_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_MOVE1L"), STRING("SFX_ENM_TOGEZO_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_JUMP1"), STRING("SFX_ENM_KURI_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_avoid_counter_thorn_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 45)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 25)
        GOTO(90)
    END_IF
    ADD(LW(1), 35)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_turn))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_OffPartsCounterAttribute, LW(10), LW(11), kCounterTopSpiky)
    CALL(btlevtcmd_OffPartsCounterAttribute, LW(10), LW(11), kCounterFrontSpiky)
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table_turn))
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    CALL(evt_snd_sfxon, STRING("SFX_CONDITION_INSIDE"), 0)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, LW(10))
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_OnPartsCounterAttribute, LW(10), LW(11), kCounterTopSpiky)
    CALL(btlevtcmd_OnPartsCounterAttribute, LW(10), LW(11), kCounterFrontSpiky)
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_ACTION)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartWaitEvent, LW(10))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(wakeup_event))
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(guard_cancel_event))
        GOTO(99)
        ELSE
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGED, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGED, 0)
            CALL_CHILD(PTR(normal_attack_event))
            GOTO(99)
        END_IF
        CALL(btlevtcmd_get_turn, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_DrawLots, LW(0), 3, 40, 40, 20)
            ELSE
            CALL(btlevtcmd_DrawLots, LW(0), 3, 40, 40, 6)
        END_IF
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL_CHILD(PTR(normal_attack_event))
            CASE_EQUAL(1)
            CALL_CHILD(PTR(guard_event))
            CASE_EQUAL(2)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHARGED, 1)
            SET_VALUE(LW(9), PTR(&weapon_togezo_charge))
            CALL_CHILD(PTR(charge_event))
        END_SWITCH
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_togezo_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_togezo_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_togezo_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_togezo_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(14))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(14))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_TURN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
    LOOP(5)
        SET_VALUE(LW(0), -36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_P_1"))
    LOOP(5)
        SET_VALUE(LW(0), -36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPINNING, 1)
    SET_VALUE(LW(0), -36)
    MULTIPLY(LW(0), LW(14))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(0))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPINNING, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(0))
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(10)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 30, 0, FLOAT(0.14941406f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(9.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togezo_attack), 0, LW(5))
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), -18)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(0))
        SET_VALUE(LW(0), 1)
        MULTIPLY(LW(0), LW(14))
        LOOP(18)
            CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(0))
            WAIT_FRAMES(3)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 50)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 26)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPINNING, 0)
    WAIT_FRAMES(10)
    GOTO(97)
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_togezo_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togezo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SET_VALUE(LW(0), 36)
    MULTIPLY(LW(0), LW(14))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, -36)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    ADD(LW(2), 10)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 30, 0, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    ADD(LW(2), 5)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 15, 0, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 15, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), 18)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(0))
        SET_VALUE(LW(0), -1)
        MULTIPLY(LW(0), LW(14))
        LOOP(18)
            CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(0))
            WAIT_FRAMES(2)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 18)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 9)
    ADD(LW(2), 5)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPINNING, 0)
    WAIT_FRAMES(3)
    CALL(evt_sub_random, 99, LW(0))
    IF_INT_LESS(LW(0), 50)
        GOTO(98)
    ELSE
        GOTO(97)
    END_IF
    LABEL(97)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_TURN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
    LOOP(5)
        SET_VALUE(LW(0), 36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_S_1"))
    LOOP(5)
        SET_VALUE(LW(0), 36)
        MULTIPLY(LW(0), LW(14))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_S_1"))
    WAIT_MS(250)
    GOTO(99)
    LABEL(98)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_ROLL3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPINNING, 1)
    SET_VALUE(LW(0), 36)
    MULTIPLY(LW(0), LW(14))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(0))
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPINNING, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(0))
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_NONE, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPINNING, 0)
    CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence_guard))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&defence_attr_guard))
    CALL(btlevtcmd_SetRegistStatus, UNIT_ID_SELF, PTR(&regist_guard))
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_guard))
    CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_guard))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kIsShelled)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GUARD, 1)
    WAIT_MS(250)
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(guard_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_TURN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
    LOOP(5)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_P_1"))
    LOOP(5)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_guard))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr_guard))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist_guard))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_guard))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table_guard))
    CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), kIsShelled)
    CALL(btlevtcmd_SetUnitWork, LW(10), UNIT_WORK_GUARD, 1)
    RETURN
EVT_END()

static EVT_BEGIN(guard_cancel_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEZO_TURN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
    LOOP(5)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGZ_S_1"))
    LOOP(5)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -36)
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(5)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_SetPartsDefenceAttrTable, LW(10), LW(11), PTR(&defence_attr))
    CALL(btlevtcmd_SetRegistStatus, LW(10), PTR(&regist))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_OnPartsAttribute, LW(10), LW(11), kIsShelled)
    CALL(btlevtcmd_SetUnitWork, LW(10), UNIT_WORK_GUARD, 0)
    RETURN
EVT_END()

static EVT_BEGIN(charge_event)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    WAIT_FRAMES(40)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 15)
    ADD(LW(2), 10)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_eff64, STRING(""), STRING("crystal_n64"), 7, LW(0), LW(1), LW(2), FLOAT(1.5f), 60, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    BEGIN_INLINE_EVENT
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, 0, FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        WAIT_MS(500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    END_INLINE_EVENT
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

