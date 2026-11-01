// Battle unit Green Fuzzy (`green_chorobon`): tst.rel's copy of rels/muj/src/unit/unit_green_chorobon.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_sub.h"

static USER_FUNC(chorobon_copy_status);

BattleUnitKind unit_green_chorobon;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_chorobon_drain_attack;
static BattleUnitSetup entry_chorobon;
static EvtWord friend_event[223];
static BattleWeapon weapon_chorobon_drain_attack_biribiri_counterd;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[14];
static PoseSoundTiming pose_sound_timing_table[4];
static EvtWord init_event[31];
static EvtWord damage_event[10];
static EvtWord counter_damage_event[41];
static EvtWord return_event[24];
static EvtWord drain_attack_event[887];
static EvtWord wait_event[6];

/**
 * Unit kind of Green Fuzzy: 5 max HP, level 19. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
BattleUnitKind unit_green_chorobon = {
    86,
    "btl_un_green_chorobon",
    5,
    0,
    1,
    1,
    19,
    0,
    3,
    70,
    0,
    60,
    9999,
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
    9.0f,
    20.0f,
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
    "SFX_ENM_CHORO2_DAMAGED1",
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
 * Defence of Green Fuzzy per element (normal, fire, ice, explosion, electric), subtracted from each hit:
 * none.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of Green Fuzzy: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of Green Fuzzy (the demos), as the percent chance a status takes hold:
 * always hit by Poison, Electric, Burn, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 90, Stop 90, Dizzy 80, Confuse 80, Freeze 65, Tiny 70, Attack Down 70, Defense Down 65,
 * Allergic 65, Fright 85, Gale Force 85, Slow 65 and instant KO 95.
 */
static StatusVulnerability regist = {90, 90, 80, 100, 80, 100, 100, 65, 100, 70, 100, 70, 100, 65, 65, 85, 85, 100, 65, 100, 100, 95};
#else
/**
 * Status vulnerability of Green Fuzzy (the final versions), as the percent chance a status takes hold:
 * always hit by Poison, Electric, Burn, Huge, Attack Up, Defense Up, Fast, Dodgy and Invisible;
 * otherwise Sleep 85, Stop 85, Dizzy 80, Confuse 85, Freeze 70, Tiny 90, Attack Down 90, Defense Down 95,
 * Allergic 60, Fright 85, Gale Force 80, Slow 80 and instant KO 95.
 */
static StatusVulnerability regist = {85, 85, 80, 100, 85, 100, 100, 70, 100, 90, 100, 90, 100, 95, 60, 85, 80, 100, 80, 100, 100, 95};
#endif

