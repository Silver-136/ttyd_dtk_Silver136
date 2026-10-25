#pragma once

#include <dolphin/types.h>
#include "evt/evt_word.h"
#include "battle/battle_damage.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_status_icon.h"
#include "driver/effdrv.h"
#include "mario/mario_pouch.h"

// BattleUnit.mAlliance: the side a unit fights on.
typedef enum BattleUnitAlliance {
    ALLIANCE_PLAYER,  // Mario and his partners
    ALLIANCE_ENEMY,
    ALLIANCE_NEUTRAL, // stage objects
} BattleUnitAlliance;

typedef enum BattleUnitToken_Flags {
    kChargeExpended = (1 << 3),     // 0x8
    kConfusedAction = (1 << 4),     // 0x10
    kCurrentlyActing = (1 << 12),   // 0x1000
    kHasRallyWinkStatus = (1 << 13) // 0x2000
} BattleUnitToken_Flags;

typedef enum PartsAttribute_Flags {          // 32-bit size
    kMainPart = (1 << 0),                    // 0x1
    kImportantPart = (1 << 1),               // 0x2
    kSemiImportantPart = (1 << 2),           // 0x4
    kUnknown_0x8 = (1 << 3),                 // 0x8
    kUnknown_0x10 = (1 << 4),                // 0x10
    kUnknown_0x40 = (1 << 6),                // 0x40
    kWeakToAttackFxR = (1 << 7),             // 0x80
    kWeakToIcePower = (1 << 8),              // 0x100
    kIsWinged = (1 << 11),                   // 0x800
    kIsShelled = (1 << 12),                  // 0x1000
    kIsBombFlippable = (1 << 13),            // 0x2000
    kUnknown_0x4000 = (1 << 14),             // 0x4000
    kNotTargetable = (1 << 16),              // 0x10000
    kUnknown_0x40000 = (1 << 18),            // 0x40000
    kUntattleable = (1 << 19),               // 0x80000
    kJumplikeCannotTarget = (1 << 20),       // 0x100000
    kHammerlikeCannotTarget = (1 << 21),     // 0x200000
    kShellTosslikeCannotTarget = (1 << 22),  // 0x400000
    kUnknown_0x800000 = (1 << 23),           // 0x800000
    kImmuneToDamageAndStatus = (1 << 29),   // 0x20000000, no HP/FP damage and no status (battle_damage.c)
    kImmuneToNegativeStatus = (1 << 30),    // 0x40000000, sleep, stop, poison, ... and instant kill blocked
    kImmuneToStatus = (1 << 31)             // 0x80000000, no status at all
} PartsAttribute_Flags;

typedef enum BattleUnitAttribute_Flags { // 32-bit size
    kUnknown_0x1 = (1 << 0),             // 0x1
    kCeiling = (1 << 1),                 // 0x2
    kFloating = (1 << 2),                // 0x4
    kUnknownUnit_0x8 = (1 << 3),         // 0x8
    kVeiled = (1 << 4),                  // 0x10
    kShellShielded = (1 << 5),           // 0x20
    kUnknownUnit_0x1000 = (1 << 12),     // 0x1000
    kUnknownUnit_0x2000 = (1 << 13),     // 0x2000
    kInactive = (1 << 17),               // 0x20000
    kUnknownUnit_0x2000000 = (1 << 25)   // 0x2000000
} BattleUnitAttribute_Flags;

typedef enum PartsCounterAttribute_Flags {   // 32-bit size
    kCounterTopSpiky = (1 << 0),             // 0x1
    kCounterPreemptiveFrontSpiky = (1 << 1), // 0x2
    kCounterFrontSpiky = (1 << 2),           // 0x4
    kCounterFiery = (1 << 4),                // 0x10
    kCounterFieryStatus = (1 << 5),          // 0x20
    kCounterIcy = (1 << 6),                  // 0x40
    kCounterIcyStatus = (1 << 7),            // 0x80
    kCounterPoison = (1 << 8),               // 0x100
    kCounterPoisonStatus = (1 << 9),         // 0x200
    kCounterElectric = (1 << 10),            // 0x400
    kCounterElectricStatus = (1 << 11),      // 0x800
    kCounterExplosive = (1 << 12),           // 0x1000
    kCounterVolatileExplosive = (1 << 13)    // 0x2000
} PartsCounterAttribute_Flags;

