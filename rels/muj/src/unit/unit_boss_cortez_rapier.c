#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"

USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AnimeWaitPlayLoopTiming);
USER_FUNC(btlevtcmd_FaceDirectionAdd);
USER_FUNC(btlevtcmd_GetDamage);
USER_FUNC(btlevtcmd_GetDamagePartsId);
USER_FUNC(btlevtcmd_GetHitPos);
USER_FUNC(btlevtcmd_OnUnitFlag);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetEventUnisonPhase);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetMaxMoveCount);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(evt_eff);
extern EvtWord cortez_weapon_blow_dead_event[];
extern EvtWord cortez_weapon_break_event[];
extern EvtWord cortez_weapon_counter_damage_event[];
extern EvtWord cortez_weapon_dead_event[];
extern EvtWord cortez_weapon_phase_event[];
extern EvtWord cortez_weapon_unison_phase_event[];

BattleUnitKind unit_boss_cortez_rapier;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
BattleWeapon weapon_cortez_rapier_float_attack;
static PoseTableEntry pose_table[6];
static BattleDataEntry data_table[21];
static BattleUnitKindPart parts[1];
static EvtWord init_event[47];
static EvtWord damage_event[10];
static EvtWord avoid_thorn_event[33];
static EvtWord phase_event[2];
static EvtWord attack_event[545];
static EvtWord wait_event[10];
static EvtWord cortez_damage_sub_event[25];

BattleUnitKind unit_boss_cortez_rapier = {
    96,
    "btl_un_cortez_rapier",
    4,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    20,
    140,
    30,
    0,
    30,
    0.0f,
    15.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    70.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    70.0f,
    0.0f,
    0.0f,
    70.0f,
    19.5f,
    0.0f,
    0.0f,
    15.0f,
    0.0f,
    140.0f,
    30.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_KOR_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x2000004,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {0, 80, 95, 100, 90, 100, 100, 85, 0, 0, 100, 0, 100, 90, 50, 0, 90, 100, 50, 0, 0, 0};

BattleWeapon weapon_cortez_rapier_float_attack = {
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
    0x2101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0xC7, // special
    0x7FF, // counterResistFlags
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

static PoseTableEntry pose_table[] = {
    {2, "KRT_Z_1"},
    {9, "KRT_Z_1"},
    {28, "KRT_S_1"},
    {31, "KRT_S_1"},
    {39, "KRT_S_1"},
    {69, "KRT_S_1"},
};

static BattleDataEntry data_table[] = {
    {9, cortez_damage_sub_event},
    {49, cortez_weapon_dead_event},
    {71, cortez_weapon_break_event},
    {58, cortez_weapon_blow_dead_event},
    {36, cortez_weapon_counter_damage_event},
    {24, cortez_weapon_counter_damage_event},
    {25, cortez_weapon_counter_damage_event},
    {26, cortez_weapon_counter_damage_event},
    {27, cortez_weapon_counter_damage_event},
    {28, cortez_weapon_counter_damage_event},
    {29, cortez_weapon_counter_damage_event},
    {30, cortez_weapon_counter_damage_event},
    {31, cortez_weapon_counter_damage_event},
    {32, cortez_weapon_counter_damage_event},
    {33, cortez_weapon_counter_damage_event},
    {34, cortez_weapon_counter_damage_event},
    {35, cortez_weapon_counter_damage_event},
    {39, avoid_thorn_event},
    {37, avoid_thorn_event},
    {38, avoid_thorn_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_cortez_rapier", "c_buki3", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x3010009, 0x6, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, -2, 16777216)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(cortez_weapon_phase_event))
    CALL(btlevtcmd_SetEventUnisonPhase, -2, PTR(cortez_weapon_unison_phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, -2, 2, 3)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetMaxMoveCount, -2, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(avoid_thorn_event)
    CALL(btlevtcmd_GetHitPos, -2, 1, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 40)
    ADD(LW(2), 5)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&weapon_cortez_rapier_float_attack))
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    CALL(btlevtcmd_AnimeWaitPlayLoopTiming, -2, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("KRT_Z_1"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 13, 0)
        CALL(evt_sub_intpl_init, 0, 0, -2160, 60)
        LOOP(60)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, -2, LW(0), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), EVT_NULLPTR, EVT_NULLPTR, LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 20)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 60, 0, 4, 0, -1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_KOR_SWORD_DANCE2"), EVT_NULLPTR, 0, LW(9))
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(evt_snd_sfx_pos, LW(9), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_SetRotateOffset, -2, 0, 13, 0)
        SET_VALUE(LW(0), 0)
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            IF_INT_LESS(LW(0), 118)
                ADD(LW(0), 1)
            END_IF
            SET_VALUE(LW(1), LW(0))
            DIVIDE(LW(1), 2)
            CALL(btlevtcmd_AddRotate, -2, LW(1), 0, 0)
            SET_VALUE(LW(5), 5)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), LW(5))
        CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), FLOAT(7.0f), LW(14))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), LW(14), 0, 1, 0, 0)
    END_SIBLING_EVENT
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_GetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(1), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(1), 40)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(2))
    IF_INT_GREATER_OR_EQUAL(LW(2), 0)
        IF_INT_LESS(LW(0), LW(1))
            WAIT_FRAMES(1)
            GOTO(10)
        END_IF
    ELSE
        IF_INT_GREATER(LW(0), LW(1))
            WAIT_FRAMES(1)
            GOTO(10)
        END_IF
    END_IF
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_DEFAULT
        GOTO(97)
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    IF_INT_EQUAL(LW(5), 21)
        DELETE_EVENT(LW(13))
        CALL(evt_snd_sfxoff, LW(9))
        DELETE_EVENT(LW(14))
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 50)
        ADD(LW(1), 15)
        BEGIN_SIBLING_EVENT
            CALL(evt_sub_intpl_init, 5, 0, -30, 20)
            CALL(evt_sub_intpl_get_value)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, -2, EVT_NULLPTR, EVT_NULLPTR, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 21, 0, 5, 0, -1)
        BEGIN_SIBLING_EVENT
            CALL(evt_sub_intpl_init, 4, -30, 0, 20)
            CALL(evt_sub_intpl_get_value)
            LOOP(21)
                CALL(evt_sub_intpl_get_value)
                CALL(btlevtcmd_SetRotate, -2, EVT_NULLPTR, EVT_NULLPTR, LW(0))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 40, 0, 4, 0, -1)
        RETURN
    END_IF
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    DELETE_EVENT(LW(14))
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), LW(0))
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), FLOAT(6.0f), LW(14))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), LW(14), 0, 5, 0, -1)
    DELETE_EVENT(LW(13))
    CALL(evt_snd_sfxoff, LW(9))
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeWaitPlayLoopTiming, -2, 1)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static EVT_BEGIN(cortez_damage_sub_event)
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetDamagePartsId, -2, LW(11))
    CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("KRT_Z_1"))
    WAIT_FRAMES(30)
    LABEL(90)
    RETURN
EVT_END()

