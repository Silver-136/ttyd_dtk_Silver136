#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "pmario_sound.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "eff/eff_spirit.h"
#include "eff/n64/eff_kumokumo_n64.h"

#include "battle/battle.h"
#include "battle/battle_sub.h"
#include "manager/evtmgr_cmd.h"
#include "battle/battle_unit.h"
#include "battle/battle_audience.h"
#include "system.h"
#include "driver/effdrv.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_camera.h"
#include "battle/battle_weapon_power.h"
#include "evt/evt_batstage.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_msg.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "data_tables.h"
#include "battle/battle_damage.h"

typedef struct CortezWork {
    s16 state;
    s16 count;
    s16 audienceId[100];
    s16 wait[100];
    s16 timer[100];
    s16 seq[100];
    void* eff[100];
    Vec srcPos[100];
    Vec dstPos[100];
} CortezWork;
BattleUnitSetup entry_honeduka;

USER_FUNC(_cortez_check_drain_audience);
USER_FUNC(_make_cortez_work_area);
USER_FUNC(btlevtcmd_SetEventPhase);
static USER_FUNC(_check_zenei_kouei);
USER_FUNC(_cortez_recovery_audience);
USER_FUNC(_cortez_drain_energy);
USER_FUNC(_cortez_collect_energy);
USER_FUNC(_cortez_get_sfx_vol);
extern EvtWord cortez_weapon_get_home_height[];
extern EvtWord cortez_weapon_set_formation_attribute[];
extern BattleUnitKind unit_boss_cortez_claw;
extern BattleUnitKind unit_boss_cortez_rapier;
extern BattleUnitKind unit_boss_cortez_saber;
extern BattleUnitKind unit_boss_cortez_sword;

BattleUnitKind unit_boss_cortez;
static StatusVulnerability regist;
BattleUnitDefense defence_mode1;
BattleUnitDefenseAttr defence_attr_mode1;
BattleUnitDefense defence_mode2_upper_head;
BattleUnitDefenseAttr defence_attr_mode2_upper_head;
BattleUnitDefense defence_mode2_upper_rib_open;
BattleUnitDefenseAttr defence_attr_mode2_upper_rib_open;
BattleUnitDefense defence_mode2_upper_rib_close;
BattleUnitDefenseAttr defence_attr_mode2_upper_rib_close;
BattleUnitDefense defence_mode2_middle_head;
BattleUnitDefenseAttr defence_attr_mode2_middle_head;
BattleUnitDefense defence_mode2_middle_rib_open;
BattleUnitDefenseAttr defence_attr_mode2_middle_rib_open;
BattleUnitDefense defence_mode2_middle_rib_close;
BattleUnitDefenseAttr defence_attr_mode2_middle_rib_close;
BattleUnitDefense defence_mode2_lower_head;
BattleUnitDefenseAttr defence_attr_mode2_lower_head;
BattleUnitDefense defence_mode2_lower_rib_open;
BattleUnitDefenseAttr defence_attr_mode2_lower_rib_open;
BattleUnitDefense defence_mode2_lower_rib_close;
BattleUnitDefenseAttr defence_attr_mode2_lower_rib_close;
BattleUnitDefense defence_mode3;
BattleUnitDefenseAttr defence_attr_mode3;
BattleUnitDefense defence_honeduka;
BattleUnitDefenseAttr defence_honeduka_attr;
BattleWeapon weapon_cortez_normal_front_attack;
BattleWeapon weapon_cortez_normal_rear_attack;
BattleWeapon weapon_cortez_midaregiri_attack;
BattleWeapon weapon_cortez_mode2_head_attack;
BattleWeapon weapon_cortez_mode2_charge;
BattleWeapon weapon_cortez_mode2_bone_attack;
BattleWeapon weapon_cortez_mode3_kamitsuki_attack;
static PoseSoundTiming pose_sound_timing_table[73];
PoseTableEntry pose_table_mode_1[10];
PoseTableEntry pose_table_mode_1_hone[10];
PoseTableEntry pose_table_mode_2_normal[4];
PoseTableEntry pose_table_mode_2_upper_normal[4];
PoseTableEntry pose_table_mode_2_upper_normal_counter[4];
PoseTableEntry pose_table_mode_2_upper_charge[4];
PoseTableEntry pose_table_mode_2_upper_charge_counter[4];
PoseTableEntry pose_table_mode_2_middle_normal[4];
PoseTableEntry pose_table_mode_2_middle_normal_counter[4];
PoseTableEntry pose_table_mode_2_middle_charge[4];
PoseTableEntry pose_table_mode_2_middle_charge_counter[4];
PoseTableEntry pose_table_mode_2_lower_normal[4];
PoseTableEntry pose_table_mode_2_lower_normal_counter[4];
PoseTableEntry pose_table_mode_2_lower_charge[4];
PoseTableEntry pose_table_mode_2_lower_charge_counter[4];
PoseTableEntry pose_table_fly_bone[4];
PoseTableEntry pose_table_mode_3[12];
PoseTableEntry pose_table_honeduka[4];
static BattleDataEntry data_table[7];
static BattleUnitKindPart parts[19];
static EvtWord init_event[260];
static EvtWord damage_event[70];
EvtWord cortez_ac_counter_damage_event[98];
static EvtWord avoid_thorn_event[89];
static EvtWord phase_event[196];
EvtWord attack_event_mode1[44];
EvtWord attack_event_mode2[47];
EvtWord at2_head_bat_event[202];
EvtWord at2_hone_charge_event[146];
EvtWord at2_hone_rain_attack_event[605];
EvtWord _bone_rain_event[114];
EvtWord attack_event_mode3[52];
static EvtWord attack_audience_event[247];
EvtWord at3_kamitsuki_attack[264];
EvtWord at1_normal_attack[205];
EvtWord at1_midaregiri_attack[251];
static EvtWord wait_event[6];
BattleUnitSetup entry_claw;
BattleUnitSetup entry_rapier;
BattleUnitSetup entry_sword;
BattleUnitSetup entry_saber;
EvtWord cortez_reset_pose_table[450];
static EvtWord cortez_damage_sub_event[153];
static EvtWord dead_event[1033];
EvtWord mode2_rib_open_event[54];
EvtWord mode2_rib_close_event[54];
EvtWord mode2_honeduka_damage_event[76];
EvtWord mode_change_event[111];

