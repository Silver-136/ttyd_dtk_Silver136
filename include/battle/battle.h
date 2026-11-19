// Declares the battle work structure, battle flags and the top-level battle functions.

#pragma once

#include "version.h"
#include <dolphin/types.h>
#include "battle/battle_ac.h"
#include "battle/battle_audience.h"
#include "battle/battle_break_slot.h"
#include "battle/battle_camera.h"
#include "battle/battle_database_common.h"
#include "battle/battle_icon.h"
#include "battle/battle_information.h"
#include "battle/battle_pad.h"
#include "battle/battle_seq_command.h"
#include "battle/battle_stage.h"
#include "battle/battle_stage_object.h"
#include "battle/battle_unit.h"
#include "battle/battle_seq_end.h"
#include "mario/mario_pouch.h"

// Battle texture id in each version's battle texture file: in US and later the ids of the textures that use it are
// 0x16 higher
#define BTL_TEX(id) ((id) + VERSION_PICK(0, 0x16))

typedef enum EquipFlags {
    EQUIP_RESET = 1 << 0,   // 0x1, clear/reset equipped badges
    EQUIP_MARIO = 1 << 1,   // 0x2, equip Mario (non-P) badges
    EQUIP_PARTNER = 1 << 2, // 0x4, equip Partner (P) badges
} EquipFlags;

void BtlUnit_EquipItem(BattleUnit* unit, EquipFlags flags, ItemType type);

typedef struct BattleAlliance BattleAlliance;

// BattleWork.flags, named from the code that sets and tests them (lecture bits: Frankly's tutorials, which commands
// stay selectable). Bits that are set but never read, or read but never set, carry _UNUSED.
typedef enum BattleFlags {
    BATTLE_FLAG_GUARD_MOVE = (1 << 0),            // 0x1, btlevtcmd_WaitGuardMove / GuardMoveEnd
    BATTLE_FLAG_FIRST_ATTACK = (1 << 1),          // 0x2, first-strike sequence
    BATTLE_FLAG_ESCAPED = (1 << 4),               // 0x10, btlevtcmd_InterruptStop type 0 (result 4)
    BATTLE_FLAG_INTERRUPT_STOP = (1 << 5),        // 0x20, btlevtcmd_InterruptStop type 1 (result 5)
    BATTLE_FLAG_DEMO_TIMEOUT = (1 << 6),          // 0x40, demo-mode idle timeout ends the battle
    BATTLE_FLAG_COMMAND_INPUT = (1 << 7),         // 0x80, command menu open (BattleCommandInit)
    BATTLE_FLAG_PARTY_IN_AUDIENCE = (1 << 8),     // 0x100, Hooktail negotiation: Mario's party in the audience area
    BATTLE_FLAG_JUMP_NO_SUBMENU = (1 << 9),       // 0x200, only the plain Jump: no weapon submenu
    BATTLE_FLAG_HAMMER_NO_SUBMENU = (1 << 10),    // 0x400, only the plain Hammer: no weapon submenu
    BATTLE_FLAG_NO_OBJECT_FALL = (1 << 12),       // 0x1000, BattleStage_ObjectFallCheck skipped
    BATTLE_FLAG_RESET_TURN = (1 << 13),           // 0x2000, btlevtcmd_reset_turn
    BATTLE_FLAG_MAJINAI_END = (1 << 22),          // 0x400000, BattleMajinaiEndCheck: Merlee's spells used up
    BATTLE_FLAG_LECTURE_NO_COMMAND = (1 << 23),   // 0x800000, every command and the position swap off
    BATTLE_FLAG_LECTURE_JUMP = (1 << 24),         // 0x1000000, only Jump (lecturer's weapon, timed command succeeds)
    BATTLE_FLAG_LECTURE_HAMMER = (1 << 25),       // 0x2000000, only Hammer
    BATTLE_FLAG_LECTURE_SPECIAL = (1 << 26),      // 0x4000000, only Special
    BATTLE_FLAG_LECTURE_FREE = (1 << 27),         // 0x8000000, free practice: Tactics replaced by Lecture End
    BATTLE_FLAG_LECTURE_ATTACK = (1 << 28),       // 0x10000000, attacks only (no items, special, tactics)
    BATTLE_FLAG_DISABLE_HRESET = (1 << 29),       // 0x20000000, battleDisableHResetCheck (Reset button counted)
    BATTLE_FLAG_ZERO_GRAVITY_UNUSED = (1 << 30),         // 0x40000000, zero-gravity battle (no code sets it)
    BATTLE_SEQ_END = (1 << 31),                   // 0x80000000
    BATTLE_FLAG_ACT_DECIDED_UNUSED = (1 << 2),    // 0x4, set when the command and target are fixed; no code tests it
    BATTLE_FLAG_FORCE_STATUS_ICONS_UNUSED = (1 << 16), // 0x10000, draw every status icon (cleared each frame; no code sets it)
    // 0x20000, draw a held-item icon on every unit; set only inside btlUnitItemDisp2, which runs only while the bit
    // is already set, so normal play never turns it on
    BATTLE_FLAG_FORCE_ITEM_ICONS_UNUSED = (1 << 17),
} BattleFlags;

