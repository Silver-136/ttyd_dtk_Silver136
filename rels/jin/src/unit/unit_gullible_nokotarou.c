// Koops (nokotarou) partner unit for the Doopliss fight in Creepy Steeple (placed off screen in the battle setup).

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "unit/party/unit_party_nokotarou.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SPIN_ON 8       // 1 keeps the _koura_rotate_start spin loop running; _koura_rotate_stop and _restore_koura_pose clear it
#define UNIT_WORK_SPIN_SPEED 9    // signed target degrees/frame around Y; add 1 for nonnegative targets or subtract 1 for negative targets, then clamp to the target
#define UNIT_WORK_SPIN_REVERSE 10 // nonzero makes the spin loop reverse direction (negates speed and target), then clears it


BattleUnitKind unit_gullible_nokotarou;
static BattleWeapon enemyWeapon_NokotarouKouraAttack;
static BattleWeapon enemyWeapon_NokotarouShubibinAttack;
static BattleUnitDefense defence_nokotarou;
static BattleUnitDefenseAttr defence_attr_nokotarou;
static BattleUnitDefense defence_turn_nokotarou;
static StatusVulnerability regist_nokotarou;
static BattleDataEntry data_table_nokotarou[3];
static BattleUnitKindPart parts_nokotarou[1];
static EvtWord dmg_turn_event[64];
static EvtWord wakeup_event[67];
static EvtWord init_event_nokotarou[62];
static EvtWord damage_event_nokotarou[24];

#if VERSION <= VERSION_DEMO_JP
static EvtWord dead_event_nokotarou[156];
#else
static EvtWord dead_event_nokotarou[177];
#endif

static EvtWord _change_koura_pose[34];
static EvtWord _change_koura_pose_fast[34];
static EvtWord _restore_koura_pose[47];
static EvtWord _koura_rotate_start[99];
static EvtWord _koura_rotate_stop[13];
static EvtWord attack_event_nokotarou[33];
static EvtWord attack_event_nokotarou_koura[393];
static EvtWord attack_event_nokotarou_shubibin[339];
static EvtWord wait_event_nokotarou[6];

/**
 * Unit kind of the Koops double:
 * 10 HP, level 0, with its size, sounds and the links to its parts, status vulnerability and scripts.
 */
