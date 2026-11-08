// Battle unit Fuzzy Horde (`chorobon_gundan`): tst.rel's copy of rels/hei/src/unit/unit_chorobon_gundan.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "dolphin/mtx.h"
#include "system.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "driver/camdrv.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "driver/extdrv.h"
#include "manager/evtmgr_cmd.h"
#include "unit/unit_chorobon_gundan.h"
#include "battle/battle_sub.h"
#include "evt/evt_ext.h"
#include "evt/evt_npc.h"
#include "libc_decls.h"
#include "battle/battle_damage.h"
#include "battle/battle_camera.h"

// Unit work slots (btlevtcmd_SetUnitWork / GetUnitWork)
enum {
    UW_CHOROBON_GUNDAN_FEW_LEFT = 0, // 1 once a hit finds 50 or fewer Fuzzies left in the horde
};

Chorogun* chorogun;
s32 chorogun_targetId;

USER_FUNC(chorogun_after_attack);
USER_FUNC(chorogun_approach_target);
USER_FUNC(chorogun_bound_target);
USER_FUNC(chorogun_counterd);
USER_FUNC(chorogun_dive_target);
USER_FUNC(chorogun_escape);
USER_FUNC(chorogun_get_alive_num);
USER_FUNC(chorogun_set_target_id);
void chorogun_ext_disp(CameraId camera, void* param);
void chorogun_ext_main(void);
void chorogun_ext_init(void);
BattleUnit* chorogun_get_ptr(void);

BattleUnitKind unit_chorobon_gundan;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[15];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
BattleWeapon weapon_chorobon_gundan_attack;
static EvtWord init_event[26];

#if VERSION <= VERSION_DEMO_JP
static EvtWord damage_event[80];
#else
static EvtWord damage_event[59];
#endif

static EvtWord attack_event[217];
static EvtWord wait_event[6];
ExtTexInfo chorogun_ext_entry_data[21];

/** Finds the Fuzzy Horde unit (initial kind 0x15) and returns it, or NULL when there is none. */
BattleUnit* chorogun_get_ptr(void) {
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == UNIT_FUZZY_HORDE) {
            return unit;
        }
    }
    return NULL;
}

/**
 * Unit kind of Fuzzy Horde: 20 max HP, level 19. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_chorobon_gundan = {
    UNIT_FUZZY_HORDE,
    "btl_un_chorobon_gundan",
    20,
    0,
    1,
    1,
    19,
    0,
    5,
    100,
    0,
    50,
    5,
    260,
    50,
    0,
    50,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    130.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    130.0f,
    0.0f,
    0.0f,
    130.0f,
    32.5f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    260.0f,
    50.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_TEAM_CHORO_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
#if VERSION <= VERSION_DEMO_JP
    kUnitLeader | kUnitNoSpin | kUnitNoHpGauge,
#else
    kUnitLeader | kUnitNoSpin,
#endif
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

/**
 * Defence of Fuzzy Horde per element (normal, fire, ice, explosion, electric), subtracted from each hit:
 * none.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of Fuzzy Horde: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Status vulnerability of Fuzzy Horde: immune to every status effect. */
static StatusVulnerability regist = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/**
 * Pose table of Fuzzy Horde: maps its battle pose types (stay, walk, hurt, dead, talk and so on) to
 * animation names, 15 entries such as CBN_N_1, CBN_Y_1, CBN_K_1, CBN_X_1.
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "CBN_N_1"},
    {POSE_TYPE_STOP, "CBN_Y_1"},
    {POSE_TYPE_FREEZE, "CBN_Y_1"},
    {POSE_TYPE_CONFUSE, "CBN_K_1"},
    {POSE_TYPE_POISON, "CBN_X_1"},
    {POSE_TYPE_DIZZY, "CBN_X_1"},
    {POSE_TYPE_NORMAL, "CBN_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "CBN_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "CBN_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "CBN_S_1"},
    {POSE_TYPE_DAMAGE, "CBN_D_1"},
    {POSE_TYPE_ATTACK, "CBN_A_1"},
    {POSE_TYPE_RUN, "CBN_R_1"},
    {POSE_TYPE_WALK, "CBN_W_1"},
    {POSE_TYPE_ACTION, "CBN_S_1"},
};

/**
 * Event table of Fuzzy Horde: maps event tags to the scripts that react to them (no custom reactions);
 * tag 0 ends it.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Body parts of Fuzzy Horde: a single part (model c_chorobon) holding its hit box.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_chorobon_gundan", "c_chorobon", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHidden | kNoShadow, 0, pose_table},
};

/** Attack data of Fuzzy Horde's attack: base damage 1, 100% accuracy. */
BattleWeapon weapon_chorobon_gundan_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
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
    TARGET_PROP_NO_FLOATING2 | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

