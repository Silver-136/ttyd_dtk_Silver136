// Rogueport Sewers (tik) battle unit: Magikoopa (kamec): its parts, defence/status tables, and attack events.

#include "version.h"
#include "eff/n64/eff_thunderflare_n64.h"
#include "battle/battle_unit.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cmd.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_camera.h"
#include "evt/evt_eff.h"
#include "evt/evt_sub.h"
#include "eff/n64/eff_pokopi_pcharge_n64.h"

#include "battle/battle.h"
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

// BattleUnit.work slots (btlevtcmd_Get/Set/AddUnitWork)
#define UNIT_WORK_SKY 0          // nonzero = sky form (pose_table_sky, data_table_sky*, airborne, height 40) from the battle setup; dmg_fall_event clears it; spawn_event copies it to clones; picks the sky or ground magic events
#define UNIT_WORK_BUNSIN 1       // 1 for a clone made by spawn_event; a clone dies on any damage or miss (bunsin_kill_event), skips the attack and the wait-pose refresh, and uses the *_bunsin data tables
#define UNIT_WORK_KILL_BUNSIN 2  // damage_event sets 1 when the original is hit while clones exist; after the damage it then removes all clones (kamec_subset_bunsin_all_kill)
#define UNIT_WORK_FADING 3       // 1 once bunsin_kill_event has started fading the clone out (a second call skips the fade)
#define UNIT_WORK_STAFF_EFFECT 4 // EffectEntry* of the staff effect (effThunderflareN64Entry in eff_staff1, effPokopiPchargeN64Entry in eff_staff2), kept at the staff tip
#define UNIT_WORK_MAGIC_EFFECT 5 // EffectEntry* of the magic shot (effMagic1N64Entry, eff_magic); eff_magic_get_posX reads its x offset


static USER_FUNC(eff_magic);
static USER_FUNC(kamec_copy_status);
static USER_FUNC(kamec_get_bunsin);
static USER_FUNC(kamec_get_live_num);
static USER_FUNC(eff_magic_get_posX);
static USER_FUNC(eff_staff2);
static USER_FUNC(eff_staff1);
static USER_FUNC(kamec_shuffle);

BattleUnitKind unit_kamec;
BattleUnitKind unit_kamec_bunsin;
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
#if VERSION <= VERSION_DEMO_US
static EvtWord spawn_event[62];
#else
static EvtWord spawn_event[66];
#endif
static EvtWord wait_event[55];

/**
 * Script helper.
 * Reads the X offset of the unit's current magic effect (the one stored in its work slot 5) into args[1];
 * args[0] is the unit id.
 */
