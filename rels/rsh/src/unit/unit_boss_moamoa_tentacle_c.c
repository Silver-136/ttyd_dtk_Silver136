#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

extern s32 boss_moamoa_check_zenei_kouei[];
extern PoseTableEntry moamoa1_weakness_pose_table[];
extern s32 moamoa_defence_no_parts[];

BattleUnitKind unit_boss_moamoa_tentacle_c;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_moamoa_attack;
static EvtWord init_event[26];
static EvtWord damage_event[10];
static EvtWord dead_event[115];
static EvtWord phase_event[2];
static EvtWord attack_event[21];
static EvtWord evt_attack_other_arm[81];
const char* A_of_C_Attack[3];
const char* B_of_C_Attack[3];
const char* C_of_C_Attack_front[8];
const char* C_of_C_Attack_back[8];
static EvtWord evt_attack[387];
static EvtWord wait_event[32];

BattleUnitKind unit_boss_moamoa_tentacle_c = {
    UNIT_SMORG_MIASMA_TENTACLE_A, // mUnitType
    "btl_un_moamoa_tentacle_a", // name
    4, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    0, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    10, // pbSoftCap
    110, // width
    70, // height
    {-50, 70}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {280.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {55.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {55.0f, 0.0f, 0.0f}, // bintaHitOffset
    {55.0f, 45.5f, 20.0f}, // kissHitOffset
    {-90.0f, 100.0f, 0.0f}, // cutBaseOffset
    110.0f, // cutWidth
    70.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    0, // hammerKnockbackChance
    0, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_MOA_G_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoSpin, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

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

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_NORMAL, "S_1C_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "D_1C_1"},
    {POSE_TYPE_DAMAGE, "D_1C_1"},
    {POSE_TYPE_ACTION, "S_1C_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_moamoa_tentacle_c", "c_g_moamoa_b", 0.0f, 0.0f, 0.0f, 88.0f, 88.0f, 10.0f, 93.0f, 93.0f, 10.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
};

static BattleWeapon weapon_moamoa_attack = {
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
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetHpGaugeOffset, UNIT_ID_SELF, -56, 63)
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_DOWN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1C"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(1))
    CALL(btlevtcmd_SetUnitWork, LW(1), 3, -1)
    CALL(btlevtcmd_GetUnitWork, LW(1), 5, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_GetUnitWork, LW(1), 4, LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(btlevtcmd_GetUnitWork, LW(1), 3, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_SetPartsDefenceTable, LW(1), 1, PTR(moamoa_defence_no_parts))
                CALL(btlevtcmd_OffPartsAttribute, LW(1), 1, 536870912)
                CALL(btlevtcmd_AnimeSetPoseTable, LW(1), 1, PTR(moamoa1_weakness_pose_table))
                CALL(btlevtcmd_SetHitOffset, LW(1), 1, 0, 32, 0)
                CALL(btlevtcmd_SetHitCursorOffset, LW(1), 1, 0, 41, 0)
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_BIG1"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_BIG2"), EVT_NULLPTR, 0, EVT_NULLPTR)
                CALL(btlevtcmd_AnimeChangePose, LW(1), 1, STRING("V_1"))
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, LW(1), 1)
            END_IF
        END_IF
    END_IF
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, -2)
        RETURN
    END_IF
    CALL_CHILD(PTR(evt_attack))
    CALL(btlevtcmd_StartWaitEvent, -2)
    RETURN
EVT_END()

static EVT_BEGIN(evt_attack_other_arm)
    SET_INT_READ_ADDR(LW(1))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE1"), EVT_NULLPTR, 0, LW(15))
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, LW(0), LW(5), LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(0), LW(5))
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, LW(6), 0, LW(10))
        IF_INT_NOT_EQUAL(LW(10), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE1"), EVT_NULLPTR, 0, LW(15))
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, LW(0), LW(5), LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, LW(0), LW(5))
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    RETURN
EVT_END()

const char* A_of_C_Attack[] = {
    "S_1A_1",
    NULL,
    NULL,
};

