#pragma once

#include <dolphin/mtx.h>
#include "driver/dispdrv.h"

#pragma enumsalwaysint off
typedef enum MarioMotion {
    MARIO_MOTION_STAY,         // 0x0
    MARIO_MOTION_WALK,         // 0x1
    MARIO_MOTION_DASH,         // 0x2
    MARIO_MOTION_JUMP,         // 0x3
    MARIO_MOTION_JUMP_NPC,     // 0x4
    MARIO_MOTION_JUMP_SW,      // 0x5
    MARIO_MOTION_JUMP_STAND,   // 0x6
    MARIO_MOTION_JUMP_2,       // 0x7
    MARIO_MOTION_JUMP_3,       // 0x8
    MARIO_MOTION_JUMP_SMALL,   // 0x9
    MARIO_MOTION_FALL,         // 0xA
    MARIO_MOTION_FALL_2,       // 0xB
    MARIO_MOTION_UPSTAIRS,     // 0xC
    MARIO_MOTION_LAND,         // 0xD
    MARIO_MOTION_TALK,         // 0xE
    MARIO_MOTION_GET_ITEM,     // 0xF
    MARIO_MOTION_HIP,          // 0x10
    MARIO_MOTION_HIP_2,        // 0x11
    MARIO_MOTION_HAMMER,       // 0x12
    MARIO_MOTION_HAMMER_2,     // 0x13
    MARIO_MOTION_JABARA,       // 0x14
    MARIO_MOTION_SLIT,         // 0x15
    MARIO_MOTION_ROLL,         // 0x16
    MARIO_MOTION_KAZE,         // 0x17
    MARIO_MOTION_PLANE,        // 0x18
    MARIO_MOTION_SHIP,         // 0x19
    MARIO_MOTION_YOSHI,        // 0x1A
    MARIO_MOTION_CLOUD,        // 0x1B
    MARIO_MOTION_VIVIAN,       // 0x1C
    MARIO_MOTION_DOKAN,        // 0x1D
    MARIO_MOTION_GRASP,        // 0x1E
    MARIO_MOTION_DAMAGE,       // 0x1F
    MARIO_MOTION_DAMAGE_TOGE,  // 0x20
    MARIO_MOTION_BOTTOMLESS,   // 0x21
    MARIO_MOTION_FORCE_RESET,  // 0x22
    MARIO_MOTION_SHADOW,       // 0x23
    MARIO_MOTION_PARTY_USE,    // 0x24
    MARIO_MOTION_KPA_SWIM,     // 0x25
    MARIO_MOTION_KPA_POW_UP,   // 0x26
    MARIO_MOTION_KPA_POW_DOWN, // 0x27
    MARIO_MOTION_DUMMY,        // 0x28
    MARIO_MOTION_MOTION_MAX = 0xFFFF
} MarioMotion;
#pragma enumsalwaysint on

#pragma enumsalwaysint off
typedef enum MarioFlags {
    MARIO_FLAG_ACTIVE = (1 << 0),
    MARIO_FLAG_CTRL_OFF = (1 << 1),
    MARIO_FLAG_CTRL_OFF_2 = (1 << 2),
    MARIO_FLAG_KEY_OFF = (1 << 3),
    MARIO_FLAG_FORCED_MOVEMENT = (1 << 5),
    MARIO_FLAG_USE_FORCE_DIRECTION = (1 << 6),
    MARIO_FLAG_STOP_AIR_MOVEMENT = (1 << 7),
    MARIO_FLAG_FLOORS_DISABLED = (1 << 8),
    MARIO_FLAG_FORCE_WALL = (1 << 9),
    MARIO_FLAG_WALLS_DISABLED = (1 << 10),
    MARIO_FLAG_HAMMER_HIT_WALL = (1 << 11),
    MARIO_FLAG_PRE_BATTLE_PHYSICS = (1 << 12),
    MARIO_FLAG_IS_CARRYING_BOBBERY = (1 << 13),
    MARIO_FLAG_ITEM_GET_DISABLED = (1 << 14),
    MARIO_FLAG_IS_PUSHING = (1 << 15),
    MARIO_FLAG_IS_JUMPING = (1 << 16),
    MARIO_FLAG_IS_FALLING = (1 << 17),
    MARIO_FLAG_IS_STEPPING = (1 << 18),
    MARIO_FLAG_HAS_INPUT_JUMP = (1 << 19),
    MARIO_FLAG_PAPER_MODE = (1 << 20),
    MARIO_FLAG_GONE_THROUGH_BARS = (1 << 21),
    MARIO_FLAG_PARTY_WALK_DISABLED = (1 << 22),
    MARIO_FLAG_SLIT_ABILITY_DISABLED = (1 << 23),
    MARIO_FLAG_TUBE_MODE = (1 << 24),
    MARIO_FLAG_BG_MODE = (1 << 25),
    MARIO_FLAG_IN_SHALLOW_WATER = (1 << 26),
    MARIO_FLAG_IS_TOUCHING_CEILING = (1 << 27),
    MARIO_FLAG_CHG_MOT_DISABLED = (1 << 28),
    MARIO_FLAG_IS_SHADOW_MARIO = (1 << 30),
    MARIO_FLAG_IS_8_BIT = (1 << 31)
} MarioFlags;
#pragma enumsalwaysint on

