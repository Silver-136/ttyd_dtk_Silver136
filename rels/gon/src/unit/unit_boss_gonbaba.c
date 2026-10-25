#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"
#include "eff/n64/eff_kemuri9_n64.h"

#include "battle/battle_unit.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include "stdlib.h"
#include "dolphin/mtx.h"
#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_batstage.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"

u8* lightGetPaperCraft(void);

USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetTalkBodyId);
s32 weaponGetPower_Gonbaba(BattleUnit* unit, s32* weapon);
static USER_FUNC(_leg_smoke_effect);
USER_FUNC(_btlgon_get_escape_param);
USER_FUNC(_set_gonbaba_light);
USER_FUNC(_wait_damage_event_end);

BattleWeapon weapon_gonbaba_fumituke_attack;
BattleWeapon weapon_gonbaba_firebreath_attack;
BattleWeapon weapon_gonbaba_kamituki_attack;
BattleUnitKind unit_boss_gonbaba;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[9];
static PoseTableEntry pose_table_weak[9];
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[3];
static EvtWord battle_entry_event[375];
EvtWord negotiation_event[1786];
EvtWord gon_extra_eaten_event[389];
static EvtWord damage_event[10];
EvtWord gonbaba_damage_sub_event[574];
static EvtWord dead_event[283];
static EvtWord init_event[160];
static EvtWord phase_event[2];
static EvtWord attack_event[56];
static EvtWord fumituke_attack_event[386];
EvtWord firebreath_attack_event[261];
static EvtWord wait_event[6];

s32 weaponGetPower_Gonbaba(BattleUnit* unit, s32* weapon) {
    s32 power = weapon[8];

    power -= unit->work[0];
    if (power <= 0) {
        power = 1;
    }
    return power;
}


static USER_FUNC(_leg_smoke_effect) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    effKemuri9N64Entry(1, 18, 90, x, y, z, 60.0f, 0.0f);
    return EVT_RETURN_DONE;
}

