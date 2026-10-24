#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"

USER_FUNC(btlevtcmd_AnimeChangePose);
USER_FUNC(btlevtcmd_AnimeChangePoseFromTable);
USER_FUNC(btlevtcmd_CheckStatus);
USER_FUNC(btlevtcmd_CheckToken);
USER_FUNC(btlevtcmd_ChoiceSamplingEnemy);
USER_FUNC(btlevtcmd_EnemyItemUseCheck);
USER_FUNC(btlevtcmd_GetEnemyBelong);
USER_FUNC(btlevtcmd_GetHomePos);
USER_FUNC(btlevtcmd_GetSelectNextEnemy);
USER_FUNC(btlevtcmd_GetUnitWork);
USER_FUNC(btlevtcmd_SamplingEnemy);
USER_FUNC(btlevtcmd_SetEventAttack);
USER_FUNC(btlevtcmd_SetEventConfusion);
USER_FUNC(btlevtcmd_SetEventDamage);
USER_FUNC(btlevtcmd_SetEventWait);
USER_FUNC(btlevtcmd_SetHomePos);
USER_FUNC(btlevtcmd_SetPos);
USER_FUNC(btlevtcmd_SetUnitWork);
USER_FUNC(btlevtcmd_StartWaitEvent);
USER_FUNC(btlevtcmd_check_battleflag);
USER_FUNC(evt_btl_camera_set_mode);
USER_FUNC(evt_sub_random);
static USER_FUNC(teresa_check_move);
static USER_FUNC(teresa_check_teresa);
static USER_FUNC(teresa_check_trans);

f32 fd_teresa_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_teresa_neg_one_382;
f32 fd_teresa___local_PI_383;
f32 fd_teresa___local_3PIO2_385;
f32 fd_teresa_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_teresa___local_PIO2_384;
f32 fd_teresa_one_381;
f32 fd_teresa_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_teresa_neg_one_365;
f32 fd_teresa___local_PI_366;
f32 fd_teresa___local_3PIO2_368;
f32 fd_teresa_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_teresa___local_PIO2_367;
f32 fd_teresa_one_364;
BattleUnitKind unit_teresa;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_teresa_attack;
static BattleWeapon weapon_teresa_trans_myself;
static BattleWeapon weapon_teresa_trans;
static BattleWeapon weapon_teresa_move;
static PoseTableEntry pose_table[17];
static BattleDataEntry data_table[1];
static EvtWord init_event[47];
static EvtWord damage_event[10];
static EvtWord attack_event[153];
static EvtWord normal_attack_event[354];
static EvtWord trans_myself_event[196];
static EvtWord trans_event[289];
static EvtWord move_event[84];
static EvtWord move_event_sub[103];
static EvtWord wait_event[6];

static USER_FUNC(teresa_check_teresa) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 result = 0;
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    if (unit == NULL) {
        evtSetValue(event, args[1], 0);
        return EVT_RETURN_DONE;
    }
    if (unit->currentKind == 0x4B) {
        result = 1;
    }
    if (unit->currentKind == 0x6A) {
        result = 1;
    }
    evtSetValue(event, args[1], result);
    return EVT_RETURN_DONE;
}

static USER_FUNC(teresa_check_move) {
    EvtWord* args = event->args;
    s32 num = 0;
    s32 i;

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (unit != NULL) {
            if (unit->currentKind == 0x4B && unit->work[0] == 0) {
                num++;
            }
            if (unit->currentKind == 0x6A && unit->work[0] == 0) {
                num++;
            }
        }
    }
    if (num >= 1) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(teresa_check_trans) {
    EvtWord* args = event->args;
    s32 num = 0;
    s32 id = evtGetValue(event, args[0]);
    s32 self;
    s32 i;

    self = BattleTransID(event, id);

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (unit != NULL) {
            if (unit->currentKind == 0x4B && !BtlUnit_CheckStatus(unit, 0x12) && i != self) {
                num++;
            }
            if (unit->currentKind == 0x6A && !BtlUnit_CheckStatus(unit, 0x12) && i != self) {
                num++;
            }
        }
    }
    if (num >= 1) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

f32 fd_teresa_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_teresa_neg_one_382 = -1.0f;

f32 fd_teresa___local_PI_383 = 3.1415927f;

f32 fd_teresa___local_3PIO2_385 = 4.712389f;

f32 fd_teresa_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_teresa___local_PIO2_384 = 1.5707964f;

f32 fd_teresa_one_381 = 1.0f;

f32 fd_teresa_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_teresa_neg_one_365 = -1.0f;

f32 fd_teresa___local_PI_366 = 3.1415927f;

f32 fd_teresa___local_3PIO2_368 = 4.712389f;

f32 fd_teresa_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_teresa___local_PIO2_367 = 1.5707964f;

