// Battle unit Beldam, in the Shadow Sirens fight (マジョリン): win.rel's copy.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "unit/party/unit_party_vivian.h"
#include "battle/battle_database_common.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"
#include "battle/unit_work.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_ATTACK_STEP 8   // scripted attack order: 0 kagenuke -> 1 powder_attack2 -> 2 powder_attack1 -> 4 random pick; 3 = blizzard next (set by the 66% line), then 4
#define UNIT_WORK_BLIZZARD_WAIT 9 // turns before the blizzard can be drawn again (2 after each blizzard; phase_event counts down)

#include "unit/unit_boss_majolyne.h"

static USER_FUNC(majo3_sort);

BattleUnitKind unit_boss_majolyne;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_majolyne_kagenuke_attack1;
static BattleWeapon weapon_majolyne_blizzard_attack1;
static BattleWeapon weapon_majolyne_powder_attack1;
static BattleWeapon weapon_majolyne_powder_attack2;
static EvtWord majolyne_down[19];
static EvtWord majolyne_up[27];
static EvtWord majo3_zoom[25];
EvtWord btl_vivian_zoom[7];
EvtWord btl_marilyn_zoom[7];
EvtWord btl_majolyne_zoom[7];
static EvtWord init_event[45];
static EvtWord damage_event[22];
static EvtWord dead_event[122];
EvtWord majo_unison_phase_event[238];
static EvtWord unison_phase_event[83];
static EvtWord phase_event[337];
static EvtWord attack_event[241];
#if VERSION >= VERSION_US
static EvtWord kagenuke_attack_event[225];
#else
static EvtWord kagenuke_attack_event[222];
#endif
static EvtWord blizzard_attack_event[243];
static EvtWord powder_attack_check[16];
static EvtWord powder_attack_event[179];
static EvtWord wait_event[6];
#if VERSION >= VERSION_US
EvtWord majo3_shadow_tail_event[79];
#else
EvtWord majo3_shadow_tail_event[78];
#endif

/**
 * Unit data of Beldam (btl_un_majolyne1), the Shadow Sirens' leader in Boggly Woods: 9 HP, level 26.
 * Vivian's init script spawns her and her sister Marilyn at the start of the fight,
 * and Beldam's phase script drives the sisters' lines.
 */