typedef enum BattleUnitType {
    UNIT_NULL,                            // 0x0
    UNIT_GOOMBA,                          // 0x1
    UNIT_PARAGOOMBA,                      // 0x2
    UNIT_SPIKY_GOOMBA,                    // 0x3
    UNIT_SPINIA,                          // 0x4
    UNIT_SPANIA,                          // 0x5
    UNIT_LORD_CRUMP_PROLOGUE,             // 0x6
    UNIT_GUS,                             // 0x7
    UNIT_BLOOPER,                         // 0x8
    UNIT_BLOOPER_LEFT_TENTACLE,           // 0x9
    UNIT_BLOOPER_RIGHT_TENTACLE,          // 0xA
    UNIT_KOOPATROL,                       // 0xB
    UNIT_MAGIKOOPA,                       // 0xC
    UNIT_MAGIKOOPA_CLONE,                 // 0xD
    UNIT_KOOPA_TROOPA,                    // 0xE
    UNIT_PARATROOPA,                      // 0xF
    UNIT_FUZZY,                           // 0x10
    UNIT_DULL_BONES,                      // 0x11
    UNIT_BALD_CLEFT,                      // 0x12
    UNIT_BRISTLE,                         // 0x13
    UNIT_GOLD_FUZZY,                      // 0x14
    UNIT_FUZZY_HORDE,                     // 0x15
    UNIT_RED_BONES,                       // 0x16
    UNIT_HOOKTAIL,                        // 0x17
    UNIT_DARK_PUFF,                       // 0x18
    UNIT_PALE_PIRANHA,                    // 0x19
    UNIT_CLEFT,                           // 0x1A
    UNIT_PIDER,                           // 0x1B
    UNIT_X_NAUT,                          // 0x1C
    UNIT_YUX,                             // 0x1D
    UNIT_MINI_YUX,                        // 0x1E
    UNIT_BELDAM_CHAPTER2,                 // 0x1F
    UNIT_MARILYN_CHAPTER2,                // 0x20
    UNIT_VIVIAN_CHAPTER2,                 // 0x21
    UNIT_MAGNUS,                          // 0x22
    UNIT_X_FIST,                          // 0x23
    UNIT_GOOMBA_GLITZVILLE,               // 0x24
    UNIT_KP_KOOPA,                        // 0x25
    UNIT_KP_PARATROOPA,                   // 0x26
    UNIT_POKEY,                           // 0x27
    UNIT_LAKITU,                          // 0x28
    UNIT_SPINY,                           // 0x29
    UNIT_HYPER_BALD_CLEFT,                // 0x2A
    UNIT_BOB_OMB,                         // 0x2B
    UNIT_BANDIT,                          // 0x2C
    UNIT_BIG_BANDIT,                      // 0x2D
    UNIT_RED_SPIKY_BUZZY,                 // 0x2E
    UNIT_SHADY_KOOPA,                     // 0x2F
    UNIT_SHADY_PARATROOPA,                // 0x30
    UNIT_RED_MAGIKOOPA,                   // 0x31
    UNIT_RED_MAGIKOOPA_CLONE,             // 0x32
    UNIT_WHITE_MAGIKOOPA,                 // 0x33
    UNIT_WHITE_MAGIKOOPA_CLONE,           // 0x34
    UNIT_GREEN_MAGIKOOPA,                 // 0x35
    UNIT_GREEN_MAGIKOOPA_CLONE,           // 0x36
    UNIT_DARK_CRAW,                       // 0x37
    UNIT_HAMMER_BRO,                      // 0x38
    UNIT_BOOMERANG_BRO,                   // 0x39
    UNIT_FIRE_BRO,                        // 0x3A
    UNIT_RED_CHOMP,                       // 0x3B
    UNIT_DARK_KOOPATROL,                  // 0x3C
    UNIT_IRON_CLEFT_RED,                  // 0x3D
    UNIT_IRON_CLEFT_GREEN,                // 0x3E
    UNIT_BOWSER_CHAPTER3,                 // 0x3F
    UNIT_RAWK_HAWK,                       // 0x40
    UNIT_MACHO_GRUBBA,                    // 0x41
    UNIT_HYPER_GOOMBA,                    // 0x42
    UNIT_HYPER_PARAGOOMBA,                // 0x43
    UNIT_HYPER_SPIKY_GOOMBA,              // 0x44
    UNIT_CRAZEE_DAYZEE,                   // 0x45
    UNIT_AMAZY_DAYZEE,                    // 0x46
    UNIT_HYPER_CLEFT,                     // 0x47
    UNIT_BUZZY_BEETLE,                    // 0x48
    UNIT_SPIKE_TOP,                       // 0x49
    UNIT_SWOOPER,                         // 0x4A
    UNIT_BOO,                             // 0x4B
    UNIT_ATOMIC_BOO,                      // 0x4C
    UNIT_DOOPLISS_CHAPTER4_FIGHT1,        // 0x4D
    UNIT_DOOPLISS_CHAPTER4_INVINCIBLE,    // 0x4E
    UNIT_DOOPLISS_CHAPTER4_FIGHT2,        // 0x4F
    UNIT_GOOMBELLA_CHAPTER4,              // 0x50
    UNIT_KOOPS_CHAPTER4,                  // 0x51
    UNIT_YOSHI_CHAPTER4,                  // 0x52
    UNIT_FLURRIE_CHAPTER4,                // 0x53
    UNIT_EMBER,                           // 0x54
    UNIT_LAVA_BUBBLE,                     // 0x55
    UNIT_GREEN_FUZZY,                     // 0x56
    UNIT_FLOWER_FUZZY,                    // 0x57
    UNIT_PUTRID_PIRANHA,                  // 0x58
    UNIT_PARABUZZY,                       // 0x59
    UNIT_BILL_BLASTER,                    // 0x5A
    UNIT_BULLET_BILL,                     // 0x5B
    UNIT_BULKY_BOB_OMB,                   // 0x5C
    UNIT_CORTEZ,                          // 0x5D
    UNIT_CORTEZ_BONE_PILE,                // 0x5E
    UNIT_CORTEZ_SWORD,                    // 0x5F
    UNIT_CORTEZ_HOOK,                     // 0x60
    UNIT_CORTEZ_RAPIER,                   // 0x61
    UNIT_CORTEZ_SABER,                    // 0x62
    UNIT_LORD_CRUMP_CHAPTER5,             // 0x63
    UNIT_X_NAUTS_CRUMP_FORMATION1,        // 0x64
    UNIT_X_NAUTS_CRUMP_FORMATION2,        // 0x65
    UNIT_X_NAUTS_CRUMP_FORMATION3,        // 0x66
    UNIT_RUFF_PUFF,                       // 0x67
    UNIT_POISON_POKEY,                    // 0x68
    UNIT_SPIKY_PARABUZZY,                 // 0x69
    UNIT_DARK_BOO,                        // 0x6A
    UNIT_SMORG,                           // 0x6B
    UNIT_SMORG_MIASMA_TENTACLE_A,         // 0x6C
    UNIT_SMORG_MIASMA_TENTACLE_B,         // 0x6D
    UNIT_SMORG_MIASMA_TENTACLE_C,         // 0x6E
    UNIT_SMORG_MIASMA_CLAW,               // 0x6F
    UNIT_ICE_PUFF,                        // 0x70
    UNIT_FROST_PIRANHA,                   // 0x71
    UNIT_MOON_CLEFT,                      // 0x72
    UNIT_Z_YUX,                           // 0x73
    UNIT_MINI_Z_YUX,                      // 0x74
    UNIT_X_YUX,                           // 0x75
    UNIT_MINI_X_YUX,                      // 0x76
    UNIT_X_NAUT_PHD,                      // 0x77
    UNIT_ELITE_X_NAUT,                    // 0x78
    UNIT_MAGNUS2_0,                       // 0x79
    UNIT_X_PUNCH,                         // 0x7A
    UNIT_SWOOPULA,                        // 0x7B
    UNIT_PHANTOM_EMBER,                   // 0x7C
    UNIT_BOMBSHELL_BILL_BLASTER,          // 0x7D
    UNIT_BOMBSHELL_BILL,                  // 0x7E
    UNIT_CHAIN_CHOMP,                     // 0x7F
    UNIT_DARK_WIZZERD,                    // 0x80
    UNIT_DARK_WIZZERD_CLONE,              // 0x81
    UNIT_DRY_BONES,                       // 0x82
    UNIT_DARK_BONES,                      // 0x83
    UNIT_GLOOMTAIL,                       // 0x84
    UNIT_BELDAM_CHAPTER8,                 // 0x85
    UNIT_MARILYN_CHAPTER8,                // 0x86
    UNIT_DOOPLISS_CHAPTER8,               // 0x87
    UNIT_DOOPLISS_CHAPTER8_MARIO,         // 0x88
    UNIT_DOOPLISS_CHAPTER8_GOOMBELLA,     // 0x89
    UNIT_DOOPLISS_CHAPTER8_KOOPS,         // 0x8A
    UNIT_DOOPLISS_CHAPTER8_YOSHI,         // 0x8B
    UNIT_DOOPLISS_CHAPTER8_FLURRIE,       // 0x8C
    UNIT_DOOPLISS_CHAPTER8_VIVIAN,        // 0x8D
    UNIT_DOOPLISS_CHAPTER8_BOBBERY,       // 0x8E
    UNIT_DOOPLISS_CHAPTER8_MS_MOWZ,       // 0x8F
    UNIT_BOWSER_CHAPTER8,                 // 0x90
    UNIT_KAMMY_KOOPA,                     // 0x91
    UNIT_GRODUS,                          // 0x92
    UNIT_GRODUS_X,                        // 0x93
    UNIT_SHADOW_QUEEN_PEACH,              // 0x94
    UNIT_SHADOW_QUEEN_INVINCIBLE,         // 0x95
    UNIT_SHADOW_QUEEN_DEMON,              // 0x96
    UNIT_SHADOW_QUEEN_LEFT_RIGHT_HAND,    // 0x97
    UNIT_SHADOW_QUEEN_DEAD_HANDS,         // 0x98
    UNIT_GLOOMBA,                         // 0x99
    UNIT_PARAGLOOMBA,                     // 0x9A
    UNIT_SPIKY_GLOOMBA,                   // 0x9B
    UNIT_DARK_KOOPA,                      // 0x9C
    UNIT_DARK_PARATROOPA,                 // 0x9D
    UNIT_BADGE_BANDIT,                    // 0x9E
    UNIT_DARK_LAKITU,                     // 0x9F
    UNIT_SKY_BLUE_SPINY,                  // 0xA0
    UNIT_WIZZERD,                         // 0xA1
    UNIT_PIRANHA_PLANT,                   // 0xA2
    UNIT_SPUNIA,                          // 0xA3
    UNIT_ARANTULA,                        // 0xA4
    UNIT_DARK_BRISTLE,                    // 0xA5
    UNIT_POISON_PUFF,                     // 0xA6
    UNIT_SWAMPIRE,                        // 0xA7
    UNIT_BOB_ULK,                         // 0xA8
    UNIT_ELITE_WIZZERD,                   // 0xA9
    UNIT_ELITE_WIZZERD_CLONE,             // 0xAA
    UNIT_BONETAIL,                        // 0xAB
    UNIT_UNUSED_RED_BUZZY,                // 0xAC
    UNIT_UNUSED_RED_PARABUZZY,            // 0xAD
    UNIT_UNUSED_RED_SPIKY_PARABUZZY,      // 0xAE
    UNIT_UNUSED_HYPER_BOB_OMB,            // 0xAF
    UNIT_UNUSED_ULTRA_BOB_OMB,            // 0xB0
    UNIT_TUTORIAL_GOOMBELLA,              // 0xB1
    UNIT_TUTORIAL_FRANKLY_B2,             // 0xB2
    UNIT_TUTORIAL_FRANKLY_B3,             // 0xB3
    UNIT_TUTORIAL_FRANKLY_B4,             // 0xB4
    UNIT_EPILOGUE_DOOPLISS_MARIO,         // 0xB5
    UNIT_EPILOGUE_FLURRIE,                // 0xB6
    UNIT_EPILOGUE_BOO,                    // 0xB7
    UNIT_EPILOGUE_ATOMIC_BOO,             // 0xB8
    UNIT_EPILOGUE_MALE_TOAD,              // 0xB9
    UNIT_EPILOGUE_FEMALE_TOAD,            // 0xBA
    UNIT_UNUSED_TEST,                     // 0xBB
    UNIT_UNUSED_CRUMP2,                   // 0xBC
    UNIT_UNUSED_BELDAM2,                  // 0xBD
    UNIT_UNUSED_MARILYN2,                 // 0xBE
    UNIT_UNUSED_VIVIAN2,                  // 0xBF
    UNIT_UNUSED_BELDAM3,                  // 0xC0
    UNIT_UNUSED_MARILYN3,                 // 0xC1
    UNIT_UNUSED_MECHA_KURI,               // 0xC2
    UNIT_UNUSED_MECHA_KAME,               // 0xC3
    UNIT_UNUSED_OKORL,                    // 0xC4
    UNIT_UNUSED_YOWARL,                   // 0xC5
    UNIT_UNUSED_TUYONARL,                 // 0xC6
    UNIT_UNUSED_WANAWANA,                 // 0xC7
    UNIT_UNUSED_MINARAI_KAMEC,            // 0xC8
    UNIT_UNUSED_SHY_GUY,                  // 0xC9
    UNIT_UNUSED_GROOVE_GUY,               // 0xCA
    UNIT_UNUSED_PYRO_GUY,                 // 0xCB
    UNIT_UNUSED_SPY_GUY,                  // 0xCC
    UNIT_UNUSED_ANTI_GUY,                 // 0xCD
    UNIT_UNUSED_BZZAP,                    // 0xCE
    UNIT_UNUSED_MINI_BZZAP,               // 0xCF
    UNIT_UNUSED_UFO,                      // 0xD0
    UNIT_UNUSED_PENNINGTON,               // 0xD1
    UNIT_UNUSED_FIGHTER,                  // 0xD2
    UNIT_UNUSED_ZESS_T,                   // 0xD3
    UNIT_UNUSED_MASTER,                   // 0xD4
    UNIT_UNUSED_REPORTER,                 // 0xD5
    UNIT_UNUSED_HOTDOG_MASTER,            // 0xD6
    UNIT_UNUSED_FLAVIO,                   // 0xD7
    UNIT_UNUSED_TREE,                     // 0xD8
    UNIT_UNUSED_SWITCH,                   // 0xD9
    UNIT_UNUSED_TESTNPC,                  // 0xDA
    UNIT_BOMB_SQUAD_BOMB,                 // 0xDB
    UNIT_SYSTEM,                          // 0xDC
    UNIT_PROLOGUE_GOOMBELLA,              // 0xDD
    UNIT_MARIO,                           // 0xDE
    UNIT_SHELL_SHIELD,                    // 0xDF
    UNIT_GOOMBELLA,                       // 0xE0
#define TYPE_PARTNER_MIN UNIT_GOOMBELLA   // 0xE0
    UNIT_KOOPS,                           // 0xE1
    UNIT_YOSHI,                           // 0xE2
    UNIT_FLURRIE,                         // 0xE3
    UNIT_VIVIAN,                          // 0xE4
    UNIT_BOBBERY,                         // 0xE5
    UNIT_MS_MOWZ,                         // 0xE6
#define TYPE_PARTNER_MAX (UNIT_MS_MOWZ + 1) // 0xE7
    UNIT_MAX,                             // 0xE7
} BattleUnitType;

