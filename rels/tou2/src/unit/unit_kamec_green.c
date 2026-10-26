#include "battle/battle_unit.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_camera.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "eff/n64/eff_pokopi_pcharge_n64.h"

#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "battle/battle_sub.h"
#include "driver/effdrv.h"
#include "manager/evtmgr_cmd.h"
#include "string.h"
#include "system.h"
#include "battle/battle_event_subset.h"
#include "battle/battle_event_default.h"
#include "battle/battle_event_cmd.h"
#include "battle/battle_weapon_power.h"
#include "party/party_motion.h"
#include "battle/battle_database_common.h"
#include "battle/battle_damage.h"

EffectEntry* effThunderflareN64Entry(s32 type, f32 x, f32 y, f32 z, f32 scale, s32 lifetime);

static USER_FUNC(eff_magic);
static USER_FUNC(kamec_copy_status);
static USER_FUNC(kamec_get_bunsin);
static USER_FUNC(kamec_get_live_num);
static USER_FUNC(eff_magic_get_posX);
static USER_FUNC(eff_staff2);
static USER_FUNC(eff_staff1);
static USER_FUNC(kamec_shuffle);

BattleUnitKind unit_kamec_green;
BattleUnitKind unit_kamec_green_bunsin;
static BattleUnitDefense defence;
static BattleUnitDefenseAttr defence_attr;
static StatusVulnerability regist;
static StatusVulnerability regist_bunsin;
static PoseTableEntry pose_table[17];
static PoseTableEntry pose_table_sky[17];
static BattleDataEntry data_table[5];
static BattleDataEntry data_table_sky[6];
static BattleDataEntry data_table_bunsin[3];
static BattleDataEntry data_table_sky_bunsin[4];
static BattleUnitKindPart parts[2];
static BattleWeapon weapon_kamec_attack_magic;
static BattleWeapon weapon_kamec_dekadeka_magic;
static BattleWeapon weapon_kamec_biribiri_magic;
static BattleWeapon weapon_kamec_trans_magic;
static BattleWeapon weapon_kamec_katikati_magic;
static BattleWeapon weapon_kamec_recover_magic;
static BattleWeapon weapon_kamec_all_recover_magic;
static BattleUnitSetup entry_kamec;
static EvtWord init_event[22];
static EvtWord init_event2[87];
static EvtWord alpha_down_event[58];
static EvtWord alpha_up_event[58];
static EvtWord damage_event[66];
static EvtWord kamec_subset_bunsin_all_kill[68];
static EvtWord kamec_subset_dead_event[203];
static EvtWord kamec_subset_blow_dead_event[78];
static EvtWord avoid_miss_event[16];
static EvtWord bunsin_kill_event[37];
static EvtWord phase_event[2];
static EvtWord dmg_fall_event[159];
static EvtWord dmg_first_attack_pos[19];
static EvtWord attack_event[296];
static EvtWord ground_magic_common_event1[305];
static EvtWord ground_magic_common_event2[61];
static EvtWord sky_magic_common_event[258];
static EvtWord magic_common_event1[27];
static EvtWord magic_common_event2[16];
static EvtWord start_avoid[26];
static EvtWord attack_magic_event[325];
static EvtWord attack_magic_event_sub_no_hit[45];
static EvtWord attack_magic_event_sub_hit[30];
static EvtWord support_magic_event[140];
static EvtWord dekadeka_magic_event[7];
static EvtWord katikati_magic_event[7];
static EvtWord biribiri_magic_event[7];
static EvtWord trans_magic_event[7];
static EvtWord recover_magic_event[142];
static EvtWord all_recover_magic_event[180];
static EvtWord bunsin_magic_event[160];
static EvtWord spawn_event[66];
static EvtWord wait_event[55];

static USER_FUNC(eff_magic_get_posX) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    evtSetFloat(event, args[1], ((EffMagic1N64Work*)((EffectEntry*)unit->work[5])->userdata)->offset.x);
    return EVT_RETURN_DONE;
}

BattleUnitKind unit_kamec_green = {
    UNIT_GREEN_MAGIKOOPA, // mUnitType
    "btl_un_kamec_green", // name
    7, // maxHp
    0, // maxFp
    1, // mDangerHP
    1, // mPerilHP
    17, // mLevel
    0, // mBonusExp
    2, // bonusCoin
    70, // bonusCoinRate
    2, // baseCoin
    50, // runRate
    9999, // pbSoftCap
    42, // width
    42, // height
    {0, 42}, // hitOffset
    {0.0f, 21.0f, 0.0f}, // centerOffset
    {0, 0}, // hpGaugeOffset
    {0.0f, 0.0f, 0.0f}, // talkTogeBaseOffset
    {21.0f, 0.0f, -10.0f}, // heldItemBaseOffset
    {0.0f, 0.0f, 0.0f}, // burnFlameOffset
    0.0f, // vivianOffsetX
    0.0f, // vivianOffsetZ
    {21.0f, 0.0f, 0.0f}, // bintaHitOffset
    {21.0f, 27.3f, 0.0f}, // kissHitOffset
    {0.0f, 21.0f, 0.0f}, // cutBaseOffset
    42.0f, // cutWidth
    42.0f, // cutHeight
    0, // turnOrder
    0, // turnOrderVariance
    0, // swallowChance
    2, // swallowAttributes
    100, // hammerKnockbackChance
    20, // itemStealParam
    {0.0f, 0.0f, 0.0f}, // starPointDispOffset
    "SFX_ENM_KAMEKU_DAMAGED1", // damageSfxName
    "SFX_BTL_DAMAGE_FIRE1", // fireDamageSfxName
    "SFX_BTL_DAMAGE_ICE1", // iceDamageSfxName
    "SFX_BTL_DAMAGE_BIRIBIRI1", // electricDamageSfxName
    0, // attributeFlags
    &regist, // statusVulnerability
    2, // numParts
    parts, // mParts
    init_event, // initEventCode
    data_table, // dataTable
};

