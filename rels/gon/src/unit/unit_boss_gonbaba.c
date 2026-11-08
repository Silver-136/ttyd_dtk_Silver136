// Hooktail (boss_gonbaba): boss battle unit used in Hooktail Castle.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_sub.h"
#include "eff/n64/eff_kemuri9_n64.h"
#include "battle/unit_work.h"

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
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "battle/battle_database_common.h"
#include "battle/battle_damage.h"
#include "battle/unit/unit_boss_gonbaba.h"

u8* lightGetPaperCraft(void);

static USER_FUNC(_leg_smoke_effect);
USER_FUNC(_btlgon_get_escape_param);
USER_FUNC(_set_gonbaba_light);
USER_FUNC(_wait_damage_event_end);

BattleWeapon weapon_gonbaba_fumituke_attack;
BattleWeapon weapon_gonbaba_firebreath_attack;
BattleWeapon weapon_gonbaba_kamituki_attack;
BattleUnitKind unit_boss_gonbaba;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;

#if VERSION <= VERSION_DEMO_JP
static PoseTableEntry pose_table[7];
static PoseTableEntry pose_table_weak[7];
#else
static PoseTableEntry pose_table[9];
static PoseTableEntry pose_table_weak[9];
#endif

static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[3];
static EvtWord battle_entry_event[375];
EvtWord negotiation_event[1786];
EvtWord gon_extra_eaten_event[389];
static EvtWord damage_event[10];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord gonbaba_damage_sub_event[];
#else
EvtWord gonbaba_damage_sub_event[574];
#endif

static EvtWord dead_event[283];
static EvtWord init_event[160];
static EvtWord phase_event[2];
static EvtWord attack_event[56];
static EvtWord fumituke_attack_event[386];
EvtWord firebreath_attack_event[261];
static EvtWord wait_event[6];

/** Hooktail's attack power: the weapon's base power minus the attack drop in the attacker's work[0] (at least 1). */
u32 weaponGetPower_Gonbaba(BattleUnit* unit, BattleWeapon* weapon, BattleUnit* target, BattleUnitPart* part) {
    s32 power = weapon->hpDamageWork[0];

    power -= unit->work[UNIT_WORK_POWER_DOWN];
    if (power <= 0) {
        power = 1;
    }
    return power;
}

/**
 * Script helper: spawns a puff of dust (an N64-style kemuri effect) at the position (args[0], args[1], args[2]);
 * used for Hooktail's footsteps.
 */
static USER_FUNC(_leg_smoke_effect) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    effKemuri9N64Entry(1, 18, 90, x, y, z, 60.0f, 0.0f);
    return EVT_RETURN_DONE;
}

/**
 * Weapon data for Hooktail's stomp: damage from its own formula (weaponGetPower_Gonbaba), a hammer-type hit,
 * one target.
 */
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, // element
    ATTACKPATTERN_PRESS_GONBABA, // damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_FRONT | TARGET_WEIGHT_UNUSED_2000, // targetWeightFlags
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

/**
 * Weapon data for Hooktail's fire breath: damage from its own formula (weaponGetPower_Gonbaba), fire element,
 * every target.
 */
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

/** Weapon data for Hooktail's bite: 5 damage, every target. */
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
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

/** Unit data of Hooktail: 20 HP, level 65; the battle starts it from init_event. */
BattleUnitKind unit_boss_gonbaba = {
    UNIT_HOOKTAIL,
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
#if VERSION <= VERSION_DEMO_JP
    0.0f,
    0.0f,
        0.0f,
    250.0f,
#else
#if VERSION >= VERSION_US
    115.0f,
#else
    0.0f,
#endif
    0.0f,
    0.0f,
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

/** Defence of Hooktail: 1 against every element. */
static BattleUnitDefense defence = {{1, 1, 1, 1, 1}};

/** Elemental reactions of Hooktail: none, every element hurts it normally. */
static BattleUnitDefenseAttr defence_attr = {{DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE, DEFENSE_ATTR_NONE}};

#if VERSION <= VERSION_DEMO_JP
static StatusVulnerability regist = {10, 0, 50, 0, 50, 10, 100, 0, 0, 0, 50, 0, 100, 0, 100, 0, 0, 100, 0, 0, 0, 0};
#else
static StatusVulnerability regist = {
    20, // sleep
    0, // stop
    50, // dizzy
    0, // poison
    50, // confuse
    10, // electric
    100, // burn
    0, // freeze
    0, // huge
    0, // tiny
    50, // attackUp
    0, // attackDown
    100, // defenseUp
    0, // defenseDown
    100, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};
#endif

/** Pose table of Hooktail: the animation each battle pose (idle, hurt, walk, attack and so on) plays. */
static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "GNB_N_1"},
    {POSE_TYPE_CONFUSE, "GNB_X_1"},
