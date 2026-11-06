/* "evtmgr" - Event Manager
 * Helps manage all the scripts the game uses.
 */

#include "manager/evtmgr.h"
#include "evt/evt_msg.h"
#include "mario/mariost.h"
#include "memory.h"
#include "manager/evtmgr_cmd.h"
#include <string.h>

//.bss
static EventWork work[2];
s32 priTbl[0x100];
s32 priIDTbl[0x100];

//.sbss
s32 evtMax;
s32 priTblNum;
BOOL runMainF;

//.sdata
s32 evtID = 1;
f32 evtSpd = 1.0f;

/**
 * The event pool in use: battles run their scripts in a second pool of 256 entries (with its own GW/GF
 * variables), so field events survive a battle untouched.
 */
EventWork* evtGetWork(void) {
    return gp->inBattle ? &work[1] : &work[0];
}

#pragma dont_inline on
/**
 * Builds this frame's run order for evtmgrMain:
 * every active event's slot and id, sorted by priority, highest first.
 */
static void make_pri_table(void) {
    EventWork* wp = evtGetWork();
    EventEntry* entry = wp->entries;
    s32 idI, idJ;
    s32 slotI, slotJ;
    s32 priEntryCount;
    s32 n, i, j;

    priEntryCount = 0;
    for (n = 0; n < wp->count; n++, entry++) {
        if (entry->flags & EVENT_ACTIVE) {
            priTbl[priEntryCount] = n;
            priIDTbl[priEntryCount] = entry->eventId;
            priEntryCount++;
        }
    }
    priTblNum = priEntryCount;

    // Sort the run table by priority, highest first
    for (i = 0; i < priEntryCount - 1; i++) {
        for (j = i + 1; j < priEntryCount; j++) {
            slotI = priTbl[i];
            slotJ = priTbl[j];
            if (wp->entries[slotI].priority < wp->entries[slotJ].priority) {
                idI = priIDTbl[i];
                idJ = priIDTbl[j];
                priTbl[i] = slotJ;
                priIDTbl[i] = idJ;
                priTbl[j] = slotI;
                priIDTbl[j] = idI;
            }
        }
    }
}
#pragma dont_inline reset

#pragma warn_no_side_effect off
/**
 * Scans an event's script up to END_SCRIPT and records the position
 * after each LABEL (up to 16) for GOTO to jump to.
 */
static void make_jump_table(EventEntry* entry) {
    s32 params, opcode;
    s32 label;
    int i, n;
    EvtWord* cmd;

    for (i = 0; i < 16; i++) { //initialize both tables
        entry->labelIdTable[i] = -1;
        entry->labelAddressTable[i] = 0;
    }

    n = 0;
    cmd = entry->nextCommand;
    while (1) {
        opcode = *cmd & 0xFFFF; // low half of the command word: the opcode
        params = *cmd++ >> 16;  // high half: the argument count; cmd moves to the first argument
        label = *cmd;           // first argument: the label id of an OPCODE_LBL
        cmd += params;          // skip the arguments: cmd points at the next command

        switch (opcode) {
            case OPCODE_END_SCRIPT:
                return;
            case OPCODE_LBL:
                entry->labelIdTable[n] = (s8)label;
                entry->labelAddressTable[n] = cmd;
                n++;
                break;
        }

        // FAKE: empty test with no effect; without it the object changes in both versions
        if (n >= 16) {
            "unused";
        }
    }
}
#pragma warn_no_side_effect on

/**
 * Boot-time setup: allocates the field and battle event pools (256 entries each),
 * clears their GW/GF variables and the message state.
 */
