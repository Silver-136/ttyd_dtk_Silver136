// Rogueport Sewers (tik, chika: underground): the paper demon's (peramajin) lecture on the plane power
// (plane_majin_lecture).

#include "version.h"
#include "icon_ids.h"
#include "eff/eff_fukidashi.h"
#include "eff/eff_itemget.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "pmario_sound.h"
#include "statuswindow.h"
#include "driver/hitdrv.h"
#include "evt/evt_lecture.h"
#include "mot/mot_slit.h"
#include "mot/mot_roll.h"
#include "driver/npcdrv.h"

#if VERSION <= VERSION_DEMO_JP
#include "evt/evt_window.h"
#endif

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
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_sub.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include <dolphin/pad.h>
#include "evt/evt_lect.h"

static OSTime wwstart;
static s32 x_flag_data[2];
static void lecture_button_disp(CameraId camera, void* param);
static LectButton lbtn[2];
/**
 * Jumps the camera 3D by the offset ofs (position and look-at both) with an immediate transition;
 * used to move the view to the item-get stage and back.
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

static Vec daiv;

static USER_FUNC(lect_map_cam_chg);
static USER_FUNC(bgm_start_wait);
static USER_FUNC(evt_nomal_cange);
static USER_FUNC(lect_push_r_check);
static USER_FUNC(evtflash);
static USER_FUNC(lect_slit_check);
static USER_FUNC(dai_moveti_get);
static USER_FUNC(dai_moveti_set);
static USER_FUNC(lect_mobj_hit_nasi);
static USER_FUNC(effitemget_busaiku);
static USER_FUNC(effitemget_open_ptr);
static USER_FUNC(effitemget_open);
static USER_FUNC(effitemgetchange_cam_ptr);
static USER_FUNC(effitemgetchange_cam);
static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings);
static USER_FUNC(lecture_button_set_pos);
static USER_FUNC(lecture_button_seq_wait);
static USER_FUNC(lecture_button_get_icon_id);
static USER_FUNC(lecture_button_set_seq);
static USER_FUNC(lecture_button_flag_on_off);
static USER_FUNC(lecture_button_set_alpha);
static USER_FUNC(lecture_button_set_icon_id);
static USER_FUNC(lecture_button_rtn);
static USER_FUNC(x_flag_wait);
static USER_FUNC(x_flag_get);
static USER_FUNC(x_flag_add);
static USER_FUNC(x_flag_set);
static USER_FUNC(lect_status_on);
static USER_FUNC(lect_status_off);
static USER_FUNC(lect_mario_ido_sum);
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);

EvtWord plane_majin_lecture_init[73];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord plane_majin_lecture[];
#else
EvtWord plane_majin_lecture[715];
#endif

/**
 * Linear interpolation helper for lect_mario_ido_sum:
 * returns the weighted blend of end and start, ((total - elapsed) * end + elapsed * start) / total.
 */
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start) {
    f32 rem = total - elapsed;

    return (rem * end + elapsed * start) / total;
}

/**
 * Script helper that moves Mario in a straight line during the lecture.
 * args[0] is the duration in ms; on the first call it saves Mario's position into LW(0) to LW(2),
 * then each frame moves him toward the target held in LW(3) to LW(5) until the time is up.
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

/** Script helper. Forces the status window closed during the lecture (statusWinForceClose). */
static USER_FUNC(lect_status_off) {
    statusWinForceClose();
    return EVT_RETURN_DONE;
}

/** Script helper. Lets the status window open again (clears the forced close). */
static USER_FUNC(lect_status_on) {
    statusWinForceCloseClear();
    return EVT_RETURN_DONE;
}

/**
 * Script helper.
 * Sets lecture sync flag args[0] (0 or 1) to the value args[1];
 * the lecture's parallel scripts use these flags to wait on each other.
 */
static USER_FUNC(x_flag_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper. Adds args[1] to lecture sync flag args[0]. */
static USER_FUNC(x_flag_add) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] += evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper. Stores lecture sync flag args[0] into the variable args[1]. */
static USER_FUNC(x_flag_get) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], x_flag_data[index]);
    return EVT_RETURN_DONE;
}

/** Script helper. Blocks until lecture sync flag args[0] equals args[1]. */
static USER_FUNC(x_flag_wait) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);

    if (value == x_flag_data[index]) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

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
 * Script helper that runs the lecture's two button icons.
 * On its first call it clears them and puts them at (200, -25).
 * Every frame it runs each icon's state machine (1 idle, 2 fade in by 7 alpha per frame, 3 fade out, 4 hide)
 * and queues the draw; it ends when both icons reach state 4.
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

