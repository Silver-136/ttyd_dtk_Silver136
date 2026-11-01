// Partner lecture in Twilight Town: the tutorial scenes that teach a new partner's field move (usu.rel's copy of the
// lecture code).

#include "version.h"
#include "evt/evt_window.h"
#include "eff/eff_mario_balloon.h"
#include "eff/eff_fukidashi.h"
#include "eff/eff_itemget.h"
#include "icon_ids.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "pmario_sound.h"
#include "statuswindow.h"
#include "driver/hitdrv.h"
#include "evt/evt_mario.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_img.h"
#include "evt/evt_msg.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_lecture.h"
#include "mot/mot_slit.h"
#include "mot/mot_roll.h"
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
#include "driver/fadedrv.h"
#include "system.h"
#include <dolphin/pad.h>
#include "evt/evt_lect.h"

static OSTime wwstart;
static s32 x_flag_data[2];
static void lecture_button_disp(CameraId camera, void* param);
static LectButton lbtn[2];
/**
 * Moves the 3D camera by the offset ofs in one step (0 frames, curve EASING_SINE_IN_OUT),
 * keeping the current position as its preset; used to shift the camera 2016 units above or below the scene.
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
static Vec daiv;

static USER_FUNC(efffukidashichange_cam);
static USER_FUNC(effitemget_busaiku);
static USER_FUNC(effitemget_open);
static USER_FUNC(effitemgetchange_cam);
static USER_FUNC(evt_nomal_cange);
static USER_FUNC(evtflash);
static USER_FUNC(lect_itemget_kesan);
static USER_FUNC(lect_party_ido_sum);
static USER_FUNC(lect_push_r_check);
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
static USER_FUNC(bgm_start_wait);
static USER_FUNC(lect_slit_check);
static USER_FUNC(dai_moveti_get);
static USER_FUNC(dai_moveti_set);
static f32 lect_party_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);
static USER_FUNC(lect_npc_balloon_on_off);
static USER_FUNC(lect_mobj_hit_nasi);
static USER_FUNC(effitemget_open_ptr);
static USER_FUNC(effitemgetchange_cam_ptr);
static USER_FUNC(lect_map_cam_chg);
static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings);
static USER_FUNC(lecture_button_get_icon_id);
static USER_FUNC(x_flag_wait);
static USER_FUNC(x_flag_get);
static USER_FUNC(x_flag_add);
static USER_FUNC(x_flag_set);
static USER_FUNC(lect_mario_ido_sum);
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);

EvtWord vivian_party_lecture_init[43];
EvtWord vivian_party_lecture[VERSION_PICK(196, 198)];
static EvtWord all_party_lecture_otosub[11];
EvtWord all_party_lecture[474];
EvtWord roll_majin_lecture_init[26];
EvtWord roll_majin_lecture_sub01[29];
EvtWord roll_majin_lecture_sub02[65];
#if VERSION <= VERSION_DEMO_US
extern EvtWord roll_majin_lecture[];
#else
EvtWord roll_majin_lecture[372];
#endif

/**
 * Linear interpolation used by the Mario move helper:
 * weights the end value by the time remaining and the start value by the time elapsed, out of total.
 */
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start) {
    f32 rem = total - elapsed;

    return (rem * end + elapsed * start) / total;
}

/**
 * Script helper: slides Mario from his current position to (LW(3), LW(4), LW(5)) over args[0] milliseconds;
 * the start position is stored in LW(0..2) on the first call. Blocks until done.
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

/** Script helper: force-closes the status window (HP/FP/coins display) so it stays out of the lecture scene. */
static USER_FUNC(lect_status_off) {
    statusWinForceClose();
    return EVT_RETURN_DONE;
}

/** Script helper: lets the status window show again after lect_status_off. */
static USER_FUNC(lect_status_on) {
    statusWinForceCloseClear();
    return EVT_RETURN_DONE;
}

/**
 * Script helper: sets lecture flag number args[0] (0 or 1) to args[1];
 * the flags let the lecture's parallel events signal each other.
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

/** Script helper: reads lecture flag number args[0] into the script variable args[1]. */
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
 * Script helper that runs the lecture button prompts every frame:
 * resets both buttons on the first call, fades each in
 * or out by 7 alpha per frame according to its sequence number and draws them;
 * finishes when both reach sequence 4 (hidden).
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

