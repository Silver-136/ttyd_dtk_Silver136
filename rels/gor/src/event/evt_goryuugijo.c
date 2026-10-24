#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include "driver/camdrv.h"
#include "string.h"
#include "driver/dispdrv.h"

void* effMiniGameEntry(s32 type, f32 x, f32 y, f32 z);
typedef struct GoryuuWork { EventEntry* event; EventEntry* subEvent; u16 flags; s32 type; s32 (*init)(EventEntry* event, BOOL isFirstCall); s32 (*main)(EventEntry* event, BOOL isFirstCall); void (*disp)(CameraId camera, void* param); void* data; } GoryuuWork;
s32 goryuugijo_init_montemedal(EventEntry* event, BOOL isFirstCall);
s32 goryuugijo_main_montemedal(EventEntry* event, BOOL isFirstCall);
void goryuugijo_disp_montemedal(CameraId camera, void* param);
s32 goryuugijo_init_slot(EventEntry* event, BOOL isFirstCall);
s32 goryuugijo_main_slot(EventEntry* event, BOOL isFirstCall);
void goryuugijo_disp_slot(CameraId camera, void* param);
void evt_goryuugijo_disp_func(CameraId camera, void* param);

USER_FUNC(evt_goryuugijo_main_func);
USER_FUNC(eff_call_yuugijo);
USER_FUNC(evt_goryuugijo_free);
USER_FUNC(evt_goryuugijo_alloc);
void evt_goryuugijo_disp_func(CameraId camera, void* param);

f32 fd_evt_goryuugijo_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_evt_goryuugijo_neg_one_382;
f32 fd_evt_goryuugijo___local_PI_383;
f32 fd_evt_goryuugijo___local_3PIO2_385;
f32 fd_evt_goryuugijo_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_evt_goryuugijo___local_PIO2_384;
f32 fd_evt_goryuugijo_one_381;
f32 fd_evt_goryuugijo_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_evt_goryuugijo_neg_one_365;
f32 fd_evt_goryuugijo___local_PI_366;
f32 fd_evt_goryuugijo___local_3PIO2_368;
f32 fd_evt_goryuugijo_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_evt_goryuugijo___local_PIO2_367;
f32 fd_evt_goryuugijo_one_364;
static EvtWord main_evt[4];

USER_FUNC(eff_call_yuugijo) {
    EvtWord* args = event->args;
    s32 type = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);

    effMiniGameEntry(type, x, y, z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_goryuugijo_free) {
    EvtWord* args = event->args;
    GoryuuWork* wp = (GoryuuWork*)evtGetValue(event, args[0]);

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

USER_FUNC(evt_goryuugijo_alloc) {
    EvtWord* args = event->args;
    GoryuuWork* wp = _mapAlloc(sizeof(GoryuuWork));

    memset(wp, 0, sizeof(GoryuuWork));
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

USER_FUNC(evt_goryuugijo_main_func) {
    GoryuuWork* wp = (GoryuuWork*)event->lwData[0];

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
    dispEntry(CAMERA_2D, 1000.0f, 0, evt_goryuugijo_disp_func, wp);
    return EVT_RETURN_BLOCK;
}

void evt_goryuugijo_disp_func(CameraId camera, void* param) {
    ((GoryuuWork*)param)->disp(camera, param);
}

f32 fd_evt_goryuugijo_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_evt_goryuugijo_neg_one_382 = -1.0f;

f32 fd_evt_goryuugijo___local_PI_383 = 3.1415927f;

f32 fd_evt_goryuugijo___local_3PIO2_385 = 4.712389f;

f32 fd_evt_goryuugijo_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_evt_goryuugijo___local_PIO2_384 = 1.5707964f;

f32 fd_evt_goryuugijo_one_381 = 1.0f;

f32 fd_evt_goryuugijo_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_evt_goryuugijo_neg_one_365 = -1.0f;

f32 fd_evt_goryuugijo___local_PI_366 = 3.1415927f;

f32 fd_evt_goryuugijo___local_3PIO2_368 = 4.712389f;

f32 fd_evt_goryuugijo_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_evt_goryuugijo___local_PIO2_367 = 1.5707964f;

f32 fd_evt_goryuugijo_one_364 = 1.0f;

static EVT_BEGIN(main_evt)
    CALL(evt_goryuugijo_main_func)
    RETURN
EVT_END()

