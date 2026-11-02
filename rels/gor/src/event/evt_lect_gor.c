// evt_lect_gor: lecture scripts for partners (Goombella, Ms. Mowz: lecture_friends messages) and the Hammer level 3
// key item, with their helpers: button prompts, camera/Mario movement, flags.

#include "version.h"
#include "icon_ids.h"
#include "evt/evt_window.h"
#include "eff/eff_mario_balloon.h"
#include "eff/eff_fukidashi.h"
#include "eff/eff_itemget.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "mario/mario_motion.h"
#include "pmario_sound.h"
#include "statuswindow.h"
#include "driver/hitdrv.h"
#include "evt/evt_img.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_lecture.h"
#include "evt/evt_map.h"
#include "evt/evt_pouch.h"

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
#include "party.h"
#include "driver/npcdrv.h"
#include "evt/evt_npc.h"
#include "driver/itemdrv.h"
#include "driver/animdrv.h"
#include "system.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_party.h"
#include "mario/mario_cam.h"
#include <dolphin/pad.h>
#include "evt/evt_lect.h"

static OSTime wwstart;
static s32 x_flag_data[2];
static void lecture_button_disp(CameraId camera, void* param);
static LectButton lbtn[2];
/**
 * Moves a camera's stored target by the offset ofs:
 * saves the current position and look-at as the preset,
 * sets the stored ones to the current plus ofs and starts a curve-11 transition with zero duration.
 * Used to jump the 3D camera 2016 units up or down for the lecture stage.
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

static EffectEntry* eff;

static USER_FUNC(efffukidashichange_cam);
static USER_FUNC(effitemget_busaiku);
static USER_FUNC(effitemget_open);
static USER_FUNC(effitemgetchange_cam);
static USER_FUNC(lect_itemget_kesan);
static USER_FUNC(lect_mario_ido_sum);
static USER_FUNC(lect_mobj_hit_nasi);
static USER_FUNC(lect_npc_balloon_on_off);
static USER_FUNC(lect_party_ido_sum);
static USER_FUNC(lect_push_b_check);
static USER_FUNC(lect_status_off);
static USER_FUNC(lect_status_on);
static USER_FUNC(lecture_button_flag_on_off);
static USER_FUNC(lecture_button_rtn);
static USER_FUNC(lecture_button_seq_wait);
static USER_FUNC(lecture_button_set_alpha);
static USER_FUNC(lecture_button_set_icon_id);
static USER_FUNC(lecture_button_set_pos);
static USER_FUNC(lecture_button_set_seq);
static USER_FUNC(set_dir);
static USER_FUNC(x_flag_get);
static USER_FUNC(x_flag_set);
static USER_FUNC(bgm_start_wait);
static USER_FUNC(lect_push_a_check);
static USER_FUNC(lect_mobj_set_nosysblend);
static USER_FUNC(juyoitem_goato);
static USER_FUNC(juyoitem_gogo);
static USER_FUNC(juyoitem_end);
static USER_FUNC(juyoitem_get_no);
static f32 lect_party_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);
static USER_FUNC(effitemget_open_ptr);
static USER_FUNC(effitemgetchange_cam_ptr);
static USER_FUNC(lect_map_cam_chg);
static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings);
static USER_FUNC(lecture_button_get_icon_id);
static USER_FUNC(x_flag_wait);
static USER_FUNC(x_flag_add);
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);

#if VERSION <= VERSION_DEMO_US
extern EvtWord christine_party_lecture_init[];
extern EvtWord christine_party_lecture[];
extern EvtWord chuchulina_party_lecture_init[];
extern EvtWord chuchulina_party_lecture[];
#else
EvtWord christine_party_lecture_init[46];
EvtWord christine_party_lecture[125];
EvtWord chuchulina_party_lecture_init[51];
EvtWord chuchulina_party_lecture[236];
#endif
static EvtWord all_party_lecture_otosub[11];
EvtWord all_party_lecture[482];
#if VERSION <= VERSION_DEMO_US
extern EvtWord hammer_lv3_juyoitem_lecture_init[];
#else
EvtWord hammer_lv3_juyoitem_lecture_init[107];
#endif
EvtWord hammer_lv3_juyoitem_lecture_sub01[29];
EvtWord hammer_lv3_juyoitem_lecture_sub02[65];
#if VERSION <= VERSION_DEMO_US
extern EvtWord hammer_lv3_juyoitem_lecture[];
#else
EvtWord hammer_lv3_juyoitem_lecture[718];
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
 * Script helper that glides Mario to a target over args[0] milliseconds:
 * on the first call it stores his position in LW(0..2),
 * then each frame it interpolates towards the target in LW(3..5). Finishes when the time is up.
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

/** Script helper that force-closes the status window (hearts, FP, coins) during a lecture. */
static USER_FUNC(lect_status_off) {
    statusWinForceClose();
    return EVT_RETURN_DONE;
}

/** Script helper that lets the status window open again after lect_status_off. */
static USER_FUNC(lect_status_on) {
    statusWinForceCloseClear();
    return EVT_RETURN_DONE;
}

