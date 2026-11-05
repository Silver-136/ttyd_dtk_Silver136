// Battle unit for Mario (マリオ) acting in Flurrie's stage play in the epilogue (nok.rel); it only plays its
// scripted part.

#include "battle/battle_unit.h"
#include "eff/n64/eff_kumokumo_n64.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_audience.h"
#include "system.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "unit/party/unit_party_christine.h"
#include "manager/evtmgr_cmd.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_MARIO_ID 0        // its own unit id
#define UNIT_WORK_CLAUDA_ID 1       // unit id of the spawned unit_act_clauda (entry_unit_data[0])
#define UNIT_WORK_TERESA1_ID 2      // unit id of the first unit_act_teresa (entry_unit_data[1])
#define UNIT_WORK_TERESA2_ID 3      // unit id of the second unit_act_teresa (entry_unit_data[2])
#define UNIT_WORK_ATMIC_TERESA_ID 4 // unit id of unit_act_atmic_teresa (entry_unit_data[3])
#define UNIT_WORK_KINOPIO1_ID 5     // unit id of the first unit_act_kinopio (entry_unit_data[4])
#define UNIT_WORK_KINOPIO2_ID 6     // unit id of the second unit_act_kinopio (entry_unit_data[5])
#define UNIT_WORK_KINOPIKO_ID 7     // unit id of unit_act_kinopiko (entry_unit_data[6])
#define UNIT_WORK_CHRISTINE_ID 8    // unit id of the spawned unitdata_Party_Christine (entry_unit_data[7])


static USER_FUNC(_clauda_breath_effect_fire);
static USER_FUNC(_clauda_breath_effect_ready);
USER_FUNC(_status_window_off);
extern BattleUnitKind unit_act_atmic_teresa;
extern BattleUnitKind unit_act_clauda;
extern BattleUnitKind unit_act_kinopiko;
extern BattleUnitKind unit_act_kinopio;
extern BattleUnitKind unit_act_teresa;

BattleUnitKind unit_act_mario;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[7];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static EvtWord init_event[15];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static EvtWord attack_event[5];
static EvtWord wait_event[6];
BattleUnitSetup entry_unit_data[8];
static EvtWord battle_entry_event[168];
EvtWord _battle_event[155];
EvtWord _act_target_enemy[33];
EvtWord _act_target_mario[26];
EvtWord _act_mario_jump_attack[240];
EvtWord _act_clauda_press_attack[350];
EvtWord _act_clauda_breath_attack[269];
EvtWord _act_teresa_attack[190];
EvtWord _act_atmic_teresa_press_attack[188];

/**
 * Unit kind of the Mario actor (HP 10, level 1):
 * its size, offsets, damage sounds, status vulnerability, parts and init event.
 */
