#include "evt/evt_cmd.h"
#include "driver/mapdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mariost.h"
#include "memory.h"
#include "pmario_sound.h"
#include <string.h>
#include "sfx_name_word.h"
#include "evt/evt_bero.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_party.h"
#include "evt/evt_sub.h"
#include "evt/evt_movefloor.h"


typedef struct MovingFloor {
    u32 flags;       // 0x0
    s32 kind;        // 0x4
    f32 speed;       // 0x8
    f32 pos;         // 0xC
    f32 cur;         // 0x10
    u32 pad14;       // 0x14
    OSTime start;    // 0x18
    s32 mode;        // 0x20
    s32 phase;       // 0x24
    s32 idx;         // 0x28
    f32 a[3];        // 0x2C
    f32 b[3];        // 0x38
    f32 pos3[3];     // 0x44
    f32 prev[3];     // 0x50
    f32 yoff;        // 0x5C
    f32 prevYoff;    // 0x60
    f32 scale;       // 0x64
    s32 time[3];     // 0x68
    s32 stmsec;      // 0x74
    u32 seHalt;      // 0x78
    u32 seId;        // 0x7C
    f32 turn;        // 0x80
    f32 angle;       // 0x84
} MovingFloor;

USER_FUNC(evt_mario_get_name_hitobj_head);
USER_FUNC(evt_mario_get_name_hitobj_ride);