BattleUnitKind unit_gullible_nokotarou = {
    UNIT_KOOPS_CHAPTER4, // unitType
    "btl_un_gullible_nokotarou", // name
    10, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    0, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9, // pbSoftCap
    28, // width
    40, // height
    {10, 30}, // hitOffset
    {0.0f, 20.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {14.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {14.0f, 0.0f, 0.0f}, // bintaHitOffset
    {14.0f, 26.0f, 0.0f}, // kissHitOffset
    {0.0f, 20.0f, 0.0f}, // cutBaseOffset
    28.0f, // cutWidth
    40.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_PARTY_DAMAGE1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoncombatant, // attributeFlags
    &regist_nokotarou, // statusVulnerability
    1, // numParts
    parts_nokotarou, // parts
    init_event_nokotarou, // initEventCode
    data_table_nokotarou, // dataTable
};

static BattleWeapon enemyWeapon_NokotarouKouraAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {2, 3, 5, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
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

static BattleWeapon enemyWeapon_NokotarouShubibinAttack = {
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
    weaponGetPowerFromGulliblePartyAttackLv, // hpDamageFn
    {2, 3, 5, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_SHELL_TOSSLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SHELL_ATTACK, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_CONFUSED_ACTION, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PAYBACK, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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

/** Defence of the Koops double's body per damage type: 1 against every damage type. */
static BattleUnitDefense defence_nokotarou = {{1, 1, 1, 1, 1}};

/** Defence attributes of the Koops double's body: none, so every damage type is dealt normally. */
static BattleUnitDefenseAttr defence_attr_nokotarou = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Defence of the Koops double's body per damage type while flipped over: 0 against every damage type. */
static BattleUnitDefense defence_turn_nokotarou = {0};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist_nokotarou = {60, 80, 70, 100, 70, 100, 100, 70, 100, 70, 100, 70, 100, 70, 70, 0, 0, 100, 70, 100, 100, 0};
#else
static StatusVulnerability regist_nokotarou = {
    60, // sleep
    80, // stop
    70, // dizzy
    100, // poison
    70, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    70, // tiny
    100, // attackUp
    70, // attackDown
    100, // defenseUp
    90, // defenseDown
    70, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    70, // slow
    100, // dodgy
    100, // invisible
    0, // ohko
};
#endif

/** Event table of the Koops double: its defeat script, its reaction to being flipped over. */
static BattleDataEntry data_table_nokotarou[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event_nokotarou},
    {UNIT_DATA_DAMAGE_TURN_EVENT, dmg_turn_event},
    {0, NULL},
};

/** Body part of the Koops double: a single part using the model "c_pnk". */
static BattleUnitKindPart parts_nokotarou[] = {
    {
        1, "btl_un_nokotarou", "c_pnk",
        0.0f, 0.0f, 0.0f,
        15.0f, 25.0f, 0.0f,
        0.0f, 30.0f, 0.0f,
        20, 30, 255,
        &defence_nokotarou, &defence_attr_nokotarou,
        kMainPart | kGuardTargetPart | kIsShelled, 0, pose_table_nokotarou_stay,
    },
};

/**
 * Flip-over reaction of the Doopliss-controlled Koops (his data event 13):
 * swaps in the upside-down defence and pose tables, sets status flag 1 and an over-turn count of 2 turns,
 * plays the PNK_D_1 pose with SFX_ENM_INSIDE1 and bounces twice on the spot.
 */
static EVT_BEGIN(dmg_turn_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_turn_nokotarou))
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_nokotarou_turn))
    CALL(btlevtcmd_OnStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), 2)
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("PNK_D_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_INSIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 6, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

/**
 * Called on Koops's turn while he is flipped over.
 * It counts the over-turn counter down; while it is still above 0 he stays on his back and goes back to waiting.
 * At 0 he restores the normal defence and pose tables, clears status flag 1, and hops upright.
 */
// Spend each overturned action decrementing the counter; restore defense and poses only at zero.
static EVT_BEGIN(wakeup_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetOverTurnCount, LW(10), LW(0))
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetOverTurnCount, LW(10), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetPartsDefenceTable, LW(10), LW(11), PTR(&defence_nokotarou))
    CALL(btlevtcmd_OffStatusFlag, LW(10), UNIT_STATUS_FLAG_FLIPPED)
    CALL(btlevtcmd_AnimeSetPoseTable, LW(10), LW(11), PTR(pose_table_nokotarou_stay))
    CALL(btlevtcmd_AnimeChangePose, LW(10), LW(11), STRING("PNK_S_1"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Init script of the Doopliss-controlled Koops.
 * Registers his wait, attack, damage and confusion handlers,
 * clears unit work 8 to 11 (the shell-spin state used by the attack scripts), sets his walk,
 * run and jump sounds and starts him waiting.
 */
static EVT_BEGIN(init_event_nokotarou)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_nokotarou))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_nokotarou))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_nokotarou))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_ON, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_REVERSE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, 0)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 6, 12, 16)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 9, 3)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage script of the Doopliss-controlled Koops:
 * if the hit carries damage code 1024 it sets GW(20)
 * so the fake Mario's phase script knows the partner was struck, stops any shell spin,
 * then runs the default damage handling.
 */
static EVT_BEGIN(damage_event_nokotarou)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(20), 1)
    END_IF
    CALL_CHILD(PTR(_koura_rotate_stop))
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Defeat script of the Doopliss-controlled Koops.
 * He first walks back to his home position if he is away from it, then collapses.
 * With GW(19) at 0 (the fake Mario is still standing) the camera frames him
 * and his farewell lines (stg4_jin_26_03_nok and stg4_jin_26_04) play;
 * with GW(19) at 1 or 2 he only plays the collapse pose, waiting for GW(19) to reach 2 first.
 * He is then removed from the battle and GW(20) is set to 2 so the fake Mario's phase script knows he is gone.
 */