/**
 * Body parts of Green Fuzzy: a single part (model c_chorobon_g) holding its hit box.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_green_chorobon", "c_chorobon_g", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, 0, pose_table},
};

/** Attack data of Green Fuzzy's chorobon drain attack: base damage 3, 100% accuracy. */
static BattleWeapon weapon_chorobon_drain_attack = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_PIERCE_DEFENSE, // special
    0, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
 * Attack event of the Green Fuzzy: may use an item; otherwise half the time it uses its drain attack and
 * otherwise it splits (friend_event) into the first free 40-unit spot behind it or in front of it, rolling again
 * when there is none.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    LABEL(0)
    SET_VALUE(LW(0), 50)
    ADD(LW(0), 50)
    SUBTRACT(LW(0), 1)
    CALL(evt_sub_random, LW(0), LW(1))
    IF_INT_LESS(LW(1), 50)
        CALL_CHILD(drain_attack_event)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
        IF_INT_LESS_OR_EQUAL(LW(0), 170)
            ADD(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(friend_event)
                WAIT_MS(500)
                GOTO(99)
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
        SET_VALUE(LW(4), LW(0))
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
        MULTIPLY(LW(4), LW(5))
        IF_INT_LESS(LW(4), 0)
            SUBTRACT(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(friend_event)
                WAIT_MS(500)
                GOTO(99)
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Spawn setup for a unit that the Green Fuzzy script adds to the battle mid-fight:
 * unit kind unit_green_chorobon, parked far off screen until its script moves it.
 */
static BattleUnitSetup entry_chorobon = {
    &unit_green_chorobon, // unitKindParams
    1, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * The Green Fuzzy splits, one of its turn actions: it wobbles and spins up (CBN_K_1, SFX_ENM_CHORO2_SPLIT1),
 * hops with SFX_ENM_CHORO2_SPLIT2 and spawns another Green Fuzzy (entry_chorobon) at its position with its HP
 * and
 * size change (chorobon_copy_status), which jumps over to the free spot found by attack_event.
 */
static EVT_BEGIN(friend_event)
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_K_1"))
    SET_VALUE(LW(12), 120)
    SET_VALUE(LW(13), 45)
    SET_VALUE(LW(14), 10)
    SET_VALUE(LW(15), 10)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 10, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_SPLIT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(0)
        WAIT_FRAMES(1)
        SUBTRACT(LW(12), 1)
        IF_INT_LESS_OR_EQUAL(LW(12), 1)
            BREAK_LOOP
        END_IF
        SUBTRACT(LW(14), 1)
        IF_INT_LESS_OR_EQUAL(LW(14), 0)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(13))
            SUBTRACT(LW(15), 1)
            SET_VALUE(LW(14), LW(15))
            IF_INT_EQUAL(LW(15), 7)
                CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_SPLIT1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
            END_IF
        END_IF
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_S_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(7), LW(8), LW(9))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, 10, FLOAT(0.5f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(7), LW(8), LW(9), FLOAT(20.0f), -1)
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_SPLIT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_chorobon), 0)
    CALL(chorobon_copy_status, UNIT_ID_SELF, LW(3))
    CALL(btlevtcmd_SetPos, LW(3), LW(7), LW(8), LW(9))
    CALL(btlevtcmd_JumpSetting, LW(3), 0, 10, FLOAT(0.5f))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 10, 0)
        SET_VALUE(LW(12), 0)
        LOOP(0)
            WAIT_FRAMES(1)
            ADD(LW(12), 1)
            IF_INT_GREATER_OR_EQUAL(LW(12), 30)
                BREAK_LOOP
            END_IF
            CALL(btlevtcmd_AddRotate, LW(3), 0, 0, 20)
        END_LOOP
        CALL(btlevtcmd_SetRotate, LW(3), 0, 0, 0)
        CALL(btlevtcmd_SetRotateOffset, LW(3), 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, LW(3), LW(0), LW(1), LW(2), FLOAT(30.0f), -1)
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    WAIT_MS(1000)
    CALL(evt_btl_camera_set_mode, 0, 0)
    RETURN
EVT_END()

/** Attack data of Green Fuzzy's chorobon drain attack biribiri counterd attack: no damage, 100% accuracy. */
static BattleWeapon weapon_chorobon_drain_attack_biribiri_counterd = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
#if VERSION <= VERSION_DEMO_US
    0, 0, 0,
#else
    0, 0, 2, // baseFpCost..superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, 0, 0, // acHelpMsg..counterResistFlags
    TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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
 * Pose table of Green Fuzzy: maps its battle pose types (stay, walk, hurt, dead, talk and so on) to
 * animation names, 18 entries such as CBN_N_1, CBN_Y_1, CBN_K_1, CBN_X_1.
 */
static PoseTableEntry pose_table[] = {
    {1, "CBN_N_1"},
    {2, "CBN_Y_1"},
    {9, "CBN_Y_1"},
    {5, "CBN_K_1"},
    {4, "CBN_X_1"},
    {3, "CBN_X_1"},
    {BTL_POSE_NORMAL, "CBN_S_1"},
    {29, "CBN_Q_1"},
    {30, "CBN_Q_1"},
    {31, "CBN_S_1"},
    {BTL_POSE_DAMAGE, "CBN_D_1"},
    {50, "CBN_A_1"},
    {POSE_TYPE_RUN, "CBN_R_1"},
    {POSE_TYPE_WALK, "CBN_W_1"},
    {POSE_TYPE_DANGER, "CBN_X_1"},
    {POSE_TYPE_PERIL, "CBN_X_1"},
    {POSE_TYPE_TALK, "CBN_T_1"},
    {POSE_TYPE_ACTION, "CBN_S_1"},
};

/**
 * Event table of Green Fuzzy: maps event tags to the scripts that react to them (counter_damage_event);
 * tag 0 ends it.
 */
