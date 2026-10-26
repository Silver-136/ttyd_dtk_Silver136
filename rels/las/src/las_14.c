#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "driver/mapdrv.h"
#include "evt/evt_img.h"
#include "evt/evt_hit.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_case.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "driver/mobjdrv.h"
#include "mario/mario_pouch.h"
#include "driver/casedrv.h"
#include "driver/itemdrv.h"

#include "driver/hitdrv.h"
#include "driver/camdrv.h"
#include "dolphin/mtx.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "party.h"
#include "party/party_cloud.h"
#include "pmario_sound.h"
#include "driver/animdrv.h"
#include "driver/imgdrv.h"
#include "driver/offscreendrv.h"
#include "string.h"
#include "memory.h"
#include "stdio.h"
#include "evt/evt_bero.h"
#include "las.h"

typedef struct CloudWork {
    s32 used;
    s32 state;
    char animName[0x20];
    char imgName[0x20];
    char offscreenName[0x20];
    char groupNames[8][0x20];
    char hitName[0x20];
    s32 limit;
    s32 counter;
    EventEntry* evt;
    void* endEvt;
    s32 timer;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    f32 lastFrame;
} CloudWork;
void mapGrpSetOffScreen(const char* name, const char* offscreen);
static CloudWork* cloud;
static u8 cloud_once_flag;
static void _mapsetofs_all(CloudWork* wp);
static void _mapclsofs_all(CloudWork* wp);
static BOOL _get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h);
static void _img_alloc_capture(EvtWord name, EvtWord shadowId, EvtWord offName, EvtWord allocMode, EvtWord x, EvtWord y, EvtWord w, EvtWord h);

static USER_FUNC(evt_cloud_main);
USER_FUNC(evt_las_cloud_ent);
USER_FUNC(evt_las_cloud_init);

static EvtWord w_door_close[12];
static EvtWord w_door_open[12];
static BeroEntry bero_entry_data[2];
static EvtWord box_evt[30];
static const char* capturemap[3];
static Vec v_list[8];
static EvtWord evt_cloud_main_evt[4];
static EvtWord cloud_evt[35];
EvtWord touch_evt[2];
static EvtWord hint_plate[14];
EvtWord las_14_init_evt[81];

static void _img_alloc_capture(EvtWord name, EvtWord shadowId, EvtWord offName, EvtWord allocMode, EvtWord x, EvtWord y, EvtWord w, EvtWord h) {
    EventEntry event;
    EvtWord args[10];

    event.args = args;
    args[0] = name;
    args[1] = shadowId;
    args[2] = offName;
    args[3] = allocMode;
    args[4] = x;
    args[5] = y;
    args[6] = w;
    args[7] = h;
    evt_img_alloc_capture(&event, TRUE);
}

static BOOL _get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h) {
    Vec pos;
    Vec screen;
    s32 minX;
    s32 maxX;
    s32 minY;
    s32 maxY;

    hitObjGetPos(name, &pos);
    PSVECAdd(&pos, &v_list[0], &screen);
    getScreenPoint(&screen, &screen);
    {
        s32 sx = screen.x;
        s32 sy = screen.y;

        minX = sx;
        maxX = sx;
        minY = sy;
        maxY = sy;
    }
    {
        s32 i;

        for (i = 1; i < 8; i++) {
            PSVECAdd(&pos, &v_list[i], &screen);
            getScreenPoint(&screen, &screen);
            if (screen.x < minX) {
                minX = screen.x;
            }
            if (maxX < screen.x) {
                maxX = screen.x;
            }
            if (screen.y < minY) {
                minY = screen.y;
            }
            if (maxY < screen.y) {
                maxY = screen.y;
            }
        }
    }
    *x = minX + 304;
    *y = minY + 240;
    *w = maxX - minX;
    *h = maxY - minY;
    if (*x < 0) {
        *x = 0;
    }
    if (*y < 0) {
        *y = 0;
    }
    if (*x + *w > 608) {
        *w = 608 - *x;
    }
    if (*y + *h > 480) {
        *h = 480 - *y;
    }
    if (*x < 0) {
        *x = 0;
    }
    if (*y < 0) {
        *y = 0;
    }
    return TRUE;
}

static void _mapsetofs_all(CloudWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpSetOffScreen(wp->groupNames[i], wp->offscreenName);
    }
}

static void _mapclsofs_all(CloudWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpClearOffScreen(wp->groupNames[i]);
    }
}

static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        1, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "las_10", // nextMapName
        "se_bero_1", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        w_door_close, // enterAnimeArg
        w_door_open, // exitAnimeArg
    },
    {0},
};

