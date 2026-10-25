#include "system.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "battle/battle_status_effect.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"



static USER_FUNC(shadow_scale_memory);
static USER_FUNC(shadow_scale_return);
static USER_FUNC(shadow_scale_small);
static USER_FUNC(pakkun_ceil_exist);

BattleUnitKind unit_monochrome_pakkun;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_pakkun_flower_attack;
static EvtWord attack_event[41];
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_ceil[18];
static BattleDataEntry data_table[15];
static EvtWord init_event[31];
static EvtWord dmg_avoid_counter_thorn_head_event[71];
static EvtWord damage_event[10];
static EvtWord counter_damage_event[30];
static EvtWord ceil_fall_ready_event[131];
static EvtWord return_event[256];
static EvtWord normal_attack_event[311];
static EvtWord wait_event[6];

BattleUnitKind unit_monochrome_pakkun = {
    25,
    "btl_un_monochrome_pakkun",
    4,
    0,
    1,
    1,
    11,
    0,
    1,
    70,
    1,
    60,
    9999,
    40,
    48,
    4,
    48,
    0.0f,
    24.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    0.0f,
    16.0f,
    32.0f,
    0.0f,
    0.0f,
    24.0f,
    0.0f,
    40.0f,
    48.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_PAKKUN_DAMAGED1",
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

static StatusVulnerability regist = {110, 90, 60, 100, 100, 100, 100, 80, 100, 95, 100, 95, 100, 95, 90, 30, 60, 100, 90, 100, 100, 95};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_monochrome_pakkun", "c_pakflwr_t", 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x9, 0x1, pose_table},
};