/** Script helper: sets the icon of lecture button args[0] to icon id args[1]. */
static USER_FUNC(lecture_button_set_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->iconId = value;
    return EVT_RETURN_DONE;
}

/** Script helper: sets the alpha (0..255) of lecture button args[0] to args[1]. */
static USER_FUNC(lecture_button_set_alpha) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->alpha = value;
    return EVT_RETURN_DONE;
}

/** Script helper: sets or clears (args[2]) the flag bits args[1] on lecture button args[0]. */
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

/** Script helper: sets the sequence number of lecture button args[0] to args[1] (2 fades in, 3 fades out, 4 hides). */
static USER_FUNC(lecture_button_set_seq) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->seq = value;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: reads a lecture button's sequence number into args[1].
 * The name says icon id but it returns the sequence number.
 */
static USER_FUNC(lecture_button_get_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    LectButton* btn = &lbtn[index];

    evtSetValue(event, args[1], btn->seq);
    return EVT_RETURN_DONE;
}

/** Script helper: blocks until lecture button args[0] reaches sequence number args[1]. */
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

/** Script helper: moves lecture button args[0] to the position (args[1], args[2], args[3]). */
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

/** Recursively assigns the camera id to a map object, its children and, below the first level, its siblings. */
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
 * Script helper: moves the named map object (args[0]) and everything attached to it onto camera id args[1],
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

/** Script helper: changes which camera the named "itemget" effect draws on (args[0] effect name, args[1] camera id). */
static USER_FUNC(effitemgetchange_cam) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    ((ItemGetWork*)effNameToPtr(name)->userdata)->camera = camId;
    return EVT_RETURN_DONE;
}

/** Script helper: same as effitemgetchange_cam but takes the effect pointer in args[0] instead of a name. */
static USER_FUNC(effitemgetchange_cam_ptr) {
    EvtWord* args = event->args;
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, args[0]);

    ((ItemGetWork*)eff->userdata)->camera = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

/** Script helper: opens the named "itemget" background effect (args[0]) by setting its state to 1. */
static USER_FUNC(effitemget_open) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/** Script helper: same as effitemget_open but takes the effect pointer in args[0]. */
static USER_FUNC(effitemget_open_ptr) {
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, event->args[0]);

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: moves the 3D camera 2016 units up (args[0] non-zero) or down;
 * hides and brings back the scene around the partner-lecture overlay.
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
 * Script helper: turns on the collision groups of the map object named args[0] ("me" means the running event's
 * own object) so they can be hit.
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
 * Script helper: computes a point args[1] units from the camera along the line to the partner's centre,
 * and returns it in args[2..4]; the "itemget" effect is placed there.
 * (0 uses the active partner, else partner 1.)
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

/** Script helper: shows (args[1] non-zero) or removes the speech balloon effect of the named NPC, drawn on camera 7. */
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

/** Script helper: changes which camera the named exclamation-bubble effect (fukidashi) draws on. */
static USER_FUNC(efffukidashichange_cam) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    ((EffFukidashiWork*)effNameToPtr(name)->userdata)->camera = camId;
    return EVT_RETURN_DONE;
}

/** Linear interpolation used by the partner move helper, same formula as lect_mario_ido_sub. */
static f32 lect_party_ido_sub(f32 elapsed, f32 total, f32 end, f32 start) {
    f32 rem = total - elapsed;

    return (rem * end + elapsed * start) / total;
}

/**
 * Script helper: slides the partner from its current position to (LW(0), LW(1), LW(2)) over args[1]
 * milliseconds; the start position is stored in LW(3..5).
 */
