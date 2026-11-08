// bom.rel (ボム村, Fahr Outpost): registers bom_00..bom_04 and the battle database; puffs of breath in the cold.

#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_mist.h"

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

void _unresolved(void);
void _epilog(void);
void _prolog(void);

EvtWord evt_bress[4];

/**
 * Module unresolved-symbol stub: if a relocation could not be resolved the REL linker points it here;
 * it does nothing.
 */
void _unresolved(void) {
}

/** REL module epilog: runs the static destructors (_dtors) when bom.rel is unloaded. */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

/**
 * REL module prolog, run when bom.rel is loaded:
 * runs the static constructors, registers the five map init scripts (bom_00 to bom_04)
 * and the Fahr Outpost battle setup tables, and starts the breath-cloud script evt_bress.
 */
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

/**
 * Breath-puff script helper, run every frame by evt_bress:
 * spawns a small mist effect in front of Mario and the first partner every 1 to 2 seconds (randomised, 60 to 119
 * frames), and in front of every active NPC, each with its own timer.
 * No puffs while Mario is in certain states (marioChkSts 4, 8, 0x10), and from the JP release on,
 * none while vivianGetStatus() returns 2 (Vivian's shadow state). Simulates visible breath in the cold.
 */
#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(bress) {
#else
USER_FUNC(bress) {
#endif

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
#if VERSION <= VERSION_DEMO_JP
    if (mario != NULL && event->userdata[0] == 0 && !marioChkSts(MARIO_STS_PAPER) && !marioChkSts(MARIO_STS_ROLL) && !marioChkSts(MARIO_STS_JABARA)) {
#else
    if (mario != NULL && event->userdata[0] == 0 && !marioChkSts(MARIO_STS_PAPER) && !marioChkSts(MARIO_STS_ROLL) && !marioChkSts(MARIO_STS_JABARA) &&
        vivianGetStatus() != VIVIAN_STATUS_HIDDEN) {
#endif
        effMistEntry(4, 30, mario->position.x, 24.0f + mario->position.y, mario->position.z, 270.0f - mario->displayDirection);
        event->userdata[0] = irand(60) + 60;
    }
#if VERSION <= VERSION_DEMO_JP
    if (party != NULL && event->userdata[1] == 0 && !marioChkSts(MARIO_STS_PAPER) && !marioChkSts(MARIO_STS_ROLL) && !marioChkSts(MARIO_STS_JABARA)) {
#else
    if (party != NULL && event->userdata[1] == 0 && !marioChkSts(MARIO_STS_PAPER) && !marioChkSts(MARIO_STS_ROLL) && !marioChkSts(MARIO_STS_JABARA) &&
        vivianGetStatus() != VIVIAN_STATUS_HIDDEN) {
#endif
        effMistEntry(4, 30, party->position.x, party->position.y + offset[party->currentMemberId], party->position.z,
                     270.0f - party->displayDirection);
        event->userdata[1] = irand(60) + 60;
    }
    if (party2 != NULL && event->userdata[2] == 0) {
        event->userdata[2] = irand(60) + 60;
    }
    for (i = 0, npc = npcWork->entries; i < npcWork->total; i++, npc++) {
        if ((npc->flags & NPC_FLAG_IN_USE) && timers[i] == 0) {
            AnimationPose* pose = animPoseGetAnimPosePtr(npc->poseId);
            effMistEntry(4, 30, npc->position.x, npc->height / 2 + npc->position.y, npc->position.z,
                         270.0f - pose->peraTargetRotY);
            timers[i] = irand(60) + 60;
        }
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Event script started at module load (not tied to one map):
 * runs the bress breath-puff helper for the lifetime of the module.
 */
EVT_BEGIN(evt_bress)
    CALL(bress)
    RETURN
EVT_END()
