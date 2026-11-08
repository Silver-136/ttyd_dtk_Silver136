// tst's copy of Yoshi's partner unit (src/unit/party/unit_party_yoshi.c), an earlier version: built with deferred
// inlining (functions emit in reverse order), older weapon icon and sweat texture ids, the older BattleUnit layout
// (tst_battle.h), and seven extra helpers for a brake-slide normal attack that no script here uses.
#include "version.h"
#include "driver/camdrv.h"
#include "data_tables.h"
#include "party/party_motion.h"
#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_stage.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_party.h"
#include "system.h"
#include "driver/animdrv.h"
#include "pmario_sound.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "tst_battle.h"
#include "battle/battle_disp.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_env.h"
#include "evt/evt_sub.h"
#include "battle/battle_attack_audience.h"
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "unit/party/unit_party_yoshi.h"
#include "battle/unit/unit_party_yoshi.h"

// FAKE: _btlYoshiDisp calls btlDispTexPlane through this float-first prototype so the float arguments are evaluated
// first; same registers as (id, color, 0, x, y, z, sX, sY), behaviour-identical.
typedef void (*BtlDispTexPlaneArgOrderFn)(f32, f32, f32, f32, f32, u32, GXColor, s32);

#if VERSION <= VERSION_DEMO_JP
static void _btlYoshiDisp(CameraId camera, YoshiGundanWork* w);
#else
static void _btlYoshiDisp(CameraId camera, YoshiGundanPart* p);
#endif

static void _yoshi_slide_move_sound(BattleUnit* unit, PoseSoundTiming* s);
static void __makeTechMenuFunc(BattleCommand* command, s32* count);

s32 _gundan_part_update(YoshiGundanPart* p, s32 i);
#include "eff/eff_kemu_test.h"
#include "eff/n64/eff_sweat_n64.h"
#include "data_tables.h"
#include "battle/battle_damage.h"
#include "mario/mario_pouch.h"

// Slot 3 is set to -1 when Ground Pound starts (as the partner Yoshi does) and never read, so it keeps its number.

typedef struct YoshiSweatWork {
    s32 pad0;
    f32 x;
    f32 y;
    f32 z;
} YoshiSweatWork;

USER_FUNC(yoshi_original_color_anim_set);
USER_FUNC(_gundan_yoshi_run_effect);
USER_FUNC(_wait_yoshig_complete);
USER_FUNC(_wait_yoshig_run);
USER_FUNC(_check_swallow_attribute);
USER_FUNC(_get_swallow_param);

#if VERSION >= VERSION_JP
USER_FUNC(_get_nomikomi_hit_position);
#endif

PoseTableEntry pose_table_yoshi_stay[25];
static BattleDataEntry data_table[8];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
PoseTableEntry pose_table_egg_g[4];
PoseTableEntry pose_table_egg_y[4];
PoseTableEntry pose_table_egg_p[4];
BattleUnitKindPart unitpartsdata_Party_Yoshi[7];
BattleUnitKind unit_yoshi;
static EvtWord init_event[54];
static EvtWord damage_event[16];
BattleWeapon partyWeapon_YoshiNormalAttack;
BattleWeapon partyWeapon_YoshiNomikomi_Shot;
BattleWeapon partyWeapon_YoshiNomikomi_Spew;
BattleWeapon partyWeapon_YoshiNomikomi_Dmg0;
BattleWeapon partyWeapon_YoshiNomikomi_Fire;
BattleWeapon partyWeapon_YoshiNomikomi_Involved;
BattleWeapon partyWeapon_YoshiEggAttack_Minimini;
BattleWeapon partyWeapon_YoshiCallGuard;
PoseSoundTiming posesound_normal_attack[15];
extern EvtWord partyYoshiAttack_NormalAttack[];
extern EvtWord partyYoshiAttack_Nomikomi[];
extern EvtWord partyYoshiAttack_EggAttack[];
extern EvtWord _egg_attack_event[];
extern EvtWord partyYoshiAttack_CallGuard[];
extern EvtWord yoshi_appeal_event[];
static EvtWord party_win_reaction[25];
static EvtWord attack_audience_event[290];
static EvtWord attack_event[2];
static EvtWord wait_event[6];
u8 _yoshig_tpl_no_tbl[8] __attribute__((aligned(4)));

/** Yoshi's damage reduction: none. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Yoshi has no elemental defence. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Yoshi is fully vulnerable to every status effect (100 for each). */
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

/**
 * Yoshi's pose table (PYS_*): standing, walking, running, hurt, command-menu wait, appeal,
 * talk and ceiling/zero-gravity poses.
 */
PoseTableEntry pose_table_yoshi_stay[] = {
    {9, "PYS_D_5"},
    {2, "PYS_D_5"},
    {3, "PYS_X_1"},
    {4, "PYS_D_2"},
    {5, "PYS_X_1"},
    {1, "PYS_N_1"},
    {27, "PYS_D_3"},
    {POSE_TYPE_NORMAL, "PYS_S_1"},
    {37, "PYS_G_2"},
    {31, "PYS_G_1"},
    {POSE_TYPE_DAMAGE, "PYS_D_1"},
    {POSE_TYPE_WALK, "PYS_W_1"},
    {POSE_TYPE_RUN, "PYS_R_1"},
    {POSE_TYPE_PERIL, "PYS_D_2"},
    {POSE_TYPE_DANGER, "PYS_D_2"},
    {POSE_TYPE_ESCAPE_STUMBLE, "PYS_D_4"},
    {POSE_TYPE_ESCAPE_RECOVER, "PYS_D_2"},
    {POSE_TYPE_TALK, "PYS_T_1"},
    {POSE_TYPE_ITEM, "PYS_I_1"},
    {POSE_TYPE_COMMAND, "PYS_C_1"},
    {POSE_TYPE_APPEAL, "PYS_O_1"},
    {POSE_TYPE_ZERO_GRAVITY, "PYS_F_1"},
    {49, "PYS_F_1"},
    {POSE_TYPE_DEFEND, "PYS_G_1"},
    {POSE_TYPE_ACTION, "PYS_S_1"},
};

/** Egg poses of the first colour (green, CHU_C_1): the same frame for every state. */
PoseTableEntry pose_table_egg_g[] = {
    {POSE_TYPE_NORMAL, "CHU_C_1"},
    {31, "CHU_C_1"},
    {POSE_TYPE_DAMAGE, "CHU_C_1"},
    {POSE_TYPE_ACTION, "CHU_C_1"},
};

/** Egg poses of the second colour (yellow, CHU_C_2): the same frame for every state. */
PoseTableEntry pose_table_egg_y[] = {
    {POSE_TYPE_NORMAL, "CHU_C_2"},
    {31, "CHU_C_2"},
    {POSE_TYPE_DAMAGE, "CHU_C_2"},
    {POSE_TYPE_ACTION, "CHU_C_2"},
};

/** Egg poses of the third colour (pink, CHU_C_3): the same frame for every state. */
PoseTableEntry pose_table_egg_p[] = {
    {POSE_TYPE_NORMAL, "CHU_C_3"},
    {31, "CHU_C_3"},
    {POSE_TYPE_DAMAGE, "CHU_C_3"},
    {POSE_TYPE_ACTION, "CHU_C_3"},
};

/**
 * Data table of Yoshi: event 5 is his Appeal, 6 his audience attack, 1 builds his move menu,
 * 48 (First Strike) does nothing, 64 is the victory reaction,
 * 7 readies him for an attack and 47 is the default counter-avoid reaction.
 */
static BattleDataEntry data_table[] = {
    {5, yoshi_appeal_event},
    {6, attack_audience_event},
    {1, __makeTechMenuFunc},
    {48, btldefaultevt_Dummy},
    {64, party_win_reaction},
    {7, subset_attack_ready_jump_event},
    {47, btldefaultevt_avoid_counter_event},
    {0, NULL},
};

/**
 * Yoshi's parts: part 1 is his body (the Yoshi model) and parts 2 to 7 are the six eggs that Mini-Egg throws.
 */
BattleUnitKindPart unitpartsdata_Party_Yoshi[] = {
    {1, "btl_un_yoshi", "c_babyyoshi", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table_yoshi_stay},
    {2, "btl_un_yoshi", "c_tamago", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_egg_g},
    {3, "btl_un_yoshi", "c_tamago", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_egg_g},
    {4, "btl_un_yoshi", "c_tamago", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_egg_g},
    {5, "btl_un_yoshi", "c_tamago", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_egg_g},
    {6, "btl_un_yoshi", "c_tamago", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_egg_g},
    {7, "btl_un_yoshi", "c_tamago", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_egg_g},
};

/**
 * Unit data of Yoshi, Mario's partner (kind 226), in the test-area copy: his sounds, size,
 * offsets and part list; the real HP and moves come from the party data and his move menu.
 */
BattleUnitKind unit_yoshi = {
    226,
    "btl_un_yoshi",
    10,
    5,
    5,
    1,
    1,
    0,
    0,
    0,
    0,
    100,
    9,
    20,
    33,
    8,
    28,
    0.0f,
    12.0f,
    0.0f,
    0,
    0,
    0.0f,
    40.0f,
    0.0f,
    0.0f,
    40.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    10.0f,
    0.0f,
    0.0f,
    10.0f,
    21.45f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_PARTY_DAMAGE1",
    "SFX_MARIO_DAMAGE_FIRE1",
    "SFX_MARIO_DAMAGE_ICE1",
    "SFX_MARIO_DAMAGE_BIRIBIRI1",
    kUnitNoHpGauge,
    &regist,
    7,
    unitpartsdata_Party_Yoshi,
    init_event,
    data_table,
};

/**
 * Yoshi's battle entry: starts 250 units behind his home spot and, after 40 frames,
 * runs in to it at speed 4 with his footstep sounds, then takes his stay pose.
 */
static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 6, 6, 0, 0)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -2, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Init for Yoshi: registers his wait, attack, damage, confusion and entry scripts and his walk,
 * run and jump sounds, clears UNIT_WORK_GROUND_POUND (set while Ground Pound runs)
 * and loads the model for his hatched colour.
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUND_POUND, 0)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 6, 6)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(yoshi_original_color_anim_set, UNIT_ID_SELF)
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Hit reaction: straightens his rotation (left over from his moves), then runs the default damage script. */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/** Empty: partner moves come from the player's menu choices, not a unit attack script. */
static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