static USER_FUNC(lect_party_ido_sum) {
    EvtWord* args = event->args;
    PartyEntry* party = partyGetPtr(args[0] == 0 ? partyCtrlNo : 1);
    s32 frames = evtGetValue(event, args[1]);
    s32 elapsed;

    if (isFirstCall) {
        wwstart = event->lifetime;
        evtSetFloat(event, LW(3), party->position.x);
        evtSetFloat(event, LW(4), party->position.y);
        evtSetFloat(event, LW(5), party->position.z);
        return EVT_RETURN_BLOCK;
    }
    elapsed = OSTicksToMilliseconds((u64)OSDiffTick(event->lifetime, wwstart));
    if (elapsed > frames) {
        elapsed = frames;
    }
    party->position.x = lect_party_ido_sub(elapsed, frames, evtGetFloat(event, LW(3)), evtGetFloat(event, LW(0)));
    party->position.y = lect_party_ido_sub(elapsed, frames, evtGetFloat(event, LW(4)), evtGetFloat(event, LW(1)));
    party->position.z = lect_party_ido_sub(elapsed, frames, evtGetFloat(event, LW(5)), evtGetFloat(event, LW(2)));
    if (elapsed >= frames) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(dai_moveti_set) {
    EvtWord* args = event->args;
    f32 x = evtGetFloat(event, args[0]);
    f32 y = evtGetFloat(event, args[1]);
    f32 z = evtGetFloat(event, args[2]);

    daiv = (Vec){x, y, z};
    return EVT_RETURN_DONE;
}

/**
 * Script helper: reads back the vector stored by dai_moveti_set into args[0..2]. Nothing in this file calls it.
 */
static USER_FUNC(dai_moveti_get) {
    EvtWord* args = event->args;

    evtSetFloat(event, args[0], daiv.x);
    evtSetFloat(event, args[1], daiv.y);
    evtSetFloat(event, args[2], daiv.z);
    return EVT_RETURN_DONE;
}

/** Script helper: returns TRUE in args[0] if Mario is currently in paper mode (MARIO_FLAG_PAPER_MODE), else FALSE. */
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
 * Script helper that plays a white flash: fades the screen out to white over 40 frames, holds it for 60 frames,
 * then fades back in; blocks until finished.
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
 * Script helper: returns TRUE in args[0] while the R trigger is held on controller 0, else FALSE;
 * the roll lecture waits on it.
 */
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
 * Script helper that returns Mario to normal after the roll/paper-mode practice:
 * cancels a roll or slit in progress and blocks until the slit display flag and roll key lock are gone.
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

/**
 * Script helper: turns the partner to face right
 * or left according to args[0] (above 180 faces left, otherwise right).
 */
static USER_FUNC(set_dir) {
    s32 dir = evtGetValue(event, event->args[0]);
    PartyEntry* party = partyGetPtr(PARTY_SLOT_PARTY);

    if (party != NULL) {
        s32 angle = dir > 180 ? 270 : 90;
        party->displayDirectionTarget = party->displayDirection = 270 - angle;
    }
    return EVT_RETURN_DONE;
}


/** Script helper: blocks until background music channel 1 has started. */
static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/**
 * Vivian lecture setup: faces Mario east, frames him and the incoming partner with a 1-second camera move,
 * and slides the partner 1 second into the shot, facing east.
 */
EVT_BEGIN(vivian_party_lecture_init)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 40)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.7998f), FLOAT(0.0f), FLOAT(30.0f), FLOAT(-101.799805f), 1000, EASING_LINEAR)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(lect_party_ido_sum, 0, 1000)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_set_dir, 0, 90, 300)
    RETURN
EVT_END()

/**
 * Vivian's tutorial after she joins (system messages lecture_friends_12_0_1, 12_1, 13 and 14):
 * the primer says X makes her pull Mario into the shadows to hide;
 * the camera moves in and Vivian holds Mario in the shadow (evt_party_vivian_hold)
 * while a Boo (テレサ) floats in from the right with a speech bubble, looks around twice and floats away,
 * unable to find them.
 * Then the message says X brings them back out, Vivian lets go, moves beside Mario,
 * and the last message explains Shade Fist (sets enemies on fire) and Veil (hide to avoid damage)
 * before she strikes a pose (PTR_O_1).
 */
