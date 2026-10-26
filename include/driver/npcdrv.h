#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include "battle/battle_unit.h"
#include "mario/mario_pouch.h"
#include "evt/evt_word.h"

#define NPC_FIELD_MAX_COUNT 36
#define NPC_BATTLE_MAX_COUNT 16

typedef enum NpcTerritoryType {
    NPC_TERRITORY_TYPE_NOTHING = 0,
    NPC_TERRITORY_TYPE_CIRCLE = 1,
    NPC_TERRITORY_TYPE_SQUARE = 2
} NpcTerritoryType;

typedef struct NpcTerritoryInfo {
    const char* description; // 0x0
    NpcTerritoryType type;   // 0x4
} NpcTerritoryInfo;

typedef struct NpcDeadInfo {
    char mapName[16]; // 0x0
    s32 mask;         // 0x10
} NpcDeadInfo;

typedef struct PointDropData {
    s32 maxStatPercent;    // 0x0, 0 terminates the table
    s32 overallDropChance; // 0x4
    s32 numDrops;          // 0x8
    s32 dropChance;        // 0xC
} PointDropData;

typedef struct BattleGroupSetup {
    s32 numEnemies;             // 0x0
    BattleUnitSetup* enemyData; // 0x4
    s32 heldItemWeight;         // 0x8
    s32 randomItemWeight;       // 0xC
    s32 noItemWeight;           // 0x10
    PointDropData* hpDropTable; // 0x14
    PointDropData* fpDropTable; // 0x18
    s32 reserved_1C;            // 0x1C, never read
} BattleGroupSetup;

typedef struct NpcBattleSelect {
    s32 weight;                  // 0x0, 0 terminates the table
    BattleGroupSetup* groupSetup; // 0x4
    struct BattleStageData* stageData; // 0x8
} NpcBattleSelect;

// NpcSetupBattleInfo.battleFlags / NpcBattleInfo.battleFlags
typedef enum NpcBattleFlags {
    NPC_BATTLE_NO_COIN_DROP = (1 << 0),               // 0x1
    NPC_BATTLE_NO_HEART_DROP = (1 << 1),              // 0x2
    NPC_BATTLE_NO_FLOWER_DROP = (1 << 2),             // 0x4
    NPC_BATTLE_NO_ITEM_DROP = (1 << 3),               // 0x8
    NPC_BATTLE_NO_ESCAPE = (1 << 4),                  // 0x10, Run Away disabled; the stage walls close
    NPC_BATTLE_MERLEE_SAFE_CURSE_UNUSED = (1 << 5),   // 0x20, Merlee never rolls curse type 4; nothing sets it
    NPC_BATTLE_NO_GAME_OVER = (1 << 6),               // 0x40, losing leaves Mario at 1 HP instead
    NPC_BATTLE_EXTRA_COINS = (1 << 8),                // 0x100, coin multiplier + 1
    NPC_BATTLE_NO_PARTY_ENTRY = (1 << 20),            // 0x100000, Mario and the partner skip their entry events
    NPC_BATTLE_NO_JUMP = (1 << 24),                   // 0x1000000, Jump command disabled
    NPC_BATTLE_NO_HAMMER = (1 << 25),                 // 0x2000000, Hammer command disabled
    NPC_BATTLE_NO_PARTY_CHANGE = (1 << 26),           // 0x4000000, partner change disabled
    NPC_BATTLE_NO_BUMP_ATTACK = (1 << 28),            // 0x10000000, Bump Attack cannot win the battle at once
    NPC_BATTLE_STORY = (1 << 29),                     // 0x20000000, story fight: Mario's other end-of-battle voices
} NpcBattleFlags;

typedef struct NpcSetupBattleInfo {
    const char* name;           // 0x0
    const char* mapName;        // 0x4, never read; map the battle is set on
    ItemType fixedItem;         // 0x8
    s32 swFlag;                 // 0xC
    NpcBattleSelect* altSelect; // 0x10, used when swFlag is set
    NpcBattleSelect* select;    // 0x14
    s32 battleFlags;            // 0x18, NpcBattleFlags, -> NpcBattleInfo.battleFlags
    s32 randomMode;             // 0x1C, -> NpcBattleInfo.randomMode; 0 rolls randomValues from randomRanges
    s8 randomRanges[16][2];     // 0x20, {min, max} per NpcBattleInfo.randomValues byte
    const char* bgmName;        // 0x40, -> NpcBattleInfo.bgmName, played by seq_battle
} NpcSetupBattleInfo;

