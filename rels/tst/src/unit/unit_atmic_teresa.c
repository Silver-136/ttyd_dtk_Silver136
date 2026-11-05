// Battle unit Atomic Boo (`atmic_teresa`): tst.rel's copy of rels/jin/src/unit/unit_atmic_teresa.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "party/party_motion.h"
#include "evt/evt_cmd.h"

#include "battle/battle.h"
#include "driver/camdrv.h"
#include "battle/battle_unit.h"
#include "driver/dispdrv.h"
#include "dolphin/mtx.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "driver/extdrv.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_sub.h"
#include "evt/evt_ext.h"
#include "evt/evt_npc.h"
#include "evt/evt_sub.h"
#include "system.h"
#include "libc_decls.h"
#include "battle/battle_damage.h"
#include "battle/unit/unit_atmic_teresa.h"

AtmTeresa* teresa;
#define atmic_teresa_scale atmic_teresa_scale
s32 teresa_count;
void teresa_ext_disp(CameraId camera, void* param);

USER_FUNC(atmic_teresa_id_set);
USER_FUNC(teresa_wait_breath2);
USER_FUNC(teresa_wait_breath);
USER_FUNC(teresa_appear_breath);
USER_FUNC(teresa_wait_bikkuri2);
USER_FUNC(teresa_wait_bikkuri);
USER_FUNC(teresa_appear_bikkuri);
USER_FUNC(target_id_set);
void teresa_ext_disp(CameraId camera, void* param);
void teresa_ext_main(void);
void teresa_ext_init(void);

BattleUnitKind unit_atmic_teresa;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static PoseTableEntry pose_table[17];
PoseTableEntry pose_table_kaokakusi[17];
static BattleDataEntry data_table[1];
static BattleUnitKindPart parts[1];
static StatusVulnerability regist;
BattleWeapon weapon_atmic_teresa_press;
BattleWeapon weapon_atmic_teresa_bikkuri;
BattleWeapon weapon_atmic_teresa_surprise;
BattleWeapon weapon_atmic_teresa_breath;
static EvtWord init_event[26];
static EvtWord damage_event[10];
static EvtWord attack_event[101];
extern EvtWord kaokakusi_event[];
extern EvtWord press_event[];
extern EvtWord surprise_event[];
extern EvtWord bikkuri_event[];
static EvtWord breath_event[254];
extern EvtWord breath_event_sub[];
static EvtWord wait_event[6];
s32 atmic_teresa_id[1];
s32 target_unit_id[1];
s32 target_parts_id[1];
f32 atmic_teresa_scale;
ExtTexInfo teresa_ext_entry_data[17];
extern EvtWord teresa_entry[];

/**
 * Unit data of the Atomic Boo: hit points, level, size, sounds, parts, status vulnerability, init event
 * and event data table.
 */
BattleUnitKind unit_atmic_teresa = {
    UNIT_ATOMIC_BOO,
    "btl_un_atmic_teresa",
    40,
    0,
    1,
    1,
    52,
    0,
    1,
    100,
    0,
    50,
#if VERSION >= VERSION_US
    6,
#else
    9999,
#endif
    120,
    110,
    30,
    105,
    0.0f,
    55.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    60.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    60.0f,
    0.0f,
    0.0f,
    46.0f,
    71.5f,
    10.0f,
    0.0f,
    55.0f,
    0.0f,
    120.0f,
    110.0f,
    0,
    0,
    0,
    2,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_ATMTLSA_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    UNIT_ATTR_FLOATING,
    &regist,
    1,
    parts,
    init_event,
    data_table,
};

