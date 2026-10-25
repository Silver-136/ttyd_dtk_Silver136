#include "battle/battle_unit.h"
#include "eff/n64/eff_kumokumo_n64.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_audience.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "unit/party/unit_party_christine.h"
#include "manager/evtmgr_cmd.h"


static USER_FUNC(_clauda_breath_effect_fire);
static USER_FUNC(_clauda_breath_effect_ready);
USER_FUNC(_status_window_off);
USER_FUNC(btlevtcmd_SetEventEntry);
extern BattleUnitKind unit_act_atmic_teresa;
extern BattleUnitKind unit_act_clauda;
extern BattleUnitKind unit_act_kinopiko;
extern BattleUnitKind unit_act_kinopio;
extern BattleUnitKind unit_act_teresa;

BattleUnitKind unit_act_mario;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[7];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[15];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static EvtWord attack_event[5];
static EvtWord wait_event[6];
BattleUnitSetup entry_unit_data[8];
static EvtWord battle_entry_event[168];
EvtWord _battle_event[155];
EvtWord _act_target_enemy[33];
EvtWord _act_target_mario[26];
EvtWord _act_mario_jump_attack[240];
EvtWord _act_clauda_press_attack[350];
EvtWord _act_clauda_breath_attack[269];
EvtWord _act_teresa_attack[190];
EvtWord _act_atmic_teresa_press_attack[188];

BattleUnitKind unit_act_mario = {
    181,
    "btl_un_act_mario",
    10,
    0,
    1,
    1,
    1,
    0,
    0,
    0,
    0,
    100,
    9,
    28,
    24,
    5,
    25,
    0.0f,
    14.0f,
    0.0f,
    0,
    0,
    0.0f,
    30.0f,
    0.0f,
    14.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    0.0f,
    14.0f,
    15.6f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BTL_DAMAGE1",
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

static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {28, "EM_S_1"},
    {31, "KNP_S_1"},
    {39, "EM_D_1"},
    {42, "EM_R_1"},
    {40, "EM_W_1"},
    {65, "EM_T_1"},
    {69, "EM_S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_act_mario", "c_mario", 0.0f, 0.0f, 0.0f, -5.0f, 30.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(battle_entry_event))
    CALL(_status_window_off)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

BattleUnitSetup entry_unit_data[] = {
    {
        &unit_act_clauda, // mUnitKindParams
        0, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {-140.0f, 0.0f, 5.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {45.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {85.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_atmic_teresa, // mUnitKindParams
        1, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {150.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_kinopio, // mUnitKindParams
        2, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {-75.0f, 0.0f, -40.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_kinopio, // mUnitKindParams
        2, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {-15.0f, 0.0f, -40.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_kinopiko, // mUnitKindParams
        2, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {-45.0f, 0.0f, -40.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unitdata_Party_Christine, // mUnitKindParams
        2, // alliance
        {0}, // pad_05
        0x4000004, // attackPhase
        {0.0f, -50.0f, 110.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_GetHomePos, -3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ChangeFaceDirection, -2, 1)
    CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_GetHomePos, -3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -3, LW(6), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, -2, LW(0))
    CALL(btlevtcmd_SetUnitWork, -2, 0, LW(0))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[0]), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 1, LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[1]), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 2, LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[2]), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 3, LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[3]), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 4, LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[4]), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 5, LW(3))
    CALL(btlevtcmd_ChangeFaceDirection, LW(3), 1)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[5]), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 6, LW(3))
    CALL(btlevtcmd_ChangeFaceDirection, LW(3), 1)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[6]), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 7, LW(3))
    CALL(btlevtcmd_ChangeFaceDirection, LW(3), 1)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[7]), 0)
    CALL(btlevtcmd_SetUnitWork, -2, 8, LW(3))
    CALL(btlevtcmd_ChangeFaceDirection, LW(3), 1)
    SPAWN_EVENT(_battle_event)
    WAIT_MS(2000)
    CALL(evt_msg_print, 2, STRING("ep_39"), 0, -2)
    WAIT_MS(1000)
    CALL(btlevtcmd_onoff_battleflag, 1, 32)
    RETURN
EVT_END()

