#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "memory.h"
#include "string.h"
#include "driver/msgdrv.h"
#include "manager/evtmgr_cmd.h"
#include "driver/dispdrv.h"
#include "driver/camdrv.h"
#include "manager/fontmgr.h"

typedef struct YuuTitleWork { EventEntry* event; u16 flags; f32 x; f32 y; f32 startX; f32 startY; f32 targetX; f32 targetY; s32 alpha; const char* msg; } YuuTitleWork;
static YuuTitleWork* _wp;
void evt_yuutitle_disp_func(CameraId camera, void* param);
void FontDrawString(f32 x, f32 y, const char* str);

USER_FUNC(evt_yuutitle_main_func);
USER_FUNC(evt_yuutitle_free);
USER_FUNC(evt_yuutitle_alloc);
void evt_yuutitle_disp_func(CameraId camera, void* param);

static EvtWord main_evt[4];

void evt_yuutitle_disp_func(CameraId camera, void* param) {
    YuuTitleWork* wp = param;
    f32 scale = 2.0f;
    f32 halfWidth;

    if (wp->alpha < 255) {
        f32 t;

        wp->alpha += 6;
        if (wp->alpha > 255) {
            wp->alpha = 255;
        }
        t = wp->alpha / 255.0f;
        wp->x = wp->targetX * t + wp->startX * (1.0f - t);
        wp->y = wp->targetY * t + wp->startY * (1.0f - t);
        t = 0.2f + 0.8f * t;
        scale *= t;
    }
    halfWidth = scale * FontGetMessageWidth(wp->msg) / 2;
    FontDrawStart();
    FontDrawScale(scale);
    FontDrawEdge();
    FontDrawColor((GXColor){0xFF, 0xFF, 0xFF, wp->alpha});
    FontDrawString(wp->x - halfWidth, wp->y, wp->msg);
    FontDrawEdgeOff();
}

USER_FUNC(evt_yuutitle_main_func) {
    YuuTitleWork* wp = (YuuTitleWork*)event->lwData[0];

    if (wp->flags & 1) {
        wp->flags |= 2;
        return EVT_RETURN_DONE;
    }
    dispEntry(CAMERA_2D, 1000.0f, 0, evt_yuutitle_disp_func, wp);
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_yuutitle_alloc) {
    EvtWord* args = event->args;
    YuuTitleWork* wp = _mapAlloc(sizeof(YuuTitleWork));

    memset(wp, 0, sizeof(YuuTitleWork));
    _wp = wp;
    wp->targetX = evtGetFloat(event, args[0]);
    wp->targetY = evtGetFloat(event, args[1]);
    wp->msg = msgSearch((const char*)evtGetValue(event, args[2]));
    wp->startX = wp->x = 200.0f + wp->targetX;
    wp->startY = wp->y = 200.0f + wp->targetY;
    wp->alpha = 0;
    wp->event = evtEntry(main_evt, 0, 0);
    wp->event->lwData[0] = (EvtWord)wp;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_yuutitle_free) {
    YuuTitleWork* wp = _wp;

    if (wp == NULL) {
        return EVT_RETURN_DONE;
    }
    if (isFirstCall) {
        wp->flags |= 1;
    }
    if (!(wp->flags & 2)) {
        return EVT_RETURN_BLOCK;
    }
    _mapFree(_wp);
    _wp = NULL;
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(main_evt)
    CALL(evt_yuutitle_main_func)
    RETURN
EVT_END()

