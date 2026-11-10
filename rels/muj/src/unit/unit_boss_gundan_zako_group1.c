// Keelhaul Key (muj) battle unit: X-Naut platoon (gundan_zako_group1, horde group 1) in Lord Crump's fight: its parts,
// defence/status tables, and attack events.

#include "version.h"
#include "zakogun.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_camera.h"
#include "evt/evt_ext.h"
#include "evt/evt_npc.h"
#include "battle/battle_status_effect.h"
#include "rels/muj/include/muj.h"
#include "battle/battle_sub.h"
#include "unit_boss_gundan_zako.h"

#include "battle/battle.h"
#include "battle/battle_stage_object.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "driver/dispdrv.h"
#include "mario/mariost.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "driver/extdrv.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_HIT_CUE 0 // nonzero once the first member is 10 frames from landing on the target (zakogun_ext_main1 adds 1); hit_check_event waits for it, then checks damage

extern ZakoGun* zakogun;
extern ZakoGunWork* work;
void zakogun_ext_main1(void);
static void zakogun_ext_disp(CameraId camera, void* param);

USER_FUNC(zakogun_attack1);
static USER_FUNC(zakogun_end_sukima);
static USER_FUNC(zakogun_start_sukima);
USER_FUNC(zakogun_dead1);
static void zakogun_ext_main(void);
static void zakogun_ext_init(void);

BattleUnitKind unit_boss_gundan_zako_group1;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[4];
static BattleDataEntry data_table[4];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_gundan_zako_group1_attack;
static EvtWord init_event[44];
static EvtWord damage_event[134];
EvtWord damage_event_kaiten_hammer[30];
static EvtWord dead_event[138];
static EvtWord attack_event[85];
static EvtWord hit_check_event[95];
static EvtWord wait_event[6];
static ExtTexInfo zakogun_ext_entry_data[49];
EvtWord zakogun_entry[19];

/** Script helper: stores the battle unit id args[0] that the X-Naut horde is aiming at (work->targetId). */
USER_FUNC(zakogun_set_target_id) {
    work->targetId = evtGetValue(event, event->args[0]);
    return EVT_RETURN_DONE;
}

/**
 * Sets the animation of one horde member: which texture to start from (base),
 * how many (num) and the frames per texture (div), and restarts the animation counter.
 */
void zakogun_set_pose(ZakoGun* zako, s32 base, s32 num, s32 div) {
    zako->poseBase = base;
    zako->poseNum = num;
    zako->poseDiv = div;
    zako->poseCount = 0;
}

/**
 * Script helper: when the horde takes damage, switches every one of the 500 X-Nauts to the hurt texture for the
 * current formation (group 1, 2 or 3), once on the first call.
 */
