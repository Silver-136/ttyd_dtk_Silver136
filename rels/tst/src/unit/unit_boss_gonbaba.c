// Battle unit Hooktail (`boss_gonbaba`): tst.rel's copy of rels/gon/src/unit/unit_boss_gonbaba.c.
#include "version.h"
#include "battle/battle_unit.h"
#include "party/party_motion.h"
#include "evt/evt_cmd.h"

#include "battle/battle_unit.h"
#include "manager/evtmgr_cmd.h"
#include "system.h"
#include "stdlib.h"
#include "dolphin/mtx.h"
#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_batstage.h"
#include "evt/evt_eff.h"
#include "evt/evt_map.h"
#include "evt/evt_msg.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "evt/evt_sub.h"
#include "driver/lightdrv.h"
#include "eff/n64/eff_kemuri9_n64.h"

// Unit work slots of Hooktail.
enum {
    UNIT_WORK_POWER_DOWN = 0,    // Attack FX R cricket hits so far (up to 20); taken off her attack power
    UNIT_WORK_PART_DEFENSE = 1, // defence change from cricket hits (down to -2), read by GONBABA_DEFENSE_ATTR
    UNIT_WORK_MESSAGE_STEP = 2,   // which cricket speech she gives next (0-3)
    UNIT_WORK_NEGOTIATED = 3,      // 1 once her plea for mercy (negotiation_event) has run
    UNIT_WORK_EATEN_AUDIENCE = 4, // swallowed spectators left that a head hit can shake loose
    UNIT_WORK_ATTACK_STEP = 5,   // first attacks: 0 fire breath next, 1 stomp next, 2 random
};

u32 weaponGetPower_Gonbaba(BattleUnit* attacker, BattleWeapon* weapon, BattleUnit* target, BattleUnitPart* part);
static USER_FUNC(_leg_smoke_effect);
USER_FUNC(_btlgon_get_escape_param);
USER_FUNC(_set_gonbaba_light);
USER_FUNC(_wait_damage_event_end);

BattleWeapon weapon_gonbaba_fumituke_attack;
BattleWeapon weapon_gonbaba_firebreath_attack;
BattleWeapon weapon_gonbaba_kamituki_attack;
// tst's copy of Hooktail still has the older bit for the same attribute.
#if VERSION == VERSION_DEMO_JP
#define GONBABA_DEFENSE_ATTR (1 << 5)
#else
#define GONBABA_DEFENSE_ATTR kPartUnitWorkDefense
#endif
BattleUnitKind unit_boss_gonbaba;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
#if VERSION <= VERSION_DEMO_US
static PoseTableEntry pose_table[7];
static PoseTableEntry pose_table_weak[7];
#else
#if VERSION <= VERSION_DEMO_US
static PoseTableEntry pose_table[7];
#else
static PoseTableEntry pose_table[9];
#endif
#if VERSION <= VERSION_DEMO_US
static PoseTableEntry pose_table_weak[7];
#else
static PoseTableEntry pose_table_weak[9];
#endif
#endif
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[3];
extern EvtWord negotiation_event[];
extern EvtWord gon_extra_eaten_event[];
extern EvtWord gonbaba_damage_sub_event[];
#if VERSION <= VERSION_DEMO_US
static EvtWord dead_event[281];
#else
#if VERSION <= VERSION_DEMO_US
static EvtWord dead_event[281];
#else
static EvtWord dead_event[283];
#endif
#endif
static EvtWord init_event[160];
static EvtWord attack_event[56];
static EvtWord fumituke_attack_event[386];
extern EvtWord firebreath_attack_event[];
static EvtWord wait_event[6];

/** Hooktail's attack power: the weapon's base power minus the attack drop in the attacker's work[0] (at least 1). */
u32 weaponGetPower_Gonbaba(BattleUnit* attacker, BattleWeapon* weapon, BattleUnit* target, BattleUnitPart* part) {
    s32 power = weapon->hpDamageWork[0];

    power -= attacker->work[UNIT_WORK_POWER_DOWN];
    if (power <= 0) {
        power = 1;
    }
    return power;
}

/**
 * Script helper: puffs a ring of dust (effKemuri9N64, 18 puffs, 90 frames, radius 60) at the x, y,
 * z in args[0]-args[2]; Hooktail's footsteps use it.
 */
static USER_FUNC(_leg_smoke_effect) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    effKemuri9N64Entry(1, 18, 90, x, y, z, 60.0f, 0.0f);
    return EVT_RETURN_DONE;
}

/** Attack data of Hooktail's gonbaba fumituke attack: base damage 5, 100% accuracy. */
BattleWeapon weapon_gonbaba_fumituke_attack = {
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
    weaponGetPower_Gonbaba, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    11, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 10, 0, 0, 0}, // stageEffectRates
};