BattleUnitKind unit_kamec_green_bunsin = {
    UNIT_GREEN_MAGIKOOPA_CLONE,
    "btl_un_kamec_green_bunsin",
    7,
    0,
    1,
    1,
    0,
    0,
    0,
    70,
    0,
    50,
    9999,
    42,
    42,
    0,
    42,
    0.0f,
    21.0f,
    0.0f,
    0,
    0,
    0.0f,
    0.0f,
    0.0f,
    21.0f,
    0.0f,
    -10.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    21.0f,
    0.0f,
    0.0f,
    21.0f,
    27.3f,
    0.0f,
    0.0f,
    21.0f,
    0.0f,
    42.0f,
    42.0f,
    0,
    0,
    0,
    2,
    0,
    20,
    0.0f,
    0.0f,
    0.0f,
    "SFX_ENM_KAMEKU_DAMAGED1",
    "SFX_BTL_DAMAGE_FIRE1",
    "SFX_BTL_DAMAGE_ICE1",
    "SFX_BTL_DAMAGE_BIRIBIRI1",
    kNoDamageNumber,
    &regist,
    2,
    parts,
    init_event,
    data_table_bunsin,
};

static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

static StatusVulnerability regist = {
    70, // sleep
    75, // stop
    80, // dizzy
    100, // poison
    70, // confuse
    100, // electric
    100, // burn
    70, // freeze
    100, // huge
    90, // tiny
    100, // attackUp
    90, // attackDown
    100, // defenseUp
    95, // defenseDown
    80, // allergic
    70, // fright
    80, // galeForce
    100, // fast
    80, // slow
    100, // dodgy
    100, // invisible
    95, // ohko
};

static StatusVulnerability regist_bunsin = {0};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "KMG_N_1"},
    {POSE_TYPE_STOP, "KMG_Y_1"},
    {POSE_TYPE_FREEZE, "KMG_Y_1"},
    {POSE_TYPE_CONFUSE, "KMG_K_1"},
    {POSE_TYPE_POISON, "KMG_X_1"},
    {POSE_TYPE_DIZZY, "KMG_X_1"},
    {POSE_TYPE_NORMAL, "KMG_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KMG_Q_1"},
    {30, "KMG_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KMG_S_1"},
    {POSE_TYPE_DAMAGE, "KMG_D_1"},
    {POSE_TYPE_RUN, "KMG_R_1"},
    {POSE_TYPE_WALK, "KMG_W_1"},
    {POSE_TYPE_DANGER, "KMG_X_1"},
    {POSE_TYPE_PERIL, "KMG_X_1"},
    {POSE_TYPE_TALK, "KMG_T_1"},
    {POSE_TYPE_ACTION, "KMG_S_1"},
};

static PoseTableEntry pose_table_sky[] = {
    {POSE_TYPE_SLEEP, "KMG_N_2"},
    {POSE_TYPE_STOP, "KMG_Y_2"},
    {POSE_TYPE_FREEZE, "KMG_Y_2"},
    {POSE_TYPE_CONFUSE, "KMG_K_2"},
    {POSE_TYPE_POISON, "KMG_X_2"},
    {POSE_TYPE_DIZZY, "KMG_X_2"},
    {POSE_TYPE_NORMAL, "KMG_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "KMG_Q_2"},
    {30, "KMG_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KMG_S_2"},
    {POSE_TYPE_DAMAGE, "KMG_D_2"},
    {POSE_TYPE_RUN, "KMG_R_2"},
    {POSE_TYPE_WALK, "KMG_W_2"},
    {POSE_TYPE_DANGER, "KMG_X_2"},
    {POSE_TYPE_PERIL, "KMG_X_2"},
    {POSE_TYPE_TALK, "KMG_T_2"},
    {POSE_TYPE_ACTION, "KMG_S_2"},
};

static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, kamec_subset_dead_event},
    {UNIT_DATA_BLOW_DEAD_EVENT, kamec_subset_blow_dead_event},
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleDataEntry data_table_sky[] = {
    {UNIT_DATA_DEAD_EVENT, kamec_subset_dead_event},
    {UNIT_DATA_BLOW_DEAD_EVENT, kamec_subset_blow_dead_event},
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_DAMAGE_FALL_EVENT, dmg_fall_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0},
};

static BattleDataEntry data_table_bunsin[] = {
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

static BattleDataEntry data_table_sky_bunsin[] = {
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_DAMAGE_FALL_EVENT, dmg_fall_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0},
};