// audience/reward rolls made when the battle is set up (npcSetupBattleInfo), read by BattleAudience
typedef struct NpcBattleRandom {
    s32 mode;        // 0x0, from NpcSetupBattleInfo.randomMode
    s8 values[16];   // 0x4, rolled from NpcSetupBattleInfo.randomRanges (read as s8 weights)
} NpcBattleRandom;

typedef struct NpcBattleInfo {
    struct BattleStageData* stageData; // 0x0
    s32 battleFlags;                   // 0x4, NpcBattleFlags
    ItemType fixedItem;                // 0x8
    ItemType itemDropped;              // 0xC
    s32 heartsDropped;                 // 0x10
    s32 flowersDropped;                // 0x14
    BattleGroupSetup* groupSetup;      // 0x18
    ItemType heldItems[8];             // 0x1C
    ItemDropData* itemDropLists[8];    // 0x3C
    ItemType stolenItems[8];           // 0x5C
    ItemType recoveredItems[8];        // 0x7C
    union {
        s32 btlSetupWork[10];          // 0x9C
        struct {
            s32 scriptWork[4];         // 0x9C, btlSetupWork[0..3] (npcGet/SetBtlSetupWork)
            NpcBattleRandom random;    // 0xAC, btlSetupWork[4..8]
            const char* bgmName;       // 0xC0, btlSetupWork[9], battle BGM played by seq_battle
        };
    };
    u8 battleRule[3];                  // 0xC4
    u8 pad_C7;                         // 0xC7, never accessed
} NpcBattleInfo;

typedef struct NpcTribeInfo {
    const char* nameJp;           // 0x0
    const char* modelName;        // 0x4
    const char* initialAnim;      // 0x8
    const char* stopAnimation;    // 0xC
    const char* stayAnim;         // 0x10
    const char* talkAnim;         // 0x14
    const char* walkAnimation;    // 0x18
    const char* runAnimation;     // 0x1C
    const char* damageAnimation;  // 0x20
    const char* confuseAnimation; // 0x24
    f32 width;                    // 0x28
    f32 height;                   // 0x2C
    f32 shadowOffsetX;            // 0x30
    f32 shadowOffsetY;            // 0x34
    f32 shadowOffsetZ;            // 0x38
    f32 runStartSpeed;            // 0x3C
    const char* footstepSfxA;     // 0x40
    const char* footstepSfxB;     // 0x44
    u16 footstepStride;           // 0x48
    const char* jumpSfx;          // 0x4C
    const char* landSfx;          // 0x50
} NpcTribeInfo;

