// Battle unit Gold Fuzzy (`gold_chorobon`): tst.rel's copy of rels/hei/src/unit/unit_gold_chorobon.c.
#include "version.h"
#include "unit/chorogun_tst.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "manager/evtmgr_cmd.h"
#include "unit/chorogun_tst.h"
#include "battle/battle_sub.h"
#include "evt/evt_msg.h"
#include "tst.h"
#include "battle/battle_damage.h"
#include "battle/battle_camera.h"

// Unit work slots of the Gold Fuzzy (btlevtcmd_SetUnitWork / GetUnitWork).
enum {
    UNIT_WORK_CHOROGUN_CALLED = 0, // 1 once the Fuzzy Horde has been spawned and called in
    UNIT_WORK_CHOROGUN_GONE = 1,   // 1 once the horde has been sent away (so it only happens once)
    UNIT_WORK_CHOROGUN = 2,   // unit id of the spawned Fuzzy Horde
};

USER_FUNC(get_gold_chorobon_id);

BattleUnitKind unit_gold_chorobon;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[15];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_gold_chorobon_attack;
static PoseSoundTiming pose_sound_timing_table[4];
extern EvtWord gold_chorobon_rabbit_move[];
BattleUnitSetup entry_chorogun;
extern EvtWord call_chorogun_event[];
static EvtWord init_event[43];

#if VERSION <= VERSION_DEMO_JP
static EvtWord damage_event[104];
#else
static EvtWord damage_event[131];
#endif

static EvtWord counter_damage_event[30];
static EvtWord return_event[24];
static EvtWord attack_event[500];
static EvtWord wait_event[6];

/**
 * Unit kind of Gold Fuzzy: 10 max HP, level 27. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_gold_chorobon = {
    UNIT_GOLD_FUZZY,
    "btl_un_gold_chorobon",
    10,
    0,
    1,
    1,
    27,
    0,
    10,
    100,
    0,
    50,
    5,
    28,
    32,
    5,
    32,
    0.0f,
    16.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    14.0f,
    0.0f,
    0.0f,
    14.0f,
    20.8f,
    0.0f,
    0.0f,
    16.0f,
    0.0f,
    28.0f,
    32.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_G_CHORO_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kUnitLeader,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

/**
 * Defence of Gold Fuzzy per element (normal, fire, ice, explosion, electric), subtracted from each hit:
 * none.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of Gold Fuzzy: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of Gold Fuzzy, as a percent multiplier on each move's status chance:
 * immune to Sleep, Stop, Dizzy, Confuse, Freeze, Tiny, Attack Down, Defense Down, Allergic, Fright,
 * Gale Force, Slow and instant KO; the normal chance (100) for Poison, Electric, Burn, Huge, Attack Up, Defense Up,
 * Fast, Dodgy and Invisible.
 */
static StatusVulnerability regist = {0, 0, 0, 100, 0, 100, 100, 0, 100, 0, 100, 0, 100, 0, 0, 0, 0, 100, 0, 100, 100, 0};

