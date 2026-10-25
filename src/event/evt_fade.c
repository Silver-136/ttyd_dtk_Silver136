#include "evt/evt_fade.h"
#include "driver/fadedrv.h"
#include "driver/camdrv.h"
#include "mario/mariost.h"
#include "manager/evtmgr_cmd.h"
#include <dolphin/gx.h>


USER_FUNC(evt_fade_in) {
    EvtWord* args = event->args;
    s32 duration;

    duration = evtGetValue(event, args[0]);
    if (isFirstCall) {
        fadeEntry(FADE_IN_BLACK, duration, (GXColor){0, 0, 0, 0xFF});
    }
    if (fadeIsFinish() != TRUE) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_out) {
    EvtWord* args = event->args;
    s32 duration;

    duration = evtGetValue(event, args[0]);
    if (isFirstCall) {
        fadeEntry(FADE_OUT_BLACK, duration, (GXColor){0, 0, 0, 0xFF});
    }
    if (fadeIsFinish() != TRUE) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_entry) {
    EvtWord* args = event->args;
    FadeType type;
    s32 duration;
    u8 r, g, b;

    type = evtGetValue(event, *args++);
    duration = evtGetValue(event, *args++);
    r = evtGetValue(event, *args++);
    g = evtGetValue(event, *args++);
    b = evtGetValue(event, *args++);
    fadeEntry(type, duration, (GXColor){r, g, b, 0xFF});
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_end_wait) {
    return fadeIsFinish() != TRUE ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_set_spot_pos) {
    EvtWord* args = event->args;
    CameraEntry* camera;
    s32 x, y, z;
    f32 sx, sy, sz;
    f32 proj[7];
    f32 viewport[6];
    f32 savedProj[7];

    x = evtGetValue(event, args[0]);
    y = evtGetValue(event, args[1]);
    z = evtGetValue(event, args[2]);
    camera = camGetPtr(CAMERA_3D);
    GXGetProjectionv(savedProj);
    GXSetProjection(camera->projection, camera->projType);
    GXGetProjectionv(proj);
    GXGetViewportv(viewport);
    GXProject((f32)x, (f32)y, (f32)z, camera->view, proj, viewport, &sx, &sy, &sz);
    fadeSetSpotPos(-gp->fbWidth / 2 + sx, gp->efbHeight / 2 - sy);
    GXSetProjectionv(savedProj);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_set_anim_ofs_pos) {
    EvtWord* args = event->args;
    f32 x, y;

    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    fadeSetAnimOfsPos(x, y);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_set_anim_virtual_pos) {
    EvtWord* args = event->args;
    f32 x, y, z;

    x = evtGetFloat(event, args[0]);
    y = evtGetFloat(event, args[1]);
    z = evtGetFloat(event, args[2]);
    fadeSetAnimVirtualPos(x, y, z);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_set_mapchange_type) {
    EvtWord* args = event->args;
    s32 area, inType, inDuration, outType, outDuration;

    area = evtGetValue(event, args[0]);
    inType = evtGetValue(event, args[1]);
    inDuration = evtGetValue(event, args[2]);
    outType = evtGetValue(event, args[3]);
    outDuration = evtGetValue(event, args[4]);
    if (!area) {
        if (inType != -1) {
            gp->mapFadeInType = inType;
        }
        if (inDuration != -1) {
            gp->mapFadeInDuration = inDuration;
        }
        if (outType != -1) {
            gp->mapFadeOutType = outType;
        }
        if (outDuration != -1) {
            gp->mapFadeOutDuration = outDuration;
        }
    } else {
        if (inType != -1) {
            gp->areaFadeInType = inType;
        }
        if (inDuration != -1) {
            gp->areaFadeInDuration = inDuration;
        }
        if (outType != -1) {
            gp->areaFadeOutType = outType;
        }
        if (outDuration != -1) {
            gp->areaFadeOutDuration = outDuration;
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_tec_onoff) {
    EvtWord* args = event->args;
    s32 on, soft;

    on = evtGetValue(event, args[0]);
    soft = evtGetValue(event, args[1]);
    if (on) {
        if (soft) {
            fadeTecSoftOn();
        } else {
            fadeTecOn();
        }
    } else {
        if (soft) {
            fadeTecSoftOff();
        } else {
            fadeTecOff();
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_softfocus_onoff) {
    EvtWord* args = event->args;

    if (evtGetValue(event, args[0])) {
        fadeSoftFocusOn();
    } else {
        fadeSoftFocusOff();
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_fade_reset) {
    EvtWord* args = event->args;

    fadeReset(evtGetValue(event, args[0]));
    return EVT_RETURN_DONE;
}
