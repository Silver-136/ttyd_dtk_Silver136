// Pirate's Grotto (dou) lecture script: the paper genie (peramajin) teaches the boat move on a ship-change platform, run when the Boat Curse is found in dou_07.

#include "version.h"
#include "icon_ids.h"
#include "evt/evt_window.h"
#include "eff/eff_fukidashi.h"
#include "eff/eff_itemget.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "pmario_sound.h"
#include "statuswindow.h"
#include "driver/hitdrv.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_lecture.h"
#include "mot/mot_slit.h"
#include "mot/mot_roll.h"
#include "driver/npcdrv.h"

#include "mario/mario.h"
#include "manager/evtmgr_cmd.h"
#include "dolphin/os.h"
#include "driver/icondrv.h"
#include "driver/dispdrv.h"
#include "string.h"
#include "driver/mapdrv.h"
#include "driver/effdrv.h"
#include "driver/camdrv.h"
#include "mario/mariost.h"
#include "dolphin/mtx.h"
#include "driver/mobjdrv.h"
#include "driver/fadedrv.h"
#include "system.h"
#include "evt/evt_map.h"
#include <dolphin/pad.h>
#include "evt/evt_lect.h"

static OSTime wwstart;
static s32 x_flag_data[2];
static void lecture_button_disp(CameraId camera, void* param);
static LectButton lbtn[2];
/**
 * Moves the camera by the offset ofs: the new target position and look-at are the current ones plus ofs,
 * applied instantly (zero-length move) with curve EASING_SINE_IN_OUT.
 * Used to jump the camera between the lecture stage and the lecture's item- get view.
 */
static inline void effitemget_cam_move(CameraEntry* camera, Vec ofs) {
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    PSVECAdd(&camera->cameraPos, &ofs, &camera->storedPos);
    PSVECAdd(&camera->cameraAt, &ofs, &camera->storedAt);
    camera->startTimeA = gp->renderTime;
    camera->durationA = 0;
    camera->stateA = 3;
    camera->curveA = EASING_SINE_IN_OUT;
}

static Vec daiv;

static USER_FUNC(dai_moveti_get);
static USER_FUNC(dai_moveti_set);
static USER_FUNC(evtflash);
static USER_FUNC(lect_map_cam_chg);
static USER_FUNC(lect_status_off);
static USER_FUNC(lect_status_on);
static USER_FUNC(lecture_button_flag_on_off);
static USER_FUNC(lecture_button_rtn);
static USER_FUNC(lecture_button_seq_wait);
static USER_FUNC(lecture_button_set_icon_id);
static USER_FUNC(lecture_button_set_pos);
static USER_FUNC(lecture_button_set_seq);
static USER_FUNC(x_flag_get);
static USER_FUNC(x_flag_set);
static USER_FUNC(x_flag_wait);
static USER_FUNC(bgm_start_wait);
static USER_FUNC(evt_nomal_cange);
static USER_FUNC(lect_push_r_check);
static USER_FUNC(lect_slit_check);
static USER_FUNC(lect_mobj_hit_nasi);
static USER_FUNC(effitemget_busaiku);
static USER_FUNC(effitemget_open_ptr);
static USER_FUNC(effitemget_open);
static USER_FUNC(effitemgetchange_cam_ptr);
static USER_FUNC(effitemgetchange_cam);
static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings);
static USER_FUNC(lecture_button_get_icon_id);
static USER_FUNC(lecture_button_set_alpha);
static USER_FUNC(x_flag_add);
static USER_FUNC(lect_mario_ido_sum);
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);

EvtWord ship_majin_lecture_init[63];
#if VERSION <= VERSION_DEMO_US
extern EvtWord ship_majin_lecture[];
#else
EvtWord ship_majin_lecture[888];
#endif

/**
 * Weighted average used for the lecture's Mario movement:
 * (remaining * end + elapsed * start) / total, so it runs from end at elapsed = 0 to start at elapsed = total.
 */
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 from, f32 to) {
    f32 rem = total - elapsed;

    return (rem * from + elapsed * to) / total;
}

