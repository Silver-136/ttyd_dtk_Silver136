#pragma once

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include "manager/evtmgr.h"
#include "manager/filemgr.h"
#include "mario/mario_pouch.h"
#include "battle/battle_database_common.h"

#pragma enumsalwaysint off
typedef enum AudienceMemberType {
    AUDIENCE_TOAD,          // 0x0, 0
    AUDIENCE_XNAUT,         // 0x1, 1
    AUDIENCE_BOO,           // 0x2, 2
    AUDIENCE_HAMMER_BRO,    // 0x3, 3
    AUDIENCE_DULL_BONES,    // 0x4, 4
    AUDIENCE_SHY_GUY,       // 0x5, 5
    AUDIENCE_CRAZY_DAYZEE,  // 0x6, 6
    AUDIENCE_PUNI,          // 0x7, 7
    AUDIENCE_KOOPA,         // 0x8, 8
    AUDIENCE_BULKY_BOBOMB,  // 0x9, 9
    AUDIENCE_GOOMBA,        // 0xA, 10
    AUDIENCE_PIRANHA_PLANT, // 0xB, 11
    AUDIENCE_LUIGI,         // 0xC, 12
    AUDIENCE_MAX            // 0xD, 13
} AudienceMemberType;
#pragma enumsalwaysint on

typedef struct StarPowerInfo {
    struct BattleWeapon* mWeapon; // 0x0
    f32 mSpAcSuccessMultiplier;   // 0x4
    u8 mStylishCommandMultiplier; // 0x8
    s8 reserved_09;               // 0x9, copied from BattleWeapon.reserved_19, never read
    s8 mBingoSlotChance;          // 0xA
    s8 pad_0B;                    // 0xB, never accessed
} StarPowerInfo;

// BattleAudienceMember.status: one handler per state, BattleAudienceCtrlProcessKinopio<Name> for every member
// type (the names follow those original handler symbols)
typedef enum BattleAudienceStatus {
    AUDIENCE_STATUS_WAIT, // 0
    AUDIENCE_STATUS_WAIT_HURIMUKI, // 1
    AUDIENCE_STATUS_WAIT_KUBI_KATAMUKE, // 2
    AUDIENCE_STATUS_WAIT_JUMP, // 3
    AUDIENCE_STATUS_JOY_RAISE_ARM, // 4
    AUDIENCE_STATUS_JOY_JUMP, // 5
    AUDIENCE_STATUS_JOY_JUMP2, // 6
    AUDIENCE_STATUS_ITEM_ON, // 7
    AUDIENCE_STATUS_ITEM_THROW, // 8
    AUDIENCE_STATUS_BOOING, // 9
    AUDIENCE_STATUS_DAMAGE, // 10
    AUDIENCE_STATUS_ENTER, // 11
    AUDIENCE_STATUS_ESCAPE, // 12
    AUDIENCE_STATUS_SLEEP, // 13
    AUDIENCE_STATUS_TRANS_EVT, // 14
    AUDIENCE_STATUS_INTRUDE, // 15
    AUDIENCE_STATUS_SING, // 16
    AUDIENCE_STATUS_SHELL, // 17
    AUDIENCE_STATUS_IGNITE, // 18
    AUDIENCE_STATUS_FIRE, // 19
    AUDIENCE_STATUS_EAT, // 20
} BattleAudienceStatus;

