#include "driver/camdrv.h"
#include "driver/animdrv.h"
#include "driver/hitdrv.h"
#include "driver/imgdrv.h"
#include "driver/offscreendrv.h"
#include "evt/evt_cmd.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "memory.h"
#include "party.h"
#include "party/party_cloud.h"
#include "pmario_sound.h"
#include "stdio.h"
#include "string.h"
#include "dolphin/mtx.h"
#include "driver/mapdrv.h"
#include "evt/evt_img.h"

typedef struct CloudWork {
    u16 flags; // 0x0
    s32 state; // 0x4
    char animName[0x20]; // 0x8
    char imgName[0x20]; // 0x28
    char offscreenName[0x20]; // 0x48
    char groupName[0x20]; // 0x68
    char hitName[0x20]; // 0x88
    s32 limit; // 0xA8
    s32 counter; // 0xAC
    EventEntry* evt; // 0xB0
    void* endEvt; // 0xB4
    s32 timer; // 0xB8
    char** groupNames; // 0xBC
    u16 w; // 0xC0
    u16 h; // 0xC2
    f32 lastFrame; // 0xC4
} CloudWork;


static CloudWork cloud[16];
static u8 cloud_once_flag;
static CloudWork* cloud_p;

static USER_FUNC(evt_cloud_main);
static EVT_BEGIN(evt_cloud_main_evt)
    CALL(evt_cloud_main)
    RETURN
EVT_END()