BattleUnitKind unit_boss_majolyne = {
    UNIT_BELDAM_CHAPTER2, // unitType
    "btl_un_majolyne1", // name
    9, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    26, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    9, // pbSoftCap
    26, // width
    34, // height
    {0, 34}, // hitOffset
    {0.0f, 17.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {6.0f, 2.0f, 0.0f}, // talkTogeBaseOffset
    {13.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {13.0f, 0.0f, 0.0f}, // bintaHitOffset
    {8.0f, 22.1f, 0.0f}, // kissHitOffset
    {0.0f, 17.0f, 0.0f}, // cutBaseOffset
    26.0f, // cutWidth
    34.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_MAJO_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    1, // numParts
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Flat damage reduction of Beldam against normal, fire, ice, explosion and electric hits: none. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element attributes of Beldam: none, so every element does its normal damage. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {70, 80, 70, 100, 70, 100, 100, 50, 100, 70, 100, 70, 100, 70, 70, 0, 0, 100, 70, 100, 100, 0};
#else
static StatusVulnerability regist = {
    70, // sleep
    80, // stop
    70, // dizzy
    100, // poison
    70, // confuse
    100, // electric
    100, // burn
    50, // freeze
    100, // huge
    80, // tiny
    100, // attackUp
    80, // attackDown
    100, // defenseUp
    95, // defenseDown
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

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "MJR_N_1"},
    {POSE_TYPE_STOP, "MJR_Z_1"},
    {POSE_TYPE_FREEZE, "MJR_Z_1"},
    {POSE_TYPE_CONFUSE, "MJR_X_1"},
    {POSE_TYPE_POISON, "MJR_X_1"},
    {POSE_TYPE_DIZZY, "MJR_X_1"},
    {POSE_TYPE_NORMAL, "MJR_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "MJR_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "MJR_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "MJR_D_1"},
    {POSE_TYPE_DAMAGE, "MJR_D_1"},
    {POSE_TYPE_ATTACK, "MJR_S_1"},
    {POSE_TYPE_RUN, "MJR_R_1"},
    {POSE_TYPE_WALK, "MJR_W_1"},
    {POSE_TYPE_DANGER, "MJR_S_1"},
    {POSE_TYPE_PERIL, "MJR_S_1"},
    {POSE_TYPE_TALK, "MJR_T_1"},
    {POSE_TYPE_ACTION, "MJR_S_1"},
};

/** Data table of Beldam: event 49 (she is defeated) runs dead_event. */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

/** Beldam's single part: her model (c_majyorin), the main target and the part that covers for allies. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_majolyne1", "c_majyorin", 0.0f, 0.0f, 0.0f, 5.0f, 20.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/** Beldam's shadow-slip push: 1 ice-element damage to one opposing target, guardable. */
static BattleWeapon weapon_majolyne_kagenuke_attack1 = {
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
    ELEMENT_ICE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
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

/** Beldam's blizzard: 2 ice-element damage to every opposing target, ignoring defense. */
static BattleWeapon weapon_majolyne_blizzard_attack1 = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_ICE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
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
 * Beldam's enlarging powder: aimed at an ally of hers, it always hits and makes the target Huge for 3 turns.
 */
static BattleWeapon weapon_majolyne_powder_attack1 = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
    1, // reserved_1B
    NULL, // hpDamageFn
    {0, 2, 2, 3, 3, 4, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_CANNOT_MISS | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, // burnTime..freezeTime
    100, // sizeChangeChance
    3, // sizeChangeTime
    2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Beldam's shrinking powder: aimed at an enemy, it makes the target Tiny for 3 turns. */
static BattleWeapon weapon_majolyne_powder_attack2 = {
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
    NULL, // hpDamageFn
    {0, 2, 2, 3, 3, 4, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, // burnTime..freezeTime
    100, // sizeChangeChance
    3, // sizeChangeTime
    -2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Beldam sinks into her shadow: after 250 ms plays the shadow sound (SFX_BOSS_MAJO_SHADOW1)
 * and hides her model (battle_evt_majo_disp_off).
 */
static EVT_BEGIN(majolyne_down)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MAJO_SHADOW1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(battle_evt_majo_disp_off, UNIT_ID_SELF, 1, 0, 0)
    RETURN
EVT_END()

/**
 * Beldam rises out of her shadow at the position in LW(0..2):
 * after 250 ms plays the rising sound (SFX_BOSS_MAJO_SHADOW2), shows her model again and sets her position.
 */
static EVT_BEGIN(majolyne_up)
    BEGIN_INLINE_EVENT
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MAJO_SHADOW2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(battle_evt_majo_disp_on, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    RETURN
EVT_END()

/**
 * Moves the battle camera onto the unit id in LW(0) (looking at its x from a fixed height and distance,
 * a 30-frame move) and waits 37 frames;
 * the Shadow Sirens' scripts use it before each of their lines.
 */
static EVT_BEGIN(majo3_zoom)
    CALL(btlevtcmd_GetPos, LW(0), LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, LW(0), 65, 332, LW(0), 30, -340, 30, EASING_SINE_IN_OUT)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

/**
 * Camera move onto Vivian (unit id in GW(10), set by her init script) through majo3_zoom,
 * used before her lines in the fight.
 */
EVT_BEGIN(btl_vivian_zoom)
    SET_VALUE(LW(0), GW(10))
    CALL_CHILD(PTR(majo3_zoom))
    RETURN
EVT_END()

/**
 * Camera move onto Marilyn (unit id in GW(9), spawned by Vivian's init script) through majo3_zoom,
 * used before her lines in the fight.
 */
EVT_BEGIN(btl_marilyn_zoom)
    SET_VALUE(LW(0), GW(9))
    CALL_CHILD(PTR(majo3_zoom))
    RETURN
EVT_END()

/**
 * Camera move onto Beldam (unit id in GW(8), spawned by Vivian's init script) through majo3_zoom,
 * used before her lines in the fight.
 */
EVT_BEGIN(btl_majolyne_zoom)
    SET_VALUE(LW(0), GW(8))
    CALL_CHILD(PTR(majo3_zoom))
    RETURN
EVT_END()

/**
 * Init script of Beldam in the Shadow Sirens fight:
 * installs her wait, phase, attack and damage scripts and the default confusion script,
 * clears unit work 8 (attack cycle step), 9 (blizzard cooldown) and 3 (shadow-tail pause),
 * starts majo3_shadow_tail_event and allocates 10 user words into GW(15).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_BLIZZARD_WAIT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_HOLD, 0)
    SPAWN_EVENT(PTR(majo3_shadow_tail_event))
    ALLOCATE_UW(10, GW(15))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Damage handler for Beldam: records in GW(11) when she took the "knocked out" damage code (1024),
 * then runs the default damage script.
 */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(11), 1)
    END_IF
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Beldam's defeat: she walks back to her home spot if displaced and the camera moves onto her;
 * if both sisters are already down (GW(12) and GW(13) nonzero) she only screams (btl_majo3_11),
 * otherwise she leaves the rest to them (btl_majo3_08).
 * Then GW(11) becomes 2 (dead) and the shared dead sequence runs.
 */
static EVT_BEGIN(dead_event)
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
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    END_IF
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL_CHILD(PTR(btl_majolyne_zoom))
    WAIT_MS(500)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("MJR_D_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("MJR_D_1"))
    IF_INT_NOT_EQUAL(GW(12), 0)
        IF_INT_NOT_EQUAL(GW(13), 0)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_11"), 0, -2)
            GOTO(90)
        END_IF
    END_IF
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_08"), 0, -2)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_D_3"))
    LABEL(90)
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    SET_VALUE(GW(11), 2)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    CALL_CHILD(PTR(subsetevt_dead_core_nospin_norotate))
    RETURN
EVT_END()

/**
 * HP-order helper shared by the Shadow Sirens' unison-phase scripts:
 * reads the HP of each sister still alive (GW(11) to GW(13) zero)
 * into the slot given by her unit work 5, orders the slots by HP with majo3_sort (highest first)
 * and writes each sister's new slot back to her unit work 5.
 */
EVT_BEGIN(majo_unison_phase_event)
    // Read the HP of each sister still standing (Vivian, Marilyn, Beldam) under the
    // formation slot she holds now.
    SET_VALUE(LW(0), -1)
    SET_VALUE(LW(1), -1)
    SET_VALUE(LW(2), -1)
    IF_INT_EQUAL(GW(13), 0)
        CALL(btlevtcmd_GetUnitWork, GW(10), MAJO_WORK_NEXT_LINE_POS, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            CALL(btlevtcmd_GetHp, GW(10), LW(0))
            SET_VALUE(LW(3), 1)
            CASE_EQUAL(1)
            CALL(btlevtcmd_GetHp, GW(10), LW(1))
            SET_VALUE(LW(4), 1)
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetHp, GW(10), LW(2))
            SET_VALUE(LW(5), 1)
        END_SWITCH
    END_IF
    IF_INT_EQUAL(GW(12), 0)
        CALL(btlevtcmd_GetUnitWork, GW(9), MAJO_WORK_NEXT_LINE_POS, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            CALL(btlevtcmd_GetHp, GW(9), LW(0))
            SET_VALUE(LW(3), 2)
            CASE_EQUAL(1)
            CALL(btlevtcmd_GetHp, GW(9), LW(1))
            SET_VALUE(LW(4), 2)
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetHp, GW(9), LW(2))
            SET_VALUE(LW(5), 2)
        END_SWITCH
    END_IF
    IF_INT_EQUAL(GW(11), 0)
        CALL(btlevtcmd_GetUnitWork, GW(8), MAJO_WORK_NEXT_LINE_POS, LW(6))
        SWITCH(LW(6))
            CASE_EQUAL(0)
            CALL(btlevtcmd_GetHp, GW(8), LW(0))
            SET_VALUE(LW(3), 3)
            CASE_EQUAL(1)
            CALL(btlevtcmd_GetHp, GW(8), LW(1))
            SET_VALUE(LW(4), 3)
            CASE_EQUAL(2)
            CALL(btlevtcmd_GetHp, GW(8), LW(2))
            SET_VALUE(LW(5), 3)
        END_SWITCH
    END_IF

    // Sort the sisters by HP, highest first.
    CALL(majo3_sort)

    // Hand out the new formation slots in that order: the healthiest sister takes slot 0 (front),
    // the weakest slot 2; defeated sisters are skipped.
    SWITCH(LW(0))
        CASE_EQUAL(1)
        IF_INT_EQUAL(GW(13), 0)
            CALL(btlevtcmd_SetUnitWork, GW(10), MAJO_WORK_NEXT_LINE_POS, 0)
        END_IF
        CASE_EQUAL(2)
        IF_INT_EQUAL(GW(12), 0)
            CALL(btlevtcmd_SetUnitWork, GW(9), MAJO_WORK_NEXT_LINE_POS, 0)
        END_IF
        CASE_EQUAL(3)
        IF_INT_EQUAL(GW(11), 0)
            CALL(btlevtcmd_SetUnitWork, GW(8), MAJO_WORK_NEXT_LINE_POS, 0)
        END_IF
    END_SWITCH
    SWITCH(LW(1))
        CASE_EQUAL(1)
        IF_INT_EQUAL(GW(13), 0)
            CALL(btlevtcmd_SetUnitWork, GW(10), MAJO_WORK_NEXT_LINE_POS, 1)
        END_IF
        CASE_EQUAL(2)
        IF_INT_EQUAL(GW(12), 0)
            CALL(btlevtcmd_SetUnitWork, GW(9), MAJO_WORK_NEXT_LINE_POS, 1)
        END_IF
        CASE_EQUAL(3)
        IF_INT_EQUAL(GW(11), 0)
            CALL(btlevtcmd_SetUnitWork, GW(8), MAJO_WORK_NEXT_LINE_POS, 1)
        END_IF
    END_SWITCH
    SWITCH(LW(2))
        CASE_EQUAL(1)
        IF_INT_EQUAL(GW(13), 0)
            CALL(btlevtcmd_SetUnitWork, GW(10), MAJO_WORK_NEXT_LINE_POS, 2)
        END_IF
        CASE_EQUAL(2)
        IF_INT_EQUAL(GW(12), 0)
            CALL(btlevtcmd_SetUnitWork, GW(9), MAJO_WORK_NEXT_LINE_POS, 2)
        END_IF
        CASE_EQUAL(3)
        IF_INT_EQUAL(GW(11), 0)
            CALL(btlevtcmd_SetUnitWork, GW(8), MAJO_WORK_NEXT_LINE_POS, 2)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Unison-phase script of Beldam, which nothing installs (init_event sets only phase_event):
 * after the player's phase it re-sorts the sisters by HP (majo_unison_phase_event);
 * if her slot (unit work 5) differs from her current one (unit work 4) she sinks into her shadow
 * and comes up at the new home spot (x 55, 105 or 155), and the turn order is re-sorted.
 */
static EVT_BEGIN(unison_phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_AFTER_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(majo_unison_phase_event))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MAJO_WORK_LINE_POS, LW(3))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MAJO_WORK_NEXT_LINE_POS, LW(4))
        IF_INT_NOT_EQUAL(LW(3), LW(4))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MAJO_WORK_LINE_POS, LW(4))
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            WAIT_MS(400)
            CALL_CHILD(PTR(majolyne_down))
            WAIT_MS(500)
            SWITCH(LW(4))
                CASE_EQUAL(0)
                SET_VALUE(LW(0), 55)
                SET_VALUE(LW(2), 0)
                CASE_EQUAL(1)
                SET_VALUE(LW(0), 105)
                SET_VALUE(LW(2), 10)
                CASE_EQUAL(2)
                SET_VALUE(LW(0), 155)
                SET_VALUE(LW(2), 20)
            END_SWITCH
            SET_VALUE(LW(1), 0)
            CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL_CHILD(PTR(majolyne_up))
            CALL(btlevtcmd_SortPhaseMoveTable)
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Beldam's phase script, after the player's phase: counts down her blizzard cooldown (unit work 9).
 * If she can act, it checks HP once per sister (unit work 6):
 * below 66% Beldam herself complains (btl_majo3_03), all living sisters get unit work 7 (enraged) and,
 * the first time, her next attack is the blizzard (unit work 8 = 3);
 * Marilyn below 50% gets ordered to hit Mario (btl_majo3_04, Marilyn grunts if she can act)
 * and Vivian below 50% gets scolded (btl_majo3_06, she apologizes), each setting that sister's unit work 7.
 */
static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_AFTER_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        // After the player's phase Beldam counts down her blizzard cooldown.
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BLIZZARD_WAIT, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_BLIZZARD_WAIT, LW(0))
        END_IF
        CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(90)
        END_IF

        // Beldam below 66% HP: she admits Mario is tougher than she thought and gets serious (btl_majo3_03);
        // the sisters turn aggressive and her next attack is the blizzard.
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MAJO_WORK_LOW_HP_TALK_DONE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
            DEBUG_PUT_REG(LW(0))
            MULTIPLY(LW(0), 66)
            DEBUG_PUT_REG(LW(0))
            DIVIDE(LW(0), 100)
            DEBUG_PUT_REG(LW(0))
            CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(1))
            DEBUG_PUT_REG(LW(1))
            IF_INT_LESS(LW(1), LW(0))
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MAJO_WORK_LOW_HP_TALK_DONE, 1)
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
                CALL_CHILD(PTR(btl_majolyne_zoom))
                CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_03"), 0, -2)
                WAIT_MS(200)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MAJO_WORK_LOW_HP_ATTACK, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, MAJO_WORK_LOW_HP_ATTACK, 1)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 3)
                END_IF
                IF_INT_EQUAL(GW(12), 0)
                    CALL(btlevtcmd_SetUnitWork, GW(9), MAJO_WORK_LOW_HP_ATTACK, 1)
                END_IF

                // Vivian below 50% HP: Beldam scolds her (btl_majo3_06) and Vivian apologizes (btl_majo3_07).
                IF_INT_EQUAL(GW(13), 0)
                    CALL(btlevtcmd_SetUnitWork, GW(10), MAJO_WORK_LOW_HP_ATTACK, 1)
                END_IF
                CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
                CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
                CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
            END_IF
        END_IF

        // Marilyn below 50% HP: Beldam orders her to hit Mario (btl_majo3_04) and Marilyn grunts back
        // (btl_majo3_05) if she can still act.
        IF_INT_EQUAL(GW(12), 0)
            CALL(btlevtcmd_GetUnitWork, GW(9), MAJO_WORK_LOW_HP_TALK_DONE, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetMaxHp, GW(9), LW(0))
                MULTIPLY(LW(0), 50)
                DIVIDE(LW(0), 100)
                CALL(btlevtcmd_GetHp, GW(9), LW(1))
                IF_INT_LESS(LW(1), LW(0))
                    CALL(btlevtcmd_SetUnitWork, GW(9), MAJO_WORK_LOW_HP_TALK_DONE, 1)
                    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
                    CALL_CHILD(PTR(btl_majolyne_zoom))
                    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, GW(9), -1, FACE_DIR_TOWARD_UNIT, LW(15))
                    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
                    WAIT_MS(200)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_04"), 0, -2)
                    WAIT_MS(200)
                    CALL(btlevtcmd_SetUnitWork, GW(9), MAJO_WORK_LOW_HP_ATTACK, 1)
                    CALL(btlevtcmd_CheckActStatus, GW(9), LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL_CHILD(PTR(btl_marilyn_zoom))
                        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_05"), 0, GW(9))
                        WAIT_MS(200)
                    END_IF
                    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
                    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
                    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
                END_IF
            END_IF
        END_IF
        IF_INT_EQUAL(GW(13), 0)
            CALL(btlevtcmd_GetUnitWork, GW(10), MAJO_WORK_LOW_HP_TALK_DONE, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetMaxHp, GW(10), LW(0))
                MULTIPLY(LW(0), 50)
                DIVIDE(LW(0), 100)
                CALL(btlevtcmd_GetHp, GW(10), LW(1))
                IF_INT_LESS(LW(1), LW(0))
                    CALL(btlevtcmd_SetUnitWork, GW(10), MAJO_WORK_LOW_HP_TALK_DONE, 1)
                    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
                    CALL_CHILD(PTR(btl_majolyne_zoom))
                    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, GW(10), -1, FACE_DIR_TOWARD_UNIT, LW(15))
                    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
                    WAIT_MS(200)
                    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_06"), 0, -2)
                    WAIT_MS(200)
                    CALL(btlevtcmd_SetUnitWork, GW(10), MAJO_WORK_LOW_HP_ATTACK, 1)
                    CALL(btlevtcmd_CheckActStatus, GW(10), LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL_CHILD(PTR(btl_vivian_zoom))
                        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("btl_majo3_07"), 0, GW(10))
                        WAIT_MS(200)
                    END_IF
                    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
                    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
                    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
                    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
                END_IF
            END_IF
        END_IF
    END_IF
    LABEL(90)
    RETURN
