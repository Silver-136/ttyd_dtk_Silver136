// Koopa (Bowser) boss: types shared between the las battle and the tou2 (Glitz Pit) copy.

#pragma once
#include <dolphin/mtx.h>
#include <dolphin/types.h>

typedef struct IconJumpWork {
    Vec start;
    Vec pos;
    Vec end;
    s32 frames;
    f32 speed;
    f32 gravity;
    f32 vy;
    u8 pad[0x7C - 0x34];
} IconJumpWork;