#if VERSION >= VERSION_JP
    {POSE_TYPE_STOP, "GNB_Z_1"},
    {POSE_TYPE_DIZZY, "GNB_X_1"},
#endif
    {POSE_TYPE_NORMAL, "GNB_S_3"},
    {POSE_TYPE_TALK, "GNB_T_3"},
    {POSE_TYPE_DAMAGE_GUARDED, "GNB_S_3"},
    {POSE_TYPE_DAMAGE, "GNB_D_1"},
    {POSE_TYPE_ACTION, "GNB_S_3"},
};

/**
 * Pose table of Hooktail once she is weakened, installed by gonbaba_damage_sub_event on each hit with the
 * Attack FX R sound (which she takes for a cricket): stay and talk become GNB_S_4 and GNB_T_4, pose 39
 * becomes GNB_V_1. negotiation_event puts the normal table back.
 */
static PoseTableEntry pose_table_weak[] = {
    {POSE_TYPE_SLEEP, "GNB_N_1"},
    {POSE_TYPE_CONFUSE, "GNB_X_1"},
#if VERSION >= VERSION_JP
    {POSE_TYPE_STOP, "GNB_N_1"},
    {POSE_TYPE_DIZZY, "GNB_X_1"},
#endif
    {POSE_TYPE_NORMAL, "GNB_S_4"},
    {POSE_TYPE_TALK, "GNB_T_4"},
    {POSE_TYPE_DAMAGE_GUARDED, "GNB_S_4"},
    {POSE_TYPE_DAMAGE, "GNB_V_1"},
    {POSE_TYPE_ACTION, "GNB_S_4"},
};

/**
 * Event table of Hooktail: its damage handler (gonbaba_damage_sub_event) and its defeat (dead_event).
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, gonbaba_damage_sub_event},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

/**
 * Parts of Hooktail: part 1 is her body, part 2 her head, and part 3 a hidden piece shown while she eats the
 * audience.
 */