BattleUnitKind unit_boss_cortez = {
    UNIT_CORTEZ, // mUnitType
    "btl_un_cortez", // name
    20, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    77, // mLevel
    0, // mBonusExp
    0, // bonusCoin
    100, // bonusCoinRate
    0, // baseCoin
    50, // runRate
    6, // pbSoftCap
    250, // width
    170, // height
    {-10, 90}, // hitOffset
    {0.0f, 0.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {125.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 25.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {32.0f, 0.0f, 0.0f}, // bintaHitOffset
    {0.0f, 0.0f, 0.0f}, // kissHitOffset
    {-70.0f, 65.0f, 0.0f}, // cutBaseOffset
    180.0f, // cutWidth
    130.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    -1, // swallowChance
    0, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_BOSS_KOR_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    kUnitNoSpin, // attributeFlags
    &regist, // statusVulnerability
    19, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

static StatusVulnerability regist = {
    40, // sleep
    40, // stop
    50, // dizzy
    100, // poison
    50, // confuse
    100, // electric
    100, // burn
    25, // freeze
    0, // huge
    0, // tiny
    100, // attackUp
    0, // attackDown
    100, // defenseUp
    90, // defenseDown
    50, // allergic
    0, // fright
    0, // galeForce
    100, // fast
    50, // slow
    0, // dodgy
    0, // invisible
    0, // ohko
};

BattleUnitDefense defence_mode1 = {{1, 1, 1, 1, 1}};

BattleUnitDefenseAttr defence_attr_mode1 = {{0, 0, 0, 0, 0}};

BattleUnitDefense defence_mode2_upper_head = {{1, 1, 1, 1, 1}};

BattleUnitDefenseAttr defence_attr_mode2_upper_head = {0};

BattleUnitDefense defence_mode2_upper_rib_open = {0};

BattleUnitDefenseAttr defence_attr_mode2_upper_rib_open = {{1, 1, 1, 1, 1}};

BattleUnitDefense defence_mode2_upper_rib_close = {{3, 3, 3, 3, 3}};

BattleUnitDefenseAttr defence_attr_mode2_upper_rib_close = {{0, 0, 0, 0, 0}};

BattleUnitDefense defence_mode2_middle_head = {{1, 1, 1, 1, 1}};

BattleUnitDefenseAttr defence_attr_mode2_middle_head = {0};

BattleUnitDefense defence_mode2_middle_rib_open = {0};

BattleUnitDefenseAttr defence_attr_mode2_middle_rib_open = {{1, 1, 1, 1, 1}};

BattleUnitDefense defence_mode2_middle_rib_close = {{3, 3, 3, 3, 3}};

BattleUnitDefenseAttr defence_attr_mode2_middle_rib_close = {0};

BattleUnitDefense defence_mode2_lower_head = {{1, 1, 1, 1, 1}};

BattleUnitDefenseAttr defence_attr_mode2_lower_head = {0};

BattleUnitDefense defence_mode2_lower_rib_open = {0};

BattleUnitDefenseAttr defence_attr_mode2_lower_rib_open = {{1, 1, 1, 1, 1}};

BattleUnitDefense defence_mode2_lower_rib_close = {{3, 3, 3, 3, 3}};

BattleUnitDefenseAttr defence_attr_mode2_lower_rib_close = {0};

BattleUnitDefense defence_mode3 = {{1, 1, 1, 1, 1}};

BattleUnitDefenseAttr defence_attr_mode3 = {0};

BattleUnitDefense defence_honeduka = {{1, 1, 1, 1, 1}};

BattleUnitDefenseAttr defence_honeduka_attr = {{0, 0, 0, 0, 0}};

BattleWeapon weapon_cortez_normal_front_attack = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    4, // targetWeightFlags
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

BattleWeapon weapon_cortez_normal_rear_attack = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    8, // targetWeightFlags
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

BattleWeapon weapon_cortez_midaregiri_attack = {
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
    {2, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

BattleWeapon weapon_cortez_mode2_head_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_FREEZE_CRUSH | SPECIAL_FLIP_SHELLED, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

BattleWeapon weapon_cortez_mode2_charge = {
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
    NULL, // hpDamageFn
    {0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    0, 0, 0, 0, 0, 0, 0, 0, // targetWeightFlags..poisonChance
    0, 0, 0, 0, 0, 0, 0, 0, // poisonTime..dodgyTime
    0, 0, 0, 0, 0, 0, 0, 0, // burnChance..atkChangeChance
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeTime..ohkoChance
    4, // chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    0, 0, 0, 0, 0, 0, // invisibleChance..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

BattleWeapon weapon_cortez_mode2_bone_attack = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    4, // targetWeightFlags
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

BattleWeapon weapon_cortez_mode3_kamitsuki_attack = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    1, // superguardsAllowed
    1.0f, // reserved_14
    1, // mStylishCommandMultiplier
    1, // reserved_19
    1, // mBingoSlotChance
    1, // reserved_1B
    weaponGetPowerDefault, // hpDamageFn
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
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
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
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

static PoseSoundTiming pose_sound_timing_table[] = {
    {"KRT_1S_1", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_1", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_1", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_1", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_1", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_1", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_1", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_1", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_1", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_1", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_1", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_1", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_2", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_2", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_2", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_2", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_2", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S_2", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_2", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_2", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_2", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_2", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_2", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_1S2_2", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_1", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_1", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_1", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_1", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_1", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_1", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_1", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_1", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_1", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_1", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_1", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_1", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_2", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_2", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_2", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_2", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_2", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S_2", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_2", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_2", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_2", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_2", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_2", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_2S2_2", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_1", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_1", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_1", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_1", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_1", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_1", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_1", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_1", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_1", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_1", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_1", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_1", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_2", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_2", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_2", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_2", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_2", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S_2", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_2", 0.01f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_2", 0.2f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_2", 0.4f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_2", 0.6f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_2", 0.8f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {"KRT_3S3_2", 1.0f, NULL, "SFX_BOSS_KOR_WAIT1", 0, 1},
    {NULL, 0.0f, NULL, NULL, 0, 1},
};

PoseTableEntry pose_table_mode_1[] = {
    {POSE_TYPE_SLEEP, "KRT_N_1"},
    {POSE_TYPE_CONFUSE, "KRT_K_1"},
    {POSE_TYPE_STOP, "KRT_Z_1"},
    {POSE_TYPE_FREEZE, "KRT_Z_1"},
    {POSE_TYPE_POISON, "KRT_X_1"},
    {POSE_TYPE_DIZZY, "KRT_K_1"},
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE, "KRT_D1_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

PoseTableEntry pose_table_mode_1_hone[] = {
    {POSE_TYPE_SLEEP, "KRT_N_1"},
    {POSE_TYPE_CONFUSE, "KRT_K_1"},
    {POSE_TYPE_STOP, "KRT_Z_1"},
    {POSE_TYPE_FREEZE, "KRT_Z_1"},
    {POSE_TYPE_POISON, "KRT_X_1"},
    {POSE_TYPE_DIZZY, "KRT_K_1"},
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE, "KRT_D1_2"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

PoseTableEntry pose_table_mode_2_normal[] = {
    {POSE_TYPE_NORMAL, "KRT_1S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_1S_1"},
    {POSE_TYPE_DAMAGE, "KRT_1D_1"},
    {POSE_TYPE_ACTION, "KRT_1S_1"},
};

PoseTableEntry pose_table_mode_2_upper_normal[] = {
    {POSE_TYPE_NORMAL, "KRT_1S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_1S_1"},
    {POSE_TYPE_DAMAGE, "KRT_1D_1"},
    {POSE_TYPE_ACTION, "KRT_1S_1"},
};

PoseTableEntry pose_table_mode_2_upper_normal_counter[] = {
    {POSE_TYPE_NORMAL, "KRT_1S2_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_1S2_1"},
    {POSE_TYPE_DAMAGE, "KRT_1D2_1"},
    {POSE_TYPE_ACTION, "KRT_1S2_1"},
};

PoseTableEntry pose_table_mode_2_upper_charge[] = {
    {POSE_TYPE_NORMAL, "KRT_1S_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_1S_2"},
    {POSE_TYPE_DAMAGE, "KRT_1D_2"},
    {POSE_TYPE_ACTION, "KRT_1S_2"},
};

PoseTableEntry pose_table_mode_2_upper_charge_counter[] = {
    {POSE_TYPE_NORMAL, "KRT_1S2_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_1S2_2"},
    {POSE_TYPE_DAMAGE, "KRT_1D2_2"},
    {POSE_TYPE_ACTION, "KRT_1S2_2"},
};

PoseTableEntry pose_table_mode_2_middle_normal[] = {
    {POSE_TYPE_NORMAL, "KRT_2S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_2S_1"},
    {POSE_TYPE_DAMAGE, "KRT_2D_1"},
    {POSE_TYPE_ACTION, "KRT_2S_1"},
};

PoseTableEntry pose_table_mode_2_middle_normal_counter[] = {
    {POSE_TYPE_NORMAL, "KRT_2S2_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_2S2_1"},
    {POSE_TYPE_DAMAGE, "KRT_2D2_1"},
    {POSE_TYPE_ACTION, "KRT_2S2_1"},
};

PoseTableEntry pose_table_mode_2_middle_charge[] = {
    {POSE_TYPE_NORMAL, "KRT_2S_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_2S_2"},
    {POSE_TYPE_DAMAGE, "KRT_2D_2"},
    {POSE_TYPE_ACTION, "KRT_2S_2"},
};

PoseTableEntry pose_table_mode_2_middle_charge_counter[] = {
    {POSE_TYPE_NORMAL, "KRT_2S2_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_2S2_2"},
    {POSE_TYPE_DAMAGE, "KRT_2D2_2"},
    {POSE_TYPE_ACTION, "KRT_2S2_2"},
};

PoseTableEntry pose_table_mode_2_lower_normal[] = {
    {POSE_TYPE_NORMAL, "KRT_3S_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_3S_1"},
    {POSE_TYPE_DAMAGE, "KRT_3D_2"},
    {POSE_TYPE_ACTION, "KRT_3S_1"},
};

PoseTableEntry pose_table_mode_2_lower_normal_counter[] = {
    {POSE_TYPE_NORMAL, "KRT_3S3_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_3S3_1"},
    {POSE_TYPE_DAMAGE, "KRT_3D3_2"},
    {POSE_TYPE_ACTION, "KRT_3S3_1"},
};

PoseTableEntry pose_table_mode_2_lower_charge[] = {
    {POSE_TYPE_NORMAL, "KRT_3S_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_3S_2"},
    {POSE_TYPE_DAMAGE, "KRT_3D_2"},
    {POSE_TYPE_ACTION, "KRT_3S_2"},
};

PoseTableEntry pose_table_mode_2_lower_charge_counter[] = {
    {POSE_TYPE_NORMAL, "KRT_3S3_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_3S3_2"},
    {POSE_TYPE_DAMAGE, "KRT_3D3_2"},
    {POSE_TYPE_ACTION, "KRT_3S3_2"},
};

PoseTableEntry pose_table_fly_bone[] = {
    {POSE_TYPE_NORMAL, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE, "KRT_Z_1"},
    {POSE_TYPE_ACTION, "KRT_Z_1"},
};

PoseTableEntry pose_table_mode_3[] = {
    {POSE_TYPE_SLEEP, "KRT_N_1"},
    {POSE_TYPE_STOP, "KRT_Z_1"},
    {POSE_TYPE_FREEZE, "KRT_Z_1"},
    {POSE_TYPE_CONFUSE, "KRT_K_1"},
    {POSE_TYPE_POISON, "KRT_X_1"},
    {POSE_TYPE_DIZZY, "KRT_K_1"},
    {POSE_TYPE_NORMAL, "KRT_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KRT_D_2"},
    {30, "KRT_D_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_S_1"},
    {POSE_TYPE_DAMAGE, "KRT_D_1"},
    {POSE_TYPE_ACTION, "KRT_S_1"},
};

PoseTableEntry pose_table_honeduka[] = {
    {POSE_TYPE_NORMAL, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KRT_Z_1"},
    {POSE_TYPE_DAMAGE, "KRT_Z_1"},
    {POSE_TYPE_ACTION, "KRT_Z_1"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DAMAGE_SUB_EVENT, cortez_damage_sub_event},
    {UNIT_DATA_DEAD_EVENT, dead_event},
    {UNIT_DATA_WEAPON_BREAK_EVENT, mode2_honeduka_damage_event},
    {UNIT_DATA_COUNTER_SUPERGUARD_EVENT, cortez_ac_counter_damage_event},
    {UNIT_DATA_AVOID_THORN_EVENT, avoid_thorn_event},
    {UNIT_DATA_AVOID_THORN_HEAD_EVENT, avoid_thorn_event},
    {0, NULL},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_cortez", "c_korutesu1", 0.0f, 0.0f, 0.0f, 145.0f, 90.0f, 30.0f, 145.0f, 90.0f, 30.0f, 20, 30, 255, &defence_honeduka, &defence_honeduka_attr, kMainPart | kGuardTargetPart | kPartNotHammerTarget | kPartNotShellTossTarget, 0, pose_table_mode_1},
    {2, "btl_un_cortez", NULL, 0.0f, 0.0f, 0.0f, 60.0f, 110.0f, 20.0f, 60.0f, 110.0f, 20.0f, 20, 30, 255, &defence_mode2_upper_rib_close, &defence_attr_mode2_upper_rib_close, kSemiImportantPart | kPartUntargetable | kPartNotJumpTarget | kPartNotHammerTarget | kHidden | kNoShadow, 0, pose_table_mode_1},
    {3, "btl_un_cortez", "c_otitabuki", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow, 0, pose_table_mode_1},
    {4, "btl_un_honeduka", "c_honezuka", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 10.0f, 0.0f, 30.0f, 10.0f, 45, 30, 255, &defence_mode1, &defence_attr_mode1, kImportantPart | kSemiImportantPart | kNoShadow, kCounterTopSpiky, pose_table_honeduka},
    {5, "btl_un_honeduka", "c_hone2", 0.0f, 0.0f, 0.0f, 15.0f, 25.0f, 0.0f, 0.0f, 30.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow, 0, pose_table_fly_bone},
    {6, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {7, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {8, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {9, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {10, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {11, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {12, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {13, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {14, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {15, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {16, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {17, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {18, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
    {19, "btl_un_honeduka", "c_hone", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence_mode1, &defence_attr_mode1, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table_fly_bone},
};

static EVT_BEGIN(init_event)
    CALL(_make_cortez_work_area, UNIT_ID_SELF)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SOLID_MODEL)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventPhase, UNIT_ID_SELF, PTR(phase_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_mode1))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(btldefaultevt_Confuse))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 6, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 7, 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, 0)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_claw), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, LW(3))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 39)
    ADD(LW(1), 41)
    SUBTRACT(LW(2), 45)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(0))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_rapier), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, LW(3))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 2)
    ADD(LW(1), 6)
    ADD(LW(2), 60)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(0))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_sword), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, LW(3))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 154)
    ADD(LW(1), 11)
    SUBTRACT(LW(2), 65)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(0))
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_saber), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, LW(3))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 116)
    ADD(LW(1), 28)
    ADD(LW(2), 73)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(0))
    CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 160, 85, 50)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    IF_INT_LESS_OR_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 4)
            CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_1))
            CALL_CHILD(PTR(btldefaultevt_Damage))
            ELSE
            CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_1_hone))
            CALL_CHILD(PTR(btldefaultevt_Damage))
        END_IF
        RETURN
    END_IF
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(btldefaultevt_Damage))
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 7, 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, 2)
        CALL_CHILD(PTR(btldefaultevt_Damage))
        CASE_EQUAL(4)
        CALL_CHILD(PTR(mode2_honeduka_damage_event))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(cortez_ac_counter_damage_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    IF_INT_LESS(LW(0), 4)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
        WAIT_MS(500)
        RETURN
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A3_1"))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
    ADD(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, 0, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_ZERO_HP, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_REVIVAL, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(6.0f), LW(15))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(15), 15, EASING_SINE_IN_OUT, 0, MOVE_SOUND_FROM_TABLE)
    RETURN