// BattleAudienceMember.flags (BattleAudience_* getters and setters). 0x400 and up are not used.
typedef enum BattleAudienceMemberFlags {
    AUDIENCE_MEMBER_FLAG_USED = (1 << 0),         // 0x1
    AUDIENCE_MEMBER_FLAG_NOT_EXIST = (1 << 1),    // 0x2, GetExist is FALSE (about to be eaten or sucked in)
    AUDIENCE_MEMBER_FLAG_DISP = (1 << 2),         // 0x4, drawn
    AUDIENCE_MEMBER_FLAG_FRONT_CENTER = (1 << 3), // 0x8, rows 0-2, columns 4-15: may hold items, join phase events
    AUDIENCE_MEMBER_FLAG_NO_SYS_CTRL = (1 << 4),  // 0x10, BattleAudience_GetSysCtrl
    AUDIENCE_MEMBER_FLAG_TARGETED = (1 << 5),     // 0x20, BattleAudience_SetTarget
    AUDIENCE_MEMBER_FLAG_ATTACKED = (1 << 6),     // 0x40, BattleAudience_Attack
    AUDIENCE_MEMBER_FLAG_WIDE = (1 << 7),         // 0x80, takes two seats (Bulky Bob-omb)
    AUDIENCE_MEMBER_FLAG_WIDE_SUB = (1 << 8),     // 0x100, the second seat of a WIDE member
    AUDIENCE_MEMBER_FLAG_NO_ESCAPE = (1 << 9),    // 0x200, GetEscapeChangeOK
} BattleAudienceMemberFlags;

typedef struct BattleAudienceMember {
    s32 flags;                 // 0x0, BattleAudienceMemberFlags
    s32 step;                  // 0x4, sub-step of the current status handler
    s32 counter;               // 0x8, per-status counter
    f32 counterF;              // 0xC, per-status float counter
    f32 workF1;                // 0x10, per-status float, cleared on status change
    f32 workF2;                // 0x14, per-status float, cleared on status change
    s8 colorStep;              // 0x18, added to colorB each frame (+-4)
    u8 status;                 // 0x19
    u8 texId;                  // 0x1A, palette texture index for the member quad
    u8 type;                   // 0x1B, AudienceMemberType
    s16 itemId;                // 0x1C, -1 is invalid
    u8 pad_1E[0x20 - 0x1E];    // 0x1E, never accessed
    s32** animTable;           // 0x20, set in BattleAudience_Entry_Sub
    s32* animStart;            // 0x24, current anim script (animTable entry); -3 jumps back here
    s32* animPc;               // 0x28, anim script read pointer (BattleAudienceAnimProcess)
    s32 animWait;              // 0x2C, frames left before the next script op
    s32 timer;                 // 0x30, per-status frame timer
    s32 animLoop[3];           // 0x34, loop counters for script ops -6/-7
    u8 animDepth;              // 0x40, loop nesting depth
    u8 colorR;                 // 0x41, draw colour (read with G/B/A in the member draw)
    u8 colorG;                 // 0x42
    u8 colorB;                 // 0x43
    u8 colorA;                 // 0x44, 0xFF = opaque (separate draw pass)
    u8 pad_45[0x48 - 0x45];    // 0x45, never accessed
    Vec position;              // 0x48
    Vec scale;                 // 0x54, final draw scale: baseScale * kind texture size (PSMTXScale in the member draw)
    Vec baseScale;             // 0x60
    Vec dispOffset;            // 0x6C, added to position for the drawn quad
    Vec velocity;              // 0x78, added to position each frame
    Vec accel;                 // 0x84, added to velocity
    Vec facingDir;             // 0x90, (0,0,-1) rotated by the member matrix; dot-product facing test
    Vec mRotation;             // 0x9C
    Vec mRotationOffset;       // 0xA8
    Vec mHomePosition;         // 0xB4
    f32 drawPosX;              // 0xC0, position + hop (+ bob), used as the thrown-item origin
    f32 drawPosY;              // 0xC4
    f32 drawPosZ;              // 0xC8
    f32 hopX;                  // 0xCC, hop offset added in the draw
    f32 hopY;                  // 0xD0, hop height, += hopSpeed, stops at 0
    f32 hopZ;                  // 0xD4
    u8 pad_D8[0xDC - 0xD8];    // 0xD8, never accessed
    f32 hopSpeed;              // 0xDC, += hopAccel per frame
    u8 pad_E0[0xE4 - 0xE0];    // 0xE0, never accessed
    f32 hopAccel;              // 0xE4, hop gravity (0 = hop finished)
    f32 angle;                 // 0xE8, init 270, wrapped to 0..360
    struct BattleUnit* targetUnit; // 0xEC, the unit the member targets
    s16 targetMemberId;        // 0xF0, index of another member it interacts with
    u8 pad_F2[0xF4 - 0xF2];    // 0xF2, never accessed
    s32 statusDelay;           // 0xF4, countdown before AUDIENCE_STATUS_SLEEP -> AUDIENCE_STATUS_WAIT_JUMP
    s32 interpFramesLeft;      // 0xF8
    s32 interpFramesTotal;     // 0xFC
    f32 startPosX;             // 0x100
    f32 targetPosX;            // 0x104
    f32 startPosY;             // 0x108
    f32 targetPosY;            // 0x10C
    f32 startPosZ;             // 0x110
    f32 targetPosZ;            // 0x114
    Vec interpDelta;           // 0x118
    f32 interpGravity;         // 0x124
    s32 interpType;            // 0x128
    union {
        s32 statusTimer;       // 0x12C, set to 5 and counted down while status == AUDIENCE_STATUS_IGNITE
        f32 bobAngle;          // 0x12C, kinopio bob phase (radians)
    };
    f32 bobOffset;             // 0x130, type 2: 5 * sin(phase) added to the drawn y
} BattleAudienceMember;