static BattleUnitKindPart parts[] = {
    {
        1, // partNum
        "btl_un_gonbaba", // partName
        "c_gonbaba", // modelName
        {0.0f, 0.0f, 0.0f}, // positionOffset
        {230.0f, 36.0f, 60.0f}, // hitBaseOffset
        {230.0f, 36.0f, 60.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
#if VERSION >= VERSION_US
        kImportantPart | kPartUnitWorkDefense, // attributes
#else
        0x22, // attributes
#endif
        0, // counterAttributes
        pose_table, // poseTable
    },
    {
        2, // partNum
        "btl_un_gonbaba", // partName
        "c_gonbaba", // modelName
        {100.0f, 0.0f, 0.0f}, // positionOffset
        {250.0f, 110.0f, 25.0f}, // hitBaseOffset
        {250.0f, 110.0f, 25.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
#if VERSION >= VERSION_US
        kMainPart | kGuardTargetPart | kPartUnitWorkDefense | kHammerlikeCannotTarget | kShellTosslikeCannotTarget | kHidden, // attributes
#else
        0x1600029, // attributes
#endif
        0, // counterAttributes
        pose_table, // poseTable
    },
    {
        3, // partNum
        "btl_un_gonbaba", // partName
        "c_gonbaba4", // modelName
        {0.0f, 0.0f, 0.0f}, // positionOffset
        {0.0f, 0.0f, 0.0f}, // hitBaseOffset
        {0.0f, 0.0f, 0.0f}, // hitCursorBaseOffset
        20, // shadowSize
        30, // hitHeight
        255, // baseAlpha
        &defence, // defense
        &defence_attr, // defenseAttr
        kNotTargetable | kHidden, // attributes
        0, // counterAttributes
        pose_table, // poseTable
    },
};

/**
 * Entry event of the Hooktail fight (replays the "dontyo" map animation):
 * the camera moves in while Mario and his partner are placed off-stage,
 * then walk in and strike a startled pose (M_I_Y/M_I_O, SFX_VOICE_MARIO_SURPRISED2_2),
 * Hooktail rises (GNB_F_3, SFX_BOSS_GNB_APPEAR1) and speaks the opening line stg1_gon_20_3
 * before the battle starts.
 */
static EVT_BEGIN(battle_entry_event)
    // The opening cutscene: the camera moves in on the stage as the map animation replays.
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_map_replayanim, 0, STRING("dontyo"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 0, 110, 1080, 0, 93, -2, 1, 0)

    // Mario and his partner start off-stage and Hooktail is hidden above the stage.
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

    // Mario and his partner walk in and stop, startled (M_I_Y).
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
        CALL(btlevtcmd_MovePosition, UNIT_ID_PARTNER, LW(3), LW(4), LW(5), 0, MOVE_SOUND_FROM_TABLE, 0)
        CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(0), POSE_TYPE_NEUTRAL)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_MARIO, FLOAT(6.0f))
    CALL(btlevtcmd_MovePosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    WAIT_FRAMES(2)

    // Hooktail drops into view (GNB_F_3).
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_F_3"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_ftomsec, 10, LW(0))
    WAIT_MS(LW(0))

    // Mario and his partner jump in fright, Mario with a cry.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 5, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_O"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_MARIO, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_SURPRISED2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 5, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_PARTNER, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_PARTNER, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_PARTNER, LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(0), POSE_TYPE_CONFUSE)
    END_INLINE_EVENT

    // Her landing shakes the camera.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_shake_h, 1, 8, 0, 20, 13)
    WAIT_MS(500)

    // The camera swings to Hooktail, who says she will have the party for a snack and wonders who to taste
    // first (stg1_gon_20_3).
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(0), POSE_TYPE_NEUTRAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 60, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_3"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_3"))
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_20_3"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)

    // Everyone goes back to their wait scripts and the camera returns to normal.
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_MARIO)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    RETURN
EVT_END()

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
 * Hooktail's plea the first time her HP runs out (called from dead_event while unit work 3 is 0).
 * She begs for mercy (stg1_gon_23, stg1_gon_24) and offers coins, then a rare badge, then a sniff of her feet
 * (yes/no prompts stg1_gon_24, stg1_gon_26, stg1_gon_28).
 * Accepting any offer walks Mario up to her and she bites
 * him (stg1_gon_30, the partner calls her a coward, stg1_gon_30_03, stg1_gon_30_04); refusing all three gets
 * stg1_gon_31. Either way unit work 3 is set to 1 so the real defeat plays next time, she draws eight audience
 * members into the eating scene (gon_extra_eaten_event, spawned eight times; two escape) and recovers to 10 HP.
 */