static BattleUnitKindPart parts[] = {
    {1, "btl_un_kamec_green", "c_kamek_g", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_kamec_green", "c_kamek_g", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kPartUntargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

static BattleWeapon weapon_kamec_attack_magic = {
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
    {4, 0, 0, 0, 0, 0, 0, 0}, // hpDamageWork
    NULL, // fpDamageFn
    {0}, // fpDamageWork
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_SELF | TARGET_CLASS_NO_OWN_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH, // special
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

static BattleWeapon weapon_kamec_dekadeka_magic = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
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
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_kamec_biribiri_magic = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, // poisonStrength..confuseTime
    100, // electricChance
    3, // electricTime
    0, 0, 0, 0, 0, 0, 0, 0, // dodgyChance..sizeChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // sizeChangeStrength..allergicChance
    0, 0, 0, 0, 0, 0, 0, 0, // allergicTime..frightChance
    0, 0, 0, 0, 0, 0, 0, 0, // galeForceChance..fpRegenTime
    0, // fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_kamec_trans_magic = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, 0, 0, 0, 0, 0, 0, 0, // atkChangeStrength..chargeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // fastChance..holdFastTime
    100, // invisibleChance
    3, // invisibleTime
    0, 0, 0, 0, // hpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_kamec_katikati_magic = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, // acHelpMsg
    SPECIAL_CANNOT_MISS | SPECIAL_IGNORE_VULNERABILITY, // special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_RANDOM, // targetWeightFlags
    0, 0, 0, 0, 0, 0, 0, 0, // sleepChance..poisonTime
    0, 0, 0, 0, 0, 0, 0, 0, // poisonStrength..burnChance
    0, 0, 0, 0, 0, 0, 0, 0, // burnTime..atkChangeTime
    0, // atkChangeStrength
    100, // defChangeChance
    3, // defChangeTime
    3, // defChangeStrength
    0, 0, 0, 0, 0, 0, 0, 0, // allergicChance..slowTime
    0, 0, 0, 0, 0, 0, 0, 0, // frightChance..hpRegenStrength
    0, 0, // fpRegenTime..fpRegenStrength
    {0}, // pad_AE
    NULL, // mAttackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

static BattleWeapon weapon_kamec_recover_magic = {
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
    TARGET_CLASS_SINGLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, 0, // acHelpMsg..special
    COUNTER_RESIST_ALL, // counterResistFlags
    TARGET_WEIGHT_IN_PERIL | TARGET_WEIGHT_LOW_HP | TARGET_WEIGHT_LOW_HP_RATIO, // targetWeightFlags
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

static BattleWeapon weapon_kamec_all_recover_magic = {
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
    TARGET_CLASS_MULTIPLE | TARGET_CLASS_ONLY_MAIN_PARTS | TARGET_CLASS_NO_OPPOSITE_SIDE | TARGET_CLASS_NO_SYSTEM_UNIT | TARGET_CLASS_NO_TREE_OR_SWITCH, // targetClassFlags
    TARGET_PROP_OPPOSITE_SIDE_FIRST, // targetPropertyFlags
    ELEMENT_NORMAL, ATTACKPATTERN_NULL, // element..damagePattern
    3, // acLevel
    2, // reserved_6F
    NULL, 0, // acHelpMsg..special
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

static BattleUnitSetup entry_kamec = {
    &unit_kamec_green_bunsin, // mUnitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    0x4000004, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 1, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 3, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 4, 0)
    CALL_CHILD(PTR(init_event2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(init_event2)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_sky))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_sky))
            ELSE
            CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_sky_bunsin))
        END_IF
        CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kUnitAirborne)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kPartNotHammerTarget)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kPartWinged)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kPartNotShellTossTarget)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 40, LW(2))
        CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 40, LW(2))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(alpha_down_event)
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 254)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, 1, 1)
    WAIT_FRAMES(1)
    CALL(evt_sub_intpl_init, EASING_LINEAR, 254, 0, 30)
    LOOP(30)
        CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
        CALL(btlevtcmd_SetAlpha, LW(0), 1, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_OnPartsAttribute, LW(0), 1, kNoShadow)
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 0)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, 1, 1)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