/** Idle: plays Yoshi's stay pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Yoshi's Ground Pound (btl_wn_pys_normal), his basic attack: 1 damage per stomp to a single enemy,
 * no FP cost, a jump-like move that cannot hit the ceiling and is repeated by tapping R.
 */
BattleWeapon partyWeapon_YoshiNormalAttack = {
    "btl_wn_pys_normal", // name
#if VERSION <= VERSION_DEMO_JP
    335,
#else
    343, // icon (tst: one below the US id)
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pys_hip_drop", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {1, 1, 1, 1, 1, 1, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_hip_drop", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_HIT | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_PREEMPTIVE_SPIKY | COUNTER_RESIST_FRONT_SPIKY, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyYoshiAttack_NormalAttack, // attackEvtScript
#if VERSION <= VERSION_DEMO_JP
    {0, 10, 10, 100, 10, 5, 5, 5, 5, 5, 0, 0},
#else
    {0, 10, 10, 100, 10, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#endif
};

/**
 * Yoshi's Gulp as listed in his move menu (btl_wn_pys_lv1, 4 FP):
 * the swallowed enemy is spat at the front-most enemy, with damage values from the table 2/4/3/5/3/6;
 * it ignores Defense and cannot be guarded.
 */
BattleWeapon partyWeapon_YoshiNomikomi_Shot = {
    "btl_wn_pys_lv1", // name
#if VERSION <= VERSION_DEMO_JP
    336,
#else
    344, // icon (tst: one below the US id)
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pys_nomikomi", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {2, 4, 3, 5, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    6, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_nomikomi", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ELECTRIC | COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_FRONT_SPIKY | COUNTER_RESIST_FIERY | COUNTER_RESIST_ICY | COUNTER_RESIST_POISON | COUNTER_RESIST_EXPLOSIVE | COUNTER_RESIST_VOLATILE_EXPLOSIVE | COUNTER_RESIST_PAYBACK | COUNTER_RESIST_HOLD_FAST, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyYoshiAttack_Nomikomi, // attackEvtScript
#if VERSION <= VERSION_DEMO_JP
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0},
#else
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * Variant of Gulp's spat-out hit used for an enemy without swallow flag 2 (damage pattern 5);
 * Yoshi is immune to every counter effect when it connects.
 */
BattleWeapon partyWeapon_YoshiNomikomi_Spew = {
    "btl_wn_pys_lv1", // name
#if VERSION <= VERSION_DEMO_JP
    336,
#else
    344, // icon (tst: one below the US id)
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pys_nomikomi", // description
    100, // baseAccuracy
    4, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {2, 4, 3, 5, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    5, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_nomikomi", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyYoshiAttack_Nomikomi, // attackEvtScript
#if VERSION <= VERSION_DEMO_JP
    {0, 0, 0, 100, 0, 5, 5, 5, 0, 0, 0, 0},
#else
    {0, 0, 0, 100, 0, 10, 5, 5, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * Gulp's no-damage hit: used when the tongue only touches an enemy that cannot be swallowed,
 * or the action command failed; it deals 0 damage.
 */
BattleWeapon partyWeapon_YoshiNomikomi_Dmg0 = {
    "btl_wn_pys_lv1", // name
#if VERSION <= VERSION_DEMO_JP
    336,
#else
    344, // icon (tst: one below the US id)
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pys_nomikomi", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_SINGLE, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_nomikomi", // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyYoshiAttack_Nomikomi, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Gulp's fire breath: after eating an enemy with swallow attribute 4 Yoshi breathes fire on every enemy for 1 damage of the
 * fire element, ignoring Defense.
 */
BattleWeapon partyWeapon_YoshiNomikomi_Fire = {
    "btl_wn_pys_lv1", // name
#if VERSION <= VERSION_DEMO_JP
    336,
#else
    344, // icon (tst: one below the US id)
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pys_nomikomi", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    0, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_nomikomi", // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyYoshiAttack_Nomikomi, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Hit taken by enemies caught in Gulp's spat-out enemy (a hammer-like recoil hit across all enemies,
 * ignoring Defense); not referenced by the scripts in this file.
 */
BattleWeapon partyWeapon_YoshiNomikomi_Involved = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, 0.0f, 0, 0, 0, 0, // baseFpCost..reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {1, 4, 2, 5, 3, 6, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
#if VERSION <= VERSION_DEMO_JP
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST,
#else
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_RECOIL | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
#endif
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
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
 * Yoshi's Mini-Egg (btl_wn_pys_lv2, 3 FP): eggs thrown at randomly sampled enemies for 1 damage each that
 * shrink them; each timed press during the action command lays one more egg.
 */
BattleWeapon partyWeapon_YoshiEggAttack_Minimini = {
    "btl_wn_pys_lv2", // name
#if VERSION <= VERSION_DEMO_JP
    337,
#else
    345, // icon (tst: one below the US id)
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pys_wonder_egg", // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 1, 1, 1, 1, 1, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_wonder_egg", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_UNGUARDABLE, // special
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
    partyYoshiAttack_EggAttack, // attackEvtScript
#if VERSION <= VERSION_DEMO_JP
    {0, 0, 0, 100, 0, 3, 3, 0, 0, 0, 0, 0},
#else
    {0, 0, 0, 100, 0, 6, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * Yoshi's Stampede (btl_wn_pys_lv3, 6 FP): the herd of Yoshis that runs across the stage;
 * 1 damage per hit to every enemy, repeated as the action command is mashed.
 * Its attack script is partyYoshiAttack_CallGuard.
 */
BattleWeapon partyWeapon_YoshiCallGuard = {
    "btl_wn_pys_lv3", // name
#if VERSION <= VERSION_DEMO_JP
    338,
#else
    346, // icon (tst: one below the US id)
#endif
    {0}, // pad_06
    0, // itemId
    "msg_pys_taigun_yoshi", // description
    100, // baseAccuracy
    6, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerFromPartyAttackLv, // hpDamageFn
    {1, 1, 1, 1, 1, 1, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SELF | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, 0, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_taigun_yoshi", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_DIMINISH_BY_HIT | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyYoshiAttack_CallGuard, // attackEvtScript
#if VERSION <= VERSION_DEMO_JP
    {0, 20, 20, 100, 20, 20, 5, 5, 20, 5, 0, 0},
#else
    {0, 20, 20, 100, 20, 40, 5, 5, 20, 5, 0, 0}, // stageEffectRates
#endif
};

/**
 * Footstep sounds for Ground Pound's run-up and slide, one per step of each pose;
 * their pitch follows the run's power.
 */
PoseSoundTiming posesound_normal_attack[] = {
    {"PYS_W_1", 0.01f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1L", 0, 1},
    {"PYS_W_1", 0.25f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1R", 0, 1},
    {"PYS_R_1", 0.01f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1L", 0, 1},
    {"PYS_R_1", 0.1f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1R", 0, 1},
    {"PYS_R_4", 0.01f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1L", 1364, 1},
    {"PYS_R_4", 0.05f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1R", 1364, 1},
    {"PYS_R_4", 0.1f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1L", 1364, 1},
    {"PYS_R_4", 0.15f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1R", 1364, 1},
    {"PYS_R_5", 0.01f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1L", 2387, 1},
    {"PYS_R_5", 0.033333335f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1R", 2387, 1},
    {"PYS_R_5", 0.083333336f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1L", 2387, 1},
    {"PYS_R_5", 0.11666667f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1R", 2387, 1},
    {"PYS_A_3", 0.01f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1L", 2387, 1},
    {"PYS_A_3", 0.083333336f, _yoshi_slide_move_sound, "SFX_PARTY_BATTLE_MOVE1R", 2387, 1},
    {NULL, 0.0f, _yoshi_slide_move_sound, NULL, 0, 1},
};

/**
 * Yoshi's basic attack (partyWeapon_YoshiNormalAttack), first entry of his move menu:
 * a charge-up (PYS_W_1, PYS_R_5) while a sibling event ramps the step sounds' pitch,
 * then a jump and flutter over the chosen enemy (btl_yoshi_yoroyoro_jump_*) with an action command whose
 * result sets how many times he pounds it.
 * Its weapon name btl_wn_pys_normal is Ground Pound in English (description msg_pys_hip_drop).
 */
EVT_BEGIN(partyYoshiAttack_NormalAttack)
    // The enemy chosen in the menu is the target; the attack is declared and protecting-unit movement is waited out.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, 7)

    // The camera pulls out while Yoshi turns to face the enemy.
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUND_POUND, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, -1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_STEP_PITCH, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_STEP_PITCH_SCALE, 100)
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(posesound_normal_attack))
    CALL(btlevtcmd_AcSetOutputParam, 1, 0)

    // He winds up in place and switches to the run pose while the step sounds' pitch ramps up over 25 frames.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_W_1"))
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(0), 0)
        LOOP(25)
            ADD(LW(0), 4)
            SET_VALUE(LW(1), 2387)
            MULTIPLY(LW(1), LW(0))
            DIVIDE(LW(1), 100)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_STEP_PITCH, LW(1))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_R_5"))
    WAIT_FRAMES(25)

    // The camera frames the enemy as he turns toward it and leaps up in front of it.
    CALL(evt_btl_camera_set_mode, 0, 11)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_J_1A"))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 25)
    ADD(LW(1), 35)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_CommandGetWeaponActionLv, LW(15))
        CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_GetPartyTechLv, UNIT_ID_SELF, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AcSetParamAll, 5, 180, 178, 23, 25, 20, 61, 1)
            CALL(btlevtcmd_AcSetGaugeParam, 60, 80, 100, 100)
            CASE_EQUAL(1)
            CALL(btlevtcmd_AcSetParamAll, 5, 180, 178, 23, 25, 25, 76, 1)
            CALL(btlevtcmd_AcSetGaugeParam, 75, 100, 100, 100)
            CASE_DEFAULT
            CALL(btlevtcmd_AcSetParamAll, 5, 180, 178, 23, 25, 34, 100, 1)
            CALL(btlevtcmd_AcSetGaugeParam, 100, 100, 100, 100)
        END_SWITCH
        CALL(btlevtcmd_AcSetFlag, 72)
        CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 6, 1, 0)
    END_SIBLING_EVENT

    // He rises and the Ground Pound action command starts: tap R repeatedly;
    // the gauge fills faster at higher move levels.
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 25, -1)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_3"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    ADD(LW(7), 40)
    SET_VALUE(LW(10), FLOAT(1.0f))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_StartAC, 1)
        CALL(btlevtcmd_ResultAC)
    END_SIBLING_EVENT

    // He wobbles through the air and drops onto the enemy.
    CALL(btl_yoshi_yoroyoro_jump_calc_param, UNIT_ID_SELF, LW(6), LW(7), LW(8), LW(10), LW(9))
    CALL(btl_yoshi_yoroyoro_jump_move, UNIT_ID_SELF, LW(6), LW(7), LW(8), LW(10), LW(9))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    SET_VALUE(LW(13), 1)
    CALL(btlevtcmd_AcGetOutputParam, 2, LW(14))

    // Each stomp: the game checks whether it connects or is avoided.
    LABEL(50)
    IF_INT_GREATER_OR_EQUAL(LW(13), 2)
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), 1048832, LW(5))
    ELSE
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), 256, LW(5))
    END_IF
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_MISS)
        CASE_OR_EQUAL(PRECHECK_LUCKY)
        CASE_OR_EQUAL(PRECHECK_DODGY)
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        CASE_END
        CASE_EQUAL(PRECHECK_HIT)
        CASE_DEFAULT
    END_SWITCH
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUND_POUND, 0)
    IF_INT_LESS(LW(13), 2)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 20, 20, 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    ELSE
        SET_VALUE(LW(6), 1)
    END_IF
    IF_INT_EQUAL(LW(13), 1)
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_J_1B"))
        IF_INT_EQUAL(LW(6), 2)
            SET_VALUE(LW(0), 3)
            SET_VALUE(LW(1), 6)
            MULTIPLY(LW(15), -60)
        ELSE
            SET_VALUE(LW(0), 1)
            SET_VALUE(LW(1), 9)
            MULTIPLY(LW(15), -40)
        END_IF
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 12, 0)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        WAIT_FRAMES(15)
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, -60, -25, 0)
        END_IF
        LOOP(LW(0))
            SET_VALUE(LW(2), LW(1))
            CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_ATTACK1"), 0)
            LOOP(LW(2))
                CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(15))
                WAIT_FRAMES(1)
            END_LOOP
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_1"))
        WAIT_FRAMES(15)
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_1"))
    END_IF
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 4, 0, 0, 0, -1)
    IF_INT_NOT_EQUAL(LW(5), PRECHECK_HIT)
        CALL(btlevtcmd_StopAC)
        IF_INT_EQUAL(LW(5), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_VALUE(LW(15), LW(1))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        SUBTRACT(LW(15), LW(1))
        DIVIDE(LW(15), 10)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(15), 0, 0, 0, -1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_D_1"))
        CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, 0, -7, 0)
        CALL(evt_btl_camera_shake_h, 0, 4, 0, 10, 13)
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_5"))
        CALL(btlevtcmd_SetDispOffset, UNIT_ID_SELF, 0, 0, 0)
        WAIT_FRAMES(54)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    END_IF
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_GetResultAC, LW(0))

    // The stomp deals its damage either way; a good command also shows the rating effect, and the audience
    // reacts to the result.
    IF_BITS_SET(LW(0), 2)
        IF_INT_GREATER_OR_EQUAL(LW(13), LW(14))
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), 131328, LW(5))
        ELSE
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), 131072, LW(5))
        END_IF
        CALL(btlevtcmd_GetPartyTechLv, UNIT_ID_SELF, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
            SWITCH(LW(0))
                CASE_GREATER_OR_EQUAL(6)
                SET_VALUE(LW(0), 2)
                CASE_GREATER_OR_EQUAL(4)
                SET_VALUE(LW(0), 1)
                CASE_GREATER_OR_EQUAL(2)
                SET_VALUE(LW(0), 0)
                CASE_DEFAULT
                SET_VALUE(LW(0), -1)
            END_SWITCH
            CASE_EQUAL(1)
            CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
            SWITCH(LW(0))
                CASE_GREATER_OR_EQUAL(4)
                SET_VALUE(LW(0), 1)
                CASE_GREATER_OR_EQUAL(2)
                SET_VALUE(LW(0), 0)
                CASE_DEFAULT
                SET_VALUE(LW(0), -1)
            END_SWITCH
            CASE_DEFAULT
            CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
            SWITCH(LW(0))
                CASE_GREATER_OR_EQUAL(4)
                SET_VALUE(LW(0), 1)
                CASE_GREATER_OR_EQUAL(2)
                SET_VALUE(LW(0), 0)
                CASE_DEFAULT
                SET_VALUE(LW(0), -1)
            END_SWITCH
        END_SWITCH
        IF_INT_GREATER_OR_EQUAL(LW(0), 0)
            SET_VALUE(LW(1), LW(0))
            SET_VALUE(LW(2), LW(13))
            SUBTRACT(LW(2), 1)
            IF_INT_GREATER(LW(1), LW(2))
                SET_VALUE(LW(1), LW(2))
            END_IF
            CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(1), LW(6))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(9), LW(1), LW(2))
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(9), LW(1), LW(2))
            SET_VALUE(LW(1), LW(13))
            ADD(LW(1), 1)
            IF_INT_GREATER(LW(1), LW(14))
                CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(6))
                CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
            END_IF
        ELSE
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
    ELSE
        IF_INT_GREATER_OR_EQUAL(LW(13), LW(14))
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), 256, LW(5))
        ELSE
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), 0, LW(5))
        END_IF
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF

    // After each stomp he bounces back up for the next one while the command allows.
    LABEL(90)
    ADD(LW(13), 1)
    IF_INT_LESS_OR_EQUAL(LW(13), LW(14))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
        ADD(LW(1), 40)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, -1)
        GOTO(50)
    END_IF

    // After the last stomp he hops back; a well-timed Stylish press adds a flourish the audience likes.
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 15, 15, 20)
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, -1)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)

    // The camera resets and Yoshi runs back to his home spot.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Yoshi's Gulp (weapons named btl_wn_pys_lv1, Gulp in English: inhale an enemy and spit it out at another),
 * with an action command whose difficulty comes from the move level.
 * A dodged tongue plays the target's avoid; a failed command
 * or an enemy that cannot be swallowed (_get_swallow_param -1) only takes the no-damage hit
 * (partyWeapon_YoshiNomikomi_Dmg0); an enemy with swallow attribute 8 only plays its damage pattern 7
 * reaction and is not swallowed.
 * Otherwise the enemy is gulped (damage pattern 2).
 * If a roll of 0 to 99 reaches its swallow value he spits it out (partyWeapon_YoshiNomikomi_Shot,
 * or _Spew when its swallow attribute lacks bit 2);
 * else it is eaten outright (Star Points kept, unit killed),
 * and an eaten enemy with swallow attribute 4, but not attribute 1, makes him breathe fire at every enemy
 * (partyWeapon_YoshiNomikomi_Fire). partyWeapon_YoshiNomikomi_Involved is not used here.
 */