EVT_BEGIN(negotiation_event)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_wait_move_end)

    // Hooktail has just run out of HP for the first time: she shakes off all status effects and the camera
    // moves in as she calls for a halt (stg1_gon_23).
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
    WAIT_FRAMES(20)
    CALL(btlevtcmd_reset_move_color_lv_all)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_4"))
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_23"), 0, -2)
    LABEL(0)

    // A loop waits until Mario is back at his home position.
    CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(3), LW(4), LW(5))
    IF_INT_NOT_EQUAL(LW(0), LW(3))
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF

    // She begs for mercy and offers 1,000 coins, with a yes/no prompt (stg1_gon_24).
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -431, 10, 735, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 0)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_24"), 0, -2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_24_yesno1"))
    CALL(evt_msg_continue)

    // Yes: she says money makes the world turn (stg1_gon_25) and moves on to her trap.
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 0)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_25"), 0, -2)
        GOTO(40)
    END_IF

    // No: Mario shakes his head and she offers a rare badge instead (stg1_gon_26).
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_NO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_MARIO, 1, PART_ANIM_RESTART, 1)
    WAIT_MS(667)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 1)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_26"), 0, -2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_26_yesno1"))
    CALL(evt_msg_continue)

    // Yes: she says of course he wants it (stg1_gon_27) and moves on to her trap.
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 1)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_27"), 0, -2)
        GOTO(40)
    END_IF

    // No again: she offers to let him sniff her feet instead (stg1_gon_28).
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_NO1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_MARIO, 1, PART_ANIM_RESTART, 1)
    WAIT_MS(667)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 1, 0)
    WAIT_FRAMES(5)
    SET_VALUE(LW(10), 2)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_28"), 0, -2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(20)
    CALL(evt_msg_select, 0, STRING("stg1_gon_28_yesno1"))
    CALL(evt_msg_continue)

    // Yes: she says she thought so (stg1_gon_29) and moves on to her trap.
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(5), 2)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_29"), 0, -2)
        GOTO(40)
    END_IF

    // Refused a third time, she grumbles that he is not very trusting (stg1_gon_31) and skips the trap.
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_NO2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_C"))
    CALL(btlevtcmd_AnimeFlagOnOff, UNIT_ID_MARIO, 1, PART_ANIM_RESTART, 1)
    WAIT_MS(1000)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 15, 0)
    WAIT_FRAMES(15)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_31"), 0, -2)
    GOTO(50)
    LABEL(40)

    // If Koops' shell shield is guarding Mario, it is dropped so the bite will land.
    CALL(btlevtcmd_GetGuardKouraId, UNIT_ID_MARIO, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_RunDataEventChild, LW(3), UNIT_DATA_KOURA_OPEN_EVENT)
        CALL(btlevtcmd_SetUnitWork, LW(3), SHELL_SHIELD_WORK_FOLLOW, 0)
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, POSE_TYPE_RUN)
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_MARIO, FLOAT(3.0f))

    // The trap: Mario walks up to her head and she bites down on him (GNB_A_2).
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 400)
    CALL(btlevtcmd_MovePosition, UNIT_ID_MARIO, LW(0), 0, LW(2), 0, MOVE_SOUND_FROM_TABLE, 0)
    IF_INT_EQUAL(LW(5), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_C_4"))
    ELSE
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_MARIO, 1, POSE_TYPE_NEUTRAL)
    END_IF
    WAIT_MS(700)
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

    // The bite deals damage to Mario with the action-command result.
    CALL(btlevtcmd_ResultACDefence, UNIT_ID_MARIO, PTR(&weapon_gonbaba_kamituki_attack))
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_MARIO, LW(4), PTR(&weapon_gonbaba_kamituki_attack), DAMAGE_RESULT_FINAL_HIT | DAMAGE_RESULT_NO_EVADE, LW(5))
    WAIT_MS(500)

    // If the bite leaves Mario with no HP, the event ends here unless he is revived; on a revival it waits for
    // his damage event to finish.
    CALL(btlevtcmd_GetHp, UNIT_ID_MARIO, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_MARIO, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
            RETURN
        END_IF
        WAIT_MS(1500)
        CALL(_wait_damage_event_end, UNIT_ID_MARIO)
    ELSE
        WAIT_MS(1500)
    END_IF

    // A loop waits for Mario to be back at his place (his positions are read only once, before the loop).
    CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_MARIO, LW(1), EVT_NULLPTR, EVT_NULLPTR)

    // BUG: the loop never re-reads LW(0)/LW(1), so it never ends if Mario was not at home when they were read.
    LABEL(41)
    IF_INT_NOT_EQUAL(LW(0), LW(1))
        WAIT_FRAMES(1)
        GOTO(41)
    END_IF

    // Koops' shell shield, if it was dropped, is put back.
    CALL(btlevtcmd_GetGuardKouraId, UNIT_ID_MARIO, LW(3))
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_SetUnitWork, LW(3), SHELL_SHIELD_WORK_FOLLOW, 1)
    END_IF

    // She gloats that they were duped (stg1_gon_30) and the partner calls her a coward (stg1_gon_30_01, _02).
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_30"), 0, -2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -115, 50, 265, -115, 49, 170, 1, 0)
    WAIT_FRAMES(5)
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(UNIT_GOOMBELLA)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_30_01"), 0, -4)
        CASE_EQUAL(UNIT_KOOPS)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_30_02"), 0, -4)
    END_SWITCH

    // She laughs at their gullibility (stg1_gon_30_03), then admits things look bad and that she had better
    // act (stg1_gon_30_04).
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -431, 10, 735, 56, 125, 37, 1, 0)
    WAIT_FRAMES(5)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_30_03"), 0, -2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 20, 0)
    WAIT_FRAMES(20)
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_30_04"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    LABEL(50)

    // Both paths meet here: her one plea is used up (unit work 3 is set to 1).
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, 1)

    // She turns toward the audience with heavy steps that shake the camera.
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

    // She moves to the front of the stage and the audience area goes dark.
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageAudienceAreaSize, EVT_NULLPTR, LW(3), EVT_NULLPTR)
    ADD(LW(1), LW(3))
    CALL(btlevtcmd_GetStageSize, EVT_NULLPTR, EVT_NULLPTR, LW(3))
    ADD(LW(2), LW(3))
    ADD(LW(2), 80)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(0), 260)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(_set_gonbaba_light)
    CALL(evt_btl_camera_shake_h, 1, 7, 0, 6000, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(600)
        CALL(evt_btl_camera_noshake, 1)
        CALL(evt_btl_camera_shake_h, 1, 7, 0, 30, 12)
    END_INLINE_EVENT
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_batstage_set_aud_dark, 0, 180, 0)
    CALL(evt_batstage_set_aud_dark_base, 0, 180, 0)
