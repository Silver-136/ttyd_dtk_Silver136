#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_audience.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
#include "libc_decls.h"
#include "eff/n64/eff_kumokumo_n64.h"
#include "eff/n64/eff_kameki_tornade_n64.h"
#include "system.h"
#include "eff/eff_batten.h"
#include "eff/eff_kemutest.h"

#include "manager/evtmgr_cmd.h"
#include "driver/effdrv.h"
#include "battle/battle.h"
#include "manager/evtmgr.h"
#include "dolphin/mtx.h"
#include "eff/n64/eff_akari_charge_n64.h"
#include "battle/battle_audience.h"
#include "eff/eff_kemu_test.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_batstage.h"
#include "evt/evt_eff.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_sub.h"
#include "pmario_sound.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots of the boss (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_ARMS_OUT 0         // 1 while the arms fly as rocket punch units (arm_bunri_event sets it, arm_return_event clears it)
#define UNIT_WORK_ARM_RETURN_TURNS 1 // set to 2 at launch; once arm_check finds no arm left, attack_event counts it down and at 0 runs arm_return_event
#define UNIT_WORK_ARMS_LAUNCHED 2    // 1 after the first arm_bunri_event (its line plays only once)
#define UNIT_WORK_ARM_0_UNUSED 3     // unit id of the rocket punch launched as arm 0; never read
#define UNIT_WORK_ARM_1_UNUSED 4     // unit id of the rocket punch launched as arm 1; never read
#define UNIT_WORK_DRILL_TALK_DONE 5  // 1 after the first drill_attack_event showed stg7_aji_41_02; while 0 attack_event always picks the drill
#define UNIT_WORK_LOW_HP_TALK_DONE 6 // 1 once damage_event showed its low-HP line (dead_event sets it too)
#define UNIT_WORK_AUD_TALK_DONE 7    // 1 after the first aud_attack_event (stg7_aji_41_04/05); at 35 HP or less with 20 or more audience it goes first while 0
#define UNIT_WORK_AUD_HIT 8          // 1 once a machine-gun shot of aud_attack_event has hit; the 10th shot then hits without a new check
// ... and of the rocket punch units it launches (unit_boss_rocket_punch_mkII below; the *_r and arm_event* events)
#define ROCKET_PUNCH_WORK_SIDE 0     // which arm: 0/1, or 2/3 for an arm spawned by arm_return_event (no fly-in); arm_check reads it
// attack_event_r subtracts 1 and waits while the result is 1 or more; set to 1 at launch and after each attack
#define ROCKET_PUNCH_WORK_WAIT 1

#include "unit/unit_boss_magnum_battender.h"
double atan(double x);
typedef struct BattenEffWork {
    s32 type;
    Vec pos;
    u8 field_0x10[0x28 - 0x10];
    Mtx mtx;
} BattenEffWork;
typedef struct TargetWork {
    s32 state;
    s32 count;
    EffectEntry* eff;
} TargetWork;
extern EvtWord aud_hit_event[];
void psndSFX_vol(s32 id, u8 vol);
EffectEntry* eff_tor;
s32 torCount;
EffectEntry* eff_jet[2];
s32 jetCount;
static s32* list;
EffectEntry* eff_suikomi;
s32 screamId;
s32 scream_fade_timer;
u8 scream_fade_start_vol;
u8 scream_fade_vol;
TargetWork* wp_target;
s32 targetSoundId;
static EffectEntry* eff_charge;
static ArmWork* work[2];

static USER_FUNC(arm_check);
static USER_FUNC(arm_end);
static USER_FUNC(arm_init);
static USER_FUNC(arm_main);
USER_FUNC(get_aud_num);
static USER_FUNC(waku_main);
static USER_FUNC(arm_init2);
static USER_FUNC(eff_dead_delete);
static USER_FUNC(eff_dead);
USER_FUNC(aud_target_mark);
USER_FUNC(aud_fire_no_hit);
USER_FUNC(aud_fire_hit);
USER_FUNC(aud_scream_fadeout);
USER_FUNC(aud_suikomi);
USER_FUNC(aud_jet_end);
USER_FUNC(aud_jet);
USER_FUNC(drill_jet);
USER_FUNC(drill_tor);

BattleUnitKind unit_boss_magnum_battender_mkII;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[17];
static PoseTableEntry pose_table_no_arm[16];
static BattleDataEntry data_table[2];
static BattleUnitKindPart parts[4];
BattleWeapon weapon_batten_drill_attack;
BattleWeapon weapon_batten_boomerang_attack;
BattleWeapon weapon_batten_aud_attack;
static BattleUnitSetup entry_rocket_punch;
EvtWord aud_hit_event[24];
EvtWord aud_no_hit_event[16];
static EvtWord init_event[92];
static EvtWord waku_event[23];
static EvtWord damage_event[111];
static EvtWord dead_event[234];
static EvtWord entry_event[78];
static EvtWord attack_event[320];
EvtWord drill_attack_event[392];
EvtWord boomerang_attack_event[324];
EvtWord aud_attack_event[823];
EvtWord aud_attack_event_sub[139];
static EvtWord arm_bunri_event[199];
static EvtWord arm_return_event[151];
static EvtWord wait_event[6];
BattleUnitKind unit_boss_rocket_punch_mkII;
static BattleUnitDefense defence_r;
static BattleUnitDefenseAttr defence_attr_r;
static StatusVulnerability regist_r;
static PoseTableEntry pose_table_r[14];
static BattleDataEntry data_table_r[1];
static BattleUnitKindPart parts_r[1];
static BattleWeapon weapon_batten_rocket_attack;
static EvtWord init_event_r[191];
static EvtWord arm_event[59];
static EvtWord arm_event_end[35];
static EvtWord damage_event_r[30];
static EvtWord attack_event_r[368];
static EvtWord wait_event_r[18];