// NpcEntry.flags (also NpcSetupInfo.flags and NpcAiInfo.flags, copied in), named from npcMain, the npcdrv
// helpers and the original AI type names in npc_data.c. 0x1000 and 0x2000 (wall probes through hitCheckAttr(4))
// are not named.
typedef enum NpcFlags {
    NPC_FLAG_IN_USE = (1 << 0),                // 0x1, slot in use (npcEntry; npcDelete clears it)
    NPC_FLAG_READY = (1 << 1),                 // 0x2, initialised: updated, drawn, talkable, hittable
    NPC_FLAG_RECORD_DEFEAT = (1 << 2),         // 0x4, _npcDead records it in the map's dead mask (no respawn)
    NPC_FLAG_ENEMY = (1 << 3),                 // 0x8, field enemy: not talkable, enemy side of fbatHitCheck
    NPC_FLAG_FLOOR_COLLISION = (1 << 4),       // 0x10, npcMoveG lands it on the floor
    NPC_FLAG_NO_HIT = (1 << 5),                // 0x20, intangible: no push, no field hit check, not blown
    NPC_FLAG_NO_CULL = (1 << 6),               // 0x40, drawn without the view volume check
    NPC_FLAG_NO_DRAW = (1 << 7),               // 0x80, model, shadow and blur not drawn
    NPC_FLAG_BLUR = (1 << 8),                  // 0x100, motion blur (npcBlurOn)
    NPC_FLAG_TALK_POSE = (1 << 9),             // 0x200, uses its talk/stay animations in conversations
    NPC_FLAG_AUTO_MOVE_ANIM = (1 << 10),       // 0x400, stay/walk/run animation picked from its speed
    NPC_FLAG_HIDE = (1 << 11),                 // 0x800, whole display skipped (npc_hide_onoff)
    NPC_FLAG_NO_PUSH_MARIO_UNUSED = (1 << 14),        // 0x4000, does not push Mario out of its body (no code sets it)
    NPC_FLAG_INSTANT_TURN = (1 << 15),         // 0x8000, paper turn snaps (pose flag 0x100)
    NPC_FLAG_WALL_BOUNCE = (1 << 16),          // 0x10000, move direction mirrored at walls
    NPC_FLAG_GRAVITY = (1 << 17),              // 0x20000, falls
    NPC_FLAG_NO_SHADOW = (1 << 18),            // 0x40000
    NPC_FLAG_SHADOW_NO_FLOOR = (1 << 19),      // 0x80000, shadow not projected to the floor
    NPC_FLAG_UPDATE_BY_EVENT = (1 << 20),      // 0x100000, movement step only when an evt_npc move function asks
    NPC_FLAG_NO_FIND = (1 << 21),              // 0x200000, no find/lost/return interrupts
    NPC_FLAG_RESUME_AFTER_TALK = (1 << 22),    // 0x400000, resumes its stopped event after the talk-range pause
    NPC_FLAG_LUIGI_POSE_FLIP = (1 << 23),      // 0x800000, set by npcEntry for c_luigi
    NPC_FLAG_NO_TALK_TURN = (1 << 24),         // 0x1000000, does not turn to face Mario
    NPC_FLAG_NO_VIEW_CORRECTION = (1 << 25),   // 0x2000000, no camera-angle turn in mtx_setup
    NPC_FLAG_VIVIAN_TYPE = (1 << 26),          // 0x4000000, animPoseGetVivianType draw path
    NPC_FLAG_2D = (1 << 27),                   // 0x8000000, Bowser side-scrolling NPC
    NPC_FLAG_STOP_FOR_EVENT = (1 << 28),       // 0x10000000, frozen by npcStopForEvent (set with battle info)
    NPC_FLAG_NO_PLAYER_ATTACK = (1 << 29),     // 0x20000000, Mario cannot hit it; its own attack still lands
    NPC_FLAG_NO_WALL_COLLISION = (1 << 30),    // 0x40000000
    NPC_FLAG_NO_TALK_BALLOON = (1 << 31),      // 0x80000000, Mario's talk balloon not shown
    NPC_FLAG_PASS_NPC_ONLY_WALLS = (1 << 13),   // 0x2000, wall probes skip HIT_ATTR_NPC_ONLY surfaces
} NpcFlags;

// NpcEntry.statusFlags (evt_npc_status_onoff / evt_npc_status_check, npcMain, npcMoveG)
typedef enum NpcStatusFlags {
    NPC_STATUS_AIRBORNE = (1 << 0),        // 0x1, npcMoveG: in the air (cleared on landing)
    NPC_STATUS_FACE_MOVE_DIR = (1 << 1),   // 0x2, faces its move direction (scripts)
    NPC_STATUS_DEAD = (1 << 2),            // 0x4, _npcDead; every hit check skips it
    NPC_STATUS_UPDATE = (1 << 4),          // 0x10, run the movement step this frame (evt_npc move functions)
    NPC_STATUS_NO_FIND_ONCE = (1 << 12),   // 0x1000, no find/lost/return interrupt for one frame
    NPC_STATUS_STOPPED = (1 << 16),        // 0x10000, npcStopForOneEvent .. npcStartForOneEvent
    NPC_STATUS_IN_WATER = (1 << 20),       // 0x100000
    NPC_STATUS_WAS_IN_WATER = (1 << 21),   // 0x200000
    NPC_STATUS_DARKEN = (1 << 22),         // 0x400000, colour halved (2D deaths by fire or magma)
    NPC_STATUS_NO_WATER_DRAG = (1 << 28),  // 0x10000000
    NPC_STATUS_TALK_PAUSE = (1 << 29),     // 0x20000000, stopped for the talk-range pause
} NpcStatusFlags;

