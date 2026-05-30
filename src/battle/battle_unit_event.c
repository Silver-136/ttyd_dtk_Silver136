#include "battle/battle_unit_event.h"

void* BattleGetUnitPtr(void* battleWork, s32 unitId);
s32 evtCheckID(s32 eventId);
s32 BattleCheckEndUnitInitEvent(void* battleWork);

s32 BattleCheckEndUnitInitEvent(void* battleWork) {
    void* unit;
    s32 result;
    s32 i;
    s32 zero;

    result = 1;
    i = 0;
    zero = 0;

    while (i < 0x40) {
        unit = BattleGetUnitPtr(battleWork, i);

        if (unit != 0) {
            if (*(s32*)((s32)unit + 0x288) != 0) {
                if (evtCheckID(*(s32*)((s32)unit + 0x288)) != 0) {
                    result = 0;
                } else {
                    *(s32*)((s32)unit + 0x288) = zero;
                }
            }
        }

        i++;
    }

    return result;
}


void BattleRunHitEventDirect(void* unit, s32 flags, s32 arg) {
}


s32 BattleRunPhaseEvent(void* unit, int bUnisonPhase) {
    return 0;
}


s32 BattleRunHitEvent(void* unit, u32 param_2) {
    return 0;
}


void BattlePhaseEventStartDeclare(void* unit) {
}


void BattleRunWaitEvent(void* unit) {
}
