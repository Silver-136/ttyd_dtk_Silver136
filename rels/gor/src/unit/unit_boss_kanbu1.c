// unit_boss_kanbu1: Lord Crump (X-Naut leader), boss battle unit.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_sub.h"
#include "system.h"

#include "battle/battle.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_NPC 0               // unit id of the npc_christine unit spawned at init; speaker of the mac_0_b_t1_* messages
#define UNIT_WORK_AC_SUCCESS_TALKED 1 // 1 once phase_event has shown mac_0_b_t1_001 after a successful action command (_mario_ac_success_check)
#define UNIT_WORK_AC_FAIL_TALKED 2    // 1 once phase_event has shown mac_0_b_t1_002 after a failed action command (_mario_ac_failed_check)
#define UNIT_WORK_HALF_HP_TALKED 3    // 1 once phase_event has shown mac_0_b_t1_003 at HP <= half of max

USER_FUNC(_mario_ac_failed_check);
USER_FUNC(_mario_ac_success_check);
extern BattleUnitKind unit_npc_christine;

BattleUnitKind unit_boss_kanbu1;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_kanbu1_attack;
BattleUnitSetup entry_npc_christine;
static EvtWord init_event[82];
static EvtWord phase_event[282];
static EvtWord damage_event[10];
static EvtWord attack_event[24];
static EvtWord wait_event[6];
static EvtWord dead_event[276];
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_guard[18];
static EvtWord normal_attack_event[365];

/**
 * Unit kind of Lord Crump:
 * 5 HP, level 20, with its size, sounds and the links to its parts, status vulnerability and scripts.
 */
