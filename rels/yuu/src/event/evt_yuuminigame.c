// evt_yuuminigame: Pianta Parlor minigame framework: shared work alloc/free, main and display callbacks.

#include "version.h"
#include "eff/eff_minigame.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include "driver/camdrv.h"
#include "string.h"
#include "driver/dispdrv.h"
#include "yuu.h"

static s32 dataadr[32];
s32 stackt[4];
s32 stackp;
void evt_yuuminigame_disp_func(CameraId camera, void* param);

USER_FUNC(evt_yuuminigame_main_func);
USER_FUNC(evt_yuuminigame_free);

static EvtWord main_evt[4];

USER_FUNC(yuu_atai_init) {
    stackp = 0;
    return EVT_RETURN_DONE;
}

/**
 * args[0] = output.
 * Pops the top value of the 4-entry value stack into it; leaves it untouched if the stack is empty.
 */
USER_FUNC(yuu_atai_pop) {
    EvtWord* args = event->args;

    if (stackp > 0) {
        stackp--;
        evtSetValue(event, args[0], stackt[stackp]);
    }
    return EVT_RETURN_DONE;
}

/** args[0] = value. Pushes it on the 4-entry value stack; ignored when the stack is full. */
USER_FUNC(yuu_atai_push) {
    EvtWord* args = event->args;

    if (stackp < 4) {
        stackt[stackp] = evtGetValue(event, args[0]);
        stackp++;
    }
    return EVT_RETURN_DONE;
}

/**
 * args[0] = effect type (0 start countdown, 1 success, 2 failure), args[1] to args[3] = x, y, z.
 * Spawns the minigame banner effect.
 */
USER_FUNC(eff_call_minigame) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effMiniGameEntry(type, x, y, z);
    return EVT_RETURN_DONE;
}

/**
 * args[0] = slot (0 to 31), args[1] = value.
 * Stores a pointer or event id in the shared slot table scripts use to find the minigame work and moving floors.
 */
USER_FUNC(yuuminigame_set_dataadr) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    if (index < 32) {
        dataadr[index] = evtGetValue(event, args[1]);
    }
    return EVT_RETURN_DONE;
}

/**
 * args[0] = slot, args[1] = output.
 * Reads back the value stored with yuuminigame_set_dataadr (0 if the slot is out of range).
 */
USER_FUNC(yuuminigame_get_dataadr) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    if (index >= 32) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], dataadr[index]);
    }
    return EVT_RETURN_DONE;
}

/**
 * args[0] = minigame work pointer (cleared to 0 afterwards).
 * Asks the minigame event to quit, blocks until it has stopped, then frees the minigame data and work area.
 */
USER_FUNC(evt_yuuminigame_free) {
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
    switch (wp->type) {
        case 3:
            yuunpc_yuuminigame4_ptr = NULL;
            break;
    }
    if (wp->data != NULL) {
        _mapFree(wp->data);
    }
    _mapFree(wp);
    evtSetValue(event, args[0], 0);
    return EVT_RETURN_DONE;
}

/**
 * args[0] = output work pointer, args[1] = minigame type (0 Plane Mode, 1 Boat Mode, 2 Tube Mode, 3 Paper Mode).
 * Allocates the work area and per-game data, hooks up that game's init, main and draw callbacks,
 * starts the main event and calls the init callback.
 */
USER_FUNC(evt_yuuminigame_alloc) {
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
            wp->init = yuuminigame_init_1hikoki;
            wp->main = yuuminigame_main_1hikoki;
            wp->disp = yuuminigame_disp_1hikoki;
            wp->data = _mapAlloc(0x18);
            memset(wp->data, 0, 0x18);
            break;
        case 1:
            wp->init = yuuminigame_init_2fune;
            wp->main = yuuminigame_main_2fune;
            wp->disp = yuuminigame_disp_2fune;
            wp->data = _mapAlloc(0x3C);
            memset(wp->data, 0, 0x3C);
            break;
        case 2:
            wp->init = yuuminigame_init_3roll;
            wp->main = yuuminigame_main_3roll;
            wp->disp = yuuminigame_disp_3roll;
            wp->data = _mapAlloc(0x2C);
            memset(wp->data, 0, 0x2C);
            break;
        case 3:
            wp->init = yuuminigame_init_4slit;
            wp->main = yuuminigame_main_4slit;
            wp->disp = yuuminigame_disp_4slit;
#if VERSION <= VERSION_DEMO_JP
            wp->data = _mapAlloc(0x24);
            memset(wp->data, 0, 0x24);
#else
            wp->data = _mapAlloc(0x28);
            memset(wp->data, 0, 0x28);
#endif
            yuunpc_yuuminigame4_ptr = wp;
            break;
    }
    wp->init(event, isFirstCall);
    return EVT_RETURN_DONE;
}

/**
 * Body of the minigame event: each frame it runs the game's main callback
 * unless a sub event (start or end script) is still running, queues the draw callback,
 * and stops once the quit flag is set.
 */
USER_FUNC(evt_yuuminigame_main_func) {
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
    dispEntry(CAMERA_2D, 1000.0f, DISP_MODE_OPAQUE, evt_yuuminigame_disp_func, wp);
    return EVT_RETURN_BLOCK;
}

/** 2D draw callback that forwards to the current minigame's own disp callback. */
void evt_yuuminigame_disp_func(CameraId camera, void* param) {
    ((MiniGameWork*)param)->disp(camera, param);
}

/** The minigame's per-frame event, started by evt_yuuminigame_alloc; runs evt_yuuminigame_main_func. */
static EVT_BEGIN(main_evt)
    CALL(evt_yuuminigame_main_func)
    RETURN
EVT_END()
