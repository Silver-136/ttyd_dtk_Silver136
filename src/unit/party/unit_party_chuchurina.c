// Battle unit data and scripts for Ms. Mowz (Chuchurina) as a partner.

#include "version.h"
#include "battle/battle.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "driver/camdrv.h"
#include "driver/dispdrv.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario_party.h"
#include "memory.h"
#include "system.h"
#include <string.h>
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "battle/battle_attack_audience.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_message.h"
#include "battle/battle_icon.h"
#include "battle/battle_camera.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_audience.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "mario/mario_pouch.h"
#include "battle/battle_damage.h"
#include "icon_ids.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_PART1_RUNNING 0 // MadowaseAttack: 1 while the body (part 1) still runs its route; the attack waits until slots 0-3 are all 0
#define UNIT_WORK_PART2_RUNNING 1 // MadowaseAttack: 1 while part 2 still runs its route
#define UNIT_WORK_PART3_RUNNING 2 // MadowaseAttack: 1 while part 3 still runs its route
#define UNIT_WORK_PART4_RUNNING 3 // MadowaseAttack: 1 while part 4 still runs its route
#define UNIT_WORK_SPIN_EVENT_ID 4 // NormalAttack: id of the sibling event that spins her back after a slap (0 = none); deleted before the next slap
#define UNIT_WORK_MAX_SLAPS 5     // NormalAttack: number of slaps (2, 3, 4 by party tech level); the attack ends when the slap count reaches it

void btlUnitPartsDisp(CameraId camId, BattleUnitPart* parts);

PoseTableEntry pose_table_chuchurina_stay[24];
static BattleDataEntry data_table[8];
BattleUnitKindPart unitpartsdata_Party_Chuchurina[4];
BattleUnitKind unitdata_Party_Chuchurina;
static EvtWord battle_entry_event[59];
static EvtWord init_event[48];
static EvtWord damage_event[16];
BattleWeapon partyWeapon_ChuchurinaNormalAttackLeft;
BattleWeapon partyWeapon_ChuchurinaNormalAttackRight;
BattleWeapon partyWeapon_ChuchurinaNormalAttackLeftLast;
BattleWeapon partyWeapon_ChuchurinaNormalAttackRightLast;
BattleWeapon partyWeapon_ChuchurinaMadowaseAttack;
BattleWeapon partyWeapon_ChuchurinaItemSteal;
BattleWeapon partyWeapon_ChuchurinaKiss;
#if VERSION <= VERSION_DEMO_US
EvtWord partyChuchurinaAttack_NormalAttack[1578];
EvtWord partyChuchurinaAttack_MadowaseAttack[2030];
#else
EvtWord partyChuchurinaAttack_NormalAttack[1590];
EvtWord partyChuchurinaAttack_MadowaseAttack[2020];
#endif
EvtWord partyChuchurinaAttack_ItemSteal[VERSION_PICK(788, 961)];
#if VERSION <= VERSION_DEMO_US
EvtWord partyChuchurinaAttack_Kiss[490];
#else
EvtWord partyChuchurinaAttack_Kiss[568];
#endif
EvtWord chuchurina_appeal_event[86];
static EvtWord party_win_reaction[18];
static EvtWord attack_audience_event[VERSION_PICK(204, 205)];
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static EvtWord attack_event[2];
static EvtWord wait_event[6];
static EvtWord main_evt[4];

USER_FUNC(_chuchu_make_extra_work_area);
USER_FUNC(mono_main);
USER_FUNC(mono_off);
USER_FUNC(mono_on);
USER_FUNC(mono_capture_event);
USER_FUNC(_chuchu_item_steal);
USER_FUNC(_get_itemsteal_param);
USER_FUNC(_make_madowase_weapon);
USER_FUNC(_get_binta_hit_position);
static void __makeTechMenuFunc(BattleCommand* command, s32* count);









