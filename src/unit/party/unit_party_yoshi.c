// Battle unit data and scripts for Yoshi as a partner.

#include "version.h"
#include "data_tables.h"
#include "battle/battle_attack_audience.h"
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "unit/party/unit_party_yoshi.h"
#include "battle/unit/unit_party_yoshi.h"
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
#include "mario/mario_pouch.h"

#include "battle/battle_disp.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_env.h"
#include "evt/evt_sub.h"
#include "battle/battle_damage.h"

#if VERSION >= VERSION_JP
void _btlYoshiDisp(CameraId camera, YoshiGundanPart* p);
#endif

void _yoshi_slide_move_sound(BattleUnit* unit, PoseSoundTiming* s);
static void __makeTechMenuFunc(BattleCommand* command, s32* count);

#if VERSION <= VERSION_DEMO_JP
static void _btlYoshiDisp(CameraId camera, YoshiGundanWork* w);
#endif

#include "eff/eff_kemu_test.h"
#include "eff/n64/eff_sweat_n64.h"
#include "icon_ids.h"

USER_FUNC(yoshi_original_color_anim_set);
USER_FUNC(_gundan_yoshi_run_effect);
USER_FUNC(_wait_yoshig_complete);
USER_FUNC(_wait_yoshig_run);
USER_FUNC(_check_swallow_attribute);
USER_FUNC(_get_swallow_param);
USER_FUNC(_get_nomikomi_hit_position);

PoseTableEntry pose_table_yoshi_stay[25];
static BattleDataEntry data_table[8];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
PoseTableEntry pose_table_egg_g[4];
PoseTableEntry pose_table_egg_y[4];
PoseTableEntry pose_table_egg_p[4];
BattleUnitKindPart unitpartsdata_Party_Yoshi[7];
BattleUnitKind unitdata_Party_Yoshi;
static EvtWord battle_entry_event[59];
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
EvtWord partyYoshiAttack_NormalAttack[1025];
EvtWord partyYoshiAttack_Nomikomi[1000];
EvtWord partyYoshiAttack_EggAttack[1266];
EvtWord _egg_attack_event[195];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord partyYoshiAttack_CallGuard[];
#else
EvtWord partyYoshiAttack_CallGuard[553];
#endif

EvtWord yoshi_appeal_event[39];
static EvtWord party_win_reaction[25];
static EvtWord attack_audience_event[290];
static EvtWord attack_event[2];
static EvtWord wait_event[6];
u8 _yoshig_tpl_no_tbl[8] __attribute__((aligned(4)));

/**
 * Yoshi's pose table: animations for each status (stop, dizzy, poison, confusion, sleep, freeze, instant KO),
 * the hurt poses, walking and running, low HP, the failed Run Away tumble, talking, item use,
 * the command menu wait, the Appeal, zero gravity, the Defend stance and plain standing.
 */
#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
#endif

PoseTableEntry pose_table_yoshi_stay[] = {
    {POSE_TYPE_FREEZE, "PYS_D_5"},
    {POSE_TYPE_STOP, "PYS_D_5"},
    {POSE_TYPE_DIZZY, "PYS_X_1"},
    {POSE_TYPE_POISON, "PYS_D_2"},
    {POSE_TYPE_CONFUSE, "PYS_X_1"},
    {POSE_TYPE_SLEEP, "PYS_N_1"},
    {POSE_TYPE_KO, "PYS_D_3"},
    {POSE_TYPE_NORMAL, "PYS_S_1"},
    {POSE_TYPE_DAMAGE_DRAINED, "PYS_G_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "PYS_G_1"},
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
    {POSE_TYPE_AVOID_COUNTER, "PYS_F_1"},
    {POSE_TYPE_DEFEND, "PYS_G_1"},
    {POSE_TYPE_ACTION, "PYS_S_1"},
};

/**
 * Yoshi's data events: Appeal, the audience attack reaction, his move list (tag 1, __makeTechMenuFunc), First
 * Strike (nothing), the victory pose, an empty step run after a move's cost is paid (7) and the spin when he
 * superguards an attack (47).
 */
#if VERSION >= VERSION_JP
static BattleDataEntry data_table[] = {
    {UNIT_DATA_APPEAL_EVENT, yoshi_appeal_event},
    {UNIT_DATA_ATTACK_AUDIENCE, attack_audience_event},
    {UNIT_DATA_TECH_MENU_FUNC, __makeTechMenuFunc},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_WIN_REACTION_EVENT, party_win_reaction},
    {UNIT_DATA_ATTACK_READY_EVENT, subset_attack_ready_jump_event},
    {UNIT_DATA_AVOID_COUNTER_EVENT, btldefaultevt_avoid_counter_event},
    {0, NULL},
};
#endif

