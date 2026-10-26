#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "battle/battle_database_common.h"
#include "evt/evt_sub.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"


BattleUnitKind unit_pansy;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
BattleWeapon weapon_pansy_attack;
static EvtWord attack_event[58];
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static EvtWord init_event[21];
static EvtWord damage_event[10];
static EvtWord song_event[300];
static EvtWord escape_event[96];
static EvtWord wait_event[6];

BattleUnitKind unit_pansy = {
    UNIT_CRAZEE_DAYZEE, // mUnitType
    "btl_un_pansy", // name
    7, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    16, // mLevel
    0, // mBonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    1, // baseCoin
    70, // runRate
    9999, // pbSoftCap
    22, // width
    32, // height
    {5, 32}, // hitOffset
    {0.0f, 16.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {11.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {11.0f, 0.0f, 0.0f}, // bintaHitOffset
    {6.0f, 20.8f, 0.0f}, // kissHitOffset
    {0.0f, 16.0f, 0.0f}, // cutBaseOffset
    22.0f, // cutWidth
    32.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_PANSY_DAMAGED1", // damageSfxName
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

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    95, // sleep
    30, // stop
    30, // dizzy
    100, // poison
    50, // confuse
    100, // electric
    100, // burn
    30, // freeze
    100, // huge
    30, // tiny
    100, // attackUp
    30, // attackDown
    100, // defenseUp
    30, // defenseDown
    30, // allergic
    100, // fright
    80, // galeForce
    100, // fast
    30, // slow
    100, // dodgy
    100, // invisible
    30, // ohko
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_pansy", "c_pansy", 0.0f, 0.0f, 0.0f, 0.0f, 29.0f, 0.0f, 0.0f, 34.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

BattleWeapon weapon_pansy_attack = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
#ifdef VERSION_G8ME01
    50, // sleepChance
#else
    75, // sleepChance
#endif
    5, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 1)
        GOTO(10)
    END_IF
    SET_VALUE(LW(0), 70)
    ADD(LW(0), 30)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    IF_INT_LESS(LW(1), 70)
        SET_VALUE(LW(9), PTR(&weapon_pansy_attack))
        CALL_CHILD(PTR(song_event))
        GOTO(99)
    END_IF
    LABEL(10)
    CALL_CHILD(PTR(escape_event))
    GOTO(99)
    LABEL(99)
    RETURN
EVT_END()

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "PAN_N_1"},
    {POSE_TYPE_STOP, "PAN_Y_1"},
    {POSE_TYPE_FREEZE, "PAN_Y_1"},
    {POSE_TYPE_CONFUSE, "PAN_K_1"},
    {POSE_TYPE_POISON, "PAN_X_1"},
    {POSE_TYPE_DIZZY, "PAN_X_1"},
    {POSE_TYPE_NORMAL, "PAN_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "PAN_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "PAN_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "PAN_S_1"},
    {POSE_TYPE_DAMAGE, "PAN_D_1"},
    {POSE_TYPE_ATTACK, "PAN_A_1"},
    {POSE_TYPE_RUN, "PAN_R_1"},
    {POSE_TYPE_WALK, "PAN_W_1"},
    {POSE_TYPE_DANGER, "PAN_X_1"},
    {POSE_TYPE_PERIL, "PAN_X_1"},
    {POSE_TYPE_TALK, "PAN_T_1"},
    {POSE_TYPE_ACTION, "PAN_S_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(song_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(0), 255)
    SET_VALUE(LW(1), 255)
    LOOP(0)
        IF_INT_LESS(LW(0), 192)
            BREAK_LOOP
        END_IF
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(1), 255)
        SUBTRACT(LW(0), 2)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PANSY_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    BEGIN_SIBLING_EVENT
        LOOP(10)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            IF_INT_LESS(LW(15), 0)
                CALL(evt_eff64, STRING(""), STRING("onpu_n64"), 1, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            ELSE
                CALL(evt_eff64, STRING(""), STRING("onpu_n64"), 2, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
            END_IF
            WAIT_FRAMES(12)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_FRAMES(120)
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
        GOTO(97)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        SET_VALUE(LW(6), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(98)
    END_IF
    GOTO(10)
    LABEL(98)
    SET_VALUE(LW(0), 192)
    SET_VALUE(LW(1), 255)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(0), 256)
            BREAK_LOOP
        END_IF
        CALL(evt_map_set_blend, 0, LW(0), LW(0), LW(1), 255)
        ADD(LW(0), 2)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_map_blend_off, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    WAIT_MS(500)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(escape_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PANSY_ESCAPE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PAN_E_1"))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 180, 0)
    WAIT_MS(250)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 255, 255, 255, 255, 100, 100, 100, 100, 0)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PANSY_ESCAPE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(20.0f), MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PANSY_ESCAPE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(9.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, 300, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
    WAIT_MS(500)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

