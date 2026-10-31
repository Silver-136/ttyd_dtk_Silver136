#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"

#include "data/mapdata.h"
#include "memory.h"
#include "string.h"
#include "data/party_data.h"
#include "gra.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
static void* pose_buf;

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/**
 * Script helper for Vivian's pose table in Twilight Trail:
 * sets entry args[0] of partyPoseTbl[6] (Vivian's party poses, slot 6) to the animation name string args[1],
 * e.g. "PTR_S_4" for her idle pose. The gra cutscenes use it to change how she stands, walks and runs.
 */
USER_FUNC(evt_gra_vivian_pose_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    partyPoseTbl[PARTY_MEMBER_VIVIAN][index] = (char*)evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("gra_00", gra_00_init_evt);
    relSetEvtAddr("gra_01", gra_01_init_evt);
    relSetEvtAddr("gra_02", gra_02_init_evt);
    relSetEvtAddr("gra_03", gra_03_init_evt);
    relSetEvtAddr("gra_04", gra_04_init_evt);
    relSetEvtAddr("gra_05", gra_05_init_evt);
    relSetEvtAddr("gra_06", gra_06_init_evt);
    relSetBtlAddr("gra", btlsetup_gra_tbl, btl_gra_setup_no_tbl);
    pose_buf = _mapAlloc(sizeof(partyPoseTbl[PARTY_MEMBER_VIVIAN]));
    memcpy(pose_buf, partyPoseTbl[PARTY_MEMBER_VIVIAN], sizeof(partyPoseTbl[PARTY_MEMBER_VIVIAN]));
}

/**
 * REL exit point of gra.rel: runs the static destructors,
 * puts Vivian's pose table back from the copy taken in _prolog and frees that copy.
 */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    memcpy(partyPoseTbl[PARTY_MEMBER_VIVIAN], pose_buf, sizeof(partyPoseTbl[PARTY_MEMBER_VIVIAN]));
    _mapFree(pose_buf);
}

/** Empty stub the linker binds to calls that were left unresolved. */
void _unresolved(void) {
}