EVT_BEGIN(_battle_event)
    LABEL(0)
    CALL_CHILD(PTR(_act_mario_jump_attack))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(evt_sub_random, 1, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(_act_clauda_press_attack))
        ELSE
        CALL_CHILD(PTR(_act_clauda_breath_attack))
    END_IF
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A_7"))
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 28)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 65)
    WAIT_MS(1000)
    CALL_CHILD(PTR(_act_teresa_attack))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 28)
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 65)
    WAIT_MS(1000)
    CALL_CHILD(PTR(_act_teresa_attack))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 28)
    CALL(btlevtcmd_GetUnitWork, -2, 4, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 65)
    WAIT_MS(1000)
    CALL_CHILD(PTR(_act_atmic_teresa_press_attack))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 28)
    CALL(btlevtcmd_GetUnitWork, -2, 5, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 65)
    WAIT_MS(1500)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 28)
    CALL(btlevtcmd_GetUnitWork, -2, 7, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 65)
    WAIT_MS(1500)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 28)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(_act_target_enemy)
    CALL(evt_sub_random, 2, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetUnitWork, -2, 2, LW(3))
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetUnitWork, -2, 3, LW(3))
        CASE_DEFAULT
        CALL(btlevtcmd_GetUnitWork, -2, 4, LW(3))
    END_SWITCH
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    RETURN
EVT_END()

EVT_BEGIN(_act_target_mario)
    CALL(evt_sub_random, 1, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
        CASE_DEFAULT
        CALL(btlevtcmd_GetUnitWork, -2, 1, LW(3))
    END_SWITCH
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    RETURN
EVT_END()

EVT_BEGIN(_act_mario_jump_attack)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 65)
    WAIT_MS(1000)
    CALL_CHILD(_act_target_enemy)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    SET_VALUE(LW(0), -20)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(6))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(6))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
    CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 3, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_J_1D"))
    WAIT_MS(50)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_StopWaitEvent, LW(3))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
        WAIT_MS(1000)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_J_1B"))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -2, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 27)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 8, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("EM_R_1"))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    RETURN
EVT_END()

EVT_BEGIN(_act_clauda_press_attack)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 65)
    WAIT_MS(1000)
    CALL_CHILD(_act_target_enemy)
    CALL(btlevtcmd_CalculateFaceDirection, LW(5), -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, LW(5), LW(15))
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A1_1"))
    CALL(btlevtcmd_SetFallAccel, LW(5), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_JumpPosition, LW(5), LW(0), LW(1), LW(2), 60, -1)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
        CALL(evt_sub_intpl_init_float, 5, FLOAT(1.0f), FLOAT(0.14941406f), 10)
        LOOP(11)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(3), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(1500)
        CALL(evt_sub_intpl_init_float, 5, FLOAT(0.14941406f), FLOAT(1.25f), 10)
        LOOP(11)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(3), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, 7, FLOAT(1.25f), FLOAT(1.0f), 30)
        LOOP(31)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(3), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(500)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 28)
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpContinue, LW(5))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.0f), FLOAT(0.39941406f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(5), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, 11, FLOAT(0.39941406f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(5), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.0f), FLOAT(1.5f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(5), LW(7), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.5f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(5), LW(7), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, LW(5), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(5), LW(0), 20)
    ADD(LW(1), 20)
    CALL(btlevtcmd_SetRotate, LW(5), 0, 0, 0)
    CALL(btlevtcmd_SetFallAccel, LW(5), FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPosition, LW(5), LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_Y_1"))
    CALL(evt_audience_joy_ending)
    WAIT_MS(500)
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_SetMoveSpeed, LW(5), FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 40)
    CALL(btlevtcmd_GetHomePos, LW(5), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, LW(5), LW(0), LW(1), LW(2), FLOAT(6.0f), LW(9))
    CALL(btlevtcmd_DivePosition, LW(5), LW(0), LW(1), LW(2), LW(9), 0, 4, 0, -1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 28)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    RETURN
EVT_END()

EVT_BEGIN(_act_clauda_breath_attack)
    CALL(btlevtcmd_JumpSetting, LW(5), 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 65)
    WAIT_MS(1000)
    CALL(_clauda_breath_effect_ready)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A2_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A2_2"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A2_3"))
    CALL(_clauda_breath_effect_fire, LW(12))
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
        LOOP(30)
            CALL(btlevtcmd_SetDispOffset, LW(3), 2, 0, 0)
            WAIT_FRAMES(2)
            CALL(btlevtcmd_SetDispOffset, LW(3), 0, 0, 0)
            WAIT_FRAMES(2)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 28)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
        LOOP(30)
            CALL(btlevtcmd_SetDispOffset, LW(3), 2, 0, 0)
            WAIT_FRAMES(2)
            CALL(btlevtcmd_SetDispOffset, LW(3), 0, 0, 0)
            WAIT_FRAMES(2)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 28)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, -2, 4, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
        LOOP(30)
            CALL(btlevtcmd_SetDispOffset, LW(3), 2, 0, 0)
            WAIT_FRAMES(2)
            CALL(btlevtcmd_SetDispOffset, LW(3), 0, 0, 0)
            WAIT_FRAMES(2)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 28)
    END_SIBLING_EVENT
    WAIT_FRAMES(120)
    CALL(evt_eff_delete_ptr, LW(12))
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_Y_1"))
    CALL(evt_audience_joy_ending)
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 28)
    RETURN
EVT_END()

