#pragma optimize_for_size on
#include "battle/battle_sub.h"
#include "battle/battle_unit.h"
#include "battle/battle.h"
double cos(double);
double sin(double);
double atan2(double, double);

f32 sinfd(f32 deg) {
	return sin((6.2831855f * deg) / 360.0f);
}

f32 cosfd(f32 deg) {
	return cos((6.2831855f * deg) / 360.0f);
}

f32 atan2f_safety(f32 y, f32 x) {
	f32 a;

	a = (x >= 0.0f) ? x : -x;
	if (a < 0.0001) {
		x = 0.0001f;
	}
	a = (y >= 0.0f) ? y : -y;
	if (a < 0.0001) {
		y = 0.0001f;
	}
	return atan2(y, x);
}

void btlMovePos(f32 speed, f32 angle, f32* x, f32* z) {
	f32 s;
	f32 c;
	f32 rad = (6.2831855f * angle) / 360.0f;
	f32 dx;
	f32 dz;

	s = sin(rad);
	c = cos(rad);
	dx = speed * s;
	dz = -speed * c;

	*x += dx;
	*z += dz;
}

f32 intpl_sub(s32 mode, s32 cur, s32 steps, f32 start, f32 end) {
	f32 diff;
	f32 remain;
	f32 c;

	switch (mode) {
		case 0:
			return start + (cur * (end - start)) / steps;
		case 1:
			return start + ((end - start) * (cur * cur)) / (steps * steps);
		case 2:
			return start + ((end - start) * (cur * (cur * cur))) / (steps * (steps * steps));
		case 3:
			return start + ((end - start) * (cur * (cur * (cur * cur)))) / (steps * (steps * (steps * steps)));
		case 7:
			c = cos(4.0f * (3.141592f * ((f32)cur / (f32)steps)));
			diff = end - start;
			return end - ((f32)(steps - cur) * ((f32)(steps - cur) * (diff * c))) / ((f32)steps * (f32)steps);
		case 8:
			c = cos(4.0f * (3.141592f * ((f32)(cur * cur) / (f32)steps)) / 15.0f);
			diff = end - start;
			return end - ((f32)(steps - cur) * ((f32)(steps - cur) * (diff * c))) / ((f32)steps * (f32)steps);
		case 4:
			diff = end - start;
			return (start + diff) - (diff * ((steps - cur) * (steps - cur))) / (steps * steps);
		case 5:
			diff = end - start;
			return (start + diff) - (diff * ((steps - cur) * ((steps - cur) * (steps - cur)))) / (steps * (steps * steps));
		case 6:
			diff = end - start;
			return (start + diff) - (diff * ((steps - cur) * ((steps - cur) * ((steps - cur) * (steps - cur))))) / (steps * (steps * (steps * steps)));
		case 10:
			c = cos(4.0f * (3.141592f * ((f32)(cur * cur) / (f32)steps)) / 40.0f);
			remain = (f32)(steps - cur); // FAKE: half-used temp; direct expr and a rem temp used twice both differ
			c = ((f32)(steps - cur) * (remain * c)) / ((f32)steps * (f32)steps);
			if (c < 0.0f) {
				c = -c;
			}
			return end - c * (end - start);
		case 11:
			c = cos(3.141592f * (f32)cur / (f32)steps);
			diff = end - start;
			return start + diff * (1.0f - c) / 2.0f;
		case 12:
			c = sin(1.570796f * (f32)cur / (f32)steps);
			diff = end - start;
			return start + diff * c;
		case 13:
			c = cos(1.570796f * (f32)cur / (f32)steps);
			diff = end - start;
			return start + diff * (1.0f - c);
	}
	return 0.0f;
}

s32 BtlCompForwardLv(s32 dir, f32 x) {
	s32 lv = 0;

	if (x > -800.0f) {
		lv = (s32)((800.0f + x) / 40.0f);
		if (lv > 48) {
			lv = 48;
		}
	}
	if (dir >= 0) {
		return lv;
	}
	return 48 - lv;
}

s32 BattleTransID(EventEntry* evt, s32 type) {
	BattleWork* work = _battleWorkPointer;
	BattleUnit* unit;

	if (type < 0) {
		switch (type) {
			case -2:
				return evt->unitId;
			case -3:
				unit = BattleGetMarioPtr(work);
				return unit->unitId;
			case -4:
				unit = BattleGetPartyPtr(work);
				if (unit) {
					return unit->unitId;
				}
				return -1;
			case -1:
				return -1;
			case -5:
				unit = BattleGetSystemPtr(work);
				return unit->unitId;
			case -6:
				return work->lectureUnitId;
		}
	}
	return type;
}

void btlsubResetMoveColorLvAll(BattleWork* work) {
	BattleWork* w = work;
	BattleUnit* unit;
	int i;
	BattleWork* w2;
	int c;

	for (i = 0; i < 64; i++) {
		w2 = w;
		unit = BattleGetUnitPtr(w2, i);
		c = 0xFF;
		if (unit) {
			unit->mMoveColorLv = c;
		}
	}
}