#pragma enumsalwaysint off
typedef enum MarioTrigFlags {
    MARIO_TRIG_FLAG_IS_STARTING_NEW_MOTION = (1 << 0),
    MARIO_TRIG_FLAG_USE_PARTNER = (1 << 1),
    MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON = (1 << 2),
    MARIO_TRIG_FLAG_ZOOM_UP_LEVEL_UNUSED = (1 << 4), // 0x10, request taken (and cleared) by marioCamZoomUpLevelMain; nothing sets it
    MARIO_TRIG_FLAG_DISP_DIR_CORRECTION = (1 << 8),
    MARIO_TRIG_FLAG_DISP_DIR_LEFT = (1 << 9),
    MARIO_TRIG_FLAG_DISP_DIR_RIGHT = (1 << 10),
    MARIO_TRIG_FLAG_CHG_ANIM = (1 << 12),
    MARIO_TRIG_FLAG_CHG_PAPER_ANIM = (1 << 14),
    MARIO_TRIG_FLAG_SET_JUMP_PARAMS = (1 << 16),
    MARIO_TRIG_FLAG_DISABLE_JUMP_KEMURI = (1 << 17),
    MARIO_TRIG_FLAG_JABARA_CHK = (1 << 20),
    MARIO_TRIG_FLAG_JABARA_MOVE_CHK = (1 << 21),
    MARIO_TRIG_FLAG_START_JABARA_SPIN = (1 << 22),
    MARIO_TRIG_FLAG_DISABLE_UNUSED_INPUT = (1 << 24),
    MARIO_TRIG_FLAG_DISABLE_TRIGGERS = (1 << 25),
    MARIO_TRIG_FLAG_DISABLE_SUB_STICK = (1 << 26),
    MARIO_TRIG_FLAG_DISABLE_STICK = (1 << 27),
    MARIO_TRIG_FLAG_DISABLE_PARTY_SLIT = (1 << 30),
    MARIO_TRIG_FLAG_NORMALIZING = (1 << 31) // evt_mario_normalize running: the partner does not start slit mode
} MarioTrigFlags;
#pragma enumsalwaysint on

/* ---- (1) per-motion scratch views: MarioWork.mot, 0x2A0 JP / 0x2B8 US, 0x1C bytes ----
 * Each motion reuses the block with its own types. Offsets are relative to the block. */
typedef struct MarioMotJabara {    // mot_jabara.c (spring jump)
    s32 reserved_0;                      // 0x00, cleared on entry
    f32 dir;                       // 0x04, displayDirectionTarget at the start
    s32 floorY;                    // 0x08, (s32)(landing y + collision box y)
    s32 reserved_C;                      // 0x0C, cleared on entry
    f32 rotSpeed;                  // 0x10, spin speed, added to rot.x
    f32 rotAccel;                  // 0x14
    s16 rotTimer;                  // 0x18
    s8 state;                      // 0x1A
    s8 animFrame;                  // 0x1B, paper anim local time
} MarioMotJabara;

typedef struct MarioMotSlit {      // mot_slit.c (paper slit / side-step)
    f32 reserved_0;                      // 0x00, cleared on entry
    s32 counter;                   // 0x04
    f32 pushDir;                   // 0x08, 0 or 180: slide direction relative to targetYaw while pushing a 0x2000 wall
    f32 lastY;                     // 0x0C, position.y last frame
    u8 pad10[4];                   // 0x10
    s16 timer;                     // 0x14
    s16 count;                     // 0x16
    s16 flags;                     // 0x18, bit 0
    s16 side;                      // 0x1A, 1 = 260 deg, 2 = 280 deg
} MarioMotSlit;

#ifndef VERSION_G8ME01
typedef struct MarioMotStay {      // mot_stay.c, JP only (US keeps these in MarioWork.stay)
    u8 pad00[0x10];                // 0x00
    s32 jabaraWait;                // 0x10, frames before the spring-jump check, capped at 10
    s32 deepSleepTimer;            // 0x14, marioChkDeepSleep: >= 10 s
    s32 holdTimer;                 // 0x18, frames the stick has been held
} MarioMotStay;
#endif

typedef struct MarioMotPeach {     // peach.c stay
    u8 pad00[0x14];                // 0x00
    s32 sleepTimer;                // 0x14, capped at 10 s
} MarioMotPeach;