/**
 * Script helper: moves Mario in a straight line over args[0] milliseconds from the position saved in LW(0) to
 * LW(2) on its first call to the target in LW(3) to LW(5), setting his position directly.
 * Done when the time is up.
 */
static USER_FUNC(lect_mario_ido_sum) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();
    s32 durationMs = evtGetValue(event, args[0]);
    s32 elapsed;

    if (isFirstCall) {
        wwstart = event->lifetime;
        evtSetFloat(event, LW(0), mario->position.x);
        evtSetFloat(event, LW(1), mario->position.y);
        evtSetFloat(event, LW(2), mario->position.z);
    }
    elapsed = OSTicksToMilliseconds((u64)OSDiffTick(event->lifetime, wwstart));
    if (elapsed > durationMs) {
        elapsed = durationMs;
    }
    mario->position.x = lect_mario_ido_sub(elapsed, durationMs, evtGetFloat(event, LW(3)), evtGetFloat(event, LW(0)));
    mario->position.y = lect_mario_ido_sub(elapsed, durationMs, evtGetFloat(event, LW(4)), evtGetFloat(event, LW(1)));
    mario->position.z = lect_mario_ido_sub(elapsed, durationMs, evtGetFloat(event, LW(5)), evtGetFloat(event, LW(2)));
    if (elapsed >= durationMs) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Script helper: lifts the force-close of the status window set by lect_status_off. */
static USER_FUNC(lect_status_on) {
    statusWinForceClose();
    return EVT_RETURN_DONE;
}

/** Script helper: forcibly closes the status window (HP and FP display) so it stays out of the lecture. */
static USER_FUNC(lect_status_off) {
    statusWinForceCloseClear();
    return EVT_RETURN_DONE;
}

/**
 * Script helper: stores args[1] in lecture flag number args[0] (there are two,
 * used to synchronise the lecture's parallel events).
 */
static USER_FUNC(x_flag_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper: adds args[1] to lecture flag number args[0]. */
static USER_FUNC(x_flag_add) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] += evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper: reads lecture flag number args[0] into args[1]. */
static USER_FUNC(x_flag_get) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], x_flag_data[index]);
    return EVT_RETURN_DONE;
}

/** Script helper: blocks until lecture flag number args[0] equals args[1]. */
static USER_FUNC(x_flag_wait) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);

    if (value == x_flag_data[index]) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/**
 * Draw callback for the two lecture button prompts:
 * draws each visible button's icon at its screen position with its alpha (nine of the icons are shifted up 15
 * units).
 */
static void lecture_button_disp(CameraId camera, void* param) {
    s32 i;

    for (i = 0; i < 2; i++) {
        LectButton* btn = &lbtn[i];
        LectButtonDisp* disp = &btn->disp;

        if (!(btn->flags & LECT_BUTTON_FLAG_HIDDEN)) {
            f32 ofs;
            Vec pos;

            switch (btn->iconId) {
                case ICON_STICK_DOWN_LEFT:
                case ICON_STICK_DOWN:
                case ICON_STICK_DOWN_RIGHT:
                case ICON_STICK_LEFT:
                case ICON_STICK_NEUTRAL:
                case ICON_STICK_RIGHT:
                case ICON_STICK_UP_LEFT:
                case ICON_STICK_UP:
                case ICON_STICK_UP_RIGHT:
                    ofs = -15.0f;
                    break;
                default:
                    ofs = 0.0f;
                    break;
            }
            pos = disp->pos;
            pos.y += ofs;
            iconDispGxAlpha(&pos, ICON_FLAG_2D | ICON_FLAG_NO_MAP_BLEND, btn->iconId, btn->alpha, 1.0f);
        }
    }
}

/**
 * Script helper that runs the lecture's two button prompts:
 * clears them on its first call, then every frame fades each in (sequence 2)
 * or out (sequence 3) by 7 alpha per frame, hides them on sequence 4, and queues lecture_button_disp.
 * Finishes when both are hidden (sequence 4).
 */
