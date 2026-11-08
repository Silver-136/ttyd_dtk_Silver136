// jin (ダイム寺院, Creepy Steeple): shared code for the whole area: REL prolog/epilog (registers jin_00 to jin_11 and
// battle tables, swaps the Vivian pose table) and evt_jin_vivian_pose_set.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "data/mapdata.h"
#include "memory.h"
#include "string.h"
#include "data/party_data.h"
#include "jin.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
static void* pose_buf;

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/**
 * Script helper: sets entry args[0] of Vivian's pose table (partyPoseTbl[6]) to the animation name args[1],
 * so Vivian uses different poses inside the steeple; the original table is restored when the REL unloads.
 */
USER_FUNC(evt_jin_vivian_pose_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    partyPoseTbl[6][index] = (char*)evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * Module entry point of jin.rel: runs the static constructors,
 * registers the init scripts of jin_00..jin_11 and the area's battle tables,
 * and keeps a copy of Vivian's pose table so scripts can change it.
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("jin_00", jin_00_init_evt);
    relSetEvtAddr("jin_01", jin_01_init_evt);
    relSetEvtAddr("jin_02", jin_02_init_evt);
    relSetEvtAddr("jin_03", jin_03_init_evt);
    relSetEvtAddr("jin_04", jin_04_init_evt);
    relSetEvtAddr("jin_05", jin_05_init_evt);
    relSetEvtAddr("jin_06", jin_06_init_evt);
    relSetEvtAddr("jin_07", jin_07_init_evt);
    relSetEvtAddr("jin_08", jin_08_init_evt);
    relSetEvtAddr("jin_09", jin_09_init_evt);
    relSetEvtAddr("jin_10", jin_10_init_evt);
    relSetEvtAddr("jin_11", jin_11_init_evt);
    relSetBtlAddr("jin", btlsetup_jin_tbl, btl_jin_setup_no_tbl);
    pose_buf = _mapAlloc(sizeof(partyPoseTbl[6]));
    memcpy(pose_buf, partyPoseTbl[6], sizeof(partyPoseTbl[6]));
}

/** Module exit point of jin.rel: runs the static destructors and restores Vivian's original pose table. */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    memcpy(partyPoseTbl[6], pose_buf, sizeof(partyPoseTbl[6]));
    _mapFree(pose_buf);
}

/** Linker stub for unresolved symbols; does nothing. */
void _unresolved(void) {
}
