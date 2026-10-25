#include "system.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"

s32 bts_x_flag_data;


USER_FUNC(bts_x_flag_wait);
USER_FUNC(bts_x_flag_get);
USER_FUNC(bts_x_flag_add);
USER_FUNC(bts_x_flag_set);
static USER_FUNC(bts_wariai_enzan_0f);
static USER_FUNC(bts_wariai_enzan_0);
USER_FUNC(bts_wariai_enzan_1f_intpl_w);
static USER_FUNC(bts_wariai_enzan_1f_intpl);
static USER_FUNC(bts_wariai_enzan_1f);
static USER_FUNC(bts_wariai_enzan_1);

BattleWeapon weapon_batten_satellite_attack;
BattleUnitKind unit_boss_batten_satellite;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[15];
EvtWord dmg_counter_event[149];
static BattleUnitKindPart parts[1];
static EvtWord init_event[30];
static EvtWord entry_event[2];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static EvtWord dead_event[2];
static EvtWord attack_event[10];
EvtWord batten_satellite_attack_attack_event[304];
static EvtWord wait_event[6];

BattleWeapon weapon_batten_satellite_attack = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    0x20000000, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    0x87, // special
    2, // counterResistFlags
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

BattleUnitKind unit_boss_batten_satellite = {
    147,
    "btl_un_batten_satellite",
    3,
    0,
    1,
    1,
    0,
    0,
    1,
    100,
    0,
    50,
    9,
    16,
    16,
    0,
    8,
    0.0f,
    0.0f,
    0.0f,
    -3,
    -6,
    0.0f,
    0.0f,
    0.0f,
    8.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    8.0f,
    0.0f,
    0.0f,
    6.0f,
    7.0f,
    0.0f,
    -1.0f,
    0.0f,
    0.0f,
    16.0f,
    16.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_SATELLITE_DAMAGED1",
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

static StatusVulnerability regist = {0, 75, 90, 0, 10, 100, 100, 75, 100, 75, 100, 75, 100, 95, 75, 0, 90, 100, 75, 100, 100, 90};

static PoseTableEntry pose_table[] = {
    {1, "N_1"},
    {2, "Y_1"},
    {9, "Y_1"},
    {5, "K_1"},
    {4, "X_1"},
    {3, "K_1"},
    {28, "S_1"},
    {29, "Q_1"},
    {30, "Q_1"},
    {31, "D_1"},
    {39, "D_1"},
    {50, "A_1"},
    {42, "S_1"},
    {40, "S_1"},
    {56, "X_1"},
    {57, "X_1"},
    {65, "A_1"},
    {69, "S_1"},
};

static BattleDataEntry data_table[] = {
    {24, dmg_counter_event},
    {25, dmg_counter_event},
    {26, dmg_counter_event},
    {27, dmg_counter_event},
    {28, dmg_counter_event},
    {29, dmg_counter_event},
    {30, dmg_counter_event},
    {31, dmg_counter_event},
    {32, dmg_counter_event},
    {33, dmg_counter_event},
    {34, dmg_counter_event},
    {35, dmg_counter_event},
    {36, dmg_counter_event},
    {50, dead_event},
    {0, NULL},
};

EVT_BEGIN(dmg_counter_event)
    CALL(btlevtcmd_OnUnitFlag, LW(10), 1)
    SWITCH(LW(0))
        CASE_EQUAL(43)
        CALL(evt_snd_sfxon, STRING("SFX_BTL_ATTACK_MISS3"), 0)
        SET_VALUE(LW(14), 36)
        CASE_OR_EQUAL(46)
        CASE_OR_EQUAL(49)
        CASE_OR_EQUAL(50)
        CASE_OR_EQUAL(54)
        SET_VALUE(LW(14), 29)
        CASE_END
        CASE_DEFAULT
        SET_VALUE(LW(14), 39)
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_RunDataEventChild, LW(10), 10)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, LW(10), LW(2), LW(3), LW(4))
    CALL(btlevtcmd_FaceDirectionSub, LW(10), LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePosition, LW(10), LW(2), LW(3), LW(4), 10, 0, 0, 0, -1)
    WAIT_MS(500)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_CheckDamageCode, LW(10), 512, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, LW(10), 2048, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SATELLITE_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, LW(10), LW(5), LW(6), LW(7))
    WAIT_MS(80)
    SET_VALUE(LW(8), 42)
    LOOP(LW(8))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(2), LW(5), LW(12))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(3), LW(6), LW(13))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(4), LW(7), LW(14))
        CALL(btlevtcmd_SetPos, -2, LW(12), LW(13), LW(14))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static BattleUnitKindPart parts[] = {
    {1, "btl_un_batten_satellite", "c_shuryo_p", 0.0f, 0.0f, 0.0f, -4.0f, -2.0f, 0.0f, -6.0f, 1.0f, 0.0f, 14, 14, 255, &defence, &defence_attr, 0x600009, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetHpGaugeOffset, -2, -5, -13)
    CALL(btlevtcmd_OnUnitFlag, -2, 3145728)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(entry_event)
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

static EVT_BEGIN(dead_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    SET_VALUE(LW(9), PTR(&weapon_batten_satellite_attack))
    CALL_CHILD(PTR(batten_satellite_attack_attack_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

EVT_BEGIN(batten_satellite_attack_attack_event)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("F_1"))
    WAIT_MS(400)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, 69)
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
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SATELLITE_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(13), LW(6), LW(7))
    SET_VALUE(LW(8), 9)
    LOOP(LW(8))
        CALL(bts_wariai_enzan_1, LW(8), 9, LW(0), LW(13), LW(10))
        CALL(bts_wariai_enzan_1, LW(8), 9, LW(1), LW(6), LW(11))
        CALL(bts_wariai_enzan_1, LW(8), 9, LW(2), LW(7), LW(12))
        CALL(btlevtcmd_SetPos, -2, LW(10), LW(11), LW(12))
        WAIT_FRAMES(1)
    END_LOOP
    SUBTRACT(LW(13), LW(0))
    DIVIDE(LW(13), 2)
    SUBTRACT(LW(6), LW(1))
    DIVIDE(LW(6), 2)
    SUBTRACT(LW(7), LW(2))
    DIVIDE(LW(7), 2)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(13), LW(0))
    ADD(LW(6), LW(1))
    ADD(LW(7), LW(2))
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(8), 4)
        LOOP(LW(8))
            CALL(bts_wariai_enzan_1, LW(8), 4, LW(0), LW(13), LW(10))
            CALL(bts_wariai_enzan_1, LW(8), 4, LW(1), LW(6), LW(11))
            CALL(bts_wariai_enzan_1, LW(8), 4, LW(2), LW(7), LW(12))
            CALL(btlevtcmd_SetPos, -2, LW(10), LW(11), LW(12))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
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
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_BOSS_SATELLITE_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, -2, LW(0), LW(1), LW(2))
    WAIT_MS(80)
    SET_VALUE(LW(8), 42)
    LOOP(LW(8))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(13), LW(0), LW(10))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(6), LW(1), LW(11))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(7), LW(2), LW(12))
        CALL(btlevtcmd_SetPos, -2, LW(10), LW(11), LW(12))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
    RETURN
