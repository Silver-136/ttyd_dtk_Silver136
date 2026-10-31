#include "battle/battle_unit_event.h"

/**
 * Checks whether every unit's init event script has finished, clearing the ids of those that have;
 * returns TRUE once none are still running. Polled by the battle start sequence after Btl_UnitSetup.
 */
BOOL BattleCheckEndUnitInitEvent(BattleWork* wp) {
    BattleUnit* unit;
    BOOL valid;
    s32 i, eventId;

    valid = TRUE;
    for (i = 0; i < 64; i++) {
        unit = BattleGetUnitPtr(wp, i);
        if (unit) {
            eventId = unit->initEventId;
            if (eventId) {
                if (evtCheckID(eventId)) {
                    valid = FALSE;
                } else {
                    unit->initEventId = 0;
                }
            }
        }
    }
    return valid;
}

/**
 * Starts the given script as a unit's damage reaction:
 * kills its running damage, wait, phase and attack scripts,
 * stores damageCode in unit->damageCode for the script to read
 * and returns the new event id (0 when code is NULL).
 */
s32 BattleRunHitEventDirect(BattleUnit* unit, s32 damageCode, void* code) {
    EventEntry* evt;
    s32 id, newId;

    newId = 0;
    id = unit->damageEventId;
    if (id) {
        evtDeleteID(id);
        unit->damageEventId = 0;
    }

    id = unit->waitEventId;
    if (id) {
        evtDeleteID(id);
        unit->waitEventId = 0;
    }

    id = unit->phaseEventId;
    if (id) {
        evtDeleteID(id);
        unit->phaseEventId = 0;
    }

    id = unit->attackEventId;
    if (id) {
        evtDeleteID(id);
        unit->attackEventId = 0;
    }

    if (code) {
        evt = evtEntry(code, 0xA, EVENT_START_IMMEDIATE);
        evt->unitId = unit->unitId;
        unit->damageCode = damageCode;
        newId = evt->eventId;
        unit->damageEventId = newId;
    }
    return newId;
}

/**
 * Runs the unit's own damage reaction script (damageEventCode) for a hit, via BattleRunHitEventDirect.
 * A hit code with low byte 0x28 is skipped unless the unit has data tag 0x2E.
 * Called from the damage code and btlevtcmd scripts.
 */
s32 BattleRunHitEvent(BattleUnit* unit, s32 damageCode) {
    void* code;
    s32 id;
    BOOL valid;

    id = 0;
    valid = TRUE;
    if ((u8)damageCode == DAMAGE_CODE_MISS && !BtlUnit_CheckData(unit, UNIT_DATA_AVOID_MISS_EVENT)) {
        valid = FALSE;
    }
    if (!valid) {
        return 0;
    }
    code = unit->damageEventCode;
    if (code) {
        id = BattleRunHitEventDirect(unit, damageCode, code);
    }
    return id;
}

/**
 * Starts the unit's phase script (its unison variant when isUnison is set)
 * and records its id in unit->phaseEventId.
 * The phase-change sequence in battle_seq.c runs every unit's unison script together,
 * then the plain ones one at a time.
 */
s32 BattleRunPhaseEvent(BattleUnit* unit, BOOL isUnison) {
    EventEntry* evt = NULL;
    s32 id = 0;
    void* code;

    if (isUnison) {
        code = unit->unisonPhaseEventCode;
        if (code) {
            evt = evtEntry(code, 0xA, EVENT_START_IMMEDIATE);
        }
    } else {
        code = unit->phaseEventCode;
        if (code) {
            evt = evtEntry(code, 0xA, EVENT_START_IMMEDIATE);
        }
    }
    if (evt) {
        evt->unitId = unit->unitId;
        id = evt->eventId;
        unit->phaseEventId = id;
    }
    return id;
}

/**
 * Kills a unit's running damage, wait and attack scripts but not its phase script;
 * a phase script calls this through btlevtcmd_PhaseEventStartDeclare before it takes over the unit.
 */
void BattlePhaseEventStartDeclare(BattleUnit* unit) {
    s32 id;

    id = unit->damageEventId;
    if (id) {
        evtDeleteID(id);
        unit->damageEventId = 0;
    }

    id = unit->waitEventId;
    if (id) {
        evtDeleteID(id);
        unit->waitEventId = 0;
    }

    id = unit->attackEventId;
    if (id) {
        evtDeleteID(id);
        unit->attackEventId = 0;
    }
}

/** Restarts the unit's idle (wait) script, replacing any running one. Always returns 0. */
s32 BattleRunWaitEvent(BattleUnit* unit) {
    EventEntry* evt;
    void* code;
    s32 id;

    id = unit->waitEventId;
    if (id) {
        evtDeleteID(id);
        unit->waitEventId = 0;
    }

    code = unit->waitEventCode;
    if (code) {
        evt = evtEntry(code, 0xA, 0);
        evt->unitId = unit->unitId;
        unit->waitEventId = evt->eventId;
    }
    return 0; //unused
}