/** Unit data of Magnus von Grapple 2.0: 70 HP, level 84; the battle starts it from init_event. */
BattleUnitKind unit_boss_magnum_battender_mkII = {
    UNIT_MAGNUS2_0, // mUnitType
    "btl_un_magnum_battender_mkii", // name
    70, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    84, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    6, // pbSoftCap
    120, // width
    160, // height
    {30, 100}, // hitOffset
    {0.0f, 80.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {30.0f, 115.0f, 0.0f}, // talkTogeBaseOffset
    {60.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {52.0f, 0.0f, 0.0f}, // bintaHitOffset
    {30.0f, 110.0f, 20.0f}, // kissHitOffset
    {0.0f, 80.0f, 0.0f}, // cutBaseOffset
    120.0f, // cutWidth
    160.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_MBTN_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    4, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

/** Defence of Magnus von Grapple 2.0: 2 against every element. */
static BattleUnitDefense defence = {{2, 2, 2, 2, 2}};

/** Elemental reactions of Magnus von Grapple 2.0: none, every element hurts it normally. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    0, // sleep
    40, // stop
    40, // dizzy
    0, // poison
    40, // confuse
    0, // electric
    100, // burn
    20, // freeze
    0, // huge
    0, // tiny
    100, // attackUp
    0, // attackDown
    100, // defenseUp
    80, // defenseDown
    40, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    30, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

/**
 * Pose table of Magnus von Grapple 2.0: the animation each battle pose (idle, hurt, walk, attack and so on)
 * plays.
 */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "MGN_S_1"},
    {POSE_TYPE_STOP, "MGN_2D_1"},
    {POSE_TYPE_FREEZE, "MGN_2D_1"},
    {POSE_TYPE_CONFUSE, "MGN_X_1"},
    {POSE_TYPE_POISON, "MGN_X_1"},
    {POSE_TYPE_DIZZY, "MGN_X_1"},
    {POSE_TYPE_NORMAL, "MGN_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "MGN_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "MGN_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "MGN_S_1"},
    {POSE_TYPE_DAMAGE, "MGN_D_1"},
    {POSE_TYPE_WALK, "MGN_W_1"},
    {POSE_TYPE_RUN_HOME, "MGN_W_2"},
    {POSE_TYPE_DANGER, "MGN_S_1"},
    {POSE_TYPE_PERIL, "MGN_S_1"},
    {POSE_TYPE_TALK, "MGN_T_1"},
    {POSE_TYPE_ACTION, "MGN_S_1"},
};

/** Pose table of Magnus von Grapple 2.0 once his arms have split off (poses without the fists). */
static PoseTableEntry pose_table_no_arm[] = {
    {POSE_TYPE_SLEEP, "MGN_S_2"},
    {POSE_TYPE_STOP, "MGN_2D_2"},
    {POSE_TYPE_FREEZE, "MGN_2D_2"},
    {POSE_TYPE_CONFUSE, "MGN_X_2"},
    {POSE_TYPE_POISON, "MGN_X_2"},
    {POSE_TYPE_DIZZY, "MGN_X_2"},
    {POSE_TYPE_NORMAL, "MGN_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "MGN_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "MGN_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "MGN_S_2"},
    {POSE_TYPE_DAMAGE, "MGN_D_2"},
    {POSE_TYPE_WALK, "MGN_W_3"},
    {POSE_TYPE_DANGER, "MGN_S_2"},
    {POSE_TYPE_PERIL, "MGN_S_2"},
    {POSE_TYPE_TALK, "MGN_T_2"},
    {POSE_TYPE_ACTION, "MGN_S_2"},
};

/** Event table of Magnus von Grapple 2.0: its defeat (dead_event, tag 49). */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

/**
 * Parts of Magnus von Grapple 2.0: part 1 is his body (the target), part 2 an extra part, part 3 the frame
 * piece that follows the body's locator17 bone, and part 4 the boomerang that splits off his body.
 */
static BattleUnitKindPart parts[] = {
#ifdef VERSION_G8ME01
    {1, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 37.0f, 103.0f, 30.0f, 37.0f, 113.0f, 61.0f, 106, 174, 255, &defence, &defence_attr, 0x600019, 0, pose_table},
#else
    {1, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 37.0f, 103.0f, 30.0f, 37.0f, 113.0f, 61.0f, 106, 174, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, 0, pose_table},
#endif
    {2, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 10.0f, 10.0f, 30.0f, 10.0f, 20.0f, 61.0f, 106, 174, 255, &defence, &defence_attr, kImportantPart | kJumplikeCannotTarget | kHidden | kNoShadow, 0, pose_table},
    {3, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 30, 30, 255, &defence, &defence_attr, kNotTargetable | kNoShadow | kAbsolutePosition, 0, pose_table},
    {4, "btl_un_magnum_battender_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 30, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

/** Weapon data for Magnus von Grapple 2.0's drill attack: 6 damage, a jump-type hit, one target. */
BattleWeapon weapon_batten_drill_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon data for Magnus von Grapple 2.0's boomerang: 6 damage, every target. */
BattleWeapon weapon_batten_boomerang_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {6, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon data for Magnus von Grapple 2.0's audience attack: 3 damage, one target. */
BattleWeapon weapon_batten_aud_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
#ifdef VERSION_G8ME01
    2, // superguardsAllowed
#else
    0, // superguardsAllowed
#endif
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {3, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    TARGET_WEIGHT_PREFER_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Setup for the X-Punch fists that Magnus von Grapple 2.0 spawns when he splits off his arms. */
static BattleUnitSetup entry_rocket_punch = {
    &unit_boss_rocket_punch_mkII, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/**
 * Event run on a thrown audience member that hit its target in the Magnus von Grapple 2.0 audience attack:
 * moves it to the hit position over LW(5) frames, makes it jump away along an arc,
 * deletes it and lowers the audience size by one.
 */
EVT_BEGIN(aud_hit_event)
    CALL(evt_audience_move_position_frame, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
    CALL(evt_audience_jump_position_gravity, LW(0), LW(6), LW(7), LW(8), LW(9), LW(10))
    CALL(evt_audience_delete, LW(0))
    CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
    RETURN
EVT_END()

/**
 * Event run on a thrown audience member that missed:
 * moves it over LW(5) frames to the miss position, deletes it and lowers the audience size by one.
 */
EVT_BEGIN(aud_no_hit_event)
    CALL(evt_audience_move_position_frame, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
    CALL(evt_audience_delete, LW(0))
    CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
    RETURN
EVT_END()

/**
 * Script function: keeps the frame part (part 3) of the boss on the locator17 bone of its body part
 * and turns it to face the camera each frame; args[0] is the unit.
 */
static USER_FUNC(waku_main) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit;
    BattleUnitPart* part;
    BattleUnitPart* waku;
    s32 poseId;
    AnimationPose* pose;
    s32 idx;
    f32 yaw;

    animGetPtr();
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    part = BtlUnit_GetPartsPtr(unit, 1);
    waku = BtlUnit_GetPartsPtr(unit, 3);
    poseId = part->poseId;
    animPoseWorldPositionEvalOn(poseId);
    animPoseGetAnimBaseDataPtr(poseId);
    pose = animPoseGetAnimPosePtr(poseId);
    idx = animPoseGetGroupIdx(poseId, "locator17");
    BtlUnit_SetPartsPos(waku, (*pose->worldPositions)[idx].x, (*pose->worldPositions)[idx].y,
                        (*pose->worldPositions)[idx].z);
    yaw = camGetPtr(CAMERA_3D)->viewYaw;
    BtlUnit_SetPartsRotate(waku, 0.0f, -yaw, 0.0f);
    return EVT_RETURN_DONE;
}

/**
 * Script function: looks for one of the boss's two punching arms (UNIT_X_FIST or UNIT_X_PUNCH) by side,
 * args[0] = 0 for the arms with work[0] of 0 or 2 and 1 for those with 1 or 3.
 * Returns the unit ID in args[1], or -1 if there is none.
 */
static USER_FUNC(arm_check) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 side = evtGetValue(event, args[0]);
    s32 found = -1;
    BattleUnit* unit;
    s32 i;

    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && (unit->currentKind == UNIT_X_FIST || unit->currentKind == UNIT_X_PUNCH)) {
            if (side == 0 && (unit->work[ROCKET_PUNCH_WORK_SIDE] == 0 || unit->work[ROCKET_PUNCH_WORK_SIDE] == 2)) {
                found = i;
                break;
            }
            if (side == 1 && (unit->work[ROCKET_PUNCH_WORK_SIDE] == 1 || unit->work[ROCKET_PUNCH_WORK_SIDE] == 3)) {
                found = i;
                break;
            }
        }
    }
    evtSetValue(event, args[1], found);
    return EVT_RETURN_DONE;
}

/**
 * Script function that plays the tornado effect under the boss during the drill attack for 70 frames,
 * following the unit's X and Z and staying 240 units below it (only while it is below 300 high),
 * then deletes it.
 */
USER_FUNC(drill_tor) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;

    BtlUnit_GetPos(unit, &x, &y, &z);
    if (isFirstCall) {
        eff_tor = effKamekiTornadeN64Entry(0, 1000, 0.0f, -1000.0f, 0.0f, 3.5f);
        torCount = 0;
    }
    if (y <= 300.0f) {
        ((BattenEffWork*)eff_tor->userdata)->pos.x = x;
        ((BattenEffWork*)eff_tor->userdata)->pos.y = y - 240.0f;
        ((BattenEffWork*)eff_tor->userdata)->pos.z = z;
    }
    if (++torCount >= 70) {
        effDelete(eff_tor);
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script function that plays the drill attack's jet flames at two points next to the unit args[0] for 90 frames,
 * then deletes them.
 */
USER_FUNC(drill_jet) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    BattenEffWork* ew;

    BtlUnit_GetPos(unit, &x, &y, &z);
    if (isFirstCall) {
        for (i = 0; i < 2; i++) {
            eff_jet[i] = effBattenEntry(3, 1000, 0.0f, -1000.0f, 0.0f, 3.0f);
        }
        jetCount = 0;
    }
    ew = eff_jet[0]->userdata;
    ew->pos.x = x - 20.0f;
    ew->pos.y = y;
    ew->pos.z = z - 20.0f;
    ew = eff_jet[1]->userdata;
    ew->pos.x = 20.0f + x;
    ew->pos.y = y;
    ew->pos.z = 20.0f + z;
    if (++jetCount >= 90) {
        for (i = 0; i < 2; i++) {
            effDelete(eff_jet[i]);
        }
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Script function that shows the two jet flames of the audience-attack pod at fixed offsets from the unit
 * args[0], tilted 20 degrees, until aud_jet_end sets jetCount to 1.
 */
USER_FUNC(aud_jet) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    Mtx mtx;
    f32 x;
    f32 y;
    f32 z;
    s32 i;
    BattenEffWork* ew;

    BtlUnit_GetPos(unit, &x, &y, &z);
    if (isFirstCall) {
        for (i = 0; i < 2; i++) {
            eff_jet[i] = effBattenEntry(3, 1000, 0.0f, -1000.0f, 0.0f, 3.0f);
        }
        jetCount = 0;
    }
    PSMTXRotRad(mtx, 'z', -0.34906584f);
    ew = eff_jet[0]->userdata;
    ew->pos.x = x - 60.0f;
    ew->pos.y = 22.5f + y;
    ew->pos.z = z;
    PSMTXCopy(mtx, ((BattenEffWork*)eff_jet[0]->userdata)->mtx);
    ew = eff_jet[1]->userdata;
    ew->pos.x = x - 15.0f;
    ew->pos.y = 22.5f + y;
    ew->pos.z = 70.0f + z;
    PSMTXCopy(mtx, ((BattenEffWork*)eff_jet[1]->userdata)->mtx);
    if (jetCount == 1) {
        for (i = 0; i < 2; i++) {
            effDelete(eff_jet[i]);
        }
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Script function: tells aud_jet to end by setting jetCount to 1. */
USER_FUNC(aud_jet_end) {
    jetCount = 1;
    return EVT_RETURN_DONE;
}

/** Script function: returns the current number of audience members in args[0]. */
USER_FUNC(get_aud_num) {
    EvtWord* args = event->args;

    evtSetValue(event, args[0], BattleAudience_GetAudienceNum());
    return EVT_RETURN_DONE;
}

/**
 * Script function for the audience attack's suction:
 * picks 20 random audience members, makes them spin
 * and fly toward the boss's pod (a dust-cloud effect and SFX_AUDIENCE_PANIC_SCREAM1 start),
 * and removes each when it arrives. The list of 20 is kept for the shots that follow.
 * Blocks until all 20 arrived.
 */
USER_FUNC(aud_suikomi) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    s32 done = 0;
    BattleAudienceMember* mem;
    s32 i;
    s32 j;
    f32 x;
    f32 y;
    f32 z;
    f32 x2;
    f32 y2;
    f32 z2;
    s32 ids[200];

    if (isFirstCall) {
        s32 n = 0;

        if (list != NULL) {
            BattleFree(list);
        }
        list = BattleAlloc(sizeof(s32) * 20);
        for (i = 0; i < 200; i++) {
            if ((u8)BattleAudience_GetExist(i) == TRUE) {
                ids[n] = i;
                n++;
            }
        }
        // Shuffle the occupied-seat IDs before taking the first twenty.
        for (i = 0; i < 1000; i++) {
            s32 a = irand(n);
            s32 b = irand(n);
            s32 tmp = ids[a];

            ids[a] = ids[b];
            ids[b] = tmp;
        }
        for (i = 0; i < 20; i++) {
            mem = BattleAudienceGetPtr(ids[i]);
            mem->flags |= AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL;
            BattleAudience_SetAnim(ids[i], 9, FALSE);
            mem->interpFramesTotal = i * 3 + 60;
            mem->counter = 0;
            mem->mRotation.y = 0.0f;
            list[i] = ids[i];
        }
        BtlUnit_GetPos(BattleGetUnitPtr(_battleWorkPointer, id), &x, &y, &z);
        eff_suikomi = effKumokumoN64Entry(2, 120, x - 10.0f, 10.0f + y, z, -40.0f, 1.0f, 1.5f);
        screamId = psndSFXOn("SFX_AUDIENCE_PANIC_SCREAM1");
    }
    BtlUnit_GetPos(BattleGetUnitPtr(_battleWorkPointer, id), &x2, &y2, &z2);
    y2 += 5.0f;
    for (i = 0; i < 200; i++) {
        for (j = 0; j < 20; j++) {
            if (i == list[j]) {
                mem = BattleAudienceGetPtr(i);
                mem->mRotation.y += 19.0f;
                if (mem->interpFrameCounter < mem->interpFramesTotal) {
                    mem->interpFrameCounter++;
                } else {
                    done++;
                    mem->flags &= ~4;
                    mem->flags |= AUDIENCE_MEMBER_FLAG_NOT_EXIST;
                }
                mem->position.x = intplGetValue(EASING_QUADRATIC_OUT, mem->mHomePosition.x, x2, mem->interpFrameCounter, mem->interpFramesTotal);
                mem->position.y = intplGetValue(EASING_CUBIC_IN, mem->mHomePosition.y, y2, mem->interpFrameCounter, mem->interpFramesTotal);
                mem->position.z = intplGetValue(EASING_QUADRATIC_OUT, mem->mHomePosition.z, z2, mem->interpFrameCounter, mem->interpFramesTotal);
                mem->baseScale.x = mem->baseScale.y = intplGetValue(EASING_QUADRATIC_OUT, 1.0f, 0.65f, mem->interpFrameCounter, mem->interpFramesTotal);
                break;
            }
        }
    }
    return done < 20 ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

// Descriptive name (not in the symbol maps); placeholder unk_aji_00007668, JP aji .text+0x75A4. See docs/NAME_PROVENANCE.md.
USER_FUNC(aud_scream_fadeout) {
    if (isFirstCall) {
        scream_fade_timer = 120;
        scream_fade_start_vol = psndSFX_get_vol(screamId);
    }
    scream_fade_timer--;
    scream_fade_vol = intplGetValue(EASING_LINEAR, scream_fade_start_vol, 0.0f, 120 - scream_fade_timer, 120);
    psndSFX_vol(screamId, scream_fade_vol);
    if (scream_fade_timer <= 0) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Returns the position of the boss's pod, 10 units right of and 78 above the unit, for the audience cannon. */
void aud_get_pod_pos(BattleUnit* unit, f32* x, f32* y, f32* z) {
    BtlUnit_GetPos(unit, x, y, z);
    *x += 10.0f;
    *y += 78.0f;
}

/** Starts the smoke puff (kemuri_test effect) at the pod's muzzle for each audience shot. */
void aud_fire_eff(f32 x, f32 y, f32 z) {
    effKemuTestSetRxRz(effKemuTestEntry(11, x - 50.0f, 20.0f + y, 50.0f + z, 0.75f), 40.0f, 110.0f);
}

/**
 * Script function: fires two audience members from the pod at the target args[1] (the unit args[0] is the boss,
 * args[2] the first of the sucked-in members, list[start]):
 * each flies a throwing arc to the target's hit position with aud_hit_event; used when the shot hits.
 */
USER_FUNC(aud_fire_hit) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 target = BattleTransID(event, evtGetValue(event, args[1]));
    s32 start = evtGetValue(event, args[2]);
    BattleUnit* unit;
    BattleUnitPart* part;
    BattleAudienceMember* mem;
    EventEntry* evt;
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    f32 hx;
    f32 hy;
    f32 hz;

    if (isFirstCall) {
        aud_get_pod_pos(BattleGetUnitPtr(_battleWorkPointer, id), &x, &y, &z);
        aud_fire_eff(x, y, z);
        unit = BattleGetUnitPtr(_battleWorkPointer, target);
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        BtlUnit_GetHitPos(unit, part, &hx, &hy, &hz);
        for (i = start; i < start + 2; i++) {
            mem = BattleAudienceGetPtr(list[i]);
            BattleAudience_SetAnim(list[i], 9, FALSE);
            mem->position = (Vec){x, y, z};
            mem->interpFramesTotal = 0;
            mem->counter = 0;
            mem->mRotation.y = 0.0f;
            mem->baseScale.x = mem->baseScale.y = 1.0f;
            mem->flags |= 4;
            evt = evtEntry(aud_hit_event, 0, EVENT_START_IMMEDIATE);
            evt->lwData[0] = list[i];
            evt->lwData[1] = (s32)(1024.0f * hx) - EVTDAT_FLOAT_BASE;
            evt->lwData[2] = (s32)(1024.0f * hy) - EVTDAT_FLOAT_BASE;
            evt->lwData[3] = (s32)(1024.0f * hz) - EVTDAT_FLOAT_BASE;
            evt->lwData[4] = 0;
            evt->lwData[5] = irand(3) + 10;
            evt->lwData[6] = (s32)(1024.0f * (hx + 200.0f * rand() / 32767.0f - 100.0f)) - EVTDAT_FLOAT_BASE;
            evt->lwData[7] = FLOAT(-100.0f);
            evt->lwData[8] = (s32)(1024.0f * (hz + 100.0f * rand() / 32767.0f - 50.0f)) - EVTDAT_FLOAT_BASE;
            evt->lwData[9] = irand(30) + 30;
            evt->lwData[10] = FLOAT(-0.3f);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script function: like aud_fire_hit but for a shot that misses:
 * the audience members fly past the target to x = -275 (aud_hit_event).
 */
USER_FUNC(aud_fire_no_hit) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    s32 target = BattleTransID(event, evtGetValue(event, args[1]));
    s32 start = evtGetValue(event, args[2]);
    BattleUnit* unit;
    BattleUnitPart* part;
    BattleAudienceMember* mem;
    EventEntry* evt;
    s32 i;
    f32 x;
    f32 y;
    f32 z;
    f32 hx;
    f32 hy;
    f32 hz;

    if (isFirstCall) {
        aud_get_pod_pos(BattleGetUnitPtr(_battleWorkPointer, id), &x, &y, &z);
        aud_fire_eff(x, y, z);
        unit = BattleGetUnitPtr(_battleWorkPointer, target);
        part = BtlUnit_GetPartsPtr(unit, BtlUnit_GetBodyPartsId(unit));
        BtlUnit_GetHitPos(unit, part, &hx, &hy, &hz);
        for (i = start; i < start + 2; i++) {
            mem = BattleAudienceGetPtr(list[i]);
            BattleAudience_SetAnim(list[i], 9, FALSE);
            mem->position = (Vec){x, y, z};
            mem->interpFramesTotal = 0;
            mem->counter = 0;
            mem->mRotation.y = 0.0f;
            mem->baseScale.x = mem->baseScale.y = 1.0f;
            mem->flags |= 4;
            evt = evtEntry(aud_hit_event, 0, EVENT_START_IMMEDIATE);
            evt->lwData[0] = list[i];
            // This miss path also starts aud_hit_event; only its first six arguments are set here.
            evt->lwData[1] = FLOAT(-275.0f);
            evt->lwData[2] = (s32)(1024.0f * (hy - 20.0f)) - EVTDAT_FLOAT_BASE;
            evt->lwData[3] = (s32)(1024.0f * hz) - EVTDAT_FLOAT_BASE;
            evt->lwData[4] = 0;
            evt->lwData[5] = irand(3) + 10;
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script function that draws the targeting marker for the audience attack at the position given by args[0] to
 * args[2]: the reticle swoops in over 60, 40 and 20 frames with SFX_BOSS_MBTN_SEARCH1 and
 * then locks on with SFX_BOSS_MBTN_LOCKON1.
 */
USER_FUNC(aud_target_mark) {
    EvtWord* args = event->args;
    f32 x = evtGetValue(event, args[0]);
    f32 y = evtGetValue(event, args[1]);
    f32 z = evtGetValue(event, args[2]);
    BattenEffWork* ew;

    if (isFirstCall) {
        wp_target = BattleAlloc(sizeof(TargetWork));
        wp_target->eff = effBattenEntry(1, 1000, 0.0f, -1000.0f, 0.0f, 1.0f);
        wp_target->state = 0;
        wp_target->count = 0;
        targetSoundId = psndSFXOn("SFX_BOSS_MBTN_SEARCH1");
    }
    ew = wp_target->eff->userdata;
    switch (wp_target->state) {
        case 0:
            wp_target->count++;
            ew->pos.x = intplGetValue(EASING_QUADRATIC_OUT, 60.0f + x, x - 5.0f, wp_target->count, 60);
            ew->pos.y = intplGetValue(EASING_QUADRATIC_OUT, 20.0f + y, 60.0f + y, wp_target->count, 60);
            ew->pos.z = 20.0f + z;
            if (wp_target->count >= 60) {
                wp_target->state = 5;
                wp_target->count = 0;
            }
            break;
        case 5:
            wp_target->count++;
            ew->pos.x = intplGetValue(EASING_QUADRATIC_OUT, x - 5.0f, x - 25.0f, wp_target->count, 40);
            ew->pos.y = intplGetValue(EASING_QUADRATIC_OUT, 60.0f + y, 20.0f + y, wp_target->count, 40);
            ew->pos.z = 20.0f + z;
            if (wp_target->count >= 40) {
                wp_target->state = 10;
                wp_target->count = 0;
            }
            break;
        case 10:
            wp_target->count++;
            ew->pos.x = intplGetValue(EASING_QUADRATIC_OUT, x - 25.0f, x, wp_target->count, 20);
            ew->pos.y = intplGetValue(EASING_QUADRATIC_OUT, 20.0f + y, y, wp_target->count, 20);
            ew->pos.z = 20.0f + z;
            if (wp_target->count >= 20) {
                wp_target->state = 15;
                wp_target->count = 0;
                effDelete(wp_target->eff);
                wp_target->eff = effBattenEntry(2, 1000, x, y, 10.0f + z, 1.0f);
                psndSFXOff(targetSoundId);
                psndSFXOn("SFX_BOSS_MBTN_LOCKON1");
            }
            break;
        case 15:
            wp_target->count++;
            if (wp_target->count >= 90) {
                effDelete(wp_target->eff);
                BattleFree(wp_target);
                return EVT_RETURN_DONE;
            }
            break;
    }
    return EVT_RETURN_BLOCK;
}

/** Script function: starts the dying boss's charge-up effect (akari_charge_n64) at args[0] to args[2]. */
static USER_FUNC(eff_dead) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    eff_charge = effAkariChargeN64Entry(0, 1000, x, y, z, 1.5f);
    return EVT_RETURN_DONE;
}

/** Script function: removes the charge-up effect started by eff_dead. */
static USER_FUNC(eff_dead_delete) {
    effDelete(eff_charge);
    return EVT_RETURN_DONE;
}

/**
 * Battle init for Magnus von Grapple 2.0: registers its wait, entry, attack, damage and confusion events,
 * sets the part 3 pose MGN_L_1, starts the frame event (waku_event) and the arm physics (arm_init)
 * and clears its unit work flags: 0 = arms detached, 1 = turns until the fists return,
 * 2 = arms have separated once, 5 and 6 are phase flags (6 is set when the boss is defeated).
 */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(entry_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("MGN_L_1"))
    SPAWN_EVENT(PTR(waku_event))
    CALL(arm_init)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARM_RETURN_TURNS, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_LAUNCHED, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_DRILL_TALK_DONE, 0)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, 125, 0, 0)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 125, 0, 0)
    CALL(btlevtcmd_SetBaseRotate, UNIT_ID_SELF, 0, -55, 0)
    CALL(btlevtcmd_SetWalkSound, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_MOVE1"), STRING("SFX_BOSS_MBTN_MOVE1"), 26, 26, 26)
    CALL(btlevtcmd_SetRunSound, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_MOVE1"), STRING("SFX_BOSS_MBTN_MOVE1"), 20, 20, 20)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Per-frame loop that calls waku_main for the boss (keeping the frame part, part 3, on the body's locator17 bone
 * and facing the camera) until the unit no longer exists.
 */
static EVT_BEGIN(waku_event)
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        GOTO(99)
    END_IF
    CALL(waku_main, UNIT_ID_SELF)
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

/**
 * Damage event of Magnus von Grapple 2.0: marks part 3 with part attribute 0x1000000
 * while damage pattern 10 is processed, runs the default damage handler,
 * and the first time its HP drops to 14 or lower (and it is not already beaten) it waits for attacks to end,
 * zooms the camera on it and speaks stg7_aji_41_06.
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_CheckDamagePattern, UNIT_ID_SELF, ATTACKPATTERN_PRESS, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW_HP_TALK_DONE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), 14)
            CALL(btlevtcmd_WaitAttackEnd)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW_HP_TALK_DONE, 1)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            SUBTRACT(LW(0), 20)
            CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
            WAIT_FRAMES(20)
            CALL(btlevtcmd_SetTalkPoseType, UNIT_ID_SELF, 65)
            CALL(btlevtcmd_SetStayPoseType, UNIT_ID_SELF, 69)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg7_aji_41_06"), 0, -2)
            CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
            CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
            WAIT_MS(500)
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Defeat sequence of Magnus von Grapple 2.0: sets the defeated flag (unit work 6),
 * plays the explosion effect over the body, zooms the camera,
 * speaks stg7_aji_41_07 and runs the common defeat handling (subsetevt_dead_core).
 */
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_LOW_HP_TALK_DONE, 1)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 70)
    ADD(LW(2), 10)
    CALL(eff_dead, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 20)
    CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_SetTalkPoseType, UNIT_ID_SELF, BTL_POSE_DAMAGE)
    CALL(btlevtcmd_SetStayPoseType, UNIT_ID_SELF, BTL_POSE_DAMAGE)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg7_aji_41_07"), 0, -2)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    WAIT_MS(500)
    CALL(arm_check, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_CheckDamageCode, LW(0), DAMAGE_RESULT_ZERO_HP, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_VALUE(LW(10), LW(0))
            BEGIN_INLINE_EVENT
                CALL_CHILD(subsetevt_dead_core)
                CALL(arm_end, 0)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kHidden)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kNoShadow)
            END_INLINE_EVENT
        END_IF
    END_IF
    CALL(arm_check, 1, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_CheckDamageCode, LW(0), DAMAGE_RESULT_ZERO_HP, LW(1))
        IF_INT_EQUAL(LW(1), 0)
            SET_VALUE(LW(10), LW(0))
            BEGIN_INLINE_EVENT
                CALL_CHILD(subsetevt_dead_core)
                CALL(arm_end, 1)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kHidden)
                CALL(btlevtcmd_OnPartsAttribute, LW(10), 1, kNoShadow)
            END_INLINE_EVENT
        END_IF
    END_IF
    SET_VALUE(LW(10), -2)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
    ADD(LW(2), 50)
    ADD(LW(1), 30)
    CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    WAIT_FRAMES(120)
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_KillUnit, LW(10), 1)
    CALL(eff_dead_delete)
    RETURN
EVT_END()

/**
 * Entrance of Magnus von Grapple 2.0 at the start of the fight:
 * waits 2 seconds, then a camera close-up on the boss with the opening lines stg7_aji_41_00
 * and stg7_aji_41_01 (the partner answers), after which the camera returns to normal.
 */
static EVT_BEGIN(entry_event)
    WAIT_FRAMES(120)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 20)
    CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
    WAIT_FRAMES(20)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg7_aji_41_00"), 0, -2)
    CALL(evt_btl_camera_set_moveto, 1, -102, 83, 334, -102, 59, 0, 20, 11)
    WAIT_FRAMES(20)
    CALL(evt_msg_print_battle_party, STRING("stg7_aji_41_01"))
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    WAIT_MS(500)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_PARTNER, LW(0))
    RETURN
EVT_END()

/**
 * Attack choice of Magnus von Grapple 2.0.
 * When both detached fists are gone it counts unit work 1 down and, at 0, calls them back (arm_return_event).
 * With the arms on it uses the drill while its HP is above 63 or on its first attack,
 * splits its arms off (arm_bunri_event) the first time after that,
 * then picks drill 40 / arm separation 40 (above 35 HP)
 * or drill 40 / separation 40 / audience attack 25 (35 HP or less).
 * With the fists out it throws the boomerang above 35 HP,
 * and at 35 HP or less boomerang 75 / audience attack 20.
 * The audience attack (aud_attack_event) needs at least 20 audience members and is used first
 * when still unused (unit work 7).
 */
static EVT_BEGIN(attack_event)
    // If the fists are out (unit work 0 is 1), it counts down how long they stay away and calls them back
    // when none are left.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(arm_check, 0, LW(0))
        IF_INT_EQUAL(LW(0), -1)
            CALL(arm_check, 1, LW(0))
            IF_INT_EQUAL(LW(0), -1)
                CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, UNIT_WORK_ARM_RETURN_TURNS, -1)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARM_RETURN_TURNS, LW(0))
                IF_INT_LESS_OR_EQUAL(LW(0), 0)
                    CALL_CHILD(PTR(arm_return_event))
                END_IF
            END_IF
        END_IF
    END_IF
    // It uses an item instead of attacking when one is ready.
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    // Arms on: it uses the drill on its first attack and while its HP is above 63.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_DRILL_TALK_DONE, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(drill_attack_event))
            GOTO(99)
        END_IF
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_GREATER(LW(0), 63)
            CALL_CHILD(PTR(drill_attack_event))
            GOTO(99)
            ELSE
            // After that the first thing it does is split its arms off into two fists (arm_bunri_event).
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_LAUNCHED, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL_CHILD(PTR(arm_bunri_event))
                GOTO(99)
            END_IF
            CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
            // Above 35 HP: drill or splitting the arms off, 40 weight each.
            IF_INT_GREATER(LW(0), 35)
                SET_VALUE(LW(0), 0)
                ADD(LW(0), 40)
                ADD(LW(0), 40)
                SUBTRACT(LW(0), 1)
                CALL(evt_sub_random, LW(0), LW(1))
                SET_VALUE(LW(0), 40)
                IF_INT_LESS(LW(1), LW(0))
                    CALL_CHILD(PTR(drill_attack_event))
                    GOTO(99)
                    ELSE
                    CALL_CHILD(PTR(arm_bunri_event))
                    GOTO(99)
                END_IF
                ELSE
                // 35 HP or less: the audience attack comes first if it is unused and there are 20 audience
                // members; then drill 40, separation 40, audience attack 25.
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AUD_TALK_DONE, LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    CALL(get_aud_num, LW(0))
                    IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                        CALL_CHILD(PTR(aud_attack_event))
                        GOTO(99)
                    END_IF
                END_IF
                LABEL(10)
                SET_VALUE(LW(0), 0)
                ADD(LW(0), 40)
                ADD(LW(0), 40)
                ADD(LW(0), 25)
                SUBTRACT(LW(0), 1)
                CALL(evt_sub_random, LW(0), LW(1))
                SET_VALUE(LW(0), 40)
                IF_INT_LESS(LW(1), LW(0))
                    CALL_CHILD(PTR(drill_attack_event))
                    GOTO(99)
                END_IF
                ADD(LW(0), 40)
                IF_INT_LESS(LW(1), LW(0))
                    CALL_CHILD(PTR(arm_bunri_event))
                    GOTO(99)
                END_IF
                ADD(LW(0), 25)
                IF_INT_LESS(LW(1), LW(0))
                    CALL(get_aud_num, LW(0))
                    IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                        CALL_CHILD(PTR(aud_attack_event))
                        GOTO(99)
                    END_IF
                END_IF
                GOTO(10)
            END_IF
        END_IF
        ELSE
        // Fists out: above 35 HP it throws the boomerang.
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_GREATER(LW(0), 35)
            CALL_CHILD(PTR(boomerang_attack_event))
            GOTO(99)
            ELSE
            // 35 HP or less: the audience attack first if unused, then boomerang 75 or audience attack 20.
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AUD_TALK_DONE, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(get_aud_num, LW(0))
                IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                    CALL_CHILD(PTR(aud_attack_event))
                    GOTO(99)
                END_IF
            END_IF
            LABEL(20)
            SET_VALUE(LW(0), 0)
            ADD(LW(0), 75)
            ADD(LW(0), 20)
            SUBTRACT(LW(0), 1)
            CALL(evt_sub_random, LW(0), LW(1))
            SET_VALUE(LW(0), 75)
            IF_INT_LESS(LW(1), LW(0))
                CALL_CHILD(PTR(boomerang_attack_event))
                GOTO(99)
            END_IF
            ADD(LW(0), 20)
            IF_INT_LESS(LW(1), LW(0))
                CALL(get_aud_num, LW(0))
                IF_INT_GREATER_OR_EQUAL(LW(0), 20)
                    CALL_CHILD(PTR(aud_attack_event))
                    GOTO(99)
                END_IF
            END_IF
            GOTO(20)
        END_IF
    END_IF
    LABEL(99)
    // The facing is reset and it goes back to waiting.
    CALL(btlevtcmd_ResetFaceDirection, -2)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Magnus Drill Attack of Magnus von Grapple 2.0 (weapon_batten_drill_attack, one target): the first time
 * (unit work 5) Crump announces it (stg7_aji_41_02). The boss winds up (MGN_A3_1 to 3 with
 * SFX_BOSS_MBTN_DRILL_ATTACK1 to 3), jets up off screen,
 * comes down upside down over the party in a tornado effect
 * through the floor with a camera shake, deals the damage with the usual dodge and guard checks,
 * then reappears at the right edge and walks home.
 */
EVT_BEGIN(drill_attack_event)
    // Choose a target.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_batten_drill_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_batten_drill_attack), LW(3), LW(4))
    // No valid target: a confused boss is flustered, otherwise the turn is skipped.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    // The first time (unit work 5), Crump announces the drill attack over a close-up of the boss
    // (stg7_aji_41_02).
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_DRILL_TALK_DONE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_DRILL_TALK_DONE, 1)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
        CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 20)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
        WAIT_FRAMES(20)
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg7_aji_41_02"), 0, -2)
        CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
        WAIT_MS(500)
    END_IF
    // The attack is declared and the guard window opens.
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_batten_drill_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_batten_drill_attack))
    // Wind-up: it crouches and fires its jets (MGN_A3_1, MGN_A3_2).
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_DRILL_ATTACK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A3_1"))
    WAIT_MS(800)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_DRILL_ATTACK2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A3_2"))
    BEGIN_SIBLING_EVENT
        CALL(drill_jet, UNIT_ID_SELF)
    END_SIBLING_EVENT
    WAIT_MS(300)
    // It jets straight up and off the top of the screen.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(300)
    // It reappears high above the stage upside down and plunges back down in a tornado, drill first
    // (MGN_A3_3).
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, -125, 500, 0)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 180, 0, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A3_3"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_DRILL_ATTACK3"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -125, -200, 0, 60, 0, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    BEGIN_INLINE_EVENT
        CALL(drill_tor, UNIT_ID_SELF)
    END_INLINE_EVENT
    LOOP(0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(1), 200)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    // The camera shakes as the drill bores through the floor.
    CALL(evt_btl_camera_shake_h, 0, 7, 0, 80, 0)
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(97)
    END_IF
    // Each target in turn gets the hit check and then the damage; the action-command result is read once.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_batten_drill_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    GOTO(95)
    LABEL(91)
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_batten_drill_attack))
        SET_VALUE(LW(6), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_batten_drill_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(95)
    LABEL(95)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(10)
    LABEL(97)
    // It keeps drilling until it has passed below the floor out of sight.
    LOOP(0)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(1), -200)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    // The camera returns to normal.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(200)
    // It reappears upright at the right edge of the stage and walks back to its home position.
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 400, LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_WALK)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_MovePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, MOVE_SOUND_FROM_TABLE, 0)
    RETURN
