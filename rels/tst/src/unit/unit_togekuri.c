// Battle unit Spiky Goomba (`togekuri`): tst.rel's copy of rels/gon/src/unit/unit_togekuri.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "system.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "evt/evt_eff.h"
#include "evt/evt_msg.h"
#include "libc_decls.h"
#include "battle/battle_damage.h"

static USER_FUNC(get_attacker);
static USER_FUNC(krb_get_dir);

BattleUnitKind unit_togekuri;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_togekuri_attack;
static PoseTableEntry pose_table[19];
static BattleDataEntry data_table[3];
static EvtWord init_event[42];
static EvtWord dmg_avoid_counter_thorn_head_event[127];
static EvtWord damage_event[10];
static f32 SinTable[91];
static EvtWord attack_event[597];
static EvtWord wait_event[6];

/**
 * Unit definition of the Spiky Goomba: 2 HP and level 8, with its sizes, damage sounds, status vulnerability,
 * parts and init script.
 */
BattleUnitKind unit_togekuri = {
    3,
    "btl_un_togekuri",
    2,
    0,
    1,
    1,
    8,
    0,
    1,
    60,
    0,
    70,
    9999,
    23,
    30,
    5,
    30,
    0.0f,
    15.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    12.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    11.5f,
    0.0f,
    0.0f,
    11.5f,
    18.0f,
    0.0f,
    0.0f,
    15.0f,
    0.0f,
    23.0f,
    30.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_TOGEKURI_DAMAGED1",
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

/** Defence of the Spiky Goomba: none, every hit does its full damage. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element modifiers of the Spiky Goomba: none, no element is special to it. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of the Spiky Goomba in the demo builds:
 * extra prone to stop, fright, Gale Force and instant KO.
 */
static StatusVulnerability regist = {100, 120, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 100, 100, 100, 100, 150};
#else
/**
 * Status vulnerability of the Spiky Goomba in the retail builds:
 * extra prone to stop, fright, Gale Force and instant KO.
 */
static StatusVulnerability regist = {100, 110, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 110, 110, 100, 100, 100, 100, 150};
#endif

/** Body part of the Spiky Goomba: a single part using the c_kuribo model. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_togekuri", "c_kuribo", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER, kCounterTopSpiky, pose_table},
};

/** Attack data of the Spiky Goomba's headbonk: 2 damage. */
static BattleWeapon weapon_togekuri_attack = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
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
 * Pose table of the Spiky Goomba: maps each battle pose (waiting, hurt, running...) to one of its TGK_*
 * animations.
 */
static PoseTableEntry pose_table[] = {
    {1, "TGK_N_1"},
    {2, "TGK_Y_1"},
    {9, "TGK_Y_1"},
    {5, "TGK_K_1"},
    {4, "TGK_I_1"},
    {3, "TGK_I_1"},
    {BTL_POSE_NORMAL, "TGK_S_1"},
    {29, "TGK_Q_1"},
    {30, "TGK_Q_1"},
    {31, "TGK_S_1"},
    {BTL_POSE_DAMAGE, "TGK_D_1"},
    {50, "TGK_A_1"},
    {41, "TGK_R_2"},
    {POSE_TYPE_RUN, "TGK_R_1"},
    {POSE_TYPE_WALK, "TGK_W_1"},
    {POSE_TYPE_DANGER, "TGK_I_1"},
    {POSE_TYPE_PERIL, "TGK_I_1"},
    {POSE_TYPE_TALK, "TGK_T_1"},
    {POSE_TYPE_ACTION, "TGK_S_1"},
};

/**
 * Data table of the Spiky Goomba: reaction scripts for an attacker hurt by its spikes;
 * a First Strike on it triggers nothing special.
 */
static BattleDataEntry data_table[] = {
    {37, dmg_avoid_counter_thorn_head_event},
    {48, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Init script of the Spiky Goomba: registers its wait,
 * attack and damage scripts (confused, it runs the normal attack script), sets its run,
 * walk and jump sounds and starts the wait script.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_TOGEKURI_MOVE1"), STRING("SFX_ENM_TOGEKURI_MOVE2"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_TOGEKURI_MOVE1"), STRING("SFX_ENM_TOGEKURI_MOVE2"), 0, 6, 6)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_JUMP1"), STRING("SFX_ENM_KURI_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Run when a stomp is stopped by the Spiky Goomba's spike (data event 37): flashes toge_flush on the spike.
 * If the lecture unit is in the battle and has not given its tip yet (work 0 is 0) and Mario
 * or Goombella was hurt, it turns round after 2.5 s,
 * says mac_0_092_06 (Mario) or mac_0_092_07 (Goombella) and marks the tip as given.
 */
static EVT_BEGIN(dmg_avoid_counter_thorn_head_event)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_HUGE, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 50)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(3))
    IF_INT_NOT_EQUAL(LW(3), 0)
        ADD(LW(1), 30)
        GOTO(90)
    END_IF
    ADD(LW(1), 40)
    GOTO(90)
    LABEL(90)
    CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_LECTURE, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, 0, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(get_attacker, LW(0))
            IF_INT_EQUAL(LW(0), 2)
                RETURN
            END_IF
            WAIT_MS(2500)
            CALL(btlevtcmd_StatusWindowOnOff, 0)
            CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_LECTURE, -1)
            CALL(get_attacker, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print, 2, STRING("mac_0_092_06"), 0, UNIT_ID_LECTURE)
            ELSE
                CALL(evt_msg_print, 2, STRING("mac_0_092_07"), 0, UNIT_ID_LECTURE)
            END_IF
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, 0, 1)
            CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_LECTURE, 1)
            CALL(btlevtcmd_StatusWindowOnOff, 1)
        END_IF
    END_IF
    RETURN