/**
 * Script helper that sets lecture flag number args[0] (0 or 1) to args[1].
 * The lecture scripts use these two words to signal between parallel events.
 */
static USER_FUNC(x_flag_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper that adds args[1] to lecture flag number args[0]. */
static USER_FUNC(x_flag_add) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] += evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper that reads lecture flag number args[0] into args[1]. */
static USER_FUNC(x_flag_get) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], x_flag_data[index]);
    return EVT_RETURN_DONE;
}

/** Script helper that blocks until lecture flag number args[0] equals args[1]. */
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
 * Draw callback of the lecture button prompts: draws the two on-screen button icons (those not hidden) with
 * their alpha; the icons for the stick directions are drawn 15 units lower.
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
            iconDispGxAlpha(&pos, ICON_FLAG_NO_MAP_BLEND | ICON_FLAG_2D, btn->iconId, btn->alpha, 1.0f);
        }
    }
}

/**
 * Script helper that runs the two lecture button prompts:
 * resets them on the first call, then each frame fades an icon in (sequence 2)
 * or out (sequence 3) by 7 alpha per frame and hides it at sequence 4; finishes when both are hidden.
 * Queues the draw every frame.
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

/** Script helper: sets the icon id of button prompt args[0] to args[1]. */
static USER_FUNC(lecture_button_set_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->iconId = value;
    return EVT_RETURN_DONE;
}

/** Script helper: sets the alpha of button prompt args[0] to args[1]. */
static USER_FUNC(lecture_button_set_alpha) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->alpha = value;
    return EVT_RETURN_DONE;
}

/** Script helper: sets (args[2] nonzero) or clears the flag bits args[1] on button prompt args[0]. */
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

/** Script helper: sets the sequence step of button prompt args[0] to args[1]. */
static USER_FUNC(lecture_button_set_seq) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->seq = value;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: stores the sequence step (not the icon id, despite the name) of button prompt args[0] into
 * args[1].
 */
static USER_FUNC(lecture_button_get_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    LectButton* btn = &lbtn[index];

    evtSetValue(event, args[1], btn->seq);
    return EVT_RETURN_DONE;
}

/** Script helper that blocks until button prompt args[0] has reached sequence step args[1]. */
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

/** Script helper: moves button prompt args[0] to the screen position (args[1], args[2], args[3]). */
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

/** Sets the camera id of a map object and, recursively, of its children and following siblings. */
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
 * Script helper that switches the map object named args[0] (and its subtree) to camera id args[1],
 * so it is drawn by the lecture camera.
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

/** Script helper that sets the camera of the named item-get effect args[0] to camera id args[1]. */
static USER_FUNC(effitemgetchange_cam) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    ((ItemGetWork*)effNameToPtr(name)->userdata)->camera = camId;
    return EVT_RETURN_DONE;
}

/** Script helper like effitemgetchange_cam, but args[0] is the effect pointer. */
static USER_FUNC(effitemgetchange_cam_ptr) {
    EvtWord* args = event->args;
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, args[0]);

    ((ItemGetWork*)eff->userdata)->camera = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper that opens the named item-get effect (args[0]) by setting its state to 1. */
static USER_FUNC(effitemget_open) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/** Script helper like effitemget_open, but args[0] is the effect pointer. */
static USER_FUNC(effitemget_open_ptr) {
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, event->args[0]);

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/**
 * Script helper that shifts the 3D camera 2016 units up (args[0] nonzero) or down.
 * The lecture openings raise it (and the partner, by the same 2016) for one frame
 * while the screen is captured into
 * the "img" image for the OFF_d_seal_upper_right paper transition, then lower it again.
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
 * Sets up Goombella's (クリスチーヌ) partner lecture:
 * Mario faces left, the camera moves in front of him and his partner glides next to him.
 */
EVT_BEGIN(christine_party_lecture_init)
#if VERSION > VERSION_DEMO_US
    CALL(evt_mapdisp_onoff, 0)
#endif
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 40)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(85.0f), FLOAT(500.0f), FLOAT(-20.0f), FLOAT(35.0f), FLOAT(-100.0f), 1000, EASING_LINEAR)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(lect_party_ido_sum, 0, 1000)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_set_dir, 0, 90, 300)
    RETURN
EVT_END()

/**
 * Goombella's partner lecture (lecture_friends_00 to 03):
 * after the introduction a Goomba (クリボー３) walks in from the right,
 * a speech balloon appears over it as an example, then it walks back and Goombella does her pose.
 */
