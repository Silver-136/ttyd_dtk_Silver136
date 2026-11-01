// Palace of Shadow battle unit: Bowser (fought with Kammy Koopa) (`boss_koopa`): parts, stats, poses and AI scripts.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "battle/battle_status_effect.h"
#include "unit_boss_kamec_obaba.h"

#include "driver/icondrv.h"
#include "driver/camdrv.h"
#include "battle/battle.h"
#include "manager/evtmgr_cmd.h"
#include "driver/dispdrv.h"
#include "system.h"
#include "battle/battle_event_default.h"
#include "battle/battle_message.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_LEANING 0        // 1 while leaning forward (KPA_U_2, spiky top counter, pose_table_forward_leaning); talk events straighten up (KPA_U_3) first
#define UNIT_WORK_MARIO_HIT_TALK 1 // 0 until an attack damages Mario, 1 then (first_mario_damage_check), 2 after the stg8_las_98 talk
#define UNIT_WORK_HALF_HP 2        // half its max HP; harf_hp_event talks once HP drops below it
#define UNIT_WORK_HALF_HP_TALKED 3 // 1 once harf_hp_event has said stg8_las_100
#define UNIT_WORK_ATTACK_STEP 4    // fixed opening order: 0 fire attack, 1 bite, 2 hip attack, then 3 = DrawLots each turn
#define UNIT_WORK_TARGET_KIND 5    // unit kind of the current attack's target (UNIT_MARIO 222 or the partners 224-230)

#include "unit/unit_boss_koopa.h"
static void _disp_icon(CameraId camera, void* param);

static USER_FUNC(_icon_jump_icon);
static USER_FUNC(party_cannot_use_command);
static USER_FUNC(mario_cannot_use_command);
extern BattleUnitKind unit_boss_kamec_obaba;

BattleUnitKind unit_boss_koopa;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[18];
static PoseTableEntry pose_table_forward_leaning[18];
static PoseTableEntry pose_table_dead[3];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[1];
static BattleWeapon weapon_koopa_hip_attack;
static BattleWeapon weapon_koopa_bite_attack;
static BattleWeapon weapon_koopa_critical_bite_attack;
static BattleWeapon weapon_koopa_fire_attack;
BattleUnitSetup entry_kamec_obaba;
static EvtWord koopa_zoom[25];
static EvtWord kamec_obaba_zoom[40];
static EvtWord init_event[124];
static EvtWord entry_event[55];
static EvtWord damage_event[22];
static EvtWord dead_event[172];
static EvtWord phase_event[134];
static EvtWord harf_hp_event[94];
static EvtWord attack_event[110];
static EvtWord hip_attack_event[VERSION_PICK(431, 432)];
static EvtWord bite_attack_event[278];
static EvtWord critical_bite_attack_event[261];
static EvtWord fire_attack_event[356];
EvtWord first_mario_damage_check[33];
EvtWord first_mario_damage_event[114];
static EvtWord wait_event[6];
static const char* cmd_disable_string[5];

/**
 * Unit data of Bowser in the Palace of Shadow: 70 HP, level 68;
 * names its parts, status table and init script.
 */