// NpcEntry.interruptFlags: requests to replace the regular event, handled by npcCheckInterrupt
typedef enum NpcInterruptFlags {
    NPC_INTERRUPT_FIND = (1 << 0),     // 0x1, Mario found
    NPC_INTERRUPT_LOST = (1 << 1),     // 0x2, Mario lost
    NPC_INTERRUPT_RETURN = (1 << 2),   // 0x4, outside the territory: go back
    NPC_INTERRUPT_BLOW = (1 << 3),     // 0x8, npcCheckBlow
    NPC_INTERRUPT_DEAD = (1 << 4),     // 0x10, _npcDead
    NPC_INTERRUPT_NO_FIND = (1 << 8),  // 0x100, no find event; set at the end of a field battle
    NPC_INTERRUPT_KEPT = 0xF00,        // bits kept when the requests are handled
    NPC_INTERRUPT_MOVE = (1 << 28),    // 0x10000000, restart the regular (move) event
} NpcInterruptFlags;

// How a Bowser-stage (2D) NPC was beaten: npcGroupDead's reason, kept in koopaEncounterType
// (evt_npc_get_kpencount_type, npcCalcScore). NpcEntry.hazardFlags holds the same bits as immunities.
typedef enum NpcKoopaEncount {
    KPA_ENCOUNT_NONE = 0,              // defeated otherwise (field battle, script)
    KPA_ENCOUNT_STOMP = (1 << 0),      // 0x1, jumped on by Bowser; NPCs in water are immune
    KPA_ENCOUNT_FIRE = (1 << 1),       // 0x2, Bowser's fire (combo score)
    KPA_ENCOUNT_STAR = (1 << 2),       // 0x4, touched while Bowser is invincible
    KPA_ENCOUNT_MOBJ = (1 << 3),       // 0x8, standing on a map object that bounced (npcKoopaModeMobjBoundDeadCheck)
    KPA_ENCOUNT_MAGMA = (1 << 4),      // 0x10, fell into magma ("a_magu"): no score
    KPA_ENCOUNT_GOAL_POLE = (1 << 5),  // 0x20, cleared when Bowser hits the goal pole
} NpcKoopaEncount;

