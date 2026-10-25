#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"

double sqrt(double x);
void BtlUnit_SetAnim(BattleUnitPart* part, const char* anim);

USER_FUNC(btlevtcmd_SetJumpSound);
static USER_FUNC(chain_init);
static USER_FUNC(chain_main);

BattleUnitKind unit_burst_wanwan;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[19];
static BattleDataEntry data_table[15];
static BattleUnitKindPart parts[9];
static BattleWeapon weapon_wanwan_attack;
static EvtWord init_event[41];
static EvtWord chain_event[23];
static EvtWord damage_event[33];
static EvtWord counter_damage_event[68];
static EvtWord dead_event[200];
static EvtWord attack_event[452];
static EvtWord wait_event[476];
static EvtWord sound_gensui_event[22];
static EvtWord wait_sound_event1[25];
static EvtWord wait_sound_event2[12];

BattleUnitKind unit_burst_wanwan = {
    UNIT_RED_CHOMP, // mUnitType
    "btl_un_burst_wanwan", // name
    6, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    24, // mLevel
    0, // mBonusExp
    1, // bonusCoin
    70, // bonusCoinRate
    3, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    34, // width
    36, // height
    {6, 44}, // hitOffset
    {0.0f, 20.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {17.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {17.0f, 0.0f, 0.0f}, // bintaHitOffset
    {17.0f, 23.4f, 0.0f}, // kissHitOffset
    {0.0f, 18.0f, 0.0f}, // cutBaseOffset
    34.0f, // cutWidth
    36.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_WAN_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    9, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{3, 99, 99, 3, 3}};

static BattleUnitDefenseAttr defence_attr = {{0, 2, 2, 0, 0}};

static StatusVulnerability regist = {0, 70, 65, 100, 65, 100, 0, 70, 100, 90, 100, 90, 100, 95, 65, 45, 30, 100, 65, 100, 100, 95};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "WAN_N_1"},
    {POSE_TYPE_STOP, "WAN_Y_1"},
    {POSE_TYPE_FREEZE, "WAN_Y_1"},
    {POSE_TYPE_CONFUSE, "WAN_K_1"},
    {POSE_TYPE_POISON, "WAN_X_1"},
    {POSE_TYPE_DIZZY, "WAN_X_1"},
    {POSE_TYPE_KO, "WAN_Z_1"},
    {POSE_TYPE_NORMAL, "WAN_S_1A"},
    {POSE_TYPE_DAMAGE_FIRE, "WAN_D_1"},
    {30, "WAN_D_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "WAN_A_1"},
    {POSE_TYPE_DAMAGE, "WAN_D_1"},
    {POSE_TYPE_ATTACK, "WAN_A_1"},
    {POSE_TYPE_RUN, "WAN_R_1"},
    {POSE_TYPE_WALK, "WAN_W_1"},
    {POSE_TYPE_DANGER, "WAN_X_1"},
    {POSE_TYPE_PERIL, "WAN_X_1"},
    {POSE_TYPE_TALK, "WAN_S_1A"},
    {POSE_TYPE_ACTION, "WAN_S_1A"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_COUNTER_FRONT_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PREEMPTIVE_SPIKY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_FIERY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ICY_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIC_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_VOLATILE_EXPLOSIVE_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_POISON_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_ELECTRIFIED_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_PAYBACK_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_HOLD_FAST_EVENT, counter_damage_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, counter_damage_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_burst_wanwan", "c_wanwan_a", 0.0f, 0.0f, 0.0f, 5.0f, 30.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kAbsolutePosition, 0, pose_table},
    {2, "btl_un_burst_wanwan", "c_wanwan_a", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {3, "btl_un_burst_wanwan", "c_wanwan_a", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_burst_wanwan", "c_wanwan_a", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {5, "btl_un_burst_wanwan", "c_wanwan_a", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {6, "btl_un_burst_wanwan", "c_wanwan_a", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {7, "btl_un_burst_wanwan", "c_wanwan_a", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {8, "btl_un_burst_wanwan", "c_wanwan_a", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {9, "btl_un_burst_wanwan", "c_wanwan_a", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
};

static BattleWeapon weapon_wanwan_attack = {
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
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_FIERY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON, // counterResistFlags
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

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, -2, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, -2, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, -2, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, -2, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
    CALL(chain_init, -2)
    SPAWN_EVENT(PTR(chain_event))
    CALL(btlevtcmd_SetJumpSound, -2, STRING("0"), STRING("SFX_ENM_WAN_LANDING1"))
    CALL(btlevtcmd_SetUnitWork, -2, 1, 127)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(chain_event)
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitId, -2, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(chain_main, -2)
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 2)
    CALL(btlevtcmd_OnAttribute, LW(10), 4)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_OffAttribute, LW(10), 4)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    RETURN
EVT_END()

static EVT_BEGIN(counter_damage_event)
    CALL(btlevtcmd_AnimeChangePoseType, -2, 1, POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_SetPartsFallAccel, LW(10), 1, FLOAT(1.0f))
    CALL(btlevtcmd_GetPartsPos, LW(10), 1, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_PartsFaceDirectionSub, LW(10), 1, LW(2), 25)
    ADD(LW(3), 25)
    CALL(btlevtcmd_DivePartsPosition, LW(10), 1, LW(2), LW(3), LW(4), 5, 0, 0, 0, -1)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_CheckDamageCode, -2, 512, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, -2, 2048, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    SET_VALUE(LW(0), 48)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LABEL(10)
    WAIT_FRAMES(1)
    SET_VALUE(LW(11), 1)
    LOOP(9)
        CALL(btlevtcmd_AddPartsRotate, LW(10), LW(11), 0, -8, 0)
        ADD(LW(11), 1)
    END_LOOP
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(10)
    END_IF
    SET_VALUE(LW(11), 1)
    LOOP(9)
        CALL(btlevtcmd_SetPartsRotate, LW(10), LW(11), 0, 0, 0)
        ADD(LW(11), 1)
    END_LOOP
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
    ADD(LW(2), 10)
    ADD(LW(1), 30)
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    SET_VALUE(LW(11), 1)
    LOOP(9)
        CALL(btlevtcmd_SetPartsRotateOffset, LW(10), LW(11), 0, 0, 0)
        ADD(LW(11), 1)
    END_LOOP
    SET_VALUE(LW(0), 30)
    LABEL(20)
    WAIT_FRAMES(1)
    SET_VALUE(LW(11), 1)
    LOOP(9)
        CALL(btlevtcmd_AddPartsRotate, LW(10), LW(11), 3, 0, 0)
        ADD(LW(11), 1)
    END_LOOP
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_CheckDamageCode, LW(10), 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_EnemyItemUseCheck, -2, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, -2)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, -2, LW(0))
        CALL(btlevtcmd_SamplingEnemy, -2, LW(0), PTR(&weapon_wanwan_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_wanwan_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, -2, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 1, 127)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(14))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_wanwan_attack))
    CALL(btlevtcmd_AttackDeclare, -2, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, -2, PTR(&weapon_wanwan_attack))
    CALL(btlevtcmd_CalculateFaceDirection, -2, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangePartsFaceDirection, -2, 1, LW(15))
    CALL(btlevtcmd_SetUnitWork, -2, 0, 0)
    CALL(btlevtcmd_StopWaitEvent, -2)
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, -2, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_A_1"))
    LOOP(2)
        CALL(btlevtcmd_GetPartsPos, -2, 1, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, -2, 1, 20, 0, FLOAT(0.69921875f))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_WAIT1"), LW(14), 0, EVT_NULLPTR)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_WAIT3"), LW(14), 0, EVT_NULLPTR)
            WAIT_FRAMES(10)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_WAIT2"), LW(14), 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), 0, LW(2), 20, -1)
    END_LOOP
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(20)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_S_1A"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_ATTACK1"), LW(14), 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPartsPos, -2, 1, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 40)
    CALL(btlevtcmd_JumpPartsSetting, -2, 1, 30, 0, FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), 0, LW(2), 30, -1)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_A_1"))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_ATTACK2"), LW(14), 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 10)
    IF_INT_LESS(LW(1), 0)
        SET_VALUE(LW(1), 0)
    END_IF
    CALL(btlevtcmd_JumpPartsSetting, -2, 1, 0, FLOAT(16.0f), FLOAT(0.0f))
    CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), LW(1), 10, 0, -1)
    CALL(btlevtcmd_PreCheckDamage, -2, LW(3), LW(4), PTR(&weapon_wanwan_attack), 0, LW(5))
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
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_WAIT1"), LW(14), 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_S_1A"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, -2, 1, 0, FLOAT(8.0f), FLOAT(0.0f))
    CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), LW(1), LW(2), 0, -1)
    GOTO(98)
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_wanwan_attack))
    CALL(btlevtcmd_CheckDamage, -2, LW(3), LW(4), PTR(&weapon_wanwan_attack), 256, LW(5))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_WAIT1"), LW(14), 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_S_1A"))
    CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, -2, 1, 0, FLOAT(8.0f), FLOAT(0.25f))
    CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), LW(1), LW(2), 0, -1)
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    WAIT_MS(250)
    LABEL(99)
    CALL(btlevtcmd_SetUnitWork, -2, 0, 1)
    CALL(btlevtcmd_ChangePartsFaceDirection, -2, 1, -1)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_SetUnitWork, -2, 2, 0)
    CALL(btlevtcmd_CheckStatus, -2, STATUS_SLEEP, LW(15))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_N_1"))
        CALL(btlevtcmd_SetPartsRotateOffset, -2, 1, 0, 10, 0)
        BEGIN_SIBLING_EVENT
            LABEL(1)
            CALL_CHILD(sound_gensui_event)
            CALL(btlevtcmd_SetPartsRotate, -2, 1, 0, 0, 0)
            WAIT_FRAMES(20)
            LOOP(10)
                WAIT_FRAMES(4)
                CALL(btlevtcmd_AddPartsRotate, -2, 1, 0, 0, 1)
            END_LOOP
            LOOP(10)
                WAIT_FRAMES(4)
                CALL(btlevtcmd_AddPartsRotate, -2, 1, 0, 0, -1)
            END_LOOP
            GOTO(1)
        END_SIBLING_EVENT
        GOTO(98)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_STOP, LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_FREEZE, LW(15))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_SetUnitWork, -2, 2, 2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_Y_1"))
        BEGIN_SIBLING_EVENT
            LABEL(5)
            CALL_CHILD(sound_gensui_event)
            CALL(evt_sub_random, 20, LW(0))
            WAIT_FRAMES(LW(0))
            GOTO(5)
        END_SIBLING_EVENT
        GOTO(98)
    END_IF
    CALL(btlevtcmd_CheckStatus, -2, STATUS_CONFUSE, LW(13))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_POISON, LW(14))
    CALL(btlevtcmd_CheckStatus, -2, STATUS_DIZZY, LW(15))
    OR(LW(15), LW(13))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)
        BEGIN_SIBLING_EVENT
            LABEL(3)
            CALL_CHILD(sound_gensui_event)
            CALL(btlevtcmd_AnimeChangePoseFromTable, -2, 1)
            CALL(evt_sub_random, 20, LW(0))
            WAIT_FRAMES(LW(0))
            SET_VALUE(LW(10), 15)
            SPAWN_EVENT(wait_sound_event1)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpPartsSetting, -2, 1, 30, 0, FLOAT(0.25f))
            CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), 0, LW(2), 30, 0)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
            SET_VALUE(LW(10), 10)
            SPAWN_EVENT(wait_sound_event2)
            CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 5)
            CALL(btlevtcmd_JumpPartsSetting, -2, 1, 20, 0, FLOAT(0.19921875f))
            CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), 0, LW(2), 20, 0)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
            SET_VALUE(LW(10), 7)
            SPAWN_EVENT(wait_sound_event2)
            CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
            CALL(btlevtcmd_JumpPartsSetting, -2, 1, 15, 0, FLOAT(0.39941406f))
            CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), 0, LW(2), 15, 0)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
            SET_VALUE(LW(10), 7)
            SPAWN_EVENT(wait_sound_event2)
            CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpPartsSetting, -2, 1, 15, 0, FLOAT(0.19921875f))
            CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), LW(1), LW(2), 15, 0)
            CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
            GOTO(3)
        END_SIBLING_EVENT
        GOTO(98)
    END_IF
    BEGIN_SIBLING_EVENT
        LABEL(4)
        CALL_CHILD(sound_gensui_event)
        CALL(evt_sub_random, 20, LW(0))
        WAIT_FRAMES(LW(0))
        SET_VALUE(LW(10), 15)
        SPAWN_EVENT(wait_sound_event1)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_A_1"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, -2, 1, 30, 0, FLOAT(0.25f))
        CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), 0, LW(2), 30, 0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
        SET_VALUE(LW(10), 10)
        SPAWN_EVENT(wait_sound_event2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_S_1A"))
        CALL(btlevtcmd_FaceDirectionAdd, -2, LW(0), 5)
        CALL(btlevtcmd_JumpPartsSetting, -2, 1, 20, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), 0, LW(2), 20, 0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
        SET_VALUE(LW(10), 7)
        SPAWN_EVENT(wait_sound_event2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_A_1"))
        CALL(btlevtcmd_FaceDirectionSub, -2, LW(0), 20)
        CALL(btlevtcmd_JumpPartsSetting, -2, 1, 15, 0, FLOAT(0.39941406f))
        CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), 0, LW(2), 15, 0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
        SET_VALUE(LW(10), 7)
        SPAWN_EVENT(wait_sound_event2)
        CALL(btlevtcmd_AnimeChangePose, -2, 1, STRING("WAN_S_1A"))
        CALL(btlevtcmd_GetPos, -2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsSetting, -2, 1, 15, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPartsPosition, -2, 1, LW(0), LW(1), LW(2), 15, 0)
        CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_LANDING1"), LW(11), 0, EVT_NULLPTR)
        GOTO(4)
    END_SIBLING_EVENT
    GOTO(98)
    LABEL(98)
    LABEL(99)
    WAIT_FRAMES(1)
    GOTO(99)
    RETURN
EVT_END()

static EVT_BEGIN(sound_gensui_event)
    CALL(btlevtcmd_GetUnitWork, -2, 1, LW(11))
    MULTIPLY_FLOAT(LW(11), FLOAT(0.69921875f))
    IF_INT_LESS(LW(11), 32)
        SET_VALUE(LW(11), 32)
    END_IF
    CALL(btlevtcmd_SetUnitWork, -2, 1, LW(11))
    RETURN
EVT_END()

static EVT_BEGIN(wait_sound_event1)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_WAIT1"), LW(11), 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_WAIT3"), LW(11), 0, EVT_NULLPTR)
    WAIT_FRAMES(LW(10))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_WAIT2"), LW(11), 0, EVT_NULLPTR)
    RETURN