static EVT_BEGIN(box_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("key"), ITEM_PALACE_KEY_0x29, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("key"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static const char* capturemap[] = {
    "S_kabe1",
    "S_ten",
    "",
};

static Vec v_list[] = {
    {-50.0f, 100.0f, 50.0f},
    {50.0f, 100.0f, 50.0f},
    {50.0f, 0.0f, 50.0f},
    {-50.0f, 0.0f, 50.0f},
    {-50.0f, 100.0f, -50.0f},
    {50.0f, 100.0f, -50.0f},
    {50.0f, 0.0f, -50.0f},
    {-50.0f, 0.0f, -50.0f},
};

static EVT_BEGIN(evt_cloud_main_evt)
    CALL(evt_cloud_main)
    RETURN
EVT_END()

static EVT_BEGIN(cloud_evt)
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_NO_DISP)
    CALL(evt_mobj_hit_onoff, STRING("box"), 1)
    CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
    SET_VALUE(GSWF(4344), 1)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    RETURN
EVT_END()

EVT_BEGIN(touch_evt)
    RETURN
EVT_END()

static EVT_BEGIN(hint_plate)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_39"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(las_14_init_evt)
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG8_LST2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
    CALL(evt_snd_set_rev_mode, 2)
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_hitobj_onoff, STRING("A_taka"), 1, 0)
    IF_INT_EQUAL(GSWF(4344), 0)
        CALL(evt_las_cloud_init)
        CALL(evt_las_cloud_ent, PTR(&capturemap), STRING("A_kura"), PTR(cloud_evt))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kura"), 0, PTR(touch_evt), 0)
        ELSE
        CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
    END_IF
    CALL(evt_mobj_itembox, STRING("box"), 0, 0, 0, 0, 0, PTR(box_evt), GSWF(4365))
    IF_INT_EQUAL(GSWF(4344), 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("box"), MOBJ_FLAG_NO_DISP)
        CALL(evt_mobj_hit_onoff, STRING("box"), 0)
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_pure"), 0, PTR(hint_plate), 0)
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
    if (party->currentMemberId != PARTY_MEMBER_FLURRIE) {
        return EVT_RETURN_BLOCK;
    }
    hit0 = (HitEntry*)cloudGetHitBreathout(0);
    hit1 = (HitEntry*)cloudGetHitBreathout(1);
    hit2 = (HitEntry*)cloudGetHitBreathout(2);
    hit3 = (HitEntry*)cloudGetHitBreathout(3);
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
            _mapsetofs_all(wp);
            wp->state++;
            break;
        case 2:
            imgEntry(wp->imgName, group);
            img = imgNameToPtr(wp->imgName, group);
            img->poseId = animPaperPoseGetId(wp->animName, group);
            img->animTime = animTimeGetTime(group);
            imgClearVirtualPoint(img);
            imgSetShadow(img, 2);
            img->flags |= IMAGE_FLAG_HIDE;
            wp->state = 222;
            break;
        case 222:
            if (!_get_boundingbox(wp->hitName, &wp->x, &wp->y, &wp->w, &wp->h)) {
                break;
            }
            _img_alloc_capture((EvtWord)wp->imgName, 0, (EvtWord)wp->offscreenName, 1, wp->x, wp->y, wp->w, wp->h);
            wp->state = 223;
            break;
        case 223:
            img = imgNameToPtr(wp->imgName, group);
            img->flags &= ~IMAGE_FLAG_HIDE;
            wp->state = 3;
        case 3:
            img = imgNameToPtr(wp->imgName, group);
            img->animName = "A_1";
            img->animTime = animTimeGetTime(group);
            img->flags &= ~IMAGE_FLAG_HIDE;
            wp->timer = 30;
            wp->lastFrame = 0.0f;
            wp->state++;
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
            wp->state = 100;
            break;
        case 99:
        case 100:
            img = imgNameToPtr(wp->imgName, group);
            _mapclsofs_all(wp);
            animPaperPoseRelease(img->poseId);
            imgFreeCapture(img, 0);
            imgSetShadow(img, 0);
            imgRelease(img, group);
            offscreenDelete(wp->offscreenName);
            cloud_once_flag = 0;
            if (wp->state == 99) {
                wp->state = 0;
                break;
            }
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(evt_las_cloud_init) {
    s32 i;

    cloud = _mapAlloc(sizeof(CloudWork) * 16);
    for (i = 0; i < 16; i++) {
        cloud[i].used = 0;
        sprintf(cloud[i].imgName, "i_name_%1d", i);
        sprintf(cloud[i].offscreenName, "o_name_%1d", i);
    }
    cloud_once_flag = 0;
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_las_cloud_ent) {
    EvtWord* args = event->args;
    char** groups;
    char* hitName;
    void* endEvt;
    CloudWork* wp;
    s32 i;
    s32 j;

    groups = (char**)evtGetValue(event, args[0]);
    hitName = (char*)evtGetValue(event, args[1]);
    endEvt = (void*)evtGetValue(event, args[2]);

    for (i = 0; i < 16; i++) {
        if (cloud[i].used == 0) {
            break;
        }
    }
    if (i >= 16) {
        return EVT_RETURN_DONE;
    }
    wp = &cloud[i];
    wp->used = 1;
    strcpy(wp->animName, "P_oof_cloud_1");
    for (j = 0; j < 8; j++) {
        if ((s8)groups[j][0] == 0) {
            break;
        }
        strcpy(wp->groupNames[j], groups[j]);
    }
    strcpy(wp->hitName, hitName);
    wp->endEvt = endEvt;
    wp->limit = 180;
    wp->counter = 0;
    wp->state = 0;
    wp->timer = 0;
    wp->evt = evtEntry(evt_cloud_main_evt, 0, 0);
    wp->evt->lwData[0] = (EvtWord)wp;
    return EVT_RETURN_DONE;
}

