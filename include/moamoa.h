#pragma once

// Smorg (moamoa) swarm on the field: shared by the map RELs that run it (eki_00, rsh_05, rsh_06). The Smorg battle
// unit (unit_boss_moamoa: ExtMoamoa*) and the rsh_00 window scene (MoamoaSprite) use their own structs; rsh_00 shares MoamoaPoseDef.

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include "driver/mapdrv.h"
#include "manager/evtmgr.h"

typedef struct MoamoaPoseDef {
    char model[16];     // model (texture group) name of the ext_moamoa_data rows
    char anim[8];       // pose name
    s16 loop;           // 0 for poses starting with "J", else 1; only rsh_00 reads it
    f32 frames;         // length of the pose in frames
    s32 start;          // first ext_moamoa_data row (texture) of the pose
    s32 count;          // number of rows (textures) of the pose
} MoamoaPoseDef;

// One swarm member.
typedef struct Moamoa {
    u16 flags;          // 0x00, MoamoaFlags
    s16 state;          // 0x02, MoamoaState
    s16 timer;          // 0x04, frame counter or wait of the current state
    union {             // 0x08
        Vec target;     // jump (MOAMOA_STATE_JUMP): where it lands
        Vec driftVel;   // MOAMOA_STATE_BLOWN_DRIFT: added to its position every frame
    };
    union {             // 0x14
        f32 vy;         // jump: vertical speed, reduced by gravity every frame
        f32 driftAccelX; // blown: added to driftVel every frame, as are the two below
    };
    union {             // 0x18
        s16 jumpTimer;  // jump: frames left
        s16 driftTimer; // blown: frames left
    };
    union {             // 0x1C
        f32 speed;      // jump: horizontal speed
        f32 driftAccelY;
        f32 circleHeight; // MOAMOA_STATE_GATHER_UNUSED: height above the target it circles
    };
    union {             // 0x20
        f32 gravity;    // jump
        f32 driftAccelZ;
        f32 circleRadius; // MOAMOA_STATE_GATHER_UNUSED
    };
    s16 next;           // 0x24, state taken when the jump ends
    f32 lastX;          // 0x28, x at the end of the last update; put back when it overlaps Mario
    f32 lastZ;          // 0x2C, z at the end of the last update
    Vec jitter;         // 0x30, random offset from its joint on the giant Smorg (rsh_06)
    s16 pose;           // 0x3C, index into MoamoaWork.defs
    OSTime startTime;   // 0x40, gp->renderTime when the pose's animation last started
    s16 group;          // 0x48, g_moamoa_chain_tbl entry: the giant Smorg segment it sits on (rsh_06)
    s16 order;          // 0x4A, place along that segment
    u8 kind;            // 0x4C, MoamoaKind
} Moamoa;

typedef enum MoamoaFlags {
    MOAMOA_FLAG_TO_TARGET = (1 << 0),        // 0x1, long hop toward its target; does not react to Mario
    MOAMOA_FLAG_JUMPING = (1 << 1),          // 0x2, jump in progress (new states go to next)
    MOAMOA_FLAG_HIT = (1 << 2),              // 0x4, hit by Mario or a partner; damage poses only
    MOAMOA_FLAG_KNOCKED_AWAY = (1 << 3),     // 0x8, hammer, shell or bomb hit (with HIT)
    MOAMOA_FLAG_ESCAPING = (1 << 4),         // 0x10, running away when the swarm breaks up
    MOAMOA_FLAG_GONE = (1 << 5),             // 0x20, hidden for good (MOAMOA_STATE_GONE)
    MOAMOA_FLAG_OFF_ROOF = (1 << 6),         // 0x40, no floor under it at start: placed beside the train (rsh_06)
    MOAMOA_FLAG_BLOWN = (1 << 7),            // 0x80, blown by Flurrie's gust (with HIT)
    MOAMOA_FLAG_FUSING = (1 << 8),           // 0x100, moving into the giant Smorg
    MOAMOA_FLAG_FUSION_ARRIVED = (1 << 9),   // 0x200, reached its place in the giant Smorg
    MOAMOA_FLAG_FUSED = (1 << 10),           // 0x400, part of the giant Smorg body
    MOAMOA_FLAG_SHOW_KIND = (1 << 11),       // 0x800, drawn with its kind's model (Smorg b look or a passenger)
} MoamoaFlags;

