// Riverside Station (eki) key-item lecture: when Ultra Boots are picked up, Kinopiko gives the Ultra Boots tutorial (all_juyoitem_lecture_rel).

#include "version.h"
#include "icon_ids.h"
#include "evt/evt_window.h"
#include "eff/eff_fukidashi.h"
#include "eff/eff_itemget.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "mario/mario_motion.h"
#include "pmario_sound.h"
#include "statuswindow.h"
#include "driver/hitdrv.h"
#include "evt/evt_msg.h"
#include "evt/evt_sub.h"
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
#include "driver/itemdrv.h"
#include "driver/animdrv.h"
#include "system.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_img.h"
#include "evt/evt_mario.h"
#include "evt/evt_npc.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_lecture.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "mario/mario_cam.h"
#include <dolphin/pad.h>
#include "evt/evt_lect.h"

static OSTime wwstart;
static s32 x_flag_data[2];
static void lecture_button_disp(CameraId camera, void* param);
static LectButton lbtn[2];
/**
 * Shifts the 3D camera's current position and look-at target by ofs (the camera's preset values are saved,
 * the shifted ones become its new stored target, with an instant move).
 * Used to hop the camera 2016 units up and back while the item-get effect is repositioned.
 */
static inline void effitemget_cam_move(CameraEntry* camera, Vec ofs) {
    camera->presetPos = camera->cameraPos;
    camera->presetAt = camera->cameraAt;
    PSVECAdd(&camera->cameraPos, &ofs, &camera->storedPos);
    PSVECAdd(&camera->cameraAt, &ofs, &camera->storedAt);
    camera->startTimeA = gp->renderTime;
    camera->durationA = 0;
    camera->stateA = 3;
    camera->curveA = 11;
}

static USER_FUNC(lect_map_cam_chg);
static USER_FUNC(lect_mario_ido_sum);
static USER_FUNC(lect_push_a_check);
static USER_FUNC(lect_status_off);
static USER_FUNC(lect_status_on);
static USER_FUNC(lecture_button_flag_on_off);
static USER_FUNC(lecture_button_rtn);
static USER_FUNC(lecture_button_seq_wait);
static USER_FUNC(lecture_button_set_alpha);
static USER_FUNC(lecture_button_set_icon_id);
static USER_FUNC(lecture_button_set_pos);
static USER_FUNC(lecture_button_set_seq);
static USER_FUNC(x_flag_get);
static USER_FUNC(x_flag_set);
static USER_FUNC(bgm_start_wait);
static USER_FUNC(lect_push_b_check);
static USER_FUNC(lect_mobj_set_nosysblend);
static USER_FUNC(juyoitem_goato);
static USER_FUNC(juyoitem_gogo);
static USER_FUNC(juyoitem_end);
static USER_FUNC(juyoitem_get_no);
static USER_FUNC(lect_mobj_hit_nasi);
static USER_FUNC(effitemget_busaiku);
static USER_FUNC(effitemget_open_ptr);
static USER_FUNC(effitemget_open);
static USER_FUNC(effitemgetchange_cam_ptr);
static USER_FUNC(effitemgetchange_cam);
static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings);
static USER_FUNC(lecture_button_get_icon_id);
static USER_FUNC(x_flag_wait);
static USER_FUNC(x_flag_add);
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);

EvtWord boots_lv3_juyoitem_lecture_init[104];
EvtWord boots_lv3_juyoitem_lecture_sub01[29];
EvtWord boots_lv3_juyoitem_lecture_sub02[78];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord boots_lv3_juyoitem_lecture[];
#else
EvtWord boots_lv3_juyoitem_lecture[479];
#endif

EvtWord all_juyoitem_lecture_rel[252];

/**
 * Linear interpolation for the lecture walking helpers:
 * returns (total - elapsed) / total * end + elapsed / total * start,
 * so at elapsed 0 it is end and at elapsed == total it is start.
 */
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start) {
    f32 rem = total - elapsed;

    return (rem * end + elapsed * start) / total;
}

/**
 * Script helper: slides Mario in a straight line over args[0] milliseconds. The first call saves his position
 * in LW(0) to LW(2); each call then places him on the line from LW(3), LW(4), LW(5) (at the start) to that
 * saved position (at the end). Blocks until the time is up.
 */