typedef struct NpcEntry {
    s32 flags;                      // 0x0, NpcFlags
    u32 reactionFlags;              // 0x4
    char description[32];           // 0x8
    NpcTribeInfo* tribe;            // 0x28
    char currentAnim[32];           // 0x2C
    char stayAnim[32];              // 0x4C
    char talkAnim[32];              // 0x6C
    Vec position;                   // 0x8C, verified
    Vec prevPosition;               // 0x98
    Vec positionHistory[5];         // 0xA4
    Vec scale;                      // 0xE0
    Vec rotation;                   // 0xEC
    Vec rotationOffset;             // 0xF8
    s32 poseId;                     // 0x104
    s32 moveAnimState;              // 0x108, 1 stay, 2 walk, 3 run anim currently set by npcMain
    const char* checkPose;          // 0x10C, anim name, "M_I_2" by default (tik_02 geso_set_checkpose sets "M_S_1")
    s32 offscreenId;                // 0x110
    GXColor color;                  // 0x114
    s32 initEvtId;                  // 0x118
    s32 regularEvtId;               // 0x11C
    void* initEvent;                // 0x120, verified
    void* moveEvent;                // 0x124, verified
    void* talkEvt;                  // 0x128
    void* deadEvent;                // 0x12C, verified
    void* findEvent;                // 0x130, verified
    void* lostEvent;                // 0x134, verified
    void* returnEvent;              // 0x138, verified
    void* blowEvent;                // 0x13C, verified
    u32 interruptFlags;             // 0x140, NpcInterruptFlags
    f32 direction;                  // 0x144, angle we're facing at, verified
    f32 savedDirection;             // 0x148, saved during field battle
    f32 width;                      // 0x14C
    f32 height;                     // 0x150
    f32 shadowScale;                // 0x154
    Vec jumpStartPos;               // 0x158
    Vec jumpTargetPos;              // 0x164
    u8 pad_170[0x178 - 0x170];      // 0x170
    OSTime jumpTicks;               // 0x178, OSTime accumulated while jumping/falling, 0 on landing
    f32 jumpElapsed;                   // 0x180, jumpTicks in seconds
    u8 pad_184[0x188 - 0x184];      // 0x184
    OSTime moveTicks;               // 0x188, OSTime accumulated since the current evt move started
    OSTime dtTicks;                 // 0x190, frame delta (capped to 16 ms after a >500 ms gap)
    OSTime lastUpdateTicks;         // 0x198, gp->renderTime of the last update
    f32 moveElapsed;                // 0x1A0, moveTicks in seconds (compared with moveTime)
    f32 moveDt;                     // 0x1A4
    f32 lastUpdateTime;             // 0x1A8, lastUpdateTicks in seconds
    f32 moveDir;                    // 0x1AC
    f32 moveSpeed;                  // 0x1B0
    Vec moveOffset;                 // 0x1B4
    f32 moveTime;                   // 0x1C0
    f32 jumpVelY;                   // 0x1C4
    f32 jumpYScale;                 // 0x1C8
    f32 jumpGravity;                // 0x1CC
    f32 maxFallSpeed;               // 0x1D0, fall speed limit, 100 in kpa_03 water
    u32 statusFlags;                // 0x1D4, NpcStatusFlags (evt_npc_status_onoff)
    s16 footstepToggle;             // 0x1D8
    f32 footstepAccum;              // 0x1DC
    const char* footstepSfxA;       // 0x1E0
    const char* footstepSfxB;       // 0x1E4
    u16 footstepStride;             // 0x1E8
    const char* jumpSfx;            // 0x1EC
    const char* landSfx;            // 0x1F0
    s32 cameraId;                   // 0x1F4, CameraId
    NpcTerritoryType territoryType; // 0x1F8
    Vec territoryBase;              // 0x1FC, verified
    Vec territoryLoiter;            // 0x208, verified
    Vec territoryHoming;            // 0x214, verified
    f32 searchRange;                // 0x220, verified
    f32 searchAngle;                // 0x224, verified
    f32 homingRange;                // 0x228, verified
    f32 homingAngle;                // 0x22C, verified
    NpcBattleInfo battleInfo;       // 0x230
    s32 attackMode;                 // 0x2F8, evt_npc_set_attack_mode
    struct HitEntry* wallHit;       // 0x2FC, npcHitCheckSide result
    struct HitEntry* groundHit;     // 0x300, hit object found by the fall/landing checks
    void* unitWork[4];              // 0x304
    u8 setupIndex;                  // 0x314, slot in the setup list (dead-mask bit)
    u8 btlCoinDropCount;            // 0x315
    u8 baseCoinCount;               // 0x316
    s8 balloonType;                 // 0x317
    u32 fbatHitKind;                // 0x318, kind from fbatHitCheck when this NPC triggered the battle
    u16 koopaEncounterType;         // 0x31C, NpcKoopaEncount
    u16 hazardFlags;                // 0x31E, NpcKoopaEncount bits the NPC is immune to
    u16 wFbatHitCheckRelated;       // 0x320
    u8 pad_322[2];                  // 0x322
    struct NpcEntry* prev;          // 0x324
    struct NpcEntry* next;          // 0x328
    struct NpcEntry* master;        // 0x32C
    struct NpcEntry* slaves[4];     // 0x330
} NpcEntry;

typedef struct NpcFieldEntry {
    char mModelName[64];                // 0x0
    char mAnimName[64];                 // 0x40
    u32 mFlags;                         // 0x80
    f32 peraRotY;                       // 0x84, AnimPose fields saved by npcReleaseFiledNpc
    f32 rotationY;                      // 0x88
    f32 scaleZFlip;                     // 0x8C
    f32 peraTargetRotY;                 // 0x90
} NpcFieldEntry;

// NpcWork.flags
typedef enum NpcWorkFlags {
    NPC_WORK_ALWAYS_ENCOUNT = (1 << 0), // 0x1, talk and field-battle contact also while Mario cannot move
                                        // (gor_01 chusan1_alwaysencont)
    NPC_WORK_FIELD_RELEASED = (1 << 1), // 0x2, field NPC models released (evt_npc_release_filednpc)
} NpcWorkFlags;

typedef struct NpcWork {
    s32 count;            // 0x0, currently allocated
    s32 total;            // 0x4, maximum allowed
    s32 flags;            // 0x8, NpcWorkFlags
    NpcEntry* entries;    // 0xC
    NpcEntry* talkTarget; // 0x10, NPC that A would talk to this frame
} NpcWork;