EVT_BEGIN(christine_party_lecture)
    CALL(evt_msg_print, 0, STRING("lecture_friends_00_0_1"), 0, 0)
    CALL(evt_msg_print, 0, STRING("lecture_friends_00_1"), 0, 0)
    CALL(evt_msg_print, 0, STRING("lecture_friends_01"), 0, STRING("party"))
    CALL(evt_mario_set_dir, 90, 150, 0)
    CALL(evt_npc_start_for_one_event, STRING("クリボー３"))
    CALL(evt_npc_get_position, STRING("クリボー３"), LW(10), LW(11), LW(12))
    CALL(evt_npc_set_position, STRING("クリボー３"), 150, 0, 500)
    CALL(evt_npc_set_camid, STRING("クリボー３"), 7)
    CALL(evt_npc_set_ry, STRING("クリボー３"), 270)
    CALL(evt_npc_move_position, STRING("クリボー３"), 5, 500, 0, FLOAT(80.0f), 0)
    CALL(lect_npc_balloon_on_off, STRING("クリボー３"), 1)
    WAIT_MS(1000)
    CALL(lect_npc_balloon_on_off, STRING("クリボー３"), 0)
    CALL(evt_msg_print, 0, STRING("lecture_friends_02"), 0, STRING("party"))
    CALL(evt_mario_set_dir, 270, 150, 0)
    CALL(evt_npc_set_ry, STRING("クリボー３"), 90)
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("クリボー３"), 150, 500, 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_camid, STRING("クリボー３"), 4)
    CALL(evt_npc_set_position, STRING("クリボー３"), LW(10), LW(11), LW(12))
    CALL(evt_npc_stop_for_one_event, STRING("クリボー３"))
    CALL(evt_msg_print, 0, STRING("lecture_friends_03"), 0, 0)
    WAIT_MS(500)
    CALL(evt_party_set_pose, 0, STRING("PKR_O_1"))
    WAIT_MS(1000)
    CALL(evt_party_set_pose, 0, STRING("PKR_S_1"))
#if VERSION > VERSION_DEMO_US
    CALL(evt_mapdisp_onoff, 1)
#endif
    RETURN
EVT_END()

/**
 * Sets up Ms. Mowz's (チュチュリーナ) partner lecture:
 * Mario is placed at the start of the stage, the camera moves in and the partner glides next to him.
 */
EVT_BEGIN(chuchulina_party_lecture_init)
#if VERSION > VERSION_DEMO_US
    CALL(evt_mapdisp_onoff, 0)
#endif
    CALL(evt_mario_set_pos, 0, 0, -600)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 40)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(100.0f), FLOAT(569.0f), FLOAT(0.0f), FLOAT(37.399414f), FLOAT(-101.799805f), 1000, EASING_LINEAR)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(lect_party_ido_sum, 0, 1000)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_set_dir, 0, 90, 300)
    RETURN
EVT_END()

/**
 * Ms. Mowz's partner lecture (lecture_friends_17 to 20):
 * she sniffs, a treasure box falls in beside Mario,
 * Mowz sniffs it out with a speech balloon and the box flashes and disappears.
 */
EVT_BEGIN(chuchulina_party_lecture)
    CALL(evt_msg_print, 0, STRING("lecture_friends_17_0_1"), 0, 0)
    CALL(evt_msg_print, 0, STRING("lecture_friends_17_1"), 0, 0)
    CALL(evt_party_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PARTY_CHURINA_SMELL1"), LW(2), LW(3), LW(4), 0)
    CALL(evt_party_set_pose, 0, STRING("PCH_A2_1"))
    WAIT_MS(1000)
    CALL(evt_party_set_pose, 0, STRING("PCH_S_1"))
    CALL(evt_msg_print, 0, STRING("lecture_friends_18"), 0, STRING("party"))
    SET_VALUE(LW(1), STRING("TESTMOBJ"))
    CALL(evt_mobj_entry, LW(1), STRING("MOBJ_TreasureBox"))
    CALL(lect_mobj_hit_nasi, LW(1))
    CALL(evt_mobj_set_camid, LW(1), 7)
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    ADD(LW(2), 200)
    CALL(evt_mobj_flag_onoff, 0, LW(1), MOBJ_FLAG_GRAVITY)
    CALL(evt_mobj_set_position, LW(1), LW(2), LW(3), LW(4))
    CALL(x_flag_set, 0, 1)
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(100.0f), FLOAT(569.0f), FLOAT(0.0f), FLOAT(37.399414f), FLOAT(-101.799805f), 0, EASING_SINE_IN_OUT)
            CALL(x_flag_get, 0, LW(0))
            IF_INT_EQUAL(LW(0), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SUBTRACT(LW(2), 30)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_mov_pos2, LW(2), LW(4), FLOAT(80.0f))
        CALL(x_flag_set, 0, 2)
    END_INLINE_EVENT
    SUBTRACT(LW(2), 40)
    CALL(evt_party_move_pos2, 0, LW(2), LW(4), FLOAT(80.0f))
    WAIT_MS(100)
    CALL(evt_party_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PARTY_CHURINA_SMELL1"), LW(2), LW(3), LW(4), 0)
    CALL(evt_party_set_pose, 0, STRING("PCH_A2_1"))
    WAIT_MS(1000)
    CALL(evt_party_get_pos, 0, LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon_3d, STRING("SFX_PARTY_CHURINA_SMELL_JUMP1"), LW(2), LW(3), LW(4), 0)
    CALL(evt_party_set_pose, 0, STRING("PCH_Y_1"))
    WAIT_MS(700)
    CALL(evt_party_set_pose, 0, STRING("PCH_S_1"))
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING("fuki"), 0, 6, 0, 0, 0, 0, 0, 0, 60)
    CALL(efffukidashichange_cam, STRING("fuki"), 7)
    CALL(evt_msg_print, 0, STRING("lecture_friends_19"), 0, STRING("party"))
    LOOP(6)
        CALL(evt_mobj_flag_onoff, 1, LW(1), MOBJ_FLAG_NO_DISP)
        WAIT_MS(100)
        CALL(evt_mobj_flag_onoff, 0, LW(1), MOBJ_FLAG_NO_DISP)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mobj_delete, LW(1))
    CALL(evt_msg_print, 0, STRING("lecture_friends_20"), 0, 0)
    CALL(evt_party_set_pose, 0, STRING("PCH_O_2"))
    WAIT_MS(1000)
    CALL(evt_party_set_pose, 0, STRING("PCH_S_1"))
