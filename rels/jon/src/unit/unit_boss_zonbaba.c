#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_eff.h"
#include "evt/evt_snd.h"
#include "eff/n64/eff_kemuri9_n64.h"
#include "battle/battle_sub.h"

#include "manager/evtmgr_cmd.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_map.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_damage.h"

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_HP_STAGE 0          // 0 full, 1 once HP <= 50%, 2 once HP <= 25% (prints tik_boss_15); attack_event picks its attack table by it
#define UNIT_WORK_RECOVERS_LEFT 1     // heals left (2 at start, recover_event subtracts 1); at 0 attack_event gives the recover draw weight 0
#define UNIT_WORK_RECOVER_TALK_DONE 2 // 1 once recover_event printed tik_boss_14 (never set: see the BUG note there)

USER_FUNC(btlevtcmd_SetEventEntry);
USER_FUNC(btlevtcmd_SetEventPhase);
USER_FUNC(btlevtcmd_SetTalkBodyId);
static USER_FUNC(_leg_smoke_effect);

BattleUnitKind unit_boss_zonbaba;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static PoseTableEntry pose_table[9];
static PoseTableEntry pose_table_weak[9];
static BattleDataEntry data_table[3];
static BattleUnitKindPart parts[2];
BattleWeapon weapon_zonbaba_fumituke_attack;
BattleWeapon weapon_zonbaba_kamituki_attack;
BattleWeapon weapon_zonbaba_cold_attack;
BattleWeapon weapon_zonbaba_confuze_attack;
BattleWeapon weapon_zonbaba_sleep_attack;
BattleWeapon weapon_zonbaba_minimini_attack;
BattleWeapon weapon_zonbaba_recover;
static EvtWord battle_entry_event[357];
static EvtWord init_event[58];
static EvtWord damage_event[10];
EvtWord zonbaba_damage_sub_event[45];
static EvtWord dead_event[264];
static EvtWord phase_event[94];
static EvtWord fumituke_attack_event[356];
static EvtWord kamituki_attack_event[141];
EvtWord breath1_attack_event[315];
EvtWord breath2_attack_event[320];
EvtWord recover_event[88];
static EvtWord attack_event[282];
static EvtWord wait_event[6];

BattleUnitKind unit_boss_zonbaba = {
    UNIT_BONETAIL,
    "btl_un_zonbaba",
    200,
    0,
    1,
    1,
    0,
    99,
    0,
    100,
    0,
    50,
#ifdef VERSION_G8ME01
    5,
#else
    7,
#endif
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
#ifdef VERSION_G8ME01
    115.0f,
#else
    0.0f,
#endif
    0.0f,
    0.0f,
    20.0f,
    0.0f,
    250.0f,
    0.0f,
    50.0f,
    380.0f,
    110.0f,
    50.0f,
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
    2,
    parts,
    init_event,
    data_table,
};

static BattleUnitDefense defence = {{2, 2, 2, 2, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    0, // sleep
    0, // stop
    0, // dizzy
    100, // poison
    0, // confuse
    100, // electric
    100, // burn
    0, // freeze
    0, // huge
    0, // tiny
    100, // attackUp
    0, // attackDown
    100, // defenseUp
    0, // defenseDown
    0, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    0, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "GNB_N_1"},
    {POSE_TYPE_CONFUSE, "GNB_X_1"},
    {POSE_TYPE_DIZZY, "GNB_X_1"},
    {POSE_TYPE_STOP, "GNB_Z_1"},
    {POSE_TYPE_NORMAL, "GNB_S_3"},
    {POSE_TYPE_TALK, "GNB_T_3"},
    {POSE_TYPE_DAMAGE_GUARDED, "GNB_S_3"},
    {POSE_TYPE_DAMAGE, "GNB_D_1"},
    {POSE_TYPE_ACTION, "GNB_S_3"},
};

