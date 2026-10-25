#include "battle/battle.h"
#include "eff/n64/eff_kumokumo_n64.h"
#include "battle/battle_database_common.h"
#include "battle/battle_stage.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_party.h"
#include "system.h"
#include <string.h>
#include "battle/battle_attack_audience.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

USER_FUNC(_check_aud_item_type);
USER_FUNC(_clauda_make_extra_work_area);
USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetJumpSound);
USER_FUNC(_make_kumoguard_weapon);
USER_FUNC(_get_clauda_kiss_hit_position);
USER_FUNC(_make_kiss_weapon);
USER_FUNC(_make_breath_weapon);
static USER_FUNC(_clauda_breath_effect_fire);
static USER_FUNC(_clauda_breath_effect_ready);
USER_FUNC(_check_blow_rate);
static void __makeTechMenuFunc(BattleCommand* command, s32* count);

BattleWeapon partyWeapon_ClaudaNormalAttack;
BattleWeapon partyWeapon_ClaudaBreathAttack;
BattleWeapon partyWeapon_ClaudaPredationAttack;
BattleWeapon partyWeapon_ClaudaKumogakureAttack;
PoseTableEntry pose_table_clauda_stay[25];
static BattleDataEntry data_table[8];
BattleUnitKindPart unitpartsdata_Party_Clauda[1];
BattleUnitKind unitdata_Party_Clauda;
static EvtWord battle_entry_event[59];
static EvtWord init_event[60];
static EvtWord damage_event[16];
EvtWord partyClaudaAttack_NormalAttack[700];
EvtWord partyClaudaAttack_BreathAttack[520];
EvtWord partyClaudaAttack_PredationAttack[875];
EvtWord partyClaudaAttack_KumoGuard[290];
EvtWord clauda_appeal_event[71];
static EvtWord attack_audience_event[15];
EvtWord attack_bad_audience_event[348];
EvtWord attack_good_audience_event[216];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static EvtWord attack_event[2];
static EvtWord wait_event[6];
static EvtWord party_win_reaction[11];


















BattleWeapon partyWeapon_ClaudaNormalAttack = {
    "btl_wn_pwd_normal", // name
#ifdef VERSION_G8ME01
    344, // icon
#else
    336, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pwd_body_press", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {1, 2, 2, 4, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_PRESS, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_body_press", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyClaudaAttack_NormalAttack, // mAttackEvtScript
    {0, 20, 20, 100, 20, 10, 2, 5, 10, 0, 0, 0}, // stageEffectRates
};

