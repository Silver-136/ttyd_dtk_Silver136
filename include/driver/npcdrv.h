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

// TODO: move this somewhere better?
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

typedef struct NpcSetupBattleInfo {
    const char* name;           // 0x0
    const char* mapName;        // 0x4, never read; map the battle is set on
    ItemType fixedItem;         // 0x8
    s32 swFlag;                 // 0xC
    NpcBattleSelect* altSelect; // 0x10, used when swFlag is set
    NpcBattleSelect* select;    // 0x14
    s32 battleFlags;            // 0x18, -> NpcBattleInfo.battleFlags
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
    s32 battleFlags;                   // 0x4, 0x10/0x40/0x20000000 checked by the battle end sequence
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
    f32 width;                    // 0x28, TODO double check
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

typedef struct NpcEntry {
    s32 flags;                      // 0x0
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
    u32 interruptFlags;             // 0x140, why the regular event was interrupted (blow/find/...)
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
    f32 maxFallSpeed;               // 0x1D0, fall speed limit, 100 in kpa_03 lava
    u32 jumpFlags;                  // 0x1D4, TODO verify
    s16 footstepToggle;             // 0x1D8
    f32 footstepAccum;              // 0x1DC
    const char* footstepSfxA;       // 0x1E0
    const char* footstepSfxB;       // 0x1E4
    u16 footstepStride;             // 0x1E8
    const char* jumpSfx;            // 0x1EC
    const char* landSfx;            // 0x1F0
    s32 cameraId;                   // 0x1F4, CameraId
    NpcTerritoryType territoryType; // 0x1F8, verified, TODO: create territory substruct?
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
    u16 koopaEncounterType;         // 0x31C
    u16 hazardFlags;                // 0x31E, 1 lava (kpa_03), 2/4/8/0x10 hazard deaths already handled
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
    f32 wCurrentRenderRotationMinus90Y; // 0x84, TODO better name
    f32 wRotationY;                     // 0x88, TODO better name
    f32 wScaleZ_flip;                   // 0x8C, TODO better name
    f32 wInitToZero4;                   // 0x90, TODO better name
} NpcFieldEntry;

typedef struct NpcWork {
    s32 count;            // 0x0, currently allocated
    s32 total;            // 0x4, maximum allowed
    s32 flags;            // 0x8
    NpcEntry* entries;    // 0xC
    NpcEntry* talkTarget; // 0x10, NPC that A would talk to this frame
} NpcWork;

typedef struct FirstStrikeInfo {
    BOOL enabled;        // 0x0
    u32 hitKind;            // 0x4, fbatSetAttackAnnounce kind (0x20000.. first-strike types), 0 when done
    u32 type;            // 0x8
    s32 timer;           // 0xC, frames into the announce slide (0..100)
    f32 x;               // 0x10, announce x, slides -600 -> 0 -> 600
    f32 y;               // 0x14, announce y
    const char* msg_tag; // 0x18
    GXColor color;       // 0x1C
} FirstStrikeInfo;

typedef struct FieldBattleInfo {
    u32 mode; //0x0, unused
    BattleUnitType party; //0x4
    u32 partyId; //0x8
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
    s32 hitKind;                 // 0x10, fbatHitCheck kind (0x10000000 = forced first attack, tou_03)
    s32 firstAttackParam;        // 0x14, set to 0x10 by tou_03 _firstattack_setting; passed to BattleInformationSetFirstAttack (seq_battle)
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
void npcReleaseFieldNpc(void);
void npcRecoveryFieldNpc(void);
void npcInit(void);
void npcReset(BOOL inBattle);
s32 npcGetReactionOfLivingBody(BOOL inBattle);
s32 npcEntry(const char* a1, const char* animName);
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
