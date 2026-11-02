// Battle unit Dull Bones (`honenoko`): tst.rel's copy of rels/gon/src/unit/unit_honenoko.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "party/party_motion.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_sub.h"
#include "battle/battle_damage.h"

// Unit work slots of the Dull Bones.
enum {
    UNIT_WORK_SPIN_DONE = 0,  // set to 1 when the thrown bone lands; stops its spin loop
    UNIT_WORK_REWARD_GIVEN = 1, // 1 once the coins, experience and star points were awarded
    UNIT_WORK_REBIRTH_TURNS = 2, // turns left until the collapsed Dull Bones rebuilds itself (0: standing)
};

static USER_FUNC(honenoko_copy_status);

BattleUnitKind unit_honenoko;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[4];
static BattleWeapon weapon_honenoko_attack;
static BattleUnitSetup entry_honenoko;
static PoseTableEntry pose_table[19];
static BattleDataEntry data_table[4];
static EvtWord init_event[31];
static EvtWord damage_event[10];
static EvtWord dead_event[190];
static EvtWord fake_dead_event[165];
static EvtWord attack_event[185];
static EvtWord normal_attack_event[428];
static EvtWord call_friend_event[311];
static EvtWord wait_event[6];

/** Unit definition of the Dull Bones: 1 HP, level 9, 4 parts, with its sounds, init script and data table. */
BattleUnitKind unit_honenoko = {
    17,
    "btl_un_honenoko",
    1,
    0,
    1,
    1,
    9,
    0,
    0,
    70,
    1,
    70,
    9999,
    36,
    40,
    -4,
    40,
    0.0f,
    20.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    18.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    18.0f,
    0.0f,
    0.0f,
    9.0f,
    24.0f,
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    36.0f,
    40.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_HONENOKO_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    0,
    &regist,
    4,
    parts,
    init_event,
    data_table,
};

/** Defence of the Dull Bones: 1 against every kind of attack. */
static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

/** Damage-type modifiers of the Dull Bones: none, no special weakness or immunity. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of the Dull Bones (100% is the normal chance): immune to Sleep and Poison; resists
 * Stop 60%, Dizzy 50%, Confusion 50%, Freeze 90%, Tiny 90%, Attack Down 90%, Defense Down 90%, Allergic 90%,
 * Fright 10%, Gale Force 90%, Slow 90% and instant KO 50%.
 */
static StatusVulnerability regist = {0, 60, 50, 0, 50, 100, 100, 90, 100, 90, 100, 90, 100, 90, 90, 10, 90, 100, 90, 100, 100, 50};
#else
static StatusVulnerability regist = {0, 95, 50, 0, 50, 100, 100, 90, 100, 100, 100, 100, 100, 95, 90, 10, 90, 100, 90, 100, 100, 50};
#endif

/**
 * Parts of the Dull Bones: 4 parts (`c_karon_g_h`, `c_karon_g_h`, `c_nagehone_g`, `c_karon_g_h`), each with
 * its hit box, defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_honenoko", "c_karon_g_h", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
    {2, "btl_un_honenoko", "c_karon_g_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW | PART_ATTR_WORLD_POSITION, 0, pose_table},
    {3, "btl_un_honenoko", "c_nagehone_g", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW | PART_ATTR_WORLD_POSITION, 0, pose_table},
    {4, "btl_un_honenoko", "c_karon_g_h", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW | PART_ATTR_WORLD_POSITION, 0, pose_table},
};

/** Attack data of the Dull Bones's bone throw: hits one opposing unit for 2 damage. */
static BattleWeapon weapon_honenoko_attack = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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
 * Spawn record for a new Dull Bones, created by the unit's own script when it calls or splits off another one
 * (unit_honenoko, enemy side, parked off-screen until the script places it).
 */