/** Attack data of Hooktail's gonbaba firebreath attack: base damage 4, 100% accuracy. */
BattleWeapon weapon_gonbaba_firebreath_attack = {
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
    weaponGetPower_Gonbaba, // hpDamageFn
    {4, 50, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_BUFFABLE | SPECIAL_STATUS_BUFFABLE | SPECIAL_CHARGE_BUFFABLE | SPECIAL_PIERCE_DEFENSE, // special
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

/** Attack data of Hooktail's gonbaba kamituki attack: base damage 5, 100% accuracy. */
BattleWeapon weapon_gonbaba_kamituki_attack = {
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
    weaponGetPowerDefault, // hpDamageFn
    {5, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
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

/**
 * Unit kind of Hooktail: 20 max HP, level 65. The battle engine reads it when the unit spawns:
 * size, offsets, sounds, status vulnerability, parts, init script and event table.
 */
#if VERSION <= VERSION_DEMO_US
BattleUnitKind unit_boss_gonbaba = {
    23,
    "btl_un_gonbaba",
    20,
    0,
    1,
    1,
    65,
    0,
    0,
    100,
    0,
    50,
    9,
    230,
    170,
    -210,
    45,
    0.0f,
    0.0f,
    0.0f,
    -250,
    0,
    280.0f,
    40.0f,
    0.0f,
    115.0f,
    0.0f,
    -10.0f,
0.0f,
0.0f,
    0.0f,
250.0f,
0.0f,
    250.0f,
    0.0f,
    0.0f,
    115.0f,
    110.5f,
    0.0f,
    -290.0f,
    85.0f,
    0.0f,
    230.0f,
    170.0f,
    0,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_GNB_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kUnitNoSpin,
    &regist,
    3,
    parts,
    init_event,
    data_table,
};
#else
BattleUnitKind unit_boss_gonbaba = {
    23,
    "btl_un_gonbaba",
    20,
    0,
    1,
    1,
    65,
    0,
    0,
    100,
    0,
    50,
    9,
    230,
    170,
    -210,
    45,
    0.0f,
    0.0f,
    0.0f,
    -250,
    0,
    280.0f,
    40.0f,
    0.0f,
    115.0f,
    0.0f,
    -10.0f,
#if VERSION >= VERSION_US
    115.0f,
#else
    0.0f,
#endif
    0.0f,
    0.0f,
#if VERSION <= VERSION_DEMO_US
    250.0f,
#else
    20.0f,
#endif
    0.0f,
    250.0f,
    0.0f,
    0.0f,
    115.0f,
    110.5f,
    0.0f,
    -290.0f,
    85.0f,
    0.0f,
    230.0f,
    170.0f,
    0,
    0,
    -1,
    0,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_GNB_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kUnitNoSpin,
    &regist,
    3,
    parts,
    init_event,
    data_table,
};
#endif

/**
 * Defence of Hooktail per element (normal, fire, ice, explosion, electric), subtracted from each hit:
 * 1 against every element.
 */
static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

/** Element reactions of Hooktail: none, every element hits normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

#if VERSION <= VERSION_DEMO_US
/**
 * Status vulnerability of Hooktail (the demos), as the percent chance a status takes hold:
 * immune to Stop, Poison, Freeze, Huge, Tiny, Attack Down, Defense Down, Fright, Gale Force, Slow, Dodgy,
 * Invisible and instant KO; always hit by Burn, Defense Up, Allergic and Fast;
 * otherwise Sleep 10, Dizzy 50, Confuse 50, Electric 10 and Attack Up 50.
 */
static StatusVulnerability regist = {10, 0, 50, 0, 50, 10, 100, 0, 0, 0, 50, 0, 100, 0, 100, 0, 0, 100, 0, 0, 0, 0};
#else
/**
 * Status vulnerability of Hooktail (the final versions), as the percent chance a status takes hold:
 * immune to Stop, Poison, Freeze, Huge, Tiny, Attack Down, Defense Down, Fright, Gale Force, Slow, Dodgy,
 * Invisible and instant KO; always hit by Burn, Defense Up, Allergic and Fast;
 * otherwise Sleep 20, Dizzy 50, Confuse 50, Electric 10 and Attack Up 50.
 */
#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {10, 0, 50, 0, 50, 10, 100, 0, 0, 0, 50, 0, 100, 0, 100, 0, 0, 100, 0, 0, 0, 0};
#else
static StatusVulnerability regist = {20, 0, 50, 0, 50, 10, 100, 0, 0, 0, 50, 0, 100, 0, 100, 0, 0, 100, 0, 0, 0, 0};
#endif
#endif

/**
 * Pose table of Hooktail: maps its battle pose types (stay, walk, hurt, dead, talk and so on) to
 * animation names, 9 entries such as GNB_N_1, GNB_X_1, GNB_Z_1, GNB_S_3.
 */
#if VERSION <= VERSION_DEMO_US
static PoseTableEntry pose_table[] = {
    {1, "GNB_N_1"},
    {5, "GNB_X_1"},
{BTL_POSE_NORMAL, "GNB_S_3"},
    {POSE_TYPE_TALK, "GNB_T_3"},
    {31, "GNB_S_3"},
    {BTL_POSE_DAMAGE, "GNB_D_1"},
    {POSE_TYPE_ACTION, "GNB_S_3"},
};
#else
static PoseTableEntry pose_table[] = {
    {1, "GNB_N_1"},
    {5, "GNB_X_1"},
#if VERSION >= VERSION_JP
    {2, "GNB_Z_1"},
    {3, "GNB_X_1"},
#endif
    {BTL_POSE_NORMAL, "GNB_S_3"},
    {POSE_TYPE_TALK, "GNB_T_3"},
    {31, "GNB_S_3"},
    {BTL_POSE_DAMAGE, "GNB_D_1"},
    {POSE_TYPE_ACTION, "GNB_S_3"},
};
#endif

/**
 * Pose table of Hooktail once she is weakened, installed by gonbaba_damage_sub_event on each hit with the
 * Attack FX R sound (which she takes for a cricket): stay and talk become GNB_S_4 and GNB_T_4, pose 39
 * becomes GNB_V_1. negotiation_event puts the normal table back.
 */
#if VERSION <= VERSION_DEMO_US
static PoseTableEntry pose_table_weak[] = {
    {1, "GNB_N_1"},
    {5, "GNB_X_1"},
{BTL_POSE_NORMAL, "GNB_S_4"},
    {POSE_TYPE_TALK, "GNB_T_4"},
    {31, "GNB_S_4"},
    {BTL_POSE_DAMAGE, "GNB_V_1"},
    {POSE_TYPE_ACTION, "GNB_S_4"},
};
#else
static PoseTableEntry pose_table_weak[] = {
    {1, "GNB_N_1"},
    {5, "GNB_X_1"},
#if VERSION >= VERSION_JP
    {2, "GNB_N_1"},
    {3, "GNB_X_1"},
#endif
    {BTL_POSE_NORMAL, "GNB_S_4"},
    {POSE_TYPE_TALK, "GNB_T_4"},
    {31, "GNB_S_4"},
    {BTL_POSE_DAMAGE, "GNB_V_1"},
    {POSE_TYPE_ACTION, "GNB_S_4"},
};
#endif

/**
 * Event table of Hooktail: maps event tags to the scripts that react to them (gonbaba_damage_sub_event
 * and dead_event); tag 0 ends it.
 */
static BattleDataEntry data_table[] = {
    {9, gonbaba_damage_sub_event},
    {49, dead_event},
    {0, NULL},
};

/**
 * Body parts of Hooktail: 5 parts (models c_gonbaba and c_gonbaba4); the first is the body.
 * Each part names its defence tables and pose table.
 */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_gonbaba", "c_gonbaba", 0.0f, 0.0f, 0.0f, 230.0f, 36.0f, 60.0f, 230.0f, 36.0f, 60.0f, 20, 30, 255, &defence, &defence_attr, kImportantPart | GONBABA_DEFENSE_ATTR, 0, pose_table},
    {2, "btl_un_gonbaba", "c_gonbaba", 100.0f, 0.0f, 0.0f, 250.0f, 110.0f, 25.0f, 250.0f, 110.0f, 25.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_MAIN | PART_ATTR_GUARD_COVER | GONBABA_DEFENSE_ATTR | PART_ATTR_HAMMERLIKE_CANNOT_TARGET | PART_ATTR_SHELL_TOSSLIKE_CANNOT_TARGET | PART_ATTR_HIDDEN, 0, pose_table},
    {3, "btl_un_gonbaba", "c_gonbaba4", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, PART_ATTR_NOT_TARGETABLE | PART_ATTR_HIDDEN, 0, pose_table},
};

/**
 * Intro of the Hooktail fight: the camera frames the arena, Mario and his partner walk in and start,
 * Hooktail shows up with SFX_BOSS_GNB_APPEAR1 (pose GNB_F_3)
 * and they react with a surprised voice (SFX_VOICE_MARIO_SURPRISED2_2).
 * She then speaks the stg1_gon_20_3 line (talk pose GNB_T_3, rest pose GNB_S_3) before the first turn.
 */
static EVT_BEGIN(battle_entry_event)
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_map_replayanim, 0, STRING("dontyo"))
    // The camera frames the arena while Mario and his partner are placed at the stage's edge, Hooktail still off screen.
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 0, 110, 1080, 0, 93, -2, 1, 0)
    CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_PARTNER, LW(3), LW(4), LW(5))
    CALL(btlevtcmd_SetPos, UNIT_ID_MARIO, LW(6), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_PARTNER, LW(6), LW(4), LW(5))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, LW(3), EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), LW(3))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_FRAMES(60)
    // Mario and his partner walk in side by side.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(0), POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_PARTNER, LW(3), LW(4), LW(5))
    SET_VALUE(LW(8), LW(0))
    SUBTRACT(LW(8), LW(3))
    IF_INT_LESS(LW(8), 0)
        MULTIPLY(LW(8), -1)
    END_IF
    CALL(btlevtcmd_GetStageSize, LW(6), EVT_NULLPTR, EVT_NULLPTR)
    SET_VALUE(LW(7), LW(6))
    ADD(LW(7), LW(8))
    MULTIPLY(LW(6), -1)
    MULTIPLY(LW(7), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_MARIO, LW(6), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_PARTNER, LW(7), LW(4), LW(5))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_PARTNER, FLOAT(6.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_PARTNER, LW(3), LW(4), LW(5), 0, -1, 0)
        CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(0), BTL_POSE_STAY)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_MARIO, FLOAT(6.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 0, -1, 0)
    // Mario stands ready and Hooktail rears up behind them.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_F_3"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ftomsec, 10, LW(0))
    WAIT_MS(LW(0))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 5, LW(0))
        WAIT_MS(LW(0))
        // Mario jumps in fright with a surprised cry and his partner hops back too.
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_O"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_MARIO, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_SURPRISED2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 25, -1)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 5, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_PARTNER, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_PARTNER, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_PARTNER, LW(0), LW(1), LW(2), 25, -1)
        CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(0), 5)
    END_INLINE_EVENT
    // Hooktail appears with a roar and the camera shakes.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 8, 0, 20, 13)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(0), BTL_POSE_STAY)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 60, 0)
    // Hooktail speaks her first line from her talking pose (the snack-time taunt).
    WAIT_FRAMES(60)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_3"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_3"))
    CALL(evt_msg_print, 2, STRING("stg1_gon_20_3"), 0, UNIT_ID_SELF)
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_MARIO)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    RETURN
EVT_END()

/**
 * Script helper: args[0], args[1] are the x and y a spectator stands at;
 * args[2]-args[4] receive how far to run (dx, dy: a 500-unit vector pointing away from the stage centre,
 * direction limited to the left half) and a random speed between 4 and 8 for the audience fleeing Hooktail.
 */
USER_FUNC(_btlgon_get_escape_param) {
    EvtWord* args = event->args;
    s32 x = evtGetValue(event, args[0]);
    s32 y = evtGetValue(event, args[1]);
    s32 dxVar = args[2];
    s32 dyVar = args[3];
    s32 speedVar = args[4];
    f32 angle = angleABf(100.0f, 205.0f, x, y);
    s32 dx;
    s32 dy;
    f32 speed;

    if (angle > 270.0f && angle < 360.0f) {
        angle = 270.0f;
    }
    if (angle < 90.0f) {
        angle = 90.0f;
    }
    dx = 500.0f * sinfd(angle);
    dy = 500.0f * -cosfd(angle);
    speed = 6.0f + rand() % 6;
    speed = 0.67f * speed;
    evtSetValue(event, dxVar, dx);
    evtSetValue(event, dyVar, dy);
    evtSetFloat(event, speedVar, speed);
    return EVT_RETURN_DONE;
}

/**
 * Hooktail's surrender scene, run by her KO event the first time her HP runs out: she begs Mario to stop
 * (stg1_gon_23, 24) and offers in turn coins (24), a rare badge (26) and a sniff of her feet (28), each a yes/no
 * choice (LW(10) = offer, LW(5) = the accepted one); Mario shakes his head at each refusal.
 * If he accepts one she bites him as he walks up (weapon_gonbaba_kamituki_attack), gloats that he was duped
 * (stg1_gon_30, with Goombella's or Koops' reply) and the fight goes on.
 * If he refuses all three she grumbles that he is not very trusting (stg1_gon_31).
 * Either way she then sets unit work 3, lunges at the stands, takes away audience members
 * (gon_extra_eaten_event) and is set to 10 HP.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Hooktail's surrender scene, run by her KO event the first time her HP runs out: she begs Mario to stop
 * (stg1_gon_23, 24) and offers in turn coins (24), a rare badge (26) and a sniff of her feet (28), each a yes/no
 * choice (LW(10) = offer, LW(5) = the accepted one); Mario shakes his head at each refusal.
 * If he accepts one she bites him as he walks up (weapon_gonbaba_kamituki_attack), gloats that he was duped
 * (stg1_gon_30, with Goombella's or Koops' reply) and the fight goes on.
 * If he refuses all three she grumbles that he is not very trusting (stg1_gon_31).
 * Either way she then sets unit work 3, lunges at the stands, takes away audience members
 * (gon_extra_eaten_event) and is set to 10 HP.
 */
