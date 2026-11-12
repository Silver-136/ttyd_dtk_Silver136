// las_14 (ラストダンジョン, Palace of Shadow): a riddle room off las_10: the cloud puzzle solved with Flurrie (cloud
// map capture).

#include "version.h"
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
#include "evt/evt_cloud_room.h"

static CloudRoomWork* cloud;
static u8 cloud_once_flag;
static void _mapsetofs_all(CloudRoomWork* wp);
static void _mapclsofs_all(CloudRoomWork* wp);
static BOOL _get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h);
static void _img_alloc_capture(EvtWord name, EvtWord shadowId, EvtWord offName, EvtWord allocMode, EvtWord x, EvtWord y, EvtWord w, EvtWord h);

static USER_FUNC(evt_cloud_main);
static USER_FUNC(evt_las_cloud_ent);
static USER_FUNC(evt_las_cloud_init);

static EvtWord w_door_close[12];
static EvtWord w_door_open[12];
static BeroEntry bero_entry_data[2];
static EvtWord box_evt[30];
static const char* capturemap[3];
Vec v_list[8];
static EvtWord evt_cloud_main_evt[4];
static EvtWord cloud_evt[35];
static EvtWord touch_evt[2];
static EvtWord hint_plate[14];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord las_14_init_evt[];
#else
EvtWord las_14_init_evt[81];
#endif

/**
 * Calls the evt_img_alloc_capture script command from C:
 * builds a fake event with the eight arguments (image name, shadow id, offscreen name, alloc mode, and the x, y,
 * width, height of the capture rectangle).
 */
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

/**
 * Projects the eight corners of a 100 by 100 by 100 box around the hit object named name to the screen
 * and returns in x, y, w, h their screen rectangle, with partial edge clamping against the 608 by 480 capture buffer.
 * Always returns TRUE.
 */
static BOOL _get_boundingbox(const char* name, s32* x, s32* y, s32* w, s32* h) {
    Vec pos;
    Vec screen;
    s32 minX;
    s32 maxX;
    s32 minY;
    s32 maxY;
    s32 sx;
    s32 sy;

    hitObjGetPos(name, &pos);
    PSVECAdd(&pos, &v_list[0], &screen);
    getScreenPoint(&screen, &screen);
    sx = screen.x;
    sy = screen.y;

    minX = sx;
    maxX = sx;
    minY = sy;
    maxY = sy;
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

/**
 * Attaches the cloud offscreen buffer of wp to every map group listed in wp->groupNames (up to 8),
 * so those groups are drawn into the capture.
 */
static void _mapsetofs_all(CloudRoomWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        // FAKE: (s8) cast on the first character (extsb); the plain char test compiles cmplwi
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpSetOffScreen(wp->groupNames[i], wp->offscreenName);
    }
}

/** Detaches the cloud offscreen buffer from every map group listed in wp->groupNames. */
static void _mapclsofs_all(CloudRoomWork* wp) {
    s32 i;

    for (i = 0; i < 8; i++) {
        // FAKE: (s8) cast on the first character (extsb); the plain char test compiles cmplwi
        if ((s8)wp->groupNames[i][0] == 0) {
            break;
        }
        mapGrpClearOffScreen(wp->groupNames[i]);
    }
}

/**
 * Door animation script (closing) for the west door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panel S_door1 by it.
 */
