// Rogueport Sewers (tik) battle unit: Paragoomba (patakuri): its parts, defence/status tables, and attack events.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_sub.h"

#include "system.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "battle/battle_damage.h"

static USER_FUNC(krb_get_dir);

BattleUnitKind unit_patakuri;
static BattleUnitDefense defence_patakuri;
static BattleUnitDefenseAttr defence_patakuri_attr;
static StatusVulnerability regist_patakuri;
static BattleWeapon weapon_patakuri_attack;
static BattleUnitKindPart parts_patakuri[1];
static PoseTableEntry pose_table_patakuri[19];
static BattleDataEntry data_table_patakuri[3];
static PoseSoundTiming pose_sound_timing_table[7];
static EvtWord init_event_patakuri[26];
static EvtWord dmg_fall_event[107];
static EvtWord patakuri_dmg_first_attack_pos[19];
static EvtWord attack_event_patakuri[391];
static EvtWord wait_event_patakuri[12];
static PoseTableEntry pose_table[20];
static BattleDataEntry data_table[2];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_kuriboo_attack;
static EvtWord init_event[42];
static EvtWord damage_event[10];
static EvtWord phase_event[2];
static EvtWord attack_event[492];
static EvtWord wait_event[6];

/**
 * Unit kind of the Paragoomba (HP 2, level 7): its size, offsets, damage sounds, status vulnerability,
 * parts and init event.
 */
BattleUnitKind unit_patakuri = {
    UNIT_PARAGOOMBA, // unitType
    "btl_un_patakuri", // name
    2, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    7, // level
    0, // bonusExp
    1, // bonusCoin
    60, // bonusCoinRate
    0, // baseCoin
    60, // runRate
    9999, // pbSoftCap
    23, // width
    25, // height
    {5, 25}, // hitOffset
    {0.0f, 12.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {12.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {11.5f, 0.0f, 0.0f}, // bintaHitOffset
    {11.5f, 16.25f, 0.0f}, // kissHitOffset
    {0.0f, 12.0f, 0.0f}, // cutBaseOffset
    23.0f, // cutWidth
    25.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_PATAKURI_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating | kUnitLeader, // attributeFlags
    &regist_patakuri, // statusVulnerability
    1, // numParts
    parts_patakuri, // parts
    init_event_patakuri, // initEventCode
    data_table_patakuri, // dataTable
};

/** Defence of the Paragoomba (Paragoomba form): no damage reduction. */
static BattleUnitDefense defence_patakuri = {{0, 0, 0, 0, 0}};

/** Defence of the Paragoomba (Paragoomba form): no damage reduction. */
static BattleUnitDefenseAttr defence_patakuri_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of the Paragoomba (Paragoomba form) (percent chance to take each status):
 * extra prone to stop, dizzy, fright, gale force and instant kill.
 */
#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist_patakuri = {100, 120, 120, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 150, 100, 100, 100, 100, 150};
#else
static StatusVulnerability regist_patakuri = {
    100, // sleep
    110, // stop
    120, // dizzy
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
    110, // fright
    110, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    150, // ohko
};
#endif

/** Weapon of the Paragoomba's attack: 1 damage. */
static BattleWeapon weapon_patakuri_attack = {
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
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
    0, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
 * Body part of the Paragoomba (Paragoomba form) (model c_kuribo):
 * hit position, defences, attributes and pose table.
 */
static BattleUnitKindPart parts_patakuri[] = {
    {1, "btl_un_patakuri", "c_kuribo", 0.0f, 0.0f, 0.0f, 0.0f, 22.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence_patakuri, &defence_patakuri_attr, kMainPart | kGuardTargetPart | kIsWinged | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table_patakuri},
};

static PoseTableEntry pose_table_patakuri[] = {
    {POSE_TYPE_SLEEP, "PTK_N_1"},
    {POSE_TYPE_STOP, "PTK_Y_1"},
    {POSE_TYPE_FREEZE, "PTK_Y_1"},
    {POSE_TYPE_CONFUSE, "PTK_K_1"},
    {POSE_TYPE_POISON, "PTK_I_1"},
    {POSE_TYPE_DIZZY, "PTK_I_1"},
    {POSE_TYPE_NORMAL, "PTK_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "PTK_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "PTK_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "PTK_S_1"},
    {POSE_TYPE_DAMAGE, "PTK_D_1"},
    {POSE_TYPE_ATTACK, "PTK_A_1"},
    {POSE_TYPE_RUN, "PTK_R_1"},
    {POSE_TYPE_RUN_HOME, "PTK_R_2"},
    {POSE_TYPE_WALK, "PTK_W_1"},
    {POSE_TYPE_DANGER, "PTK_I_1"},
    {POSE_TYPE_PERIL, "PTK_I_1"},
    {POSE_TYPE_TALK, "PTK_T_1"},
    {POSE_TYPE_ACTION, "PTK_S_1"},
};

/** Data table of the Paragoomba: its fall when knocked out of the air and its First Strike reaction. */
static BattleDataEntry data_table_patakuri[] = {
    {UNIT_DATA_DAMAGE_FALL_EVENT, dmg_fall_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, patakuri_dmg_first_attack_pos},
    {0, NULL},
};

/** Sounds the Paragoomba's poses play as they animate: wing flaps while waiting, hovering and moving. */
static PoseSoundTiming pose_sound_timing_table[] = {
    {"PTK_S_1", 0.01f, NULL, "SFX_ENM_PATAKURI_WAIT1", 0, 1},
    {"PTK_W_1", 0.01f, NULL, "SFX_ENM_PATAKURI_MOVE1", 0, 1},
    {"PTK_R_1", 0.01f, NULL, "SFX_ENM_PATAKURI_MOVE1", 0, 1},
    {"PTK_K_1", 0.01f, NULL, "SFX_ENM_PATAKURI_WAIT1", 0, 1},
    {"PTK_N_1", 0.01f, NULL, "SFX_ENM_PATAKURI_WAIT1", 0, 1},
    {"PTK_I_1", 0.01f, NULL, "SFX_ENM_PATAKURI_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

/**
 * Init script of the Paragoomba.
 * Registers its wait, attack, damage and confusion scripts, sets the flying movement sounds and starts waiting.
 */
static EVT_BEGIN(init_event_patakuri)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_patakuri))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_patakuri))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event_patakuri))
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Reaction when a Paragoomba loses its wings: it drops with SFX_ENM_FALL1, lands with a hop,
 * and its pose table and data switch to those of the walking Goomba.
 */