EVT_BEGIN(negotiation_event)
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    // Hooktail's HP has run out: her status effects are cleared, her normal pose table comes back with the
    // weakened stay and talk poses (GNB_S_4, GNB_T_4), and the camera closes in.
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_reset_move_color_lv_all)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_4"))
    // She yells for Mario to wait.
    CALL(evt_msg_print, 2, STRING("stg1_gon_23"), 0, UNIT_ID_SELF)
    // Wait until Mario is back at his home position.
    LABEL(0)
    CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(3), LW(4), LW(5))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -431, 10, 735, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 0)
    // She says she gives up and offers 1,000 coins in apology, a yes/no choice.
    CALL(evt_msg_print, 2, STRING("stg1_gon_24"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_24_yesno1"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 0)
        // If Mario accepts, she is delighted and tells him to come and collect.
        CALL(evt_msg_print, 2, STRING("stg1_gon_25"), 0, UNIT_ID_SELF)
        GOTO(40)
    END_IF
    // If he refuses, Mario shakes his head and she offers a rare badge instead, another yes/no choice.
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_NO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_MARIO, 1, PART_ANIM_RESTART, 1)
    WAIT_MS(667)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 1)
    CALL(evt_msg_print, 2, STRING("stg1_gon_26"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_26_yesno1"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 1)
        // If he accepts the badge, she tells him to come and get it.
        CALL(evt_msg_print, 2, STRING("stg1_gon_27"), 0, UNIT_ID_SELF)
        GOTO(40)
    END_IF
    // If he refuses again, he shakes his head and she offers to let him smell her feet, a last yes/no choice.
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_NO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_MARIO, 1, PART_ANIM_RESTART, 1)
    WAIT_MS(667)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 2)
    CALL(evt_msg_print, 2, STRING("stg1_gon_28"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_28_yesno1"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 2)
        // If he accepts, she tells him to come and sniff.
        CALL(evt_msg_print, 2, STRING("stg1_gon_29"), 0, UNIT_ID_SELF)
        GOTO(40)
    END_IF
    // If he refuses all three offers, he shakes his head once more, she complains that he is not very
    // trusting and the bite is skipped.
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_NO2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_MARIO, 1, PART_ANIM_RESTART, 1)
    WAIT_MS(1000)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 15, 0)
    WAIT_FRAMES(15)
    CALL(evt_msg_print, 2, STRING("stg1_gon_31"), 0, UNIT_ID_SELF)
    GOTO(50)
    LABEL(40)
    // He accepted an offer: if Koops' Shell Shield guards Mario, it opens (its event 59) until he is back.
    CALL(btlevtcmd_GetGuardKouraId, UNIT_ID_MARIO, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_RunDataEventChild, LW(3), 59)
        CALL(btlevtcmd_SetUnitWork, LW(3), 2, 0)
    END_IF
    // Mario walks up to take the offer.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_MARIO, FLOAT(3.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 400)
    CALL(btlevtcmd_MovePosition, UNIT_ID_MARIO, LW(0), 0, LW(2), 0, -1, 0)
    IF_INT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_C_4"))
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, BTL_POSE_STAY)
    END_IF
    WAIT_MS(700)
    // As he comes close, Hooktail bites him (GNB_A_2) and the bite's damage is checked against his guard.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_BITE1"), EVT_NULLPTR, 0, -220, 80, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 15, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_BITE2"), EVT_NULLPTR, 0, -260, 40, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A_2"))
    CALL(btlevtcmd_ftomsec, 20, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_MARIO, LW(4))
    CALL(btlevtcmd_ResultACDefence, UNIT_ID_MARIO, PTR(&weapon_gonbaba_kamituki_attack))
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_MARIO, LW(4), PTR(&weapon_gonbaba_kamituki_attack), 1048832, LW(5))
    WAIT_MS(500)
    // If the bite KOs Mario with no Life Shroom to revive him, the scene ends here; otherwise it waits for
    // his damage reaction.
    CALL(btlevtcmd_GetHp, UNIT_ID_MARIO, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_MARIO, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_btl_camera_set_prilimit, 0)
            RETURN
        END_IF
        WAIT_MS(1500)
        CALL(_wait_damage_event_end, UNIT_ID_MARIO)
    ELSE
        WAIT_MS(1500)
    END_IF
    // Wait until Mario is back at his home position, then the Shell Shield closes again.
    CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(1), EVT_NULLPTR, EVT_NULLPTR)
    LABEL(41)
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        WAIT_FRAMES(1)
        GOTO(41)
    END_IF
    CALL(btlevtcmd_GetGuardKouraId, UNIT_ID_MARIO, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_SetUnitWork, LW(3), 2, 1)
    END_IF
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    // She gloats that Mario was completely duped by her ruse.
    CALL(evt_msg_print, 2, STRING("stg1_gon_30"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -115, 50, 265, -115, 49, 170, 1, 0)
    WAIT_FRAMES(5)
    // Goombella or Koops calls her a coward.
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(224)
        CALL(evt_msg_print, 2, STRING("stg1_gon_30_01"), 0, UNIT_ID_PARTNER)
        CASE_EQUAL(225)
        CALL(evt_msg_print, 2, STRING("stg1_gon_30_02"), 0, UNIT_ID_PARTNER)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -431, 10, 735, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    // Hooktail mocks the heroes for being so gullible.
    CALL(evt_msg_print, 2, STRING("stg1_gon_30_03"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 20, 0)
    WAIT_FRAMES(20)
    // She admits things are not looking good and says she had better do something.
    CALL(evt_msg_print, 2, STRING("stg1_gon_30_04"), 0, UNIT_ID_SELF)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    LABEL(50)
    CALL(evt_btl_camera_set_mode, 1, 0)
    // Both paths meet here: unit work 3 is set to 1 (the surrender is used up) and Hooktail turns toward the
    // audience with heavy steps that shake the camera.
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_W_4"))
    CALL(btlevtcmd_ftomsec, 35, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -75, 0, 30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 23, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -12, 0, -30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 34, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -31, 0, 30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 23, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, 125, 0, -30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 34, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, 137, 0, 30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 17, LW(0))
    WAIT_MS(LW(0))
    WAIT_FRAMES(120)
    // She is moved out to the front of the stage, over the audience area.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(3), EVT_NULLPTR)
    ADD(LW(1), LW(3))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(3))
    ADD(LW(2), LW(3))
    ADD(LW(2), 80)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), 260)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    // Her light is moved, the camera rumbles and the audience area is darkened.
    CALL(_set_gonbaba_light)
    CALL(evt_btl_camera_shake_h, 1, 7, 0, 6000, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_btl_camera_noshake, 1)
        CALL(evt_btl_camera_shake_h, 1, 7, 0, 30, 12)
    END_INLINE_EVENT
CALL(evt_batstage_set_aud_dark, 0, 180, 0)
    CALL(evt_batstage_set_aud_dark_base, 0, 180, 0)