#if VERSION > VERSION_DEMO_US
    CALL(evt_mapdisp_onoff, 1)
#endif
    RETURN
EVT_END()

/**
 * Sub-event of the partner lecture: holds the current music
 * and plays the get-partner jingle on channel 1 for two seconds.
 */
static EVT_BEGIN(all_party_lecture_otosub)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_GET_PARTY1"))
    WAIT_MS(2000)
    RETURN
EVT_END()

EVT_BEGIN(all_party_lecture)
    // The new partner joins: unless it is Goombella (already beside Mario), the placeholder NPC is swapped for
    // the real party member at the same spot.
    CALL(evt_mario_normalize)
    IF_INT_NOT_EQUAL(LW(9), 1)
        IF_INT_NOT_EQUAL(LW(9), 5)
            CALL(evt_mario_goodbye_party, 0)
        ELSE
            CALL(evt_mario_kill_party, 0)
        END_IF
        WAIT_MS(500)
        CALL(evt_npc_get_position, LW(8), LW(0), LW(1), LW(2))
        CALL(evt_npc_get_dir, LW(8), LW(3))
        CALL(evt_npc_set_position, LW(8), 0, -1000, 0)
        CALL(evt_pouch_party_join, LW(9))
        CALL(evt_mario_set_party_pos, 0, LW(9), LW(0), LW(1), LW(2))
        CALL(set_dir, LW(3))
    ELSE
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    END_IF

    // The party-join fanfare starts and the partner is shown in a bright item-get style effect.
    CALL(evt_party_set_camid, 0, 7)
    SPAWN_EVENT_GET_ID(PTR(all_party_lecture_otosub), LW(11))
    CALL(lect_itemget_kesan, 0, FLOAT(600.0f), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("sub_bg"), STRING("itemget"), 1, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(effitemgetchange_cam, STRING("sub_bg"), 5)

    // The partner explains itself in a text box that depends on who it is.
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    SWITCH(LW(9))
        CASE_EQUAL(1)
        CALL(evt_msg_print, 0, STRING("lecture_friends_00"), 0, 0)
        CASE_EQUAL(2)
        CALL(evt_msg_print, 0, STRING("lecture_friends_04"), 0, 0)
        CASE_EQUAL(5)
        CALL(evt_msg_print, 0, STRING("lecture_friends_07"), 0, 0)
        CASE_EQUAL(4)
        CALL(evt_pouch_get_yoshiname, LW(0))
        CALL(evt_msg_print_insert, 0, STRING("lecture_friends_09"), 0, 0, LW(0))
        CASE_EQUAL(6)
        CALL(evt_msg_print, 0, STRING("lecture_friends_12"), 0, 0)
        CASE_EQUAL(3)
        CALL(evt_msg_print, 0, STRING("lecture_friends_15"), 0, 0)
        CALL(evt_eff_softdelete, STRING("sunlight"))
        CASE_EQUAL(7)
        CALL(evt_msg_print, 0, STRING("lecture_friends_17"), 0, 0)
    END_SWITCH

    // The fanfare is cut off if still playing and the field music resumes.
    IS_EVENT_RUNNING(LW(11), LW(12))
    IF_INT_EQUAL(LW(12), 1)
        DELETE_EVENT(LW(11))
        CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
        CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    END_IF
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_mario_set_camid, 7)
    CALL(effitemget_open, STRING("sub_bg"))
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))

    // A lecture jingle plays and the scene is set up for the demonstration: the field music and ambience are
    // held.
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PARTY_LECTURE1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_stop_for_event)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_EVT_LECTURE1"))
    CALL(evt_snd_envoff, PSND_PAUSE)
    CALL(lect_cam_save)
    CALL(effitemget_busaiku, 1)
    WAIT_FRAMES(1)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    SET_VALUE(LW(3), LW(1))
    ADD(LW(3), 2016)
    CALL(evt_party_set_pos, 0, LW(0), LW(3), LW(2))
    CALL(effitemgetchange_cam, STRING("sub_bg"), 4)

    // The lecture screen is set up as a paper seal image in the upper right corner.
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
    CALL(evt_eff_softdelete, STRING("sub_bg"))
    WAIT_FRAMES(1)
    CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(lect_test_systemlevel)

    // Goombella and Ms. Mowz get a special demonstration setup.
    SWITCH(LW(9))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(christine_party_lecture_init))
        CASE_EQUAL(7)
        CALL_CHILD(PTR(chuchulina_party_lecture_init))
    END_SWITCH

    // Mario blinks out and in while the lecture stage is prepared.
    LOOP(6)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LECTUREN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
        WAIT_MS(100)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LECTUREN_BLINKING1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)

    // Goombella demonstrates her Tattle on a Goomba, or Ms. Mowz sniffs out a treasure box, on the stage.
    SWITCH(LW(9))
        CASE_EQUAL(1)
        CALL_CHILD(PTR(christine_party_lecture))
        CASE_EQUAL(7)
        CALL_CHILD(PTR(chuchulina_party_lecture))
    END_SWITCH

    // Mario and the partner flicker in turn as the stage ends.
    LOOP(6)
        CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 1)
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LECTUREN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
        CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 0)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 1)

    // The cameras are restored and the field music returns.
    CALL(evt_mario_init_camid)
    CALL(evt_party_init_camid, 0)
    CALL(lect_cam_load, 0)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_HIDE)
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 0)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_snd_envoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_envon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PARTY_LECTURE1"), LW(0), LW(1), LW(2), 0)

    // The paper seal folds away and the NPCs are released.
    CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
    CALL(evt_img_wait_animend, STRING("img"))
    CALL(evt_img_release, STRING("img"))
    CALL(evt_paper_delete, STRING("OFF_d_seal_upper_right"))
    CALL(evt_npc_start_for_event)
    SWITCH(LW(9))
    END_SWITCH
    RETURN