static f32 sintbl[450] = {
    0.0f, 0.017452f, 0.034899f, 0.052336f, 0.069756f, 0.087156f, 0.104528f, 0.121869f,
    0.139173f, 0.156434f, 0.173648f, 0.190809f, 0.207912f, 0.224951f, 0.241922f, 0.258819f,
    0.275637f, 0.292372f, 0.309017f, 0.325568f, 0.34202f, 0.358368f, 0.374607f, 0.390731f,
    0.406737f, 0.422618f, 0.438371f, 0.45399f, 0.469471f, 0.48481f, 0.5f, 0.515038f,
    0.529919f, 0.544639f, 0.559193f, 0.573576f, 0.587785f, 0.601815f, 0.615661f, 0.62932f,
    0.642788f, 0.656059f, 0.669131f, 0.681998f, 0.694658f, 0.707107f, 0.71934f, 0.731354f,
    0.743145f, 0.754709f, 0.766044f, 0.777146f, 0.788011f, 0.798635f, 0.809017f, 0.819152f,
    0.829037f, 0.83867f, 0.848048f, 0.857167f, 0.866025f, 0.87462f, 0.882947f, 0.891006f,
    0.898794f, 0.906308f, 0.913545f, 0.920505f, 0.927184f, 0.93358f, 0.939693f, 0.945518f,
    0.951056f, 0.956305f, 0.961262f, 0.965926f, 0.970296f, 0.97437f, 0.978148f, 0.981627f,
    0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.99863f,
    0.999391f, 0.999848f, 1.0f, 0.999848f, 0.999391f, 0.99863f, 0.997564f, 0.996195f,
    0.994522f, 0.992546f, 0.990268f, 0.987688f, 0.984808f, 0.981627f, 0.978148f, 0.97437f,
    0.970296f, 0.965926f, 0.961262f, 0.956305f, 0.951057f, 0.945519f, 0.939693f, 0.933581f,
    0.927184f, 0.920505f, 0.913546f, 0.906308f, 0.898794f, 0.891007f, 0.882948f, 0.87462f,
    0.866026f, 0.857168f, 0.848048f, 0.838671f, 0.829038f, 0.819152f, 0.809017f, 0.798636f,
    0.788011f, 0.777146f, 0.766045f, 0.75471f, 0.743145f, 0.731354f, 0.71934f, 0.707107f,
    0.694659f, 0.681999f, 0.669131f, 0.656059f, 0.642788f, 0.629321f, 0.615662f, 0.601815f,
    0.587786f, 0.573577f, 0.559193f, 0.544639f, 0.52992f, 0.515039f, 0.5f, 0.48481f,
    0.469472f, 0.453991f, 0.438372f, 0.422619f, 0.406737f, 0.390732f, 0.374607f, 0.358369f,
    0.342021f, 0.325569f, 0.309017f, 0.292372f, 0.275638f, 0.25882f, 0.241923f, 0.224952f,
    0.207912f, 0.190809f, 0.173649f, 0.156435f, 0.139174f, 0.12187f, 0.104529f, 0.087156f,
    0.069757f, 0.052337f, 0.0349f, 0.017453f, 1e-6f, -0.017452f, -0.034899f, -0.052335f,
    -0.069756f, -0.087155f, -0.104528f, -0.121869f, -0.139173f, -0.156434f, -0.173648f, -0.190808f,
    -0.207911f, -0.22495f, -0.241921f, -0.258819f, -0.275637f, -0.292371f, -0.309016f, -0.325568f,
    -0.342019f, -0.358367f, -0.374606f, -0.39073f, -0.406736f, -0.422618f, -0.438371f, -0.45399f,
    -0.469471f, -0.484809f, -0.499999f, -0.515037f, -0.529919f, -0.544639f, -0.559192f, -0.573576f,
    -0.587785f, -0.601814f, -0.615661f, -0.62932f, -0.642787f, -0.656058f, -0.66913f, -0.681998f,
    -0.694658f, -0.707106f, -0.719339f, -0.731353f, -0.743144f, -0.754709f, -0.766044f, -0.777145f,
    -0.78801f, -0.798635f, -0.809016f, -0.819152f, -0.829037f, -0.83867f, -0.848048f, -0.857167f,
    -0.866025f, -0.874619f, -0.882947f, -0.891006f, -0.898794f, -0.906307f, -0.913545f, -0.920504f,
    -0.927183f, -0.93358f, -0.939692f, -0.945518f, -0.951056f, -0.956304f, -0.961261f, -0.965926f,
    -0.970295f, -0.97437f, -0.978147f, -0.981627f, -0.984808f, -0.987688f, -0.990268f, -0.992546f,
    -0.994522f, -0.996195f, -0.997564f, -0.998629f, -0.999391f, -0.999848f, -1.0f, -0.999848f,
    -0.999391f, -0.99863f, -0.997564f, -0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f,
    -0.984808f, -0.981627f, -0.978148f, -0.97437f, -0.970296f, -0.965926f, -0.961262f, -0.956305f,
    -0.951057f, -0.945519f, -0.939693f, -0.933581f, -0.927184f, -0.920505f, -0.913546f, -0.906308f,
    -0.898795f, -0.891007f, -0.882948f, -0.87462f, -0.866026f, -0.857168f, -0.848049f, -0.838671f,
    -0.829038f, -0.819153f, -0.809018f, -0.798636f, -0.788011f, -0.777147f, -0.766045f, -0.75471f,
    -0.743146f, -0.731354f, -0.719341f, -0.707108f, -0.694659f, -0.681999f, -0.669132f, -0.65606f,
    -0.642788f, -0.629321f, -0.615662f, -0.601816f, -0.587786f, -0.573577f, -0.559194f, -0.54464f,
    -0.52992f, -0.515039f, -0.500001f, -0.484811f, -0.469473f, -0.453991f, -0.438372f, -0.422619f,
    -0.406737f, -0.390732f, -0.374608f, -0.358369f, -0.342022f, -0.325569f, -0.309018f, -0.292373f,
    -0.275638f, -0.25882f, -0.241923f, -0.224952f, -0.207913f, -0.19081f, -0.173649f, -0.156435f,
    -0.139175f, -0.121871f, -0.10453f, -0.087157f, -0.069758f, -0.052337f, -0.034901f, -0.017454f,
    -1e-6f, 0.017451f, 0.034899f, 0.052335f, 0.069755f, 0.087154f, 0.104527f, 0.121868f,
    0.139172f, 0.156433f, 0.173647f, 0.190808f, 0.20791f, 0.22495f, 0.241921f, 0.258818f,
    0.275636f, 0.292371f, 0.309015f, 0.325567f, 0.342019f, 0.358367f, 0.374605f, 0.39073f,
    0.406736f, 0.422617f, 0.43837f, 0.45399f, 0.469471f, 0.484809f, 0.499999f, 0.515037f,
    0.529918f, 0.544638f, 0.559191f, 0.573575f, 0.587784f, 0.601814f, 0.615661f, 0.629319f,
    0.642787f, 0.656058f, 0.66913f, 0.681997f, 0.694657f, 0.707105f, 0.719339f, 0.731353f,
    0.743144f, 0.754709f, 0.766044f, 0.777145f, 0.78801f, 0.798635f, 0.809016f, 0.819151f,
    0.829037f, 0.83867f, 0.848047f, 0.857166f, 0.866025f, 0.874619f, 0.882947f, 0.891006f,
    0.898793f, 0.906307f, 0.913545f, 0.920504f, 0.927183f, 0.93358f, 0.939692f, 0.945518f,
    0.951056f, 0.956304f, 0.961261f, 0.965925f, 0.970295f, 0.97437f, 0.978147f, 0.981627f,
    0.984808f, 0.987688f, 0.990268f, 0.992546f, 0.994522f, 0.996195f, 0.997564f, 0.998629f,
    0.999391f, 0.999848f,
};