EVT_END()

static EVT_BEGIN(avoid_thorn_event)
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 4)
        CALL(btlevtcmd_GetHitPos, UNIT_ID_SELF, 4, LW(0), LW(1), LW(2))
        CALL(evt_eff, STRING(""), STRING("toge_flush"), 0, LW(0), LW(1), LW(2), 60, 0, 0, 0, 0, 0, 0, 0)
    ELSE
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_RIB_COUNTER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1C_3"))
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2C_3"))
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            CASE_EQUAL(3)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3C_3"))
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        END_SWITCH
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, 0)
        CALL_CHILD(cortez_reset_pose_table)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108869)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(evt_sub_random, 3, LW(0))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 8, LW(0))
    END_IF
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    SWITCH(LW(0))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CASE_END
        CASE_DEFAULT
        GOTO(50)
    END_SWITCH
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 7, LW(0))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 7, 0)
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(mode2_rib_close_event)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    BEGIN_SIBLING_EVENT_GET_ID(LW(15))
        SET_VALUE(LW(15), -1)
        SWITCH(LW(0))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2B_1"))
            CALL(btlevtcmd_ftomsec, 80, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_MOVE2"), EVT_NULLPTR, 0, LW(15))
            CASE_EQUAL(3)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3B_1"))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_MOVE2"), EVT_NULLPTR, 0, LW(15))
        END_SWITCH
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        IF_INT_NOT_EQUAL(LW(15), -1)
            CALL(evt_snd_sfxoff, LW(15))
        END_IF
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 1)
    CALL_CHILD(cortez_reset_pose_table)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
    LABEL(50)
    CALL(btlevtcmd_CheckPhase, LW(0), 67108865)
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    SUBTRACT(LW(0), 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_StopWaitEvent, UNIT_ID_SELF)
    CALL_CHILD(mode2_rib_close_event)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
    LABEL(99)
    RETURN