EVT_BEGIN(partyYoshiAttack_Nomikomi)
    // The enemy chosen in the menu is the target; Gulp is declared and its FP cost paid.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SWALLOWED, 0)
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, 7)

    // The Gulp action command is set up (press R and release when the light comes on),
    // tighter at higher move levels.
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(15))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 160)
        SET_VALUE(LW(1), 40)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 140)
        SET_VALUE(LW(1), 30)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 120)
        SET_VALUE(LW(1), 25)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 100)
        SET_VALUE(LW(1), 20)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 80)
        SET_VALUE(LW(1), 10)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 50)
        SET_VALUE(LW(1), 5)
        CASE_DEFAULT
        SET_VALUE(LW(0), 30)
        SET_VALUE(LW(1), 2)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 3, LW(0), LW(1), 0, 100, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 15)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 15, 1, 0)

    // The camera frames Yoshi and the target, then the command starts.
    CALL(evt_btl_camera_set_mode, 0, 10)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, -1, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    LABEL(5)
    CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(5)
    END_IF

    // Against a pre-emptive spiky enemy he walks up to it first and takes the spike damage on the way.
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), 256, LW(5))
    IF_INT_EQUAL(LW(5), PRECHECK_PREEMPTIVE_SPIKY)
#if VERSION <= VERSION_DEMO_JP
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY(LW(6), 40)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
#else
        CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
        MULTIPLY_FLOAT(LW(6), 40)
        CALL(_get_nomikomi_hit_position, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
#endif
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 80, -1, 0)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), 256, LW(5))
    END_IF
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
#if VERSION <= VERSION_DEMO_JP
    MULTIPLY(LW(6), 40)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