typedef struct BattleDataEntry {
    s32 tag;    // 0x0
    void* data; // 0x4
} BattleDataEntry;

typedef struct BattleUnitDefense {
    s8 defenses[5]; // 0x0
} BattleUnitDefense;

typedef struct BattleUnitDefenseAttr {
    s8 defenseAttrs[5]; // 0x0
} BattleUnitDefenseAttr;

typedef struct StatusVulnerability {
    u8 sleep;        // 0x0
    u8 stop;         // 0x1
    u8 dizzy;        // 0x2
    u8 poison;       // 0x3
    u8 confuse;      // 0x4
    u8 electric;     // 0x5
    u8 burn;         // 0x6
    u8 freeze;       // 0x7
    u8 huge;         // 0x8
    u8 tiny;         // 0x9
    u8 attackUp;     // 0xA
    u8 attackDown;   // 0xB
    u8 defenseUp;    // 0xC
    u8 defenseDown;  // 0xD
    u8 allergic;     // 0xE
    u8 fright;       // 0xF
    u8 galeForce;    // 0x10
    u8 fast;         // 0x11
    u8 slow;         // 0x12
    u8 dodgy;        // 0x13
    u8 invisible;    // 0x14
    u8 ohko;         // 0x15
} StatusVulnerability;

typedef struct PoseTableEntry {
    s32 poseType;         // 0x0
    const char* poseName; // 0x4
} PoseTableEntry;