static EVT_BEGIN(dead_event_nokotarou)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(3), LW(4), LW(5))
    SET_VALUE(LW(6), 1)
    IF_INT_EQUAL(LW(0), LW(3))
        IF_INT_EQUAL(LW(1), LW(4))
            IF_INT_EQUAL(LW(2), LW(5))
                SET_VALUE(LW(6), 0)
            END_IF
        END_IF
    END_IF
    IF_INT_EQUAL(LW(6), 1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    END_IF
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    SWITCH(GW(19))
        CASE_EQUAL(0)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 130, 58, 228, 130, 38, -20, 30, 11)
        WAIT_FRAMES(37)
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg4_jin_26_03_nok"), 0, -2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_D_3"))
#if VERSION >= VERSION_JP
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
#endif
        WAIT_MS(500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 90, 58, 228, 90, 38, -20, 30, 11)
        WAIT_FRAMES(37)
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg4_jin_26_04"), 0, GW(16))
        WAIT_MS(200)
        CASE_EQUAL(1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_D_3"))
#if VERSION >= VERSION_JP
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
#endif
        LOOP(0)
            IF_INT_EQUAL(GW(19), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CASE_EQUAL(2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_D_3"))
#if VERSION >= VERSION_JP
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_DIE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
#endif
    END_SWITCH
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 1)
    SET_VALUE(GW(20), 2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

/**
 * Koops pulls into his shell: plays PNK_K_2 and then PNK_K_1
 * while turning on the spot through 18 + 6 frames of 15 degrees each.
 */
static EVT_BEGIN(_change_koura_pose)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
    LOOP(18)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
    END_LOOP
    RETURN
EVT_END()

/**
 * Faster version of the pull-into-the-shell turn:
 * 10 frames of 27 degrees in the PNK_K_2 pose, then 3 frames of 30 degrees in PNK_K_1.
 * Not called by any script in this file.
 */
static EVT_BEGIN(_change_koura_pose_fast)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
    LOOP(10)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 27, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
    LOOP(3)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 30, 0)
    END_LOOP
    RETURN
EVT_END()

/**
 * Koops comes back out of his shell after a shell attack:
 * clears the spin flag (unit work 8) and rotation,
 * then unfolds through PNK_K_1 (6 frames of 45 degrees) and PNK_K_2 (6 frames of 15 degrees).
 */
static EVT_BEGIN(_restore_koura_pose)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_ON, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_1"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 45, 0)
    END_LOOP
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PNK_K_2"))
    LOOP(6)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 15, 0)
    END_LOOP
    RETURN
EVT_END()

/**
 * Starts the shell spin: sets the spin flag (unit work 8)
 * and launches a background loop that adds a Y rotation every frame until the flag is cleared.
 * The speed ramps by 1 degree per frame towards the target in unit work 9 and is negated
 * when unit work 10 is set.
 */
static EVT_BEGIN(_koura_rotate_start)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_ON, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_REVERSE, 0)
    BEGIN_INLINE_EVENT
        LABEL(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_ON, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            GOTO(10)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_REVERSE, LW(11))
        IF_INT_NOT_EQUAL(LW(11), 0)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_REVERSE, 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(10))
            MULTIPLY(LW(10), -1)
            MULTIPLY(LW(0), -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(10))
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, LW(10))
        IF_INT_GREATER_OR_EQUAL(LW(10), 0)
            ADD(LW(0), 1)
            IF_INT_GREATER(LW(0), LW(10))
                SET_VALUE(LW(0), LW(10))
            END_IF
        ELSE
            SUBTRACT(LW(0), 1)
            IF_INT_LESS(LW(0), LW(10))
                SET_VALUE(LW(0), LW(10))
            END_IF
        END_IF
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
        LABEL(10)
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * Ends the shell spin: clears the spin flag (unit work 8), which stops the rotation loop,
 * and resets his rotation to 0.
 */
static EVT_BEGIN(_koura_rotate_stop)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_ON, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    RETURN
EVT_END()

/**
 * Attack script of the Doopliss-controlled Koops.
 * If he is still flipped over he spends the action in wakeup_event, getting up only when its counter reaches zero;
 * otherwise he draws lots between the single-target shell attack (80 percent)
 * and the all-target spin attack (20 percent), then returns to waiting.
 */
