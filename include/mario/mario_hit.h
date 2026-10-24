#pragma once

#include "dolphin/types.h"
#include "driver/hitdrv.h"
#include "dolphin/mtx.h"

BOOL marioChkBero(HitEntry* hit);
HitEntry* marioHitCheckVec(Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist);

HitEntry* dou10_yoko_yari(f32* out);
HitEntry* marioChkFrontStep(f32* y, f32* h, f32* ang);
HitEntry* marioHitCheck2(Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist);
HitEntry* marioLandonChk_old(f32* outY, f32 spd);
void marioSearchFrontWall_staying(f32 dz);
HitEntry* marioSearchGround(f32* y, f32* b, f32* c, f32 spd, f32 h);
HitEntry* marioSearchGroundRoll(f32* y, f32* b, f32* c, f32 spd, f32 h);
HitEntry* marioSearchHead_jabara(Vec* pos, f32* h, HitEntry** out, f32 d);
HitEntry* marioSearchHead_roll(Vec* pos, f32* h, f32 d);
HitEntry* searchFrontWall(Vec* pos, f32* h, f32 dx, f32 dz, f32 w);

void marioChkGnd(void);
BOOL marioChkWallAround(Vec* pos, s32 mode, f32 unused, f32 dir, f32 w, f32 h);