static USER_FUNC(lect_mario_ido_sum) {
    EvtWord* args = event->args;
    MarioWork* mario = marioGetPtr();
    s32 frames = evtGetValue(event, args[0]);
    s32 elapsed;

    if (isFirstCall) {
        wwstart = event->lifetime;
        evtSetFloat(event, LW(0), mario->position.x);
        evtSetFloat(event, LW(1), mario->position.y);
        evtSetFloat(event, LW(2), mario->position.z);
    }
    elapsed = OSTicksToMilliseconds((u64)OSDiffTick(event->lifetime, wwstart));
    if (elapsed > frames) {
        elapsed = frames;
    }
    mario->position.x = lect_mario_ido_sub(elapsed, frames, evtGetFloat(event, LW(3)), evtGetFloat(event, LW(0)));
    mario->position.y = lect_mario_ido_sub(elapsed, frames, evtGetFloat(event, LW(4)), evtGetFloat(event, LW(1)));
    mario->position.z = lect_mario_ido_sub(elapsed, frames, evtGetFloat(event, LW(5)), evtGetFloat(event, LW(2)));
    if (elapsed >= frames) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Script helper: forces the status window (HP/FP/coins bar) closed for the lecture. */
static USER_FUNC(lect_status_off) {
    statusWinForceClose();
    return EVT_RETURN_DONE;
}

/** Script helper: lets the status window open again after the lecture (statusWinForceCloseClear). */
static USER_FUNC(lect_status_on) {
    statusWinForceCloseClear();
    return EVT_RETURN_DONE;
}

/**
 * Script helper: stores args[1] in the lecture's private flag slot args[0] (two slots,
 * used to signal between the lecture's parallel scripts).
 */
static USER_FUNC(x_flag_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper: adds args[1] to the lecture flag slot args[0]. */
static USER_FUNC(x_flag_add) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] += evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper: reads the lecture flag slot args[0] into the script variable args[1]. */
static USER_FUNC(x_flag_get) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], x_flag_data[index]);
    return EVT_RETURN_DONE;
}

/** Script helper: blocks until the lecture flag slot args[0] equals args[1]. */
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
 * 2D draw callback that draws the two lecture button icons (when not hidden) at their positions with their
 * current alpha; the stick direction icons (ICON_STICK_*) are drawn 15 units lower.
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
 * Script helper that runs for the whole lecture as a sibling event:
 * sets up the two button icons on the first call,
 * then every frame advances each button's sequence (fade in while seq 2, fade out while seq 3, hidden at seq 4)
 * and queues lecture_button_disp. Finishes once both buttons are hidden.
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
    dispEntry(CAMERA_2D, 0.0f, DISP_MODE_ALPHA_TEST, lecture_button_disp, NULL);
    return EVT_RETURN_BLOCK;
}

/** Script helper: sets the icon of lecture button args[0] to the icon id args[1]. */
static USER_FUNC(lecture_button_set_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->iconId = value;
    return EVT_RETURN_DONE;
}

/** Script helper: sets the opacity (0 to 255) of lecture button args[0] to args[1]. */
static USER_FUNC(lecture_button_set_alpha) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->alpha = value;
    return EVT_RETURN_DONE;
}

/** Script helper: sets (args[2] nonzero) or clears the flag bits args[1] on lecture button args[0]. */
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

/**
 * Script helper: puts lecture button args[0] into sequence step args[1] (2 fades it in, 3 fades it out,
 * 1 is idle, 4 hides it).
 */
static USER_FUNC(lecture_button_set_seq) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->seq = value;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: reads the sequence step of lecture button args[0] into args[1] (despite the name it does not
 * return the icon id).
 */
static USER_FUNC(lecture_button_get_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    LectButton* btn = &lbtn[index];

    evtSetValue(event, args[1], btn->seq);
    return EVT_RETURN_DONE;
}

/** Script helper: blocks until lecture button args[0] has reached sequence step args[1]. */
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

/** Script helper: moves lecture button args[0] to the screen position (args[1], args[2], args[3]). */
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
 * Sets the camera id of a map object and, recursively,
 * of its children (and of the siblings that follow a child) so the whole model is drawn by the given camera.
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