EVT_END()

static EVT_BEGIN(wait_sound_event2)
    WAIT_FRAMES(LW(10))
    CALL(btlevtcmd_snd_se, -2, STRING("SFX_ENM_WAN_WAIT4"), LW(11), 0, EVT_NULLPTR)
    RETURN
EVT_END_PAD()

static USER_FUNC(chain_main) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    BattleUnit* unit;
    BattleUnitPart* part;
    f32 x;
    f32 y;
    f32 z;
    f32 x1;
    f32 y1;
    f32 z1;
    f32 x2;
    f32 y2;
    f32 z2;
    f32 len;
    f32 sag;
    s32 i;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 t;

    id = evtGetValue(event, *args++);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (unit == NULL) {
        return EVT_RETURN_BLOCK;
    }
    part = BtlUnit_GetPartsPtr(unit, 1);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &x, &y, &z);
    BtlUnit_SetPartsPos(BtlUnit_GetPartsPtr(unit, 9), x - 20.0f * unit->faceDirection, y, z);
    // 2: remember the chain's shape relative to the stake
    if (unit->work[2] == 2) {
        BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 9), &x, &y, &z);
        for (i = 8; i >= 1; i--) {
            part = BtlUnit_GetPartsPtr(unit, i);
            BtlUnit_GetPartsPos(part, &x2, &y2, &z2);
            part->work[0] = x2 - x;
            part->work[1] = y2 - y;
        }
        unit->work[2] = 1;
    }
    // 1: keep that shape
    if (unit->work[2] == 1) {
        BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 9), &x, &y, &z);
        for (i = 8; i >= 1; i--) {
            part = BtlUnit_GetPartsPtr(unit, i);
            BtlUnit_SetPartsPos(part, x + part->work[0], y + part->work[1], z);
        }
        return EVT_RETURN_DONE;
    }
    // otherwise hang the links between the chomp and the stake
    BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 1), &x1, &y1, &z1);
    BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 9), &x2, &y2, &z2);
    dx = x1 - x2;
    dy = y1 - y2;
    dz = z1 - z2;
    len = (f32)sqrt(dx * dx + dy * dy + dz * dz) / 8;
    if (len < 10.0f) {
        len = 10.0f;
    }
    sag = 300.0f / (len * len);
    part = BtlUnit_GetPartsPtr(unit, 1);
    BtlUnit_GetPartsPos(part, &x, &y, &z);
    if (y > 110.0f) {
        y = 110.0f;
    }
    BtlUnit_SetPartsPos(part, x, y, z);
    BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, 1), &x, &y, &z);
    x += -(unit->faceDirection * 16);
    y += 16.0f;
    BtlUnit_SetPartsPos(BtlUnit_GetPartsPtr(unit, 2), x, y, z);
    for (i = 2; i <= 7; i++) {
        BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, i), &x1, &y1, &z1);
        part = BtlUnit_GetPartsPtr(unit, i + 1);
        BtlUnit_GetPartsPos(part, &x2, &y2, &z2);
        y2 -= sag;
        dx = x1 - x2;
        dy = y1 - y2;
        dz = z1 - z2;
        t = 1.0f - len / (f32)sqrt(dx * dx + dy * dy + dz * dz);
        x2 += dx * t;
        y2 += dy * t;
        z2 += dz * t;
        if (y2 < 0.0f) {
            y2 = 0.0f;
        }
        BtlUnit_SetPartsPos(part, x2, y2, z2);
    }
    for (i = 9; i >= 3; i--) {
        BtlUnit_GetPartsPos(BtlUnit_GetPartsPtr(unit, i), &x1, &y1, &z1);
        part = BtlUnit_GetPartsPtr(unit, i - 1);
        BtlUnit_GetPartsPos(part, &x2, &y2, &z2);
        y2 -= sag;
        dx = x1 - x2;
        dy = y1 - y2;
        dz = z1 - z2;
        t = 1.0f - len / (f32)sqrt(dx * dx + dy * dy + dz * dz);
        x2 += dx * t;
        y2 += dy * t;
        z2 += dz * t;
        if (y2 < 0.0f) {
            y2 = 0.0f;
        }
        BtlUnit_SetPartsPos(part, x2, y2, z2);
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(chain_init) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    BattleUnit* unit;
    BattleUnitPart* part;
    f32 x;
    f32 y;
    f32 z;
    s32 i;

    id = evtGetValue(event, *args++);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &x, &y, &z);
    // the chain links all start at the chomp
    for (i = 2; i <= 7; i++) {
        part = BtlUnit_GetPartsPtr(unit, i);
        BtlUnit_SetPartsPos(part, x, y, z);
        BtlUnit_SetAnim(part, "WAN_S_1B");
    }
    part = BtlUnit_GetPartsPtr(unit, 8);
    BtlUnit_SetPartsPos(part, x - 20.0f * unit->faceDirection, y, z);
    BtlUnit_SetAnim(part, "WAN_S_1B");
    part = BtlUnit_GetPartsPtr(unit, 9);
    BtlUnit_SetPartsPos(part, x - 20.0f * unit->faceDirection, y, z);
    BtlUnit_SetAnim(part, "WAN_Z_2");
    return EVT_RETURN_DONE;
}