static USER_FUNC(eff_magic_get_posX) {
    EvtWord* args = event->args;
    s32 id = BattleTransID(event, evtGetValue(event, args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    evtSetFloat(event, args[1], ((EffMagic1N64Work*)((EffectEntry*)unit->work[UNIT_WORK_MAGIC_EFFECT])->userdata)->offset.x);
    return EVT_RETURN_DONE;
}

/**
 * Unit kind of the Magikoopa (HP 7, level 25): its size, offsets, damage sounds, status vulnerability,
 * parts and init event.
 */
BattleUnitKind unit_kamec = {
    UNIT_MAGIKOOPA, // unitType
    "btl_un_kamec", // name
    7, // maxHp
    0, // maxFp
    1, // dangerHP
    1, // perilHP
    25, // level
    0, // bonusExp
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
    parts, // parts
    init_event, // initEventCode
    data_table, // dataTable
};

/**
 * Unit kind of the Magikoopa (HP 7, level 0): its size, offsets, damage sounds, status vulnerability,
 * parts and init event.
 */
BattleUnitKind unit_kamec_bunsin = {
    UNIT_MAGIKOOPA_CLONE,
    "btl_un_kamec_bunsin",
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

/** Defence of the Magikoopa: no damage reduction. */
static BattleUnitDefense defence = {{0, 0, 0, 0, 0}};

/** Element reactions of the Magikoopa: no special reaction to any element. */
static BattleUnitDefenseAttr defence_attr = {{0, 0, 0, 0, 0}};

/** Status vulnerability of the Magikoopa (percent chance to take each status): resists most statuses. */
#if VERSION <= VERSION_DEMO_US
static StatusVulnerability regist = {40, 70, 60, 100, 60, 100, 100, 60, 100, 50, 100, 50, 100, 50, 50, 40, 70, 100, 60, 100, 100, 95};
#else
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
#endif

/** Status vulnerability of the Magikoopa (clone) (percent chance to take each status): immune to sleep. */
static StatusVulnerability regist_bunsin = {0};

static PoseTableEntry pose_table[] = {
    {POSE_TYPE_SLEEP, "KMK_N_1"},
    {POSE_TYPE_STOP, "KMK_Y_1"},
    {POSE_TYPE_FREEZE, "KMK_Y_1"},
    {POSE_TYPE_CONFUSE, "KMK_K_1"},
    {POSE_TYPE_POISON, "KMK_X_1"},
    {POSE_TYPE_DIZZY, "KMK_X_1"},
    {POSE_TYPE_NORMAL, "KMK_S_1"},
    {POSE_TYPE_DAMAGE_FIRE, "KMK_Q_1"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KMK_Q_1"},
    {POSE_TYPE_DAMAGE_GUARDED, "KMK_S_1"},
    {POSE_TYPE_DAMAGE, "KMK_D_1"},
    {POSE_TYPE_RUN, "KMK_R_1"},
    {POSE_TYPE_WALK, "KMK_W_1"},
    {POSE_TYPE_DANGER, "KMK_X_1"},
    {POSE_TYPE_PERIL, "KMK_X_1"},
    {POSE_TYPE_TALK, "KMK_T_1"},
    {POSE_TYPE_ACTION, "KMK_S_1"},
};

static PoseTableEntry pose_table_sky[] = {
    {POSE_TYPE_SLEEP, "KMK_N_2"},
    {POSE_TYPE_STOP, "KMK_Y_2"},
    {POSE_TYPE_FREEZE, "KMK_Y_2"},
    {POSE_TYPE_CONFUSE, "KMK_K_2"},
    {POSE_TYPE_POISON, "KMK_X_2"},
    {POSE_TYPE_DIZZY, "KMK_X_2"},
    {POSE_TYPE_NORMAL, "KMK_S_2"},
    {POSE_TYPE_DAMAGE_FIRE, "KMK_Q_2"},
    {POSE_TYPE_DAMAGE_FIRE_2_UNUSED, "KMK_Q_2"},
    {POSE_TYPE_DAMAGE_GUARDED, "KMK_S_2"},
    {POSE_TYPE_DAMAGE, "KMK_D_2"},
    {POSE_TYPE_RUN, "KMK_R_2"},
    {POSE_TYPE_WALK, "KMK_W_2"},
    {POSE_TYPE_DANGER, "KMK_X_2"},
    {POSE_TYPE_PERIL, "KMK_X_2"},
    {POSE_TYPE_TALK, "KMK_T_2"},
    {POSE_TYPE_ACTION, "KMK_S_2"},
};

/**
 * Data table of the Magikoopa: event handlers for dodging, a First Strike on it (no reaction),
 * being defeated and being blown away dead.
 */
static BattleDataEntry data_table[] = {
    {UNIT_DATA_DEAD_EVENT, kamec_subset_dead_event},
    {UNIT_DATA_BLOW_DEAD_EVENT, kamec_subset_blow_dead_event},
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Data table of the airborne Magikoopa: its defeat, blown-away defeat, dodge,
 * fall out of the air and First Strike reactions.
 */
static BattleDataEntry data_table_sky[] = {
    {UNIT_DATA_DEAD_EVENT, kamec_subset_dead_event},
    {UNIT_DATA_BLOW_DEAD_EVENT, kamec_subset_blow_dead_event},
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_DAMAGE_FALL_EVENT, dmg_fall_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0},
};

/** Data table of a Magikoopa clone: its dodge reaction; a First Strike on it is ignored. */
static BattleDataEntry data_table_bunsin[] = {
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, btldefaultevt_Dummy},
    {0, NULL},
};

/**
 * Data table of an airborne Magikoopa clone: its dodge reaction,
 * the fall when knocked out of the air and its First Strike reaction.
 */
static BattleDataEntry data_table_sky_bunsin[] = {
    {UNIT_DATA_AVOID_MISS_EVENT, avoid_miss_event},
    {UNIT_DATA_DAMAGE_FALL_EVENT, dmg_fall_event},
    {UNIT_DATA_FIRST_ATTACK_POS_EVENT, dmg_first_attack_pos},
    {0},
};

/** Parts of the Magikoopa (2 parts): hit positions, defences, attributes and pose tables. */
static BattleUnitKindPart parts[] = {
    {1, "btl_un_kamec", "c_kamek", 0.0f, 0.0f, 0.0f, 0.0f, 35.0f, 0.0f, 0.0f, 45.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kMainPart | kGuardTargetPart, 0, pose_table},
    {2, "btl_un_kamec", "c_kamek", 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 20, 30, 255, &defence, &defence_attr, kNotTargetable | kHidden | kNoShadow | kAbsolutePosition, 0, pose_table},
};

/** Weapon of the Magikoopa's attack spell: 4 damage. */
static BattleWeapon weapon_kamec_attack_magic = {
    NULL, 0, // name..icon
    {0}, // pad_06
    0, NULL, // itemId..description
    100, // baseAccuracy
    0, 0, // baseFpCost..baseSpCost
    2, // superguardsAllowed
    1.0f, // reserved_14
    1, // stylishCommandMultiplier
    1, // reserved_19
    1, // bingoSlotChance
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
#if VERSION <= VERSION_DEMO_US
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH | SPECIAL_IGNORE_VULNERABILITY,
#else
    SPECIAL_BADGE_POWER | SPECIAL_STATUS_POWER | SPECIAL_CHARGEABLE | SPECIAL_PIERCE_DEFENSE | SPECIAL_FREEZE_CRUSH, // special
#endif
    COUNTER_RESIST_ALL, // counterResistFlags
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

/** Weapon of the Magikoopa's Huge spell: no HP damage; may inflict size change. */
static BattleWeapon weapon_kamec_dekadeka_magic = {
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of the Magikoopa's Electric spell: no HP damage; may inflict electric shock. */
static BattleWeapon weapon_kamec_biribiri_magic = {
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of the Magikoopa's Invisible spell: no HP damage; may inflict invisibility. */
static BattleWeapon weapon_kamec_trans_magic = {
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of the Magikoopa's defence-up spell: no HP damage; may inflict defence change. */
static BattleWeapon weapon_kamec_katikati_magic = {
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of the Magikoopa's heal spell: no HP damage. */
static BattleWeapon weapon_kamec_recover_magic = {
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Weapon of the Magikoopa's heal-all spell: no HP damage. */
static BattleWeapon weapon_kamec_all_recover_magic = {
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
    NULL, // attackEvtScript
    {0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0}, // stageEffectRates
};

/** Spawn setup for a Magikoopa clone: starts far off-screen until the Magikoopa places it. */
static BattleUnitSetup entry_kamec = {
    &unit_kamec_bunsin, // unitKindParams
    ALLIANCE_ENEMY, // alliance
    {0}, // pad_05
    BATTLE_PHASE_ENEMY, // attackPhase
    {-1000.0f, -1000.0f, -1000.0f}, // position
    0, // hitOffsetX
    {0}, // unitWork
    NULL, // itemDropTable
};

/** Init script of the real Magikoopa. Registers its wait, attack, damage and other scripts and starts it waiting. */
static EVT_BEGIN(init_event)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_FADING, 0)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_STAFF_EFFECT, 0)
    CALL_CHILD(PTR(init_event2))
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/** Init script of a Magikoopa clone (bunsin). Same as init_event but the clone uses the sky data table (it floats). */
static EVT_BEGIN(init_event2)
    CALL(btlevtcmd_SetEventWait, UNIT_ID_SELF, PTR(wait_event))
    CALL(btlevtcmd_SetEventAttack, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_SetEventDamage, UNIT_ID_SELF, PTR(damage_event))
    CALL(btlevtcmd_SetEventConfusion, UNIT_ID_SELF, PTR(attack_event))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SKY, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table_sky))
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_sky))
        ELSE
            CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_sky_bunsin))
        END_IF
        CALL(btlevtcmd_OnAttribute, UNIT_ID_SELF, kFloating)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kIsWinged)
        CALL(btlevtcmd_OnPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), 40, LW(2))
        CALL(btlevtcmd_SetHomePos, UNIT_ID_SELF, LW(0), 40, LW(2))
    END_IF
    RETURN