BEGIN_INLINE_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_7"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_MARIO, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 20, -1)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_PARTNER, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_PARTNER, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_PARTNER, LW(0), LW(1), LW(2), 20, -1)
    END_INLINE_EVENT
    // Hooktail snaps at eight audience members (gon_extra_eaten_event for each): members 32 and 53 tumble out
    // and flee, the others are eaten.
    SET_VALUE(LW(0), 13)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 14)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 32)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 33)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 34)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 35)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 53)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 54)
    SPAWN_EVENT(gon_extra_eaten_event)
    // Her HP is set back to 10 and a recovery effect shows her gaining strength.
    CALL(btlevtcmd_SetHp, UNIT_ID_SELF, 10)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), 40)
    ADD(LW(1), 180)
    ADD(LW(2), 70)
    CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
    // She chews with her eating pose (GNB_E_1) and her extra part 3 is shown.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_E_1"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("GNB_P_1"))
        CALL(btlevtcmd_ftomsec, 14, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_ftomsec, 56, LW(0))
        WAIT_MS(LW(0))
    END_INLINE_EVENT
    CALL(btlevtcmd_ftomsec, 32, LW(0))
    WAIT_MS(LW(0))
    // The audience panics: each remaining member jumps up and scrambles away from the stage, then is removed.
    CALL(evt_snd_sfxon, STRING("SFX_AUDIENCE_GNB_PANIC1"), 0)
    SET_VALUE(LW(0), 0)
    LOOP(100)
        CALL(evt_audience_flag_check, LW(0), 1, LW(4))
        IF_INT_EQUAL(LW(4), 0)
            GOTO(60)
        END_IF
        CALL(evt_audience_flag_check, LW(0), 16, LW(4))
        IF_INT_NOT_EQUAL(LW(4), 0)
            GOTO(60)
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_audience_flag_on, LW(0), 18)
            IF_INT_LESS(LW(0), 40)
                CALL(evt_audience_set_animpose, LW(0), 7, 0)
            ELSE
                CALL(evt_audience_set_animpose, LW(0), 9, 0)
            END_IF
            CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
            CALL(evt_sub_random, 5, LW(5))
            ADD(LW(5), 17)
            ADD(LW(2), LW(5))
            CALL(evt_sub_random, 8, LW(6))
            ADD(LW(6), 4)
            SET_VALUE(LW(11), LW(0))
            MODULO(LW(11), 10)
            IF_INT_NOT_EQUAL(LW(11), 0)
                SET_VALUE(LW(11), -1)
            ELSE
                CALL(evt_sub_random, 2, LW(11))
                SWITCH(LW(11))
                    CASE_EQUAL(0)
                    CASE_EQUAL(1)
                    SET_VALUE(LW(11), -1)
                    CASE_DEFAULT
                    SET_VALUE(LW(11), -1)
                END_SWITCH
            END_IF
            CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), LW(6), FLOAT(0.5f))
            CALL(evt_sub_random, 11, LW(7))
            WAIT_FRAMES(LW(7))
            SUBTRACT(LW(2), LW(5))
            CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), LW(6), FLOAT(0.0f))
            CALL(evt_audience_set_animpose, LW(0), 7, 0)
            CALL(evt_sub_random, 300, LW(6))
            ADD(LW(6), 400)
            WAIT_MS(LW(6))
            IF_INT_EQUAL(LW(0), 43)
                CALL(evt_audience_set_animpose, LW(0), 9, 0)
                CALL(evt_audience_set_rotate, LW(0), -90, 100, 0)
                CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                ADD(LW(2), 2)
                CALL(evt_audience_set_position, LW(0), LW(1), LW(2), LW(3))
                IF_INT_NOT_EQUAL(LW(11), -1)
                    CALL(evt_snd_sfxoff, LW(11))
                END_IF
            ELSE
                CALL(evt_audience_get_position, LW(0), LW(8), LW(9), LW(10))
                SET_VALUE(LW(12), LW(0))
                MODULO(LW(12), 10)
                IF_INT_NOT_EQUAL(LW(12), 0)
                    SET_VALUE(LW(12), -1)
                ELSE
                    CALL(evt_sub_random, 1, LW(12))
                    IF_INT_EQUAL(LW(12), 0)
                        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN1"), LW(8), LW(9), LW(10), LW(12))
                    ELSE
                        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN2"), LW(8), LW(9), LW(10), LW(12))
                    END_IF
                    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
                        LOOP(1000)
                            CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                            CALL(evt_snd_sfx_pos, LW(12), LW(1), LW(2), LW(3))
                            IF_INT_NOT_EQUAL(LW(11), -1)
                                CALL(evt_snd_sfx_pos, LW(11), LW(1), LW(2), LW(3))
                            END_IF
                            WAIT_FRAMES(1)
                        END_LOOP
                    END_SIBLING_EVENT
                END_IF
                CALL(evt_audience_set_animpose, LW(0), 7, 0)
                CALL(_btlgon_get_escape_param, LW(1), LW(3), LW(5), LW(6), LW(7))
                CALL(evt_audience_get_position, LW(0), LW(8), LW(9), LW(10))
                ADD(LW(5), LW(8))
                ADD(LW(6), LW(10))
                CALL(evt_audience_move_position_speed, LW(0), LW(5), LW(2), LW(6), LW(7))
                IF_INT_NOT_EQUAL(LW(11), -1)
                    CALL(evt_snd_sfxoff, LW(11))
                END_IF
                IF_INT_NOT_EQUAL(LW(12), -1)
                    DELETE_EVENT(LW(13))
                    CALL(evt_snd_sfxoff, LW(12))
                END_IF
                CALL(evt_audience_delete, LW(0))
                CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
            END_IF
        END_INLINE_EVENT
        LABEL(60)
        ADD(LW(0), 1)
    END_LOOP
    // Any audience members left are removed.
    LOOP(100)
        CALL(evt_audience_delete, LW(0))
        CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
        ADD(LW(0), 1)
    END_LOOP
    CALL(btlevtcmd_ftomsec, 38, LW(0))
    WAIT_MS(LW(0))
    // Chewing sounds play while the camera pulls back; then it moves in on her and part 3 is hidden again.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 4, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 45, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 44, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 0, 140, 900, 0, 60, 0, 30, 0)
    CALL(btlevtcmd_ftomsec, 284, LW(0))
    WAIT_MS(LW(0))
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -115, 50, 200, -115, -30, -670, 20, 0)
    WAIT_FRAMES(25)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_S_5"))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    // Goombella or Koops is appalled.
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(224)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_00_kur"), 0, UNIT_ID_PARTNER)
        CASE_EQUAL(225)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_00_nok"), 0, UNIT_ID_PARTNER)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 40, 98, 686, 57, 125, 182, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_SetTogeOffset, UNIT_ID_SELF, 310, 60, 0)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_5"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_5"))
    // Hooktail laughs: her strength is back.
    CALL(evt_msg_print, 2, STRING("stg1_gon_31_01"), 0, UNIT_ID_SELF)
    CALL(btlevtcmd_SetTogeOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -115, 40, 200, -115, 39, 170, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_S_3"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 260)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    // The partner urges Mario to finish her off.
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(224)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_02"), 0, UNIT_ID_PARTNER)
        CASE_EQUAL(225)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_03"), 0, UNIT_ID_PARTNER)
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 0, 110, 860, 0, 60, 170, 20, 0)
    WAIT_FRAMES(20)
    // Mario and his partner jump to new home positions in front of her, the turn counter is reset and the
    // battle resumes.
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(6), POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_PARTNER, LW(3), LW(4), LW(5))
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(6), EVT_NULLPTR)
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(7))
    BEGIN_INLINE_EVENT
        ADD(LW(4), LW(6))
        ADD(LW(5), LW(7))
        ADD(LW(5), 80)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_PARTNER, FLOAT(0.69921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_PARTNER, LW(3), LW(4), LW(5), 27, -1)
        CALL(btlevtcmd_SetHomePos, UNIT_ID_PARTNER, LW(3), LW(4), LW(5))
    END_INLINE_EVENT
    ADD(LW(1), LW(6))
    ADD(LW(2), LW(7))
    ADD(LW(2), 80)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_MARIO, FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 27, 0)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_MARIO_JUMP_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_MARIO, LW(3))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, LW(3), STRING("M_I_Y"))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(4))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(4), POSE_TYPE_ACTION)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_reset_turn)
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_PARTY_IN_AUDIENCE)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_MARIO)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#else
/**
 * Hooktail's surrender scene, run by her KO event the first time her HP runs out: she begs Mario to stop
 * (stg1_gon_23, 24) and offers in turn coins (24), a rare badge (26) and a sniff of her feet (28), each a yes/no
 * choice (LW(10) = offer, LW(5) = the accepted one); Mario shakes his head at each refusal.
 * If he accepts one she bites him as he walks up (weapon_gonbaba_kamituki_attack), gloats that he was duped
 * (stg1_gon_30, with Goombella's or Koops' reply) and the fight goes on.
 * If he refuses all three she grumbles that he is not very trusting (stg1_gon_31).
 * Either way she then sets unit work 3, lunges at the stands, takes away audience members
 * (gon_extra_eaten_event) and is set to 10 HP.
 */
EVT_BEGIN(negotiation_event)
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    // Hooktail's HP has run out: her status effects are cleared, her normal pose table comes back with the
    // weakened stay and talk poses (GNB_S_4, GNB_T_4), and the camera closes in.
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_reset_move_color_lv_all)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_4"))
    // She yells for Mario to wait.
    CALL(evt_msg_print, 2, STRING("stg1_gon_23"), 0, UNIT_ID_SELF)
    // Wait until Mario is back at his home position.
    LABEL(0)
    CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(3), LW(4), LW(5))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -431, 10, 735, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 0)
    // She says she gives up and offers 1,000 coins in apology, a yes/no choice.
    CALL(evt_msg_print, 2, STRING("stg1_gon_24"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_24_yesno1"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 0)
        // If Mario accepts, she is delighted and tells him to come and collect.
        CALL(evt_msg_print, 2, STRING("stg1_gon_25"), 0, UNIT_ID_SELF)
        GOTO(40)
    END_IF
    // If he refuses, Mario shakes his head and she offers a rare badge instead, another yes/no choice.
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_NO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_MARIO, 1, PART_ANIM_RESTART, 1)
    WAIT_MS(667)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 1)
    CALL(evt_msg_print, 2, STRING("stg1_gon_26"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_26_yesno1"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 1)
        // If he accepts the badge, she tells him to come and get it.
        CALL(evt_msg_print, 2, STRING("stg1_gon_27"), 0, UNIT_ID_SELF)
        GOTO(40)
    END_IF
    // If he refuses again, he shakes his head and she offers to let him smell her feet, a last yes/no choice.
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_NO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_MARIO, 1, PART_ANIM_RESTART, 1)
    WAIT_MS(667)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 2)
    CALL(evt_msg_print, 2, STRING("stg1_gon_28"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_28_yesno1"))
    CALL(evt_msg_continue)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 2)
        // If he accepts, she tells him to come and sniff.
        CALL(evt_msg_print, 2, STRING("stg1_gon_29"), 0, UNIT_ID_SELF)
        GOTO(40)
    END_IF
    // If he refuses all three offers, he shakes his head once more, she complains that he is not very
    // trusting and the bite is skipped.
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_NO2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_MARIO, 1, PART_ANIM_RESTART, 1)
    WAIT_MS(1000)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 15, 0)
    WAIT_FRAMES(15)
    CALL(evt_msg_print, 2, STRING("stg1_gon_31"), 0, UNIT_ID_SELF)
    GOTO(50)
    LABEL(40)
    // He accepted an offer: if Koops' Shell Shield guards Mario, it opens (its event 59) until he is back.
    CALL(btlevtcmd_GetGuardKouraId, UNIT_ID_MARIO, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_RunDataEventChild, LW(3), 59)
        CALL(btlevtcmd_SetUnitWork, LW(3), 2, 0)
    END_IF
    // Mario walks up to take the offer.
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_MARIO, FLOAT(3.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 400)
    CALL(btlevtcmd_MovePosition, UNIT_ID_MARIO, LW(0), 0, LW(2), 0, -1, 0)
    IF_INT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_C_4"))
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, BTL_POSE_STAY)
    END_IF
    WAIT_MS(700)
    // As he comes close, Hooktail bites him (GNB_A_2) and the bite's damage is checked against his guard.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_BITE1"), EVT_NULLPTR, 0, -220, 80, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 15, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_BITE2"), EVT_NULLPTR, 0, -260, 40, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A_2"))
    CALL(btlevtcmd_ftomsec, 20, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_MARIO, LW(4))
    CALL(btlevtcmd_ResultACDefence, UNIT_ID_MARIO, PTR(&weapon_gonbaba_kamituki_attack))
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_MARIO, LW(4), PTR(&weapon_gonbaba_kamituki_attack), 1048832, LW(5))
    WAIT_MS(500)
    // If the bite KOs Mario with no Life Shroom to revive him, the scene ends here; otherwise it waits for
    // his damage reaction.
    CALL(btlevtcmd_GetHp, UNIT_ID_MARIO, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_MARIO, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_btl_camera_set_prilimit, 0)
            RETURN
        END_IF
        WAIT_MS(1500)
        CALL(_wait_damage_event_end, UNIT_ID_MARIO)
    ELSE
        WAIT_MS(1500)
    END_IF
    // Wait until Mario is back at his home position, then the Shell Shield closes again.
    CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(1), EVT_NULLPTR, EVT_NULLPTR)
    LABEL(41)
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        WAIT_FRAMES(1)
        GOTO(41)
    END_IF
    CALL(btlevtcmd_GetGuardKouraId, UNIT_ID_MARIO, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_SetUnitWork, LW(3), 2, 1)
    END_IF
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    // She gloats that Mario was completely duped by her ruse.
    CALL(evt_msg_print, 2, STRING("stg1_gon_30"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -115, 50, 265, -115, 49, 170, 1, 0)
    WAIT_FRAMES(5)
    // Goombella or Koops calls her a coward.
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(224)
        CALL(evt_msg_print, 2, STRING("stg1_gon_30_01"), 0, UNIT_ID_PARTNER)
        CASE_EQUAL(225)
        CALL(evt_msg_print, 2, STRING("stg1_gon_30_02"), 0, UNIT_ID_PARTNER)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -431, 10, 735, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    // Hooktail mocks the heroes for being so gullible.
    CALL(evt_msg_print, 2, STRING("stg1_gon_30_03"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 20, 0)
    WAIT_FRAMES(20)
    // She admits things are not looking good and says she had better do something.
    CALL(evt_msg_print, 2, STRING("stg1_gon_30_04"), 0, UNIT_ID_SELF)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    LABEL(50)
    CALL(evt_btl_camera_set_mode, 1, 0)
    // Both paths meet here: unit work 3 is set to 1 (the surrender is used up) and Hooktail turns toward the
    // audience with heavy steps that shake the camera.
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_W_4"))
    CALL(btlevtcmd_ftomsec, 35, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -75, 0, 30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 23, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -12, 0, -30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 34, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -31, 0, 30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 23, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, 125, 0, -30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 34, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, 137, 0, 30, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 17, LW(0))
    WAIT_MS(LW(0))
    WAIT_FRAMES(120)
    // She is moved out to the front of the stage, over the audience area.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(3), EVT_NULLPTR)
    ADD(LW(1), LW(3))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(3))
    ADD(LW(2), LW(3))
    ADD(LW(2), 80)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), 260)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    // Her light is moved, the camera rumbles and the audience area is darkened.
    CALL(_set_gonbaba_light)
    CALL(evt_btl_camera_shake_h, 1, 7, 0, 6000, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_btl_camera_noshake, 1)
        CALL(evt_btl_camera_shake_h, 1, 7, 0, 30, 12)
    END_INLINE_EVENT