// Field hit kinds: fbatHitCheck results, FieldBattleData.hitKind, fbatSetAttackAnnounce
typedef enum FbatHitKind {
    FBAT_HIT_NONE = 0,
    FBAT_HIT_TOUCH = 0x10000,           // plain contact: no first strike
    FBAT_HIT_JUMP = 0x20000,            // Mario first strikes ...
    FBAT_HIT_HIP = 0x40000,             // ... landing in MARIO_MOTION_HIP
    FBAT_HIT_JUMP_UNUSED = 0x80000,     // handled like a jump; nothing produces it
    FBAT_HIT_HAMMER = 0x100000,
    FBAT_HIT_SPIN_HAMMER = 0x200000,    // below hammer level 3
    FBAT_HIT_ULTRA_SPIN_HAMMER = 0x400000,
    FBAT_HIT_SHELL = 0x800000,          // the partner's shell (party member kind 2)
    FBAT_HIT_BOMB = 0x1000000,          // the partner's bomb (party member kind 3)
    FBAT_HIT_MARIO_FIRST = 0x1FE0000,   // every Mario first strike above
    FBAT_HIT_ENEMY_FIRST = 0x10000000,  // the enemy strikes first
} FbatHitKind;

// fbatHitCheck flags: which hits to look for
typedef enum FbatCheckFlags {
    FBAT_CHECK_CURRENT = (1 << 0),      // 0x1, hammer: where the swing is now (not the whole swing); party: only while attacking
    FBAT_CHECK_TOUCH = (1 << 4),        // 0x10, body contact; with it nothing is found while Mario is invincible
    FBAT_CHECK_JUMP = (1 << 5),         // 0x20
    FBAT_CHECK_HAMMER = (1 << 6),       // 0x40
    FBAT_CHECK_SPIN_HAMMER = (1 << 7),  // 0x80
    FBAT_CHECK_SHELL = (1 << 8),        // 0x100
    FBAT_CHECK_BOMB = (1 << 9),         // 0x200
    FBAT_CHECK_ENEMY = (1 << 12),       // 0x1000, field enemies (otherwise other NPCs)
    FBAT_CHECK_REACTION = (1 << 13),    // 0x2000, only NPCs with reactionFlags bit 0; no first-strike announce
} FbatCheckFlags;

typedef struct FirstStrikeInfo {
    BOOL enabled;        // 0x0
    u32 hitKind;            // 0x4, FbatHitKind, 0 when done
    u32 type;            // 0x8
    s32 timer;           // 0xC, frames into the announce slide (0..100)
    f32 x;               // 0x10, announce x, slides -600 -> 0 -> 600
    f32 y;               // 0x14, announce y
    const char* msg_tag; // 0x18
    GXColor color;       // 0x1C
} FirstStrikeInfo;

// FieldBattleInfo.firstAttack (BattleInformationSetFirstAttack, from the field hit kind in seq_battle)
typedef enum BattleFirstAttack {
    FIRST_ATTACK_NONE = 0,
    FIRST_ATTACK_JUMP = 1,
    FIRST_ATTACK_SPIN_JUMP = 2,     // marioWeapon_KururinJump (FBAT_HIT_HIP)
    FIRST_ATTACK_JUMP_UNUSED = 3,   // FBAT_HIT_JUMP_UNUSED
    FIRST_ATTACK_HAMMER = 4,
    FIRST_ATTACK_SPIN_HAMMER = 5,
    FIRST_ATTACK_ULTRA_HAMMER = 6,
    FIRST_ATTACK_SHELL = 7,         // the partner strikes first (partyWeapon_NokotarouFirstAttack)
    FIRST_ATTACK_BOMB = 8,          // partyWeapon_SandersFirstAttack
    FIRST_ATTACK_ENEMY = 9,         // and above: the enemy strikes first
} BattleFirstAttack;

typedef struct FieldBattleInfo {
    u32 mode; //0x0, unused
    BattleUnitType party; //0x4
    u32 firstAttack; //0x8, BattleFirstAttack
    NpcBattleInfo* setup; //0xC
    u32 result; //0x10
    u32 coins; //0x14, BattleStoreCoin
    u8 ruleType; //0x18, NpcBattleInfo.battleRule[0], index into the _rule_msg tables
    u8 ruleKeepResult; //0x19
    u8 ruleParam; //0x1A, battleRule[1], printed in the rule message
    u8 ruleParam2; //0x1B, battleRule[2], never read
} FieldBattleInfo;