static USER_FUNC(lecture_button_rtn) {
    LectButton* btn;
    s32 i;
    s32 done;

    if (isFirstCall) {
        memset(lbtn, 0, sizeof(lbtn));
        {
            Vec pos = {200.0f, -25.0f, 0.0f};

            for (i = 0; i < 2; i++) {
                lbtn[i].disp.pos = pos;
            }
        }
    }
    done = 0;
    for (i = 0; i < 2; i++) {
        btn = &lbtn[i];
        switch (btn->seq) {
            case 0:
                btn->iconId = ICON_BUTTON_X;
                btn->seq = 1;
                break;
            case 2:
                btn->alpha += 7;
                if (btn->alpha >= 255) {
                    btn->alpha = 255;
                    btn->seq = 1;
                }
                break;
            case 3:
                btn->alpha -= 7;
                if (btn->alpha <= 0) {
                    btn->alpha = 0;
                    btn->seq = 1;
                }
                break;
            case 4:
                btn->flags |= 1;
                done++;
                break;
        }
    }
    if (done >= 2) {
        return EVT_RETURN_DONE;
    }
    dispEntry(CAMERA_2D, 0.0f, 1, lecture_button_disp, NULL);
    return EVT_RETURN_BLOCK;
}

/** Script helper: sets the icon of button args[0] to icon id args[1]. */
static USER_FUNC(lecture_button_set_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->iconId = value;
    return EVT_RETURN_DONE;
}

/** Script helper: sets the alpha of button args[0] to args[1]. */
static USER_FUNC(lecture_button_set_alpha) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->alpha = value;
    return EVT_RETURN_DONE;
}

/** Script helper: turns the flag bits args[1] on (args[2] nonzero) or off for button args[0]. */
static USER_FUNC(lecture_button_flag_on_off) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    u8 flag = evtGetValue(event, args[1]);

    s32 on = evtGetValue(event, args[2]);
    LectButton* btn = &lbtn[index];

    if (on) {
        btn->flags |= flag;
    } else {
        btn->flags &= ~flag;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: sets the fade sequence of button args[0] to args[1] (2 fades in, 3 fades out, 4 hides, 1 is idle). */
static USER_FUNC(lecture_button_set_seq) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->seq = value;
    return EVT_RETURN_DONE;
}

/** Script helper: reads the sequence number (not the icon id, despite the name) of button args[0] into args[1]. */
static USER_FUNC(lecture_button_get_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    LectButton* btn = &lbtn[index];

    evtSetValue(event, args[1], btn->seq);
    return EVT_RETURN_DONE;
}

/** Script helper: blocks until button args[0] has reached sequence args[1] (usually 1, idle after a fade). */
static USER_FUNC(lecture_button_seq_wait) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 seq = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    if (btn->seq != seq) {
        return EVT_RETURN_BLOCK;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: moves button args[0] to the screen position args[1..3]. */
static USER_FUNC(lecture_button_set_pos) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    f32 x = evtGetFloat(event, args[1]);
    f32 y = evtGetFloat(event, args[2]);
    f32 z = evtGetFloat(event, args[3]);
    LectButton* btn = &lbtn[index];

    btn->disp.pos = (Vec){x, y, z};
    return EVT_RETURN_DONE;
}

/**
 * Sets the camera id of a map object and its children (and its siblings when siblings is set),
 * so a whole object draws on another camera.
 */
static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings) {
    obj->cameraId = camId;
    if (obj->child != NULL) {
        lect_map_cam_chg_sub(obj->child, camId, TRUE);
    }
    if (siblings && obj->next != NULL) {
        lect_map_cam_chg_sub(obj->next, camId, TRUE);
    }
}

/** Script helper: puts the map object named args[0] (with its children) on camera id args[1]. */
static USER_FUNC(lect_map_cam_chg) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);
    MapObject* obj = mapGetMapObj(name);

    if (obj != NULL) {
        lect_map_cam_chg_sub(obj, camId, FALSE);
    }
    return EVT_RETURN_DONE;
}

/** Script helper: moves the item-get effect named args[0] to camera id args[1]. */
static USER_FUNC(effitemgetchange_cam) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    ((ItemGetWork*)effNameToPtr(name)->userdata)->camera = camId;
    return EVT_RETURN_DONE;
}

