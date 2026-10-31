// eff_ice.c: ice-block effect: effIceEntry and the ice mesh face-data pointers.

#pragma once

#include "driver/effdrv.h"
#include "dolphin/types.h"

EffectEntry* effIceEntry(s32 type, s32 duration, f32 x, f32 y, f32 z, f32 scale);

extern s16* ice_effFaceDt_pPlane1;
extern s16* ice_effFaceDt_pPlane10;
extern s16* ice_effFaceDt_pPlane11;
extern s16* ice_effFaceDt_pPlane12;
extern s16* ice_effFaceDt_pPlane13;
extern s16* ice_effFaceDt_pPlane14;
extern s16* ice_effFaceDt_pPlane15;
extern s16* ice_effFaceDt_pPlane16;
extern s16* ice_effFaceDt_pPlane17;
extern s16* ice_effFaceDt_pPlane2;
extern s16* ice_effFaceDt_pPlane3;
extern s16* ice_effFaceDt_pPlane4;
extern s16* ice_effFaceDt_pPlane5;
extern s16* ice_effFaceDt_pPlane6;
extern s16* ice_effFaceDt_pPlane7;
extern s16* ice_effFaceDt_pPlane8;
extern s16* ice_effFaceDt_pPlane9;