/**
 * Defense of the Atomic Boo against normal, fire, ice, explosion and electric attacks: none, so it takes
 * full damage from every element.
 */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Damage-type reactions of the Atomic Boo: none, so every element hurts it normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Pose table of the Atomic Boo: maps each battle pose (idle, hurt, attacking and so on) to its animation. */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "N_1"},
    {POSE_TYPE_STOP, "Y_1"},
    {POSE_TYPE_FREEZE, "Y_1"},
    {POSE_TYPE_CONFUSE, "K_1"},
    {POSE_TYPE_POISON, "X_1"},
    {POSE_TYPE_DIZZY, "X_1"},
    {BTL_POSE_NORMAL, "S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "S_1"},
    {BTL_POSE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "R_1"},
    {POSE_TYPE_WALK, "W_1"},
    {POSE_TYPE_DANGER, "X_1"},
    {POSE_TYPE_PERIL, "X_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "S_1"},
};

/** Pose table of the Atomic Boo while it covers its face with its hands. */
PoseTableEntry pose_table_kaokakusi[] = {
    {POSE_TYPE_SLEEP, "A_1"},
    {POSE_TYPE_STOP, "A_1"},
    {POSE_TYPE_FREEZE, "A_1"},
    {POSE_TYPE_CONFUSE, "A_1"},
    {POSE_TYPE_POISON, "A_1"},
    {POSE_TYPE_DIZZY, "A_1"},
    {BTL_POSE_NORMAL, "A_1"},
    {POSE_TYPE_DAMAGE_FIRE, "Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "A_1"},
    {BTL_POSE_DAMAGE, "D_1"},
    {POSE_TYPE_RUN, "A_1"},
    {POSE_TYPE_WALK, "A_1"},
    {POSE_TYPE_DANGER, "A_1"},
    {POSE_TYPE_PERIL, "A_1"},
    {POSE_TYPE_TALK, "T_1"},
    {POSE_TYPE_ACTION, "A_1"},
};

/**
 * Event data table of the Atomic Boo: no custom reactions, so the standard battle handlers run for every
 * event.
 */
static BattleDataEntry data_table[] = {
    {0, NULL},
};

/**
 * Parts of the Atomic Boo: a single body part (part 1, model c_atmic_trs), each with its defense tables,
 * attributes and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_atmic_teresa", "c_atmic_trs", 0.0f, 0.0f, 0.0f, 10.0f, 80.0f, 0.0f, 0.0f, 110.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET, 0, pose_table},
};

#if VERSION <= VERSION_DEMO_JP

/**
 * Status vulnerability of the Atomic Boo (the early demos), as percent of each status's normal chance:
 * immune to Huge, Tiny, Attack Down and Fright; resists 12 statuses (down to 5% of the normal chance).
 */
static StatusVulnerability regist = {40, 40, 40, 40, 40, 40, 100, 30, 0, 0, 100, 0, 100, 40, 40, 0, 10, 100, 40, 100, 100, 5};
#else

/**
 * Status vulnerability of the Atomic Boo (JP and later), as percent of each status's normal
 * chance: immune to Huge, Tiny, Attack Down and Fright; resists 10 statuses (down to 5% of the normal
 * chance).
 */
static StatusVulnerability regist = {40, 40, 40, 100, 40, 100, 100, 30, 0, 0, 100, 0, 100, 80, 40, 0, 10, 100, 40, 100, 100, 5};
#endif

/** Weapon record of the Atomic Boo's Press: 4 damage and can hit several targets. */
BattleWeapon weapon_atmic_teresa_press = {
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
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_SHATTERS_FREEZE | SPECIAL_FLIP_SHELLED, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 50, 50, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Weapon record of the Atomic Boo's split-up attack (bikkuri, Japanese for surprise, in the code):
 * 4 damage, can hit several targets and ignores Defense.
 */
BattleWeapon weapon_atmic_teresa_bikkuri = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
#if VERSION <= VERSION_DEMO_JP
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY,
#else
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_PIERCE_DEFENSE, // special
#endif
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, // targetWeightFlags..sleepTime
    50, // stopChance
    3, // stopTime
    0, 0, 0, 0, 0, 0, 0, 0, // dizzyChance..electricChance
    0, 0, 0, 0, 0, 0, 0, 0, // electricTime..sizeChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeTime..defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Weapon record of the Atomic Boo's surprise attack (surprise_event): 4 damage, can hit several targets and
 * ignores Defense.
 */
BattleWeapon weapon_atmic_teresa_surprise = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
#if VERSION <= VERSION_DEMO_JP
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_DIMINISH_BY_TARGET | SPECIAL_PIERCE_DEFENSE | SPECIAL_IGNORE_VULNERABILITY,
#else
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_DIMINISH_BY_TARGET | SPECIAL_PIERCE_DEFENSE, // special
#endif
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, // poisonTime..poisonStrength
    50, // confuseChance
    3, // confuseTime
    0, 0, 0, 0, 0, 0, 0, 0, // electricChance..freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon record of the Atomic Boo's breath attack: 3 damage and can hit several targets. */
BattleWeapon weapon_atmic_teresa_breath = {
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
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGE_BUFFABLE | SPECIAL_DIMINISH_BY_HIT | SPECIAL_SHATTERS_FREEZE, // special
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

/**
 * Init script of the Atomic Boo boss: registers its wait,
 * attack and damage handlers (confusion uses the default handler),
 * stores its own unit id (atmic_teresa_id_set),
 * builds the flock of small Boos (teresa_entry) and starts the idle event.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(atmic_teresa_id_set, UNIT_ID_SELF)
    CALL_CHILD(teresa_entry)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage reaction: runs the default damage handling (btldefaultevt_Damage) with LW(10) = -2 (the unit itself)
 * and LW(11) = 1.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Chooses the Atomic Boo's attack: item use first;
 * while its face is visible (unit work 0 = 0) it covers its face 30 times in 70,
 * uses the Press 20 in 70 and the split-up attack (bikkuri_event) 20 in 70;
 * while its face is covered (unit work 0 = 1) it picks the surprise attack (surprise_event)
 * or the Breath (breath_event) with equal chance.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_FACE_HIDDEN, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(1), 30)
        ADD(LW(1), 20)
        ADD(LW(1), 20)
        SUBTRACT(LW(1), 1)
        CALL(evt_sub_random, LW(1), LW(0))
        SET_VALUE(LW(1), 30)
        IF_INT_LESS(LW(0), LW(1))
            CALL_CHILD(kaokakusi_event)
            GOTO(99)
        END_IF
        ADD(LW(1), 20)
        IF_INT_LESS(LW(0), LW(1))
            CALL_CHILD(press_event)
            GOTO(99)
        END_IF
        CALL_CHILD(bikkuri_event)
        GOTO(99)
    ELSE
        SET_VALUE(LW(1), 50)
        ADD(LW(1), 50)
        SUBTRACT(LW(1), 1)
        CALL(evt_sub_random, LW(1), LW(0))
        SET_VALUE(LW(1), 50)
        IF_INT_LESS(LW(0), LW(1))
            CALL_CHILD(surprise_event)
            GOTO(99)
        END_IF
        CALL_CHILD(breath_event)
        GOTO(99)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * The Atomic Boo covers its face ('kaokakusi'), one of its turn actions (30 in 70 while its face shows):
 * it dives 60 toward the party (pose type 40, SFX_BOSS_ATMTLSA_MOVE2), turns its back (A_1,
 * SFX_BOSS_ATMTLSA_FACE_HIDE1), glides home while spinning back round, then sets unit work 0 to 1 and the
 * hands-over-face pose table.
 */
EVT_BEGIN(kaokakusi_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_MOVE2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 60)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 10, EASING_QUADRATIC_OUT, 0, -1)
    WAIT_FRAMES(20)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_FACE_HIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_1"))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, 0, EASING_CUBIC_OUT, 0, -1)
    WAIT_FRAMES(10)
    LOOP(15)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 1, 0, 12, 0)
    END_LOOP
    WAIT_FRAMES(20)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FACE_HIDDEN, 1)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_kaokakusi))
    RETURN