static EVT_BEGIN(w_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

/**
 * Door animation script (opening) for the west door, used as the enter/exit animation of this room's bero entry.
 * LW(0) holds the angle step of the swing, and the script turns the door panel S_door1 by it.
 */
static EVT_BEGIN(w_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING("S_door1"), 0, LW(0), 0)
    RETURN
EVT_END()

/** Exit of las_14: west door w_bero_1 to las_10 (se_bero_1). */
static BeroEntry bero_entry_data[] = {
    {
        "w_bero_1", // hitName
        1, // type
        1, // sfxId
        BERO_DIR_AUTO, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Event script: the key chest of las_14 opens and Mario receives Palace Key 0x29. */
static EVT_BEGIN(box_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
    CALL(evt_item_entry, STRING("key"), ITEM_PALACE_KEY_0x29, LW(0), LW(1), LW(2), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("key"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map groups (S_kabe1, S_ten) that the cloud of this room (evt_las_cloud_ent) draws into its offscreen
 * buffer; the cloud's paper animation is a capture of them. The empty string ends the list.
 */
static const char* capturemap[] = {
    "S_kabe1",
    "S_ten",
    "",
};

/**
 * The eight corners of a 100-unit box around a cloud's hit object; _get_boundingbox projects them to the
 * screen to size the cloud capture.
 */
Vec v_list[] = {
    {-50.0f, 100.0f, 50.0f},
    {50.0f, 100.0f, 50.0f},
    {50.0f, 0.0f, 50.0f},
    {-50.0f, 0.0f, 50.0f},
    {-50.0f, 100.0f, -50.0f},
    {50.0f, 100.0f, -50.0f},
    {50.0f, 0.0f, -50.0f},
    {-50.0f, 0.0f, -50.0f},
};

/**
 * Event script: runs the cloud-blowing mechanism (evt_cloud_main) of las_14,
 * the room where Flurrie's gust uncovers the way.
 */
static EVT_BEGIN(evt_cloud_main_evt)
    CALL(evt_cloud_main)
    RETURN
EVT_END()

/**
 * Event script: the cloud puzzle of las_14 is solved.
 * The key chest becomes visible and solid, the collision A_kura is turned off,
 * GSWF_LAS_14_WALL_BLOWN_AWAY is set and a success jingle plays.
 */
static EVT_BEGIN(cloud_evt)
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_NO_DISP)
    CALL(evt_mobj_hit_onoff, STRING("box"), 1)
    CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
    SET_VALUE(GSWF_LAS_14_WALL_BLOWN_AWAY, 1)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_ENV_SUCCESS1"))
    WAIT_MS(2000)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    RETURN
EVT_END()

/** A-button handler (facing the cloud hit area); empty. */
static EVT_BEGIN(touch_evt)
    RETURN
EVT_END()

/** Event script: reading the hint plate of las_14 (stg8_las_39). */
static EVT_BEGIN(hint_plate)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("stg8_las_39"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Map init script for las_14, a Riddle Tower room (the chest holds Palace Key 0x29):
 * music, the door to las_10, the cloud Flurrie must blow away (hit area A_kura)
 * until GSWF_LAS_14_WALL_BLOWN_AWAY, the key chest (hidden until solved) and the hint plate.
 */
EVT_BEGIN(las_14_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG8_LST2"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG8_LAS1"))
#if VERSION >= VERSION_JP
    CALL(evt_snd_set_rev_mode, 2)
#endif
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    CALL(evt_hitobj_onoff, STRING("A_taka"), 1, 0)
    IF_INT_EQUAL(GSWF_LAS_14_WALL_BLOWN_AWAY, 0)
        CALL(evt_las_cloud_init)
        CALL(evt_las_cloud_ent, PTR(&capturemap), STRING("A_kura"), PTR(cloud_evt))
        CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_kura"), 0, PTR(touch_evt), 0)
    ELSE
        CALL(evt_hitobj_onoff, STRING("A_kura"), 1, 0)
    END_IF
    CALL(evt_mobj_itembox, STRING("box"), 0, 0, 0, MOBJ_ITEMBOX_NORMAL, 0, PTR(box_evt), GSWF_LAS_14_PALACE_KEY_CHEST)
    IF_INT_EQUAL(GSWF_LAS_14_WALL_BLOWN_AWAY, 0)
        CALL(evt_mobj_flag_onoff, 1, STRING("box"), MOBJ_FLAG_NO_DISP)
        CALL(evt_mobj_hit_onoff, STRING("box"), 0)
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("A_pure"), 0, PTR(hint_plate), 0)
    RETURN
EVT_END()

/**
 * Script helper (runs as its own event, one per cloud): the state machine of the cloud puzzle.
 * While Flurrie is the partner and her gust hits the cloud's hit area, it captures the cloud's paper animation,
 * plays the wind sound, and advances a counter by 2 per update in the blowing state, including brief hit losses;
 * once the count passes the limit (180) while the gust hits, it runs the end event and the cloud disappears for good.
 * Leaving gust motion starts cleanup immediately; losing all hit matches uses a 30-update grace counter.
 */
static USER_FUNC(evt_cloud_main) {
    CloudRoomWork* wp = (CloudRoomWork*)event->lwData[0];
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
    if (party->currentMemberId != PARTY_FLURRIE) {
        return EVT_RETURN_BLOCK;
    }
    hit0 = cloudGetHitBreathout(0);
    hit1 = cloudGetHitBreathout(1);
    hit2 = cloudGetHitBreathout(2);
    hit3 = cloudGetHitBreathout(3);
    switch (wp->state) {
        case 0:
            // FAKE: (s32) cast on the u8 flag gives cmpwi; the plain compare gives cmplwi
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

static USER_FUNC(evt_las_cloud_init) {
    s32 i;

    cloud = _mapAlloc(sizeof(CloudRoomWork) * 16);
    for (i = 0; i < 16; i++) {
        cloud[i].used = 0;
        sprintf(cloud[i].imgName, "i_name_%1d", i);
        sprintf(cloud[i].offscreenName, "o_name_%1d", i);
    }
    cloud_once_flag = 0;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: registers a cloud puzzle.
 * args[0] is the list of map groups to capture, args[1] the cloud's hit object name,
 * args[2] the event to run when the cloud has been blown away; starts evt_cloud_main_evt for it.
 */
static USER_FUNC(evt_las_cloud_ent) {
    EvtWord* args = event->args;
    char** groups;
    char* hitName;
    void* endEvt;
    CloudRoomWork* wp;
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
        // FAKE: (s8) cast on the first character (extsb); the plain char test compiles cmplwi
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
