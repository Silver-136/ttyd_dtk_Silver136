#pragma once

#include <dolphin/types.h>

/* BattleWeapon.targetClassFlags: which units an attack may target */
#define TARGET_CLASS_NO_MARIO_OR_SHELL       0x1 // not Mario or the shell shield
#define TARGET_CLASS_NO_PARTNER              0x2 // not a partner
#define TARGET_CLASS_NO_ENEMY                0x10 // not an enemy (UNIT_GOOMBA..UNIT_UNUSED_FLAVIO)
#define TARGET_CLASS_NO_TREE_OR_SWITCH       0x20 // not UNIT_UNUSED_TREE or UNIT_UNUSED_SWITCH
#define TARGET_CLASS_NO_SYSTEM_UNIT          0x40 // not UNIT_UNUSED_TESTNPC..UNIT_PROLOGUE_GOOMBELLA
#define TARGET_CLASS_NO_OPPOSITE_SIDE        0x100 // not the attacker's opponents
#define TARGET_CLASS_NO_OWN_SIDE             0x200 // not the attacker's own side
#define TARGET_CLASS_NO_SELF                 0x1000 // not the attacker
#define TARGET_CLASS_NO_SAME_KIND            0x2000 // not another unit of the attacker's kind
#define TARGET_CLASS_ONLY_SELF               0x4000 // only the attacker
#define TARGET_CLASS_ONLY_MARIO              0x10000 // only Mario
#define TARGET_CLASS_ONLY_TREE_OR_SWITCH     0x20000 // only UNIT_UNUSED_TREE or UNIT_UNUSED_SWITCH
#define TARGET_CLASS_ONLY_MAIN_PARTS         0x100000 // only main or important parts
#define TARGET_CLASS_ONLY_MAIN_OR_SEMI_PARTS 0x200000 // only main, important or semi-important parts
#define TARGET_CLASS_SINGLE                  0x1000000 // one target at a time
#define TARGET_CLASS_MULTIPLE                0x2000000 // all targets at once
#define TARGET_CLASS_NONE                    0x80000000 // no valid targets

/* BattleWeapon.targetWeightFlags: how an enemy chooses among the targets */
#define TARGET_WEIGHT_MARIO         0x1 // prefer Mario
#define TARGET_WEIGHT_PARTNER       0x2 // prefer a partner
#define TARGET_WEIGHT_FRONT         0x4 // prefer targets early in the target list
#define TARGET_WEIGHT_BACK          0x8 // prefer targets late in the target list
#define TARGET_WEIGHT_OWN_SIDE      0x10 // prefer the attacker's side
#define TARGET_WEIGHT_OPPOSITE_SIDE 0x20 // prefer the attacker's opponents
#define TARGET_WEIGHT_LOW_HP_RATIO  0x100 // prefer a low HP / max HP
#define TARGET_WEIGHT_HIGH_HP_RATIO 0x200 // prefer a high HP / max HP
#define TARGET_WEIGHT_LOW_HP        0x400 // prefer low HP
#define TARGET_WEIGHT_HIGH_HP       0x800 // prefer high HP
#define TARGET_WEIGHT_IN_PERIL      0x1000 // prefer targets in Peril
#define TARGET_WEIGHT_RANDOM        0x80000000 // pick at random by weight (else the heaviest)

// BattleWeapon.counterResistFlags: counters the weapon ignores (BattleCheckCounter)
typedef enum BattleWeaponCounterResist_Flags {
    COUNTER_RESIST_ELECTRIC = 0x1,
    COUNTER_RESIST_TOP_SPIKY = 0x2,
    COUNTER_RESIST_PREEMPTIVE_SPIKY = 0x4,
    COUNTER_RESIST_FRONT_SPIKY = 0x8,
    COUNTER_RESIST_FIERY = 0x10,
    COUNTER_RESIST_ICY = 0x20,
    COUNTER_RESIST_POISON = 0x40,
    COUNTER_RESIST_EXPLOSIVE = 0x80,
    COUNTER_RESIST_VOLATILE_EXPLOSIVE = 0x100,
    COUNTER_RESIST_PAYBACK = 0x200,
    COUNTER_RESIST_HOLD_FAST = 0x400, // the Payback status and the Return Postage badge
    COUNTER_RESIST_ALL = 0x7FF,
} BattleWeaponCounterResist_Flags;