static BattleDataEntry data_table[] = {
    {26, counter_damage_event},
    {25, counter_damage_event},
    {27, counter_damage_event},
    {28, counter_damage_event},
    {29, counter_damage_event},
    {30, counter_damage_event},
    {31, counter_damage_event},
    {32, counter_damage_event},
    {33, counter_damage_event},
    {34, counter_damage_event},
    {35, counter_damage_event},
    {36, counter_damage_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Hops the Fuzzy along x toward LW(5) in hops of LW(8) units at height LW(6), squashing before each,
 * then makes a final jump onto (LW(5), LW(6), LW(7)) with SFX_ENM_CHORO2_LANDING1 and straightens up.
 */
static EVT_BEGIN(rabbit_move)
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
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(5), LW(6), LW(7), 0, -1)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
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
 * Sound cues of Green Fuzzy's animations: for each of its 4 animation names,
 * the sound effect that plays with it (SFX_ENM_CHORO2_MOVE1, SFX_ENM_CHORO2_WAIT2 and
 * SFX_ENM_CHORO2_WAIT1).
 */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"CBN_S_1", 0.5833333f, NULL, "SFX_ENM_CHORO2_MOVE1", 0, 1},
    {"CBN_S_1", 0.8333333f, NULL, "SFX_ENM_CHORO2_WAIT2", 0, 1},
    {"CBN_S_1", 1.5833334f, NULL, "SFX_ENM_CHORO2_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/**
 * Init event of the Green Fuzzy: registers its wait, attack, damage and confusion scripts (the attack script
 * doubles as the confusion one), its pose sound table and its jump sound (SFX_ENM_CHORO2_MOVE1), then starts the
 * wait event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_MOVE1"), 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage event of the Green Fuzzy: runs btldefaultevt_Damage on itself (LW(10) = -2, LW(11) = 1). */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Data-table events 25 to 36, run when the Fuzzy is hurt by a target's counter (damage codes 0x2C to 0x37):
 * resets its scale, plays subsetevt_counter_damage and, unless the counter KO'd it without a Life Shroom,
 * hops home (return_event).
 */
static EVT_BEGIN(counter_damage_event)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamageCode, LW(10), LW(0))
    CALL_CHILD(subsetevt_counter_damage)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL_CHILD(return_event)
    RETURN
EVT_END()

/** Hops the Green Fuzzy back to its home position in 40-unit hops (rabbit_move). */
static EVT_BEGIN(return_event)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
    SET_VALUE(LW(8), 40)
    CALL_CHILD(rabbit_move)
    RETURN
EVT_END()

/**
 * Drain attack of the Green Fuzzy (weapon_chorobon_drain_attack): it hops up to one target and jumps onto it.
 * When the target would counter (Electrified or Hold Fast) it takes
 * weapon_chorobon_drain_attack_biribiri_counterd
 * instead; otherwise the Fuzzy latches on, shaking the target, deals the damage, swells, and heals itself by the
 * HP damage dealt (with the recovery effect), then hops off and home.
 */