/**
 * Script helper: moves the map object named args[0] (with its child objects) to the camera id args[1],
 * so the lecture can draw it on the lecture camera.
 */
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

/** Script helper: changes the camera that the named item-get effect (args[0]) draws with to the camera id args[1]. */
static USER_FUNC(effitemgetchange_cam) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    ((ItemGetWork*)effNameToPtr(name)->userdata)->camera = camId;
    return EVT_RETURN_DONE;
}

/** Script helper: same as effitemgetchange_cam but takes the effect entry pointer in args[0]. */
static USER_FUNC(effitemgetchange_cam_ptr) {
    EvtWord* args = event->args;
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, args[0]);

    ((ItemGetWork*)eff->userdata)->camera = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper: starts the named item-get effect (args[0]) by moving it to its opening state. */
static USER_FUNC(effitemget_open) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/** Script helper: same as effitemget_open but takes the effect entry pointer in args[0]. */
static USER_FUNC(effitemget_open_ptr) {
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, event->args[0]);

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] nonzero moves the 3D camera 2016 units up,
 * zero moves it back down (effitemget_cam_move).
 * The important-item lecture lifts the camera and Mario 2016 units,
 * captures the screen into an image for the paper-seal transition one frame later, and moves both back.
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
 * Sets up the stage for the Ultra Boots lecture:
 * moves Mario's real position aside, places Kinopiko (the Toad teacher) next to him,
 * sets a close lecture camera (Mario held in place for one second while it moves; the slide's start and end are
 * his own position), then puts Mario at his lecture spot (0, -600, 0), ending with Mario facing right.
 */
EVT_BEGIN(boots_lv3_juyoitem_lecture_init)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_mario_set_pos, 0, -600, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 150)
    CALL(evt_npc_flag_onoff, 1, STRING("キノピコ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("キノピコ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("キノピコ"), 270)
    CALL(evt_mario_set_pos, LW(3), LW(4), LW(5))
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(26.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, EASING_LINEAR)
    CALL(evt_mario_cont_onoff, 0)
    CALL(lect_mario_ido_sum, 1000)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_pos, 0, -600, 0)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(26.0f), FLOAT(30.0f), FLOAT(-100.0f), 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pos, 0, -600, 0)
    CALL(evt_mario_set_dir, 90, 300, 0)
    RETURN
EVT_END()

/**
 * Lecture helper: shows the two button icons (A solid, the stick icon dimmed)
 * and flips the A icon between its pressed and released pictures every 300 ms.
 */
EVT_BEGIN(boots_lv3_juyoitem_lecture_sub01)
    CALL(lecture_button_set_alpha, 0, 255)
    CALL(lecture_button_set_alpha, 1, 128)
    CALL(lecture_button_set_icon_id, 1, ICON_STICK_NEUTRAL)
    LOOP(0)
        CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_A_PRESSED)
        WAIT_MS(300)
        CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_A)
        WAIT_MS(300)
    END_LOOP
    RETURN
EVT_END()

/**
 * Lecture helper: while Mario is crouching on the spring (A held),
 * shows the A icon pressed and dimmed and cycles the stick icon around its directions every 15 ms to show
 * where to push; runs until the main lecture script deletes it.
 */
EVT_BEGIN(boots_lv3_juyoitem_lecture_sub02)
    CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_A_PRESSED)
    LOOP(0)
        CALL(evt_mario_get_motion, LW(3))
        IF_INT_EQUAL(LW(3), MARIO_MOTION_STAY)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(lecture_button_set_alpha, 0, 128)
    CALL(lecture_button_set_alpha, 1, 255)
    LOOP(0)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_LEFT)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_UP_LEFT)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_UP)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_UP_RIGHT)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_RIGHT)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_DOWN_RIGHT)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_DOWN)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_DOWN_LEFT)
        WAIT_MS(15)
    END_LOOP
    RETURN
EVT_END()

/**
 * The Ultra Boots lecture proper.
 * Kinopiko explains the spring jump (lecture_u_boots_04 to 08):
 * the player holds A and rotates the stick to crouch and releases A to launch, while the button icons show the
 * controls;
 * the script lets Mario try it until he reaches the target height,
 * after which Kinopiko offers to repeat the lesson (lecture_u_boots_10 and 11)
 * and finally says goodbye (lecture_u_boots_13) and leaves.
 */