typedef enum BattleSequence {
    SEQ_UNKNOWN = 0,
    SEQ_INIT = 1,
    SEQ_FIRST_ACT = 2,
    SEQ_TURN = 3,
    SEQ_PHASE = 4,
    SEQ_MOVE = 5,
    SEQ_ACT = 6,
    SEQ_END = 7
} BattleSequence;

// BattleSetSeq(wp, SEQ_UNKNOWN, ...): the battle's top-level stage (BattleSequenceManager).
enum {
    BATTLE_STAGE_INIT = 0,
    BATTLE_STAGE_FIRST_ACT = 1,
    BATTLE_STAGE_TURN = 2,
    BATTLE_STAGE_END = 3,
    BATTLE_STAGE_DONE = 4,
};

// SEQ_INIT steps (btlseqInit).
enum {
    SEQ_INIT_START = 0x1000000,
    SEQ_INIT_UNIT_SETUP = 0x1000001,
    SEQ_INIT_UNIT_INIT_WAIT = 0x1000002,
    SEQ_INIT_LOAD_ANIMS = 0x1000003,
    SEQ_INIT_PAUSE = 0x1000004,
    SEQ_INIT_FINISH = 0x1000005,
};

// SEQ_END steps (btlseqEnd).
enum {
    SEQ_END_START = 0x7000000,
    SEQ_END_LOSE_FADE = 0x7000001,
    SEQ_END_LOSE_FADE_WAIT = 0x7000002,
    SEQ_END_POST_EVENT_START = 0x7000003,
    SEQ_END_POST_EVENT_WAIT = 0x7000004,
    SEQ_END_WIN_CLEAR_STATUS = 0x7000005,
    SEQ_END_WIN_UNIT_EVENTS = 0x7000006,
    SEQ_END_EXP_UP_WAIT = 0x7000007,
    SEQ_END_POINTS_INIT = 0x7000008,
    SEQ_END_POINTS_DELAY = 0x7000009,
    SEQ_END_POINTS_SHOW = 0x700000A,
    SEQ_END_POINTS_COUNT = 0x700000B,
    SEQ_END_POINTS_HOLD = 0x700000C,
    SEQ_END_POINTS_HIDE = 0x700000D,
    SEQ_END_MAJINAI_END_START = 0x700000E,
    SEQ_END_MAJINAI_END_WAIT = 0x700000F,
    SEQ_END_LEVEL_UP_CHECK = 0x7000010,
    SEQ_END_LEVEL_UP_CURTAIN = 0x7000011,
    SEQ_END_LEVEL_UP_SELECT = 0x7000012,
    SEQ_END_LEVEL_UP_CONFIRM_UNUSED = 0x7000013,
    SEQ_END_LEVEL_UP_JUMP_WAIT = 0x7000014,
    SEQ_END_LEVEL_UP_APPLY = 0x7000015,
    SEQ_END_LEVEL_UP_SHOW = 0x7000016,
    SEQ_END_RANK_UP_START = 0x7000017,
    SEQ_END_RANK_UP_WAIT = 0x7000018,
    SEQ_END_VICTORY_POSE = 0x7000019,
    SEQ_END_VICTORY_POSE_WAIT = 0x700001A,
    SEQ_END_RUN_OFF = 0x700001B,
    SEQ_END_LEAVE = 0x700001C,
    SEQ_END_FADE_OUT = 0x700001D,
    SEQ_END_FADE_OUT_WAIT = 0x700001E,
    SEQ_END_CLEANUP = 0x700001F,
};