typedef struct MarioMotHammer {    // mot_hammer.c mot_hammer
    s32 hitFrames;                 // 0x00, hit kind (7 -> 3); frame limit for the hit
    f32 dir;                       // 0x04, swing direction
    s32 hitKind;                   // 0x08
} MarioMotHammer;

typedef struct MarioMotHammer2 {   // mot_hammer.c mot_hammer2 / mot_ham2_post (spin hammer)
    u8 pad00[4];                   // 0x00
    s16 timer;                     // 0x04
    u8 pad06[2];                   // 0x06
    f32 spinSpeed;                 // 0x08, deg/frame by hammer level
    u8 pad0C[4];                   // 0x0C
    f32 animTime;                  // 0x10
    s16 flags;                     // 0x14, MarioMotHammer2Flags
    u8 pad16[2];                   // 0x16
    s32 sfxId;                     // 0x18
} MarioMotHammer2;

// MarioMotHammer2.flags (mot_hammer2: the spin hammer charge)
typedef enum MarioMotHammer2Flags {
    HAMMER2_FLAG_AT_LEVEL = (1 << 0),      // 0x1, the swing reached the current charge level (cleared by stick rubbing)
    HAMMER2_FLAG_SWAY_BACK = (1 << 4),     // 0x10, at the level: swaying back down
    HAMMER2_FLAG_LEVEL_UP = (1 << 12),     // 0x1000, a charge level was reached this frame
    HAMMER2_FLAG_CHARGED_POSE = (1 << 13), // 0x2000, the charged pose (M_H_3A/6A/9A) was set
} MarioMotHammer2Flags;

// MarioMotRoll.flags (mot_roll)
#define ROLL_FLAG_SLOPE_DAMP_UNUSED (1 << 4)   // 0x10, damps the speed on flat ground; nothing sets it
#define ROLL_FLAG_NO_SLOPE_UNUSED (1 << 5)     // 0x20, ignores the ground slope; nothing sets it
#define ROLL_FLAG_EVT (1 << 8)                 // 0x100, setRollEvtFlag: an event drives the roll
#define ROLL_FLAG_FIX (1 << 9)                 // 0x200, marioRollFixOn: the roll cannot be cancelled
#define ROLL_FLAG_SFX_UNUSED (1 << 12)         // 0x1000, the rolling sound started; never read
#define ROLL_FLAG_STICK_UNUSED (1 << 14)       // 0x4000, the stick was moved; never read
#define ROLL_FLAG_NO_SLOPE_SPEED_UNUSED (1 << 16) // 0x10000, keeps the speed off the slope; nothing sets it

typedef struct MarioMotRoll {      // mot_roll.c
    u8 pad00[4];                   // 0x00
    s32 flags;                     // 0x04, ROLL_FLAG_*
    s32 sfxId;                     // 0x08, only JP uses it (US moved it to MotRollWork.sfxId)
} MarioMotRoll;

typedef struct MarioMotHip {       // mot_hip.c
    f32 angle;                     // 0x00
#ifndef VERSION_G8ME01
    s32 sfxId[2];                  // 0x04, JP only (US uses soundId[3], soundId[4])
#endif
} MarioMotHip;

typedef struct MarioMotDokan {     // mot_dokan.c, evt_mario.c evt_mario_paper_pose_dokan
    f32 rotSpeed;                  // 0x00
    f32 reserved_4;                      // 0x04, cleared on entry
    f32 startZ;                    // 0x08, position.z on entry
} MarioMotDokan;

typedef struct MarioMotDamage {    // mot_damage.c
    f32 rootY;                     // 0x00, set_damage_root_ypos
    u8 pad04[4];                   // 0x04
    f32 camStepY;                  // 0x08, camera y step
    f32 riseSpeed;                 // 0x0C
} MarioMotDamage;

typedef struct MarioMotPlane {     // mot_plane.c
    u8 pad00[4];                   // 0x00
    f32 turnSpeed;                 // 0x04, added to displayDirection
    f32 startY;                    // 0x08, position.y at take-off
    f32 groundY;                   // 0x0C
} MarioMotPlane;

typedef struct MarioMotShip {      // mot_ship.c
    u8 pad00[4];                   // 0x00
    s32 animTime;                  // 0x04, paper anim local time, counts down from 0x58
    u8 pad08[8];                   // 0x08
    f32 dispStepY;                 // 0x10, dispOfs.y step
} MarioMotShip;

typedef struct MarioMotYoshi {     // party_yoshi.c, mario.c marioDisp
    f32 bobSpeed;                  // 0x00
    f32 bobOfs;                    // 0x04, added to the base y
    f32 baseY;                     // 0x08
    u8 pad0C[4];                   // 0x0C
    f32 velX;                      // 0x10
    f32 velZ;                      // 0x14
    f32 rotZ;                      // 0x18, z tilt in marioDisp
} MarioMotYoshi;