typedef struct BattleAudienceItem {
    u32 flags;              // 0x0
    s32 state;              // 0x4
    s32 timer;              // 0x8, per-state frame timer
    s32 audienceId;         // 0xC, member that threw it
    ItemType mItemType;     // 0x10
    Vec position;           // 0x14
    f32 scale;              // 0x20
    f32 rotZ;               // 0x24
    Vec velocity;           // 0x28
    Vec accel;              // 0x34, added to velocity each frame
    struct BattleUnit* target; // 0x40, unit the item is thrown at
    s32 mode;               // 0x44, nonzero once it is a hazard (lands on a unit)
} BattleAudienceItem;

typedef struct BattleAudienceStar {
    s32 flags;                // 0x0
    s32 state;                // 0x4, 0 spawn, 5 rise, 10 wait, 15 spiral to the AP gauge, 20 fade
    Vec position;             // 0x8
    f32 reserved_14;          // 0x14, random -20..20 at spawn, never read
    f32 reserved_18;          // 0x18, random 0..10 at spawn, never read
    f32 reserved_1C;          // 0x1C, 0 at spawn, never read
    Vec startPos;             // 0x20, interpolation start
    Vec targetPos;            // 0x2C, interpolation target / spiral centre
    Vec scale;                // 0x38
    s32 timer;                // 0x44, frames in the current state
    s32 duration;             // 0x48, frames for the current state
    f32 radius;               // 0x4C, spiral radius
    f32 startRadius;          // 0x50, spiral radius at the start of state 15
    f32 angle;                // 0x54, spiral angle (radians)
    f32 angleSpeed;           // 0x58, spiral angle step per frame
    u8 alpha;                 // 0x5C
    u8 pad_5D[0x60 - 0x5D];
} BattleAudienceStar;

typedef struct BattleAudienceSound {
    u8 header[0x10 - 0x0];         // 0x0, raw head (AudSound view in battle_audience.c)
    s32 mFadeoutTime;              // 0x10
    s32 mForceFadeTimer;           // 0x14
    s32 mForceFadeLength;          // 0x18
    u8 mBaseSfxVolume;             // 0x1C
    u8 mCurrentBaseSfxVolume;      // 0x1D
    u8 mVolumeMultiplier;          // 0x1E
    u8 mFadeStartVolumeMultiplier; // 0x1F
    u8 mFadeEndVolumeMultiplier;   // 0x20
    u8 pad_21[3];                  // 0x21, never accessed
} BattleAudienceSound;

typedef struct BattleAudienceWindow {
    u8 mActive;             // 0x0
    u8 pad_01[0x8 - 0x1];   // 0x1, never accessed
    f32 x;                  // 0x8, window x
    f32 y;                  // 0xC, window y
    f32 z;                  // 0x10, window z
    s32 slideTimer;         // 0x14, 0..15 slide-in frames
    f32 count;              // 0x18, number to display for audience count
} BattleAudienceWindow;

