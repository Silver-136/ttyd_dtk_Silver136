// Hooktail Castle (gon) lecture script: the paper genie (peramajin) teaches the slit move (squeezing through narrow gaps), run when the Paper Curse is found in gon_06.

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
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
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
    camera->curveA = EASING_SINE_IN_OUT;
}

static Vec daiv;

static USER_FUNC(evtflash);
static USER_FUNC(lect_slit_check);
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
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);
static USER_FUNC(lect_mario_ido_sum);
static USER_FUNC(x_flag_add);
static USER_FUNC(x_flag_wait);
static USER_FUNC(lecture_button_set_alpha);
static USER_FUNC(lecture_button_get_icon_id);
static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings);
static USER_FUNC(lect_map_cam_chg);
static USER_FUNC(effitemgetchange_cam);
static USER_FUNC(effitemgetchange_cam_ptr);
static USER_FUNC(effitemget_open);
static USER_FUNC(effitemget_open_ptr);
static USER_FUNC(effitemget_busaiku);
static USER_FUNC(lect_mobj_hit_nasi);
static USER_FUNC(dai_moveti_set);
static USER_FUNC(dai_moveti_get);
static USER_FUNC(lect_push_r_check);
static USER_FUNC(evt_nomal_cange);
static USER_FUNC(bgm_start_wait);

EvtWord slit_majin_lecture_init[26];
EvtWord slit_majin_lecture[390];

/**
 * Linear blend used by lect_mario_ido_sum: returns (total - elapsed) / total of end plus elapsed / total of
 * start, so the value moves from start toward end as elapsed grows.
 */
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start) {
    f32 rem = total - elapsed;

    return (rem * end + elapsed * start) / total;
}

/**
 * Script helper: moves Mario in a straight line over args[0] milliseconds from
 * where he stands to the point in LW(3), LW(4), LW(5), saving his starting point in LW(0) to LW(2).
 * Blocks until the time is up.
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
 * current alpha; icons 075 to 07D (the stick direction icons) are drawn 15 units higher.
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
    dispEntry(CAMERA_2D, 0.0f, 1, lecture_button_disp, NULL);
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
// Despite its name, this query returns the sequence state, not the icon ID.
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
 * zero moves it back down (effitemget_cam_move). Nothing in this file calls it; the lecture copies in eki, gor,
 * hei and mri lift the camera and Mario 2016 units, capture the screen into an image for the paper-seal
 * transition one frame later, and move both back.
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
 * Script helper: turns on hit-group flag 1 for the hit objects of the map object named by args[0] ("me" means
 * the object running the script), which the hit checks skip, so the object loses its collision (nasi = none).
 * Nothing in this file calls it.
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
 * Script helper: stores the vector (args[0], args[1], args[2]) in daiv,
 * a lecture-private position that the lecture later reads back with dai_moveti_get (used to remember where Mario
 * stood).
 */
static USER_FUNC(dai_moveti_set) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    daiv = (Vec){x, y, z};
    return EVT_RETURN_DONE;
}

/** Script helper: reads the position saved by dai_moveti_set into the script variables args[0], args[1], args[2]. */
static USER_FUNC(dai_moveti_get) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], daiv.x);
    evtSetFloat(event, args[1], daiv.y);
    evtSetFloat(event, args[2], daiv.z);
    return EVT_RETURN_DONE;
}

/**
 * Script helper: writes TRUE into args[0] while Mario is in paper mode (MARIO_FLAG_PAPER_MODE, the thin sideways
 * form), otherwise FALSE.
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
 * Script helper: flashes the screen white twice:
 * fades out to white over 40 frames, holds 60 ms, fades back in over 40 frames and holds 60 ms.
 * Used for the genie's light effect.
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

/** Script helper: writes TRUE into args[0] while controller 0 holds the R trigger, otherwise FALSE. */
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
 * Script helper: puts Mario back to his normal form at the end of the lesson:
 * cancels a roll and, if he is in paper mode, cancels the slit motion;
 * blocks until the roll is over and the slit display flag is clear.
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


/** Script helper: blocks until the lecture music (BGM channel 1) has actually started playing. */
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/**
 * Sets up the camera and Mario's facing for the paper mode lesson:
 * system level 0, a fixed lecture camera shot, Mario facing right.
 */
EVT_BEGIN(slit_majin_lecture_init)
    CALL(lect_set_systemlevel, 0)
    CALL(lect_test_systemlevel)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(0.0f), FLOAT(42.0f), FLOAT(-101.799805f), 1000, EASING_LINEAR)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

EVT_BEGIN(slit_majin_lecture)
    // Set up the lesson: the button prompt icon appears beside Mario and the genie floats in front of the
    // camera.
    BEGIN_SIBLING_EVENT
        CALL(lecture_button_rtn)
    END_SIBLING_EVENT
    WAIT_FRAMES(1)
    CALL(lecture_button_set_pos, 0, FLOAT(-200.0f), FLOAT(-25.0f), FLOAT(0.0f))
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
    // The genie laughs at Mario for falling for the trap and explains the castle's curse.
    CALL(evt_npc_set_anim, STRING("ペラ魔人"), STRING("P_1"))
    WAIT_MS(200)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_12_12"), 0, STRING("ペラ魔人"))
    // Mario is dazzled by flashes of light.
    CALL(evt_mario_set_pose, STRING("M_N_H"))
    LOOP(4)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_PERAMAJIN_LIGHT1"), LW(0), LW(1), LW(2), 0)
        CALL(evtflash)
    END_LOOP
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    // The genie announces the paper curse and tells Mario to hold the R button.
    CALL(evt_msg_print, 0, STRING("stg1_gon_12_13"), 0, STRING("ペラ魔人"))