typedef struct MarioMotJump {      // mot_jump.c, mario_motion.c marioLandOn / marioChgJumpStandMotion
    u8 pad00[0x10];                // 0x00
    f32 dist;                      // 0x10, height risen (jump stand) / distance from lastGroundPos (land)
    f32 standSpeed;                // 0x14, from the spring mobj (+0x38)
    f32 standHeight;               // 0x18, marioGetJumpHeight
} MarioMotJump;

typedef struct MarioMotKaze {      // mario_motion.c mot_kaze
    f32 spinSpeed;                 // 0x00
} MarioMotKaze;

typedef struct MarioMotBottomless { // mario_motion.c mot_bottomless
    u8 pad00[0xC];                 // 0x00
    f32 speed;                     // 0x0C
    f32 angle;                     // 0x10
} MarioMotBottomless;

typedef struct MarioMotKpa {       // koopa_motion.c
    u16 animFrame;                 // 0x00
    s16 reserved_2;                      // 0x02, cleared on entry
} MarioMotKpa;

typedef union MarioMotWork {       // 0x2A0 JP, 0x2B8 US, 0x1C bytes
    s32 word[7];                   // whole-block clears (mot_jabara, mot_roll)
    MarioMotJabara jabara;
    MarioMotSlit slit;
#ifndef VERSION_G8ME01
    MarioMotStay stay;
#endif
    MarioMotPeach peach;
    MarioMotHammer hammer;
    MarioMotHammer2 hammer2;
    MarioMotRoll roll;
    MarioMotHip hip;
    MarioMotDokan dokan;
    MarioMotDamage damage;
    MarioMotPlane plane;
    MarioMotShip ship;
    MarioMotYoshi yoshi;
    MarioMotJump jump;
    MarioMotKaze kaze;
    MarioMotBottomless bottomless;
    MarioMotKpa kpa;
} MarioMotWork;

#ifdef VERSION_G8ME01
typedef struct MarioStayWork {     // US only, MarioWork 0x2AC: mot_stay's counters left the scratch block
    s32 jabaraWait;                // 0x2AC
    s32 holdTimer;                 // 0x2B0, only cleared on US
    s32 deepSleepTimer;            // 0x2B4, marioChkDeepSleep
} MarioStayWork;
#endif

/* MarioWork.characterId: who the player controls (marioSetCharMode) */
// MarioWork.dispFlags, named from the setters (marioPaperOn/Off, the balloon and camera functions, peach/battle pose
// code, the JP demo's marioDispLeftOnlyOn/RightOnlyOn, marioBlurOn, marioSetDirEvent, evt_mario_shadow_onoff) and
// the drawing code. 0x20 (write-only) and 0x20000000 (only blocks the rear pose) are not named.
typedef enum MarioDispFlags {
    MARIO_DISP_FLAG_HIDE = (1 << 0),              // 0x1, not drawn
    MARIO_DISP_FLAG_POSE_LOCK = (1 << 1),         // 0x2, pose and paper changes ignored
    MARIO_DISP_FLAG_NO_REAR_POSE = (1 << 2),      // 0x4
    MARIO_DISP_FLAG_NO_DIR_FLIP = (1 << 3),       // 0x8
    MARIO_DISP_FLAG_FLIP = (1 << 4),              // 0x10
    MARIO_DISP_FLAG_CAM_STILL_UNUSED = (1 << 5),  // 0x20, the 3D camera did not move in x since the last frame (set while the stick is tilted); nothing reads it
    MARIO_DISP_FLAG_NO_MAKE_DISP_DIR = (1 << 8),  // 0x100, display direction not recomputed
    MARIO_DISP_FLAG_DISP_DIR_SNAP = (1 << 9),     // 0x200, turns happen at once
    MARIO_DISP_FLAG_LEFT_ONLY = (1 << 10),        // 0x400, always drawn facing direction 0
    MARIO_DISP_FLAG_RIGHT_ONLY = (1 << 11),       // 0x800, always drawn facing direction 180
    MARIO_DISP_FLAG_SHADOW_OFF = (1 << 12),       // 0x1000
    MARIO_DISP_FLAG_DIR_EVENT = (1 << 15),        // 0x8000, direction set by an event (marioSetDirEvent)
    MARIO_DISP_FLAG_NO_UPDATE_CAM_POS = (1 << 16), // 0x10000
    MARIO_DISP_FLAG_CAM_FOLLOW_Y_OFF = (1 << 17), // 0x20000
    MARIO_DISP_FLAG_CAM_COLLISION = (1 << 18),    // 0x40000
    MARIO_DISP_FLAG_ZOOM = (1 << 20),             // 0x100000, zoomed in (zoomOffTimer clears it)
    MARIO_DISP_FLAG_ZOOM_OFF_REQ = (1 << 21),     // 0x200000
    MARIO_DISP_FLAG_SLIT = (1 << 24),             // 0x1000000, paper slit mode
    MARIO_DISP_FLAG_BALLOON = (1 << 25),          // 0x2000000, talk balloon shown
    MARIO_DISP_FLAG_EVT_BALLOON = (1 << 26),      // 0x4000000, balloon set by evt_mario_balloon_*
    MARIO_DISP_FLAG_BLUR = (1 << 27),             // 0x8000000, marioBlurOn
    MARIO_DISP_FLAG_EVT_POSE = (1 << 28),         // 0x10000000, event pose from the e_mario group (marioAnimeId)
    MARIO_DISP_FLAG_PAPER = (1 << 30),            // 0x40000000, marioPaperOn
    MARIO_DISP_FLAG_REAR_POSE = (1 << 31),        // 0x80000000, a_mario_r (rear) pose group
} MarioDispFlags;