EVT_END()

/**
 * Beldam's attack choice: uses an item if the AI picks one;
 * otherwise unit work 8 runs a fixed opening of shadow slip, shrinking powder on an enemy,
 * then enlarging powder on an ally,
 * and afterwards a weighted pick: shadow slip 40, enlarging powder 20 (if a target is not already Huge),
 * shrinking powder 20 (if a target is not already Tiny),
 * and the blizzard 20 once she is enraged (unit work 7) and its cooldown (unit work 9) is over.
 * Unit work 8 = 3 (set when she drops below 66% HP) uses the blizzard next;
 * the blizzard sets a two-phase cooldown.
 */
static EVT_BEGIN(attack_event)
    // Use an item if the AI decided to.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF

    // Opening moves by unit work 8: shadow slip, then shrinking powder on an enemy,
    // then enlarging powder on an ally; 3 is the blizzard.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 1)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_kagenuke_attack1))
        CALL_CHILD(PTR(kagenuke_attack_event))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 2)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack2))
        CALL_CHILD(PTR(powder_attack_check))
        CALL_CHILD(PTR(powder_attack_event))
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 4)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack1))
        CALL_CHILD(PTR(powder_attack_check))
        CALL_CHILD(PTR(powder_attack_event))
        CASE_EQUAL(3)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 4)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_BLIZZARD_WAIT, 2)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_blizzard_attack1))
        CALL_CHILD(PTR(blizzard_attack_event))
        CASE_EQUAL(4)
        // After the opening, a weighted pick: shadow slip 40,
        // plus 20 for each powder still useful and 20 for the blizzard once she is
        // angry and its cooldown is over.
        SET_VALUE(LW(14), 40)
        SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack1))
        CALL_CHILD(PTR(powder_attack_check))
        IF_INT_NOT_EQUAL(LW(3), -1)
            SET_VALUE(LW(10), LW(3))
            SET_VALUE(LW(11), LW(4))
            CALL(btlevtcmd_CheckStatus, LW(3), STATUS_HUGE, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                ADD(LW(14), 20)
            END_IF
        END_IF
        SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack2))
        CALL_CHILD(PTR(powder_attack_check))
        IF_INT_NOT_EQUAL(LW(3), -1)
            SET_VALUE(LW(12), LW(3))
            SET_VALUE(LW(13), LW(4))
            CALL(btlevtcmd_CheckStatus, LW(3), STATUS_TINY, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                ADD(LW(14), 20)
            END_IF
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, MAJO_WORK_LOW_HP_ATTACK, LW(2))
        IF_INT_NOT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BLIZZARD_WAIT, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                ADD(LW(14), 20)
            END_IF
        END_IF
        SUBTRACT(LW(14), 1)

        // Roll the die and run the chosen attack.
        CALL(evt_sub_random, LW(14), LW(0))
        SET_VALUE(LW(1), 40)
        IF_INT_LESS(LW(0), LW(1))
            SET_VALUE(LW(9), PTR(&weapon_majolyne_kagenuke_attack1))
            CALL_CHILD(PTR(kagenuke_attack_event))
            GOTO(90)
        END_IF
        ADD(LW(1), 20)
        IF_INT_LESS(LW(0), LW(1))
            SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack1))
            SET_VALUE(LW(3), LW(10))
            SET_VALUE(LW(4), LW(11))
            CALL_CHILD(PTR(powder_attack_event))
            GOTO(90)
        END_IF
        ADD(LW(1), 20)
        IF_INT_LESS(LW(0), LW(1))
            SET_VALUE(LW(9), PTR(&weapon_majolyne_powder_attack2))
            SET_VALUE(LW(3), LW(12))
            SET_VALUE(LW(4), LW(13))
            CALL_CHILD(PTR(powder_attack_event))
            GOTO(90)
        ELSE
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_BLIZZARD_WAIT, 2)
            SET_VALUE(LW(9), PTR(&weapon_majolyne_blizzard_attack1))
            CALL_CHILD(PTR(blizzard_attack_event))
        END_IF
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Beldam's shadow-slip attack (weapon_majolyne_kagenuke_attack1, 1 damage to one enemy):
 * she sinks into her shadow, rises in front of the target, pushes it and sinks again to return home.
 */
