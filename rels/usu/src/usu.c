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
USER_FUNC(evt_usu_vivian_pose_set);

USER_FUNC(evt_usu_vivian_pose_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    partyPoseTbl[6][index] = (char*)evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("usu_00", usu_00_init_evt);
    relSetEvtAddr("usu_01", usu_01_init_evt);
    pose_buf = _mapAlloc(sizeof(partyPoseTbl[6]));
    memcpy(pose_buf, partyPoseTbl[6], sizeof(partyPoseTbl[6]));
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    memcpy(partyPoseTbl[6], pose_buf, sizeof(partyPoseTbl[6]));
    _mapFree(pose_buf);
}

void _unresolved(void) {
}

// FAKE: stand-in for the weak constant pair of MSL sqrtf (see docs/FAKES.md)
#pragma push
#pragma dollar_identifiers on
static f64 _three$localstatic4$sqrtf = 3.0;

static f64 _half$localstatic3$sqrtf = 0.5;
#pragma pop