BattleWeapon partyWeapon_ClaudaBreathAttack = {
    "btl_wn_pwd_lv1", // name
#ifdef VERSION_G8ME01
    345, // icon
#else
    337, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pwd_breath", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {1, 2, 2, 3, 3, 4, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_BLOW, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_breath", // acHelpMsg
    0x20287, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, // fastChance..frightChance
    100, // galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    partyClaudaAttack_BreathAttack, // mAttackEvtScript
#ifdef VERSION_G8ME01
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

BattleWeapon partyWeapon_ClaudaPredationAttack = {
    "btl_wn_pwd_lv2", // name
#ifdef VERSION_G8ME01
    346, // icon
#else
    338, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pwd_sexy_kiss", // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#ifdef VERSION_G8ME01
    1, // mStylishCommandMultiplier
#else
    2, // mStylishCommandMultiplier
#endif
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    weaponGetACOutputParam, // hpDamageFn
    {0, 1, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {3, 3, 0, 0, 0, 0, 0, 0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_sexy_kiss", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyClaudaAttack_PredationAttack, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon partyWeapon_ClaudaKumogakureAttack = {
    "btl_wn_pwd_lv3", // name
#ifdef VERSION_G8ME01
    347, // icon
#else
    339, // icon
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pwd_kumogakure", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    5, // reserved_19
    100, // mBingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_kumogakure", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, // poisonStrength..electricTime
    100, // dodgyChance
    2, // dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    partyClaudaAttack_KumoGuard, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};

PoseTableEntry pose_table_clauda_stay[] = {
    {POSE_TYPE_FREEZE, "PWD_D_5"},
    {POSE_TYPE_STOP, "PWD_D_5"},
    {POSE_TYPE_DIZZY, "PWD_X_1"},
    {POSE_TYPE_POISON, "PWD_D_2"},
    {POSE_TYPE_CONFUSE, "PWD_X_1"},
    {POSE_TYPE_SLEEP, "PWD_N_1"},
    {POSE_TYPE_KO, "PWD_D_3"},
    {POSE_TYPE_NORMAL, "PWD_S_1"},
    {37, "PWD_G_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "PWD_G_1"},
    {POSE_TYPE_DAMAGE, "PWD_D_1"},
    {POSE_TYPE_WALK, "PWD_W_1"},
    {POSE_TYPE_RUN, "PWD_R_1"},
    {POSE_TYPE_PERIL, "PWD_D_2"},
    {POSE_TYPE_DANGER, "PWD_D_2"},
    {POSE_TYPE_ESCAPE_STUMBLE, "PWD_D_3"},
    {POSE_TYPE_ESCAPE_RECOVER, "PWD_D_2"},
    {POSE_TYPE_TALK, "PWD_T_1"},
    {POSE_TYPE_ITEM, "PWD_I_1"},
    {POSE_TYPE_COMMAND, "PWD_C_1"},
    {POSE_TYPE_APPEAL, "PWD_O_1"},
    {POSE_TYPE_ZERO_GRAVITY, "PWD_X_1"},
    {POSE_TYPE_AVOID_COUNTER, "PWD_A4_3"},
    {POSE_TYPE_DEFEND, "PWD_G_1"},
    {POSE_TYPE_ACTION, "PWD_S_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_APPEAL_EVENT, clauda_appeal_event},
    {UNIT_DATA_ATTACK_AUDIENCE, attack_audience_event},
    {UNIT_DATA_TECH_MENU_FUNC, __makeTechMenuFunc},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_WIN_REACTION_EVENT, party_win_reaction},
    {UNIT_DATA_ATTACK_READY_EVENT, subset_attack_ready_jump_event},
    {UNIT_DATA_AVOID_COUNTER_EVENT, btldefaultevt_avoid_counter_event},
    {0, NULL},
};

BattleUnitKindPart unitpartsdata_Party_Clauda[] = {
    {1, "btl_un_clauda", "c_windy", 0.0f, 0.0f, 0.0f, 16.0f, 44.0f, 5.0f, 0.0f, 44.0f, 0.0f, 26, 50, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table_clauda_stay},
};

BattleUnitKind unitdata_Party_Clauda = {
    UNIT_FLURRIE,
    "btl_un_clauda",
    10,
    5,
    5,
    1,
    1,
    0,
    0,
    0,
    0,
    100,
    9,
    20,
    50,
    16,
    44,
    0.0f,
    12.0f,
    0.0f,
    0,
    0,
    0.0f,
    40.0f,
    0.0f,
    0.0f,
    40.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    10.0f,
    0.0f,
    0.0f,
    10.0f,
    32.5f,
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
    "SFX_PARTY_DAMAGE1",
    "SFX_MARIO_DAMAGE_FIRE1",
    "SFX_MARIO_DAMAGE_ICE1",
    "SFX_MARIO_DAMAGE_BIRIBIRI1",
    kFloating | kUnitNoHpGauge,
    &regist,
    1,
    unitpartsdata_Party_Clauda,
    init_event,
    data_table,
};

static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 250)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(4.0f))
    CALL(btlevtcmd_SetMoveSoundDataWork, -2, STRING("SFX_PARTY_BATTLE_SUBHANG1"), STRING("SFX_PARTY_BATTLE_SUBHANG1"), 0, 9, 3, 0, 0)
    CALL(btlevtcmd_MovePosition, -2, LW(0), LW(1), LW(2), 0, -2, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(_clauda_make_extra_work_area)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetWalkSound, -2, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, -2, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 0, 9, 8)
    CALL(btlevtcmd_SetDiveSound, -2, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 0, 9, 8)
    CALL(btlevtcmd_SetJumpSound, -2, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_SetEventEntry, -2, PTR(battle_entry_event))
    CALL(btlevtcmd_OnUnitFlag, -2, BATTLE_UNIT_FLAG_FLOAT_CAPABLE | BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

EVT_BEGIN(partyClaudaAttack_NormalAttack)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    CALL(btlevtcmd_AcSetParamAll, LW(3), LW(4), 180, LW(0), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, -2, 8, 1, 0)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_OffUnitFlag, -2, BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A1_1"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_GetResultAC, LW(5))
    IF_BITS_CLEAR(LW(5), 2)
        CALL(evt_sub_random, 40, LW(5))
        SUBTRACT(LW(5), 20)
        ADD(LW(0), LW(5))
        CALL(evt_sub_random, 20, LW(5))
        SUBTRACT(LW(5), 10)
        ADD(LW(1), LW(5))
    END_IF
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 60, -1)
    CALL(btlevtcmd_OnUnitFlag, -2, BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
    IF_INT_NOT_EQUAL(LW(5), 1)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        GOTO(20)
    END_IF
    SET_VALUE(LW(5), 1)
    LABEL(10)
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
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A1_2"))
    CALL(btlevtcmd_JumpContinue, -2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_D_3"))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.0f), FLOAT(0.39941406f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, 11, FLOAT(0.39941406f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.0f), FLOAT(1.5f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.5f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    WAIT_FRAMES(30)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    GOTO(95)
    LABEL(20)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_ATT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 131328, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    ELSE
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    CALL(btlevtcmd_JumpContinue, -2)
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_ACRStart, -2, 0, 16, 16, 20)
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 30, 13)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.0f), FLOAT(0.39941406f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, 11, FLOAT(0.39941406f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.0f), FLOAT(1.5f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.5f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_NOT_EQUAL(LW(6), 2)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CALL(evt_audience_acrobat_notry)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 20)
    ADD(LW(1), 20)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, FLOAT(-60.0f), 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_Y_1"))
    WAIT_MS(500)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), FLOAT(6.0f), LW(15))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), LW(15), 0, 4, 0, -1)
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(partyClaudaAttack_BreathAttack)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, LW(12))
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    WAIT_FRAMES(20)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, -2, -2)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(1))
    SUBTRACT(LW(1), 3)
    SWITCH(LW(1))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 300)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 300)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 300)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 300)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 300)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 300)
        CASE_DEFAULT
        SET_VALUE(LW(0), 300)
    END_SWITCH
    CALL(btlevtcmd_AcGetDifficulty, LW(1))
    CALL(btlevtcmd_AcSetParamAll, 1, LW(0), 4, 40, 60, 80, 100, LW(1))
    CALL(btlevtcmd_SetupAC, -2, 14, 1, 0)
    CALL(_clauda_breath_effect_ready)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A2_1"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_BREATH1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A2_2"))
    CALL(evt_btl_camera_nomove_x_onoff, 0, 1)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    CALL(evt_btl_camera_nomove_z_onoff, 0, 1)
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 0)
    CALL(evt_btl_camera_nomove_x_onoff, 0, 0)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 0)
    CALL(evt_btl_camera_nomove_z_onoff, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A2_3"))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(btlevtcmd_StageDispellFog)
    END_INLINE_EVENT
    CALL(_clauda_breath_effect_fire, LW(13))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_BREATH2"), EVT_NULLPTR, 0, LW(14))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(14), -1)
        LABEL(5)
        CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
        WAIT_FRAMES(1)
        GOTO(5)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(8)
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(_check_blow_rate, LW(3), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 3, 1)
        END_IF
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        GOTO(8)
    END_IF
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_StopAC)
    SET_VALUE(LW(11), 99)
    SET_VALUE(LW(12), PTR(&partyWeapon_ClaudaBreathAttack))
    CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
    CALL(_make_breath_weapon, LW(12), LW(0))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_SetDispOffset, LW(3), 0, 0, 0)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
            GOTO(51)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
            GOTO(51)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
            GOTO(51)
        END_IF
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
        CALL(btlevtcmd_GetDamageCode, LW(3), LW(0))
        IF_INT_EQUAL(LW(0), 29)
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        END_IF
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    LABEL(50)
    WAIT_MS(300)
    LABEL(51)
    SUBTRACT(LW(11), 1)
    IF_INT_GREATER_OR_EQUAL(LW(11), 1)
        LABEL(52)
        CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
        IF_INT_NOT_EQUAL(LW(3), -1)
            ADD(LW(10), 1)
            GOTO(10)
        END_IF
    END_IF
    LABEL(80)
    WAIT_MS(1500)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_snd_sfxoff, LW(14))
    CALL(evt_eff_delete_ptr, LW(13))
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_NEUTRAL)
    CALL(btlevtcmd_ACRStart, -2, 0, 25, 25, 20)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_NOT_EQUAL(LW(6), 2)
        CALL(evt_audience_acrobat_notry)
        WAIT_MS(500)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_Y_1"))
    WAIT_MS(1000)
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(partyClaudaAttack_PredationAttack)
    CALL(btlevtcmd_JumpSetting, -2, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_GetPartyTechLv, -2, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_AcGetDifficulty, LW(0))
        SUBTRACT(LW(0), 3)
        SWITCH(LW(0))
            CASE_LESS_OR_EQUAL(-3)
            SET_VALUE(LW(0), 120)
            SET_VALUE(LW(1), 0)
            CASE_EQUAL(-2)
            SET_VALUE(LW(0), 100)
            SET_VALUE(LW(1), 0)
            CASE_EQUAL(-1)
            SET_VALUE(LW(0), 80)
            SET_VALUE(LW(1), 1)
            CASE_EQUAL(0)
            SET_VALUE(LW(0), 70)
            SET_VALUE(LW(1), 2)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), 66)
            SET_VALUE(LW(1), 2)
            CASE_EQUAL(2)
            SET_VALUE(LW(0), 62)
            SET_VALUE(LW(1), 2)
            CASE_DEFAULT
            SET_VALUE(LW(0), 58)
            SET_VALUE(LW(1), 2)
        END_SWITCH
        CALL(btlevtcmd_AcSetParamAll, 0, LW(0), LW(1), 5, 3, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
        CALL(btlevtcmd_AcSetGaugeParam, 60, 80, 100, 100)
        CASE_DEFAULT
        CALL(btlevtcmd_AcGetDifficulty, LW(0))
        SUBTRACT(LW(0), 3)
        SWITCH(LW(0))
            CASE_LESS_OR_EQUAL(-3)
            SET_VALUE(LW(0), 120)
            SET_VALUE(LW(1), 0)
            CASE_EQUAL(-2)
            SET_VALUE(LW(0), 100)
            SET_VALUE(LW(1), 0)
            CASE_EQUAL(-1)
            SET_VALUE(LW(0), 80)
            SET_VALUE(LW(1), 1)
            CASE_EQUAL(0)
            SET_VALUE(LW(0), 70)
            SET_VALUE(LW(1), 2)
            CASE_EQUAL(1)
            SET_VALUE(LW(0), 66)
            SET_VALUE(LW(1), 2)
            CASE_EQUAL(2)
            SET_VALUE(LW(0), 62)
            SET_VALUE(LW(1), 2)
            CASE_DEFAULT
            SET_VALUE(LW(0), 58)
            SET_VALUE(LW(1), 2)
        END_SWITCH
        CALL(btlevtcmd_AcSetParamAll, 0, LW(0), LW(1), 3, 2, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
        CALL(btlevtcmd_AcSetGaugeParam, 34, 68, 100, 100)
    END_SWITCH
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, -2, 17, 1, 60)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY_FLOAT(LW(6), 40)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
        CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(3.0f))
        CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 40, 0, 5, 0, -1)
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
    END_IF
    CALL(_get_clauda_kiss_hit_position, -2, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, -2, LW(6))
    MULTIPLY_FLOAT(LW(6), 20)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    ADD(LW(2), 5)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 40, 0, 5, 0, -1)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A3_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(btlevtcmd_CommandPreCheckDamage, -2, LW(3), LW(4), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_OR_EQUAL(6)
        CASE_OR_EQUAL(4)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
    END_SWITCH
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
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A3_2"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CommandPreCheckCounter, -2, LW(3), LW(4), 256, LW(0))
    SWITCH(LW(0))
        CASE_OR_EQUAL(7)
        CASE_OR_EQUAL(8)
        CASE_OR_EQUAL(9)
        CASE_OR_EQUAL(10)
        CASE_OR_EQUAL(11)
        CASE_OR_EQUAL(12)
        CASE_OR_EQUAL(13)
        CASE_OR_EQUAL(14)
        CASE_OR_EQUAL(15)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A3_2"))
        CALL(btlevtcmd_CommandCheckDamage, -2, LW(3), LW(4), 256, LW(5))
        CASE_END
    END_SWITCH
    CALL(btlevtcmd_OffUnitFlag, -2, BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_KISS1"), EVT_NULLPTR, 0, LW(14))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A3_2"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("kiss"), 2, LW(0), LW(1), LW(2), LW(6), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 2, 1)
    CALL(btlevtcmd_StartAC, 1)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_SetRGB, LW(3), LW(4), 255, 255, 255)
        LOOP(45)
            CALL(btlevtcmd_GetRGB, LW(3), LW(4), LW(0), LW(1), LW(2))
            SUBTRACT(LW(1), 2)
            IF_INT_LESS(LW(1), 190)
                SET_VALUE(LW(1), 190)
            END_IF
            SUBTRACT(LW(2), 4)
            CALL(btlevtcmd_SetRGB, LW(3), LW(4), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(45)
            CALL(btlevtcmd_GetRGB, LW(3), LW(4), LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 2)
            SUBTRACT(LW(1), 2)
            ADD(LW(2), 3)
            IF_INT_LESS(LW(1), 0)
                SET_VALUE(LW(1), 0)
            END_IF
            CALL(btlevtcmd_SetRGB, LW(3), LW(4), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_ResultAC)
    CALL(evt_snd_sfxoff, LW(14))
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_GetResultAC, LW(9))
    IF_BITS_SET(LW(9), 2)
        CALL(btlevtcmd_GetPartyTechLv, -2, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            SET_VALUE(LW(7), 3)
            CASE_DEFAULT
            SET_VALUE(LW(7), 2)
        END_SWITCH
        CALL(btlevtcmd_AcGetOutputParam, 0, LW(8))
        IF_INT_GREATER_OR_EQUAL(LW(8), LW(7))
            SUBTRACT(LW(8), LW(7))
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(8), LW(6))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        ELSE
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
    ELSE
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    CALL(btlevtcmd_OnUnitFlag, -2, BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_SetRGB, LW(3), LW(4), 255, 255, 255)
    CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
    CALL(_make_kiss_weapon, LW(12), LW(0))
    IF_BITS_SET(LW(9), 2)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
    END_IF
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetHpDamage, LW(3), LW(0))
        WAIT_FRAMES(50)
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_GetPos, -2, LW(5), LW(6), LW(7))
            CALL(btlevtcmd_GetStatusMg, -2, LW(8))
            MULTIPLY_FLOAT(LW(8), 40)
            ADD(LW(6), LW(8))
            CALL(btlevtcmd_RecoverHp, -2, 1, LW(0))
            CALL(evt_eff, STRING(""), STRING("recovery"), 7, LW(5), LW(6), LW(7), LW(0), 0, 0, 0, 0, 0, 0, 0)
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_KISS2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A3_3"))
    WAIT_FRAMES(5)
    CALL(btlevtcmd_ACRStart, -2, 0, 29, 29, 0)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A_7"))
        WAIT_FRAMES(56)
    ELSE
        CALL(evt_audience_acrobat_notry)
        WAIT_FRAMES(60)
    END_IF
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 40, 0, 5, 0, -1)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(partyClaudaAttack_KumoGuard)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, -2, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, -2)
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, -2, LW(15))
    CALL(btlevtcmd_GetPos, -2, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(3), LW(1), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(0), LW(1))
    DIVIDE(LW(0), 2)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_set_moveto, 1, LW(0), 72, 400, LW(0), 42, 0, 30, 5)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, -2, LW(0))
    CALL(btlevtcmd_AcSetParamAll, 3, 0, 40, 0, 1, 1, 2, 8)
    CALL(btlevtcmd_AcSetFlag, 5)
    CALL(btlevtcmd_SetupAC, -2, 18, 1, 0)
    WAIT_FRAMES(1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A4_2"))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A4_3"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_HIDE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(150)
    CALL(btlevtcmd_GetResultAC, LW(6))
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, LW(3), LW(7))
    DIVIDE(LW(7), 2)
    ADD(LW(1), LW(7))
    ADD(LW(2), 20)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 8, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
    ADD(LW(0), 1)
    CALL(_make_kumoguard_weapon, LW(12), LW(0))
    CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 131328, LW(5))
        SUBTRACT(LW(0), 1)
        CALL(btlevtcmd_GetResultPrizeLv, -2, LW(0), LW(6))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    ELSE
        CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), LW(12), 256, LW(5))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    CALL(btlevtcmd_ACRStart, -2, 0, 29, 29, 20)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A_7"))
        WAIT_MS(1000)
    ELSE
        CALL(evt_audience_acrobat_notry)
        WAIT_MS(1000)
    END_IF
    LABEL(99)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(clauda_appeal_event)
    CALL(btlevtcmd_RunDataEventChild, -2, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_APPEAL)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_APPEAL1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 15)
    ADD(LW(1), 30)
    CALL(btlevtcmd_GetFaceDirection, -2, LW(14))
    MULTIPLY(LW(14), 45)
    CALL(evt_eff, STRING(""), STRING("kiss"), 2, LW(0), LW(1), LW(2), LW(14), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AudienceAppeal, -2)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_audience_event)
    CALL(_check_aud_item_type, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(attack_bad_audience_event))
        CASE_DEFAULT
        CALL_CHILD(PTR(attack_good_audience_event))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(attack_bad_audience_event)
    CALL(btlevtcmd_OffUnitFlag, -2, BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A1_1"))
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(15)
        CALL(evt_btl_camera_set_mode, 0, 13)
        CALL(evt_btl_camera_set_homing_unit_audience, 0, -2, 1, LW(3))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 200)
    END_SIBLING_EVENT
    SET_VALUE(LW(5), LW(1))
    ADD(LW(1), 28)
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 5)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, -2, LW(0), LW(1), LW(2), LW(6))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A1_1"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        DIVIDE(LW(6), 2)
        WAIT_FRAMES(LW(6))
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A1_2"))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_MarioJumpPosition, -2, LW(0), LW(1), LW(2), LW(6), 0, 0)
    CALL(_attack_aud, LW(3), 0)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(btlevtcmd_JumpContinue, -2)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 30, 13)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, 0, 0)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.0f), FLOAT(0.39941406f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, 11, FLOAT(0.39941406f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.0f), FLOAT(1.5f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, 11, FLOAT(1.5f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, -2, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 20)
    ADD(LW(1), 20)
    CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_OnUnitFlag, -2, BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    WAIT_MS(400)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), 0, LW(2), 16, 0, 4, 0, 0)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), FLOAT(6.0f), LW(15))
    ADD(LW(15), 30)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), LW(15), 20, 4, 0, -1)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(attack_good_audience_event)
    CALL(btlevtcmd_OffUnitFlag, -2, BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A1_1"))
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(evt_btl_camera_set_mode, 0, 13)
        CALL(evt_btl_camera_set_homing_unit_audience, 0, -2, 1, LW(3))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 200)
    END_SIBLING_EVENT
    SET_VALUE(LW(5), LW(1))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 30)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.5996094f))
    CALL(btlevtcmd_DivePosition, -2, LW(0), 0, LW(2), 40, 15, 4, 0, 0)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), 20, 0, 4, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A3_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_KISS1"), EVT_NULLPTR, 0, LW(14))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A3_2"))
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 25)
    CALL(evt_eff, STRING(""), STRING("kiss"), 2, LW(0), LW(1), LW(2), LW(6), 0, 0, 0, 0, 0, 0, 0)
    CALL(_attack_aud, LW(3), 1)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, 0, 0)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    WAIT_MS(500)
    CALL(evt_snd_sfxoff, LW(14))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BTL_CLAUD_KISS2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_A3_3"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, -2, LW(0), 0, LW(2), 16, 0, 4, 0, 0)
    CALL(btlevtcmd_SetMoveSpeed, -2, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, -2, LW(0), LW(1), LW(2), FLOAT(6.0f), LW(15))
    ADD(LW(15), 30)
    CALL(btlevtcmd_DivePosition, -2, LW(0), LW(1), LW(2), LW(15), 20, 4, 0, -1)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