EVT_END()

/**
 * The Atomic Boo's Press: it declares an attack on every target, flies to x -125 at height 100 (R_1) and drops
 * onto the ground there (Z_1) with a long camera shake.
 * Each target in turn dodges or takes weapon_atmic_teresa_press (the guard result is applied on the first target
 * only, if it is hit); then it floats home to its waiting pose.
 */
EVT_BEGIN(press_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_atmic_teresa_press))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_atmic_teresa_press), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_atmic_teresa_press))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_atmic_teresa_press))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_MOVE2"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("R_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -125, 100, LW(2), 120, 10, EASING_QUADRATIC_OUT, 0, -1)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("Z_1"))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -125, 0, LW(2), 30, 0, EASING_QUADRATIC_IN, 0, -1)
    CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_press), 0, LW(5))
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
        CASE_END
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
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_press))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_press), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(0)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    LABEL(98)
    WAIT_FRAMES(60)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("W_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 50, EASING_QUARTIC_OUT, 0, -1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    WAIT_MS(250)
    LABEL(99)
    RETURN
EVT_END()

/**
 * The Atomic Boo's surprise attack (surprise_event), used while its face is covered:
 * it turns its back and backs off, uncovers its face
 * (unit work 0 = 0, normal pose table), lunges to x 0 in pose A_2 with SFX_BOSS_ATMTLSA_SURPRISED1 and a strong
 * camera shake, and each target dodges or takes weapon_atmic_teresa_surprise (the guard result on the first target
 * only, if it is hit).
 * Then it floats home.
 */
EVT_BEGIN(surprise_event)

    // The Atomic Boo picks its targets for the surprise attack, which hits the whole party.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_atmic_teresa_surprise))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_atmic_teresa_surprise), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_atmic_teresa_surprise))

    // The attack is announced.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_atmic_teresa_surprise))

    // The Atomic Boo turns its back to the party, still covering its face, and drifts away.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_FACE_HIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 10, EASING_QUADRATIC_OUT, 0, -1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FACE_HIDDEN, 0)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_2"))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, 0, LW(1), LW(2), 10, 0, EASING_CUBIC_OUT, 0, -1)

    // It spins around, uncovers its face and lunges at the party with a strong camera shake.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_SURPRISED1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_w, 0, 10, 0, 60, 0)
    CALL(evt_btl_camera_shake_h, 0, 10, 0, 60, 4)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)

    // For each target in turn, find out how it reacts: it dodges or takes the hit.
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_surprise), 0, LW(5))
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
        CASE_END
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
    GOTO(97)

    // The surprise attack lands: the damage is applied (the guard result only counts on the first target).
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_surprise))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_surprise), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)

    // Move on to the next target, if there is one.
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(0)
    END_IF
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)

    // The Atomic Boo floats back to its home position.
    LABEL(98)
    WAIT_FRAMES(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("W_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -20, EASING_QUARTIC_OUT, 0, -1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    WAIT_MS(250)
    LABEL(99)
    RETURN
EVT_END()

/**
 * The Atomic Boo's split-up attack (bikkuri_event): eight small Boos split off it
 * (it shrinks a little for each)
 * and circle the party's side of the stage (teresa_appear_bikkuri, teresa_wait_bikkuri), then cry out
 * (SFX_BOSS_ATMTLSA_SURPRISED2) and each target dodges or takes weapon_atmic_teresa_bikkuri.
 * It whistles them back (SFX_BOSS_ATMTLSA_WHISTLE1) and they merge into it in puffs of smoke
 * (SFX_BOSS_ATMTLSA_UNION1), restoring its size.
 */
EVT_BEGIN(bikkuri_event)

    // The Atomic Boo picks its targets for the split-up attack, which hits the whole party.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_atmic_teresa_bikkuri))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_atmic_teresa_bikkuri), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_atmic_teresa_bikkuri))

    // The attack is announced.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_atmic_teresa_press))

    // Small Boos split off the Atomic Boo and circle the party's side of the stage.
    CALL(teresa_appear_bikkuri)
    WAIT_FRAMES(75)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, LW(3), -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 5)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_S_ROUND1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(teresa_wait_bikkuri)
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_FRAMES(30)

    // The Atomic Boo cries out.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_SURPRISED2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)

    // For each target in turn, find out how it reacts: it dodges or takes the hit.
    LABEL(0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_bikkuri), 0, LW(5))
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
    GOTO(97)

    // The split-up attack lands: the damage is applied.
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_bikkuri))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_bikkuri), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)

    // Move on to the next target, if there is one.
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(0)
    END_IF
    LABEL(98)
    WAIT_FRAMES(30)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(60)

    // The Atomic Boo whistles the small Boos back and they merge into it in puffs of smoke.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_WHISTLE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(teresa_wait_bikkuri2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_UNION1"), EVT_NULLPTR, 0, LW(15))
    LOOP(3)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        ADD(LW(2), 1)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 8, LW(0), LW(1), LW(2), FLOAT(1.25f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(30)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(250)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    LABEL(99)
    RETURN
EVT_END()

/**
 * The Atomic Boo's breath, used while its face is covered: it turns its back, backs off and uncovers its face
 * (unit work 0 = 0, pose A_3), returns home and blows (SFX_BOSS_ATMTLSA_BLOW2) a swarm of 30 small Boos at the
 * targets (teresa_appear_breath), with breath_event_sub resolving each target.
 * It then whistles them back (SFX_BOSS_ATMTLSA_WHISTLE1) and they merge into it in puffs of smoke
 * (SFX_BOSS_ATMTLSA_UNION1), restoring its size.
 */
static EVT_BEGIN(breath_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_atmic_teresa_breath), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(target_id_set, LW(3), LW(4))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_FACE_HIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 180, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 30)
    SUBTRACT(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 10, EASING_QUADRATIC_OUT, 0, -1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FACE_HIDDEN, 0)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_3"))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 1, 0, 0, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 10, 0, EASING_CUBIC_OUT, 0, -1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_BLOW2"), EVT_NULLPTR, 0, LW(15))
    CALL(teresa_appear_breath, 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(20)
    END_IF
    SPAWN_EVENT(breath_event_sub)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(10)
    LABEL(20)
    WAIT_FRAMES(120)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(teresa_wait_breath)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_WHISTLE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("A_4"))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("S_1"))
    CALL(teresa_wait_breath2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_ATMTLSA_UNION1"), EVT_NULLPTR, 0, LW(15))
    LOOP(3)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        ADD(LW(2), 1)
        CALL(evt_eff, STRING(""), STRING("kemuri_test"), 8, LW(0), LW(1), LW(2), FLOAT(1.25f), 0, 0, 0, 0, 0, 0, 0)
        WAIT_FRAMES(30)
    END_LOOP
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(1500)
    CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    RETURN
EVT_END()

/**
 * One target of Atomic Boo's breath attack: runs the pre-damage check,
 * then deals three ticks of weapon_atmic_teresa_breath damage 15, 30 and 30 frames apart,
 * each with its own action-command check, or plays the avoid reaction.
 */
EVT_BEGIN(breath_event_sub)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_breath), 0, LW(5))
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
    GOTO(99)
    LABEL(91)
    WAIT_FRAMES(15)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_breath), 0, LW(5))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_breath), 0, LW(5))
    WAIT_FRAMES(30)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_atmic_teresa_breath))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_atmic_teresa_breath), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(99)
    RETURN