#else
    MULTIPLY_FLOAT(LW(6), 40)
    CALL(_get_nomikomi_hit_position, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
#endif

    // He walks up to the enemy and faces it.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 80, -1, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_ResultAC)

    // His tongue shoots out.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_2"))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW1"), 0)
    IF_INT_NOT_EQUAL(LW(5), PRECHECK_HIT)
        IF_INT_EQUAL(LW(5), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        WAIT_FRAMES(50)
        GOTO(90)
    END_IF
    WAIT_FRAMES(7)

    // An enemy that cannot be swallowed, or a failed command, only gets the no-damage hit.
    CALL(_get_swallow_param, LW(3), LW(7))
    IF_INT_EQUAL(LW(7), -1)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_YoshiNomikomi_Dmg0), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_GULP, LW(5))
        WAIT_FRAMES(43)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_CLEAR(LW(0), 2)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_YoshiNomikomi_Dmg0), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_GULP, LW(5))
        WAIT_FRAMES(43)
        GOTO(90)
    END_IF
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW2"), 0)

    // An enemy with swallow flag 8 only plays its gulp reaction (damage pattern 7); Yoshi lets it go.
    CALL(_check_swallow_attribute, LW(3), 8, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 7, 1)
        WAIT_FRAMES(13)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_7"))
        CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW5"), 0)
        WAIT_FRAMES(34)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_5"))
        WAIT_FRAMES(54)
        GOTO(90)
    END_IF
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)

    // The tongue pulls the enemy in, the camera zooms in and the swallow sounds play.
    CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 2, 1)
    WAIT_FRAMES(3)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 12)
    CALL(btlevtcmd_DivePosition, LW(3), LW(0), LW(1), LW(2), 5, 0, 0, 0, -1)
    CALL(btlevtcmd_OnAttribute, LW(3), kUnitHidden)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW3"), 0)
    WAIT_FRAMES(6)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_3"))
    WAIT_FRAMES(7)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)

    // A random roll against the enemy's swallow value: a high roll spits it out at another enemy,
    // a low one swallows it.
    CALL(evt_sub_random, 99, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), LW(7))
        BEGIN_INLINE_EVENT
            CALL(evt_env_blur_on, 0, 600)
        END_INLINE_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_4"))
        CALL(btlevtcmd_ftomsec, 22, LW(0))
        WAIT_MS(LW(0))
        BEGIN_INLINE_EVENT_GET_ID(LW(15))
            CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_NOMIKOMI6"), 0)
            CALL(btlevtcmd_OffAttribute, LW(3), kUnitHidden)
            CALL(_check_swallow_attribute, LW(3), 2, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_YoshiNomikomi_Spew), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_GULP, LW(5))
            ELSE
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_YoshiNomikomi_Shot), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_GULP, LW(5))
            END_IF
            CALL(btlevtcmd_GetResultAC, LW(0))
            IF_BITS_SET(LW(0), 2)
                CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
            ELSE
                CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
            END_IF
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 15, 15, 20)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        END_INLINE_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        GOTO(90)
    END_IF

    // Eaten: the enemy vanishes from battle and its Star Points are kept for the party.
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SWALLOWED, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW5"), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_7"))
    WAIT_FRAMES(26)
    CALL(_check_swallow_attribute, LW(3), 1, LW(1))
    CALL(_check_swallow_attribute, LW(3), 4, LW(2))
    CALL(btlevtcmd_GetExp, LW(3), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_KillUnit, LW(3), 0)
    WAIT_FRAMES(8)
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_6"))
        CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW7"), 0)
        WAIT_FRAMES(50)
        GOTO(90)
    ELSE
        IF_INT_EQUAL(LW(2), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_5"))
            CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW4"), 0)
            WAIT_FRAMES(54)
            GOTO(90)
        END_IF
    END_IF

    // An enemy with swallow flag 4, but not flag 1, makes him breathe fire on every enemy.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_4"))
    WAIT_FRAMES(22)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(18)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 15)
    SET_VALUE(LW(3), LW(0))
    SET_VALUE(LW(4), LW(1))
    SET_VALUE(LW(5), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(3), 240)
    CALL(evt_eff, STRING("eff"), STRING("breath_fire"), 1, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 1, 2, 60, FLOAT(0.5f), FLOAT(30.0f))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&partyWeapon_YoshiNomikomi_Fire))
    CALL(btlevtcmd_GetSelectEnemy, LW(10), LW(11))
    LABEL(10)
    IF_INT_EQUAL(LW(10), -1)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(10), LW(11), PTR(&partyWeapon_YoshiNomikomi_Fire), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_GULP, LW(5))
    CALL(btlevtcmd_GetSelectNextEnemy, LW(10), LW(11))
    GOTO(10)
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)

    // Yoshi runs back to his home spot; a well-timed Stylish press on the way makes him pose and spin.
    LABEL(95)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SWALLOWED, LW(5))
    IF_INT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(4.0f), LW(5))
    IF_INT_LESS_OR_EQUAL(LW(5), 20)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
        GOTO(99)
    END_IF
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SUBTRACT(LW(5), 1)
        SET_VALUE(LW(6), LW(5))
        SUBTRACT(LW(6), 10)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(6), LW(5), LW(5), 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(5), -1, 0)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_O_1"))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_J_1B"))
        CALL(evt_sub_intpl_init, 4, 0, 360, 30)
        LOOP(30)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_IF
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Yoshi's Mini-Egg (partyWeapon_YoshiEggAttack_Minimini, named btl_wn_pys_lv2: eggs that shrink enemies to Tiny,
 * 100 percent for 3 turns): during the action command he lays eggs (parts 2 and up, each given a random yellow,
 * pink or green egg pose table), then throws them at randomly sampled enemies,
 * each flight run by _egg_attack_event.
 */
