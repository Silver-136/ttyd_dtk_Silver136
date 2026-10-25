#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "evt/evt_eff.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"


BattleUnitKind unit_yamitogedaruma;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefense defence_turn;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_turn[15];
static BattleDataEntry data_table[6];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_togedaruma_attack;
static EvtWord init_event[21];
static EvtWord dmg_avoid_counter_thorn_head_event[63];
static EvtWord dmg_avoid_counter_thorn_front_event[121];
static EvtWord damage_event[10];
static EvtWord dmg_turn_event[131];
static EvtWord wakeup_event[134];
static EvtWord attack_event[358];
static EvtWord wait_event[102];

BattleUnitKind unit_yamitogedaruma = {
    UNIT_DARK_BRISTLE, // mUnitType
    "btl_un_yamitogedaruma", // name
    8, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    34, // mLevel
    1, // mBonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    2, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    46, // width
    40, // height
    {4, 40}, // hitOffset
    {0.0f, 20.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {23.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {23.0f, 0.0f, 0.0f}, // bintaHitOffset
    {20.0f, 26.0f, 0.0f}, // kissHitOffset
    {0.0f, 20.0f, 0.0f}, // cutBaseOffset
    46.0f, // cutWidth
    40.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_TOGEDA_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{4, VERSION_PICK(4, 99), 4, 4, 4}};

static BattleUnitDefenseAttr defence_attr = {{0, VERSION_PICK(0, 2), 0, 0, 0}};

static BattleUnitDefense defence_turn = {0};

static StatusVulnerability regist = {
    20, // sleep
    80, // stop
    20, // dizzy
    100, // poison
    20, // confuse
    100, // electric
    VERSION_PICK(100, 0), // burn
    40, // freeze
    100, // huge
    50, // tiny
    100, // attackUp
    50, // attackDown
    100, // defenseUp
    100, // defenseDown
    40, // allergic
    30, // fright
    20, // galeForce
    100, // fast
    40, // slow
    100, // dodgy
    100, // invisible
    30, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "TGD_N_1"},
    {POSE_TYPE_STOP, "TGD_I_1"},
    {POSE_TYPE_FREEZE, "TGD_I_1"},
    {POSE_TYPE_CONFUSE, "TGD_K_1"},
    {POSE_TYPE_POISON, "TGD_X_1"},
    {POSE_TYPE_DIZZY, "TGD_X_1"},
    {POSE_TYPE_NORMAL, "TGD_S_4"},
    {POSE_TYPE_DAMAGE_FIRE, "TGD_Q_1"},
    {30, "TGD_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "TGD_S_2"},
    {POSE_TYPE_DAMAGE, "TGD_D_1"},
    {POSE_TYPE_ATTACK, "TGD_A_1"},
    {POSE_TYPE_WALK, "TGD_S_4"},
    {POSE_TYPE_RUN, "TGD_S_4"},
    {POSE_TYPE_DANGER, "TGD_X_1"},
    {POSE_TYPE_PERIL, "TGD_X_1"},
    {POSE_TYPE_TALK, "TGD_T_1"},
    {POSE_TYPE_ACTION, "TGD_S_4"},
};

static PoseTableEntry pose_table_turn[] = {
    {POSE_TYPE_SLEEP, "TGD_N_2"},
    {POSE_TYPE_STOP, "TGD_I_2"},
    {POSE_TYPE_FREEZE, "TGD_Z_4"},
    {POSE_TYPE_CONFUSE, "TGD_K_2"},
    {POSE_TYPE_POISON, "TGD_X_2"},
    {POSE_TYPE_DIZZY, "TGD_X_2"},
    {POSE_TYPE_NORMAL, "TGD_S_5"},
    {POSE_TYPE_DAMAGE_FIRE, "TGD_Q_2"},
    {30, "TGD_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "TGD_S_3"},
    {POSE_TYPE_DAMAGE, "TGD_D_2"},
    {POSE_TYPE_DANGER, "TGD_X_2"},
    {POSE_TYPE_PERIL, "TGD_X_2"},
    {POSE_TYPE_TALK, "TGD_S_5"},
    {POSE_TYPE_ACTION, "TGD_S_5"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_head_event},
    {UNIT_DATA_AVOID_THORN_EVENT, dmg_avoid_counter_thorn_front_event},
    {UNIT_DATA_AVOID_THORN_FRONT_EVENT, dmg_avoid_counter_thorn_front_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_yamitogedaruma", "c_togedaruma_b", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kIsBombFlippable, kCounterTopSpiky | kCounterPreemptiveFrontSpiky | kCounterFrontSpiky, pose_table},
};

static BattleWeapon weapon_togedaruma_attack = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY, // counterResistFlags
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
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 58)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 32)
        GOTO(90)
    END_IF
    ADD(LW(1), 45)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_avoid_counter_thorn_front_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FLIPPED, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_3"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_4"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_1"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_2"))
        END_IF
    END_SIBLING_EVENT
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 53)
        ADD(LW(1), 26)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 33)
        ADD(LW(1), 16)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 43)
    ADD(LW(1), 21)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(60)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(12))
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("TGD_S_1"))
    IF_INT_LESS_OR_EQUAL(LW(12), 0)
        SET_VALUE(LW(12), 0)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 10, 0)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                IF_INT_GREATER_OR_EQUAL(LW(12), 10)
                    BREAK_LOOP
                END_IF
                ADD(LW(12), 1)
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -18)
            END_LOOP
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 25, -1)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("TGD_S_5"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 10, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 10, LW(2))
    RETURN
EVT_END()

static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence))
    CALL(btlevtcmd_OffStatusFlag, LW(10), 1)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    SET_VALUE(LW(12), 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 10, 0)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            IF_INT_GREATER_OR_EQUAL(LW(12), 10)
                BREAK_LOOP
            END_IF
            ADD(LW(12), 1)
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, -18)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_ENM_TOGEDA_MOVE6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("TGD_S_4"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(wakeup_event)
        RETURN
    END_IF
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, -2)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_togedaruma_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_togedaruma_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, 16, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_togedaruma_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_togedaruma_attack))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_1"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_A_1"))
    WAIT_MS(400)
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togedaruma_attack), 256, LW(5))
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
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_A_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 0, FLOAT(0.099609375f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), FLOAT(60.0f), -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_N_1"))
    WAIT_MS(1500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE5"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_togedaruma_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togedaruma_attack), 256, LW(5))
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_TOGEDA_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(5.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_J_2"))
    WAIT_FRAMES(15)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGD_S_4"))
    WAIT_MS(400)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    LABEL(0)
    SET_VALUE(LW(15), 0)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_SLEEP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_STOP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DIZZY, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_POISON, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FREEZE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        ADD(LW(15), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, 2)
        ELSE
        CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, 2)
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

