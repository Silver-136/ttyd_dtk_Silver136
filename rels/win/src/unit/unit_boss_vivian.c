#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "unit/party/unit_party_vivian.h"
#include "battle/battle_database_common.h"

USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_AnimeChangePoseType);
USER_FUNC(btlevtcmd_CheckDamageCode);
USER_FUNC(btlevtcmd_CheckPhase);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_FaceDirectionSub);
USER_FUNC(btlevtcmd_GetHomePos);
USER_FUNC(btlevtcmd_GetPos);
USER_FUNC(btlevtcmd_GetUnitId);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_MovePosition);
USER_FUNC(btlevtcmd_ResetFaceDirection);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetHomePos);
USER_FUNC(btlevtcmd_SetMoveSpeed);
USER_FUNC(btlevtcmd_SetPos);
USER_FUNC(btlevtcmd_SetStayPose);
USER_FUNC(btlevtcmd_SetTalkPose);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_SortPhaseMoveTable);
USER_FUNC(btlevtcmd_SpawnUnit);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_StatusWindowOnOff);
USER_FUNC(btlevtcmd_WaitAttackEnd);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_sub_random);
extern EvtWord btl_majolyne_zoom[];
extern EvtWord btl_marilyn_zoom[];
extern EvtWord btl_vivian_zoom[];
extern EvtWord majo3_shadow_tail_event[];
extern EvtWord majo_unison_phase_event[];
extern BattleUnitKind unit_boss_majolyne;
extern BattleUnitKind unit_boss_marilyn;

BattleUnitKind unit_boss_vivian;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_vivian_kagenuke_attack1;
BattleWeapon weapon_vivian_flame_attack1;
static BattleUnitSetup entry_marilyn;
static BattleUnitSetup entry_majolyne;
static EvtWord vivian_down[19];
static EvtWord vivian_up[27];
static EvtWord init_event[179];
static EvtWord entry_event[46];
static EvtWord damage_event[22];
static EvtWord dead_event[189];
static EvtWord unison_phase_event[81];
static EvtWord phase_event[2];
static EvtWord attack_event[98];
#ifdef VERSION_G8ME01
static EvtWord kagenuke_attack_event[207];
#else
static EvtWord kagenuke_attack_event[204];
#endif
EvtWord flame_attack_event[185];
#ifdef VERSION_G8ME01
static EvtWord wait_event[7];
#else
static EvtWord wait_event[6];
#endif