static EVT_BEGIN(party_win_reaction)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("PWD_Y_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    RETURN
EVT_END()



USER_FUNC(_clauda_make_extra_work_area) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    unit->countersetWeapon = BattleAlloc(sizeof(BattleWeapon));
    return EVT_RETURN_DONE;
}

static void __makeTechMenuFunc(BattleCommand* command, s32* count) {
    s32 techLv;

    techLv = partyGetTechLv(BattleTransPartyId(BattleGetPartyPtr(_battleWorkPointer)->currentKind));

    command->weapon[*count].index = -1;
    command->weapon[*count].itemId = 0;
    command->weapon[*count].weapon = &partyWeapon_ClaudaNormalAttack;
    command->weapon[*count].enabled = 0;
    command->weapon[*count].icon = command->weapon[*count].weapon->icon;
    command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
    (*count)++;

    if (techLv >= 0) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_ClaudaBreathAttack;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 1) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_ClaudaPredationAttack;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 2) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_ClaudaKumogakureAttack;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
}

USER_FUNC(_check_blow_rate) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 id, out;
    BattleUnit* unit;

    id = evtGetValue(event, args[0]);
    out = args[1];
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    evtSetValue(event, out, unit->vulnStatus->galeForceRate);
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
    if (unit->faceDirection >= 0) {
        eff = effKumokumoN64Entry(0, 60, pos.x, pos.y, pos.z, 0.0f, 2.0f, 1.0f);
    } else {
        eff = effKumokumoN64Entry(0, 60, pos.x, pos.y, pos.z, 180.0f, 2.0f, 1.0f);
    }
    ((EffKumokumoN64Particle*)eff->userdata)->r1 = 76;
    ((EffKumokumoN64Particle*)eff->userdata)->g1 = 167;
    ((EffKumokumoN64Particle*)eff->userdata)->b1 = 202;
    ((EffKumokumoN64Particle*)eff->userdata)->r2 = 125;
    ((EffKumokumoN64Particle*)eff->userdata)->g2 = 27;
    ((EffKumokumoN64Particle*)eff->userdata)->b2 = 245;
    return EVT_RETURN_DONE;
}

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
    if (unit->faceDirection >= 0) {
        eff = effKumokumoN64Entry(0, 0, pos.x, pos.y, pos.z, 0.0f, -2.0f, 1.0f);
    } else {
        eff = effKumokumoN64Entry(0, 0, pos.x, pos.y, pos.z, 180.0f, -2.0f, 1.0f);
    }
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