/** Script helper: as effitemgetchange_cam, but the effect is given by pointer in args[0]. */
static USER_FUNC(effitemgetchange_cam_ptr) {
    EvtWord* args = event->args;
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, args[0]);

    ((ItemGetWork*)eff->userdata)->camera = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper: opens the item-get effect named args[0] (sets its state to 1). */
static USER_FUNC(effitemget_open) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/** Script helper: as effitemget_open, but the effect is given by pointer in args[0]. */
static USER_FUNC(effitemget_open_ptr) {
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, event->args[0]);

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: moves the 3D camera 2016 units up (args[0] nonzero) or down. Nothing in this file calls it;
 * the lecture copies in eki, gor, hei and mri lift the camera and Mario 2016 units, capture the screen
 * into an image for the paper-seal transition one frame later, and move both back.
 */
static USER_FUNC(effitemget_busaiku) {
    s32 up = evtGetValue(event, event->args[0]);
    CameraEntry* camera = camGetPtr(CAMERA_3D);
    Vec ofs;

    if (up) {
        ofs = (Vec){0.0f, 2016.0f, 0.0f};
    } else {
        ofs = (Vec){0.0f, -2016.0f, 0.0f};
    }
    effitemget_cam_move(camera, ofs);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: switches on the hit groups of the map object named args[0] ("me" means the object running the
 * event), skipping unnamed ones.
 */
static USER_FUNC(lect_mobj_hit_nasi) {
    const char* name = (const char*)evtGetValue(event, event->args[0]);
    MapObjectEntry* mobj;
    s32 i;

    if (strcmp(name, "me") != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = event->thisMapObj;
    }
    for (i = 0; i < 2; i++) {
        if (mobj->hitObj[i].name != NULL && strcmp(mobj->hitObj[i].name, "") != 0) {
            hitGrpFlagOn(mobj->hitObj[i].name, HIT_FLAG_OFF);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: stores the offset args[0..2] (floats) for the ship-change platform (daiv),
 * which the lecture then applies to the platform every frame.
 */
static USER_FUNC(dai_moveti_set) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    daiv = (Vec){x, y, z};
    return EVT_RETURN_DONE;
}

/** Script helper: reads the platform offset set by dai_moveti_set into args[0..2]. */
static USER_FUNC(dai_moveti_get) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], daiv.x);
    evtSetFloat(event, args[1], daiv.y);
    evtSetFloat(event, args[2], daiv.z);
    return EVT_RETURN_DONE;
}

/** Script helper: sets args[0] to TRUE if Mario is currently in paper-thin (slit) mode. */
static USER_FUNC(lect_slit_check) {
    EvtWord* args = event->args;

    if (marioGetPtr()->flags & MARIO_FLAG_PAPER_MODE) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper: a camera flash for the genie's light:
 * fades out to white in 40 ms, holds for 60 ms, fades back in over 40 ms and holds 60 ms.
 */
static USER_FUNC(evtflash) {
    if (isFirstCall) {
        event->userdata[2] = 0;
    }
    switch (event->userdata[2]) {
        case 0:
            fadeEntry(FADE_OUT_BLACK, 40, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            event->userdata[2]++;
        case 1:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[0] = 60;
            event->savedTime = (s32)gp->renderTime;
            event->userdata[2]++;
            if (event->userdata[0] == 0) {
                event->userdata[2]++;
            }
            return EVT_RETURN_BLOCK;
        case 2:
            if (OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) < (u32)event->userdata[0]) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[2]++;
            return EVT_RETURN_BLOCK;
        case 3:
            fadeEntry(FADE_IN_BLACK, 40, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
            event->userdata[2]++;
        case 4:
            if (fadeIsFinish() != TRUE) {
                return EVT_RETURN_BLOCK;
            }
            event->userdata[0] = 60;
            event->savedTime = (s32)gp->renderTime;
            event->userdata[2]++;
            if (event->userdata[0] == 0) {
                event->userdata[2]++;
            }
            return EVT_RETURN_BLOCK;
        case 5:
            if (OSTicksToMilliseconds((u32)gp->renderTime - (u32)event->savedTime) < (u32)event->userdata[0]) {
                return EVT_RETURN_BLOCK;
            }
            return EVT_RETURN_DONE;
    }
    return EVT_RETURN_DONE;
}

/**
 * Prepares the boat-move lecture: switches the system level for the lecture,
 * puts Mario aside while the camera is set relative to him, restores him and starts him facing right,
 * and puts the ship-change platform on camera 7.
 */
EVT_BEGIN(ship_majin_lecture_init)
    CALL(lect_set_systemlevel, 0)
    CALL(lect_test_systemlevel)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(0.0f), FLOAT(42.0f), FLOAT(-101.799805f), 1000, EASING_LINEAR)
    WAIT_MS(1000)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_pos, -300, -600, 0)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(0.0f), FLOAT(42.0f), FLOAT(-101.799805f), 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pos, -300, -600, 0)
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(lect_map_cam_chg, STRING("S_Ship_Change_01"), 7)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