USER_FUNC(zakogun_damage_pose) {
    s32 i;
    ZakoGun* zako;

    if (isFirstCall) {
        for (i = 0; i < 500; i++) {
            zako = &zakogun[i];
            if (zako != NULL) {
                switch (work->flag) {
                    case 0:
                    case 2:
                        zakogun_set_pose(zako, 0x21, 3, 3);
                        break;
                    case 1:
                        zakogun_set_pose(zako, 0x2d, 3, 2);
                        break;
                }
            }
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: after the damage animation switches every X-Naut of the horde back to its idle texture for the
 * current formation, once on the first call.
 */
USER_FUNC(zakogun_damage_pose_cancel) {
    s32 i;
    ZakoGun* zako;

    if (isFirstCall) {
        for (i = 0; i < 500; i++) {
            zako = &zakogun[i];
            if (zako != NULL) {
                switch (work->flag) {
                    case 0:
                        zakogun_set_pose(zako, 0x15, 1, 1);
                        break;
                    case 1:
                        zakogun_set_pose(zako, 0x25, 4, 6);
                        break;
                    case 2:
                        zakogun_set_pose(zako, 0, 20, 2);
                        break;
                }
            }
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Returns the battle unit of the horde: the first unit whose kind is 0x64,
 * 0x65 or 0x66 (the three horde groups), or NULL.
 */
BattleUnit* zakogun_get_ptr(void) {
    BattleUnit* unit;
    s32 i;
    BattleWork* wp = _battleWorkPointer;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && (unit->initialKind == UNIT_X_NAUTS_CRUMP_FORMATION1 || unit->initialKind == UNIT_X_NAUTS_CRUMP_FORMATION2 || unit->initialKind == UNIT_X_NAUTS_CRUMP_FORMATION3)) {
            return unit;
        }
    }
    return NULL;
}

/**
 * Unit data of X-Nauts: 6 HP, 50x50 units in size; ties together its sounds, parts, status vulnerability,
 * init event and event table.
 */
BattleUnitKind unit_boss_gundan_zako_group1 = {
    UNIT_X_NAUTS_CRUMP_FORMATION1, // unitType
    "btl_un_gundan_zako_group1", // name
    6, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    0, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    50, // width
    50, // height
    {0, 50}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {25.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {25.0f, 0.0f, 0.0f}, // bintaHitOffset
    {25.0f, 32.5f, 0.0f}, // kissHitOffset
    {-4.0f, 25.0f, 0.0f}, // cutBaseOffset
    50.0f, // cutWidth
    50.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_TEAM_3RD_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoSpin, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of X-Nauts: 0 against every kind of hit. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Defence attributes of X-Nauts: none, every element hits it normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/**
 * Status vulnerability of X-Nauts (susceptibility multipliers): immune to sleep, stop, dizzy,
 * poison, confusion, electrified, burn, freeze, Huge, Tiny, Attack Up, Attack Down, Defense Up,
 * Defense Down, allergic, fright, Gale Force, Fast, Slow, Dodgy, Invisible, instant KO.
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
    {POSE_TYPE_NORMAL, "Z_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "Z_1"},
    {POSE_TYPE_DAMAGE, "Z_1"},
    {POSE_TYPE_ACTION, "Z_1"},
};

/** Event table of X-Nauts: damage_event_kaiten_hammer (tag 18), dead_event. */
static BattleDataEntry data_table[] = {
    {18, damage_event_kaiten_hammer},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/** Parts of X-Nauts: 1 part (model c_zako_n). */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_gundan_zako_group1", "c_zako_n", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 40.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHidden | kNoShadow, 0, pose_table},
};

/** Weapon of X-Nauts (attack): targets one unit for 5 damage; used by attack_event. */
static BattleWeapon weapon_gundan_zako_group1_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#if VERSION >= VERSION_US
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT, // targetWeightFlags
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
 * Init event of X-Naut group 1: installs its wait, attack, damage and confusion events,
 * clears UNIT_WORK_HIT_CUE (the pre-landing hit cue) and moves it to z = 15;
 * Lord Crump's init already registered the X-Naut crowd.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HIT_CUE, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), 15)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), 15)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage event of X-Naut group 1: when the hit did damage or the group has one of several statuses (KO, sleep,
 * dizzy, confusion, freeze, stop, poison, burn, tiny, defense down, slow or allergic),
 * the X-Nauts take the hurt pose and freeze in formation (zakogun_start_sukima 0).
 * Then the default damage reaction runs and the X-Nauts go back to normal.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamage, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        SET_VALUE(LW(15), 1)
    ELSE
        SET_VALUE(LW(15), 0)
    END_IF
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INSTANT_KILL, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_SLEEP, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DIZZY, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_CONFUSE, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_FREEZE, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_STOP, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_POISON, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_BURN, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_TINY, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_DEFENSE_DOWN, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_SLOW, LW(14))
    OR(LW(15), LW(14))
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_ALLERGIC, LW(14))
    OR(LW(15), LW(14))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(zakogun_damage_pose)
        CALL(zakogun_start_sukima, 0)
    END_IF
    CALL_CHILD(btldefaultevt_Damage)
    CALL(zakogun_damage_pose_cancel)
    CALL(zakogun_end_sukima)
    RETURN
EVT_END()

/**
 * Damage event for the horde in group 1 on a spinning-hammer or Ultra Hammer hit (data tag 18):
 * when damage was dealt, plays the hurt animation (zakogun_damage_pose), shakes the X-Nauts in place (zakogun_start_sukima 1)
 * for 80 frames, then restores the X-Nauts to formation.
 */
EVT_BEGIN(damage_event_kaiten_hammer)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetDamage, UNIT_ID_SELF, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(zakogun_damage_pose)
        CALL(zakogun_start_sukima, 1)
        WAIT_FRAMES(80)
        CALL(zakogun_damage_pose_cancel)
        CALL(zakogun_end_sukima)
    ELSE
        WAIT_FRAMES(80)
    END_IF
    RETURN
EVT_END()

/**
 * Death event of X-Naut group 1: marks the group dying (unit work 1),
 * the six X-Nauts collapse (zakogun_dead1) with the enemy-death sounds and smoke,
 * its coins and experience are stored with the Star Point effect, and the unit is removed.
 */
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, ZAKO_GROUP1_WORK_DYING, 1)
    CALL(zakogun_dead1)
    WAIT_FRAMES(36)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(48)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        LOOP(10)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(evt_sub_random, 100, LW(3))
            SUBTRACT(LW(3), 50)
            ADD(LW(0), LW(3))
            CALL(evt_sub_random, 30, LW(3))
            ADD(LW(1), LW(3))
            SET_VALUE(LW(2), 75)
            CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetCoin, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(3), LW(1), LW(2))
    CALL(_add_star_point_disp_offset, UNIT_ID_SELF, LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    ELSE
        CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 2)
    END_IF
    RETURN
EVT_END()

/**
 * Attack event of X-Naut group 1: picks one target with weapon_gundan_zako_group1_attack
 * and the six X-Nauts leap at it (zakogun_attack1) while hit_check_event deals the hit.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HIT_CUE, 0)
    SET_VALUE(LW(9), PTR(&weapon_gundan_zako_group1_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF

        // BUG: this script has no LABEL(99): evtSearchLabel resumes at this GOTO's argument word, which runs
        // as SET_TYPE with the next command word as its value, and the script goes on after the GOTO.
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(zakogun_set_target_id, LW(3))
    BEGIN_SIBLING_EVENT
        CALL_CHILD(hit_check_event)
    END_SIBLING_EVENT
    CALL(zakogun_attack1)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Hit check of X-Naut group 1's attack: waits for the lead X-Naut's pre-landing cue (UNIT_WORK_HIT_CUE, raised when
 * its landing timer reaches 10), then starts the target's avoid pose on a dodge
 * or applies the hit with the action-command defence.
 */
static EVT_BEGIN(hit_check_event)
    LOOP(0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HIT_CUE, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
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
    LABEL(90)
    GOTO(98)
    LABEL(91)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(98)
    LABEL(98)
    LABEL(99)
    RETURN
EVT_END()

/** Wait (idle) event of X-Naut group 1: sets the idle pose from its pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

static ExtTexInfo zakogun_ext_entry_data[] = {
    {"c_zako_n", "S_1", 0.0f},
    {"c_zako_n", "S_1", 2.0f},
    {"c_zako_n", "S_1", 4.0f},
    {"c_zako_n", "S_1", 6.0f},
    {"c_zako_n", "S_1", 8.0f},
    {"c_zako_n", "S_1", 10.0f},
    {"c_zako_n", "S_1", 12.0f},
    {"c_zako_n", "S_1", 14.0f},
    {"c_zako_n", "S_1", 16.0f},
    {"c_zako_n", "S_1", 18.0f},
    {"c_zako_n", "S_1", 20.0f},
    {"c_zako_n", "S_1", 22.0f},
    {"c_zako_n", "S_1", 24.0f},
    {"c_zako_n", "S_1", 26.0f},
    {"c_zako_n", "S_1", 28.0f},
    {"c_zako_n", "S_1", 30.0f},
    {"c_zako_n", "S_1", 32.0f},
    {"c_zako_n", "S_1", 34.0f},
    {"c_zako_n", "S_1", 36.0f},
    {"c_zako_n", "S_1", 38.0f},
    {"c_zako_n", "Z_2", 0.0f},
    {"c_zako_n", "Z_3", 0.0f},
    {"c_zako_n", "R_1", 0.0f},
    {"c_zako_n", "R_1", 1.0f},
    {"c_zako_n", "R_1", 2.0f},
    {"c_zako_n", "R_1", 3.0f},
    {"c_zako_n", "R_1", 4.0f},
    {"c_zako_n", "R_1", 5.0f},
    {"c_zako_n", "R_1", 6.0f},
    {"c_zako_n", "R_1", 7.0f},
    {"c_zako_n", "R_1", 8.0f},
    {"c_zako_n", "R_1", 9.0f},
    {"c_zako_n", "A_3", 0.0f},
    {"c_zako_n", "D_1", 0.0f},
    {"c_zako_n", "D_1", 3.0f},
    {"c_zako_n", "D_1", 6.0f},
    {"c_zako_n", "J_1A", 0.0f},
    {"c_zako_n", "S_2", 0.0f},
    {"c_zako_n", "S_2", 10.0f},
    {"c_zako_n", "S_2", 20.0f},
    {"c_zako_n", "S_2", 30.0f},
    {"c_zako_n", "A_5", 0.0f},
    {"c_zako_n", "A_5", 8.0f},
    {"c_zako_n", "A_5", 12.0f},
    {"c_zako_n", "A_2", 0.0f},
    {"c_zako_n", "D_5", 0.0f},
    {"c_zako_n", "D_5", 2.0f},
    {"c_zako_n", "D_5", 4.0f},
    {0},
};

/**
 * Spawns the horde: registers the "ext_zakogun" NPC with the c_zako_n model
 * and 500 extended poses driven by zakogun_ext_init and zakogun_ext_main.
 */
EVT_BEGIN(zakogun_entry)
    CALL(evt_npc_entry, STRING("ext_zakogun"), STRING("c_zako_n")) // hidden X-Naut template for the 500-sprite horde
    CALL(evt_npc_set_position, STRING("ext_zakogun"), 0, -1000, 0)
    CALL(evt_ext_entry, 500, PTR(zakogun_ext_entry_data), PTR(zakogun_ext_init), PTR(zakogun_ext_main), 0)
    RETURN
EVT_END()

static void zakogun_ext_init(void) {
    u8* flags = &_battleWorkPointer->stageObjectWork.flags;
    BattleStageObject* body;
    BattleStageObject* wire;

    work = BattleAlloc(sizeof(ZakoGunWork));
    work->zako = BattleAlloc(500 * sizeof(ZakoGun));
    zakogun = work->zako;
    work->flag = 0;
    body = BattleSearchObjectPtr("if_body");
    wire = BattleSearchObjectPtr("if_wire");
    wire->position = (Vec){0.0f, 250.0f, 0.0f};
    body->position = wire->position;
    *flags |= STAGE_FLAG_NO_IRON_FRAME | STAGE_FLAG_KEEP_IRON_FRAME_UNUSED;
}

/** Main callback of the horde extended draw: runs the update for the current formation (group 1, 2 or 3). */
static void zakogun_ext_main(void) {
    switch (work->flag) {
        case 0:
            zakogun_ext_main1();
            break;
        case 1:
            zakogun_ext_main2();
            break;
        case 2:
            zakogun_ext_main3();
            break;
    }
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, zakogun_ext_disp, NULL);
}

/** Draw callback of the horde: loads render mode, vertices, textures and TEV then draws all the extended poses. */
static void zakogun_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

/**
 * Script helper: writes the battle unit id of the horde leader (the unit of kind 0x63, Lord Crump of chapter 5) to
 * args[0], or -1 if there is none.
 */
USER_FUNC(zakogun_get_kanbu_id_evtcmd) {
    BattleUnit* unit;
    s32 id = -1;
    BattleWork* wp = _battleWorkPointer;
    s32 i;
    EvtWord* args = event->args;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == UNIT_LORD_CRUMP_CHAPTER5) {
            id = i;
        }
    }
    evtSetValue(event, args[0], id);
    return EVT_RETURN_DONE;
}