#if VERSION >= VERSION_JP

/**
 * Reward script for the unit LW(10) when the horde is sent off (escape_chorogun_event):
 * plays its death sound, pays out its coins and EXP and spawns the star point effect above it.
 */
EVT_BEGIN(chorogun_dead_reward)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
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
    RETURN
EVT_END()
#endif

/**
 * Init script of the Fuzzy Horde: installs the wait,
 * attack and damage handlers (confusion uses the default Confuse script),
 * clears unit work 0 (set once 50 or fewer Fuzzies are left) and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CHOROBON_GUNDAN_FEW_LEFT, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage handler of the Fuzzy Horde: for each point of damage up to 50 Fuzzies leave the swarm (chorogun_escape)
 * while more than 50 remain; once 50 or fewer are left it sets unit work 0 to 1.
 * On the final hit of an attack, it runs escape_chorogun_event if 500 or fewer are alive;
 * the early demos instead remove the unit on that final hit when unit work 0 is set.
 */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_GetDamage, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(0), 0)
    LOOP(0)
        IF_INT_GREATER_OR_EQUAL(LW(0), LW(15))
            BREAK_LOOP
        END_IF
        CALL(chorogun_get_alive_num, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), 50)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UW_CHOROBON_GUNDAN_FEW_LEFT, 1)
        ELSE
            CALL(chorogun_escape)
        END_IF
        ADD(LW(0), 1)
    END_LOOP
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UW_CHOROBON_GUNDAN_FEW_LEFT, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_btl_camera_set_mode, 0, 0)
        CALL(chorogun_escape)
        CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
        GOTO(99)
    END_IF
#endif
    CALL(chorogun_get_alive_num, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 500)
        CALL_CHILD(escape_chorogun_event)
        GOTO(99)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * Attack script of the Fuzzy Horde: after the item-use check,
 * picks a target and the swarm flies in (chorogun_approach_target);
 * if the attack connects it dives onto the target (alive count + 100) / 200 times (at least once),
 * each time with the guard check and a damage check, with the final-hit flag on the last dive.
 * Misses wait one second; the swarm then returns (chorogun_after_attack).
 */
static EVT_BEGIN(attack_event)
    // The Fuzzy Horde first checks whether to use a held item instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // It picks a target; with none, it ends the turn.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_chorobon_gundan_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_chorobon_gundan_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_chorobon_gundan_attack))

    // The attack is declared and paid for, and the camera follows the swarm.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_chorobon_gundan_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(chorogun_set_target_id, LW(3))
    CALL(chorogun_approach_target)

    // The swarm flies in toward the target; the damage check decides between a miss and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_chorobon_gundan_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // On a miss the swarm hovers for a second.
    LABEL(90)
    WAIT_MS(1000)
    GOTO(98)

    // On a hit it dives onto the target several times, depending on how many Fuzzies are left, with a guard check each time.
    LABEL(91)
    CALL(chorogun_get_alive_num, LW(15))
    ADD(LW(15), 100)
    DIVIDE(LW(15), 200)
    IF_INT_LESS_OR_EQUAL(LW(15), 0)
        SET_VALUE(LW(15), 1)
    END_IF
    SET_VALUE(LW(14), 1)
    LOOP(0)
        IF_INT_GREATER(LW(14), LW(15))
            BREAK_LOOP
        END_IF
        CALL(chorogun_dive_target, 30, LW(6))
        WAIT_FRAMES(30)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_chorobon_gundan_attack))
        IF_INT_EQUAL(LW(14), LW(15))
            CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_chorobon_gundan_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
        ELSE
            CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_chorobon_gundan_attack), 0, LW(5))
        END_IF
        IF_INT_EQUAL(LW(5), CHECK_DAMAGE_SUPERGUARDED)
            CALL(chorogun_counterd, LW(6))
        ELSE
            CALL(chorogun_bound_target, LW(6))
        END_IF
        ADD(LW(14), 1)
        WAIT_FRAMES(30)
    END_LOOP
    GOTO(98)

    // The swarm flies back home and the camera resets.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(chorogun_after_attack)
    WAIT_MS(1000)
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
 * Texture entries for the Fuzzy Horde swarm: 20 copies of the Fuzzy model animation CBN_W_1 with start
 * offsets 0, 2, 4 ... 38 frames, so the swarm does not wiggle in step; the {0} entry ends the list.
 */
