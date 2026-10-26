#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "rels/yuu/include/yuu.h"

#include "dolphin/os.h"
#include "memory.h"
#include "manager/evtmgr_cmd.h"
#include "string.h"
#include "mario/mariost.h"
#include "driver/dispdrv.h"
#include "dolphin/mtx.h"
#include "mario/mario.h"
#include "driver/camdrv.h"
#include "icon_ids.h"

typedef struct YuuCounterWork {
    EventEntry* event;
    u16 flags;
    u32 dispFlags;
    s32 type;
    f32 x;
    f32 y;
    s32 data;
    s32 field_0x1C;
    s64 elapsed;
    OSTime lastTime;
    s32 limit;
    s32 field_0x34;
} YuuCounterWork;
static void* yuunpc_wp;
static YuuCounterWork* _wp;
void evtYuuSetWindowEvt(EvtWord* evt);
void evt_yuucounter_disp_func(CameraId camera, void* param);
static const GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
static const GXColor bgColor = {0xFF, 0xFF, 0xFF, 0x99};
void iconDispGxCol(Mtx pos, s16 flags, u16 iconId, GXColor color);

USER_FUNC(evt_yuucounter_main_func);
USER_FUNC(yuucounter_disp_flag_onoff);
u32 yuucounter_flag(u32 mask);
void yuucounter_flag_off(u32 mask);
void yuucounter_set_data(s32 data);
USER_FUNC(evt_yuucounter_free);
USER_FUNC(evt_yuucounter_alloc);

static EvtWord main_evt[4];
EvtWord yuucounter_window_disp_evt[6];
EvtWord yuucounter_window_null_evt[2];

u32 yuucounter_flag(u32 mask) {
    if (_wp == NULL) {
        return 0;
    }
    return _wp->dispFlags & mask;
}

void yuucounter_flag_off(u32 mask) {
    if (_wp == NULL) {
        return;
    }
    _wp->dispFlags &= ~mask;
}

void yuucounter_flag_on(u32 mask) {
    if (_wp == NULL) {
        return;
    }
    _wp->dispFlags |= mask;
}

void yuucounter_set_data(s32 data) {
    if (_wp == NULL) {
        return;
    }
    _wp->data = data;
}

s32 yuucounter_get_data(void) {
    if (_wp == NULL) {
        return 0;
    }
    return _wp->data;
}