// BattleDebugCtrl.flags: debug switches that force a stage or audience effect
typedef enum BattleDebugFlags {
    BATTLE_DEBUG_FORCE_OBJECT_FALL = (1 << 0),        // 0x1
    BATTLE_DEBUG_FORCE_FALL_ON_AUDIENCE = (1 << 1),   // 0x2, a falling stage object lands on the audience
    BATTLE_DEBUG_FORCE_DESTROY_A1 = (1 << 4),         // 0x10
    BATTLE_DEBUG_FORCE_DESTROY_A2 = (1 << 5),         // 0x20
    BATTLE_DEBUG_FORCE_DESTROY_B = (1 << 6),          // 0x40
    BATTLE_DEBUG_FORCE_IRON_FRAME_FALL = (1 << 7),    // 0x80
    BATTLE_DEBUG_FORCE_NOZZLE_WORK = (1 << 8),        // 0x100
    BATTLE_DEBUG_FORCE_NOZZLE_DIR_CHANGE = (1 << 9),  // 0x200
    BATTLE_DEBUG_AUDIENCE_NUM = (1 << 16),            // 0x10000, the audience count is audienceNum
    BATTLE_DEBUG_ITEM_THROW = (1 << 17),              // 0x20000, the audience throws items far more often
    BATTLE_DEBUG_FORCE_LUIGI = (1 << 18),             // 0x40000, Luigi joins the audience (otherwise 1 in 20)
    BATTLE_DEBUG_SLOT_ITEM = (1 << 19),               // 0x80000, the slot item is slotItem
} BattleDebugFlags;

typedef struct BattleDebugCtrl {
    s32 flags;        // 0x0, BattleDebugFlags
    s32 pad_04;       // 0x4, never accessed
    s32 pad_08;       // 0x8, never accessed
    s32 audienceNum;  // 0xC, targetAudienceCount when flags & 0x10000
    s32 audienceKind; // 0x10, 1..: forced audience kind
    s32 slotItem;     // 0x14, item_appear_force when flags & 0x80000
} BattleDebugCtrl;

typedef struct BattleActRecord {   // u8 counters for the battle-condition rules (BtlActRec_*)
    u8 marioJumpMoves;         // 0x00, weapon class 0 (cond 1/2)
    u8 marioHammerMoves;       // 0x01, class 1 (cond 3/4)
    u8 marioSpecialAttacks;    // 0x02, class 4, attacking (cond 5/6)
    u8 marioSpecialSupports;   // 0x03, class 4, non-attacking
    u8 marioDamageTaken;       // 0x04, BtlActRec_AddPoint (cond 7/8)
    u8 partnerDamageTaken;     // 0x05, cond 9/10
    u8 marioHitsTaken;         // 0x06, cond 13/14
    u8 partnerHitsTaken;       // 0x07, cond 15/16
    u8 powerBounceCombo;       // 0x08, cond 21
    u8 marioAttackItems;       // 0x09, class 2, attacking (cond 22/23)
    u8 marioSupportItems;      // 0x0A, class 2, non-attacking
    u8 partnerAttackItems;     // 0x0B, cond 24/25
    u8 partnerSupportItems;    // 0x0C
    u8 marioPartyChanges;      // 0x0D, tactics 0 (cond 28/29)
    u8 partnerPartyChanges;    // 0x0E, cond 30/31
    u8 marioAudienceAttacks;   // 0x0F, cond 34/35
    u8 partnerAudienceAttacks; // 0x10, cond 36/37
    u8 marioAppeals;           // 0x11, tactics 5/7 (cond 38/39)
    u8 partnerAppeals;         // 0x12, cond 40/41
    u8 marioFpSpent;           // 0x13, cond 44/45
    u8 marioFpMoves;           // 0x14, cond 50/51
    u8 partnerFpSpent;         // 0x15, cond 46/47
    u8 partnerFpMoves;         // 0x16, cond 52/53
    u8 marioCharges;           // 0x17, tactics entry 1: Charge
    u8 partnerCharges;         // 0x18
    u8 marioSuperCharges;      // 0x19, tactics entry 2: Super Charge
    u8 partnerSuperCharges;    // 0x1A
    u8 marioEscapes;           // 0x1B, tactics entry 3: Escape
    u8 partnerEscapes;         // 0x1C
    u8 partnerAttackMoves;     // 0x1D, weapon class 6, attacking
    u8 partnerSupportMoves;    // 0x1E, class 6, non-attacking
    u8 turnCount;              // 0x1F, cond 58/59
} BattleActRecord;