/** Script helper. Sets the icon of button args[0] to the icon id args[1]. */
static USER_FUNC(lecture_button_set_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->iconId = value;
    return EVT_RETURN_DONE;
}

/** Script helper. Sets the opacity of button args[0] to args[1]. */
static USER_FUNC(lecture_button_set_alpha) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->alpha = value;
    return EVT_RETURN_DONE;
}

/** Script helper. Sets (args[2] nonzero) or clears the flag bits args[1] of button args[0] (for instance hidden). */
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

/** Script helper. Sets the state of button args[0] to args[1]: 2 fades it in, 3 fades it out, 4 hides it. */
static USER_FUNC(lecture_button_set_seq) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->seq = value;
    return EVT_RETURN_DONE;
}

/**
 * Script helper. Stores the current state (seq) of button args[0] into args[1].
 * Despite the name it does not return the icon id.
 */
static USER_FUNC(lecture_button_get_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    LectButton* btn = &lbtn[index];

    evtSetValue(event, args[1], btn->seq);
    return EVT_RETURN_DONE;
}

/** Script helper. Blocks until button args[0] reaches state args[1]. */
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

/** Script helper. Moves button args[0] to the position (args[1], args[2], args[3]). */
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
 * Recursive helper for lect_map_cam_chg: puts a map object,
 * its children and (if siblings is set) its siblings onto the camera camId.
 */
// Descend through this subtree; only recursive calls include following siblings.
static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings) {
    obj->cameraId = camId;
    if (obj->child != NULL) {
        lect_map_cam_chg_sub(obj->child, camId, TRUE);
    }
    if (siblings && obj->next != NULL) {
        lect_map_cam_chg_sub(obj->next, camId, TRUE);
    }
}

/** Script helper. Moves the map object named args[0], with its children, onto the camera args[1]. */
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

/** Script helper. Changes the camera that the named item-get effect (args[0]) draws on to args[1]. */
static USER_FUNC(effitemgetchange_cam) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    ((ItemGetWork*)effNameToPtr(name)->userdata)->camera = camId;
    return EVT_RETURN_DONE;
}

/** Script helper. The same as effitemgetchange_cam, but takes the effect entry pointer in args[0]. */
static USER_FUNC(effitemgetchange_cam_ptr) {
    EvtWord* args = event->args;
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, args[0]);

    ((ItemGetWork*)eff->userdata)->camera = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper. Starts the opening state of the named item-get effect (state 1). */
static USER_FUNC(effitemget_open) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/** Script helper. The same as effitemget_open, but takes the effect entry pointer. */
static USER_FUNC(effitemget_open_ptr) {
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, event->args[0]);

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

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
 * Script helper.
 * Turns the hit areas (nasi = none) of the named map object (or "me") on;
 * each of its two hit entries with a name is switched on with hitGrpFlagOn.
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

/** Script helper. Stores the vector (args[0], args[1], args[2]) as the lecture platform's movement (daiv). */
static USER_FUNC(dai_moveti_set) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    daiv = (Vec){x, y, z};
    return EVT_RETURN_DONE;
}

/** Script helper. Reads the stored platform movement into args[0], args[1], args[2]. */
static USER_FUNC(dai_moveti_get) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], daiv.x);
    evtSetFloat(event, args[1], daiv.y);
    evtSetFloat(event, args[2], daiv.z);
    return EVT_RETURN_DONE;
}

/**
 * Script helper.
 * Stores TRUE in args[0] if Mario is currently in paper mode (slit through a narrow gap), otherwise FALSE.
 */
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
 * Script helper.
 * A fixed white flash: fades to white over 40 ms, holds 60 ms, fades back over 40 ms and waits another 60 ms.
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
 * Sets up the black-chest demon's (peramajin) lecture on plane mode; run by tik_19's evt_majin2 just before
 * plane_majin_lecture: sets Mario's system level to 0, moves the camera over one second, places Mario at
 * (-300, -600, 0) facing 90 degrees, and moves the "hikou" map objects onto camera 7 (lect_map_cam_chg).
 */