EVT_END()

/**
 * Fades the unit in LW(0) (part 1) out, alpha 254 to 0 over 30 frames, and turns on its part's no-shadow
 * attribute; does nothing for unit -1. Used by bunsin_kill_event when a clone vanishes and by bunsin_magic_event
 * when the caster disappears before its clones appear.
 */
static EVT_BEGIN(alpha_down_event)
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 254)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, PART_ANIM_CHANGED, 1)
    WAIT_FRAMES(1)
    CALL(evt_sub_intpl_init, EASING_LINEAR, 254, 0, 30)
    LOOP(30)
        CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
        CALL(btlevtcmd_SetAlpha, LW(0), 1, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_OnPartsAttribute, LW(0), 1, kNoShadow)
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 0)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, PART_ANIM_CHANGED, 1)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

/**
 * Fades the unit in LW(0) (part 1) back in: turns the no-shadow attribute off
 * and raises alpha from 0 to 254 over
 * 30 frames, ending at 255; does nothing for unit -1.
 * bunsin_magic_event spawns it for the caster and each clone.
 */
static EVT_BEGIN(alpha_up_event)
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_OffPartsAttribute, LW(0), 1, kNoShadow)
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 0)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, PART_ANIM_CHANGED, 1)
    WAIT_FRAMES(1)
    CALL(evt_sub_intpl_init, EASING_LINEAR, 0, 254, 30)
    LOOP(30)
        CALL(evt_sub_intpl_get_value_para, LW(1), LW(2))
        CALL(btlevtcmd_SetAlpha, LW(0), 1, LW(1))
        WAIT_FRAMES(1)
    END_LOOP
    CALL(btlevtcmd_SetAlpha, LW(0), 1, 255)
    CALL(btlevtcmd_AnimeFlagOnOff, LW(0), 1, PART_ANIM_CHANGED, 1)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

/**
 * Damage reaction of the Magikoopa.
 * Runs the default damage; if it is a clone that was hit, or the real one died,
 * it kills the clones (bunsin_kill_event, kamec_subset_bunsin_all_kill).
 */
static EVT_BEGIN(damage_event)
    SET_VALUE(LW(10), UNIT_ID_SELF)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_KILL_BUNSIN, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(kamec_get_bunsin, LW(1), LW(2), LW(3), LW(4), LW(0))
        IF_INT_GREATER_OR_EQUAL(LW(0), 1)
            CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_KILL_BUNSIN, 1)
        END_IF
    ELSE
        SET_VALUE(LW(0), -2)
        CALL_CHILD(PTR(bunsin_kill_event))
        RETURN
    END_IF
    CALL_CHILD(PTR(btldefaultevt_Damage))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), -1)
        CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_KILL_BUNSIN, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL_CHILD(PTR(kamec_subset_bunsin_all_kill))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Kills all four of the Magikoopa's clones: fetches their unit ids with kamec_get_bunsin
 * and spawns bunsin_kill_event for each existing one.
 */
static EVT_BEGIN(kamec_subset_bunsin_all_kill)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, LW(0))
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

/**
 * Death handler shared by the dead events, for the unit in LW(10):
 * runs its data-table death event if it has one (slot 50), takes its back item, clears its statuses,
 * removes its clones and then kills the unit (kill mode 2, or 0 when the damage code says it was blown away).
 */
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