static USER_FUNC(evt_cloud_main) {
    CloudWork* wp = (CloudWork*)event->lwData[0];
    s32 group = gp->inBattle != 0;
    MarioWork* mario = marioGetPtr();
    PartyEntry* party = partyGetPtr(0);
    HitEntry* hit0;
    HitEntry* hit1;
    HitEntry* hit2;
    HitEntry* hit3;
    HitEntry* hit;
    ImageEntry* img;

    if (isFirstCall) {
        return EVT_RETURN_BLOCK;
    }
    if (party == NULL) {
        return EVT_RETURN_BLOCK;
    }
    hit0 = cloudGetHitBreathout(0);
    hit1 = cloudGetHitBreathout(1);
    hit2 = cloudGetHitBreathout(2);
    hit3 = cloudGetHitBreathout(3);
    switch (wp->state) {
        case 0:
            if ((s32)cloud_once_flag == 1) {
                break;
            }
            hit = hitNameToPtr(wp->hitName);
            if (hit0 == hit || hit1 == hit || hit2 == hit || hit3 == hit) {
                wp->state++;
            }
            wp->counter = 0;
            break;
        case 1:
            if (mario->displayDirectionTarget > 90.0f && mario->displayDirectionTarget <= 270.0f) {
                strcpy(wp->animName, "P_oof_cloud_2");
            } else {
                strcpy(wp->animName, "P_oof_cloud_1");
            }
            if (!animGroupBaseAsync(wp->animName, group, NULL)) {
                break;
            }
            cloud_once_flag = 1;
            animPaperPoseEntry(wp->animName, group);
            offscreenEntry(wp->offscreenName);
            if (!(wp->flags & 2)) {
                mapGrpSetOffScreen(wp->groupName, wp->offscreenName);
            } else {
                char** p = wp->groupNames;
                while (*p != NULL) {
                    mapGrpSetOffScreen(*p, wp->offscreenName);
                    p++;
                }
            }
            wp->state++;
            break;
        case 2: {
            s32 x, y;
            s32 w, h;
            u16 uw, uh;
            Vec pos;
            EventEntry evt;
            EvtWord args[10];

            if (wp->w != 0 && wp->h != 0) {
                mapObjGetPos(wp->groupName, &pos);
                PSMTXMultVec(camGetPtr(CAMERA_3D)->view, &pos, &pos);
                PSMTX44MultVec(camGetPtr(CAMERA_3D)->projection, &pos, &pos);
                pos.x = (1.0f + pos.x) * (f32)gp->fbWidth / 2.0f;
                pos.y = (1.0f - pos.y) * (f32)gp->efbHeight / 2.0f;
                uw = wp->w;
                uh = wp->h;
                w = uw;
                h = uh;
                x = (s32)(pos.x - (f32)(s32)((u32)uw >> 1));
                y = (s32)(pos.y - (f32)(s32)(uh >> 1));
                if (x < 0) {
                    x = 0;
                }
                if (y < 0) {
                    y = 0;
                }
                if (w > gp->fbWidth - x) {
                    w = gp->fbWidth - x;
                }
                if (h > gp->efbHeight - y) {
                    h = gp->efbHeight - y;
                }
            } else {
                h = 0;
                w = 0;
                y = 0;
                x = 0;
            }
            imgEntry(wp->imgName, group);
            img = imgNameToPtr(wp->imgName, group);
            img->flags |= IMAGE_FLAG_HIDE;
            img->poseId = animPaperPoseGetId(wp->animName, group);
            img->animTime = animTimeGetTime(group);
            evt.args = args;
            args[0] = (EvtWord)wp->imgName;
            args[1] = 0;
            args[2] = (EvtWord)wp->offscreenName;
            args[3] = 1;
            args[4] = x;
            args[5] = y;
            args[6] = w;
            args[7] = h;
            evt_img_alloc_capture(&evt, TRUE);
            imgClearVirtualPoint(img);
            imgSetShadow(img, 2);
            wp->state++;
            break;
        }
        case 3:
            img = imgNameToPtr(wp->imgName, group);
            img->flags &= ~IMAGE_FLAG_HIDE;
            img->animName = "A_1";
            img->animTime = animTimeGetTime(group);
            mapGrpFlagOn(wp->groupName, MAP_OBJ_FLAG_HIDDEN);
            wp->timer = 30;
            wp->state++;
            wp->lastFrame = 0.0f;
            break;
        case 4:
            img = imgNameToPtr(wp->imgName, group);
            if (wp->lastFrame == 0.0f || wp->lastFrame + 1.0f < img->loopTimes) {
                psndSFXOn_3D("SFX_OFF_CLAUD_WIND2", &mario->position);
                wp->lastFrame = img->loopTimes;
            }
            if (mario->currMotionId != MARIO_MOTION_CLOUD) {
                wp->state = 10;
                break;
            }
            hit = hitNameToPtr(wp->hitName);
            if (hit0 != hit && hit1 != hit && hit2 != hit && hit3 != hit) {
                if (wp->timer == 0) {
                    wp->state = 10;
                } else {
                    wp->timer--;
                    wp->counter += 2;
                }
            } else {
                wp->timer = 30;
                wp->counter += 2;
                if (wp->counter > wp->limit) {
                    wp->state = 20;
                }
            }
            break;
        case 10:
            mapGrpFlagOff(wp->groupName, MAP_OBJ_FLAG_HIDDEN);
            wp->state = 99;
            break;
        case 20:
            psndSFXOn_3D("SFX_OFF_CLAUD_WIND3", &mario->position);
            if (wp->endEvt != NULL) {
                evtEntry(wp->endEvt, 0, 0);
            }
            img = imgNameToPtr(wp->imgName, group);
            img->animName = "A_2";
            img->animTime = animTimeGetTime(group);
            hitObjFlagOn(wp->hitName, 1);
            wp->timer = 60;
            wp->state++;
            break;
        case 21:
            if (wp->timer != 0) {
                wp->timer--;
                break;
            }
            wp->timer = 30;
            wp->counter += 2;
            wp->state = 99;
            break;
        case 99:
            img = imgNameToPtr(wp->imgName, group);
            if (!(wp->flags & 2)) {
                mapGrpClearOffScreen(wp->groupName);
            } else {
                char** p = wp->groupNames;
                while (*p != NULL) {
                    mapGrpClearOffScreen(*p);
                    p++;
                }
            }
            animPaperPoseRelease(img->poseId);
            imgFreeCapture(img, 0);
            imgSetShadow(img, 0);
            imgRelease(img, group);
            offscreenDelete(wp->offscreenName);
            cloud_once_flag = 0;
            wp->state = 0;
            break;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_cloud_init) {
    s32 i;

    for (i = 0; i < 16; i++) {
        cloud[i].flags = 0;
        sprintf(cloud[i].imgName, "i_name_%1d", i);
        sprintf(cloud[i].offscreenName, "o_name_%1d", i);
    }
    cloud_p = NULL;
    cloud_once_flag = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_cloud_ent) {
    EvtWord* args = event->args;
    s32 multi;
    char* groupName;
    char* hitName;
    void* endEvt;
    CloudWork* wp;
    s32 i;

    multi = evtGetValue(event, args[0]);
    groupName = (char*)evtGetValue(event, args[1]);
    hitName = (char*)evtGetValue(event, args[2]);
    endEvt = (void*)evtGetValue(event, args[3]);

    for (i = 0; i < 16; i++) {
        if (!(cloud[i].flags & 1)) {
            break;
        }
    }
    if (i >= 16) {
        return EVT_RETURN_DONE;
    }
    wp = &cloud[i];
    wp->flags |= 1;
    strncpy(wp->animName, "P_oof_cloud_1", 0x20);
    if (multi == 0) {
        strncpy(wp->groupName, groupName, 0x20);
    } else {
        strncpy(wp->groupName, *(char**)groupName, 0x20);
    }
    strncpy(wp->hitName, hitName, 0x20);
    wp->endEvt = endEvt;
    wp->limit = 180;
    wp->counter = 0;
    wp->state = 0;
    wp->timer = 0;
    wp->w = 0;
    wp->h = 0;
    wp->evt = evtEntry(evt_cloud_main_evt, 0, 0);
    wp->evt->lwData[0] = (EvtWord)wp;
    cloud_p = wp;
    if (multi != 0) {
        wp->groupNames = (char**)groupName;
        wp->flags |= 2;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_cloud_set_cap_size) {
    EvtWord* args = event->args;
    u16 w = evtGetValue(event, args[0]);
    u16 h = evtGetValue(event, args[1]);

    cloud_p->w = w;
    cloud_p->h = h;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_cloud_get_mode) {
    EvtWord* args = event->args;

    if (cloud_p == NULL) {
        evtSetValue(event, args[0], -1);
        return EVT_RETURN_DONE;
    }
    evtSetValue(event, args[0], cloud_p->state);
    return EVT_RETURN_DONE;
}