#if VERSION <= VERSION_DEMO_US
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
PoseTableEntry pose_table_chuchurina_stay[] = {
    {POSE_TYPE_FREEZE, "PCH_D_4"},
    {POSE_TYPE_STOP, "PCH_D_4"},
    {POSE_TYPE_DIZZY, "PCH_X_1"},
    {POSE_TYPE_POISON, "PCH_D_2"},
    {POSE_TYPE_CONFUSE, "PCH_X_1"},
    {POSE_TYPE_SLEEP, "PCH_N_1"},
    {POSE_TYPE_KO, "PCH_D_3"},
    {POSE_TYPE_NORMAL, "PCH_S_1"},
    {POSE_TYPE_DAMAGE_DRAINED, "PCH_G_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "PCH_G_1"},
    {POSE_TYPE_DAMAGE, "PCH_D_1"},
    {POSE_TYPE_WALK, "PCH_W_1"},
    {POSE_TYPE_RUN, "PCH_R_1"},
    {POSE_TYPE_PERIL, "PCH_D_2"},
    {POSE_TYPE_DANGER, "PCH_D_2"},
    {POSE_TYPE_ESCAPE_STUMBLE, "PCH_D_4"},
    {POSE_TYPE_ESCAPE_RECOVER, "PCH_D_2"},
    {POSE_TYPE_TALK, "PCH_T_1"},
    {POSE_TYPE_ITEM, "PCH_I_1"},
    {POSE_TYPE_COMMAND, "PCH_C_1"},
    {POSE_TYPE_ZERO_GRAVITY, "PCH_X_1"},
    {POSE_TYPE_AVOID_COUNTER, "PCH_O_1"},
    {POSE_TYPE_DEFEND, "PCH_G_1"},
    {POSE_TYPE_ACTION, "PCH_S_1"},
};

/**
 * Ms. Mowz's battle data events: Appeal (5), the reaction when an audience member is attacked (6), her move
 * menu builder (1), nothing on a First Strike (48), the victory pose (64), an empty step run after a move's
 * cost is paid (7) and the default spin when she superguards an attack (47).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_APPEAL_EVENT, chuchurina_appeal_event},
    {UNIT_DATA_ATTACK_AUDIENCE, attack_audience_event},
    {UNIT_DATA_TECH_MENU_FUNC, __makeTechMenuFunc},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {UNIT_DATA_WIN_REACTION_EVENT, party_win_reaction},
    {UNIT_DATA_ATTACK_READY_EVENT, subset_attack_ready_jump_event},
    {UNIT_DATA_AVOID_COUNTER_EVENT, btldefaultevt_avoid_counter_event},
    {0, NULL},
};

/**
 * Ms. Mowz's parts, all model c_tyutyu: part 1 is her body; parts 2-4 are hidden, untargetable copies of her
 * that pop out and dash around during Tease.
 */
BattleUnitKindPart unitpartsdata_Party_Chuchurina[] = {
    {1, "btl_un_chuchurina", "c_tyutyu", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table_chuchurina_stay},
    {2, "btl_un_chuchurina", "c_tyutyu", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_chuchurina_stay},
    {3, "btl_un_chuchurina", "c_tyutyu", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_chuchurina_stay},
    {4, "btl_un_chuchurina", "c_tyutyu", 0.0f, 0.0f, 0.0f, 16.0f, 24.0f, 5.0f, 0.0f, 24.0f, 0.0f, 20, 24, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_chuchurina_stay},
};

/**
 * Ms. Mowz's battle unit (name btl_un_chuchurina), used whenever she is the partner in a fight: size, hurt sounds,
 * status table, four parts and init script. No HP gauge is drawn over her; her HP comes from the partner stats.
 */
BattleUnitKind unitdata_Party_Chuchurina = {
    UNIT_MS_MOWZ, // unitType
    "btl_un_chuchurina", // name
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
    {10, 22}, // hitOffset
    {0.0f, 12.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 30.0f, 0.0f}, // talkTogeBaseOffset
    {0.0f, 30.0f, 0.0f}, // heldItemBaseOffset
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
    4, // numParts
    unitpartsdata_Party_Chuchurina, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/**
 * Ms. Mowz's battle entry: starts 250 units behind her home spot and, after 40 frames,
 * runs in to it at speed 4 with her footstep sounds, then takes her stay pose.
 */
static EVT_BEGIN(battle_entry_event)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 250)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_SetMoveSoundDataWork, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_SUBMOVE1L"), STRING("SFX_PARTY_BATTLE_SUBMOVE1R"), 0, 7, 7, 0, 0)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_KEEP, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Init for Ms. Mowz: allocates the spare weapon Tease copies into (_chuchu_make_extra_work_area),
 * registers her wait, attack, damage, confusion and entry scripts and her walk, run and jump sounds.
 */
static EVT_BEGIN(init_event)
    CALL(_chuchu_make_extra_work_area)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Hit reaction: straightens her rotation (left over from her spinning moves), then runs the default damage script. */
static EVT_BEGIN(damage_event)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

#if VERSION <= VERSION_DEMO_US
/** Idle: plays the stay pose from the current pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
#endif
BattleWeapon partyWeapon_ChuchurinaNormalAttackLeft = {
    "btl_wn_pcr_normal", // name
    ICON_PARTNER_MOVE_NORMAL, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pch_binta", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_L, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_binta", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
#if VERSION <= VERSION_DEMO_US
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
#else
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
#endif
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_NormalAttack, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 4, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/** Love Slap's slap from the other side (2 damage), alternating with the one above. */
BattleWeapon partyWeapon_ChuchurinaNormalAttackRight = {
    "btl_wn_pcr_normal", // name
    ICON_PARTNER_MOVE_NORMAL, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pch_binta", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_R, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_binta", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
#if VERSION <= VERSION_DEMO_US
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
#else
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
#endif
    0x80002004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_NormalAttack, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 4, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 3, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/** Love Slap's final slap (or one after a failed command), its power the command's gauge level. */
BattleWeapon partyWeapon_ChuchurinaNormalAttackLeftLast = {
    "btl_wn_pcr_normal", // name
    ICON_PARTNER_MOVE_NORMAL, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pch_binta", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetACOutputParam, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_L_LAST, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_binta", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_NormalAttack, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 4, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/** Love Slap's final slap from the other side, its power the command's gauge level. */
BattleWeapon partyWeapon_ChuchurinaNormalAttackRightLast = {
    "btl_wn_pcr_normal", // name
    ICON_PARTNER_MOVE_NORMAL, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pch_binta", // description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    weaponGetACOutputParam, // hpDamageFn
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_BINTA_R_LAST, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_binta", // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_GROUND_WINGED | SPECIAL_CONFUSED_ACTION | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_NormalAttack, // attackEvtScript
#if VERSION >= VERSION_US
    {0, 0, 0, 100, 0, 4, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#else
    {0, 0, 0, 100, 0, 2, 2, 0, 0, 0, 0, 0}, // stageEffectRates
#endif
};

/**
 * Tease (btl_wn_pcr_madowase, 3 FP), run by partyChuchurinaAttack_MadowaseAttack: no damage, hits every enemy and
 * can make them dizzy for 3 turns. The script applies a copy of it (_make_madowase_weapon) whose dizzy chance
 * comes from the hold-and-release gauge command.
 */
BattleWeapon partyWeapon_ChuchurinaMadowaseAttack = {
    "btl_wn_pcr_madowase", // name
    ICON_PARTNER_MOVE_LV2, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pch_madowaseru", // description
    100, // baseAccuracy
    3, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
#if VERSION >= VERSION_US
    1, // stylishCommandMultiplier
#else
    3, // stylishCommandMultiplier
#endif
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_madowaseru", // acHelpMsg
    SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, // sleepChance..stopTime
    100, // dizzyChance
    3, // dizzyTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonChance..dodgyChance
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyTime..sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_MadowaseAttack, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Kiss Thief (btl_wn_pcr_steal, 2 FP), run by partyChuchurinaAttack_ItemSteal: no damage, one frontmost enemy;
 * with the stop-the-gauge command she steals its held item or badge.
 */
BattleWeapon partyWeapon_ChuchurinaItemSteal = {
    "btl_wn_pcr_steal", // name
    ICON_PARTNER_MOVE_LV1, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pch_heart_catch", // description
    100, // baseAccuracy
    2, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_CHUCHURINA_ITEMSTEAL, // damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_heart_catch", // acHelpMsg
    SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_TOP_SPIKY | COUNTER_RESIST_PAYBACK, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_ItemSteal, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Smooch (btl_wn_pcr_kiss, 10 FP), run by partyChuchurinaAttack_Kiss: always hits one ally, preferring one low on
 * HP or in peril; restores HP by the gauge level reached by alternately tapping A and B.
 */
BattleWeapon partyWeapon_ChuchurinaKiss = {
    "btl_wn_pcr_kiss", // name
    ICON_PARTNER_MOVE_LV3, // icon
    {0}, // pad_06
    0, // itemId
    "msg_pch_kiss", // description
    100, // baseAccuracy
    10, // baseFpCost
    0, 0, // baseSpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    5, // reserved_19
    100, // bingoSlotChance
    50, // reserved_1B
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    "msg_ac_chuchurina_kiss", // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_LOW_HP_RATIO | TARGET_WEIGHT_PREFER_LOW_HP | TARGET_WEIGHT_PREFER_IN_PERIL | TARGET_WEIGHT_UNUSED_2000 | TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    partyChuchurinaAttack_Kiss, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 5, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Ms. Mowz's Love Slap: she moves to the enemy's slap point (_get_binta_hit_position) and slaps it back
 * and forth, one hit per successful action command press, the last slap using the final-hit weapon;
 * then returns home.
 */
/**
 * Ms. Mowz's Love Slap: she moves to the enemy's slap point (_get_binta_hit_position) and slaps it back
 * and forth, one hit per successful action command press, the last slap using the final-hit weapon;
 * then returns home.
 */
EVT_BEGIN(partyChuchurinaAttack_NormalAttack)
    // Find the chosen enemy and pay for the move; with no target, skip to the end.
    CALL(btlevtcmd_JumpSetting, UNIT_ID_SELF, 20, FLOAT(0.0f), FLOAT(0.69921875f))
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
    // Camera follows her and the target.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_PART_TO_PART)
    CALL(evt_btl_camera_set_homing_unitparts, 0, UNIT_ID_SELF, 1, LW(3), LW(4))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    // Against an enemy with pre-emptive front spikes (pre-check result 5) she first walks up in front of it,
    // and the damage check there pricks her on the spikes.
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY_FLOAT(LW(6), 40)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_CommandCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    // She walks to the slap point beside the enemy and faces it.
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(5))
    MULTIPLY_FLOAT(LW(5), 12)
    CALL(_get_binta_hit_position, UNIT_ID_SELF, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(5))
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    ADD(LW(2), 5)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    // Camera closes in; she winds up while the left-right stick gauge command is set up from the difficulty
    // and her rank.
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A1_1"))
    BEGIN_SIBLING_EVENT_GET_ID(LW(0))
        CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
        CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_AcGetDifficulty, LW(0))
        SUBTRACT(LW(0), 3)
        SWITCH(LW(0))
            CASE_LESS_OR_EQUAL(-3)
            SET_VALUE(LW(1), 25)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(-2)
            SET_VALUE(LW(1), 20)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(-1)
            SET_VALUE(LW(1), 17)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(0)
            SET_VALUE(LW(1), 15)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(1)
            SET_VALUE(LW(1), 14)
            SET_VALUE(LW(2), 30)
            CASE_EQUAL(2)
            SET_VALUE(LW(1), 13)
            SET_VALUE(LW(2), 30)
            CASE_DEFAULT
            SET_VALUE(LW(1), 12)
            SET_VALUE(LW(2), 30)
        END_SWITCH
        CALL(btlevtcmd_GetPartyTechLv, UNIT_ID_SELF, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AcSetParamAll, 14, 210, 178, LW(1), LW(2), 34, 35, EVT_NULLPTR)
            CALL(btlevtcmd_AcSetGaugeParam, 34, 68, 100, 100)
            CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_MIN_LEVEL_1)
            CASE_EQUAL(1)
            CALL(btlevtcmd_AcSetParamAll, 14, 210, 178, LW(1), LW(2), 50, 51, EVT_NULLPTR)
            CALL(btlevtcmd_AcSetGaugeParam, 34, 68, 100, 100)
            CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_MIN_LEVEL_1)
            CASE_DEFAULT
            CALL(btlevtcmd_AcSetParamAll, 14, 210, 178, LW(1), LW(2), 100, 100, EVT_NULLPTR)
            CALL(btlevtcmd_AcSetGaugeParam, 100, 100, 100, 100)
            CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_MIN_LEVEL_1)
        END_SWITCH
        CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 6, 1, 0)
        WAIT_FRAMES(1)
        CALL(btlevtcmd_StartAC, 1)
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(0))
    // While the command runs she pulses and turns redder as the gauge fills.
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(6), 0)
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            ADD(LW(6), 1)
            SET_VALUE(LW(0), LW(6))
            DIVIDE(LW(0), 2)
            MODULO(LW(0), 2)
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0498047f), FLOAT(1.0498047f), FLOAT(1.0f))
            ELSE
                CALL(btlevtcmd_SetScale, UNIT_ID_SELF, FLOAT(1.0f), FLOAT(1.0f), FLOAT(1.0f))
            END_IF
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(5), 5)
        LOOP(LW(5))
            CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
            SET_VALUE(LW(1), 255)
            SET_VALUE(LW(2), 190)
            MULTIPLY(LW(2), LW(0))
            DIVIDE(LW(2), 100)
            SUBTRACT(LW(1), LW(2))
            CALL(btlevtcmd_SetRGB, UNIT_ID_SELF, 1, 255, LW(1), LW(1))
            WAIT_FRAMES(1)
            SET_VALUE(LW(5), 5)
        END_LOOP
    END_SIBLING_EVENT
    // Command over: the number of slaps comes from her rank (2, 3 or 4).
    CALL(btlevtcmd_ResultAC)
    DELETE_EVENT(LW(14))
    DELETE_EVENT(LW(15))
    CALL(btlevtcmd_GetPartyTechLv, UNIT_ID_SELF, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MAX_SLAPS, 4)
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MAX_SLAPS, 3)
        CASE_DEFAULT
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MAX_SLAPS, 2)
    END_SWITCH
    // She leans back to wind up the first slap.
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 300)
    CALL(btlevtcmd_SetRGB, UNIT_ID_SELF, 1, 255, 255, 255)
    BEGIN_SIBLING_EVENT_GET_ID(LW(14))
        SET_VALUE(LW(8), -75)
        CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
        IF_INT_GREATER_OR_EQUAL(LW(0), 180)
            SUBTRACT(LW(0), 360)
        END_IF
        CALL(evt_sub_intpl_init, EASING_CUBIC_OUT, LW(0), LW(8), 8)
        LOOP(8)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
        SET_VALUE(LW(8), -60)
        SET_VALUE(LW(9), -75)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(9), LW(8), 4)
        LOOP(4)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(14))
    // Each slap starts here: check whether the enemy can be slapped.
    SET_VALUE(LW(13), 18)
    SET_VALUE(LW(10), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_EVENT_ID, 0)
    LABEL(10)
    ADD(LW(10), 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A1_2"))
    IF_INT_GREATER_OR_EQUAL(LW(10), 2)
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(5))
    ELSE
        CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    SWITCH(LW(5))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_OR_EQUAL(6)
        CASE_OR_EQUAL(4)
        CASE_END
        CASE_EQUAL(1)
        CASE_DEFAULT
        WAIT_MS(1000)
    END_SWITCH
    // She cannot hit it: the enemy's dodge or counter plays, she spins round, looks dejected and goes home.
    IF_INT_NOT_EQUAL(LW(5), 1)
        DELETE_EVENT(LW(14))
        SET_VALUE(LW(14), 0)
        CALL(btlevtcmd_StopAC)
        IF_INT_EQUAL(LW(5), 3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        END_IF
        IF_INT_EQUAL(LW(5), 6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        END_IF
        IF_INT_EQUAL(LW(5), 2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        END_IF
        LOOP(20)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 18, 0)
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(36)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 10, 0)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A2_4"))
        WAIT_FRAMES(52)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        GOTO(95)
    END_IF
    // Slaps alternate right and left; the last slap, or one after a failed command, uses the final-slap weapon.
    SET_VALUE(LW(0), LW(10))
    MODULO(LW(0), 2)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackRight))
    ELSE
        SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeft))
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_EVENT_ID, LW(15))
    IF_INT_NOT_EQUAL(LW(15), 0)
        DELETE_EVENT(LW(15))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_EVENT_ID, 0)
    END_IF
    IF_INT_NOT_EQUAL(LW(14), 0)
        DELETE_EVENT(LW(14))
        SET_VALUE(LW(14), 0)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MAX_SLAPS, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
        GOTO(20)
    ELSE
        CALL(btlevtcmd_GetResultAC, LW(6))
        IF_BITS_CLEAR(LW(6), AC_RESULT_SUCCESS)
            GOTO(20)
        END_IF
    END_IF
    GOTO(21)
    LABEL(20)
    IF_INT_EQUAL(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeft))
        SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeftLast))
    ELSE
        SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackRightLast))
    END_IF
    // If the enemy would counter this slap, she straightens up, the slap lands and the script ends here.
    LABEL(21)