void evtmgrInit(void) {
    EventWork* wp;

    wp = &work[0];
    wp->count = 0x100;
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(EventEntry));
    wp->lastUpdate = gp->renderTime;
    memset(wp->entries, 0, wp->count * sizeof(EventEntry));
    memset(wp->gwData, 0, sizeof(wp->gwData));
    memset(wp->gfData, 0, sizeof(wp->gfData));

    wp = &work[1];
    wp->count = 0x100;
    wp->entries = __memAlloc(HEAP_DEFAULT, wp->count * sizeof(EventEntry));
    wp->lastUpdate = gp->renderTime;
    memset(wp->entries, 0, wp->count * sizeof(EventEntry));
    memset(wp->gwData, 0, sizeof(wp->gwData));
    memset(wp->gfData, 0, sizeof(wp->gfData));
    evtMax = 0;
    priTblNum = 0;
    runMainF = FALSE;
    evt_msg_init();
}

/**
 * Kills every event in the current pool (GW/GF variables are kept);
 * runs on map changes and on entering and leaving battle.
 */
void evtmgrReInit(void) {
    EventWork* wp = evtGetWork();

    wp->lastUpdate = gp->renderTime;
    memset(wp->entries, 0, wp->count * sizeof(EventEntry));
    evtMax = 0;
    runMainF = FALSE;
    evt_msg_init();
}

/**
 * Re-applies the current system level (marioStSystemLevel: pause menus and the like) by stopping all events of
 * the types it suspends, so an event started while the game is paused starts paused too.
 */
static void evtEntryRunCheck(EventEntry* entry) {
    switch (_mariostSystemLevel) {
        case 1:
            evtStopAll(1);
            break;
        case 2:
            evtStopAll(2);
            break;
        case 3:
            evtStopAll(0x10);
            break;
        case 4:
            evtStopAll(EVENT_TYPE_DEFAULT);
            break;
        case 0:
            break;
    }
}