EVT_END()

EVT_BEGIN(boomerang_attack_event)
    // Choose a target; the boomerang flies across every target.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_batten_boomerang_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_batten_boomerang_attack), LW(3), LW(4))
    // No valid target: a confused boss is flustered, otherwise the turn is skipped.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    // The attack is declared and the guard window opens.
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_batten_boomerang_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_batten_boomerang_attack))
    // The boss splits its boomerang part (part 4) off its body (MGN_A6_1).
    BEGIN_SIBLING_EVENT
        WAIT_MS(900)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_BOOME_DIVIDE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A6_1"))
    WAIT_MS(1160)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_BOOME_ROUND1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 70)
    ADD(LW(1), 70)
    CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPartsRotate, UNIT_ID_SELF, 4, 0, 55, 65)
    BEGIN_SIBLING_EVENT
        WAIT_MS(1600)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_BOOME_ACTIVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("MGN_A4_2"))
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kHidden)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kNoShadow)
    BEGIN_SIBLING_EVENT
        WAIT_MS(2500)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 4, STRING("MGN_A4_3"))
    END_SIBLING_EVENT
    // The boomerang swings in two short arcs in front of the boss.
    CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2))
    ADD(LW(1), 30)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2), 80, -40, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 25)
    CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2), 80, 40, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    // Then it sails across the whole party to the far side of the stage.
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_DivePartsPosition, UNIT_ID_SELF, 4, -400, 120, LW(2), 40, -20, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_BOOME_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_FRAMES(20)
    SET_VALUE(LW(6), 0)
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    IF_INT_EQUAL(LW(3), -1)
        GOTO(97)
    END_IF
    // Each target in turn gets the hit check and then the damage as the boomerang passes; the action-command
    // result is read once.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_batten_boomerang_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    GOTO(95)
    LABEL(91)
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_batten_boomerang_attack))
        SET_VALUE(LW(6), 1)
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_batten_boomerang_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(95)
    LABEL(95)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    GOTO(10)
    LABEL(97)
    WAIT_MS(700)
    // The boomerang part is hidden again and the camera returns to normal.
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 4, kHidden)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 4, kNoShadow)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(200)
    RETURN