EVT_BEGIN(partyYoshiAttack_EggAttack)
    // The target is chosen; the attack is declared and its FP cost paid.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, 7)

    // Each of the six eggs is given a randomly chosen enemy to fly at.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&partyWeapon_YoshiEggAttack_Minimini))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&partyWeapon_YoshiEggAttack_Minimini), LW(3), LW(4))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 2, PARTS_WORK_TARGET, LW(3))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 2, PARTS_WORK_TARGET_PART, LW(4))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 2, PARTS_WORK_PRECHECK, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 2, PARTS_WORK_LAST_AT_TARGET, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 2, PARTS_WORK_EVENT, 0)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&partyWeapon_YoshiEggAttack_Minimini))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&partyWeapon_YoshiEggAttack_Minimini), LW(10), LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 7, PARTS_WORK_TARGET, LW(10))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 7, PARTS_WORK_TARGET_PART, LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 7, PARTS_WORK_PRECHECK, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 7, PARTS_WORK_LAST_AT_TARGET, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 7, PARTS_WORK_EVENT, 0)
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&partyWeapon_YoshiEggAttack_Minimini))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&partyWeapon_YoshiEggAttack_Minimini), LW(10), LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 6, PARTS_WORK_TARGET, LW(10))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 6, PARTS_WORK_TARGET_PART, LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 6, PARTS_WORK_PRECHECK, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 6, PARTS_WORK_LAST_AT_TARGET, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 6, PARTS_WORK_EVENT, 0)
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&partyWeapon_YoshiEggAttack_Minimini))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&partyWeapon_YoshiEggAttack_Minimini), LW(10), LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 5, PARTS_WORK_TARGET, LW(10))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 5, PARTS_WORK_TARGET_PART, LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 5, PARTS_WORK_PRECHECK, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 5, PARTS_WORK_LAST_AT_TARGET, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 5, PARTS_WORK_EVENT, 0)
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&partyWeapon_YoshiEggAttack_Minimini))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&partyWeapon_YoshiEggAttack_Minimini), LW(10), LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 4, PARTS_WORK_TARGET, LW(10))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 4, PARTS_WORK_TARGET_PART, LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 4, PARTS_WORK_PRECHECK, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 4, PARTS_WORK_LAST_AT_TARGET, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 4, PARTS_WORK_EVENT, 0)
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&partyWeapon_YoshiEggAttack_Minimini))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&partyWeapon_YoshiEggAttack_Minimini), LW(10), LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, PARTS_WORK_TARGET, LW(10))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, PARTS_WORK_TARGET_PART, LW(11))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, PARTS_WORK_PRECHECK, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, PARTS_WORK_LAST_AT_TARGET, 0)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, PARTS_WORK_EVENT, 0)

    // The camera frames Yoshi and the Mini-Egg action command is set up.
    CALL(evt_btl_camera_set_mode, 0, 10)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, -1, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_GetPartyTechLv, UNIT_ID_SELF, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        SET_VALUE(LW(1), 4)
        CASE_DEFAULT
        SET_VALUE(LW(1), 3)
    END_SWITCH
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 480)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 360)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 240)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 180)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 150)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 100)
        CASE_DEFAULT
        SET_VALUE(LW(0), 60)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 0, LW(0), LW(1), 0, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_AcSetOutputParam, 1, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 20, 1, 60)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 272, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(12), 2)

    // While the command runs, each timed press makes Yoshi lay another egg, up to the command's limit.
    LABEL(5)
    CALL(btlevtcmd_AcGetOutputParam, 0, LW(13))
    ADD(LW(13), 1)
    IF_INT_GREATER(LW(12), LW(13))
        CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_StopAC)
            WAIT_MS(500)
            CALL(btlevtcmd_AcGetOutputParam, 0, LW(13))
            IF_INT_GREATER_OR_EQUAL(LW(13), 1)
                CALL(btlevtcmd_AcGetOutputParam, 0, LW(13))
                ADD(LW(13), 1)
                GOTO(25)
            ELSE
                CALL(btlevtcmd_AudienceDeclareACResult, PTR(&partyWeapon_YoshiEggAttack_Minimini), -1)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_5"))
                WAIT_FRAMES(54)
                CALL(evt_audience_ap_recovery)
                CALL(btlevtcmd_InviteApInfoReport)
                GOTO(95)
            END_IF
        END_IF
        WAIT_FRAMES(1)
        GOTO(5)
    END_IF
    SET_VALUE(LW(0), LW(12))
    SUBTRACT(LW(0), 1)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(2)
        SET_VALUE(LW(0), 0)
        CASE_LESS_OR_EQUAL(3)
        SET_VALUE(LW(0), 1)
        CASE_DEFAULT
        SET_VALUE(LW(0), 2)
    END_SWITCH
    CALL(btlevtcmd_GetResultPrizeLv, UNIT_ID_SYSTEM, LW(0), LW(6))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, PTR(&partyWeapon_YoshiEggAttack_Minimini), LW(6))

    // A laid egg gets a random colour (green, yellow or pink) and drops from Yoshi to the floor.
    LABEL(10)
    CALL(evt_sub_random, 2, LW(0))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_EGG_KIND, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(12), PTR(pose_table_egg_y))
        CASE_EQUAL(2)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(12), PTR(pose_table_egg_p))
        CASE_DEFAULT
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(12), PTR(pose_table_egg_g))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(12), POSE_TYPE_ACTION)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), 1, 4)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_WEAPON, PTR(&partyWeapon_YoshiEggAttack_Minimini))
    LABEL(20)
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
    CALL(btlevtcmd_SetPartsScale, UNIT_ID_SELF, LW(12), FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_8"))
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_12"))
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(5))
    MULTIPLY_FLOAT(LW(5), 10)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), LW(5))
    ADD(LW(1), LW(5))
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsDispOffset, UNIT_ID_SELF, LW(12), 0, 0, 0)
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(12), 0, 0, 0)
    CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(12), 0, 0, 0)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(12), kHidden | kNoShadow)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_EGG1"), 0)
    SET_VALUE(LW(15), 0)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(12), 0, 6, 0)
        LOOP(30)
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(12), 0, 0, 48)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(12), 0, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(5), LW(12))
        SUBTRACT(LW(5), 2)
        MULTIPLY_FLOAT(LW(5), 10)
        ADD_FLOAT(LW(5), 15)
        CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(0))
        MULTIPLY_FLOAT(LW(5), LW(0))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), LW(5))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, LW(12), FLOAT(0.19921875f))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2), 30, -1)
    END_SIBLING_EVENT
    WAIT_FRAMES(10)
    ADD(LW(12), 1)
    GOTO(5)

    // Eggs aimed at the same enemy are grouped so that only the last of them counts as the final hit.
    LABEL(25)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 2, PARTS_WORK_LAST_AT_TARGET, -1)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 3, PARTS_WORK_LAST_AT_TARGET, -1)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 4, PARTS_WORK_LAST_AT_TARGET, -1)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 5, PARTS_WORK_LAST_AT_TARGET, -1)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 6, PARTS_WORK_LAST_AT_TARGET, -1)
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, 7, PARTS_WORK_LAST_AT_TARGET, -1)
    SET_VALUE(LW(12), LW(13))
    LABEL(26)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_LAST_AT_TARGET, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_LAST_AT_TARGET, 1)
        SET_VALUE(LW(14), LW(12))
        LABEL(27)
        SUBTRACT(LW(14), 1)
        IF_INT_GREATER_OR_EQUAL(LW(14), 2)
            CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET, LW(0))
            CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(14), PARTS_WORK_TARGET, LW(1))
            IF_INT_EQUAL(LW(0), LW(1))
                CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(14), PARTS_WORK_LAST_AT_TARGET, 0)
            END_IF
            GOTO(27)
        END_IF
    END_IF
    SUBTRACT(LW(12), 1)
    IF_INT_GREATER_OR_EQUAL(LW(12), 2)
        GOTO(26)
    END_IF
    SET_VALUE(LW(12), 2)
    LABEL(28)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_LAST_AT_TARGET, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET, LW(0))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET_PART, LW(1))
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(0), LW(1), 256, LW(6))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_PRECHECK, LW(6))
        IF_INT_EQUAL(LW(6), 1)
            SET_VALUE(LW(14), LW(12))
            LABEL(29)
            ADD(LW(14), 1)
            IF_INT_LESS_OR_EQUAL(LW(14), LW(13))
                CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET, LW(0))
                CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(14), PARTS_WORK_TARGET, LW(1))
                IF_INT_EQUAL(LW(0), LW(1))
                    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(14), PARTS_WORK_LAST_AT_TARGET, LW(0))
                    IF_INT_NOT_EQUAL(LW(0), 0)
                        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(14), PARTS_WORK_LAST_AT_TARGET, 2)
                    END_IF
                END_IF
                GOTO(29)
            END_IF
        END_IF
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET, LW(0))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET_PART, LW(1))
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(0), LW(1), 256, LW(6))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_PRECHECK, LW(6))
        CASE_DEFAULT
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET, LW(0))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET_PART, LW(1))
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(0), LW(1), 1048832, LW(6))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_PRECHECK, LW(6))
    END_SWITCH
    IF_INT_LESS(LW(12), LW(13))
        ADD(LW(12), 1)
        GOTO(28)
    END_IF

    // The camera resets and the eggs are thrown one after the other.
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    SET_VALUE(LW(12), 2)

    // Yoshi turns to the next egg's target and throws it.
    LABEL(30)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(12), kHidden | kNoShadow)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_EGG_KIND, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        SET_VALUE(LW(6), STRING("PYS_A3_1"))
        SET_VALUE(LW(7), STRING("PYS_A3_2"))
        SET_VALUE(LW(8), STRING("PYS_A3_3"))
        CASE_EQUAL(2)
        SET_VALUE(LW(6), STRING("PYS_A4_1"))
        SET_VALUE(LW(7), STRING("PYS_A4_2"))
        SET_VALUE(LW(8), STRING("PYS_A4_3"))
        CASE_DEFAULT
        SET_VALUE(LW(6), STRING("PYS_A2_1"))
        SET_VALUE(LW(7), STRING("PYS_A2_2"))
        SET_VALUE(LW(8), STRING("PYS_A2_3"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(6))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(8))
    WAIT_FRAMES(7)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL_CHILD(_egg_attack_event)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_EVENT, LW(15))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    WAIT_FRAMES(10)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    LABEL(90)
    IF_INT_NOT_EQUAL(LW(12), LW(13))
        ADD(LW(12), 1)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET, LW(3))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET_PART, LW(4))
        GOTO(30)
    END_IF

    // After the last egg he stops the action command and checks the Stylish timing window.
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 15, 15, 20)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(0), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)

    // Every egg is allowed to finish its flight.
    LABEL(95)
    SET_VALUE(LW(12), 2)
    LABEL(96)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_EVENT, LW(15))
    IF_INT_NOT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_EVENT, 0)
    END_IF
    ADD(LW(12), 1)
    IF_INT_LESS_OR_EQUAL(LW(12), LW(13))
        GOTO(96)
    END_IF

    // The camera resets and Yoshi runs back to his home spot.
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -1, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    WAIT_MS(2000)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Flight of one of Yoshi's eggs (part LW(12)): it leaves Yoshi,
 * spins and dives at speed 5 to the hit position of the target LW(3), LW(4),
 * where it bursts with the egg sounds and a small bomb effect.
 * The pre-check result kept in PARTS_WORK_PRECHECK decides between the target's avoid and the hit,
 * which uses the weapon in PARTS_WORK_WEAPON with statuses applied on any hit (0x40000000) and the last-hit flag
 * when PARTS_WORK_LAST_AT_TARGET is set.
 */
EVT_BEGIN(_egg_attack_event)
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
    MULTIPLY_FLOAT(LW(6), 10)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), LW(6))
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(12), kHidden | kNoShadow)
    CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, LW(12), FLOAT(5.0f))
    CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, LW(12), FLOAT(0.099609375f))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(5.0f), LW(5))
        CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
        MULTIPLY_FLOAT(LW(6), 6)
        CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(12), 0, LW(6), 0)
        LOOP(LW(5))
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(12), 0, 0, 48)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_EGG3"), 0)
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2), 0, 25, 0, 0, -1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(12), kHidden | kNoShadow)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_EGG2"), 0)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(0.5996094f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_PRECHECK, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        GOTO(89)
    END_IF
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_WEAPON, LW(0))
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_LAST_AT_TARGET, LW(1))
    IF_INT_EQUAL(LW(1), 0)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(0), DAMAGE_RESULT_AC_SUCCESS | DAMAGE_RESULT_STATUS_EVERY_HIT, LW(5))
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(0), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS | DAMAGE_RESULT_STATUS_EVERY_HIT, LW(5))
    END_IF
    LABEL(89)
    RETURN
EVT_END()

/**
 * Yoshi's Stampede (partyWeapon_YoshiCallGuard,
 * named btl_wn_pys_lv3: a herd of Yoshis attacks every enemy):
 * he pays the cost, runs the action command (PYS_A_10, A_11) and calls the herd with a call sound;
 * _gundan_yoshi_run_effect runs the herd across the stage,
 * and each sampled enemy takes as many hits as the command's output gives, the last with the last-hit flag.
 */
EVT_BEGIN(partyYoshiAttack_CallGuard)
    // Stampede is declared and its FP cost paid; every enemy is targeted.
    SET_VALUE(LW(12), PTR(&partyWeapon_YoshiCallGuard))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, 7)
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)

    // The action command (alternate L and R) is set up; its timing depends on the move level.
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 280)
        SET_VALUE(LW(1), 18)
        SET_VALUE(LW(2), 25)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 250)
        SET_VALUE(LW(1), 17)
        SET_VALUE(LW(2), 27)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 240)
        SET_VALUE(LW(1), 16)
        SET_VALUE(LW(2), 29)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 240)
        SET_VALUE(LW(1), 15)
        SET_VALUE(LW(2), 30)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 200)
        SET_VALUE(LW(1), 15)
        SET_VALUE(LW(2), 30)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 180)
        SET_VALUE(LW(1), 15)
        SET_VALUE(LW(2), 30)
        CASE_DEFAULT
        SET_VALUE(LW(0), 150)
        SET_VALUE(LW(1), 14)
        SET_VALUE(LW(2), 30)
    END_SWITCH
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_AcSetParamAll, 15, LW(0), 178, LW(1), LW(2), 17, 18, EVT_NULLPTR)
#else
    CALL(btlevtcmd_AcSetParamAll, 15, LW(0), 178, LW(1), LW(2), 20, 61, EVT_NULLPTR)
#endif
    CALL(btlevtcmd_AcSetGaugeParam, 60, 80, 100, 100)
    CALL(btlevtcmd_AcSetFlag, 64)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 6, 1, 0)

    // The camera frames Yoshi as he starts to shout; the command begins.
    CALL(evt_btl_camera_set_mode, 0, 7)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_10"))
    CALL(btlevtcmd_ftof, 24, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)

    // He calls the herd, with a call sound and a puff of smoke.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_11"))