static BattleWeapon weapon_pakkun_flower_attack = {
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
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
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

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(9), PTR(&weapon_pakkun_flower_attack))
        CALL_CHILD(PTR(normal_attack_event))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    SET_VALUE(LW(9), PTR(&weapon_pakkun_flower_attack))
    CALL_CHILD(PTR(normal_attack_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static PoseTableEntry pose_table[] = {
    {1, "PKF_N_1"},
    {2, "PKF_Y_1"},
    {9, "PKF_Y_1"},
    {5, "PKF_K_1"},
    {4, "PKF_X_1"},
    {3, "PKF_X_1"},
    {28, "PKF_S_1"},
    {29, "PKF_Q_1"},
    {30, "PKF_Q_1"},
    {31, "PKF_S_2"},
    {39, "PKF_D_1"},
    {50, "PKF_A_1B"},
    {40, "PKF_S_1"},
    {42, "PKF_S_1"},
    {56, "PKF_X_1"},
    {57, "PKF_X_1"},
    {65, "PKF_T_1"},
    {69, "PKF_S_1"},
};

static PoseTableEntry pose_table_ceil[] = {
    {1, "PKF_N_2"},
    {2, "PKF_Y_2"},
    {9, "PKF_Y_2"},
    {5, "PKF_K_2"},
    {4, "PKF_X_2"},
    {3, "PKF_X_2"},
    {28, "PKF_S_3"},
    {29, "PKF_Q_2"},
    {30, "PKF_Q_2"},
    {31, "PKF_S_4"},
    {39, "PKF_D_2"},
    {50, "PKF_A_5"},
    {40, "PKF_S_3"},
    {42, "PKF_S_3"},
    {56, "PKF_X_2"},
    {57, "PKF_X_2"},
    {65, "PKF_S_3"},
    {69, "PKF_S_3"},
};

static BattleDataEntry data_table[] = {
    {37, dmg_avoid_counter_thorn_head_event},
    {26, counter_damage_event},
    {25, counter_damage_event},
    {27, counter_damage_event},
    {28, counter_damage_event},
    {29, counter_damage_event},
    {30, counter_damage_event},
    {31, counter_damage_event},
    {32, counter_damage_event},
    {33, counter_damage_event},
    {34, counter_damage_event},
    {35, counter_damage_event},
    {36, counter_damage_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, 50, 0)
    CALL(shadow_scale_memory, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 31)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_S_1"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 63)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 37)
        GOTO(90)
    END_IF
    ADD(LW(1), 50)
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

static EVT_BEGIN(counter_damage_event)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_D_1"))
    WAIT_MS(1000)
    CALL(btlevtcmd_CheckDamageCode, -2, 512, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, -2, 2048, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL_CHILD(PTR(return_event))
    RETURN
EVT_END()

static EVT_BEGIN(ceil_fall_ready_event)
    CALL(btlevtcmd_SetEventCeilFall, -2, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAKKUN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_small, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_E_4"))
    WAIT_MS(300)
    CALL(btlevtcmd_OnAttribute, -2, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kNoShadow)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHomePos, -2, LW(0), 0, LW(2))
    WAIT_MS(500)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_return, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_OffAttribute, -2, kUnitHidden)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, kNoShadow)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_E_1"))
    CALL(btlevtcmd_SetHitOffset, -2, 1, 0, 0, 0)
    CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, 50, 0)
    CALL(btlevtcmd_SetStatusIconOffset, -2, 4, 48)
    CALL(btlevtcmd_SetKissHitOffset, -2, 16, 32, 0)
    CALL(btlevtcmd_OffAttribute, -2, kCeiling)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(return_event)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(pakkun_ceil_exist, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_sub_random, 1, LW(15))
    ELSE
        SET_VALUE(LW(15), 0)
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAKKUN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_small, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_E_2"))
    WAIT_MS(300)
    CALL(btlevtcmd_OnAttribute, -2, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kNoShadow)
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_SetEventCeilFall, -2, 0)
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, -2, LW(0), 0, LW(2))
        CALL(btlevtcmd_SetHomePos, -2, LW(0), 0, LW(2))
        WAIT_MS(500)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        BEGIN_SIBLING_EVENT
            CALL(shadow_scale_return, -2)
        END_SIBLING_EVENT
        CALL(btlevtcmd_OffAttribute, -2, kUnitHidden)
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, kNoShadow)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_E_1"))
        CALL(btlevtcmd_SetHitOffset, -2, 1, 0, 0, 0)
        CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, 50, 0)
        CALL(btlevtcmd_SetStatusIconOffset, -2, 4, 48)
        CALL(btlevtcmd_SetKissHitOffset, -2, 16, 32, 0)
        CALL(btlevtcmd_OffAttribute, -2, kCeiling)
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, kShellTosslikeCannotTarget)
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, kHammerlikeCannotTarget)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
        WAIT_MS(500)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_S_1"))
    ELSE
        CALL(btlevtcmd_SetEventCeilFall, -2, PTR(ceil_fall_ready_event))
        CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, -2, LW(0), 130, LW(2))
        CALL(btlevtcmd_SetHomePos, -2, LW(0), 130, LW(2))
        WAIT_MS(500)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        BEGIN_SIBLING_EVENT
            CALL(shadow_scale_return, -2)
        END_SIBLING_EVENT
        CALL(btlevtcmd_OffAttribute, -2, kUnitHidden)
        CALL(btlevtcmd_OffPartsAttribute, -2, 1, kNoShadow)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_E_3"))
        CALL(btlevtcmd_SetHitOffset, -2, 1, 0, -80, 0)
        CALL(btlevtcmd_SetHitCursorOffset, -2, 1, 0, -50, 0)
        CALL(btlevtcmd_SetStatusIconOffset, -2, 4, 10)
        CALL(btlevtcmd_SetKissHitOffset, -2, 16, -32, 0)
        CALL(btlevtcmd_OnAttribute, -2, kCeiling)
        CALL(btlevtcmd_OnPartsAttribute, -2, 1, kShellTosslikeCannotTarget)
        CALL(btlevtcmd_OnPartsAttribute, -2, 1, kHammerlikeCannotTarget)
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_ceil))
        WAIT_MS(500)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_S_3"))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(9))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAKKUN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_small, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_CheckAttribute, -2, kCeiling, LW(15))
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_E_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_E_4"))
    END_IF
    WAIT_MS(300)
    CALL(btlevtcmd_OnAttribute, -2, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, -2, 1, kNoShadow)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 45)
    CALL(btlevtcmd_SetPos, -2, LW(0), 0, LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_SetHitOffset, -2, 1, 0, 0, 0)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAKKUN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(shadow_scale_return, -2)
    END_SIBLING_EVENT
    CALL(btlevtcmd_OffAttribute, -2, kUnitHidden)
    CALL(btlevtcmd_OffPartsAttribute, -2, 1, kNoShadow)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_E_1"))
    WAIT_MS(150)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_A_1A"))
    WAIT_MS(200)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_PAKKUN_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_A_1B"))
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PKF_A_1C"))
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
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(9), 256, LW(5))
    GOTO(98)
    LABEL(98)
    WAIT_MS(500)
    CALL_CHILD(return_event)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static USER_FUNC(shadow_scale_return) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);

    if (isFirstCall) {
        unit->work[1] = 0;
    }
    part->shadowScale = intplGetValue(0, 0.0f, unit->work[0], unit->work[1], 15);
    if (unit->work[1] >= 15) {
        return EVT_RETURN_DONE;
    }
    unit->work[1]++;
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(shadow_scale_small) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);

    if (isFirstCall) {
        unit->work[1] = 0;
    }
    part->shadowScale = intplGetValue(0, unit->work[0], 0.0f, unit->work[1], 15);
    if (unit->work[1] >= 15) {
        return EVT_RETURN_DONE;
    }
    unit->work[1]++;
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(shadow_scale_memory) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);

    unit->work[0] = part->shadowScale;
    return EVT_RETURN_DONE;
}

static USER_FUNC(pakkun_ceil_exist) {
    EvtWord* args = event->args;

    if (_battleWorkPointer->stageObjectWork.flags & 0x20) {
        evtSetValue(event, args[0], 0);
    } else {
        evtSetValue(event, args[0], 1);
    }
    return EVT_RETURN_DONE;
}