static EVT_BEGIN(kagenuke_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A1_1"))
    CALL_CHILD(majolyne_down)
    WAIT_MS(300)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
#if VERSION >= VERSION_US
    ADD(LW(2), 5)
#endif
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 30)
    SET_VALUE(LW(1), 0)
    CALL_CHILD(majolyne_up)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MAJO_PUSH1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A1_2"))
    WAIT_FRAMES(25)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A1_3"))
    WAIT_MS(350)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A1_4"))
    WAIT_MS(200)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(98)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A1_1"))
    CALL_CHILD(majolyne_down)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(200)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL_CHILD(majolyne_up)
    RETURN
EVT_END()

/**
 * Beldam's blizzard attack (weapon_majolyne_blizzard_attack1):
 * she opens her hands and blows an icy breath ("ibuki" effect)
 * that hits every enemy for 2 damage, piercing defense; the weapon gives no freeze chance.
 */
static EVT_BEGIN(blizzard_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MAJO_HAND_OPEN1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A1_2"))
    WAIT_FRAMES(25)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A1_3"))
    WAIT_MS(700)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A1_4"))
    WAIT_MS(200)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MAJO_ATTACK_ICE1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    IF_INT_GREATER_OR_EQUAL(LW(5), 0)
        CALL(evt_eff, 0, STRING("ibuki"), 0, LW(0), LW(1), LW(2), 180, 0, 0, 0, 0, 0, 0, 0)
    ELSE
        CALL(evt_eff, 0, STRING("ibuki"), 1, LW(0), LW(1), LW(2), 180, 0, 0, 0, 0, 0, 0, 0)
    END_IF
    WAIT_FRAMES(150)
    CALL(evt_snd_sfxoff, LW(15))
    LABEL(10)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    LABEL(91)

    // Defense input is checked only for the first selected target.
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(1000)
    RETURN
EVT_END()

/**
 * Powder-attack helper: picks a target for the weapon in LW(9) with the sampling calls
 * and returns it in LW(3) (unit, -1 if none)
 * and LW(4) (part).
 */
static EVT_BEGIN(powder_attack_check)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    RETURN
EVT_END()

/**
 * Beldam's powder attack on the target in LW(3)/LW(4) with the weapon in LW(9):
 * weapon_majolyne_powder_attack1 makes an ally (a sister or Beldam herself) Huge
 * and weapon_majolyne_powder_attack2 makes an enemy Tiny,
 * both for 3 turns and without damage. With no target she gets flustered when confused, else does nothing.
 */
static EVT_BEGIN(powder_attack_event)
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    IF_INT_EQUAL(LW(9), PTR(&weapon_majolyne_powder_attack1))
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_MIDPOINT)
        CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 200)
    END_IF
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MAJO_HAND_OPEN1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A2_1"))
    WAIT_MS(700)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A2_2"))
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MJR_A2_3"))
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_FRAMES(30)
    GOTO(99)
    LABEL(99)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait event of Beldam: switches to pose table entry 1 (idle). */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Background loop that keeps Beldam's shadow-tail position (unit work 0..2) following her position on the stage
 * floor each frame, until the unit is gone.
 */