/**
 * The paper genie's boat-move lecture on the ship-change platform:
 * the genie curses Mario with flashes of light and explains Boat Mode (stg1_gon_majin4_12 to 14), the Boat
 * Panel slides in, and Mario must try it with a flashing button prompt: only Y works until he is a boat, he
 * sails right and back, then only Y works until he turns back. The practice repeats while the player answers
 * "Run that by me again" (stg1_gon_majin4_16, 17); otherwise the genie says goodbye (stg1_gon_majin4_18), the
 * panel slides back, the genie is moved out of sight and MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON is cleared.
 */
EVT_BEGIN(ship_majin_lecture)
    // Lecture starts: the on-screen button prompt is set up and the Boat Panel (S_Ship_Change_01) is placed
    // at x -50, its hit area following it from now on.
    CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON)
    BEGIN_SIBLING_EVENT
        CALL(lecture_button_rtn)
    END_SIBLING_EVENT
    WAIT_FRAMES(1)
    CALL(lecture_button_set_pos, 0, FLOAT(-200.0f), FLOAT(-25.0f), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("S_Ship_Change_01"))
        SET_VALUE(LW(1), STRING("A_rec"))
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
        CALL(dai_moveti_set, FLOAT(-50.0f), FLOAT(0.0f), FLOAT(0.0f))
        LOOP(0)
            CALL(dai_moveti_get, LW(2), LW(3), LW(4))
            CALL(evt_mapobj_trans, 1, LW(0), LW(2), LW(3), LW(4))
            CALL(evt_hit_bind_update, LW(1))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT

    // The paper genie is drawn on the lecture camera and kept just below its look-at point.
    CALL(evt_npc_flag_onoff, 1, STRING("ペラ魔人"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("ペラ魔人"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_camid, STRING("ペラ魔人"), 7)
    WAIT_FRAMES(1)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_cam_get_at, CAMERA_3D, LW(0), LW(1), LW(2))
            SUBTRACT(LW(1), 25)
            CALL(evt_npc_set_position, STRING("ペラ魔人"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT

    // The genie gloats and curses Mario, the light flashes over him four times, and the genie explains that Y
    // now turns him into a paper boat in certain places.
    CALL(evt_npc_set_anim, STRING("ペラ魔人"), STRING("P_1"))
    WAIT_MS(200)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_12"), 0, STRING("ペラ魔人"))
    CALL(evt_mario_set_pose, STRING("M_N_H"))
    LOOP(4)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_PERAMAJIN_LIGHT1"), LW(0), LW(1), LW(2), 0)
        CALL(evtflash)
    END_LOOP
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_13"), 0, STRING("ペラ魔人"))

    // The camera pans and the Boat Panel slides left from x -50 until it stops at x -270.
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(80.0f), FLOAT(42.0f), FLOAT(-101.799805f), 1000, EASING_SINE_IN_OUT)
    SET_FLOAT_VALUE(LW(1), FLOAT(2.6660156f))
    CALL(dai_moveti_get, LW(0), LW(2), LW(3))
    LOOP(0)
        SUBTRACT_FLOAT(LW(0), LW(1))
        IF_INT_LESS_OR_EQUAL(LW(0), FLOAT(-270.0f))
            SET_FLOAT_VALUE(LW(0), FLOAT(-270.0f))
            CALL(dai_moveti_set, LW(0), LW(2), LW(3))
            BREAK_LOOP
        ELSE
            CALL(dai_moveti_set, LW(0), LW(2), LW(3))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP

    // The genie tells Mario to try it: a Boat Panel glows when he stands on it.
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_14"), 0, STRING("ペラ魔人"))

    // Once Mario passes x -220 the camera follows him; the Boat Panel's glow events (evt_ride_airport while
    // he stands on it, evt_nonride_airport after) run.
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(0), -220)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        LOOP(0)
            CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(0.0f), FLOAT(42.0f), FLOAT(-101.799805f), 0, EASING_SINE_IN_OUT)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(x_flag_set, 0, 1)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("S_Ship_Change_01"))
        SET_VALUE(LW(1), STRING("Ship_Change_01"))
        LOOP(0)
            SPAWN_EVENT_GET_ID(PTR(evt_ride_airport), LW(2))
            WAIT_FRAMES(2)
            IS_EVENT_RUNNING(LW(2), LW(3))
            IF_INT_EQUAL(LW(3), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(x_flag_wait, 0, 2)
        IS_EVENT_RUNNING(LW(2), LW(3))
        IF_INT_EQUAL(LW(3), 1)
            DELETE_EVENT(LW(2))
        END_IF
        LOOP(0)
            SPAWN_EVENT_GET_ID(PTR(evt_nonride_airport), LW(2))
            WAIT_FRAMES(2)
            IS_EVENT_RUNNING(LW(2), LW(3))
            IF_INT_EQUAL(LW(3), 1)
                BREAK_LOOP
            END_IF
        END_LOOP
        IS_EVENT_RUNNING(LW(2), LW(3))
        IF_INT_EQUAL(LW(3), 1)
            DELETE_EVENT(LW(2))
        END_IF
    END_INLINE_EVENT

    // Mario walks onto the panel at x -200 and the genie tells him to press Y and become a paper boat.
    CALL(evt_mario_mov_pos2, FLOAT(-200.0f), FLOAT(0.0f), FLOAT(80.0f))
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_14_1"), 0, STRING("ペラ魔人"))

    // Practice loop: the button prompt appears and flashes while the player is asked to turn Mario into a
    // boat.
    LOOP(0)
        CALL(lecture_button_flag_on_off, 0, 2, 1)
        CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_Y)
        CALL(lecture_button_set_seq, 0, 2)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(x_flag_set, 0, 1)
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_Y_PRESSED)
                WAIT_MS(300)
                CALL(x_flag_get, 0, LW(0))
                IF_INT_EQUAL(LW(0), 2)
                    BREAK_LOOP
                END_IF
                CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_Y)
                WAIT_MS(300)
                CALL(x_flag_get, 0, LW(0))
                IF_INT_EQUAL(LW(0), 2)
                    BREAK_LOOP
                END_IF
            END_LOOP
        END_INLINE_EVENT

        // Only the Y button is enabled (stick off) until Mario is in boat form (MARIO_MOTION_SHIP).
        CALL(evt_cam_letter_box_disable, 1)
        CALL(mainwindow_opendisable)
        CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_DISABLE_STICK)
        CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, 2048)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_key_onoff, 1)
        LOOP(0)
            CALL(lect_status_on)
            WAIT_FRAMES(1)
            CALL(evt_mario_get_motion, LW(3))
            IF_INT_EQUAL(LW(3), 25)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(lect_status_off)
        CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_DISABLE_STICK)
        CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, 0)
        CALL(mainwindow_openenable)
        CALL(evt_cam_letter_box_disable, 0)

        // Controls are locked again; the button prompt fades.
        BEGIN_SIBLING_EVENT
            CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_Y_PRESSED)
            WAIT_MS(300)
            CALL(lecture_button_set_seq, 0, 3)
            CALL(lecture_button_seq_wait, 0, 1)
            CALL(lecture_button_flag_on_off, 0, 2, 0)
        END_SIBLING_EVENT
        CALL(x_flag_set, 0, 2)
        CALL(lecture_button_set_seq, 0, 3)

        // Mario sails right across the platform and back left, with the genie crowing about his shame.
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            IF_INT_LESS_OR_EQUAL(LW(1), FLOAT(-614.0f))
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_set_dir, 90, 300, 0)
        WAIT_MS(500)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_flag_on_off, 0, 2, 0)
        CALL(evt_msg_print_add, 0, STRING("stg1_gon_majin4_15"))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP1"), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_mario_mov_pos2, FLOAT(155.0f), FLOAT(0.0f), FLOAT(80.0f))
        CALL(evt_snd_sfxoff, LW(3))
        CALL(evt_mario_set_dir, 270, 300, 0)
        WAIT_MS(500)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_MARIO_SHIP1"), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_mario_mov_pos2, FLOAT(-145.0f), FLOAT(0.0f), FLOAT(80.0f))
        CALL(evt_snd_sfxoff, LW(3))
        WAIT_MS(500)

        // The genie tells Mario to return to normal by approaching the panel and pressing Y.
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_15_0"), 0, STRING("ペラ魔人"))
        CALL(lecture_button_flag_on_off, 0, 2, 1)
        CALL(lecture_button_set_pos, 0, FLOAT(-200.0f), FLOAT(-25.0f), FLOAT(0.0f))
        CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_Y)
        CALL(lecture_button_set_seq, 0, 2)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(x_flag_set, 0, 1)

        // The prompt is shown again; the player must turn back to Mario.
        BEGIN_INLINE_EVENT
            LOOP(0)
                CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_Y_PRESSED)
                WAIT_MS(300)
                CALL(x_flag_get, 0, LW(0))
                IF_INT_EQUAL(LW(0), 2)
                    BREAK_LOOP
                END_IF
                CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_Y)
                WAIT_MS(300)
                CALL(x_flag_get, 0, LW(0))
                IF_INT_EQUAL(LW(0), 2)
                    BREAK_LOOP
                END_IF
            END_LOOP
        END_INLINE_EVENT

        // Only the Y button is enabled (stick off) until Mario leaves boat form.
        CALL(evt_cam_letter_box_disable, 1)
        CALL(mainwindow_opendisable)
        CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_DISABLE_STICK)
        CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, 2048)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_key_onoff, 1)
        LOOP(0)
            CALL(lect_status_on)
            CALL(evt_mario_get_motion, LW(3))
            IF_INT_NOT_EQUAL(LW(3), 25)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(lect_status_off)
        CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_DISABLE_STICK)
        CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, 0)
        CALL(mainwindow_openenable)

        // Controls are locked again and the Boat Panel's glow events run once more.
        CALL(x_flag_set, 0, 2)
        CALL(lecture_button_set_seq, 0, 3)
        WAIT_FRAMES(1)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_flag_on_off, 0, 2, 0)
        CALL(x_flag_set, 0, 1)
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(0), STRING("S_Ship_Change_01"))
            SET_VALUE(LW(1), STRING("Ship_Change_01"))
            LOOP(0)
                SPAWN_EVENT_GET_ID(PTR(evt_ride_airport), LW(2))
                WAIT_FRAMES(2)
                IS_EVENT_RUNNING(LW(2), LW(3))
                IF_INT_EQUAL(LW(3), 1)
                    BREAK_LOOP
                END_IF
            END_LOOP
            CALL(x_flag_wait, 0, 2)
            IS_EVENT_RUNNING(LW(2), LW(3))
            IF_INT_EQUAL(LW(3), 1)
                DELETE_EVENT(LW(2))
            END_IF
            LOOP(0)
                SPAWN_EVENT_GET_ID(PTR(evt_nonride_airport), LW(2))
                WAIT_FRAMES(2)
                IS_EVENT_RUNNING(LW(2), LW(3))
                IF_INT_EQUAL(LW(3), 1)
                    BREAK_LOOP
                END_IF
            END_LOOP
            IS_EVENT_RUNNING(LW(2), LW(3))
            IF_INT_EQUAL(LW(3), 1)
                DELETE_EVENT(LW(2))
            END_IF
        END_INLINE_EVENT

        // The genie asks whether Mario understands; on "Run that by me again" he says to just press Y and try
        // again, and the loop repeats; otherwise it exits.
        CALL(evt_msg_print_add, 0, STRING("stg1_gon_majin4_15_1"))
        CALL(evt_msg_select, 0, STRING("stg1_gon_majin4_16"))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_continue)