ExtTexInfo chorogun_ext_entry_data[] = {
    {"c_chorobon", "CBN_W_1", 0.0f},
    {"c_chorobon", "CBN_W_1", 2.0f},
    {"c_chorobon", "CBN_W_1", 4.0f},
    {"c_chorobon", "CBN_W_1", 6.0f},
    {"c_chorobon", "CBN_W_1", 8.0f},
    {"c_chorobon", "CBN_W_1", 10.0f},
    {"c_chorobon", "CBN_W_1", 12.0f},
    {"c_chorobon", "CBN_W_1", 14.0f},
    {"c_chorobon", "CBN_W_1", 16.0f},
    {"c_chorobon", "CBN_W_1", 18.0f},
    {"c_chorobon", "CBN_W_1", 20.0f},
    {"c_chorobon", "CBN_W_1", 22.0f},
    {"c_chorobon", "CBN_W_1", 24.0f},
    {"c_chorobon", "CBN_W_1", 26.0f},
    {"c_chorobon", "CBN_W_1", 28.0f},
    {"c_chorobon", "CBN_W_1", 30.0f},
    {"c_chorobon", "CBN_W_1", 32.0f},
    {"c_chorobon", "CBN_W_1", 34.0f},
    {"c_chorobon", "CBN_W_1", 36.0f},
    {"c_chorobon", "CBN_W_1", 38.0f},
    {0},
};

/**
 * Creates the swarm: adds the hidden NPC model ext_chorobon (c_chorobon) at y = -1000
 * and registers a 1000-pose extdrv object driven by chorogun_ext_init and chorogun_ext_main.
 */
EVT_BEGIN(chorogun_entry)
    CALL(evt_npc_entry, STRING("ext_chorobon"), STRING("c_chorobon"))
    CALL(evt_npc_set_position, STRING("ext_chorobon"), 0, -1000, 0)
    CALL(evt_ext_entry, 1000, PTR(chorogun_ext_entry_data), PTR(chorogun_ext_init), PTR(chorogun_ext_main), 0)
    RETURN
EVT_END()

/**
 * Initialises the 1000 swarm Fuzzies: allocates their records,
 * parks each pose off screen and gives it a start position on a jittered grid (50 per row, 5 units apart in x, 7
 * in z), then shuffles the positions with 500 random swaps.
 */
void chorogun_ext_init(void) {
    s32 i;
    s32 a, b;
    f32 tx, ty, tz;
    ExtdrvPose* pose = extGetPosePtr();

    chorogun = BattleAlloc(sizeof(Chorogun) * 1000);
    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        pose->flags = EXT_POSE_FLAG_VISIBLE;
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = 0.0f;
        (*pose->mtx)[1][3] = -1000.0f;
        (*pose->mtx)[2][3] = 0.0f;
        pose->dir = 90.0f;
        chorogun[i].flags = 0;
        chorogun[i].state = 0;
        chorogun[i].vel = (Vec){0.0f, 0.0f, 0.0f};
        if (i == 0) {
            chorogun[i].timer = 22;
        } else {
            chorogun[i].timer = irand(45);
        }
        chorogun[i].pos.x = -50.0f + 5.0f * (i % 50) + (10.0f * rand() / 32767.0f - 5.0f);
        chorogun[i].pos.y = 0.0f;
        chorogun[i].pos.z = -70.0f + 7.0f * (i / 50) + (5.0f * rand() / 32767.0f - 2.5f);
    }
    for (i = 0; i < 500; i++) {
        a = irand(extGetPoseNum());
        b = irand(extGetPoseNum());
        tx = chorogun[a].pos.x;
        ty = chorogun[a].pos.y;
        tz = chorogun[a].pos.z;
        chorogun[a].pos.x = chorogun[b].pos.x;
        chorogun[a].pos.y = chorogun[b].pos.y;
        chorogun[a].pos.z = chorogun[b].pos.z;
        chorogun[b].pos.x = tx;
        chorogun[b].pos.y = ty;
        chorogun[b].pos.z = tz;
    }
}