// BUG: none of the four entry functions (evtEntry, evtEntryType, evtChildEntry, evtBrotherEntry) checks that the search found a free entry; with the pool full they continue with the entry one past the array.
EventEntry* evtEntry(void* evtCode, s32 priority, u8 flags) {
    EventWork* wp = evtGetWork();
    EventEntry* entry;
    int i, id;

    //search until we find one not-in-use
    entry = wp->entries;
    for (i = 0, id = 0; i < wp->count; i++, id++, entry++) {
        if (!(entry->flags & EVENT_ACTIVE)) {
            break;
        }
    }

    evtMax++;
    memset(entry, 0, sizeof(EventEntry));
    entry->flags = (u8)(flags | EVENT_ACTIVE);
    entry->nextCommand = evtCode;
    entry->restartFrom = evtCode;
    entry->currCommand = evtCode;
    entry->opcode = OPCODE_NEXT;
    entry->waitingEvent = NULL;
    entry->waitingOnEvent = NULL;
    entry->brotherEvent = NULL;
    entry->priority = priority;
    entry->eventId = evtID++;
    entry->unitId = -1;
    entry->loopDepth = -1;
    entry->switchDepth = -1;
    entry->type = EVENT_TYPE_DEFAULT;
    entry->name = NULL;
    entry->timescale = evtSpd;
    entry->commandsLeft = 0.0f;
    entry->caseId = -1;
    entry->ownerNPC = NULL;
    entry->lifetime = 0;
    for (i = 0; i < 16; i++) {
        entry->lwData[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        entry->lfData[i] = 0;
    }

    make_jump_table(entry);

    if (runMainF && entry->flags & EVENT_START_IMMEDIATE) {
        priTbl[priTblNum] = id;
        priIDTbl[priTblNum] = entry->eventId;
        priTblNum++;
    }

    evtEntryRunCheck(entry);

    if (!evtID) {
        evtID = 1;
    }

    return entry;
}

/**
 * evtEntry with an explicit type mask, which decides which system levels pause the event (0 = never paused,
 * used for sounds and lectures that must play on while the game is paused).
 */
EventEntry* evtEntryType(void* evtCode, u8 priority, u8 flags, u8 type) {
    EventWork* wp = evtGetWork();
    EventEntry* entry;
    int i, id;

    //search until we find one not-in-use
    entry = wp->entries;
    for (i = 0, id = 0; i < wp->count; i++, id++, entry++) {
        if (!(entry->flags & EVENT_ACTIVE)) {
            break;
        }
    }

    evtMax++;
    memset(entry, 0, sizeof(EventEntry));
    entry->flags = (u8)(flags | EVENT_ACTIVE);
    entry->nextCommand = evtCode;
    entry->restartFrom = evtCode;
    entry->currCommand = evtCode;
    entry->opcode = OPCODE_NEXT;
    entry->waitingEvent = NULL;
    entry->waitingOnEvent = NULL;
    entry->brotherEvent = NULL;
    entry->priority = priority;
    entry->eventId = evtID++;
    entry->unitId = -1;
    entry->loopDepth = -1;
    entry->switchDepth = -1;
    entry->type = type;
    entry->name = NULL;
    entry->timescale = evtSpd;
    entry->commandsLeft = 0.0f;
    entry->caseId = -1;
    entry->ownerNPC = NULL;
    entry->lifetime = 0;
    for (i = 0; i < 16; i++) {
        entry->lwData[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        entry->lfData[i] = 0;
    }

    make_jump_table(entry);

    if (runMainF && entry->flags & EVENT_START_IMMEDIATE) {
        priTbl[priTblNum] = id;
        priIDTbl[priTblNum] = entry->eventId;
        priTblNum++;
    }
    evtEntryRunCheck(entry);
    if (!evtID) {
        evtID = 1;
    }
    return entry;
}

/**
 * Starts a child script for CALL_CHILD: the parent waits until it ends.
 * The child copies the parent's LW/LF variables, UW/UF bases, owner, unit, type and message window state,
 * runs one priority higher and runs this frame; evtDelete copies the variables back to the parent.
 */
EventEntry* evtChildEntry(EventEntry* parent, void* evtCode, u8 flags) {
    EventWork* wp = evtGetWork();
    EventEntry* entry;
    int i, id;

    //search until we find one not-in-use
    entry = wp->entries;
    for (i = 0, id = 0; i < wp->count; i++, id++, entry++) {
        if (!(entry->flags & EVENT_ACTIVE)) {
            break;
        }
    }

    evtMax++;
    parent->waitingOnEvent = entry;
    parent->flags |= EVENT_WAITING_ON_CHILD;
    memset(entry, 0, sizeof(EventEntry));
    entry->flags = (u8)(flags | EVENT_ACTIVE);
    entry->nextCommand = evtCode;
    entry->restartFrom = evtCode;
    entry->currCommand = evtCode;
    entry->opcode = OPCODE_NEXT;
    entry->waitingEvent = parent;
    entry->waitingOnEvent = NULL;
    entry->brotherEvent = NULL;
    entry->priority = (u8)(parent->priority + 1);
    entry->eventId = evtID++;
    entry->unitId = parent->unitId;
    entry->loopDepth = -1;
    entry->switchDepth = -1;
    entry->type = parent->type;
    entry->name = NULL;
    entry->uwBase = parent->uwBase;
    entry->ufBase = parent->ufBase;
    entry->timescale = evtSpd;
    entry->commandsLeft = 0.0f;
    entry->caseId = -1;
    entry->ownerNPC = parent->ownerNPC;
    entry->lifetime = 0;

    entry->selectWindowId = parent->selectWindowId;
    entry->printWindowId = parent->printWindowId;
    entry->printWindowFlags = parent->printWindowFlags;
    entry->msgPos = parent->msgPos;
    entry->msgPosZ = parent->msgPosZ;
    entry->msgPriority = parent->msgPriority;

    for (i = 0; i < 16; i++) {
        entry->lwData[i] = parent->lwData[i];
    }
    for (i = 0; i < 3; i++) {
        entry->lfData[i] = parent->lfData[i];
    }

    make_jump_table(entry);

    if (runMainF) {
        priTbl[priTblNum] = id;
        priIDTbl[priTblNum] = entry->eventId;
        priTblNum++;
    }

    evtEntryRunCheck(entry);

    if (!evtID) {
        evtID = 1;
    }
    return entry;
}

/**
 * Starts a script alongside brother (BEGIN_SIBLING_EVENT):
 * same priority, type, owner and copied variables, but the caller keeps running.
 * Deleting or stopping brother does the same to it.
 */
EventEntry* evtBrotherEntry(EventEntry* brother, void* evtCode, u8 flags) {
    EventWork* wp = evtGetWork();
    EventEntry* entry;
    int i, id;

    //search until we find one not-in-use
    entry = wp->entries;
    for (i = 0, id = 0; i < wp->count; i++, id++, entry++) {
        if (!(entry->flags & EVENT_ACTIVE)) {
            break;
        }
    }

    evtMax++;
    memset(entry, 0, sizeof(EventEntry));
    entry->flags = (u8)(flags | EVENT_ACTIVE);
    entry->nextCommand = evtCode;
    entry->restartFrom = evtCode;
    entry->currCommand = evtCode;
    entry->opcode = OPCODE_NEXT;
    entry->waitingEvent = NULL;
    entry->brotherEvent = brother;
    entry->waitingOnEvent = NULL;
    entry->priority = brother->priority;
    entry->eventId = evtID++;
    entry->unitId = brother->unitId;
    entry->loopDepth = -1;
    entry->switchDepth = -1;
    entry->type = brother->type;
    entry->name = NULL;
    entry->uwBase = brother->uwBase;
    entry->ufBase = brother->ufBase;
    entry->timescale = evtSpd;
    entry->commandsLeft = 0.0f;
    entry->caseId = -1;
    entry->ownerNPC = brother->ownerNPC;
    entry->lifetime = 0;

    for (i = 0; i < 16; i++) {
        entry->lwData[i] = brother->lwData[i];
    }
    for (i = 0; i < 3; i++) {
        entry->lfData[i] = brother->lfData[i];
    }

    make_jump_table(entry);

    if (runMainF) {
        priTbl[priTblNum] = id;
        priIDTbl[priTblNum] = entry->eventId;
        priTblNum++;
    }

    if (!evtID) {
        evtID = 1;
    }

    evtEntryRunCheck(entry);

    return entry;
}

/** Starts an event's script over from its first command (loops, switches, timing and labels reset; variables kept). */
EventEntry* evtRestart(EventEntry* entry) {
    entry->currCommand = entry->nextCommand = entry->restartFrom;
    entry->opcode = OPCODE_NEXT;
    entry->timescale = 1.0f;
    entry->commandsLeft = 0.0f;
    entry->loopDepth = -1;
    entry->switchDepth = -1;
    entry->timescale = evtSpd;
    entry->commandsLeft = 0.0f;
    entry->lifetime = 0;
    make_jump_table(entry);
    evtEntryRunCheck(entry);
    return entry;
}

/**
 * Runs every active event once per frame (from mariost.c), highest priority first,
 * skipping stopped events and ones waiting on a child.
 * Each event gets one evtmgrCmd run (commands until one blocks or yields) per unit of its speed;
 * an evtmgrCmd result of 1 (a command returned a negative code) ends the whole pass for this frame.
 * Also advances each event's lifetime by the frame's time (capped after long stalls).
 */
void evtmgrMain(void) {
    EventWork* wp = evtGetWork();
    EventEntry* entry;
    BOOL finished;
    OSTime delta;
    int i, ret;
    s32* entryNum;
    s32* eventId;

    runMainF = TRUE; // entries created while this runs are added to this frame's run table

    delta = gp->renderTime - wp->lastUpdate;
    if (delta < 0) {
        delta = 0;
    }

    if (OSTicksToMilliseconds(delta) > 500) {
        delta = OSMillisecondsToTicks(1000 / gp->framerate);
    }
    wp->lastUpdate = gp->renderTime;
    make_pri_table();

    entryNum = priTbl;
    eventId = priIDTbl;
    for (i = 0; i < priTblNum; i++) {
        entry = &wp->entries[*entryNum];
        if ((entry->flags & EVENT_ACTIVE) && (entry->eventId == *eventId)) {
            if (!(entry->flags & (0x80 | EVENT_WAITING_ON_CHILD | EVENT_STOPPED))) {
                finished = FALSE;
                entry->lifetime += delta;
                entry->commandsLeft += entry->timescale;

                while (TRUE) {
                    if (entry->commandsLeft < 1.0f) {
                        break;
                    }
                    entry->commandsLeft -= 1.0f;
                    ret = evtmgrCmd(entry);
                    if (ret == 1) {
                        finished = TRUE;
                        break;
                    }
                    if (ret == -1) {
                        break;
                    }
                }

                if (finished != FALSE) {
                    break;
                }
            }
        }
        entryNum++;
        eventId++;
    }
    runMainF = FALSE;
}

/**
 * Ends an event together with its child and all its brother events.
 * A child hands its LW/LF variables and message window state back to the parent, which then resumes.
 */
void evtDelete(EventEntry* entry) {
    EventWork* wp = evtGetWork();
    EventEntry* waiting;
    int i;

    if (entry->flags & EVENT_ACTIVE) {
        if (entry->waitingOnEvent) {
            evtDelete(entry->waitingOnEvent);
        }

        waiting = wp->entries;
        for (i = 0; i < wp->count; i++, waiting++) {
            if ((waiting->flags & EVENT_ACTIVE) && (waiting->brotherEvent == entry)) {
                evtDelete(waiting);
            }
        }

        waiting = entry->waitingEvent;
        if (waiting != NULL) {
            waiting->flags &= ~EVENT_WAITING_ON_CHILD;
            waiting->waitingOnEvent = NULL;
            for (i = 0; i < 16; i++) {
                waiting->lwData[i] = entry->lwData[i];
            }
            for (i = 0; i < 3; i++) {
                waiting->lfData[i] = entry->lfData[i];
            }
            waiting->selectWindowId = entry->selectWindowId;
            waiting->printWindowId = entry->printWindowId;
            waiting->printWindowFlags = entry->printWindowFlags;
            waiting->msgPos = entry->msgPos;
            waiting->msgPosZ = entry->msgPosZ;
            waiting->msgPriority = entry->msgPriority;
        }
        entry->flags &= ~EVENT_ACTIVE;
        memset(entry, 0, sizeof(EventEntry));
        evtMax--;
    }
}

/** Ends the event with this id (and its children and brothers) if it is still running. */
void evtDeleteID(s32 eventId) {
    EventWork* wp = evtGetWork();
    EventEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & EVENT_ACTIVE) && (entry->eventId == eventId)) {
            evtDelete(entry);
        }
    }
}

