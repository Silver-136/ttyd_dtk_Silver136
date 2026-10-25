#pragma once

#include "battle/battle.h"
#include "manager/evtmgr.h"

// Special unit ids that BattleTransID resolves; event scripts pass them where a unit id is expected (-1 stays -1).
enum {
    UNIT_ID_LECTURE = -6, // BattleWork.lectureUnitId
    UNIT_ID_SYSTEM = -5,  // the system unit
    UNIT_ID_PARTNER = -4, // the current partner (-1 when there is none)
    UNIT_ID_MARIO = -3,
    UNIT_ID_SELF = -2,    // the unit the script runs for (EventEntry.unitId)
};

void btlsubResetMoveColorLvAll(BattleWork* work);
s32 BattleTransID(EventEntry* event, s32 type);

void btlMovePos(f32 speed, f32 angle, f32* x, f32* z);
s32 BtlCompForwardLv(s32 dir, f32 x);
f32 atan2f_safety(f32 y, f32 x);
f32 cosfd(f32 deg);
f32 sinfd(f32 deg);