BattleUnitKind unit_boss_kanbu1 = {
    UNIT_LORD_CRUMP_PROLOGUE, // unitType
    "btl_un_kanbu1", // name
    5, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    20, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9, // pbSoftCap
    56, // width
    59, // height
    {0, 56}, // hitOffset
    {0.0f, 26.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {28.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {28.0f, 0.0f, 0.0f}, // bintaHitOffset
    {28.0f, 38.35f, 0.0f}, // kissHitOffset
    {0.0f, 26.0f, 0.0f}, // cutBaseOffset
    42.0f, // cutWidth
    52.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_KANBU_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of Lord Crump's body per damage type: 0 against every damage type. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Defence attributes of Lord Crump's body: none, so every damage type is dealt normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Status vulnerability of Lord Crump, as a percent of the normal chance: immune to fright, instant kill. */
static StatusVulnerability regist = {
    100, // sleep
    100, // stop
    100, // dizzy
    100, // poison
    100, // confuse
    100, // electric
    100, // burn
    100, // freeze
    100, // huge
    100, // tiny
    100, // attackUp
    100, // attackDown
    100, // defenseUp
    100, // defenseDown
    100, // allergic
    0, // fright
    100, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    0, // ohko
};

/** Event table of Lord Crump: nothing special on a First Strike, its defeat script. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

/** Body part of Lord Crump: a single part using the model "c_kanbu". */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_kanbu1", "c_kanbu", 0.0f, 0.0f, 0.0f, 0.0f, 45.0f, 0.0f, 0.0f, 50.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/** Lord Crump's attack: 1 damage to a single target, a hammer-style hit. */
BattleWeapon weapon_kanbu1_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    0, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Setup entry that adds Goombella as a unit of Lord Crump's battle
 * (his init script places her at (-140, 0, 5)).
 */
BattleUnitSetup entry_npc_christine = {
    &unit_npc_christine, // unitKindParams
    ALLIANCE_NEUTRAL, // alliance
    {0}, // pad_05
    BATTLE_PHASE_BEFORE_PLAYER, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Init event of Lord Crump: installs his wait, attack, damage, confusion and phase events,
 * spawns the Goombella unit (entry_npc_christine) at (-140, 0, 5) as his unit work 0,
 * clears work 1 to 3 (message flags) and sets the heavy walk and run sounds.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_NO_OBJECT_FALL)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_npc_christine), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_NPC, LW(3))
    CALL(btlevtcmd_SetPos, LW(3), -140, 0, 5)
    CALL(btlevtcmd_SetHomePos, LW(3), -140, 0, 5)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_SUCCESS_TALKED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_FAIL_TALKED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HALF_HP_TALKED, 0)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_MOVE1L"), STRING("SFX_BOSS_KANBU_MOVE1R"), 0, 10, 10)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_MOVE1L"), STRING("SFX_BOSS_KANBU_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Phase event of Lord Crump: on turn 1 before the player's phase the camera swings to Mario,
 * who turns round while Goombella speaks (mac_0_b_t1_000) and raises his hand.
 * After the player's phase it plays one-time comments:
 * when Crump is at half HP or less (mac_0_b_t1_003),
 * after Mario's first successful action command (001) and after his first failed one (002).
 */
static EVT_BEGIN(phase_event)
    // Turn 1, before the player's phase: the camera swings to Mario, who turns round.
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_StopWaitEvent, UNIT_ID_MARIO)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_S_1"))
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
            CALL(evt_btl_camera_set_moveto, 0, -117, 62, 284, -115, 43, 0, 30, 11)
            WAIT_MS(500)
            BEGIN_SIBLING_EVENT_GET_ID(LW(5))
                CALL(evt_sub_intpl_init, EASING_LINEAR, 0, 180, 15)
                LOOP(16)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, UNIT_ID_MARIO, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(5))
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NPC, LW(3))
            CALL(btlevtcmd_SetTalkPose, LW(3), STRING("PKR_T_1"))
            CALL(btlevtcmd_SetStayPose, LW(3), STRING("PKR_S_1"))
            // Goombella speaks and Mario raises his hand; the camera swings back.
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t1_000"), 0, LW(3))
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_HAND_UP1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_2"))
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_MARIO, 1)
            CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, POSE_TYPE_NEUTRAL)
            BEGIN_SIBLING_EVENT_GET_ID(LW(5))
                CALL(evt_sub_intpl_init, EASING_LINEAR, 180, 0, 15)
                LOOP(16)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, UNIT_ID_MARIO, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(5))
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_MARIO)
            RETURN
        END_IF
    END_IF
    // The half-HP message returns early; action-command feedback can follow on a later phase.
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_AFTER_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HALF_HP_TALKED, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(1))
            DIVIDE(LW(1), 2)
            CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
            IF_INT_LESS_OR_EQUAL(LW(0), LW(1))
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HALF_HP_TALKED, 1)
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NPC, LW(3))
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NPC, LW(3))
                // Lord Crump is at half HP or less: Goombella comments.
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t1_003"), 0, LW(3))
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
                RETURN
            END_IF
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_SUCCESS_TALKED, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(_mario_ac_success_check, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_SUCCESS_TALKED, 1)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NPC, LW(3))
                // Mario's first successful action command: Goombella comments.
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t1_001"), 0, LW(3))
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            END_IF
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_FAIL_TALKED, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(_mario_ac_failed_check, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AC_FAIL_TALKED, 1)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NPC, LW(3))
                // Mario's first failed action command: Goombella comments.
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t1_002"), 0, LW(3))
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