/**
 * Mario's action command results in this battle, each counted up to 200 by btlevtcmd_AudienceDeclareACResult,
 * which updates both through one pointer to this pair.
 */
typedef struct BattleMarioAcCount {
    u8 success; // 0x0
    u8 failed;  // 0x1
} BattleMarioAcCount;

// StChgMsg.flags: whose status changed.
enum {
    ST_CHG_MSG_MARIO = 1,
    ST_CHG_MSG_PARTNER = 2,
    ST_CHG_MSG_OTHER = 4,
};

// BattleAlliance.clearFlags: BattleSeq checks them for clearConditionMet.
enum {
    ALLIANCE_CLEAR_NONE = 0,
    ALLIANCE_CLEAR_ALL_OUT = 1,    // every unit of the side is out
    ALLIANCE_CLEAR_LEADER_OUT = 2, // Mario's side: Mario is out
};

typedef struct StChgMsg {
    s8 type;     // 0x0, -1 = free
    u8 flags;    // 0x1
    u8 timer;    // 0x2, frames left on screen; a 0x300 press clears it
    s32 unitId;  // 0x4
    s32 changed; // 0x8, 1: strength changed / newly inflicted (selects msg1), as StChgInfo.changed
} StChgMsg;

typedef struct StChgWork {
    StChgMsg msg[28];     // 0x0
    char text[0x100];     // 0x150
    s32 maskHi;           // 0x250 (BattleAnnounce 0x368): 64-bit message mask, read as s64 (with the 0x100 text of US and later and the early demos it is only 4-aligned); all set when the luck event starts
    s32 maskLo;           // 0x254
} StChgWork;

typedef struct BattleAnnounce {
    s16 x;                           // 0x0
    s16 y;                           // 0x2
    s32 params[4];                   // 0x4
#if VERSION != VERSION_JP
    char text[0x100];                // 0x14 (4 shorter than JP)
#else
    char text[0x104];                // 0x14
#endif
    StChgWork stChg;                 // 0x118 (US 0x114), status-change message queue (battle_status_effect)
} BattleAnnounce;

/**
 * The action-command hint box (battle_ac_help.c). BattleAcHelpMain and _ac_help_disp read both fields
 * through one pointer to this pair.
 */
typedef struct BattleAcHelpWork {
    const char* msg; // 0x0, message id of the hint (the weapon's acHelpMsg)
    s32 dispType;    // 0x4, AC_HELP_DISP_OFF/XLU/ON
} BattleAcHelpWork;

struct BattleAlliance {
    u16 clearFlags;     // 0x0
    s8 attackDirection; // 0x2
    // pad 1 byte
    BOOL clearConditionMet; // 0x4
};

typedef struct BattleWorkPartyInfo {
    f32 sizeMultiplier;       // 0x0, saved BattleUnit.sizeMultiplier
    BtlStatusCopy statusTurns;     // 0x4, saved from BattleUnit.sleepTurns
    s32 statusFlags;          // 0x24, saved BattleUnit.statusFlags
#if VERSION >= VERSION_US
    u16 usCommandDisabled[2]; // US and later: saved BattleUnit usCommandDisabled
#endif
} BattleWorkPartyInfo;

typedef struct BattleWorkTarget {
    s16 unitIdx;           // 0x0
    s16 partIdx;           // 0x2
    s16 posX;              // 0x4, cursor point (_btlSamplingEnemy)
    s16 posY;              // 0x6
    s16 posZ;              // 0x8
    s32 x;                 // 0xC
    s32 y;                 // 0x10
    s32 z;                 // 0x14
    s32 score;             // 0x18
    u8 forwardLv;          // 0x1C
    s8 side;               // 0x1D
    s32 reserved_20;       // 0x20, only set to -1
} BattleWorkTarget;

typedef struct BattleWorkWeaponTargets {
    BattleWeapon* weapon;           // 0x0
    BattleWorkTarget targets[74];   // 0x4
    s8 numTargets;                  // 0xA6C
    s8 targetIndices[74];           // 0xA6D
    s8 currentTarget;               // 0xAB7
    s32 unitId;                     // 0xAB8, BattleSamplingEnemy args
    s8 belong;                      // 0xABC
    u32 classFlags;                 // 0xAC0
    u32 propFlags;                  // 0xAC4
    s8 direction;                   // 0xAC8
} BattleWorkWeaponTargets;