#if VERSION <= VERSION_DEMO_US
    CALL(evt_batstage_set_aud_dark, 0, 180, 0)
    CALL(evt_batstage_set_aud_dark_base, 0, 180, 0)
#else
    CALL(evt_batstage_set_aud_dark, 0, 24, 0)
    CALL(evt_batstage_set_aud_dark_base, 0, 24, 0)
#endif
    // Mario and his partner jump back in surprise.
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_7"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_MARIO, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 20, -1)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_PARTNER, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_PARTNER, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_PARTNER, LW(0), LW(1), LW(2), 20, -1)
    END_INLINE_EVENT
    // Hooktail snaps at eight audience members (gon_extra_eaten_event for each): members 32 and 53 tumble out
    // and flee, the others are eaten.
    SET_VALUE(LW(0), 13)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 14)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 32)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 33)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 34)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 35)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 53)
    SPAWN_EVENT(gon_extra_eaten_event)
    SET_VALUE(LW(0), 54)
    SPAWN_EVENT(gon_extra_eaten_event)
    // Her HP is set back to 10 and a recovery effect shows her gaining strength.
    CALL(btlevtcmd_SetHp, UNIT_ID_SELF, 10)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), 40)
    ADD(LW(1), 180)
    ADD(LW(2), 70)
    CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)
    // She chews with her eating pose (GNB_E_1) and her extra part 3 is shown.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_E_1"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("GNB_P_1"))
        CALL(btlevtcmd_ftomsec, 14, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
        CALL(btlevtcmd_ftomsec, 56, LW(0))
        WAIT_MS(LW(0))
    END_INLINE_EVENT
    CALL(btlevtcmd_ftomsec, 32, LW(0))
    WAIT_MS(LW(0))
    // The audience panics: each remaining member jumps up and scrambles away from the stage, then is removed.
    CALL(evt_snd_sfxon, STRING("SFX_AUDIENCE_GNB_PANIC1"), 0)
    SET_VALUE(LW(0), 0)
    LOOP(100)
        CALL(evt_audience_flag_check, LW(0), 1, LW(4))
        IF_INT_EQUAL(LW(4), 0)
            GOTO(60)
        END_IF
        CALL(evt_audience_flag_check, LW(0), 16, LW(4))
        IF_INT_NOT_EQUAL(LW(4), 0)
            GOTO(60)
        END_IF
        BEGIN_INLINE_EVENT
            CALL(evt_audience_flag_on, LW(0), 18)
            IF_INT_LESS(LW(0), 40)
                CALL(evt_audience_set_animpose, LW(0), 7, 0)
            ELSE
                CALL(evt_audience_set_animpose, LW(0), 9, 0)
            END_IF
            CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
            CALL(evt_sub_random, 5, LW(5))
            ADD(LW(5), 17)
            ADD(LW(2), LW(5))
            CALL(evt_sub_random, 8, LW(6))
            ADD(LW(6), 4)
            SET_VALUE(LW(11), LW(0))
            MODULO(LW(11), 10)
            IF_INT_NOT_EQUAL(LW(11), 0)
                SET_VALUE(LW(11), -1)
            ELSE
                CALL(evt_sub_random, 2, LW(11))
                SWITCH(LW(11))
                    CASE_EQUAL(0)
                    CASE_EQUAL(1)
                    SET_VALUE(LW(11), -1)
                    CASE_DEFAULT
                    SET_VALUE(LW(11), -1)
                END_SWITCH
            END_IF
            CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), LW(6), FLOAT(0.5f))
            CALL(evt_sub_random, 11, LW(7))
            WAIT_FRAMES(LW(7))
            SUBTRACT(LW(2), LW(5))
            CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), LW(6), FLOAT(0.0f))
            CALL(evt_audience_set_animpose, LW(0), 7, 0)
            CALL(evt_sub_random, 300, LW(6))
            ADD(LW(6), 400)
            WAIT_MS(LW(6))
            IF_INT_EQUAL(LW(0), 43)
                CALL(evt_audience_set_animpose, LW(0), 9, 0)
                CALL(evt_audience_set_rotate, LW(0), -90, 100, 0)
                CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                ADD(LW(2), 2)
                CALL(evt_audience_set_position, LW(0), LW(1), LW(2), LW(3))
                IF_INT_NOT_EQUAL(LW(11), -1)
                    CALL(evt_snd_sfxoff, LW(11))
                END_IF
            ELSE
                CALL(evt_audience_get_position, LW(0), LW(8), LW(9), LW(10))
                SET_VALUE(LW(12), LW(0))
                MODULO(LW(12), 10)
                IF_INT_NOT_EQUAL(LW(12), 0)
                    SET_VALUE(LW(12), -1)
                ELSE
                    CALL(evt_sub_random, 1, LW(12))
                    IF_INT_EQUAL(LW(12), 0)
                        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN1"), LW(8), LW(9), LW(10), LW(12))
                    ELSE
                        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN2"), LW(8), LW(9), LW(10), LW(12))
                    END_IF
                    BEGIN_SIBLING_EVENT_GET_ID(LW(13))
                        LOOP(1000)
                            CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                            CALL(evt_snd_sfx_pos, LW(12), LW(1), LW(2), LW(3))
                            IF_INT_NOT_EQUAL(LW(11), -1)
                                CALL(evt_snd_sfx_pos, LW(11), LW(1), LW(2), LW(3))
                            END_IF
                            WAIT_FRAMES(1)
                        END_LOOP
                    END_SIBLING_EVENT
                END_IF
                CALL(evt_audience_set_animpose, LW(0), 7, 0)
                CALL(_btlgon_get_escape_param, LW(1), LW(3), LW(5), LW(6), LW(7))
                CALL(evt_audience_get_position, LW(0), LW(8), LW(9), LW(10))
                ADD(LW(5), LW(8))
                ADD(LW(6), LW(10))
                CALL(evt_audience_move_position_speed, LW(0), LW(5), LW(2), LW(6), LW(7))
                IF_INT_NOT_EQUAL(LW(11), -1)
                    CALL(evt_snd_sfxoff, LW(11))
                END_IF
                IF_INT_NOT_EQUAL(LW(12), -1)
                    DELETE_EVENT(LW(13))
                    CALL(evt_snd_sfxoff, LW(12))
                END_IF
                CALL(evt_audience_delete, LW(0))
                CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
            END_IF
        END_INLINE_EVENT
        LABEL(60)
        ADD(LW(0), 1)
    END_LOOP
    // Any audience members left are removed.
    LOOP(100)
        CALL(evt_audience_delete, LW(0))
        CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
        ADD(LW(0), 1)
    END_LOOP
    CALL(btlevtcmd_ftomsec, 38, LW(0))
    WAIT_MS(LW(0))
    // Chewing sounds play while the camera pulls back; then it moves in on her and part 3 is hidden again.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 4, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 45, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 44, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 0, 140, 900, 0, 60, 0, 30, 0)
    CALL(btlevtcmd_ftomsec, 284, LW(0))
    WAIT_MS(LW(0))
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -115, 50, 200, -115, -30, -670, 20, 0)
    WAIT_FRAMES(25)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_S_5"))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, PART_ATTR_HIDDEN)
    CALL(btlevtcmd_StatusWindowOnOff, 0)
    // Goombella or Koops is appalled.
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(224)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_00_kur"), 0, UNIT_ID_PARTNER)
        CASE_EQUAL(225)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_00_nok"), 0, UNIT_ID_PARTNER)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 40, 98, 686, 57, 125, 182, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_SetTogeOffset, UNIT_ID_SELF, 310, 60, 0)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_5"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_5"))
    // Hooktail laughs: her strength is back.
    CALL(evt_msg_print, 2, STRING("stg1_gon_31_01"), 0, UNIT_ID_SELF)
    CALL(btlevtcmd_SetTogeOffset, UNIT_ID_SELF, 0, 0, 0)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -115, 40, 200, -115, 39, 170, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_S_3"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 260)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    // The partner urges Mario to finish her off.
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(224)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_02"), 0, UNIT_ID_PARTNER)
        CASE_EQUAL(225)
        CALL(evt_msg_print, 2, STRING("stg1_gon_31_03"), 0, UNIT_ID_PARTNER)
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, 1)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, 0, 110, 860, 0, 60, 170, 20, 0)
    WAIT_FRAMES(20)
    // Mario and his partner jump to new home positions in front of her, the turn counter is reset and the
    // battle resumes.
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(6))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(6), POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_PARTNER, LW(3), LW(4), LW(5))
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(6), EVT_NULLPTR)
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(7))
    BEGIN_INLINE_EVENT
        ADD(LW(4), LW(6))
        ADD(LW(5), LW(7))
        ADD(LW(5), 80)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_PARTNER, FLOAT(0.69921875f))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_PARTNER, LW(3), LW(4), LW(5), 27, -1)
        CALL(btlevtcmd_SetHomePos, UNIT_ID_PARTNER, LW(3), LW(4), LW(5))
    END_INLINE_EVENT
    ADD(LW(1), LW(6))
    ADD(LW(2), LW(7))
    ADD(LW(2), 80)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_MARIO, FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 27, 0)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_MARIO_JUMP_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_MARIO, LW(3))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, LW(3), STRING("M_I_Y"))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(4))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(4), POSE_TYPE_ACTION)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_reset_turn)
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_PARTY_IN_AUDIENCE)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_MARIO)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#endif