EVT_END()

/**
 * Audience attack of Magnus von Grapple 2.0 (weapon_batten_aud_attack, one target): the first time
 * (unit work 7) Crump talks in close-up before it (stg7_aji_41_04) and again before firing (stg7_aji_41_05).
 * The boss sucks 20 audience members into its pod (aud_suikomi), takes aim (aud_target_mark)
 * and fires them at the target in ten rounds of two (aud_attack_event_sub).
 */
EVT_BEGIN(aud_attack_event)
    // Choose a target.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_batten_aud_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_batten_aud_attack), LW(3), LW(4))
    // No valid target: a confused boss is flustered, otherwise the turn is skipped.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        RETURN
    END_IF
    // The first time (unit work 7), Crump talks over a close-up of the boss before the attack
    // (stg7_aji_41_04).
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AUD_TALK_DONE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
        CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 20)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("MGN_T_1"))
            CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("MGN_S_1"))
        ELSE
            CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("MGN_T_2"))
            CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("MGN_S_2"))
        END_IF
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg7_aji_41_04"), 0, -2)
        CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
        WAIT_MS(500)
    END_IF
    // The attack is declared and the guard window opens.
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_batten_aud_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_batten_aud_attack))
    WAIT_FRAMES(30)
    BEGIN_SIBLING_EVENT
        WAIT_MS(200)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_MOVE1"), EVT_NULLPTR, 0, LW(15))
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_MOVE1"), EVT_NULLPTR, 0, LW(15))
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_MOTOR_ACTIVE1"), EVT_NULLPTR, 0, LW(15))
        WAIT_MS(700)
        CALL(evt_snd_sfxoff, LW(15))
    END_SIBLING_EVENT
    // Wind-up: the boss opens its pod and fires its jets (a different pose set is used with the fists out).
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_1"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_1"))
    END_IF
    WAIT_MS(1600)
    BEGIN_SIBLING_EVENT
        CALL(drill_jet, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_2"))
    END_IF
    WAIT_MS(300)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    // It rises on its jets while the camera moves to the audience and the audience area darkens.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    WAIT_MS(300)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_AUDIENCE)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_batstage_set_aud_dark, 0, 30, 0)
    BEGIN_SIBLING_EVENT
        CALL(aud_jet, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_7"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_7"))
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(2), 150)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), 25, LW(2), 60, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_3"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_3"))
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_INHALE1"), EVT_NULLPTR, 0, LW(15))
    // It sucks 20 audience members into its pod.
    CALL(aud_suikomi, UNIT_ID_SELF)
    BEGIN_SIBLING_EVENT
        CALL(aud_scream_fadeout)
    END_SIBLING_EVENT
    CALL(evt_snd_sfxoff, LW(15))
    // It sinks back down and the jets shut off.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_7"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_7"))
    END_IF
    WAIT_MS(300)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(aud_jet_end)
    WAIT_MS(300)
    // The audience lighting comes back, the camera returns and the boss goes back to its home position.
    CALL(evt_batstage_return_aud_dark_base, 30, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    BEGIN_SIBLING_EVENT
        CALL(drill_jet, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_2"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_2"))
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 300)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_MOTOR_ACTIVE1"), EVT_NULLPTR, 0, LW(15))
        WAIT_MS(600)
        CALL(evt_snd_sfxoff, LW(15))
        WAIT_MS(400)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_CHEST_OPEN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_4"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_4"))
    END_IF
    WAIT_MS(1200)
    // The first time (unit work 7 is 0), Crump speaks again before the shots (stg7_aji_41_05).
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AUD_TALK_DONE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AUD_TALK_DONE, 1)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
        CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 20)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("MGN_T_3"))
            CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("MGN_A2_8"))
        ELSE
            CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("MGN_T_4"))
            CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("MGN_A5_8"))
        END_IF
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg7_aji_41_05"), 0, -2)
        CALL(btlevtcmd_SetTalkPoseType, UNIT_ID_SELF, 65)
        CALL(btlevtcmd_SetStayPoseType, UNIT_ID_SELF, 69)
        CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
        WAIT_MS(500)
    END_IF
    // It takes aim at the target (aud_target_mark).
    CALL(btlevtcmd_GetPartsPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    ADD(LW(1), 20)
    CALL(aud_target_mark, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_5"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_5"))
    END_IF
    SET_VALUE(LW(10), 0)
    // It fires the audience members at the target in ten rounds of two (aud_attack_event_sub).
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AUD_HIT, 0)
    LOOP(10)
        WAIT_MS(250)
        CALL_CHILD(aud_attack_event_sub)
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), -10)
    ADD(LW(1), 78)
    WAIT_MS(250)
    CALL(evt_eff, STRING(""), STRING("machingegun"), 0, LW(0), LW(1), LW(2), 180, FLOAT(8.0f), FLOAT(2.0996094f), 0, 0, 0, 0, 0)
    WAIT_MS(250)
    CALL(evt_eff, STRING(""), STRING("machingegun"), 0, LW(0), LW(1), LW(2), 170, FLOAT(8.0f), FLOAT(2.0996094f), 0, 0, 0, 0, 0)
    WAIT_MS(250)
    CALL(evt_eff, STRING(""), STRING("machingegun"), 0, LW(0), LW(1), LW(2), 185, FLOAT(8.0f), FLOAT(2.0996094f), 0, 0, 0, 0, 0)
    // The boss closes its pod again and the camera returns to normal.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_8"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_8"))
    END_IF
    WAIT_MS(1000)
    BEGIN_SIBLING_EVENT
        WAIT_MS(100)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_CHEST_OPEN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        WAIT_MS(300)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_MOVE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A2_6"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A5_6"))
    END_IF
    WAIT_MS(1000)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_MS(200)
    RETURN