typedef struct BattleUnitKindPart {
    s32 partNum;                                   // 0x0
    const char* partName;                          // 0x4
    const char* modelName;                         // 0x8
    Vec positionOffset;                            // 0xC
    Vec hitBaseOffset;                             // 0x18
    Vec hitCursorBaseOffset;                       // 0x24
    s16 shadowSize;                                // 0x30, -> BattleWorkUnitPart.shadowSize
    s16 reserved_32;                               // 0x32, -> BattleWorkUnitPart.reserved_1A4, never read
    s16 baseAlpha;                                 // 0x34
    BattleUnitDefense* defense;                    // 0x38
    BattleUnitDefenseAttr* defenseAttr;            // 0x3C
    PartsAttribute_Flags attributes;               // 0x40
    PartsCounterAttribute_Flags counterAttributes; // 0x44
    void* poseTable;                               // 0x48
} BattleUnitKindPart;

// movement sound loop state (BtlUnit_SetSeMode / _MoveSoundControlCore)
typedef struct BecSndWork {
    const char* name0; // 0x0
    const char* name1; // 0x4
    s16 reserved_08;       // 0x8, BtlUnit_SetSeMode arg a
    s16 reserved_0A;       // 0xA, BtlUnit_SetSeMode arg b
    s16 delay;         // 0xC, start delay
    s16 period0;       // 0xE
    s16 period1;       // 0x10
    s16 timer;         // 0x12
    s16 count;         // 0x14
    s32 sfxId;         // 0x18
} BecSndWork;

// one movement sound set (BtlUnit_LoadSeMode)
typedef struct BtlPoseSoundEntry {
    const char* name0;
    const char* name1;
    s16 delay;   // start delay frames
    s16 period0; // frames between name0 plays
    s16 period1; // frames between name1 plays
} BtlPoseSoundEntry;

typedef struct BtlPoseSoundTable {
    const char* jumpName0;       // 0x0, mode 2: jump start
    const char* jumpName1;       // 0x4, mode 2/3: landing
    BtlPoseSoundEntry entry[3];  // 0x8, mode 0 run, 1 walk, 4 dive (btlevtcmd_Set*RunSound/WalkSound/DiveSound)
} BtlPoseSoundTable;

// pose-synced sound playback (BtlUnit_PoseSoundMain)
typedef struct BtlPoseSoundData {
    const char* animName; // 0x0
    f32 time;             // 0x4
    s32 (*callback)(struct BattleUnit*, struct BtlPoseSoundData*, void*); // 0x8
    const char* sfxName;  // 0xC
    s16 pitch;            // 0x10
    u16 flags;            // 0x12
} BtlPoseSoundData;

typedef struct BtlPartPoseSound {
    BtlPoseSoundData* table; // 0x0
    u32 loopCount;           // 0x4
    f32 time;                // 0x8
    f32 prevTime;            // 0xC
    s32 count;               // 0x10
    u8 flag;                 // 0x14
} BtlPartPoseSound;

typedef struct BattleWorkUnitPartBlurEntry {
    s32 flags;       // 0x0
    Mtx orientation; // 0x4
    f32 rotation;    // 0x34
    GXColor base;    // 0x38
    GXColor blurColor[2]; // 0x3C, newest / oldest after-image tint (copied from BattleWorkUnitPartBlur.color)
} BattleWorkUnitPartBlurEntry;

typedef struct BattleWorkUnitPartBlur {
    s32 flags;                            // 0x0
    GXColor color[2];                     // 0x4, after-image tint, newest / oldest (btlevtcmd_SetPartsBlur)
    BattleWorkUnitPartBlurEntry work[10]; // 0xC
} BattleWorkUnitPartBlur;

typedef struct BattleWorkUnitPart {
    struct BattleWorkUnitPart* mNextPart;                  // 0x0
    BattleUnitKindPart* mKindPartParams;                   // 0x4
    const char* partName;                                  // 0x8
    Vec mHomePosition;                                     // 0xC
    Vec mPosition;                                         // 0x18
    Vec mPositionOffset;                                   // 0x24
    Vec mDisplayOffset;                                    // 0x30
    Vec mBaseRotation;                                     // 0x3C
    Vec mRotation;                                         // 0x48
    Vec mRotationOffset;                                   // 0x54
    Vec mScale;                                            // 0x60
    Vec mBaseScale;                                        // 0x68
    f32 shadowScale;                                       // 0x78, extra shadow size multiplier
    Vec mMoveStartPos;                                     // 0x7C
    Vec mMoveCurrentPos;                                   // 0x88
    Vec mMoveTargetPos;                                    // 0x94
    s32 moveTotalFrames;                                        // 0xA0, frame count of the current parts move/jump
    f32 mMoveSpeedXZ;                                      // 0xA4
    f32 mFallAccel;                                        // 0xA8
    f32 moveSpeedY;                                        // 0xAC, jump vertical speed, minus mFallAccel per frame
    f32 moveAngle;                                         // 0xB0, XZ angle to the move target
    f32 moveDistance;                                      // 0xB4, XZ distance to the move target (then the per-frame remainder)
    f32 moveFrame;                                         // 0xB8, current frame of an interpolated parts move
    s8 moveDirection;                                      // 0xBC
    s8 faceDirection;                                      // 0xBD
    u8 pad_BE[0xC0 - 0xBE];                                // 0xBE, never accessed
    BecSndWork moveSnd;                                    // 0xC0, move-sound loop (BtlUnit_LoadSeMode / _MoveSoundControlCore)
    u8 pad_DC[0xF8 - 0xDC];                                // 0xDC, never accessed
    BtlPoseSoundTable moveSndTable;                        // 0xF8, btlevtcmd_SetPartsJump/Run/Walk/DiveSound
    EvtWord work[16];                                      // 0x130, part work words (btlevtcmd_SetPartsWork / GetPartsWork): numbers and table pointers
    Vec mHitBaseOffset;                                    // 0x170, copied from the kind part's hitBaseOffset
    Vec mHitOffset;                                        // 0x17C
    Vec mHitCursorBaseOffset;                              // 0x188, copied from the kind part's hitCursorBaseOffset
    Vec mHitCursorOffset;                                  // 0x194
    s16 hitOffsetX;                                        // 0x1A0, target-cursor X offset (btl_hitoffsetx)
    s16 shadowSize;                                        // 0x1A2, from kind part 0x30; scales shadowEntry
    s16 reserved_1A4;                                      // 0x1A4, copied from BattleUnitKindPart.reserved_32, never read
    u16 gravityTimer;                                      // 0x1A6, frame counter for the gravity bob offset
    u16 floatTimer;                                        // 0x1A8, frame counter for the floating offset
    u8 pad_1AA[0x1AC - 0x1AA];                             // 0x1AA, never accessed
    PartsAttribute_Flags attributes;                       // 0x1AC
    PartsCounterAttribute_Flags counterAttributes;         // 0x1B0
    BattleUnitDefense* defense;                            // 0x1B4
    BattleUnitDefenseAttr* defenseAttr;                    // 0x1B8
    void* poseTable;                                       // 0x1BC
    s32 poseId;                                            // 0x1C0
    char poseName[0x40];                                   // 0x1C4, current pose name (strcmp'd in unit scripts)
    u32 animFlags;                                         // 0x204
    f32 animSpeed;                                         // 0x208, init 1.0
    f32 animSpeed2;                                        // 0x20C, init 1.0
    void (*dispCallback)(struct BattleWorkUnitPart*, s32); // 0x210, called before (0) and after (1) drawing
    u8 poseState;                                          // 0x214
    u8 poseType;                                           // 0x215
    u16 poseTimer;                                         // 0x216
    GXColor drawColor;                                     // 0x218, material colour set per frame from dispColor
    BattleWorkUnitPartBlur blurWork;                       // 0x21C
    BtlPartPoseSound poseSound;                            // 0x4D0, btlevtcmd_AnimeSetPoseSoundTable
    s32 handle;                                            // 0x4E8, -1 at entry, reset to -1 on unit delete; never set otherwise in C
    struct BattleUnit* mOwner;                             // 0x4EC
    GXColor color;                                         // 0x4F0
    GXColor dispColor;                                     // 0x4F4, color with alpha = baseAlpha * color.a
    f32 dispZOffset;                                       // 0x4F8, added to z for the display sort
    s8 rotationOrder;                                      // 0x4FC, selects the rotation matrix concat order
    u8 pad_4FD[0x500 - 0x4FD];                             // 0x4FD, never accessed
} BattleUnitPart;

