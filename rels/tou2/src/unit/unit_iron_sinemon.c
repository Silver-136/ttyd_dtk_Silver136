#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_msg.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"

#include "battle/battle.h"
#include "mario/mario_pouch.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SILENT 0  // 1 skips the damage and phase talk events (init_event2, used by the green copy); 0 for this one (init_event1)
#define UNIT_WORK_TWIN_ID 1 // unit id of the other Iron Cleft (get_id: the other UNIT_IRON_CLEFT_* kind), who speaks the reply lines

USER_FUNC(btlevtcmd_SetEventPhase);
static USER_FUNC(get_id);
static USER_FUNC(yoshi_exist);

BattleUnitKind unit_iron_sinemon;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static BattleUnitDefense defence_turn;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_iron_sinemon_attack;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[3];
static EvtWord init_event1[18];
static EvtWord init_event2[18];
static EvtWord init_event_sub[47];
static EvtWord dmg_avoid_counter_thorn_head_event[76];
static EvtWord damage_event[102];
static EvtWord phase_event[188];
static EvtWord attack_event[385];
static EvtWord wait_event[6];

BattleUnitKind unit_iron_sinemon = {
    UNIT_IRON_CLEFT_RED, // mUnitType
    "btl_un_iron_sinemon", // name
    6, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    18, // mLevel
    0, // mBonusExp
    2, // bonusCoin
    100, // bonusCoinRate
    1, // baseCoin
    70, // runRate
    9, // pbSoftCap
    52, // width
    54, // height
    {4, 54}, // hitOffset
    {0.0f, 27.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {26.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {26.0f, 0.0f, 0.0f}, // bintaHitOffset
    {16.0f, 36.0f, 0.0f}, // kissHitOffset
    {0.0f, 27.0f, 0.0f}, // cutBaseOffset
    52.0f, // cutWidth
    54.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_ISIN_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitLeader, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event1, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{4, 4, 4, 4, 4}};

static BattleUnitDefense defence_turn = {0};

static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    0, // dizzy
    0, // poison
    0, // confuse
    0, // electric
    0, // burn
    0, // freeze
    0, // huge
    0, // tiny
    0, // attackUp
    0, // attackDown
    0, // defenseUp
    0, // defenseDown
    0, // allergic
    0, // fright
    0, // galeForce
    0, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_iron_sinemon", "c_sinemon_a", 0.0f, 0.0f, 0.0f, 0.0f, 37.0f, 0.0f, 0.0f, 45.0f, 0.0f, 30, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, kCounterTopSpiky, pose_table},
};

static BattleWeapon weapon_iron_sinemon_attack = {
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
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH, // special
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
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "SIN_N_1"},
    {POSE_TYPE_STOP, "SIN_Y_1"},
    {POSE_TYPE_FREEZE, "SIN_Y_1"},
    {POSE_TYPE_CONFUSE, "SIN_K_1"},
    {POSE_TYPE_POISON, "SIN_I_1"},
    {POSE_TYPE_DIZZY, "SIS_I_1"},
    {POSE_TYPE_NORMAL, "SIN_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "SIN_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "SIN_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "SIN_A_1a"},
    {POSE_TYPE_DAMAGE, "SIN_D_1"},
    {POSE_TYPE_ATTACK, "SIN_A_1a"},
    {POSE_TYPE_WALK, "SIN_W_1"},
    {POSE_TYPE_RUN, "SIN_R_1"},
    {POSE_TYPE_TALK, "SIN_T_1"},
    {POSE_TYPE_DANGER, "SIN_I_1"},
    {POSE_TYPE_PERIL, "SIN_I_1"},
    {POSE_TYPE_ACTION, "SIN_S_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_head_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static EVT_BEGIN(init_event1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SILENT, 0)
    CALL(get_id, LW(0), LW(1))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TWIN_ID, LW(1))
    CALL_CHILD(PTR(init_event_sub))
    RETURN
EVT_END()

static EVT_BEGIN(init_event2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SILENT, 1)
    CALL(get_id, LW(0), LW(1))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TWIN_ID, LW(0))
    CALL_CHILD(PTR(init_event_sub))
    RETURN
EVT_END()

static EVT_BEGIN(init_event_sub)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetBaseScale, UNIT_ID_SELF, FLOAT(1.5f), FLOAT(1.5f), FLOAT(1.0f))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BOSS_ISIN_MOVE1"), STRING("SFX_BOSS_ISIN_MOVE2"), 0, 10, 10)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BOSS_ISIN_MOVE1"), STRING("SFX_BOSS_ISIN_MOVE2"), 0, 7, 7)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
        ADD(LW(1), 67)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 14)
        ADD(LW(1), 37)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    ADD(LW(1), 52)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SILENT, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2511), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetTotalDamage, UNIT_ID_SELF, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 91, 74, 334, 91, 52, 0, 30, 4)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg3_tou_272"), 0, -2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TWIN_ID, LW(0))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg3_tou_273"), 0, LW(0))
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    SET_VALUE(GSWF(2511), 1)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SILENT, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    IF_INT_EQUAL(GSWF(2512), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_get_turn, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_PARTNER)
            CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
            CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 1, -104, 74, 334, -102, 52, 0, 30, 4)
            WAIT_FRAMES(30)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            IF_INT_EQUAL(GSWF(2510), 0)
                CALL(evt_msg_print_battle_party, STRING("stg3_tou_266"))
                ELSE
                CALL(evt_msg_print_battle_party, STRING("stg3_tou_267"))
            END_IF
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            CALL(evt_btl_camera_set_moveto, 1, 91, 74, 334, 91, 52, 0, 30, 4)
            WAIT_FRAMES(30)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL(yoshi_exist, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetTalkPoseType, UNIT_ID_SELF, 65)
                CALL(btlevtcmd_SetStayPoseType, UNIT_ID_SELF, 69)
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg3_tou_268"), 0, -2)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TWIN_ID, LW(0))
                CALL(btlevtcmd_SetTalkPoseType, LW(0), 65)
                CALL(btlevtcmd_SetStayPoseType, LW(0), 69)
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg3_tou_269"), 0, LW(0))
                ELSE
                CALL(btlevtcmd_SetTalkPoseType, UNIT_ID_SELF, 65)
                CALL(btlevtcmd_SetStayPoseType, UNIT_ID_SELF, 69)
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg3_tou_270"), 0, -2)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TWIN_ID, LW(0))
                CALL(btlevtcmd_SetTalkPoseType, LW(0), 65)
                CALL(btlevtcmd_SetStayPoseType, LW(0), 69)
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg3_tou_271"), 0, LW(0))
            END_IF
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            IF_INT_EQUAL(GSWF(2510), 0)
                SET_VALUE(GSWF(2510), 1)
            END_IF
            CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_iron_sinemon_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_iron_sinemon_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_iron_sinemon_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_iron_sinemon_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        LOOP(0)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ISIN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ISIN_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ISIN_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    WAIT_MS(750)
    DELETE_EVENT(LW(14))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), FLOAT(30.0f))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 25)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_iron_sinemon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 75)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    WAIT_MS(500)
    GOTO(98)
    LABEL(91)
    CALL(evt_btl_camera_noshake, 0)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_iron_sinemon_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_iron_sinemon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(1.0f), FLOAT(0.39941406f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 75)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ISIN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 0, 5, 0, 20, 0)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.5f), FLOAT(0.39941406f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 100)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(evt_btl_camera_shake_h, 0, 5, 0, 20, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ISIN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    GOTO(98)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(evt_btl_camera_noshake, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static USER_FUNC(get_id) {
    EvtWord* args = event->args;
    s32 i;
    s32 id1;
    s32 id2;

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, i);
        if (unit != NULL) {
            if (unit->currentKind == UNIT_IRON_CLEFT_RED) {
                id1 = i;
            }
            if (unit->currentKind == UNIT_IRON_CLEFT_GREEN) {
                id2 = i;
            }
        }
    }
    evtSetValue(event, args[0], id1);
    evtSetValue(event, args[1], id2);
    return EVT_RETURN_DONE;
}
static USER_FUNC(yoshi_exist) {
    EvtWord* args = event->args;
    PouchData* pouch = pouchGetPtr();
    s32 exist = 0;

    if (pouch->partyData[4].flags & 1) {
        exist = 1;
    }
    evtSetValue(event, args[0], exist);
    return EVT_RETURN_DONE;
}
