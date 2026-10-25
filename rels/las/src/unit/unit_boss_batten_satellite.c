#include "system.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_sub.h"

#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_damage.h"

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
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
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
    UNIT_GRODUS_X, // mUnitType
    "btl_un_batten_satellite", // name
    3, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    0, // mLevel
    0, // mBonusExp
    1, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9, // pbSoftCap
    16, // width
    16, // height
    {0, 8}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {-3, -6}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {8.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {8.0f, 0.0f, 0.0f}, // bintaHitOffset
    {6.0f, 7.0f, 0.0f}, // kissHitOffset
    {-1.0f, 0.0f, 0.0f}, // cutBaseOffset
    16.0f, // cutWidth
    16.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_SATELLITE_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    0, // sleep
    75, // stop
    90, // dizzy
    0, // poison
    10, // confuse
    100, // electric
    100, // burn
    75, // freeze
    100, // huge
    75, // tiny
    100, // attackUp
    75, // attackDown
    100, // defenseUp
    95, // defenseDown
    75, // allergic
    0, // fright
    90, // galeForce
    100, // fast
    75, // slow
    100, // dodgy
    100, // invisible
    90, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "K_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {30, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "D_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_ATTACK, "A_1"},
    {POSE_TYPE_RUN, "S_1"},
    {POSE_TYPE_WALK, "S_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "A_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_COUNTER_TOP_SPIKY_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, dmg_counter_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, dmg_counter_event},
    {UNIT_DATA_PRE_DEAD_EVENT, dead_event},
    {0, NULL},
};

EVT_BEGIN(dmg_counter_event)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_SHOW_HP)
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SATELLITE_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, LW(10), LW(5), LW(6), LW(7))
    WAIT_MS(80)
    SET_VALUE(LW(8), 42)
    LOOP(LW(8))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(2), LW(5), LW(12))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(3), LW(6), LW(13))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(4), LW(7), LW(14))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(12), LW(13), LW(14))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static BattleUnitKindPart parts[] = {
    {1, "btl_un_batten_satellite", "c_shuryo_p", 0.0f, 0.0f, 0.0f, -4.0f, -2.0f, 0.0f, -6.0f, 1.0f, 0.0f, 14, 14, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetHpGaugeOffset, UNIT_ID_SELF, -5, -13)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_FLOAT_CAPABLE | BATTLE_UNIT_FLAG_FLOAT_ACTIVE)
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
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("F_1"))
    WAIT_MS(400)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SATELLITE_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(13), LW(6), LW(7))
    SET_VALUE(LW(8), 9)
    LOOP(LW(8))
        CALL(bts_wariai_enzan_1, LW(8), 9, LW(0), LW(13), LW(10))
        CALL(bts_wariai_enzan_1, LW(8), 9, LW(1), LW(6), LW(11))
        CALL(bts_wariai_enzan_1, LW(8), 9, LW(2), LW(7), LW(12))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
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
            CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
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
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 256, LW(5))
    LABEL(98)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_SATELLITE_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_MS(80)
    SET_VALUE(LW(8), 42)
    LOOP(LW(8))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(13), LW(0), LW(10))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(6), LW(1), LW(11))
        CALL(bts_wariai_enzan_1, LW(8), 42, LW(7), LW(2), LW(12))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
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