typedef struct BattleWorkUnitBadgesEquipped {
    u8 CloseCall;     // 0x0
    u8 PrettyLucky;   // 0x1
    u8 LuckyDay;      // 0x2
    u8 reserved_03;       // 0x3, badge count slot 3, never read
    u8 PowerPlus;     // 0x4
    u8 PUpDDown;      // 0x5
    u8 AllOrNothing;  // 0x6
    u8 MegaRush;      // 0x7
    u8 PowerRush;     // 0x8
    u8 PDownDUp;      // 0x9
    u8 DoublePain;    // 0xA
    u8 LastStand;     // 0xB
    u8 DefendPlus;    // 0xC
    u8 DamageDodge;   // 0xD
    u8 HappyHeart;    // 0xE
    u8 HappyFlower;   // 0xF
    u8 ReturnPostage; // 0x10
    u8 HpPlus;        // 0x11
    u8 FpPlus;        // 0x12
    u8 DoubleDip;     // 0x13
    u8 TripleDip;     // 0x14
    u8 FlowerSaver;   // 0x15
    u8 FeelingFine;   // 0x16
    u8 ZapTap;        // 0x17
    u8 PityFlower;    // 0x18
    u8 HpDrain;       // 0x19
    u8 FpDrain;       // 0x1A
    u8 Refund;        // 0x1B
    u8 Charge;        // 0x1C
    u8 SuperCharge;   // 0x1D
    u8 DefendUnused;  // 0x1E
    u8 Jumpman;       // 0x1F
    u8 Hammerman;     // 0x20
    u8 IcePower;      // 0x21
    u8 SpikeShield;   // 0x22
    u8 SuperAppeal;   // 0x23
    u8 LuckyStart;    // 0x24
    u8 Simplifier;    // 0x25
    u8 Unsimplifier;  // 0x26
    u8 AutoCommand;   // 0x27
} BattleWorkUnitBadgesEquipped;

typedef struct BattleWorkHpGauge { // TODO: diff header file?
    u16 mode;          // 0x0, gauge style; 1 = trailing bar
    u16 flags;         // 0x2, 1 = hide the gauge when visibleTimer runs out
    s16 drainTimer;    // 0x4, frames left of the bar animation
    s16 drainLength;   // 0x6, bar animation length (20, or 40 in mode 1)
    s16 visibleTimer;  // 0x8, frames the gauge stays up; fades out below 20
    s16 visibleLength; // 0xA, drainLength + 30
    s32 prevHp;        // 0xC, HP the bar animates from
    s32 targetHp;      // 0x10, HP the bar animates to
    f32 barRatio;      // 0x14, main bar fill, 0..1
    f32 trailRatio;    // 0x18, trailing (lost HP) bar fill, 0..1
} BattleWorkHpGauge;

// TODO: move somewhere more sensible
typedef struct ItemDropData {
    ItemType type;         // 0x0
    s16 holdWeight;        // 0x4
    s16 dropWeight;        // 0x6
} ItemDropData;

// note to self: +4 is near mUnk_HpGauge at 0x1FC
typedef struct StChgInfo {
    s8 type;         // 0x0, -1 = free
    s8 strength;     // 0x1, status strength being announced
    u8 stagger;      // 0x2, 10 per queued entry
    u8 stackOffset;  // 0x3, 0x3C or 0x50, +10 per entry from the newest
    s32 reserved_04; // 0x4, always set to 0
    s32 changed;     // 0x8, 1: strength changed / newly inflicted (selects msg1)
} StChgInfo;                  // status-change message queue entry

typedef struct BtlStatusCopy {
    u8 data[0x1E]; // BattleUnit status turns/strengths from mSleepTurns, copied as one block
} BtlStatusCopy;