EVT_BEGIN(vivian_party_lecture)
    CALL(evt_msg_print, 0, STRING("lecture_friends_12_0_1"), 0, 0)
    CALL(evt_msg_print, 0, STRING("lecture_friends_12_1"), 0, 0)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(105.0f), FLOAT(597.0f), FLOAT(0.0f), FLOAT(63.0f), FLOAT(-101.799805f), 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)
    CALL(evt_cam_letter_box_disable, 1)
    CALL(evt_party_vivian_hold, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 200)
    ADD(LW(1), 50)
    CALL(evt_npc_start_for_one_event, STRING("テレサ"))
    CALL(evt_npc_flag_onoff, 1, STRING("テレサ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("テレサ"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_set_position, STRING("テレサ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_camid, STRING("テレサ"), 7)
    CALL(evt_npc_set_ry, STRING("テレサ"), 270)
    CALL(evt_npc_get_position, STRING("テレサ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 200)
    CALL(evt_npc_move_position, STRING("テレサ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_NPC, STRING("fuki"), STRING("テレサ"), 1, 0, 0, 0, 0, 0, 0, 60)
    CALL(efffukidashichange_cam, STRING("fuki"), 7)
    WAIT_MS(1000)
    LOOP(2)
        CALL(evt_npc_reverse_ry, STRING("テレサ"))
        WAIT_MS(800)
    END_LOOP
    SUBTRACT(LW(0), 200)
    CALL(evt_npc_move_position, STRING("テレサ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_position, STRING("テレサ"), 0, -1000, 0)
    CALL(evt_npc_stop_for_one_event, STRING("テレサ"))
    CALL(evt_msg_print_add, 0, STRING("lecture_friends_13"))
    CALL(evt_party_vivian_unhold, 0)
    CALL(evt_cam_letter_box_disable, 0)
#if VERSION >= VERSION_US
    CALL(evt_party_stop, 0)
    WAIT_FRAMES(16)
    CALL(evt_mario_wait_landon)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.7998f), FLOAT(0.0f), FLOAT(30.0f), FLOAT(-101.799805f), 1000, EASING_LINEAR)
#else
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.7998f), FLOAT(0.0f), FLOAT(30.0f), FLOAT(-101.799805f), 1000, EASING_LINEAR)
    CALL(evt_party_stop, 0)
    WAIT_FRAMES(16)
#endif
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 40)
    CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(80.0f))
    CALL(evt_party_set_dir, 0, 90, 300)
    WAIT_MS(1000)
    CALL(evt_msg_print_add, 0, STRING("lecture_friends_14"))
    CALL(evt_party_set_pose, 0, STRING("PTR_O_1"))
    WAIT_MS(1000)
    CALL(evt_party_set_pose, 0, STRING("PTR_S_1"))
    RETURN
EVT_END()

/**
 * Helper run alongside all_party_lecture: holds the field BGM
 * and plays the "BGM_FF_GET_PARTY1" partner-joined jingle on channel 1.
 */
static EVT_BEGIN(all_party_lecture_otosub)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_GET_PARTY1"))
    WAIT_MS(2000)
    RETURN
EVT_END()

/**
 * Shared "partner joins" cutscene run by each partner's pickup scene (LW(8) the partner NPC, LW(9) the partner
 * id): the partner joins the party with a fanfare,
 * an "itemget" background appears with that partner's joining message, the partner's lesson runs
 * (this map area's copy has only Vivian's) and the game is restored.
 */
EVT_BEGIN(all_party_lecture)
    // The partner NPC steps out of the scene and joins the party at its spot.
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
    // The joining jingle plays over the item-get backdrop with this partner's "has joined your party!" line.
    CALL(evt_party_set_camid, 0, 7)
    SPAWN_EVENT_GET_ID(PTR(all_party_lecture_otosub), LW(11))
    CALL(lect_itemget_kesan, 0, FLOAT(600.0f), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("sub_bg"), STRING("itemget"), 1, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(effitemgetchange_cam, STRING("sub_bg"), 5)
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
    // Stop the jingle, then fold the scene into the lecture frame with the lecture music.
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
    // The lesson: Mario blinks out of sight, Vivian's lesson plays, then Mario and the partner blink back in.
    CALL(lect_test_systemlevel)
    SWITCH(LW(9))
        CASE_EQUAL(6)
        CALL_CHILD(PTR(vivian_party_lecture_init))
    END_SWITCH
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
    SWITCH(LW(9))
        CASE_EQUAL(6)
        CALL_CHILD(PTR(vivian_party_lecture))
    END_SWITCH
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
    // Back to the normal camera, music and sounds; the lecture frame folds away.
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_HIDE)
    CALL(evt_party_dispflg_onoff, 0, PARTY_DISP_FLAG_HIDE, 1)
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
 * Setup of the Tube Curse lecture, run by usu_01's black-chest scene (evt_majin2) before roll_majin_lecture:
 * sets the system level to 0 (lect_set_systemlevel),
 * moves the event camera into place over 1 second and turns Mario to face right.
 */
EVT_BEGIN(roll_majin_lecture_init)
    CALL(lect_set_systemlevel, 0)
    CALL(lect_test_systemlevel)
    CALL(evt_cam3d_evt_set_rel_dir, FLOAT(0.0f), FLOAT(98.0f), FLOAT(558.0f), FLOAT(0.0f), FLOAT(42.0f), FLOAT(-101.799805f), 1000, EASING_LINEAR)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_FRAMES(1)
    RETURN
EVT_END()

EVT_BEGIN(roll_majin_lecture_sub01)
    CALL(lecture_button_set_alpha, 0, 255)
    CALL(lecture_button_set_alpha, 1, 128)
    CALL(lecture_button_set_icon_id, 1, ICON_STICK_NEUTRAL)
    LOOP(0)
        CALL(lecture_button_set_icon_id, 0, ICON_TRIGGER_R_PRESSED)
        WAIT_MS(300)
        CALL(lecture_button_set_icon_id, 0, ICON_TRIGGER_R)
        WAIT_MS(300)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(roll_majin_lecture_sub02)
    CALL(lecture_button_set_alpha, 0, 128)
    CALL(lecture_button_set_alpha, 1, 255)
    CALL(lecture_button_set_icon_id, 0, ICON_TRIGGER_R_PRESSED)
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
 * The Tube Curse lecture after Mario opens Twilight Town's black chest:
 * the chest spirit (ペラ魔人) appears by the camera
 * and curses him (stg4_usu_majin3_12 to 14, flashing light on Mario),
 * telling him to hold R and rotate the stick.
 * Mario then practices with the status bar and main menu off, only R and the stick enabled,
 * and R and stick prompts (roll_majin_lecture_sub01 and sub02) until he rolls into a tube (motion 22).
 * The spirit gloats (stg4_usu_majin3_18) and asks whether he got it (stg4_usu_majin3_19);
 * "I don't get it" repeats the practice, and once he understands the spirit signs off (20_1 and 21)
 * and disappears.
 */
EVT_BEGIN(roll_majin_lecture)
    // The chest spirit appears at the camera's focus and follows it.
    BEGIN_SIBLING_EVENT
        CALL(lecture_button_rtn)
    END_SIBLING_EVENT
    WAIT_FRAMES(1)
    CALL(lecture_button_set_pos, 0, FLOAT(-200.0f), FLOAT(-25.0f), FLOAT(0.0f))
    CALL(lecture_button_set_pos, 1, FLOAT(-140.0f), FLOAT(-25.0f), FLOAT(0.0f))
    CALL(evt_npc_flag_onoff, 0, STRING("ペラ魔人"), NPC_FLAG_HIDE)
    CALL(evt_cam_get_at, CAMERA_3D, LW(0), LW(1), LW(2))
    SUBTRACT(LW(1), 25)
    CALL(evt_npc_set_position, STRING("ペラ魔人"), LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("ペラ魔人"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 1, STRING("ペラ魔人"), NPC_FLAG_NO_HIT | NPC_FLAG_NO_WALL_COLLISION)
    WAIT_FRAMES(1)
    BEGIN_SIBLING_EVENT
        LOOP(0)
            CALL(evt_cam_get_at, CAMERA_3D, LW(0), LW(1), LW(2))
            SUBTRACT(LW(1), 25)
            CALL(evt_npc_set_position, STRING("ペラ魔人"), LW(0), LW(1), LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    CALL(evt_npc_set_camid, STRING("ペラ魔人"), 7)
    CALL(evt_npc_set_anim, STRING("ペラ魔人"), STRING("P_1"))
    WAIT_MS(200)
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_12"), 0, STRING("ペラ魔人"))
    CALL(evt_mario_set_pose, STRING("M_N_H"))
    LOOP(4)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_PERAMAJIN_LIGHT1"), LW(0), LW(1), LW(2), 0)
        CALL(evtflash)
    END_LOOP
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_13"), 0, STRING("ペラ魔人"))
#if VERSION <= VERSION_DEMO_US
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_14"), 0, STRING("ペラ魔人"))
#else
    CALL(evt_msg_print_add, 0, STRING("stg4_usu_majin3_14"))
#endif
    // Practice: only R and the stick work, with button prompts, until Mario rolls into a tube.
    LOOP(0)
        CALL(lecture_button_flag_on_off, 0, 2, 1)
        CALL(lecture_button_set_icon_id, 0, ICON_TRIGGER_R)
        CALL(lecture_button_set_seq, 0, 2)
        CALL(lecture_button_flag_on_off, 1, 2, 1)
        CALL(lecture_button_set_icon_id, 1, ICON_STICK_NEUTRAL)
        CALL(lecture_button_set_seq, 1, 2)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_seq_wait, 1, 1)
        SPAWN_EVENT_GET_ID(PTR(roll_majin_lecture_sub01), LW(11))
        SET_VALUE(LW(7), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SET_VALUE(LW(7), 0)
        CALL(evt_cam_letter_box_disable, 1)
        CALL(mainwindow_opendisable)
        CALL(evt_mario_trigflag_onoff, 1, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, PAD_TRIGGER_R)
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_mario_key_onoff, 1)
        LOOP(0)
            CALL(lect_status_off)
            CALL(evt_mario_get_pos, 0, LW(5), LW(1), LW(6))
            CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
            CALL(evt_mario_get_motion, LW(3))
            IF_INT_EQUAL(LW(3), MARIO_MOTION_ROLL)
                BREAK_LOOP
            END_IF
            IF_INT_EQUAL(LW(7), 0)
                CALL(lect_push_r_check, LW(7))
                IF_INT_NOT_EQUAL(LW(7), 0)
                    DELETE_EVENT(LW(11))
                    SPAWN_EVENT_GET_ID(PTR(roll_majin_lecture_sub02), LW(11))
                END_IF
            ELSE
                IF_INT_EQUAL(LW(7), 1)
                    CALL(lect_push_r_check, LW(7))
                    IF_INT_EQUAL(LW(7), 0)
                        DELETE_EVENT(LW(11))
                        SPAWN_EVENT_GET_ID(PTR(roll_majin_lecture_sub01), LW(11))
                    END_IF
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
#if VERSION > VERSION_DEMO_US
        CALL(evt_msg_continue)
#endif
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(lect_status_on)
        CALL(evt_mario_trigflag_onoff, 0, MARIO_TRIG_FLAG_DISABLE_SUB_STICK)
        CALL(evt_mario_set_enable_key, 0)
        CALL(mainwindow_openenable)
        CALL(evt_cam_letter_box_disable, 0)
        DELETE_EVENT(LW(11))
        CALL(lecture_button_set_seq, 0, 3)
        CALL(lecture_button_set_seq, 1, 3)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_seq_wait, 1, 1)
        WAIT_MS(300)
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_print_add, 0, STRING("stg4_usu_majin3_18"))
#else
        CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_18"), 0, STRING("ペラ魔人"))