static EVT_BEGIN(alpha_up_event)
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_OffPartsAttribute, LW(0), 1, kNoShadow)
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 0)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, 1, 1)
    WAIT_FRAMES(1)
    CALL(evt_sub_intpl_init, EASING_LINEAR, 0, 254, 30)
    LOOP(30)
        CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
        CALL(btlevtcmd_SetAlpha, LW(0), 1, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 255)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, 1, 1)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(kamec_get_bunsin, LW(1), LW(2), LW(3), LW(4), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 2, 1)
        END_IF
        ELSE
        SET_VALUE(LW(0), -2)
        CALL_CHILD(PTR(bunsin_kill_event))
        RETURN
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 2, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL_CHILD(PTR(kamec_subset_bunsin_all_kill))
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(kamec_subset_bunsin_all_kill)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(kamec_get_bunsin, LW(1), LW(2), LW(3), LW(4), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_WaitAttackEnd)
            CALL(kamec_get_bunsin, LW(1), LW(2), LW(3), LW(4), LW(0))
            SET_VALUE(LW(0), LW(1))
            SPAWN_EVENT(PTR(bunsin_kill_event))
            SET_VALUE(LW(0), LW(2))
            SPAWN_EVENT(PTR(bunsin_kill_event))
            SET_VALUE(LW(0), LW(3))
            SPAWN_EVENT(PTR(bunsin_kill_event))
            SET_VALUE(LW(0), LW(4))
            SPAWN_EVENT(PTR(bunsin_kill_event))
            LOOP(0)
                CALL(kamec_get_bunsin, LW(1), LW(2), LW(3), LW(4), LW(0))
                IF_INT_EQUAL(LW(0), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_IF
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(kamec_subset_dead_event)
    CALL(btlevtcmd_CheckDataOfDataTable, LW(10), LW(0), UNIT_DATA_PRE_DEAD_EVENT)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_RunDataEventChild, LW(10), UNIT_DATA_PRE_DEAD_EVENT)
    END_IF
    SET_VALUE(LW(0), 48)
    CALL(btlevtcmd_OnUnitFlag, LW(10), BATTLE_UNIT_FLAG_HIDE_HELD_ITEM)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    LABEL(10)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 0, -15, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_SetRotate, LW(10), 0, 0, 0)
    CALL(btlevtcmd_snd_se, LW(10), STRING("SFX_BTL_ENEMY_DIE1_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, LW(10), LW(0), LW(1), LW(2))
    ADD(LW(2), 10)
    CALL(evt_eff, 0, STRING("kemuri_test"), 4, LW(0), LW(1), LW(2), FLOAT(0.7998047f), 0, 0, 0, 0, 0, 0, 0)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL(btlevtcmd_GetPos, LW(10), LW(3), LW(1), LW(2))
    ADD(LW(2), 10)
    ADD(LW(1), 30)
    CALL(_add_star_point_disp_offset, LW(10), LW(3), LW(1), LW(2))
    CALL(evt_eff, STRING(""), STRING("star_point"), 0, LW(3), LW(1), LW(2), LW(0), 0, 0, 0, 0, 0, 0, 0)
    IF_INT_GREATER_OR_EQUAL(LW(0), 1)
        LOOP(LW(0))
        END_LOOP
    END_IF
    CALL(btlevtcmd_SetRotateOffset, LW(10), 0, 0, 0)
    SET_VALUE(LW(0), 30)
    LABEL(20)
    WAIT_FRAMES(1)
    CALL(btlevtcmd_AddRotate, LW(10), 3, 0, 0)
    SUBTRACT(LW(0), 1)
    IF_INT_GREATER(LW(0), 0)
        GOTO(20)
    END_IF
    CALL(btlevtcmd_GetBackItem, LW(10))
    CALL(btlevtcmd_ClearAllStatus, LW(10))
    CALL(btlevtcmd_OnAttribute, LW(10), kUnitHidden)
    CALL(btlevtcmd_OnAttribute, LW(10), kUnitNoShadow)
    CALL_CHILD(kamec_subset_bunsin_all_kill)
    CALL(btlevtcmd_CheckDamageCode, LW(10), DAMAGE_RESULT_DEATH, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_KillUnit, LW(10), 0)
    ELSE
        CALL(btlevtcmd_KillUnit, LW(10), 2)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(kamec_subset_blow_dead_event)
    CALL(evt_snd_sfxon, STRING("SFX_BTL_CLAUD_BREATH3"), 0)
    CALL(btlevtcmd_GetBodyId, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, LW(0), POSE_TYPE_DAMAGE)
    BEGIN_SIBLING_EVENT
        LABEL(0)
        CALL(btlevtcmd_AddRotate, UNIT_ID_SELF, 0, 20, 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_SetMoveSpeed, UNIT_ID_SELF, FLOAT(2.0f))
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetStageSize, LW(0), EVT_NULLPTR, EVT_NULLPTR)
    ADD(LW(1), 40)
    CALL(btlevtcmd_DivePosition, UNIT_ID_SELF, LW(0), LW(1), LW(2), 0, -10, EASING_CUBIC_IN, 0, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_GetCoin, LW(10), LW(0))
    CALL(btlevtcmd_StoreCoin, LW(0))
    CALL(btlevtcmd_GetExp, LW(10), LW(0))
    CALL(btlevtcmd_StoreExp, LW(0))
    CALL_CHILD(kamec_subset_bunsin_all_kill)
    CALL(btlevtcmd_KillUnit, UNIT_ID_SELF, 0)
    RETURN
EVT_END()

static EVT_BEGIN(avoid_miss_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), -2)
        CALL_CHILD(PTR(bunsin_kill_event))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(bunsin_kill_event)
    CALL(btlevtcmd_GetUnitId, LW(0), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(0), 3, LW(1))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_SetUnitWork, LW(0), 3, 1)
    CALL_CHILD(PTR(alpha_down_event))
    LABEL(10)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_KillUnit, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_fall_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kUnitAirborne)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kPartWinged)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kPartNotHammerTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kPartNotShellTossTarget)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    ELSE
        CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_bunsin))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KMG_H_1"))
        CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 2, kHidden)
        WAIT_FRAMES(20)
        CALL(evt_sub_intpl_init, EASING_LINEAR, 255, 0, 30)
        LOOP(30)
            CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
            CALL(btlevtcmd_SetAlpha, UNIT_ID_SELF, 2, LW(1))
            WAIT_FRAMES(1)
        END_LOOP
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 2, kHidden)
    END_INLINE_EVENT
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_FALL1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetFallAccel, UNIT_ID_SELF, FLOAT(1.0f))
    CALL(btlevtcmd_JumpPosition, UNIT_ID_SELF, LW(0), 0, LW(2), 10, MOVE_SOUND_FROM_TABLE)
    CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 0, LW(2))
    WAIT_MS(500)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, 0, 0)
    CALL_CHILD(init_event2)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    RETURN
EVT_END()

