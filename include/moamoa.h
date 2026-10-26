#pragma once

// Smorg (moamoa) swarm on the field: shared by the map RELs that run it (eki_00, rsh_05, rsh_06). The Smorg battle
// unit (unit_boss_moamoa) and the rsh_00 window scene use their own structs.

#include <dolphin/types.h>
#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include "driver/mapdrv.h"
#include "manager/evtmgr.h"

typedef struct MoamoaPoseDef {
    char model[16];
    char anim[8];
    s16 loop;
    f32 speed;
    s32 start;
    s32 count;
} MoamoaPoseDef;

// One swarm member.
typedef struct Moamoa {
    u16 flags;          // 0x00
    s16 state;          // 0x02
    s16 timer;          // 0x04
    Vec target;         // 0x08
    f32 vy;             // 0x14
    s16 jumpTimer;      // 0x18
    f32 speed;          // 0x1C
    f32 gravity;        // 0x20
    s16 next;           // 0x24
    f32 lastX;          // 0x28
    f32 lastZ;          // 0x2C
    Vec jitter;         // 0x30, rsh_06 only
    s16 pose;           // 0x3C
    OSTime startTime;   // 0x40
    s16 group;          // 0x48, animation chain group (eki_00, rsh_06)
    s16 order;          // 0x4A, place within its chain group
    u8 kind;            // 0x4C
} Moamoa;

// The swarm.
typedef struct MoamoaWork {
    u16 flags;                 // 0x000
    s32 count;                 // 0x004
    Moamoa* moa;               // 0x008
    MapObject* mobj;           // 0x00C
    f32 mobjY;                 // 0x010
    Vec target[3];             // 0x014
    s16 hitCount;              // 0x038
    s16 targetHits[2];         // 0x03A, hits near each target; above 50 the escape event for that side runs
    s16 field_0x3e;            // 0x03E
    s16 hammerFrames;          // 0x040, frames Mario has been in the hammer motion
    s16 idleFrames;            // 0x042
    EventEntry* escapeEvt;     // 0x044
    s32 animPoseId;            // 0x048 (eki_00, rsh_06)
    s32 numDefs;               // 0x04C
    MoamoaPoseDef defs[32];    // 0x050
    f32 angle;                 // 0x550
    f32 dist;                  // 0x554
    s32 bounceCount;           // 0x558
    f32 breathDir;             // 0x55C
    f32 breathDist;            // 0x560
    Vec breathPos;             // 0x564
    s16 chainNum[128];         // 0x570, members per chain group (eki_00, rsh_06)
    s16 chainIdx[128][2];      // 0x670, animation group indices per chain group
} MoamoaWork;