/** Yoshi's defence (shared by his egg parts): 0 against every element. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Yoshi's defence attributes: normal (0) against every element, no weakness or immunity. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

/** Yoshi's status chances: 100 for every status (badges lower them in battle). */
#if VERSION >= VERSION_JP
static StatusVulnerability regist = {
    100, // sleep
    100, // stop
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
    100, // fright
    100, // galeForce
    100, // fast
    100, // slow
    100, // dodgy
    100, // invisible
    100, // ohko
};
#endif

/** Pose table for a green Mini-Egg (egg parts 2-7): the CHU_C_1 egg model for every pose. */
PoseTableEntry pose_table_egg_g[] = {
    {POSE_TYPE_NORMAL, "CHU_C_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "CHU_C_1"},
    {POSE_TYPE_DAMAGE, "CHU_C_1"},
    {POSE_TYPE_ACTION, "CHU_C_1"},
};

/** Pose table for a yellow Mini-Egg: the CHU_C_2 egg model for every pose. */
PoseTableEntry pose_table_egg_y[] = {
    {POSE_TYPE_NORMAL, "CHU_C_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "CHU_C_2"},
    {POSE_TYPE_DAMAGE, "CHU_C_2"},
    {POSE_TYPE_ACTION, "CHU_C_2"},
};

/** Pose table for a pink Mini-Egg: the CHU_C_3 egg model for every pose. */
PoseTableEntry pose_table_egg_p[] = {
    {POSE_TYPE_NORMAL, "CHU_C_3"},
    {POSE_TYPE_DAMAGE_GUARDED, "CHU_C_3"},
    {POSE_TYPE_DAMAGE, "CHU_C_3"},
    {POSE_TYPE_ACTION, "CHU_C_3"},
};

/**
 * Yoshi's battle parts: part 1 is his targetable body (model c_babyyoshi);
 * parts 2-7 are hidden, untargetable eggs (c_tamago) that Mini-Egg lays and throws.
 */
#if VERSION <= VERSION_DEMO_JP
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
#endif

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
 * Yoshi's battle unit (UNIT_YOSHI), listed in battle_party.c's partner table:
 * seven parts, the partner hit sounds and no enemy HP gauge.
 */
BattleUnitKind unitdata_Party_Yoshi = {
    UNIT_YOSHI, // unitType
    "btl_un_yoshi", // name
    10, // maxHp
    5, // maxFp
    5, // dangerHP
    1, // perilHP
    1, // level
    0, // bonusExp
    0, // bonusCoin
    0, // bonusCoinRate
    0, // baseCoin
    100, // runRate
    9, // pbSoftCap
    20, // width
    33, // height
    {8, 28}, // hitOffset
    {0.0f, 12.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 40.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 40.0f, 0.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {10.0f, 0.0f, 0.0f}, // bintaHitOffset
    {10.0f, 21.45f, 0.0f}, // kissHitOffset
    {0.0f, 0.0f, 0.0f}, // cutBaseOffset
    0.0f, // cutWidth
    0.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_PARTY_DAMAGE1", // damageSfxName
    "SFX_MARIO_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_MARIO_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_MARIO_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoHpGauge, // attributeFlags
    &regist, // statusVulnerability
    7, // numParts
    unitpartsdata_Party_Yoshi, // parts
    init_event, // initEventCode
    data_table, // dataTable
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
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_KEEP, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Init for Yoshi: registers his wait, attack, damage, confusion and entry scripts and his walk,
 * run and jump sounds, clears unit work 2 (Ground Pound sets it to 1 from its run-up until the first pound)
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

#if VERSION <= VERSION_DEMO_JP
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
#endif

/**
 * Ground Pound (0 FP): repeated pounds on one enemy from above (not ceiling enemies), 1 damage each,
 * lowered by each earlier hit; flips shelled enemies and grounds winged ones. Usable while confused.
 */
BattleWeapon partyWeapon_YoshiNormalAttack = {
    "btl_wn_pys_normal", // name
    ICON_PARTNER_MOVE_NORMAL, // icon
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_NO_CEILING | TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
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
#if VERSION >= VERSION_US
    {0, 10, 10, 100, 10, 10, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#else
    {0, 10, 10, 100, 10, 5, 5, 5, 5, 5, 0, 0}, // stageEffectRates
#endif
};

/**
 * Gulp (4 FP), the hit on an enemy spat out as a shot at the enemies behind it (swallow attribute 2):
 * pierces defence; by rank 2, 3 or 3 damage on a failed action command, 4, 5 or 6 on success.
 */
BattleWeapon partyWeapon_YoshiNomikomi_Shot = {
    "btl_wn_pys_lv1", // name
    ICON_PARTNER_MOVE_LV1, // icon
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SWALLOW_SHOT, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_nomikomi", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL & ~COUNTER_RESIST_PREEMPTIVE_SPIKY, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyYoshiAttack_Nomikomi, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 10, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * Gulp (4 FP), the hit on an enemy that is spat straight out (no swallow attribute 2):
 * pierces defence; by rank 2, 3 or 3 damage on a failed action command, 4, 5 or 6 on success.
 */
BattleWeapon partyWeapon_YoshiNomikomi_Spew = {
    "btl_wn_pys_lv1", // name
    ICON_PARTNER_MOVE_LV1, // icon
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_SWALLOW_SPEW, // damagePattern
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
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 10, 5, 5, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 5, 5, 5, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/** Gulp's 0-damage tongue hit on an enemy that cannot be swallowed or after a failed action command. */
BattleWeapon partyWeapon_YoshiNomikomi_Dmg0 = {
    "btl_wn_pys_lv1", // name
    ICON_PARTNER_MOVE_LV1, // icon
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
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
 * Gulp's fire breath after swallowing an enemy with swallow attribute 4:
 * 1 fire damage to every enemy, pierces defence.
 */
BattleWeapon partyWeapon_YoshiNomikomi_Fire = {
    "btl_wn_pys_lv1", // name
    ICON_PARTNER_MOVE_LV1, // icon
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_FIRE, // element
    ATTACKPATTERN_NULL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_nomikomi", // acHelpMsg
    SPECIAL_PIERCE_DEFENSE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM | TARGET_WEIGHT_FRONT, // targetWeightFlags
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
 * Damage dealt to the enemy behind when Gulp spits an enemy into it (subsetevt_swallow_shot_damage):
 * pierces defence; by rank 1, 2 or 3 damage on a failed action command, 4, 5 or 6 on success.
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
#if VERSION <= VERSION_DEMO_JP
    TARGET_CLASS_NO_TREE_OR_SWITCH | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_MULTIPLE,
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST,
#else
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_RECOIL | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
#endif
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
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
 * Mini-Egg (3 FP): eggs thrown at random enemies, 1 damage each; a hit shrinks the enemy to Tiny
 * (strength -2) for 3 turns. Cannot be guarded.
 */
BattleWeapon partyWeapon_YoshiEggAttack_Minimini = {
    "btl_wn_pys_lv2", // name
    ICON_PARTNER_MOVE_LV2, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pys_wonder_egg", // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    2, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {1, 1, 1, 1, 1, 1, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
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
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 6, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 3, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * Stampede (6 FP): a herd of Yoshis tramples every enemy, 1 damage per trample, lowered by each earlier hit;
 * the action command decides how many tramples. Cannot be guarded.
 */
BattleWeapon partyWeapon_YoshiCallGuard = {
    "btl_wn_pys_lv3", // name
    ICON_PARTNER_MOVE_LV3, // icon
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
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
#if VERSION >= VERSION_US
    {0, 20, 20, 100, 20, 40, 5, 5, 20, 5, 0, 0}, // stageEffectRates
#else
    {0, 20, 20, 100, 20, 20, 5, 5, 20, 5, 0, 0}, // stageEffectRates
#endif
};

/**
 * Footstep sounds during Ground Pound's run-up and flutter (walk, run, the fast-leg poses and the flutter pose):
 * left and right steps played by _yoshi_slide_move_sound at fixed points of each loop.
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
 * Yoshi's Ground Pound: he jumps up over the enemy
 * and flutters above it during the action command (the yoroyoro jump helpers),
 * then pounds it once per hit the command earned before returning home.
 */
EVT_BEGIN(partyYoshiAttack_NormalAttack)
    // Pick the target, pay the cost and aim the camera at Yoshi.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, -1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    WAIT_FRAMES(20)

    // He faces the enemy and revs his legs in place, the footsteps speeding up.
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_GROUND_POUND, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, -1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_STEP_PITCH, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_STEP_PITCH_SCALE, 100)
    CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(posesound_normal_attack))
    CALL(btlevtcmd_AcSetOutputParam, 1, 0)
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

    // The camera moves to the enemy and Yoshi leaps toward it; the action command (tap R) is set up for his rank.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetTakeoffPosition, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
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
        CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_FAST_GAUGE_DISP | AC_REPEATEDLY_LV_FLAG_MIN_LEVEL_1)
        CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 6, 1, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_WaitEventEnd, LW(15))

    // He flutters over the enemy while the command runs; its result is the number of pounds.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_3"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
    ADD(LW(7), 40)
    SET_VALUE(LW(10), FLOAT(1.0f))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_StartAC, 1)
        CALL(btlevtcmd_ResultAC)
    END_SIBLING_EVENT
    CALL(btl_yoshi_yoroyoro_jump_calc_param, UNIT_ID_SELF, LW(6), LW(7), LW(8), LW(10), LW(9))
    CALL(btl_yoshi_yoroyoro_jump_move, UNIT_ID_SELF, LW(6), LW(7), LW(8), LW(10), LW(9))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    SET_VALUE(LW(13), 1)
    CALL(btlevtcmd_AcGetOutputParam, 2, LW(14))

    // Each pound: check whether it connects; before the first one, pressing A makes him spin (the stylish).
    LABEL(50)
    IF_INT_GREATER_OR_EQUAL(LW(13), 2)
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(5))
    ELSE
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
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

    // He drops onto the enemy; on a miss the enemy dodges and Yoshi crashes to the floor, ending the move.
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 4, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    IF_INT_NOT_EQUAL(LW(5), PRECHECK_HIT)
        CALL(btlevtcmd_StopAC)
        IF_INT_EQUAL(LW(5), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SET_VALUE(LW(15), LW(1))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        SUBTRACT(LW(15), LW(1))
        DIVIDE(LW(15), 10)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(15), 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
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

    // A hit: with a successful command, the pounds after the first show a rising rating (Nice, Good...).
    CALL(btlevtcmd_ac_timing_flag_onoff, 1, 1)
    WAIT_FRAMES(2)
    CALL(btlevtcmd_ac_timing_flag_onoff, 0, 1)
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        IF_INT_GREATER_OR_EQUAL(LW(13), LW(14))
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        ELSE
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_AC_SUCCESS, LW(5))
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
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
        ELSE
            CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), 0, LW(5))
        END_IF
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF

    // He bounces up for the next pound until all earned pounds are used.
    LABEL(90)
    ADD(LW(13), 1)
    IF_INT_LESS_OR_EQUAL(LW(13), LW(14))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.19921875f))
        ADD(LW(1), 40)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        GOTO(50)
    END_IF

    // He hops back down to the floor; pressing A on the landing is the stylish.
    CALL(btlevtcmd_StopAC)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, EVT_NULLPTR, LW(1), EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 60)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 15, 15, 20)
    END_SIBLING_EVENT
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE)
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

    // The audience refills Star Power, then he walks home.
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Yoshi's Gulp: he runs to the target's mouth point (_get_nomikomi_hit_position) and tries to swallow it.
 * Units that cannot be swallowed or a failed command just take a hit;
 * others are either spat into the enemy behind them or, by their swallowAttribute bits,
 * swallowed whole for their experience.
 */