static EVT_BEGIN(dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

static EVT_BEGIN(attack_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(9), PTR(&weapon_kamec_attack_magic))
        CALL_CHILD(PTR(attack_magic_event))
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        GOTO(99)
    END_IF
    CALL(kamec_get_live_num, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        LABEL(10)
        SET_VALUE(LW(1), 40)
        ADD(LW(1), 0)
        ADD(LW(1), 40)
        SUBTRACT(LW(1), 1)
        CALL(evt_sub_random, LW(1), LW(0))
        SET_VALUE(LW(1), 40)
        IF_INT_LESS(LW(0), LW(1))
            SET_VALUE(LW(9), PTR(&weapon_kamec_attack_magic))
            CALL_CHILD(PTR(attack_magic_event))
            GOTO(99)
        END_IF
        ADD(LW(1), 0)
        IF_INT_LESS(LW(0), LW(1))
            SET_VALUE(LW(9), PTR(&weapon_kamec_recover_magic))
            CALL_CHILD(PTR(recover_magic_event))
            IF_INT_EQUAL(LW(0), 0)
                GOTO(10)
            END_IF
            GOTO(99)
            ELSE
            CALL_CHILD(PTR(bunsin_magic_event))
            GOTO(99)
        END_IF
        ELSE
        CALL(kamec_get_bunsin, LW(1), LW(2), LW(3), LW(4), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
            IF_INT_NOT_EQUAL(LW(0), 0)
                CALL_CHILD(PTR(subsetevt_confuse_flustered))
                GOTO(99)
            END_IF
            SET_VALUE(LW(9), PTR(&weapon_kamec_attack_magic))
            CALL_CHILD(PTR(attack_magic_event))
            GOTO(99)
            ELSE
            LABEL(20)
            SET_VALUE(LW(1), 50)
            ADD(LW(1), 0)
            ADD(LW(1), 0)
            ADD(LW(1), 20)
            ADD(LW(1), 20)
            ADD(LW(1), 0)
            ADD(LW(1), 0)
            CALL(evt_sub_random, LW(1), LW(0))
            SET_VALUE(LW(1), 50)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_attack_magic))
                CALL_CHILD(PTR(attack_magic_event))
                GOTO(99)
            END_IF
            ADD(LW(1), 0)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_dekadeka_magic))
                CALL_CHILD(PTR(dekadeka_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
            END_IF
            ADD(LW(1), 0)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_katikati_magic))
                CALL_CHILD(PTR(katikati_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
            END_IF
            ADD(LW(1), 20)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_biribiri_magic))
                CALL_CHILD(PTR(biribiri_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
            END_IF
            ADD(LW(1), 20)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_trans_magic))
                CALL_CHILD(PTR(trans_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
            END_IF
            ADD(LW(1), 0)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_recover_magic))
                CALL_CHILD(PTR(recover_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
                ELSE
                SET_VALUE(LW(9), PTR(&weapon_kamec_all_recover_magic))
                CALL_CHILD(PTR(all_recover_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
            END_IF
        END_IF
    END_IF
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

static EVT_BEGIN(ground_magic_common_event1)
    BEGIN_SIBLING_EVENT
        LOOP(2)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(40)
        END_LOOP
    END_SIBLING_EVENT
    CALL(kamec_get_bunsin, LW(11), LW(12), LW(13), LW(14), LW(0))
    IF_INT_EQUAL(LW(15), 0)
        BEGIN_SIBLING_EVENT
            CALL(eff_staff1, UNIT_ID_SELF)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            IF_INT_NOT_EQUAL(LW(11), -1)
                CALL(eff_staff1, LW(11))
            END_IF
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            IF_INT_NOT_EQUAL(LW(12), -1)
                CALL(eff_staff1, LW(12))
            END_IF
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            IF_INT_NOT_EQUAL(LW(13), -1)
                CALL(eff_staff1, LW(13))
            END_IF
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            IF_INT_NOT_EQUAL(LW(14), -1)
                CALL(eff_staff1, LW(14))
            END_IF
        END_SIBLING_EVENT
    ELSE
        BEGIN_SIBLING_EVENT
            CALL(eff_staff2, UNIT_ID_SELF)
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_1A"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMG_A_1A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMG_A_1A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMG_A_1A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMG_A_1A"))
    END_IF
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_1B"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMG_A_1B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMG_A_1B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMG_A_1B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMG_A_1B"))
    END_IF
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_1C"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMG_A_1C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMG_A_1C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMG_A_1C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMG_A_1C"))
    END_IF
    WAIT_FRAMES(30)
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC8"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_1D"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMG_A_1D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMG_A_1D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMG_A_1D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMG_A_1D"))
    END_IF
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_1E"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMG_A_1E"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMG_A_1E"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMG_A_1E"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMG_A_1E"))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(ground_magic_common_event2)
    BEGIN_SIBLING_EVENT
        LOOP(2)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(40)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        CALL(eff_staff1, UNIT_ID_SELF)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_1A"))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_2A"))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC7"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_2B"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_2C"))
    RETURN
EVT_END()