#endif
        CALL(evt_msg_select, 0, STRING("stg4_usu_majin3_19"))
#if VERSION <= VERSION_DEMO_US
        CALL(evt_msg_continue)
#endif
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_nomal_cange)
            CALL(evt_msg_toge, MSG_TOGE_NONE, 0, 0, 0)
#if VERSION <= VERSION_DEMO_US
            CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_20"), 0, STRING("ペラ魔人"))
#else
            CALL(evt_msg_print_add, 0, STRING("stg4_usu_majin3_20"))
#endif
        ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
#if VERSION <= VERSION_DEMO_US
    CALL(evt_msg_toge, 1, 0, 0, 0)
    CALL(evt_msg_print, 0, STRING("stg4_usu_majin3_20_1"), 0, STRING("ペラ魔人"))
#else
    CALL(evt_msg_print_add, 0, STRING("stg4_usu_majin3_20_1"))
#endif
    CALL(evt_nomal_cange)
    CALL(evt_msg_print_add, 0, STRING("stg4_usu_majin3_21"))
    CALL(evt_npc_set_anim, STRING("ペラ魔人"), STRING("P_2"))
    WAIT_MS(200)
    CALL(evt_npc_set_camid, STRING("ペラ魔人"), 4)
    CALL(evt_npc_set_position, STRING("ペラ魔人"), 0, -1000, 0)
    CALL(evt_npc_flag_onoff, 1, STRING("ペラ魔人"), NPC_FLAG_HIDE)
    RETURN
EVT_END()