EVT_END()

USER_FUNC(bts_x_flag_wait) {
    s32 v = evtGetValue(event, event->args[0]);

    if (v == bts_x_flag_data) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(bts_x_flag_get) {
    evtSetValue(event, event->args[0], bts_x_flag_data);
    return EVT_RETURN_DONE;
}

USER_FUNC(bts_x_flag_add) {
    bts_x_flag_data += evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

USER_FUNC(bts_x_flag_set) {
    bts_x_flag_data = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(bts_wariai_enzan_0f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

static USER_FUNC(bts_wariai_enzan_0) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]) - 1.0f;
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

USER_FUNC(bts_wariai_enzan_1f_intpl_w) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);
    s32 out = args[4];
    f32 t1 = evtGetFloat(event, args[5]);
    f32 t2 = evtGetFloat(event, args[6]);
    f32 r1;
    f32 r2;
    f32 w;

    if (t1 >= 14.0f) {
        t1 -= 14.0f;
        if (t1 >= 14.0f) {
            evtSetFloat(event, out, c);
            return EVT_RETURN_DONE;
        }
        r1 = 1.0f - intplGetValue(t1, 1.0f, 0.0f, b - a, b);
    } else {
        r1 = intplGetValue(t1, 0.0f, 1.0f, b - a, b);
    }
    if (t2 >= 14.0f) {
        t2 = t1 - 14.0f;
        if (t2 >= 14.0f) {
            evtSetFloat(event, out, c);
            return EVT_RETURN_DONE;
        }
        r2 = 1.0f - intplGetValue(t2, 1.0f, 0.0f, b - a, b);
    } else {
        r2 = intplGetValue(t2, 0.0f, 1.0f, b - a, b);
    }
    w = r1 * r2;
    evtSetFloat(event, out, c * (1.0f - w) + d * w);
    return EVT_RETURN_DONE;
}

static USER_FUNC(bts_wariai_enzan_1f_intpl) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);
    s32 out = args[4];
    f32 t = evtGetFloat(event, args[5]);
    f32 r;

    if (t >= 14.0f) {
        t -= 14.0f;
        if (t >= 14.0f) {
            evtSetFloat(event, out, c);
            return EVT_RETURN_DONE;
        }
        r = 1.0f - intplGetValue(t, 1.0f, 0.0f, b - a, b);
    } else {
        r = intplGetValue(t, 0.0f, 1.0f, b - a, b);
    }
    evtSetFloat(event, out, (1.0f - r) * c + r * d);
    return EVT_RETURN_DONE;
}

static USER_FUNC(bts_wariai_enzan_1f) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetFloat(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}

static USER_FUNC(bts_wariai_enzan_1) {
    EvtWord* args = event->args;
    f32 a = evtGetFloat(event, args[0]) - 1.0f;
    f32 b = evtGetFloat(event, args[1]);
    f32 c = evtGetFloat(event, args[2]);
    f32 d = evtGetFloat(event, args[3]);

    evtSetValue(event, args[4], (a * c + d * (b - a)) / b);
    return EVT_RETURN_DONE;
}