static EVT_BEGIN(sky_magic_common_event)
    BEGIN_SIBLING_EVENT
        LOOP(2)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(40)
        END_LOOP
    END_SIBLING_EVENT
    CALL(kamec_get_bunsin, LW(11), LW(12), LW(13), LW(14), LW(0))
    IF_INT_EQUAL(LW(15), 0)
        BEGIN_SIBLING_EVENT
            CALL(eff_staff1, UNIT_ID_SELF)
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            IF_INT_NOT_EQUAL(LW(11), -1)
                CALL(eff_staff1, LW(11))
            END_IF
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            IF_INT_NOT_EQUAL(LW(12), -1)
                CALL(eff_staff1, LW(12))
            END_IF
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            IF_INT_NOT_EQUAL(LW(13), -1)
                CALL(eff_staff1, LW(13))
            END_IF
        END_SIBLING_EVENT
        BEGIN_SIBLING_EVENT
            IF_INT_NOT_EQUAL(LW(14), -1)
                CALL(eff_staff1, LW(14))
            END_IF
        END_SIBLING_EVENT
    ELSE
        BEGIN_SIBLING_EVENT
            CALL(eff_staff2, UNIT_ID_SELF)
        END_SIBLING_EVENT
    END_IF
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_3A"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMG_A_3A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMG_A_3A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMG_A_3A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMG_A_3A"))
    END_IF
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC8"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_3B"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMG_A_3B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMG_A_3B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMG_A_3B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMG_A_3B"))
    END_IF
    WAIT_FRAMES(35)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC7"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_3C"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMG_A_3C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMG_A_3C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMG_A_3C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMG_A_3C"))
    END_IF
    WAIT_FRAMES(15)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMG_A_3D"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMG_A_3D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMG_A_3D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMG_A_3D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMG_A_3D"))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(magic_common_event1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(13))
        IF_INT_NOT_EQUAL(LW(3), LW(13))
            CALL_CHILD(PTR(ground_magic_common_event1))
            ELSE
            CALL_CHILD(PTR(ground_magic_common_event2))
        END_IF
        ELSE
        CALL_CHILD(PTR(sky_magic_common_event))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(magic_common_event2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(ground_magic_common_event2))
        ELSE
        CALL_CHILD(PTR(sky_magic_common_event))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(start_avoid)
    IF_INT_EQUAL(LW(5), 3)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_LUCKY)
    END_IF
    IF_INT_EQUAL(LW(5), 6)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_DODGY)
    END_IF
    IF_INT_EQUAL(LW(5), 2)
        CALL(btlevtcmd_StartAvoid, LW(3), DAMAGE_CODE_MISS)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(attack_magic_event)
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
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
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(15), 0)
    CALL_CHILD(magic_common_event1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC3_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    CALL(btlevtcmd_PreCheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), 0, LW(5))
    SWITCH(LW(5))
        CASE_OR_EQUAL(4)
        GOTO(90)
        CASE_END
        CASE_EQUAL(3)
        GOTO(90)
        CASE_END
        CASE_EQUAL(6)
        GOTO(90)
        CASE_EQUAL(2)
        GOTO(90)
        CASE_EQUAL(1)
        GOTO(91)
        CASE_END
        CASE_DEFAULT
        GOTO(98)
        CASE_END
        LABEL(90)
        BEGIN_SIBLING_EVENT
            WAIT_FRAMES(1)
            LOOP(0)
                CALL(btlevtcmd_GetFaceDirection, UNIT_ID_SELF, LW(0))
                IF_INT_EQUAL(LW(0), -1)
                    CALL(eff_magic_get_posX, UNIT_ID_SELF, LW(0))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                    IF_INT_LESS_OR_EQUAL(LW(0), LW(6))
                        CALL_CHILD(start_avoid)
                        BREAK_LOOP
                    END_IF
                ELSE
                    CALL(eff_magic_get_posX, UNIT_ID_SELF, LW(0))
                    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(6), LW(7), LW(8))
                    IF_INT_GREATER_OR_EQUAL(LW(0), LW(6))
                        CALL_CHILD(start_avoid)
                        BREAK_LOOP
                    END_IF
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT
        CALL(kamec_get_bunsin, LW(6), LW(7), LW(8), LW(13), LW(0))
        SET_VALUE(LW(14), -2)
        CALL_CHILD(attack_magic_event_sub_no_hit)
        SET_VALUE(LW(14), LW(6))
        CALL_CHILD(attack_magic_event_sub_no_hit)
        SET_VALUE(LW(14), LW(7))
        CALL_CHILD(attack_magic_event_sub_no_hit)
        SET_VALUE(LW(14), LW(8))
        CALL_CHILD(attack_magic_event_sub_no_hit)
        SET_VALUE(LW(14), LW(13))
        CALL_CHILD(attack_magic_event_sub_no_hit)
        WAIT_FRAMES(60)
        GOTO(98)
        LABEL(91)
        CALL(kamec_get_bunsin, LW(6), LW(7), LW(8), LW(13), LW(0))
        SET_VALUE(LW(14), -2)
        CALL_CHILD(attack_magic_event_sub_hit)
        SET_VALUE(LW(14), LW(6))
        CALL_CHILD(attack_magic_event_sub_hit)
        SET_VALUE(LW(14), LW(7))
        CALL_CHILD(attack_magic_event_sub_hit)
        SET_VALUE(LW(14), LW(8))
        CALL_CHILD(attack_magic_event_sub_hit)
        SET_VALUE(LW(14), LW(13))
        CALL_CHILD(attack_magic_event_sub_hit)
        WAIT_FRAMES(45)
        CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
        CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
        GOTO(98)
        LABEL(98)
        WAIT_FRAMES(30)
        CALL(kamec_get_bunsin, LW(6), LW(7), LW(8), LW(9), LW(0))
        CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_NEUTRAL)
        IF_INT_NOT_EQUAL(LW(6), -1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(6), 1, POSE_TYPE_NEUTRAL)
        END_IF
        IF_INT_NOT_EQUAL(LW(7), -1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(7), 1, POSE_TYPE_NEUTRAL)
        END_IF
        IF_INT_NOT_EQUAL(LW(8), -1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(8), 1, POSE_TYPE_NEUTRAL)
        END_IF
        IF_INT_NOT_EQUAL(LW(9), -1)
            CALL(btlevtcmd_AnimeChangePoseType, LW(9), 1, POSE_TYPE_NEUTRAL)
        END_IF
        LABEL(99)
        CALL(btlevtcmd_ResetFaceDirection, -2)
        RETURN
EVT_END()

static EVT_BEGIN(attack_magic_event_sub_no_hit)
    IF_INT_EQUAL(LW(14), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetPos, LW(14), LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(btlevtcmd_GetStageSize, LW(10), EVT_NULLPTR, EVT_NULLPTR)
    CALL(btlevtcmd_GetFaceDirection, LW(14), LW(0))
    MULTIPLY(LW(10), LW(0))
    SUBTRACT(LW(11), 5)
    CALL(eff_magic, LW(14), LW(10), LW(11), LW(12), 60)
    RETURN
EVT_END()

static EVT_BEGIN(attack_magic_event_sub_hit)
    IF_INT_EQUAL(LW(14), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetPos, LW(14), LW(0), LW(1), LW(2))
    ADD(LW(1), 10)
    CALL(btlevtcmd_GetHitPos, LW(3), LW(4), LW(10), LW(11), LW(12))
    CALL(eff_magic, LW(14), LW(10), LW(11), LW(12), 45)
    RETURN
EVT_END()

static EVT_BEGIN(support_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            SET_VALUE(LW(0), 1)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    IF_INT_EQUAL(LW(15), 10)
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_HUGE, LW(0))
    END_IF
    IF_INT_EQUAL(LW(15), 14)
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_DEFENSE_UP, LW(0))
    END_IF
    IF_INT_EQUAL(LW(15), 6)
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_ELECTRIC, LW(0))
    END_IF
    IF_INT_EQUAL(LW(15), 18)
        CALL(btlevtcmd_CheckStatus, LW(3), STATUS_INVISIBLE, LW(0))
    END_IF
    IF_INT_EQUAL(LW(0), 1)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(15), 0)
    CALL_CHILD(PTR(magic_common_event1))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_ResultACDefence, LW(3), LW(9))
    CALL(btlevtcmd_CheckDamage, UNIT_ID_SELF, LW(3), LW(4), LW(9), DAMAGE_RESULT_FINAL_HIT, LW(5))
    WAIT_FRAMES(30)
    LABEL(99)
    SET_VALUE(LW(0), 1)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(dekadeka_magic_event)
    SET_VALUE(LW(15), 10)
    CALL_CHILD(PTR(support_magic_event))
    RETURN
