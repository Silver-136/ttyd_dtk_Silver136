#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"
#include "evt/evt_sub.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

static USER_FUNC(teresa_check_move);
static USER_FUNC(teresa_check_teresa);
static USER_FUNC(teresa_check_trans);

BattleUnitKind unit_purple_teresa;
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
    if (unit->currentKind == UNIT_BOO) {
        result = 1;
    }
    if (unit->currentKind == UNIT_DARK_BOO) {
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
            if (unit->currentKind == UNIT_BOO && unit->work[0] == 0) {
                num++;
            }
            if (unit->currentKind == UNIT_DARK_BOO && unit->work[0] == 0) {
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
            if (unit->currentKind == UNIT_BOO && !BtlUnit_CheckStatus(unit, STATUS_INVISIBLE) && i != self) {
                num++;
            }
            if (unit->currentKind == UNIT_DARK_BOO && !BtlUnit_CheckStatus(unit, STATUS_INVISIBLE) && i != self) {
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

BattleUnitKind unit_purple_teresa = {
    UNIT_DARK_BOO, // mUnitType
    "btl_un_purple_teresa", // name
    8, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    24, // mLevel
    0, // mBonusExp
    4, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    34, // width
    32, // height
    {0, 32}, // hitOffset
    {0.0f, 16.0f, 0.0f}, // centerOffset
    {0, -3}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {17.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {17.0f, 0.0f, 0.0f}, // bintaHitOffset
    {17.0f, 20.8f, 0.0f}, // kissHitOffset
    {0.0f, 16.0f, 0.0f}, // cutBaseOffset
    34.0f, // cutWidth
    32.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_TELESA_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitAirborne, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    40, // sleep
    40, // stop
    100, // dizzy
    0, // poison
    40, // confuse
    100, // electric
    100, // burn
    40, // freeze
    100, // huge
    85, // tiny
    100, // attackUp
    85, // attackDown
    100, // defenseUp
    90, // defenseDown
    50, // allergic
    40, // fright
    85, // galeForce
    100, // fast
    0, // slow
    100, // dodgy
    100, // invisible
    10, // ohko
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_purple_teresa", "c_teresa_p", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
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
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
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

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "A_2"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

static BattleDataEntry data_table[] = {
    {0, NULL},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 15)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(normal_attack_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INVISIBLE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(13), 0)
        ELSE
        SET_VALUE(LW(13), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(14), 0)
        ELSE
        CALL(teresa_check_trans, UNIT_ID_SELF, LW(14))
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(0))
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
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(normal_attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_teresa_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_teresa_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_teresa_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_teresa_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
        SET_VALUE(LW(7), 256)
        LOOP(16)
            WAIT_FRAMES(1)
            SUBTRACT(LW(7), 16)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
        END_LOOP
        WAIT_FRAMES(20)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 0)
        SET_VALUE(LW(7), -1)
        LOOP(8)
            WAIT_FRAMES(1)
            ADD(LW(7), 32)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 15)
    ADD(LW(2), 5)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -5, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    WAIT_FRAMES(15)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(9)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    WAIT_FRAMES(15)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_teresa_attack), 0, LW(5))
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
        LABEL(90)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("X_1"))
        WAIT_FRAMES(60)
        GOTO(98)
        LABEL(91)
        CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_add_zoom, 0, -50)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_teresa_attack))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_teresa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
        WAIT_FRAMES(30)
        GOTO(98)
        LABEL(98)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
        CALL(btlevtcmd_ResetFaceDirection, -2)
        RETURN
EVT_END()

static EVT_BEGIN(trans_myself_event)
    SET_VALUE(LW(3), -2)
    SET_VALUE(LW(4), 1)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INVISIBLE, LW(8))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_teresa_trans_myself))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_teresa_trans_myself))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    IF_INT_EQUAL(LW(8), 0)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
        SET_VALUE(LW(7), 256)
        BEGIN_SIBLING_EVENT
            LOOP(16)
                WAIT_FRAMES(1)
                SUBTRACT(LW(7), 8)
                CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_FRAMES(10)
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 15, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 15, 0)
    END_LOOP
    WAIT_FRAMES(12)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_teresa_trans_myself))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_teresa_trans_myself), DAMAGE_RESULT_FINAL_HIT, LW(5))
    RETURN
EVT_END()

static EVT_BEGIN(trans_event)
    LABEL(0)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_teresa_trans))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_teresa_trans), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(teresa_check_teresa, LW(3), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_INVISIBLE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(1)
        END_IF
    END_IF
    GOTO(0)
    LABEL(1)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_teresa_trans))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_teresa_trans))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
    IF_INT_LESS_OR_EQUAL(LW(0), LW(10))
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    ELSE
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    SET_VALUE(LW(7), 256)
    BEGIN_SIBLING_EVENT
        LOOP(16)
            WAIT_FRAMES(1)
            SUBTRACT(LW(7), 16)
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 40)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -5, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 0)
    SET_VALUE(LW(7), -1)
    LOOP(16)
        WAIT_FRAMES(1)
        ADD(LW(7), 16)
        CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, LW(7))
    END_LOOP
    CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 1, 255)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_teresa_trans_myself))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_teresa_trans_myself), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -5, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(move_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_teresa_move))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_teresa_move), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
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
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(move_event_sub)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(1))
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
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, (kPartNotHammerTarget | kPartNotShellTossTarget))
    BEGIN_SIBLING_EVENT
        LOOP(60)
            CALL(btlevtcmd_AddRotate, LW(3), 0, LW(1), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
        IF_INT_EQUAL(LW(3), LW(0))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("T_1"))
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TELESA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPartsPos, LW(3), 1, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 40)
    CALL(btlevtcmd_DivePosition, LW(3), LW(0), LW(1), LW(2), 60, 10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