// Losing the wings installs the ground Goomba parts, data and event handlers.
static EVT_BEGIN(dmg_fall_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_OffAttribute, LW(10), kFloating)
    CALL(btlevtcmd_OffPartsAttribute, LW(10), LW(11), kIsWinged)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), LW(14))
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table))
    CALL(btlevtcmd_ChangeDataTable, LW(10), PTR(data_table))
    CALL(btlevtcmd_ReplaceParts, LW(10), LW(11), PTR(parts), 1)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_KIND_CHANGED)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_SetFallAccel, LW(10), FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, LW(10), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, LW(10), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(500)
    CALL(btlevtcmd_ChangeKind, LW(10), UNIT_GOOMBA)
    CALL_CHILD(init_event)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePoseType, LW(10), LW(11), POSE_TYPE_DAMAGE)
    RETURN
EVT_END()

/**
 * Data-table event 48, run when the battle opens with Mario's or his partner's First Strike on the Paragoomba:
 * puts it at height 10 and waits for the attack to end.
 */
static EVT_BEGIN(patakuri_dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * The Paragoomba's dive attack: it flies up, swoops down on the target with SFX_ENM_PATAKURI_MOVE2 and MOVE3,
 * and the pre-damage check decides between a miss (with an avoid animation for some results)
 * and a hit (damage with the action-command result); then it returns to its place.
 */
static EVT_BEGIN(attack_event_patakuri)

    // The Paragoomba may use an item instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF

    // Choose a target; a confused Paragoomba with none is flustered, and either way the turn ends.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_patakuri_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_patakuri_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // The camera frames the Paragoomba and its target, and the attack is announced.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_patakuri_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_patakuri_attack))

    // It flaps up and hovers above and in front of the target, turning to face it.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 50)
    ADD(LW(1), 40)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(100)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)

    // It folds its wings and dives at the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 50)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAKURI_MOVE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 5, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_NONE)

    // Check how the target defends against the dive.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_patakuri_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // A miss or dodge: the Paragoomba swoops past and then pulls up.
    LABEL(90)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAKURI_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(0), 50)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -20, EASING_QUADRATIC_IN, 0, MOVE_SOUND_NONE)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    WAIT_MS(500)
    GOTO(98)

    // The dive hits: the camera shakes and the damage is dealt.
    LABEL(91)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_PATAKURI_MOVE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -16, EASING_QUADRATIC_IN, 0, MOVE_SOUND_NONE)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_patakuri_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_patakuri_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)

    // It flutters back home and returns to waiting.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_PATAKURI_MOVE1"), STRING("SFX_ENM_PATAKURI_MOVE1"), 0, 15, 15, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(7.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_LINEAR, 0, MOVE_SOUND_KEEP)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script of the Paragoomba: shows its flying idle pose from the pose table. */
static EVT_BEGIN(wait_event_patakuri)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    LABEL(0)
    WAIT_FRAMES(30)
    GOTO(0)
    RETURN
