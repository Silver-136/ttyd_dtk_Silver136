#pragma once

#include "mario/mario.h"
#include <dolphin/types.h>

#pragma enumsalwaysint off
typedef enum PartyMember {
    PARTY_MEMBER_NONE,      // 0
    PARTY_MEMBER_GOOMBELLA, // 1
    PARTY_MEMBER_KOOPS,     // 2
    PARTY_MEMBER_BOBBERY,   // 3
    PARTY_MEMBER_YOSHI,     // 4
    PARTY_MEMBER_FLURRIE,   // 5
    PARTY_MEMBER_VIVIAN,    // 6
    PARTY_MEMBER_MS_MOWZ,   // 7
#define MAIN_PARTY_MEMBER_MAX PARTY_MEMBER_MS_MOWZ + 1
    PARTY_MEMBER_EGG,                // 8
    PARTY_MEMBER_FLAVIO,             // 9
    PARTY_MEMBER_PUNIO,              // 10
    PARTY_MEMBER_FRANKLY,            // 11
    PARTY_MEMBER_CRAW,               // 12
    PARTY_MEMBER_GOOMBELLA_FOLLOWER, // 13
    PARTY_MEMBER_KOOPS_FOLLOWER,     // 14
    PARTY_MEMBER_BOBBERY_FOLLOWER,   // 15
    PARTY_MEMBER_YOSHI_FOLLOWER,     // 16
    PARTY_MEMBER_FLURRIE_FOLLOWER,   // 17
    PARTY_MEMBER_VIVIAN_FOLLOWER,    // 18
    PARTY_MEMBER_MS_MOWZ_FOLLOWER    // 19
} PartyMember;
#pragma enumsalwaysint on

#pragma enumsalwaysint off
typedef enum PartyFlags {
    PARTY_FLAG_IS_ACTIVE = (1 << 0),
    PARTY_FLAG_IS_GROUNDED = (1 << 1),
    PARTY_FLAG_IS_FOLLOWER = (1 << 2),
    PARTY_FLAG_IS_PARTNER = (1 << 3),
    PARTY_FLAG_IS_JUMPING = (1 << 4),
    PARTY_FLAG_IS_FALLING = (1 << 5),
    PARTY_FLAG_IS_STEPPING = (1 << 6),
    PARTY_FLAG_IN_SHALLOW_WATER = (1 << 7),
    PARTY_FLAG_IS_BEING_USED = (1 << 8),
    PARTY_FLAG_KEEP_POSE_UNUSED = (1 << 9), // 0x200, keep pose and facing (no stand pose on stopping, no display-direction update, fixed rotation offset); nothing sets it
    PARTY_FLAG_PAPER_MODE = (1 << 12),
    PARTY_FLAG_MOT_SLIT_FLOOR = (1 << 13),
    PARTY_FLAG_IS_RIDING_PLANE = (1 << 16),
    PARTY_FLAG_IS_RIDING_SHIP = (1 << 17),
    PARTY_FLAG_IS_BEHIND_MARIO = (1 << 19),
    PARTY_FLAG_SLEEP_ON = (1 << 22),
    PARTY_FLAG_BG_MODE = (1 << 23),
    PARTY_FLAG_DISABLE_FRONT_WALL_CHK = (1 << 24),
    PARTY_FLAG_FORCE_FALL = (1 << 25),
    PARTY_FLAG_DISABLE_FLOORS = (1 << 26),
    PARTY_FLAG_RUN_OFF = (1 << 28),
    PARTY_FLAG_CTRL_OFF = (1 << 29),
    PARTY_FLAG_CTRL_OFF_2 = (1 << 30),
    PARTY_FLAG_IS_EVT_USE = (1 << 31)
} PartyFlags;
#pragma enumsalwaysint on

#pragma enumsalwaysint off
typedef enum PartySlotId {
    PARTY_SLOT_PARTY = 0,    // 0x0
    PARTY_SLOT_FOLLOWER = 1, // 0x1
    PARTY_SLOT_NONE = -1
} PartySlotId;
#pragma enumsalwaysint on

