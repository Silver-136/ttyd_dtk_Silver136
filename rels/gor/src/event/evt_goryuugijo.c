// evt_goryuugijo: Pianta Parlor hall helpers: the driver of the Monte medal counter and slot machine minigames.

#include "eff/eff_minigame.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "rels/gor/include/gor.h"
#include "evt/evt_yuugijou.h"

#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include "driver/camdrv.h"
#include "string.h"
#include "driver/dispdrv.h"

void evt_goryuugijo_disp_func(CameraId camera, void* param);

USER_FUNC(evt_goryuugijo_main_func);
USER_FUNC(eff_call_yuugijo);
USER_FUNC(evt_goryuugijo_free);

static EvtWord main_evt[4];

/**
 * Script helper that spawns a minigame effect: args[0] is the effect type and args[1..3] the position,
 * passed to effMiniGameEntry. Used by the Pianta Parlor hall in gor_03.
 */
USER_FUNC(eff_call_yuugijo) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effMiniGameEntry(type, x, y, z);
    return EVT_RETURN_DONE;
}

/**
 * Script helper that shuts down the hall minigame whose work block pointer is in args[0]:
 * on the first call it sets the quit flag, waits until the main function has acknowledged it,
 * frees the data and the block and clears args[0]. Does nothing when the pointer is 0.
 */
USER_FUNC(evt_goryuugijo_free) {
    EvtWord* args = event->args;
    MiniGameWork* wp = (MiniGameWork*)evtGetValue(event, args[0]);

    if (wp == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        wp->flags |= 1;
    }
    if (!(wp->flags & 2)) {
        return EVT_RETURN_BLOCK;
    }
    if (wp->data != NULL) {
        _mapFree(wp->data);
    }
    _mapFree(wp);
    evtSetValue(event, args[0], 0);
    return EVT_RETURN_DONE;
}

/**
 * Script helper that starts a hall minigame: allocates its work block (pointer stored in args[0]),
 * picks the callbacks by type args[1] (0 the Monte medal counter with 4 bytes of data,
 * 1 the slot machine with 0x78 bytes), starts the driver script main_evt and calls the game's init.
 */
USER_FUNC(evt_goryuugijo_alloc) {
    EvtWord* args = event->args;
    MiniGameWork* wp = _mapAlloc(sizeof(MiniGameWork));

    memset(wp, 0, sizeof(MiniGameWork));
    evtSetValue(event, args[0], (EvtWord)wp);
    wp->type = evtGetValue(event, args[1]);
    wp->event = evtEntry(main_evt, 0, 0);
    wp->event->lwData[0] = (EvtWord)wp;
    wp->subEvent = NULL;
    switch (wp->type) {
        case 0:
            wp->init = goryuugijo_init_montemedal;
            wp->main = goryuugijo_main_montemedal;
            wp->disp = goryuugijo_disp_montemedal;
            wp->data = _mapAlloc(4);
            memset(wp->data, 0, 4);
            break;
        case 1:
            wp->init = goryuugijo_init_slot;
            wp->main = goryuugijo_main_slot;
            wp->disp = goryuugijo_disp_slot;
            wp->data = _mapAlloc(0x78);
            memset(wp->data, 0, 0x78);
            break;
    }
    wp->init(event, isFirstCall);
    return EVT_RETURN_DONE;
}

/**
 * Per-frame driver of a hall minigame (work block in LW(0)):
 * stops when the quit flag is set, otherwise runs the game's main function unless a sub-event is still running,
 * and queues the 2D draw (priority 1000). Never finishes by itself.
 */
USER_FUNC(evt_goryuugijo_main_func) {
    MiniGameWork* wp = (MiniGameWork*)event->lwData[0];

    if (wp->flags & 1) {
        wp->flags |= 2;
        return EVT_RETURN_DONE;
    }
    if (wp->subEvent != NULL) {
        if (evtCheckID(wp->subEvent->eventId)) {
            goto disp;
        }
        wp->subEvent = NULL;
    }
    wp->main(event, isFirstCall);
disp:
    dispEntry(CAMERA_2D, 1000.0f, DISP_MODE_OPAQUE, evt_goryuugijo_disp_func, wp);
    return EVT_RETURN_BLOCK;
}

/** Draw callback of a hall minigame: forwards to the game's own disp function. */
void evt_goryuugijo_disp_func(CameraId camera, void* param) {
    ((MiniGameWork*)param)->disp(camera, param);
}

/**
 * Driver script of a hall minigame, started by the start helper with evtEntry:
 * runs evt_goryuugijo_main_func, which yields every frame until the quit flag is set.
 */
static EVT_BEGIN(main_evt)
    CALL(evt_goryuugijo_main_func)
    RETURN
EVT_END()