BattleUnitKind unit_boss_koopa = {
    UNIT_BOWSER_CHAPTER8, // unitType
    "btl_un_koopa", // name
    70, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    68, // level
    0, // bonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    6, // pbSoftCap
    86, // width
    80, // height
    {0, 70}, // hitOffset
    {0.0f, 40.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {40.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {43.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {21.0f, 0.0f, 0.0f}, // bintaHitOffset
    {36.0f, 72.0f, 0.0f}, // kissHitOffset
    {0.0f, 40.0f, 0.0f}, // cutBaseOffset
    86.0f, // cutWidth
    80.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_KOOPA_DAMAGED1", // damageSfxName
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

/**
 * Defence of Bowser against each element (normal, fire, ice, explosion, electric):
 * 2 against every element, so each hit is cut by 2.
 */
static BattleUnitDefense defence = {{2, 2, 2, 2, 2}};

/**
 * Damage modifiers of Bowser per element (normal, fire, ice, explosion, electric):
 * none, so no element is a weakness, an immunity or a heal.
 */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {30, 30, 30, 100, 40, 100, 100, 25, 100, 50, 100, 50, 100, 30, 30, 0, 0, 100, 30, 100, 100, 5};
#else
static StatusVulnerability regist = {
    30, // sleep
    30, // stop
    30, // dizzy
    100, // poison
    40, // confuse
    100, // electric
    100, // burn
    25, // freeze
    100, // huge
    75, // tiny
    100, // attackUp
    75, // attackDown
    100, // defenseUp
    80, // defenseDown
    30, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    30, // slow
    100, // dodgy
    100, // invisible
    5, // ohko
};
#endif

/** Pose table of Bowser: animation names for each pose type (idle, walk, hurt, attack and so on). */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "KPA_N_3"},
    {POSE_TYPE_STOP, "KPA_Z_6"},
    {POSE_TYPE_FREEZE, "KPA_Z_6"},
    {POSE_TYPE_CONFUSE, "KPA_K_2"},
    {POSE_TYPE_POISON, "KPA_S_2"},
    {POSE_TYPE_DIZZY, "KPA_S_2"},
    {POSE_TYPE_NORMAL, "KPA_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KPA_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KPA_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KPA_D_1"},
    {POSE_TYPE_DAMAGE, "KPA_D_1"},
    {POSE_TYPE_ATTACK, "KPA_S_1"},
    {POSE_TYPE_RUN, "KPA_R_1"},
    {POSE_TYPE_WALK, "KPA_W_1"},
    {POSE_TYPE_DANGER, "KPA_S_1"},
    {POSE_TYPE_PERIL, "KPA_S_1"},
    {POSE_TYPE_TALK, "KPA_T_1"},
    {POSE_TYPE_ACTION, "KPA_S_1"},
};

/**
 * Pose table of Bowser while leaning forward: animation names for each pose type (idle, walk, hurt, attack
 * and so on).
 */
static PoseTableEntry pose_table_forward_leaning[] = {
    {POSE_TYPE_SLEEP, "KPA_N_2"},
    {POSE_TYPE_STOP, "KPA_Z_5"},
    {POSE_TYPE_FREEZE, "KPA_Z_5"},
    {POSE_TYPE_CONFUSE, "KPA_K_1"},
    {POSE_TYPE_POISON, "KPA_X_1"},
    {POSE_TYPE_DIZZY, "KPA_X_1"},
    {POSE_TYPE_NORMAL, "KPA_S_8"},
    {POSE_TYPE_DAMAGE_FIRE, "KPA_D_4"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KPA_D_4"},
    {POSE_TYPE_DAMAGE_GUARDED, "KPA_D_4"},
    {POSE_TYPE_DAMAGE, "KPA_D_4"},
    {POSE_TYPE_ATTACK, "KPA_S_8"},
    {POSE_TYPE_RUN, "KPA_R_1"},
    {POSE_TYPE_WALK, "KPA_W_1"},
    {POSE_TYPE_DANGER, "KPA_S_8"},
    {POSE_TYPE_PERIL, "KPA_S_8"},
    {POSE_TYPE_TALK, "KPA_T_9"},
    {POSE_TYPE_ACTION, "KPA_S_8"},
};

/**
 * Pose table of Bowser once defeated: animation names for each pose type (idle, walk, hurt, attack and so
 * on).
 */
static PoseTableEntry pose_table_dead[] = {
    {POSE_TYPE_NORMAL, "KPA_Z_3"},
    {POSE_TYPE_DAMAGE, "KPA_Z_3"},
    {POSE_TYPE_ACTION, "KPA_Z_3"},
};

/**
 * Data table of Bowser: event scripts and values the battle looks up by tag (damage reactions, dodge, first
 * strike and so on).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

/** Part of Bowser: its one body part with hit box, defence tables and pose table. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_koopa", "c_koopa", 0.0f, 0.0f, 0.0f, 34.0f, 62.0f, 0.0f, 22.0f, 78.0f, 0.0f, 70, 80, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
};

/** Weapon of Bowser's hip drop: damage 7. */
static BattleWeapon weapon_koopa_hip_attack = {
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
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_PRESS, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

/** Weapon of Bowser's bite: damage 7; inflicts Poison (30%). */
static BattleWeapon weapon_koopa_bite_attack = {
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
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT, // targetWeightFlags
    0, 0, 0, 0, 0, 0, // sleepChance..dizzyTime
    30, // poisonChance
    10, // poisonTime
    1, // poisonStrength
    0, 0, 0, 0, 0, 0, 0, 0, // confuseChance..burnTime
    0, 0, 0, 0, 0, 0, 0, 0, // freezeChance..atkChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeChance..fastChance
    0, 0, 0, 0, 0, 0, 0, 0, // fastTime..invisibleChance
    0, 0, 0, 0, 0, // invisibleTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of Bowser's critical bite: damage 2. */
static BattleWeapon weapon_koopa_critical_bite_attack = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT, // targetWeightFlags
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

/** Weapon of Bowser's fire breath: damage 7. */
static BattleWeapon weapon_koopa_fire_attack = {
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
    {7, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Setup used to spawn Kammy Koopa: unit kind, alliance, position and turn phase. */
BattleUnitSetup entry_kamec_obaba = {
    &unit_boss_kamec_obaba, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/** Camera helper of the Bowser fight: moves the battle camera to a close view of Bowser at the X in LW(0). */
static EVT_BEGIN(koopa_zoom)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_btl_camera_set_moveto, 0, LW(0), 95, 350, LW(0), 45, -300, 30, 11)
    WAIT_FRAMES(37)
    RETURN
EVT_END()

/**
 * Camera helper of the Bowser fight: moves the battle camera to a close view of Kammy Koopa (the unit in GW(9));
 * the view is higher when Kammy's unit work 0 is 1 (she is up on her broom).
 */
static EVT_BEGIN(kamec_obaba_zoom)
    CALL(btlevtcmd_GetUnitWork, GW(9), KAMEC_OBABA_WORK_SKY, LW(1))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    IF_INT_EQUAL(LW(1), 1)
        CALL(evt_btl_camera_set_moveto, 0, 135, 106, 296, 135, 55, -402, 30, 11)
    ELSE
        CALL(evt_btl_camera_set_moveto, 0, 140, 34, 296, 140, 50, -402, 30, 11)
    END_IF
    WAIT_FRAMES(37)
    RETURN
EVT_END()

/**
 * Init event of Bowser: installs his wait, phase, attack, damage, confusion and entry events,
 * spawns Kammy Koopa (entry_kamec_obaba, her unit id in GW(9)) and clears his unit work:
 * 1 (the first-hit cutscene state: 1 once Mario first hurts him, 2 after first_mario_damage_event),
 * 2 (the HP threshold for his half-HP line),
 * 3 (that line already said) and the attack stage counter.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(entry_event))
    CALL(btlevtcmd_SpawnUnit, GW(9), PTR(&entry_kamec_obaba), 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    ADD(LW(1), 40)
    ADD(LW(2), 10)
    CALL(btlevtcmd_SetPos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, GW(9), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    SET_VALUE(GW(8), LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LEANING, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MARIO_HIT_TALK, 0)
    CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
    DIVIDE(LW(0), 2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HALF_HP, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HALF_HP_TALKED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 0)
    SET_VALUE(GW(10), 0)
    SET_VALUE(GW(11), 0)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BOSS_KOOPA_MOVE1L"), STRING("SFX_BOSS_KOOPA_MOVE1R"), 0, 15, 15)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BOSS_KOOPA_MOVE1L"), STRING("SFX_BOSS_KOOPA_MOVE1R"), 0, 8, 8)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Entry event of the Bowser fight: the status window is hidden,
 * Bowser (KPA_S_1 then KPA_Y_2) speaks (stg8_las_96)
 * and Kammy answers (stg8_las_97) with the camera cutting between them.
 */
static EVT_BEGIN(entry_event)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_S_1"))
    WAIT_FRAMES(60)
    CALL_CHILD(koopa_zoom)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_KOOPA_LAUGH2_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_Y_2"))
    WAIT_MS(1000)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_96"), 0, -2)
    WAIT_MS(300)
    CALL_CHILD(kamec_obaba_zoom)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_97"), 0, GW(9))
    WAIT_MS(300)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

/**
 * Damage event of Bowser: when the hit defeats him (damage flag 0x400) sets GW(10) = 1 (he is going down;
 * Kammy's
 * scripts check it), then runs the default damage handling for every hit.
 */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(GW(10), 1)
    END_IF
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

/**
 * Dead event of Bowser: the status window is hidden, the camera goes to Bowser,
 * and depending on how the fight ended (GW(11)) he says stg8_las_100_02 or stg8_las_100_04,
 * and the common slow-death sequence runs with GW(10) set to 2 (the fight is over).
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
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    END_IF
    WAIT_FRAMES(1)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL_CHILD(koopa_zoom)
    SWITCH(GW(11))
        CASE_EQUAL(0)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_KOOPA_SATIATED2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_D_2"))
        WAIT_MS(200)
        CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("KPA_T_8"))
        CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("KPA_Z_3"))
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_100_02"), 0, -2)
        WAIT_MS(300)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_dead))
        SET_VALUE(GW(10), 2)
        SET_VALUE(LW(10), UNIT_ID_SELF)
        CALL_CHILD(subsetevt_dead_core_nospin_norotate)
        CASE_DEFAULT
        CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("KPA_D_1"))
        CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("KPA_D_1"))
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_100_04"), 0, -2)
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_KOOPA_SATIATED2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_D_2"))
        WAIT_MS(700)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_dead))
        SET_VALUE(GW(10), 2)
        SET_VALUE(LW(10), UNIT_ID_SELF)
        CALL_CHILD(subsetevt_dead_core_nospin_norotate)
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

/**
 * Phase event of Bowser: skipped while he is knocked out (status 27, instant KO).
 * Before the player phase it handles Kammy's broom flag (unit work 0),
 * and runs harf_hp_event so he can speak when his HP gets low.
 */
static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckStatus, UNIT_ID_SELF, STATUS_INSTANT_KILL, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_get_turn, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_BEFORE_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LEANING, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_sub_random, 99, LW(1))
                IF_INT_LESS(LW(1), 0)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LEANING, 1)
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_U_2"))
                    WAIT_MS(200)
                    CALL(btlevtcmd_OnPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
                    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_forward_leaning))
                END_IF
            ELSE
                CALL(evt_sub_random, 99, LW(1))
                IF_INT_LESS(LW(1), 100)
                    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LEANING, 0)
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_U_3"))
                    WAIT_MS(200)
                    CALL(btlevtcmd_OffPartsCounterAttribute, UNIT_ID_SELF, 1, kCounterTopSpiky)
                    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
                END_IF
            END_IF
            CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        END_IF
    END_IF
    LABEL(90)
    CALL(btlevtcmd_CheckPhase, LW(0), BATTLE_PHASE_AFTER_PLAYER)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_PhaseEventStartDeclare, UNIT_ID_SELF)
        CALL_CHILD(PTR(harf_hp_event))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    END_IF
    LABEL(99)
    RETURN
EVT_END()

/**
 * Event of Bowser when his HP first falls below the threshold stored in unit work 2:
 * he groans (KPA_U_3), the camera zooms in and he speaks (stg8_las_100) in a new stance (KPA_T_3, KPA_S_4).
 */
static EVT_BEGIN(harf_hp_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HALF_HP_TALKED, LW(1))
    IF_INT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HALF_HP, LW(1))
        IF_INT_LESS(LW(0), LW(1))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HALF_HP_TALKED, 1)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LEANING, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_U_3"))
                WAIT_MS(200)
            END_IF
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL_CHILD(PTR(koopa_zoom))
            CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("KPA_T_3"))
            CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("KPA_S_4"))
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_100"), 0, -2)
            WAIT_MS(300)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LEANING, LW(0))
            IF_INT_EQUAL(LW(0), 1)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_U_2"))
                WAIT_MS(200)
            END_IF
            CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Attack event of Bowser: can use an item; otherwise the first three turns go in order (fire breath, bite, hip
 * drop) and afterwards he draws lots among the hip drop, bite, critical bite and fire breath.
 * A first-hit reaction from Mario (first_mario_damage_event) can interrupt.
 */
