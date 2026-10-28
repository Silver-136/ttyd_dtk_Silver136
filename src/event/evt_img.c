#include "driver/animdrv.h"
#include "driver/imgdrv.h"
#include "driver/offscreendrv.h"
#include "evt/evt_cmd.h"
#include "mario/mariost.h"
#include "manager/evtmgr_cmd.h"
#include <dolphin/gx.h>
#include "sdk/DEMOInit.h"
#include "driver/mapdrv.h"

USER_FUNC(evt_img_entry) {
    imgEntry((const char*)evtGetValue(event, event->args[0]), gp->inBattle != 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_set_position) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    ImageEntry* img = imgNameToPtr(name, gp->inBattle != 0);
    img->offsetX = x;
    img->offsetY = y;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_set_paper) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    const char* anim = (const char*)evtGetValue(event, args[1]);
    s32 group = gp->inBattle != 0;
    ImageEntry* img = imgNameToPtr(name, group);
    img->poseId = animPaperPoseGetId(anim, group);
    img->animTime = animTimeGetTime(group);
    animPoseSetMaterialFlagOn(img->poseId, MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_set_paper_anim) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    const char* anim = (const char*)evtGetValue(event, args[1]);
    s32 group = gp->inBattle != 0;
    ImageEntry* img = imgNameToPtr(name, group);
    img->animName = anim;
    img->animTime = animTimeGetTime(group);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_alloc_capture) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 shadowId = evtGetValue(event, args[1]);
    const char* offName = (const char*)evtGetValue(event, args[2]);
    s32 allocMode = evtGetValue(event, args[3]);
    s32 x = evtGetValue(event, args[4]);
    s32 y = evtGetValue(event, args[5]);
    s32 w = evtGetValue(event, args[6]);
    s32 h = evtGetValue(event, args[7]);
    ImageEntry* img = imgNameToPtr(name, gp->inBattle != 0);
    ImageShadow* sh = &img->shadows[shadowId];
    s32 offId;
    s32 i;

    if (offName != NULL) {
        offId = offscreenNameToId(offName);
    } else {
        offId = -1;
    }
    if (sh->flags & IMG_SHADOW_ACTIVE) {
        return EVT_RETURN_DONE;
    }
    x -= x & 1;
    y -= y & 1;
    w += w & 1;
    h += h & 1;
    for (i = 0; i < 3; i++) {
        if (i != shadowId && (img->shadows[i].flags & IMG_SHADOW_ACTIVE) && (img->width != (u16)w || img->height != (u16)h)) {
            return EVT_RETURN_DONE;
        }
    }
    if (x == 0 && y == 0 && w == 0 && h == 0 && offId != -1) {
        img->width = 0;
        img->height = 0;
        sh->x = 0;
        sh->y = 0;
        if (shadowId == 2) {
            sh->copyFmt = GX_CTF_A8;
            sh->texFmt = GX_TF_I8;
        } else {
            sh->copyFmt = GX_TF_RGBA8;
            sh->texFmt = GX_TF_RGBA8;
        }
        img->flags |= IMAGE_FLAG_OFFSCREEN_RECT;
    } else {
        if (x < 0 || y < 0 || w <= 0 || h <= 0 ||
            x + w > DEMOGetRenderModeObj()->fbWidth || y + h > DEMOGetRenderModeObj()->efbHeight) {
            return EVT_RETURN_DONE;
        }
        img->width = w;
        img->height = h;
        sh->x = x;
        sh->y = y;
        if (shadowId == 2) {
            sh->copyFmt = GX_CTF_A8;
            sh->texFmt = GX_TF_I8;
        } else {
            sh->copyFmt = GX_TF_RGBA8;
            sh->texFmt = GX_TF_RGBA8;
        }
    }
    sh->offscreenId = offId;
    sh->flags = IMG_SHADOW_ACTIVE | IMG_SHADOW_PENDING;
    sh->allocMode = allocMode;
    if (allocMode != 0) {
        sh->flags |= IMG_SHADOW_COPY_PENDING;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_free_capture) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 shadowId = evtGetValue(event, args[1]);
    imgFreeCapture(imgNameToPtr(name, gp->inBattle != 0), shadowId);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_onoff) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 on = evtGetValue(event, args[1]);
    ImageEntry* img = imgNameToPtr(name, gp->inBattle != 0);
    if (on != 0) {
        img->flags &= ~IMAGE_FLAG_HIDE;
    } else {
        img->flags |= IMAGE_FLAG_HIDE;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_set_virtual_point) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    ImageEntry* img = imgNameToPtr(name, gp->inBattle != 0);
    Vec tmp;
    Vec pos = { 0.0f, 0.0f, 0.0f };
    pos.x = x;
    pos.y = y;
    pos.z = z;
    // FAKE: spare copy of pos; passing &pos changes the object
    tmp = pos;
    imgSetVirtualPoint(img, &tmp);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_clear_virtual_point) {
    imgClearVirtualPoint(imgNameToPtr((const char*)evtGetValue(event, event->args[0]), gp->inBattle != 0));
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_release) {
    ImageEntry* img = imgNameToPtr((const char*)evtGetValue(event, event->args[0]), gp->inBattle != 0);
    imgRelease(img, gp->inBattle != 0);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_wait_animend) {
    ImageEntry* img = imgNameToPtr((const char*)evtGetValue(event, event->args[0]), gp->inBattle != 0);
    if (isFirstCall) {
        return EVT_RETURN_BLOCK;
    }
    if (img->loopTimes < 1.0f) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_set_shadow) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 id = evtGetValue(event, args[1]);
    imgSetShadow(imgNameToPtr(name, gp->inBattle != 0), id);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_set_paper_timerate) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 rate = evtGetFloat(event, args[1]);
    ImageEntry* img = imgNameToPtr(name, gp->inBattle != 0);
    animPoseSetLocalTimeRate(img->poseId, rate);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_img_set_color) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    u8 r = evtGetValue(event, args[1]);
    u8 g = evtGetValue(event, args[2]);
    u8 b = evtGetValue(event, args[3]);
    u8 a = evtGetValue(event, args[4]);
    ImageEntry* img = imgNameToPtr(name, gp->inBattle != 0);
    animPoseSetMaterialFlagOn(img->poseId, MAP_OBJ_FLAG_COLOR);
    animPoseSetMaterialEvtColor(img->poseId, (GXColor){r, g, b, a});
    return EVT_RETURN_DONE;
}

// Descriptive name (not in the symbol maps).
USER_FUNC(evt_img_set_z) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    f32 z = evtGetFloat(event, args[1]);
    imgNameToPtr(name, gp->inBattle != 0)->order = z;
    return EVT_RETURN_DONE;
}
