#pragma once

#include <dolphin/types.h>
#include <dolphin/mtx.h>

/* mot_plane's work: MarioWork.motPlane (the 0x288 union). Allocated 0x48 by mot_plane's
 * trigger block, freed by mot_plane_post / on landing. Same on JP and US. */
typedef struct MotPlaneWork {
    u32 flags;                    // 0x00: bit0 = facing (hit attr bit 4 of the jump-from object), 0x10 = set in the dive, cleared on pull-up
    f32 pitch;                    // 0x04: nose pitch in degrees (sincosf(90 + pitch) in the collision probe)
    f32 yaw;                      // 0x08: 90 when facing right, 270 when left
    f32 roll;                     // 0x0C
    f32 diveSpeed;                // 0x10: speed gained from the pitch, k * sin(|pitch|); 2.0 on take-off
    f32 pitchSpeed;               // 0x14: per-frame pitch change from the stick, capped at 1.0
    f32 sinkSpeed;                // 0x18: added to position.y, eases back to 0
    f32 pitchRecover;             // 0x1C: per-frame pitch change while recovering, eases to 0
    s32 stallTimer;               // 0x20: frames below stall speed with the stick held back; >= limit -> sub-motion 60
    f32 pitchMin;                 // 0x24: lowest pitch allowed (-22 in a dive), eases to planeData[4]
    s32 sfxId;                    // 0x28: glide loop (0x17F), -1 when off
    u8 pad_2C[0x30 - 0x2C];   // 0x2C
    Vec frontPos;                 // 0x30: front collision probe start
    Vec tipPos;                   // 0x3C: nose tip; probe runs frontPos -> tipPos
} MotPlaneWork;                   // size 0x48

void flyMain(void);
void marioForcePlaneAnime(void);
s32 marioGetPlaneStatus(void);
s32 marioPlaneGetFlyingDir(void);
void mot_plane(void);
void mot_plane_post(void);
BOOL mario_plane_cancel(void);