static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(9), PTR(&weapon_koopa_fire_attack))
        CALL_CHILD(PTR(fire_attack_event))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 1)
        CASE_EQUAL(1)
        SET_VALUE(LW(9), PTR(&weapon_koopa_bite_attack))
        CALL_CHILD(PTR(bite_attack_event))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 2)
        CASE_EQUAL(2)
        SET_VALUE(LW(9), PTR(&weapon_koopa_hip_attack))
        CALL_CHILD(PTR(hip_attack_event))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 3)
        CASE_DEFAULT
#if VERSION <= VERSION_DEMO_US
        CALL(btlevtcmd_DrawLots, LW(0), 4, 20, 30, 0, 30)
#else
        CALL(btlevtcmd_DrawLots, LW(0), 4, 20, 20, 0, 40)
#endif
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_koopa_hip_attack))
            CALL_CHILD(PTR(hip_attack_event))
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_koopa_bite_attack))
            CALL_CHILD(PTR(bite_attack_event))
            CASE_EQUAL(2)
            SET_VALUE(LW(9), PTR(&weapon_koopa_critical_bite_attack))
            CALL_CHILD(PTR(critical_bite_attack_event))
            CASE_EQUAL(3)
            SET_VALUE(LW(9), PTR(&weapon_koopa_fire_attack))
            CALL_CHILD(PTR(fire_attack_event))
        END_SWITCH
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(PTR(first_mario_damage_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Hip drop attack of Bowser (weapon_koopa_hip_attack):
 * he jumps up and slams down on one target, which can dodge
 * or takes the damage with the action command defence check and the camera shakes;
 * it can also hit Kammy's side when he is confused.
 */
static EVT_BEGIN(hip_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    // Pick a target for the attack; a confused unit that has none flusters.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_KIND, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    // Announce the attack; the camera swings to follow it.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    // Bowser walks up to the target.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOOPA_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    // He crouches, gathering himself for the jump.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_2A"))
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_2B"))
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(25)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_2C"))
        WAIT_FRAMES(15)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_2D"))
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.25f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 20)
    // He leaps high into the air.
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 50, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOOPA_HIP_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    LABEL(90)
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    // He lands with a crash and the camera shakes.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_2E"))
    WAIT_MS(500)
    GOTO(98)
    // The attack connects.
    LABEL(91)
    // Deal the damage, reduced by the action command defence.
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(evt_btl_camera_shake_h, 0, 3, 0, 50, 13)
    CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_2E"))
    WAIT_MS(300)
    CALL(btlevtcmd_GetResultACDefence, LW(7))
    IF_INT_LESS_OR_EQUAL(LW(7), 3)
        SET_VALUE(LW(15), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_KIND, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(UNIT_MARIO)
            CALL(mario_cannot_use_command, LW(15))
            CASE_BETWEEN(UNIT_GOOMBELLA, UNIT_MS_MOWZ)
#if VERSION >= VERSION_US
            CALL(party_cannot_use_command, LW(15), LW(3))
#else
            CALL(party_cannot_use_command, LW(15))
#endif
            CASE_DEFAULT
            SET_VALUE(LW(0), -1)
        END_SWITCH
        IF_INT_NOT_EQUAL(LW(0), -1)
            IF_INT_NOT_EQUAL(LW(15), 0)
                BEGIN_INLINE_EVENT
                    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(5), LW(6), LW(7))
                    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(5), 60)
                    CALL(btlevtcmd_TransStageFloorPosition, LW(5), LW(6), LW(7))
                    CALL(_icon_jump_icon, LW(15), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(0.5f), 30)
                    SET_VALUE(LW(0), LW(5))
                    SET_VALUE(LW(1), LW(6))
                    SET_VALUE(LW(2), LW(7))
                    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(5), 40)
                    CALL(_icon_jump_icon, LW(15), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(0.5f), 20)
                    SET_VALUE(LW(0), LW(5))
                    SET_VALUE(LW(1), LW(6))
                    SET_VALUE(LW(2), LW(7))
                    CALL(btlevtcmd_FaceDirectionSub, LW(3), LW(5), 25)
                    CALL(_icon_jump_icon, LW(15), LW(0), LW(1), LW(2), LW(5), LW(6), LW(7), FLOAT(0.5f), 12)
                END_INLINE_EVENT
            END_IF
            BEGIN_SIBLING_EVENT
                CALL(btlevtcmd_AnnounceMessage, 0, 0, 0, LW(0), 120)
            END_SIBLING_EVENT
        END_IF
    END_IF
    // Return to its place.
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)
    // He turns and walks back to his place.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_S_1"))
    CALL_CHILD(first_mario_damage_check)
    RETURN