EVT_BEGIN(partyYoshiAttack_Nomikomi)
    // Pick the target and pay the cost.
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
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)

    // Action command (hold R, release when the light comes on): the window shrinks at higher difficulty.
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
    CALL(btlevtcmd_AcSetFlag, AC_POWER_GAUGE_FLAG_FILL_WHILE_HELD | AC_POWER_GAUGE_FLAG_PLAIN_GAUGE | AC_POWER_GAUGE_FLAG_SHOW_ZONE_LAMP | AC_POWER_GAUGE_FLAG_LAMP_SHIFT_LEFT)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 15, 1, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, -1, 0)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)

    // Once R is pressed he walks up to the enemy (a pre-emptive spiky enemy hurts him on the way).
    LABEL(5)
    CALL(btlevtcmd_AcGetOutputParam, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(5)
    END_IF
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), PRECHECK_PREEMPTIVE_SPIKY)
        CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
        MULTIPLY_FLOAT(LW(6), 40)
        CALL(_get_nomikomi_hit_position, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 80, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
    MULTIPLY_FLOAT(LW(6), 40)
    CALL(_get_nomikomi_hit_position, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 80, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))

    // He shoots out his tongue; a miss plays the enemy's dodge reaction.
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_2"))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW1"), 0)
    IF_INT_NOT_EQUAL(LW(5), PRECHECK_HIT)
        IF_INT_EQUAL(LW(5), PRECHECK_LUCKY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(5), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        WAIT_FRAMES(50)
        GOTO(90)
    END_IF

    // Enemies that cannot be swallowed, or a failed command, just take a 0-damage tongue hit.
    WAIT_FRAMES(7)
    CALL(_get_swallow_param, LW(3), LW(7))
    IF_INT_EQUAL(LW(7), -1)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_YoshiNomikomi_Dmg0), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_GULP, LW(5))
        WAIT_FRAMES(43)
        GOTO(90)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_CLEAR(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_YoshiNomikomi_Dmg0), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_GULP, LW(5))
        WAIT_FRAMES(43)
        GOTO(90)
    END_IF

    // Swallow attribute 8: the enemy plays its gulp reaction (damage pattern 7) and Yoshi lets it go.
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW2"), 0)
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

    // He pulls the enemy into his mouth; it is hidden while inside.
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_DamageDirect, LW(3), LW(4), 0, 0, 2, 1)
    WAIT_FRAMES(3)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 12)
    CALL(btlevtcmd_DivePosition, LW(3), LW(0), LW(1), LW(2), 5, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_OnAttribute, LW(3), kUnitHidden)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_SWALLOW3"), 0)
    WAIT_FRAMES(6)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_3"))
    WAIT_FRAMES(7)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)

    // Roll 0-99 against the enemy's swallow rate: at or above it he spits it out (as a shot at the enemies behind
    // with attribute 2), with the rating effect and a stylish chance.
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
            IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
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

    // Otherwise he swallows it whole: the enemy is removed and its experience kept.
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

    // An enemy with swallow attribute 4 lets him breathe fire over every enemy.
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

    // Star Power refill, then he runs home; after a whole swallow the run home ends with a stylish spin chance.
    LABEL(90)
    CALL(btlevtcmd_StopAC)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    LABEL(95)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SWALLOWED, LW(5))
    IF_INT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(4.0f), LW(5))
    IF_INT_LESS_OR_EQUAL(LW(5), 20)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        GOTO(99)
    END_IF
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SUBTRACT(LW(5), 1)
        SET_VALUE(LW(6), LW(5))
        SUBTRACT(LW(6), 10)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(6), LW(5), LW(5), 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(5), MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    IF_INT_EQUAL(LW(6), 2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_O_1"))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_J_1B"))
        CALL(evt_sub_intpl_init, EASING_QUADRATIC_OUT, 0, 360, 30)
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
 * Yoshi's Mini-Egg: he lays eggs as the action command allows and throws each at a target (_egg_attack_event);
 * hits can shrink the enemies. Ends with an optional stylish.
 */