EVT_END()

EVT_BEGIN(attack_event_mode1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 6, LW(0))
    IF_INT_LESS(LW(0), 3)
        SET_VALUE(LW(1), LW(0))
        ADD(LW(1), 1)
        IF_INT_GREATER_OR_EQUAL(LW(1), 3)
            SET_VALUE(LW(1), 0)
        END_IF
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 6, LW(1))
    END_IF
    SWITCH(LW(0))
        CASE_OR_EQUAL(0)
        CASE_OR_EQUAL(1)
        CALL_CHILD(PTR(at1_normal_attack))
        CASE_END
        CASE_EQUAL(2)
        CALL_CHILD(PTR(at1_midaregiri_attack))
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(attack_event_mode2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(at2_hone_rain_attack_event))
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    SWITCH(LW(0))
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CALL_CHILD(PTR(at2_hone_charge_event))
        RETURN
        CASE_END
    END_SWITCH
    CALL(btlevtcmd_DrawLots, LW(0), 2, 25, 75)
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL_CHILD(PTR(at2_hone_charge_event))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(at2_head_bat_event))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(at2_head_bat_event)
    SET_VALUE(LW(12), PTR(&weapon_cortez_mode2_head_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(_check_zenei_kouei, LW(3), LW(13))
    IF_INT_EQUAL(LW(13), 0)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1A_1"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1A_3"))
        END_IF
    ELSE
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1A_2"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1A_4"))
        END_IF
    END_IF
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_HEAD_ATTACK1"), EVT_NULLPTR, 0, LW(15))
        WAIT_MS(667)
        CALL(evt_snd_sfxoff, LW(15))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_ftomsec, 50, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_HEAD_ATTACK2"), EVT_NULLPTR, 0, LW(15))
        CALL(btlevtcmd_ftomsec, 18, LW(0))
        WAIT_MS(LW(0))
        CALL(evt_snd_sfxoff, LW(15))
    END_INLINE_EVENT
    WAIT_MS(1116)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_DEFAULT
        GOTO(91)
    END_SWITCH
    LABEL(90)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    GOTO(99)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(at2_hone_charge_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(1))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 50, 140, 520, 50, 90, -260, 60, 4)
        IF_INT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2A_1"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2A1_1"))
        END_IF
        CASE_EQUAL(2)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 50, 140, 520, 50, 90, -260, 60, 4)
        IF_INT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2A_2"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2A2_2"))
        END_IF
        CASE_EQUAL(3)
        CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
        CALL(evt_btl_camera_set_moveto, 0, 50, 140, 520, 50, 90, -260, 60, 4)
        IF_INT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2A_3"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2A3_3"))
        END_IF
    END_SWITCH
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_INHALE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    WAIT_MS(2000)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL_CHILD(cortez_reset_pose_table)
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, UNIT_ID_SELF, 1, PTR(&weapon_cortez_mode2_charge), DAMAGE_RESULT_FINAL_HIT, LW(5))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(at2_hone_rain_attack_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 10, 0)
    SET_VALUE(LW(12), PTR(&weapon_cortez_mode2_bone_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclareAll, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3A_1"))
        CASE_EQUAL(2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3A_2"))
        CASE_EQUAL(3)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3A_3"))
    END_SWITCH
    WAIT_MS(1700)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    END_SIBLING_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FLY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 5, kHidden)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 5, STRING("KRT_A_1"))
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 5)
        WAIT_MS(500)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 5, kHidden)
    END_INLINE_EVENT
    SET_VALUE(LW(5), 12)
    SPAWN_EVENT(_bone_rain_event)
    SET_VALUE(LW(5), 13)
    SPAWN_EVENT(_bone_rain_event)
    SET_VALUE(LW(5), 14)
    SPAWN_EVENT(_bone_rain_event)
    SET_VALUE(LW(5), 15)
    SPAWN_EVENT(_bone_rain_event)
    SET_VALUE(LW(5), 16)
    SPAWN_EVENT(_bone_rain_event)
    SET_VALUE(LW(5), 17)
    SPAWN_EVENT(_bone_rain_event)
    SET_VALUE(LW(5), 18)
    SPAWN_EVENT(_bone_rain_event)
    SET_VALUE(LW(5), 19)
    SPAWN_EVENT(_bone_rain_event)
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 6, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 6, kHidden)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 6, FLOAT(0.89941406f))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 6, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 6, 0, 0, 0)
        LOOP(30)
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 6, 0, 0, 30)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 9, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 9, kHidden)
            CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 9, FLOAT(0.89941406f))
            CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
            CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 9, LW(0), LW(1), LW(2), 30, MOVE_SOUND_FROM_TABLE)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_SetPartsRotateOffset, UNIT_ID_SELF, 9, 0, 0, 0)
            LOOP(30)
                CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 9, 0, 0, 30)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    SET_VALUE(LW(10), 0)
    LABEL(10)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_DEFAULT
        GOTO(90)
    END_SWITCH
    LABEL(90)
    IF_INT_EQUAL(LW(10), 0)
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_JumpPartsContinue, UNIT_ID_SELF, 6)
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 6, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
            CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 6, FLOAT(0.69921875f))
            CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 6, LW(0), LW(1), LW(2), 24, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 6, kHidden)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            LOOP(24)
                CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 6, 0, 0, 30)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    ELSE
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_JumpPartsContinue, UNIT_ID_SELF, 9)
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 9, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
            CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 9, FLOAT(0.69921875f))
            CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 9, LW(0), LW(1), LW(2), 24, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 9, kHidden)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            LOOP(24)
                CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 9, 0, 0, 30)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    GOTO(97)
    LABEL(91)
    IF_INT_EQUAL(LW(10), 0)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    END_IF
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(10), 0)
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 6, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 80)
            CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 6, FLOAT(0.5996094f))
            CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 6, LW(0), LW(1), LW(2), 26, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 6, kHidden)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            LOOP(26)
                CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 6, 0, 0, 30)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    ELSE
        BEGIN_INLINE_EVENT
            CALL(btlevtcmd_GetPartsPos, UNIT_ID_SELF, 9, LW(0), LW(1), LW(2))
            CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 80)
            CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, 9, FLOAT(0.5996094f))
            CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, 9, LW(0), LW(1), LW(2), 26, MOVE_SOUND_FROM_TABLE)
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 9, kHidden)
        END_INLINE_EVENT
        BEGIN_INLINE_EVENT
            LOOP(26)
                CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, 9, 0, 0, 30)
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT
    END_IF
    LABEL(97)
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        WAIT_MS(60)
        ADD(LW(10), 1)
        GOTO(10)
    END_IF
    WAIT_FRAMES(84)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(_bone_rain_event)
    CALL(evt_sub_random, 20, LW(10))
    ADD(LW(10), 20)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_sub_random, 90, LW(7))
        SUBTRACT(LW(7), 45)
        ADD(LW(0), LW(7))
        CALL(evt_sub_random, 60, LW(7))
        SUBTRACT(LW(7), 30)
        ADD(LW(2), LW(7))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, LW(5), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, LW(5), kHidden)
        CALL(btlevtcmd_SetPartsFallAccel, UNIT_ID_SELF, LW(5), FLOAT(1.3994141f))
        CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(evt_sub_random, 120, LW(7))
        SUBTRACT(LW(7), 60)
        ADD(LW(0), LW(7))
        CALL(evt_sub_random, 60, LW(7))
        SUBTRACT(LW(7), 30)
        ADD(LW(2), LW(7))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_JumpPartsPosition, UNIT_ID_SELF, LW(5), LW(0), LW(1), LW(2), LW(10), MOVE_SOUND_FROM_TABLE)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, LW(5), kHidden)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        LOOP(LW(10))
            CALL(btlevtcmd_AddPartsRotate, UNIT_ID_SELF, LW(5), 0, 0, 30)
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(attack_event_mode3)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 9, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(1))
        DIVIDE(LW(1), 2)
        CALL(btlevtcmd_GetHp, UNIT_ID_SELF, LW(0))
        IF_INT_LESS_OR_EQUAL(LW(0), LW(1))
            CALL(_cortez_check_drain_audience, UNIT_ID_SELF, LW(0))
            IF_INT_GREATER_OR_EQUAL(LW(0), 1)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 9, LW(0))
                ADD(LW(0), 1)
                CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 9, LW(0))
                CALL_CHILD(PTR(attack_audience_event))
                RETURN
            END_IF
        END_IF
    END_IF
    CALL_CHILD(PTR(at3_kamitsuki_attack))
    RETURN