EVT_END()

/**
 * Bite attack of Bowser (weapon_koopa_bite_attack): he runs at one target and bites;
 * the target can dodge or takes the damage with the action command defence check.
 */
static EVT_BEGIN(bite_attack_event)
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
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_KIND, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 65)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_3A"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOOPA_BITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(170)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_3B"))
    WAIT_MS(150)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOOPA_BITE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    LABEL(90)
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_3C"))
    WAIT_MS(150)
    GOTO(98)
    LABEL(91)
    WAIT_MS(100)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_3C"))
    WAIT_MS(150)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_S_1"))
    CALL_CHILD(first_mario_damage_check)
    RETURN
EVT_END()

/**
 * Critical bite attack of Bowser (weapon_koopa_critical_bite_attack):
 * a stronger bite on one target, with the same dodge or defence check.
 */
static EVT_BEGIN(critical_bite_attack_event)
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
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_KIND, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 65)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_3A"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOOPA_BITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(170)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_3B"))
    WAIT_MS(150)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOOPA_BITE3"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    LABEL(90)
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_3C"))
    WAIT_MS(150)
    GOTO(98)
    LABEL(91)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_4"))
    WAIT_MS(1500)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_3C"))
    WAIT_MS(150)
    LABEL(98)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL_CHILD(first_mario_damage_check)
    RETURN
