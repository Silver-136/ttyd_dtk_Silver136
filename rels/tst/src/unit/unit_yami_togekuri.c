#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_msg.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_sub.h"
#include "battle/unit_work.h"

#include "system.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "battle/battle_database_common.h"
#include "battle/battle_damage.h"

static USER_FUNC(get_attacker);
static USER_FUNC(krb_get_dir);
static u16 krb_comp_dir(f32 x, f32 z);

BattleUnitKind unit_yami_togekuri;
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
 * Unit data of the Spiky Gloomba in the Pit of 100 Trials: 7 HP, level 13;
 * names its parts, status table and init script.
 */
BattleUnitKind unit_yami_togekuri = {
    UNIT_SPIKY_GLOOMBA, // unitType
    "btl_un_yami_togekuri", // name
    7, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    13, // level
    0, // bonusExp
#if VERSION <= VERSION_DEMO_US
1,
#else
    2, // bonusCoin
#endif
    70, // bonusCoinRate
#if VERSION <= VERSION_DEMO_US
1,
#else
    0, // baseCoin
#endif
    50, // runRate
    9999, // pbSoftCap
    23, // width
    30, // height
#if VERSION <= VERSION_DEMO_US
    5,
    30,
    0.0f,
    15.0f,
    0.0f,
    0,
    0,
#else
    {5, 30}, // hitOffset
    {0.0f, 15.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {12.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
#endif
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
#if VERSION <= VERSION_DEMO_US
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
#else
    {11.5f, 0.0f, 0.0f}, // bintaHitOffset
    {11.5f, 18.0f, 0.0f}, // kissHitOffset
    {0.0f, 15.0f, 0.0f}, // cutBaseOffset
#endif
    23.0f, // cutWidth
    30.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
#if VERSION <= VERSION_DEMO_US
    0.0f,
    0.0f,
    0.0f,
#else
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
#endif
    "SFX_ENM_TOGEKURI_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitLeader, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/**
 * Defence of the Spiky Gloomba against each element (normal, fire, ice, explosion, electric):
 * none, so every hit does its full damage.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/**
 * Damage modifiers of the Spiky Gloomba per element (normal, fire, ice, explosion, electric):
 * none, so no element is a weakness, an immunity or a heal.
 */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {80, 100, 80, 100, 80, 100, 100, 80, 100, 80, 100, 80, 100, 80, 80, 100, 100, 100, 80, 100, 100, 80};
#else
static StatusVulnerability regist = {
    80, // sleep
    90, // stop
    80, // dizzy
    100, // poison
    80, // confuse
    100, // electric
    100, // burn
    80, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    90, // defenseDown
    80, // allergic
    90, // fright
    90, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    80, // ohko
};
#endif

/** Part of the Spiky Gloomba: its one body part with hit box, defence tables and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_yami_togekuri", "c_kuribo_y", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, kCounterTopSpiky, pose_table},
};

/** Weapon of the Spiky Gloomba's spiked tumble: damage 4. */
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
 * Pose table of the Spiky Gloomba: animation names for each pose type (idle, walk, hurt, attack and so on).
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "TGK_N_1"},
    {POSE_TYPE_STOP, "TGK_Y_1"},
    {POSE_TYPE_FREEZE, "TGK_Y_1"},
    {POSE_TYPE_CONFUSE, "TGK_K_1"},
    {POSE_TYPE_POISON, "TGK_I_1"},
    {POSE_TYPE_DIZZY, "TGK_I_1"},
    {POSE_TYPE_NORMAL, "TGK_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "TGK_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "TGK_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "TGK_S_1"},
    {POSE_TYPE_DAMAGE, "TGK_D_1"},
    {POSE_TYPE_ATTACK, "TGK_A_1"},
    {POSE_TYPE_RUN_HOME, "TGK_R_2"},
    {POSE_TYPE_RUN, "TGK_R_1"},
    {POSE_TYPE_WALK, "TGK_W_1"},
    {POSE_TYPE_DANGER, "TGK_I_1"},
    {POSE_TYPE_PERIL, "TGK_I_1"},
    {POSE_TYPE_TALK, "TGK_T_1"},
    {POSE_TYPE_ACTION, "TGK_S_1"},
};

/**
 * Data table of the Spiky Gloomba: event scripts
 * and values the battle looks up by tag (damage reactions, dodge, first strike and so on).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, dmg_avoid_counter_thorn_head_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Init event of the Spiky Gloomba: installs its wait, attack, damage and confusion events and sets its run,
 * walk and jump sounds.
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
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_LECTURE, LECTURE_KURIBOO_WORK_TOGEKURI_TALK_DONE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(get_attacker, LW(0))
            IF_INT_EQUAL(LW(0), 2)
                RETURN
            END_IF
            WAIT_MS(2500)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_LECTURE, -1)
            CALL(get_attacker, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_092_06"), 0, -6)
                ELSE
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("mac_0_092_07"), 0, -6)
            END_IF
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_LECTURE, LECTURE_KURIBOO_WORK_TOGEKURI_TALK_DONE, 1)
            CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_LECTURE, 1)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
        END_IF
    END_IF
    RETURN
EVT_END()

/** Damage event of the Spiky Gloomba: runs the default damage handling with LW(10) = -2 and LW(11) = 1. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Sine of 0 to 90 degrees in one-degree steps; the Spiky Gloomba's krb_get_dir searches it to turn a slope
 * into an angle.
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
 * Attack event of the Spiky Gloomba (weapon_togekuri_attack): uses an enemy item if it has one,
 * otherwise picks a target and flusters if there is none.
 * Otherwise it runs at the target and tumbles onto it with its spikes (krb_get_dir gives the spin).
 * A miss makes it land on its back (TGK_D_1) and roll upright again (TGK_A_2, TGK_K_1);
 * a hit applies the damage with the action command defence check. It then hops off and walks back to its place.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        // It uses an enemy item if it has one.
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.5996094f))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    // Pick a target for the attack; a confused unit that has none flusters.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_togekuri_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_togekuri_attack), LW(3), LW(4))
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
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_togekuri_attack))
    // Announce the attack; the camera swings to follow it.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_togekuri_attack))
    // Run at the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 80)
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
    // Curl up and tumble onto the target, spikes first.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 15, 0)
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
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK_2)
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(2), 2)
    SUBTRACT(LW(1), 4)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 28, MOVE_SOUND_FROM_TABLE)
    // Find out whether the attack will connect or miss.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togekuri_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    END_SWITCH
    // The attack misses or is dodged.
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KURI_LANDING_MISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, 0, FLOAT(-12.0f), 0)
    // A miss makes it land on its back.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("TGK_D_1"))
    WAIT_FRAMES(40)
    // It rolls upright again.
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(40)
    GOTO(95)
    // The attack connects.
    LABEL(91)
    CALL(evt_btl_camera_shake_h, 0, 1, 1, 8, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_add_zoom, 0, -50)
    LOOP(4)
        // The landing squashes it flat for a moment.
        CALL(btlevtcmd_AddScale, UNIT_ID_SELF, FLOAT(0.07910156f), FLOAT(-0.119140625f), 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_togekuri_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_togekuri_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    // It tumbles off the target and hops clear.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 28, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, MOVE_SOUND_FROM_TABLE)
    GOTO(95)
    // Walk back to its place.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_ENM_KURI_MOVE1"), STRING("SFX_ENM_KURI_MOVE2"), 0, 3, 3, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN_HOME)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(5.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_KEEP, 0)
    // The attack is over; back to waiting.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Script helper: args[0..1] = x, z of the first point, args[2..3] = x, z of the second,
 * args[4] = angle in and out.
 * Returns the angle (degrees) from the first point towards the second; unchanged if the points coincide.
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

double hypot(double x, double y);

/**
 * Angle in degrees (0-359) of the vector (x, z) from the sine table,
 * found by a seven-step binary search of SinTable and then placed in the right quadrant.
 * Not called anywhere in this file.
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
 * Script helper: returns in args[0] who is acting: 0 if the active unit began as Mario (kind 0xDE),
 * 1 if as Goombella (0xE0), else 2. Picks the tutorial line in dmg_avoid_counter_thorn_head_event.
 */
static USER_FUNC(get_attacker) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit = BattleGetUnitPtr(wp, wp->activeUnitId);

    if (unit->initialKind == UNIT_MARIO) {
        evtSetValue(event, args[0], 0);
    } else if (unit->initialKind == UNIT_GOOMBELLA) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 2);
    }
    return EVT_RETURN_DONE;
}

/** Idle event: shows the unit's table pose for state 1. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