#define MOVING_FLOOR_ARG() ((MovingFloor*)evtGetValue(event, event->args[0]))
#define WOBBLE(w)                                                                                  \
    if ((w)->kind == 1) {                                                                          \
        (w)->yoff = 3.0f * sintbl[(s32)(5.625f * (f32)((gp->retraceCount + event->eventId * 39) & 63))]; \
    } else {                                                                                       \
        (w)->yoff = 0.0f;                                                                          \
    }

USER_FUNC(evt_moving_floor_alloc) {
    EvtWord* args = event->args;
    MovingFloor* w = (MovingFloor*)_mapAlloc(sizeof(MovingFloor));
    memset(w, 0, sizeof(MovingFloor));
    w->mode = event->lwData[6];
    w->kind = event->lwData[12];
    switch (w->mode) {
        case 0:
            w->flags &= ~1;
            w->idx = 0;
            w->scale = 1.0f;
            w->a[0] = event->lwData[2];
            w->b[0] = event->lwData[3];
            w->time[0] = event->lwData[4];
            w->pos3[0] = w->a[0];
            w->turn = event->lwData[11];
            break;
        case 1:
            w->idx = 1;
            w->scale = 1.0f;
            w->a[1] = event->lwData[2];
            w->b[1] = event->lwData[3];
            w->time[1] = event->lwData[4];
            w->pos3[1] = w->a[1];
            w->turn = event->lwData[11];
            break;
        case 2:
            w->idx = 2;
            w->scale = 1.0f;
            w->a[2] = event->lwData[2];
            w->b[2] = event->lwData[3];
            w->time[2] = event->lwData[4];
            w->pos3[2] = w->a[2];
            w->turn = event->lwData[11];
            break;
        case 3:
            w->idx = 0;
            w->scale = 1.0f;
            w->a[0] = event->lwData[2];
            w->b[0] = event->lwData[3];
            w->time[0] = event->lwData[4];
            w->pos3[0] = 0.5f * w->a[0] + 0.5f * w->b[0];
            w->a[2] = event->lwData[8];
            w->b[2] = event->lwData[9];
            w->time[2] = event->lwData[10];
            w->pos3[2] = 0.5f * w->a[2] + 0.5f * w->b[2];
            w->turn = event->lwData[11];
            break;
        case 4:
            w->idx = 2;
            w->scale = 1.0f;
            w->a[0] = event->lwData[2];
            w->b[0] = event->lwData[3];
            w->time[0] = event->lwData[4];
            w->pos3[0] = w->a[0];
            w->a[2] = event->lwData[8];
            w->b[2] = event->lwData[9];
            w->time[2] = event->lwData[10];
            w->pos3[2] = w->a[2];
            w->turn = event->lwData[11];
            break;
        case 5:
            w->flags &= ~1;
            w->idx = 0;
            w->scale = 1.0f;
            w->a[0] = event->lwData[2];
            w->b[0] = event->lwData[3];
            w->time[0] = event->lwData[4];
            w->pos3[0] = w->a[0];
            w->turn = event->lwData[11];
            break;
    }
    w->stmsec = event->lwData[5];
    w->prev[0] = w->pos3[0];
    w->prev[1] = w->pos3[1];
    w->prev[2] = w->pos3[2];
    w->prevYoff = w->yoff;
    WOBBLE(w)
    evtSetValue(event, args[0], (EvtWord)w);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_moving_floor_free) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    if (w == NULL) {
        return EVT_RETURN_DONE;
    }
    _mapFree(w);
    evtSetValue(event, args[0], 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_moving_floor_init) {
    MovingFloor* w = MOVING_FLOOR_ARG();
    s32 dir;
    f32 t;
    f32 d;
    switch (w->mode) {
        case 0:
            dir = w->phase;
            t = 0.0f;
            w->phase++;
            if (w->phase >= 2) {
                w->phase = 0;
            }
            break;
        case 1:
            dir = w->phase;
            t = 0.0f;
            w->phase++;
            if (w->phase >= 2) {
                w->phase = 0;
            }
            break;
        case 2:
            dir = w->phase;
            t = 0.0f;
            w->phase++;
            if (w->phase >= 2) {
                w->phase = 0;
            }
            break;
        case 3:
            switch (w->phase) {
                case 0:
                    dir = 1;
                    t = 0.5f;
                    w->flags &= ~1;
                    w->idx = 0;
                    w->scale = 1.0f;
                    break;
                case 1:
                    dir = 0;
                    t = 0.0f;
                    w->flags |= 1;
                    w->idx = 0;
                    w->scale = 0.5f;
                    break;
                case 2:
                    dir = 1;
                    t = 0.5f;
                    w->flags &= ~1;
                    w->idx = 2;
                    w->scale = 1.0f;
                    break;
                case 3:
                    dir = 0;
                    t = 0.0f;
                    w->flags |= 1;
                    w->idx = 2;
                    w->scale = 0.5f;
                    break;
                case 4:
                    dir = 0;
                    t = 0.5f;
                    w->flags &= ~1;
                    w->idx = 0;
                    w->scale = 1.0f;
                    break;
                case 5:
                    dir = 1;
                    t = 0.0f;
                    w->flags |= 1;
                    w->idx = 0;
                    w->scale = 0.5f;
                    break;
                case 6:
                    dir = 0;
                    t = 0.5f;
                    w->flags &= ~1;
                    w->idx = 2;
                    w->scale = 1.0f;
                    break;
                case 7:
                    dir = 1;
                    t = 0.0f;
                    w->flags |= 1;
                    w->idx = 2;
                    w->scale = 0.5f;
                    break;
            }
            w->phase++;
            if (w->phase >= 8) {
                w->phase = 0;
            }
            break;
        case 4:
            switch (w->phase) {
                case 0:
                    w->idx = 2;
                    w->scale = 1.0f;
                    dir = 0;
                    t = 0.0f;
                    break;
                case 1:
                    w->idx = 0;
                    w->scale = 1.0f;
                    dir = 0;
                    t = 0.0f;
                    break;
                case 2:
                    w->idx = 2;
                    w->scale = 1.0f;
                    dir = 1;
                    t = 0.0f;
                    break;
                case 3:
                    w->idx = 0;
                    w->scale = 1.0f;
                    dir = 1;
                    t = 0.0f;
                    break;
            }
            w->phase++;
            if (w->phase >= 4) {
                w->phase = 0;
            }
            break;
        case 5:
            dir = w->phase;
            t = 1.0f;
            break;
    }
    if (dir) {
        d = w->a[w->idx] - w->b[w->idx];
    } else {
        d = w->b[w->idx] - w->a[w->idx];
    }
    w->speed = d / (f32)w->time[w->idx];
    if (dir) {
        w->pos = w->b[w->idx] * (1.0f - t) + w->a[w->idx] * t;
    } else {
        w->pos = w->a[w->idx] * (1.0f - t) + w->b[w->idx] * t;
    }
    w->cur = (f32)w->time[w->idx] * t;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_moving_floor_main) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    s32 us = OSTicksToMicroseconds((u32)event->lifetime - (u32)w->start);
    f32 t = (f32)us;
    w->pos += w->speed * t / 1000.0f;
    w->cur += t / 1000.0f;
    w->prev[0] = w->pos3[0];
    w->prev[1] = w->pos3[1];
    w->prev[2] = w->pos3[2];
    w->prevYoff = w->yoff;
    WOBBLE(w)
    w->pos3[w->idx] = w->pos;
    evtSetFloat(event, args[1], w->pos3[0]);
    evtSetFloat(event, args[2], w->pos3[1] + w->yoff);
    evtSetFloat(event, args[3], w->pos3[2]);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_moving_floor_main2) {
    MovingFloor* w = MOVING_FLOOR_ARG();
    w->start = event->lifetime;
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_get_end) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    s32 a = (s32)w->cur;
    s32 b = (s32)((f32)w->time[w->idx] * w->scale);
    if (a >= b) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_get_pause) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    if (w->flags & 2) {
        evtSetValue(event, args[1], 1);
    } else {
        evtSetValue(event, args[1], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_get_kind) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    evtSetValue(event, args[1], w->kind);
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_get_mode) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    evtSetValue(event, args[1], w->mode);
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_update_turn) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    w->angle += w->turn;
    if (w->angle < 0.0f) {
        w->angle += 360.0f;
    }
    if (w->angle >= 360.0f) {
        w->angle -= 360.0f;
    }
    evtSetFloat(event, args[1], w->angle);
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_getf_pos) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    w->prevYoff = w->yoff;
    WOBBLE(w)
    evtSetFloat(event, args[1], w->pos3[0]);
    evtSetFloat(event, args[2], w->pos3[1] + w->yoff);
    evtSetFloat(event, args[3], w->pos3[2]);
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_getf_now_bpoint) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    evtSetFloat(event, args[1], w->pos3[0] - w->prev[0]);
    evtSetFloat(event, args[2], (w->pos3[1] + w->yoff) - (w->prev[1] + w->prevYoff));
    evtSetFloat(event, args[3], w->pos3[2] - w->prev[2]);
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_get_stmsec) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    if (w->flags & 1) {
        evtSetValue(event, args[1], 0);
    } else {
        evtSetValue(event, args[1], w->stmsec * 60 / 1000);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_getf_xz_scale) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    f32 v = evtGetFloat(event, (s32)(w->pos3[1] + w->yoff));
    evtSetFloat(event, args[1], (500.0f - v) / 500.0f);
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_reverse) {
    MovingFloor* w = MOVING_FLOOR_ARG();
    f32 e;
    if (w->speed >= 0.0f) {
        return EVT_RETURN_DONE;
    }
    w->speed *= -1.0f;
    e = (f32)w->time[w->idx] * w->scale;
    w->cur = e - w->cur;
    w->phase ^= 1;
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_set_se) {
    EvtWord* args = event->args;
    MovingFloor* w = MOVING_FLOOR_ARG();
    char* name = (char*)evtGetValue(event, args[1]);
    MapObject* obj;
    Vec pos;
    s32 flags;
    if (w->seHalt) {
        return EVT_RETURN_DONE;
    }
    if (w->seId == 0) {
        mapObjGetPos(name, &pos);
        obj = mapGetMapObj(name);
        if (obj == NULL) {
            mapErrorEntry(0, name);
            return EVT_RETURN_DONE;
        }
        flags = 0;
        if ((psndGetFlag() & PSND_FLAG_DOOR_IN) && !(obj->flags & (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2))) {
            flags = 0x10000;
        }
        w->seId = psndSFXOn_3D(SFX_NAME_WORD(flags | PSND_SFX_STG1_TIK_FLOOR_MOVE1), &pos);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_stop_se) {
    MovingFloor* w = MOVING_FLOOR_ARG();
    if (w->seHalt == 0 && w->seId) {
        psndSFXOff(w->seId);
        w->seId = 0;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(moving_floor_halt_se) {
    MovingFloor* w = MOVING_FLOOR_ARG();
    w->seHalt = 1;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_moving_floor_delete) {
    s32 id = evtGetValue(event, event->args[0]);
    EventEntry* e = evtGetPtrID(id);
    MovingFloor* w = (MovingFloor*)e->lwData[9];
    if (w->seHalt == 0 && w->seId) {
        psndSFXOff(w->seId);
        w->seId = 0;
    }
    if (w != NULL) {
        _mapFree(w);
        e->lwData[9] = 0;
    }
    if (evtCheckID(id)) {
        evtDeleteID(id);
    }
    return EVT_RETURN_DONE;
}

EVT_BEGIN(moving_floor_bero_check)
    CALL(evt_sub_get_areaname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("yuu"))
        RETURN
    END_IF
    LOOP(0)
        CALL(evt_bero_exec_wait, 65536)
        DEBUG_PUT_MSG(STRING("\216\251\223\256\215\355\217\234\203`\203F\203b\203N\221\322\202\277"))
        CALL(evt_bero_exec_wait, 1)
        CALL(evt_bero_get_now_number, LW(10))
        CALL(evt_bero_get_info_nextarea, LW(10), LW(3), LW(4))
        IF_INT_NOT_EQUAL(LW(3), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    DEBUG_PUT_MSG(STRING("\216\251\223\256\215\355\217\234\212\256\227\271"))
    CALL(moving_floor_stop_se, LW(9))
    CALL(moving_floor_halt_se, LW(9))
    RETURN
EVT_END()

EVT_BEGIN(moving_floor)
    CALL(evt_moving_floor_alloc, LW(9))
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    SPAWN_EVENT(moving_floor_bero_check)
    LOOP(0)
        CALL(evt_moving_floor_init, LW(9))
        CALL(evt_moving_floor_main2, LW(9))
        CALL(moving_floor_get_mode, LW(9), LW(8))
        IF_INT_EQUAL(LW(8), 5)
            GOTO(0)
        END_IF
        LOOP(0)
            LOOP(0)
                CALL(moving_floor_get_pause, LW(9), LW(8))
                IF_INT_EQUAL(LW(8), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
                CALL(moving_floor_getf_pos, LW(9), LW(12), LW(13), LW(14))
                CALL(evt_moving_floor_main2, LW(9))
                CALL(evt_mapobj_trans, 1, LW(0), LW(12), LW(13), LW(14))
                CALL(moving_floor_update_turn, LW(9), LW(8))
                CALL(evt_mapobj_rotate, 1, LW(0), FLOAT(0.0f), LW(8), FLOAT(0.0f))
                CALL(evt_hit_bind_update, LW(1))
                IF_STR_NOT_EQUAL(LW(7), 0)
                    CALL(evt_mapobj_trans, 1, LW(7), LW(12), 0, LW(14))
                    CALL(evt_mapobj_rotate, 1, LW(7), FLOAT(0.0f), LW(8), FLOAT(0.0f))
                    CALL(moving_floor_getf_xz_scale, LW(9), LW(8))
                    CALL(evt_mapobj_scale, 1, LW(7), LW(8), 1, LW(8))
                END_IF
            END_LOOP
            WAIT_FRAMES(1)
            CALL(evt_moving_floor_main, LW(9), LW(12), LW(13), LW(14))
            CALL(evt_moving_floor_main2, LW(9))
            CALL(evt_mapobj_trans, 1, LW(0), LW(12), LW(13), LW(14))
            CALL(moving_floor_update_turn, LW(9), LW(8))
            CALL(evt_mapobj_rotate, 1, LW(0), FLOAT(0.0f), LW(8), FLOAT(0.0f))
            CALL(evt_hit_bind_update, LW(1))
            IF_STR_NOT_EQUAL(LW(7), 0)
                CALL(evt_mapobj_trans, 1, LW(7), LW(12), 0, LW(14))
                CALL(evt_mapobj_rotate, 1, LW(7), FLOAT(0.0f), LW(8), FLOAT(0.0f))
                CALL(moving_floor_getf_xz_scale, LW(9), LW(8))
                CALL(evt_mapobj_scale, 1, LW(7), LW(8), 1, LW(8))
            END_IF
            CALL(evt_mario_get_name_hitobj_head, LW(2))
            CALL(evt_mario_get_party, LW(10))
            SET_VALUE(LW(3), 0)
            IF_INT_NOT_EQUAL(LW(10), 0)
                CALL(evt_party_get_name_hitobj_head, 0, LW(3))
            END_IF
            CALL(evt_mario_get_exparty, LW(10))
            SET_VALUE(LW(4), 0)
            IF_INT_NOT_EQUAL(LW(10), 0)
                CALL(evt_party_get_name_hitobj_head, 1, LW(4))
            END_IF
            SET_VALUE(LF(0), 0)
            IF_INT_EQUAL(LW(6), 1)
                IF_STR_EQUAL(LW(2), LW(1))
                    SET_VALUE(LF(0), 1)
                END_IF
                IF_STR_EQUAL(LW(3), LW(1))
                    SET_VALUE(LF(0), 1)
                END_IF
                IF_STR_EQUAL(LW(4), LW(1))
                    SET_VALUE(LF(0), 1)
                END_IF
            END_IF
            IF_INT_EQUAL(LF(0), 1)
                CALL(moving_floor_reverse, LW(9))
            END_IF
            CALL(moving_floor_get_kind, LW(9), LW(8))
            IF_INT_EQUAL(LW(8), 0)
                CALL(moving_floor_getf_now_bpoint, LW(9), LW(12), LW(13), LW(14))
                CALL(evt_mario_get_name_hitobj_ride, LW(8))
                IF_STR_EQUAL(LW(8), LW(1))
                    CALL(evt_mario_set_hosei_xyz, LW(12), LW(13), LW(14))
                END_IF
                CALL(evt_party_get_name_hitobj_ride, 0, LW(8))
                IF_STR_EQUAL(LW(8), LW(1))
                    CALL(evt_party_set_hosei_xyz, 0, LW(12), LW(13), LW(14))
                END_IF
                CALL(evt_party_get_name_hitobj_ride, 1, LW(8))
                IF_STR_EQUAL(LW(8), LW(1))
                    CALL(evt_party_set_hosei_xyz, 1, LW(12), LW(13), LW(14))
                END_IF
            END_IF
            CALL(moving_floor_get_end, LW(9), LW(8))
            IF_INT_NOT_EQUAL(LW(8), 0)
                BREAK_LOOP
            END_IF
            CALL(moving_floor_get_kind, LW(9), LW(8))
            IF_INT_EQUAL(LW(8), 0)
                SET_VALUE(LF(0), 1)
                IF_FLOAT_EQUAL(LW(12), FLOAT(0.0f))
                    IF_FLOAT_EQUAL(LW(13), FLOAT(0.0f))
                        IF_FLOAT_EQUAL(LW(14), FLOAT(0.0f))
                            SET_VALUE(LF(0), 0)
                        END_IF
                    END_IF
                END_IF
                IF_INT_EQUAL(LF(0), 0)
                    CALL(moving_floor_stop_se, LW(9))
                ELSE
                    CALL(moving_floor_set_se, LW(9), LW(0))
                END_IF
            END_IF
        END_LOOP
        CALL(moving_floor_stop_se, LW(9))
        LABEL(0)
        CALL(moving_floor_get_stmsec, LW(9), LW(10))
        LOOP(LW(10))
            CALL(moving_floor_get_mode, LW(9), LW(8))
            IF_INT_EQUAL(LW(8), 5)
                SET_VALUE(LW(10), 0)
            ELSE
                IF_INT_EQUAL(LW(10), 0)
                    BREAK_LOOP
                END_IF
            END_IF
            LOOP(0)
                CALL(moving_floor_get_pause, LW(9), LW(8))
                IF_INT_EQUAL(LW(8), 0)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
                CALL(moving_floor_getf_pos, LW(9), LW(12), LW(13), LW(14))
                CALL(evt_moving_floor_main2, LW(9))
                CALL(evt_mapobj_trans, 1, LW(0), LW(12), LW(13), LW(14))
                CALL(moving_floor_update_turn, LW(9), LW(8))
                CALL(evt_mapobj_rotate, 1, LW(0), FLOAT(0.0f), LW(8), FLOAT(0.0f))
                CALL(evt_hit_bind_update, LW(1))
                IF_STR_NOT_EQUAL(LW(7), 0)
                    CALL(evt_mapobj_trans, 1, LW(7), LW(12), 0, LW(14))
                    CALL(evt_mapobj_rotate, 1, LW(7), FLOAT(0.0f), LW(8), FLOAT(0.0f))
                    CALL(moving_floor_getf_xz_scale, LW(9), LW(8))
                    CALL(evt_mapobj_scale, 1, LW(7), LW(8), 1, LW(8))
                END_IF
            END_LOOP
            WAIT_FRAMES(1)
            CALL(moving_floor_getf_pos, LW(9), LW(12), LW(13), LW(14))
            CALL(evt_moving_floor_main2, LW(9))
            CALL(evt_mapobj_trans, 1, LW(0), LW(12), LW(13), LW(14))
            CALL(moving_floor_update_turn, LW(9), LW(8))
            CALL(evt_mapobj_rotate, 1, LW(0), FLOAT(0.0f), LW(8), FLOAT(0.0f))
            CALL(evt_hit_bind_update, LW(1))
            IF_STR_NOT_EQUAL(LW(7), 0)
                CALL(evt_mapobj_trans, 1, LW(7), LW(12), 0, LW(14))
                CALL(evt_mapobj_rotate, 1, LW(7), FLOAT(0.0f), LW(8), FLOAT(0.0f))
                CALL(moving_floor_getf_xz_scale, LW(9), LW(8))
                CALL(evt_mapobj_scale, 1, LW(7), LW(8), 1, LW(8))
            END_IF
        END_LOOP
    END_LOOP
    CALL(evt_moving_floor_free, LW(9))
    RETURN
EVT_END()

EVT_BEGIN(ridedown_floor)
    CALL(evt_hit_bind_mapobj, LW(1), LW(0))
    SET_VALUE(LW(10), LW(2))
    SUBTRACT(LW(10), LW(3))
    GET_INT_INDEXED(LW(6), LW(14))
    GET_INT_INDEXED(LW(6), LW(15))
    SET_VALUE(LW(9), 0)
    LOOP(0)
        IF_INT_EQUAL(LW(5), 0)
            GET_INT_INDEXED(LW(7), LW(12))
            AND(LW(12), 65534)
            SET_INT_INDEXED(LW(7), LW(12))
        ELSE
            GET_INT_INDEXED(LW(7), LW(12))
            AND(LW(12), 65533)
            SET_INT_INDEXED(LW(7), LW(12))
        END_IF
        CALL(evt_mario_get_name_hitobj_ride, LW(11))
        IF_STR_EQUAL(LW(11), LW(1))
            IF_INT_EQUAL(LW(5), 0)
                GET_INT_INDEXED(LW(6), LW(11))
                SUBTRACT(LW(11), LW(4))
                IF_INT_LESS_OR_EQUAL(LW(11), 0)
                    SET_VALUE(LW(11), 0)
                END_IF
                SET_INT_INDEXED(LW(6), LW(11))
                GET_INT_INDEXED(LW(7), LW(12))
                OR(LW(12), 1)
                SET_INT_INDEXED(LW(7), LW(12))
            ELSE
                GET_INT_INDEXED(LW(6), LW(11))
                ADD(LW(11), LW(4))
                IF_INT_GREATER_OR_EQUAL(LW(11), 10000)
                    SET_VALUE(LW(11), 10000)
                END_IF
                SET_INT_INDEXED(LW(6), LW(11))
                GET_INT_INDEXED(LW(7), LW(12))
                OR(LW(12), 2)
                SET_INT_INDEXED(LW(7), LW(12))
            END_IF
        END_IF
        GET_INT_INDEXED(LW(7), LW(12))
        IF_INT_EQUAL(LW(12), 0)
            IF_INT_EQUAL(LW(5), 0)
                GET_INT_INDEXED(LW(6), LW(11))
                IF_INT_GREATER_OR_EQUAL(LW(11), LW(15))
                    SUBTRACT(LW(11), LW(4))
                    IF_INT_LESS_OR_EQUAL(LW(11), LW(15))
                        SET_VALUE(LW(11), LW(15))
                    END_IF
                ELSE
                    ADD(LW(11), LW(4))
                    IF_INT_GREATER_OR_EQUAL(LW(11), LW(15))
                        SET_VALUE(LW(11), LW(15))
                    END_IF
                END_IF
                SET_INT_INDEXED(LW(6), LW(11))
            END_IF
        END_IF
        GET_INT_INDEXED(LW(6), LW(11))
        MULTIPLY(LW(11), LW(10))
        MULTIPLY_FLOAT(LW(11), FLOAT(0.009765625f))
        MULTIPLY_FLOAT(LW(11), FLOAT(0.009765625f))
        IF_INT_EQUAL(LW(5), 0)
            SET_FLOAT_VALUE(LW(12), LW(3))
            ADD_FLOAT(LW(12), LW(11))
        ELSE
            SET_FLOAT_VALUE(LW(12), LW(2))
            SUBTRACT_FLOAT(LW(12), LW(11))
        END_IF
        CALL(evt_mapobj_trans, 1, LW(0), 0, LW(12), 0)
        CALL(evt_hit_bind_update, LW(1))
        SET_FLOAT_VALUE(LW(13), LW(12))
        SUBTRACT_FLOAT(LW(13), LW(14))
        SET_FLOAT_VALUE(LW(14), LW(12))
        CALL(evt_mario_get_name_hitobj_ride, LW(11))
        IF_STR_EQUAL(LW(11), LW(1))
            CALL(evt_mario_set_hosei_xyz, 0, LW(13), 0)
        END_IF
        CALL(evt_party_get_name_hitobj_ride, 0, LW(11))
        IF_STR_EQUAL(LW(11), LW(1))
            CALL(evt_party_set_hosei_xyz, 0, 0, LW(13), 0)
        END_IF
        CALL(evt_party_get_name_hitobj_ride, 1, LW(11))
        IF_STR_EQUAL(LW(11), LW(1))
            CALL(evt_party_set_hosei_xyz, 1, 0, LW(13), 0)
        END_IF
        IF_FLOAT_EQUAL(LW(13), FLOAT(0.0f))
            IF_INT_NOT_EQUAL(LW(9), 0)
                CALL(evt_snd_sfxoff, LW(9))
                SET_VALUE(LW(9), 0)
            END_IF
        ELSE
            IF_INT_EQUAL(LW(9), 0)
                CALL(evt_mapobj_get_position, LW(0), LW(11), LW(12), LW(13))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_TIK_FLOOR_MOVE1"), LW(11), LW(12), LW(13), LW(9))
            END_IF
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