// BattleAudience.flags: 0x10..0x1000 are set by one BattleAudience_Case_* each and read by BattleAudience_PerAct.
// 0x1, 0x2 (set, never read), 0x4 and 0x4000 (Hooktail only) are not named.
typedef enum BattleAudienceFlags {
    AUDIENCE_FLAG_PHASE_REACTION = (1 << 3),       // 0x8
    AUDIENCE_FLAG_AC_GOOD = (1 << 4),              // 0x10
    AUDIENCE_FLAG_AC_BAD = (1 << 5),               // 0x20
    AUDIENCE_FLAG_GUARD_GOOD = (1 << 6),           // 0x40
    AUDIENCE_FLAG_GUARD_BAD = (1 << 7),            // 0x80
    AUDIENCE_FLAG_ACROBAT_GOOD = (1 << 8),         // 0x100
    AUDIENCE_FLAG_ACROBAT_BAD = (1 << 9),          // 0x200
    AUDIENCE_FLAG_ENEMY_DAMAGE = (1 << 10),        // 0x400
    AUDIENCE_FLAG_ENEMY_NO_DAMAGE = (1 << 11),     // 0x800
    AUDIENCE_FLAG_KILL_ENEMY = (1 << 12),          // 0x1000
    AUDIENCE_FLAG_NOISE_ON = (1 << 13),            // 0x2000
    AUDIENCE_FLAG_PUNI_ESCAPE = (1 << 15),         // 0x8000
    AUDIENCE_FLAG_PREV_COMMAND_INPUT = (1 << 16),  // 0x10000
    AUDIENCE_FLAG_NO_DISP = (1 << 17),             // 0x20000, aud_set_draw
    AUDIENCE_FLAG_EVENT_CTRL = (1 << 18),          // 0x40000, no reactions, item throws or count changes (lecture)
    AUDIENCE_FLAG_SP_GAINED_UNUSED = (1 << 1),     // 0x2, set when star power was gained; never read
    AUDIENCE_FLAG_NO_AUDIENCE_EVENTS = (1 << 14),  // 0x4000, audience events leave the crowd as it is (Hooktail)
} BattleAudienceFlags;

typedef struct BattleAudience {
    s32 flags;                         // 0x0, BattleAudienceFlags
    EventEntry* event;                 // 0x4
    s32 apStarSfxId;                   // 0x8, SFX_BTL_SYS_AP_STAR1 handle
    FileEntry* normalTex;              // 0xC
    FileEntry* guestTex[2];            // 0x10
    AudienceMemberType guestType[2];   // 0x18
    u8 pad_1A[0x1BC - 0x1A];           // 0x1A, never accessed (guestType is 2 bytes: AudienceMemberType is under enumsalwaysint off)
    BattleAudienceMember members[200]; // 0x1BC
    BattleAudienceItem items[100];     // 0xF25C
    BattleAudienceStar starpower[100]; // 0x10E7C
    BattleAudienceSound sounds[24];    // 0x133FC
    BattleAudienceWindow window;       // 0x1375C
    f32 mAudienceDeltaMultiplier;      // 0x13778
    f32 mTargetAudienceCount;          // 0x1377C
    f32 mAddedAudienceCount;           // 013780
    s32 mCurrentAudienceIntCount;      // 0x13784
    s32 mCurrentAudienceIntCountRight; // 0x13788
    s32 mCurrentAudienceIntCountLeft;  // 0x1378C
    s32 mMaxAudience;                  // 0x13790
    s32 typeWeights[12];               // 0x13794, per-type audience weights (BattleAudienceSettingAudience)
    s32 starPowerGain;                 // 0x137C4, star power gained this action
    StarPowerInfo* impendingBonuses;   // 0x137C8, copy of the last StarPowerInfo (BattleAudience_ApRecoveryBuild)
    s32 crowdPleasedStreak;            // 0x137CC, consecutive good actions, capped at 2
    s32 crowdDispleasedStreak;         // 0x137D0, consecutive bad actions, capped at 5
    s32 mNumStylishCommandsThisAttack; // 0x137D4
    u8 pad_137D8[0x137DC - 0x137D8];   // 0x137D8, never accessed
    s32 mCheckPhaseReactionState;      // 0x137DC
    s32 phaseSub;                      // 0x137E0, phase sub-state
    s32 presentItemNo;                 // 0x137E4
    s32 presentItemType;               // 0x137E8
    s32 presentTargetUnitId;           // 0x137EC
    s32 itemThrowCount;                // 0x137F0, audience item throws this battle
    s32 itemThrowMax;                  // 0x137F4, throw limit rolled at battle start (0-3; 10000 when forced)
    s32 presentHitCount;               // 0x137F8, thrown items that landed, capped at 3; adds to presentWeapon damage
    BattleWeapon presentWeapon;        // 0x137FC, BattleWeapon copy (0xC0) for a thrown hazard item, passed to BattleCheckDamage
    s32 mItemOnMemberId;               // 0x138BC
    Vec mItemOnMemberPos;              // 0x138C0
    s32 phaseList[14];                 // 0x138CC
    s32 phaseListCount;                // 0x13904
    s32 mTurnEndPhaseEventChance;      // 0x13908
    s32 joyLevel;                      // 0x1390C, audience joy level
    BOOL excited;                      // 0x13910, if toads are cheering
} BattleAudience;