typedef enum MoamoaState {
    MOAMOA_STATE_WANDER = 0,
    MOAMOA_STATE_FLEE = 1,                   // hop away from Mario
    MOAMOA_STATE_APPROACH = 2,               // hop toward Mario (only entered in rsh_06)
    MOAMOA_STATE_JUMP = 3,                   // jump to target, then LAND
    MOAMOA_STATE_LAND = 4,                   // short pause, then next
    MOAMOA_STATE_FUSION_WAIT = 14,
    MOAMOA_STATE_FUSION_MOVE = 15,
    MOAMOA_STATE_ESCAPE_TURN = 25,
    MOAMOA_STATE_ESCAPE_WAIT = 26,
    MOAMOA_STATE_ESCAPE_RUN = 27,
    MOAMOA_STATE_ESCAPE_EXIT = 28,           // rsh_05
    MOAMOA_STATE_STOMPED = 36,               // three hops away, then SHRINK
    MOAMOA_STATE_STOMPED_HOP2 = 37,
    MOAMOA_STATE_STOMPED_HOP3 = 38,
    MOAMOA_STATE_SHRINK = 39,
    MOAMOA_STATE_KNOCKED_AWAY = 47,
    MOAMOA_STATE_BLOWN_DRIFT = 48,
    MOAMOA_STATE_RESPAWN = 49,
    MOAMOA_STATE_GATHER_WAIT_UNUSED = 50,    // 50-52: circle a target, then FUSION_ARRIVED; nothing enters 50
    MOAMOA_STATE_GATHER_INIT_UNUSED = 51,
    MOAMOA_STATE_GATHER_UNUSED = 52,
    MOAMOA_STATE_FUSED = 61,                 // follow its joint on the giant Smorg
    MOAMOA_STATE_GONE = 62,
    MOAMOA_STATE_BLOW_START = 63,            // evt_moamoa_blow
    MOAMOA_STATE_BLOW_WAIT = 64,
    MOAMOA_STATE_BLOW_AWAY = 65,
} MoamoaState;

// Models by kind (group_tbl): the two Smorg looks and the captured train passengers (by their NPC names).
typedef enum MoamoaKind {
    MOAMOA_KIND_MOA_A = 0,
    MOAMOA_KIND_MOA_B = 1,
    MOAMOA_KIND_KINOPIO = 2,
    MOAMOA_KIND_COOK = 3,
    MOAMOA_KIND_POWAN = 4,
    MOAMOA_KIND_SARARY = 5,
    MOAMOA_KIND_ROSE = 6,
    MOAMOA_KIND_PAPA = 7,
    MOAMOA_KIND_MAMA = 8,
    MOAMOA_KIND_KONARI = 9,
    MOAMOA_KIND_MOA_A_S2_UNUSED = 10,        // c_n_moa_a with pose "S_2": never assigned, no such pose
} MoamoaKind;

// The swarm.
typedef struct MoamoaWork {
    u16 flags;                 // 0x000, MoamoaWorkFlags
    s32 count;                 // 0x004, number of members (one ext pose each)
    Moamoa* moa;               // 0x008, the members
    MapObject* mobj;           // 0x00C, rsh_05: the train map object "ressha"
    f32 mobjY;                 // 0x010, rsh_05: the train's height last frame; members move with its change
    Vec target[3];             // 0x014, points the members gather round (member i uses target[i % 3]);
                               // rsh_05: the waitress, the conductor and a fixed point
    s16 hitCount;              // 0x038, hits on members; a hit member comes back only while it is below 250
                               // (eki_00, where above 250 the rest escape) or 500 (rsh_05)
    s16 targetHits[3];         // 0x03A, hits near each target (rsh_05); above 50 the escape event for that side runs
    s16 hammerFrames;          // 0x040, frames Mario has been in the hammer motion
    s16 idleFrames;            // 0x042, Mario-still counter, incremented/capped at 1000 in eki_00, rsh_05 and rsh_06; no other consumer found
    EventEntry* escapeEvt;     // 0x044, evt_moamoa_escape, started once
    s32 animPoseId;            // 0x048, pose of the giant Smorg NPC "base" (rsh_06); never read
    s32 numDefs;               // 0x04C, entries of defs
    MoamoaPoseDef defs[32];    // 0x050, poses built from ext_moamoa_data (moamoa_pose_init)
    f32 angle;                 // 0x550, angle from Mario to the member being updated
    f32 dist;                  // 0x554, XZ distance from Mario to the member being updated
    s32 bounceCount;           // 0x558, bounces of Mario off members in one fall (at most 2); 0 once he is
                               // neither jumping nor falling
    f32 breathDir;             // 0x55C, Flurrie's gust direction while she is in use (cloudGetBreathDir), else -1
    f32 breathDist;            // 0x560, reach of that gust (cloudGetBreathDist), else -1
    Vec breathPos;             // 0x564, gust point; members within 30 of it are blown
    s16 chainNum[128];         // 0x570, members per chain group (rsh_06)
    s16 chainIdx[128][2];      // 0x670, the two animation groups of each chain group (rsh_06)
} MoamoaWork;

typedef enum MoamoaWorkFlags {
    MOAMOA_WORK_FLAG_ACTIVE = (1 << 0),           // 0x1, drawn and reacting to the player
    MOAMOA_WORK_FLAG_DRAW_OFFSCREEN = (1 << 1),   // 0x2, drawn into the offscreen capture (eki_00 elevator)
    MOAMOA_WORK_FLAG_NO_HIT = (1 << 2),           // 0x4, members cannot be hit (rsh_06)
    MOAMOA_WORK_FLAG_NO_PUSH = (1 << 3),          // 0x8, members on the roof do not push Mario (rsh_06)
    MOAMOA_WORK_FLAG_PARTY_IN_USE = (1 << 4),     // 0x10, copy of PARTY_FLAG_IS_BEING_USED, updated every frame
    MOAMOA_WORK_FLAG_TARGET0_CLEARED = (1 << 12), // 0x1000, target 0 freed (rsh_05: the waitress)
    MOAMOA_WORK_FLAG_TARGET1_CLEARED = (1 << 13), // 0x2000, target 1 freed (rsh_05: the conductor)
} MoamoaWorkFlags;