// MarioWork.peachFlags: one exclusive status (peachSetSts*, evt_peach_set_condition) and the X-Naut disguise
typedef enum PeachFlags {
    PEACH_FLAG_AFTERSHOWER = (1 << 4),  // 0x10
    PEACH_FLAG_INVISIBLE = (1 << 5),    // 0x20
    PEACH_FLAG_HAPPY = (1 << 6),        // 0x40
    PEACH_FLAG_UNEASY = (1 << 7),       // 0x80
    PEACH_FLAG_ANGRY = (1 << 8),        // 0x100
    PEACH_FLAG_ORDINARY = (1 << 9),     // 0x200
    PEACH_FLAG_CARRY = (1 << 10),       // 0x400
    PEACH_FLAG_STS_MASK = 0x7F0,        // the seven status bits
    PEACH_FLAG_GUNDAN = (1 << 13),      // 0x2000, peachTransformOn: disguised as an X-Naut
} PeachFlags;

typedef enum MarioCharacter {
    CHARACTER_MARIO,
    CHARACTER_PEACH,
    CHARACTER_BOWSER,
} MarioCharacter;

typedef struct MarioWork {
    u32 flags;                     // 0x0
    u32 dispFlags;                 // 0x4, MarioDispFlags
    u32 rollFlags;                 // 0x8, mot_roll: 0x10 while rolling (motion 0x16), 0 when done; evt_mario waits for 0
    u32 trigFlags;                 // 0xC
    s32 peachFlags;                // 0x10, PeachFlags
    u32 stageFlags;                // 0x14, Bowser stages: bit0 clear = side view (kpaGetStageViewType), bit1 = kpaGetStageType
    const char* animName;          // 0x18
    const char* paperAnimName;     // 0x1C
    void* reserved_20;             // 0x20, only cleared (marioInit, Vivian hide)
    u8 pad_24[0x28 - 0x24];        // 0x24, never accessed
    u16 poseTime;                  // 0x28
    u8 pad_2A[0x2E - 0x2A];        // 0x2A, never accessed
    MarioMotion currMotionId;      // 0x2E
    MarioMotion prevMotionId;      // 0x30, currMotionId before the last motion change (marioChgMot)
    u8 pad_32[0x34 - 0x32];        // 0x32, never accessed
    u32 frameCounter;              // 0x34, +1 every marioMain frame; bit0 drives blinking
    s8 ctrlOffCount;               // 0x38
    s8 keyOffCount;                // 0x39
    s8 hammerSfxIdx;               // 0x3A, se_data index stepping by 2 (mot_hammer)
    s8 partyUseDelay;              // 0x3B, frames before partyDoWork after the partner button (5); blocks marioChkTransform while nonzero
    s8 characterId;                // 0x3C
    s8 colorId;                    // 0x3D
    s8 dokanType;                  // 0x3E, pipe kind set by evt_mario before motion 0x1D (mot_dokan)
    s8 wallPushTimer;              // 0x3F, frames pushed into a wall before hitobjPush is set
    s8 wMotionTimer;               // 0x40
    s8 hammerChargeTimer;          // 0x41, mot_hammer charge frames (capped at 20)
    s8 hammerChargeLv;             // 0x42, mot_hammer charge stage (3 = full)
    s8 hammerDir;                  // 0x43, mot_hammer swing side (0/1)
    s32 currSubMotionId;           // 0x44
    s32 multiTimer;                // 0x48, per-motion frame counter (Koopa footstep SFX every 20, npcdrv flag-1 shadow size); ttyd-tools wYawnTimer, but no yawn use here
    s16 mutekiTimer;                     // 0x4C
    s16 forceMoveTimer;            // 0x4E
    s16 airTimer;                  // 0x50
    s16 jumpPeakAirTime;           // 0x52, airTimer when the jump reached its peak (mario_motion.c marioJump)
    s16 prevAirTimer;              // 0x54, airTimer of the previous frame (marioMain end)
    u8 pad_56[0x58 - 0x56];        // 0x56, never accessed
    s16 idleTimer;                 // 0x58, mot_stay: frames standing (idle pose after 4 s)
    s16 idleKeyTimer;              // 0x5A, mot_stay: frames before the stay sub-motion reacts
    s16 zoomOffDelay;              // 0x5C, frames before marioCamZoomOff fires (32 on request)
    s16 zoomOffTimer;              // 0x5E, frames until dispFlags 0x100000 (zoomed) clears
    s16 zoomLevel;                 // 0x60, 1..3 picks the zoom distance (520/475/440, else 450)
    s16 zoomOffTime;               // 0x62, zoom-out time in 10 ms units (passed to evt_cam3d_evt_off)
    f32 camZoomWeight;             // 0x64, camShiftZ; scales the projection shift in camdrv
    f32 camZoom;                   // 0x68, zoom factor (2.0 default) for the camdrv projection shift
    Vec camZoomOfs;                // 0x6C, zoom focus offset from position
    s16 kpaNoHitTimer;             // 0x78, Bowser: frames of no enemy contact (kpaNoHitEnemyCheck)
    s16 itemGetDisableTimer;       // 0x7A, frames until flag 0x4000 (item get disabled) clears
    f32 jumpVelocityY;             // 0x7C, JP0 in marioMakeJumpPara: position step per frame
    f32 jumpAccelerationY;         // 0x80, JP1: velocity += this / div
    f32 jumpJerkY;                 // 0x84, JP2: acceleration += this / div
    f32 jumpSnapY;                 // 0x88, JP3: jerk += this / div
    Vec position;                  // 0x8C
    Vec dispOfs;                   // 0x98, added to position for drawing
    Vec dispOfs2;                  // 0xA4, second draw offset
    Vec rotOfs;                    // 0xB0, rotation pivot offset (.y = half height times scale)
    Vec rot;                       // 0xBC, rotation in degrees (moo_* spin .z)
    Vec scale;                     // 0xC8
    Vec fallStartPos;              // 0xD4, position when airTimer was 0 (jump/fall start)
    Vec returnPos;                 // 0xE0, pipe entry position; bg-mode damage returns here
    Vec camTarget;                  // 0xEC
    Vec camCollision;                  // 0xF8
    u8 pad_104[0x110 - 0x104];     // 0x104, never accessed
    Vec basePos;                   // 0x110, reference position (init, Bowser/Peach motion 8)
    Vec lastGroundPos;             // 0x11C, position copied when leaving the ground (jump/roll/stay start), y = landing height
    f32 jumpDistance;              // 0x128, horizontal distance from lastGroundPos (marioSetFallPara/landing)
    f32 jumpPeakY;                 // 0x12C, position.y at the jump peak (marioSetPeakYpos)
    f32 tubeScaleY;                    // 0x130, y scale while in tube mode
    CameraId camId;                // 0x134
    f32 camShiftTargetX;                    // 0x138
    f32 camShiftX;                    // 0x13C
    f32 camShiftTargetY;                    // 0x140
    f32 camShiftY;                    // 0x144
    f32 camShiftTargetZ;                    // 0x148
    f32 camShiftZ;                    // 0x14C
    Vec camShiftRate;                    // 0x150
    f32 camFollowRate;             // 0x15C
    f32 prevCamPosX;                    // 0x160
    f32 baseHeight;                // 0x164, basePos.y - position.y (marioChkGndRoll)
    s32 screenX;                    // 0x168
    s32 screenY;                    // 0x16C
    s32 screenZ;                   // 0x170
    Vec floorMove;                 // 0x174, per-frame position correction: moving floor, evt_mario_set_hosei_xyz
    f32 baseSpeed;                 // 0x180, same on US
    f32 mBaseWalkSpeed;            // 0x184
    f32 mBaseDashSpeed;            // 0x188, same on US
    f32 moveDist;                  // 0x18C, distance to move this frame (marioMove)
#ifdef VERSION_G8ME01
    f32 jumpStartSpeed;            // 0x190 US only (+4 from here): baseSpeed at jump start
#endif
    f32 controlStickSensitivity;   // 0x190
    f32 controlStickAngle;         // 0x194
    f32 targetYaw;                 // 0x198
    f32 moveDirection;                    // 0x19C
    f32 directionView;             // 0x1A0
    f32 forceDir;                  // 0x1A4, forced view direction while flags 0x40 (evt_mario_set_force_dir)
    f32 displayDirection;                           // 0x1A8
    f32 displayDirectionTarget;                     // 0x1AC
    f32 savedDisplayDirection;     // 0x1B0, displayDirection saved by mot_hammer
    Vec wPlayerCollisionBox;       // 0x1B4 JP, 0x1B8 US
    f32 groundY;                   // 0x1C0, floor height under Mario (shadow)
    f32 stepUpY;                   // 0x1C4, height of a small step in front
    f32 wallPushDist;              // 0x1C8, largest push distance into the front wall
    f32 wallPushDir;               // 0x1CC, direction of that push
    s32 wallPushHold;              // 0x1D0, frames the push is held (14)
    void* touchNpc;                // 0x1D4, NpcEntry* (marioGetTouchNpcPtr)
    struct NpcEntry* targetNpc;    // 0x1D8, NPC in range for interaction (npcdrv)
    void* hitobjFrontA;             // 0x1DC, wall in front found on an A press
    void* hitobjFront;              // 0x1E0
    void* hitobjStandOn;           // 0x1E4, 0x1E8 US HitEntry
    void* hitobjJumpFrom;          // 0x1E8, 0x1EC US HitEntry
    void* hitobjPush;              // 0x1EC JP, 0x1F0 US HitEntry
    void* hitobjWall;               // 0x1F0, wall touched this frame (before push threshold)
    void* hitobjHead;              // 0x1F4
    void* hitobjUnder;              // 0x1F8
    void* hitobjHammer;             // 0x1FC, HitEntry hit by the hammer (mobjdrv/casedrv/marioChkHitHammer)
    void* hitobjHammerNew;          // 0x200, HitEntry found by this swing frame (copied to hitobjHammer)
    void* damageHit;               // 0x204, HitEntry
    void* prevHitobjStandOn;       // 0x208, hitobjStandOn of the previous frame (marioMain end)
    struct HitEntry* hitobjFrontWall; // 0x20C, front wall found by searchFrontWall
    f32 frontWallDist;             // 0x210, distance to the front wall (1000 = none)
    f32 groundSlope;               // 0x214, floor slope angle (degrees)
    Vec groundRot;                 // 0x218, floor normal as x/z rotation angles (degrees)
    f32 playerGravity;             // 0x224 JP, 228 US, 1.0 (0.5 on moo_* moon maps); scales walk/dash speed and jump gravity (marioSetGravity)
    s32 paperAnimGroupId[5];       // 0x228 JP, 0x22C US
    s32 paperPoseId;               // 0x23C JP, 0x240 US
    u8 pad_240[0x241 - 0x240];     // 0x240, never accessed
    s8 currPartySlotId[2];         // 0x241 JP, 0x245 US
    s8 prevPartyMemberId[2];       // 0x243 JP, 0x247 US
    u8 pad_245[0x246 - 0x245];     // 0x245, never accessed
    u16 buttonHold;                // 0x246, gp->button masked by enableKeyMask
    u16 buttonTrg;               // 0x248
    u16 buttonRepeat;              // 0x24A
    u16 buttonUp;                  // 0x24C
    s8 wStickDir1;                 // 0x24E JP, 0x252 US
    s8 wStickDir2;                 // 0x24F JP, 0x253 US
    s8 substickX;                  // 0x250
    s8 substickY;                  // 0x251, negated
    u8 triggerL;                   // 0x252
    u8 triggerR;                   // 0x253
    u8 analogA;                    // 0x254, always 0
    u8 analogB;                    // 0x255, always 0
    u8 pad_256[0x258 - 0x256];    // 0x256, never accessed
    u32 demoButtonPrev;            // 0x258, DemoPad buttons last frame
    u32 demoButtonNew;             // 0x25C, DemoPad buttons pressed this frame
    void* balloonEffect;           // 0x260, EffectEntry*
    void* balloonNpc;              // 0x264, NpcEntry*
    s32 slitPosSet;                // 0x268, mot_slit: slitPos is valid
    Vec slitPos;                   // 0x26C, mot_slit saved position
#ifdef VERSION_G8ME01
    s32 soundId[5];                // 0x278, US: 5 sounds (+8 from here)
#else
    s32 soundId[3];                // 0x278
#endif
    u32 enableKeyMask;             // 0x284 JP, 0x290 US: evt_mario_set_enable_key; 0 = all buttons
    union {                        // 0x288 JP, 0x294 US: per-motion work (motstruct design)
        void* motStruct;
        struct MotShipWork* motShip;
        struct MotPlaneWork* motPlane;
        struct MotJabaraWork* motJabara;
        struct MotRollWork* motRoll;
    };
    struct KpaWork* kpaWork;       // 0x28C JP, 0x298 US
    f32 dirEvtStartDir;            // 0x290 JP, 0x29C US: marioSetDirEventMain
    f32 dirEvtTargetDir;           // 0x294
    s32 dirEvtMsec;                // 0x298
    s32 dirEvtFrame;               // 0x29C
#ifdef VERSION_G8ME01
    MarioStayWork stay;            // 0x2AC US only (+0x18 from here)
#endif
    MarioMotWork mot;              // 0x2A0 JP, 0x2B8 US
    u8 pad_2BC[0x2C0 - 0x2BC];     // 0x2BC, never accessed
    s16 coinDropTimer;             // 0x2C0, set to 3 s by npcdrv (with muteki); marioMain drops a coin every 30 frames
    u8 pad_2C2[0x2C4 - 0x2C2];     // 0x2C2, never accessed
    f32 kpaSpeed;                  // 0x2C4, Bowser walk speed (koopa.c, koopa_motion.c)
    u32 bottomlessReload;          // 0x2C8, marioReloadMapOnBottomlessOn/Off; u32: marioBottomless tests it unsigned (cmplwi)
    Vec bottomlessResetPosition;   // 0x2CC JP, 0x2E4 US
    s16 slitState;                 // 0x2D8, mot_slit state machine
    s16 slitTimer2;                // 0x2DA
    s32 slitTimer;                 // 0x2DC
} MarioWork;                       // size 0x2E0 JP, 0x2F8 US