/**
 * Per-frame update of the swarm: runs each Fuzzy's state machine and moves its pose matrix:
 * entrance (1 to 21: drop from a random spot in the sky, hop to its slot and keep hopping with the Horde unit),
 * hopping over to the target (30 to 40), a dive (45), a bounce off the target (46), a knock-back from a counter
 * (48), the regroup after an attack (50) and running away off the right edge (60 to 99).
 * Then it queues chorogun_ext_disp.
 */
void chorogun_ext_main(void) {
    BattleWork* wp;
    BattleUnit* unit;
    s32 i;
    s32 n;
    ExtdrvPose* pose;
    Chorogun* c;
    f32 d;
    f32 dz;
    f32 dx;
    f32 tz;
    f32 tx;
    f32 dx2;
    f32 dz2;
    f32 x, y, z;

    wp = _battleWorkPointer;
    unit = chorogun_get_ptr();
    pose = extGetPosePtr();
    n = 0;
    for (i = 0; i < extGetPoseNum(); i++) {
        if (chorogun[i].flags & CHOROGUN_FLAG_ALIVE) {
            n++;
        }
    }
    n = 1000 - n;
    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        pose->texId = (gp->retraceCount + i * i) % 20;
        c = &chorogun[i];
        (*pose->mtx)[0][3] += c->vel.x;
        (*pose->mtx)[1][3] += c->vel.y;
        (*pose->mtx)[2][3] += c->vel.z;
        switch (c->state) {
            case 0:
                break;
            case 1:
                c->timer--;
                if (c->timer > 0) {
                    break;
                }
                (*pose->mtx)[0][3] = 500.0f * rand() / 32767.0f - 250.0f;
                (*pose->mtx)[1][3] = 250.0f + 100.0f * rand() / 32767.0f;
                (*pose->mtx)[2][3] = 800.0f * rand() / 32767.0f - 100.0f;
                c->vel.x = 0.0f;
                c->vel.y = 0.0f;
                c->vel.z = 0.0f;
                c->speed = 3.0f;
                c->state = 2;
                c->timer = 0;
                if (i == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_FALL2", 0x7F, 0, &(Vec){0.0f, 200.0f, 200.0f});
                }
                break;
            case 2:
                if ((*pose->mtx)[1][3] <= 0.0f) {
                    if (i % 100 == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_FALL1", 0x7F, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                    }
                    c->state = 10;
                } else {
                    c->vel.y += -0.25f;
                }
                break;
            case 10:
                if ((*pose->mtx)[1][3] <= 0.0f) {
                    dx = c->pos.x - (*pose->mtx)[0][3];
                    dz = c->pos.z - (*pose->mtx)[2][3];
                    d = sqrt(dx * dx + dz * dz);
                    if (i % 100 == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_TOGETHER1", 0x7F, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                    }
                    if (d <= 5.0f) {
                        c->state = 20;
                    } else {
                        if (d <= 20.0f) {
                            c->speed *= 0.9f;
                        }
                        c->vel.x = c->speed * (dx / d);
                        c->vel.y = c->speed;
                        c->vel.z = c->speed * (dz / d);
                        (*pose->mtx)[1][3] = 0.0f;
                    }
                } else {
                    c->vel.y += -0.5f;
                }
                break;
            case 20:
                (*pose->mtx)[1][3] = 0.0f;
                c->vel.x = 0.0f;
                c->vel.y = c->speed;
                c->vel.z = 0.0f;
                c->state = 21;
                break;
            case 21:
                d = unit->position.x - unit->homePosition.x + intplGetValue(EASING_LINEAR, c->pos.x, unit->homePosition.x, n, 1000);
                tz = unit->position.z - unit->homePosition.z + intplGetValue(EASING_LINEAR, c->pos.z, unit->homePosition.z, n, 1000);
                if ((*pose->mtx)[1][3] <= unit->position.y) {
                    dx2 = d - (*pose->mtx)[0][3];
                    dz2 = tz - (*pose->mtx)[2][3];
                    d = sqrt(dx2 * dx2 + dz2 * dz2);
                    if (i % 100 == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_WAIT1", 0x60, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                    }
                    c->speed = 3.0f + 0.5f * rand() / 32767.0f;
                    c->vel.x = c->speed * (dx2 / d);
                    c->vel.y = c->speed;
                    c->vel.z = c->speed * (dz2 / d);
                    (*pose->mtx)[1][3] = unit->position.y;
                } else {
                    c->vel.y += -0.5f;
                }
                break;
            case 30:
                c->state = 31;
                c->timer = 0;
                BtlUnit_GetPos(BattleGetUnitPtr(wp, chorogun_targetId), &x, &y, &z);
                x += 50.0f * rand() / 32767.0f - 25.0f;
                c->target.x = x;
                z += 50.0f * rand() / 32767.0f - 25.0f;
                c->target.z = z;
            case 31:
                if ((*pose->mtx)[1][3] <= 0.0f) {
                    dz2 = c->target.x - (*pose->mtx)[0][3];
                    dx2 = c->target.z - (*pose->mtx)[2][3];
                    d = sqrt(dz2 * dz2 + dx2 * dx2);
                    if (i % 100 == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_MOVE1", 0x60, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                    }
                    if (d <= 30.0f || c->timer >= 240) {
                        c->state = 40;
                    } else {
                        c->vel.x = c->speed * (dz2 / d);
                        c->vel.y = c->speed;
                        c->vel.z = c->speed * (dx2 / d);
                        (*pose->mtx)[1][3] = 0.0f;
                        c->timer++;
                    }
                } else {
                    c->vel.y += -0.5f;
                }
                break;
            case 40:
                if ((*pose->mtx)[1][3] <= 0.0f) {
                    (*pose->mtx)[1][3] = 0.0f;
                    c->vel.x = 0.0f;
                    c->vel.y = c->speed;
                    c->vel.z = 0.0f;
                } else {
                    c->vel.y += -0.5f;
                }
                break;
            case 45:
                c->timer--;
                if (c->timer <= 0) {
                    c->timer = 0;
                    c->vel.y = 0.0f;
                } else {
                    c->vel.y += -0.5f;
                }
                break;
            case 46:
                c->state = 47;
                c->timer = 0;
                c->vel.y = 6.0f;
                if ((*pose->mtx)[1][3] <= 0.0f) {
                    (*pose->mtx)[1][3] = 0.1f;
                }
            case 47:
                if ((*pose->mtx)[1][3] <= 0.0f) {
                    c->state = 40;
                    (*pose->mtx)[1][3] = 0.0f;
                    c->vel.x = 0.0f;
                    c->vel.y = c->speed;
                    c->vel.z = 0.0f;
                } else {
                    c->vel.y += -0.5f;
                }
                break;
            case 48:
                c->state = 49;
                c->timer = 0;
                c->vel.x = -4.0f;
                c->vel.y = 10.0f;
                if ((*pose->mtx)[1][3] <= 0.0f) {
                    (*pose->mtx)[1][3] = 0.1f;
                }
            case 49:
                if ((*pose->mtx)[1][3] <= 0.0f) {
                    c->state = 40;
                    (*pose->mtx)[1][3] = 0.0f;
                    c->vel.x = 0.0f;
                    c->vel.y = c->speed;
                    c->vel.z = 0.0f;
                } else {
                    c->vel.y += -0.5f;
                }
                break;
            case 50:
                c->state = 20;
                break;
            case 60:
                c->state = 61;
                break;
            case 61:
                if ((*pose->mtx)[1][3] <= 0.0f) {
                    if (i % 100 == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_MOVE1", 0x60, 0, &(Vec){(*pose->mtx)[0][3], (*pose->mtx)[1][3], (*pose->mtx)[2][3]});
                    }
                    (*pose->mtx)[1][3] = 0.0f;
                    c->vel.x = 5.0f;
                    c->vel.y = c->speed + irand(5);
                    c->vel.z = 0.0f;
                } else {
                    c->vel.y += -0.5f;
                }
                if ((*pose->mtx)[0][3] >= 250.0f) {
                    c->state = 99;
                }
                break;
            case 99:
                (*pose->mtx)[0][3] = 0.0f;
                (*pose->mtx)[1][3] = -1000.0f;
                (*pose->mtx)[2][3] = 0.0f;
                break;
        }
    }
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, chorogun_ext_disp, NULL);
}

/** Draw callback for the swarm: sets up the extdrv render mode, vertices, texture and TEV, then draws all poses. */
void chorogun_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

/** Script helper: args[0] receives how many swarm Fuzzies are flagged alive. */
USER_FUNC(chorogun_get_alive_num) {
    EvtWord* args = event->args;
    s32 i;
    s32 n = 0;

    for (i = 0; i < extGetPoseNum(); i++) {
        if (chorogun[i].flags & CHOROGUN_FLAG_ALIVE) {
            n++;
        }
    }
    evtSetValue(event, args[0], n);
    return 2;
}

/** Script helper: stores the unit id in args[0] as the target the swarm dives at. */
USER_FUNC(chorogun_set_target_id) {
    chorogun_targetId = evtGetValue(event, event->args[0]);
    return 2;
}

/**
 * Script helper: on the first call flags every swarm Fuzzy alive and starts its entrance (state 1);
 * blocks until all of them have reached state 21.
 */
USER_FUNC(chorogun_appear) {
    s32 i;
    s32 n;
    Chorogun* c;

    if (isFirstCall) {
        for (i = 0; i < extGetPoseNum(); i++) {
            c = &chorogun[i];
            c->flags |= CHOROGUN_FLAG_ALIVE;
            c->state = 1;
        }
    }
    n = 0;
    for (i = 0; i < extGetPoseNum(); i++) {
        if (chorogun[i].state == 21) {
            n++;
        }
    }
    if (n == extGetPoseNum()) {
        return 2;
    }
    return 0;
}

/**
 * Script helper: sends every living swarm Fuzzy toward the target (state 30) and blocks
 * until all have arrived (state 40).
 */
USER_FUNC(chorogun_approach_target) {
    s32 i;

    if (isFirstCall) {
        for (i = 0; i < extGetPoseNum(); i++) {
            if (chorogun[i].flags & CHOROGUN_FLAG_ALIVE) {
                chorogun[i].state = 30;
            }
        }
    }
    for (i = 0; i < extGetPoseNum(); i++) {
        if ((chorogun[i].flags & CHOROGUN_FLAG_ALIVE) && chorogun[i].state != 40) {
            return 0;
        }
    }
    return 2;
}

/**
 * Script helper: args[0] is the flight time in frames.
 * Picks a random living Fuzzy, launches it at the target unit on a ballistic arc with that frame timer, plays the attack sound and writes the Fuzzy's index to args[1].
 */
USER_FUNC(chorogun_dive_target) {
    ExtdrvPose* pose;
    s32 frames;
    s32 n;
    s32 sel;
    EvtWord* args;
    Chorogun* c;
    s32 list[1000];
    s32 i;
    f32 t;
    f32* row;
    f32 x, y, z;
    f32 dx, dy, dz;
    ExtdrvPose* p;
    BattleUnit* unit;

    args = event->args;
    n = 0;
    frames = evtGetValue(event, args[0]);
    t = frames;
    pose = extGetPosePtr();
    for (i = 0; i < extGetPoseNum(); i++) {
        if (chorogun[i & 0xFFFFFFFFFFFFFFFFu].flags & CHOROGUN_FLAG_ALIVE) {
            list[n++] = i;
        }
    }
    sel = list[irand(n)];
    c = &chorogun[sel];
    c->state = 45;
    c->timer = frames;
    BtlUnit_GetPos(BattleGetUnitPtr(_battleWorkPointer, chorogun_targetId), &x, &y, &z);
    p = &pose[sel];
    row = (*p->mtx)[2];
    dx = x - (*p->mtx)[0][3];
    dy = y - row[3];
    dz = z - row[3];
    c->vel.x = dx / t;
    c->vel.y = (2.0f * dy - -0.5f * t * t) / (2.0f * t);
    c->vel.z = dz / t;
    evtSetValue(event, args[1], sel);
    unit = chorogun_get_ptr();
    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_CHORO_ATTACK1", 0x80, 0, &(Vec){(*p->mtx)[0][3], (*p->mtx)[1][3], (*p->mtx)[2][3]});
    return 2;
}

/** Script helper: args[0] is a swarm Fuzzy index; sets it to state 46, bouncing off the target after landing. */
USER_FUNC(chorogun_bound_target) {
    s32 id = evtGetValue(event, event->args[0]);

    chorogun[id].state = 46;
    return 2;
}

/** Script helper: args[0] is a swarm Fuzzy index; sets it to state 48, knocked back by a counter. */
USER_FUNC(chorogun_counterd) {
    s32 id = evtGetValue(event, event->args[0]);

    chorogun[id].state = 48;
    return 2;
}

/** Script helper: on the first call sets every living swarm Fuzzy to state 50, the after-attack regroup. */
USER_FUNC(chorogun_after_attack) {
    s32 i;

    if (isFirstCall) {
        for (i = 0; i < extGetPoseNum(); i++) {
            if (chorogun[i].flags & CHOROGUN_FLAG_ALIVE) {
                chorogun[i].state = 50;
            }
        }
    }
    return 2;
}

/**
 * Script helper: selects up to 50 living Fuzzies using distance comparisons and makes them run away:
 * they stop counting as alive and go to state 60. The first 50 living Fuzzies encountered are candidates regardless of distance; later ones must be within 50 units.
 */
USER_FUNC(chorogun_escape) {
    f32 dist[1000];
    s32 idx[1000];
    BattleUnit* unit = chorogun_get_ptr();
    s32 i, j, m;
    s32 n = 0;
    Chorogun* c;
    ExtdrvPose* pose = extGetPosePtr();
    f32 dx, dz, d, tf;
    s32 ti;

    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        if (chorogun[i].flags & CHOROGUN_FLAG_ALIVE) {
            dx = unit->homePosition.x - (*pose->mtx)[0][3];
            dz = unit->homePosition.z - (*pose->mtx)[2][3];
            d = dx * dx + dz * dz;
            if (n >= 50) {
                if (d <= 2500.0f) {
                    dist[n] = d;
                    idx[n] = i;
                    n++;
                }
            } else {
                dist[n] = d;
                idx[n] = i;
                n++;
            }
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (j = 1; j < n; j++) {
            if (dist[i] > dist[j]) {
                tf = dist[i];
                ti = idx[i];
                dist[i] = dist[j];
                idx[i] = idx[j];
                dist[j] = tf;
                idx[j] = ti;
            }
        }
    }
    m = n >= 50 ? 50 : n;
    for (i = 0; i < m; i++) {
        c = &chorogun[idx[i]];
        c->flags &= ~CHOROGUN_FLAG_ALIVE;
        c->state = 60;
    }
    return 2;
}

/** Script helper: makes every living swarm Fuzzy run away (not alive, state 60). */
USER_FUNC(chorogun_escape_all) {
    s32 i;
    Chorogun* c;

    for (i = 0; i < extGetPoseNum(); i++) {
        c = &chorogun[i];
        if (c->flags & CHOROGUN_FLAG_ALIVE) {
            c->flags &= ~CHOROGUN_FLAG_ALIVE;
            c->state = 60;
        }
    }
    return 2;
}