EVT_BEGIN(partyYoshiAttack_EggAttack)
    // Pay the cost and pick a random target for each of the six egg parts.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_WeaponAftereffect, LW(0))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
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

    // Action command (press A when the light comes on to lay an egg): up to 3 eggs, 4 at Ultra rank;
    // less time at higher difficulty.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART)
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
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), (FACE_DIR_TOWARD_UNIT | FACE_DIR_REVERSE), LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_StartAC, 1)
    SET_VALUE(LW(12), 2)

    // Wait for each egg; if the command ends with none laid he groans and the move ends.
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

    // A new egg: the rating effect for the count so far, and a random colour (green, yellow or pink).
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

    // The egg pops out of Yoshi with a spin and rolls to its place behind him.
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
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(12), (kHidden | kNoShadow))
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
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    WAIT_FRAMES(10)
    ADD(LW(12), 1)
    GOTO(5)

    // All eggs laid: mark the last egg aimed at each enemy, which deals that enemy's final hit.
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

    // Decide in advance whether each egg connects; once an earlier egg hits an enemy, the last egg at it is
    // pre-checked without evasion.
    SET_VALUE(LW(12), 2)
    LABEL(28)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_LAST_AT_TARGET, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET, LW(0))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET_PART, LW(1))
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(0), LW(1), DAMAGE_RESULT_FINAL_HIT, LW(6))
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
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(0), LW(1), DAMAGE_RESULT_FINAL_HIT, LW(6))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_PRECHECK, LW(6))
        CASE_DEFAULT
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET, LW(0))
        CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_TARGET_PART, LW(1))
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(0), LW(1), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(6))
        CALL(btlevtcmd_SetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_PRECHECK, LW(6))
    END_SWITCH
    IF_INT_LESS(LW(12), LW(13))
        ADD(LW(12), 1)
        GOTO(28)
    END_IF

    // He throws the eggs one by one, each with a throwing pose for its colour (_egg_attack_event).
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    SET_VALUE(LW(12), 2)
    LABEL(30)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(12), (kHidden | kNoShadow))
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

    // Stylish after the last throw, then the Star Power refill.
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

    // Wait for every egg to land, then he runs home.
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
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    WAIT_MS(2000)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Throws one Mini-Egg: launches egg part LW(12) from Yoshi to target LW(3)/LW(4) in a spinning arc,
 * bursts it there and applies the damage weapon in its part work 2,
 * or plays the target's dodge when part work 5 marks a miss.
 */