EVT_END()

/**
 * Fire breath attack of Bowser (weapon_koopa_fire_attack):
 * he inhales (KPA_A_1A) and breathes a stream of fire at one target,
 * which can dodge or takes the damage with the action command defence check.
 */
static EVT_BEGIN(fire_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    // Pick a target for the attack; a confused unit that has none flusters.
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitKind, LW(3), LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_KIND, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    // Announce the attack; the camera swings to follow it.
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 120)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    // Bowser walks up to the target.
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 1)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    WAIT_MS(170)
    // He inhales, then breathes a stream of fire.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_1A"))
    WAIT_MS(300)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_KOOPA_FIRE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOOPA_FIRE_BREATH1"), EVT_NULLPTR, 0, LW(15))
    BEGIN_INLINE_EVENT
        WAIT_MS(1500)
        CALL(evt_snd_sfxoff, LW(15))
    END_INLINE_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_1B"))
    WAIT_MS(300)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(5))
    SET_FLOAT_VALUE(LW(6), FLOAT(60.0f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(6))
    SET_FLOAT_VALUE(LW(6), FLOAT(22.5f))
    MULTIPLY_FLOAT(LW(6), LW(5))
    ADD_FLOAT(LW(1), LW(6))
    SUBTRACT(LW(2), 5)
    MULTIPLY_FLOAT(LW(5), FLOAT(3.5f))
    CALL(evt_eff, STRING(""), STRING("gonbaba_breath"), 7, LW(0), LW(1), LW(2), LW(5), 90, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_1C"))
    WAIT_MS(300)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    // Find out whether the attack will connect or miss.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    GOTO(97)
    // The attack connects.
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        // Deal the damage, reduced by the action command defence.
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(97)
    CALL_CHILD(first_mario_damage_check)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    LABEL(98)
    WAIT_MS(1200)
    // He lowers his head and the fire dies down.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_A_1D"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(250)
    // He walks back to his place.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_R_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    RETURN
EVT_END()

/**
 * Event script (Bowser's first-damage watcher):
 * if Bowser's work[1] is 0 and work[5] is 222, then
 * when the damage dealt to him is nonzero it sets work[1] to 1,
 * flagging that Mario has hurt him for the first time.
 */
EVT_BEGIN(first_mario_damage_check)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MARIO_HIT_TALK, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_TARGET_KIND, LW(0))
        IF_INT_EQUAL(LW(0), UNIT_MARIO)
            CALL(btlevtcmd_GetTotalDamage, LW(3), LW(1))
            IF_INT_NOT_EQUAL(LW(1), 0)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MARIO_HIT_TALK, 1)
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Event script: the cutscene after Mario first damages Bowser (work[1] is 1, then set to 2).
 * The camera zooms on Bowser, who laughs the hit off and speaks (stg8_las_98), then on Kammy Koopa,
 * who adds a line (stg8_las_99) if she is still in the battle (GW(11) is 0).
 */