const char* B_of_C_Attack[] = {
    "S_1B_1",
    NULL,
    NULL,
};

const char* C_of_C_Attack_front[] = {
    "A_1C_1",
    "A_1C_2",
    NULL,
    "A_1C_3",
    NULL,
    "A_1C_4",
    "S_1C_1",
    NULL,
};

const char* C_of_C_Attack_back[] = {
    "A_1C_5",
    "A_1C_6",
    NULL,
    "A_1C_7",
    NULL,
    "A_1C_8",
    "S_1C_1",
    NULL,
};

static EVT_BEGIN(evt_attack)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_moamoa_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_moamoa_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        RETURN
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_moamoa_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_moamoa_attack))
    CALL(boss_moamoa_check_zenei_kouei, LW(3), LW(1))
    IF_INT_EQUAL(LW(1), 0)
        SET_VALUE(LF(0), 0)
    ELSE
        SET_VALUE(LF(0), 1)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(6))
    CALL(btlevtcmd_SetUnitWork, LW(6), 0, 0)
    CALL(btlevtcmd_AnimeChangePose, LW(6), 1, STRING("S_1_1"))
    SET_VALUE(LW(15), 0)
    CALL(btlevtcmd_GetUnitWork, LW(6), 5, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        SET_VALUE(LW(1), PTR(&A_of_C_Attack))
        SET_VALUE(LW(5), 1)
        SPAWN_EVENT(evt_attack_other_arm)
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(6), 4, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        SET_VALUE(LW(1), PTR(&B_of_C_Attack))
        SET_VALUE(LW(5), 1)
        SPAWN_EVENT(evt_attack_other_arm)
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        SET_INT_READ_ADDR(PTR(&C_of_C_Attack_front))
        SET_VALUE(LW(15), 1)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE1"), EVT_NULLPTR, 0, LW(14))
            WAIT_MS(600)
            CALL(evt_snd_sfxoff, LW(14))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
    ELSE
        SET_INT_READ_ADDR(PTR(&C_of_C_Attack_back))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE1"), EVT_NULLPTR, 0, LW(14))
            WAIT_MS(900)
            CALL(evt_snd_sfxoff, LW(14))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
    END_IF
    LOOP(0)
        READ_INT(LW(2))
        IF_INT_EQUAL(LW(2), 0)
            BREAK_LOOP
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    END_LOOP
    WAIT_MS(1000)
    IF_INT_EQUAL(LF(0), 0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MOA_G_ARM_MOVE3_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_IF
    IF_INT_EQUAL(LW(15), 0)
        LOOP(0)
            READ_INT(LW(2))
            IF_INT_EQUAL(LW(2), 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        END_LOOP
    ELSE
        BEGIN_SIBLING_EVENT
            LOOP(0)
                READ_INT(LW(2))
                IF_INT_EQUAL(LW(2), 0)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            END_LOOP
            LOOP(0)
                READ_INT(LW(2))
                IF_INT_EQUAL(LW(2), 0)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
                WAIT_FRAMES(1)
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            END_LOOP
            SET_VALUE(GF(9), 0)
        END_SIBLING_EVENT
        WAIT_MS(70)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_moamoa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_EQUAL(1)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_moamoa_attack))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_moamoa_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
        CASE_EQUAL(4)
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
    END_SWITCH
    CALL(btlevtcmd_SetUnitWork, LW(6), 0, 1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    IF_INT_EQUAL(LW(15), 0)
        LOOP(0)
            READ_INT(LW(2))
            IF_INT_EQUAL(LW(2), 0)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(2))
            WAIT_FRAMES(1)
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        END_LOOP
    ELSE
        SET_VALUE(GF(9), 1)
        WAIT_WHILE(GF(9))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_GetUnitWork, LW(6), 5, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(10), 1)
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(6), 4, LW(10))
    IF_INT_NOT_EQUAL(LW(10), -1)
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(10), 1)
    END_IF
    RETURN
EVT_END()