#else
    CALL(evt_batstage_set_aud_dark, 0, 24, 0)
    CALL(evt_batstage_set_aud_dark_base, 0, 24, 0)
#endif

    // Mario and his partner jump back in fright.
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_N_7"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_MARIO, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
        WAIT_FRAMES(60)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(40)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_PARTNER, FLOAT(0.69921875f))
        CALL(btlevtcmd_GetPos, UNIT_ID_PARTNER, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_PARTNER, LW(0), LW(1), LW(2), 20, MOVE_SOUND_FROM_TABLE)
    END_INLINE_EVENT
    SET_VALUE(LW(0), 13)

    // The eating scene plays gon_extra_eaten_event for eight spectators; spectators 32 and 53 escape.
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

    // She recovers to 10 HP with a healing sparkle.
    CALL(btlevtcmd_SetHp, UNIT_ID_SELF, 10)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(0), 40)
    ADD(LW(1), 180)
    ADD(LW(2), 70)
    CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), 10, 0, 0, 0, 0, 0, 0, 0)

    // She starts to chew (GNB_E_1) and the audience panics.
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_E_1"))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_EAT_APPEAR1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("GNB_P_1"))
        CALL(btlevtcmd_ftomsec, 14, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_ftomsec, 56, LW(0))
        WAIT_MS(LW(0))
    END_INLINE_EVENT
    CALL(btlevtcmd_ftomsec, 32, LW(0))
    WAIT_MS(LW(0))
    CALL(evt_snd_sfxon, STRING("SFX_AUDIENCE_GNB_PANIC1"), 0)
    SET_VALUE(LW(0), 0)

    // The spectators panic and most run for the exits and are removed; indices divisible by ten get running
    // sounds, while spectator 43 stays behind.
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

    // Spectators in slots 100 through 199 are removed.
    LOOP(100)
        CALL(evt_audience_delete, LW(0))
        CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
        ADD(LW(0), 1)
    END_LOOP
    CALL(btlevtcmd_ftomsec, 38, LW(0))
    WAIT_MS(LW(0))

    // Chewing sounds play while she eats.
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

    // The camera pulls back as she finishes.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 0, 140, 900, 0, 60, 0, 30, 0)
    CALL(btlevtcmd_ftomsec, 284, LW(0))
    WAIT_MS(LW(0))

    // The camera moves in on Hooktail and Mario's partner reacts (stg1_gon_31_00_kur, _nok).
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -115, 50, 200, -115, -30, -670, 20, 0)
    WAIT_FRAMES(25)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_S_5"))
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(UNIT_GOOMBELLA)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_31_00_kur"), 0, -4)
        CASE_EQUAL(UNIT_KOOPS)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_31_00_nok"), 0, -4)
    END_SWITCH
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 40, 98, 686, 57, 125, 182, 1, 0)
    WAIT_FRAMES(1)

    // She announces that her strength has returned (stg1_gon_31_01).
    CALL(btlevtcmd_SetTogeOffset, UNIT_ID_SELF, 310, 60, 0)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_5"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_5"))
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_31_01"), 0, -2)
    CALL(btlevtcmd_SetTogeOffset, UNIT_ID_SELF, 0, 0, 0)

    // The camera pulls back and the partner says it is time to finish her off (stg1_gon_31_02, _03).
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -115, 40, 200, -115, 39, 170, 1, 0)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_S_3"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 260)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    CALL(btlevtcmd_GetUnitKind, UNIT_ID_PARTNER, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(UNIT_GOOMBELLA)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_31_02"), 0, -4)
        CASE_EQUAL(UNIT_KOOPS)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_31_03"), 0, -4)
    END_SWITCH
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)

    // Mario and his partner jump back to their places, the turn counter is reset and the battle goes on.
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, 0, 110, 860, 0, 60, 170, 20, 0)
    WAIT_FRAMES(20)
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
        CALL(btlevtcmd_JumpPosition, UNIT_ID_PARTNER, LW(3), LW(4), LW(5), 27, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_SetHomePos, UNIT_ID_PARTNER, LW(3), LW(4), LW(5))
    END_INLINE_EVENT
    ADD(LW(1), LW(6))
    ADD(LW(2), LW(7))
    ADD(LW(2), 80)
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_MARIO, FLOAT(0.69921875f))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_J_1B"))
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_BATTLE_JUMP1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_JumpPosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 27, MOVE_SOUND_NONE)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_MARIO_JUMP_LANDING1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_MARIO, LW(3))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, LW(3), STRING("M_I_Y"))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(4))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(4), POSE_TYPE_ACTION)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_reset_turn)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(btlevtcmd_onoff_battleflag, 1, BATTLE_FLAG_PARTY_IN_AUDIENCE)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_MARIO)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