struct HitEntry;
struct EffectEntry;

/* PartyEntry 0x174-0x187: per-partner scratch. Each partner file uses its own view.
   Field types copy the existing casts exactly (signedness is load-bearing). */
typedef struct PartyWorkCommon { // party.c, evt_party.c (generic jump arcs)
    f32 accelY;                  // 0x174, added to velY each frame
    u8 pad178[0x188 - 0x178];    // 0x178
} PartyWorkCommon;

typedef struct PartyWorkYoshi {  // party_yoshi.c
    f32 gravityVel;              // 0x174, gravity += this
    f32 gravityAccel;            // 0x178, this += accelY
    u8 pad17C[0x188 - 0x17C];    // 0x17C
} PartyWorkYoshi;

typedef struct PartyWorkNokonoko { // party_nokonoko.c
    struct ItemEntry* heldItem;  // 0x174, item picked up while in the shell
    union {
        u32 shellSpinBits;       // 0x178, zeroed as u32
        f32 shellSpin;           // 0x178, 50.0 then -= 2.2 per frame
    };
    union {
        u32 word17C;             // 0x17C, zeroed as u32
        struct {
            s16 dashed;          // 0x17C, PH s16 0/1
            s16 shellFlags;      // 0x17E, PH s16 bits 1, 2
        };
    };
    u32 word180;                 // 0x180, zeroed only
    u8 pad184[0x188 - 0x184];    // 0x184
} PartyWorkNokonoko;

typedef struct PartyWorkBomhei { // party_bomhei.c
    u8 pad174[0x178 - 0x174];    // 0x174
    s16 timer;                   // 0x178
    u8 state;                    // 0x17A, 10/20/30/40/100...
    u8 syncFrame;                // 0x17B, index into m_w_7_sync
    s32 reserved_17C;                  // 0x17C, init -1
    s32 sfxId;                   // 0x180, fuse sfx (0x93C)
    s32 sfxId2;                  // 0x184, sfx 0x93D
} PartyWorkBomhei;

typedef struct PartyWorkCloud {  // party_cloud.c
    s32 reserved_174;                  // 0x174, zeroed in cloud_init only
    struct EffectEntry* breathEffect; // 0x178, KumokumoN64 breath cloud (zeroed as s32 too)
    union {
        f32 hoverAngle;          // 0x17C, cloud_hover_*: -= hoverSpeed, revise360
        s32 hoverAngleBits;      // 0x17C, zeroed as s32 (cloud_init, _cloud_use)
    };
    union {
        f32 hoverSpeed;          // 0x180, 4.25
        s32 hoverSpeedBits;      // 0x180, zeroed as s32
    };
    s32 lockAnims;               // 0x184, US cloudLockAnimationsOn/Off (unused on JP)
} PartyWorkCloud;

typedef struct PartyWorkVivian { // party_vivian.c
    f32 shadowSize;              // 0x174, 10..20; party.c reads it as the in-use shadow size
    u8 pad178[0x17C - 0x178];    // 0x178
    f32 followOfs;               // 0x17C, 0..-13, movePos distance behind Mario
    u8 pad180[0x182 - 0x180];    // 0x180
    s16 marioAnimTime;           // 0x182, local time for Mario's paper anim while hidden
    struct EffectEntry* rippleEffect; // 0x184, effRippleEntry; NULLed when the effect dies (flags == 0)
} PartyWorkVivian;

typedef union PartyWork {
    PartyWorkCommon common;
    PartyWorkYoshi yoshi;
    PartyWorkNokonoko nokonoko;
    PartyWorkBomhei bomhei;
    PartyWorkCloud cloud;
    PartyWorkVivian vivian;
} PartyWork;