EVT_END()

/** Damage reaction of the Spiky Goomba: the default damage script. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Sine table for 0 to 90 degrees, one entry per degree;
 * krb_get_dir looks angles up in it to tilt the Spiky Goomba along its jump.
 */
static f32 SinTable[] = {
    0.0f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 0.087156f, 0.104528f, 0.121869f,
    0.139173f, 0.156434f, 0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 0.258819f,
    0.275637f, 0.292372f, 0.309017f, 0.325568f, 0.34202f, 0.358368f, 0.374607f, 0.390731f,
    0.406737f, 0.422618f, 0.438371f, 0.45399f, 0.469472f, 0.48481f, 0.5f, 0.515038f,
    0.529919f, 0.544639f, 0.559193f, 0.573576f, 0.587785f, 0.601815f, 0.615661f, 0.62932f,
    0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 0.707107f, 0.71934f, 0.731354f,
    0.743145f, 0.75471f, 0.766044f, 0.777146f, 0.788011f, 0.798636f, 0.809017f, 0.819152f,
    0.829038f, 0.838671f, 0.848048f, 0.857167f, 0.866025f, 0.87462f, 0.882948f, 0.891007f,
    0.898794f, 0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.93358f, 0.939693f, 0.945519f,
    0.951057f, 0.956305f, 0.961262f, 0.965926f, 0.970296f, 0.97437f, 0.978148f, 0.981627f,
    0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.99863f,
    0.999391f, 0.999848f, 1.0f,
};

/**
 * The Spiky Goomba's headbonk (weapon_togekuri_attack): may use an item first;
 * otherwise it walks up beside the target and leaps onto it head first, tilting along its arc.
 * A hit squashes it, applies the guard result and damage and bounces it back three times;
 * a miss lands it on the floor (TGK_D_1) before it hops back. Then it walks home.
 */