EVT_BEGIN(plane_majin_lecture_init)
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
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(0.0f), FLOAT(42.0f), FLOAT(-101.799805f), 0, EASING_SINE_IN_OUT)
    CALL(lect_map_cam_chg, STRING("hikou"), 7)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

/**
 * The paper demon's lecture on plane mode (mac_majin_13 to mac_majin_19).
 * The demon explains the power with flashes (SFX_EVT_PERAMAJIN_LIGHT1), shows button icons to press,
 * has Mario ride the plane and try it while the sync flags track his progress,
 * and asks if he wants to hear it again (mac_majin_17); at the end the status window comes back.
 */
EVT_BEGIN(plane_majin_lecture)
    // The button prompt and the moving airplane panel are set up; the paper demon hovers before the camera.
    BEGIN_SIBLING_EVENT
        CALL(lecture_button_rtn)
    END_SIBLING_EVENT
    WAIT_FRAMES(1)
    CALL(lecture_button_set_pos, 0, FLOAT(-200.0f), FLOAT(-25.0f), FLOAT(0.0f))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(0), STRING("hikou"))
        SET_VALUE(LW(1), STRING("a_asiba"))
        SET_VALUE(LW(5), STRING("a_hikou"))
        CALL(evt_hit_bind_mapobj, LW(1), LW(0))
        CALL(evt_hit_bind_mapobj, LW(5), LW(0))
        CALL(dai_moveti_set, FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f))
        LOOP(0)
            CALL(dai_moveti_get, LW(2), LW(3), LW(4))
            CALL(evt_mapobj_trans, 1, LW(0), LW(2), LW(3), LW(4))
            CALL(evt_hit_bind_update, LW(1))
            CALL(evt_hit_bind_update, LW(5))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("ペラ魔人"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("ペラ魔人"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_camid, STRING("ペラ魔人"), CAMERA_3D_EFFECTB)
    WAIT_FRAMES(1)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_cam_get_at, CAMERA_3D, LW(0), LW(1), LW(2))
            SUBTRACT(LW(1), 25)
            CALL(evt_npc_set_position, STRING("ペラ魔人"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT

    // The demon reveals his trick, laughs at Mario and curses him in flashes of light.
    CALL(evt_npc_set_anim, STRING("ペラ魔人"), STRING("P_1"))
    WAIT_MS(200)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_majin_13"), 0, STRING("ペラ魔人"))
    CALL(evt_mario_set_pose, STRING("M_N_H"))
    LOOP(4)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_PERAMAJIN_LIGHT1"), LW(0), LW(1), LW(2), 0)
        CALL(evtflash)
    END_LOOP
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_majin_14"), 0, STRING("ペラ魔人"))

    // The camera pans as the airplane panel slides in beside Mario.
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(80.0f), FLOAT(42.0f), FLOAT(-101.799805f), 1000, EASING_SINE_IN_OUT)
    SET_FLOAT_VALUE(LW(1), FLOAT(2.6660156f))
    CALL(dai_moveti_get, LW(0), LW(2), LW(3))
    LOOP(0)
        SUBTRACT_FLOAT(LW(0), LW(1))
        IF_INT_LESS_OR_EQUAL(LW(0), FLOAT(-220.0f))
            SET_FLOAT_VALUE(LW(0), FLOAT(-220.0f))
            CALL(dai_moveti_set, LW(0), LW(2), LW(3))
            BREAK_LOOP
        ELSE
            CALL(dai_moveti_set, LW(0), LW(2), LW(3))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP

    // The demon explains airplane panels; the lesson below repeats until Mario says he gets it.
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("mac_majin_15"), 0, STRING("ペラ魔人"))
    LOOP(0)
        CALL(x_flag_set, 0, 1)
        BEGIN_INLINE_EVENT
            SET_VALUE(LW(0), STRING("hikou"))
            SET_VALUE(LW(1), STRING("a_hikou_ue"))
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
        CALL(x_flag_set, 1, 1)
        BEGIN_SIBLING_EVENT
            LOOP(0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                IF_INT_GREATER_OR_EQUAL(LW(0), -220)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            LOOP(0)
#if VERSION >= VERSION_JP
                CALL(x_flag_get, 1, LW(0))
                IF_INT_NOT_EQUAL(LW(0), 3)
#endif
                    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(0.0f), FLOAT(42.0f), FLOAT(-101.799805f), 0, EASING_SINE_IN_OUT)
#if VERSION <= VERSION_DEMO_JP
                CALL(x_flag_get, 1, LW(0))