EVT_END()

EVT_BEGIN(aud_attack_event_sub)
    IF_INT_EQUAL(LW(10), 9)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_AUD_HIT, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            GOTO(91)
        END_IF
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_MACHINEGUN1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_batten_aud_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    SET_VALUE(LW(11), LW(10))
    MULTIPLY(LW(11), 2)
    CALL(aud_fire_no_hit, UNIT_ID_SELF, LW(3), LW(11))
    GOTO(95)
    LABEL(91)
    SET_VALUE(LW(11), LW(10))
    MULTIPLY(LW(11), 2)
    CALL(aud_fire_hit, UNIT_ID_SELF, LW(3), LW(11))
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_batten_aud_attack))
    IF_INT_EQUAL(LW(10), 9)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_batten_aud_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_batten_aud_attack), 0, LW(5))
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_AUD_HIT, 1)
    GOTO(95)
    LABEL(95)
    ADD(LW(10), 1)
    RETURN
EVT_END()

/**
 * Separates the boss's arms (bunri = separation):
 * sets unit work 0 (arms detached), 2 (separated once) and 1 = 2,
 * shows a close-up with stg7_aji_41_03 the first time, plays poses MGN_A1_1 to 3,
 * spawns two rocket punch units (unit_boss_rocket_punch_mkII, left and right fists) whose ids go in unit work 3
 * and 4, and switches to the pose table without arms.
 */