f32 fd_teresa_one_364 = 1.0f;

BattleUnitKind unit_teresa = {
    75,
    "btl_un_teresa",
    7,
    0,
    1,
    1,
    17,
    0,
    2,
    70,
    1,
    50,
    9999,
    34,
    32,
    0,
    32,
    0.0f,
    16.0f,
    0.0f,
    0,
    -3,
    0.0f,
    0.0f,
    0.0f,
    17.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    17.0f,
    0.0f,
    0.0f,
    17.0f,
    20.8f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    34.0f,
    32.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_TELESA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x4,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {50, 50, 105, 0, 50, 100, 100, 50, 100, 90, 100, 90, 100, 95, 60, 50, 90, 100, 10, 100, 100, 10};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_teresa", "c_teresa", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0, pose_table},
};

static BattleWeapon weapon_teresa_attack = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    0x1101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    0x7FF, // counterResistFlags
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

static BattleWeapon weapon_teresa_trans_myself = {
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
    0x1101260, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x108, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, // holdFastTime
    100, // invisibleChance
    2, // invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_teresa_trans = {
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
    0x1101160, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x108, // special
    0x7FF, // counterResistFlags
    0x80000000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    100, // invisibleChance
    2, // invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_teresa_move = {
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
    0x1100160, // targetClassFlags
    0x20000000, // targetPropertyFlags
    0, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    8, // special
    0x7FF, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static PoseTableEntry pose_table[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "X_1"},
    {28, "S_1"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "A_2"},
    {39, "D_1"},
    {40, "W_1"},
    {42, "R_1"},
    {56, "X_1"},
    {57, "X_1"},
    {65, "T_1"},
    {69, "S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 15)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, 18, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(13), 0)
        ELSE
        SET_VALUE(LW(13), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, 5, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 0)
        ELSE
        CALL(teresa_check_trans, -2, LW(14))
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, 5, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(15), 0)
        ELSE
        CALL(teresa_check_move, LW(15))
    END_IF
    SET_VALUE(LW(0), 0)
    ADD(LW(0), 50)
    ADD(LW(0), 25)
    ADD(LW(0), 25)
    ADD(LW(0), 15)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 25)
    IF_INT_LESS(LW(1), LW(0))
        IF_INT_EQUAL(LW(13), 1)
            CALL_CHILD(PTR(trans_myself_event))
            GOTO(99)
        END_IF
    END_IF
    ADD(LW(0), 25)
    IF_INT_LESS(LW(1), LW(0))
        IF_INT_EQUAL(LW(14), 1)
            CALL_CHILD(PTR(trans_event))
            GOTO(99)
        END_IF
    END_IF
    ADD(LW(0), 15)
    IF_INT_LESS(LW(1), LW(0))
        IF_INT_EQUAL(LW(15), 1)
            CALL_CHILD(PTR(move_event))
            GOTO(99)
        END_IF
    END_IF
    CALL_CHILD(PTR(normal_attack_event))
    GOTO(99)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_teresa_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_teresa_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_teresa_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_teresa_attack))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TELESA_MOVE3"), -250000000, 0, -250000000)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetAlpha, -2, 1, 255)
        SET_VALUE(LW(7), 256)
        LOOP(16)
            WAIT_FRAMES(1)
            SUBTRACT(LW(7), 16)
            CALL(btlevtcmd_SetAlpha, -2, 1, LW(7))
        END_LOOP
        WAIT_FRAMES(20)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TELESA_MOVE4"), -250000000, 0, -250000000)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1"))
        CALL(btlevtcmd_SetAlpha, -2, 1, 0)
        SET_VALUE(LW(7), -1)
        LOOP(8)
            WAIT_FRAMES(1)
            ADD(LW(7), 32)
            CALL(btlevtcmd_SetAlpha, -2, 1, LW(7))
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 15)
    ADD(LW(2), 5)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 60, -5, 4, 0, -1)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_SetAlpha, -2, 1, 255)
    WAIT_FRAMES(15)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(9)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TELESA_ATTACK1"), -250000000, 0, -250000000)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_2"))
    WAIT_FRAMES(15)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_teresa_attack), 0, LW(5))
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
        LABEL(90)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("X_1"))
        WAIT_FRAMES(60)
        GOTO(98)
        LABEL(91)
        CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_add_zoom, 0, -50)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_teresa_attack))
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_teresa_attack), 256, LW(5))
        WAIT_FRAMES(30)
        GOTO(98)
        LABEL(98)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 42)
        CALL(btlevtcmd_AddPartsRotate, -2, 1, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 40, 0, 4, 0, -1)
        CALL(btlevtcmd_SetPartsRotate, -2, 1, 0, 0, 0)
        CALL(btlevtcmd_ResetFaceDirection, -2)
        RETURN
EVT_END()