#if VERSION <= VERSION_DEMO_US
    CALL(btlevtcmd_CommandPreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), 256, LW(0))
#else
    CALL(btlevtcmd_PreCheckCounter, UNIT_ID_SELF, LW(3), LW(4), LW(12), 256, LW(0))
#endif
    SWITCH(LW(0))
        CASE_OR_EQUAL(7)
        CASE_OR_EQUAL(8)
        CASE_OR_EQUAL(9)
        CASE_OR_EQUAL(10)
        CASE_OR_EQUAL(11)
        CASE_OR_EQUAL(12)
        CASE_OR_EQUAL(13)
        CASE_OR_EQUAL(14)
        CASE_OR_EQUAL(15)
        CASE_OR_EQUAL(16)
        CASE_OR_EQUAL(17)
#if VERSION > VERSION_DEMO_US
        IF_INT_EQUAL(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeft))
            SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackLeftLast))
        ELSE
            SET_VALUE(LW(12), PTR(&partyWeapon_ChuchurinaNormalAttackRightLast))
        END_IF
#endif
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            SET_VALUE(LW(7), LW(13))
            CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
            // BUG: this IF never gets an END_IF (END_SIBLING_EVENT comes first); the rest of the script is indented one level deeper because of it
            IF_INT_GREATER_OR_EQUAL(LW(0), 180)
                SUBTRACT(LW(0), 360)
                CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(0), 0, 4)
                LOOP(4)
                    CALL(evt_sub_intpl_get_value)
                    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, LW(0))
                    WAIT_FRAMES(1)
                    IF_INT_EQUAL(LW(1), 1)
                        BREAK_LOOP
                    END_IF
                END_LOOP
                CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(15))
            CALL(btlevtcmd_GetResultAC, LW(6))
            IF_BITS_SET(LW(6), AC_RESULT_SUCCESS)
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
            ELSE
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
            END_IF
            RETURN
            CASE_END
        END_SWITCH
        // The slap lands: a screen shake, a slap sound (a heavier one for the last slap) and the damage.
        CALL(evt_btl_camera_shake_h, 0, 2, 0, 12, 13)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MAX_SLAPS, LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
            CALL(evt_snd_sfxon, STRING("SFX_BTL_CHUCHURINA_ATTACK3"), 0)
            IF_BITS_SET(LW(6), AC_RESULT_SUCCESS)
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
            ELSE
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
            END_IF
        ELSE
            SET_VALUE(LW(0), LW(10))
            AND(LW(0), 1)
            IF_INT_EQUAL(LW(0), 0)
                CALL(evt_snd_sfxon, STRING("SFX_BTL_CHUCHURINA_ATTACK2"), 0)
            ELSE
                CALL(evt_snd_sfxon, STRING("SFX_BTL_CHUCHURINA_ATTACK1"), 0)
            END_IF
            IF_BITS_SET(LW(6), AC_RESULT_SUCCESS)
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_AC_SUCCESS, LW(5))
            ELSE
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
            END_IF
        END_IF
        // Rating for this slap: the success effect and the audience's reaction.
        CALL(btlevtcmd_GetResultAC, LW(6))
        IF_BITS_SET(LW(6), AC_RESULT_SUCCESS)
            CALL(btlevtcmd_AcGetOutputParam, 2, LW(0))
            SUBTRACT(LW(0), 2)
            IF_INT_GREATER_OR_EQUAL(LW(0), 0)
                CALL(btlevtcmd_GetResultPrizeLv, LW(3), LW(0), LW(11))
                CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_ACSuccessEffect, LW(11), LW(0), LW(1), LW(2))
                CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(11))
            ELSE
                CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
            END_IF
        ELSE
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        END_IF
        // Good command: she swings back for the next slap, or spins round between slaps from the third slap on.
        IF_BITS_SET(LW(6), AC_RESULT_SUCCESS)
            SWITCH(LW(10))
                CASE_LESS_OR_EQUAL(2)
                BEGIN_SIBLING_EVENT_GET_ID(LW(15))
                    SET_VALUE(LW(7), LW(13))
                    IF_INT_GREATER_OR_EQUAL(LW(7), 0)
                        SET_VALUE(LW(8), 75)
                    ELSE
                        SET_VALUE(LW(8), -75)
                    END_IF
                    CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(0), EVT_NULLPTR)
                    IF_INT_GREATER_OR_EQUAL(LW(0), 180)
                        SUBTRACT(LW(0), 360)
                    END_IF
                    CALL(evt_sub_intpl_init, EASING_CUBIC_OUT, LW(0), LW(8), 14)
                    LOOP(14)
                        CALL(evt_sub_intpl_get_value)
                        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                        WAIT_FRAMES(1)
                    END_LOOP
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MAX_SLAPS, LW(0))
                    IF_INT_LESS(LW(10), LW(0))
                        SET_VALUE(LW(7), LW(13))
                        IF_INT_GREATER_OR_EQUAL(LW(7), 0)
                            SET_VALUE(LW(8), 75)
                            SET_VALUE(LW(9), 75)
                        ELSE
                            SET_VALUE(LW(8), -75)
                            SET_VALUE(LW(9), -75)
                        END_IF
                        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(9), LW(8), 6)
                        LOOP(6)
                            CALL(evt_sub_intpl_get_value)
                            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                            WAIT_FRAMES(1)
                        END_LOOP
                    ELSE
                        SET_VALUE(LW(7), LW(13))
                        IF_INT_GREATER_OR_EQUAL(LW(7), 0)
                            SET_VALUE(LW(8), 0)
                            SET_VALUE(LW(9), 75)
                        ELSE
                            SET_VALUE(LW(8), 0)
                            SET_VALUE(LW(9), -75)
                        END_IF
                        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, LW(9), LW(8), 12)
                        LOOP(12)
                            CALL(evt_sub_intpl_get_value)
                            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                            WAIT_FRAMES(1)
                        END_LOOP
                    END_IF
                END_SIBLING_EVENT
                CALL(btlevtcmd_WaitEventEnd, LW(15))
                CASE_DEFAULT
                BEGIN_SIBLING_EVENT_GET_ID(LW(15))
                    CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(8), EVT_NULLPTR)
                    SET_VALUE(LW(7), 360)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MAX_SLAPS, LW(0))
                    IF_INT_GREATER_OR_EQUAL(LW(10), LW(0))
                        SET_VALUE(LW(0), LW(8))
                        IF_INT_GREATER_OR_EQUAL(LW(8), 180)
                            SUBTRACT(LW(0), 360)
                        END_IF
                        IF_INT_GREATER_OR_EQUAL(LW(13), 0)
                            SUBTRACT(LW(7), LW(0))
                        ELSE
                            ADD(LW(7), LW(0))
                        END_IF
                    ELSE
                        ADD(LW(7), 60)
                        SET_VALUE(LW(0), LW(8))
                        IF_INT_GREATER_OR_EQUAL(LW(8), 180)
                            SUBTRACT(LW(0), 360)
                        END_IF
                        IF_INT_GREATER_OR_EQUAL(LW(13), 0)
                            SUBTRACT(LW(7), LW(0))
                        ELSE
                            ADD(LW(7), LW(0))
                        END_IF
                    END_IF
                    IF_INT_LESS(LW(13), 0)
                        MULTIPLY(LW(7), -1)
                    END_IF
                    CALL(evt_sub_intpl_init, EASING_CUBIC_OUT, 0, LW(7), 20)
                    LOOP(20)
                        CALL(evt_sub_intpl_get_value)
                        ADD(LW(0), LW(8))
                        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                        WAIT_FRAMES(1)
                    END_LOOP
                END_SIBLING_EVENT
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SPIN_EVENT_ID, LW(15))
                WAIT_FRAMES(10)
            END_SWITCH
        ELSE
            // Failed command: she spins twice, staggers dizzily and the move ends.
            BEGIN_SIBLING_EVENT_GET_ID(LW(15))
                CALL(btlevtcmd_GetRotate, UNIT_ID_SELF, EVT_NULLPTR, LW(8), EVT_NULLPTR)
                SET_VALUE(LW(7), 720)
                IF_INT_GREATER_OR_EQUAL(LW(10), 1)
                    SET_VALUE(LW(0), LW(8))
                    IF_INT_GREATER_OR_EQUAL(LW(8), 180)
                        SUBTRACT(LW(0), 360)
                    END_IF
                    SUBTRACT(LW(7), LW(0))
                END_IF
                IF_INT_LESS(LW(13), 0)
                    MULTIPLY(LW(7), -1)
                END_IF
                CALL(evt_sub_intpl_init, EASING_LINEAR, 0, LW(7), 60)
                LOOP(60)
                    CALL(evt_sub_intpl_get_value)
                    ADD(LW(0), LW(8))
                    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, LW(0), 0)
                    WAIT_FRAMES(1)
                END_LOOP
            END_SIBLING_EVENT
            CALL(btlevtcmd_WaitEventEnd, LW(15))
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_X_1"))
            WAIT_FRAMES(60)
            CALL(btlevtcmd_StopAC)
            CALL(evt_audience_ap_recovery)
            CALL(btlevtcmd_InviteApInfoReport)
            GOTO(95)
        END_IF
        // Next slap from the other side while slaps remain.
        MULTIPLY(LW(13), -1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MAX_SLAPS, LW(0))
        IF_INT_LESS(LW(10), LW(0))
            GOTO(10)
        END_IF
        // After the last slap: audience AP and a pleased pose.
        LABEL(90)
        CALL(btlevtcmd_StopAC)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A1_3"))
        WAIT_FRAMES(40)
        // She runs home; after a good command a stylish window near the end of the run lets her hop with a pose.
        LABEL(95)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(0), LW(2), FACE_DIR_TOWARD_X, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(14))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(14), MOVE_SOUND_FROM_TABLE, 0)
        END_SIBLING_EVENT
        SET_VALUE(LW(11), LW(14))
        SUBTRACT(LW(11), 11)
        SUBTRACT(LW(14), 1)
        IF_INT_GREATER(LW(14), LW(11))
            SET_VALUE(LW(13), LW(14))
            SUBTRACT(LW(13), 10)
            IF_BITS_SET(LW(6), AC_RESULT_SUCCESS)
                CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, LW(13), LW(14), 0)
                CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
                IF_INT_GREATER_OR_EQUAL(LW(6), 2)
                    DELETE_EVENT(LW(15))
                    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.5f))
                    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 30)
                    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_O_1"))
                    CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
                    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
                    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
                    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
                    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
                ELSE
                    CALL(btlevtcmd_WaitEventEnd, LW(15))
                END_IF
            ELSE
                CALL(btlevtcmd_WaitEventEnd, LW(15))
            END_IF
        ELSE
            CALL(btlevtcmd_WaitEventEnd, LW(15))
        END_IF
        // End of the move: face forward, apply the HP and FP drain badges, back to idle.
        LABEL(99)
        CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
        CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