#if VERSION <= VERSION_DEMO_JP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    MULTIPLY(LW(5), 30)
    CALL(evt_eff, STRING(""), STRING("kiss"), 3, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
#else
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(6))
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(0))
    MULTIPLY_FLOAT(LW(6), LW(0))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), LW(6))
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    MULTIPLY(LW(5), 30)
    CALL(evt_eff, 0, STRING("kiss"), 3, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ENM_TOGENOKO_CALL1"), LW(0), LW(1), LW(2), 0)
#endif
    CALL(btlevtcmd_ftof, 33, LW(0))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_FRAMES(LW(0))
    END_SIBLING_EVENT
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, LW(0), LW(0), 20)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        BEGIN_SIBLING_EVENT
            WAIT_MS(166)
            CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        END_SIBLING_EVENT
    END_SWITCH

    // The herd begins to run across the stage while the camera rumbles.
    BEGIN_INLINE_EVENT_GET_ID(LW(9))
        WAIT_MS(1000)
        CALL(_gundan_yoshi_run_effect, UNIT_ID_SELF)
    END_INLINE_EVENT
    WAIT_MS(166)
    CALL(evt_btl_camera_set_mode, 0, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_shake_h, 0, 1, 0, 6000, 0)
    WAIT_MS(1000)
    CALL(_wait_yoshig_run, UNIT_ID_SELF)
    SET_VALUE(LW(15), 0)

    // For each enemy in turn: the game checks whether the stampede reaches it or it dodges.
    LABEL(70)
    IF_INT_EQUAL(LW(3), -1)
        IF_INT_NOT_EQUAL(LW(15), 0)
            CALL(btlevtcmd_WaitEventEnd, LW(15))
        END_IF
        CALL(btlevtcmd_WaitEventEnd, LW(9))
        GOTO(90)
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(6))
    IF_INT_NOT_EQUAL(LW(6), PRECHECK_HIT)
        IF_INT_EQUAL(LW(6), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), 38)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), 39)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), 40)
        END_IF
        GOTO(75)
    END_IF
    CALL(btlevtcmd_AcGetOutputParam, 2, LW(14))

    // The enemy takes one hit for each repetition the action command earned,
    // the last one counting as the final hit.
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        LOOP(LW(14))
            IF_INT_GREATER_OR_EQUAL(LW(14), 2)
                CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
                IF_INT_GREATER_OR_EQUAL(LW(0), 2)
                    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_AC_SUCCESS, LW(5))
                ELSE
                    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), 0, LW(5))
                END_IF
            ELSE
                CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
                SWITCH(LW(0))
                    CASE_GREATER_OR_EQUAL(6)
                    SET_VALUE(LW(0), 2)
                    CASE_GREATER_OR_EQUAL(4)
                    SET_VALUE(LW(0), 1)
                    CASE_GREATER_OR_EQUAL(2)
                    SET_VALUE(LW(0), 0)
                    CASE_DEFAULT
                    SET_VALUE(LW(0), -1)
                END_SWITCH
                IF_INT_GREATER_OR_EQUAL(LW(0), 0)
                    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
                    CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(6))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
                ELSE
                    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
                    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
                END_IF
            END_IF
            WAIT_FRAMES(10)
        END_LOOP
    END_SIBLING_EVENT
    LABEL(75)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(70)

    // The herd finishes crossing, the camera stops shaking and the audience reaction is reported.
    LABEL(90)
    CALL(_wait_yoshig_complete, UNIT_ID_SELF)
    CALL(evt_btl_camera_noshake, 1)
    WAIT_MS(500)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

u8 _yoshig_tpl_no_tbl[8] __attribute__((aligned(4))) = { // battle tex id per sweat frame (tst ids: JP +7, JP demo: JP)
#if VERSION <= VERSION_DEMO_JP
    0x46, 0x49, 0x47, 0x46, 0x4A, 0x48, 0, 0,
#else
    0x4D, 0x50, 0x4E, 0x4D, 0x51, 0x4F, 0, 0,
#endif
};

/**
 * Script helper: args[0] is Yoshi.
 * When the player picked a non-default Yoshi colour (pouchGetPartyColor(PARTY_YOSHI) is 1-6),
 * swaps his body to the matching c_babyyoshiN model, loading it asynchronously and blocking until it is ready;
 * nothing happens for the default colour.
 */
USER_FUNC(yoshi_original_color_anim_set) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnitPart* part;
    const char* name;
    s32 id;

    id = evtGetValue(event, *args++);
    id = BattleTransID(event, id);
    BattleGetUnitPtr(wp, id);
    switch (pouchGetPartyColor(PARTY_YOSHI)) {
        case 1:
            name = "c_babyyoshi2";
            break;
        case 2:
            name = "c_babyyoshi3";
            break;
        case 3:
            name = "c_babyyoshi4";
            break;
        case 4:
            name = "c_babyyoshi5";
            break;
        case 5:
            name = "c_babyyoshi6";
            break;
        case 6:
            name = "c_babyyoshi7";
            break;
        case 0:
        default:
            name = NULL;
            break;
    }
    if (isFirstCall) {
        if (name == NULL) {
            return EVT_RETURN_DONE;
        }
        part = BattleGetUnitPartsPtr(id, 1);
        animPoseGetAnimPosePtr(part->poseId);
        if (part->poseId != -1) {
            animPoseRelease(part->poseId);
            part->poseId = -1;
        }
    }
    if (animEffectAsync(name, 1)) {
        part = BattleGetUnitPartsPtr(id, 1);
        part->poseId = animPoseEntry(name, 1);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

#define ADD_WEAPON(w)                                                               \
    command->weapon[*count].index = -1;                                             \
    command->weapon[*count].itemId = 0;                                             \
    command->weapon[*count].weapon = (BattleWeapon*)(w);                            \
    command->weapon[*count].enabled = 0;                                            \
    command->weapon[*count].icon = command->weapon[*count].weapon->icon;            \
    command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name); \
    (*count)++;

/**
 * Builds Yoshi's move list for the battle menu:
 * his basic attack always, Gulp from tech level 0, Mini-Egg from 1 and Stampede from 2 (partyGetTechLv),
 * with each weapon's icon and localized name.
 */
static void __makeTechMenuFunc(BattleCommand* command, s32* count) {
    s32 techLv;

    techLv = partyGetTechLv(BattleTransPartyId(BattleGetPartyPtr(_battleWorkPointer)->currentKind));
    ADD_WEAPON(&partyWeapon_YoshiNormalAttack)
    if (techLv >= 0) {
        ADD_WEAPON(&partyWeapon_YoshiNomikomi_Shot)
    }
    if (techLv >= 1) {
        ADD_WEAPON(&partyWeapon_YoshiEggAttack_Minimini)
    }
    if (techLv >= 2) {
        ADD_WEAPON(&partyWeapon_YoshiCallGuard)
    }
}

/** Distance of the brake slide after n frames: 16 per frame, 14/15 less each frame. */
f32 _yoshi_slide_distance(s32 n) {
    f32 dist;
    s32 i;

    if (n < 0) {
        return 16.0f * n;
    }
    dist = 0.0f;
    for (i = 0; i < n; i++) {
        dist += 16.0f - 0.93333334f * (i + 1);
    }
    return dist;
}

/** arg 0 (out): the full brake slide distance (15 frames) */
USER_FUNC(_get_yoshi_slide_distance) {
    EvtWord* args = event->args;
    EvtWord out = args[0];

    evtSetFloat(event, out, _yoshi_slide_distance(15));
    return EVT_RETURN_DONE;
}

/**
 * args: unit id, target x, target y (unused), target z.
 * Slides the unit 15 frames towards the target x and moves z to the target;
 * UNIT_WORK_STEP_PITCH_SCALE falls from 100 to 50 with the distance covered.
 */
USER_FUNC(_yoshi_slide_move) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 tx = evtGetValue(event, args[1]);
    s32 ty = evtGetValue(event, args[2]);
    s32 tz = evtGetValue(event, args[3]);
    BattleUnit* unit;
    Vec pos;
    f32 dist;

    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    if (isFirstCall) {
        event->userWork[0] = 0;
        event->userWork[1] = pos.x;
        event->userWork[2] = pos.z;
        if (tx - pos.x >= 0.0f) {
            event->userWork[3] = 1;
        } else {
            event->userWork[3] = -1;
        }
    }
    event->userWork[0]++;
    dist = _yoshi_slide_distance(event->userWork[0]);
    pos.x = dist * event->userWork[3] + event->userWork[1];
    pos.z = event->userWork[2] + event->userWork[0] / 15 * (tz - event->userWork[2]);
    BtlUnit_SetPos(unit, pos.x, pos.y, pos.z);
    unit->work[UNIT_WORK_STEP_PITCH_SCALE] = 100.0f - 50.0f * dist / _yoshi_slide_distance(15);
    return event->userWork[0] < 15 ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

/** args: mode, start, end, steps; arg 4 (out): end minus the interpolated value one step before the end */
USER_FUNC(_intpl_last_step) {
    EvtWord* args = event->args;
    s32 mode = evtGetValue(event, args[0]);
    s32 start = evtGetValue(event, args[1]);
    s32 end = evtGetValue(event, args[2]);
    s32 steps = evtGetValue(event, args[3]);
    EvtWord out = args[4];

    evtSetFloat(event, out, end - intplGetValue(mode, start, end, steps - 1, steps));
    return EVT_RETURN_DONE;
}

// BattleActions.work as the normal attack's action command leaves it
typedef struct YoshiSlideAcWork {
    u8 pad_0[0xC];
    u32 flags; // 0xC, bit 0 -> arg 1
    u8 pad_10[0x40 - 0x10];
    s32 power; // 0x40, AC power in percent (sets the step pitch)
    s32 stage; // 0x44, 1-3: run pose
} YoshiSlideAcWork;

/**
 * arg 0 (in/out): last stage seen; arg 1 (out): AC flag bit 0.
 * Follows the action command: a new stage changes the run pose,
 * and UNIT_WORK_STEP_PITCH follows the AC power.
 */
USER_FUNC(_yoshi_slide_ac_check) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    EvtWord stageArg = args[0];
    s32 last = evtGetValue(event, stageArg);
    EvtWord flagArg = args[1];
#if VERSION <= VERSION_DEMO_JP
    YoshiSlideAcWork* ac = (YoshiSlideAcWork*)TST_DEMO_BATTLE_WORK(wp)->actionCommands.work;
#else
    YoshiSlideAcWork* ac = (YoshiSlideAcWork*)wp->actionCommands.work;