EVT_BEGIN(_act_teresa_attack)
    CALL_CHILD(PTR(_act_target_mario))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 42)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetAlpha, LW(5), LW(6), 255)
        SET_VALUE(LW(7), 256)
        LOOP(16)
            WAIT_FRAMES(1)
            SUBTRACT(LW(7), 16)
            CALL(btlevtcmd_SetAlpha, LW(5), LW(6), LW(7))
        END_LOOP
        WAIT_FRAMES(20)
        CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("A_1"))
        CALL(btlevtcmd_SetAlpha, LW(5), LW(6), 0)
        SET_VALUE(LW(7), -1)
        LOOP(8)
            WAIT_FRAMES(1)
            ADD(LW(7), 32)
            CALL(btlevtcmd_SetAlpha, LW(5), LW(6), LW(7))
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(5), LW(0), 20)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, LW(5), LW(0), LW(1), LW(2), 60, -5, 4, 0, -1)
    CALL(btlevtcmd_SetAlpha, LW(5), LW(6), 255)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("A_2"))
    WAIT_MS(250)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_StopWaitEvent, LW(3))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
        WAIT_MS(1000)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), 42)
    CALL(btlevtcmd_AddPartsRotate, LW(5), LW(6), 0, 180, 0)
    CALL(btlevtcmd_GetHomePos, LW(5), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, LW(5), LW(0), LW(1), LW(2), 40, 0, 4, 0, -1)
    CALL(btlevtcmd_SetPartsRotate, LW(5), LW(6), 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 28)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    RETURN
EVT_END()

EVT_BEGIN(_act_atmic_teresa_press_attack)
    CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("R_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 10)
    CALL(btlevtcmd_DivePosition, LW(5), -125, 100, LW(2), 120, 10, 4, 0, -1)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("Z_1"))
    CALL(btlevtcmd_DivePosition, LW(5), -125, 0, LW(2), 30, 0, 1, 0, -1)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(3))
        CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
        CALL(btlevtcmd_StopWaitEvent, LW(3))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
        WAIT_MS(1000)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetUnitWork, -2, 1, LW(3))
        CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
        CALL(btlevtcmd_StopWaitEvent, LW(3))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), 39)
        WAIT_MS(1000)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("W_1"))
    CALL(btlevtcmd_GetHomePos, LW(5), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, LW(5), LW(0), LW(1), LW(2), 60, 50, 6, 0, -1)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("S_1"))
    RETURN
EVT_END()

static USER_FUNC(_clauda_breath_effect_fire) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    BattleUnit* unit;
    s32 var;
    Vec pos;
    EffectEntry* eff;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    var = args[0];
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.x += unit->mSizeMultiplier * (unit->faceDirection * 25);
    pos.y += 15.0f * unit->mSizeMultiplier;
    eff = effKumokumoN64Entry(0, 0, pos.x, pos.y, pos.z, 0.0f, -2.0f, 1.0f);
    ((EffKumokumoN64Particle*)eff->userdata)->r1 = 76;
    ((EffKumokumoN64Particle*)eff->userdata)->g1 = 167;
    ((EffKumokumoN64Particle*)eff->userdata)->b1 = 202;
    ((EffKumokumoN64Particle*)eff->userdata)->r2 = 125;
    ((EffKumokumoN64Particle*)eff->userdata)->g2 = 27;
    ((EffKumokumoN64Particle*)eff->userdata)->b2 = 245;
    ((EffKumokumoN64Particle*)eff->userdata)->rot = -10.0f;
    evtSetValue(event, var, (EvtWord)eff);
    return EVT_RETURN_DONE;
}

static USER_FUNC(_clauda_breath_effect_ready) {
    BattleWork* wp = _battleWorkPointer;
    s32 id;
    BattleUnit* unit;
    Vec pos;
    EffectEntry* eff;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.x += unit->mSizeMultiplier * (unit->faceDirection * 20);
    pos.y += 25.0f * unit->mSizeMultiplier;
    eff = effKumokumoN64Entry(0, 60, pos.x, pos.y, pos.z, 0.0f, 2.0f, 1.0f);
    ((EffKumokumoN64Particle*)eff->userdata)->r1 = 76;
    ((EffKumokumoN64Particle*)eff->userdata)->g1 = 167;
    ((EffKumokumoN64Particle*)eff->userdata)->b1 = 202;
    ((EffKumokumoN64Particle*)eff->userdata)->r2 = 125;
    ((EffKumokumoN64Particle*)eff->userdata)->g2 = 27;
    ((EffKumokumoN64Particle*)eff->userdata)->b2 = 245;
    return EVT_RETURN_DONE;
}

USER_FUNC(_status_window_off) {
    _battleWorkPointer->statusWindowOff = 1;
    return EVT_RETURN_DONE;
}

