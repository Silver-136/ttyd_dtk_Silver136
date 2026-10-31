// The moving boulder (iwa) of Twilight Trail: one shared work block and the script that drives it.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "party.h"
#include "statuswindow.h"

#include "mario/mario.h"
#include "driver/hitdrv.h"
#include "system.h"

typedef struct IwaMoveWork {
    s32 init;
    s32 timer;
    s32 state;
    EventEntry* event;
    EvtWord* script;
} IwaMoveWork;
IwaMoveWork iwamove;

USER_FUNC(evt_iwamove_main);
USER_FUNC(evt_iwamove_ent);
USER_FUNC(evt_iwamove_init);

EvtWord evt_iwamove_main_evt[4];

/**
 * Starts the boulder push watcher: stores the script in args[0] (run when the push is done)
 * and spawns evt_iwamove_main_evt, passing the shared work block in its LW(0).
 */
USER_FUNC(evt_iwamove_ent) {
    IwaMoveWork* wk = &iwamove;

    wk->script = (EvtWord*)event->args[0];
    wk->state = 0;
    wk->event = evtEntry(evt_iwamove_main_evt, 0, 0);
    wk->event->lwData[0] = (EvtWord)&iwamove;
    return EVT_RETURN_DONE;
}

/** Resets the shared boulder work block's init flag. */
USER_FUNC(evt_iwamove_init) {
    iwamove.init = 0;
    return EVT_RETURN_DONE;
}

/**
 * Per-frame watcher for the push: when Mario pushes the A_h_iwa collision it locks his pose,
 * shows the status window and counts 80 frames of holding right on the stick; releasing early restores control;
 * after 80 frames it starts the given script. Never finishes by itself.
 */
USER_FUNC(evt_iwamove_main) {
    IwaMoveWork* wk = (IwaMoveWork*)event->lwData[0];
    MarioWork* mario = marioGetPtr();

    if (isFirstCall) {
        return EVT_RETURN_BLOCK;
    }
    switch (wk->state) {
        case 0:
            if (mario->hitobjPush == hitNameToPtr("A_h_iwa")) {
                wk->timer = 80;
                wk->state++;
                marioKeyOff();
                partyKeyOff();
                marioChgPose("M_O_1");
                mario->dispFlags |= MARIO_DISP_FLAG_POSE_LOCK;
                statusWinForceOpen();
            }
            break;
        case 1:
            if (keyGetStickX(0) > 10) {
                if (wk->timer-- == 0) {
                    wk->state++;
                }
            } else {
                wk->state--;
                mario->dispFlags &= ~MARIO_DISP_FLAG_POSE_LOCK;
                marioChgPose("M_S_1");
                marioKeyOn();
                partyKeyOn();
                statusWinForceOff();
            }
            break;
        case 2:
            if (wk->script != NULL) {
                evtEntry(wk->script, 0, 0);
            }
            statusWinForceOff();
            wk->state++;
            break;
        case 3:
            break;
    }
    return EVT_RETURN_BLOCK;
}

/** Wrapper that runs evt_iwamove_main. */
EVT_BEGIN(evt_iwamove_main_evt)
    CALL(evt_iwamove_main)
    RETURN
EVT_END()