static BattleUnitSetup entry_honenoko = {
    &unit_honenoko, // unitKindParams
    1, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Pose table of the Dull Bones: its 19 battle poses (status, hurt and action) and the animation each one
 * plays; the idle animation is `KRN_S_1`.
 */
static PoseTableEntry pose_table[] = {
    {1, "KRN_N_1"},
    {2, "KRN_Y_1"},
    {9, "KRN_Y_1"},
    {5, "KRN_K_1"},
    {4, "KRN_X_1"},
    {3, "KRN_X_1"},
    {27, "KRN_Z_2"},
    {BTL_POSE_NORMAL, "KRN_S_1"},
    {29, "KRN_Q_1"},
    {30, "KRN_Q_1"},
    {31, "KRN_S_1"},
    {BTL_POSE_DAMAGE, "KRN_D_1"},
    {50, "KRN_A_1"},
    {POSE_TYPE_RUN, "KRN_R_1"},
    {POSE_TYPE_WALK, "KRN_W_1"},
    {POSE_TYPE_DANGER, "KRN_X_1"},
    {POSE_TYPE_PERIL, "KRN_X_1"},
    {POSE_TYPE_TALK, "KRN_T_1"},
    {POSE_TYPE_ACTION, "KRN_S_1"},
};

/**
 * Data table of the Dull Bones: event 49 runs dead_event; event 15 runs fake_dead_event; event 48 does
 * nothing (btldefaultevt_Dummy).
 */
static BattleDataEntry data_table[] = {
    {49, dead_event},
    {15, fake_dead_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Init script of Dull Bones: installs the wait, attack,
 * damage and confusion handlers (confusion reuses the attack),
 * clears unit work 1 (reward paid flag) and sets its landing sound, and starts the wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, 0)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("0"), STRING("SFX_ENM_HONENOKO_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage handler: runs the shared default damage script (btldefaultevt_Damage) with its default parameters
 * (LW(10) = -2, LW(11) = 1).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Real death script of Dull Bones: spins it for 48 frames with the defeat sound, shows a smoke puff,
 * and the first time pays the coins, experience and star points (unit work 1 marks them as paid).
 * It then tilts it for 30 frames, collects any item it held,
 * and if the killing hit was flagged 0x400 removes the unit for good.
 */
static EVT_BEGIN(dead_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
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
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, 1)
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
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()

/**
 * Fake death script of Dull Bones: it moves to the spot given in LW(2) to LW(4),
 * and the first time pays out the coins, experience and star points (unit work 1 = 1) with a smoke puff.
 * It then plays the crumbled pose KRN_D_2 with the crumble sound, waits a second,
 * makes the unit unavailable (KillUnit mode 2),
 * and picks a random 0 or 1 in unit work 2 (the time it stays collapsed).
 */
static EVT_BEGIN(fake_dead_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(2), LW(3), LW(4))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(2), LW(3), LW(4), 0, -1, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REWARD_GIVEN, 1)
        SET_VALUE(LW(10), UNIT_ID_SELF)
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
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_D_2"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_CRUMBLE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(1000)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 2)
    CALL(evt_sub_random, 2, LW(0))
    ADD(LW(0), 2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REBIRTH_TURNS, LW(0))
    CALL(btlevtcmd_SetHitOffset, LW(10), LW(11), 0, -20, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Reassembly script of Dull Bones, run from its attack turn while collapsed:
 * counts down the collapse timer in unit work 2 and stays down while it is still 1 or more;
 * at 0 it plays KRN_D_3 and KRN_D_4 with the unite sounds, restores full HP,
 * clears the collapsed attribute (0x20000) and resets its hit offset.
 */
static EVT_BEGIN(rebirth_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_REBIRTH_TURNS, LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_REBIRTH_TURNS, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_UNITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_D_4"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_UNITE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kInactive)
    CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetHp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 0, 0, 0)
    RETURN
EVT_END()

/**
 * Attack script of Dull Bones: when battle flag 2 is set it uses the normal attack;
 * if it is still collapsed (unit work 2 is 1 or more) it first rebuilds itself with rebirth_event.
 * After the item-use check it picks the normal attack 85 percent of the time;
 * otherwise, if there is free space on the stage, it calls a friend (call_friend_event) from either side.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(normal_attack_event)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_REBIRTH_TURNS, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(rebirth_event)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    LABEL(0)
    SET_VALUE(LW(0), 85)
    ADD(LW(0), 0)
    ADD(LW(0), 15)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    SET_VALUE(LW(0), 85)
    IF_INT_LESS(LW(1), LW(0))
        CALL_CHILD(normal_attack_event)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(1)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
        SET_VALUE(LW(4), LW(0))
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
        MULTIPLY(LW(4), LW(5))
        IF_INT_LESS(LW(4), 0)
            SUBTRACT(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(call_friend_event)
                WAIT_MS(500)
                GOTO(99)
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(1)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
        IF_INT_LESS_OR_EQUAL(LW(0), 170)
            ADD(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 180, 0)
                CALL_CHILD(call_friend_event)
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
                WAIT_MS(500)
                GOTO(99)
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    GOTO(0)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Starts the target's dodge animation for the thrown bone depending on the avoid result in LW(5):
 * 3, 6 and 2 map to avoid animations 38, 39 and 40 for the unit in LW(3).
 */
static EVT_BEGIN(start_avoid)
    IF_INT_EQUAL(LW(5), 3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
    END_IF
    IF_INT_EQUAL(LW(5), 6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
    END_IF
    IF_INT_EQUAL(LW(5), 2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
    END_IF
    RETURN
EVT_END()

/**
 * Dull Bones' normal attack (weapon_honenoko_attack):
 * it picks a target, plays KRN_A_1 and KRN_A_2 and throws its bone (part 2, KRN_H_1) spinning across the stage
 * with the bone-throw sound; the target dodges as the bone passes it,
 * and a hit applies the damage with the guard-timing check. The bone bounces and returns to the unit.
 */
static EVT_BEGIN(normal_attack_event)
    // Picks the target: the preset first-attack target (after a short pause) when battle flag 2 is set, otherwise a
    // random opposing unit; with none it does nothing (or is flustered when confused).
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_honenoko_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_honenoko_attack), LW(3), LW(4))
    END_IF
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // Declares the attack, pays its cost and turns to face the target.
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_honenoko_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_honenoko_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // Wind-up: it pauses, raises its arm (KRN_A_1) and swings it back (KRN_A_2).
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_A_1"))
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_A_2"))
    WAIT_MS(250)

    // It hurls a bone (part 2) that spins end over end as it flies; the spin runs until the throw ends.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_SEND_GROUP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_BONE_THROW1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KRN_H_1"))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_HIDDEN)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 0)
        LOOP(0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, LW(14))
            IF_INT_EQUAL(LW(14), 1)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 2, 0, 0, 25)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT

    // Checks the outcome of the hit: a miss, dodge or guard goes to the no-damage branch; a connecting hit applies
    // the Action Command defence result and deals the damage.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_honenoko_attack), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // Miss branch: the bone flies past the target and lands at the far edge of the stage, and the target starts its
    // dodge when the bone gets level with it.
    LABEL(90)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_LESS_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            ELSE
                CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                IF_INT_GREATER_OR_EQUAL(LW(0), LW(6))
                    CALL_CHILD(start_avoid)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 14)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(9))
    MULTIPLY(LW(0), LW(9))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(6.0f), FLOAT(0.049804688f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 0, 0)
    CALL(evt_snd_sfxoff, LW(15))
    GOTO(98)

    // Hit branch: the bone arcs onto the target, the defence result is read and the damage is applied, then the bone
    // bounces off behind it.
    LABEL(91)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 14)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(6.0f), FLOAT(0.19921875f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 0, 0)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_honenoko_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_honenoko_attack), 256, LW(5))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), -50)
    CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 2, 0, FLOAT(2.0f), FLOAT(0.2998047f))
    CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 0, 0)
    GOTO(98)

    // Ends the bone's spin and hides the bone part again; the unit settles back into its wait event.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_HIDDEN)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, PART_ATTR_NO_SHADOW)
    WAIT_FRAMES(10)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Dull Bones calls a friend: it throws its bones one after another (part 3, KRN_H_n poses)
 * and collects them (part 4, KRN_C_n) over six rounds with the call sounds, reassembles with the unite sounds,
 * and spawns a new Dull Bones (entry_honenoko) at the free spot in LW(0) to LW(2),
 * copying its status to it (honenoko_copy_status).
 */