typedef struct BattleUnit {
    s32 unitId;                                  // 0x0
    BattleUnitType initialKind;                  // 0x4
    BattleUnitType currentKind;                  // 0x8
    s8 mAlliance;                                // 0xC
    u8 mLevel;                                   // 0xD
    s8 groupSetupId;                             // 0xE
    u8 pad_0F;                                   // 0xF, never accessed
    struct BattleUnitKind* mKindParams;          // 0x10
    BattleUnitPart* mParts;                  // 0x14
    BattleDataEntry* dataTable;                  // 0x18
    s32 flags;                                   // 0x1C
    s8 moveState;                                // 0x20
    s8 maxMoveCount;                             // 0x21
    s8 movesRemaining;                           // 0x22
    s8 maxMovesThisTurn;                         // 0x23
    s8 activeTurns;                              // 0x24
    s8 swallowRate;                              // 0x25
    s8 swallowAttribute;                         // 0x26
    u8 pad_27;                                   // 0x27, never accessed
    s32 movePriority;                            // 0x28
    s32 attackPhase;                             // 0x2C
    Vec mHomePosition;                           // 0x30
    Vec mPosition;                               // 0x3C
    Vec mPositionOffset;                         // 0x48
    Vec mDisplayOffset;                          // 0x54
    Vec mBaseRotation;                           // 0x60
    Vec mRotation;                               // 0x6C
    Vec mRotationOffset;                         // 0x78
    Vec offsetCenter;                            // 0x84
    Vec mScale;                                  // 0x90
    Vec mBaseScale;                              // 0x9C
    Vec togeOffset;                              // 0xA8
    Vec heldItemBaseOffset;                      // 0xB4, copied from BattleUnitKind.heldItemBaseOffset
    Vec heldItemOffset;                          // 0xC0
    s16 mWidth;                                  // 0xCC
    s16 mHeight;                                 // 0xCE
    s16 mStatusIconOffset[2];                    // 0xD0
    s16 mHpGaugeOffset[2];                       // 0xD4
    Vec cutBaseOffset;                           // 0xD8
    f32 cutWidth;                                // 0xE4
    f32 cutHeight;                               // 0xE8
    Vec bintaHitOffset;                          // 0xEC, Love Slap
    Vec kissHitOffset;                           // 0xF8, Lip Lock
    BattleUnitAttribute_Flags mAttributeFlags;   // 0x104
    s16 maxHp;                                   // 0x108
    s16 baseMaxHp;                               // 0x10A
    s16 currentHp;                               // 0x10C
    s16 maxFp;                                   // 0x10E
    s16 baseMaxFp;                               // 0x110
    s16 currentFp;                               // 0x112
    f32 mSizeMultiplier;                         // 0x114
    union {
        struct {
            s8 mSleepTurns;                              // 0x118
            s8 mSleepStrength;                           // 0x119
            s8 mStopTurns;                               // 0x11A
            s8 mDizzyTurns;                              // 0x11B
            s8 mPoisonTurns;                             // 0x11C
            s8 mPoisonStrength;                          // 0x11D
            s8 mConfusionTurns;                          // 0x11E
            s8 mElectricTurns;                           // 0x11F
            s8 mDodgyTurns;                              // 0x120
            s8 mBurnTurns;                               // 0x121
            s8 mFreezeTurns;                             // 0x122
            s8 mSizeChangeTurns;                         // 0x123
            s8 mSizeChangeStrength;                      // 0x124
            s8 mAtkChangeTurns;                          // 0x125
            s8 mAtkChangeStrength;                       // 0x126
            s8 mDefChangeTurns;                          // 0x127
            s8 mDefChangeStrength;                       // 0x128
            s8 mChargeStrength;                          // 0x129
            s8 mAllergicTurns;                           // 0x12A
            s8 flippedTurns;                             // 0x12B, turn count left until flipping back
            s8 mInvisibleTurns;                          // 0x12C
            s8 mPaybackTurns;                            // 0x12D
            s8 mHoldFastTurns;                           // 0x12E
            s8 mFastTurns;                               // 0x12F
            s8 mSlowTurns;                               // 0x130
            s8 mHpRegenTurns;                            // 0x131
            s8 mHpRegenStrength;                         // 0x132
            s8 mFpRegenTurns;                            // 0x133
            s8 mFpRegenStrength;                         // 0x134
            s8 mInstantKillStrength;                     // 0x135
        };
        BtlStatusCopy statusTurns;                // 0x118, the 0x1E status bytes as one block (party info save/restore)
    };
    u8 pad_136[2];                               // 0x136, never accessed
    s32 mStatusFlags;                            // 0x138
#ifdef VERSION_G8ME01
    u16 usCommandDisabled[2]; // US only (+4 from here): per-unit turns left for the two command blocks JP keeps in BattleWork commandDisabled[3..4]
#endif
    s32 protectId;                               // 0x13C
    BattleVulnerableStatus* vulnStatus;          // 0x140
    Vec mMoveStartPos;                           // 0x144
    Vec mMoveCurrentPos;                         // 0x150
    Vec mMoveTargetPos;                          // 0x15C
    s32 moveFrames;                              // 0x168, move/jump frame count (BEC_MVFRAME)
    f32 mMoveSpeedXZ;                            // 0x16C
    f32 mFallAccel;                              // 0x170
    f32 mMoveSpeedY;                             // 0x174
    f32 mMoveDirection;                          // 0x178
    f32 mMoveDistance;                           // 0x17C, XZ distance of the move (mMoveSpeedXZ = this / moveFrames)
    f32 mMoveTotalFrames;                        // 0x180, moveFrames at the move start (as f32)
    s8 moveDirection;                            // 0x184
    s8 faceDirection;                            // 0x185
    u8 pad_186[0x188 - 0x186];                   // 0x186 (US 0x18A), never accessed
    BecSndWork moveSnd;                          // 0x188 (US 0x18C)
    Vec mJumpStep;                               // 0x1A4 (US 0x1A8), per-frame XZ/Y step of btlevtcmd_MarioJumpPosition
    f32 mJumpAngle;                              // 0x1B0 (US 0x1B4), arc angle in degrees
    f32 mJumpAngleStep;                          // 0x1B4 (US 0x1B8), arc angle added per frame
    f32 mJumpBaseY;                              // 0x1B8 (US 0x1BC), last Y position of the jump
    u8 pad_1BC[0x1C0 - 0x1BC];                   // 0x1BC (US 0x1C0), never accessed
    BtlPoseSoundTable moveSndTable;              // 0x1C0 (US 0x1C4), jump names + run/walk/dive entries (btlevtcmd_SetJump/Run/Walk/DiveSound)
    BattleWorkHpGauge hpGauge;                   // 0x1F8
    EvtWord work[16];                            // 0x214, unit work words (btlevtcmd_SetUnitWork / GetUnitWork): numbers, effect and work pointers; floats stored in the word (SetUnitWorkFloat / GetUnitWorkFloat)
    BattleUnitPart* currentTarget;           // 0x254
    s32 reserved_258;                            // 0x258 (US 0x25C), only cleared at turn start (battle_seq)
    s16 hpDamageCount;                           // 0x25C, number of damaging moves
    u8 pad_25E[0x260 - 0x25E];                   // 0x25E, never accessed
    s32 totalHpDamageTaken;                      // 0x260
    s32 totalFpDamageTaken;                      // 0x264 (US 0x268), sum of fpDamageTaken this turn
    s32 totalFpLost;                             // 0x268 (US 0x26C), sum of fpLost this turn
    s8 hpDamageTaken;                            // 0x26C
    s8 fpDamageTaken;                            // 0x26D
    s8 fpLost;                                   // 0x26E (US 0x272), FP actually lost by the last hit (fpDamageTaken clamped to current FP)
    s8 superguardCountTaken;                     // 0x26F (US 0x273), attacker's superguardedCount when this unit superguarded it
    s32 damagePattern;                           // 0x270, damage pattern of the last hit (selects the hurt pose), 0 when cleared
    s32 damageCode;                              // 0x274
    BattleUnitToken_Flags mTokenFlags;           // 0x278
    s32 superguardedCount;                       // 0x27C (US 0x280), times this unit was superguarded this turn; weapon special 0x20 subtracts it from damage
    s32 totalDamageDealt;                        // 0x280, damage this unit dealt as attacker
    s32 initEventId;                             // 0x284
    void* waitEventCode;                         // 0x288
    s32 waitEventId;                             // 0x28C
    void* unisonPhaseEventCode;                  // 0x290
    void* phaseEventCode;                        // 0x294
    s32 phaseEventId;                            // 0x298
    void* attackEventCode;                       // 0x29C, btlevtcmd_SetEventAttack / BtlUnit_GetData(unit, 2/3)
    void* confusionEventCode;                    // 0x2A0, btlevtcmd_SetEventConfusion
    s32 attackEventId;                           // 0x2A4
    s32 attackEventFlags;                        // 0x2A8, selects/controls the act event in battle_seq (0 by SetEventAttack)
    void* damageEventCode;                       // 0x2AC
    s32 damageEventId;                           // 0x2B0
    void* entryEventCode;                        // 0x2B4 (US 0x2B8), btlevtcmd_SetEventEntry
    s32 entryEventId;                                // 0x2B8 (US 0x2BC)
    void* ceilFallEventCode;                     // 0x2BC (US 0x2C0), btlevtcmd_SetEventCeilFall, run by the stage ceiling fall
    s32 ceilFallEventId;                                // 0x2C0 (US 0x2C4)
    void* actEndEvent;                           // 0x2C4 (US 0x2C8), run when the unit's action ends (battle_seq)
    s32 actEndEventId;                                   // 0x2C8 (US 0x2CC)
    void* hitEventCode;                          // 0x2CC (US 0x2D0), run on the attacker by BattleRunHitEventDirect
    s32 talkPartId;                                  // 0x2D0 (US 0x2D4)
    const char* talkPose;                        // 0x2D4 (US 0x2D8), pose type 0x41 (btlevtcmd talk anim)
    const char* stayPose;                        // 0x2D8 (US 0x2DC), pose type 0x2B
    union {
        BattleWorkUnitBadgesEquipped badgesEquipped; // 0x2DC
        u8 badgeCounts[sizeof(BattleWorkUnitBadgesEquipped)]; // 0x2DC, the same 40 counters by index
    };
    ItemType heldItem;                           // 0x304, 4 bytes
    ItemDropData* dropTable;                     // 0x308
    u8 stolenCoins;                              // 0x30C, coins a Bandit (borodo) stole; dropped back as its held coin item
    u8 mMoveColorLv;                             // 0x30D
    u8 moveColorLvDisp;                          // 0x30E, current move colour level, ramps toward mMoveColorLv
    u8 pad_30F;                                  // 0x30F, never accessed
    struct BattleWeapon* countersetWeapon;       // 0x310
    void (*deleteFunc)(struct BattleUnit*);      // 0x314 (US 0x318), called by BtlUnit_Delete
    struct smartEntry* screenTexBuf;             // 0x318 (US 0x31C), smartAlloc EFB copy (0x260x0x1E0, unit_party_chuchurina), freed on unit delete
    u8 pad_31C[0x320 - 0x31C];                   // 0x31C (US 0x320), never accessed
    EffectEntry* sleepEff;                       // 0x320, BSE_Sleep
    EffectEntry* fireEff;                        // 0x324
    EffectEntry* iceEff;                         // 0x328
    EffectEntry* sparkEff;                       // 0x32C
    struct IconEntry* kageIcon;                  // 0x330, BSE_Kagegakure
    u16 mBurnDamageFlameState;                   // 0x334
    u16 mbBurnDamageFlameActive;                 // 0x336
    s32 mBurnDamageFlameAnimTimer;               // 0x338
    f32 mBurnDamageFlameScale;                   // 0x33C
    EffectEntry* mBurnDamageFlameEff;            // 0x340
    BattleWorkStatusIcon mStatusIconWork;        // 0x344
    StChgInfo statusChangeInfo[6];               // 0xAE4, BattleStatusChangeInfoWorkInit
    s32 stageObjectId;                           // 0xB2C
} BattleUnit;