/** Returns the battle unit id of the horde leader (kind 0x63), or -1 if there is none. */
s32 zakogun_get_kanbu_id(void) {
    s32 i;
    BattleUnit* unit;
    BattleWork* wp = _battleWorkPointer;
    s32 id = -1;

    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit != NULL && unit->initialKind == UNIT_LORD_CRUMP_CHAPTER5) {
            id = i;
        }
    }
    return id;
}

/** Script helper: starts formation 1: selects it (work->flag = 0) and sets up the first six X-Nauts of the pool. */
USER_FUNC(zakogun_ext_init1) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();

    work->flag = 0;
    for (i = 0; i < 6; i++, pose++) {
        zakogun_ext_init123_common(pose, i);
    }
    return EVT_RETURN_DONE;
}

/**
 * Common initialisation for one horde member: shows its pose,
 * puts it below the stage at (0, -1000, 0) facing right and clears its state, timer and position.
 */
void zakogun_ext_init123_common(ExtdrvPose* pose, s32 i) {
    pose->flags = 1;
    PSMTXIdentity(*pose->mtx);
    (*pose->mtx)[0][3] = 0.0f;
    (*pose->mtx)[1][3] = -1000.0f;
    (*pose->mtx)[2][3] = 0.0f;
    pose->dir = 90.0f;
    zakogun[i].field_0x0 = 0;
    zakogun[i].state = 0;
    zakogun[i].timer = 0;
    zakogun[i].pos = (Vec){0.0f, -1000.0f, 0.0f};
    zakogun[i].home = (Vec){0.0f, 0.0f, 0.0f};
    zakogun[i].rot.y = 0.0f;
}