USER_FUNC(_set_gonbaba_light) {
    u8* light = lightGetPaperCraft();
    Mtx mtx;

    *(Vec*)(light + 0x24) = (Vec){-200.0f, 50.0f, 500.0f};
    PSMTXScale(mtx, 3.0f, 3.0f, 3.0f);
    PSMTXMultVec(mtx, (Vec*)(light + 0x24), (Vec*)(light + 0x24));
    return EVT_RETURN_DONE;
}

/**
 * Plays one spectator in Hooktail's audience-eating scene:
 * the audience member jumps up and is lifted toward her mouth; spectators 32 and 53 escape, while the others
 * are removed. negotiation_event starts it eight times for different spectators.
 */
EVT_BEGIN(gon_extra_eaten_event)
    WAIT_FRAMES(14)

    // One spectator is swallowed: it is marked as busy and jumps up.
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
    CALL(evt_audience_move_position_speed, LW(0), LW(1), LW(2), LW(3), FLOAT(10.0f))

    // Spectator 32 is lifted and spun toward her mouth, then lands, screams and runs off before it is
    // removed.
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

        // BUG: unlike spectator 53 and the others, this path deletes the member without
        // evt_audience_num_updown_in_event(-1), so the audience target count stays one higher.
        CALL(evt_audience_delete, LW(0))
        RETURN
    END_IF

    // Spectator 53 does the same with a different scream and running sound.
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

    // Any other spectator is simply removed.
    CALL(evt_audience_delete, LW(0))
    CALL(evt_audience_num_updown_in_event, FLOAT(-1.0f))
    RETURN
EVT_END()

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

/** Damage event of Hooktail: runs the default damage reaction (btldefaultevt_Damage) for herself. */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(btldefaultevt_Damage)
    RETURN
EVT_END()

/**
 * Hooktail's damage sub-event (registered in her event table):
 * counts hits made with the Attack FX R sound (damage code 0x1000000), switches her to the weak pose table,
 * plays the matching hurt pose and, when the head is hit, sends spectators running from the stage.
 */