EVT_BEGIN(boots_lv3_juyoitem_lecture)
    // The camera switches to the lecture view and Toadette walks up to Mario.
    BEGIN_SIBLING_EVENT
        CALL(lecture_button_rtn)
    END_SIBLING_EVENT
    CALL(lect_map_cam_chg, STRING("S_rec"), 7)
    CALL(evt_npc_set_camid, STRING("キノピコ"), 7)
    WAIT_FRAMES(1)
    CALL(lecture_button_set_pos, 0, FLOAT(-200.0f), FLOAT(-25.0f), FLOAT(0.0f))
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 85)
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)

    // Toadette congratulates Mario on the Ultra Boots and explains the Spring Jump: hold A, rotate the stick,
    // release A.
    CALL(evt_msg_print, 0, STRING("lecture_u_boots_04"), 0, STRING("キノピコ"))
    CALL(evt_msg_print_add, 0, STRING("lecture_u_boots_05"))

    // Practice loop: the prompt tells Mario to try it, and Toadette encourages him now and then until he
    // reaches the target height.
    LOOP(0)
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("lecture_u_boots_06"), 0, 0)
        CALL(x_flag_set, 1, 1)
        BEGIN_INLINE_EVENT
            CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_3"), STRING("T_3"))
            CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_3"))
            LOOP(0)
                CALL(evt_sub_random, 2, LW(1))
                ADD(LW(1), 4)
                MULTIPLY(LW(1), 2)
                LOOP(LW(1))
                    WAIT_MS(500)
                    CALL(x_flag_get, 1, LW(0))
                    IF_INT_EQUAL(LW(0), 2)
                        BREAK_LOOP
                    END_IF
                END_LOOP
                CALL(x_flag_get, 1, LW(0))
                IF_INT_EQUAL(LW(0), 2)
                    BREAK_LOOP
                END_IF
                CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("T_3"))
                CALL(evt_msg_print, 0, STRING("lecture_u_boots_07"), 0, STRING("キノピコ"))
                CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_3"))
            END_LOOP
            CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_1"), STRING("T_1"))
            CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_1"))
        END_INLINE_EVENT

        // The camera moves in and the on-screen button icons show the controls.
        CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(85.0f), FLOAT(520.0f), FLOAT(0.0f), FLOAT(40.0f), FLOAT(-100.0f), 1000, EASING_SINE_IN_OUT)
        WAIT_MS(1000)
        CALL(lecture_button_set_pos, 0, FLOAT(-200.0f), FLOAT(-25.0f), FLOAT(0.0f))
        CALL(lecture_button_set_pos, 1, FLOAT(-140.0f), FLOAT(-25.0f), FLOAT(0.0f))
        CALL(lecture_button_flag_on_off, 0, 2, 1)
        CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_A)
        CALL(lecture_button_set_seq, 0, 2)
        CALL(lecture_button_flag_on_off, 1, 2, 1)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_NEUTRAL)
        CALL(lecture_button_set_seq, 1, 2)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_seq_wait, 1, 1)

        // Mario gets limited controls; the button icons change as he crouches and launches.
        SPAWN_EVENT_GET_ID(PTR(boots_lv3_juyoitem_lecture_sub01), LW(11))
        SET_VALUE(LW(7), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SET_VALUE(LW(7), 0)
        CALL(mainwindow_opendisable)
        CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, PAD_BUTTON_A)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_key_onoff, 1)
        LOOP(0)
            CALL(lect_status_off)
            CALL(evt_mario_get_pos, 0, LW(5), LW(1), LW(6))
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            IF_INT_GREATER_OR_EQUAL(LW(1), FLOAT(-470.0f))
                CALL(x_flag_set, 1, 2)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(7), 0)
                CALL(lect_push_a_check, LW(7))
                IF_INT_NOT_EQUAL(LW(7), 0)
                    DELETE_EVENT(LW(11))
                    SPAWN_EVENT_GET_ID(PTR(boots_lv3_juyoitem_lecture_sub02), LW(11))
                END_IF
            ELSE
                IF_INT_EQUAL(LW(7), 1)
                    CALL(lect_push_a_check, LW(7))
                    IF_INT_EQUAL(LW(7), 0)
                        DELETE_EVENT(LW(11))
                        SPAWN_EVENT_GET_ID(PTR(boots_lv3_juyoitem_lecture_sub01), LW(11))
                    END_IF
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(lect_status_on)
        CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, 0)
        CALL(mainwindow_openenable)

        // Mario reached the height: controls are restored and Toadette explains how the Spring Jump lets him
        // grab pipes overhead.
        DELETE_EVENT(LW(11))
        CALL(lecture_button_set_seq, 0, 3)
        CALL(lecture_button_set_seq, 1, 3)
        CALL(evt_msg_continue)
        CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(85.0f), FLOAT(520.0f), FLOAT(0.0f), FLOAT(40.0f), FLOAT(-100.0f), 1000, EASING_SINE_IN_OUT)
        WAIT_MS(1000)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_seq_wait, 1, 1)
        CALL(evt_msg_print, 0, STRING("lecture_u_boots_08"), 0, STRING("キノピコ"))