EVT_END()

/**
 * Ms. Mowz's Tease: she teases the enemies, and the action command result sets the dizzy chance of a copy of the
 * weapon (_make_madowase_weapon) that is then applied to each target; misses make the target dodge.
 */
/**
 * Ms. Mowz's Tease: she teases the enemies, and the action command result sets the dizzy chance of a copy of the
 * weapon (_make_madowase_weapon) that is then applied to each target; misses make the target dodge.
 */
#if VERSION <= VERSION_DEMO_US
#if VERSION <= VERSION_DEMO_US
/**
 * Kiss Thief's spawned helper: runs mono_main, which draws the captured greyscale screen every frame;
 * the move deletes this event when the effect ends.
 */
static EVT_BEGIN(main_evt)
    CALL(mono_main)
    RETURN
EVT_END()
#endif
/**
 * Ms. Mowz's Tease: she teases the enemies, and the action command result sets the dizzy chance of a copy of the
 * weapon (_make_madowase_weapon) that is then applied to each target; misses make the target dodge.
 */
#endif
EVT_BEGIN(partyChuchurinaAttack_MadowaseAttack)
    // Declare the move on all enemies and pay for it; with no target, skip to the end.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    // Set up the hold-and-release gauge command from the difficulty.
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_NO_FULL_BONUS | AC_REPEATEDLY_LV_FLAG_FAST_GAUGE_DISP | AC_REPEATEDLY_LV_FLAG_HOLD_OVERHEAT)
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 3)
        SET_VALUE(LW(1), 50)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 4)
        SET_VALUE(LW(1), 80)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 5)
        SET_VALUE(LW(1), 100)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 7)
        SET_VALUE(LW(1), 100)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 8)
        SET_VALUE(LW(1), 110)
        CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_NO_FULL_BONUS | AC_REPEATEDLY_LV_FLAG_INSTANT_GAUGE_DISP | AC_REPEATEDLY_LV_FLAG_HOLD_OVERHEAT)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 10)
        SET_VALUE(LW(1), 120)
        CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_NO_FULL_BONUS | AC_REPEATEDLY_LV_FLAG_INSTANT_GAUGE_DISP | AC_REPEATEDLY_LV_FLAG_HOLD_OVERHEAT)
        CASE_DEFAULT
        SET_VALUE(LW(0), 12)
        SET_VALUE(LW(1), 130)
#if VERSION <= VERSION_DEMO_US
        CALL(btlevtcmd_AcSetFlag, 49)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 1, 240, 178, LW(0), LW(1), 100, 1, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetGaugeParam, 100, 100, 100, 100)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 6, 1, 0)
    // Camera pulls back over her and the gauge command runs.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, 0, 3)
#else
        CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_NO_FULL_BONUS | AC_REPEATEDLY_LV_FLAG_INSTANT_GAUGE_DISP | AC_REPEATEDLY_LV_FLAG_HOLD_OVERHEAT)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 1, 240, 178, LW(0), LW(1), 100, 1, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetGaugeParam, 34, 68, 100, 100)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 6, 1, 0)
    // Camera pulls back over her and the gauge command runs.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