EVT_BEGIN(_egg_attack_event)
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
    MULTIPLY_FLOAT(LW(6), 10)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), LW(6))
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(12), (kHidden | kNoShadow))
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
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2), 0, 25, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(12), (kHidden | kNoShadow))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_EGG2"), 0)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(0.5996094f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetPartsWork, UNIT_ID_SELF, LW(12), PARTS_WORK_PRECHECK, LW(6))
    IF_INT_NOT_EQUAL(LW(6), 1)
        IF_INT_EQUAL(LW(6), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
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
 * Yoshi's Stampede: he calls a herd of Yoshis that runs across the stage (_gundan_yoshi_run_effect)
 * while the action command builds damage, hitting every enemy as the herd passes,
 * then waits for the herd to finish.
 */
EVT_BEGIN(partyYoshiAttack_CallGuard)
    // Target every enemy and pay the cost.
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
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)

    // Action command (alternate L and R to fill the gauge): its length depends on the difficulty.
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
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
    CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_MIN_LEVEL_1)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 6, 1, 0)

    // The camera pulls back; Yoshi winds up while the command runs, then calls the herd.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 4)
    CALL(evt_btl_camera_set_zoom, 0, 400)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_10"))
    CALL(btlevtcmd_ftof, 24, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PYS_A_11"))
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(6))
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(0))
    MULTIPLY_FLOAT(LW(6), LW(0))