EVT_BEGIN(first_mario_damage_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MARIO_HIT_TALK, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MARIO_HIT_TALK, 2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_S_1"))
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
        CALL_CHILD(koopa_zoom)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LEANING, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_U_3"))
            WAIT_MS(200)
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_VOICE_KOOPA_LAUGH2_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_Y_2"))
        WAIT_MS(1000)
        CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("KPA_T_2"))
        CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("KPA_S_3"))
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_98"), 0, -2)
        WAIT_MS(300)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_S_1"))
        IF_INT_EQUAL(GW(11), 0)
            CALL_CHILD(kamec_obaba_zoom)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg8_las_99"), 0, GW(9))
            WAIT_MS(300)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LEANING, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KPA_U_2"))
            WAIT_MS(200)
        END_IF
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    END_IF
    RETURN
EVT_END()

/** Wait event of Bowser: plays his idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Message keys shown when a battle command is disabled:
 * jump, hammer, item, partner technique and partner item.
 */
static const char* cmd_disable_string[] = {
    "btl_jump_cmd_disable",
    "btl_hammer_cmd_disable",
    "btl_item_cmd_disable",
    "btl_party_tech_cmd_disable",
    "btl_party_item_cmd_disable",
};

/**
 * Draw callback for the jumping command icon: draws the icon at the position stored in the IconJumpWork of the
 * event.
 */