USER_FUNC(evt_yuucounter_free) {
    YuuCounterWork* wp = _wp;

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

USER_FUNC(evt_yuucounter_alloc) {
    EvtWord* args = event->args;
    YuuCounterWork* wp = _mapAlloc(sizeof(YuuCounterWork));
    s32 value;

    memset(wp, 0, sizeof(YuuCounterWork));
    _wp = wp;
    wp->x = evtGetFloat(event, args[0]);
    wp->y = evtGetFloat(event, args[1]);
    wp->type = evtGetValue(event, args[2]);
    value = evtGetValue(event, args[3]);
    switch (wp->type) {
        case 1:
            wp->limit = value * 1000;
            wp->lastTime = gp->renderTime;
            wp->elapsed = 0;
            wp->data = wp->limit;
            break;
        case 2:
            wp->field_0x34 = value;
            break;
    }
    wp->event = evtEntry(main_evt, 0, 0);
    wp->event->lwData[0] = (EvtWord)wp;
    if (wp->type == 1) {
        evtYuuSetWindowEvt(yuucounter_window_disp_evt);
    } else {
        evtYuuSetWindowEvt(yuucounter_window_null_evt);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_yuucounter_main_func) {
    YuuCounterWork* wp = (YuuCounterWork*)event->lwData[0];

    if (wp->flags & 1) {
        wp->flags |= 2;
        return EVT_RETURN_DONE;
    }
    dispEntry(CAMERA_2D, 1000.0f, 0, evt_yuucounter_disp_func, wp);
    return EVT_RETURN_BLOCK;
}

void evt_yuucounter_disp_func(CameraId camera, void* param) {
    YuuCounterWork* wp = param;
    GXColor color = white;
    GXColor numColor;
    GXColor iconColor;
    f32 scale;
    f32 xOfs;
    s32 value;
    s32 digit;
    BOOL skip2;
    s32 k;
    s32 i;
    s32 msecs;
    s32 iconId;
    s32 j;
    s32 secs;
    s32 digits[4];
    Mtx mtx;
    Mtx scaleMtx;
    Mtx base;
    Vec pos;
    MarioWork* mario;

    iconColor = white;
    numColor = white;
    switch (wp->type) {
        case 0:
            if (!(wp->dispFlags & 2)) {
                mario = marioGetPtr();
                wp->data = (s32)(200.0f * (1300.0f + mario->position.x));
            }
            scale = 2.5f;
            xOfs = -50.0f;
            iconId = ICON_COUNTER_POINT;
            if (wp->data < 0) {
                return;
            }
            break;
        case 1:
            if (!(wp->dispFlags & 2)) {
                if (wp->data >= 0) {
                    if (wp->dispFlags & 4) {
                        wp->dispFlags &= ~4;
                        wp->lastTime = gp->renderTime;
                    }
                    wp->elapsed += (s32)gp->renderTime - (s32)wp->lastTime;
                    wp->lastTime = gp->renderTime;
                    wp->data = wp->limit - OSTicksToMilliseconds(wp->elapsed);
                } else {
                    wp->data = -1;
                }
            }
            scale = 3.0f;
            iconId = ICON_COUNTER_COLON;
            xOfs = -42.0f;
            break;
        case 2:
            if (!(wp->dispFlags & 2)) {
                mario = marioGetPtr();
                pos = mario->position;
                wp->data = yuunpc_juni(&pos);
            }
            scale = 2.5f;
            iconId = -1;
            xOfs = -46.0f;
            break;
    }

    if (wp->dispFlags & 1) {
        return;
    }
    value = wp->data;
    if (value < 0) {
        value = 0;
    }
    secs = value / 1000;
    msecs = value % 1000;
    PSMTXTrans(base, wp->x, wp->y, 0.0f);
    PSMTXScale(scaleMtx, scale, 1.0f, 1.0f);
    PSMTXConcat(base, scaleMtx, mtx);
    iconDispGxCol(mtx, 0x10, ICON_COUNTER_BG, bgColor);

    switch (wp->type) {
        case 0: {
            BOOL skip;
            skip = TRUE;
            digits[0] = secs / 100;
            secs %= 100;
            digits[1] = secs / 10;
            digits[2] = secs % 10;
            for (j = 0; j < 3; j++) {
                digit = digits[j];
                if (digit != 0 || j == 2) {
                    skip = FALSE;
                }
                if (!skip) {
                    PSMTXTrans(mtx, xOfs + (f32)(j * 30), 8.0f, 0.0f);
                    PSMTXConcat(base, mtx, mtx);
                    iconDispGxCol(mtx, 0x10, digit + ICON_COUNTER_DIGIT_0, numColor);
                }
            }
            break;
        }
        case 1:
            digits[0] = secs / 60;
            secs %= 60;
            digits[1] = secs / 10;
            digits[2] = secs % 10;
            for (i = 0; i < 3; i++) {
                PSMTXTrans(mtx, xOfs + (f32)(i * 30) - (f32)((i == 0) * 16), 8.0f, 0.0f);
                PSMTXConcat(base, mtx, mtx);
                iconDispGxCol(mtx, 0x10, digits[i] + ICON_COUNTER_DIGIT_0, numColor);
            }
            PSMTXTrans(mtx, 8.0f + xOfs, 12.0f, 0.0f);
            PSMTXScale(scaleMtx, 2.0f, 2.0f, 2.0f);
            PSMTXConcat(mtx, scaleMtx, mtx);
            PSMTXConcat(base, mtx, mtx);
            iconDispGxCol(mtx, 0x10, iconId, iconColor);
            break;
        case 2:
            skip2 = TRUE;
            value = (value + 1) % 100000;
            digits[0] = value / 1000;
            value %= 1000;
            digits[1] = value / 100;
            value %= 100;
            digits[2] = value / 10;
            digits[3] = value % 10;
            for (k = 0; k < 4; k++) {
                digit = digits[k];
                if (digit != 0 || k == 3) {
                    skip2 = FALSE;
                }
                if (!skip2) {
                    PSMTXTrans(mtx, xOfs + (f32)(k * 30), 8.0f, 0.0f);
                    PSMTXConcat(base, mtx, mtx);
                    iconDispGxCol(mtx, 0x10, digit + ICON_COUNTER_DIGIT_0, numColor);
                }
            }
            break;
    }

    if (iconId != -1) {
        PSMTXTrans(mtx, 90.0f + xOfs - 12.0f, 10.0f, 0.0f);
        PSMTXConcat(base, mtx, mtx);
        iconDispGxCol(mtx, 0x10, iconId, iconColor);
    }

    switch (wp->type) {
        case 0:
        case 1:
            digits[0] = msecs / 100;
            digits[1] = (msecs % 100) / 10;
            for (i = 0; i < 2; i++) {
                PSMTXTrans(mtx, 90.0f + xOfs + (f32)(i * 15), 8.0f, 0.0f);
                PSMTXScale(scaleMtx, 0.5f, 0.5f, 0.5f);
                PSMTXConcat(mtx, scaleMtx, mtx);
                PSMTXConcat(base, mtx, mtx);
                iconDispGxCol(mtx, 0x10, digits[i] + ICON_COUNTER_DIGIT_0, numColor);
            }
            break;
        case 2:
            break;
    }
}

USER_FUNC(yuucounter_disp_flag_onoff) {
    EvtWord* args = event->args;
    BOOL on = evtGetValue(event, args[0]);
    u32 mask = evtGetValue(event, args[1]);
    YuuCounterWork* wp = _wp;

    if (on) {
        wp->dispFlags |= mask;
    } else {
        wp->dispFlags &= ~mask;
    }
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(main_evt)
    CALL(evt_yuucounter_main_func)
    RETURN
EVT_END()

EVT_BEGIN(yuucounter_window_disp_evt)
    CALL(yuucounter_disp_flag_onoff, 1, 4)
    RETURN
EVT_END()

EVT_BEGIN(yuucounter_window_null_evt)
    RETURN
EVT_END()