BattleUnitKind unit_boss_vivian = {
    33,
    "btl_un_vivian1",
    10,
    0,
    1,
    1,
    26,
    0,
    0,
    100,
    0,
    50,
    9,
    30,
    40,
    0,
    40,
    0.0f,
    20.0f,
    0.0f,
    0,
    0,
    6.0f,
    16.0f,
    0.0f,
    15.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    15.0f,
    0.0f,
    0.0f,
    11.0f,
    26.0f,
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    30.0f,
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
    "SFX_BOSS_VIVI_DAMAGED1",
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

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {70, 80, 70, 100, 70, 100, 100, 50, 100, 80, 100, 80, 100, 95, 70, 0, 0, 100, 70, 100, 100, 0};

static PoseTableEntry pose_table[] = {
    {1, "PTR_N_1"},
    {2, "PTR_Z_1"},
    {9, "PTR_Z_1"},
    {5, "PTR_X_1"},
    {4, "PTR_X_1"},
    {3, "PTR_X_1"},
    {28, "PTR_S_4"},
    {29, "PTR_Q_1"},
    {30, "PTR_Q_1"},
    {31, "PTR_D_1"},
    {39, "PTR_D_1"},
    {50, "PTR_S_4"},
    {42, "PTR_R_1"},
    {40, "PTR_W_1"},
    {56, "PTR_D_2"},
    {57, "PTR_D_2"},
    {65, "PTR_T_5"},
    {69, "PTR_S_4"},
};

static BattleDataEntry data_table[] = {
    {49, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_vivian", "c_vivian", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

BattleWeapon weapon_vivian_kagenuke_attack1 = {
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    2, // counterResistFlags
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

BattleWeapon weapon_vivian_flame_attack1 = {
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
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x2101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    1, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x20040, // special
    0x7FF, // counterResistFlags
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

static BattleUnitSetup entry_marilyn = {
    &unit_boss_marilyn, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static BattleUnitSetup entry_majolyne = {
    &unit_boss_majolyne, // mUnitKindParams
    1, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(vivian_down)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_VIVI_SHADOW1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(battle_evt_majo_disp_off, -2, 1, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(vivian_up)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_VIVI_SHADOW2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(battle_evt_majo_disp_on, -2, 1, LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, -2, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(entry_event))
    CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 5, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 6, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 7, 0)
    CALL(btlevtcmd_GetUnitId, -2, GW(10))
    CALL(btlevtcmd_SpawnUnit, GW(9), PTR(&entry_marilyn), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 50)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetPos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, GW(9), 4, 1)
    CALL(btlevtcmd_SetUnitWork, GW(9), 5, 1)
    CALL(btlevtcmd_SetUnitWork, GW(9), 6, 0)
    CALL(btlevtcmd_SetUnitWork, GW(9), 7, 0)
    CALL(btlevtcmd_SpawnUnit, GW(8), PTR(&entry_majolyne), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 100)
    ADD(LW(2), 20)
    CALL(btlevtcmd_SetPos, GW(8), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, GW(8), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, GW(8), 4, 2)
    CALL(btlevtcmd_SetUnitWork, GW(8), 5, 2)
    CALL(btlevtcmd_SetUnitWork, GW(8), 6, 0)
    CALL(btlevtcmd_SetUnitWork, GW(8), 7, 0)
    SET_VALUE(GW(13), 0)
    SET_VALUE(GW(12), 0)
    SET_VALUE(GW(11), 0)
    SET_VALUE(GW(14), 2)
    CALL(btlevtcmd_SetUnitWork, -2, 8, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    SPAWN_EVENT(PTR(majo3_shadow_tail_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    WAIT_FRAMES(60)
    WAIT_FRAMES(60)
    CALL_CHILD(PTR(btl_majolyne_zoom))
    CALL(evt_msg_print, 2, STRING("btl_majo3_00"), 0, GW(8))
    WAIT_MS(300)
    CALL_CHILD(PTR(btl_marilyn_zoom))
    CALL(evt_msg_print, 2, STRING("btl_majo3_01"), 0, GW(9))
    WAIT_MS(300)
    CALL_CHILD(PTR(btl_vivian_zoom))
    CALL(evt_msg_print, 2, STRING("btl_majo3_02"), 0, -2)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_CheckDamageCode, -2, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(13), 1)
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
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
    SWITCH(GW(11))
        CASE_EQUAL(0)
        SWITCH(GW(12))
            CASE_EQUAL(0)
            CASE_EQUAL(1)
            LOOP(0)
                IF_INT_EQUAL(GW(12), 2)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            GOTO(99)
            CASE_EQUAL(2)
        END_SWITCH
        CASE_EQUAL(1)
        LOOP(0)
            IF_INT_EQUAL(GW(11), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        GOTO(99)
        CASE_EQUAL(2)
        SWITCH(GW(12))
            CASE_EQUAL(0)
            CASE_EQUAL(1)
            LOOP(0)
                IF_INT_EQUAL(GW(12), 2)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            GOTO(99)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL_CHILD(PTR(btl_vivian_zoom))
            CALL(evt_msg_print, 2, STRING("btl_majo3_13"), 0, -2)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
            GOTO(90)
        END_SWITCH
        SWITCH(GW(12))
            CASE_EQUAL(0)
            CASE_DEFAULT
        END_SWITCH
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("PTR_D_1"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("PTR_D_1"))
    CALL_CHILD(PTR(btl_vivian_zoom))
    CALL(evt_msg_print, 2, STRING("btl_majo3_10"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    LABEL(99)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_D_3"))
    SET_VALUE(GW(13), 2)
    SET_VALUE(LW(10), -2)
    CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
    RETURN
EVT_END()

static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(majo_unison_phase_event))
        CALL(btlevtcmd_GetUnitWork, -2, 4, LW(3))
        CALL(btlevtcmd_GetUnitWork, -2, 5, LW(4))
        IF_INT_NOT_EQUAL(LW(3), LW(4))
            CALL(btlevtcmd_SetUnitWork, -2, 4, LW(4))
            CALL(btlevtcmd_StartWaitEvent, -2)
            CALL_CHILD(PTR(vivian_down))
            WAIT_MS(500)
            SWITCH(LW(4))
                CASE_EQUAL(0)
                SET_VALUE(LW(0), 55)
                SET_VALUE(LW(2), 0)
                CASE_EQUAL(1)
                SET_VALUE(LW(0), 105)
                SET_VALUE(LW(2), 10)
                CASE_EQUAL(2)
                SET_VALUE(LW(0), 155)
                SET_VALUE(LW(2), 20)
            END_SWITCH
            SET_VALUE(LW(1), 0)
            CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
            CALL_CHILD(PTR(vivian_up))
            CALL(btlevtcmd_SortPhaseMoveTable)
            CALL(btlevtcmd_StartWaitEvent, -2)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, -2, 8, 1)
        CALL(btlevtcmd_SetUnitWork, -2, 7, 2)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 8, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(9), PTR(&weapon_vivian_kagenuke_attack1))
        CALL_CHILD(PTR(kagenuke_attack_event))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, -2, 8, 2)
        SET_VALUE(LW(9), PTR(&weapon_vivian_flame_attack1))
        CALL_CHILD(PTR(flame_attack_event))
        CASE_EQUAL(2)
        SET_VALUE(LW(1), 70)
        ADD(LW(1), 30)
        SUBTRACT(LW(1), 1)
        CALL(evt_sub_random, LW(1), LW(0))
        IF_INT_LESS(LW(0), 70)
            SET_VALUE(LW(9), PTR(&weapon_vivian_kagenuke_attack1))
            CALL_CHILD(PTR(kagenuke_attack_event))
            ELSE
            SET_VALUE(LW(9), PTR(&weapon_vivian_flame_attack1))
            CALL_CHILD(PTR(flame_attack_event))
        END_IF
    END_SWITCH
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(kagenuke_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL_CHILD(vivian_down)
    WAIT_MS(300)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
#ifdef VERSION_G8ME01
    ADD(LW(2), 5)
#endif
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    SET_VALUE(LW(1), 0)
    CALL_CHILD(vivian_up)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A1_2"))
    WAIT_MS(70)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_VIVI_ATTACK4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(98)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 43)
    CALL_CHILD(vivian_down)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(200)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL_CHILD(vivian_up)
    RETURN
EVT_END()

EVT_BEGIN(flame_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PTR_A3_1"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_VIVI_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 6, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
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
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(1000)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