static EVT_BEGIN(arm_bunri_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_LAUNCHED, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
        CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        SUBTRACT(LW(0), 20)
        CALL(evt_btl_camera_set_moveto, 1, LW(0), 160, 600, LW(0), 110, -100, 20, 11)
        WAIT_FRAMES(20)
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg7_aji_41_03"), 0, -2)
        CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
        WAIT_MS(500)
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_LAUNCHED, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARM_RETURN_TURNS, 2)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_ACTIVE1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A1_1"))
    WAIT_MS(600)
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(200)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_STOP1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A1_2"))
    WAIT_MS(500)
    CALL(evt_snd_sfxoff, LW(15))
    WAIT_MS(100)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A1_3"))
    WAIT_MS(100)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_IGNITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_w, 0, 3, 0, 20, 0)
    WAIT_MS(100)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_rocket_punch), 0)
    CALL(btlevtcmd_SetUnitWork, LW(3), ROCKET_PUNCH_WORK_SIDE, 0)
    CALL(btlevtcmd_SetUnitWork, LW(3), ROCKET_PUNCH_WORK_WAIT, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARM_0_UNUSED, LW(3))
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), 1, POSE_TYPE_ACTION)
    CALL(btlevtcmd_SpawnUnit, LW(4), PTR(&entry_rocket_punch), 0)
    CALL(btlevtcmd_SetUnitWork, LW(4), ROCKET_PUNCH_WORK_SIDE, 1)
    CALL(btlevtcmd_SetUnitWork, LW(4), ROCKET_PUNCH_WORK_WAIT, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARM_1_UNUSED, LW(4))
    CALL(btlevtcmd_AnimeChangePoseType, LW(4), 1, POSE_TYPE_ACTION)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_no_arm))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    WAIT_MS(1500)
    RETURN