/**
 * Pose table of Gold Fuzzy: maps its battle pose types (stay, walk, hurt, dead, talk and so on) to
 * animation names, 18 entries such as CBN_N_1, CBN_Y_1, CBN_K_1, CBN_X_1.
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "CBN_N_1"},
    {POSE_TYPE_STOP, "CBN_Y_1"},
    {POSE_TYPE_FREEZE, "CBN_Y_1"},
    {POSE_TYPE_CONFUSE, "CBN_K_1"},
    {POSE_TYPE_POISON, "CBN_X_1"},
    {POSE_TYPE_DIZZY, "CBN_X_1"},
    {BTL_POSE_NORMAL, "CBN_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "CBN_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "CBN_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "CBN_S_1"},
    {BTL_POSE_DAMAGE, "CBN_D_1"},
    {POSE_TYPE_ATTACK, "CBN_A_1"},
    {POSE_TYPE_RUN, "CBN_R_1"},
    {POSE_TYPE_WALK, "CBN_W_1"},
    {POSE_TYPE_DANGER, "CBN_X_1"},
    {POSE_TYPE_PERIL, "CBN_X_1"},
    {POSE_TYPE_TALK, "CBN_T_1"},
    {POSE_TYPE_ACTION, "CBN_S_1"},
};

/**
 * Event table of Gold Fuzzy: maps event tags to the scripts that react to them (counter_damage_event);
 * tag 0 ends it.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_COUNTER_TOP_SPIKY_EVENT, counter_damage_event},
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

/**
 * Body parts of Gold Fuzzy: a single part (model c_chorobon_k) holding its hit box.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_gold_chorobon", "c_chorobon_k", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
};

/** Attack data of Gold Fuzzy's attack: base damage 1, 100% accuracy. */
BattleWeapon weapon_gold_chorobon_attack = {
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    0, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
 * Sound cues of Gold Fuzzy's CBN_S_1 animation: 3 timing entries for
 * SFX_BOSS_G_CHORO_MOVE1, SFX_BOSS_G_CHORO_WAIT2 and SFX_BOSS_G_CHORO_WAIT1.
 */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"CBN_S_1", 0.5833333f, NULL, "SFX_BOSS_G_CHORO_MOVE1", 0, 1},
    {"CBN_S_1", 0.8333333f, NULL, "SFX_BOSS_G_CHORO_WAIT2", 0, 1},
    {"CBN_S_1", 1.5833334f, NULL, "SFX_BOSS_G_CHORO_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/**
 * Hops the Gold Fuzzy along x toward LW(5) in hops of LW(8) units at height LW(6), squashing before each,
 * then makes a final jump onto (LW(5), LW(6), LW(7)) with SFX_BOSS_G_CHORO_ATTACK1.
 */
EVT_BEGIN(gold_chorobon_rabbit_move)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LABEL(0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(11), LW(5))
    SUBTRACT(LW(11), LW(0))
    SET_VALUE(LW(12), LW(11))
    IF_INT_LESS(LW(12), 0)
        MULTIPLY(LW(12), -1)
    END_IF
    IF_INT_GREATER(LW(12), LW(8))
        IF_INT_GREATER_OR_EQUAL(LW(11), 0)
            ADD(LW(0), LW(8))
        ELSE
            SUBTRACT(LW(0), LW(8))
        END_IF
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(6), LW(2), 0, -1)
        LOOP(2)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        GOTO(0)
    ELSE
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(5), LW(6), LW(7), 0, MOVE_SOUND_NONE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_G_CHORO_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        LOOP(2)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_IF
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/**
 * Spawn setup for a unit that the Gold Fuzzy script adds to the battle mid-fight:
 * unit kind unit_chorobon_gundan on the enemy side at (75, 0, 0).
 */
BattleUnitSetup entry_chorogun = {
    &unit_chorobon_gundan, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {75.0f, 0.0f, 0.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Gold Fuzzy calls the Fuzzy Horde: it holds its pose, the camera swings out, a message (stg1_hei_43) plays,
 * then chorogun_appear brings the swarm in while Mario looks around.
 */
EVT_BEGIN(call_chorogun_event)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_S_1"))
    WAIT_MS(1000)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 120, 40, 200, 120, -10, -500, 30, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_reset_move_color_lv_all)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg1_hei_43"), 0, UNIT_ID_SELF)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    WAIT_MS(1000)
    BEGIN_SIBLING_EVENT
        WAIT_MS(500)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_7"))
    END_SIBLING_EVENT
    CALL(chorogun_appear)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_MARIO, 1)
    RETURN
EVT_END()

/**
 * Run by the Fuzzy Horde's damage event, as the Horde, on the final hit of an attack once 500 or fewer Fuzzies
 * are left: waits 2 seconds;
 * from JP on it stops if the Horde is gone or the Gold Fuzzy's unit work 1 is already set, and otherwise sets it
 * and pays out the Horde's coins and EXP (chorogun_dead_reward).
 * Then every swarm Fuzzy runs away (chorogun_escape_all) and a second later the Horde unit (the Gold Fuzzy's
 * unit work 2) is removed; the early demos only set the flag at the end.
 */
EVT_BEGIN(escape_chorogun_event)
    WAIT_MS(2000)
    CALL(get_gold_chorobon_id, LW(4))
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(4), UNIT_WORK_CHOROGUN_GONE, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, LW(4), UNIT_WORK_CHOROGUN_GONE, 1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(10))
    CALL_CHILD(chorogun_dead_reward)
#endif
    CALL(chorogun_escape_all)
    WAIT_MS(1000)
    CALL(btlevtcmd_GetUnitWork, LW(4), UNIT_WORK_CHOROGUN, LW(3))
    CALL(btlevtcmd_KillUnit, LW(3), 0)
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_SetUnitWork, LW(4), UNIT_WORK_CHOROGUN_GONE, 1)
#endif
    RETURN
EVT_END()