EVT_BEGIN(gonbaba_damage_sub_event)
    // A hit with the Attack FX R cricket sound (damage code 0x1000000) is counted and switches her to the
    // weak pose table.
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
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_weak))
    END_IF

    // A hit that does no damage and has no cricket sound skips the hurt reaction.
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ATTACK_FX_R, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(90)
        END_IF
    END_IF

    // A hit on her body (part 1) plays GNB_D_2.
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(11))
    IF_INT_EQUAL(LW(11), 1)
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("GNB_D_2"))
    ELSE
        // A hit on her head plays GNB_D_1 and shakes up to two spectators loose, who run off the stage.
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

    // After her plea (unit work 3 is 1) the first two cricket speeches are skipped; she gives the third one
    // (label 50) once.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        GOTO(50)
    END_IF

    // Her speech counter (unit work 2) decides which line she speaks.
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
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_4"))
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_wait_move_end)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))

    // First cricket sound: she complains about the awful noise (stg1_gon_21); second: she struggles not to
    // gag (stg1_gon_22).
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 1)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_21"), 0, -2)
        CASE_EQUAL(1)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 1, 93, 160, 536, 93, 123, 25, 20, 0)
        WAIT_FRAMES(20)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 2)
        CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_22"), 0, -2)
    END_SWITCH

    // The camera returns to normal and she goes back to her usual pose.
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
    LABEL(50)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, LW(0))
    IF_INT_GREATER_OR_EQUAL(LW(0), 3)
        RETURN
    END_IF

    // Third time: she gags again and wonders why crickets always make her faint (stg1_gon_31_04).
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_MESSAGE_STEP, 3)
#if VERSION >= VERSION_JP
    CALL(btlevtcmd_WaitAttackEnd)
#endif
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_T_4"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_S_4"))
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_wait_move_end)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
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
    CALL(evt_msg_print, MSG_PRINT_BATTLE_UNIT, STRING("stg1_gon_31_04"), 0, -2)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
    LABEL(90)

    // A whoosh plays at the spot that was hit.
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_GetHitPos, UNIT_ID_SELF, LW(0), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_ATTACK_MISS2"), LW(0), LW(1), LW(2), 0)
    RETURN
EVT_END()

/**
 * Dead event of Hooktail. The first time she would die (unit work 3 is 0) it runs negotiation_event instead.
 * Otherwise she collapses (GNB_X_1), the camera closes in on her, she speaks her last line (stg1_gon_32),
 * the death animation plays (GNB_D_3, GNB_H_1), exp is stored and the unit is marked defeated with KillUnit.
 */
// The first defeat enters negotiation; only a defeat after negotiation takes the final death path.
static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_NEGOTIATED, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(negotiation_event)
        RETURN
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_X_1"))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_DOWN1"), EVT_NULLPTR, 0, LW(15))
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_btl_camera_wait_move_end)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
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
    CALL(evt_msg_toge, MSG_TOGE_AT_POS, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_32"), 0, 0)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
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
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 90, MOVE_SOUND_FROM_TABLE)
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
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    RETURN
EVT_END()

/**
 * Init event of Hooktail.
 * Installs her wait, attack, damage and entry events (confusion uses the default confusion script),
 * registers the audience entries 13, 14, 32 to 35, 53 and 54 with flag 512,
 * sets the audience base flag (32768 in the early demos, 16384 in JP and later),
 * sets her base scale and clears unit works 0 to 5.
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
#if VERSION <= VERSION_DEMO_JP
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

/** Empty phase event of Hooktail; nothing registers it. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/**
 * Attack event of Hooktail: can use an item.
 * Unit work 5 tracks her opening non-item attacks: the first is the fire breath, the second the stomp,
 * and from then on she picks the stomp or the fire breath at random (50 percent each).
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
 * Stomp attack of Hooktail (fumituke): she steps forward with two leg-smoke steps and camera shakes,
 * raises a foot (GNB_A_3) and stamps on one target (weapon_gonbaba_fumituke_attack),
 * which is checked for a miss or dodge before the action-command defence and damage; she then steps back.
 */