static void _disp_icon(CameraId camera, void* param) {
    EventEntry* event = param;

    iconDispGx(&((IconJumpWork*)event->userdatap[0])->pos, 0, event->userdata[1], 1.0f);
}

/**
 * Script helper: animates a command icon (icon id args[0]) on a parabolic jump from (args[1..3]) to (args[4..6])
 * with gravity args[7] over args[8] frames, drawn each frame; blocks until it lands.
 * Used to show a battle command icon being thrown away when Bowser disables it.
 */
static USER_FUNC(_icon_jump_icon) {
    EvtWord* args = event->args;
    s32 icon = evtGetValue(event, args[0]);
    f32 x0 = evtGetFloat(event, args[1]);
    f32 y0 = evtGetFloat(event, args[2]);
    f32 z0 = evtGetFloat(event, args[3]);
    f32 x1 = evtGetFloat(event, args[4]);
    f32 y1 = evtGetFloat(event, args[5]);
    f32 z1 = evtGetFloat(event, args[6]);
    f32 gravity = evtGetFloat(event, args[7]);
    s32 frames = evtGetValue(event, args[8]);
    IconJumpWork* wk;

    if (isFirstCall) {
        Vec start;

        event->userdatap[0] = BattleAlloc(sizeof(IconJumpWork));
        wk = event->userdatap[0];
        wk->end.x = x1;
        wk->end.y = y1;
        wk->end.z = z1;
        start.x = x0;
        start.y = y0;
        start.z = z0;
        wk->pos = start;
        wk->start = start;
        wk->gravity = gravity;
        wk->frames = frames;
        wk->vy = (wk->end.y - wk->start.y) / wk->frames + wk->gravity * wk->frames / 2;
        wk->speed = distABf(wk->start.x, wk->start.z, wk->end.x, wk->end.z) / wk->frames;
        event->userdata[1] = icon;
        event->userdata[2] = 0;
    }
    wk = event->userdatap[0];
    if (event->userdata[2]) {
        BattleFree(wk);
        return EVT_RETURN_DONE;
    }
    wk->pos.y += wk->vy;
    wk->vy -= wk->gravity;
    if (wk->vy < 0.0f && wk->pos.y < wk->end.y) {
        wk->pos.y = wk->end.y;
    }
    btlMovePos(wk->speed, angleABf(wk->start.x, wk->start.z, wk->end.x, wk->end.z), &wk->pos.x, &wk->pos.z);
    wk->frames--;
    if (wk->frames <= 0) {
        event->userdata[2] = 1;
        wk->pos = wk->end;
    }
    dispEntry(CAMERA_3D, 900.0f, 1, _disp_icon, event);
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper: Bowser disables one of the partner's commands for 3 turns at random (a coin flip between two
 * choices; the US version stores the turn count on the partner unit given in args[1]).
 * Returns in args[0] the id of the weapon/message of the disabled command and puts the matching string in LW(0).
 */
