// Declares the battle unit transition ids and movement and math helpers.

#pragma once

#include "battle/battle.h"
#include "manager/evtmgr.h"

// Special unit ids that BattleTransID resolves; event scripts pass them where a unit id is expected.
enum {
    UNIT_ID_LECTURE = -6, // BattleWork.lectureUnitId
    UNIT_ID_SYSTEM = -5,  // the system unit
    UNIT_ID_PARTNER = -4, // the current partner (UNIT_ID_NONE when there is none)
    UNIT_ID_MARIO = -3,
    UNIT_ID_SELF = -2,    // the unit the script runs for (EventEntry.unitId)
    UNIT_ID_NONE = -1,    // no unit: BattleTransID passes it through and returns it for an absent unit
};

void btlsubResetMoveColorLvAll(BattleWork* work);
s32 BattleTransID(EventEntry* event, s32 type);

void btlMovePos(f32 speed, f32 angle, f32* x, f32* z);
s32 BtlCompForwardLv(s32 dir, f32 x);
f32 atan2f_safety(f32 y, f32 x);
f32 cosfd(f32 deg);
f32 sinfd(f32 deg);

f32 intpl_sub(s32 mode, f32 start, f32 end, s32 cur, s32 steps);