BattleAudience* BattleAudienceBaseGetPtr(void);
BattleAudienceMember* BattleAudienceGetPtr(s32 id);
BattleAudienceItem* BattleAudienceItemGetPtr(s32 id);
BattleAudienceWindow* BattleAudienceWinGetPtr(void);
BattleAudienceSound* BattleAudienceSoundGetPtr(s32 id);
void BattleAudience_Init(void);
void BattleAudience_Main(void);
void BattleAudience_ActInit(void);
void BattleAudience_PerAct(void);
u8 BattleAudience_CheckReaction(void);
void BattleAudience_PerPhase(u32 value);
u8 BattleAudience_CheckReactionPerPhase(void);
void BattleAudience_Disp(void);
void BattleAudience_End(void);
BOOL BattleAudience_Entry(s32 index, AudienceMemberType type, u8 status);
void BattleAudience_Delete(s32 index);
void BattleAudience_SetAnim(s32 id, s32 animId, BOOL force);
void BattleAudience_GetPosition(s32 index, f32* x, f32* y, f32* z);
void BattleAudience_GetHomePosition(s32 index, f32* x, f32* y, f32* z);
void BattleAudience_GetRotate(s32 index, f32* x, f32* y, f32* z);
void BattleAudience_SetPosition(s32 index, f32 x, f32 y, f32 z);
void BattleAudience_SetRotate(s32 index, f32 x, f32 y, f32 z);
void BattleAudience_SetRotateOffset(s32 index, f32 x, f32 y, f32 z);
void BattleAudience_GetItemOn(s32* memberId, f32* x, f32* y, f32* z, ItemType* itemType);
void BattleAudience_GetItemOn2(s32* memberId, f32* x, f32* y, f32* z, ItemType* itemType);
void BattleAudience_SetTarget(s32 index);
void BattleAudience_Attack(s32 index);
void BattleAudience_ChangeStatus(s32 index, s32 status);
BOOL BattleAudience_GetExist(s32 id);
BOOL BattleAudience_GetSysCtrl(s32 index);
BOOL BattleAudience_GetWaiting(s32 index);
s32 BattleAudience_HaitiRandForFallObject(void);
s32 BattleAudience_GetAudienceNum(void);
void BattleAudience_WinSetActive(u8 active);

void BattleAudienceAddTargetNum(f32 value, f32 value2);
void BattleAudienceJoyEnding(void);
void BattleAudienceSoundCheer(s32 duration, s32 fade);
void BattleAudience_Case_AcrobatNoTry(void);
s32 BattleAudience_GetPresentItemNo(void);
s32 BattleAudience_GetPresentItemType(void);
s32 BattleAudience_GetPresentTargetUnitId(void);
void BattleAudience_SetPresentItemNo(s32 no);

