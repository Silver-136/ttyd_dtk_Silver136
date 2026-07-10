#include "battle/battle_unit_event.h"

void* BattleGetUnitPtr(void* battleWork, s32 unitId);
s32 evtCheckID(s32 eventId);
s32 BattleCheckEndUnitInitEvent(void* battleWork);
void* evtEntry(void* script, s32 priority, s32 flags);
void evtDeleteID(s32 eventId);
s32 BtlUnit_CheckData(void* unit, s32 id);

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


s32 BattleRunHitEventDirect(void* unit, s32 flags, void* script) {
    s32 eventId;
    s32 result;
    void* event;

    result = 0;

    eventId = *(s32*)((s32)unit + 0x2B4);
    if (eventId != 0) {
        evtDeleteID(eventId);
        *(s32*)((s32)unit + 0x2B4) = 0;
    }

    eventId = *(s32*)((s32)unit + 0x290);
    if (eventId != 0) {
        evtDeleteID(eventId);
        *(s32*)((s32)unit + 0x290) = 0;
    }

    eventId = *(s32*)((s32)unit + 0x29C);
    if (eventId != 0) {
        evtDeleteID(eventId);
        *(s32*)((s32)unit + 0x29C) = 0;
    }

    eventId = *(s32*)((s32)unit + 0x2A8);
    if (eventId != 0) {
        evtDeleteID(eventId);
        *(s32*)((s32)unit + 0x2A8) = 0;
    }

    if (script != 0) {
        event = evtEntry(script, 10, 0x20);
        *(s32*)((s32)event + 0x160) = *(s32*)unit;
        *(s32*)((s32)unit + 0x278) = flags;
        result = *(s32*)((s32)event + 0x15C);
        *(s32*)((s32)unit + 0x2B4) = result;
    }

    return result;
}


s32 BattleRunPhaseEvent(void* unit, int bUnisonPhase) {
    void* script;
    void* event;
    s32 result;

    event = 0;
    result = 0;

    if (bUnisonPhase != 0) {
        script = *(void**)((s32)unit + 0x294);
        if (script != 0) {
            event = evtEntry(script, 10, 0x20);
        }
    } else {
        script = *(void**)((s32)unit + 0x298);
        if (script != 0) {
            event = evtEntry(script, 10, 0x20);
        }
    }

    if (event != 0) {
        *(s32*)((s32)event + 0x160) = *(s32*)unit;
        result = *(s32*)((s32)event + 0x15C);
        *(s32*)((s32)unit + 0x29C) = result;
    }

    return result;
}


s32 BattleRunHitEvent(void* unit, u32 param_2) {
    s32 result;
    s32 allow;
    void* script;

    result = 0;
    allow = 1;
    if ((u8)param_2 == 0x28) {
        if (BtlUnit_CheckData(unit, 0x2E) == 0) {
            allow = 0;
        }
    }

    if (allow == 0) {
        return 0;
    }

    script = *(void**)((s32)unit + 0x2B0);
    if (script != 0) {
        result = BattleRunHitEventDirect(unit, param_2, script);
    }

    return result;
}


void BattlePhaseEventStartDeclare(void* unit) {
    s32 eventId;

    eventId = *(s32*)((s32)unit + 0x2B4);
    if (eventId != 0) {
        evtDeleteID(eventId);
        *(s32*)((s32)unit + 0x2B4) = 0;
    }

    eventId = *(s32*)((s32)unit + 0x290);
    if (eventId != 0) {
        evtDeleteID(eventId);
        *(s32*)((s32)unit + 0x290) = 0;
    }

    eventId = *(s32*)((s32)unit + 0x2A8);
    if (eventId != 0) {
        evtDeleteID(eventId);
        *(s32*)((s32)unit + 0x2A8) = 0;
    }
}

s32 BattleRunWaitEvent(void* unit) {
    void* event;
    s32 eventId;

    eventId = *(s32*)((s32)unit + 0x290);
    if (eventId != 0) {
        evtDeleteID(eventId);
        *(s32*)((s32)unit + 0x290) = 0;
    }

    if (*(void**)((s32)unit + 0x28C) != 0) {
        event = evtEntry(*(void**)((s32)unit + 0x28C), 10, 0);
        *(s32*)((s32)event + 0x160) = *(s32*)unit;
        *(s32*)((s32)unit + 0x290) = *(s32*)((s32)event + 0x15C);
    }

    return 0;
}