/** TRUE while the event with this id is still running; scripts poll it through IS_EVENT_RUNNING. */
BOOL evtCheckID(s32 eventId) {
    int i;
    EventWork* wp = evtGetWork();
    EventEntry* entry = wp->entries;

    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & EVENT_ACTIVE) && (entry->eventId == eventId)) {
            return TRUE;
        }
    }
    return FALSE;
}

/** Changes an event's priority; takes effect from the next frame's ordering. */
void evtSetPri(EventEntry* entry, u32 priority) {
    entry->priority = (u8)priority;
}

/** Sets how many commands per frame the event runs (speed times the global evtSpd). */
void evtSetSpeed(EventEntry* entry, f32 speed) {
    entry->timescale = speed * evtSpd;
}

/** Changes an event's type mask, the set of system levels that pause it. */
void evtSetType(EventEntry* entry, u32 type) {
    entry->type = (u8)type;
}

/** Pauses an event, its child and brother events, each only if its type mask shares a bit with type. */
void evtStop(EventEntry* entry, u32 type) {
    EventWork* wp = gp->inBattle ? &work[1] : &work[0];
    EventEntry* brother;
    int i;

    if (entry->waitingOnEvent) {
        evtStop(entry->waitingOnEvent, type);
    }

    brother = wp->entries;
    for (i = 0; i < wp->count; i++, brother++) {
        if ((brother->flags & EVENT_ACTIVE) && (brother->brotherEvent == entry)) {
            evtStop(brother, type);
        }
    }

    if (entry->type & type) {
        entry->flags |= EVENT_STOPPED;
    }
}