#endif
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
#if VERSION >= VERSION_JP
    ADD(LW(1), LW(6))
#endif
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    MULTIPLY(LW(5), 30)
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_eff, STRING(""), STRING("kiss"), 3, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
#else
    CALL(evt_eff, 0, STRING("kiss"), 3, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_ENM_TOGENOKO_CALL1"), LW(0), LW(1), LW(2), 0)
#endif

    // Stylish chance as he calls.
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

    // The herd of Yoshis runs across the stage while the screen rumbles.
    BEGIN_INLINE_EVENT_GET_ID(LW(9))
        WAIT_MS(1000)
        CALL(_gundan_yoshi_run_effect, UNIT_ID_SELF)
    END_INLINE_EVENT
    WAIT_MS(166)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_shake_h, 0, 1, 0, 6000, 0)
    WAIT_MS(1000)
    CALL(_wait_yoshig_run, UNIT_ID_SELF)

    // Each enemy in turn as the herd reaches it: a miss plays its dodge reaction.
    SET_VALUE(LW(15), 0)
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
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_DODGY)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(6), PRECHECK_MISS)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        GOTO(75)
    END_IF

    // The herd tramples it once per hit the gauge earned; the last trample carries the rating effect.
    CALL(btlevtcmd_AcGetOutputParam, 2, LW(14))
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

    // Wait for the herd to leave, stop the rumble, refill Star Power and end the turn.
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