EVT_END()

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "KUR_N_1"},
    {POSE_TYPE_STOP, "KUR_Y_1"},
    {POSE_TYPE_FREEZE, "KUR_Y_1"},
    {POSE_TYPE_CONFUSE, "KUR_K_1"},
    {POSE_TYPE_POISON, "KUR_I_1"},
    {POSE_TYPE_DIZZY, "KUR_I_1"},
    {POSE_TYPE_NORMAL, "KUR_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KUR_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KUR_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KUR_S_1"},
    {POSE_TYPE_DAMAGE, "KUR_D_1"},
    {POSE_TYPE_ATTACK, "KUR_A_1"},
    {POSE_TYPE_ATTACK_2, "KUR_A_2"},
    {POSE_TYPE_RUN, "KUR_R_1"},
    {POSE_TYPE_RUN_HOME, "KUR_R_2"},
    {POSE_TYPE_WALK, "KUR_W_1"},
    {POSE_TYPE_DANGER, "KUR_I_1"},
    {POSE_TYPE_PERIL, "KUR_I_1"},
    {POSE_TYPE_TALK, "KUR_T_1"},
    {POSE_TYPE_ACTION, "KUR_S_1"},
};

/** Data table of the Paragoomba: event handlers for a First Strike on it (no reaction). */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0},
};

/** Defence of the Paragoomba: no damage reduction. */
static BattleUnitDefense defence = {0};

/** Element reactions of the Paragoomba: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {0};

/**
 * Status vulnerability of the Paragoomba (percent chance to take each status):
 * extra prone to stop, fright and instant kill; also gale force in the demos.
 */
#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {100, 120, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 120, 120, 100, 100, 100, 100, 150};
#else
static StatusVulnerability regist = {
    100, // sleep
    110, // stop
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
    110, // fright
    100, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    150, // ohko
};
#endif

/** Parts of the Paragoomba : hit positions, defences, attributes and pose tables. */
static BattleUnitKindPart parts[] = {
    {
        1, // partNum
        "btl_un_kuriboo", // partName
        "c_kuribo", // modelName
        {0}, // positionOffset
        {0.0f, 22.0f, 0.0f}, // hitBaseOffset
        {0.0f, 30.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        kMainPart | kGuardTargetPart, // attributes
        0, // counterAttributes
        pose_table, // poseTable
    },
};

/** Weapon of the Paragoomba's headbonk: 1 damage, jump-type contact attack. */
static BattleWeapon weapon_kuriboo_attack = {
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
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED, // special
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
 * Init script of the Goomba form of the Paragoomba (after losing its wings).
 * Registers its wait, attack and damage scripts and the walking sounds.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 6, 6)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 6, 6)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_ENM_KURI_JUMP1"), STRING("SFX_ENM_KURI_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Damage reaction of the walking Paragoomba: the default damage script. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/** Phase script of the walking Paragoomba; does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/** The Goomba-form headbutt attack, the same as the Goomba's. */
static EVT_BEGIN(attack_event)

    // Choose the target: the First Strike target if the battle opened with one, otherwise a normal pick.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_kuriboo_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_kuriboo_attack), LW(3), LW(4))
    END_IF

    // No target left: a confused Goomba is flustered; either way it ends its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF

    // It may use a held item instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF

    // The camera frames the Goomba and its target, then the attack is announced
    // and the guard timing is awaited.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.5996094f))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_kuriboo_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_kuriboo_attack))

    // The Goomba runs up to a spot 100 units in front of the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 100)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(170)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_nomove_y_onoff, 0, 1)

    // It leaps at the target in an arc, tilting along its path and curling into its attack pose.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPosFloat, UNIT_ID_SELF, LW(5), LW(6), LW(7))
        SET_VALUE(LW(8), 0)
        LOOP(32)
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
        SET_VALUE(LW(15), POSE_TYPE_ATTACK)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, LW(15))
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 32, MOVE_SOUND_FROM_TABLE)

    // Check how the target defends against the headbonk.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_kuriboo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
        GOTO(91)
        CASE_END
    END_SWITCH

    // A miss or dodge: the Goomba bounces on past it in two shrinking hops.
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(15), POSE_TYPE_ACTION)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_MoveDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(500)
    GOTO(95)

    // The headbonk lands: the camera shakes and the damage is dealt,
    // then the Goomba bounces back off the target in three small hops.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KURI_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_kuriboo_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_kuriboo_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(15), POSE_TYPE_ACTION)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 0)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, MOVE_SOUND_FROM_TABLE)
    GOTO(95)

    // Run back home and return to waiting.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 3, 3, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN_HOME)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_KEEP, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

// Preserve the previous angle when the two sampled positions coincide.
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

/** Wait script of the walking Paragoomba: shows its idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