#if VERSION >= VERSION_JP
        CALL(evt_msg_continue)
        WAIT_MS(500)
#endif
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(1), 1)
        ADD(LW(2), 1)
        CALL(evt_mario_normalize)
        CALL(evt_mario_wait_landon)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_mario_set_pos, LW(0), LW(1), 0)
        CALL(evt_mario_set_dir, 90, 300, 0)

        // Toadette steps next to Mario and asks if he wants to try again.
        CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(26.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, EASING_SINE_IN_OUT)
        WAIT_MS(1000)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(0), 65)
        CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)
        CALL(evt_npc_set_ry, STRING("キノピコ"), 270)
        CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_2"), STRING("T_2"))
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print_add, 0, STRING("lecture_u_boots_10"))
#else
        CALL(evt_msg_print, 0, STRING("lecture_u_boots_10"), 0, STRING("キノピコ"))

#endif

// If he says "One more time" the loop repeats; otherwise it ends.
        CALL(evt_msg_select, 0, STRING("lecture_u_boots_11"))
        CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_1"), STRING("T_1"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("lecture_u_boots_12"))
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP

    // Toadette says goodbye, telling him he has mastered the Spring Jump, and walks off.
    CALL(evt_msg_print_add, 0, STRING("lecture_u_boots_13"))
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    ADD(LW(0), 100)
    CALL(evt_npc_set_ry, STRING("キノピコ"), 90)
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_position, STRING("キノピコ"), 0, -1000, 0)
    RETURN
EVT_END()

/**
 * Entry point of the key-item lecture, run when Mario first picks up a key item such as the Ultra Boots (the
 * Ultra Boots case calls it through lect_juyoitem_rel_set):
 * stops the map, plays the lecture music, shows the item on its own camera,
 * then runs the lecture for the item (only item 8, the Ultra Boots, has one here),
 * blinks Mario back in with the lecture sound, restores the camera and music and lets the map run again.
 */