BattleUnitKind unit_act_mario = {
    UNIT_EPILOGUE_DOOPLISS_MARIO, // unitType
    "btl_un_act_mario", // name
    10, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    1, // level
    0, // bonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    28, // width
    24, // height
    {5, 25}, // hitOffset
    {0.0f, 14.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 30.0f, 0.0f}, // talkTogeBaseOffset
    {14.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {14.0f, 0.0f, 0.0f}, // bintaHitOffset
    {14.0f, 15.6f, 0.0f}, // kissHitOffset
    {0.0f, 0.0f, 0.0f}, // cutBaseOffset
    0.0f, // cutWidth
    0.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BTL_DAMAGE1", // damageSfxName
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

/** Defence of the Mario actor: no damage reduction. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of the Mario actor: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of the Mario actor (percent chance to take each status):
 * immune to sleep, stop, dizzy, poison, confusion, electric shock, burn, freeze, growing huge, shrinking,
 * attack up, attack down, defence up, defence down, allergy, fright, gale force, speed up, slow, dodgy,
 * invisibility and instant kill.
 */
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
    {POSE_TYPE_NORMAL, "EM_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KNP_S_1"},
    {POSE_TYPE_DAMAGE, "EM_D_1"},
    {POSE_TYPE_RUN, "EM_R_1"},
    {POSE_TYPE_WALK, "EM_W_1"},
    {POSE_TYPE_TALK, "EM_T_1"},
    {POSE_TYPE_ACTION, "EM_S_1"},
};

/** Data table of the Mario actor: empty; it has no event hooks. */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/** Body part of the Mario actor (model c_mario): hit position, defences, attributes and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_act_mario", "c_mario", 0.0f, 0.0f, 0.0f, -5.0f, 30.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/**
 * Init event of the Mario stage actor: installs the wait event and battle_entry_event as the entry event,
 * hides the status window and starts the wait event. This unit runs the whole stage play.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(_status_window_off)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event of the Mario stage actor: runs the default damage handling with LW(10) = -2 and LW(11) = 1. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Phase event of the Mario stage actor: does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/** Attack event of the Mario stage actor: just restarts the wait event; the play is driven by _battle_event. */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle event of the Mario stage actor: shows the unit's table pose for part 1. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Cast of Flurrie's epilogue stage play: Flurrie as Clauda, two Boos, the Atomic Boo, two Toads,
 * a female Toad and Goombella, with their starting spots.
 */
BattleUnitSetup entry_unit_data[] = {
    {
        &unit_act_clauda, // unitKindParams
        ALLIANCE_PLAYER, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {-140.0f, 0.0f, 5.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {45.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {85.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_atmic_teresa, // unitKindParams
        ALLIANCE_ENEMY, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {150.0f, 40.0f, 0.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_kinopio, // unitKindParams
        ALLIANCE_NEUTRAL, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {-75.0f, 0.0f, -40.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_kinopio, // unitKindParams
        ALLIANCE_NEUTRAL, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {-15.0f, 0.0f, -40.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unit_act_kinopiko, // unitKindParams
        ALLIANCE_NEUTRAL, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {-45.0f, 0.0f, -40.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
    {
        &unitdata_Party_Christine, // unitKindParams
        ALLIANCE_NEUTRAL, // alliance
        {0}, // pad_05
        BATTLE_PHASE_ENEMY, // attackPhase
        {0.0f, -50.0f, 110.0f}, // position
        0, // hitOffsetX
        {0}, // unitWork
        NULL, // itemDropTable
    },
};

/**
 * Entry of Flurrie's stage play "Paper Mario" (her epilogue):
 * hides the status window, places Mario's actor at Mario's spot
 * and moves the real Mario off the left edge of the stage, and keeps its own unit id in unit work 0.
 * It spawns the eight other actors of entry_unit_data (Flurrie as Clauda, two Boos, the Atomic Boo, two Toads,
 * a female Toad and Goombella), keeping their unit ids in unit work 1 to 8, and starts _battle_event.
 * After two seconds the narration (ep_39) tells of Flurrie's hit play about the adventure,
 * with Doopliss in the troupe; then battle flag 32 is set.
 */
static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_MARIO, LW(6), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MARIO_ID, LW(0))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[0]), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CLAUDA_ID, LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[1]), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TERESA1_ID, LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[2]), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TERESA2_ID, LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[3]), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATMIC_TERESA_ID, LW(3))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[4]), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_KINOPIO1_ID, LW(3))
    CALL(btlevtcmd_ChangeFaceDirection, LW(3), 1)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[5]), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_KINOPIO2_ID, LW(3))
    CALL(btlevtcmd_ChangeFaceDirection, LW(3), 1)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[6]), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_KINOPIKO_ID, LW(3))
    CALL(btlevtcmd_ChangeFaceDirection, LW(3), 1)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_unit_data[7]), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHRISTINE_ID, LW(3))
    CALL(btlevtcmd_ChangeFaceDirection, LW(3), 1)
    SPAWN_EVENT(_battle_event)
    WAIT_MS(2000)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("ep_39"), 0, -2)
    WAIT_MS(1000)
    CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_INTERRUPT_STOP)
    RETURN
EVT_END()

/**
 * Looping stage-play script: Mario jumps on an enemy actor,
 * then Flurrie (Clauda) attacks (a body press or her breath, 50/50) and strikes a pose,
 * then the two Boos (Teresas) and the Atomic Boo each take a turn,
 * and a male and a female Toad each say a line (talk pose).
 * It repeats forever; nothing in it ends the battle.
 */
EVT_BEGIN(_battle_event)
    LABEL(0)
    CALL_CHILD(_act_mario_jump_attack)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CLAUDA_ID, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(evt_sub_random, 1, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL_CHILD(_act_clauda_press_attack)
    ELSE
        CALL_CHILD(_act_clauda_breath_attack)
    END_IF
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A_7"))
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_NORMAL)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TERESA1_ID, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_TALK)
    WAIT_MS(1000)
    CALL_CHILD(_act_teresa_attack)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_NORMAL)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TERESA2_ID, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_TALK)
    WAIT_MS(1000)
    CALL_CHILD(_act_teresa_attack)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_NORMAL)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATMIC_TERESA_ID, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_TALK)
    WAIT_MS(1000)
    CALL_CHILD(_act_atmic_teresa_press_attack)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_NORMAL)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_KINOPIO1_ID, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_TALK)
    WAIT_MS(1500)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_NORMAL)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_KINOPIKO_ID, LW(5))
    CALL(btlevtcmd_GetBodyId, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_TALK)
    WAIT_MS(1500)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_NORMAL)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Picks a random enemy actor of the play (unit work 2, 3 or 4: the two Boos and the Atomic Boo) as the target:
 * returns its unit id in LW(3) and its body id in LW(4).
 */