/** Resumes events paused by evtStop, again only those whose type mask shares a bit with type. */
void evtStart(EventEntry* entry, u32 type) {
    EventWork* wp = gp->inBattle ? &work[1] : &work[0];
    EventEntry* brother;
    int i;

    if (entry->waitingOnEvent) {
        evtStart(entry->waitingOnEvent, type);
    }

    brother = wp->entries;
    for (i = 0; i < wp->count; i++, brother++) {
        if ((brother->flags & EVENT_ACTIVE) && (brother->brotherEvent == entry)) {
            evtStart(brother, type);
        }
    }

    if (entry->type & type) {
        entry->flags &= ~EVENT_STOPPED;
    }
}

/** Pauses the event with this id (and its children and brothers) for type mask EVENT_TYPE_DEFAULT (every bit but 0x10). */
void evtStopID(s32 eventId) {
    EventWork* wp = evtGetWork();
    EventEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & EVENT_ACTIVE) && (entry->eventId == eventId)) {
            evtStop(entry, EVENT_TYPE_DEFAULT);
        }
    }
}

/** Resumes the event with this id (and its children and brothers) for type mask EVENT_TYPE_DEFAULT (every bit but 0x10). */
void evtStartID(s32 eventId) {
    EventWork* wp = evtGetWork();
    EventEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & EVENT_ACTIVE) && (entry->eventId == eventId)) {
            evtStart(entry, EVENT_TYPE_DEFAULT);
        }
    }
}