EVT_END()

static EVT_BEGIN(attack_audience_event)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 140, 120, 500, 140, 85, 9, 60, 11)
    WAIT_FRAMES(60)
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_OFF)
    CALL(btlevtcmd_SetTalkPose, UNIT_ID_SELF, STRING("KRT_T_1"))
    CALL(btlevtcmd_SetStayPose, UNIT_ID_SELF, STRING("KRT_S_1"))
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg5_dou_24_bt01"), 0, -2)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(evt_btl_camera_set_moveto, 0, 80, 115, 820, 80, 60, 6, 60, 11)
    CALL(evt_batstage_set_aud_dark, 0, 60, 0)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), 120, 60, -10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(evt_msg_print, MSG_PRINT_NO_SPEAKER, STRING("stg5_dou_24_bt02"), 0, -2)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A2_1"))
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_INHALE2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    BEGIN_SIBLING_EVENT
        CALL(evt_snd_sfxon, STRING("SFX_AUDIENCE_PANIC_SCREAM1"), LW(15))
        WAIT_FRAMES(120)
        CALL(_cortez_get_sfx_vol, LW(15), LW(14))
        SET_VALUE(LW(13), 120)
        LOOP(120)
            WAIT_FRAMES(1)
            IF_INT_GREATER(LW(13), 0)
                SUBTRACT(LW(13), 1)
            END_IF
            SET_VALUE(LW(0), LW(14))
            MULTIPLY(LW(0), LW(13))
            DIVIDE(LW(0), 120)
            CALL(evt_snd_sfx_vol, LW(15), LW(0))
            DEBUG_PUT_REG(LW(14))
            DEBUG_PUT_REG(LW(0))
        END_LOOP
        CALL(evt_snd_sfxoff, LW(15))
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A2_2"))
    CALL(_cortez_collect_energy, UNIT_ID_SELF)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A2_3"))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    END_SIBLING_EVENT
    WAIT_MS(125)
    CALL(_cortez_drain_energy, UNIT_ID_SELF, LW(5))
    CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, 1, LW(5))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("recovery"), 0, LW(0), LW(1), LW(2), LW(5), 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1500)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    BEGIN_INLINE_EVENT_GET_ID(LW(15))
        CALL(_cortez_recovery_audience, UNIT_ID_SELF)
    END_INLINE_EVENT
    CALL(evt_batstage_return_aud_dark_base, 60, 0)
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 60, -10, EASING_QUADRATIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_WaitEventEnd, LW(15))
    CALL(btlevtcmd_StatusWindowOnOff, STATUS_WINDOW_EVENT_ON)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(at3_kamitsuki_attack)
    SET_VALUE(LW(12), PTR(&weapon_cortez_mode3_kamitsuki_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_W_1"))
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionAdd, LW(3), LW(0), 40)
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(6.0f), LW(15))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(15), -12, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_ROUND1"), EVT_NULLPTR, 0, LW(15))
        CALL(btlevtcmd_ftomsec, 32, LW(0))
        WAIT_MS(LW(0))
        CALL(evt_snd_sfxoff, LW(15))
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 46, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BITE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A3_3"))
    CALL(btlevtcmd_ftomsec, 50, LW(0))
    WAIT_MS(LW(0))
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_DEFAULT
        GOTO(91)
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    IF_INT_EQUAL(LW(5), 21)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A3_1"))
        GOTO(98)
    END_IF
    LABEL(97)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_Z_1"))
    LABEL(98)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 50)
    ADD(LW(1), 10)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 30, -10, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_S_1"))
    CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetMoveFrame, UNIT_ID_SELF, LW(0), LW(1), LW(2), FLOAT(6.0f), LW(15))
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), LW(15), 15, EASING_SINE_IN_OUT, 0, MOVE_SOUND_FROM_TABLE)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(at1_normal_attack)
    IF_INT_EQUAL(LW(0), 0)
        SET_VALUE(LW(12), PTR(&weapon_cortez_normal_front_attack))
    ELSE
        SET_VALUE(LW(12), PTR(&weapon_cortez_normal_rear_attack))
    END_IF
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_READY1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(_check_zenei_kouei, LW(3), LW(13))
    IF_INT_EQUAL(LW(13), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A1_1"))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ftomsec, 95, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_STICK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 99, LW(0))
        WAIT_MS(LW(0))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A1_2"))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ftomsec, 95, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_STICK1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 100, LW(0))
        WAIT_MS(LW(0))
    END_IF
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_DEFAULT
        GOTO(91)
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    LABEL(97)
    WAIT_FRAMES(3)
    CALL(evt_btl_camera_shake_h, 0, 2, 0, 10, 13)
    WAIT_FRAMES(7)
    CALL(evt_btl_camera_shake_h, 0, 8, 0, 30, 13)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(at1_midaregiri_attack)
    SET_VALUE(LW(12), PTR(&weapon_cortez_midaregiri_attack))
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(12))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(12), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        GOTO(99)
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(12))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(12))
    CALL(_check_zenei_kouei, LW(3), LW(13))
    IF_INT_EQUAL(LW(13), 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A2_1"))
    ELSE
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_A2_2"))
    END_IF
    SET_VALUE(LW(0), 0)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 33, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 84, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 145, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(btlevtcmd_ftomsec, 198, LW(0))
        WAIT_MS(LW(0))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_SHAKE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    LABEL(10)
    SWITCH(LW(10))
        CASE_EQUAL(0)
        CALL(btlevtcmd_ftomsec, 38, LW(0))
        WAIT_MS(LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_ftomsec, 60, LW(0))
        WAIT_MS(LW(0))
        CASE_EQUAL(2)
        CALL(btlevtcmd_ftomsec, 54, LW(0))
        WAIT_MS(LW(0))
        CASE_DEFAULT
        CALL(btlevtcmd_ftomsec, 56, LW(0))
        WAIT_MS(LW(0))
    END_SWITCH
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
        GOTO(90)
        CASE_EQUAL(6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
        GOTO(90)
        CASE_EQUAL(2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
        GOTO(90)
        CASE_DEFAULT
        GOTO(91)
    END_SWITCH
    LABEL(90)
    GOTO(97)
    LABEL(91)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(12))
    IF_INT_GREATER_OR_EQUAL(LW(10), 3)
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), DAMAGE_RESULT_FINAL_HIT, LW(5))
    ELSE
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(12), 0, LW(5))
    END_IF
    LABEL(97)
    ADD(LW(10), 1)
    IF_INT_LESS_OR_EQUAL(LW(10), 3)
        GOTO(10)
    END_IF
    LABEL(99)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    RETURN
EVT_END()