#endif
    s32 stage = ac->stage;
    BattleUnit* unit = BattleGetUnitPtr(wp, BattleTransID(event, -2));
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));

    if (stage != last) {
        evtSetValue(event, stageArg, stage);
        switch (stage) {
            case 1:
                BtlUnit_SetAnim(part, "PYS_R_1");
                break;
            case 2:
                BtlUnit_SetAnim(part, "PYS_R_4");
                break;
            case 3:
                BtlUnit_SetAnim(part, "PYS_R_5");
                break;
        }
    }
    unit->work[UNIT_WORK_STEP_PITCH] = ac->power * 2387 / 100;
    if (ac->flags & 1) {
        evtSetValue(event, flagArg, 1);
    } else {
        evtSetValue(event, flagArg, 0);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper for Yoshi's Ground Pound flutter ('yoroyoro' jump):
 * args[0] is the unit, args[1]-args[3] the target x, y, z;
 * sets up a 180-frame move at a constant speed and writes the speed to args[4] and the frame count to args[5].
 */
USER_FUNC(btl_yoshi_yoroyoro_jump_calc_param) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, outSpeed, outFrames;
    f32 tx, ty, tz, speed;
    f32 x, y, z;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    tx = (f32)evtGetValue(event, args[1]);
    ty = (f32)evtGetValue(event, args[2]);
    tz = (f32)evtGetValue(event, args[3]);
    outSpeed = args[4];
    speed = evtGetFloat(event, outSpeed);
    unit = BattleGetUnitPtr(wp, id);
    outFrames = args[5];
    if (speed > 0.0f) {
        unit->moveSpeedXZ = speed;
    }
    if (unit->moveSpeedXZ <= 0.0f) {
        unit->moveSpeedXZ = 1.0f;
    }
    BtlUnit_SetMoveTargetPos(unit, tx, ty, tz);
    BtlUnit_GetPos(unit, &x, &y, &z);
    BtlUnit_SetMoveStartPos(unit, x, y, z);
    BtlUnit_SetMoveCurrentPos(unit, x, y, z);
    unit->mMoveDirection = angleABf(x, z, tx, tz);
    unit->moveDistance = distABf(x, z, tx, tz);
    if (tx - x > 0.0f) {
        unit->moveDirection = 1;
    } else if (tx - x < 0.0f) {
        unit->moveDirection = -1;
    }
    unit->moveDistance = __fabs(tx - x);
    unit->moveFrames = 180;
    unit->moveTotalFrames = (f32)unit->moveFrames;
    unit->moveSpeedY = 0.0f;
    unit->moveSpeedXZ = unit->moveDistance / (f32)unit->moveFrames;
    evtSetFloat(event, outSpeed, unit->moveSpeedXZ);
    evtSetValue(event, outFrames, unit->moveFrames);
    return EVT_RETURN_DONE;
}

/**
 * Script helper called every frame: advances Yoshi's flutter toward the target set by
 * btl_yoshi_yoroyoro_jump_calc_param, bobbing him up and down and spawning a sweat drop every 21 frames;
 * returns DONE when the 180 frames are used up.
 */
USER_FUNC(btl_yoshi_yoroyoro_jump_move) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;
    f32 sweatAngle;
    EffectEntry* eff;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    evtGetValue(event, args[1]);
    evtGetValue(event, args[2]);
    evtGetValue(event, args[3]);
    evtGetFloat(event, args[4]);
    unit = BattleGetUnitPtr(wp, id);
    evtGetValue(event, args[5]);
    if (isFirstCall) {
        event->blocked = 1;
        event->userdata[1] = 20;
        event->userdata[2] = 0;
    }
    if (unit->moveCurrentPos.x != unit->moveTargetPos.x) {
        if (__fabs(unit->moveTargetPos.x - unit->moveCurrentPos.x) < unit->moveSpeedXZ) {
            unit->moveCurrentPos.x = unit->moveTargetPos.x;
            if (!unit->moveCurrentPos.z) {
            }
        } else {
            unit->moveCurrentPos.x += unit->moveSpeedXZ * unit->moveDirection;
        }
    }
    if (unit->moveCurrentPos.z != unit->moveTargetPos.z) {
        if (__fabs(unit->moveTargetPos.z - unit->moveCurrentPos.z) < unit->moveSpeedXZ) {
            unit->moveCurrentPos.z = unit->moveTargetPos.z;
        } else if (unit->moveTargetPos.z - unit->moveStartPos.z > 0.0f) {
            unit->moveCurrentPos.z += unit->moveSpeedXZ;
        } else {
            unit->moveCurrentPos.z -= unit->moveSpeedXZ;
        }
    }
    sweatAngle = 10.0f * sinfd(180.0f + 360.0f * (unit->moveTotalFrames - (f32)(unit->moveFrames % 90)) / 90.0f);
    BtlUnit_GetPartsPtr(unit, 1);
    unit->moveCurrentPos.y = unit->moveSpeedY + (unit->moveStartPos.y + (unit->moveTargetPos.y - unit->moveStartPos.y) * (unit->moveTotalFrames - (f32)unit->moveFrames) / unit->moveTotalFrames);
    unit->moveSpeedY = sweatAngle;
    unit->work[UNIT_WORK_STEP_PITCH_SCALE] = (s32)(75.0f - 25.0f * sinfd(360.0f * (unit->moveTotalFrames - (f32)(unit->moveFrames % 90)) / 90.0f));
    BtlUnit_SetPos(unit, unit->moveCurrentPos.x, unit->moveCurrentPos.y, unit->moveCurrentPos.z);
    event->userdata[1]++;
    if (event->userdata[1] > 20) {
        event->userdata[1] = 0;
        if (unit->moveDirection >= 0) {
            sweatAngle = -50.0f;
        } else {
            sweatAngle = 50.0f;
        }
        event->userdata[2] = (EvtWord)effSweatN64Entry(1, unit->moveCurrentPos.x, unit->moveCurrentPos.y + unit->sizeMultiplier * (0.5f * BtlUnit_GetHeight(unit)), unit->moveCurrentPos.z, 5.0f * unit->sizeMultiplier, sweatAngle, 16);
    } else {
        eff = (EffectEntry*)event->userdata[2];
        if (event->userdata[1] < 16) {
            ((YoshiSweatWork*)eff->userdata)->x = unit->moveCurrentPos.x;
            ((YoshiSweatWork*)eff->userdata)->y = unit->moveCurrentPos.y + unit->sizeMultiplier * (0.5f * BtlUnit_GetHeight(unit));
            ((YoshiSweatWork*)eff->userdata)->z = unit->moveCurrentPos.z;
        }
    }
    unit->moveFrames--;
    if (unit->moveFrames <= 0) {
        BtlUnit_SetPos(unit, unit->moveTargetPos.x, unit->moveTargetPos.y, unit->moveTargetPos.z);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Pose sound callback: plays the footstep sound at pitch UNIT_WORK_STEP_PITCH * UNIT_WORK_STEP_PITCH_SCALE / 100. */
static void _yoshi_slide_move_sound(BattleUnit* unit, PoseSoundTiming* s) {
    BtlUnit_snd_se(unit, s->sfxName, EVT_NULLPTR, (s16)(unit->work[UNIT_WORK_STEP_PITCH] * unit->work[UNIT_WORK_STEP_PITCH_SCALE] / 100)); // the step sound's pitch
}

#if VERSION >= VERSION_JP // the JP demo has no _get_nomikomi_hit_position

/**
 * Script helper: args[0] is Yoshi, args[1] the swallowed target;
 * args[3]-args[5] receive the world position of the target's hit offset (bintaHitOffset scaled by its size,
 * x mirrored by facing direction).
 */
USER_FUNC(_get_nomikomi_hit_position) {
    BattleWork* wp;
    EvtWord* args;
    BattleUnit* unit;
    Vec pos;
    s32 id, targetId, dir;
    s32 outZ, outX, outY;

    wp = _battleWorkPointer;
    args = event->args;
    id = evtGetValue(event, args[0]);
    targetId = evtGetValue(event, args[1]);
    evtGetValue(event, args[2]);
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    BattleGetUnitPtr(wp, BattleTransID(event, id));
    unit = BattleGetUnitPtr(wp, BattleTransID(event, targetId));
    dir = unit->faceDirection;
    BtlUnit_GetPos(unit, &pos.x, &pos.y, &pos.z);
    pos.x += dir * (unit->bintaHitOffset.x * unit->sizeMultiplier);
    pos.y += unit->bintaHitOffset.y * unit->sizeMultiplier;
    pos.z += unit->bintaHitOffset.z * unit->sizeMultiplier;
    evtSetFloat(event, outX, pos.x);
    evtSetFloat(event, outY, pos.y);
    evtSetFloat(event, outZ, pos.z);
    return EVT_RETURN_DONE;
}
#endif

/** arg 0: animation speed of Yoshi's body part */
USER_FUNC(_set_yoshi_anim_speed) {
    f32 speed = evtGetFloat(event, event->args[0]);

    BattleGetUnitPartsPtr(BattleTransID(event, -2), 1)->animSpeed2 = speed;
    return EVT_RETURN_DONE;
}

/** Script helper: args[0] is a unit; args[1] receives its swallowRate, how easily Yoshi's Gulp swallows it. */
USER_FUNC(_get_swallow_param) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;
    s32 out;
    EvtWord* args = event->args;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    out = args[1];
    unit = BattleGetUnitPtr(wp, id);
    evtSetValue(event, out, unit->swallowRate);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] is a unit, args[1] a mask;
 * args[2] receives 1 when the unit's swallowAttribute has any of the mask bits set, else 0.
 */
USER_FUNC(_check_swallow_attribute) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, out;
    u16 mask;
    EvtWord* args = event->args;

    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    mask = args[1];
    out = args[2];
    unit = BattleGetUnitPtr(wp, id);
    if ((u8)unit->swallowAttribute & mask) {
        evtSetValue(event, out, 1);
    } else {
        evtSetValue(event, out, 0);
    }
    return EVT_RETURN_DONE;
}

/** Puts Yoshi's body animation at frame 13. */
USER_FUNC(_set_yoshi_anim_time) {
    animPoseSetLocalTime(BattleGetUnitPartsPtr(BattleTransID(event, -2), 1)->poseId, 13.0f);
    return EVT_RETURN_DONE;
}

