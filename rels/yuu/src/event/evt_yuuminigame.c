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
void* effMiniGameEntry(s32 type, f32 x, f32 y, f32 z);
s32 yuuminigame_init_1hikoki(EventEntry* event, BOOL isFirstCall);
s32 yuuminigame_main_1hikoki(EventEntry* event, BOOL isFirstCall);
void yuuminigame_disp_1hikoki(CameraId camera, void* param);
s32 yuuminigame_init_2fune(EventEntry* event, BOOL isFirstCall);
s32 yuuminigame_main_2fune(EventEntry* event, BOOL isFirstCall);
void yuuminigame_disp_2fune(CameraId camera, void* param);
s32 yuuminigame_init_3roll(EventEntry* event, BOOL isFirstCall);
s32 yuuminigame_main_3roll(EventEntry* event, BOOL isFirstCall);
void yuuminigame_disp_3roll(CameraId camera, void* param);
s32 yuuminigame_init_4slit(EventEntry* event, BOOL isFirstCall);
s32 yuuminigame_main_4slit(EventEntry* event, BOOL isFirstCall);
void yuuminigame_disp_4slit(CameraId camera, void* param);
void evt_yuuminigame_disp_func(CameraId camera, void* param);

USER_FUNC(evt_yuuminigame_main_func);
USER_FUNC(yuu_atai_init);
USER_FUNC(yuu_atai_pop);
USER_FUNC(yuu_atai_push);
USER_FUNC(eff_call_minigame);
USER_FUNC(yuuminigame_set_dataadr);
USER_FUNC(yuuminigame_get_dataadr);
USER_FUNC(evt_yuuminigame_free);
USER_FUNC(evt_yuuminigame_alloc);

static EvtWord main_evt[4];

USER_FUNC(yuu_atai_init) {
    stackp = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(yuu_atai_pop) {
    EvtWord* args = event->args;

    if (stackp > 0) {
        stackp--;
        evtSetValue(event, args[0], stackt[stackp]);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(yuu_atai_push) {
    EvtWord* args = event->args;

    if (stackp < 4) {
        stackt[stackp] = evtGetValue(event, args[0]);
        stackp++;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(eff_call_minigame) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effMiniGameEntry(type, x, y, z);
    return EVT_RETURN_DONE;
}

USER_FUNC(yuuminigame_set_dataadr) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    if (index < 32) {
        dataadr[index] = evtGetValue(event, args[1]);
    }
    return EVT_RETURN_DONE;
}

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
            wp->data = _mapAlloc(0x28);
            memset(wp->data, 0, 0x28);
            yuunpc_yuuminigame4_ptr = wp;
            break;
    }
    wp->init(event, isFirstCall);
    return EVT_RETURN_DONE;
}

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
    dispEntry(CAMERA_2D, 1000.0f, 0, evt_yuuminigame_disp_func, wp);
    return EVT_RETURN_BLOCK;
}

void evt_yuuminigame_disp_func(CameraId camera, void* param) {
    ((MiniGameWork*)param)->disp(camera, param);
}

static EVT_BEGIN(main_evt)
    CALL(evt_yuuminigame_main_func)
    RETURN
EVT_END()