// PartyEntry.flags2: the partner's display flags (evt_party_dispflg_onoff; scripts pair it with
// evt_mario_dispflag_onoff, and the bits mirror MarioDispFlags). 0x40/0x80 (never set) and 0x200 are not named.
typedef enum PartyDispFlags {
    PARTY_DISP_FLAG_HIDE = (1 << 0),             // 0x1, no draw, no shadow
    PARTY_DISP_FLAG_POSE_LOCK = (1 << 1),        // 0x2, pose and paper changes ignored
    PARTY_DISP_FLAG_NO_FLIP = (1 << 3),          // 0x8, partyDisp does not mirror
    PARTY_DISP_FLAG_NO_MAKE_DISP_DIR = (1 << 4), // 0x10, partyMakeDispDir skipped
    PARTY_DISP_FLAG_DISP_DIR_SNAP = (1 << 5),    // 0x20, turns happen at once
    PARTY_DISP_FLAG_SLIT = (1 << 8),             // 0x100, paper slit mode
    PARTY_DISP_FLAG_FAMICOM = (1 << 15),         // 0x8000, partySetFamicomMode
    PARTY_DISP_FLAG_SHADOW_OFF = (1 << 20),      // 0x100000, partyShadowOff
    PARTY_DISP_FLAG_SHADOW_NODECAY = (1 << 21),  // 0x200000, shadow drawn with color_tbl_nodecay
    PARTY_DISP_FLAG_OUT_OF_SCREEN = (1 << 23),   // 0x800000, evt_party_outofscreen
    PARTY_DISP_FLAG_PAPER = (1 << 26),           // 0x4000000, partyPaperOn
    PARTY_DISP_FLAG_TURN_LEFT_UNUSED = (1 << 6),  // 0x40, extra 180-degree yaw while facing left (no code sets it)
    PARTY_DISP_FLAG_TURN_RIGHT_UNUSED = (1 << 7), // 0x80, extra 180-degree yaw while facing right (no code sets it)
    PARTY_DISP_FLAG_SLIT_DIR_280 = (1 << 9),      // 0x200, slit walk drawn at direction 280 (else 260)
} PartyDispFlags;

// PartyEntry.trigFlags: one-shot change requests and door-move state (evt_party_trigflg_onoff). 0x10000 (no code sets it)
// and 0x800000 are not named.
typedef enum PartyTrigFlags {
    PARTY_TRIG_SLIT_INIT = (1 << 0),             // 0x1, start of the slit / roll paper motion
    PARTY_TRIG_CHG_MOT = (1 << 1),               // 0x2, motion changed
    PARTY_TRIG_CHG_MOVE_MODE = (1 << 2),         // 0x4, move mode changed
    PARTY_TRIG_CHG_RUN_MODE = (1 << 3),          // 0x8, partyChgRunMode
    PARTY_TRIG_DISP_DIR_CORRECTION = (1 << 12),  // 0x1000
    PARTY_TRIG_CHG_ANIM = (1 << 13),             // 0x2000, pose changed
    PARTY_TRIG_CHG_PAPER_ANIM = (1 << 14),       // 0x4000, paper pose changed
    PARTY_TRIG_FOLLOW_CLOSE = (1 << 17),         // 0x20000, door move: follow close
    PARTY_TRIG_HALF_PUSH = (1 << 18),            // 0x40000, door move: halved push box (partyMarioHosei)
    PARTY_TRIG_BEHIND_DIR_SET = (1 << 20),       // 0x100000, party_set_behind_dir
    PARTY_TRIG_DOOR_MOVE = (1 << 21),            // 0x200000, door and loading-zone gathering
    PARTY_TRIG_JUMP_TARGET_SET = (1 << 22),      // 0x400000
    PARTY_TRIG_LANDED = (1 << 23),               // 0x800000, landed: the next motion change keeps the footmarks
    PARTY_TRIG_SPLASH = (1 << 24),               // 0x1000000, water splash requested
    PARTY_TRIG_HOME_DIR_FROM_MOVE_UNUSED = (1 << 16), // 0x10000, behind-Mario direction from moveTarget (no code sets it)
} PartyTrigFlags;