typedef struct BattlePhaseEvtEntry {
    s32 unitId;   // 0x0
    s32 priority; // 0x4
} BattlePhaseEvtEntry;

typedef struct BattleAfterReaction {
    s32 unitId; // 0x0, -1 = free slot
    s32 tag;    // 0x4
} BattleAfterReaction;

// BattleWork.badgeEquippedFlags: badges Mario has equipped (BattleInit, from the pouch)
typedef enum BattleBadgeFlags {
    BATTLE_BADGE_QUICK_CHANGE = (1 << 0),   // 0x1, ITEM_QUICK_CHANGE
    BATTLE_BADGE_PEEKABOO = (1 << 1),       // 0x2, ITEM_PEEKABOO
    BATTLE_BADGE_TIMING_TUTOR = (1 << 2),   // 0x4, ITEM_TIMING_TUTOR
    BATTLE_BADGE_HEART_FINDER = (1 << 3),   // 0x8, ITEM_HEART_FINDER
    BATTLE_BADGE_FLOWER_FINDER = (1 << 4),  // 0x10, ITEM_FLOWER_FINDER
    BATTLE_BADGE_MONEY_MONEY = (1 << 7),    // 0x80, ITEM_MONEY_MONEY
    BATTLE_BADGE_ATTACK_FX_R = (1 << 8),    // 0x100, ITEM_ATTACK_FX_R
    BATTLE_BADGE_ATTACK_FX_Y = (1 << 9),    // 0x200, ITEM_ATTACK_FX_Y
    BATTLE_BADGE_ATTACK_FX_B = (1 << 10),   // 0x400, ITEM_ATTACK_FX_B
    BATTLE_BADGE_ATTACK_FX_G = (1 << 11),   // 0x800, ITEM_ATTACK_FX_G
    BATTLE_BADGE_ATTACK_FX_P = (1 << 12),   // 0x1000, ITEM_ATTACK_FX_P
} BattleBadgeFlags;

