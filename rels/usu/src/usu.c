// usu.rel (ウスグラ村, Twilight Town): registers usu_00 and usu_01.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "data/mapdata.h"
#include "memory.h"
#include "string.h"
#include "data/party_data.h"
#include "usu.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
static void* pose_buf;

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/**
 * Script helper: replaces entry args[0] of Vivian's pose table (partyPoseTbl[6]) with the pose name string in
 * args[1]; the copy is restored in _epilog.
 */
USER_FUNC(evt_usu_vivian_pose_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    partyPoseTbl[PARTY_VIVIAN][index] = (char*)evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/**
 * REL entry point for usu.rel: runs the static constructors, registers the init scripts of usu_00 and usu_01,
 * and saves a copy of Vivian's party pose table so the pose edits can be undone on unload.
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("usu_00", usu_00_init_evt);
    relSetEvtAddr("usu_01", usu_01_init_evt);
    pose_buf = _mapAlloc(sizeof(partyPoseTbl[PARTY_VIVIAN]));
    memcpy(pose_buf, partyPoseTbl[PARTY_VIVIAN], sizeof(partyPoseTbl[PARTY_VIVIAN]));
}

/**
 * REL exit point for usu.rel: runs the static destructors
 * and restores Vivian's original pose table from the copy made in _prolog.
 */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    memcpy(partyPoseTbl[PARTY_VIVIAN], pose_buf, sizeof(partyPoseTbl[PARTY_VIVIAN]));
    _mapFree(pose_buf);
}

/** Handler called when an unresolved import in usu.rel is hit; does nothing. */
void _unresolved(void) {
}