EVT_END()

/** Idle event: sets the unit's body part 1 to the pose given by its pose table for its current state. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/** Unit id of the Atomic Boo, stored during initialization so that the small Boos' code can find it. */
s32 atmic_teresa_id[] = {
    -1,
};

/** Unit id of the target of the Atomic Boo's current attack, which the breath's small Boos fly toward. */
s32 target_unit_id[] = {
    -1,
};

/** Part id of the target of the Atomic Boo's current attack, which the breath's small Boos fly toward. */
s32 target_parts_id[] = {
    -1,
};

/**
 * Body scale the Atomic Boo is meant to have; teresa_ext_main eases it back toward this value as small
 * Boos split off and return.
 */
f32 atmic_teresa_scale = 1.0f;

/**
 * Sprite frames of the small Atomic Boos (model c_teresa), picked by each pose's texId: 0-7 cycle the S_1
 * animation, 8-15 show A_1 and then A_2 frames (the split-up attack's animation).
 */
ExtTexInfo teresa_ext_entry_data[] = {
    {"c_teresa", "S_1", 0.0f},
    {"c_teresa", "S_1", 2.0f},
    {"c_teresa", "S_1", 4.0f},
    {"c_teresa", "S_1", 6.0f},
    {"c_teresa", "S_1", 8.0f},
    {"c_teresa", "S_1", 10.0f},
    {"c_teresa", "S_1", 6.0f},
    {"c_teresa", "S_1", 7.0f},
    {"c_teresa", "A_1", 0.0f},
    {"c_teresa", "A_2", 6.0f},
    {"c_teresa", "A_2", 7.0f},
    {"c_teresa", "A_2", 8.0f},
    {"c_teresa", "A_2", 9.0f},
    {"c_teresa", "A_2", 10.0f},
    {"c_teresa", "A_2", 11.0f},
    {"c_teresa", "A_2", 12.0f},
    {0},
};