/** Yoshi's Appeal (data tag 5): appeal pose and sound, then the audience is fired up. */
EVT_BEGIN(yoshi_appeal_event)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
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
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(12), 2)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2))
    BEGIN_SIBLING_EVENT
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_AUDIENCE)
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
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(12), (kHidden | kNoShadow))
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
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(12), (kHidden | kNoShadow))
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
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(12), LW(0), LW(1), LW(2), 0, 25, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(12), (kHidden | kNoShadow))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_YOSHI_EGG2"), 0)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(0.5996094f), 0, 0, 0, 0, 0, 0, 0)
    CALL(_attack_aud, LW(3), 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_MS(500)
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Empty: partner moves come from the player's menu choices, not a unit attack script. */
static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

/** Idle (JP and later position; the early demos define it near the top of the file): plays the stay pose from the pose table. */
#if VERSION >= VERSION_JP
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
#endif

u8 _yoshig_tpl_no_tbl[8] __attribute__((aligned(4))) = { // battle tex id per sweat frame (US ids +0x16)
    VERSION_PICK(0x46, 0x5C), VERSION_PICK(0x49, 0x5F), VERSION_PICK(0x47, 0x5D), VERSION_PICK(0x46, 0x5C),
    VERSION_PICK(0x4A, 0x60), VERSION_PICK(0x48, 0x5E), 0, 0,
};

/**
 * Swaps unit args[0]'s main part to the battle model for Yoshi's hatched colour (party colour slot 4,
 * c_babyyoshi2 to c_babyyoshi7); colour 0 keeps the default green model. Blocks until the model has loaded.
 * Run from init_event.
 */
USER_FUNC(yoshi_original_color_anim_set) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnitPart* part;
    const char* name;
    EvtWord* args = event->args;
    s32 id;

    if (part && part) { // FAKE: empty test of the uninitialised part pointer; removing it changes codegen
    }
    id = evtGetValue(event, args[0]);
    id = BattleTransID(event, id);
    BattleGetUnitPtr(wp, id);
    switch (pouchGetPartyColor(PARTNER_YOSHI)) {
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

/**
 * Builds Yoshi's battle move list (data tag 1):
 * Ground Pound and Gulp always, Mini-Egg from Super rank and Stampede from Ultra rank,
 * appending each to command->weapon and advancing *count.
 */
static void __makeTechMenuFunc(BattleCommand* command, s32* count) {
    s32 techLv;

    techLv = partyGetTechLv(BattleTransPartyId(BattleGetPartyPtr(_battleWorkPointer)->currentKind));

    command->weapon[*count].index = -1;
    command->weapon[*count].itemId = 0;
    command->weapon[*count].weapon = &partyWeapon_YoshiNormalAttack;
    command->weapon[*count].enabled = 0;
    command->weapon[*count].icon = command->weapon[*count].weapon->icon;
    command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
    (*count)++;

    if (techLv >= 0) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_YoshiNomikomi_Shot;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 1) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_YoshiEggAttack_Minimini;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 2) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_YoshiCallGuard;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
}

/**
 * Sets up Ground Pound's flutter for unit args[0] towards (args[1], args[2], args[3]):
 * a 180-frame move whose horizontal speed covers the X distance.
 * Returns the speed in args[4] and the frame count in args[5].
 */