// BattleWeapon.special (bits checked in battle_damage.c; 0x200 and 0x8000 are set in some tables, their use is
// not clear)
typedef enum BattleWeaponSpecialFlags {
    SPECIAL_BADGE_POWER = 0x1, // badges add to or subtract from the power (BattlePreCheckDamage)
    SPECIAL_STATUS_POWER = 0x2, // Huge/Tiny, Attack Up/Down and Merlee's boost change the power
    SPECIAL_CHARGEABLE = 0x4, // uses up a Charge
    SPECIAL_CANNOT_MISS = 0x8,
    SPECIAL_DIMINISH_BY_HIT = 0x10, // power minus the hits the target took this attack (min 1)
    SPECIAL_DIMINISH_BY_TARGET = 0x20, // power minus the attacker's targets so far (min 1)
    SPECIAL_PIERCE_DEFENSE = 0x40,
    SPECIAL_FREEZE_CRUSH = 0x80, // meant to break frozen targets; its rand(100) < 0 test never passes
    SPECIAL_IGNORE_VULNERABILITY = 0x100, // status chance ignores the target's resistance (unless it is 0)
    SPECIAL_IGNITABLE = 0x400, // fire element when the attacker is burning
    SPECIAL_ATTACK_FX = 0x800, // plays the Attack FX badge sounds
    SPECIAL_FLIP_SHELLED = 0x1000,
    SPECIAL_FLIP_BOMB_FLIPPABLE = 0x2000,
    SPECIAL_GROUND_WINGED = 0x4000,
    SPECIAL_CONFUSED_ACTION = 0x10000, // can be picked as a confused unit's action
    SPECIAL_UNGUARDABLE = 0x20000,
    SPECIAL_CONTACTS_CLONES = 0x40000, // hits clone parts (PartsAttribute 0x4000) instead of passing through
} BattleWeaponSpecialFlags;

// BattleWeapon.element (weapon tables: Ice Storm and blizzards 2, bombs and dynamite 3, thunder items 4)
typedef enum BattleWeaponElement {
    ELEMENT_NORMAL,
    ELEMENT_FIRE,
    ELEMENT_ICE,
    ELEMENT_EXPLOSION,
    ELEMENT_ELECTRIC,
} BattleWeaponElement;