BattleWeapon weapon_gonbaba_fumituke_attack = {
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
    weaponGetPower_Gonbaba, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_PRESS_GONBABA, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 10, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_gonbaba_firebreath_attack = {
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
    weaponGetPower_Gonbaba, // hpDamageFn
    {4, 50, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

BattleWeapon weapon_gonbaba_kamituki_attack = {
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
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

BattleUnitKind unit_boss_gonbaba = {
    23,
    "btl_un_gonbaba",
    20,
    0,
    1,
    1,
    65,
    0,
    0,
    100,
    0,
    50,
    9,
    230,
    170,
    -210,
    45,
    0.0f,
    0.0f,
    0.0f,
    -250,
    0,
    280.0f,
    40.0f,
    0.0f,
    115.0f,
    0.0f,
    -10.0f,
#ifdef VERSION_G8ME01
    115.0f,
#else
    0.0f,
#endif
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    250.0f,
    0.0f,
    0.0f,
    115.0f,
    110.5f,
    0.0f,
    -290.0f,
    85.0f,
    0.0f,
    230.0f,
    170.0f,
    0,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_GNB_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0x200000,
    &regist,
    3,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {20, 0, 50, 0, 50, 10, 100, 0, 0, 0, 50, 0, 100, 0, 100, 0, 0, 100, 0, 0, 0, 0};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "GNB_N_1"},
    {POSE_TYPE_CONFUSE, "GNB_X_1"},
    {POSE_TYPE_STOP, "GNB_Z_1"},
    {POSE_TYPE_DIZZY, "GNB_X_1"},
    {POSE_TYPE_NORMAL, "GNB_S_3"},
    {POSE_TYPE_TALK, "GNB_T_3"},
    {POSE_TYPE_DAMAGE_GUARDED, "GNB_S_3"},
    {POSE_TYPE_DAMAGE, "GNB_D_1"},
    {POSE_TYPE_ACTION, "GNB_S_3"},
};

static PoseTableEntry pose_table_weak[] = {
    {POSE_TYPE_SLEEP, "GNB_N_1"},
    {POSE_TYPE_CONFUSE, "GNB_X_1"},
    {POSE_TYPE_STOP, "GNB_N_1"},
    {POSE_TYPE_DIZZY, "GNB_X_1"},
    {POSE_TYPE_NORMAL, "GNB_S_4"},
    {POSE_TYPE_TALK, "GNB_T_4"},
    {POSE_TYPE_DAMAGE_GUARDED, "GNB_S_4"},
    {POSE_TYPE_DAMAGE, "GNB_V_1"},
    {POSE_TYPE_ACTION, "GNB_S_4"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, gonbaba_damage_sub_event},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
#ifdef VERSION_G8ME01
    {1, "btl_un_gonbaba", "c_gonbaba", 0.0f, 0.0f, 0.0f, 230.0f, 36.0f, 60.0f, 230.0f, 36.0f, 60.0f, 20, 30, 255, &defence, &defence_attr, 0x82, 0, pose_table},
    {2, "btl_un_gonbaba", "c_gonbaba", 100.0f, 0.0f, 0.0f, 250.0f, 110.0f, 25.0f, 250.0f, 110.0f, 25.0f, 20, 30, 255, &defence, &defence_attr, 0x1600089, 0, pose_table},
#else
    {1, "btl_un_gonbaba", "c_gonbaba", 0.0f, 0.0f, 0.0f, 230.0f, 36.0f, 60.0f, 230.0f, 36.0f, 60.0f, 20, 30, 255, &defence, &defence_attr, 0x22, 0, pose_table},
    {2, "btl_un_gonbaba", "c_gonbaba", 100.0f, 0.0f, 0.0f, 250.0f, 110.0f, 25.0f, 250.0f, 110.0f, 25.0f, 20, 30, 255, &defence, &defence_attr, 0x1600029, 0, pose_table},
#endif
    {3, "btl_un_gonbaba", "c_gonbaba4", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, 0x1010000, 0, pose_table},
};

static EVT_BEGIN(battle_entry_event)
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_map_replayanim, 0, STRING("dontyo"))
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 0, 110, 1080, 0, 93, -2, 1, 0)
    CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_GetHomePos, -3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -4, LW(3), LW(4), LW(5))
    CALL(btlevtcmd_SetPos, -3, LW(6), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -4, LW(6), LW(4), LW(5))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, LW(3), EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(1), LW(3))
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePoseType, -3, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetBodyId, -4, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -4, LW(0), POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, -3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -4, LW(3), LW(4), LW(5))
    SET_VALUE(LW(8), LW(0))
    SUBTRACT(LW(8), LW(3))
    IF_INT_LESS(LW(8), 0)
        MULTIPLY(LW(8), -1)
    END_IF
    CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
    SET_VALUE(LW(7), LW(6))
    ADD(LW(7), LW(8))
    MULTIPLY(LW(6), -1)
    MULTIPLY(LW(7), -1)
    CALL(btlevtcmd_SetPos, -3, LW(6), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -4, LW(7), LW(4), LW(5))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetMoveSpeed, -4, FLOAT(6.0f))
        CALL(btlevtcmd_MovePosition, -4, LW(3), LW(4), LW(5), 0, -1, 0)
        CALL(btlevtcmd_GetBodyId, -4, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, -4, LW(0), POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, -3, FLOAT(6.0f))
    CALL(btlevtcmd_MovePosition, -3, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_I_Y"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_F_3"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ftomsec, 10, LW(0))
    WAIT_MS(LW(0))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 5, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_I_O"))
        CALL(btlevtcmd_SetFallAccel, -3, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, -3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_snd_se, -3, STRING("SFX_VOICE_MARIO_SURPRISED2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpPosition, -3, LW(0), LW(1), LW(2), 25, -1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 5, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_SetFallAccel, -4, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, -4, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, -4, LW(0), LW(1), LW(2), 25, -1)
        CALL(btlevtcmd_GetBodyId, -4, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, -4, LW(0), POSE_TYPE_CONFUSE)
    END_INLINE_EVENT
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 8, 0, 20, 13)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_I_Y"))
    CALL(btlevtcmd_GetBodyId, -4, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, -4, LW(0), POSE_TYPE_NEUTRAL)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 60, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("GNB_T_3"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("GNB_S_3"))
    CALL(evt_msg_print, 2, STRING("stg1_gon_20_3"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(btlevtcmd_StartWaitEvent, -3)
    CALL(btlevtcmd_StartWaitEvent, -4)
    CALL(btlevtcmd_StartWaitEvent, -2)
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    RETURN
EVT_END()

USER_FUNC(_btlgon_get_escape_param) {
    EvtWord* args = event->args;
    s32 x = evtGetValue(event, args[0]);
    s32 y = evtGetValue(event, args[1]);
    s32 dxVar = args[2];
    s32 dyVar = args[3];
    s32 speedVar = args[4];
    f32 angle = angleABf(100.0f, 205.0f, x, y);
    s32 dx;
    s32 dy;
    f32 speed;

    if (angle > 270.0f && angle < 360.0f) {
        angle = 270.0f;
    }
    if (angle < 90.0f) {
        angle = 90.0f;
    }
    dx = 500.0f * sinfd(angle);
    dy = 500.0f * -cosfd(angle);
    speed = 6.0f + rand() % 6;
    speed = 0.67f * speed;
    evtSetValue(event, dxVar, dx);
    evtSetValue(event, dyVar, dy);
    evtSetFloat(event, speedVar, speed);
    return EVT_RETURN_DONE;
}

EVT_BEGIN(negotiation_event)
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(btlevtcmd_ClearAllStatus, -2)
    CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table))
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_reset_move_color_lv_all)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("GNB_S_4"))
    CALL(evt_msg_print, 2, STRING("stg1_gon_23"), 0, -2)
    LABEL(0)
    CALL(btlevtcmd_GetPos, -3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -3, LW(3), LW(4), LW(5))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -431, 10, 735, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 0)
    CALL(evt_msg_print, 2, STRING("stg1_gon_24"), 0, -2)
    CALL(evt_btl_camera_set_mode, 1, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_24_yesno1"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 0)
        CALL(evt_msg_print, 2, STRING("stg1_gon_25"), 0, -2)
        GOTO(40)
    END_IF
    CALL(btlevtcmd_snd_se, -3, STRING("SFX_VOICE_MARIO_NO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, -3, 1, 512, 1)
    WAIT_MS(667)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 1)
    CALL(evt_msg_print, 2, STRING("stg1_gon_26"), 0, -2)
    CALL(evt_btl_camera_set_mode, 1, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_26_yesno1"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 1)
        CALL(evt_msg_print, 2, STRING("stg1_gon_27"), 0, -2)
        GOTO(40)
    END_IF
    CALL(btlevtcmd_snd_se, -3, STRING("SFX_VOICE_MARIO_NO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, -3, 1, 512, 1)
    WAIT_MS(667)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 2)
    CALL(evt_msg_print, 2, STRING("stg1_gon_28"), 0, -2)
    CALL(evt_btl_camera_set_mode, 1, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_28_yesno1"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 2)
        CALL(evt_msg_print, 2, STRING("stg1_gon_29"), 0, -2)
        GOTO(40)
    END_IF
    CALL(btlevtcmd_snd_se, -3, STRING("SFX_VOICE_MARIO_NO2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, -3, 1, 512, 1)
    WAIT_MS(1000)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 15, 0)
    WAIT_FRAMES(15)
    CALL(evt_msg_print, 2, STRING("stg1_gon_31"), 0, -2)
    GOTO(50)
    LABEL(40)
    CALL(btlevtcmd_GetGuardKouraId, -3, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_RunDataEventChild, LW(3), UNIT_DATA_KOURA_OPEN_EVENT)
        CALL(btlevtcmd_SetUnitWork, LW(3), 2, 0)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, -3, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, -3, FLOAT(3.0f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 400)
    CALL(btlevtcmd_MovePosition, -3, LW(0), 0, LW(2), 0, -1, 0)
    IF_INT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_C_4"))
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, -3, 1, POSE_TYPE_NEUTRAL)
    END_IF
    WAIT_MS(700)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_BITE1"), EVT_NULLPTR, 0, -220, 80, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 15, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_BITE2"), EVT_NULLPTR, 0, -260, 40, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_A_2"))
    CALL(btlevtcmd_ftomsec, 20, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_GetBodyId, -3, LW(4))
    CALL(btlevtcmd_ResultACDefence, -3, PTR(&weapon_gonbaba_kamituki_attack))
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_CheckDamage, -2, -3, LW(4), PTR(&weapon_gonbaba_kamituki_attack), 1048832, LW(5))
    WAIT_MS(500)
    CALL(btlevtcmd_GetHp, -3, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, -3, 2048, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_btl_camera_set_prilimit, 0)
            RETURN
        END_IF
        WAIT_MS(1500)
        CALL(_wait_damage_event_end, -3)
    ELSE
        WAIT_MS(1500)
    END_IF
    CALL(btlevtcmd_GetPos, -3, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, -3, LW(1), EVT_NULLPTR, EVT_NULLPTR)
    LABEL(41)
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        WAIT_FRAMES(1)
        GOTO(41)
    END_IF
    CALL(btlevtcmd_GetGuardKouraId, -3, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_SetUnitWork, LW(3), 2, 1)
    END_IF
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    CALL(evt_msg_print, 2, STRING("stg1_gon_30"), 0, -2)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -115, 50, 265, -115, 49, 170, 1, 0)
    WAIT_FRAMES(5)
    CALL(btlevtcmd_GetUnitKind, -4, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(UNIT_GOOMBELLA)
        CALL(evt_msg_print, 2, STRING("stg1_gon_30_01"), 0, -4)
        CASE_EQUAL(UNIT_KOOPS)
        CALL(evt_msg_print, 2, STRING("stg1_gon_30_02"), 0, -4)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -431, 10, 735, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    CALL(evt_msg_print, 2, STRING("stg1_gon_30_03"), 0, -2)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 20, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_print, 2, STRING("stg1_gon_30_04"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    LABEL(50)
    CALL(evt_btl_camera_set_mode, 1, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 3, 1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_W_4"))
    CALL(btlevtcmd_ftomsec, 35, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -75, 0, 30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 23, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -12, 0, -30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 34, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -31, 0, 30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 23, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, 125, 0, -30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 34, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, 137, 0, 30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 17, LW(0))
    WAIT_MS(LW(0))
    WAIT_FRAMES(120)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(3), EVT_NULLPTR)
    ADD(LW(1), LW(3))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(3))
    ADD(LW(2), LW(3))
    ADD(LW(2), 80)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(0), 260)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(_set_gonbaba_light)
    CALL(evt_btl_camera_shake_h, 1, 7, 0, 6000, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_btl_camera_noshake, 1)
        CALL(evt_btl_camera_shake_h, 1, 7, 0, 30, 12)
    END_INLINE_EVENT
    CALL(evt_batstage_set_aud_dark, 0, 24, 0)
    CALL(evt_batstage_set_aud_dark_base, 0, 24, 0)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_N_7"))
        CALL(btlevtcmd_SetFallAccel, -3, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, -3, LW(0), LW(1), LW(2), 20, -1)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_I_Y"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_SetFallAccel, -4, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, -4, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, -4, LW(0), LW(1), LW(2), 20, -1)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 13)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 14)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 32)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 33)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 34)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 35)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 53)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 54)
    SPAWN_EVENT(gon_extra_eaten_event)
    CALL(btlevtcmd_SetHp, -2, 10)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), 40)
    ADD(LW(1), 180)
    ADD(LW(2), 70)
    CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_E_1"))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_EAT_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, -2, 3, STRING("GNB_P_1"))
        CALL(btlevtcmd_ftomsec, 14, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_OffPartsAttribute, -2, 3, kHidden)
        CALL(btlevtcmd_ftomsec, 56, LW(0))
        WAIT_MS(LW(0))
    END_INLINE_EVENT
    CALL(btlevtcmd_ftomsec, 32, LW(0))
    WAIT_MS(LW(0))
    CALL(evt_snd_sfxon, STRING("SFX_AUDIENCE_GNB_PANIC1"), 0)
    SET_VALUE(LW(0), 0)
    LOOP(100)
        CALL(evt_audience_flag_check, LW(0), 1, LW(4))
        IF_INT_EQUAL(LW(4), 0)
            GOTO(60)
        END_IF
        CALL(evt_audience_flag_check, LW(0), 16, LW(4))
        IF_INT_NOT_EQUAL(LW(4), 0)
            GOTO(60)
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_audience_flag_on, LW(0), 18)
            IF_INT_LESS(LW(0), 40)
                CALL(evt_audience_set_animpose, LW(0), 7, 0)
            ELSE
                CALL(evt_audience_set_animpose, LW(0), 9, 0)
            END_IF
            CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
            CALL(evt_sub_random, 5, LW(5))
            ADD(LW(5), 17)
            ADD(LW(2), LW(5))
            CALL(evt_sub_random, 8, LW(6))
            ADD(LW(6), 4)
            SET_VALUE(LW(11), LW(0))
            MODULO(LW(11), 10)
            IF_INT_NOT_EQUAL(LW(11), 0)
                SET_VALUE(LW(11), -1)
            ELSE
                CALL(evt_sub_random, 2, LW(11))
                SWITCH(LW(11))
                    CASE_EQUAL(0)
                    CASE_EQUAL(1)
                    SET_VALUE(LW(11), -1)
                    CASE_DEFAULT
                    SET_VALUE(LW(11), -1)
                END_SWITCH
            END_IF
            CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), LW(6), FLOAT(0.5f))
            CALL(evt_sub_random, 11, LW(7))
            WAIT_FRAMES(LW(7))
            SUBTRACT(LW(2), LW(5))
            CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), LW(6), FLOAT(0.0f))
            CALL(evt_audience_set_animpose, LW(0), 7, 0)
            CALL(evt_sub_random, 300, LW(6))
            ADD(LW(6), 400)
            WAIT_MS(LW(6))
            IF_INT_EQUAL(LW(0), 43)
                CALL(evt_audience_set_animpose, LW(0), 9, 0)
                CALL(evt_audience_set_rotate, LW(0), -90, 100, 0)
                CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                ADD(LW(2), 2)
                CALL(evt_audience_set_position, LW(0), LW(1), LW(2), LW(3))
                IF_INT_NOT_EQUAL(LW(11), -1)
                    CALL(evt_snd_sfxoff, LW(11))
                END_IF
            ELSE
                CALL(evt_audience_get_position, LW(0), LW(8), LW(9), LW(10))
                SET_VALUE(LW(12), LW(0))
                MODULO(LW(12), 10)
                IF_INT_NOT_EQUAL(LW(12), 0)
                    SET_VALUE(LW(12), -1)
                ELSE
                    CALL(evt_sub_random, 1, LW(12))
                    IF_INT_EQUAL(LW(12), 0)
                        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN1"), LW(8), LW(9), LW(10), LW(12))
                    ELSE
                        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN2"), LW(8), LW(9), LW(10), LW(12))
                    END_IF
                    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
                        LOOP(1000)
                            CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                            CALL(evt_snd_sfx_pos, LW(12), LW(1), LW(2), LW(3))
                            IF_INT_NOT_EQUAL(LW(11), -1)
                                CALL(evt_snd_sfx_pos, LW(11), LW(1), LW(2), LW(3))
                            END_IF
                            WAIT_FRAMES(1)
                        END_LOOP
                    END_SIBLING_EVENT
                END_IF
                CALL(evt_audience_set_animpose, LW(0), 7, 0)
                CALL(_btlgon_get_escape_param, LW(1), LW(3), LW(5), LW(6), LW(7))
                CALL(evt_audience_get_position, LW(0), LW(8), LW(9), LW(10))
                ADD(LW(5), LW(8))
                ADD(LW(6), LW(10))
                CALL(evt_audience_move_position_speed, LW(0), LW(5), LW(2), LW(6), LW(7))
                IF_INT_NOT_EQUAL(LW(11), -1)
                    CALL(evt_snd_sfxoff, LW(11))
                END_IF
                IF_INT_NOT_EQUAL(LW(12), -1)
                    DELETE_EVENT(LW(13))
                    CALL(evt_snd_sfxoff, LW(12))
                END_IF
                CALL(evt_audience_delete, LW(0))
                CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
            END_IF
        END_INLINE_EVENT
        LABEL(60)
        ADD(LW(0), 1)
    END_LOOP
    LOOP(100)
        CALL(evt_audience_delete, LW(0))
        CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
        ADD(LW(0), 1)
    END_LOOP
    CALL(btlevtcmd_ftomsec, 38, LW(0))
    WAIT_MS(LW(0))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 4, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 45, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 44, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 0, 140, 900, 0, 60, 0, 30, 0)
    CALL(btlevtcmd_ftomsec, 284, LW(0))
    WAIT_MS(LW(0))
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -115, 50, 200, -115, -30, -670, 20, 0)
    WAIT_FRAMES(25)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_S_5"))
    CALL(btlevtcmd_OnPartsAttribute, -2, 3, kHidden)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_GetUnitKind, -4, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(UNIT_GOOMBELLA)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_00_kur"), 0, -4)
        CASE_EQUAL(UNIT_KOOPS)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_00_nok"), 0, -4)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 40, 98, 686, 57, 125, 182, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_SetTogeOffset, -2, 310, 60, 0)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("GNB_T_5"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("GNB_S_5"))
    CALL(evt_msg_print, 2, STRING("stg1_gon_31_01"), 0, -2)
    CALL(btlevtcmd_SetTogeOffset, -2, 0, 0, 0)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -115, 40, 200, -115, 39, 170, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_S_3"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 260)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_I_Y"))
    CALL(btlevtcmd_GetUnitKind, -4, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(UNIT_GOOMBELLA)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_02"), 0, -4)
        CASE_EQUAL(UNIT_KOOPS)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_03"), 0, -4)
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 0, 110, 860, 0, 60, 170, 20, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_GetBodyId, -4, LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, -4, LW(6), POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, -3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, -4, LW(3), LW(4), LW(5))
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(6), EVT_NULLPTR)
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(7))
    BEGIN_INLINE_EVENT
        ADD(LW(4), LW(6))
        ADD(LW(5), LW(7))
        ADD(LW(5), 80)
        CALL(btlevtcmd_SetFallAccel, -4, FLOAT(0.69921875f))
        CALL(btlevtcmd_JumpPosition, -4, LW(3), LW(4), LW(5), 27, -1)
        CALL(btlevtcmd_SetHomePos, -4, LW(3), LW(4), LW(5))
    END_INLINE_EVENT
    ADD(LW(1), LW(6))
    ADD(LW(2), LW(7))
    ADD(LW(2), 80)
    CALL(btlevtcmd_SetFallAccel, -3, FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePose, -3, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_snd_se, -3, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, -3, LW(0), LW(1), LW(2), 27, 0)
    CALL(btlevtcmd_SetHomePos, -3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, -3, STRING("SFX_MARIO_JUMP_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetBodyId, -3, LW(3))
    CALL(btlevtcmd_AnimeChangePose, -3, LW(3), STRING("M_I_Y"))
    CALL(btlevtcmd_GetBodyId, -4, LW(4))
    CALL(btlevtcmd_AnimeChangePoseType, -4, LW(4), POSE_TYPE_ACTION)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_reset_turn)
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(btlevtcmd_onoff_battleflag, 1, 256)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, -3)
    CALL(btlevtcmd_StartWaitEvent, -4)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