/** Pauses every event whose type matches; marioStSystemLevel uses it when menus or cutscenes freeze the game. */
void evtStopAll(u32 type) {
    EventWork* wp = evtGetWork();
    EventEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & EVENT_ACTIVE) {
            evtStop(entry, type);
        }
    }
}

/** Resumes every event whose type matches; marioStSystemLevel(0) resumes them all. */
void evtStartAll(u32 type) {
    EventWork* wp = evtGetWork();
    EventEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if (entry->flags & EVENT_ACTIVE) {
            evtStart(entry, type);
        }
    }
}

/** Pauses every matching event except entry (its own children and brothers too, if they match). */
void evtStopOther(EventEntry* entry, u32 type) {
    EventWork* wp = evtGetWork();
    EventEntry* search = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, search++) {
        if ((search->flags & EVENT_ACTIVE) && (search != entry)) {
            evtStop(search, type);
        }
    }
}

/** Resumes every matching event except entry. */
void evtStartOther(EventEntry* entry, u32 type) {
    EventWork* wp = evtGetWork();
    EventEntry* search = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, search++) {
        if ((search->flags & EVENT_ACTIVE) && (search != entry)) {
            evtStart(search, type);
        }
    }
}

/** The event in pool slot index, or NULL if that slot is free. */
EventEntry* evtGetPtr(s32 index) {
    EventWork* wp = evtGetWork();
    EventEntry* entry = &wp->entries[index];
    if (entry->flags & EVENT_ACTIVE) {
        return entry;
    }
    return NULL;
}

/** The running event with this id, or NULL once it has ended; ids are what scripts store to refer to events. */
EventEntry* evtGetPtrID(s32 eventId) {
    EventWork* wp = evtGetWork();
    EventEntry* entry = wp->entries;
    int i;

    for (i = 0; i < wp->count; i++, entry++) {
        if ((entry->flags & EVENT_ACTIVE) && (entry->eventId == eventId)) {
            return entry;
        }
    }
    return NULL;
}
