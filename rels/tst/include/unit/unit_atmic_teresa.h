// Atmic Teresa (Atomic Boo): the swarm of small Teresa drawn as ext poses, shared by jin and its tst copy.
#pragma once

#include <dolphin/mtx.h>
#include <dolphin/types.h>

// AtmTeresa.animType
enum {
    ATM_TERESA_ANIM_FLOAT = 0,  // texture frames 0-7 looping, 3 frames each
    ATM_TERESA_ANIM_GATHER = 1, // texture 8 for 30 frames, then 9-15
};

// One small Teresa of the swarm (BattleAlloc'd array of 30, `teresa`).
typedef struct AtmTeresa {
    u8 state;      // 0x00
    u8 animType;   // 0x01, ATM_TERESA_ANIM_*
    s32 animFrame; // 0x04
    s32 wait;      // 0x08, frames to stay still before moving
    s32 timer;     // 0x0C
    Vec vel;       // 0x10
    Vec start;     // 0x1C
    Vec target;    // 0x28
    u8 intplMode;  // 0x34, intplGetValue curve for the move from start to target
} AtmTeresa;