/** Script helper: args[0] is Yoshi; blocks until the Stampede herd exists and its average x is positive. */
USER_FUNC(_wait_yoshig_run) {
    BattleUnit* unit;
    YoshiGundanWork* w;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    w = (YoshiGundanWork*)TST_BATTLE_UNIT(unit)->countersetWeapon;
    if (w == NULL) {
        return EVT_RETURN_BLOCK;
    }
    if (w->meanX <= 0.0f) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: args[0] is Yoshi; blocks until the Stampede herd has been freed. */
USER_FUNC(_wait_yoshig_complete) {
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (TST_BATTLE_UNIT(unit)->countersetWeapon) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper for the Stampede: runs the herd of 256 Yoshi sprites across the stage,
 * animating their run cycles, kicking up smoke and panning a looped sound with the herd's average x.
 * Allocates the herd on its first call and frees it when every Yoshi has arrived; blocks while it runs.
 */
USER_FUNC(_gundan_yoshi_run_effect) {
    BattleUnit* unit;
    YoshiGundanWork* w;
    YoshiGundanPart* p;
    s32 i;
    s32 allDone;
    Vec pos;
    s32 id;
    Vec tmp;

    do {
        id = evtGetValue(event, event->args[0]);
        id = BattleTransID(event, id);
        unit = BattleGetUnitPtr(_battleWorkPointer, id);
        if (isFirstCall) {
            w = BattleAlloc(sizeof(YoshiGundanWork));
            TST_BATTLE_UNIT(unit)->countersetWeapon = (BattleWeapon*)w;
            memset(w, 0, sizeof(YoshiGundanWork));
            w->state = 0;
            w->meanX = 0.0f;
            event->userdata[0] = 0;
            tmp.x = (f32)id;
            event->userdata[1] = psndSFXOn("SFX_BTL_YOSHI_TAIGUN1");
        } else {
            w = (YoshiGundanWork*)TST_BATTLE_UNIT(unit)->countersetWeapon;
        }
        switch (w->state) {
            case 0:
                w->state++;
            case 1:
                w->meanX = 0.0f;
                allDone = TRUE;
                for (i = 0; i < 256; i++) {
                    p = &w->parts[i];
                    p->frameCount++;
                    if (p->frameCount > 2) {
                        p->frameCount = 0;
                        p->frame++;
                        if (p->frame >= 6) {
                            p->frame = 0;
                        }
                    }
                    if (_gundan_part_update(p, i)) {
                        allDone = FALSE;
                    }
                    w->meanX += p->x;
                }
                if (event->userdata[0] > 0) {
                    event->userdata[0]--;
                } else {
                    f32 sc;
                    p = &w->parts[irand(256)];
                    sc = 1.0f + 0.05f * (f32)irand(20);
#if VERSION <= VERSION_DEMO_JP
                    effKemuTestEntry(0, p->x, 5.0f + p->y + (f32)irand(10), p->z, sc);
#else
                    effKemuTestEntry(0, p->x, 5.0f + p->y + (f32)irand(15), 65.0f, sc);
#endif
                }
                w->meanX *= 0.00390625f;
                pos.x = w->meanX;
                pos.y = 0.0f;
                pos.z = 0.0f;
                psndSFX_pos(event->userdata[1], &pos);
                if (allDone) {
                    w->state = 999;
                }
                break;
            case 999:
                psndSFXOff(event->userdata[1]);
                BattleFree(TST_BATTLE_UNIT(unit)->countersetWeapon);
                TST_BATTLE_UNIT(unit)->countersetWeapon = NULL;
                return EVT_RETURN_DONE;
        }
#if VERSION <= VERSION_DEMO_JP
        dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, (DispCallback)_btlYoshiDisp, w);
#else
        for (i = 0; i < 256; i++) {
            dispEntry(CAMERA_3D, dispCalcZ((Vec*)p), VERSION_PICK(DISP_MODE_XLU, DISP_MODE_ALPHA_TEST), (DispCallback)_btlYoshiDisp, &w->parts[i]);
        }
#endif
    } while (0);
    return EVT_RETURN_BLOCK;
}

/**
 * Updates herd member i of the Stampede for one frame.
 * State 0 places it in a row of 16 behind the left edge (x = -300 minus 30 per row, plus 0 to 9 at random;
 * z lane -80 + 10 * (i % 16)), with a random start delay of 0 to 2 frames, animation frame,
 * speed 3 to 4 and z drift.
 * It then runs right, bouncing its z between -80 and 80 (65 from US on), until it reaches x = 300,
 * where it stops (state 999). Returns TRUE while it is still running, FALSE once stopped.
 */
s32 _gundan_part_update(YoshiGundanPart* p, s32 i) {
    switch (p->state) {
        case 0:
            p->state++;
            p->delay = irand(3);
            p->x = (f32)(-(i / 16 * 30 + 300) + irand(10));
            p->y = 0.0f;
            p->z = (f32)(i % 16 * 10 - 80);
            p->frame = irand(6);
            p->frameCount = irand(5);
            p->vx = 0.1f * (f32)irand(10) + 3.0f;
            p->vy = 0.0f;
            p->vz = 0.001f * (f32)(irand(2000) - 1000);
        case 1:
            if (p->delay > 0) {
                p->delay--;
                break;
            }
        case 2:
            p->z += p->vz;
            if (p->z >= VERSION_PICK(80.0f, 65.0f)) {
                p->z = VERSION_PICK(80.0f, 65.0f);
                p->vz *= -1.0f;
            }
            if (p->z <= -80.0f) {
                p->z = -80.0f;
                p->vz *= -1.0f;
            }
            p->x += p->vx;
            if (p->x >= 300.0f) {
                p->x = 300.0f;
                p->vx = 0.0f;
                p->state = 999;
            }
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

/**
 * Draw callback for the Stampede herd: draws the Yoshi run-cycle sprites (texture plane at the Yoshi's position
 * and frame, flipped by direction).
 */
#if VERSION <= VERSION_DEMO_JP
static void _btlYoshiDisp(CameraId camera, YoshiGundanWork* w) {
    YoshiGundanPart* p;
    s32 i;
    f32 s;
    for (i = 0; i < 256; i++) {
        p = &w->parts[i];
        s = (p->vx < 0.0f) ? 0.4f : -0.4f;
        ((BtlDispTexPlaneArgOrderFn)btlDispTexPlane)(p->x, p->y + 20.0f, p->z, s, 0.4f, _yoshig_tpl_no_tbl[p->frame], (GXColor){255, 255, 255, 255}, 0);
    }
}
#else
static void _btlYoshiDisp(CameraId camera, YoshiGundanPart* p) {
    f32 s;
    s = (p->vx < 0.0f) ? 0.4f : -0.4f;
    ((BtlDispTexPlaneArgOrderFn)btlDispTexPlane)(p->x, p->y + 20.0f, p->z, s, 0.4f, _yoshig_tpl_no_tbl[p->frame], (GXColor){255, 255, 255, 255}, 0);
}
#endif

/**
 * Yoshi's Appeal command: runs his data event 7, plays the appeal pose and sound,
 * calls the audience appeal after 40 frames and then returns to waiting.
 */
EVT_BEGIN(yoshi_appeal_event)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, 7)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_APPEAL)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_APPEAL1"), LW(0), LW(1), LW(2), 0)
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AudienceAppeal, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Victory reaction (data tag 64): plays Yoshi's PYS_Y_1 pose through twice, then his stay pose. */
static EVT_BEGIN(party_win_reaction)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_Y_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    RETURN
EVT_END()

/**
 * Audience-attack reaction (data tag 6): from his spot Yoshi turns to the chosen audience member
 * and throws a randomly green, yellow or pink egg (part 2) at it, which bursts on impact; no running offstage.
 */
static EVT_BEGIN(attack_audience_event)
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), 1, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(12), 2)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(evt_btl_camera_set_mode, 0, 13)
        CALL(evt_btl_camera_set_homing_unit_audience, 0, UNIT_ID_SELF, LW(12), LW(3))
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
        CALL(evt_btl_camera_set_zoom, 0, 200)
    END_SIBLING_EVENT
    CALL(evt_sub_random, 2, LW(0))
    CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_EGG_KIND, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(12), PTR(pose_table_egg_y))
        CASE_EQUAL(2)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(12), PTR(pose_table_egg_p))
        CASE_DEFAULT
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, LW(12), PTR(pose_table_egg_g))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(12), POSE_TYPE_ACTION)
    CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, LW(12), FLOAT(7.5f))
    CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, LW(12), FLOAT(0.099609375f))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(12), kHidden | kNoShadow)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_EGG_KIND, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        SET_VALUE(LW(6), STRING("PYS_A3_1"))
        SET_VALUE(LW(7), STRING("PYS_A3_2"))
        SET_VALUE(LW(8), STRING("PYS_A3_3"))
        CASE_EQUAL(2)
        SET_VALUE(LW(6), STRING("PYS_A4_1"))
        SET_VALUE(LW(7), STRING("PYS_A4_2"))
        SET_VALUE(LW(8), STRING("PYS_A4_3"))
        CASE_DEFAULT
        SET_VALUE(LW(6), STRING("PYS_A2_1"))
        SET_VALUE(LW(7), STRING("PYS_A2_2"))
        SET_VALUE(LW(8), STRING("PYS_A2_3"))
    END_SWITCH
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(6))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, LW(8))
    BEGIN_SIBLING_EVENT
        WAIT_MS(1000)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    WAIT_FRAMES(7)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(12), kHidden | kNoShadow)
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    ADD(LW(1), 28)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(5.0f), LW(5))
        CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, LW(12), 0, 6, 0)
        LOOP(LW(5))
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(12), 0, 0, 48)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_EGG3"), 0)
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2), 0, 25, 0, 0, -1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(12), kHidden | kNoShadow)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_EGG2"), 0)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(0.5996094f), 0, 0, 0, 0, 0, 0, 0)
    CALL(_attack_aud, LW(3), 0)
    CALL(evt_btl_camera_set_mode, 0, 3)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, 0, 0)
        WAIT_MS(500)
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()