/**
 * Death script when the Magikoopa is blown away (Flurrie's gust):
 * plays SFX_BTL_CLAUD_BREATH3, kills all its clones and removes the unit.
 */
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

/**
 * Runs when an attack misses the Magikoopa; kills the clones (bunsin_kill_event) so the player sees
 * which one was real. Guess.
 */
static EVT_BEGIN(avoid_miss_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(0), -2)
        CALL_CHILD(PTR(bunsin_kill_event))
    END_IF
    RETURN
EVT_END()

/** Removes the clone whose unit id is in LW(0). */
static EVT_BEGIN(bunsin_kill_event)
    CALL(btlevtcmd_GetUnitId, LW(0), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        RETURN
    END_IF
    CALL(btlevtcmd_GetUnitWork, LW(0), UNIT_WORK_FADING, LW(1))
    IF_INT_EQUAL(LW(1), 1)
        GOTO(10)
    END_IF
    CALL(btlevtcmd_SetUnitWork, LW(0), UNIT_WORK_FADING, 1)
    CALL_CHILD(PTR(alpha_down_event))
    LABEL(10)
    CALL(btlevtcmd_WaitAttackEnd)
    CALL(btlevtcmd_KillUnit, LW(0), 0)
    RETURN
EVT_END()

/** Phase script of the Magikoopa; does nothing. */
static EVT_BEGIN(phase_event)
    RETURN
EVT_END()

/**
 * Fall reaction when a Magikoopa is hit while floating:
 * it switches to the clone data table, fades, falls with SFX_ENM_FALL1 and lands. Guess.
 */
static EVT_BEGIN(dmg_fall_event)
    SET_VALUE(LW(10), -2)
    SET_VALUE(LW(11), 1)
    CALL(btlevtcmd_OffAttribute, UNIT_ID_SELF, kFloating)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kIsWinged)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kHammerlikeCannotTarget)
    CALL(btlevtcmd_OffPartsAttribute, UNIT_ID_SELF, 1, kShellTosslikeCannotTarget)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    WAIT_MS(500)
    CALL(btlevtcmd_AnimeSetPoseTable, UNIT_ID_SELF, 1, PTR(pose_table))
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table))
    ELSE
        CALL(btlevtcmd_ChangeDataTable, UNIT_ID_SELF, PTR(data_table_bunsin))
    END_IF
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    BEGIN_INLINE_EVENT
        CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_SetPartsPos, UNIT_ID_SELF, 2, LW(0), LW(1), LW(2))
        CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 2, STRING("KMK_H_1"))
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
    CALL(btlevtcmd_SetUnitWork, UNIT_ID_SELF, UNIT_WORK_SKY, 0)
    CALL_CHILD(init_event2)
    CALL(btlevtcmd_AnimeChangePoseType, UNIT_ID_SELF, 1, POSE_TYPE_DAMAGE)
    RETURN
EVT_END()

/**
 * Data event 48 of the flying Magikoopa (data_table_sky), run by the default damage event when the battle opens
 * with Mario's or the partner's First Strike on it (damage code 0x39): sets its Y to 10 and waits for the attack
 * to end.
 */