EVT_BEGIN(all_juyoitem_lecture_rel)
    CALL(evt_mario_normalize)
    CALL(juyoitem_get_no, LW(10))
    CALL(evt_mario_set_camid, 7)
    CALL(effitemget_open_ptr, LW(9))
    CALL(juyoitem_gogo)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PARTY_LECTURE1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_stop_for_event)
    CALL(evt_npc_start_for_one_event, STRING("キノピコ"))
    CALL(evt_party_stop, 0)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_LECTURE1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(bgm_start_wait)
    CALL(lect_cam_save)
    CALL(effitemget_busaiku, 1)
    WAIT_FRAMES(1)
    CALL(evt_mario_cont_onoff, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(1))
    ADD_FLOAT(LW(3), FLOAT(2016.0f))
    CALL(evt_mario_set_pos, LW(0), LW(3), LW(2))
    CALL(effitemgetchange_cam_ptr, LW(9), 4)
    CALL(evt_paper_entry, STRING("OFF_d_seal_upper_right"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_seal_upper_right"))
    CALL(evt_img_free_capture, STRING("img"), 0)
    CALL(evt_img_alloc_capture, STRING("img"), 0, 0, 1, 0, 0, 608, 480)
    CALL(evt_img_clear_virtual_point, STRING("img"))
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("Z_1"))
    CALL(evt_img_onoff, STRING("img"), 1)
    WAIT_FRAMES(1)
    CALL(effitemget_busaiku, 0)
    CALL(evt_eff_softdelete_ptr, LW(9))
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    CALL(evt_mario_cont_onoff, 1)
    CALL(lect_test_systemlevel)
    CALL(evt_npc_sound_data_set, STRING("キノピコ"), STRING("SFX_FLD_NPC_KINOPIO_MOVE1L"), STRING("SFX_FLD_NPC_KINOPIO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    SWITCH(LW(10))
        CASE_EQUAL(8)
        CALL_CHILD(boots_lv3_juyoitem_lecture_init)
    END_SWITCH
    SWITCH(LW(10))
        CASE_EQUAL(8)
        CALL_CHILD(boots_lv3_juyoitem_lecture)
    END_SWITCH
    LOOP(6)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LECTUREN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mario_init_camid)
    CALL(lect_cam_load, 0)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    CALL(evt_cam_road_reset)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PARTY_LECTURE1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(juyoitem_goato)
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_seal_upper_right"))
    CALL(evt_npc_start_for_event)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

/**
 * Script helper: switches off the collision of the map object named args[0] ("me" for the running one): each
 * of its (up to two) named hit objects gets HIT_FLAG_OFF through hitGrpFlagOn (nasi = none).
 * Unused in this file.
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
        // hitObj[i].name is an array, so the NULL test is always true; the original code has it and MWCC still emits the compare.
        if (mobj->hitObj[i].name != NULL && strcmp(mobj->hitObj[i].name, "") != 0) {
            hitGrpFlagOn(mobj->hitObj[i].name, 1);
        }
    }
    return EVT_RETURN_DONE;
}

/** Script helper: writes the item id of the important item being shown (stored in event->lwData[8]) into args[0]. */
static USER_FUNC(juyoitem_get_no) {
    evtSetValue(event, event->args[0], ((ItemEntry*)event->lwData[8])->itemId);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: ends the important-item display:
 * removes the item-get effect (lwData[9]), puts Mario back on camera 4 and clears the item's effect flag.
 */
static USER_FUNC(juyoitem_end) {
    ItemEntry* item = (ItemEntry*)event->lwData[8];

    effSoftDelete((EffectEntry*)event->lwData[9]);
    marioSetCamId(CAMERA_3D);
    iconFlagOff(item->name, ICON_FLAG_CAM_EFFECT_B);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: starts the lecture for an important item:
 * clears the item icon's effect flag, parks the item below the map,
 * puts Mario in his standing motion and sets the system level to 0.
 */
static USER_FUNC(juyoitem_gogo) {
    ItemEntry* item = (ItemEntry*)event->lwData[8];

    iconFlagOff(item->name, ICON_FLAG_CAM_EFFECT_B);
    itemSetPosition(item, 0.0f, -1000.0f, 0.0f);
    marioChgStayMotion();
    marioStSystemLevel(0);
    return EVT_RETURN_DONE;
}

/** Script helper: hides the important item's icon after the lecture. */
static USER_FUNC(juyoitem_goato) {
    iconFlagOn(((ItemEntry*)event->lwData[8])->name, ICON_FLAG_HIDDEN);
    return EVT_RETURN_DONE;
}

static USER_FUNC(lect_mobj_set_nosysblend) {
    const char* name = (const char*)evtGetValue(event, event->args[0]);
    MapObjectEntry* mobj;

    if (strcmp(name, "me") != 0) {
        mobj = mobjNameToPtr(name);
    } else {
        mobj = event->thisMapObj;
    }
    animPoseSetMaterialFlagOn(mobj->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    return EVT_RETURN_DONE;
}

/** Script helper: writes TRUE into args[0] while controller 0 holds the A button, otherwise FALSE. */
static USER_FUNC(lect_push_a_check) {
    EvtWord* args = event->args;

    if (keyGetButton(0) & PAD_BUTTON_A) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}

/** Script helper: writes TRUE into args[0] while controller 0 holds the B button, otherwise FALSE. */
static USER_FUNC(lect_push_b_check) {
    EvtWord* args = event->args;

    if (keyGetButton(0) & PAD_BUTTON_B) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}


/** Script helper: blocks until the lecture music (BGM channel 1) has actually started playing. */
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