static EVT_BEGIN(attack_event)
    // If the Spiky Goomba holds an item, it uses that instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // A target is picked from the opposing party.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_togekuri_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_togekuri_attack), LW(3), LW(4))

    // No target to attack: a confused Spiky Goomba just flusters about, otherwise it skips its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // The camera frames the Spiky Goomba and its target.
    CALL(evt_btl_camera_set_mode, 0, 8)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_togekuri_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_togekuri_attack))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 80)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))

    // The Spiky Goomba runs up beside the target.
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, -1, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, BTL_POSE_STAY)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(170)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 50)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)

    // It leaps, its body tilting along the arc of the jump.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        SET_VALUE(LW(8), 0)
        LOOP(28)
            CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(9), LW(10), LW(11))
            CALL(krb_get_dir, LW(5), LW(6), LW(9), LW(10), LW(8))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(8))
            SET_FLOAT_VALUE(LW(5), LW(9))
            SET_FLOAT_VALUE(LW(6), LW(10))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(12)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 51)
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 2)
    SUBTRACT(LW(1), 4)

    // It comes down on the target head first.
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 28, -1)

    // Check whether the attack will connect; a miss sends the target into its dodge.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togekuri_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // A miss: it lands head down on the floor, rights itself and hops back.
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KURI_LANDING_MISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, 0, FLOAT(-12.0f), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGK_D_1"))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGK_A_2"))
    CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 12, 0)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(6))
        DIVIDE(LW(6), 10)
        MULTIPLY(LW(6), -1)
        SET_VALUE(LW(7), 10)
        LABEL(10)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(6))
        SUBTRACT(LW(7), 1)
        WAIT_FRAMES(1)
        IF_INT_GREATER_OR_EQUAL(LW(7), 1)
            GOTO(10)
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGK_K_1"))
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), -30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(40)
    GOTO(95)

    // A hit: the target takes the damage and the Spiky Goomba bounces off it three times.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    LOOP(4)
        // It squashes against the target for a moment.
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.07910156f), FLOAT(-0.119140625f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_togekuri_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togekuri_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, BTL_POSE_STAY)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, EVT_NULLPTR, LW(6))
        DIVIDE(LW(6), 10)
        MULTIPLY(LW(6), -1)
        SET_VALUE(LW(7), 10)
        LABEL(20)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(6))
        SUBTRACT(LW(7), 1)
        WAIT_FRAMES(1)
        IF_INT_GREATER_OR_EQUAL(LW(7), 1)
            GOTO(20)
        END_IF
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 28, -1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, -1)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, -1)
    GOTO(95)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 3, 3, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, 41)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -2, 0)

    // Turn finished: the Spiky Goomba goes back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Script helper for the Goomba family: args[0], args[1] are the x and z of one point and args[2],
 * args[3] those of another; args[4] receives the angle between them (angleABf of the x/z difference, degrees).
 * When the two points coincide, args[4] keeps the value it already held.
 */
static USER_FUNC(krb_get_dir) {
    EvtWord* args = event->args;
    f32 x1 = evtGetFloat(event, args[0]);
    f32 z1 = evtGetFloat(event, args[1]);
    f32 x2 = evtGetFloat(event, args[2]);
    f32 z2 = evtGetFloat(event, args[3]);
    s32 dir = evtGetValue(event, args[4]);
    f32 dx = x2 - x1;
    f32 dz = z2 - z1;

    if (dx == 0.0f && dz == 0.0f) {
        evtSetValue(event, args[4], dir);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[4], angleABf(dx, dz, 0.0f, 0.0f));
    return EVT_RETURN_DONE;
}

/**
 * Direction of the vector (x, z) in whole degrees (0-359):
 * binary-searches SinTable (0-90 degrees) for |z| / length and mirrors the result into the quadrant that x
 * and z select.
 */
static u16 krb_comp_dir(f32 x, f32 z) {
    f32 dist = hypot(x, z);
    f32 s = __fabs(z) / dist;
    u16 i;
    u16 lo = 0;
    u16 hi = 90;
    u16 mid;

    for (i = 0; i < 7; i++) {
        mid = lo + (hi - lo) / 2;
        if (SinTable[mid] < s) {
            lo = mid;
        }
        if (!(SinTable[(u16)mid] < s)) {
            hi = mid;
        }
    }
    mid = __fabs(SinTable[lo] - s) < __fabs(SinTable[hi] - s) ? lo : hi;
    if (x < 0.0f && z >= 0.0f) {
        mid = 180 - mid;
    }
    if (x < 0.0f && z < 0.0f) {
        mid += 180;
    }
    if (x >= 0.0f && z < 0.0f) {
        mid = 360 - mid;
    }
    return (u16)mid;
}

/**
 * Script helper: writes to args[0] who the active unit (the attacker) is by its initial kind:
 * 0 for Mario (0xDE), 1 for Goombella (0xE0) and 2 for anyone else.
 * dmg_avoid_counter_thorn_head_event uses it to pick the lecture unit's scolding line for jumping on the spiky
 * head (mac_0_092_06 for Mario, mac_0_092_07 for Goombella) and to skip it for anyone else.
 */
static USER_FUNC(get_attacker) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);

    if (unit->initialKind == 0xDE) {
        evtSetValue(event, args[0], 0);
    } else if (unit->initialKind == 0xE0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 2);
    }
    return EVT_RETURN_DONE;
}

/** Wait script of the Spiky Goomba: shows its idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