typedef struct BattleUnitKind {
    BattleUnitType mUnitType;                 // 0x0
    const char* name;                         // 0x4
    s16 maxHp;                                // 0x8
    s16 maxFp;                                // 0xA
    u8 mDangerHP;                             // 0xC
    u8 mPerilHP;                              // 0xD
    u8 mLevel;                                // 0xE
    u8 mBonusExp;                             // 0xF
    u8 bonusCoin;                             // 0x10
    u8 bonusCoinRate;                         // 0x11
    u8 baseCoin;                              // 0x12
    u8 runRate;                               // 0x13
    u16 pbSoftCap;                            // 0x14
    s16 width;                                // 0x16
    s16 height;                               // 0x18
    s16 hitOffset[2];                         // 0x1A
    Vec centerOffset;                         // 0x20
    s16 hpGaugeOffset[2];                     // 0x2C
    Vec talkTogeBaseOffset;                   // 0x30
    Vec heldItemBaseOffset;                   // 0x3C
    Vec burnFlameOffset;                      // 0x48
    f32 vivianOffsetX;                        // 0x54, _get_vivian_attack_position: x offset (times facing and scale x)
    f32 vivianOffsetZ;                        // 0x58, _get_vivian_attack_position: z offset (times scale z)
    Vec bintaHitOffset;                       // 0x5C
    Vec kissHitOffset;                        // 0x68
    Vec cutBaseOffset;                        // 0x74
    f32 cutWidth;                             // 0x80
    f32 cutHeight;                            // 0x84
    s8 turnOrder;                             // 0x88
    s8 turnOrderVariance;                     // 0x89
    s8 swallowChance;                         // 0x8A
    u8 swallowAttributes;                     // 0x8B
    u8 hammerKnockbackChance;                 // 0x8C
    u8 itemStealParam;                        // 0x8D
    Vec starPointDispOffset;                  // 0x90
    const char* damageSfxName;                // 0x9C
    const char* fireDamageSfxName;            // 0xA0
    const char* iceDamageSfxName;             // 0xA4
    const char* explosionDamageSfxName;       // 0xA8
    u32 attributeFlags;                       // 0xAC
    StatusVulnerability* statusVulnerability; // 0xB0
    s8 numParts;                              // 0xB4
    BattleUnitKindPart* mParts;               // 0xB8
    void* initEventCode;                      // 0xBC
    BattleDataEntry* dataTable;               // 0xC0
} BattleUnitKind;

typedef struct BattleUnitSetup {
    BattleUnitKind* mUnitKindParams; // 0x0
    s8 alliance;                     // 0x4, -> BattleUnit.mAlliance (BtlUnit_Entry); 2 for stage objects
    u8 pad_05[3];                    // 0x5, never read
    s32 attackPhase;                 // 0x8, -> BattleUnit.attackPhase
    Vec position;                    // 0xC, -> unit pos/offset pos/home pos
    s32 hitOffsetX;                  // 0x18, -> every BattleUnitPart.hitOffsetX
    s32 unitWork[4];                 // 0x1C, -> BattleUnit.work[0..3]
    ItemDropData* itemDropTable;     // 0x2C
} BattleUnitSetup;