EVT_BEGIN(_act_target_enemy)
    CALL(evt_sub_random, 2, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TERESA1_ID, LW(3))
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TERESA2_ID, LW(3))
        CASE_DEFAULT
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATMIC_TERESA_ID, LW(3))
    END_SWITCH
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    RETURN
EVT_END()

/**
 * Picks Mario (unit work 0) or Clauda (unit work 1) as the target of a Boo attack:
 * returns the unit id in LW(3) and the body id in LW(4).
 */
EVT_BEGIN(_act_target_mario)
    CALL(evt_sub_random, 1, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MARIO_ID, LW(3))
        CASE_DEFAULT
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CLAUDA_ID, LW(3))
    END_SWITCH
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    RETURN
EVT_END()

/**
 * Mario's part of the play: after a talk pose he runs up
 * and jumps on a random enemy actor (which gets a hit effect, a cheer from the audience and its hurt pose),
 * bounces back with a few smaller hops and runs to his home position.
 */
EVT_BEGIN(_act_mario_jump_attack)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_TALK)
    WAIT_MS(1000)
    CALL_CHILD(_act_target_enemy)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    SET_VALUE(LW(0), -20)
    SET_VALUE(LW(1), 0)
    SET_VALUE(LW(2), 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(6))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(6))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1B"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    SET_VALUE(LW(6), 36)
    CALL(btlevtcmd_MarioJumpParam, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6))
    CALL(btlevtcmd_MarioJumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(6), 3, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1D"))
    WAIT_MS(50)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_StopWaitEvent, LW(3))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_DAMAGE)
        WAIT_MS(1000)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_J_1B"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 27)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("EM_R_1"))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    RETURN
EVT_END()

/**
 * Flurrie's (Clauda's) body press: Flurrie (unit in LW(5), body in LW(6)) leaps onto a random enemy actor
 * and squashes it flat while she squashes and stretches,
 * then bounces off and poses (PWD_Y_1) with the audience cheering and floats home; the victim springs back up.
 */