BattleUnitSetup entry_claw = {
    &unit_boss_cortez_claw, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_rapier = {
    &unit_boss_cortez_rapier, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_sword = {
    &unit_boss_cortez_sword, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

BattleUnitSetup entry_saber = {
    &unit_boss_cortez_saber, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {1000.0f, 0.0f, 1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

EVT_BEGIN(cortez_reset_pose_table)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_1))
        CASE_EQUAL(1)
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 160, 120, 50)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_upper_normal))
                ELSE
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_upper_charge))
            END_IF
            CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 2, PTR(&defence_mode2_upper_rib_close))
            CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 2, PTR(&defence_attr_mode2_upper_rib_close))
            ELSE
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_upper_normal_counter))
                ELSE
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_upper_charge_counter))
            END_IF
            CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 2, PTR(&defence_mode2_upper_rib_open))
            CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 2, PTR(&defence_attr_mode2_upper_rib_open))
        END_IF
        CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence_mode2_upper_head))
        CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&defence_attr_mode2_upper_head))
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kPartNotHammerTarget | kPartNotShellTossTarget)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kPartNotJumpTarget | kPartNotHammerTarget | kPartNotShellTossTarget)
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 20, 15, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 20, 15, 0)
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 2, 0, 0, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 2, 0, 0, 0)
        CASE_EQUAL(2)
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 160, 85, 50)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_middle_normal))
                ELSE
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_middle_charge))
            END_IF
            CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 2, PTR(&defence_mode2_middle_rib_close))
            CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 2, PTR(&defence_attr_mode2_middle_rib_close))
            ELSE
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_middle_normal_counter))
                ELSE
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_middle_charge_counter))
            END_IF
            CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 2, PTR(&defence_mode2_middle_rib_open))
            CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 2, PTR(&defence_attr_mode2_middle_rib_open))
        END_IF
        CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence_mode2_middle_head))
        CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&defence_attr_mode2_middle_head))
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kPartNotHammerTarget | kPartNotShellTossTarget)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kPartNotJumpTarget | kPartNotHammerTarget | kPartNotShellTossTarget)
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, 10, 10, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, 10, 10, 0)
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 2, -7, -20, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 2, -7, -20, 0)
        CASE_EQUAL(3)
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 150, 85, 50)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_lower_normal))
                ELSE
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_lower_charge))
            END_IF
            CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 2, PTR(&defence_mode2_lower_rib_close))
            CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 2, PTR(&defence_attr_mode2_lower_rib_close))
            ELSE
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 10, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_lower_normal_counter))
                ELSE
                CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_2_lower_charge_counter))
            END_IF
            CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 2, PTR(&defence_mode2_lower_rib_open))
            CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 2, PTR(&defence_attr_mode2_lower_rib_open))
            CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kPartNotHammerTarget | kPartNotShellTossTarget)
        END_IF
        CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence_mode2_lower_head))
        CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&defence_attr_mode2_lower_head))
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kPartNotHammerTarget | kPartNotShellTossTarget)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kPartNotHammerTarget | kPartNotShellTossTarget)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kPartNotJumpTarget)
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, -15, -5, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, -15, -5, 0)
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 2, -20, -35, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 2, -20, -35, 0)
        CASE_DEFAULT
        CALL(btlevtcmd_SetKissHitOffset, UNIT_ID_SELF, 30, 50, 50)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_3))
        CALL(btlevtcmd_SetPartsDefenceTable, UNIT_ID_SELF, 1, PTR(&defence_mode3))
        CALL(btlevtcmd_SetPartsDefenceAttrTable, UNIT_ID_SELF, 1, PTR(&defence_attr_mode3))
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kPartNotHammerTarget | kPartNotShellTossTarget)
        CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitAirborne)
        CALL(btlevtcmd_SetHitOffset, UNIT_ID_SELF, 1, -135, -64, 0)
        CALL(btlevtcmd_SetHitCursorOffset, UNIT_ID_SELF, 1, -135, -65, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(cortez_damage_sub_event)
    CALL_CHILD(PTR(cortez_reset_pose_table))
    CALL(btlevtcmd_GetDamage, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_GetHitPos, UNIT_ID_SELF, LW(0), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_BTL_ATTACK_MISS2"), LW(0), LW(1), LW(2), 0)
        GOTO(50)
    END_IF
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
        ELSE
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1D_3"))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2D_3"))
            CASE_EQUAL(3)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
            IF_INT_EQUAL(LW(0), 0)
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3D_3"))
                ELSE
                CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3D3_1"))
            END_IF
        END_SWITCH
    END_IF
    WAIT_FRAMES(30)
    GOTO(60)
    LABEL(50)
    LABEL(60)
    CALL(btlevtcmd_GetDamagePartsId, UNIT_ID_SELF, LW(0))
    IF_INT_EQUAL(LW(0), 2)
        CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_FINAL_HIT, LW(1))
        IF_INT_NOT_EQUAL(LW(1), 0)
            CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_DEATH, LW(1))
            IF_INT_EQUAL(LW(1), 0)
                CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
                SWITCH(LW(0))
                    CASE_OR_EQUAL(1)
                    CASE_OR_EQUAL(2)
                    CASE_OR_EQUAL(3)
                    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
                    IF_INT_EQUAL(LW(0), 0)
                        CALL_CHILD(PTR(mode2_rib_open_event))
                    END_IF
                    CASE_END
                END_SWITCH
            END_IF
        END_IF
    END_IF
END_IF
LABEL(99)
RETURN
EVT_END()