/**
 * Init script of the Gold Fuzzy: installs the wait,
 * attack and damage handlers (confusion uses the default Confuse script), starts the wait event,
 * spawns its Fuzzy horde setup (chorogun_entry), clears unit work 0 and 1 (horde called / dying flags),
 * and sets the pose sound table and jump and landing sounds.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL_CHILD(chorogun_entry)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_CALLED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_GONE, 0)
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_BOSS_G_CHORO_MOVE1"), STRING("SFX_BOSS_G_CHORO_LANDING1"))
    RETURN
EVT_END()

/**
 * Damage handler of the Gold Fuzzy.
 * If its Fuzzy horde has been summoned and this hit would knock it out (damage flag 0x200 without the Life
 * Shroom flag 0x800), it hands out the horde's rewards in JP and later (chorogun_dead_reward),
 * makes all the Fuzzies escape and removes the horde unit.
 * It then runs the default damage script, and the first time its HP drops to 7
 * or less (but not 0) it spawns the Fuzzy horde (entry_chorogun) with call_chorogun_event.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_GONE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_CALLED, LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL(chorogun_escape_all)
                        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN, LW(3))
                        CALL(btlevtcmd_KillUnit, LW(3), 0)
                        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_GONE, 1)
#else
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN, LW(0))
    CALL(btlevtcmd_GetUnitId, LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(1), -1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_GONE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_CALLED, LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_GONE, 1)
                        CALL(btlevtcmd_WaitAttackEnd)
                        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN, LW(10))
                        CALL_CHILD(chorogun_dead_reward)
                        SET_VALUE(LW(10), -2)
                        CALL(chorogun_escape_all)
                        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN, LW(3))
                        CALL(btlevtcmd_KillUnit, LW(3), 0)
                    END_IF
#endif
                END_IF
            END_IF
        END_IF
    END_IF
    CALL_CHILD(btldefaultevt_Damage)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_CALLED, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 7)
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
#if VERSION >= VERSION_JP
                CALL(btlevtcmd_WaitAttackEnd)
#endif
                CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_chorogun), 0)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN, LW(3))
                CALL_CHILD(call_chorogun_event)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_CHOROGUN_CALLED, 1)
                GOTO(99)
            END_IF
        END_IF
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * Damage handler for when the Gold Fuzzy was hit by a counter attack:
 * resets its scale, runs the shared counter-damage script,
 * and if it is still alive hops back home with return_event.
 */
static EVT_BEGIN(counter_damage_event)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(subsetevt_counter_damage)
    CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(return_event)
    END_IF
    RETURN
EVT_END()

/**
 * Hops the Gold Fuzzy back to its home position in 40-unit hops (gold_chorobon_rabbit_move) after an attack
 * or a counter hit.
 */
static EVT_BEGIN(return_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    SET_VALUE(LW(8), 40)
    CALL_CHILD(gold_chorobon_rabbit_move)
    RETURN
EVT_END()

/**
 * The Gold Fuzzy's attack (weapon_gold_chorobon_attack): it picks a target, hops up to it and jumps on it;
 * a miss makes it bounce off, a hit shakes the camera and deals the damage with the guard-timing check,
 * after which it bounces back off the target, hops twice in place and hops home with return_event.
 */
static EVT_BEGIN(attack_event)

    // The Gold Fuzzy first checks whether to use a held item instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // It picks a target; with none, it ends the turn (or flusters when confused).
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_gold_chorobon_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_gold_chorobon_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gold_chorobon_attack))

    // The attack is declared and paid for, and the camera follows the Fuzzy.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_gold_chorobon_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 100)

    // It hops toward the target and jumps up onto it.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(5), LW(6), LW(7))
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(5), 50)
    SET_VALUE(LW(8), 50)
    CALL_CHILD(gold_chorobon_rabbit_move)
    WAIT_FRAMES(20)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 60, 0, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 2)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)

    // The damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gold_chorobon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // On a miss it bounces off the target and lands behind it (30 units against the way the target faces).
    LABEL(90)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(3.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 5)
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(1.5f), FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 30)
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_N_1"))
    WAIT_MS(1000)
    GOTO(98)

    // On a hit the camera shakes, the action-command defence is resolved and the Fuzzy damages the target.
    LABEL(91)
    LOOP(4)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.07910156f), FLOAT(-0.119140625f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gold_chorobon_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gold_chorobon_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LOOP(4)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.07910156f), FLOAT(0.119140625f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)

    // It bounces back off the target to the ground in two shorter jumps, then hops twice in place (CBN_A_1).
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, -1)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 10, -1)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_A_1"))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, -1)
        WAIT_FRAMES(16)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 6, -1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_S_1"))
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_FRAMES(10)
    GOTO(98)

    // It hops back home and the camera resets.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 0)
    CALL_CHILD(return_event)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle script: sets the standard waiting pose from the unit's pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Script helper: args[0] receives the unit index of the Gold Fuzzy (initial kind 0x14),
 * or -1 when it is not on the field.
 */
USER_FUNC(get_gold_chorobon_id) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    s32 i;

    for (i = 0; i < 64; i++) {
        BattleUnit* unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == UNIT_GOLD_FUZZY) {
            break;
        }
    }
    if (i != 64) {
        evtSetValue(event, args[0], i);
    } else {
        evtSetValue(event, args[0], -1);
    }
    return EVT_RETURN_DONE;
}