EVT_BEGIN(_act_clauda_press_attack)

    // Flurrie pauses on stage, picks a random actor to flatten and turns toward it.
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_TALK)
    WAIT_MS(1000)
    CALL_CHILD(_act_target_enemy)
    CALL(btlevtcmd_CalculateFaceDirection, LW(5), -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, LW(5), LW(15))

    // She leaps high over the stage and comes down on the victim.
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A1_1"))
    CALL(btlevtcmd_SetFallAccel, LW(5), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_JumpPosition, LW(5), LW(0), LW(1), LW(2), 60, MOVE_SOUND_FROM_TABLE)

    // The victim is squashed flat, stays pressed for a moment, then springs back up to its full height.
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_DAMAGE)
        CALL(evt_sub_intpl_init_float, EASING_CUBIC_OUT, FLOAT(1.0f), FLOAT(0.14941406f), 10)
        LOOP(11)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(3), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(1500)
        CALL(evt_sub_intpl_init_float, EASING_CUBIC_OUT, FLOAT(0.14941406f), FLOAT(1.25f), 10)
        LOOP(11)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(3), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, EASING_WAVE_SHRINKING, FLOAT(1.25f), FLOAT(1.0f), 30)
        LOOP(31)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(3), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(3), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        WAIT_MS(500)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_NORMAL)
    END_SIBLING_EVENT

    // Flurrie squashes on landing and stretches wide as she bounces off.
    CALL(btlevtcmd_JumpContinue, LW(5))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(1.0f), FLOAT(0.39941406f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(5), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(0.39941406f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(5), LW(0), LW(7), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(1.0f), FLOAT(1.5f), 10)
        LOOP(10)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(5), LW(7), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_sub_intpl_init_float, EASING_SINE_IN_OUT, FLOAT(1.5f), FLOAT(1.0f), 6)
        LOOP(6)
            CALL(evt_sub_intpl_get_float, LW(7), EVT_NULLPTR)
            CALL(btlevtcmd_GetScale, LW(5), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetScale, LW(5), LW(7), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT

    // She hops off the victim, strikes her victory pose and the audience cheers.
    CALL(btlevtcmd_GetPos, LW(5), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(5), LW(0), 20)
    ADD(LW(1), 20)
    CALL(btlevtcmd_SetRotate, LW(5), 0, 0, 0)
    CALL(btlevtcmd_SetFallAccel, LW(5), FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPosition, LW(5), LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_Y_1"))
    CALL(evt_audience_joy_ending)
    WAIT_MS(500)
    CALL(btlevtcmd_WaitEventEnd, LW(14))

    // She floats back to her spot on stage.
    CALL(btlevtcmd_SetMoveSpeed, LW(5), FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, LW(5), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, LW(5), LW(0), LW(1), LW(2), FLOAT(6.0f), LW(9))
    CALL(btlevtcmd_DivePosition, LW(5), LW(0), LW(1), LW(2), LW(9), 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_NORMAL)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    RETURN
EVT_END()

/**
 * Flurrie's (Clauda's) breath: she winds up (blue-violet smoke puffs), breathes at all three enemy actors,
 * who each get a hit effect and a cheer and shake while the breath lasts, then the effect is removed,
 * she poses and the audience cheers.
 */
EVT_BEGIN(_act_clauda_breath_attack)
    CALL(btlevtcmd_JumpSetting, LW(5), 20, FLOAT(0.0f), FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_TALK)
    WAIT_MS(1000)
    CALL(_clauda_breath_effect_ready)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A2_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A2_2"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, LW(5), LW(6))
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_A2_3"))
    CALL(_clauda_breath_effect_fire, LW(12))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TERESA1_ID, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_DAMAGE)
        LOOP(30)
            CALL(btlevtcmd_SetDispOffset, LW(3), 2, 0, 0)
            WAIT_FRAMES(2)
            CALL(btlevtcmd_SetDispOffset, LW(3), 0, 0, 0)
            WAIT_FRAMES(2)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_NORMAL)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TERESA2_ID, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_DAMAGE)
        LOOP(30)
            CALL(btlevtcmd_SetDispOffset, LW(3), 2, 0, 0)
            WAIT_FRAMES(2)
            CALL(btlevtcmd_SetDispOffset, LW(3), 0, 0, 0)
            WAIT_FRAMES(2)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_NORMAL)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATMIC_TERESA_ID, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_DAMAGE)
        LOOP(30)
            CALL(btlevtcmd_SetDispOffset, LW(3), 2, 0, 0)
            WAIT_FRAMES(2)
            CALL(btlevtcmd_SetDispOffset, LW(3), 0, 0, 0)
            WAIT_FRAMES(2)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_NORMAL)
    END_SIBLING_EVENT
    WAIT_FRAMES(120)
    CALL(evt_eff_delete_ptr, LW(12))
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("PWD_Y_1"))
    CALL(evt_audience_joy_ending)
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_NORMAL)
    RETURN
EVT_END()

/**
 * A Boo's attack in the play: the Boo (unit in LW(5), body in LW(6)) fades out,
 * reappears beside Mario or Flurrie (picked at random), hits it, and floats back home;
 * the target gets a hit effect and its hurt pose and the audience cheers.
 */
EVT_BEGIN(_act_teresa_attack)
    CALL_CHILD(_act_target_mario)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_RUN)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetAlpha, LW(5), LW(6), 255)
        SET_VALUE(LW(7), 256)
        LOOP(16)
            WAIT_FRAMES(1)
            SUBTRACT(LW(7), 16)
            CALL(btlevtcmd_SetAlpha, LW(5), LW(6), LW(7))
        END_LOOP
        WAIT_FRAMES(20)
        CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("A_1"))
        CALL(btlevtcmd_SetAlpha, LW(5), LW(6), 0)
        SET_VALUE(LW(7), -1)
        LOOP(8)
            WAIT_FRAMES(1)
            ADD(LW(7), 32)
            CALL(btlevtcmd_SetAlpha, LW(5), LW(6), LW(7))
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(5), LW(0), 20)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, LW(5), LW(0), LW(1), LW(2), 60, -5, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetAlpha, LW(5), LW(6), 255)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("A_2"))
    WAIT_MS(250)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_StopWaitEvent, LW(3))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_DAMAGE)
        WAIT_MS(1000)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePoseType, LW(5), LW(6), POSE_TYPE_RUN)
    CALL(btlevtcmd_AddPartsRotate, LW(5), LW(6), 0, 180, 0)
    CALL(btlevtcmd_GetHomePos, LW(5), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, LW(5), LW(0), LW(1), LW(2), 40, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetPartsRotate, LW(5), LW(6), 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_NORMAL)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    RETURN
