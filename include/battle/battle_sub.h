#pragma once

#include "battle/battle.h"
#include "manager/evtmgr.h"

void btlsubResetMoveColorLvAll(BattleWork* work);
s32 BattleTransID(EventEntry* event, s32 type);

void btlMovePos(f32 speed, f32 angle, f32* x, f32* z);
s32 BtlCompForwardLv(s32 dir, f32 x);
f32 atan2f_safety(f32 y, f32 x);
f32 cosfd(f32 deg);
f32 sinfd(f32 deg);