#endif
    CALL(evt_btl_camera_set_moveto, 1, LW(0), 73, 400, LW(0), 43, 0, 40, 5)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_StopAC)
    // She spins on the spot.
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_I_1"))
    LOOP(40)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 36, 0)
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    // Footsteps for her and her three copies switch to the teasing sounds.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A4_1"))
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetDiveSound, UNIT_ID_SELF, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_BTL_CHURINA_CONFUSE_JUMP1"), STRING("SFX_BTL_CHURINA_CONFUSE_LANDING1"))
    CALL(btlevtcmd_SetPartsWalkSound, UNIT_ID_SELF, 2, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetPartsRunSound, UNIT_ID_SELF, 2, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsDiveSound, UNIT_ID_SELF, 2, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsJumpSound, UNIT_ID_SELF, 2, STRING("SFX_BTL_CHURINA_CONFUSE_JUMP1"), STRING("SFX_BTL_CHURINA_CONFUSE_LANDING1"))
    CALL(btlevtcmd_SetPartsWalkSound, UNIT_ID_SELF, 3, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetPartsRunSound, UNIT_ID_SELF, 3, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsDiveSound, UNIT_ID_SELF, 3, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsJumpSound, UNIT_ID_SELF, 3, STRING("SFX_BTL_CHURINA_CONFUSE_JUMP1"), STRING("SFX_BTL_CHURINA_CONFUSE_LANDING1"))
    CALL(btlevtcmd_SetPartsWalkSound, UNIT_ID_SELF, 4, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetPartsRunSound, UNIT_ID_SELF, 4, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsDiveSound, UNIT_ID_SELF, 4, STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1L"), STRING("SFX_BTL_CHURINA_CONFUSE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetPartsJumpSound, UNIT_ID_SELF, 4, STRING("SFX_BTL_CHURINA_CONFUSE_JUMP1"), STRING("SFX_BTL_CHURINA_CONFUSE_LANDING1"))
    // Three copies of her pop out of puffs of smoke around her and drop to the floor.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    SUBTRACT(LW(2), 20)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 180, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("PCH_A3_1"))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden | kNoShadow)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CHURINA_CONFUSE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 2, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FallPartsPosition, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2), 25)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 2, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 2, POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    SUBTRACT(LW(2), 10)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 10)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 3, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("PCH_A3_2"))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden | kNoShadow)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 3, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FallPartsPosition, UNIT_ID_SELF, 3, LW(0), LW(1), LW(2), 25)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 3, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 3, POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    ADD(LW(2), 10)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), -10)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 4, 0, 180, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("PCH_A3_3"))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kHidden | kNoShadow)
    CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(30)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 4, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FallPartsPosition, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2), 25)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 4, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 4, POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    // All four of them blur, and the camera pans forward with them.
    WAIT_FRAMES(85)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART1_RUNNING, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART2_RUNNING, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART3_RUNNING, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART4_RUNNING, 1)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 1, 255, 255, 255, 255, 100, 100, 100, 50, 1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 2, 255, 255, 255, 255, 100, 100, 100, 50, 1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kMotionBlur)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 3, 255, 255, 255, 255, 100, 100, 100, 50, 1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kMotionBlur)
    CALL(btlevtcmd_SetPartsBlur, UNIT_ID_SELF, 4, 255, 255, 255, 255, 100, 100, 100, 50, 1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 4, kMotionBlur)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(5))
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(3.0f), LW(7))
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 90, 500, LW(0), 60, 0, LW(7), 0)
    END_SIBLING_EVENT
    // She dashes forward and zigzags back and forth, then vanishes in a puff.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(5))
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), 40)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 60)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 180, 0)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        ADD(LW(0), 40)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 60)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 180, 0)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        ADD(LW(0), 40)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kMotionBlur)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART1_RUNNING, 0)
    END_SIBLING_EVENT
    // Each copy dashes forward and zigzags in its own pattern, then vanishes in a puff.
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(6), 2)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, LW(6), FLOAT(0.69921875f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(5))
        CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, LW(6), FLOAT(3.0f))
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 0, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2))
        ADD(LW(0), 60)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 90)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
        ADD(LW(0), 60)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 90)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
        ADD(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
        CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden | kNoShadow)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kMotionBlur)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART2_RUNNING, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(6), 3)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, LW(6), FLOAT(0.69921875f))
        WAIT_FRAMES(20)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(5))
        CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, LW(6), FLOAT(3.0f))
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 0, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 80)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
        ADD(LW(0), 160)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 40, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 40)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 15, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 60)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        ADD(LW(0), 20)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden | kNoShadow)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kMotionBlur)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART3_RUNNING, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_VALUE(LW(6), 4)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, LW(6), FLOAT(0.69921875f))
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(6), POSE_TYPE_RUN)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetStageSize, LW(5), EVT_NULLPTR, EVT_NULLPTR)
        DIVIDE(LW(5), 3)
        SET_VALUE(LW(0), 0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(5))
        CALL(btlevtcmd_SetPartsMoveSpeed, UNIT_ID_SELF, LW(6), FLOAT(3.0f))
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 0, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2))
        ADD(LW(0), 30)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        ADD(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        ADD(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 0, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        SUBTRACT(LW(0), 30)
        CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, LW(6), 0, 180, 0)
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(6), LW(0), LW(1), LW(2), 10, MOVE_SOUND_FROM_TABLE)
        CALL(evt_eff, STRING(""), STRING("bomb"), 0, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 4, kHidden | kNoShadow)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kMotionBlur)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART4_RUNNING, 0)
    END_SIBLING_EVENT
    // Wait until all four have finished.
    LABEL(9)
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART1_RUNNING, LW(1))
    ADD(LW(0), LW(1))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART2_RUNNING, LW(1))
    ADD(LW(0), LW(1))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART3_RUNNING, LW(1))
    ADD(LW(0), LW(1))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_PART4_RUNNING, LW(1))
    ADD(LW(0), LW(1))
    IF_INT_NOT_EQUAL(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(9)
    END_IF
    // For each enemy: if it can be reached, apply a copy of the weapon with the dizzy chance from the gauge;
    // otherwise its dodge plays.
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CHURINA_CONFUSE1"), 0)
    SET_VALUE(LW(10), 0)
    SET_VALUE(LW(15), 0)
    LABEL(10)
    CALL(btlevtcmd_CommandPreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), DAMAGE_RESULT_FINAL_HIT, LW(6))
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
        GOTO(50)
    END_IF
    CALL(btlevtcmd_AcGetOutputParam, 1, LW(0))
    CALL(_make_madowase_weapon, LW(12), LW(0))
    CALL(btlevtcmd_GetResultAC, LW(0))
    IF_BITS_SET(LW(0), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_AC_SUCCESS, LW(5))
        CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        IF_INT_GREATER_OR_EQUAL(LW(6), 0)
            CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
            ADD(LW(15), 1)
            CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
        END_IF
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    END_IF
    // Next enemy.
    LABEL(50)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    // Nothing was hit: the audience is let down.
    IF_INT_EQUAL(LW(15), 0)
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    // Camera back to normal; she leaps back spinning, with a stylish window during the leap.
    LABEL(80)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_B_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 80)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.14941406f))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CHURINA_CONFUSE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 5, 0)
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
        MULTIPLY(LW(5), 9)
        LOOP(40)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(5))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
        MULTIPLY(LW(5), 36)
        LOOP(20)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, LW(5))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
#if VERSION >= VERSION_US
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 40, 59, 59, 0)
#else
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 45, 59, 59, 0)
#endif
    END_SIBLING_EVENT
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    // Stylish hit: a cheerful pose and she dashes home; otherwise (before JP, a dizzy pose first) she runs home.
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        WAIT_MS(300)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_US
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, 2, 0, -1)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CASE_DEFAULT
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 0, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_D_3"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        WAIT_MS(300)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(evt_btl_camera_set_mode, 0, 0)
#else
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CASE_DEFAULT
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(evt_audience_acrobat_notry)
        CALL(evt_audience_ap_recovery)
        CALL(btlevtcmd_InviteApInfoReport)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
#endif
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_R_2"))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_SWITCH
    // End of the move: apply the HP and FP drain badges and restore her normal footstep sounds.
    LABEL(99)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL_CHILD(btldefaultevt_SuitoruBadgeEffect)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 15, 13)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetDiveSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_MOVE1L"), STRING("SFX_PARTY_BATTLE_MOVE1R"), 0, 7, 7)
    CALL(btlevtcmd_SetJumpSound, UNIT_ID_SELF, STRING("SFX_PARTY_BATTLE_JUMP1"), STRING("SFX_PARTY_BATTLE_LANDING1"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(partyChuchurinaAttack_ItemSteal)
    // Declare the move and pay for it; with no target, skip to the end.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    // Against an enemy with pre-emptive front spikes (pre-check result 5) she just runs up beside it (US and
    // later with a stylish window on the way) and the damage check runs there without the action command.
    SET_VALUE(LW(14), 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_ChuchurinaItemSteal), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), 5)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_GetWidth, LW(3), LW(15))
        DIVIDE(LW(15), 2)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(15))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
#if VERSION >= VERSION_US
        CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(8))
        BEGIN_SIBLING_EVENT_GET_ID(LW(9))
            CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), MOVE_SOUND_FROM_TABLE, 0)
        END_SIBLING_EVENT
        IF_INT_GREATER(LW(8), 5)
            SUBTRACT(LW(8), 2)
            SET_VALUE(LW(15), LW(8))
            DIVIDE(LW(15), 3)
            SET_VALUE(LW(8), LW(15))
            MULTIPLY(LW(8), 2)
            CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(15), LW(8), LW(8), 0)
            CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
            SWITCH(LW(6))
                CASE_GREATER_OR_EQUAL(2)
                DELETE_EVENT(LW(9))
                CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_Y_1"))
                CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
                CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
                CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
                CASE_DEFAULT
                CALL(evt_audience_acrobat_notry)
            END_SWITCH
        ELSE
            CALL(btlevtcmd_WaitEventEnd, LW(9))
        END_IF
#else
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
#endif
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_ChuchurinaItemSteal), DAMAGE_RESULT_FINAL_HIT, LW(5))
        RETURN
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
    CALL(btlevtcmd_GetWidth, LW(3), LW(15))
    MULTIPLY(LW(15), 2)
    DIVIDE(LW(15), 3)
    ADD(LW(15), 10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(15))
    ADD(LW(2), 8)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
#if VERSION >= VERSION_US
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, LW(8))
    BEGIN_SIBLING_EVENT_GET_ID(LW(9))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(8), MOVE_SOUND_FROM_TABLE, 0)
    END_SIBLING_EVENT
    IF_INT_GREATER(LW(8), 5)
        SUBTRACT(LW(8), 2)
        SET_VALUE(LW(15), LW(8))
        DIVIDE(LW(15), 3)
        SET_VALUE(LW(8), LW(15))
        MULTIPLY(LW(8), 2)
        CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, LW(15), LW(8), LW(8), 0)
        CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
        SWITCH(LW(6))
            CASE_GREATER_OR_EQUAL(2)
            DELETE_EVENT(LW(9))
            CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_Y_1"))
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
            CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
            CASE_DEFAULT
            CALL(evt_audience_acrobat_notry)
            CALL(btlevtcmd_WaitEventEnd, LW(9))
        END_SWITCH
    ELSE
        CALL(btlevtcmd_WaitEventEnd, LW(9))
    END_IF