static EVT_BEGIN(dmg_first_attack_pos)
    CALL(btlevtcmd_GetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    SET_VALUE(LW(1), 10)
    CALL(btlevtcmd_SetPos, UNIT_ID_SELF, LW(0), LW(1), LW(2))
    CALL(btlevtcmd_WaitAttackEnd)
    RETURN
EVT_END()

/**
 * Attack script of the Magikoopa.
 * Chooses a spell: usually the attack magic (weapon_kamec_attack_magic), or the heal, buff, Huge, Electric,
 * Invisible or defence spells for its allies, or creates its clones (bunsin_magic_event) depending on how many
 * clones exist and on random chances.
 */
static EVT_BEGIN(attack_event)
    // In a confused or forced turn it simply fires its attack spell.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        SET_VALUE(LW(9), PTR(&weapon_kamec_attack_magic))
        CALL_CHILD(PTR(attack_magic_event))
        GOTO(99)
    END_IF
    // A clone (not the real Magikoopa) does nothing on its own turn;
    // the real one may use an item instead of casting.
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(btlevtcmd_EnemyItemUseCheck, UNIT_ID_SELF, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL_CHILD(LW(0))
        GOTO(99)
    END_IF
    // Only one Magikoopa is left: it casts its attack spell most of the time, sometimes heals an ally,
    // or else splits into clones.
    CALL(kamec_get_live_num, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        LABEL(10)
        SET_VALUE(LW(1), 40)
        ADD(LW(1), 20)
        ADD(LW(1), 40)
        SUBTRACT(LW(1), 1)
        CALL(evt_sub_random, LW(1), LW(0))
        SET_VALUE(LW(1), 40)
        IF_INT_LESS(LW(0), LW(1))
            SET_VALUE(LW(9), PTR(&weapon_kamec_attack_magic))
            CALL_CHILD(PTR(attack_magic_event))
            GOTO(99)
        END_IF
        ADD(LW(1), 20)
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
        // Clones are out: the real one shoots its attack spell (a confused one is flustered).
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
            // Otherwise it picks a spell at random: the attack spell most often, or else the Huge,
            // defence-up, Electric, Invisible and heal spells,
            // trying again when no ally needs the chosen one.
            LABEL(20)
            SET_VALUE(LW(1), 50)
            ADD(LW(1), 10)
            ADD(LW(1), 10)
            ADD(LW(1), 10)
            ADD(LW(1), 10)
            ADD(LW(1), 10)
            ADD(LW(1), 10)
            CALL(evt_sub_random, LW(1), LW(0))
            SET_VALUE(LW(1), 50)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_attack_magic))
                CALL_CHILD(PTR(attack_magic_event))
                GOTO(99)
            END_IF
            ADD(LW(1), 10)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_dekadeka_magic))
                CALL_CHILD(PTR(dekadeka_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
            END_IF
            ADD(LW(1), 10)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_katikati_magic))
                CALL_CHILD(PTR(katikati_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
            END_IF
            ADD(LW(1), 10)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_biribiri_magic))
                CALL_CHILD(PTR(biribiri_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
            END_IF
            ADD(LW(1), 10)
            IF_INT_LESS(LW(0), LW(1))
                SET_VALUE(LW(9), PTR(&weapon_kamec_trans_magic))
                CALL_CHILD(PTR(trans_magic_event))
                IF_INT_EQUAL(LW(0), 0)
                    GOTO(20)
                END_IF
                GOTO(99)
            END_IF
            ADD(LW(1), 10)
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
    // The spell is done and the Magikoopa returns to waiting.
    LABEL(99)
    CALL(btlevtcmd_StartWaitEvent, UNIT_ID_SELF)
    RETURN
EVT_END()

/**
 * Spell-casting animation on the ground at another unit: the KMK_A_1A to A_1E wand poses with
 * SFX_ENM_KAMEKU_MAGIC1, 4 and 8, copied by every clone (kamec_get_bunsin); with LW(15) = 0 the caster and its
 * clones show the thunder-flare staff effect (eff_staff1), otherwise only the caster shows the charge light
 * (eff_staff2).
 */
static EVT_BEGIN(ground_magic_common_event1)
    // The magic hum plays twice while the Magikoopa and its clones prepare.
    BEGIN_SIBLING_EVENT
        LOOP(2)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(40)
        END_LOOP
    END_SIBLING_EVENT
    // A flare of light gathers on the staff (the real one's, and every clone's).
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
    // The Magikoopa and its clones raise their wands.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_1A"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMK_A_1A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMK_A_1A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMK_A_1A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMK_A_1A"))
    END_IF
    WAIT_FRAMES(60)
    // The wands glow with a rising sound.
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC4"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_1B"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMK_A_1B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMK_A_1B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMK_A_1B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMK_A_1B"))
    END_IF
    WAIT_FRAMES(20)
    // They swing the wands up and hold them.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_1C"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMK_A_1C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMK_A_1C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMK_A_1C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMK_A_1C"))
    END_IF
    WAIT_FRAMES(30)
    // A last magic sound, then they thrust the wands forward.
    BEGIN_SIBLING_EVENT
        WAIT_FRAMES(10)
        CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC8"), EVT_NULLPTR, 0, EVT_NULLPTR)
    END_SIBLING_EVENT
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_1D"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMK_A_1D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMK_A_1D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMK_A_1D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMK_A_1D"))
    END_IF
    WAIT_FRAMES(20)
    // The wand poses finish, ready for the spell itself.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_1E"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMK_A_1E"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMK_A_1E"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMK_A_1E"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMK_A_1E"))
    END_IF
    RETURN
EVT_END()

/**
 * Spell-casting animation on the ground for a spell on itself or the whole party:
 * the caster alone plays KMK_A_1A
 * and then KMK_A_2A to A_2C with SFX_ENM_KAMEKU_MAGIC6 and 7 and the thunder-flare staff effect.
 */
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
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_1A"))
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC6"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_2A"))
    WAIT_FRAMES(40)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC7"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_2B"))
    WAIT_FRAMES(20)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_2C"))
    RETURN
EVT_END()

/**
 * Spell-casting animation for a flying Magikoopa (unit work 0 set): the KMK_A_3A to A_3D poses with
 * SFX_ENM_KAMEKU_MAGIC1, 8 and 7, copied by every clone, with the staff effect chosen by LW(15) as in
 * ground_magic_common_event1.
 */
static EVT_BEGIN(sky_magic_common_event)
    // The magic hum plays twice while the flying Magikoopa and its clones prepare.
    BEGIN_SIBLING_EVENT
        LOOP(2)
            CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC1"), EVT_NULLPTR, 0, EVT_NULLPTR)
            WAIT_FRAMES(40)
        END_LOOP
    END_SIBLING_EVENT
    // A flare of light gathers on the staff (the real one's, and every clone's).
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
    // The Magikoopa and its clones raise their wands in the air.
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_3A"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMK_A_3A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMK_A_3A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMK_A_3A"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMK_A_3A"))
    END_IF
    // The wands glow with a rising sound.
    WAIT_FRAMES(60)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC8"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_3B"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMK_A_3B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMK_A_3B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMK_A_3B"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMK_A_3B"))
    END_IF
    // They sweep the wands forward as the spell sound peaks.
    WAIT_FRAMES(35)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC7"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_3C"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMK_A_3C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMK_A_3C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMK_A_3C"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMK_A_3C"))
    END_IF
    // The wand poses finish, ready for the spell itself.
    WAIT_FRAMES(15)
    CALL(btlevtcmd_AnimeChangePose, UNIT_ID_SELF, 1, STRING("KMK_A_3D"))
    IF_INT_NOT_EQUAL(LW(11), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(11), 1, STRING("KMK_A_3D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(12), 1, STRING("KMK_A_3D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(13), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(13), 1, STRING("KMK_A_3D"))
    END_IF
    IF_INT_NOT_EQUAL(LW(14), -1)
        CALL(btlevtcmd_AnimeChangePose, LW(14), 1, STRING("KMK_A_3D"))
    END_IF
    RETURN
EVT_END()

/**
 * Plays the casting animation for a spell at the target LW(3):
 * when flying (unit work 0 set) sky_magic_common_event,
 * else ground_magic_common_event1, or ground_magic_common_event2 when the target is the caster itself.
 */
static EVT_BEGIN(magic_common_event1)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SKY, LW(0))
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