EVT_END()

/**
 * Calls the fists back: clears unit work 0, plays pose MGN_A1_6 for 0.6 seconds,
 * spawns two fresh rocket punch units (work 0 = 2 and 3, left and right) whose ids go in unit work 3 and 4
 * and switches back to the pose table with arms.
 */
static EVT_BEGIN(arm_return_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARMS_OUT, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A1_6"))
    WAIT_MS(600)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_rocket_punch), 0)
    CALL(btlevtcmd_SetUnitWork, LW(3), ROCKET_PUNCH_WORK_SIDE, 2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARM_0_UNUSED, LW(3))
    CALL(btlevtcmd_AnimeChangePoseType, LW(3), 1, POSE_TYPE_ACTION)
    CALL(btlevtcmd_SpawnUnit, LW(4), PTR(&entry_rocket_punch), 0)
    CALL(btlevtcmd_SetUnitWork, LW(4), ROCKET_PUNCH_WORK_SIDE, 3)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_ARM_1_UNUSED, LW(4))
    CALL(btlevtcmd_AnimeChangePoseType, LW(4), 1, POSE_TYPE_ACTION)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_SetPos, LW(3), 300, 40, -100)
        CALL(btlevtcmd_DivePosition, LW(3), 30, 100, -40, 30, -10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetPos, LW(4), 350, 40, 100)
    CALL(btlevtcmd_DivePosition, LW(4), 80, 100, 40, 30, -10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    CALL(arm_end, 0)
    CALL(arm_end, 1)
    CALL(btlevtcmd_KillUnit, LW(3), 0)
    CALL(btlevtcmd_KillUnit, LW(4), 0)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_IGNITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_w, 0, 3, 0, 20, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A1_7"))
    WAIT_MS(300)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A1_8"))
    WAIT_MS(600)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_ACTION)
    WAIT_MS(500)
    RETURN
EVT_END()

/** Idle script of Magnus von Grapple 2.0: sets the idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

/**
 * Unit data of the X-Punch fist of Magnus von Grapple 2.0: 5 HP, level 0; the battle starts it from
 * init_event_r.
 */
BattleUnitKind unit_boss_rocket_punch_mkII = {
    UNIT_X_PUNCH,
    "btl_un_rocket_punch_mkii",
    5,
    0,
    1,
    1,
    0,
    0,
    0,
    100,
    0,
    50,
    10,
    50,
    34,
    0,
    15,
    0.0f,
    0.0f,
    0.0f,
    0,
    -20,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    25.0f,
    0.0f,
    0.0f,
    24.0f,
    16.0f,
    0.0f,
    -5.0f,
    0.0f,
    0.0f,
    50.0f,
    34.0f,
    0,
    0,
    -1,
    0,
    100,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_BOSS_MBTN_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kFloating,
    &regist_r,
    1,
    parts_r,
    init_event_r,
    data_table_r,
};

/** Defence of the X-Punch fist of Magnus von Grapple 2.0: none against any element. */
static BattleUnitDefense defence_r = {{0, 0, 0, 0, 0}};

/**
 * Elemental reactions of the X-Punch fist of Magnus von Grapple 2.0: none, every element hurts it normally.
 */