/**
 * Creates the flock of small Atomic Boos used by the boss's attacks:
 * adds the hidden NPC model ext_teresa (c_teresa) parked at y = -1000
 * and registers a 30-pose extdrv object driven by teresa_ext_init and teresa_ext_main.
 */
EVT_BEGIN(teresa_entry)
    CALL(evt_npc_entry, STRING("ext_teresa"), STRING("c_teresa"))
    CALL(evt_npc_set_position, STRING("ext_teresa"), 0, -1000, 0)
    CALL(evt_ext_entry, 30, PTR(teresa_ext_entry_data), PTR(teresa_ext_init), PTR(teresa_ext_main), 0)
    RETURN
EVT_END()

/**
 * Initialises the 30 small Atomic Boos: allocates their AtmTeresa state records
 * and parks every pose off screen at (0, -1000, 0), facing 90 degrees, in state 0.
 */
void teresa_ext_init(void) {
    s32 i;
    ExtdrvPose* pose = extGetPosePtr();

    teresa = BattleAlloc(sizeof(AtmTeresa) * 30);
    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        pose->flags = EXT_POSE_FLAG_VISIBLE;
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = 0.0f;
        (*pose->mtx)[1][3] = -1000.0f;
        (*pose->mtx)[2][3] = 0.0f;
        pose->dir = 90.0f;
        teresa[i].state = 0;
        teresa[i].animType = 0;
        teresa[i].animFrame = 0;
        {
            Vec zero = {0.0f, 0.0f, 0.0f};
            teresa[i].vel = zero;
        }
    }
}

/**
 * Per-frame update of the 30 small Atomic Boos (extdrv poses), one state machine each.
 * Split-up attack, bikkuri (states 1 to 31, first eight Boos):
 * they split off the Atomic Boo (shrinking it by 0.05 each), circle
 * at radius 90 around x -100, play an animation together for 180 frames, fly back to random spots
 * around the Atomic Boo and shrink away.
 * Breath (51 to 71): they burst out of the Atomic Boo (shrinking it by 0.015 each) along random easing curves
 * to points around 200 units past the target's hit position, then fly back around it and shrink away.
 * It then queues teresa_ext_disp.
 */