#else
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
#endif
    // Camera closes in on her as she puckers up.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, 1)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    WAIT_MS(200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_K_1"))
    WAIT_FRAMES(6)
    // She cannot reach it: the enemy's dodge plays and the move ends.
    IF_INT_NOT_EQUAL(LW(5), 1)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        SWITCH(LW(5))
            CASE_OR_EQUAL(4)
            CASE_END
            CASE_EQUAL(3)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
            CASE_EQUAL(6)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
            CASE_EQUAL(2)
            CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
            CASE_EQUAL(1)
            CASE_DEFAULT
        END_SWITCH
        WAIT_FRAMES(30)
        CALL(btlevtcmd_StopAC)
        GOTO(90)
    END_IF
    // She blows a kiss at the enemy, whose held item is shown above it.
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&partyWeapon_ChuchurinaItemSteal), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(15))
    MULTIPLY(LW(15), 2)
    DIVIDE(LW(15), 3)
    ADD(LW(1), LW(15))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 20)
    CALL(evt_eff, STRING(""), STRING("kiss"), 0, LW(0), LW(1), LW(2), 1, 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CHURINA_KISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OnUnitFlag, LW(3), BATTLE_UNIT_FLAG_SHOW_ITEM_WHILE_ACTING)
    WAIT_FRAMES(20)
    // She closes her eyes, the screen turns grey around her and the stop-the-gauge command runs, its speed and
    // success zone set from the difficulty and the enemy's item.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_Z_1"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_SELF, 1, PART_ANIM_NO_DISP, 1)
    WAIT_FRAMES(1)
    CALL(mono_capture_event)
    SPAWN_EVENT_GET_ID(PTR(main_evt), LW(13))
    BEGIN_INLINE_EVENT
        CALL(mono_on)
    END_INLINE_EVENT
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(_get_itemsteal_param, LW(3), LW(0), LW(1))
    CALL(btlevtcmd_AcSetParamAll, 1, LW(0), 2, 2, LW(1), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetFlag, 0)
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 15, 1, 0)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_GetResultAC, LW(6))
    // Success: she strikes a pose; then she leaps onto the enemy.
    IF_BITS_SET(LW(6), AC_RESULT_SUCCESS)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A4_1"))
    END_IF
    WAIT_FRAMES(35)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CHURINA_EYE1"), 0)
    WAIT_FRAMES(47)
    CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetWidth, LW(3), LW(15))
    DIVIDE(LW(15), 2)
    ADD(LW(0), LW(15))
    CALL(btlevtcmd_GetHeight, LW(3), LW(15))
    ADD(LW(1), LW(15))
    ADD(LW(1), 5)
    ADD(LW(2), 8)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.69921875f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 16, MOVE_SOUND_FROM_TABLE)
    // Try the steal. Nothing taken: she falls off, the colour returns and she runs home.
    CALL(btlevtcmd_GetResultAC, LW(6))
    CALL(_chuchu_item_steal, LW(3), LW(14), LW(6))
    IF_INT_EQUAL(LW(14), 0)
        CALL(btlevtcmd_JumpContinue, UNIT_ID_SELF)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_F_1"))
        WAIT_FRAMES(22)
        BEGIN_INLINE_EVENT
            CALL(mono_off)
            DELETE_EVENT(LW(13))
        END_INLINE_EVENT
        WAIT_FRAMES(20)
        CALL(btlevtcmd_OffUnitFlag, LW(3), BATTLE_UNIT_FLAG_SHOW_ITEM_WHILE_ACTING)
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_R_2"))
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        GOTO(99)
    END_IF
// Stolen: the rating effect and a heart sound, the colour returns and she flips back down off the enemy.
#if VERSION >= VERSION_US
    CALL(btlevtcmd_GetResultPrizeLv, 0, 0, LW(6))
#else
    CALL(btlevtcmd_GetResultPrizeLv, LW(3), 0, LW(6))
#endif
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CHURINA_HEARTCATCH1"), 0)
    CALL(btlevtcmd_OffUnitFlag, LW(3), BATTLE_UNIT_FLAG_SHOW_ITEM_WHILE_ACTING)
    BEGIN_INLINE_EVENT
        CALL(mono_off)
        DELETE_EVENT(LW(13))
    END_INLINE_EVENT
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
    CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(0.2998047f))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A3_1"))
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 10, 0)
        LOOP(18)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 0, 20)
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        CALL(btlevtcmd_SetRotateOffset, UNIT_ID_SELF, 0, 0, 0)
    END_INLINE_EVENT
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    // Camera back to normal; with a stolen item she carries its icon home over her head,
    // and the item message shows.
    LABEL(90)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    IF_INT_NOT_EQUAL(LW(14), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
        ADD(LW(1), LW(5))
        ADD(LW(1), 5)
        CALL(btlevtcmd_BtlIconEntryItemId, LW(14), LW(0), LW(1), LW(2), LW(8))
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
            CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
            CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 180, 0)
            CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
        END_SIBLING_EVENT
        LABEL(91)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHeight, UNIT_ID_SELF, LW(5))
        ADD(LW(1), LW(5))
        ADD(LW(1), 5)
        CALL(btlevtcmd_BtlIconSetPosition, LW(8), LW(0), LW(1), LW(2))
        IS_EVENT_RUNNING(LW(15), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            WAIT_FRAMES(1)
            GOTO(91)
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A2_2"))
        WAIT_FRAMES(4)
        CALL(btlevtcmd_AnnounceSetParam, 0, LW(14))
        CALL(btlevtcmd_AnnounceMessage, 1, 0, 0, STRING("btl_msg_steal_item_get"), 90)
        CALL(btlevtcmd_BtlIconDelete, LW(8))
    ELSE
        CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(3.0f))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 180, 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    END_IF
    // End of the move: audience AP, back to idle.
    LABEL(99)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Ms. Mowz's Smooch: she kisses Mario, healing him by an amount the action command builds up,
 * with the recovery effect, then returns home.
 */
/**
 * Ms. Mowz's Smooch: she kisses Mario, healing him by an amount the action command builds up,
 * with the recovery effect, then returns home.
 */
EVT_BEGIN(partyChuchurinaAttack_Kiss)
    // Find the chosen ally and pay for the move; with no target, skip to the end.
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_CommandPayWeaponCost, UNIT_ID_SELF)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_btl_camera_set_zoom, 0, 250)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
#if VERSION <= VERSION_DEMO_US
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
#else
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    END_IF
    CALL_CHILD(subsetevt_check_target_movable)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CalculateFaceDirection, LW(3), LW(4), UNIT_ID_SELF, -1, FACE_DIR_TOWARD_UNIT, LW(15))
        CALL(btlevtcmd_ChangeFaceDirection, LW(3), LW(15))
#endif
    END_IF
    // Set up the alternate-A-and-B gauge command from the difficulty.
    CALL(btlevtcmd_CommandGetWeaponActionLv, LW(0))
    CALL(btlevtcmd_AcSetDifficulty, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AcGetDifficulty, LW(0))
    SUBTRACT(LW(0), 3)
    SWITCH(LW(0))
        CASE_LESS_OR_EQUAL(-3)
        SET_VALUE(LW(0), 300)
        SET_VALUE(LW(1), 20)
        SET_VALUE(LW(2), 20)
        CASE_EQUAL(-2)
        SET_VALUE(LW(0), 270)
        SET_VALUE(LW(1), 18)
        SET_VALUE(LW(2), 22)
        CASE_EQUAL(-1)
        SET_VALUE(LW(0), 240)
        SET_VALUE(LW(1), 17)
        SET_VALUE(LW(2), 24)
        CASE_EQUAL(0)
        SET_VALUE(LW(0), 210)
        SET_VALUE(LW(1), 16)
        SET_VALUE(LW(2), 25)
        CASE_EQUAL(1)
        SET_VALUE(LW(0), 195)
        SET_VALUE(LW(1), 14)
        SET_VALUE(LW(2), 26)
        CASE_EQUAL(2)
        SET_VALUE(LW(0), 180)
        SET_VALUE(LW(1), 13)
        SET_VALUE(LW(2), 27)
        CASE_DEFAULT
        SET_VALUE(LW(0), 150)
        SET_VALUE(LW(1), 12)
        SET_VALUE(LW(2), 30)
    END_SWITCH
    CALL(btlevtcmd_AcSetParamAll, 11, LW(0), 178, LW(1), LW(2), 12, 37, EVT_NULLPTR)
    CALL(btlevtcmd_AcSetGaugeParam, 36, 72, 100, 100)
    CALL(btlevtcmd_AcSetFlag, AC_REPEATEDLY_LV_FLAG_MIN_LEVEL_1)
    // The command runs while she walks up to the ally.
    CALL(btlevtcmd_SetupAC, UNIT_ID_SELF, 6, 1, 0)
    WAIT_FRAMES(22)
    CALL(btlevtcmd_StartAC, 1)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
#if VERSION <= VERSION_DEMO_US
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 20)
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 235, -1, 0)
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), 16, LW(15))
#else
        CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(7))
        MULTIPLY_FLOAT(LW(6), 12)
        MULTIPLY_FLOAT(LW(7), 7)
        CALL(btlevtcmd_GetPos, LW(3), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(6))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), LW(7))
        CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 235, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
#endif
        CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    END_SIBLING_EVENT
    // Rating from the gauge level, which is also the HP she restores.
    CALL(btlevtcmd_ResultAC)
    CALL(btlevtcmd_AcGetOutputParam, 2, LW(5))
    SWITCH(LW(5))
        CASE_GREATER_OR_EQUAL(10)
        SET_VALUE(LW(5), 2)
        CASE_GREATER_OR_EQUAL(7)
        SET_VALUE(LW(5), 1)
        CASE_GREATER_OR_EQUAL(4)
        SET_VALUE(LW(5), 0)
        CASE_DEFAULT
        SET_VALUE(LW(5), -1)
    END_SWITCH
    IF_INT_GREATER_OR_EQUAL(LW(5), 0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetResultPrizeLv, UNIT_ID_SYSTEM, LW(5), LW(6))
        CALL(btlevtcmd_ACSuccessEffect, LW(6), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), LW(6))
    ELSE
        CALL(btlevtcmd_AudienceDeclareACResult, LW(12), -1)
    END_IF
    CALL(btlevtcmd_GetResultAC, LW(6))
    CALL(btlevtcmd_StopAC)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 350)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_K_1"))
    WAIT_FRAMES(6)