void BattleAudience_SetPresentItemType(s32 type);
void BattleAudience_SetPresentTargetUnitId(s32 unitId);
extern const Vec vec3_802f972c[];
extern const Vec vec3_802f9738[];
extern const Vec vec3_802f9750[];
extern const Vec vec3_802f975c[];

void BattleAudienceSoundClap(s32 duration, s32 fade);
void BattleAudienceSoundSetVolAll(u8 vol, s32 length);
void BattleAudienceSoundStop(s32 index);
void BattleAudienceSoundWhistle(void);
s32 BattleAudienceDetectPakkunEatTarget(s32 index);
BattleUnit* BattleAudienceDetectTargetAll(void);
BOOL BattleAudienceItemOn(s32 index, u16 itemId, s32 numItems);
void BattleAudienceJoySACLecture(void);
void BattleAudienceNumToTarget(void);
void BattleAudiencePuniAllEscape(void);
void BattleAudienceSoundBombFire(s32 index);
void BattleAudienceSoundBombIgnite(s32 index, s32 duration, s32 fade);
void BattleAudienceSoundBooing(void);
void BattleAudienceSoundBooingKind(s32 kind);
void BattleAudienceSoundCallKind(s32 kind);
void BattleAudienceSoundCheerKind(s32 kind, s32 duration, s32 fade);
void BattleAudienceSoundHandBeat(void);
void BattleAudienceSoundItemThrow(s32 kind, s32 index);
void BattleAudienceSoundPakkunEat(s32 index);
void BattleAudienceSoundRun(s32 kind);
void BattleAudienceSoundShell(s32 index);
void BattleAudienceSoundSing(s32 index);
void BattleAudienceSoundSleep(s32 index);
void BattleAudienceSoundWhistleKind(s32 kind);
void BattleAudienceSoundZZZ(s32 index);
void BattleAudience_ApRecoveryBuild(StarPowerInfo* info);
void BattleAudience_Case_AcrobatBad(void);
void BattleAudience_Case_AcrobatGood(void);
void BattleAudience_Case_ActionCommandBad(BattleWeapon* weapon);
void BattleAudience_Case_KillEnemy(s32 level);
void BattleAudience_Case_ActionCommandGood(BattleWeapon* weapon);
void BattleAudience_Case_MarioBigDamage(s32 damage);
void BattleAudience_Case_EnemyDamage(s32 damage);
void BattleAudience_Case_Appeal(BattleUnit* unit);
void BattleAudience_Case_Countered(void);
void BattleAudience_Case_EnemyNoDamage(void);
void BattleAudience_Case_Escape(void);
void BattleAudience_Case_Escape_Bad(void);
void BattleAudience_Case_Escape_Good(void);
void BattleAudience_Case_FallObject_Aud(s32 index, u8 type);
void BattleAudience_Case_FallObject_Stage(void);
void BattleAudience_Case_FastVictory(f32 rate);
void BattleAudience_Case_FinalAttack(void);
void BattleAudience_Case_FirstAttack_Bad(void);
void BattleAudience_Case_FirstAttack_Good(void);
void BattleAudience_Case_GreatVictory(void);
void BattleAudience_Case_GuardBad(void);
void BattleAudience_Case_GuardGood(void);
void BattleAudience_Case_HaikeiSet(void);
void BattleAudience_Case_JumpNewRecord(void);
void BattleAudience_Case_MarioDanger(void);
void BattleAudience_Case_MarioPinch(void);
void BattleAudience_Case_PartyDown(void);
void BattleAudience_Case_TurnEnd(void);
s32 BattleAudience_GetAudienceNoFromOffset(s32 index, s32 column, s32 row);
BOOL BattleAudience_GetFront(s32 index);
s32 BattleAudienceAddAudienceNum(s32 num);
void BattleAudienceSoundSetVol(s32 index, u8 vol, s32 length);
BOOL BattleAudience_GetAnimEnd(s32 index);
s32 BattleAudienceSoundCheck(s32 index);
