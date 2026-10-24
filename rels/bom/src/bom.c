#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "mario/mario.h"
#include "party.h"
#include "driver/npcdrv.h"
#include "driver/animdrv.h"
#include "party/party_vivian.h"
#include "memory.h"
#include "string.h"
#include "system.h"
#include "bom.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
void* effMistEntry(s32 type, s32 lifetime, f32 x, f32 y, f32 z, f32 angle);

USER_FUNC(bress);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

f32 fd_bom_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_bom_neg_one_382;
f32 fd_bom___local_PI_383;
f32 fd_bom___local_3PIO2_385;
f32 fd_bom_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_bom___local_PIO2_384;
f32 fd_bom_one_381;
f32 fd_bom_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_bom_neg_one_365;
f32 fd_bom___local_PI_366;
f32 fd_bom___local_3PIO2_368;
f32 fd_bom_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_bom___local_PIO2_367;
f32 fd_bom_one_364;
f64 fd_bom__three_localstatic4_sqrtf;
f64 fd_bom__half_localstatic3_sqrtf;
EvtWord evt_bress[4];

void _unresolved(void) {
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("bom_00", bom_00_init_evt);
    relSetEvtAddr("bom_01", bom_01_init_evt);
    relSetEvtAddr("bom_02", bom_02_init_evt);
    relSetEvtAddr("bom_03", bom_03_init_evt);
    relSetEvtAddr("bom_04", bom_04_init_evt);
    relSetBtlAddr("bom", btlsetup_bom_tbl, btl_bom_setup_no_tbl);
    evtEntryType(evt_bress, 0, 0, 0);
}

f32 fd_bom_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_bom_neg_one_382 = -1.0f;

f32 fd_bom___local_PI_383 = 3.1415927f;

f32 fd_bom___local_3PIO2_385 = 4.712389f;

f32 fd_bom_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_bom___local_PIO2_384 = 1.5707964f;

f32 fd_bom_one_381 = 1.0f;

f32 fd_bom_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_bom_neg_one_365 = -1.0f;

f32 fd_bom___local_PI_366 = 3.1415927f;

f32 fd_bom___local_3PIO2_368 = 4.712389f;

f32 fd_bom_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_bom___local_PIO2_367 = 1.5707964f;

f32 fd_bom_one_364 = 1.0f;

f64 fd_bom__three_localstatic4_sqrtf = 3.0;

f64 fd_bom__half_localstatic3_sqrtf = 0.5;

USER_FUNC(bress) {
    static f32 offset[8] = {0.0f, 10.0f, 24.0f, 10.0f, 15.0f, 20.0f, 20.0f, 10.0f};
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);
    PartyEntry* party2 = partyGetPtr(1);
    NpcWork* npcWork = npcGetWorkPtr();
    s32* timers;
    NpcEntry* npc;
    s32 i;

    if (isFirstCall) {
        event->userdata[0] = 30;
        event->userdata[1] = 30;
        event->userdata[2] = 30;
        event->userWork[3] = (EvtWord)(timers = _mapAlloc(npcWork->total * sizeof(s32)));
        memset(timers, 0, npcWork->total * sizeof(s32));
    }
    timers = (s32*)event->userWork[3];
    if (event->userdata[0] != 0) {
        event->userdata[0]--;
    }
    if (event->userdata[1] != 0) {
        event->userdata[1]--;
    }
    if (event->userdata[2] != 0) {
        event->userdata[2]--;
    }
    for (i = 0; i < npcWork->total; i++) {
        if (timers[i] != 0) {
            timers[i]--;
        }
    }
    if (mario != NULL && event->userdata[0] == 0 && !marioChkSts(4) && !marioChkSts(0x10) && !marioChkSts(8) &&
        vivianGetStatus() != 2) {
        effMistEntry(4, 30, mario->position.x, 24.0f + mario->position.y, mario->position.z, 270.0f - mario->displayDirection);
        event->userdata[0] = irand(60) + 60;
    }
    if (party != NULL && event->userdata[1] == 0 && !marioChkSts(4) && !marioChkSts(0x10) && !marioChkSts(8) &&
        vivianGetStatus() != 2) {
        effMistEntry(4, 30, party->position.x, party->position.y + offset[party->currentMemberId], party->position.z,
                     270.0f - party->displayDirection);
        event->userdata[1] = irand(60) + 60;
    }
    if (party2 != NULL && event->userdata[2] == 0) {
        event->userdata[2] = irand(60) + 60;
    }
    for (i = 0, npc = npcWork->entries; i < npcWork->total; i++, npc++) {
        if ((npc->flags & 1) && timers[i] == 0) {
            AnimationPose* pose = animPoseGetAnimPosePtr(npc->poseId);
            effMistEntry(4, 30, npc->position.x, npc->height / 2 + npc->position.y, npc->position.z,
                         270.0f - pose->peraTargetRotY);
            timers[i] = irand(60) + 60;
        }
    }
    return EVT_RETURN_BLOCK;
}

EVT_BEGIN(evt_bress)
    CALL(bress)
    RETURN
EVT_END()

