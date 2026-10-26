#pragma once

#include <dolphin/types.h>
#include "party.h"

struct HitEntry;

void marioSetBottomlessResetPosition(f32 x, f32 y, f32 z);
f32 revise360(f32 angle);
f32 toMovedir(f32 angle);
void marioAdjustMoveDir(void);

s32 N_marioGetMovementSpeed(void);
void N_marioReloadMapOnBottomlessOn(void);
void marioSetPeakYpos(void);
f32 partyToMovedir(PartyEntry* party, f32 angle);
f32 toMovedir2(f32 angle, f32 ofs);
f32 toMovedirSimple(f32 angle);

void N_marioSetBottomlessRespawnPosOnBeroEntry(f32 x, f32 y, f32 z);
void N_marioReloadMapOnBottomlessOff(void);
void N_marioSetBottomlessRespawnPos(f32 x, f32 y, f32 z);
void marioBottomless(void);
void marioForceMoveMain(void);
void marioGetRubInit(void);
void marioGetStick(f32* dir, f32* mag);
void marioResetGravity(void);
void marioUpdateKeyData(void);
void* marioGetTouchNpcPtr(void);
BOOL marioSetDirEventMain(void);
BOOL marioChkPlanePlace(struct HitEntry* hit);