#else
                END_IF
#endif
                IF_INT_EQUAL(LW(0), 2)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_SIBLING_EVENT

        // Mario walks onto the panel and is told to press the button; its icon flashes until he does.
        CALL(evt_mario_mov_pos2, FLOAT(-200.0f), FLOAT(0.0f), FLOAT(80.0f))
        CALL(evt_msg_print_add, 0, STRING("mac_majin_15_1"))
        CALL(lecture_button_set_pos, 0, FLOAT(-200.0f), FLOAT(-25.0f), FLOAT(0.0f))
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
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_cam_letter_box_disable, 1)
        CALL(mainwindow_opendisable)
        CALL(evt_mario_trigflag_onoff, 1, -2147483648)
        CALL(evt_mario_trigflag_onoff, 1, 1073741824)
        CALL(evt_mario_set_enable_key, 2048)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_key_onoff, 1)
        LOOP(0)
            CALL(lect_status_off)
            WAIT_FRAMES(1)
            CALL(evt_mario_get_motion, LW(3))
            IF_INT_EQUAL(LW(3), 24)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(lect_status_on)
        CALL(evt_mario_trigflag_onoff, 0, -2147483648)
        CALL(evt_mario_trigflag_onoff, 0, 1073741824)
        CALL(evt_mario_set_enable_key, 0)
        CALL(mainwindow_openenable)
        CALL(evt_cam_letter_box_disable, 0)
#else
        LOOP(0)
            CALL(evt_key_get_buttontrg, 0, LW(0))
            IF_BITS_SET(LW(0), PAD_BUTTON_Y)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_msg_continue)
        CALL(evt_mario_paper_plane, 1)
#endif

        // Mario turns into a paper airplane and glides off the panel, and the view follows him down.
        CALL(x_flag_set, 0, 2)
        CALL(lecture_button_set_seq, 0, 3)
        BEGIN_INLINE_EVENT
            CALL(x_flag_set, 0, 1)
            LOOP(0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                IF_INT_LESS_OR_EQUAL(LW(1), -750)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
#if VERSION >= VERSION_JP
            CALL(x_flag_set, 1, 3)
            WAIT_FRAMES(1)
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            CALL(dai_moveti_set, FLOAT(3000.0f), FLOAT(0.0f), FLOAT(0.0f))
            CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
            SET_VALUE(LW(0), -300)
            SET_VALUE(LW(1), -490)
            SET_VALUE(LW(2), 0)
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            CALL(evt_cam3d_evt_set, FLOAT(-297.0f), FLOAT(-351.0f), FLOAT(457.0f), FLOAT(-297.0f), FLOAT(-449.0f), FLOAT(-101.799805f), 1000, EASING_LINEAR)
            CALL(evt_mario_cont_onoff, 0)
            CALL(lect_mario_ido_sum, 1000)
            CALL(evt_mario_cont_onoff, 1)
            CALL(dai_moveti_set, FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f))
            CALL(x_flag_set, 1, 1)
#endif
            LOOP(0)
                CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
                CALL(x_flag_get, 0, LW(3))
                IF_INT_NOT_EQUAL(LW(3), 1)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_INLINE_EVENT

        // The demon explains steering and asks whether Mario got all that.
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_flag_on_off, 0, 2, 0)
        WAIT_MS(300)
#if VERSION <= VERSION_DEMO_JP
        CALL(dai_moveti_get, LW(0), LW(1), LW(2))
        ADD_FLOAT(LW(0), FLOAT(700.0f))
        SUBTRACT_FLOAT(LW(1), FLOAT(260.0f))
        CALL(dai_moveti_set, LW(0), LW(1), LW(2))
        CALL(evt_msg_print_add, 0, STRING("mac_majin_16"))
#else
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("mac_majin_16"), 0, STRING("ペラ魔人"))
#endif
        CALL(evt_msg_select, 0, STRING("mac_majin_17"))
#if VERSION <= VERSION_DEMO_JP
        IF_INT_EQUAL(LW(0), 0)
            CALL(x_flag_set, 1, 2)
            CALL(x_flag_set, 0, 2)
            CALL(evt_mario_paper_plane, 0)
            CALL(dai_moveti_set, FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f))
#else
        SET_VALUE(LW(3), LW(0))
#endif
        LOOP(0)
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
#if VERSION <= VERSION_DEMO_JP
            IF_INT_LESS_OR_EQUAL(LW(1), -900)