static PoseTableEntry pose_table_weak[] = {
    {POSE_TYPE_SLEEP, "GNB_N_1"},
    {POSE_TYPE_CONFUSE, "GNB_X_1"},
    {POSE_TYPE_DIZZY, "GNB_X_1"},
    {POSE_TYPE_STOP, "GNB_N_1"},
    {POSE_TYPE_NORMAL, "GNB_S_4"},
    {POSE_TYPE_TALK, "GNB_T_4"},
    {POSE_TYPE_DAMAGE_GUARDED, "GNB_S_4"},
    {POSE_TYPE_DAMAGE, "GNB_V_1"},
    {POSE_TYPE_ACTION, "GNB_S_4"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, zonbaba_damage_sub_event},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_zonbaba", "c_gonbaba_z", 0.0f, 0.0f, 0.0f, 230.0f, 36.0f, 60.0f, 230.0f, 36.0f, 60.0f, 20, 30, 255, &defence, &defence_attr, kImportantPart, 0, pose_table},
    {2, "btl_un_zonbaba", "c_gonbaba_z", 100.0f, 0.0f, 0.0f, 250.0f, 110.0f, 25.0f, 250.0f, 110.0f, 25.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart | kHammerlikeCannotTarget | kShellTosslikeCannotTarget | kHidden, 0, pose_table},
};

BattleWeapon weapon_zonbaba_fumituke_attack = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    0x2004, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 10, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_zonbaba_kamituki_attack = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_HAMMERLIKE | TARGET_PROP_ONLY_FRONTMOST | TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_UNGUARDABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_zonbaba_cold_attack = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, // burnChance..burnTime
    30, // freezeChance
    3, // freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_zonbaba_confuze_attack = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, // poisonTime..poisonStrength
    30, // confuseChance
    3, // confuseTime
    0, 0, 0, 0, 0, 0, 0, 0, // electricChance..freezeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeChance..defChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeStrength..slowChance
    0, 0, 0, 0, 0, 0, 0, 0, // slowTime..hpRegenTime
    0, 0, 0, // hpRegenStrength..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_zonbaba_sleep_attack = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, // targetWeightFlags
    30, // sleepChance
    5, // sleepTime
    0, 0, 0, 0, 0, 0, 0, 0, // stopChance..confuseChance
    0, 0, 0, 0, 0, 0, 0, 0, // confuseTime..freezeChance
    0, 0, 0, 0, 0, 0, 0, 0, // freezeTime..defChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // defChangeTime..fastTime
    0, 0, 0, 0, 0, 0, 0, 0, // slowChance..invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_zonbaba_minimini_attack = {
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
    {8, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, // burnChance..freezeTime
    30, // sizeChangeChance
    3, // sizeChangeTime
    -2, // sizeChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeChance..allergicTime
    0, 0, 0, 0, 0, 0, 0, 0, // ohkoChance..galeForceChance
    0, 0, 0, 0, 0, 0, 0, 0, // paybackTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_zonbaba_recover = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, 0, // baseFpCost..superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_SELF | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_SAME_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    0, 0, 0, 0, 0, 0, 0, 0, // chargeStrength..paybackTime
    0, 0, 0, 0, 0, 0, 0, // holdFastTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static EVT_BEGIN(battle_entry_event)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_EVENT)
    CALL(evt_map_replayanim, 0, STRING("dontyo"))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
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
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_F_3"))
    WAIT_FRAMES(1)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    WAIT_FRAMES(10)
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(5)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_O"))
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_MARIO, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_MARIO, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_snd_se, UNIT_ID_MARIO, STRING("SFX_VOICE_MARIO_SURPRISED2_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_JumpPosition, UNIT_ID_MARIO, LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(5)
        CALL(btlevtcmd_SetFallAccel, UNIT_ID_PARTNER, FLOAT(0.2998047f))
        CALL(btlevtcmd_GetPos, UNIT_ID_PARTNER, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPosition, UNIT_ID_PARTNER, LW(0), LW(1), LW(2), 25, MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(0), POSE_TYPE_CONFUSE)
    END_INLINE_EVENT
    CALL(evt_snd_sfxon, STRING("SFX_BOSS_GNB_APPEAR1"), 0)
    CALL(evt_btl_camera_shake_h, 1, 8, 0, 20, 13)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_MARIO, 1, STRING("M_I_Y"))
    CALL(btlevtcmd_GetBodyId, UNIT_ID_PARTNER, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_PARTNER, LW(0), POSE_TYPE_NEUTRAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -233, 45, 452, 56, 125, 37, 60, 0)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_boss_12"), 0, -2)
    WAIT_MS(300)
    CALL(evt_msg_print_battle_party, STRING("tik_boss_13"))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_MARIO)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_PARTNER)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_prilimit, BTL_CAM_PRI_NORMAL)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(evt_btl_camera_set_moveSpeedLv, 0, 3)
    RETURN