#if VERSION <= VERSION_DEMO_US
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 25)
    ADD(LW(1), 40)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(6))
    MULTIPLY(LW(6), -45)
    CALL(evt_eff, STRING(""), STRING("kiss"), 0, LW(0), LW(1), LW(2), LW(6), 0, 0, 0, 0, 0, 0, 0)
#else
    IF_INT_GREATER_OR_EQUAL(LW(5), 0)
        CALL_CHILD(subsetevt_check_target_movable)
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePoseType, LW(3), LW(4), POSE_TYPE_KISSED_CHUCHURINA)
        END_IF
    END_IF
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
    MULTIPLY_FLOAT(LW(6), 16)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), LW(6))
    CALL(btlevtcmd_GetStatusMg, UNIT_ID_SELF, LW(6))
    MULTIPLY_FLOAT(LW(6), 40)
    ADD(LW(1), LW(6))
    CALL(btlevtcmd_GetFaceDirection, LW(3), LW(6))
    MULTIPLY(LW(6), 45)
    CALL(evt_eff, 0, STRING("kiss"), 0, LW(0), LW(1), LW(2), LW(6), 0, 0, 0, 0, 0, 0, 0)
#endif
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CHURINA_KISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    // Hearts rise from the ally, and two seconds later it regains HP by the gauge level.
    WAIT_FRAMES(40)
    CALL(btlevtcmd_GetPartsPos, LW(3), LW(4), LW(12), LW(13), LW(14))
    CALL(btlevtcmd_GetHeight, LW(3), LW(5))
    CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    ADD(LW(13), LW(5))
    CALL(btlevtcmd_AcGetOutputParam, 2, LW(5))
    CALL(evt_eff, STRING("eff"), STRING("recovery"), 0, LW(12), LW(13), LW(14), LW(5), 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        WAIT_FRAMES(120)
        CALL(btlevtcmd_RecoverHp, LW(3), LW(4), LW(5))
    END_INLINE_EVENT
    // Stylish window after the kiss.
    CALL(btlevtcmd_ACRStart, UNIT_ID_SELF, 0, 15, 15, 20)
    CALL(btlevtcmd_ACRGetResult, LW(6), LW(7))
    SWITCH(LW(6))
        CASE_GREATER_OR_EQUAL(2)
        CALL(btlevtcmd_CommandGetWeaponAddress, UNIT_ID_SELF, LW(12))
        CALL(btlevtcmd_AudienceDeclareAcrobatResult, LW(12), 1, 0, 0, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_Y_1"))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        WAIT_MS(300)
        CASE_DEFAULT
        CALL(evt_audience_acrobat_notry)
    END_SWITCH
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 1)
    CALL(evt_audience_ap_recovery)
    CALL(btlevtcmd_InviteApInfoReport)
#if VERSION > VERSION_DEMO_US
    CALL_CHILD(subsetevt_check_target_movable)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_ResetFaceDirection, LW(3))
        CALL(btlevtcmd_StartWaitEvent, LW(3))
    END_IF
#endif
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_ResetFaceDirection, UNIT_ID_SELF)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Ms. Mowz's Appeal (data tag 5): appeal sound and pose, the audience is fired up,
 * then she blows a kiss towards it.
 */
EVT_BEGIN(chuchurina_appeal_event)
    CALL(btlevtcmd_RunDataEventChild, UNIT_ID_SELF, UNIT_DATA_ATTACK_READY_EVENT)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_APPEAL1"), LW(0), LW(1), LW(2), 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_O_3"))
    CALL(btlevtcmd_AudienceAppeal, UNIT_ID_SELF)
    CALL(btlevtcmd_ftomsec, 45, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 15)
    ADD(LW(1), 30)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(14))
    MULTIPLY(LW(14), 45)
    CALL(evt_eff, STRING(""), STRING("kiss"), 0, LW(0), LW(1), LW(2), LW(14), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_CHURINA_KISS1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Victory reaction (data tag 64): plays Ms. Mowz's PCH_Y_1 pose to the end, holds a third of a second,
 * then her stay pose.
 */
static EVT_BEGIN(party_win_reaction)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_Y_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    WAIT_MS(333)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
    RETURN
EVT_END()

/**
 * Audience-attack reaction (data tag 6): Ms. Mowz runs offstage, reappears in the audience,
 * strikes her Tease pose (PCH_A4_1) and then PCH_Y_1 to knock out the chosen audience member, and runs home.
 */
static EVT_BEGIN(attack_audience_event)
    // She runs off the edge of the stage.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(3), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(4))
    SET_VALUE(LW(8), LW(3))
    MULTIPLY(LW(8), LW(4))
    MULTIPLY(LW(8), -1)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(6.0f))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(8), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    WAIT_FRAMES(20)
    // She reappears at the edge of the audience and the camera follows her to the chosen audience member.
    CALL(_get_attack_aud_no, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, LW(4), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(5))
    SET_VALUE(LW(6), LW(4))
    MULTIPLY(LW(6), LW(5))
    MULTIPLY(LW(6), -1)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(6), LW(1), LW(2))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_AUDIENCE)
    CALL(evt_btl_camera_set_homing_unit_audience, 0, UNIT_ID_SELF, 1, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 100)
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 24)
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    // She strikes her Tease pose, then a cheer, and the audience member is knocked out; the camera returns later.
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_A4_1"))
    CALL(btlevtcmd_ftof, 52, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("PCH_Y_1"))
    CALL(btlevtcmd_ftof, 7, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(_attack_aud, LW(3), 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        WAIT_MS(1500)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        WAIT_MS(1000)
    END_SIBLING_EVENT
    // She runs out of the audience, re-enters from the other side of the stage and walks home.
    CALL(btlevtcmd_ftof, 35, LW(0))
    WAIT_FRAMES(LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(4.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(6), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(8), LW(1), LW(2))
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
#if VERSION >= VERSION_US
EVT_END_PAD()
#else
EVT_END()
#endif

/** Ms. Mowz's defence: 0 against every element. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Ms. Mowz's element reactions: none special, every element hits her normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Ms. Mowz's status vulnerabilities: base rate (100) for every status. */
#if VERSION > VERSION_DEMO_US
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

/** Empty: partner moves come from the player's menu choices, not a unit attack script. */
static EVT_BEGIN(attack_event)
    RETURN
EVT_END()

/** Idle: plays the stay pose from the current pose table. */
#if VERSION > VERSION_DEMO_US
/** Idle: plays the stay pose from the current pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
#endif

/**
 * Kiss Thief's spawned helper: runs mono_main, which draws the captured greyscale screen every frame;
 * the move deletes this event when the effect ends.
 */
#if VERSION > VERSION_DEMO_US
/**
 * Kiss Thief's spawned helper: runs mono_main, which draws the captured greyscale screen every frame;
 * the move deletes this event when the effect ends.
 */
static EVT_BEGIN(main_evt)
    CALL(mono_main)
    RETURN
EVT_END()
#endif

s32 mono_alpha;

/**
 * Allocates a spare BattleWeapon in the running unit's countersetWeapon slot,
 * which Tease later fills with its per-use copy (_make_madowase_weapon). Run from init_event.
 */
USER_FUNC(_chuchu_make_extra_work_area) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(wp, id);
    unit->countersetWeapon = BattleAlloc(sizeof(BattleWeapon));
    return EVT_RETURN_DONE;
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
 * Builds Ms. Mowz's battle move list (data tag 1):
 * Love Slap and Kiss Thief always, Tease from Super rank and Smooch from Ultra rank,
 * appending each to command->weapon and advancing *count.
 */
static void __makeTechMenuFunc(BattleCommand* command, s32* count) {
    s32 techLv;

    techLv = partyGetTechLv(BattleTransPartyId(BattleGetPartyPtr(_battleWorkPointer)->currentKind));

    command->weapon[*count].index = -1;
    command->weapon[*count].itemId = 0;
    command->weapon[*count].weapon = &partyWeapon_ChuchurinaNormalAttackLeftLast;
    command->weapon[*count].enabled = 0;
    command->weapon[*count].icon = command->weapon[*count].weapon->icon;
    command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
    (*count)++;

    if (techLv >= 0) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_ChuchurinaItemSteal;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 1) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_ChuchurinaMadowaseAttack;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
    if (techLv >= 2) {
        command->weapon[*count].index = -1;
        command->weapon[*count].itemId = 0;
        command->weapon[*count].weapon = &partyWeapon_ChuchurinaKiss;
        command->weapon[*count].enabled = 0;
        command->weapon[*count].icon = command->weapon[*count].weapon->icon;
        command->weapon[*count].name = msgSearch(command->weapon[*count].weapon->name);
        (*count)++;
    }
}

/**
 * Returns in args[3]-args[5] where Love Slap should hit target unit args[1]:
 * its position plus its bintaHitOffset (scaled by its size, X mirrored by its facing).
 * args[0] and args[2] (the part) are read but not used.
 */
USER_FUNC(_get_binta_hit_position) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* target;
    Vec pos;
    s32 id, targetId, dir;
    s32 outZ, outX, outY;

    id = evtGetValue(event, args[0]);
    targetId = evtGetValue(event, args[1]);
    evtGetValue(event, args[2]);
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    BattleGetUnitPtr(wp, BattleTransID(event, id));
    target = BattleGetUnitPtr(wp, BattleTransID(event, targetId));
    dir = target->faceDirection;
    BtlUnit_GetPos(target, &pos.x, &pos.y, &pos.z);
    pos.x += dir * (target->bintaHitOffset.x * target->sizeMultiplier);
    pos.y += target->bintaHitOffset.y * target->sizeMultiplier;
    pos.z += target->bintaHitOffset.z * target->sizeMultiplier;
    evtSetFloat(event, outX, pos.x);
    evtSetFloat(event, outY, pos.y);
    evtSetFloat(event, outZ, pos.z);
    return EVT_RETURN_DONE;
}

/**
 * Copies the weapon pointed to by args[0] into the unit's spare weapon (countersetWeapon),
 * sets its dizzy chance to args[1] (the Tease action command output),
 * and writes the copy's address back into args[0].
 */
USER_FUNC(_make_madowase_weapon) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 index;
    BattleWeapon* weapon;
    void* src;
    s32 id, value;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(wp, id);
    index = args[0];
    weapon = unit->countersetWeapon;
    src = (void*)evtGetValue(event, index);
    value = evtGetValue(event, args[1]);
    memcpy(weapon, src, sizeof(BattleWeapon));
    weapon->dizzyChance = value;
    evtSetValue(event, index, (EvtWord)weapon);
    return EVT_RETURN_DONE;
}

