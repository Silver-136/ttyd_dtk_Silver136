#pragma once

#include "party.h"
#include "driver/hitdrv.h"
#include "dolphin/mtx.h"
#include "dolphin/types.h"

struct HitEntry;
struct PartyEntry;

void partyChkGnd(PartyEntry* party);
HitEntry* partyHitCheck(PartyEntry* party, Vec* pos, Vec* dir, Vec* outPos, Vec* outNrm, f32* dist);
HitEntry* partySearchFrontWall(PartyEntry* party, Vec* pos, f32 dist, f32 ang);
HitEntry* partySearchGround(PartyEntry* party, f32 range, f32 depth);
HitEntry* partySearchWallFront(PartyEntry* party, Vec* pos, f32 dist, f32 ang);
HitEntry* partyChkFrontStep(PartyEntry* party, f32 range, f32* outY, f32* outDist, f32* outAng);
BOOL partyChkBetweenWalls(PartyEntry* party, Vec pos);
void partyChkWallAround(PartyEntry* party, Vec* pos, f32 spd, f32 dir, f32 width, f32 height);
struct HitEntry* partySearchHead(struct PartyEntry* party, Vec* pos, f32* dist, f32 angle);