typedef struct BattleWeapon {
    const char* name;                 // 0x0, msg tag
    u16 icon;                         // 0x4
    u8 pad_06[0x8 - 0x6];             // 0x6, always 0, never accessed
    s32 itemId;                       // 0x8
    const char* description;          // 0xC
    s8 baseAccuracy;                  // 0x10
    s8 baseFpCost;                    // 0x11
    s8 baseSpCost;                    // 0x12
    u8 superguardsAllowed;            // 0x13
    f32 reserved_14;                  // 0x14, 1.0f in every weapon table, never read
    s8 mStylishCommandMultiplier;     // 0x18
    s8 reserved_19;                   // 0x19, 1 (5 on player moves); copied to StarPowerInfo.weaponParam, never read
    s8 mBingoSlotChance;              // 0x1A
    s8 reserved_1B;                   // 0x1B, 1 (50 on player moves), never read
    void* hpDamageFn;                 // 0x1C
    u32 hpDamageWork[8];              // 0x20
    void* fpDamageFn;                 // 0x40
    u32 fpDamageWork[8];              // 0x44
    u32 targetClassFlags;             // 0x64
    u32 targetPropertyFlags;          // 0x68
    u8 element;                       // 0x6C
    u8 damagePattern;                 // 0x6D
    s8 acLevel;                       // 0x6E
    s8 reserved_6F;                   // 0x6F, 2 in every weapon table, never read
    const char* acHelpMsg;            // 0x70
    BattleWeaponSpecialFlags special; // 0x74
    u32 counterResistFlags;           // 0x78
    u32 targetWeightFlags;            // 0x7C
    u8 sleepChance;                   // 0x80, status chances are % (0-100), times in turns
    s8 sleepTime;                     // 0x81
    u8 stopChance;                    // 0x82
    s8 stopTime;                      // 0x83
    u8 dizzyChance;                   // 0x84
    s8 dizzyTime;                     // 0x85
    u8 poisonChance;                  // 0x86
    s8 poisonTime;                    // 0x87
    s8 poisonStrength;                // 0x88
    u8 confuseChance;                 // 0x89
    s8 confuseTime;                   // 0x8A
    u8 electricChance;                // 0x8B
    s8 electricTime;                  // 0x8C
    u8 dodgyChance;                   // 0x8D
    s8 dodgyTime;                     // 0x8E
    u8 burnChance;                    // 0x8F
    s8 burnTime;                      // 0x90
    u8 freezeChance;                  // 0x91
    s8 freezeTime;                    // 0x92
    u8 sizeChangeChance;              // 0x93
    s8 sizeChangeTime;                // 0x94
    s8 sizeChangeStrength;            // 0x95, >0 grow, <0 shrink
    u8 atkChangeChance;               // 0x96
    s8 atkChangeTime;                 // 0x97
    s8 atkChangeStrength;             // 0x98, >0 up, <0 down
    u8 defChangeChance;               // 0x99
    s8 defChangeTime;                 // 0x9A
    s8 defChangeStrength;             // 0x9B, >0 up, <0 down
    u8 allergicChance;                // 0x9C
    s8 allergicTime;                  // 0x9D
    s8 ohkoChance;                    // 0x9E
    s8 chargeStrength;                // 0x9F
    u8 fastChance;                    // 0xA0
    s8 fastTime;                      // 0xA1
    u8 slowChance;                    // 0xA2
    s8 slowTime;                      // 0xA3
    u8 frightChance;                  // 0xA4
    u8 galeForceChance;               // 0xA5
    s8 paybackTime;                   // 0xA6
    s8 holdFastTime;                  // 0xA7
    u8 invisibleChance;               // 0xA8
    s8 invisibleTime;                 // 0xA9
    s8 hpRegenTime;                   // 0xAA
    s8 hpRegenStrength;               // 0xAB
    s8 fpRegenTime;                   // 0xAC
    s8 fpRegenStrength;               // 0xAD
    u8 pad_AE[0xB0 - 0xAE];           // 0xAE, always 0, never accessed
    void* mAttackEvtScript;           // 0xB0
    u8 stageEffectRates[0xC0 - 0xB4]; // 0xB4, btlevtcmd_WeaponAftereffect: [0..3] A1+A2 / A1 / A2 / none fall weights, [4] B destroy, [5] nozzle turn, [6] nozzle fire, [7] iron frame fall, [8] object fall (%), [9..11] unused
} BattleWeapon;

#include "driver/npcdrv.h" /* ItemDropData, PointDropData */