#else
            IF_INT_GREATER_OR_EQUAL(LW(1), -550)
#endif
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_mario_cont_onoff, 0)
        CALL(evt_mario_set_pos, -300, -600, 0)
        CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(0.0f), FLOAT(42.0f), FLOAT(-101.799805f), EASING_LINEAR, 11)
        CALL(evt_mario_set_pos, -300, -400, 0)
        WAIT_MS(1000)
        CALL(evt_mario_cont_onoff, 1)
#else
        IF_INT_EQUAL(LW(3), 0)
            CALL(x_flag_set, 1, 2)
            WAIT_FRAMES(2)
            CALL(evt_cam3d_evt_set, FLOAT(-300.0f), FLOAT(-460.0f), FLOAT(457.0f), FLOAT(-300.0f), FLOAT(-558.0f), FLOAT(-101.799805f), 1200, EASING_LINEAR)
            CALL(x_flag_set, 0, 2)
            CALL(evt_mario_paper_plane, 0)
#endif

            // After "I zoned out", Mario lands, the panel slides back in and the demon goes over it again.
            CALL(evt_mario_wait_landon)
            CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(80.0f), FLOAT(42.0f), FLOAT(-101.799805f), 1000, EASING_SINE_IN_OUT)
            SET_FLOAT_VALUE(LW(1), FLOAT(2.6660156f))
            CALL(dai_moveti_get, LW(0), LW(2), LW(3))
            LOOP(0)
                SUBTRACT_FLOAT(LW(0), LW(1))
                IF_INT_LESS_OR_EQUAL(LW(0), FLOAT(-220.0f))
                    SET_FLOAT_VALUE(LW(0), FLOAT(-220.0f))
                    CALL(dai_moveti_set, LW(0), LW(2), LW(3))
                    BREAK_LOOP
                ELSE
                    CALL(dai_moveti_set, LW(0), LW(2), LW(3))
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            CALL(evt_msg_print_add, 0, STRING("mac_majin_18"))
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP

    // The demon hints at how to change back; Mario drops out of plane mode and the demon leaves.
    CALL(evt_msg_print_add, 0, STRING("mac_majin_18_1"))
#if VERSION >= VERSION_JP
    CALL(x_flag_set, 1, 2)
    WAIT_FRAMES(2)
    CALL(evt_cam3d_evt_set, FLOAT(-300.0f), FLOAT(-460.0f), FLOAT(457.0f), FLOAT(-300.0f), FLOAT(-558.0f), FLOAT(-101.799805f), 1200, EASING_LINEAR)
#endif
    CALL(x_flag_set, 0, 2)
    CALL(evt_mario_paper_plane, 0)
#if VERSION <= VERSION_DEMO_JP
    LOOP(0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS_OR_EQUAL(LW(1), -850)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
#else
    CALL(evt_mario_wait_landon)
#endif
    CALL(evt_msg_print_add, 0, STRING("mac_majin_19"))
    CALL(evt_npc_set_anim, STRING("ペラ魔人"), STRING("P_2"))
    WAIT_MS(200)
    CALL(evt_npc_set_camid, STRING("ペラ魔人"), CAMERA_3D)
    CALL(evt_npc_set_position, STRING("ペラ魔人"), 0, -1000, 0)
    RETURN
#if VERSION <= VERSION_DEMO_JP
EVT_END()
#else
EVT_END_PAD()
#endif

/** Script helper. Stores TRUE in args[0] while the R trigger is held on controller 0, otherwise FALSE. */
#if VERSION >= VERSION_JP
static USER_FUNC(lect_push_r_check) {
    EvtWord* args = event->args;

    if (keyGetButton(0) & PAD_TRIGGER_R) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}
#endif

#if VERSION <= VERSION_DEMO_JP
static USER_FUNC(lect_push_r_check) {
    EvtWord* args = event->args;

    if (keyGetButton(0) & PAD_TRIGGER_R) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}
#endif

/**
 * Script helper (normal change).
 * Cancels Mario's roll and paper mode so he is back to normal,
 * and blocks until the roll is finished and the slit (paper) pose is over.
 */
static USER_FUNC(evt_nomal_cange) {
    MarioWork* mario = marioGetPtr();

    if (isFirstCall) {
#if VERSION <= VERSION_DEMO_JP
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


/** Script helper. Blocks until the music on channel 1 has started. */
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