/**
 * Plays the casting animation for the party-wide heal (all_recover_magic_event):
 * ground_magic_common_event2 on the
 * ground, sky_magic_common_event when flying (unit work 0 set).
 */
static EVT_BEGIN(magic_common_event2)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SKY, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(ground_magic_common_event2))
    ELSE
        CALL_CHILD(PTR(sky_magic_common_event))
    END_IF
    RETURN
EVT_END()

/**
 * Starts the target's avoid animation for the result of a pre-damage check (3, 6 or 2 select the three avoid
 * types).
 */
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

/**
 * The Magikoopa's attack spell (weapon in LW(9)): takes the first-attack target or samples one, casts
 * (magic_common_event1) with SFX_ENM_KAMEKU_MAGIC3_2, and fires a shot from itself and from every clone.
 * On a miss the shots fly past to the stage edge (attack_magic_event_sub_no_hit)
 * and the target's avoid starts as
 * the caster's shot passes it; on a hit they fly to the target (attack_magic_event_sub_hit)
 * and the action-command
 * result and damage are applied once. All of them then return to their stand pose.
 */
static EVT_BEGIN(attack_magic_event)
    // Choose the target: the First Strike target if the battle opened with one, otherwise a normal pick.
    CALL(btlevtcmd_check_battleflag, LW(0), BATTLE_FLAG_FIRST_ATTACK)
    IF_INT_NOT_EQUAL(LW(0), 0)
        CALL(btlevtcmd_GetFirstAttackTarget, LW(3), LW(4))
    ELSE
        CALL(btlevtcmd_GetEnemyBelong, UNIT_ID_SELF, LW(0))
        CALL(btlevtcmd_SamplingEnemy, UNIT_ID_SELF, LW(0), LW(9))
        CALL(btlevtcmd_ChoiceSamplingEnemy, LW(9), LW(3), LW(4))
    END_IF
    // No target left: a confused Magikoopa is flustered; either way it ends its turn.
    IF_INT_EQUAL(LW(3), -1)
        CALL(btlevtcmd_CheckToken, UNIT_ID_SELF, kConfusedAction, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL_CHILD(subsetevt_confuse_flustered)
            RETURN
        END_IF
        GOTO(99)
    END_IF
    // The attack is announced and the guard timing is awaited, and the Magikoopa turns to face its target.
    CALL(btlevtcmd_WeaponAftereffect, LW(9))
    CALL(btlevtcmd_AttackDeclare, UNIT_ID_SELF, LW(3), LW(4))
    CALL(btlevtcmd_WaitGuardMove)
    CALL(btlevtcmd_PayWeaponCost, UNIT_ID_SELF, LW(9))
    CALL(btlevtcmd_CalculateFaceDirection, UNIT_ID_SELF, -1, LW(3), LW(4), FACE_DIR_TOWARD_UNIT, LW(15))
    CALL(btlevtcmd_ChangeFaceDirection, UNIT_ID_SELF, LW(15))
    SET_VALUE(LW(15), 0)
    // The Magikoopa (and its clones) wave their wands and charge the spell.
    CALL_CHILD(magic_common_event1)
    CALL(btlevtcmd_snd_se, UNIT_ID_SELF, STRING("SFX_ENM_KAMEKU_MAGIC3_2"), EVT_NULLPTR, 0, EVT_NULLPTR)
    CALL(evt_btl_camera_set_mode, BTL_CAM_PRI_NORMAL, BTL_CAM_MODE_DEFAULT)
    // Check how the target defends against the blast.
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
        // Missed: the magic bolts fly past the target to the edge of the stage,
        // and the target hops out of the way as the real Magikoopa's bolt passes.
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
        // Hit: every bolt flies in, the target is blasted and the damage is dealt once.
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
        // All the casters lower their wands and return to their stand pose.
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

/**
 * Fires a missing shot from the unit in LW(14) (-2 = the caster, -1 = no clone: nothing):
 * the magic effect flies at
 * the target's height to the stage edge in the unit's facing direction over 60 frames.
 */
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

/**
 * Fires a hitting shot from the unit in LW(14) (-2 = the caster, -1 = no clone: nothing): the magic effect flies
 * to the target's hit position over 45 frames. The damage is applied by attack_magic_event.
 */
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

/**
 * Common part of the support spells (Huge, Defence up, Electrified, Invisible), with the status type in LW(15)
 * (10, 14, 6 or 18): samples an ally with the weapon in LW(9); if it already has that status nothing is cast and
 * LW(0) returns 0 (so the caller can pick something else);
 * otherwise it casts (magic_common_event1) and applies the
 * weapon to the ally, and LW(0) returns 1.
 */
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

/** Casts the size-up spell (dekadeka = huge) on an ally: makes it Huge (size change strength 2) for three turns. */
static EVT_BEGIN(dekadeka_magic_event)
    SET_VALUE(LW(15), 10)
    CALL_CHILD(PTR(support_magic_event))
    RETURN
EVT_END()

/** Casts the hardening spell (katikati = rock-hard) on an ally: raises its defence by 3 for three turns. */
static EVT_BEGIN(katikati_magic_event)
    SET_VALUE(LW(15), 14)
    CALL_CHILD(PTR(support_magic_event))
    RETURN
EVT_END()

/** Casts the electric spell (biribiri = tingling) on an ally: it becomes Electrified for three turns. */
static EVT_BEGIN(biribiri_magic_event)
    SET_VALUE(LW(15), 6)
    CALL_CHILD(PTR(support_magic_event))
    RETURN
EVT_END()

/** Casts the transparency spell on an ally: it becomes Invisible for three turns. */
static EVT_BEGIN(trans_magic_event)
    SET_VALUE(LW(15), 18)
    CALL_CHILD(PTR(support_magic_event))
    RETURN
EVT_END()

/** Heal spell on one ally: restores up to 8 HP. */
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

/**
 * Heal spell on all allies: restores up to 4 HP each.
 * Only the first target's missing HP is checked, so it may heal nobody when only that one is at full HP.
 */
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
    // BUG: inverted check. This should give up when there is no next enemy (LW(3) == -1); as written it
    // gives up whenever there is one, so only the first enemy is checked for missing HP, and if the
    // first is the only one and at full HP, the loop runs again with LW(3) == -1.
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

/**
 * The Magikoopa's clone spell (bunsin = split): casts with only its own staff light, fades out
 * (SFX_ENM_KAMEKU_OFFSHOOT1), spawns a clone (spawn_event) on each free spot found in 40-unit steps on both
 * sides
 * of it (down to x 0 and up to x 170), shuffles the real one's position with the clones' (kamec_shuffle)
 * and fades
 * them all in with SFX_ENM_KAMEKU_OFFSHOOT2.
 */
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
    s32 lifetime = evtGetValue(event, args[4]);
    f32 x;
    f32 y;
    f32 z;
    EffectEntry* eff;

    BtlUnit_GetPos(unit, &x, &y, &z);
    // from the tip of the wand
    x += 25.0f * unit->sizeMultiplier * unit->faceDirection;
    y += 20.0f * unit->sizeMultiplier;
    z += 5.0f;
    eff = effMagic1N64Entry(0, lifetime, x, y, z, tx, ty, tz);
    unit->work[UNIT_WORK_MAGIC_EFFECT] = (EvtWord)eff;
    ((EffMagic1N64Work*)eff->userdata)->scale = unit->sizeMultiplier;
    return EVT_RETURN_DONE;
}