struct BattleWork {
    s16 turnCount;                      // 0x0, battle turn, +1 per turn (0 at BattleInit)
    u16 turnSubSeq;                     // 0x2, sub-step of turn step 0x3000009 (start-of-turn Burn)
    s32 seqUnknownValue;                // 0x4, SEQ_UNKNOWN step (BATTLE_STAGE_*; BattleSetSeq / BattleGetSeq)
    BattleAlliance alliances[3];        // 0x8, per alliance (BattleUnit.alliance)
    BattleUnit* units[64];              // 0x20, by unitId (BattleGetUnitPtr); NULL = free
    s32 movePriorityQueue[64];          // 0x120, unit ids in turn order; -1 = none
    BattlePhaseEvtEntry phaseEvtTable[64]; // 0x220, units whose phase events run, in order
    s32 activeUnitId;                   // 0x420, unit acting now; -1 = none
    s32 lectureUnitId;                  // 0x424, unit id -6 resolves to (_battle_set_lecture_unit_id); its weapons replace the menu weapons, -1 = none
    BattleWorkWeaponTargets weaponTargets; // 0x428, targets of the weapon being aimed (battle_detect)
    u32 flags;                          // 0xEF4, BattleFlags
    s32 dispFlags;                      // 0xEF8, BATTLE_DISP_FLAG_*
    s32 ruleDispTimer;                  // 0xEFC, frames: first-act delay (0x2000001), rule display (0x3000004, 120)
    s32 stockExp;                       // 0xF00, BattleStoreExp/BattleGetStockExp
    s32 stockExpDisp;                   // 0xF04, counts up to stockExp in BattleStockExpDisp
    s32 stockExpTimer;                  // 0xF08, frame counter for that count-up
    s32 seqInitValue;                   // 0xF0C, SEQ_INIT step (BattleSetSeq / BattleGetSeq)
    s32 seqFirstActValue;               // 0xF10, SEQ_FIRST_ACT step
    s32 seqTurnValue;                   // 0xF14, SEQ_TURN step
    s32 seqPhaseValue;                  // 0xF18, SEQ_PHASE step
    s32 seqMoveValue;                   // 0xF1C, SEQ_MOVE step
    s32 seqActValue;                    // 0xF20, SEQ_ACT step
    s32 seqEndValue;                    // 0xF24, SEQ_END step
    BattleSeqEndWork* seqEndWork;       // 0xF28
    BattleWorkPad padWork[4];           // 0xF2C, input history of the four controllers (battle_pad)
    BattleCommand commandMenu;          // 0x171C
    BattleActions actionCommands;       // 0x1C90
    s32 stylishCurFrame;               // 0x271C, frames since BattleAcrobatStart
    s32 stylishWindowStart;            // 0x2720, the Stylish A press counts after this frame
    s32 stylishWindowEnd;              // 0x2724, the A press counts through this frame
    s32 stylishEndFrame;               // 0x2728, the check gives up at this frame
    s32 stylishUnitId;                 // 0x272C, unit doing the Stylish move
    s32 stylishResult;                 // 0x2730, STYLISH_RESULT_*
    s32 stylishEarlyFrames;            // 0x2734, pad history frames checked for an early press
    FieldBattleInfo* fbatInfo;          // 0x2738, field battle info passed to BattleInit
    s32 statusWindowOff;                // 0x273C, 1 keeps the status window closed (BattleStatusWindowCheck)
    s32 lectureMode;                    // 0x2740, 1 during a tutorial lecture: status window closed
    s32 statusWindowSystemOn;           // 0x2744, BattleStatusWindowSystemOn/Off
    s32 statusWindowEventOn;            // 0x2748, BattleStatusWindowEventOn/Off
    s32 statusWindowAPRecoveryOn;       // 0x274C, BattleStatusWindowAPRecoveryOn/Off
    s32 reserved_2750;                  // 0x2750, never accessed
    BattleCameraWork cameraWork;        // 0x2754
    BattleAudience audience;            // 0x2858
    BreakSlotWork breakSlot;            // 0x1616C, battle_break_slot.c
    BattleWorkPartyInfo partyInfo[7];   // 0x162A0, partner state kept while switched out (BtlUnit_SetParamToPouch)
    u32 tattledUnitTypeFlags[8];        // 0x163B8 (US 0x163D4), one bit per unit kind tattled (battle_monosiri)
    s32 badgeEquippedFlags;             // 0x163D8 (US 0x163F4), BattleBadgeFlags
    FileEntry* menuTex;                 // 0x163DC (US 0x163F8), battle menu texture file (battle_menu_disp)
    BattleStage stage;                  // 0x163E0 (US 0x163FC)
    BattleActRecord actRecord;          // 0x16F1C (US 0x16F38), battle condition counters (battle_actrecord)
    BattleMarioAcCount marioAc;         // 0x16F3C (US 0x16F58)
    u8 pad_16F3E[0x16F40 - 0x16F3E];    // 0x16F3E (US 0x16F5A), never accessed
    BattleAfterReaction afterReactions[64]; // 0x16F40 (US 0x16F5C), BattleAfterReactionEntry/Release/Main queue
    BattleStageObject stageObjects[32]; // 0x17140 (US 0x1715C)
    BattleStageObjectWork stageObjectWork; // 0x180C0 (US 0x180DC), battle_stage_object.c (flags: BattleStageFlags)
    BattleIcon icons[16];               // 0x182B0 (US 0x182CC)
    BattleAnnounce announce;            // 0x18C70 (US 0x18C8C)
    s8 alertTick;                       // 0x18FE0 (US 0x18FF8), frames to the next danger/peril alarm sound (60)
    u8 majinaiState;                    // 0x18FE1 (US 0x18FF9), MajinaiType of the Merlee charm in effect
    u16 luckTimer;                      // 0x18FE2 (US 0x18FFA), frames of the Lucky Start event, up to 300
    StarPowerInfo impendingBonuses;     // 0x18FE4 (US 0x18FFC), Star Power bonus for the last action command
    BattleAcHelpWork acHelp;            // 0x18FF0 (US 0x19008), battle_ac_help.c
    u8 pad_18FF8[0x19038 - 0x18FF8];    // 0x18FF8 (US 0x19010), never accessed
#if VERSION >= VERSION_US
    s32 commandDisabled[3]; // US and later
#else
    s32 commandDisabled[5];             // 0x19038 (US 0x19050), turns blocked by Bowser's attacks (3, -1 a turn)
#endif
    s32 luckEventId;                    // 0x1904C (US 0x1905C), id of the running Lucky Start event
    s32 reserveItems[4];                // 0x19050 (US 0x19060), pouch items to remove (BattleConsumeReserveItem)
    u32 curtainSfxId;                    // 0x19060 (US 0x19070); u32: its -1 tests are unsigned in the retail code
    u8 lastAcSuccess;                   // 0x19064 (US 0x19074), 1 after a successful action command (btlevtcmd_AudienceDeclareACResult)
    u8 pad_19065[0x19068 - 0x19065];    // 0x19065 (US 0x19075), never accessed
    BattleDebugCtrl debug;              // 0x19068 (US 0x19078), debug overrides read by the audience and break slot
    u32 lectureFlags;                   // 0x19080 (US 0x19090), bit 0 set by the Frankly lecture (aud_attack_init/end); continues msgWindow lectureWindowId
    s32 lectureWindowId;                // 0x19084 (US 0x19094)
};
extern BattleWork* _battleWorkPointer;