extern PointDropData battle_flower_drop_param_default[];
extern PointDropData battle_flower_drop_param_default2[];
extern PointDropData battle_flower_drop_param_default3[];
extern PointDropData battle_flower_drop_param_default4[];
extern PointDropData battle_flower_drop_param_default5[];
extern PointDropData battle_heart_drop_param_default[];
extern PointDropData battle_heart_drop_param_default2[];
extern PointDropData battle_heart_drop_param_default3[];
extern PointDropData battle_heart_drop_param_default4[];
extern PointDropData battle_heart_drop_param_default5[];
extern ItemDropData battle_item_NONE[];
extern ItemDropData battle_item_atmic_teresa[];
extern ItemDropData battle_item_badge_borodo[];
extern ItemDropData battle_item_barriern[];
extern ItemDropData battle_item_barriern_custom[];
extern ItemDropData battle_item_barriern_z[];
extern ItemDropData battle_item_basabasa[];
extern ItemDropData battle_item_basabasa_chururu[];
extern ItemDropData battle_item_basabasa_green[];
extern ItemDropData battle_item_bllizard[];
extern ItemDropData battle_item_bomhei[];
extern ItemDropData battle_item_boomerang_bros[];
extern ItemDropData battle_item_borodo[];
extern ItemDropData battle_item_borodo_king[];
extern ItemDropData battle_item_bubble[];
extern ItemDropData battle_item_burst_wanwan[];
extern ItemDropData battle_item_champion[];
extern ItemDropData battle_item_chorobon[];
extern ItemDropData battle_item_churantalar[];
extern ItemDropData battle_item_cortez[];
extern ItemDropData battle_item_dark_keeper[];
extern ItemDropData battle_item_default_param_list[];
extern ItemDropData battle_item_dokugassun[];
extern ItemDropData battle_item_faker_mario[];
extern ItemDropData battle_item_fire_bros[];
extern ItemDropData battle_item_flower_chorobon[];
extern ItemDropData battle_item_gesso[];
extern ItemDropData battle_item_giant_bomb[];
extern ItemDropData battle_item_gold_chorobon[];
extern ItemDropData battle_item_gonbaba[];
extern ItemDropData battle_item_green_chorobon[];
extern ItemDropData battle_item_gundan_zako[];
extern ItemDropData battle_item_gundan_zako_elite[];
extern ItemDropData battle_item_gundan_zako_magician[];
extern ItemDropData battle_item_hammer_bros[];
extern ItemDropData battle_item_hannya[];
extern ItemDropData battle_item_heavy_bom[];
extern ItemDropData battle_item_hennya[];
extern ItemDropData battle_item_hermos[];
extern ItemDropData battle_item_hinnya[];
extern ItemDropData battle_item_honenoko[];
extern ItemDropData battle_item_hyper_jyugem[];
extern ItemDropData battle_item_hyper_kuriboo[];
extern ItemDropData battle_item_hyper_patakuri[];
extern ItemDropData battle_item_hyper_sinemon[];
extern ItemDropData battle_item_hyper_sinnosuke[];
extern ItemDropData battle_item_hyper_togekuri[];
extern ItemDropData battle_item_hyper_togemet[];
extern ItemDropData battle_item_ice_pakkun[];
extern ItemDropData battle_item_iron_sinemon[];
extern ItemDropData battle_item_iron_sinemon2[];
extern ItemDropData battle_item_jyugem[];
extern ItemDropData battle_item_kamec[];
extern ItemDropData battle_item_kanbu1[];
extern ItemDropData battle_item_karon[];
extern ItemDropData battle_item_killer[];
extern ItemDropData battle_item_killer_cannon[];
extern ItemDropData battle_item_kuriboo[];
extern ItemDropData battle_item_kurikuri[];
extern ItemDropData battle_item_kurokumorn[];
extern ItemDropData battle_item_macho_gance[];
extern ItemDropData battle_item_magnum_battender[];
extern ItemDropData battle_item_mahorn[];
extern ItemDropData battle_item_mahorn_custom[];
extern ItemDropData battle_item_met[];
extern ItemDropData battle_item_moamoa[];
extern ItemDropData battle_item_monban[];
extern ItemDropData battle_item_monochrome__pakkun[];
extern ItemDropData battle_item_monochrome_kurokumorn[];
extern ItemDropData battle_item_monochrome_sinemon[];
extern ItemDropData battle_item_nokonoko[];
extern ItemDropData battle_item_nokonoko_fighter[];
extern ItemDropData battle_item_pakkun_flower[];
extern ItemDropData battle_item_pansy[];
extern ItemDropData battle_item_patakuri[];
extern ItemDropData battle_item_patamet[];
extern ItemDropData battle_item_patapata[];
extern ItemDropData battle_item_patapata_fighter[];
extern ItemDropData battle_item_patatogemet[];
extern ItemDropData battle_item_phantom[];
extern ItemDropData battle_item_piders[];
extern ItemDropData battle_item_poison_pakkun[];
extern ItemDropData battle_item_purple_teresa[];
extern ItemDropData battle_item_rampell[];
extern ItemDropData battle_item_red_honenoko[];
extern ItemDropData battle_item_sambo[];
extern ItemDropData battle_item_sambo_mummy[];
extern ItemDropData battle_item_sinemon[];
extern ItemDropData battle_item_sinnosuke[];
extern ItemDropData battle_item_super_killer[];
extern ItemDropData battle_item_super_killer_cannon[];
extern ItemDropData battle_item_super_mahorn[];
extern ItemDropData battle_item_teresa[];
extern ItemDropData battle_item_togedaruma[];
extern ItemDropData battle_item_togekuri[];
extern ItemDropData battle_item_togemet[];
extern ItemDropData battle_item_togenoko[];
extern ItemDropData battle_item_togenoko_ace[];
extern ItemDropData battle_item_togezo[];
extern ItemDropData battle_item_twinkling_pansy[];
extern ItemDropData battle_item_ura_noko[];
extern ItemDropData battle_item_ura_pata[];
extern ItemDropData battle_item_vivian1[];
extern ItemDropData battle_item_wanwan[];
extern ItemDropData battle_item_yami_kuriboo[];
extern ItemDropData battle_item_yami_noko[];
extern ItemDropData battle_item_yami_patakuri[];
extern ItemDropData battle_item_yami_togekuri[];
extern ItemDropData battle_item_yamitogedaruma[];
extern ItemDropData battle_item_zonbaba[];