/**
 * Script helper.
 * Keeps a charging-light effect at the Magikoopa's staff tip;
 * the effect is created on the first call in work slot 4 and the script blocks until the effect ends.
 */
static USER_FUNC(eff_staff2) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    f32* wk;

    if (isFirstCall) {
        unit->work[UNIT_WORK_STAFF_EFFECT] = (EvtWord)effPokopiPchargeN64Entry(0, 60, 0.0f, -1000.0f, 0.0f, 1.0f);
    }
    if (!(((EffectEntry*)unit->work[UNIT_WORK_STAFF_EFFECT])->flags & 1)) {
        return EVT_RETURN_DONE;
    }
    wk = ((EffectEntry*)unit->work[UNIT_WORK_STAFF_EFFECT])->userdata;
    wk[13] = unit->sizeMultiplier;
    BtlUnit_GetPos(unit, &x, &y, &z);
    wk[1] = x + 22.5f * unit->sizeMultiplier * unit->faceDirection;
    wk[2] = y + 40.0f * unit->sizeMultiplier;
    wk[3] = 5.0f + z;
    return EVT_RETURN_BLOCK;
}

/** Script helper. The same as eff_staff2 with a thunder-flare effect instead of the charge light. */
static USER_FUNC(eff_staff1) {
    s32 id = BattleTransID(event, evtGetValue(event, event->args[0]));
    BattleUnit* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    f32 x;
    f32 y;
    f32 z;
    f32* wk;

    if (isFirstCall) {
        unit->work[UNIT_WORK_STAFF_EFFECT] = (EvtWord)effThunderflareN64Entry(0, 60, 0.0f, -1000.0f, 0.0f, 1.0f);
    }
    if (!(((EffectEntry*)unit->work[UNIT_WORK_STAFF_EFFECT])->flags & 1)) {
        return EVT_RETURN_DONE;
    }
    wk = ((EffectEntry*)unit->work[UNIT_WORK_STAFF_EFFECT])->userdata;
    wk[10] = unit->sizeMultiplier;
    BtlUnit_GetPos(unit, &x, &y, &z);
    wk[2] = x + 22.5f * unit->sizeMultiplier * unit->faceDirection;
    wk[3] = y + 40.0f * unit->sizeMultiplier;
    wk[4] = 5.0f + z;
    return EVT_RETURN_BLOCK;
}

