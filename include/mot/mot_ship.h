// mot_ship.c: Mario's paper boat motion (mot_ship, mot_ship_post), swirl (whirlpool) power and ship-place checks.

#pragma once

#include "dolphin/types.h"

BOOL marioChkShipMoveMode(void);
void marioShipChgFall(void);
BOOL marioChkSwirlPower(void);
void marioForceShipAnime(void);
void marioReInit_ship(void);
void marioSetSwirlPower(s32 msec, f32 speed, f32 angle);
// marioShipChanging: Boat Mode phase.
enum {
    SHIP_STATUS_NONE = 0,
    SHIP_STATUS_FOLDING = 1,   // folding into or out of the boat
    SHIP_STATUS_SETTLING = 2,  // settling onto the water
    SHIP_STATUS_SAILING = 3,
};

s32 marioShipChanging(void);
void marioShipForceStop(void);
void marioShipSetDispDir(void);
void mot_ship(void);
void mot_ship_post(void);
s32 marioShipGetDir(void);
BOOL marioChkShipPlace(HitEntry* hit);