typedef struct FieldBattleData {
    s16 mode;                    // 0x0
    u8 subState;                 // 0x2
    u8 pad_03;                   // 0x3, never accessed
    NpcEntry* npc;               // 0x4, encounter npc
    NpcEntry* target;            // 0x8
    s32 evtId;                   // 0xC
    s32 hitKind;                 // 0x10, FbatHitKind
    s32 firstAttackParam;        // 0x14, BattleFirstAttack of an enemy first strike (0x10 from tou_03 _firstattack_setting)
    u8 pad_18[0x1C - 0x18];      // 0x18, never accessed
    u32 firstAttackFlags;        // 0x1C, bit 1 cleared by tou_03 _firstattack_setting, not read in C
    FieldBattleInfo info;        // 0x20
    void* postBattleEvent;       // 0x3C, evt run after a won battle (battle_seq_end)
    s32 postBattleEventId;       // 0x40
    s32 postBattleMode;          // 0x44, nonzero: battle_seq_end skips to sequence 3 / 0x3000001
    u32 merleeEvtId;             // 0x48, _evt_majinai_coin_up_effect id
    NpcDeadInfo deadInfo[64];    // 0x4C
    u8 merleeCoinBonus;          // 0x54C, Merlee coin curse fired: coin drops x3
    u8 sandersBombTrigger;       // 0x54D
    u8 pad_54E[0x550 - 0x54E];   // 0x54E, never accessed
    FirstStrikeInfo firstStrike; // 0x550
    s32 battleWait;              // 0x570, frames seq_battle has waited for the first-strike announce (gives up at 200)
    s32 kpaComboCount;           // 0x574, Bowser stomp combo, score multiplier
    s32 kpaJumpKills;            // 0x578, Bowser kills during one jump
    s32 kpaJumpScore;            // 0x57C, score banked during one jump, shown on landing
} FieldBattleData;

NpcWork* npcGetWorkPtr(void);
void npcReleaseFiledNpc(void);
void npcRecoveryFiledNpc(void);
void npcInit(void);
void npcReset(BOOL inBattle);
s32 npcGetReactionOfLivingBody(BOOL inBattle);
s32 npcEntry(const char* description, const char* animName);
NpcTribeInfo* npcGetTribe(const char* tribeName);
void npcDelete(NpcEntry* entry);
void npcDeleteGroup(NpcEntry* entry);
void npcMain(void);
NpcEntry* npcNameToPtr(const char* name);
NpcEntry* npcNameToPtr_NoAssert(const char* name);
void npcSetLink(NpcEntry* npc, NpcEntry* other);
void npcSetBtlSetupWork(NpcEntry* npc, s32 index, s32 value);
s32 npcGetBtlSetupWork(NpcEntry* npc, s32 index);
void npcBlurOff(const char* name);
void npcBlurOn(const char* name);
void npcSetColor(const char* name, GXColor color);
void npcStartForOneEvent(NpcEntry* npc);
void npcStartForEvent(void);
void npcStopForOneEvent(NpcEntry* npc);
void npcStopForEvent(void);
BOOL npcCheckInterrupt(NpcEntry* npc);
void npcGroupDead(NpcEntry* npc, u16 reason);
f32 npcTransRytoFaceDir(NpcEntry* npc);

FieldBattleData* fbatGetPointer(void);

void npcSetSlave(NpcEntry* npc, NpcEntry* slave, s32 id);
void npcSetBattleInfo(NpcEntry* npc, s32 battleInfoId);
void fbatChangeMode(s16 mode);

NpcTerritoryType dbGetDefData(NpcTerritoryInfo* info, const char* description);

BOOL N_fbatPreventMarioEventChk(void);

extern EvtWord evt_majinai_effect_se_btl[];
void fbatSandersBombTriggerOn(void);

extern EvtWord _evt_majinai_coin_up_effect[];

s32 npcCalcScore(NpcEntry* npc);
void npcGetBackItemEntry(NpcEntry* npc);
void npcSetMarioAutoTalkPose(const char* stayPose, const char* talkPose);
void npcTuningRy(NpcEntry* npc, f32 angle);
void fbatSetAttackAnnounce(s32 hitKind);
void fbatSetAttackAnnounceEnable(void);
BOOL npcCheckBlow(NpcEntry* npc);
void npcClearDeadInfo(void);
void npcExecAllInitEvt(void);
void npcExecAllReglEvt(void);
BOOL npcWaitAllInitEvtEnd(void);
NpcEntry* npcNearDistCheck(f32 x, f32 y, f32 z, f32 range);
void npcSetStayPose(char* name);
void npcSetTalkPose(char* name);
NpcEntry* fbatHitCheckAll(void);
