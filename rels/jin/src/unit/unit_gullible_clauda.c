#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "unit/party/unit_party_clauda.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"
#include "system.h"

USER_FUNC(btlevtcmd_SetJumpSound);

BattleUnitKind unit_gullible_clauda;
static BattleWeapon enemyWeapon_ClaudaNormalAttack;
static BattleUnitDefense defence_clauda;
static BattleUnitDefenseAttr defence_attr_clauda;
static StatusVulnerability regist_clauda;
static BattleDataEntry data_table_clauda[2];
static BattleUnitKindPart parts_clauda[1];
static EvtWord init_event_clauda[50];
EvtWord damage_event_clauda[22];
static EvtWord dead_event_clauda[177];
static EvtWord attack_event_clauda[393];
static EvtWord wait_event_clauda[6];

BattleUnitKind unit_gullible_clauda = {
    UNIT_FLURRIE_CHAPTER4, // mUnitType
    "btl_un_gullible_clauda", // name
    10, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    0, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9, // pbSoftCap
    42, // width
    46, // height
    {12, 40}, // hitOffset
    {0.0f, 23.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {21.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {21.0f, 0.0f, 0.0f}, // bintaHitOffset
    {21.0f, 29.9f, 0.0f}, // kissHitOffset
    {0.0f, 23.0f, 0.0f}, // cutBaseOffset
    42.0f, // cutWidth
    46.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_PARTY_DAMAGE1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoncombatant, // attributeFlags
    &regist_clauda, // statusVulnerability
    1, // numParts
    parts_clauda, // mParts
    init_event_clauda, // initEventCode
    data_table_clauda, // dataTable
};

static BattleWeapon enemyWeapon_ClaudaNormalAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {2, 4, 6, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_PRESS, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
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

static BattleUnitDefense defence_clauda = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr_clauda = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist_clauda = {
    60, // sleep
    80, // stop
    105, // dizzy
    100, // poison
    70, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    70, // tiny
    100, // attackUp
    70, // attackDown
    100, // defenseUp
    90, // defenseDown
    70, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    0, // ohko
};

static BattleDataEntry data_table_clauda[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_clauda},
    {0, NULL},
};

static BattleUnitKindPart parts_clauda[] = {
    {1, "btl_un_clauda", "c_windy", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence_clauda, &defence_attr_clauda, kMainPart | kGuardTargetPart, 0, pose_table_clauda_stay},
};

static EVT_BEGIN(init_event_clauda)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_clauda))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_clauda))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_clauda))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 0, 9, 3)
    CALL(btlevtcmd_SetDiveSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_HANG1"), STRING("SFX_PARTY_BATTLE_HANG1"), 0, 9, 3)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(damage_event_clauda)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(20), 1)
    END_IF
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event_clauda)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(3), LW(4), LW(5))
    SET_VALUE(LW(6), 1)
    IF_INT_EQUAL(LW(0), LW(3))
        IF_INT_EQUAL(LW(1), LW(4))
            IF_INT_EQUAL(LW(2), LW(5))
                SET_VALUE(LW(6), 0)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL(btlevtcmd_ResetFaceDirection, -2)
    END_IF
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    SWITCH(GW(19))
        CASE_EQUAL(0)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 130, 58, 228, 130, 38, -20, 30, 11)
        WAIT_FRAMES(37)
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg4_jin_26_03_win"), 0, -2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_D_3"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 90, 58, 228, 90, 38, -20, 30, 11)
        WAIT_FRAMES(37)
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg4_jin_26_04"), 0, GW(16))
        WAIT_MS(200)
        CASE_EQUAL(1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_D_3"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        LOOP(0)
            IF_INT_EQUAL(GW(19), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CASE_EQUAL(2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_D_3"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 1)
    SET_VALUE(GW(20), 2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_clauda)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_ClaudaNormalAttack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(400)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_A1_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CLAUD_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_A1_2"))
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PWD_D_3"))
        WAIT_FRAMES(30)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CLAUD_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 30, 13)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(1.0f), FLOAT(0.39941406f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(0.39941406f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(0), LW(5), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(1.0f), FLOAT(1.5f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(1.5f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(5), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, UNIT_ID_SELF, LW(5), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    WAIT_MS(700)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_clauda)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