#endif
        CALL(evt_cam_letter_box_disable, 0)
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_mario_set_dir, 90, 300, 0)
#if VERSION <= VERSION_DEMO_US
            CALL(evt_msg_toge, 1, 0, 0, 0)
            CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_17"), 0, STRING("ペラ魔人"))
#else
            CALL(evt_msg_print_add, 0, STRING("stg1_gon_majin4_17"))
#endif
        ELSE
            CALL(x_flag_set, 0, 2)
            WAIT_FRAMES(2)
            BREAK_LOOP
        END_IF
    END_LOOP

    // The platform slides back into place while the genie says goodbye and thanks Mario for letting him do his
    // thing.
    BEGIN_INLINE_EVENT
        CALL(x_flag_set, 0, 1)
        SET_FLOAT_VALUE(LW(1), FLOAT(2.6660156f))
        CALL(dai_moveti_get, LW(0), LW(2), LW(3))
        LOOP(0)
            ADD_FLOAT(LW(0), LW(1))
            IF_INT_GREATER_OR_EQUAL(LW(0), FLOAT(-50.0f))
                SET_FLOAT_VALUE(LW(0), FLOAT(-50.0f))
                CALL(dai_moveti_set, LW(0), LW(2), LW(3))
                CALL(x_flag_set, 0, 2)
                BREAK_LOOP
            ELSE
                CALL(dai_moveti_set, LW(0), LW(2), LW(3))
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
#if VERSION <= VERSION_DEMO_US
    CALL(evt_msg_print, 0, STRING("stg1_gon_majin4_18"), 0, STRING("ペラ魔人"))