USER_FUNC(_set_gonbaba_light) {
    u8* light = lightGetPaperCraft();
    Mtx mtx;

    *(Vec*)(light + 0x24) = (Vec){-200.0f, 50.0f, 500.0f};
    PSMTXScale(mtx, 3.0f, 3.0f, 3.0f);
    PSMTXMultVec(mtx, (Vec*)(light + 0x24), (Vec*)(light + 0x24));
    return EVT_RETURN_DONE;
}

EVT_BEGIN(gon_extra_eaten_event)
    WAIT_FRAMES(14)
    CALL(btlevtcmd_SetUnitWork, -2, 4, 6)
    CALL(evt_audience_flag_on, LW(0), 18)
    CALL(evt_audience_flag_off, LW(0), 4)
    CALL(evt_audience_set_animpose, LW(0), 9, 0)
    CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(2), 40)
    CALL(evt_audience_set_position, LW(0), LW(1), LW(2), LW(3))
    WAIT_FRAMES(1)
    ADD(LW(2), 80)
    SUBTRACT(LW(1), 80)
    CALL(evt_audience_move_position_speed, LW(0), LW(1), LW(2), LW(3), FLOAT(10.0f))
    IF_INT_EQUAL(LW(0), 32)
        CALL(evt_audience_flag_on, LW(0), 4)
        BEGIN_INLINE_EVENT
            CALL(evt_audience_set_rotate_offset, LW(0), 0, 18, 0)
            SET_VALUE(LW(4), 0)
            SET_VALUE(LW(5), 0)
            LOOP(45)
                ADD(LW(4), 2)
                ADD(LW(5), 15)
                CALL(evt_audience_set_rotate, LW(0), LW(4), LW(5), 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
        CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(2), EVT_NULLPTR)
        SUBTRACT(LW(1), 40)
        ADD(LW(2), 1)
        CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), 60, FLOAT(1.0f))
        CALL(evt_audience_set_rotate_offset, LW(0), 0, 0, 0)
        CALL(evt_audience_set_rotate, LW(0), 90, 0, 0)
        WAIT_FRAMES(30)
        CALL(evt_audience_set_rotate_offset, LW(0), 0, 0, 0)
        SET_VALUE(LW(1), 90)
        LOOP(90)
            SUBTRACT(LW(1), 1)
            CALL(evt_audience_set_rotate, LW(0), LW(1), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_audience_set_rotate, LW(0), 0, 0, 0)
        CALL(evt_audience_set_animpose, LW(0), 7, 0)
        WAIT_FRAMES(10)
        CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_SCREAM2"), LW(1), LW(2), LW(3), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN1"), LW(1), LW(2), LW(3), LW(5))
        BEGIN_SIBLING_EVENT_GET_ID(LW(6))
            LOOP(1000)
                CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                CALL(evt_snd_sfx_pos, LW(5), LW(1), LW(2), LW(3))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(evt_audience_move_position_speed, LW(0), -300, LW(2), LW(3), FLOAT(4.5f))
        DELETE_EVENT(LW(6))
        CALL(evt_snd_sfxoff, LW(5))
        CALL(evt_audience_delete, LW(0))
        RETURN
    END_IF
    IF_INT_EQUAL(LW(0), 53)
        CALL(evt_audience_flag_on, LW(0), 4)
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(4), 0)
            LOOP(45)
                ADD(LW(4), 2)
                CALL(evt_audience_set_rotate, LW(0), LW(4), 0, 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
        CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(2), EVT_NULLPTR)
        ADD(LW(3), 40)
        ADD(LW(2), 1)
        CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), 60, FLOAT(1.0f))
        CALL(evt_audience_set_rotate, LW(0), 90, 0, 0)
        WAIT_FRAMES(30)
        CALL(evt_audience_set_rotate, LW(0), 0, 0, 0)
        CALL(evt_audience_set_rotate_offset, LW(0), 0, 0, 0)
        SET_VALUE(LW(1), 90)
        LOOP(90)
            SUBTRACT(LW(1), 1)
            CALL(evt_audience_set_rotate, LW(0), LW(1), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_audience_set_rotate, LW(0), 0, 0, 0)
        CALL(evt_audience_set_animpose, LW(0), 7, 0)
        WAIT_FRAMES(10)
        CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_SCREAM3"), LW(1), LW(2), LW(3), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN2"), LW(1), LW(2), LW(3), LW(5))
        BEGIN_SIBLING_EVENT_GET_ID(LW(6))
            LOOP(1000)
                CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                CALL(evt_snd_sfx_pos, LW(5), LW(1), LW(2), LW(3))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(evt_audience_move_position_speed, LW(0), -300, LW(2), LW(3), FLOAT(4.5f))
        DELETE_EVENT(LW(6))
        CALL(evt_snd_sfxoff, LW(5))
        CALL(evt_audience_delete, LW(0))
        CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
        RETURN
    END_IF
    CALL(evt_audience_delete, LW(0))
    CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
    RETURN