typedef struct PartyEntry {
    u32 flags;                          // 0x0
    u32 flags2;                         // 0x4, PartyDispFlags
    u32 trigFlags;                      // 0x8, PartyTrigFlags
    s32 paperAnimGroup;                 // 0xC
    s32 reserved_10;                    // 0x10, only set to -1 at entry
    s32 paperPoseId;                    // 0x14
    const char* animName;               // 0x18, current pose name (strcmp'd with "PYS_S_2" etc.)
    const char* paperAnimName;          // 0x1C, partyChgPaper anim name, as MarioWork.paperAnimName
    u16 poseTime;                       // 0x20, cleared on every pose/paper change (MarioWork.poseTime)
    u8 pad_22[0x24 - 0x22];             // 0x22, never accessed
    s32 moveFrames;                     // 0x24, frames left in a move/jump (16 at jump start)
    s16 forceMoveFrames;                // 0x28, partySetForceMove frame count; evt_party_move waits for 0
    s16 mutekiTimer;                    // 0x2A, copied from MarioWork 0x4C (marioSetMutekiTime)
    s8 kemuriTimer;                     // 0x2C, frames between effKemuri2Entry dust puffs (mario_kemuri)
    s8 footstepTimer;                   // 0x2D, partyWalkSe frame counter
    s8 dirKind;                         // 0x2E, set from the pipe exit dir (partyDokanEnd)
    s8 currentSlotId;                   // 0x2F
    s8 port;                            // 0x30, controller port (partyUpdateKeyData)
    s8 currentMemberId;                 // 0x31, PartyMembers
    u8 moveKind;                        // 0x32, 1 = flying partner (cloud, vivian)
    u8 memberKind;                      // 0x33, set by each *_init to its PartyMember id (kuribo 1, nokonoko 2, yoshi 4, cloud 5, vivian 6)
    u8 runMode;                         // 0x34, partyChgRunMode
    u8 prevRunMode;                     // 0x35, saved by partyChgRunMode
    u8 motionId;                        // 0x36, partyChgMot
    u8 slitOn;                          // 0x37, 1 while squeezing through a slit (allPartySlitOn); blocks marioUseParty
    u8 motionStep;                      // 0x38, sub-step of the current motion (ride, jump)
    u8 useMotionId;                     // 0x39
    u8 slitStep;                        // 0x3A, party_slit state (0/2/10/20/40)
    u8 moveMode;                        // 0x3B, partyChgMoveMode
    u8 pad_3C;                          // 0x3C, never accessed
    u8 moveArg;                         // 0x3D, arg of partyMoveBehindMario / BesideMario; never read in C
    u8 reserved_3E;                     // 0x3E, only cleared
    s8 flyRequest;                      // 0x3F, partyMoveFlyInit: 1/3 = go behind Mario, 2 = other; cleared there, never set in C
    f32 homingDist;                     // 0x40, evt_party_set_homing_dist (default 50)
    u32 reserved_44;                    // 0x44, only set to 0x10
    u8 pad_48[0x4C - 0x48];             // 0x48, never accessed
    s8 splashTimer;                     // 0x4C, set to 20 after a water splash, counts down
    s8 wallStuckFrames;                 // 0x4D, frames blocked by a wall
    s8 noFloorFrames;                   // 0x4E, frames without floor
    s8 tailFlags;                       // 0x4F, Vivian: bit 0 = tail off
    u8 pad_50[0x51 - 0x50];             // 0x50, never accessed
    s8 footmarkMode;                    // 0x51, which footmark list is in use (partyClearFootmark / 2)
    s16 pushCount;                      // 0x52, frames pushed by Mario, capped at 100
    f32 homeDir;                        // 0x54, direction from Mario to the partner's follow spot
    Vec position;                       // 0x58
    union {
        Vec rot;                        // 0x64, same layout as MarioWork.rot
        struct {
            u8 rotXY[0x6C - 0x64];      // 0x64, rot.x / rot.y
            f32 rotation;               // 0x6C, rot.z: degrees, fmod 360
        };
    };
    Vec scale;                          // 0x70
    Vec dispOfs;                        // 0x7C, added to position when drawing (cloud hover bob, vivian sink)
    union {
        Vec rotOfs;                     // 0x88, rotation pivot (MarioWork.rotOfs)
        struct {
            u8 rotOfsX[0x8C - 0x88];    // 0x88, rotOfs.x
            f32 rotOfsY;                // 0x8C, rotOfs.y, half height times scale
            f32 rotOfsZ;                // 0x90, rotOfs.z
        };
    };
    Vec moveTarget;                     // 0x94, move/jump destination (partyGetAppearPos)
    f32 pushVelX;                       // 0xA0, push-away velocity from Mario (sin * scale)
    u8 pad_A4[0xA8 - 0xA4];             // 0xA4, never accessed
    f32 pushVelZ;                       // 0xA8, push-away velocity from Mario (-cos * scale)
    union {
        Vec floorMove;                  // 0xAC, movement from a moving floor (MarioWork 0x174)
        struct {
            f32 floorMoveX;             // 0xAC, floorMove.x
            f32 floorMoveY;             // 0xB0, floorMove.y
            f32 floorMoveZ;             // 0xB4, floorMove.z
        };
    };
    Vec trailPos;                       // 0xB8, Vivian: lags position by 0.1 per frame, at most VIVIAN_FOLLOW_DIST behind; reset to position at entry
    union {
        Vec groundRot;                  // 0xC4, floor normal as x/z rotation angles (partyNrmSet)
        struct {
            u8 groundRotX[0xC8 - 0xC4]; // 0xC4, groundRot.x
            f32 groundRotY;             // 0xC8, groundRot.y
            f32 groundRotZ;             // 0xCC, groundRot.z
        };
    };
    u8 pad_D0[0xE0 - 0xD0];             // 0xD0, never accessed
    f32 groundSlope;                    // 0xE0, floor angle from partyNrmSet; steps only up to 30
    f32 groundY;                        // 0xE4, floor height found by partySearchGround
    f32 dropDepth;                      // 0xE8, height of a drop below the search depth (0 on floor, -1 none)
    f32 frontWallDist;                  // 0xEC, distance to the front wall (-1 = none)
    f32 height;                         // 0xF0
    f32 width;                          // 0xF4
    f32 camDir;                         // 0xF8, camera yaw; stick input is rotated by it
    f32 moveDir;                        // 0xFC, copied from targetDir when moving
    f32 targetDir;                      // 0x100, angleABf from position to the move target
    f32 moveSpeed;                      // 0x104, distance/18 etc.
    f32 targetSpeed;                    // 0x108, partySetTargetSpd; follow speed = targetSpeed * playerGravity * partySpdRate
    f32 displayDirection;               // 0x10C, copied from MarioWork displayDirection
    f32 displayDirectionTarget;         // 0x110, copied from MarioWork displayDirectionTarget
    f32 velY;                           // 0x114, vertical velocity; jump ends when <= 0
    f32 accelY;                         // 0x118, vertical acceleration (-dy at jump start, -1.6 when falling)
    f32 gravity;                        // 0x11C, subtracted from velY each frame (1.6 / 0.4)
    f32 stepY;                          // 0x120, height of the step in front (partyChkFrontStep)
    f32 spinSpeed;                      // 0x124, ride spin, +2 per frame up to 30
    f32 moveDistance;                   // 0x128, distABf to moveTarget; moveSpeed = moveDistance / moveFrames
    struct HitEntry* hitobjFront;       // 0x12C, front wall (partySearchFrontWall)
    struct HitEntry* hitobjAction;      // 0x130, object hit by the partner action (bomb rock, shell, case type 15)
    struct HitEntry* hitobjPush;        // 0x134, front wall while moving
    struct HitEntry* hitobjStandOn;     // 0x138, hit object stood on (MarioWork.hitobjStandOn)
    struct HitEntry* hitobjJumpFrom;    // 0x13C, hitobjStandOn at jump start
    struct HitEntry* hitobjHead;        // 0x140, ceiling (partySearchHead)
    void* hitobjUnder;                  // 0x144, cleared with hitobjStandOn; read only in aji_13 (belt check); name by MarioWork order (Head, Under)
    s32 reserved_148;                   // 0x148, only cleared
    struct HitEntry* damageHit;         // 0x14C, HitEntry with attr 0x40000000 stood on (as MarioWork.damageHit)
    u16 buttonHold;                     // 0x150, partyUpdateKeyData: gp->button
    u16 buttonTrg;                      // 0x152, gp->buttonNew
    u16 buttonRepeat;                   // 0x154, gp->buttonRepeat
    u16 buttonUp;                       // 0x156, gp->buttonUp
    s8 stickX;                          // 0x158
    s8 stickY;                          // 0x159, negated
    s8 subStickX;                       // 0x15A
    s8 subStickY;                       // 0x15B, negated
    s8 triggerL;                        // 0x15C
    s8 triggerR;                        // 0x15D
    s8 reserved_15E;                    // 0x15E, cleared with the controller state, never read
    s8 reserved_15F;                    // 0x15F, cleared with the controller state, never read
    MarioWork* playerPtr;               // 0x160
    s32 camId;                          // 0x164
    u32 yoshiPsndSFXId;                 // 0x168
    s32 actionTime;                     // 0x16C, msec for an evt-driven action (nokotaro kick hold, cloud breathout)
    void* useStruct;                    // 0x170, per-partner heap work (PartyCloud*, YoshiWork*, ...)
    PartyWork work;                     // 0x174, per-partner scratch views
} PartyEntry; // size 0x188

