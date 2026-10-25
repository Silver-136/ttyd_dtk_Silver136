#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "unit/party/unit_party_yoshi.h"
#include "battle/battle_database_common.h"

USER_FUNC(yoshi_original_color_anim_set);

BattleUnitKind unit_gullible_yoshi;
static BattleWeapon enemyWeapon_YoshiNormalAttack;
static BattleUnitDefense defence_yoshi;
static BattleUnitDefenseAttr defence_attr_yoshi;
static StatusVulnerability regist_yoshi;
static BattleDataEntry data_table_yoshi[2];
static BattleUnitKindPart parts_yoshi[1];
static EvtWord init_event_yoshi[24];
EvtWord damage_event_yoshi[22];
static EvtWord dead_event_yoshi[177];
static EvtWord attack_event_yoshi[748];
static EvtWord wait_event_yoshi[7];

BattleUnitKind unit_gullible_yoshi = {
    82,
    "btl_un_gullible_yoshi",
    10,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    9,
    24,
    28,
    4,
    24,
    0.0f,
    14.0f,
    0.0f,
    0,
    0,
    0.0f,
    40.0f,
    0.0f,
    12.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    12.0f,
    0.0f,
    0.0f,
    12.0f,
    18.2f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    24.0f,
    28.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_PARTY_DAMAGE1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x10000000,
    &regist_yoshi,
    1,
    parts_yoshi,
    init_event_yoshi,
    data_table_yoshi,
};

static BattleWeapon enemyWeapon_YoshiNormalAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {1, 1, 1, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20001004, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_HIT | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

static BattleUnitDefense defence_yoshi = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr_yoshi = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist_yoshi = {60, 80, 70, 100, 70, 100, 100, 70, 100, 70, 100, 70, 100, 90, 70, 0, 0, 100, 70, 100, 100, 0};

static BattleDataEntry data_table_yoshi[] = {
    {49, dead_event_yoshi},
    {0, NULL},
};

static BattleUnitKindPart parts_yoshi[] = {
    {1, "btl_un_yoshi", "c_babyyoshi", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence_yoshi, &defence_attr_yoshi, 0x9, 0, pose_table_yoshi_stay},
};

static EVT_BEGIN(init_event_yoshi)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event_yoshi))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event_yoshi))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event_yoshi))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(yoshi_original_color_anim_set, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(damage_event_yoshi)
    CALL(btlevtcmd_CheckDamageCode, -2, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(20), 1)
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event_yoshi)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, LW(3), LW(4), LW(5))
    SET_VALUE(LW(6), 1)
    IF_INT_EQUAL(LW(0), LW(3))
        IF_INT_EQUAL(LW(1), LW(4))
            IF_INT_EQUAL(LW(2), LW(5))
                SET_VALUE(LW(6), 0)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
        CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_ResetFaceDirection, -2)
    END_IF
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    SWITCH(GW(19))
        CASE_EQUAL(0)
        CALL(evt_btl_camera_set_mode, 0, 3)
        CALL(evt_btl_camera_set_moveto, 0, 130, 58, 228, 130, 38, -20, 30, 11)
        WAIT_FRAMES(37)
        CALL(evt_msg_print, 2, STRING("stg4_jin_26_03_yos"), 0, -2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_D_3"))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(500)
        CALL(evt_btl_camera_set_mode, 0, 3)
        CALL(evt_btl_camera_set_moveto, 0, 90, 58, 228, 90, 38, -20, 30, 11)
        WAIT_FRAMES(37)
        CALL(evt_msg_print, 2, STRING("stg4_jin_26_04"), 0, GW(16))
        WAIT_MS(200)
        CASE_EQUAL(1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_D_3"))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        LOOP(0)
            IF_INT_EQUAL(GW(19), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CASE_EQUAL(2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_D_3"))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    CALL(btlevtcmd_KillUnit, -2, 1)
    SET_VALUE(GW(20), 2)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_yoshi)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_YoshiNormalAttack))
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    CALL(btlevtcmd_RunDataEventChild, -2, 7)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    WAIT_FRAMES(20)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        LOOP(0)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(2)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(3)
        END_LOOP
    END_INLINE_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_R_5"))
    WAIT_FRAMES(25)
    DELETE_EVENT(LW(13))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_J_1A"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 25)
    ADD(LW(1), 35)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 25, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_A_3"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    ADD(LW(7), 40)
    SET_VALUE(LW(10), FLOAT(1.0f))
    BEGIN_INLINE_EVENT_GET_ID(LW(13))
        LOOP(0)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 190, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 381, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 668, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 859, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 1145, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 1241, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 1527, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 1814, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 2100, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 2196, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1L"), EVT_NULLPTR, 1742, EVT_NULLPTR)
            WAIT_FRAMES(6)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_PARTY_BATTLE_MOVE1R"), EVT_NULLPTR, 1527, EVT_NULLPTR)
            WAIT_FRAMES(7)
        END_LOOP
    END_INLINE_EVENT
    CALL(btl_yoshi_yoroyoro_jump_calc_param, -2, LW(6), LW(7), LW(8), LW(10), LW(9))
    CALL(btl_yoshi_yoroyoro_jump_move, -2, LW(6), LW(7), LW(8), LW(10), LW(9))
    DELETE_EVENT(LW(13))
    CALL(evt_sub_random, 4, LW(11))
    ADD(LW(11), 1)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 1048832, LW(5))
    ELSE
        CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    END_IF
    SWITCH(LW(5))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_OR_EQUAL(6)
        CASE_OR_EQUAL(4)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
    END_SWITCH
    IF_INT_LESS_OR_EQUAL(LW(10), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKR_A_2B"))
        WAIT_FRAMES(20)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKR_J_1A"))
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_GetFaceDirection, -2, LW(15))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_J_1B"))
        SET_VALUE(LW(0), 1)
        SET_VALUE(LW(1), 9)
        MULTIPLY(LW(15), -40)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 12, 0)
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        WAIT_FRAMES(15)
        LOOP(LW(0))
            SET_VALUE(LW(2), LW(1))
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_YOSHI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            LOOP(LW(2))
                CALL(btlevtcmd_AddRotate, -2, 0, 0, LW(15))
                WAIT_FRAMES(1)
            END_LOOP
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_A_1"))
        WAIT_FRAMES(15)
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_A_1"))
    END_IF
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 4, 0, 0, 0, -1)
    IF_INT_NOT_EQUAL(LW(5), 1)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        SET_VALUE(LW(15), LW(1))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        SUBTRACT(LW(15), LW(1))
        DIVIDE(LW(15), 10)
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), LW(15), 0, 0, 0, -1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_D_1"))
        CALL(btlevtcmd_SetDispOffset, -2, 0, -7, 0)
        CALL(evt_btl_camera_shake_h, 0, 4, 0, 10, 13)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_A_5"))
        CALL(btlevtcmd_SetDispOffset, -2, 0, 0, 0)
        WAIT_FRAMES(54)
        GOTO(95)
    END_IF
    IF_INT_LESS(LW(10), LW(11))
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 0, LW(5))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.19921875f))
        ADD(LW(1), 40)
        CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
        ADD(LW(10), 1)
        GOTO(10)
    ELSE
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    END_IF
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 30, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PYS_D_1"))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 25)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 43)
    WAIT_FRAMES(20)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_yoshi)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END_PAD()