static EVT_BEGIN(fumituke_attack_event)
    // Choose a target.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_gonbaba_fumituke_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_gonbaba_fumituke_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF

    // The attack is declared and the script waits for any bodyguard movement.
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_gonbaba_fumituke_attack))

    // She steps forward with two heavy steps, each with leg smoke and a camera shake (GNB_W_2).
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

    // She raises a foot (GNB_A_3) above the target.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A_3"))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_ARM_UP1"), EVT_NULLPTR, 0, -320, 30, 30, EVT_NULLPTR)
        CALL(btlevtcmd_ftomsec, 42, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_ARM_DOWN1"), EVT_NULLPTR, 0, -360, 50, 30, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(btlevtcmd_ftomsec, 47, LW(0))
    WAIT_MS(LW(0))

    // Hit check: results 3, 6 and 2 start an avoid; result 4 skips it, and result 1 goes on to the damage.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gonbaba_fumituke_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(PRECHECK_INTANGIBLE)
        GOTO(90)
        CASE_END
        CASE_EQUAL(PRECHECK_LUCKY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_DODGY)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(PRECHECK_MISS)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(PRECHECK_HIT)
        GOTO(91)
        CASE_DEFAULT
        GOTO(97)
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)

    // Hit: the damage is dealt with the action-command result.
    CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gonbaba_fumituke_attack))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gonbaba_fumituke_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)
    LABEL(97)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -400, 0, -5, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -400)
        ADD(LW(2), -5)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    WAIT_FRAMES(3)

    // The stomp: her foot comes down with a big camera shake.
    CALL(evt_btl_camera_shake_h, 0, 5, 0, 10, 13)
    WAIT_FRAMES(7)
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gonbaba_fumituke_attack))
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 30, 13)
    WAIT_FRAMES(35)

    // She steps back (GNB_W_3) with two more smoky steps.
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
 * Hooktail's fire breath: picks targets, lets her wind up (neck rises, roar),
 * spawns the gonbaba_breath flame effect and deals the weapon_gonbaba_firebreath_attack damage to Mario
 * and the partner (all targets), then returns her to her normal pose.
 */
EVT_BEGIN(firebreath_attack_event)
    // Choose targets: the breath reaches Mario and his partner.
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), PTR(&weapon_gonbaba_firebreath_attack))
    CALL(btlevtcmd_ChoiceSamplingEnemy, PTR(&weapon_gonbaba_firebreath_attack), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF

    // The attack is declared on all targets and the script waits for any bodyguard movement.
    CALL(btlevtcmd_WeaponAftereffect, PTR(&weapon_gonbaba_firebreath_attack))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, PTR(&weapon_gonbaba_firebreath_attack))

    // Wind-up: her neck rises with a roar (GNB_A_1).
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
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_BREATH_FIRE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        ADD(LW(11), 45)
        SUBTRACT(LW(8), 10)
        CALL(evt_eff, 0, STRING("gonbaba_breath"), 0, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
    END_INLINE_EVENT

    // The flame breath rolls out toward the targets.
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(12), EVT_NULLPTR, EVT_NULLPTR)
    SUBTRACT(LW(12), LW(6))
    IF_INT_LESS(LW(12), 0)
        MULTIPLY(LW(12), -1)
    END_IF
    ADD(LW(12), 20)
    MULTIPLY(LW(12), LW(11))
    DIVIDE(LW(12), LW(10))
    ADD(LW(12), 105)
    WAIT_FRAMES(LW(12))
    SET_VALUE(LW(10), 0)
    LABEL(10)

    // Each target gets the hit check; result 1 proceeds to damage, and the action-command result is read only
    // when that hit is on the first target in the list.
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gonbaba_firebreath_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
        // BUG: this script has no LABEL(98): evtSearchLabel resumes at this GOTO's argument word, which runs
        // as SET_SPD with the next command word as its value, and the script goes on after the GOTO.
        GOTO(98)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), PTR(&weapon_gonbaba_firebreath_attack))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), PTR(&weapon_gonbaba_firebreath_attack), DAMAGE_RESULT_FINAL_HIT, LW(5))
    GOTO(97)
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        WAIT_FRAMES(20)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    WAIT_FRAMES(84)
    LABEL(99)

    // She goes back to her normal pose and waits.
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Wait (idle) event of Hooktail: sets her idle pose from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()