EVT_END()

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetTalkBodyId, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_SetBaseScale, UNIT_ID_SELF, FLOAT(1.2998047f), FLOAT(1.2998047f), FLOAT(0.5f))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_RECOVERS_LEFT, 2)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_RECOVER_TALK_DONE, 0)
    CALL(btlevtcmd_SetEventEntry, UNIT_ID_SELF, PTR(battle_entry_event))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL_CHILD(PTR(btldefaultevt_Damage))
    RETURN
EVT_END()

EVT_BEGIN(zonbaba_damage_sub_event)
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(11))
    IF_INT_EQUAL(LW(11), 1)
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("GNB_D_2"))
        ELSE
        CALL(btlevtcmd_AnimeChangePose, LW(10), 1, STRING("GNB_D_1"))
    END_IF
    CALL(btlevtcmd_GetDamage, LW(10), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetHitPos, LW(10), LW(11), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_ATTACK_MISS2"), LW(0), LW(1), LW(2), 0)
    END_IF
    WAIT_MS(1000)
    RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_X_1"))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_DOWN1"), EVT_NULLPTR, 0, -250, 0, 0, LW(15))
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
    WAIT_FRAMES(90)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("GNB_X_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("GNB_X_1"))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_boss_16"), 0, -2)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_EVENT, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 1, -38, 33, 1005, -38, 92, 13, 60, 0)
    CALL(evt_snd_sfxoff, LW(15))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_DOWN2"), EVT_NULLPTR, 0, -250, 0, 0, EVT_NULLPTR)
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
    SUBTRACT(LW(0), 20)
    SUBTRACT(LW(2), 20)
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

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108867)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_CheckActStatus, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(0), 50)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, 1)
        END_IF
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
        MULTIPLY(LW(0), 25)
        DIVIDE(LW(0), 100)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(1))
        IF_INT_LESS_OR_EQUAL(LW(1), LW(0))
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, 2)
            CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_boss_15"), 0, -2)
            CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
        END_IF
    END_SWITCH
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(fumituke_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_W_2"))
    WAIT_FRAMES(57)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -300, 0, -30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -300)
        ADD(LW(2), -30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    WAIT_FRAMES(46)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -300, 0, 30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -300)
        ADD(LW(2), 30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A_3"))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_ARM_UP1"), EVT_NULLPTR, 0, -320, 30, 30, EVT_NULLPTR)
        WAIT_MS(700)
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_ARM_DOWN1"), EVT_NULLPTR, 0, -360, 50, 30, EVT_NULLPTR)
    END_INLINE_EVENT
    WAIT_FRAMES(45)
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
    GOTO(97)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    WAIT_FRAMES(7)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 30, 13)
    WAIT_FRAMES(35)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_W_3"))
    WAIT_FRAMES(51)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -180, 0, 30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -180)
        ADD(LW(2), 30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    WAIT_FRAMES(46)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_MOVE1"), EVT_NULLPTR, 0, -100, 0, 30, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(0), -100)
        ADD(LW(2), -30)
        CALL(_leg_smoke_effect, LW(0), LW(1), LW(2))
    END_SIBLING_EVENT
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    WAIT_FRAMES(7)
    LABEL(99)
    RETURN
EVT_END()

static EVT_BEGIN(kamituki_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_BITE1"), EVT_NULLPTR, 0, -220, 80, 0, EVT_NULLPTR)
        WAIT_MS(250)
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_BITE2"), EVT_NULLPTR, 0, -260, 40, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A_2"))
    WAIT_FRAMES(20)
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
        GOTO(99)
        CASE_END
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(97)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    LABEL(99)
    WAIT_FRAMES(20)
    RETURN
