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
USER_FUNC(evt_gra_vivian_pose_set);

f32 fd_gra_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_gra_neg_one_382;
f32 fd_gra___local_PI_383;
f32 fd_gra___local_3PIO2_385;
f32 fd_gra_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_gra___local_PIO2_384;
f32 fd_gra_one_381;
f32 fd_gra_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_gra_neg_one_365;
f32 fd_gra___local_PI_366;
f32 fd_gra___local_3PIO2_368;
f32 fd_gra_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_gra___local_PIO2_367;
f32 fd_gra_one_364;

USER_FUNC(evt_gra_vivian_pose_set) {
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
    relSetEvtAddr("gra_00", gra_00_init_evt);
    relSetEvtAddr("gra_01", gra_01_init_evt);
    relSetEvtAddr("gra_02", gra_02_init_evt);
    relSetEvtAddr("gra_03", gra_03_init_evt);
    relSetEvtAddr("gra_04", gra_04_init_evt);
    relSetEvtAddr("gra_05", gra_05_init_evt);
    relSetEvtAddr("gra_06", gra_06_init_evt);
    relSetBtlAddr("gra", btlsetup_gra_tbl, btl_gra_setup_no_tbl);
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

f32 fd_gra_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gra_neg_one_382 = -1.0f;

f32 fd_gra___local_PI_383 = 3.1415927f;

f32 fd_gra___local_3PIO2_385 = 4.712389f;

f32 fd_gra_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gra___local_PIO2_384 = 1.5707964f;

f32 fd_gra_one_381 = 1.0f;

f32 fd_gra_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gra_neg_one_365 = -1.0f;

f32 fd_gra___local_PI_366 = 3.1415927f;

f32 fd_gra___local_3PIO2_368 = 4.712389f;

f32 fd_gra_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gra___local_PIO2_367 = 1.5707964f;

f32 fd_gra_one_364 = 1.0f;