#else
    CALL(evt_msg_print_add, 0, STRING("stg1_gon_majin4_18"))
#endif
    CALL(x_flag_wait, 0, 2)

    // The camera returns, the genie plays his P_2 animation and is moved out of sight, and the lecture ends.
    CALL(lect_map_cam_chg, STRING("S_Ship_Change_01"), 4)
    CALL(evt_npc_set_anim, STRING("ペラ魔人"), STRING("P_2"))
    WAIT_MS(200)
    CALL(evt_npc_set_camid, STRING("ペラ魔人"), 4)
    CALL(evt_npc_set_position, STRING("ペラ魔人"), 0, -1000, 0)
    CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_SHIP_PLANE_EVT_ON)
    RETURN
EVT_END()

/** Script helper: sets args[0] to TRUE while the R trigger is held on controller 1. */
static USER_FUNC(lect_push_r_check) {
    EvtWord* args = event->args;

    if (keyGetButton(0) & PAD_TRIGGER_R) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper that returns Mario to normal form:
 * cancels a roll or paper-slit mode on its first call, and blocks until he is no longer rolling or in a slit.
 */
static USER_FUNC(evt_nomal_cange) {
    MarioWork* mario = marioGetPtr();

    if (isFirstCall) {
#if VERSION <= VERSION_DEMO_US
        marioRollCancel();
#else
        marioRollTryCancel();
#endif
        if (mario->flags & MARIO_FLAG_PAPER_MODE) {
            motSlitCancel3();
        }
    }
    if (!marioRollKeyDisable()) {
        return EVT_RETURN_BLOCK;
    }
    return (mario->dispFlags & MARIO_DISP_FLAG_SLIT) ? EVT_RETURN_BLOCK : EVT_RETURN_DONE;
}


/** Script helper: blocks until music channel 1 has started playing. */
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