static EVT_BEGIN(call_friend_event)
    // The camera pulls back to show the whole party as the unit starts to call for backup.
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    SET_VALUE(LW(10), 0)

    // Six times it tosses a bone piece (part 3) in an arc onto a heap (part 4) that grows a little taller with each
    // piece.
    LOOP(6)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_A_3"))
        WAIT_FRAMES(5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_A_4"))
        WAIT_FRAMES(15)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_SEND_GROUP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SWITCH(LW(10))
            CASE_EQUAL(0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_4"))
            CASE_EQUAL(1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_5"))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_5"))
            CASE_EQUAL(3)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_3"))
            CASE_EQUAL(4)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_3"))
            CASE_DEFAULT
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRN_H_2"))
        END_SWITCH
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 1, LW(4), LW(5), LW(6))
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(4), 15)
        ADD(LW(5), 14)
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 3, LW(4), LW(5), LW(6))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, 0)
            SET_VALUE(LW(15), 0)
            LOOP(0)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_DONE, LW(14))
                IF_INT_EQUAL(LW(14), 1)
                    BREAK_LOOP
                END_IF
                CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 3, 0, 0, LW(15))
                ADD(LW(15), 25)
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(btlevtcmd_JumpPartsSetting, UNIT_ID_SELF, 3, 0, FLOAT(6.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 20, 0)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_SEND_GROUP2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        SWITCH(LW(10))
            CASE_EQUAL(0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_5"))
            CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW)
            CASE_EQUAL(1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_4"))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_3"))
            CASE_EQUAL(3)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_2"))
            CASE_EQUAL(4)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_C_1"))
            CASE_DEFAULT
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_Z_2"))
        END_SWITCH
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW)
        ADD(LW(10), 1)
    END_LOOP

    // The heap shakes and the bones pull together (reunite sounds).
    WAIT_FRAMES(15)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_D_3"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_UNITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("KRN_D_4"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_HONENOKO_UNITE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(30)

    // A new bony unit is spawned on the heap and given this unit's status effects.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRN_S_1"))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_honenoko), 0)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 4, PART_ATTR_HIDDEN | PART_ATTR_NO_SHADOW)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(honenoko_copy_status, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

/** Idle script: sets the standard waiting pose from the unit's pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Script helper: args[0] and args[1] are unit ids (BattleTransID).
 * Copies the size-change (Mini/Mega) turns and strength from the first unit to the second,
 * so a unit spawned or replaced mid-battle keeps the size it had. Called with the new unit (LW(3)).
 */
static USER_FUNC(honenoko_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = BattleTransID(event, evtGetValue(event, args[0]));
    s32 id2 = BattleTransID(event, evtGetValue(event, args[1]));
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    unit2->sizeChangeTurns = unit1->sizeChangeTurns;
    unit2->sizeChangeStrength = unit1->sizeChangeStrength;
    return EVT_RETURN_DONE;
}