/** Damage event of Lord Crump: runs the default damage handling. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Attack event of Lord Crump: uses an enemy item if he has one,
 * otherwise always runs normal_attack_event with weapon_kanbu1_attack.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(9), PTR(&weapon_kanbu1_attack))
    CALL_CHILD(PTR(normal_attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait (idle) event of Lord Crump: shows the idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Death event of Lord Crump: spins him over 48 frames with the defeat sound, puffs smoke, pays out the coins,
 * experience and Star Points, tips him over, then runs a short Goombella dialogue with Mario turning to face her
 * (mac_0_b_t1_004 and 005) before the unit is removed.
 * It is the scripted end of the fight with Goombella's commentary.
 */
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_PRE_DEAD_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_PRE_DEAD_EVENT)
    END_IF
    SET_VALUE(LW(0), 48)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LABEL(10)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
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
    CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
    SET_VALUE(LW(0), 30)
    LABEL(20)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, (kHidden | kNoShadow))
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_MARIO)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_S_1"))
    BEGIN_SIBLING_EVENT_GET_ID(LW(5))
        CALL(evt_sub_intpl_init, EASING_LINEAR, 0, 180, 15)
        LOOP(16)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_MARIO, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(5))
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NPC, LW(3))
    CALL(btlevtcmd_SetTalkPose, LW(3), STRING("PKR_T_1"))
    CALL(btlevtcmd_SetStayPose, LW(3), STRING("PKR_S_1"))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t1_004"), 0, LW(3))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_VICTORY)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_b_t1_005"), 0, LW(3))
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    BEGIN_SIBLING_EVENT_GET_ID(LW(5))
        CALL(evt_sub_intpl_init, EASING_LINEAR, 180, 0, 15)
        LOOP(16)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_MARIO, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(5))
    CALL(btlevtcmd_KillUnit, LW(10), 0)
    RETURN
EVT_END()

/**
 * Pose table of Lord Crump:
 * the animation played for each pose type (idle, hurt, walk, run, attack and so on).
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "S_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_2"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/**
 * Pose table of Lord Crump while guarding:
 * the animation played for each pose type (idle, hurt, walk, run, attack and so on).
 */
static PoseTableEntry pose_table_guard[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {POSE_TYPE_NORMAL, "G_2"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "G_2"},
    {POSE_TYPE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_2"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "G_2"},
};

/**
 * Lord Crump's basic attack (weapon in LW(9), weapon_kanbu1_attack):
 * picks a target, runs at it and does a belly flop.
 * He hops (SFX_BOSS_KANBU_JUMP1, poses A_1A and A_1B) over the target;
 * the pre-damage check decides between a dodge and a hit,
 * and on a hit he lands on it (A_1C, SFX_BOSS_KANBU_HIP_ATTACK1 and 2),
 * bounces and gets up (D_1) before returning home.
 * Falls back to the flustered reaction when confused with nothing to hit.
 */
static EVT_BEGIN(normal_attack_event)
    // Target: the first-strike target if the battle opened that way, otherwise one picked for his attack.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    // No target: a confused Crump flusters, otherwise the attack ends.
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
    // He runs at the target.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_2"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    // Wind-up: he hops up (A_1A, A_1B) over the target.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1A"))
        WAIT_FRAMES(10)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1B"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(2.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE)
    // The pre-damage check decides between a dodge and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
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
        // Miss: he comes down without hitting (the target dodges for some kinds of miss).
        LABEL(90)
        GOTO(98)
        // Hit: he belly flops onto the target for the weapon damage.
        LABEL(91)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(98)
        LABEL(98)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        // He lands flat (A_1C) with a thud, then bounces twice.
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_HIP_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 0, FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1C"))
        WAIT_FRAMES(25)
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_HIP_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(7)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KANBU_HIP_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        // He gets up (D_1) and dusts himself off.
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("D_1"))
        WAIT_FRAMES(15)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 15, 0, FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        // He trots back to his home position.
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
        WAIT_FRAMES(10)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        RETURN
EVT_END()

USER_FUNC(_mario_ac_success_check) {
    evtSetValue(event, event->args[0], _battleWorkPointer->marioAcSuccess);
    return EVT_RETURN_DONE;
}
/** Script helper: stores in args[0] whether Mario's last action command failed (the battle work's marioAcFailed). */
USER_FUNC(_mario_ac_failed_check) {
    evtSetValue(event, event->args[0], _battleWorkPointer->marioAcFailed);
    return EVT_RETURN_DONE;
}