USER_FUNC(btl_yoshi_yoroyoro_jump_calc_param) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id, outSpeed, outFrames;
    f32 tx, ty, tz, speed;
    f32 x, y, z;
    EvtWord* args = event->args;

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
 * Runs the flutter set up by btl_yoshi_yoroyoro_jump_calc_param one frame per call:
 * drifts unit args[0] toward the target while bobbing up and down, sets the step sound pitch scale in
 * UNIT_WORK_STEP_PITCH_SCALE, and spawns a sweat drop every 21 frames. Blocks until the 180 frames are over, then snaps to the target.
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
            ((EffSweatN64Work*)eff->userdata)->position.x = unit->moveCurrentPos.x;
            ((EffSweatN64Work*)eff->userdata)->position.y = unit->moveCurrentPos.y + unit->sizeMultiplier * (0.5f * BtlUnit_GetHeight(unit));
            ((EffSweatN64Work*)eff->userdata)->position.z = unit->moveCurrentPos.z;
        }
    }
    unit->moveFrames--;
    if (unit->moveFrames <= 0) {
        BtlUnit_SetPos(unit, unit->moveTargetPos.x, unit->moveTargetPos.y, unit->moveTargetPos.z);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Pose sound callback for Yoshi's walk, run and flutter poses:
 * plays the footstep sound at pitch UNIT_WORK_STEP_PITCH * UNIT_WORK_STEP_PITCH_SCALE / 100.
 */
void _yoshi_slide_move_sound(BattleUnit* unit, PoseSoundTiming* s) {
    BtlUnit_snd_se(unit, s->sfxName, EVT_NULLPTR, (s16)(unit->work[UNIT_WORK_STEP_PITCH] * unit->work[UNIT_WORK_STEP_PITCH_SCALE] / 100)); // the slide sound's pitch
}

/**
 * Returns in args[3]-args[5] where Yoshi's tongue should reach target unit args[1] for Gulp:
 * its position plus its bintaHitOffset (scaled by size, X mirrored by facing).
 * args[0] and args[2] (the part) are read but not used.
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

/** Returns unit args[0]'s swallowRate in args[1]; Gulp treats -1 as cannot be swallowed. */
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
 * Returns 1 in args[2] if unit args[0]'s swallowAttribute has any of the bits in args[1], else 0.
 * Gulp checks bit 8 (hurts when swallowed), 2 (spat as a shot) and 1 and 4 (how it is swallowed whole).
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

/**
 * Blocks until the Stampede herd of unit args[0] exists
 * and its average X position (meanX) has crossed the stage centre, so the attack can start hitting enemies.
 */
USER_FUNC(_wait_yoshig_run) {
    BattleUnit* unit;
    YoshiGundanWork* w;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    w = (YoshiGundanWork*)unit->countersetWeapon;
    if (w == NULL) {
        return EVT_RETURN_BLOCK;
    }
    if (w->meanX <= 0.0f) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Blocks until unit args[0]'s Stampede herd has run off and its work (kept in countersetWeapon) is freed. */
USER_FUNC(_wait_yoshig_complete) {
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (unit->countersetWeapon) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/**
 * Moves one Stampede Yoshi (index i) for a frame:
 * on the first call places it in a 16-wide grid off the left edge with a random speed and small depth drift,
 * then runs it right, bouncing between the depth limits, until it reaches X 300.
 * Returns FALSE once it has finished.
 */
static inline s32 _gundan_part_update(YoshiGundanPart* p, s32 i) {
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
 * Runs the Stampede herd for unit args[0]: allocates 256 running Yoshi sprites (work stored in
 * countersetWeapon), moves them each frame with dust puffs and a run sound that follows the herd's average X,
 * draws them, and frees the work and stops the sound when all are across.
 * US and later cap the herd's depth at 65 instead of 80.
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(_gundan_yoshi_run_effect) {
#else
USER_FUNC(_gundan_yoshi_run_effect) {
#endif

    BattleUnit* unit;
    YoshiGundanWork* w;
    YoshiGundanPart* p;
    s32 i;
    s32 allDone;
    Vec pos;
    s32 id;

    do {
    id = evtGetValue(event, event->args[0]);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    if (isFirstCall) {
        w = BattleAlloc(sizeof(YoshiGundanWork));
        unit->countersetWeapon = (BattleWeapon*)w;
        memset(w, 0, sizeof(YoshiGundanWork));
        w->state = 0;
        w->meanX = 0.0f;
        event->userdata[0] = 0;
        event->userdata[1] = psndSFXOn("SFX_BTL_YOSHI_TAIGUN1");
    } else {
        w = (YoshiGundanWork*)unit->countersetWeapon;
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
        BattleFree(unit->countersetWeapon);
        unit->countersetWeapon = NULL;
        return EVT_RETURN_DONE;
    }
#if VERSION <= VERSION_DEMO_JP
    dispEntry(CAMERA_3D, 0.0f, DISP_MODE_XLU, (DispCallback)_btlYoshiDisp, w);
#else
    for (i = 0; i < 256; i++) {
        // Every runner is sorted with the depth of p left by the loop above; the callback data still selects each runner.
        dispEntry(CAMERA_3D, dispCalcZ((Vec*)p), VERSION_PICK(DISP_MODE_XLU, DISP_MODE_ALPHA_TEST), (DispCallback)_btlYoshiDisp, &w->parts[i]);
    }
#endif
    } while (0);
    return EVT_RETURN_BLOCK;
}

/** Draws one Stampede Yoshi (JP and later; the early demos draw all 256 in one call). */
#if VERSION <= VERSION_DEMO_JP
static void _btlYoshiDisp(CameraId camera, YoshiGundanWork* w) {
    YoshiGundanPart* p;
    s32 i;
#else
void _btlYoshiDisp(CameraId camera, YoshiGundanPart* p) {
#endif
    f32 s;
#if VERSION <= VERSION_DEMO_JP
    for (i = 0; i < 256; i++) {
        p = &w->parts[i];
        s = (p->vx < 0.0f) ? 0.4f : -0.4f;
        btlDispTexPlane(p->x, p->y + 20.0f, p->z, s, 0.4f, _yoshig_tpl_no_tbl[p->frame], (GXColor){255,255,255,255}, 0);
    }
}
#else
    s = (p->vx < 0.0f) ? 0.4f : -0.4f;
    btlDispTexPlane(p->x, p->y + 20.0f, p->z, s, 0.4f, _yoshig_tpl_no_tbl[p->frame], (GXColor){255,255,255,255}, 0);
}
#endif

// FAKE: stand-in that nothing calls; its s32-to-f32 conversion likely shapes the constant pool (as
// kpaStrippedPool does in koopa.c); removing it changes the object
void yoshiStrippedPool(f32* f, s32 i) {
    *f = (f32)i;
}