/**
 * Per-frame update of formation 1 (six X-Nauts):
 * moves each one through its state machine (appearing, walking, attacking, hurt, dead, running away)
 * and updates its pose matrix.
 */
void zakogun_ext_main1(void) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit = zakogun_get_ptr();
    ExtdrvPose* pose = extGetPosePtr();
    ZakoGun* zako;
    s32 i;
    s32 n;
    s32 j;
    f32 x, y, z;
    f32 d;
    Mtx m;

    for (i = 0; i < 6; i++, pose++) {
        zako = &zakogun[i];
        pose->texId = zako->poseBase + zako->poseCount / zako->poseDiv;
        zako->poseCount++;
        if (zako->poseCount >= zako->poseNum * zako->poseDiv) {
            zako->poseCount = 0;
        }
        if (i == 1 && zako->state >= 35 && zako->state < 80) {
            unit->position.x = zako->pos.x;
        }
        switch (zako->state) {
            case 0:
                zako->state = 5;
                zako->timer = 0;
                zako->rot.x = zako->rot.y = zako->rot.z = 0.0f;
                zakogun_set_pose(zako, 0, 20, 2);
                break;
            case 5:
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->timer = 0;
                    zako->state = 10;
                }
                break;
            case 10:
                zako->pos.x = zako->start.x = 300.0f + irand(100);
                zako->pos.y = zako->start.y = 0.0f;
                zako->pos.z = zako->start.z = irand(100) - 50.0f;
                zako->state = 12;
                zako->rot.y = 0.0f;
            case 12:
                x = unit->homePosition.x;
                y = unit->homePosition.y;
                z = unit->homePosition.z;
                switch (i) {
                    case 0:
                        zako->target.x = x - 10.0f;
                        zako->target.y = y;
                        zako->target.z = z - 10.0f;
                        break;
                    case 1:
                        zako->target.x = x;
                        zako->target.y = y;
                        zako->target.z = z;
                        break;
                    case 2:
                        zako->target.x = x + 10.0f;
                        zako->target.y = y;
                        zako->target.z = z + 10.0f;
                        break;
                    case 3:
                        zako->target.x = x + 45.0f;
                        zako->target.y = y;
                        zako->target.z = z - 5.0f;
                        break;
                    case 4:
                        zako->target.x = x + 55.0f;
                        zako->target.y = y;
                        zako->target.z = z + 5.0f;
                        break;
                    case 5:
                        zako->target.x = x + 75.0f;
                        zako->target.y = y;
                        zako->target.z = z;
                        break;
                }
                zako->state = 15;
                zako->timer = zako->timerMax = irand(40) + 40;
                zakogun_set_pose(zako, 0x16, 10, 1);
                break;
            case 15:
                zako->pos.x = intplGetValue(EASING_LINEAR, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y = intplGetValue(EASING_LINEAR, zako->start.y, zako->target.y, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(EASING_LINEAR, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                if (i == 5) {
                    if (zako->timer % 12 == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1L", 0x7f, 0, &zako->pos);
                    }
                    if (zako->timer % 12 == 6) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1R", 0x7f, 0, &zako->pos);
                    }
                }
                zako->timer--;
                if (zako->timer < 0) {
                    zako->timer = -1;
                    zako->rot.y = 0.0f;
                    if (i >= 3 && i <= 5) {
                        switch (i) {
                            case 3:
                            case 4:
                                for (j = 0, n = 0; j < 3; j++) {
                                    if (zakogun[j].state == 20) {
                                        n++;
                                    }
                                }
                                if (n == 3) {
                                    zako->state = 16;
                                }
                                break;
                            case 5:
                                for (j = 3, n = 0; j < 5; j++) {
                                    if (zakogun[j].state == 20) {
                                        n++;
                                    }
                                }
                                if (n == 2) {
                                    zako->state = 16;
                                }
                                break;
                        }
                    } else {
                        zako->state = 20;
                        zako->timer = 0;
                        zakogun_set_pose(zako, 0x15, 1, 1);
                    }
                }
                break;
            case 16:
                x = unit->homePosition.x;
                y = unit->homePosition.y;
                z = unit->homePosition.z;
                zako->start.x = zako->pos.x;
                zako->start.y = zako->pos.y;
                zako->start.z = zako->pos.z;
                switch (i) {
                    case 3:
                        zako->target.x = x - 5.0f;
                        zako->target.y = y + 10.0f;
                        zako->target.z = z - 5.0f;
                        break;
                    case 4:
                        zako->target.x = x + 5.0f;
                        zako->target.y = y + 10.0f;
                        zako->target.z = z + 5.0f;
                        break;
                    case 5:
                        zako->target.x = x;
                        zako->target.y = y + 20.0f;
                        zako->target.z = z;
                        break;
                }
                zako->state = 17;
                zako->timer = zako->timerMax = 20;
                zako->vel.y = (2.0f * (zako->target.y - zako->start.y) - -200.0f) / 40.0f;
                zakogun_set_pose(zako, 0x22, 1, 1);
                break;
            case 17:
                zako->pos.x = intplGetValue(EASING_LINEAR, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(EASING_LINEAR, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.5f;
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 20;
                    zako->timer = 0;
                    zako->pos.y = zako->target.y;
                    zakogun_set_pose(zako, 0x15, 1, 1);
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_WAKE1", 0x7f, 0, &zako->pos);
                }
                break;
            case 20:
                if (unit == NULL) {
                    break;
                }
                BtlUnit_GetPos(unit, &x, &y, &z);
                zako->timer++;
                zako->timer = zakogun[0].timer;
                if (zako->timer >= 30) {
                    zako->timer -= 30;
                }
                switch (i) {
                    case 0:
                        zako->pos.x = zako->home.x = x - 10.0f;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z - 10.0f;
                        break;
                    case 1:
                        zako->pos.x = zako->home.x = x;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z;
                        break;
                    case 2:
                        zako->pos.x = zako->home.x = x + 10.0f;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z + 10.0f;
                        break;
                    case 3:
                        zako->pos.x = zako->home.x = x - 5.0f;
                        if (zako->timer < 10) {
                            zako->pos.y = zako->home.y = y + 10.0f - 0.1f * zako->timer;
                        } else if (zako->timer < 20) {
                            zako->pos.y = zako->home.y = y + 10.0f - 1.0f;
                        } else {
                            zako->pos.y = zako->home.y = y + 10.0f - 1.0f + 0.1f * (zako->timer - 20.0f);
                        }
                        zako->pos.z = zako->home.z = z - 5.0f;
                        break;
                    case 4:
                        zako->pos.x = zako->home.x = x + 5.0f;
                        if (zako->timer < 10) {
                            zako->pos.y = zako->home.y = y + 10.0f - 0.1f * zako->timer;
                        } else if (zako->timer < 20) {
                            zako->pos.y = zako->home.y = y + 10.0f - 1.0f;
                        } else {
                            zako->pos.y = zako->home.y = y + 10.0f - 1.0f + 0.1f * (zako->timer - 20.0f);
                        }
                        zako->pos.z = zako->home.z = z + 5.0f;
                        break;
                    case 5:
                        zako->pos.x = zako->home.x = x;
                        if (zako->timer < 15) {
                            zako->pos.y = zako->home.y = y + 20.0f - 0.15f * zako->timer;
                        } else {
                            zako->pos.y = zako->home.y = y + 20.0f - 2.25f + 0.15f * (zako->timer - 15.0f);
                        }
                        zako->pos.z = zako->home.z = z;
                        break;
                }
                break;
            case 30:
                if (i >= 0 && i <= 2) {
                    zakogun_set_pose(zako, 0x14, 1, 1);
                }
                zako->state = 35;
                zako->timer = 5;
            case 35:
                if (i >= 3 && i <= 5) {
                    zako->pos.y += 3.0f;
                }
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 36;
                    zako->timer = 30;
                }
                break;
            case 36:
                zako->timer--;
                if (i >= 0 && i <= 2) {
                    if (zako->timer == 15) {
                        zakogun_set_pose(zako, 0x16, 10, 1);
                    }
                    if (zako->timer <= 15) {
                        if (zako->timer % 12 == 0) {
                            BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1L", 0x7f, 0, &zako->pos);
                        }
                        if (zako->timer % 12 == 6) {
                            BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1R", 0x7f, 0, &zako->pos);
                        }
                    }
                }
                if (zako->timer <= 0) {
                    zako->state = 40;
                }
                break;
            case 40:
                BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
                x += 100.0f;
                zako->start.x = zako->pos.x;
                zako->start.y = zako->pos.y;
                zako->start.z = zako->pos.z;
                zako->target.x = x;
                zako->target.y = zako->pos.y;
                zako->target.z = zako->pos.z;
                zako->state = 45;
                zako->timer = zako->timerMax = 40;
            case 45:
                zako->pos.x = intplGetValue(EASING_LINEAR, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                if (zako->timer % 12 == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1L", 0x7f, 0, &zako->pos);
                }
                if (zako->timer % 12 == 6) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1R", 0x7f, 0, &zako->pos);
                }
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 50;
                    zako->timer = 0;
                }
                break;
            case 50: {
                f32 ofs[6] = {-20.0f, -10.0f, 0.0f, 10.0f, 20.0f, 30.0f};

                BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
                zako->start.x = zako->pos.x;
                zako->start.y = zako->pos.y;
                zako->start.z = zako->pos.z;
                zako->target.x = x - ofs[i];
                zako->target.y = 0.0f;
                zako->target.z = zako->start.z;
                zako->state = 55;
                zako->timer = zako->timerMax = 40;
                zako->vel.y = (2.0f * (zako->target.y - zako->start.y) - -320.0f) / 80.0f;
                zakogun_set_pose(zako, 0x20, 1, 1);
                if (i == 0) {
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_ATTACK1", 0x7f, 0, &zako->pos);
                }
                break;
            }
            case 55:
                BtlUnit_GetPos(BattleGetUnitPtr(wp, work->targetId), &x, &y, &z);
                zako->pos.x = intplGetValue(EASING_LINEAR, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(EASING_LINEAR, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.2f;
                zako->timer--;
                if (zako->timer <= 10) {
                    zako->state = 58;
                    if (i == 0) {
                        // Cue the single damage check before landing; all six must later return to state 20.
                        unit->work[UNIT_WORK_HIT_CUE]++;
                        zakogun_set_pose(zako, 0, 20, 2);
                    }
                }
                break;
            case 58:
                zako->pos.x = intplGetValue(EASING_LINEAR, zako->start.x, zako->target.x, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.z = intplGetValue(EASING_LINEAR, zako->start.z, zako->target.z, zako->timerMax - zako->timer, zako->timerMax);
                zako->pos.y += zako->vel.y;
                zako->vel.y += -0.2f;
                zako->timer--;
                if (zako->timer < 0) {
                    zako->state = 60;
                    zako->timer = 30;
                    zako->pos.y = zako->target.y;
                    zakogun_set_pose(zako, 0x15, 1, 1);
                    if (i == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_ATTACK1_2", 0x7f, 0, &zako->pos);
                    }
                }
                break;
            case 60:
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->state = 62;
                    zakogun_set_pose(zako, 0x16, 10, 1);
                }
                break;
            case 62:
                zako->state = 12;
                zako->rot.y = 180.0f;
                zako->start = zako->pos;
                unit->position = unit->homePosition;
                btl_camera_set_mode(BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT);
                break;
            case 80:
                zako->rot.z = 0.0f;
                zako->state = 81;
                zako->timer = 6;
            case 81:
                if (i >= 0 && i <= 2) {
                    zako->rot.z += 15.0f;
                    if (zako->rot.z >= 45.0f) {
                        zako->rot.z = 45.0f;
                    }
                }
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->state = 82;
                    zako->timer = 10;
                    zako->vel.y = 0.0f;
                    if (i >= 0 && i <= 2) {
                        zako->rot.z = 45.0f;
                    }
                }
                break;
            case 82:
                if (i >= 3 && i <= 4) {
                    zako->rot.z += 15.0f;
                    if (zako->rot.z >= 45.0f) {
                        zako->rot.z = 45.0f;
                    }
                    zako->pos.y += zako->vel.y;
                    zako->vel.y += -0.5f;
                    if (zako->pos.y < 0.0f) {
                        zako->pos.y = 0.0f;
                    }
                }
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->state = 83;
                    zako->timer = 20;
                    zako->vel.y = 0.0f;
                    if (i >= 3 && i <= 4) {
                        zako->rot.z = 45.0f;
                        zako->pos.y = 0.0f;
                    }
                }
                break;
            case 83:
                if (i == 5) {
                    zako->rot.z += 15.0f;
                    if (zako->rot.z >= 45.0f) {
                        zako->rot.z = 45.0f;
                    }
                    zako->pos.y += zako->vel.y;
                    zako->vel.y += -0.5f;
                    if (zako->pos.y < 0.0f) {
                        zako->pos.y = 0.0f;
                    }
                }
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->rot.y = 0.0f;
                    zako->state = 84;
                    zako->timer = 48;
                    if (i == 5) {
                        zako->rot.z = 45.0f;
                        zako->pos.y = 0.0f;
                    }
                }
                break;
            case 84:
                if (zako->timer <= 48) {
                    zako->rot.y -= 15.0f;
                }
                zako->timer--;
                if (zako->timer <= 0) {
                    zako->rot.y = 0.0f;
                    zako->state = 85;
                }
                break;
            case 85:
                zako->rot.x += 3.0f;
                if (zako->rot.x >= 90.0f) {
                    zako->state = -1;
                    zako->pos = (Vec){0.0f, -1000.0f, 0.0f};
                }
                break;
            case 90:
                zako->pos.x += 5.0f;
                if (i == 0) {
                    if (gp->retraceCount % 12 == 0) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1L", 0x7f, 0, &zako->pos);
                    }
                    if (gp->retraceCount % 12 == 6) {
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_TEAM_3RD_MOVE1R", 0x7f, 0, &zako->pos);
                    }
                }
                if (zako->pos.x >= 300.0f) {
                    zako->state = -1;
                }
                break;
            case 95:
            case 96:
                d = 0.0f;
                BtlUnit_GetPos(unit, &x, &y, &z);
                if (zako->state == 96) {
                    zako->timer++;
                    zako->timer = zakogun[0].timer;
                    if ((zako->timer & 3) == 0 || (zako->timer & 3) == 1) {
                        d = 1.0f;
                    } else {
                        d = -1.0f;
                    }
                }
                switch (i) {
                    case 0:
                        zako->pos.x = zako->home.x = x - 10.0f + d;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z - 10.0f;
                        break;
                    case 1:
                        zako->pos.x = zako->home.x = x + d;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z;
                        break;
                    case 2:
                        zako->pos.x = zako->home.x = x + 10.0f + d;
                        zako->pos.y = zako->home.y = y;
                        zako->pos.z = zako->home.z = z + 10.0f;
                        break;
                    case 3:
                        zako->pos.x = zako->home.x = x - 5.0f + d;
                        zako->pos.y = zako->home.y = y + 20.0f;
                        zako->pos.z = zako->home.z = z - 5.0f;
                        break;
                    case 4:
                        zako->pos.x = zako->home.x = x + 5.0f + d;
                        zako->pos.y = zako->home.y = y + 20.0f;
                        zako->pos.z = zako->home.z = z + 5.0f;
                        break;
                    case 5:
                        zako->pos.x = zako->home.x = x + d;
                        zako->pos.y = zako->home.y = y + 40.0f;
                        zako->pos.z = zako->home.z = z;
                        break;
                }
                break;
        }
        PSMTXIdentity(*pose->mtx);
        PSMTXRotRad(m, 'x', 0.017453292f * zako->rot.x);
        PSMTXConcat(*pose->mtx, m, *pose->mtx);
        PSMTXRotRad(m, 'y', 0.017453292f * zako->rot.y);
        PSMTXConcat(*pose->mtx, m, *pose->mtx);
        PSMTXRotRad(m, 'z', 0.017453292f * zako->rot.z);
        PSMTXConcat(*pose->mtx, m, *pose->mtx);
        (*pose->mtx)[0][3] = zako->pos.x;
        (*pose->mtx)[1][3] = zako->pos.y;
        (*pose->mtx)[2][3] = zako->pos.z + i * 0.25f;
    }
}

