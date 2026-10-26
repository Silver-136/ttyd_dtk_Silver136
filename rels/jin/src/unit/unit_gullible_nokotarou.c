#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "unit/party/unit_party_nokotarou.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

USER_FUNC(btlevtcmd_SetJumpSound);

BattleUnitKind unit_gullible_nokotarou;
static BattleWeapon enemyWeapon_NokotarouKouraAttack;
static BattleWeapon enemyWeapon_NokotarouShubibinAttack;
static BattleUnitDefense defence_nokotarou;
static BattleUnitDefenseAttr defence_attr_nokotarou;
static BattleUnitDefense defence_turn_nokotarou;
static StatusVulnerability regist_nokotarou;
static BattleDataEntry data_table_nokotarou[3];
static BattleUnitKindPart parts_nokotarou[1];
static EvtWord dmg_turn_event[64];
static EvtWord wakeup_event[67];
static EvtWord init_event_nokotarou[62];
static EvtWord damage_event_nokotarou[24];
static EvtWord dead_event_nokotarou[177];
static EvtWord _change_koura_pose[34];
static EvtWord _change_koura_pose_fast[34];
static EvtWord _restore_koura_pose[47];
static EvtWord _koura_rotate_start[99];
static EvtWord _koura_rotate_stop[13];
static EvtWord attack_event_nokotarou[33];
static EvtWord attack_event_nokotarou_koura[393];
static EvtWord attack_event_nokotarou_shubibin[339];
static EvtWord wait_event_nokotarou[6];

BattleUnitKind unit_gullible_nokotarou = {
    UNIT_KOOPS_CHAPTER4, // mUnitType
    "btl_un_gullible_nokotarou", // name
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
    28, // width
    40, // height
    {10, 30}, // hitOffset
    {0.0f, 20.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {14.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {14.0f, 0.0f, 0.0f}, // bintaHitOffset
    {14.0f, 26.0f, 0.0f}, // kissHitOffset
    {0.0f, 20.0f, 0.0f}, // cutBaseOffset
    28.0f, // cutWidth
    40.0f, // cutHeight
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
    &regist_nokotarou, // statusVulnerability
    1, // numParts
    parts_nokotarou, // mParts
    init_event_nokotarou, // initEventCode
    data_table_nokotarou, // dataTable
};

static BattleWeapon enemyWeapon_NokotarouKouraAttack = {
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
    {2, 3, 5, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
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

static BattleWeapon enemyWeapon_NokotarouShubibinAttack = {
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
    {2, 3, 5, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
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

static BattleUnitDefense defence_nokotarou = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr_nokotarou = {{0, 0, 0, 0, 0}};

static BattleUnitDefense defence_turn_nokotarou = {0};

static StatusVulnerability regist_nokotarou = {
    60, // sleep
    80, // stop
    70, // dizzy
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

static BattleDataEntry data_table_nokotarou[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_nokotarou},
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {0, NULL},
};

static BattleUnitKindPart parts_nokotarou[] = {
    {1, "btl_un_nokotarou", "c_pnk", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence_nokotarou, &defence_attr_nokotarou, kMainPart | kGuardTargetPart | kIsShelled, 0, pose_table_nokotarou_stay},
};

static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn_nokotarou))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_nokotarou_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("PNK_D_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, -1)
    RETURN
EVT_END()

static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_nokotarou))
    CALL(btlevtcmd_OffStatusFlag, LW(10), 1)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_nokotarou_stay))
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("PNK_S_1"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(init_event_nokotarou)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_nokotarou))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_nokotarou))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_nokotarou))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, 0)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 9, 3)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_nokotarou)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, 1024, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(20), 1)
    END_IF
    CALL_CHILD(PTR(_koura_rotate_stop))
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event_nokotarou)
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
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
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
        CALL(evt_msg_print, 2, STRING("stg4_jin_26_03_nok"), 0, -2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_D_3"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 90, 58, 228, 90, 38, -20, 30, 11)
        WAIT_FRAMES(37)
        CALL(evt_msg_print, 2, STRING("stg4_jin_26_04"), 0, GW(16))
        WAIT_MS(200)
        CASE_EQUAL(1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_D_3"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        LOOP(0)
            IF_INT_EQUAL(GW(19), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CASE_EQUAL(2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_D_3"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SWITCH
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 1)
    SET_VALUE(GW(20), 2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

static EVT_BEGIN(_change_koura_pose)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
    LOOP(18)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(_change_koura_pose_fast)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 27, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
    LOOP(3)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 30, 0)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(_restore_koura_pose)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(_koura_rotate_start)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 0)
    BEGIN_INLINE_EVENT
        LABEL(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 8, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(10)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(11))
        IF_INT_NOT_EQUAL(LW(11), 0)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 9, LW(10))
            MULTIPLY(LW(10), -1)
            MULTIPLY(LW(0), -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, LW(10))
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 9, LW(10))
        IF_INT_GREATER_OR_EQUAL(LW(10), 0)
            ADD(LW(0), 1)
            IF_INT_GREATER(LW(0), LW(10))
                SET_VALUE(LW(0), LW(10))
            END_IF
            ELSE
            SUBTRACT(LW(0), 1)
            IF_INT_LESS(LW(0), LW(10))
                SET_VALUE(LW(0), LW(10))
            END_IF
        END_IF
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
        LABEL(10)
    END_INLINE_EVENT
    RETURN
EVT_END()

static EVT_BEGIN(_koura_rotate_stop)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_nokotarou)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(wakeup_event))
        RETURN
    END_IF
    CALL(btlevtcmd_DrawLots, LW(0), 2, 80, 20)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL_CHILD(PTR(attack_event_nokotarou_koura))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(attack_event_nokotarou_shubibin))
    END_SWITCH
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_nokotarou_koura)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_NokotarouKouraAttack))
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL_CHILD(_change_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_ATTACK1"), EVT_NULLPTR, 0, LW(13))
    SET_VALUE(LW(0), 15)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, 45)
    CALL_CHILD(_koura_rotate_start)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    WAIT_FRAMES(30)
    CALL(evt_snd_sfxoff, LW(13))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_ATTACK2"), EVT_NULLPTR, 0, LW(13))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(5), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(13))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(13), -1, 0)
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
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        CALL_CHILD(_koura_rotate_stop)
        CALL_CHILD(_restore_koura_pose)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, -1)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, -1)
    CALL_CHILD(_restore_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_nokotarou_shubibin)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_NokotarouShubibinAttack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL_CHILD(_change_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_SHUBIBIN1"), EVT_NULLPTR, 0, LW(13))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 0)
    SET_VALUE(LW(0), 15)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, 45)
    CALL_CHILD(_koura_rotate_start)
    WAIT_MS(500)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    WAIT_MS(500)
    DELETE_EVENT(LW(15))
    CALL(evt_snd_sfxoff, LW(13))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_SHUBIBIN2"), EVT_NULLPTR, 0, LW(13))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(5), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
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
        GOTO(50)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(80)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -400, LW(1), LW(2), 0, -1, 0)
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    MULTIPLY(LW(0), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    CALL_CHILD(_koura_rotate_stop)
    CALL_CHILD(_restore_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_nokotarou)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