EVT_END()

EVT_BEGIN(breath1_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A_1"))
    SET_VALUE(LW(10), 120)
    SET_VALUE(LW(11), 30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(6), 360)
    ADD(LW(7), 30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(12), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(12), 360)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(12), LW(10))
    BEGIN_INLINE_EVENT
        WAIT_MS(500)
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_NECK_UP1"), EVT_NULLPTR, 0, -250, 0, 0, EVT_NULLPTR)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(105)
        CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, LW(13), EVT_NULLPTR, 0, -300, 0, 0, EVT_NULLPTR)
        ADD(LW(11), 45)
        SUBTRACT(LW(8), 10)
        SWITCH(LW(9))
            CASE_EQUAL(PTR(&weapon_zonbaba_cold_attack))
            CALL(evt_eff, 0, STRING("gonbaba_breath"), 3, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(PTR(&weapon_zonbaba_confuze_attack))
            CALL(evt_eff, 0, STRING("gonbaba_breath"), 4, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(PTR(&weapon_zonbaba_sleep_attack))
            CALL(evt_eff, 0, STRING("gonbaba_breath"), 5, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(PTR(&weapon_zonbaba_minimini_attack))
            CALL(evt_eff, 0, STRING("gonbaba_breath"), 6, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
        END_SWITCH
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
    WAIT_FRAMES(LW(12))
    SET_VALUE(LW(10), 0)
    LABEL(10)
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
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    RETURN
EVT_END()

EVT_BEGIN(breath2_attack_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A2_1"))
    WAIT_MS(833)
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, STRING("SFX_BOSS_GNB_NECK_UP1"), EVT_NULLPTR, 0, -250, 0, 0, EVT_NULLPTR)
    WAIT_MS(1000)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("GNB_A2_3"))
    CALL(btlevtcmd_snd_se_offset, UNIT_ID_SELF, LW(13), EVT_NULLPTR, 0, -300, 0, 0, EVT_NULLPTR)
    SET_VALUE(LW(10), 120)
    SET_VALUE(LW(11), 30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(6), LW(7), LW(8))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(6), 360)
    ADD(LW(7), 30)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(12), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(12), 360)
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(12), LW(10))
    BEGIN_INLINE_EVENT
        WAIT_FRAMES(20)
        ADD(LW(11), 45)
        SUBTRACT(LW(8), 10)
        SWITCH(LW(9))
            CASE_EQUAL(PTR(&weapon_zonbaba_cold_attack))
            CALL(evt_eff, 0, STRING("gonbaba_breath"), 3, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(PTR(&weapon_zonbaba_confuze_attack))
            CALL(evt_eff, 0, STRING("gonbaba_breath"), 4, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(PTR(&weapon_zonbaba_sleep_attack))
            CALL(evt_eff, 0, STRING("gonbaba_breath"), 5, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
            CASE_EQUAL(PTR(&weapon_zonbaba_minimini_attack))
            CALL(evt_eff, 0, STRING("gonbaba_breath"), 6, LW(6), LW(7), LW(8), FLOAT(3.0f), LW(11), 0, 0, 0, 0, 0, 0)
        END_SWITCH
    END_INLINE_EVENT
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(12), EVT_NULLPTR, EVT_NULLPTR)
    SUBTRACT(LW(12), LW(6))
    IF_INT_LESS(LW(12), 0)
        MULTIPLY(LW(12), -1)
    END_IF
    ADD(LW(12), 20)
    MULTIPLY(LW(12), LW(11))
    DIVIDE(LW(12), LW(10))
    ADD(LW(12), 20)
    WAIT_FRAMES(LW(12))
    SET_VALUE(LW(10), 0)
    LABEL(10)
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
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
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
    RETURN
EVT_END()

EVT_BEGIN(recover_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_RECOVER_TALK_DONE, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
        // BUG: GetUnitWork (reading into the constant 1), apparently meant as SetUnitWork: the slot stays 0, so
        // tik_boss_14 is printed on every recover
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_RECOVER_TALK_DONE, 1)
        CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("tik_boss_14"), 0, -2)
        CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_RECOVERS_LEFT, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SUBTRACT(LW(0), 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_RECOVERS_LEFT, LW(0))
    END_IF
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, 1, 20)
    CALL(btlevtcmd_GetHitPos, UNIT_ID_SELF, 1, LW(0), LW(1), LW(2))
    ADD(LW(0), -27)
    ADD(LW(1), 15)
    ADD(LW(2), 10)
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(0), LW(1), LW(2), 20, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(40)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_HP_STAGE, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_DrawLots, LW(0), 6, 25, 20, 10, 10, 10, 10)
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_fumituke_attack))
            CALL_CHILD(PTR(fumituke_attack_event))
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_kamituki_attack))
            CALL_CHILD(PTR(kamituki_attack_event))
            CASE_EQUAL(2)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_COLD1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_cold_attack))
            CALL_CHILD(PTR(breath2_attack_event))
            CASE_EQUAL(3)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_CHANGE1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_confuze_attack))
            CALL_CHILD(PTR(breath1_attack_event))
            CASE_EQUAL(4)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_CHANGE1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_sleep_attack))
            CALL_CHILD(PTR(breath2_attack_event))
            CASE_EQUAL(5)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_CHANGE1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_minimini_attack))
            CALL_CHILD(PTR(breath1_attack_event))
        END_SWITCH
        CASE_EQUAL(1)
        SET_VALUE(LW(1), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_RECOVERS_LEFT, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(1), 20)
        END_IF
        CALL(btlevtcmd_DrawLots, LW(0), 7, 25, 20, 10, 10, 10, 10, LW(1))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_fumituke_attack))
            CALL_CHILD(PTR(fumituke_attack_event))
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_kamituki_attack))
            CALL_CHILD(PTR(kamituki_attack_event))
            CASE_EQUAL(2)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_COLD1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_cold_attack))
            CALL_CHILD(PTR(breath2_attack_event))
            CASE_EQUAL(3)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_CHANGE1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_confuze_attack))
            CALL_CHILD(PTR(breath1_attack_event))
            CASE_EQUAL(4)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_CHANGE1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_sleep_attack))
            CALL_CHILD(PTR(breath2_attack_event))
            CASE_EQUAL(5)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_CHANGE1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_minimini_attack))
            CALL_CHILD(PTR(breath1_attack_event))
            CASE_EQUAL(6)
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_recover))
            CALL_CHILD(PTR(recover_event))
        END_SWITCH
        CASE_EQUAL(2)
        SET_VALUE(LW(1), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_RECOVERS_LEFT, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            SET_VALUE(LW(1), 50)
        END_IF
        CALL(btlevtcmd_DrawLots, LW(0), 7, 25, 20, 10, 10, 10, 10, LW(1))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_fumituke_attack))
            CALL_CHILD(PTR(fumituke_attack_event))
            CASE_EQUAL(1)
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_kamituki_attack))
            CALL_CHILD(PTR(kamituki_attack_event))
            CASE_EQUAL(2)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_COLD1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_cold_attack))
            CALL_CHILD(PTR(breath2_attack_event))
            CASE_EQUAL(3)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_CHANGE1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_confuze_attack))
            CALL_CHILD(PTR(breath1_attack_event))
            CASE_EQUAL(4)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_CHANGE1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_sleep_attack))
            CALL_CHILD(PTR(breath2_attack_event))
            CASE_EQUAL(5)
            SET_VALUE(LW(13), STRING("SFX_BOSS_GNB_BREATH_CHANGE1"))
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_minimini_attack))
            CALL_CHILD(PTR(breath1_attack_event))
            CASE_EQUAL(6)
            SET_VALUE(LW(9), PTR(&weapon_zonbaba_recover))
            CALL_CHILD(PTR(recover_event))
        END_SWITCH
    END_SWITCH
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()


static USER_FUNC(_leg_smoke_effect) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    effKemuri9N64Entry(1, 18, 90, x, y, z, 60.0f, 0.0f);
    return EVT_RETURN_DONE;
}
