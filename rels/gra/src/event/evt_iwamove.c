#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "mario/mario.h"
#include "driver/hitdrv.h"
#include "system.h"

typedef struct IwaMoveWork { s32 init; s32 timer; s32 state; EventEntry* event; EvtWord* script; } IwaMoveWork;
IwaMoveWork iwamove;
void partyKeyOn(void);
void partyKeyOff(void);
void statusWinForceOpen(void);
void statusWinForceOff(void);

USER_FUNC(evt_iwamove_main);
USER_FUNC(evt_iwamove_ent);
USER_FUNC(evt_iwamove_init);

EvtWord evt_iwamove_main_evt[4];

USER_FUNC(evt_iwamove_ent) {
    IwaMoveWork* wk = &iwamove;

    wk->script = (EvtWord*)event->args[0];
    wk->state = 0;
    wk->event = evtEntry(evt_iwamove_main_evt, 0, 0);
    wk->event->lwData[0] = (EvtWord)&iwamove;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_iwamove_init) {
    iwamove.init = 0;
    return EVT_RETURN_DONE;
}

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
                mario->dispFlags |= 2;
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
                mario->dispFlags &= ~2;
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

EVT_BEGIN(evt_iwamove_main_evt)
    CALL(evt_iwamove_main)
    RETURN
EVT_END()