void teresa_ext_main(void) {
    s32 i;
    s32 j;
    ExtdrvPose* pose = extGetPosePtr();
    AtmTeresa* t;
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, atmic_teresa_id[0]);
    BattleUnit* target = BattleGetUnitPtr(_battleWorkPointer, target_unit_id[0]);
    s32 n;
    f32 angle;
    s32 waits[8] = {0, 2, 5, 7, 10, 12, 15, 17};
    f32 ofs[30][2] = {
        {-30.0f, 8.0f},
        {-28.0f, 10.0f},
        {-26.0f, 6.0f},
        {-24.0f, 30.0f},
        {-22.0f, 16.0f},
        {-20.0f, 50.0f},
        {-18.0f, 14.0f},
        {-16.0f, 18.0f},
        {-14.0f, 60.0f},
        {-12.0f, 34.0f},
        {-10.0f, 66.0f},
        {-8.0f, 8.0f},
        {-6.0f, 28.0f},
        {-4.0f, 40.0f},
        {-2.0f, 46.0f},
        {0.0f, 20.0f},
        {2.0f, 26.0f},
        {4.0f, 52.0f},
        {6.0f, 74.0f},
        {8.0f, 68.0f},
        {10.0f, 0.0f},
        {12.0f, 28.0f},
        {14.0f, 44.0f},
        {16.0f, 72.0f},
        {18.0f, 42.0f},
        {20.0f, 76.0f},
        {22.0f, 56.0f},
        {24.0f, 32.0f},
        {26.0f, 4.0f},
        {28.0f, 2.0f},
    };
    u8 modes[7] = {0, 1, 2, 3, 4, 5, 6};
    f32 x, y, z;
    f32 hx, hy, hz;
    f32 sx, sy, sz;

    if (++teresa_count >= 360) {
        teresa_count -= 360;
    }
    if (unit != NULL) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        y += 40.0f;
        BtlUnit_GetScale(unit, &sx, &sy, &sz);
        if (sx > atmic_teresa_scale) {
            sx -= 0.005f;
            if (sx <= atmic_teresa_scale) {
                sx = atmic_teresa_scale;
            }
            BtlUnit_SetScale(unit, sx, sx, sz);
        }
        if (sx < atmic_teresa_scale) {
            sx += 0.005f;
            if (sx >= atmic_teresa_scale) {
                sx = atmic_teresa_scale;
            }
            BtlUnit_SetScale(unit, sx, sx, sz);
        }
    }
    if (target != NULL) {
        BattleUnitPart* part = BtlUnit_GetPartsPtr(target, target_parts_id[0]);
        BtlUnit_GetHitPos(target, part, &hx, &hy, &hz);
    }

    for (i = 0; i < extGetPoseNum(); i++, pose++) {
        t = &teresa[i];
        switch (t->animType) {
            case 1:
                t->animFrame++;
                if (t->animFrame <= 30) {
                    pose->texId = 8;
                } else if (t->animFrame < 44) {
                    pose->texId = (t->animFrame - 30) / 2 + 9;
                } else {
                    pose->texId = 15;
                }
                break;
            default:
                t->animFrame++;
                if (t->animFrame >= 24) {
                    t->animFrame -= 24;
                }
                pose->texId = t->animFrame / 3;
                break;
        }
        if (t->wait >= 1) {
            t->wait--;
            continue;
        }
        (*pose->mtx)[0][3] += t->vel.x;
        (*pose->mtx)[1][3] += t->vel.y;
        (*pose->mtx)[2][3] += t->vel.z;
        if ((*pose->mtx)[1][3] < 0.0f) {
            (*pose->mtx)[1][3] = 0.0f;
        }

        switch (t->state) {
            case 0:
                (*pose->mtx)[0][3] = 0.0f;
                (*pose->mtx)[1][3] = -1000.0f;
                (*pose->mtx)[2][3] = 0.0f;
                pose->dir = 90.0f;
                (*pose->mtx)[1][1] = 1.0f;
                (*pose->mtx)[0][0] = 1.0f;
                break;
            case 1:
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                (*pose->mtx)[0][3] = x;
                (*pose->mtx)[1][3] = y;
                (*pose->mtx)[2][3] = z - 5.0f;
                (*pose->mtx)[0][0] = 0.0f;
                (*pose->mtx)[1][1] = 0.0f;
                angle = 0.785398f * i;
                t->vel.x = 2.0f * (f32)cos(angle);
                t->vel.y = 2.0f * (f32)sin(angle);
                t->vel.z = 0.0f;
                t->wait = waits[i];
                t->timer = 50;
                t->state = 5;
                break;
            case 5:
                t->timer--;
                if (t->timer == 25) {
                    atmic_teresa_scale -= 0.05f;
                    if (i % 2 == 0) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = (*pose->mtx)[0][3];
                        pos.y = (*pose->mtx)[1][3];
                        pos.z = (*pose->mtx)[2][3];
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_ATMTLSA_DIVIDE1", 0xF1194D80, 0, &pos);
                    }
                }
                (*pose->mtx)[0][0] += 0.02f;
                (*pose->mtx)[1][1] += 0.02f;
                if (t->timer <= 0) {
                    pose->flags |= EXT_POSE_FLAG_BILLBOARD;
                    t->state = 10;
                    t->timer = 150;
                    t->vel.x = 0.0f;
                    t->vel.y = 0.0f;
                    t->vel.z = 0.0f;
                    if (i % 2 == 0) {
                        Vec pos = {0.0f, 0.0f, 0.0f};
                        pos.x = (*pose->mtx)[0][3];
                        pos.y = (*pose->mtx)[1][3];
                        pos.z = (*pose->mtx)[2][3];
                        BtlUnit_snd_se_pos(unit, "SFX_BOSS_ATMTLSA_S_MOVE1", 0xF1194D80, 0, &pos);
                    }
                }
                break;
            case 10:
                angle = 6.283184f * (0.125f * i + 2.0f * teresa_count / 360.0f);
                t->target.x = 90.0f * (f32)cos(angle) - 100.0f;
                t->target.y = 10.0f;
                t->target.z = 90.0f * (f32)sin(angle);
                if ((*pose->mtx)[0][3] >= -100.0f) {
                    pose->dir = 90.0f;
                } else {
                    pose->dir = 270.0f;
                }
                t->vel.x = (t->target.x - (*pose->mtx)[0][3]) / 20.0f;
                t->vel.y = (t->target.y - (*pose->mtx)[1][3]) / 20.0f;
                t->vel.z = (t->target.z - (*pose->mtx)[2][3]) / 20.0f;
                if (t->timer <= 0) {
                    t->state = 15;
                    t->vel.x = 0.0f;
                    t->vel.y = 0.0f;
                    t->vel.z = 0.0f;
                }
                t->timer--;
                break;
            case 15:
                angle = 6.283184f * (0.125f * i + 2.0f * teresa_count / 360.0f);
                t->target.x = 90.0f * (f32)cos(angle) - 100.0f;
                t->target.y = 10.0f;
                t->target.z = 90.0f * (f32)sin(angle);
                if ((*pose->mtx)[0][3] >= -100.0f) {
                    pose->dir = 90.0f;
                } else {
                    pose->dir = 270.0f;
                }
                t->vel.x = (t->target.x - (*pose->mtx)[0][3]) / 20.0f;
                t->vel.y = (t->target.y - (*pose->mtx)[1][3]) / 20.0f;
                t->vel.z = (t->target.z - (*pose->mtx)[2][3]) / 20.0f;
                n = 0;
                for (j = 0; j < 8; j++) {
                    if (teresa[j].state != 15) {
                        break;
                    }
                    n++;
                }
                if (n == 8) {
                    for (j = 0; j < 8; j++) {
                        AtmTeresa* p = &teresa[j];
                        p->state = 20;
                        p->animType = 1;
                        p->animFrame = 0;
                        p->vel.x = 0.0f;
                        p->vel.y = 0.0f;
                        p->vel.z = 0.0f;
                    }
                }
                break;
            case 20:
                if (t->animFrame >= 180) {
                    t->state = 23;
                    t->animType = 0;
                    t->animFrame = 0;
                    t->timer = 60;
                    t->wait = waits[i];
                    pose->dir = 90.0f;
                }
                break;
            case 23:
                t->target.x = (x - 35.0f) + irand(90);
                t->target.y = (y - 35.0f) + irand(90);
                t->target.z = (z - 30.0f) - 0.05f * i;
                t->timer = 60;
                t->state = 25;
                if (i % 2 == 0) {
                    Vec pos = {0.0f, 0.0f, 0.0f};
                    pos.x = (*pose->mtx)[0][3];
                    pos.y = (*pose->mtx)[1][3];
                    pos.z = (*pose->mtx)[2][3];
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_ATMTLSA_S_MOVE1", 0xF1194D80, 0, &pos);
                }
                break;
            case 25:
                t->vel.x = (t->target.x - (*pose->mtx)[0][3]) / 20.0f;
                t->vel.y = (t->target.y - (*pose->mtx)[1][3]) / 20.0f;
                t->vel.z = (t->target.z - (*pose->mtx)[2][3]) / 20.0f;
                if (t->timer <= 0) {
                    t->state = 30;
                    t->vel.x = 0.0f;
                    t->vel.y = 0.0f;
                    t->vel.z = 0.0f;
                    pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                }
                t->timer--;
                break;
            case 31:
                (*pose->mtx)[0][0] -= 0.0125f;
                (*pose->mtx)[1][1] = (*pose->mtx)[0][0];
                if ((*pose->mtx)[0][0] <= 0.0f) {
                    t->state = 0;
                }
                break;
            case 51:
            case 52:
                pose->flags &= ~EXT_POSE_FLAG_BILLBOARD;
                (*pose->mtx)[0][3] = x - 15.0f;
                (*pose->mtx)[1][3] = y - 5.0f;
                (*pose->mtx)[2][3] = 5.0f + z;
                (*pose->mtx)[1][1] = 0.0f;
                (*pose->mtx)[0][0] = 0.0f;
                t->vel.x = 0.0f;
                t->vel.y = 0.0f;
                t->vel.z = 0.0f;
                t->start.x = (*pose->mtx)[0][3];
                t->start.y = (*pose->mtx)[1][3];
                t->start.z = (*pose->mtx)[2][3];
                if (t->state == 51) {
                    t->target.x = (hx + ofs[i][0]) - 200.0f;
                } else {
                    t->target.x = (hx + ofs[i][0]) - 200.0f;
                }
                if (t->state == 51) {
                    t->target.y = (hy + ofs[i][1]) - 50.0f;
                } else {
                    t->target.y = (hy + ofs[i][1]) - 50.0f;
                }
                t->target.z = (hz - 10.0f) - 0.05f * i;
                t->wait = i * 3;
                t->timer = 0;
                t->state = 53;
                t->intplMode = modes[irand(7)];
                break;
            case 53:
                atmic_teresa_scale -= 0.015f;
                t->state = 55;
                break;
            case 55:
                (*pose->mtx)[0][0] += 0.04f;
                if ((*pose->mtx)[0][0] >= 1.0f) {
                    (*pose->mtx)[0][0] = 1.0f;
                }
                (*pose->mtx)[1][1] = (*pose->mtx)[0][0];
                (*pose->mtx)[0][3] = intplGetValue(t->intplMode, t->start.x, t->target.x, t->timer, 30);
                (*pose->mtx)[1][3] = intplGetValue(t->intplMode, t->start.y, t->target.y, t->timer, 30);
                (*pose->mtx)[2][3] = intplGetValue(t->intplMode, t->start.z, t->target.z, t->timer, 30);
                if (t->timer >= 30) {
                    t->state = 60;
                }
                t->timer++;
                break;
            case 63:
                t->target.x = (x - 35.0f) + irand(90);
                t->target.y = (y - 35.0f) + irand(90);
                t->target.z = (z - 30.0f) - 0.05f * i;
                t->timer = 60;
                t->state = 65;
                if (i % 2 == 0) {
                    Vec pos = {0.0f, 0.0f, 0.0f};
                    pos.x = (*pose->mtx)[0][3];
                    pos.y = (*pose->mtx)[1][3];
                    pos.z = (*pose->mtx)[2][3];
                    BtlUnit_snd_se_pos(unit, "SFX_BOSS_ATMTLSA_S_MOVE1", 0xF1194D80, 0, &pos);
                }
                break;
            case 65:
                t->vel.x = (t->target.x - (*pose->mtx)[0][3]) / 15.0f;
                t->vel.y = (t->target.y - (*pose->mtx)[1][3]) / 15.0f;
                t->vel.z = (t->target.z - (*pose->mtx)[2][3]) / 15.0f;
                if (t->timer <= 0) {
                    t->state = 70;
                    t->vel.x = 0.0f;
                    t->vel.y = 0.0f;
                    t->vel.z = 0.0f;
                }
                t->timer--;
                break;
            case 71:
                (*pose->mtx)[0][0] -= 0.0125f;
                (*pose->mtx)[1][1] = (*pose->mtx)[0][0];
                if ((*pose->mtx)[0][0] <= 0.0f) {
                    t->state = 0;
                }
                break;
        }
    }
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_ALPHA_TEST, teresa_ext_disp, NULL);
}