BOOL BtlUnit_Init(void);
BattleUnit* BtlUnit_Entry(BattleUnitSetup* setup);
BOOL BtlUnit_Delete(BattleUnit* unit);
BattleUnit* BtlUnit_Spawn(BattleUnitSetup* setup, s32 flags);
s32 BtlUnit_GetUnitId(BattleUnit* unit);
BattleUnitPart* BtlUnit_GetPartsPtr(BattleUnit* unit, s32 partNum);
s32 BtlUnit_GetBodyPartsId(BattleUnit* unit);
void BtlUnit_GetPos(BattleUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetPos(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddPos(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsPos(BattleUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetPartsPos(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsPos(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsWorldPos(BattleUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_GetHitPos(BattleUnit* unit, BattleUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetHitOffset(BattleUnit* unit, BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetHitCursorOffset(BattleUnit* unit, BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_GetHomePos(BattleUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetHomePos(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddHomePos(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsHomePos(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetRotate(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetRotate(BattleUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_AddRotate(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsRotate(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsRotate(BattleUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_AddPartsRotate(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetBaseRotate(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetBaseRotate(BattleUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetPartsBaseRotate(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsBaseRotate(BattleUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetRotateOffset(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsRotateOffset(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsRotateOffset(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetBaseScale(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetScale(BattleUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetScale(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddScale(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsBaseScale(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsScale(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsScale(BattleUnitPart* part, f32 x, f32 y, f32 z);
s32 BtlUnit_GetWidth(BattleUnit* unit);
s32 BtlUnit_GetHeight(BattleUnit* unit);
void BtlUnit_SetHeight(BattleUnit* unit, s16 height);
void BtlUnit_SetOffsetPos(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsOffsetPos(BattleUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetPartsOffsetPos(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsOffsetPos(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetDispOffset(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsDispOffset(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsDispOffset(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetMoveStartPos(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsMoveStartPos(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetMoveCurrentPos(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsMoveCurrentPos(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetMoveTargetPos(BattleUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsMoveTargetPos(BattleUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetFallAccel(BattleUnit* unit, f32 fallAccel);
void BtlUnit_SetPartsFallAccel(BattleUnitPart* part, f32 fallAccel);
void BtlUnit_SetMoveSpeed(BattleUnit* unit, f32 moveSpeedXZ);
void BtlUnit_SetPartsMoveSpeed(BattleUnitPart* part, f32 moveSpeedXZ);
void BtlUnit_SetJumpSpeed(BattleUnit* unit, f32 moveSpeedY);
s8 BtlUnit_GetBelong(BattleUnit* unit);
void BtlUnit_GetStatus(BattleUnit* unit, StatusEffectType type, s8* turns, s8* strength);
BOOL BtlUnit_SetStatus(BattleUnit* unit, StatusEffectType type, s8 turns, s8 strength);
void BtlUnit_ClearStatus(BattleUnit* unit);
BOOL BtlUnit_CheckRecoveryStatus(BattleUnit* unit, StatusEffectType type);
BOOL BtlUnit_CheckStatus(BattleUnit* unit, StatusEffectType type);
s32 BtlUnit_CheckStatusFlag(BattleUnit* unit, s32 flags);
void BtlUnit_OnStatusFlag(BattleUnit* unit, s32 flags);
void BtlUnit_OffStatusFlag(BattleUnit* unit, s32 flags);
s32 BtlUnit_CheckUnitFlag(BattleUnit* unit, s32 flags);
void BtlUnit_OnUnitFlag(BattleUnit* unit, s32 flags);
void BtlUnit_OffUnitFlag(BattleUnit* unit, s32 flags);
char* BtlUnit_GetPoseNameFromType(BattleUnitPart* part, s32 type);

void BtlUnit_CheckPinchStatus(BattleUnit* unit, BOOL noAudience);

void BtlUnit_SetParamToPouch(BattleUnit* unit);

void BtlUnit_ResetMoveStatus(BattleUnit* unit);

s32 BtlUnit_GetFp(BattleUnit* unit);
void BtlUnit_SetFp(BattleUnit* unit, s32 value);
s32 BtlUnit_GetMaxFp(BattleUnit* unit);
void BtlUnit_SetMaxFp(BattleUnit* unit, s32 value);
void BtlUnit_RecoverHp(BattleUnit* unit, s32 value);
void BtlUnit_RecoverFp(BattleUnit* unit, s32 value);
s32 BtlUnit_GetHitDamage(BattleUnit* unit);
s32 BtlUnit_GetTotalHitDamage(BattleUnit* unit);
void* BtlUnit_GetData(BattleUnit* unit, s32 tag);
BOOL BtlUnit_CheckData(BattleUnit* unit, s32 tag);
s32 BtlUnit_GetEnemyBelong(BattleUnit* unit);
void BtlUnit_ReviseHpFp(BattleUnit* unit);
void BtlUnit_SetParamFromPouch(BattleUnit* unit);
s32 BtlUnit_snd_se(BattleUnit* unit, const char* sfxName, s32 volume, s32 pan);
BattleUnit* BtlUnit_GetGuardKouraPtr(BattleUnit* unit);

BOOL BtlUnit_CanActStatus(BattleUnit* unit);
void BtlUnit_SetAnimType(BattleUnitPart* part, s32 type);
void BtlUnit_SetBodyAnim(BattleUnit* unit, const char* name);
void BtlUnit_SetBodyAnimType(BattleUnit* unit, s32 type);
BOOL BtlUnit_CanGuardStatus(BattleUnit* unit);
void BtlUnit_ChangeStayAnim(s32 unitId);
void BtlUnit_ChangeTalkAnim(s32 unitId);
BOOL BtlUnit_GetACPossibility(BattleUnit* unit);
s32 BtlUnit_GetCoin(BattleUnit* unit);
s32 BtlUnit_GetExp(BattleUnit* unit);
void BtlUnit_GetTalkTogePos(s32 unitId, f32* x, f32* y, f32* z);
void BtlUnit_HpGaugeMain(BattleUnit* unit);
void BtlUnit_LoadSeMode(s32 mode, s32 kind, BtlPoseSoundTable* table, BecSndWork* work);
void BtlUnit_SetCommandAnimPose(BattleUnit* unit);
void BtlUnit_SetSeMode(BecSndWork* work, const char* name0, const char* name1, s16 c, s16 e, s16 unused, s16 a, s16 b);
BOOL BtlUnit_CheckShadowGuard(BattleUnit* unit);
BOOL BtlUnit_EnemyItemCanUseCheck(s32 item);
void BtlUnit_SetTotalHitDamage(BattleUnit* unit, s16 damage);