void battleMain(void);
void battleInit(void);
void battleEnd(void);

void BattleAfterReactionQueueInit(void);

BattleUnit* BattleGetPartnerPtr(BattleWork* work, BattleUnit* unit);
BattleUnit* BattleGetPartyPtr(BattleWork* wp);
BattleUnit* BattleGetMarioPtr(BattleWork* work);
BattleUnit* BattleGetSystemPtr(BattleWork* work);
BattleUnitPart* BattleGetUnitPartsPtr(s32 index, s32 partNum);
void BattleSetUnitPtr(BattleWork* wp, s32 index, BattleUnit* unit);
BattleUnit* BattleGetUnitPtr(BattleWork* wp, s32 index);
void BattleFree(void* ptr);
void* BattleAlloc(u32 size);

// BTL_ALLOC/BTL_FREE: US and later route these battle-heap sites through BattleAlloc/BattleFree; JP and the early
// demos call the heap directly.
#if VERSION >= VERSION_US
#define BTL_ALLOC(size) BattleAlloc(size)
#define BTL_FREE(ptr) BattleFree(ptr)
#else
#define BTL_ALLOC(size) __memAlloc(HEAP_BATTLE, size)
#define BTL_FREE(ptr) __memFree(HEAP_BATTLE, ptr)
#endif

void BattleIncSeq(BattleWork* wp, BattleSequence seq);
s32 BattleGetSeq(BattleWork* wp, BattleSequence seq);
f32 BattleGetFloorHeight(BattleWork* wp, f32 x, f32 y, f32 z);
void BattleSetSeq(BattleWork* wp, BattleSequence seq, u32 value);
void BattleSetMarioParamToFieldBattle(BattleWork* work);
BOOL Btl_UnitSetup(BattleWork* work);
void BattleEnd(void);
void BattleInit(FieldBattleInfo* info);
BOOL battleSeqEndCheck(void);

struct BattleUnit* BattleChangeParty(BattleWork* wp);
s32 BattleTransPartyId(BattleUnitType kind);

BOOL battleDisableHResetCheck(void);

void BattleConsumeReserveItem(void);
s32 BattleGetStockExp(BattleWork* wp);
void BattleMajinaiDone(void);
void BattleMajinaiEndCheck(void);
void BattleStatusWindowAPRecoveryOff(void);
void BattleStatusWindowEventOn(void);
void BattleStatusWindowSystemOff(void);
void BattleStatusWindowSystemOn(void);
void BattleAfterReactionEntry(s32 unitId, s32 tag);
BOOL BattleAfterReactionMain(void);
void BattleStatusWindowAPRecoveryOn(void);
void BattleStatusWindowEventOff(void);
void BattleStoreCoin(BattleWork* wp, s32 coin);
void BattleStoreExp(BattleWork* wp, s32 exp);
BattleUnitType BattleTransPartyIdToUnitKind(s32 partyId);
u8 BattleMajinaiCheck(void);
BOOL BattleStatusWindowCheck(void);
BOOL BattlePartyAnimeLoad(s32 partyId);

// Names from LLCOOLT's tree.
enum {
    BATTLE_DISP_FLAG_COMMAND_MENU = 1,
    BATTLE_DISP_FLAG_STAR_POINTS = 2,
};

enum {
    LECTURE_FLAG_CONTINUE_WINDOW = 1,
};
#define BATTLE_STAGE_OBJECT_MAX 32
#define BATTLE_UNIT_MAX 64