EVT_END()

/**
 * The Atomic Boo's attack in the play: it rises above the left side of the stage and drops onto Mario
 * and Flurrie, both getting a hit effect, their hurt pose and a cheer, then it returns to its home position.
 */
EVT_BEGIN(_act_atmic_teresa_press_attack)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MARIO_ID, LW(3))
    CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("R_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 10)
    CALL(btlevtcmd_DivePosition, LW(5), -125, 100, LW(2), 120, 10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("Z_1"))
    CALL(btlevtcmd_DivePosition, LW(5), -125, 0, LW(2), 30, 0, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MARIO_ID, LW(3))
        CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
        CALL(btlevtcmd_StopWaitEvent, LW(3))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_DAMAGE)
        WAIT_MS(1000)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CLAUDA_ID, LW(3))
        CALL(btlevtcmd_GetBodyId, LW(3), LW(4))
        CALL(btlevtcmd_StopWaitEvent, LW(3))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        ADD(LW(2), 5)
        CALL(evt_eff, 0, STRING("hit"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 3, 0, 0, 0, 0, 0, 0)
        CALL(evt_audience_joy_ending)
        CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_DAMAGE)
        WAIT_MS(1000)
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_SIBLING_EVENT
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("W_1"))
    CALL(btlevtcmd_GetHomePos, LW(5), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, LW(5), LW(0), LW(1), LW(2), 60, 50, EASING_QUARTIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, LW(5), LW(6), STRING("S_1"))
    RETURN
EVT_END()

/**
 * Script helper: spawns the smoke puff effect (effKumokumoN64Entry, blue-to-violet colours) in front of the
 * current event's owning unit (UNIT_ID_SELF).
 * Stores the effect pointer in the script variable args[0] so the script can delete it later.
 */
static USER_FUNC(_clauda_breath_effect_fire) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    BattleUnit* unit;
    s32 var;
    Vec pos;
    EffectEntry* eff;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(wp, id);
    var = args[0];
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.x += unit->sizeMultiplier * (unit->faceDirection * 25);
    pos.y += 15.0f * unit->sizeMultiplier;
    eff = effKumokumoN64Entry(0, 0, pos.x, pos.y, pos.z, 0.0f, -2.0f, 1.0f);
    ((EffKumokumoN64Particle*)eff->userdata)->r1 = 76;
    ((EffKumokumoN64Particle*)eff->userdata)->g1 = 167;
    ((EffKumokumoN64Particle*)eff->userdata)->b1 = 202;
    ((EffKumokumoN64Particle*)eff->userdata)->r2 = 125;
    ((EffKumokumoN64Particle*)eff->userdata)->g2 = 27;
    ((EffKumokumoN64Particle*)eff->userdata)->b2 = 245;
    ((EffKumokumoN64Particle*)eff->userdata)->rot = -10.0f;
    evtSetValue(event, var, (EvtWord)eff);
    return EVT_RETURN_DONE;
}

/** Script helper: spawns the smaller smoke puff effect in front of the current event's owning unit during the breath windup. */
static USER_FUNC(_clauda_breath_effect_ready) {
    BattleWork* wp = _battleWorkPointer;
    s32 id;
    BattleUnit* unit;
    Vec pos;
    EffectEntry* eff;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.x += unit->sizeMultiplier * (unit->faceDirection * 20);
    pos.y += 25.0f * unit->sizeMultiplier;
    eff = effKumokumoN64Entry(0, 60, pos.x, pos.y, pos.z, 0.0f, 2.0f, 1.0f);
    ((EffKumokumoN64Particle*)eff->userdata)->r1 = 76;
    ((EffKumokumoN64Particle*)eff->userdata)->g1 = 167;
    ((EffKumokumoN64Particle*)eff->userdata)->b1 = 202;
    ((EffKumokumoN64Particle*)eff->userdata)->r2 = 125;
    ((EffKumokumoN64Particle*)eff->userdata)->g2 = 27;
    ((EffKumokumoN64Particle*)eff->userdata)->b2 = 245;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: turns the battle status window off (statusWindowOff = 1)
 * so it does not show during the stage play.
 */
USER_FUNC(_status_window_off) {
    _battleWorkPointer->statusWindowOff = 1;
    return EVT_RETURN_DONE;
}