/**
 * Script helper.
 * Makes the Magikoopa clone args[1] share the real Magikoopa args[0]'s HP
 * and status effects (the copy's HP gauge is refreshed).
 */
static USER_FUNC(kamec_copy_status) {
    EvtWord* args = event->args;
    s32 id1 = evtGetValue(event, args[0]);
    s32 id2 = evtGetValue(event, args[1]);
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit1 = BattleGetUnitPtr(wp, id1);
    BattleUnit* unit2 = BattleGetUnitPtr(wp, id2);

    // Copy the original's current HP and status bytes, then synchronize the clone's HP gauge.
    unit2->currentHp = unit1->currentHp;
    memcpy(&unit2->sleepTurns, &unit1->sleepTurns, 0x1E);
    unit2->hpGauge.prevHp = unit2->currentHp;
    unit2->hpGauge.targetHp = unit2->currentHp;
    unit2->hpGauge.barRatio = unit2->hpGauge.trailRatio = (f32)unit2->currentHp / unit2->maxHp;
    return EVT_RETURN_DONE;
}

/**
 * Script helper.
 * Randomly swaps the stage positions (X and Z, current and home) of the Magikoopa and its clones ten times,
 * so the player cannot tell which is the real one.
 */
static USER_FUNC(kamec_shuffle) {
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* kamecs[6];
    s32 n = 0;
    s32 i;

    // Collect the original and clones, then perform ten random pair swaps of their current and home X/Z.
    for (i = 0; i < 64; i++) {
        kamecs[n] = wp->units[i];
        if (kamecs[n] != NULL && (kamecs[n]->currentKind == UNIT_MAGIKOOPA || kamecs[n]->currentKind == UNIT_MAGIKOOPA_CLONE)) {
            n++;
        }
    }
    for (i = 0; i < 10; i++) {
        s32 a = irand(n);
        s32 b = irand(n);
        BattleUnit* ua = kamecs[a];
        BattleUnit* ub = kamecs[b];
        f32 tmp;

        tmp = ua->position.x;
        ua->position.x = ub->position.x;
        ub->position.x = tmp;
        tmp = ua->homePosition.x;
        ua->homePosition.x = ub->homePosition.x;
        ub->homePosition.x = tmp;
        tmp = ua->position.z;
        ua->position.z = ub->position.z;
        ub->position.z = tmp;
        tmp = ua->homePosition.z;
        ua->homePosition.z = ub->homePosition.z;
        ub->homePosition.z = tmp;
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper.
 * Writes the unit ids of up to four Magikoopa clones (bunsin) to the first four arguments, -1 for empty slots,
 * and the number of clones to the fifth.
 */
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
        if (unit != NULL && unit->currentKind == UNIT_MAGIKOOPA_CLONE) {
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

/**
 * Script helper.
 * Counts the enemy-side units (alliance 1) that do not have STATUS_INSTANT_KILL, and stores the number in args[0].
 */
static USER_FUNC(kamec_get_live_num) {
    s32 i;
    EvtWord* args = event->args;
    s32 num = 0;
    BattleWork* wp = _battleWorkPointer;
    BattleUnit* unit;

    for (i = 0; i < 64; i++) {
        unit = wp->units[i];
        if (unit != NULL && unit->alliance == ALLIANCE_ENEMY && !BtlUnit_CheckStatus(unit, STATUS_INSTANT_KILL)) {
            num++;
        }
    }
    evtSetValue(event, args[0], num);
    return EVT_RETURN_DONE;
}

/**
 * Spawns one clone at the position in LW(0) to LW(2):
 * copies the airborne flag and statuses of the real unit, marks it as a clone in work 1,
 * gives it the clone attack event and resistances, and starts it invisible until alpha_up_event.
 */
static EVT_BEGIN(spawn_event)
    CALL(btlevtcmd_SpawnUnit, LW(3), PTR(&entry_kamec), 0)
    CALL(btlevtcmd_SetAlpha, LW(3), 1, 0)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_SKY, LW(10))
    CALL(btlevtcmd_SetUnitWork, LW(3), UNIT_WORK_SKY, LW(10))
    WAIT_FRAMES(2)
    CALL(btlevtcmd_SetPos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_SetHomePos, LW(3), LW(0), LW(1), LW(2))
    CALL(btlevtcmd_GetUnitId, UNIT_ID_SELF, LW(13))
    CALL(kamec_copy_status, LW(13), LW(3))
#if VERSION > VERSION_DEMO_US
    CALL(btlevtcmd_OffAttribute, LW(3), kUnitNoHpGauge)
#endif
    CALL(btlevtcmd_SetUnitWork, LW(3), UNIT_WORK_BUNSIN, 1)
    CALL(btlevtcmd_SetEventAttack, LW(3), PTR(attack_event))
    CALL(btlevtcmd_SetRegistStatus, LW(3), PTR(&regist_bunsin))
    CALL(btlevtcmd_OnPartsAttribute, LW(3), 1, kClone)
    RETURN
EVT_END()

/** Wait script of the Magikoopa: shows its idle pose (and the clones' poses) from the pose table. */
static EVT_BEGIN(wait_event)
    CALL(btlevtcmd_GetUnitWork, UNIT_ID_SELF, UNIT_WORK_BUNSIN, LW(0))
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

