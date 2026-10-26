#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "evt/evt_msg.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"


BattleUnitKind unit_boss_kanbu2;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_kanbu2_attack;
static EvtWord init_event[37];
static EvtWord damage_event[10];
static EvtWord dead_event[26];
static EvtWord attack_event[24];
static EvtWord wait_event[6];
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_guard[18];
static EvtWord normal_attack_event[365];

BattleUnitKind unit_boss_kanbu2 = {
    UNIT_UNUSED_CRUMP2, // mUnitType
    "btl_un_kanbu2", // name
    15, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    1, // mLevel
    10, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    56, // width
    59, // height
    {56, 59}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 0.0f, 0.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {21.0f, 0.0f, 0.0f}, // bintaHitOffset
    {28.0f, 38.35f, 0.0f}, // kissHitOffset
    {0.0f, 26.0f, 0.0f}, // cutBaseOffset
    42.0f, // cutWidth
    52.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BTL_DAMAGE1", // damageSfxName
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

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
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
    0, // fright
    0, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    0, // ohko
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_kanbu2", "c_kanbu", 0.0f, 0.0f, 0.0f, 0.0f, 45.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

BattleWeapon weapon_kanbu2_attack = {
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
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    0, // counterResistFlags
    4, // targetWeightFlags
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

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_MOVE1L"), STRING("SFX_BOSS_KANBU_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_MOVE1L"), STRING("SFX_BOSS_KANBU_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, STRING("このあと\nまぐなむばってんだーに　のるよてい<key>"), 0, 0)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(5.0f), FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, 300, 0, 0, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(9), PTR(&weapon_kanbu2_attack))
    CALL_CHILD(PTR(normal_attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {30, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_2"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static PoseTableEntry pose_table_guard[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "G_2"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {30, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "G_2"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_2"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "G_2"},
};

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_2"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(2.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
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
        LABEL(90)
        GOTO(98)
        LABEL(91)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(98)
        LABEL(98)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_HIP_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
        WAIT_FRAMES(25)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_HIP_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_HIP_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_1"))
        WAIT_FRAMES(15)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 15, 0, FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        RETURN
EVT_END()