static EVT_BEGIN(trans_myself_event)
    SET_VALUE(LW(3), -2)
    SET_VALUE(LW(4), 1)
    CALL(btlevtcmd_CheckStatus, -2, 18, LW(8))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_teresa_trans_myself))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_teresa_trans_myself))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 30)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 30, 10, 4, 0, -1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TELESA_MOVE2"), -250000000, 0, -250000000)
    IF_INT_EQUAL(LW(8), 0)
        CALL(btlevtcmd_SetAlpha, -2, 1, 255)
        SET_VALUE(LW(7), 256)
        BEGIN_SIBLING_EVENT
            LOOP(16)
                WAIT_FRAMES(1)
                SUBTRACT(LW(7), 8)
                CALL(btlevtcmd_SetAlpha, -2, 1, LW(7))
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("A_1"))
    CALL(btlevtcmd_SetPartsRotate, -2, 1, 0, 180, 0)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 10, 0, 5, 0, -1)
    WAIT_FRAMES(10)
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPartsRotate, -2, 1, 0, 15, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPartsRotate, -2, 1, 0, 15, 0)
    END_LOOP
    WAIT_FRAMES(12)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
    CALL(btlevtcmd_SetAlpha, -2, 1, 255)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_teresa_trans_myself))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_teresa_trans_myself), 256, LW(5))
    RETURN
EVT_END()

static EVT_BEGIN(trans_event)
    LABEL(0)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_teresa_trans))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_teresa_trans), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(teresa_check_teresa, LW(3), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_CheckStatus, LW(3), 18, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(1)
        END_IF
    END_IF
    GOTO(0)
    LABEL(1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_teresa_trans))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_teresa_trans))
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, -2, LW(10), LW(11), LW(12))
    IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
        CALL(btlevtcmd_SetPartsRotate, -2, 1, 0, 0, 0)
    ELSE
        CALL(btlevtcmd_SetPartsRotate, -2, 1, 0, 180, 0)
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TELESA_MOVE3"), -250000000, 0, -250000000)
    CALL(btlevtcmd_SetAlpha, -2, 1, 255)
    SET_VALUE(LW(7), 256)
    BEGIN_SIBLING_EVENT
        LOOP(16)
            WAIT_FRAMES(1)
            SUBTRACT(LW(7), 16)
            CALL(btlevtcmd_SetAlpha, -2, 1, LW(7))
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 40)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 40)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 60, -5, 4, 0, -1)
    CALL(btlevtcmd_SetPartsRotate, -2, 1, 0, 0, 0)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TELESA_MOVE4"), -250000000, 0, -250000000)
    CALL(btlevtcmd_SetAlpha, -2, 1, 0)
    SET_VALUE(LW(7), -1)
    LOOP(16)
        WAIT_FRAMES(1)
        ADD(LW(7), 16)
        CALL(btlevtcmd_SetAlpha, -2, 1, LW(7))
    END_LOOP
    CALL(btlevtcmd_SetAlpha, -2, 1, 255)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 20)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 40, 0, 0, 0, -1)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_teresa_trans_myself))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_teresa_trans_myself), 256, LW(5))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 30, -5, 4, 0, -1)
    CALL(btlevtcmd_SetPartsRotate, -2, 1, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(move_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_teresa_move))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_teresa_move), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_FRAMES(30)
    SET_VALUE(LW(12), 80)
    LABEL(0)
    CALL(teresa_check_teresa, LW(3), LW(5))
    IF_INT_EQUAL(LW(5), 1)
        CALL(btlevtcmd_GetUnitWork, LW(3), 0, LW(6))
        IF_INT_EQUAL(LW(6), 0)
            SPAWN_EVENT(PTR(move_event_sub))
            ADD(LW(12), 20)
        END_IF
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(0)
    END_IF
    WAIT_FRAMES(LW(12))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(move_event_sub)
    CALL(btlevtcmd_GetUnitId, -2, LW(1))
    IF_INT_EQUAL(LW(3), LW(1))
        SET_VALUE(LW(0), 0)
        SET_VALUE(LW(1), 0)
    ELSE
        CALL(evt_sub_random, 20, LW(0))
        ADD(LW(0), 60)
        SET_VALUE(LW(1), 12)
    END_IF
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, 1)
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, 6291456)
    BEGIN_SIBLING_EVENT
        LOOP(60)
            CALL(btlevtcmd_AddRotate, LW(3), 0, LW(1), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_GetUnitId, -2, LW(0))
        IF_INT_EQUAL(LW(3), LW(0))
            CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("T_1"))
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_TELESA_MOVE1"), -250000000, 0, -250000000)
    CALL(btlevtcmd_GetPartsPos, LW(3), 1, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 40)
    CALL(btlevtcmd_DivePosition, LW(3), LW(0), LW(1), LW(2), 60, 10, 4, 0, -1)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