#if VERSION <= VERSION_DEMO_US
CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_12_14"), 0, STRING("ペラ魔人"))
#else
    CALL(evt_msg_print_add, 0, STRING("stg1_gon_12_14"))
    // Practice loop: the prompt icon blinks while Mario tries to turn sideways.
#endif
    LOOP(0)
        LOOP(0)
            CALL(lecture_button_flag_on_off, 0, 2, 1)
            CALL(lecture_button_set_icon_id, 0, ICON_TRIGGER_R)
            CALL(lecture_button_set_seq, 0, 2)
            CALL(lecture_button_seq_wait, 0, 1)
            CALL(x_flag_set, 0, 1)
            BEGIN_INLINE_EVENT
                LOOP(0)
                    CALL(lecture_button_set_icon_id, 0, ICON_TRIGGER_R_PRESSED)
                    WAIT_MS(300)
                    CALL(x_flag_get, 0, LW(0))
                    IF_INT_EQUAL(LW(0), 2)
                        BREAK_LOOP
                    END_IF
                    CALL(lecture_button_set_icon_id, 0, ICON_TRIGGER_R)
                    WAIT_MS(300)
                    CALL(x_flag_get, 0, LW(0))
                    IF_INT_EQUAL(LW(0), 2)
                        BREAK_LOOP
                    END_IF
                END_LOOP
            END_INLINE_EVENT
            CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
            SET_VALUE(LW(7), 0)
            CALL(evt_cam_letter_box_disable, 1)
            CALL(mainwindow_opendisable)
            CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_DISABLE_STICK)
            CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
            CALL(evt_mario_set_enable_key, 32)
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_mario_key_onoff, 1)
            // Mario is moved only by the check; the lesson is passed once he stays paper-thin for 180 frames.
            LOOP(0)
                CALL(lect_status_off)
                CALL(evt_mario_get_pos, 0, LW(5), LW(1), LW(6))
                CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
                WAIT_FRAMES(1)
                CALL(lect_slit_check, LW(3))
                IF_INT_EQUAL(LW(7), 0)
                    IF_INT_EQUAL(LW(3), 1)
                        ADD(LW(7), 1)
                        SET_VALUE(LW(11), 0)
                    END_IF
                    ELSE
                    IF_INT_EQUAL(LW(7), 1)
                        IF_INT_EQUAL(LW(3), 0)
                            SET_VALUE(LW(11), 0)
                            BREAK_LOOP
                        END_IF
                        ADD(LW(11), 1)
                        IF_INT_GREATER_OR_EQUAL(LW(11), 180)
                            SET_VALUE(LW(11), 1)
                            BREAK_LOOP
                        END_IF
                    END_IF
                END_IF
            END_LOOP
#if VERSION > VERSION_DEMO_US
            CALL(evt_msg_continue)
            // Control is returned to normal and the prompt is dismissed.
#endif
            CALL(evt_mario_key_onoff, 0)
            CALL(evt_mario_key_onoff, 0)
            CALL(lect_status_on)
            CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_DISABLE_STICK)
            CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
            CALL(evt_mario_set_enable_key, 0)
            CALL(mainwindow_openenable)
            CALL(x_flag_set, 0, 2)
            CALL(evt_cam_letter_box_disable, 0)
            CALL(lecture_button_set_seq, 0, 3)
            CALL(evt_mario_normalize)
            IF_INT_EQUAL(LW(11), 1)
                LOOP(0)
                    WAIT_FRAMES(1)
                    CALL(lect_slit_check, LW(3))
                    IF_INT_EQUAL(LW(3), 0)
                        BREAK_LOOP
                    END_IF
                END_LOOP
            END_IF
            CALL(evt_mario_adjust_dir)
            CALL(evt_mario_set_pose, STRING("M_S_1"))
            CALL(lecture_button_seq_wait, 0, 1)
            WAIT_MS(300)
            // If Mario failed to hold paper mode, the genie scolds him and he tries again.
            IF_INT_EQUAL(LW(11), 1)
                BREAK_LOOP
                ELSE
#if VERSION <= VERSION_DEMO_US
CALL(evt_msg_continue)
#endif
                CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
                CALL(evt_msg_print, 0, STRING("stg1_gon_12_15_1"), 0, STRING("ペラ魔人"))
            END_IF
        END_LOOP
#if VERSION <= VERSION_DEMO_US
CALL(evt_msg_print_add, 0, STRING("stg1_gon_12_15"))
#else
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg1_gon_12_15"), 0, STRING("ペラ魔人"))
        // The genie asks whether Mario understands the curse.
#endif
        CALL(evt_msg_select, 0, STRING("stg1_gon_12_15_yn"))
#if VERSION <= VERSION_DEMO_US
CALL(evt_msg_continue)
#else
        // No: the genie scolds him and the lesson repeats.
#endif
        IF_INT_EQUAL(LW(0), 0)
#if VERSION <= VERSION_DEMO_US
CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
            CALL(evt_msg_print, 0, STRING("stg1_gon_12_16"), 0, STRING("ペラ魔人"))
#else
            CALL(evt_msg_print_add, 0, STRING("stg1_gon_12_16"))
#endif
            ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
#if VERSION <= VERSION_DEMO_US
CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg1_gon_12_17"), 0, STRING("ペラ魔人"))
#else
    CALL(evt_msg_print_add, 0, STRING("stg1_gon_12_17"))
    // The genie says farewell and disappears.
#endif
    CALL(evt_npc_set_anim, STRING("ペラ魔人"), STRING("P_2"))
    WAIT_MS(200)
    CALL(evt_npc_set_camid, STRING("ペラ魔人"), 4)
    CALL(evt_npc_set_position, STRING("ペラ魔人"), 0, -1000, 0)
    RETURN
EVT_END()