static EVT_BEGIN(attack_event_nokotarou)
    CALL(btlevtcmd_GetOverTurnCount, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL_CHILD(PTR(wakeup_event))
        RETURN
    END_IF
    CALL(btlevtcmd_DrawLots, LW(0), 2, 80, 20)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL_CHILD(PTR(attack_event_nokotarou_koura))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(attack_event_nokotarou_shubibin))
    END_SWITCH
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Koops's shell attack: after an item check he picks a target, pulls into his shell,
 * spins up (SFX_BTL_NOKO_ATTACK1/2) with the camera zooming in, then slides into the target.
 * If the pre-damage check returns a non-hit result he overshoots and walks home;
 * a hit deals the damage, and he then tumbles back in three decreasing hops,
 * comes out of the shell and walks home.
 */
static EVT_BEGIN(attack_event_nokotarou_koura)

    // He may use an enemy item instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_NokotarouKouraAttack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // Target: he picks one for his shell attack.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))

    // The attack is declared.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL_CHILD(_change_koura_pose)

    // Wind-up: he pulls into his shell and spins up while the camera zooms in.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_ATTACK1"), EVT_NULLPTR, 0, LW(13))
    SET_VALUE(LW(0), 15)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 45)
    CALL_CHILD(_koura_rotate_start)
    WAIT_FRAMES(30)

    // He slides at the target; the pre-damage check decides between a dodge and a hit.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    WAIT_FRAMES(30)
    CALL(evt_snd_sfxoff, LW(13))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_ATTACK2"), EVT_NULLPTR, 0, LW(13))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(5), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(13))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(13), MOVE_SOUND_FROM_TABLE, 0)
    IF_INT_NOT_EQUAL(LW(6), PRECHECK_HIT)
        IF_INT_EQUAL(LW(6), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
#if VERSION <= VERSION_DEMO_JP
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 40)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
#else
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
#endif
        CALL_CHILD(_koura_rotate_stop)
        CALL_CHILD(_restore_koura_pose)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        WAIT_FRAMES(30)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        GOTO(99)
    END_IF

    // Hit: the shell slams into the target for its damage.
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))

    // After the successful hit, he prepares to hop back from the target.
    LABEL(90)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))

    // He tumbles back in three decreasing hops and comes out of the shell.
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 18, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5996094f))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 12, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 8)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 8, MOVE_SOUND_FROM_TABLE)
    CALL_CHILD(_restore_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)

    // He walks back to his home position.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Koops's second attack (shubibin): a spinning shell slide that attempts to hit every selected enemy.
 * He pulls into his shell, spins up (SFX_BTL_NOKO_SHUBIBIN1/2), then slides through the targets in turn,
 * damaging those whose pre-check returns 1, and off the edge of the stage;
 * he reappears on the opposite side, returns home and comes out of the shell.
 */
static EVT_BEGIN(attack_event_nokotarou_shubibin)

    // He may use an enemy item instead of attacking.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    SET_VALUE(LW(12), PTR(&enemyWeapon_NokotarouShubibinAttack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))

    // Target: he picks the targets for his attack.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))

    // The attack is declared.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL_CHILD(_change_koura_pose)

    // Wind-up: he pulls into his shell and spins up for a second.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_SHUBIBIN1"), EVT_NULLPTR, 0, LW(13))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 0)
    SET_VALUE(LW(0), 15)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_SPEED, 45)
    CALL_CHILD(_koura_rotate_start)
    WAIT_MS(500)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    WAIT_MS(500)
    DELETE_EVENT(LW(15))
    CALL(evt_snd_sfxoff, LW(13))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_NOKO_SHUBIBIN2"), EVT_NULLPTR, 0, LW(13))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_posoffset, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    SET_VALUE(LW(10), 0)

    // He slides through the targets in turn; the pre-damage check decides between a dodge and a hit for each.
    LABEL(10)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(10.0f))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(5), LW(2))
    ADD(LW(2), 5)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_NOT_EQUAL(LW(6), PRECHECK_HIT)
        IF_INT_EQUAL(LW(6), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        GOTO(50)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 0)

    // Hit: the shell slams into that target for its damage.
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF

    // After the last target he slides off the edge of the stage.
    LABEL(80)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, -400, LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)

    // He reappears on the opposite side and walks back to his home position, coming out of his shell.
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    MULTIPLY(LW(0), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL_CHILD(_koura_rotate_stop)
    CALL_CHILD(_restore_koura_pose)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait script: plays Koops's idle pose from his pose table. */
static EVT_BEGIN(wait_event_nokotarou)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