static EVT_BEGIN(dead_event)
    CALL(btlevtcmd_ClearAllStatus, UNIT_ID_SELF)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 1)
        CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_mode2))
        CALL(btlevtcmd_RecoverHp, UNIT_ID_SELF, 1, 999)
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 4, kPartUntargetable)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_D2_1"))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ftomsec, 125, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ftomsec, 120, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ftomsec, 118, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ftomsec, 117, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_SWORD_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            WAIT_MS(2000)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_Z_1"))
            CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_D2_2"))
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_ftomsec, 33, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_MOVE2"), EVT_NULLPTR, 0, LW(7))
            CALL(btlevtcmd_ftomsec, 36, LW(0))
            WAIT_MS(LW(0))
            CALL(evt_snd_sfxoff, LW(7))
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SetHp, UNIT_ID_SELF, LW(0))
        CALL(evt_fade_entry, 24, 1000, 0, 0, 0)
        CALL(evt_fade_set_anim_ofs_pos, 0, 130)
        CALL(evt_fade_set_anim_virtual_pos, 0, 0, 80)
        CALL(evt_fade_end_wait)
        CALL(btlevtcmd_AnimeChangeGroup, UNIT_ID_SELF, 1, STRING("c_korutesu2"))
        CALL_CHILD(cortez_reset_pose_table)
        CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, PTR(pose_sound_timing_table))
        CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
        CALL(evt_fade_entry, 25, 1000, 0, 0, 0)
        CALL(evt_fade_set_anim_ofs_pos, 0, 130)
        CALL(evt_fade_set_anim_virtual_pos, 0, 0, 80)
        CALL(evt_fade_end_wait)
        CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, -80, 120, 0)
        CALL(btlevtcmd_SetCutWidth, UNIT_ID_SELF, 200)
        CALL(btlevtcmd_SetCutHeight, UNIT_ID_SELF, 100)
        CALL(btlevtcmd_reset_turn)
        CASE_OR_EQUAL(1)
        CASE_OR_EQUAL(2)
        CASE_OR_EQUAL(3)
        CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event_mode3))
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1B_1"))
            CASE_EQUAL(2)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2B_1"))
            CALL(btlevtcmd_ftomsec, 80, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_MOVE2"), EVT_NULLPTR, 0, LW(15))
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            CALL(evt_snd_sfxoff, LW(15))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1B_1"))
            CASE_EQUAL(3)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_MOVE2"), EVT_NULLPTR, 0, LW(15))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3B_1"))
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            CALL(evt_snd_sfxoff, LW(15))
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1B_1"))
        END_SWITCH
        BEGIN_SIBLING_EVENT
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 7, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 8, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 4, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 6, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 7, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 8, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 8, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 5, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 7, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 5, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 7, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 14, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            CALL(btlevtcmd_ftomsec, 30, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 4)
        WAIT_MS(1000)
        CALL(btlevtcmd_AnimeSetPoseSoundTable, UNIT_ID_SELF, 1, 0)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 3, STRING("KRT_A_1"))
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 3, kHidden)
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 3)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(3))
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), 1)
        CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, (kPartUntargetable | kHidden | kNoShadow))
        CALL(btlevtcmd_OffAttribute, LW(3), kUnitNoHpGauge)
        CALL(btlevtcmd_SetMaxMoveCount, LW(3), 1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(3))
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), 1)
        CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, (kPartUntargetable | kHidden | kNoShadow))
        CALL(btlevtcmd_OffAttribute, LW(3), kUnitNoHpGauge)
        CALL(btlevtcmd_SetMaxMoveCount, LW(3), 1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 4, LW(3))
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), 1)
        CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, (kPartUntargetable | kHidden | kNoShadow))
        CALL(btlevtcmd_OffAttribute, LW(3), kUnitNoHpGauge)
        CALL(btlevtcmd_SetMaxMoveCount, LW(3), 1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 3, LW(3))
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), 1)
        CALL(btlevtcmd_OffPartsAttribute, LW(3), 1, (kPartUntargetable | kHidden | kNoShadow))
        CALL(btlevtcmd_OffAttribute, LW(3), kUnitNoHpGauge)
        CALL(btlevtcmd_SetMaxMoveCount, LW(3), 1)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 3, kHidden | kNoShadow)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 4, kPartUntargetable | kHidden | kNoShadow)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kPartUntargetable)
        SET_VALUE(LW(11), 1)
        LOOP(4)
            CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, LW(11), LW(9))
            BEGIN_INLINE_EVENT
                CALL_CHILD(cortez_weapon_get_home_height)
                CALL(btlevtcmd_GetUnitWork, LW(9), 2, LW(6))
                IF_INT_EQUAL(LW(6), 2)
                    CALL(btlevtcmd_FaceDirectionAdd, LW(9), LW(0), 15)
                END_IF
                CALL(btlevtcmd_PhaseEventStartDeclare, LW(9))
                CALL(btlevtcmd_DivePosition, LW(9), LW(0), LW(5), LW(2), 60, 0, EASING_CUBIC_OUT, 0, MOVE_SOUND_FROM_TABLE)
                CALL(btlevtcmd_SetHomePos, LW(9), LW(0), LW(5), LW(2))
                CALL_CHILD(cortez_weapon_set_formation_attribute)
                CALL(btlevtcmd_StartWaitEvent, LW(9))
            END_INLINE_EVENT
            ADD(LW(11), 1)
        END_LOOP
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        ADD(LW(1), 40)
        CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_fade_entry, 24, 1000, 0, 0, 0)
        CALL(evt_fade_set_anim_ofs_pos, 0, 130)
        CALL(evt_fade_set_anim_virtual_pos, 0, 0, 80)
        CALL(evt_fade_end_wait)
        CALL(btlevtcmd_AnimeChangeGroup, UNIT_ID_SELF, 1, STRING("c_korutesu3"))
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_mode_3))
        CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_SetDiveSound, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_MOVE3"), 0, 0, 0, 0)
        CALL(btlevtcmd_GetHomePos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(evt_fade_entry, 25, 1000, 0, 0, 0)
        CALL(evt_fade_set_anim_ofs_pos, 0, 130)
        CALL(evt_fade_set_anim_virtual_pos, 0, 0, 80)
        CALL(evt_fade_end_wait)
        CALL(btlevtcmd_GetMaxHp, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SetHp, UNIT_ID_SELF, LW(0))
        CALL_CHILD(cortez_reset_pose_table)
        CALL(btlevtcmd_SetCutBaseOffset, UNIT_ID_SELF, 0, 48, 0)
        CALL(btlevtcmd_SetCutWidth, UNIT_ID_SELF, 90)
        CALL(btlevtcmd_SetCutHeight, UNIT_ID_SELF, 96)
        CALL(btlevtcmd_reset_turn)
        CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
        CASE_END
        CASE_DEFAULT
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), 0, -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, -1)
            CALL(btlevtcmd_GetDataFromDataTable, LW(3), LW(0), UNIT_DATA_WEAPON_BREAK_EVENT)
            CALL(btlevtcmd_RunHitEventDirect, LW(3), 0, LW(0), 0)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), 0, -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, -1)
            CALL(btlevtcmd_GetDataFromDataTable, LW(3), LW(0), UNIT_DATA_WEAPON_BREAK_EVENT)
            CALL(btlevtcmd_RunHitEventDirect, LW(3), 0, LW(0), 0)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 3, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), 0, -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, -1)
            CALL(btlevtcmd_GetDataFromDataTable, LW(3), LW(0), UNIT_DATA_WEAPON_BREAK_EVENT)
            CALL(btlevtcmd_RunHitEventDirect, LW(3), 0, LW(0), 0)
        END_IF
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 4, LW(3))
        IF_INT_NOT_EQUAL(LW(3), -1)
            CALL(btlevtcmd_SetUnitWork, LW(3), 0, -1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, -1)
            CALL(btlevtcmd_GetDataFromDataTable, LW(3), LW(0), UNIT_DATA_WEAPON_BREAK_EVENT)
            CALL(btlevtcmd_RunHitEventDirect, LW(3), 0, LW(0), 0)
        END_IF
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_D_3"))
        BEGIN_SIBLING_EVENT
            WAIT_MS(1500)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_BONE_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
        END_SIBLING_EVENT
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHidden | kNoShadow)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_TransStageFloorPosition, LW(0), LW(1), LW(2))
        ADD(LW(2), 10)
        CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(1.0f), 0, 0, 0, 0, 0, 0, 0)
        CALL(btlevtcmd_GetCoin, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_StoreCoin, LW(0))
        CALL(btlevtcmd_GetExp, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_StoreExp, LW(0))
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(3), LW(1), LW(2))
        ADD(LW(2), 10)
        ADD(LW(1), 30)
        CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(1000)
        CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 1)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(mode2_rib_open_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, 2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1D2_3"))
        CASE_EQUAL(2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2D2_3"))
        CASE_EQUAL(3)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3D3_3"))
    END_SWITCH
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_INHALE1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kPartUntargetable)
    CALL_CHILD(cortez_reset_pose_table)
    RETURN
EVT_END()

EVT_BEGIN(mode2_rib_close_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 11, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1D2_4"))
        CASE_EQUAL(2)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2D2_4"))
        CASE_EQUAL(3)
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_3D3_4"))
    END_SWITCH
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_RIB_COUNTER1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
    CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kPartUntargetable)
    CALL_CHILD(cortez_reset_pose_table)
    RETURN
EVT_END()

EVT_BEGIN(mode2_honeduka_damage_event)
    CALL(btlevtcmd_OnUnitFlag, UNIT_ID_SELF, BATTLE_UNIT_FLAG_SHOW_HP)
    SET_VALUE(LW(8), 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 7, 1)
    CALL(btlevtcmd_CheckDamageCode, UNIT_ID_SELF, DAMAGE_RESULT_FINAL_HIT, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        BEGIN_SIBLING_EVENT_GET_ID(LW(8))
            CALL_CHILD(PTR(mode_change_event))
        END_SIBLING_EVENT
    END_IF
    BEGIN_SIBLING_EVENT_GET_ID(LW(9))
        CALL(evt_sub_intpl_init, EASING_WAVE_SHRINKING_FASTER, 6, 0, 30)
        LOOP(30)
            CALL(evt_sub_intpl_get_value)
            CALL(btlevtcmd_SetPartsDispOffset, UNIT_ID_SELF, 4, 0, LW(0), 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(btlevtcmd_WaitEventEnd, LW(9))
    IF_INT_NOT_EQUAL(LW(8), 0)
        CALL(btlevtcmd_WaitEventEnd, LW(8))
    END_IF
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(1))
    IF_INT_NOT_EQUAL(LW(1), 0)
        CALL_CHILD(PTR(dead_event))
        RETURN
    END_IF
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

EVT_BEGIN(mode_change_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 5, LW(0))
    SWITCH(LW(0))
        CASE_EQUAL(1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1H_1"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_1H2_1"))
        END_IF
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_MOVE2"), EVT_NULLPTR, 0, LW(15))
        CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
        CALL(evt_snd_sfxoff, LW(15))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 2)
        CALL_CHILD(cortez_reset_pose_table)
        CASE_EQUAL(2)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 11, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2H_1"))
        ELSE
            CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KRT_2H2_1"))
        END_IF
        BEGIN_SIBLING_EVENT_GET_ID(LW(15))
            CALL(btlevtcmd_ftomsec, 80, LW(0))
            WAIT_MS(LW(0))
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_BOSS_KOR_MOVE2"), EVT_NULLPTR, 0, LW(15))
            CALL(btlevtcmd_AnimeWaitPlayComplete, UNIT_ID_SELF, 1)
            CALL(evt_snd_sfxoff, LW(15))
        END_SIBLING_EVENT
        CALL(btlevtcmd_WaitEventEnd, LW(15))
        CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 5, 3)
        CALL_CHILD(cortez_reset_pose_table)
        CALL_CHILD(mode2_rib_open_event)
        CASE_DEFAULT
    END_SWITCH
    RETURN