EVT_END()

/**
 * Sets up the Ultra Hammer lecture: Mario is moved aside while the Toad guide (キノピコ) is placed next to the stage
 * and the camera is set to face them.
 */
EVT_BEGIN(hammer_lv3_juyoitem_lecture_init)
#if VERSION > VERSION_DEMO_US
    CALL(evt_mapdisp_onoff, 0)
#endif
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_mario_set_pos, 0, -600, 30)
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
    CALL(evt_mario_set_pos, 0, -600, 30)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(26.0f), FLOAT(30.0f), FLOAT(-100.0f), 0, EASING_SINE_IN_OUT)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pos, 0, -600, 30)
    CALL(evt_mario_set_dir, 90, 300, 0)
    RETURN
EVT_END()

/**
 * Button prompt animation for the Ultra Hammer lecture, step one:
 * the first prompt is bright and flashes between two icons while the second is dimmed.
 */
EVT_BEGIN(hammer_lv3_juyoitem_lecture_sub01)
    CALL(lecture_button_set_alpha, 0, 255)
    CALL(lecture_button_set_alpha, 1, 128)
    CALL(lecture_button_set_icon_id, 1, ICON_STICK_NEUTRAL)
    LOOP(0)
        CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_B_PRESSED)
        WAIT_MS(300)
        CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_B)
        WAIT_MS(300)
    END_LOOP
    RETURN
EVT_END()

/**
 * Button prompt animation for the Ultra Hammer lecture, step two:
 * the second prompt is bright and cycles through the eight direction icons while the first is dimmed.
 */
EVT_BEGIN(hammer_lv3_juyoitem_lecture_sub02)
    CALL(lecture_button_set_alpha, 0, 128)
    CALL(lecture_button_set_alpha, 1, 255)
    CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_B_PRESSED)
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
 * The Ultra Hammer lecture (lecture_u_hammer_04 to 07):
 * the Toad guide walks in and explains how to use the hammer with the button prompts,
 * and checks that Mario presses A and B himself.
 * Guess on the detail: the prompts and button checks follow the sub01 and sub02 animations.
 */
