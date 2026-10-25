#pragma once

#include "dolphin/types.h"
#include "dolphin/mtx.h"

void kpaAddCoin(s32 coins);
void kpaAddScore(s32 score);
void kpaAddScorePos(s32 score, Vec* pos);

extern const Vec vec3_802fe200;
void kpaAddCoinPos(s32 coins, Vec* pos);