/**
 * Kiss Thief action command parameters for target args[0]:
 * args[1] gets the success rate, 80 at normal difficulty and 20 lower per level above (minimum 5);
 * args[2] gets 100 minus the target's itemStealParam.
 */
USER_FUNC(_get_itemsteal_param) {
    BattleWork* wp;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id, out1, out2, rate, resist;

    id = evtGetValue(event, args[0]);
    out1 = args[1];
    wp = _battleWorkPointer;
    out2 = args[2];
    unit = BattleGetUnitPtr(wp, BattleTransID(event, id));
    rate = 80 - (wp->actionCommands.acDifficulty - 3) * 20;
    if (rate <= 5) {
        rate = 5;
    }
    resist = 100 - unit->kindParams->itemStealParam;
    evtSetValue(event, out1, rate);
    evtSetValue(event, out2, resist);
    return EVT_RETURN_DONE;
}

/**
 * Kiss Thief's steal from target args[0].
 * With action command result args[2] bit 2 set and room in the pouch,
 * takes its held item (or, if none, a weighted roll of its drop table, falling back to a coin),
 * clears the item from the field battle setup and unequips it if it was a badge.
 * Returns the item in args[1], or 0 when nothing is stolen.
 */
USER_FUNC(_chuchu_item_steal) {
    ItemDropData* table;
    BattleWork* wp;
    s32 flag;
    s32 out;
    BattleUnit* unit;
    s32 id;
    u16 item;
    EvtWord* args;

    args = event->args;
    wp = _battleWorkPointer;
    id = evtGetValue(event, args[0]);
    out = args[1];
    flag = evtGetValue(event, args[2]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, id));
    item = unit->heldItem;
    if (item == ITEM_NULL) {
        s32 total, r;
        u32 pick;
        // the result of this roll is not used
        if (unit->dropTable != NULL) {
            irand(100);
        }
        table = unit->dropTable;
        if (table == NULL) {
            pick = 0;
        } else {
            // FAKE: dead re-test (table is non-NULL in this branch); without it the object changes
            if (table != NULL) {
                s32 i;
                total = 0;
                for (i = 0; table[i].holdWeight > 0 || table[i].type != 0; i++) {
                    total += table[i].holdWeight;
                }
                if (total > 0) {
                    r = irand(total);
                    for (;; table++) {
                        r -= table->holdWeight;
                        if (r < 0) {
                            pick = table->type;
                            goto done;
                        }
                    }
                }
            }
            pick = 0;
        }
    done:
        item = pick;
        if (item == ITEM_NULL) {
            item = ITEM_COIN;
        }
    }
    if (!(flag & AC_RESULT_SUCCESS)) {
        evtSetValue(event, out, 0);
        return EVT_RETURN_DONE;
    }
    // FAKE: no-op (u16) cast (item is u16); the plain call changes the object
    if (pouchGetItem((u16)item) == 0) {
        evtSetValue(event, out, 0);
        return EVT_RETURN_DONE;
    }
    unit->heldItem = ITEM_NULL;
    if (BtlUnit_CheckUnitFlag(unit, BATTLE_UNIT_FLAG_STOLEN_ITEM)) {
        BtlUnit_OffUnitFlag(unit, BATTLE_UNIT_FLAG_STOLEN_ITEM);
        if (unit->groupSetupId >= 0) {
            NpcBattleInfo* setup = wp->fbatInfo->setup;
            setup->heldItems[unit->groupSetupId] = ITEM_NULL;
            setup->stolenItems[unit->groupSetupId] = ITEM_NULL;
        }
    } else if (unit->groupSetupId >= 0) {
        NpcBattleInfo* setup = wp->fbatInfo->setup;
        setup->heldItems[unit->groupSetupId] = ITEM_NULL;
    }
    if (item >= BADGE_MIN && item < BADGE_MAX) {
        if (unit->currentKind == UNIT_MARIO) {
            BtlUnit_EquipItem(unit, EQUIP_RESET | EQUIP_MARIO, 0);
        } else if (unit->currentKind >= TYPE_PARTNER_MIN && unit->currentKind < TYPE_PARTNER_MAX) {
            BtlUnit_EquipItem(unit, EQUIP_RESET | EQUIP_PARTNER, 0);
        } else {
            BtlUnit_EquipItem(unit, EQUIP_RESET, 0);
        }
    }
    // FAKE: no-op (u16) cast (item is u16); the plain call changes the object
    evtSetValue(event, out, (u16)item);
    return EVT_RETURN_DONE;
}

/**
 * Display callback that copies the current 608x480 frame into a new intensity (greyscale) texture in the unit's
 * screenTexBuf for Kiss Thief, then makes the unit's main part visible again.
 */
void mono_capture(CameraId camId, void* param) {
    BattleUnit* unit = param;
    BattleUnitPart* part = BtlUnit_GetPartsPtr(unit, 1);
    unit->screenTexBuf = smartAlloc(GXGetTexBufferSize(0x260, 0x1E0, GX_TF_I8, GX_FALSE, 0), 1);
    GXSetTexCopySrc(0, 0, 0x260, 0x1E0);
    GXSetTexCopyDst(0x260, 0x1E0, GX_TF_I8, GX_FALSE);
    GXCopyTex(unit->screenTexBuf->address, GX_FALSE);
    GXPixModeSync();
    part->animFlags &= ~PART_ANIM_NO_DISP;
}

/**
 * Display callback that draws the captured greyscale frame full-screen with alpha mono_alpha and
 * then redraws the partner's main part over it, so Ms. Mowz stays in colour during Kiss Thief.
 */
void mono_disp(CameraId camId, BattleUnit* unit) {
    CameraEntry* cam = camGetPtr(camId);
    smartEntry* texBuf = unit->screenTexBuf;
    void* buf = NULL;
    GXTexObj tex;
    if (texBuf != NULL) {
        buf = texBuf->address;
    }
    if (buf != NULL) {
        GXInitTexObj(&tex, buf, 0x260, 0x1E0, GX_TF_I8, GX_CLAMP, GX_CLAMP, GX_FALSE);
        GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&tex, GX_TEXMAP0);
        GXLoadPosMtxImm(cam->view, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXSetNumChans(0);
        GXSetNumTexGens(1);
        GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
        GXSetTevColor(GX_TEVREG0, (GXColor){0xFF, 0xFF, 0xFF, mono_alpha});
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        GXSetZCompLoc(GX_TRUE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(-304.0f, 240.0f, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);
        GXPosition3f32(304.0f, 240.0f, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
        GXPosition3f32(304.0f, -240.0f, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);
        GXPosition3f32(-304.0f, -240.0f, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);
        {
            BattleUnit* party = BattleGetPartyPtr(_battleWorkPointer);
            if (party != NULL) {
                BattleUnitPart* part = BtlUnit_GetPartsPtr(party, 1);
                CameraId prevCamId = camGetCurNo();
                camSetCurNo(CAMERA_3D);
                btlUnitPartsDisp(CAMERA_3D, part);
                camSetCurNo(prevCamId);
            }
        }
    }
}

/**
 * Registers mono_disp for the running unit every frame;
 * never returns, so Kiss Thief runs it in a spawned event and deletes that event when the effect ends.
 */
USER_FUNC(mono_main) {
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    dispEntry(CAMERA_2D, 910.0f, 0, (DispCallback)mono_disp, unit);
    return EVT_RETURN_BLOCK;
}

/** Queues mono_capture for this frame to grab the screen for Kiss Thief's greyscale effect. */
USER_FUNC(mono_capture_event) {
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    BtlUnit_GetPartsPtr(unit, 1);
    dispEntry(CAMERA_2D, 910.0f, 0, (DispCallback)mono_capture, unit);
    return EVT_RETURN_DONE;
}

/** Fades Kiss Thief's greyscale overlay in: raises mono_alpha from 0 by 10 a frame until it reaches 255. */
USER_FUNC(mono_on) {
    if (isFirstCall) {
        mono_alpha = 0;
    }
    mono_alpha += 10;
    if (mono_alpha > 255) {
        mono_alpha = 255;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Fades Kiss Thief's greyscale overlay out by 10 a frame; at 0 it frees the captured screen texture. */
USER_FUNC(mono_off) {
    BattleUnit* unit;
    s32 id;

    id = BattleTransID(event, UNIT_ID_SELF);
    unit = BattleGetUnitPtr(_battleWorkPointer, id);
    mono_alpha -= 10;
    if (mono_alpha < 0) {
        mono_alpha = 0;
        BtlUnit_GetPartsPtr(unit, 1);
        if (unit->screenTexBuf != NULL) {
            smartFree(unit->screenTexBuf);
        }
        unit->screenTexBuf = NULL;
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}