EVT_END()

static USER_FUNC(_check_zenei_kouei) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 out = args[1];
    f32 x, y, z;

    id = BattleTransID(event, id);
    BtlUnit_GetPos(BattleGetUnitPtr(wp, id), &x, &y, &z);
    if (x >= -120.0f) {
        evtSetValue(event, out, 0);
    } else {
        evtSetValue(event, out, 1);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(_cortez_recovery_audience) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    CortezWork* work;
    BOOL done;
    s32 i;
    s32 aid;

    id = BattleTransID(event, id);
    work = (CortezWork*)BattleGetUnitPtr(wp, id)->countersetWeapon;
    if (isFirstCall) {
        work->state = 0;
    }
    switch (work->state) {
        case 0:
            for (i = 0; i < 100; i++) {
                if (work->audienceId[i] != -1) {
                    work->seq[i] = 0;
                }
            }
            work->state++;
        case 1:
            done = TRUE;
            for (i = 0; i < 100; i++) {
                if (work->audienceId[i] != -1) {
                    switch (work->seq[i]) {
                        case 0:
                            work->seq[i]++;
                            work->timer[i] = 0;
                        case 1:
                            work->timer[i]++;
                            done = TRUE;
                            break;
                    }
                }
            }
            if (!done) {
                break;
            }
            work->state++;
        case 2:
            for (i = 0; i < 100; i++) {
                aid = work->audienceId[i];
                if (aid != -1) {
                    BattleAudience_Delete(aid);
                    BattleAudienceAddTargetNum(0.0f, -1.0f);
                }
            }
            work->state++;
            return EVT_RETURN_DONE;
        default:
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(_cortez_drain_energy) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 out = args[1];
    CortezWork* work;
    s32 i;

    id = BattleTransID(event, id);
    work = (CortezWork*)BattleGetUnitPtr(wp, id)->countersetWeapon;
    for (i = 0; i < 100; i++) {
        if (work->eff[i] != NULL) {
            effDelete(work->eff[i]);
            work->eff[i] = NULL;
        }
    }
    evtSetValue(event, out, work->count);
    return EVT_RETURN_DONE;
}

USER_FUNC(_cortez_collect_energy) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, *args++);
    BattleUnit* unit;
    CortezWork* work;
    BattleAudienceMember* member;
    BOOL done;
    s32 aid;
    s32 i;
    BOOL kumo;
    f32 x, y, z;

    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    work = (CortezWork*)unit->countersetWeapon;
    if (isFirstCall) {
        work->state = 0;
    }
    switch (work->state) {
        case 0:
            kumo = FALSE;
            for (i = 0; i < 100; i++) {
                aid = work->audienceId[i];
                if (aid != -1) {
                    member = BattleAudienceGetPtr(aid);
                    member->flags |= (AUDIENCE_MEMBER_FLAG_NOT_EXIST | AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL);
                    BattleAudience_SetAnim(aid, 9, TRUE);
                    work->wait[i] = irand(60) + 30;
                    work->srcPos[i] = member->position;
                    BtlUnit_GetPos(unit, &work->dstPos[i].x, &work->dstPos[i].y, &work->dstPos[i].z);
                    work->dstPos[i].y += 20.0f;
                    effSpiritEntry(0, work->wait[i] + 60, work->srcPos[i].x, work->srcPos[i].y, work->srcPos[i].z,
                                   work->dstPos[i].x, work->dstPos[i].y, work->dstPos[i].z, 2.0f);
                    if (!kumo) {
                        kumo = TRUE;
                        effKumokumoN64Entry(1, 60, work->dstPos[i].x, work->dstPos[i].y, work->dstPos[i].z, 180.0f,
                                            -1.0f, 1.0f);
                    }
                }
            }
            work->state++;
        case 1:
            done = TRUE;
            for (i = 0; i < 100; i++) {
                aid = work->audienceId[i];
                if (aid != -1) {
                    switch (work->seq[i]) {
                        case 0:
                            if (work->wait[i] > 0) {
                                work->wait[i]--;
                                done = FALSE;
                                break;
                            }
                            work->seq[i]++;
                            work->timer[i] = 0;
                        case 1:
                            work->timer[i]++;
                            BattleAudience_GetRotate(aid, &x, &y, &z);
                            x = intplGetValue(EASING_LINEAR, 0.0f, -90.0f, work->timer[i], 30);
                            BattleAudience_SetRotate(aid, x, y, z);
                            if (work->timer[i] >= 30) {
                                work->seq[i]++;
                            } else {
                                done = FALSE;
                                break;
                            }
                        case 2:
                            work->timer[i] = 0;
                            work->seq[i]++;
                        case 3:
                            work->timer[i]++;
                            if (work->timer[i] >= 60) {
                                work->seq[i]++;
                            }
                            done = FALSE;
                            break;
                        case 4:
                            break;
                    }
                }
            }
            if (!done) {
                break;
            }
        default:
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(_cortez_check_drain_audience) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 out = args[1];
    s32 seat;
    CortezWork* work;
    s32 i;

    id = BattleTransID(event, id);
    work = (CortezWork*)BattleGetUnitPtr(wp, id)->countersetWeapon;
    work->count = 0;
    for (i = 0; i < 100; i++) {
        work->audienceId[i] = -1;
        work->eff[i] = NULL;
        work->seq[i] = 0;
        seat = 10 + (i / 10) * 20 + i % 10;
        if ((u8)BattleAudience_GetExist(seat)) {
            work->audienceId[i] = seat;
            work->count++;
        }
    }
    evtSetValue(event, out, work->count);
    return EVT_RETURN_DONE;
}

USER_FUNC(_cortez_get_sfx_vol) {
    EvtWord* args = event->args;
    s32 id = evtGetValue(event, args[0]);
    s32 out = args[1];

    evtSetValue(event, out, psndSFX_get_vol(id));
    return EVT_RETURN_DONE;
}

USER_FUNC(_make_cortez_work_area) {
    BattleWork* wp = _battleWorkPointer;
    EvtWord* args = event->args;
    BattleUnit* unit;
    s32 id;

    id = evtGetValue(event, *args++);
    id = BattleTransID(event, id);
    unit = BattleGetUnitPtr(wp, id);
    unit->countersetWeapon = BattleAlloc(sizeof(CortezWork));
    return EVT_RETURN_DONE;
}