/**
 * Draw callback for the small Atomic Boos: sets up the extdrv render mode, vertices, texture and TEV,
 * then draws all poses.
 */
void teresa_ext_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

/**
 * Script helper: stores the unit id in args[0] (BattleTransID) as the Atomic Boo whose position the small Boos
 * follow.
 */
USER_FUNC(atmic_teresa_id_set) {
    s32 id = evtGetValue(event, event->args[0]);

    atmic_teresa_id[0] = BattleTransID(event, id);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: stores the target unit id (args[0], BattleTransID)
 * and target part id (args[1]) the small Boos fly toward.
 */
USER_FUNC(target_id_set) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 partId = evtGetValue(event, args[1]);

    target_unit_id[0] = BattleTransID(event, id);
    target_parts_id[0] = partId;
    return EVT_RETURN_DONE;
}

/** Script helper: starts the first eight small Boos on their appearance (state 1) for the split-up attack. */
USER_FUNC(teresa_appear_bikkuri) {
    s32 i;

    for (i = 0; i < 8; i++) {
        teresa[i].state = 1;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: blocks until all eight split-up Boos have reached state 15 (in position). */
USER_FUNC(teresa_wait_bikkuri) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (teresa[i].state != 15) {
            return EVT_RETURN_BLOCK;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: blocks until all eight split-up Boos have reached state 30,
 * then shrinks them away (state 31) and resets atmic_teresa_scale to 1.0.
 */
USER_FUNC(teresa_wait_bikkuri2) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (teresa[i].state != 30) {
            return EVT_RETURN_BLOCK;
        }
    }
    for (i = 0; i < 8; i++) {
        teresa[i].state = 31;
    }
    atmic_teresa_scale = 1.0f;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] picks the breath swarm variant (1 starts every small Boo in state 51,
 * anything else in state 52).
 */
USER_FUNC(teresa_appear_breath) {
    s32 type = evtGetValue(event, event->args[0]);
    s32 i;
    s32 state = (type == 1) ? 51 : 52;

    for (i = 0; i < extGetPoseNum(); i++) {
        teresa[i].state = state;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: blocks until all 30 small Boos have reached state 60,
 * then moves them on to state 63 with a 30-frame wait.
 */
USER_FUNC(teresa_wait_breath) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (teresa[i].state != 60) {
            return EVT_RETURN_BLOCK;
        }
    }
    for (i = 0; i < 30; i++) {
        AtmTeresa* t = &teresa[i];
        t->state = 63;
        t->wait = 30;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: blocks until all 30 small Boos have reached state 70,
 * then moves them to state 71 and resets atmic_teresa_scale to 1.0.
 */
USER_FUNC(teresa_wait_breath2) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (teresa[i].state != 70) {
            return EVT_RETURN_BLOCK;
        }
    }
    for (i = 0; i < 30; i++) {
        teresa[i].state = 71;
    }
    atmic_teresa_scale = 1.0f;
    return EVT_RETURN_DONE;
}