EVT_BEGIN(hammer_lv3_juyoitem_lecture)
    // The button prompt icons are prepared and Toadette walks up to Mario.
    BEGIN_SIBLING_EVENT
        CALL(lecture_button_rtn)
    END_SIBLING_EVENT
    CALL(evt_npc_set_camid, STRING("キノピコ"), 7)
    WAIT_FRAMES(1)
    CALL(lecture_button_set_pos, 0, FLOAT(-200.0f), FLOAT(-25.0f), FLOAT(0.0f))
    CALL(lecture_button_set_pos, 1, FLOAT(-140.0f), FLOAT(-25.0f), FLOAT(0.0f))
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 85)
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)

    // She congratulates Mario on the Ultra Hammer and explains the new hammer technique: hold B and twist the
    // stick.
    CALL(evt_msg_print, 0, STRING("lecture_u_hammer_04"), 0, STRING("キノピコ"))
    CALL(evt_msg_print_add, 0, STRING("lecture_u_hammer_05"))

    // Practice loop: Toadette tells Mario to try it, and nags him now and then until he does.
    LOOP(0)
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("lecture_u_hammer_06"), 0, 0)
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
                CALL(evt_msg_print, 0, STRING("lecture_u_hammer_07"), 0, STRING("キノピコ"))
                CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_3"))
            END_LOOP
            CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_1"), STRING("T_1"))
            CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_1"))
        END_INLINE_EVENT

        // The camera frames Mario and the button prompts start flashing.
        CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(85.0f), FLOAT(520.0f), FLOAT(0.0f), FLOAT(40.0f), FLOAT(-100.0f), 1000, EASING_SINE_IN_OUT)
        WAIT_MS(1000)
        CALL(lecture_button_flag_on_off, 0, 2, 1)
        CALL(lecture_button_set_icon_id, 0, ICON_BUTTON_B)
        CALL(lecture_button_set_seq, 0, 2)
        CALL(lecture_button_flag_on_off, 1, 2, 1)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_NEUTRAL)
        CALL(lecture_button_set_seq, 1, 2)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_seq_wait, 1, 1)
        SPAWN_EVENT_GET_ID(PTR(hammer_lv3_juyoitem_lecture_sub01), LW(11))
        SET_VALUE(LW(7), 0)

        // Mario gets limited control; the lesson watches his hammer charge.
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SET_VALUE(LW(7), 0)
        SET_VALUE(LW(12), 0)
        CALL(mainwindow_opendisable)
        CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, 512)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_key_onoff, 1)
        LOOP(0)
            CALL(lect_status_off)
            CALL(evt_mario_get_pos, 0, LW(5), LW(1), LW(6))
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            CALL(evt_mario_get_motion, LW(3))
            IF_INT_EQUAL(LW(12), 0)
                IF_INT_EQUAL(LW(3), 19)
                    ADD(LW(12), 1)
                END_IF
            ELSE
                IF_INT_EQUAL(LW(12), 1)
                    IF_INT_NOT_EQUAL(LW(3), 19)
                        CALL(x_flag_set, 1, 2)
                        BREAK_LOOP
                    END_IF
                END_IF
            END_IF
            IF_INT_EQUAL(LW(12), 0)
                SWITCH(LW(7))
                    CASE_EQUAL(0)
                    CALL(lect_push_b_check, LW(7))
                    IF_INT_NOT_EQUAL(LW(7), 0)
                        DELETE_EVENT(LW(11))
                        SPAWN_EVENT_GET_ID(PTR(hammer_lv3_juyoitem_lecture_sub02), LW(11))
                    END_IF
                    CASE_EQUAL(1)
                    CALL(lect_push_b_check, LW(7))
                    IF_INT_EQUAL(LW(7), 0)
                        DELETE_EVENT(LW(11))
                        SPAWN_EVENT_GET_ID(PTR(hammer_lv3_juyoitem_lecture_sub01), LW(11))
                    END_IF
                END_SWITCH
            ELSE
                IF_INT_LESS_OR_EQUAL(LW(7), 1)
                    SET_VALUE(LW(7), 2)
                    CALL(lecture_button_set_seq, 0, 3)
                    CALL(lecture_button_set_seq, 1, 3)
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP

        // Mario released the charge: control returns to normal and the prompts are dismissed.
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(lect_status_on)
        CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, 0)
        CALL(mainwindow_openenable)
        DELETE_EVENT(LW(11))
        CALL(lecture_button_set_seq, 0, 3)
        CALL(lecture_button_set_seq, 1, 3)
        CALL(evt_msg_continue)

        // Mario strikes a pose, then turns to Toadette.
        CALL(evt_mario_set_pose, STRING("M_V_2"))
        WAIT_MS(500)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_seq_wait, 1, 1)
        WAIT_MS(300)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_mario_adjust_dir)
        CALL(evt_mario_set_dir, 90, 150, 0)
        CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(26.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, EASING_LINEAR)
        WAIT_MS(1000)

        // Toadette asks whether Mario understood; if not, the practice repeats.
        CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_2"), STRING("T_2"))
        CALL(evt_msg_print, 0, STRING("lecture_u_hammer_08"), 0, STRING("キノピコ"))
        CALL(evt_msg_select, 0, STRING("lecture_u_hammer_09"))
        CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_1"), STRING("T_1"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("lecture_u_hammer_10"))
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP

    // Toadette is pleased and steps aside to show what the new hammer can do.
    CALL(evt_msg_print_add, 0, STRING("lecture_u_hammer_11"))
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(52.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, EASING_SINE_IN_OUT)
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    ADD(LW(0), 35)
    CALL(evt_npc_set_ry, STRING("キノピコ"), 90)
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_ry, STRING("キノピコ"), 270)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("A_1"))
    WAIT_MS(200)

    // A test block falls in front of Mario.
    SET_VALUE(LW(1), STRING("TESTMOBJ"))
    CALL(evt_mobj_entry, LW(1), STRING("MOBJ_Lv2Block"))
    CALL(lect_mobj_hit_nasi, LW(1))
    CALL(evt_mobj_set_camid, LW(1), 7)
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    ADD(LW(2), 50)
    ADD(LW(3), 150)
    SET_VALUE(LW(4), 0)
    CALL(evt_mobj_flag_onoff, 1, LW(1), MOBJ_FLAG_GRAVITY)
    CALL(evt_mobj_set_position, LW(1), LW(2), LW(3), LW(4))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_LECTURE_BLOCK_FALL1"), LW(7))
    CALL(evt_mobj_set_gravity_bound, LW(1), FLOAT(0.16601563f), FLOAT(1.0f))
    WAIT_MS(600)
    CALL(evt_snd_sfxoff, LW(7))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_LECTURE_BLOCK_DOWN1"), 0)
    WAIT_MS(400)
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("A_2"))
    WAIT_MS(170)

    // Toadette comments as the block flashes and disappears.
    CALL(evt_msg_print, 0, STRING("lecture_u_hammer_12"), 0, STRING("キノピコ"))
    WAIT_MS(300)
    LOOP(6)
        CALL(evt_mobj_flag_onoff, 1, LW(1), MOBJ_FLAG_NO_DISP)
        WAIT_MS(100)
        CALL(evt_mobj_flag_onoff, 0, LW(1), MOBJ_FLAG_NO_DISP)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mobj_delete, LW(1))
    WAIT_MS(300)

    // She gives a few last pointers, strolling along in the room.
    CALL(evt_msg_print, 0, STRING("lecture_u_hammer_13"), 0, STRING("キノピコ"))
    CALL(evt_npc_flag_onoff, 0, STRING("キノピコ"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_4"), STRING("T_4"))
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_4"))
    CALL(evt_msg_print_add, 0, STRING("lecture_u_hammer_13_1"))
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("W_2"))
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(30.0f), 0)
    CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_1"), STRING("T_1"))
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_1"))
    CALL(evt_msg_print, 0, STRING("lecture_u_hammer_13_2"), 0, STRING("キノピコ"))
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("W_2"))
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(30.0f), 0)
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_4"))
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("W_2"))
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    ADD(LW(0), 10)
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(30.0f), 0)
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_4"))
    WAIT_MS(1500)

    // Toadette says goodbye and walks off.
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    ADD(LW(0), 70)
    CALL(evt_npc_set_ry, STRING("キノピコ"), 90)
    WAIT_MS(300)
    CALL(evt_npc_flag_onoff, 0, STRING("キノピコ"), NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("R_2"))
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_position, STRING("キノピコ"), 0, -1000, 0)
#if VERSION > VERSION_DEMO_US
    CALL(evt_mapdisp_onoff, 1)
