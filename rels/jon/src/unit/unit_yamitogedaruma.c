#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "evt/evt_eff.h"
#include "battle/battle_status_effect.h"


BattleUnitKind unit_yamitogedaruma;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefense defence_turn;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_turn[15];
static BattleDataEntry data_table[6];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_togedaruma_attack;
static EvtWord init_event[21];
static EvtWord dmg_avoid_counter_thorn_head_event[63];
static EvtWord dmg_avoid_counter_thorn_front_event[121];
static EvtWord damage_event[10];
static EvtWord dmg_turn_event[131];
static EvtWord wakeup_event[134];
static EvtWord attack_event[358];
static EvtWord wait_event[102];

BattleUnitKind unit_yamitogedaruma = {
    165,
    "btl_un_yamitogedaruma",
    8,
    0,
    1,
    1,
    34,
    1,
    2,
    70,
    2,
    50,
    9999,
    46,
    40,
    4,
    40,
    0.0f,
    20.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    23.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    23.0f,
    0.0f,
    0.0f,
    20.0f,
    26.0f,
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    46.0f,
    40.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_TOGEDA_DAMAGED1",
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

static BattleUnitDefense defence = {{4, VERSION_PICK(4, 99), 4, 4, 4}};

static BattleUnitDefenseAttr defence_attr = {{0, VERSION_PICK(0, 2), 0, 0, 0}};

static BattleUnitDefense defence_turn = {0};

static StatusVulnerability regist = {20, 80, 20, 100, 20, 100, VERSION_PICK(100, 0), 40, 100, 50, 100, 50, 100, 100, 40, 30, 20, 100, 40, 100, 100, 30};

static PoseTableEntry pose_table[] = {
    {1, "TGD_N_1"},
    {2, "TGD_I_1"},
    {9, "TGD_I_1"},
    {5, "TGD_K_1"},
    {4, "TGD_X_1"},
    {3, "TGD_X_1"},
    {28, "TGD_S_4"},
    {29, "TGD_Q_1"},
    {30, "TGD_Q_1"},
    {31, "TGD_S_2"},
    {39, "TGD_D_1"},
    {50, "TGD_A_1"},
    {40, "TGD_S_4"},
    {42, "TGD_S_4"},
    {56, "TGD_X_1"},
    {57, "TGD_X_1"},
    {65, "TGD_T_1"},
    {69, "TGD_S_4"},
};

static PoseTableEntry pose_table_turn[] = {
    {1, "TGD_N_2"},
    {2, "TGD_I_2"},
    {9, "TGD_Z_4"},
    {5, "TGD_K_2"},
    {4, "TGD_X_2"},
    {3, "TGD_X_2"},
    {28, "TGD_S_5"},
    {29, "TGD_Q_2"},
    {30, "TGD_Q_2"},
    {31, "TGD_S_3"},
    {39, "TGD_D_2"},
    {56, "TGD_X_2"},
    {57, "TGD_X_2"},
    {65, "TGD_S_5"},
    {69, "TGD_S_5"},
};

static BattleDataEntry data_table[] = {
    {13, dmg_turn_event},
    {37, dmg_avoid_counter_thorn_head_event},
    {39, dmg_avoid_counter_thorn_front_event},
    {38, dmg_avoid_counter_thorn_front_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_yamitogedaruma", "c_togedaruma_b", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x2009, 0x7, pose_table},
};

static BattleWeapon weapon_togedaruma_attack = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY, // counterResistFlags
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

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 58)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 32)
        GOTO(90)
    END_IF
    ADD(LW(1), 45)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_avoid_counter_thorn_front_event)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEDA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_CheckStatus, -2, STATUS_FLIPPED, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_J_3"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_J_4"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_J_1"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_J_2"))
        END_IF
    END_SIBLING_EVENT
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 53)
        ADD(LW(1), 26)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 33)
        ADD(LW(1), 16)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 43)
    ADD(LW(1), 21)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
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
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(12))
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("TGD_S_1"))
    IF_INT_LESS_OR_EQUAL(LW(12), 0)
        SET_VALUE(LW(12), 0)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 10, 0)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                IF_INT_GREATER_OR_EQUAL(LW(12), 10)
                    BREAK_LOOP
                END_IF
                ADD(LW(12), 1)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, -2, 0, 0, -18)
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 25, -1)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("TGD_S_5"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, LW(0), 10, LW(2))
    CALL(btlevtcmd_SetHomePos, -2, LW(0), 10, LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_OffStatusFlag, LW(10), 1)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    SET_VALUE(LW(12), 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 10, 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(12), 10)
                BREAK_LOOP
            END_IF
            ADD(LW(12), 1)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddRotate, -2, 0, 0, -18)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_ENM_TOGEDA_MOVE6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("TGD_S_4"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHomePos, -2, LW(0), 0, LW(2))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetOverTurnCount, -2, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, -2)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_togedaruma_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_togedaruma_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_togedaruma_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_togedaruma_attack))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEDA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_J_1"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEDA_MOVE2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_A_1"))
    WAIT_MS(400)
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(7.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_togedaruma_attack), 256, LW(5))
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
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_A_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEDA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, -2, 0, 0, FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), 0, LW(2), FLOAT(60.0f), -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEDA_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_N_1"))
    WAIT_MS(1500)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_J_1"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEDA_MOVE5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_togedaruma_attack))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_togedaruma_attack), 256, LW(5))
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TOGEDA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, -2, 0, FLOAT(5.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_J_2"))
    WAIT_FRAMES(15)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("TGD_S_4"))
    WAIT_MS(400)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    LABEL(0)
    SET_VALUE(LW(15), 0)
    CALL(btlevtcmd_CheckStatus, -2, STATUS_SLEEP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_STOP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_DIZZY, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_POISON, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_CONFUSE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_FREEZE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_OffPartsCounterAttribute, -2, 1, 2)
        ELSE
        CALL(btlevtcmd_OnPartsCounterAttribute, -2, 1, 2)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

