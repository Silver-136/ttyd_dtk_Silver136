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

USER_FUNC(evt_usu_vivian_pose_set) {
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
    relSetEvtAddr("usu_00", usu_00_init_evt);
    relSetEvtAddr("usu_01", usu_01_init_evt);
    pose_buf = _mapAlloc(sizeof(partyPoseTbl[PARTY_MEMBER_VIVIAN]));
    memcpy(pose_buf, partyPoseTbl[PARTY_MEMBER_VIVIAN], sizeof(partyPoseTbl[PARTY_MEMBER_VIVIAN]));
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    memcpy(partyPoseTbl[PARTY_MEMBER_VIVIAN], pose_buf, sizeof(partyPoseTbl[PARTY_MEMBER_VIVIAN]));
    _mapFree(pose_buf);
}

void _unresolved(void) {
}