/** Script helper for group 1 attack: sets all six X-Nauts to attack state 30 and blocks until they finish attacking. */
USER_FUNC(zakogun_attack1) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        for (i = 0; i < 6; i++) {
            zakogun[i].state = 30;
        }
    }
    for (i = 0, n = 0; i < 6; i++) {
        if (zakogun[i].state == 20) {
            n++;
        }
    }
    return n == 6 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/** Script helper: puts the six X-Nauts of group 1 into their death state (80). */
USER_FUNC(zakogun_dead1) {
    s32 i;

    for (i = 0; i < 6; i++) {
        zakogun[i].state = 80;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: sends the six X-Nauts of group 1 running away (state 90, escape animation);
 * blocks until they have left.
 */
USER_FUNC(zakogun_escape1) {
    s32 i;
    s32 n;

    if (isFirstCall) {
        for (i = 0; i < 6; i++) {
            ZakoGun* zako = &zakogun[i];
            zako->state = 90;
            zakogun_set_pose(zako, 0x16, 10, 1);
        }
    }
    for (i = 0, n = 0; i < 6; i++) {
        if (zakogun[i].state == -1) {
            n++;
        }
    }
    return n == 6 ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/**
 * Script helper (sukima, an opening): holds each X-Naut of group 1 whose state is outside 80 to 89 still at its formation spot,
 * shaking it from side to side (state 96) when args[0] is 1 or just holding it (state 95) otherwise.
 */
static USER_FUNC(zakogun_start_sukima) {
    s32 mode = evtGetValue(event, event->args[0]);
    s32 i;

    for (i = 0; i < 6; i++) {
        ZakoGun* zako = &zakogun[i];
        if (zako->state < 80 || zako->state >= 90) {
            if (mode == 1) {
                zako->state = 96;
                zako->timer = 0;
            } else {
                zako->state = 95;
            }
        }
    }
    return EVT_RETURN_DONE;
}

/** Script helper: returns each X-Naut of group 1 whose state is outside 80 to 89 to the normal formation state (20). */
static USER_FUNC(zakogun_end_sukima) {
    s32 i;

    for (i = 0; i < 6; i++) {
        ZakoGun* zako = &zakogun[i];
        if (zako->state < 80 || zako->state >= 90) {
            zako->state = 20;
        }
    }
    return EVT_RETURN_DONE;
}

ZakoGunWork* work;
ZakoGun* zakogun;