static EVT_BEGIN(drain_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // The Green Fuzzy picks a target; with none, it ends the turn (or flusters when confused).
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_chorobon_drain_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_chorobon_drain_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_chorobon_drain_attack))

    // The attack is declared and paid for, and the camera follows the Fuzzy.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_chorobon_drain_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)

    // It hops toward the target and jumps up onto it.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 0, FLOAT(4.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(5), LW(6), LW(7))
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(5), 50)
    SET_VALUE(LW(8), 50)
    CALL_CHILD(rabbit_move)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 60, 0, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 5)
    SUBTRACT(LW(1), 10)
    SUBTRACT(LW(2), 2)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)

    // The damage check decides between a miss, a counter and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_chorobon_drain_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // On a miss it bounces off the target and lands in front of it.
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
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_N_1"))
    WAIT_MS(1000)
    GOTO(98)

    // On a hit it first checks whether the target would counter with a shock.
    LABEL(91)
    SET_VALUE(LF(0), 0)
    CALL(btlevtcmd_PreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_chorobon_drain_attack), 256, LW(5))
    IF_INT_EQUAL(LW(5), 17)
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LW(5), 14)
        SET_VALUE(LF(0), 1)
    END_IF

    // If the target counters, the Green Fuzzy takes the counter damage instead and the attack ends.
    IF_INT_EQUAL(LF(0), 1)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_chorobon_drain_attack))
        SET_VALUE(LW(9), PTR(&weapon_chorobon_drain_attack_biribiri_counterd))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_ATTACK1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)

    // Otherwise it clings on: the target shakes while the Green Fuzzy spins on its back.
    CALL(btlevtcmd_GetBodyId, LW(3), LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(0), 37)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), 89)
        SET_VALUE(LW(1), 1)
        SET_VALUE(LW(2), 2)
        SET_VALUE(LW(5), LW(2))
        LABEL(20)
        SUBTRACT(LW(5), 1)
        IF_INT_LESS_OR_EQUAL(LW(5), 0)
            SET_VALUE(LW(5), LW(2))
            IF_INT_EQUAL(LW(1), 0)
                SET_VALUE(LW(1), 1)
            ELSE
                SET_VALUE(LW(1), 0)
            END_IF
        END_IF
        CALL(btlevtcmd_SetDispOffset, LW(3), LW(1), 0, 0)
        WAIT_FRAMES(1)
        SUBTRACT(LW(0), 1)
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            GOTO(20)
        END_IF
        CALL(btlevtcmd_SetDispOffset, LW(3), 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_ATTACK1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(14), 20)
    IF_INT_EQUAL(LW(15), -1)
        SET_VALUE(LW(14), -20)
    END_IF
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(14))
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(1.5f))

    // It holds on for a moment in its attack pose, draining.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_A_2"))
    WAIT_FRAMES(90)
    CALL(btlevtcmd_AnimeSetMotionSpeed, UNIT_ID_SELF, 1, FLOAT(1.0f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_R_2"))
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    LOOP(4)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, 0, FLOAT(-0.19921875f), 0)
    END_LOOP
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(0.19921875f), FLOAT(1.0f))
    LOOP(20)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.009765625f), FLOAT(0.049804688f), 0)
    END_LOOP
    WAIT_FRAMES(1)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))

    // The drain lands: the camera shakes and the damage is dealt.
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_chorobon_drain_attack))
    SET_VALUE(LW(9), PTR(&weapon_chorobon_drain_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetHpDamage, LW(3), LW(10))

    // The Green Fuzzy lets go and hops off.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.5f), FLOAT(0.5f), 0)
    END_IF
    LOOP(4)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.07910156f), FLOAT(0.119140625f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.31933594f), FLOAT(-0.4794922f), 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 20, -1)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.31933594f), FLOAT(0.4794922f), 0)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 10, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("CBN_W_1"))
    IF_INT_GREATER_OR_EQUAL(LW(10), 1)
        LOOP(20)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.024414062f), FLOAT(-0.024414062f), 0)

            // It heals itself by the amount drained (HP, or FP for the FP-draining weapon), with a recovery effect.
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(1))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        ADD(LW(6), 45)
        ADD(LW(7), 10)
        IF_INT_EQUAL(LW(9), PTR(&weapon_chorobon_drain_attack))
            CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, LW(1), LW(10))
            CALL(evt_eff, STRING(""), STRING("recovery"), 7, LW(5), LW(6), LW(7), LW(10), 0, 0, 0, 0, 0, 0, 0)
        ELSE
            CALL(btlevtcmd_RecoverFp, UNIT_ID_SELF, LW(1), LW(10))
            CALL(evt_eff, STRING(""), STRING("recovery"), 8, LW(5), LW(6), LW(7), LW(10), 0, 0, 0, 0, 0, 0, 0)
        END_IF
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    END_IF
    LOOP(2)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 30)
        CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_CHORO2_ATTACK3"), 0, 0, -1, -1, 0, 0)
        LOOP(2)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, -2)
        WAIT_FRAMES(16)
        LOOP(2)
            CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
        CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, 0, 0, 0, -1, -1, 0, 0)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 6, -2)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.15917969f), FLOAT(-0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    LOOP(2)
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(-0.15917969f), FLOAT(0.23925781f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    GOTO(98)

    // It hops back home and the camera resets.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 0)
    CALL_CHILD(return_event)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Script helper: args[0] and args[1] are unit ids (BattleTransID).
 * Copies the first unit's HP (capped at the second one's maximum) and its size-change (Mini/Mega) turns
 * and strength to the second, then resets the second one's HP gauge to match.
 * Called with the newly spawned unit (LW(3)) from the unit's own scripts.
 */
static USER_FUNC(chorobon_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = BattleTransID(event, evtGetValue(event, args[0]));
    s32 id2 = BattleTransID(event, evtGetValue(event, args[1]));
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    unit2->currentHp = unit1->currentHp;
    if (unit2->currentHp > unit2->maxHp) {
        unit2->currentHp = unit2->maxHp;
    }
    unit2->sizeChangeTurns = unit1->sizeChangeTurns;
    unit2->sizeChangeStrength = unit1->sizeChangeStrength;
    unit2->hpGauge.prevHp = unit2->currentHp;
    unit2->hpGauge.targetHp = unit2->currentHp;
    unit2->hpGauge.barRatio = unit2->hpGauge.trailRatio = (f32)unit2->currentHp / unit2->maxHp;
    return EVT_RETURN_DONE;
}

/** Wait event of the Green Fuzzy: shows part 1's pose for its current state from its pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