/**
 * Script helper: moves the paper-craft light to (-600, 150, 1500), the direction (-200, 50, 500) scaled by 3,
 * to light Hooktail's stage.
 */
USER_FUNC(_set_gonbaba_light) {
    LightEntry* light = lightGetPaperCraft();
    Mtx mtx;

    light->position = (Vec){-200.0f, 50.0f, 500.0f};
    PSMTXScale(mtx, 3.0f, 3.0f, 3.0f);
    PSMTXMultVec(mtx, &light->position, &light->position);
    return EVT_RETURN_DONE;
}

/**
 * One audience member (LW(0)) caught when Hooktail lunges at the stands in negotiation_event:
 * it is lifted up and
 * pulled toward her.
 * Members 32 and 53 tumble out onto the stage and flee off the left side screaming (SFX_AUDIENCE_PANIC_SCREAM2/3
 * and PANIC_RUN1/2); every other one is simply removed (eaten, as the name says).
 * Each removed member except 32 lowers the audience count by one.
 */
EVT_BEGIN(gon_extra_eaten_event)
    // The audience member is startled and lifted out of the crowd.
    WAIT_FRAMES(14)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, 6)
    CALL(evt_audience_flag_on, LW(0), 18)
    CALL(evt_audience_flag_off, LW(0), 4)
    CALL(evt_audience_set_animpose, LW(0), 9, 0)
    CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
    ADD(LW(2), 40)
    CALL(evt_audience_set_position, LW(0), LW(1), LW(2), LW(3))
    WAIT_FRAMES(1)
    ADD(LW(2), 80)
    SUBTRACT(LW(1), 80)
    // It is pulled toward Hooktail's mouth.
    CALL(evt_audience_move_position_speed, LW(0), LW(1), LW(2), LW(3), FLOAT(10.0f))
    // Audience member 32 tumbles out onto the stage, spinning, and runs off the left side screaming.
    IF_INT_EQUAL(LW(0), 32)
        CALL(evt_audience_flag_on, LW(0), 4)
        BEGIN_INLINE_EVENT
            CALL(evt_audience_set_rotate_offset, LW(0), 0, 18, 0)
            SET_VALUE(LW(4), 0)
            SET_VALUE(LW(5), 0)
            LOOP(45)
                ADD(LW(4), 2)
                ADD(LW(5), 15)
                CALL(evt_audience_set_rotate, LW(0), LW(4), LW(5), 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
        CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(2), EVT_NULLPTR)
        SUBTRACT(LW(1), 40)
        ADD(LW(2), 1)
        CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), 60, FLOAT(1.0f))
        CALL(evt_audience_set_rotate_offset, LW(0), 0, 0, 0)
        CALL(evt_audience_set_rotate, LW(0), 90, 0, 0)
        WAIT_FRAMES(30)
        CALL(evt_audience_set_rotate_offset, LW(0), 0, 0, 0)
        SET_VALUE(LW(1), 90)
        LOOP(90)
            SUBTRACT(LW(1), 1)
            CALL(evt_audience_set_rotate, LW(0), LW(1), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_audience_set_rotate, LW(0), 0, 0, 0)
        CALL(evt_audience_set_animpose, LW(0), 7, 0)
        WAIT_FRAMES(10)
        CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_SCREAM2"), LW(1), LW(2), LW(3), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN1"), LW(1), LW(2), LW(3), LW(5))
        BEGIN_SIBLING_EVENT_GET_ID(LW(6))
            LOOP(1000)
                CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                CALL(evt_snd_sfx_pos, LW(5), LW(1), LW(2), LW(3))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(evt_audience_move_position_speed, LW(0), -300, LW(2), LW(3), FLOAT(4.5f))
        DELETE_EVENT(LW(6))
        CALL(evt_snd_sfxoff, LW(5))
        CALL(evt_audience_delete, LW(0))
        RETURN
    END_IF
    // Audience member 53 does the same, and the audience count goes down by one.
    IF_INT_EQUAL(LW(0), 53)
        CALL(evt_audience_flag_on, LW(0), 4)
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(4), 0)
            LOOP(45)
                ADD(LW(4), 2)
                CALL(evt_audience_set_rotate, LW(0), LW(4), 0, 0)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
        CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
        CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(2), EVT_NULLPTR)
        ADD(LW(3), 40)
        ADD(LW(2), 1)
        CALL(evt_audience_jump_position_firstsp, LW(0), LW(1), LW(2), LW(3), 60, FLOAT(1.0f))
        CALL(evt_audience_set_rotate, LW(0), 90, 0, 0)
        WAIT_FRAMES(30)
        CALL(evt_audience_set_rotate, LW(0), 0, 0, 0)
        CALL(evt_audience_set_rotate_offset, LW(0), 0, 0, 0)
        SET_VALUE(LW(1), 90)
        LOOP(90)
            SUBTRACT(LW(1), 1)
            CALL(evt_audience_set_rotate, LW(0), LW(1), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_audience_set_rotate, LW(0), 0, 0, 0)
        CALL(evt_audience_set_animpose, LW(0), 7, 0)
        WAIT_FRAMES(10)
        CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_SCREAM3"), LW(1), LW(2), LW(3), 0)
        CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_PANIC_RUN2"), LW(1), LW(2), LW(3), LW(5))
        BEGIN_SIBLING_EVENT_GET_ID(LW(6))
            LOOP(1000)
                CALL(evt_audience_get_position, LW(0), LW(1), LW(2), LW(3))
                CALL(evt_snd_sfx_pos, LW(5), LW(1), LW(2), LW(3))
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(evt_audience_move_position_speed, LW(0), -300, LW(2), LW(3), FLOAT(4.5f))
        DELETE_EVENT(LW(6))
        CALL(evt_snd_sfxoff, LW(5))
        CALL(evt_audience_delete, LW(0))
        CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
        RETURN
    END_IF
    // Any other audience member is simply removed (eaten), and the audience count goes down by one.
    CALL(evt_audience_delete, LW(0))
    CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
    RETURN
EVT_END()

/** Script helper: args[0] is a unit; blocks while its damage event is still running. */
USER_FUNC(_wait_damage_event_end) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id;
    BattleUnit* unit;

    id = evtGetValue(event, *args++);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    if (unit == NULL) {
        return EVT_RETURN_DONE;
    }
    if (unit->damageEventId != 0 && evtCheckID(unit->damageEventId)) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Damage reaction: runs the default damage handling (btldefaultevt_Damage) with LW(10) = -2 (the unit itself)
 * and LW(11) = 1.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Hooktail's damage reaction (data-table event 9, run by subsetevt_damage).
 * A hit with the Attack FX R sound (DAMAGE_RESULT_ATTACK_FX_R, the cricket chirp) counts up unit work 0 (to 20),
 * lowers unit work 1, the defense change of her GONBABA_DEFENSE_ATTR parts (down to -2), and switches
 * to the weak pose table.
 * A hit that does damage plays GNB_D_2 (part 1) or GNB_D_1, and the latter sends up to two spectators (counted
 * down in unit work 4) jumping off the stands and running away.
 * On the last hit with that sound she complains the first two times (stg1_gon_21, 22, unit work 2), or after the
 * surrender (unit work 3) once with stg1_gon_31_04.
 */
#if VERSION <= VERSION_DEMO_US
EVT_BEGIN(gonbaba_damage_sub_event)
    // A hit with the cricket chirp (Attack FX R) is recognised: Hooktail's weak spot.
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ATTACK_FX_R, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_POWER_DOWN, LW(0))
        IF_INT_LESS(LW(0), 20)
            ADD(LW(0), 1)
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_POWER_DOWN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART_DEFENSE, LW(0))
        IF_INT_GREATER(LW(0), -2)
            SUBTRACT(LW(0), 1)
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART_DEFENSE, LW(0))
        // She switches to her weakened poses and the cricket's effect builds up on her.
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_weak))
    END_IF
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ATTACK_FX_R, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(90)
        END_IF
    END_IF
    // She reels from the damage in her hurt pose.
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(11))
    IF_INT_EQUAL(LW(11), 1)
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("GNB_D_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("GNB_D_1"))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, LW(0))
        SET_VALUE(LW(9), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, LW(0))
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                SUBTRACT(LW(0), 350)
                ADD(LW(1), 20)
                // Spectators jump down from the stands and run away in fright.
                CALL(evt_audience_entry, LW(9), 0)
                CALL(evt_audience_set_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_audience_flag_on, LW(9), 22)
                CALL(evt_audience_set_animpose, LW(9), 9, 0)
                CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
                SUBTRACT(LW(0), 50)
                CALL(evt_audience_jump_position_firstsp, LW(9), LW(0), LW(1), LW(2), 60, FLOAT(1.0f))
                CALL(evt_audience_set_animpose, LW(9), 7, 0)
                CALL(evt_audience_get_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_RUN1"), LW(0), LW(1), LW(2), LW(7))
                CALL(evt_audience_move_position_speed, LW(9), -300, LW(1), LW(2), FLOAT(4.5f))
                CALL(evt_snd_sfxoff, LW(7))
                CALL(evt_audience_delete, LW(9))
            END_INLINE_EVENT
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, LW(0))
        SET_VALUE(LW(9), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, LW(0))
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                SUBTRACT(LW(0), 320)
                ADD(LW(1), 30)
                CALL(evt_audience_entry, LW(9), 0)
                CALL(evt_audience_set_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_audience_flag_on, LW(9), 22)
                CALL(evt_audience_set_animpose, LW(9), 9, 0)
                CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
                SUBTRACT(LW(0), 10)
                ADD(LW(2), 30)
                CALL(evt_audience_jump_position_firstsp, LW(9), LW(0), LW(1), LW(2), 54, FLOAT(1.0f))
                CALL(evt_audience_set_animpose, LW(9), 7, 0)
                CALL(evt_audience_get_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_RUN1"), LW(0), LW(1), LW(2), LW(7))
                CALL(evt_audience_move_position_speed, LW(9), -300, LW(1), LW(2), FLOAT(4.5f))
                CALL(evt_snd_sfxoff, LW(7))
                CALL(evt_audience_delete, LW(9))
            END_INLINE_EVENT
        END_IF
    END_IF
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ATTACK_FX_R, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 2)
        RETURN
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_4"))
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 1)
        CALL(evt_msg_print, 2, STRING("stg1_gon_21"), 0, UNIT_ID_SELF)
        CASE_EQUAL(1)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 2)
        CALL(evt_msg_print, 2, STRING("stg1_gon_22"), 0, UNIT_ID_SELF)
    END_SWITCH
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
    LABEL(50)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 3)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 3)
CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_4"))
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(evt_btl_camera_set_mode, 1, 3)
    SET_VALUE(LW(1), 160)
    SET_VALUE(LW(2), 536)
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    ADD(LW(1), LW(5))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(5))
    ADD(LW(2), LW(5))
    ADD(LW(2), 80)
    SET_VALUE(LW(5), LW(1))
    SUBTRACT(LW(5), 37)
    SET_VALUE(LW(6), LW(2))
    SUBTRACT(LW(6), 511)
    CALL(evt_btl_camera_set_moveto, 1, 93, LW(1), LW(2), 93, LW(5), LW(6), 20, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_print, 2, STRING("stg1_gon_31_04"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
    LABEL(90)
RETURN
EVT_END()
#else
EVT_BEGIN(gonbaba_damage_sub_event)
    // A hit with the cricket chirp (Attack FX R) is recognised: Hooktail's weak spot.
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ATTACK_FX_R, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_POWER_DOWN, LW(0))
        IF_INT_LESS(LW(0), 20)
            ADD(LW(0), 1)
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_POWER_DOWN, LW(0))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART_DEFENSE, LW(0))
        IF_INT_GREATER(LW(0), -2)
            SUBTRACT(LW(0), 1)
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART_DEFENSE, LW(0))
        // She switches to her weakened poses and the cricket's effect builds up on her.
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_weak))
    END_IF
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ATTACK_FX_R, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(90)
        END_IF
    END_IF
    // She reels from the damage in her hurt pose.
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(11))
    IF_INT_EQUAL(LW(11), 1)
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("GNB_D_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("GNB_D_1"))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, LW(0))
        SET_VALUE(LW(9), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, LW(0))
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                SUBTRACT(LW(0), 350)
                ADD(LW(1), 20)
                // Spectators jump down from the stands and run away in fright.
                CALL(evt_audience_entry, LW(9), 0)
                CALL(evt_audience_set_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_audience_flag_on, LW(9), 22)
                CALL(evt_audience_set_animpose, LW(9), 9, 0)
                CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
                SUBTRACT(LW(0), 50)
                CALL(evt_audience_jump_position_firstsp, LW(9), LW(0), LW(1), LW(2), 60, FLOAT(1.0f))
                CALL(evt_audience_set_animpose, LW(9), 7, 0)
                CALL(evt_audience_get_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_RUN1"), LW(0), LW(1), LW(2), LW(7))
                CALL(evt_audience_move_position_speed, LW(9), -300, LW(1), LW(2), FLOAT(4.5f))
                CALL(evt_snd_sfxoff, LW(7))
                CALL(evt_audience_delete, LW(9))
            END_INLINE_EVENT
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, LW(0))
        SET_VALUE(LW(9), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            SUBTRACT(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, LW(0))
            BEGIN_INLINE_EVENT
                CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                SUBTRACT(LW(0), 320)
                ADD(LW(1), 30)
                CALL(evt_audience_entry, LW(9), 0)
                CALL(evt_audience_set_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_audience_flag_on, LW(9), 22)
                CALL(evt_audience_set_animpose, LW(9), 9, 0)
                CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(1), EVT_NULLPTR)
                SUBTRACT(LW(0), 10)
                ADD(LW(2), 30)
                CALL(evt_audience_jump_position_firstsp, LW(9), LW(0), LW(1), LW(2), 54, FLOAT(1.0f))
                CALL(evt_audience_set_animpose, LW(9), 7, 0)
                CALL(evt_audience_get_position, LW(9), LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING("SFX_AUDIENCE_RUN1"), LW(0), LW(1), LW(2), LW(7))
                CALL(evt_audience_move_position_speed, LW(9), -300, LW(1), LW(2), FLOAT(4.5f))
                CALL(evt_snd_sfxoff, LW(7))
                CALL(evt_audience_delete, LW(9))
            END_INLINE_EVENT
        END_IF
    END_IF
    WAIT_FRAMES(30)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_FINAL_HIT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ATTACK_FX_R, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 2)
        RETURN
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_WaitAttackEnd)
#endif
    // When the cricket sound finishes her off for the first or second time she complains about the awful chirping.
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_4"))
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 1)
        CALL(evt_msg_print, 2, STRING("stg1_gon_21"), 0, UNIT_ID_SELF)
        CASE_EQUAL(1)
        CALL(evt_btl_camera_set_mode, 1, 3)
        CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 2)
        CALL(evt_msg_print, 2, STRING("stg1_gon_22"), 0, UNIT_ID_SELF)
    END_SWITCH
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
    LABEL(50)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 3)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 3)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_WaitAttackEnd)
#endif
    // After her surrender, the last cricket hit makes her feel faint again.
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_4"))
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(evt_btl_camera_set_mode, 1, 3)
    SET_VALUE(LW(1), 160)
    SET_VALUE(LW(2), 536)
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    ADD(LW(1), LW(5))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(5))
    ADD(LW(2), LW(5))
    ADD(LW(2), 80)
    SET_VALUE(LW(5), LW(1))
    SUBTRACT(LW(5), 37)
    SET_VALUE(LW(6), LW(2))
    SUBTRACT(LW(6), 511)
    CALL(evt_btl_camera_set_moveto, 1, 93, LW(1), LW(2), 93, LW(5), LW(6), 20, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_print, 2, STRING("stg1_gon_31_04"), 0, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_prilimit, 0)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
    LABEL(90)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_GetHitPos, UNIT_ID_SELF, LW(0), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_ATTACK_MISS2"), LW(0), LW(1), LW(2), 0)
#endif
    RETURN
EVT_END()
#endif

/**
 * Defeat of Hooktail.
 * The first time her HP runs out (unit work 3 = 0) she does not die
 * but goes into negotiation_event instead, where she begs and bargains with Mario.
 * The second time she collapses (GNB_X_1, GNB_D_3, GNB_H_1 to 3, with SFX_BOSS_GNB_DOWN1 to 3),
 * the camera pulls back, the stg1_gon_32 line is shown and the unit is removed.
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Defeat of Hooktail.
 * The first time her HP runs out (unit work 3 = 0) she does not die
 * but goes into negotiation_event instead, where she begs and bargains with Mario.
 * The second time she collapses (GNB_X_1, GNB_D_3, GNB_H_1 to 3, with SFX_BOSS_GNB_DOWN1 to 3),
 * the camera pulls back, the stg1_gon_32 line is shown and the unit is removed.
 */
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(negotiation_event)
        RETURN
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_X_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_DOWN1"), EVT_NULLPTR, 0, LW(15))
CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(evt_btl_camera_set_mode, 1, 3)
    SET_VALUE(LW(1), 160)
    SET_VALUE(LW(2), 536)
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    ADD(LW(1), LW(5))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(5))
    ADD(LW(2), LW(5))
    ADD(LW(2), 80)
    SET_VALUE(LW(5), LW(1))
    SUBTRACT(LW(5), 37)
    SET_VALUE(LW(6), LW(2))
    SUBTRACT(LW(6), 511)
    CALL(evt_btl_camera_set_moveto, 1, 93, LW(1), LW(2), 93, LW(5), LW(6), 30, 0)
    WAIT_MS(1500)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_X_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_X_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 310)
    ADD(LW(1), 60)
    CALL(evt_msg_toge, 2, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_32"), 0, 0)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -38, 33, 1005, -38, 92, 13, 60, 0)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_DOWN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_D_3"))
    WAIT_FRAMES(42)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_H_1"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), 25)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_H_1"))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(4), 120)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, LW(5), 0)
        LOOP(40)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(4))
            SUBTRACT(LW(4), 3)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 300)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 90, -1)
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_GNB_DOWN3"), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_H_2"))
    CALL(evt_btl_camera_shake_h, 1, 8, 0, 30, 13)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_H_3"))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_FRAMES(60)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_prilimit, 0)
    RETURN
EVT_END()
#else
/**
 * Defeat of Hooktail.
 * The first time her HP runs out (unit work 3 = 0) she does not die
 * but goes into negotiation_event instead, where she begs and bargains with Mario.
 * The second time she collapses (GNB_X_1, GNB_D_3, GNB_H_1 to 3, with SFX_BOSS_GNB_DOWN1 to 3),
 * the camera pulls back, the stg1_gon_32 line is shown and the unit is removed.
 */
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(negotiation_event)
        RETURN
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_X_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_DOWN1"), EVT_NULLPTR, 0, LW(15))
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_WaitAttackEnd)
#endif
    CALL(evt_btl_camera_set_prilimit, 1)
    CALL(evt_btl_camera_wait_move_end)
    CALL(evt_btl_camera_set_mode, 1, 3)
    SET_VALUE(LW(1), 160)
    SET_VALUE(LW(2), 536)
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(5), EVT_NULLPTR)
    ADD(LW(1), LW(5))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(5))
    ADD(LW(2), LW(5))
    ADD(LW(2), 80)
    SET_VALUE(LW(5), LW(1))
    SUBTRACT(LW(5), 37)
    SET_VALUE(LW(6), LW(2))
    SUBTRACT(LW(6), 511)
    CALL(evt_btl_camera_set_moveto, 1, 93, LW(1), LW(2), 93, LW(5), LW(6), 30, 0)
    WAIT_MS(1500)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_X_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_X_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 310)
    ADD(LW(1), 60)
    CALL(evt_msg_toge, 2, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_32"), 0, 0)
    CALL(evt_btl_camera_set_mode, 1, 3)
    CALL(evt_btl_camera_set_moveto, 1, -38, 33, 1005, -38, 92, 13, 60, 0)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_DOWN2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_D_3"))
    WAIT_FRAMES(42)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_H_1"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), 25)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_H_1"))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(5), 100)
        SET_VALUE(LW(4), 120)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, LW(5), 0)
        LOOP(40)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(4))
            SUBTRACT(LW(4), 3)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 300)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 90, -1)
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_GNB_DOWN3"), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_H_2"))
    CALL(evt_btl_camera_shake_h, 1, 8, 0, 30, 13)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_H_3"))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_FRAMES(60)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_prilimit, 0)
    RETURN