EVT_BEGIN(majo3_shadow_tail_event)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_X, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Y, LW(1))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Z, LW(2))
    LABEL(0)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(5))
    IF_INT_EQUAL(LW(5), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_HOLD, LW(5))
    IF_INT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_X, LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Y, LW(1))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, SHADOW_TAIL_WORK_Z, LW(2))
    END_IF
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

/**
 * Script helper: sorts three (id, value) pairs from lwData[3..5] and lwData[0..2] by value, highest first,
 * and writes the ids back to lwData[0..2]; used to order the sisters by HP.
 */
static USER_FUNC(majo3_sort) {
    MajoSortEntry tbl[3];
    MajoSortEntry tmp;
    s32 i;

    for (i = 0; i < 3; i++) {
        tbl[i].id = event->lwData[3 + i];
        tbl[i].value = event->lwData[i];
    }
    if (tbl[0].value < tbl[1].value) {
        tmp = tbl[0];
        tbl[0] = tbl[1];
        tbl[1] = tmp;
    }
    if (tbl[1].value < tbl[2].value) {
        tmp = tbl[1];
        tbl[1] = tbl[2];
        tbl[2] = tmp;
    }
    if (tbl[0].value < tbl[1].value) {
        tmp = tbl[0];
        tbl[0] = tbl[1];
        tbl[1] = tmp;
    }
    for (i = 0; i < 3; i++) {
        event->lwData[i] = tbl[i].id;
    }
    return EVT_RETURN_DONE;
}
