// Battle unit Bombshell Bill (`super_killer`): tst.rel's copy of rels/las/src/unit/unit_super_killer.c.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"


BattleUnitKind unit_super_killer;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_killer_charge_attack;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static EvtWord init_event[39];
static EvtWord damage_event[10];
static EvtWord start_avoid[26];
static EvtWord attack_event[407];
static EvtWord wait_event[6];

/**
 * Unit data of the Bombshell Bill: 3 HP, 32x26 units in size; ties together its sounds, parts,
 * status vulnerability, init event and event table.
 */
BattleUnitKind unit_super_killer = {
    UNIT_BOMBSHELL_BILL, // unitType
    "btl_un_super_killer", // name
    3, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    1, // level
    0, // bonusExp
    0, // bonusCoin
    70, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    32, // width
    26, // height
    {3, 26}, // hitOffset
    {0.0f, 13.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {16.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {16.0f, 0.0f, 0.0f}, // bintaHitOffset
    {16.0f, 16.9f, 0.0f}, // kissHitOffset
    {0.0f, 13.0f, 0.0f}, // cutBaseOffset
    32.0f, // cutWidth
    26.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_KILLER_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kFloating, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of the Bombshell Bill: 2 against every kind of hit. */
static BattleUnitDefense defence = {{2, 2, 2, 2, 2}};

/** Defence attributes of the Bombshell Bill: none, every element hits it normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {0, 0, 100, 0, 40, 150, 100, 65, 100, 65, 100, 65, 100, 65, 65, 0, 90, 100, 65, 100, 100, 90};
#else
static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    100, // dizzy
    0, // poison
    40, // confuse
    150, // electric
    100, // burn
    60, // freeze
    100, // huge
    85, // tiny
    100, // attackUp
    85, // attackDown
    100, // defenseUp
    95, // defenseDown
    65, // allergic
    0, // fright
    90, // galeForce
    100, // fast
    65, // slow
    100, // dodgy
    100, // invisible
    90, // ohko
};
#endif

/** Parts of the Bombshell Bill: 1 part (model c_kilr_g). */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_super_killer", "c_kilr_g", 0.0f, 0.0f, 0.0f, 0.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/** Weapon of the Bombshell Bill (charge attack): targets one unit for 6 damage; used by attack_event. */
static BattleWeapon weapon_killer_charge_attack = {
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
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "KIL_N_1"},
    {POSE_TYPE_STOP, "KIL_D_1"},
    {POSE_TYPE_FREEZE, "KIL_D_1"},
    {POSE_TYPE_CONFUSE, "KIL_K_1"},
    {POSE_TYPE_POISON, "KIL_X_1"},
    {POSE_TYPE_DIZZY, "KIL_X_1"},
    {POSE_TYPE_NORMAL, "KIL_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KIL_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KIL_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KIL_A_1"},
    {POSE_TYPE_DAMAGE, "KIL_D_1"},
    {POSE_TYPE_ATTACK, "KIL_A_1"},
    {POSE_TYPE_WALK, "KIL_W_1"},
    {POSE_TYPE_RUN, "KIL_R_1"},
    {POSE_TYPE_DANGER, "KIL_X_1"},
    {POSE_TYPE_PERIL, "KIL_X_1"},
    {POSE_TYPE_TALK, "KIL_T_1"},
    {POSE_TYPE_ACTION, "KIL_S_1"},
};

/** Event table of the Bombshell Bill: it has no special event scripts. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Registers the Bombshell Bill's wait, attack and damage events (confusion reuses the attack event),
 * starts the idle loop and lifts its position and home 10 units off the ground, so it flies.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 10, LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 10, LW(2))
    RETURN
EVT_END()

/** Hit reaction: just the default btldefaultevt_Damage for its single part. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Makes the target play its dodge reaction for the precheck result in LW(5) (3, 6 and 2 map to avoid poses 38,
 * 39 and
 * 40) on the target in LW(3); other results do nothing.
 */
static EVT_BEGIN(start_avoid)
    IF_INT_EQUAL(LW(5), 3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
    END_IF
    IF_INT_EQUAL(LW(5), 6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
    END_IF
    IF_INT_EQUAL(LW(5), 2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
    END_IF
    RETURN
EVT_END()

/**
 * Attack (also used when confused): uses a held item if the AI wants to, otherwise picks a target,
 * charges up (SFX_ENM_KILLER_CHARGE1) and flies at it.
 * On a hit it deals 6 damage, explodes in smoke, hides itself and pays out its coins and Star Points;
 * on a miss or dodge it flies past off screen and hides.
 * Then, as also when it has no target (unless confused into the flustered act),
 * the Bill removes itself (KillUnit): one shot per Bill.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_MS(750)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        // If the AI wants to use a held item, the Bill does that instead and ends its turn.
        CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(LW(0))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            RETURN
        END_IF
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_killer_charge_attack))
        CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_killer_charge_attack), LW(3), LW(4))
    END_IF
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
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_killer_charge_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_killer_charge_attack))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // The Bill charges up with a rumble (pose type 50), then rockets forward (SFX_ENM_KILLER_MOVE1).
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_CHARGE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ATTACK)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_killer_charge_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        GOTO(90)
        CASE_EQUAL(2)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
    END_SWITCH

    // The Bill misses or is blocked: it flies past the target, the target dodges, and it speeds off past the
    // edge of the screen.
    LABEL(90)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(8.0f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 10, LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL_CHILD(start_avoid)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 500)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 10, LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 15, 0)
    WAIT_MS(300)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    WAIT_MS(500)
    GOTO(98)

    // The Bill hits: it slams into the target and the screen shakes, then it explodes in a puff of smoke.
    LABEL(91)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(8.0f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 10, LW(2), 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_killer_charge_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_killer_charge_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(evt_btl_camera_shake_w, 0, 5, 0, 15, 0)
    CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kNoShadow)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KILLER_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    ADD(LW(1), 20)
    CALL(evt_eff, STRING(""), STRING("kemuri_test"), 17, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)

    // It pays out its coins and Star Points, shown over the spot where it blew up.
    CALL(btlevtcmd_GetCoin, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(3), LW(1), LW(2))
    ADD(LW(2), 10)
    ADD(LW(1), 30)
    CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_MS(500)
    GOTO(98)

    // The camera resets.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(200)

    // The Bill has spent its single shot, so it removes itself from the battle.
    LABEL(99)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

/** Idle: plays the stand pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