#endif
    RETURN
EVT_END()

/**
 * Ceremony for picking up an important item (the item entry is in LW(8), its effect in LW(9)):
 * the item flies up to the item-get effect, the lecture jingle plays, a paper page cutout opens and,
 * for item 11 (the Ultra Hammer), the Toad guide gives the hammer lecture;
 * at the end Mario blinks back into the field, the music resumes and the item is hidden.
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
        CASE_EQUAL(11)
        CALL_CHILD(hammer_lv3_juyoitem_lecture_init)
    END_SWITCH
    SWITCH(LW(10))
        CASE_EQUAL(11)
        CALL_CHILD(hammer_lv3_juyoitem_lecture)
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
 * Script helper that turns on the collision of a map object's two hit objects without making it solid,
 * so the lecture's treasure box can be placed on the stage.
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
            hitGrpFlagOn(mobj->hitObj[i].name, 1);
        }
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper that finds a point args[1] units from the camera in the direction of the partner (args[0] 0 =
 * the active partner, otherwise partner slot 1) and stores it in args[2..4];
 * stores 0 when there is no such partner.
 */
static USER_FUNC(lect_itemget_kesan) {
    EvtWord* args = event->args;
    PartyEntry* party = partyGetPtr(args[0] == 0 ? partyCtrlNo : 1);
    f32 dist = evtGetFloat(event, args[1]);
    CameraEntry* cam = camGetPtr(CAMERA_3D);
    Vec pos;
    Vec dir;

    if (party == NULL) {
        evtSetFloat(event, args[2], 0.0f);
        evtSetFloat(event, args[3], 0.0f);
        evtSetFloat(event, args[4], 0.0f);
        return EVT_RETURN_DONE;
    }
    pos = party->position;
    pos.y += party->height / 2;
    PSVECSubtract(&pos, &cam->cameraPos, &dir);
    PSVECNormalize(&dir, &dir);
    PSVECScale(&dir, &dir, dist);
    PSVECAdd(&cam->cameraPos, &dir, &dir);
    evtSetFloat(event, args[2], dir.x);
    evtSetFloat(event, args[3], dir.y);
    evtSetFloat(event, args[4], dir.z);
    return EVT_RETURN_DONE;
}

/**
 * Script helper that shows (args[1] nonzero) or removes Mario's speech balloon effect above the named NPC,
 * drawn by camera 7.
 */
static USER_FUNC(lect_npc_balloon_on_off) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);

    if (evtGetValue(event, args[1])) {
        eff = effMarioBalloonEntry(1, npc);
        ((MarioBalloonWork*)eff->userdata)->camera = 7;
    } else {
        effDelete(eff);
    }
    return EVT_RETURN_DONE;
}