USER_FUNC(_make_breath_weapon) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 index;
    BattleWeapon* weapon;
    void* src;
    s32 id, value;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    index = args[0];
    weapon = unit->countersetWeapon;
    src = (void*)evtGetValue(event, index);
    value = evtGetValue(event, args[1]);
    memcpy(weapon, src, sizeof(BattleWeapon));
    weapon->galeForceChance = value;
    evtSetValue(event, index, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}

USER_FUNC(_make_kiss_weapon) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 index;
    BattleWeapon* weapon;
    void* src;
    s32 id;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    index = args[0];
    weapon = unit->countersetWeapon;
    src = (void*)evtGetValue(event, index);
    evtGetValue(event, args[1]);
    memcpy(weapon, src, sizeof(BattleWeapon));
    evtSetValue(event, index, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}

USER_FUNC(_get_clauda_kiss_hit_position) {
    EvtWord* args = event->args;
    Vec pos, tmp;
    s32 dir;
    BattleUnit* target;
    BattleWork* wp = _battleWorkPointer;
    s32 targetId;
    s32 outX;
    s32 outY;
    BattleUnit* unit;
    s32 outZ;
    s32 id;

    id = evtGetValue(event, args[0]);
    targetId = evtGetValue(event, args[1]);
    evtGetValue(event, args[2]);
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    target = BattleGetUnitPtr(wp, BattleTransID(event, targetId));
    dir = target->faceDirection;
    BtlUnit_GetPos(target, &pos.x, &pos.y, &pos.z);
    pos.x += dir * (target->kissHitOffset.x * target->mSizeMultiplier);
    pos.y += target->kissHitOffset.y * target->mSizeMultiplier;
    pos.z += target->kissHitOffset.z * target->mSizeMultiplier;
    pos.y -= 30.0f * unit->mSizeMultiplier;
    tmp = pos;
    tmp.y = BattleGetFloorHeight(wp, tmp.x, tmp.y, tmp.z);
    if (tmp.y > pos.y) {
        pos.y = tmp.y;
    }
    evtSetFloat(event, outX, pos.x);
    evtSetFloat(event, outY, pos.y);
    evtSetFloat(event, outZ, pos.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(_make_kumoguard_weapon) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 index;
    BattleWeapon* weapon;
    void* src;
    s32 id, value;

    id = BattleTransID(event, -2);
    unit = BattleGetUnitPtr(wp, id);
    index = args[0];
    weapon = unit->countersetWeapon;
    src = (void*)evtGetValue(event, index);
    value = evtGetValue(event, args[1]);
    memcpy(weapon, src, sizeof(BattleWeapon));
    weapon->dodgyTime = value;
    evtSetValue(event, index, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}