EVT_END()

static EVT_BEGIN(katikati_magic_event)
    SET_VALUE(LW(15), 14)
    CALL_CHILD(PTR(support_magic_event))
    RETURN
EVT_END()

static EVT_BEGIN(biribiri_magic_event)
    SET_VALUE(LW(15), 6)
    CALL_CHILD(PTR(support_magic_event))
    RETURN
EVT_END()

static EVT_BEGIN(trans_magic_event)
    SET_VALUE(LW(15), 18)
    CALL_CHILD(PTR(support_magic_event))
    RETURN
EVT_END()

static EVT_BEGIN(recover_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            SET_VALUE(LW(0), 1)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetHp, LW(3), LW(0))
    CALL(btlevtcmd_GetMaxHp, LW(3), LW(1))
    IF_INT_EQUAL(LW(0), LW(1))
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(15), 0)
    CALL_CHILD(PTR(magic_common_event1))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_RecoverHp, LW(3), LW(4), 8)
    CALL(btlevtcmd_GetPartsPos, LW(3), LW(4), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetHeight, LW(3), LW(5))
    CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
    MULTIPLY_FLOAT(LW(5), LW(6))
    ADD(LW(1), LW(5))
    CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(0), LW(1), LW(2), 8, 0, 0, 0, 0, 0, 0, 0)
    WAIT_FRAMES(40)
    LABEL(99)
    SET_VALUE(LW(0), 1)
    CALL(btlevtcmd_ResetFaceDirection, -2)
    RETURN
EVT_END()

static EVT_BEGIN(all_recover_magic_event)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(PTR(subsetevt_confuse_flustered))
            SET_VALUE(LW(0), 1)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(5)
    CALL(btlevtcmd_GetHp, LW(3), LW(0))
    CALL(btlevtcmd_GetMaxHp, LW(3), LW(1))
    IF_INT_LESS(LW(0), LW(1))
        GOTO(8)
    END_IF
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        SET_VALUE(LW(0), 0)
        RETURN
    END_IF
    GOTO(5)
    LABEL(8)
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL_CHILD(PTR(magic_common_event2))
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    WAIT_FRAMES(30)
    CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
    CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
    CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    CALL(btlevtcmd_GetSelectEnemy, LW(3), LW(4))
    LABEL(10)
    BEGIN_SIBLING_EVENT
        CALL(evt_sub_random, 10, LW(0))
        WAIT_FRAMES(LW(0))
        CALL(btlevtcmd_RecoverHp, LW(3), LW(4), 4)
        CALL(btlevtcmd_GetPartsPos, LW(3), LW(4), LW(0), LW(1), LW(2))
        CALL(btlevtcmd_GetHeight, LW(3), LW(5))
        CALL(btlevtcmd_GetStatusMg, LW(3), LW(6))
        MULTIPLY_FLOAT(LW(5), LW(6))
        ADD(LW(1), LW(5))
        CALL(evt_eff, STRING(""), STRING("recovery"), 0, LW(0), LW(1), LW(2), 4, 0, 0, 0, 0, 0, 0, 0)
    END_SIBLING_EVENT
    CALL(btlevtcmd_GetSelectNextEnemy, LW(3), LW(4))
    IF_INT_NOT_EQUAL(LW(3), -1)
        GOTO(10)
    END_IF
    SET_VALUE(LW(0), 1)
    WAIT_FRAMES(120)
    RETURN
EVT_END()

static EVT_BEGIN(bunsin_magic_event)
    SET_VALUE(LW(3), -2)
    SET_VALUE(LW(4), 1)
    SET_VALUE(LW(15), 1)
    CALL_CHILD(magic_common_event1)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_FREE)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_OFFSHOOT1"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(0), -2)
    CALL_CHILD(alpha_down_event)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(btlevtcmd_FaceDirectionAdd, UNIT_ID_SELF, LW(0), 40)
        SET_VALUE(LW(4), LW(0))
        IF_INT_GREATER(LW(4), 0)
            SUBTRACT(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(spawn_event)
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    LOOP(0)
        CALL(btlevtcmd_FaceDirectionSub, UNIT_ID_SELF, LW(0), 40)
        IF_INT_LESS_OR_EQUAL(LW(0), 170)
            ADD(LW(2), 10)
            CALL(btlevtcmd_CheckSpace, LW(3), LW(0), LW(1), LW(2), 40, 0, 0, 0)
            IF_INT_EQUAL(LW(3), -1)
                CALL_CHILD(spawn_event)
            END_IF
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(kamec_shuffle)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_OFFSHOOT2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    SET_VALUE(LW(0), -2)
    SPAWN_EVENT(alpha_up_event)
    CALL(kamec_get_bunsin, LW(1), LW(2), LW(3), LW(4), LW(0))
    SET_VALUE(LW(0), LW(1))
    SPAWN_EVENT(alpha_up_event)
    SET_VALUE(LW(0), LW(2))
    SPAWN_EVENT(alpha_up_event)
    SET_VALUE(LW(0), LW(3))
    SPAWN_EVENT(alpha_up_event)
    SET_VALUE(LW(0), LW(4))
    SPAWN_EVENT(alpha_up_event)
    WAIT_FRAMES(60)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    RETURN
EVT_END()

static USER_FUNC(eff_magic) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 tx = evtGetFloat(event, args[1]);
    f32 ty = evtGetFloat(event, args[2]);
    f32 tz = evtGetFloat(event, args[3]);
    s32 type = evtGetValue(event, args[4]);
    f32 x;
    f32 y;
    f32 z;
    EffectEntry* eff;

    BtlUnit_GetPos(unit, &x, &y, &z);
    // from the tip of the wand
    x += 25.0f * unit->mSizeMultiplier * unit->faceDirection;
    y += 20.0f * unit->mSizeMultiplier;
    z += 5.0f;
    eff = effMagic1N64Entry(0, type, x, y, z, tx, ty, tz);
    unit->work[5] = (EvtWord)eff;
    ((EffMagic1N64Work*)eff->userdata)->scale = unit->mSizeMultiplier;
    return EVT_RETURN_DONE;
}