MarioWork* marioGetPtr(void);
s32 marioGetColor(void);
void marioSetCharMode(s32 mode);
void marioSetFamicomMode(s32 mode);
void marioSetSpec(void);
void marioEntry(void);
void marioItemGetDisable(void);
void marioItemGetChk(void);
BOOL marioItemGetOk(void);
BOOL marioCaseEventValidChk(void);
BOOL marioFrontActionValidChk(void);
BOOL marioCheckMenuDisable(void);
BOOL marioChkKey(void);
BOOL marioChkCtrl(void);
BOOL N_marioChkUseParty(void);
void marioFBattlePrepare(void);
void marioFBattlePost(void);
s32 marioCtrlOffChk(void);
s32 marioKeyOffChk(void);
s32 marioCtrlOff(void);
s32 marioCtrlOn(void);
s32 marioCtrlOff2(void);
s32 marioCtrlOn2(void);
s32 marioKeyOff(void);
s32 marioKeyOn(void);
s16 marioSetMutekiTime(s32 msec);
BOOL marioBgmodeChk(void);
void marioBgmodeOn(void);
void marioBgmodeOff(void);
// marioChkSts: TRUE when Mario is in any of the given states
typedef enum MarioStsCheck {
    MARIO_STS_JUMPING = (1 << 0),  // 0x1, MARIO_FLAG_IS_JUMPING
    MARIO_STS_FALLING = (1 << 1),  // 0x2, MARIO_FLAG_IS_FALLING
    MARIO_STS_PAPER = (1 << 2),    // 0x4, MARIO_FLAG_PAPER_MODE
    MARIO_STS_JABARA = (1 << 3),   // 0x8, MARIO_MOTION_JABARA
    MARIO_STS_ROLL = (1 << 4),     // 0x10, MARIO_MOTION_ROLL
    MARIO_STS_KAZE = (1 << 5),     // 0x20, MARIO_MOTION_KAZE
} MarioStsCheck;
BOOL marioChkSts(s32 flags);
void marioResetHitObj(const char* name);
void marioReset(void);
void marioOfsRotReset(void);
void marioSoundInit(void);
void marioPoseInit(void);
void marioInit(void);
void marioReInit(void);
void marioMain(void);
void marioCtrlOff2Main(void);
void marioMove(void);
void marioMoveMain(void);
BOOL marioChkPushAnime(void);
s32 marioAnimeId(void);
void marioChgDamagePose(void);
void marioChgEvtPose(const char* name);
const char* toDotMarioPose(const char* name);
void marioChgPose(const char* name);
void marioChgPoseTime(const char* name, s16 frames);
void marioChgPaper(const char* name);
void marioSetPaperAnimeLocalTime(s32 frames);
BOOL marioPaperOn(const char* name);
void marioPaperOff(void);
void marioPaperLightOn(void);
void marioPaperLightOff(void);
void marioGetScreenPos(Vec position, f32* x, f32* y, f32* z);
BOOL marioChkInScreen(s32 x, s32 y);
f32 marioGetScale(void);
void marioMakeDispDir(void);
void marioRearAnime(void);
void marioPreDisp(void);
void marioDispBlur(CameraId camId, void* param);
void marioDisp(CameraId camId, void* param);

#ifdef VERSION_G8ME01
#define MARIO_STAY(mp) ((mp)->stay)
#else
#define MARIO_STAY(mp) ((mp)->mot.stay)
#endif