EVT_END()
#endif

/**
 * Init script of Hooktail: registers her wait, attack and damage handlers (confusion uses the default handler)
 * and the battle-entry event, sets her speaking body part to part 1 and clears her unit work (0 to 5;
 * 3 is the negotiation flag, 5 the attack-order counter).
 */
#if VERSION <= VERSION_DEMO_US
/**
 * Init script of Hooktail: registers her wait, attack and damage handlers (confusion uses the default handler)
 * and the battle-entry event, sets her speaking body part to part 1 and clears her unit work (0 to 5;
 * 3 is the negotiation flag, 5 the attack-order counter).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetTalkBodyId, UNIT_ID_SELF, 1)
    CALL(evt_audience_delete, 13)
    CALL(evt_audience_entry, 13, 0)
    CALL(evt_audience_delete, 14)
    CALL(evt_audience_entry, 14, 0)
    CALL(evt_audience_delete, 32)
    CALL(evt_audience_entry, 32, 0)
    CALL(evt_audience_delete, 33)
    CALL(evt_audience_entry, 33, 0)
    CALL(evt_audience_delete, 34)
    CALL(evt_audience_entry, 34, 0)
    CALL(evt_audience_delete, 35)
    CALL(evt_audience_entry, 35, 0)
    CALL(evt_audience_delete, 53)
    CALL(evt_audience_entry, 53, 0)
    CALL(evt_audience_delete, 54)
    CALL(evt_audience_entry, 54, 0)
    CALL(evt_audience_flag_on, 13, 512)
    CALL(evt_audience_flag_on, 14, 512)
    CALL(evt_audience_flag_on, 32, 512)
    CALL(evt_audience_flag_on, 33, 512)
    CALL(evt_audience_flag_on, 34, 512)
    CALL(evt_audience_flag_on, 35, 512)
    CALL(evt_audience_flag_on, 53, 512)
    CALL(evt_audience_flag_on, 54, 512)
CALL(evt_audience_base_flag_on, 32768)
CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(btlevtcmd_SetBaseScale, UNIT_ID_SELF, FLOAT(1.2998047f), FLOAT(1.2998047f), FLOAT(0.5f))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_POWER_DOWN, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART_DEFENSE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#else
/**
 * Init script of Hooktail: registers her wait, attack and damage handlers (confusion uses the default handler)
 * and the battle-entry event, sets her speaking body part to part 1 and clears her unit work (0 to 5;
 * 3 is the negotiation flag, 5 the attack-order counter).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetTalkBodyId, UNIT_ID_SELF, 1)
    CALL(evt_audience_delete, 13)
    CALL(evt_audience_entry, 13, 0)
    CALL(evt_audience_delete, 14)
    CALL(evt_audience_entry, 14, 0)
    CALL(evt_audience_delete, 32)
    CALL(evt_audience_entry, 32, 0)
    CALL(evt_audience_delete, 33)
    CALL(evt_audience_entry, 33, 0)
    CALL(evt_audience_delete, 34)
    CALL(evt_audience_entry, 34, 0)
    CALL(evt_audience_delete, 35)
    CALL(evt_audience_entry, 35, 0)
    CALL(evt_audience_delete, 53)
    CALL(evt_audience_entry, 53, 0)
    CALL(evt_audience_delete, 54)
    CALL(evt_audience_entry, 54, 0)
    CALL(evt_audience_flag_on, 13, 512)
    CALL(evt_audience_flag_on, 14, 512)
    CALL(evt_audience_flag_on, 32, 512)
    CALL(evt_audience_flag_on, 33, 512)
    CALL(evt_audience_flag_on, 34, 512)
    CALL(evt_audience_flag_on, 35, 512)
    CALL(evt_audience_flag_on, 53, 512)
    CALL(evt_audience_flag_on, 54, 512)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_audience_base_flag_on, 32768)
#else
    CALL(evt_audience_base_flag_on, 16384)
#endif
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(btlevtcmd_SetBaseScale, UNIT_ID_SELF, FLOAT(1.2998047f), FLOAT(1.2998047f), FLOAT(0.5f))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_POWER_DOWN, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART_DEFENSE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_EATEN_AUDIENCE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
#endif

/** Phase event: empty, it does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/**
 * Chooses Hooktail's attack: item use first; her first attack of the fight is the fire breath,
 * her second the stomp (fumituke_attack_event), and after that each is picked with 50% chance.
 * Unit work 5 holds the attack count.
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
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 1)
        CALL_CHILD(firebreath_attack_event)
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ATTACK_STEP, 2)
        CALL_CHILD(fumituke_attack_event)
        CASE_DEFAULT
        CALL(evt_sub_random, 99, LW(0))
        IF_INT_LESS(LW(0), 50)
            CALL_CHILD(fumituke_attack_event)
        ELSE
            CALL_CHILD(firebreath_attack_event)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

/**
 * Hooktail's stomp ("fumituke" = trampling): she picks a target, steps forward (GNB_W_2, SFX_BOSS_GNB_MOVE1 and
 * _leg_smoke_effect at each foot) and brings her claw down (GNB_A_3, SFX_BOSS_GNB_ARM_UP1 and ARM_DOWN1).
 * The target dodges or takes the damage with its guard result; then she steps back (GNB_W_3) without having
 * moved her unit.
 */
static EVT_BEGIN(fumituke_attack_event)
    // Hooktail picks a target and declares the stomp.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_gonbaba_fumituke_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_gonbaba_fumituke_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_gonbaba_fumituke_attack))
    // She steps forward on heavy feet, and the camera shakes at each footfall.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_W_2"))
    CALL(btlevtcmd_ftomsec, 56, LW(0))
    WAIT_MS(LW(0))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -300, 0, -30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -300)
        ADD(LW(2), -30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 46, LW(0))
    WAIT_MS(LW(0))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -300, 0, 30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -300)
        ADD(LW(2), 30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 1, LW(0))
    WAIT_MS(LW(0))
    // She raises her claw and brings it down on the target.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A_3"))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_ARM_UP1"), EVT_NULLPTR, 0, -320, 30, 30, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 42, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_ARM_DOWN1"), EVT_NULLPTR, 0, -360, 50, 30, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(btlevtcmd_ftomsec, 47, LW(0))
    WAIT_MS(LW(0))
    // The damage check decides whether the target dodges or takes the stomp.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gonbaba_fumituke_attack), 256, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), 38)
        GOTO(90)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), 39)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), 40)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_DEFAULT
        GOTO(97)
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gonbaba_fumituke_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gonbaba_fumituke_attack), 256, LW(5))
    GOTO(97)
    LABEL(97)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -400, 0, -5, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -400)
        ADD(LW(2), -5)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    // The stomp lands and the camera shakes hard.
    WAIT_FRAMES(3)
    CALL(evt_btl_camera_shake_h, 0, 5, 0, 10, 13)
    WAIT_FRAMES(7)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gonbaba_fumituke_attack))
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 30, 13)
    WAIT_FRAMES(35)
    // Hooktail steps back to her place.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_W_3"))
    CALL(btlevtcmd_ftomsec, 51, LW(0))
    WAIT_MS(LW(0))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -180, 0, 30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -180)
        ADD(LW(2), 30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    CALL(btlevtcmd_ftomsec, 46, LW(0))
    WAIT_MS(LW(0))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -100, 0, 30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -100)
        ADD(LW(2), -30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Hooktail's fire breath: she declares an attack on every target, raises her neck (GNB_A_1,
 * SFX_BOSS_GNB_NECK_UP1) and breathes the gonbaba_breath flame effect along the stage
 * (SFX_BOSS_GNB_BREATH_FIRE1); each target in turn then dodges or takes weapon_gonbaba_firebreath_attack.
 */
EVT_BEGIN(firebreath_attack_event)
    // Hooktail declares the fire breath on every target.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_gonbaba_firebreath_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_gonbaba_firebreath_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gonbaba_firebreath_attack))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_gonbaba_firebreath_attack))
    // She rears her head up on her long neck.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A_1"))
    SET_VALUE(LW(10), 120)
    SET_VALUE(LW(11), 30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(6), 360)
    ADD(LW(7), 30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(9), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(9), 360)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(9), LW(10))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 30, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_NECK_UP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 105, LW(0))
        WAIT_MS(LW(0))
        // She breathes a line of fire along the stage.
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_BREATH_FIRE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        ADD(LW(11), 45)
        SUBTRACT(LW(8), 10)
        CALL(evt_eff, 0, STRING("gonbaba_breath"), 0, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(12), EVT_NULLPTR, EVT_NULLPTR)
    SUBTRACT(LW(12), LW(6))
    IF_INT_LESS(LW(12), 0)
        MULTIPLY(LW(12), -1)
    END_IF
    ADD(LW(12), 20)
    MULTIPLY(LW(12), LW(11))
    DIVIDE(LW(12), LW(10))
    ADD(LW(12), 105)
    // The flame travels toward the target before the damage check.
    WAIT_FRAMES(LW(12))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gonbaba_firebreath_attack), 256, LW(5))
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
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gonbaba_firebreath_attack))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gonbaba_firebreath_attack), 256, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        // Each target in turn dodges or takes the burn.
        WAIT_FRAMES(20)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    // Hooktail lowers her head and the attack ends.
    WAIT_FRAMES(84)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Idle event: sets the unit's body part 1 to the pose given by its pose table for its current state. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