static USER_FUNC(party_cannot_use_command) {
    BattleWork* wp = _battleWorkPointer;
    s32 r = irand(1);
    s32 var = event->args[0];
#if VERSION >= VERSION_US
    s32 id = evtGetValue(event, event->args[1]);
    s32 weapon = 0;
    BattleUnit* unit;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
#else
    s32 weapon = 0;
#endif
#if VERSION >= VERSION_US

    switch (r) {
        case 0:
            if (unit != NULL) {
                unit->usCommandDisabled[0] = 3;
            }
            weapon = 344;
            break;
        case 1:
            if (unit != NULL) {
                unit->usCommandDisabled[1] = 3;
            }
            weapon = 340;
#else
    switch (r) {
        case 0:
            wp->commandDisabled[3] = 3;
            weapon = 336;
            break;
        case 1:
            wp->commandDisabled[4] = 3;
            weapon = 332;
#endif
            break;
    }
    event->lwData[0] = (EvtWord)cmd_disable_string[r + 3];
    evtSetValue(event, var, weapon);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: Bowser disables one of Mario's three commands (the first three command slots) for 3 turns at
 * random. Returns in args[0] the id of the matching weapon/message and puts the command's name string in LW(0).
 */
static USER_FUNC(mario_cannot_use_command) {
    BattleWork* wp = _battleWorkPointer;
    s32 r = irand(2);
    s32 var = event->args[0];
    s32 weapon = 0;

    switch (r) {
        case 0:
            wp->commandDisabled[0] = 3;
#if VERSION >= VERSION_US
            weapon = 334;
#else
            weapon = 326;
#endif
            break;
        case 1:
            wp->commandDisabled[1] = 3;
#if VERSION >= VERSION_US
            weapon = 337;
#else
            weapon = 329;
#endif
            break;
        case 2:
            wp->commandDisabled[2] = 3;
#if VERSION >= VERSION_US
            weapon = 340;
#else
            weapon = 332;
#endif
            break;
    }
    event->lwData[0] = (EvtWord)cmd_disable_string[r];
    evtSetValue(event, var, weapon);
    return EVT_RETURN_DONE;
}