static BattleUnitDefenseAttr defence_attr_r = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist_r = {
    0, // sleep
    70, // stop
    70, // dizzy
    100, // poison
    70, // confuse
    100, // electric
    100, // burn
    70, // freeze
    0, // huge
    0, // tiny
    100, // attackUp
    0, // attackDown
    100, // defenseUp
    85, // defenseDown
    70, // allergic
    0, // fright
    100, // galeForce
    100, // fast
    70, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

/** Pose table of the X-Punch fist: its flying and stay poses. */
static PoseTableEntry pose_table_r[] = {
    {POSE_TYPE_SLEEP, "MGN_A1_9"},
    {POSE_TYPE_STOP, "MGN_A1_4"},
    {POSE_TYPE_FREEZE, "MGN_A1_4"},
    {POSE_TYPE_CONFUSE, "MGN_A1_9"},
    {POSE_TYPE_POISON, "MGN_A1_9"},
    {POSE_TYPE_DIZZY, "MGN_A1_9"},
    {POSE_TYPE_NORMAL, "MGN_A1_9"},
    {POSE_TYPE_DAMAGE_FIRE, "MGN_A1_9"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "MGN_A1_9"},
    {POSE_TYPE_DAMAGE_GUARDED, "MGN_A1_9"},
    {POSE_TYPE_DAMAGE, "MGN_A1_9"},
    {POSE_TYPE_DANGER, "MGN_A1_9"},
    {POSE_TYPE_PERIL, "MGN_A1_9"},
    {POSE_TYPE_ACTION, "MGN_A1_9"},
};

/** Event table of the X-Punch fist of Magnus von Grapple 2.0: empty, it has no special reactions. */
static BattleDataEntry data_table_r[] = {
    {0, NULL},
};

/** Part of the X-Punch fist: its single flying fist part (the target). */
static BattleUnitKindPart parts_r[] = {
    {1, "btl_un_rocket_punch_mkii", "c_mb_robo_2b", 0.0f, 0.0f, 0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 10.0f, 0.0f, 106, 174, 255, &defence_r, &defence_attr_r, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget, kCounterPreemptiveFrontSpiky, pose_table_r},
};

/**
 * Weapon data for the X-Punch fist of Magnus von Grapple 2.0's rocket punch: 9 damage, a jump-type hit, one
 * target.
 */
static BattleWeapon weapon_batten_rocket_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {9, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_JUMPLIKE | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0x2004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/**
 * Battle init for a rocket punch fist of Magnus von Grapple 2.0 (unit work 0: 0 and 2 are the left fist,
 * 1 and 3 the right): registers its wait, attack, damage and confusion events, turns it sideways,
 * sets up the arm physics (arm_init2), starts arm_event and, for the freshly separated fists (work 0 or 1),
 * makes it fly out in an arc with SFX_BOSS_MBTN_PUNCH_FLY1 and settle at its hovering home position.
 */
static EVT_BEGIN(init_event_r)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A1_4"))
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event_r))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_r))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event_r))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetBaseRotate, UNIT_ID_SELF, 0, -90, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, ROCKET_PUNCH_WORK_SIDE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(arm_init2, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        CALL(arm_init2, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(arm_init2, 1)
    END_IF
    IF_INT_EQUAL(LW(0), 3)
        CALL(arm_init2, 1)
    END_IF
    SPAWN_EVENT(arm_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, ROCKET_PUNCH_WORK_SIDE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 30, 100, -40)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -100, 150, -60, 20, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, 350, 110, -80, 30, 50, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, 30, 60, -40, 25, -30, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, 30, 60, -40)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 80, 100, 40)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -50, 150, 60, 20, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, 300, 120, 80, 30, 50, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, 80, 60, 40, 30, -30, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, 80, 60, 40)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(arm_event)
    LABEL(0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL_CHILD(PTR(arm_event_end))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, ROCKET_PUNCH_WORK_SIDE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(arm_main, UNIT_ID_SELF, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        CALL(arm_main, UNIT_ID_SELF, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(arm_main, UNIT_ID_SELF, 1)
    END_IF
    IF_INT_EQUAL(LW(0), 3)
        CALL(arm_main, UNIT_ID_SELF, 1)
    END_IF
    GOTO(0)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(arm_event_end)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, ROCKET_PUNCH_WORK_SIDE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(arm_end, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 2)
        CALL(arm_end, 0)
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        CALL(arm_end, 1)
    END_IF
    IF_INT_EQUAL(LW(0), 3)
        CALL(arm_end, 1)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(damage_event_r)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(arm_event_end))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_event_r)
    CALL(btlevtcmd_AddUnitWork, UNIT_ID_SELF, ROCKET_PUNCH_WORK_WAIT, -1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, ROCKET_PUNCH_WORK_WAIT, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, ROCKET_PUNCH_WORK_WAIT, 1)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_batten_rocket_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_batten_rocket_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
        END_IF
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(evt_sub_random, 1, LW(6))
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_batten_rocket_attack))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_batten_rocket_attack))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_UNIT_TO_UNIT)
    CALL(evt_btl_camera_set_homing_unit, 0, UNIT_ID_SELF, LW(3))
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 2)
    CALL(evt_btl_camera_set_zoom, 0, 200)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("MGN_A1_4"))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_batten_rocket_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    GOTO(95)
    LABEL(91)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(10), LW(11), LW(12))
            IF_INT_LESS_OR_EQUAL(LW(10), LW(0))
                CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_batten_rocket_attack))
                CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_batten_rocket_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
                CALL(evt_btl_camera_shake_w, 0, 3, 0, 10, 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    GOTO(95)
    LABEL(95)
    IF_INT_EQUAL(LW(6), 0)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 110)
        SET_VALUE(LW(1), 120)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(5)
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -20)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, -250, 0, LW(2), 20, -10, EASING_LINEAR, 0, MOVE_SOUND_FROM_TABLE)
    ELSE
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), -200)
        ADD(LW(1), 140)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 40, -80, EASING_QUADRATIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    END_IF
    WAIT_MS(250)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, ROCKET_PUNCH_WORK_SIDE, LW(10))
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 300, LW(1), -100)
    ELSE
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, 300, LW(1), 100)
    END_IF
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_MBTN_PUNCH_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    LABEL(99)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event_r)
    CALL(btlevtcmd_SetRotate, UNIT_ID_SELF, 0, 0, 0)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    LABEL(0)
    WAIT_FRAMES(1)
    GOTO(0)
    RETURN
EVT_END()

static USER_FUNC(arm_init) {
    s32 i;

    for (i = 0; i < 2; i++) {
        work[i] = BattleAlloc(sizeof(ArmWork));
        work[i]->prevPos = work[i]->pos = (Vec){0.0f, 0.0f, 0.0f};
    }
    return EVT_RETURN_DONE;
}

/** Script function: creates the arm effect (effBattenEntry type 3) for the arm args[0]. */
static USER_FUNC(arm_init2) {
    s32 idx = evtGetValue(event, event->args[0]);
    ArmWork* arm = work[idx];

    arm->eff = effBattenEntry(3, 1000, 0.0f, -1000.0f, 0.0f, 1.0f);
    return EVT_RETURN_DONE;
}

/**
 * Script function: each frame, places the arm effect for the arm args[1] on the unit args[0],
 * tilts it by the direction of travel (smoothed over three frames) and rotates it with the unit's rotation,
 * with a small offset (0, -18, 0) along its own axis.
 */
static USER_FUNC(arm_main) {
    EvtWord* args = event->args;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;
    s32 id = evtGetValue(event, args[0]);
    s32 idx = evtGetValue(event, args[1]);
    ArmWork* arm = work[idx];
    BattenEffWork* eff;
    f32 x, y, z;
    f32 rx, ry, rz;
    f32 bx, by, bz;
    Mtx m;
    Mtx rot;
    Vec ofs;
    f32 dx, dy;

    if (arm->eff == NULL) {
        return EVT_RETURN_DONE;
    }
    eff = arm->eff->userdata;
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    BtlUnit_GetPos(unit, &x, &y, &z);
    arm->prevPos = arm->pos;
    {
        Vec p = {0.0f, 0.0f, 0.0f};

        p.x = x;
        p.y = y;
        p.z = z;
        arm->pos = p;
    }
    if (evtCheckID(unit->waitEventId) != TRUE) {
        dx = arm->pos.x - arm->prevPos.x;
        dy = arm->pos.y - arm->prevPos.y;
        if (dx != 0.0f) {
            arm->targetRot = 360.0f * (f32)atan(dy / dx) / 6.283184f;
            arm->rot += (arm->targetRot - arm->rot) / 3.0f;
            BtlUnit_GetRotate(unit, &rx, &ry, &rz);
            BtlUnit_SetRotate(unit, rx, ry, arm->rot);
        } else {
            arm->targetRot = 0.0f;
        }
    }
    BtlUnit_GetBaseRotate(unit, &bx, &by, &bz);
    BtlUnit_GetRotate(unit, &rx, &ry, &rz);
    rx += bx;
    ry += by;
    rz += bz;
    PSMTXRotRad(m, 'x', 1.5707964f);
    PSMTXRotRad(rot, 'y', 0.017453292f * ry);
    PSMTXConcat(rot, m, m);
    PSMTXRotRad(rot, 'x', 0.017453292f * rx);
    PSMTXConcat(rot, m, m);
    PSMTXRotRad(rot, 'z', 0.017453292f * rz);
    PSMTXConcat(rot, m, m);
    PSMTXCopy(m, eff->mtx);
    ofs = (Vec){0.0f, -18.0f, 0.0f};
    PSMTXMultVec(m, &ofs, &ofs);
    eff->pos.x = arm->pos.x + ofs.x;
    eff->pos.y = arm->pos.y + ofs.y;
    eff->pos.z = arm->pos.z + ofs.z;
    return EVT_RETURN_DONE;
}

/** Script function: removes the arm effect of arm args[0]. */
static USER_FUNC(arm_end) {
    s32 idx = evtGetValue(event, event->args[0]);
    ArmWork* arm = work[idx];

    if (arm->eff != NULL) {
        effDelete(arm->eff);
        arm->eff = NULL;
    }
    return EVT_RETURN_DONE;
}