PartyEntry* partyGetPtr(s32 index);
void partyCtrlOn(void);
void partyCtrlOff(void);
PartyEntry* anotherPartyGetPtr(s32 index);
BOOL partyPaperOn(PartyEntry* party, const char* animName);

void partyClearCont(PartyEntry* party);
extern s32 partyCtrlNo;
f32 partyGetHeight(PartyEntry* party);
void partyInitCamId(PartyEntry* party);
void partyKeyOff(void);
void partyKeyOn(void);
void partyPrevRunMode(PartyEntry* party);
void partyRun(PartyEntry* party);
void partyStop(PartyEntry* party);
void party_force_ride_yoshi(PartyEntry* party);
void party_ride_yoshi_force_move(void);

extern char* yoshigroup[7];

const char* partyChgPoseId(PartyEntry* party, s32 index);
void partyChgPose(PartyEntry* party, const char* pose);
void partyUsePost(PartyEntry* party);
void L_allPartyRideOn(void);
void allPartyForceRideOn(void);
void allPartyRideOff(void);
void allPartyRideOff2(void);
void allPartyRidePlane(void);
void allPartyRideShip(void);
void partyChgRunMode(PartyEntry* party, s32 mode);
void partyDokanEnd(s32 id, s32 dir);
s32 partyEntry2Pos(s32 memberId, f32 x, f32 y, f32 z);
s32 partyEntryPos(s32 memberId, f32 x, f32 y, f32 z);
void partyHello(PartyEntry* party);
void partyInit(void);
s32 partyKill2(s32 id);
void partyMain(void);
void partyPaperLightOff(PartyEntry* party);
void partyPaperOff(PartyEntry* party);
void partyReInit(void);
void partySetFamicomMode(PartyEntry* party, s32 mode);
void partyShadowOff(PartyEntry* party);
void partyUpdateKeyData(PartyEntry* party);
s32 yoshiSetColor(void);
void partyChgPaper(PartyEntry* party, char* paper);
BOOL partyDoWork(void);
s32 partyEntry(s32 memberId);
s32 partyEntry2(s32 memberId);
s32 partyEntry2Hello(s32 memberId);
s32 partyEntryHello(s32 memberId);
BOOL partyGoodbye(s32 id);
s32 partyGoodbyeInit(PartyEntry* party);
s32 partyGoodbyeMain(PartyEntry* party);
s32 partyKill(s32 id);
s32 partyRideChk(PartyEntry* party);
void partySetCamId(PartyEntry* party, s32 cameraId);
void partySetForceMove(PartyEntry* party, s32 frames, f32 dir, f32 speed);