EVT_END()

USER_FUNC(_wait_damage_event_end) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    BattleUnit* unit;

    id = evtGetValue(event, *args++);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    if (unit->damageEventId != 0 && evtCheckID(unit->damageEventId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

EVT_BEGIN(gonbaba_damage_sub_event)
    CALL(btlevtcmd_CheckDamageCode, -2, 16777216, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, -2, 0, LW(0))
        IF_INT_LESS(LW(0), 20)
            ADD(LW(0), 1)
        END_IF
        CALL(btlevtcmd_SetUnitWork, -2, 0, LW(0))
        CALL(btlevtcmd_GetUnitWork, -2, 1, LW(0))
        IF_INT_GREATER(LW(0), -2)
            SUBTRACT(LW(0), 1)
        END_IF
        CALL(btlevtcmd_SetUnitWork, -2, 1, LW(0))
        CALL(btlevtcmd_AnimeSetPoseTable, -2, 1, PTR(pose_table_weak))
    END_IF
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, -2, 16777216, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(90)
        END_IF
    END_IF
    CALL(btlevtcmd_GetDamagePartsId, -2, LW(11))
    IF_INT_EQUAL(LW(11), 1)
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("GNB_D_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("GNB_D_1"))
        CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
        SET_VALUE(LW(9), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                SUBTRACT(LW(0), 350)
                ADD(LW(1), 20)
                CALL(evt_audience_entry, LW(9), 0)
                CALL(evt_audience_set_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_audience_flag_on, LW(9), 22)
                CALL(evt_audience_set_animpose, LW(9), 9, 0)
                CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
                SUBTRACT(LW(0), 50)
                CALL(evt_audience_jump_position_firstsp, LW(9), LW(0), LW(1), LW(2), 60, FLOAT(1.0f))
                CALL(evt_audience_set_animpose, LW(9), 7, 0)
                CALL(evt_audience_get_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_RUN1"), LW(0), LW(1), LW(2), LW(7))
                CALL(evt_audience_move_position_speed, LW(9), -300, LW(1), LW(2), FLOAT(4.5f))
                CALL(evt_snd_sfxoff, LW(7))
                CALL(evt_audience_delete, LW(9))
            END_INLINE_EVENT
        END_IF
        CALL(btlevtcmd_GetUnitWork, -2, 4, LW(0))
        SET_VALUE(LW(9), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, -2, 4, LW(0))
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
                SUBTRACT(LW(0), 320)
                ADD(LW(1), 30)
                CALL(evt_audience_entry, LW(9), 0)
                CALL(evt_audience_set_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_audience_flag_on, LW(9), 22)
                CALL(evt_audience_set_animpose, LW(9), 9, 0)
                CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
                SUBTRACT(LW(0), 10)
                ADD(LW(2), 30)
                CALL(evt_audience_jump_position_firstsp, LW(9), LW(0), LW(1), LW(2), 54, FLOAT(1.0f))
                CALL(evt_audience_set_animpose, LW(9), 7, 0)
                CALL(evt_audience_get_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_RUN1"), LW(0), LW(1), LW(2), LW(7))
                CALL(evt_audience_move_position_speed, LW(9), -300, LW(1), LW(2), FLOAT(4.5f))
                CALL(evt_snd_sfxoff, LW(7))
                CALL(evt_audience_delete, LW(9))
            END_INLINE_EVENT
        END_IF
    END_IF
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDamageCode, -2, 256, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamageCode, -2, 16777216, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 2)
        RETURN
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetHp, -2, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("GNB_S_4"))
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 1)
        CALL(evt_msg_print, 2, STRING("stg1_gon_21"), 0, -2)
        CASE_EQUAL(1)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 2)
        CALL(evt_msg_print, 2, STRING("stg1_gon_22"), 0, -2)
    END_SWITCH
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
    LABEL(50)
    CALL(btlevtcmd_GetUnitWork, -2, 2, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 3)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 2, 3)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("GNB_S_4"))
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(evt_btl_camera_set_mode, 1, 3)
    SET_VALUE(LW(1), 160)
    SET_VALUE(LW(2), 536)
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    ADD(LW(1), LW(5))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(5))
    ADD(LW(2), LW(5))
    ADD(LW(2), 80)
    SET_VALUE(LW(5), LW(1))
    SUBTRACT(LW(5), 37)
    SET_VALUE(LW(6), LW(2))
    SUBTRACT(LW(6), 511)
    CALL(evt_btl_camera_set_moveto, 1, 93, LW(1), LW(2), 93, LW(5), LW(6), 20, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_print, 2, STRING("stg1_gon_31_04"), 0, -2)
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
    LABEL(90)
    CALL(btlevtcmd_GetDamagePartsId, -2, LW(0))
    CALL(btlevtcmd_GetHitPos, -2, LW(0), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_ATTACK_MISS2"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetUnitWork, -2, 3, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(negotiation_event)
        RETURN
    END_IF
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_X_1"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_DOWN1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(evt_btl_camera_set_mode, 1, 3)
    SET_VALUE(LW(1), 160)
    SET_VALUE(LW(2), 536)
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    ADD(LW(1), LW(5))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(5))
    ADD(LW(2), LW(5))
    ADD(LW(2), 80)
    SET_VALUE(LW(5), LW(1))
    SUBTRACT(LW(5), 37)
    SET_VALUE(LW(6), LW(2))
    SUBTRACT(LW(6), 511)
    CALL(evt_btl_camera_set_moveto, 1, 93, LW(1), LW(2), 93, LW(5), LW(6), 30, 0)
    WAIT_MS(1500)
    CALL(btlevtcmd_SetTalkPose, -2, STRING("GNB_X_1"))
    CALL(btlevtcmd_SetStayPose, -2, STRING("GNB_X_1"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 310)
    ADD(LW(1), 60)
    CALL(evt_msg_toge, 2, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_32"), 0, 0)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -38, 33, 1005, -38, 92, 13, 60, 0)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_DOWN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_D_3"))
    WAIT_FRAMES(42)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_H_1"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    ADD(LW(0), 25)
    CALL(btlevtcmd_SetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_H_1"))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(4), 120)
        CALL(btlevtcmd_SetRotateOffset, -2, 0, LW(5), 0)
        LOOP(40)
            CALL(btlevtcmd_SetRotate, -2, 0, 0, LW(4))
            SUBTRACT(LW(4), 3)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, -2, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_SetFallAccel, -2, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 300)
    CALL(btlevtcmd_JumpPosition, -2, LW(0), LW(1), LW(2), 90, -1)
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_GNB_DOWN3"), 0)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_H_2"))
    CALL(evt_btl_camera_shake_h, 1, 8, 0, 30, 13)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_H_3"))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_FRAMES(60)
    CALL(btlevtcmd_KillUnit, -2, 1)
    CALL(evt_btl_camera_set_prilimit, 0)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, -2, 16777216)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetTalkBodyId, -2, 1)
    CALL(evt_audience_delete, 13)
    CALL(evt_audience_entry, 13, 0)
    CALL(evt_audience_delete, 14)
    CALL(evt_audience_entry, 14, 0)
    CALL(evt_audience_delete, 32)
    CALL(evt_audience_entry, 32, 0)
    CALL(evt_audience_delete, 33)
    CALL(evt_audience_entry, 33, 0)
    CALL(evt_audience_delete, 34)
    CALL(evt_audience_entry, 34, 0)
    CALL(evt_audience_delete, 35)
    CALL(evt_audience_entry, 35, 0)
    CALL(evt_audience_delete, 53)
    CALL(evt_audience_entry, 53, 0)
    CALL(evt_audience_delete, 54)
    CALL(evt_audience_entry, 54, 0)
    CALL(evt_audience_flag_on, 13, 512)
    CALL(evt_audience_flag_on, 14, 512)
    CALL(evt_audience_flag_on, 32, 512)
    CALL(evt_audience_flag_on, 33, 512)
    CALL(evt_audience_flag_on, 34, 512)
    CALL(evt_audience_flag_on, 35, 512)
    CALL(evt_audience_flag_on, 53, 512)
    CALL(evt_audience_flag_on, 54, 512)
    CALL(evt_audience_base_flag_on, 16384)
    CALL(btlevtcmd_SetEventEntry, -2, PTR(battle_entry_event))
    CALL(btlevtcmd_SetBaseScale, -2, FLOAT(1.2998047f), FLOAT(1.2998047f), FLOAT(0.5f))
    CALL(btlevtcmd_SetUnitWork, -2, 3, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 1, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 4, 0)
    CALL(btlevtcmd_SetUnitWork, -2, 5, 0)
    CALL(btlevtcmd_StartWaitEvent, -2)
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
    CALL(btlevtcmd_GetUnitWork, -2, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_SetUnitWork, -2, 5, 1)
        CALL_CHILD(PTR(firebreath_attack_event))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, -2, 5, 2)
        CALL_CHILD(PTR(fumituke_attack_event))
        CASE_DEFAULT
        CALL(evt_sub_random, 99, LW(0))
        IF_INT_LESS(LW(0), 50)
            CALL_CHILD(PTR(fumituke_attack_event))
            ELSE
            CALL_CHILD(PTR(firebreath_attack_event))
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(fumituke_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_gonbaba_fumituke_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_gonbaba_fumituke_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_gonbaba_fumituke_attack))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_W_2"))
    CALL(btlevtcmd_ftomsec, 56, LW(0))
    WAIT_MS(LW(0))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -300, 0, -30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(0), -300)
        ADD(LW(2), -30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 46, LW(0))
    WAIT_MS(LW(0))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -300, 0, 30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(0), -300)
        ADD(LW(2), 30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 1, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_A_3"))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_ARM_UP1"), EVT_NULLPTR, 0, -320, 30, 30, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 42, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_ARM_DOWN1"), EVT_NULLPTR, 0, -360, 50, 30, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(btlevtcmd_ftomsec, 47, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_gonbaba_fumituke_attack), 256, LW(5))
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
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gonbaba_fumituke_attack))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_gonbaba_fumituke_attack), 256, LW(5))
    GOTO(97)
    LABEL(97)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -400, 0, -5, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(0), -400)
        ADD(LW(2), -5)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    WAIT_FRAMES(3)
    CALL(evt_btl_camera_shake_h, 0, 5, 0, 10, 13)
    WAIT_FRAMES(7)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gonbaba_fumituke_attack))
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 30, 13)
    WAIT_FRAMES(35)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_W_3"))
    CALL(btlevtcmd_ftomsec, 51, LW(0))
    WAIT_MS(LW(0))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -180, 0, 30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(0), -180)
        ADD(LW(2), 30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 46, LW(0))
    WAIT_MS(LW(0))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, -2, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -100, 0, 30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        ADD(LW(0), -100)
        ADD(LW(2), -30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    CALL(btlevtcmd_AnimeWaitPlayComplete, -2, 1)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(firebreath_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
    CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_gonbaba_firebreath_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_gonbaba_firebreath_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gonbaba_firebreath_attack))
    CALL(btlevtcmd_AttackDeclareAll, -2)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_gonbaba_firebreath_attack))
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("GNB_A_1"))
    SET_VALUE(LW(10), 120)
    SET_VALUE(LW(11), 30)
    CALL(btlevtcmd_GetPos, -2, LW(6), LW(7), LW(8))
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(6), 360)
    ADD(LW(7), 30)
    CALL(btlevtcmd_GetPos, -2, LW(9), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(9), 360)
    CALL(btlevtcmd_FaceDirectionAdd, -2, LW(9), LW(10))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 30, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_NECK_UP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 105, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_GNB_BREATH_FIRE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        ADD(LW(11), 45)
        SUBTRACT(LW(8), 10)
        CALL(evt_eff, 0, STRING("gonbaba_breath"), 0, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(12), EVT_NULLPTR, EVT_NULLPTR)
    SUBTRACT(LW(12), LW(6))
    IF_INT_LESS(LW(12), 0)
        MULTIPLY(LW(12), -1)
    END_IF
    ADD(LW(12), 20)
    MULTIPLY(LW(12), LW(11))
    DIVIDE(LW(12), LW(10))
    ADD(LW(12), 105)
    WAIT_FRAMES(LW(12))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_gonbaba_firebreath_attack), 256, LW(5))
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
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gonbaba_firebreath_attack))
    END_IF
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_gonbaba_firebreath_attack), 256, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        WAIT_FRAMES(20)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    WAIT_FRAMES(84)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

