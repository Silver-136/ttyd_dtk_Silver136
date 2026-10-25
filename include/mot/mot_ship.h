#pragma once

#include "dolphin/types.h"

BOOL marioChkShipMoveMode(void);
void marioShipChgFall(void);
BOOL marioChkSwirlPower(void);
void marioForceShipAnime(void);
void marioReInit_ship(void);
void marioSetSwirlPower(s32 msec, f32 a, f32 b);
s32 marioShipChanging(void);
void marioShipForceStop(void);
void marioShipSetDispDir(void);
void mot_ship(void);
void mot_ship_post(void);
s32 N_marioShipGetDir(void);
BOOL marioChkShipPlace(HitEntry* hit);