/** Script helper that sets the camera of the named speech-bubble effect args[0] to camera id args[1]. */
static USER_FUNC(efffukidashichange_cam) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    ((EffFukidashiWork*)effNameToPtr(name)->userdata)->camera = camId;
    return EVT_RETURN_DONE;
}

/** Linear interpolation for the partner glide, the same as lect_mario_ido_sub. */
static f32 lect_party_ido_sub(f32 elapsed, f32 total, f32 end, f32 start) {
    f32 rem = total - elapsed;

    return (rem * end + elapsed * start) / total;
}

/**
 * Script helper that glides the partner (args[0] 0 = the active partner, otherwise slot 1) to a target over
 * args[1] milliseconds.
 * On the first call it stores the partner's position in LW(3..5);
 * then it interpolates between LW(0..2) and that start. Finishes when the time is up.
 */
static USER_FUNC(lect_party_ido_sum) {
    EvtWord* args = event->args;
    PartyEntry* party = partyGetPtr(args[0] == 0 ? partyCtrlNo : 1);
    s32 durationMs = evtGetValue(event, args[1]);
    s32 elapsed;

    if (isFirstCall) {
        wwstart = event->lifetime;
        evtSetFloat(event, LW(3), party->position.x);
        evtSetFloat(event, LW(4), party->position.y);
        evtSetFloat(event, LW(5), party->position.z);
        return EVT_RETURN_BLOCK;
    }
    elapsed = OSTicksToMilliseconds((u64)OSDiffTick(event->lifetime, wwstart));
    if (elapsed > durationMs) {
        elapsed = durationMs;
    }
    party->position.x = lect_party_ido_sub(elapsed, durationMs, evtGetFloat(event, LW(3)), evtGetFloat(event, LW(0)));
    party->position.y = lect_party_ido_sub(elapsed, durationMs, evtGetFloat(event, LW(4)), evtGetFloat(event, LW(1)));
    party->position.z = lect_party_ido_sub(elapsed, durationMs, evtGetFloat(event, LW(5)), evtGetFloat(event, LW(2)));
    if (elapsed >= durationMs) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Script helper that stores the item id of the item being picked up (the item entry in LW(8)) in args[0]. */
static USER_FUNC(juyoitem_get_no) {
    evtSetValue(event, event->args[0], ((ItemEntry*)event->lwData[8])->itemId);
    return EVT_RETURN_DONE;
}

/** Script helper that ends the item-get effect (LW(9)), restores Mario's camera id and turns off the item's glow. */
static USER_FUNC(juyoitem_end) {
    ItemEntry* item = (ItemEntry*)event->lwData[8];

    effSoftDelete((EffectEntry*)event->lwData[9]);
    marioSetCamId(CAMERA_3D);
    iconFlagOff(item->name, ICON_FLAG_CAM_EFFECT_B);
    return EVT_RETURN_DONE;
}

/**
 * Script helper that prepares the item pick-up lecture:
 * turns off the item's glow, moves the item out of sight, resets Mario's pose and system level.
 */
static USER_FUNC(juyoitem_gogo) {
    ItemEntry* item = (ItemEntry*)event->lwData[8];

    iconFlagOff(item->name, ICON_FLAG_CAM_EFFECT_B);
    itemSetPosition(item, 0.0f, -1000.0f, 0.0f);
    marioChgStayMotion();
    marioStSystemLevel(0);
    return EVT_RETURN_DONE;
}

/** Script helper that hides the picked-up item's icon once the lecture is over. */
static USER_FUNC(juyoitem_goato) {
    iconFlagOn(((ItemEntry*)event->lwData[8])->name, ICON_FLAG_HIDDEN);
    return EVT_RETURN_DONE;
}

/**
 * Script helper that turns off screen blending on the map object named args[0] ("me" is the running event's
 * object).
 */
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

/** Script helper that stores TRUE in args[0] while A is held, else FALSE. */
static USER_FUNC(lect_push_a_check) {
    EvtWord* args = event->args;

    if (keyGetButton(0) & PAD_BUTTON_A) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}

/** Script helper that stores TRUE in args[0] while B is held, else FALSE. */
static USER_FUNC(lect_push_b_check) {
    EvtWord* args = event->args;

    if (keyGetButton(0) & PAD_BUTTON_B) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}

/**
 * Script helper that turns the active partner to face left or right on screen:
 * the display direction becomes 0 when the angle args[0] is over 180 and 180 otherwise.
 */
static USER_FUNC(set_dir) {
    s32 dir = evtGetValue(event, event->args[0]);
    PartyEntry* party = partyGetPtr(0);

    if (party != NULL) {
        s32 angle = dir > 180 ? 270 : 90;
        party->displayDirectionTarget = party->displayDirection = 270 - angle;
    }
    return EVT_RETURN_DONE;
}


/** Script helper that waits until the lecture music (BGM channel 1) has started. */
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