static USER_FUNC(eff_staff2) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    f32* wk;

    if (isFirstCall) {
        unit->work[4] = (EvtWord)effPokopiPchargeN64Entry(0, 60, 0.0f, -1000.0f, 0.0f, 1.0f);
    }
    if (!(((EffectEntry*)unit->work[4])->flags & 1)) {
        return EVT_RETURN_DONE;
    }
    wk = ((EffectEntry*)unit->work[4])->userdata;
    wk[13] = unit->mSizeMultiplier;
    BtlUnit_GetPos(unit, &x, &y, &z);
    wk[1] = x + 22.5f * unit->mSizeMultiplier * unit->faceDirection;
    wk[2] = y + 40.0f * unit->mSizeMultiplier;
    wk[3] = 5.0f + z;
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(eff_staff1) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    f32* wk;

    if (isFirstCall) {
        unit->work[4] = (EvtWord)effThunderflareN64Entry(0, 0.0f, -1000.0f, 0.0f, 1.0f, 60);
    }
    if (!(((EffectEntry*)unit->work[4])->flags & 1)) {
        return EVT_RETURN_DONE;
    }
    wk = ((EffectEntry*)unit->work[4])->userdata;
    wk[10] = unit->mSizeMultiplier;
    BtlUnit_GetPos(unit, &x, &y, &z);
    wk[2] = x + 22.5f * unit->mSizeMultiplier * unit->faceDirection;
    wk[3] = y + 40.0f * unit->mSizeMultiplier;
    wk[4] = 5.0f + z;
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(kamec_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = evtGetValue(event, args[0]);
    s32 id2 = evtGetValue(event, args[1]);
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    // the copy shares the real magikoopa's hp and status
    unit2->currentHp = unit1->currentHp;
    memcpy(&unit2->mSleepTurns, &unit1->mSleepTurns, 0x1E);
    unit2->hpGauge.prevHp = unit2->currentHp;
    unit2->hpGauge.targetHp = unit2->currentHp;
    unit2->hpGauge.barRatio = unit2->hpGauge.trailRatio = (f32)unit2->currentHp / unit2->maxHp;
    return EVT_RETURN_DONE;
}

static USER_FUNC(kamec_shuffle) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* kamecs[6];
    s32 n = 0;
    s32 i;

    // the real magikoopa and its copies
    for (i = 0; i < 64; i++) {
        kamecs[n] = wp->units[i];
        if (kamecs[n] != NULL && (kamecs[n]->currentKind == UNIT_GREEN_MAGIKOOPA || kamecs[n]->currentKind == UNIT_GREEN_MAGIKOOPA_CLONE)) {
            n++;
        }
    }
    for (i = 0; i < 10; i++) {
        s32 a = irand(n);
        s32 b = irand(n);
        BattleUnit* ua = kamecs[a];
        BattleUnit* ub = kamecs[b];
        f32 tmp;

        tmp = ua->mPosition.x;
        ua->mPosition.x = ub->mPosition.x;
        ub->mPosition.x = tmp;
        tmp = ua->mHomePosition.x;
        ua->mHomePosition.x = ub->mHomePosition.x;
        ub->mHomePosition.x = tmp;
        tmp = ua->mPosition.z;
        ua->mPosition.z = ub->mPosition.z;
        ub->mPosition.z = tmp;
        tmp = ua->mHomePosition.z;
        ua->mHomePosition.z = ub->mHomePosition.z;
        ub->mHomePosition.z = tmp;
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(kamec_get_bunsin) {
    EvtWord* args = event->args;
    s32 copies[4];
    s32 n = 0;
    s32 i;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    // indices of the magikoopa's copies
    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && unit->currentKind == UNIT_GREEN_MAGIKOOPA_CLONE) {
            copies[n] = i;
            n++;
        }
    }
    for (i = 0; i < 4; i++) {
        if (i < n) {
            evtSetValue(event, *args++, copies[i]);
        } else {
            evtSetValue(event, *args++, -1);
        }
    }
    evtSetValue(event, *args, n);
    return EVT_RETURN_DONE;
}

static USER_FUNC(kamec_get_live_num) {
    s32 i;
    EvtWord* args = event->args;
    s32 num = 0;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && unit->mAlliance == ALLIANCE_ENEMY && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
            num++;
        }
    }
    evtSetValue(event, args[0], num);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(spawn_event)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_kamec), 0)
    CALL(btlevtcmd_SetAlpha, LW(3), 1, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 0, LW(10))
    CALL(btlevtcmd_SetUnitWork, LW(3), 0, LW(10))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(13))
    CALL(kamec_copy_status, LW(13), LW(3))
    CALL(btlevtcmd_OffAttribute, LW(3), kUnitNoHpGauge)
    CALL(btlevtcmd_SetUnitWork, LW(3), 1, 1)
    CALL(btlevtcmd_SetEventAttack, LW(3), PTR(attack_event))
    CALL(btlevtcmd_SetRegistStatus, LW(3), PTR(&regist_bunsin))
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, kClone)
    RETURN
EVT_END()

static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, 1, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(kamec_get_bunsin, LW(1), LW(2), LW(3), LW(4), LW(0))
    CALL(btlevtcmd_AnimeChangePoseFromTable, UNIT_ID_SELF, 1)
    IF_INT_NOT_EQUAL(LW(1), -1)
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(1), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(2), -1)
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(2), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(3), 1)
    END_IF
    IF_INT_NOT_EQUAL(LW(4), -1)
        CALL(btlevtcmd_AnimeChangePoseFromTable, LW(4), 1)
    END_IF
    RETURN
EVT_END()

