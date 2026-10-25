#include "icon_ids.h"
#include "eff/eff_mario_balloon.h"
#include "eff/eff_fukidashi.h"
#include "eff/eff_itemget.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "mario/mario_motion.h"
#include "pmario_sound.h"
#include "statuswindow.h"

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

static OSTime wwstart;
static s32 x_flag_data[2];
static void lecture_button_disp(CameraId camera, void* param);
typedef struct LectButtonDisp { s32 unk0; Vec pos; } LectButtonDisp;
typedef struct LectButton { u8 flags; u8 seq; u8 pad_2[0x8 - 0x2]; LectButtonDisp disp; u8 pad_18[0x40 - 0x18]; s16 iconId; s16 alpha; } LectButton;
static LectButton lbtn[2];
static inline void effitemget_cam_move(CameraEntry* camera, Vec ofs) { camera->presetPos = camera->cameraPos; camera->presetAt = camera->cameraAt; PSVECAdd(&camera->cameraPos, &ofs, &camera->storedPos); PSVECAdd(&camera->cameraAt, &ofs, &camera->storedAt); camera->startTimeA = gp->renderTime; camera->durationA = 0; camera->stateA = 3; camera->curveA = 11; }
void hitGrpFlagOn(const char* name, u32 flags);
EffectEntry* effMarioBalloonEntry(s32 type, NpcEntry* npc);
static EffectEntry* eff;

static USER_FUNC(effitemget_busaiku);
static USER_FUNC(effitemget_open);
static USER_FUNC(effitemgetchange_cam);
USER_FUNC(evt_cam3d_evt_set_rel);
USER_FUNC(evt_cam3d_evt_set_rel_dir);
USER_FUNC(evt_cam_letter_box_disable);
USER_FUNC(evt_cam_shake);
USER_FUNC(evt_eff);
USER_FUNC(evt_eff_softdelete);
USER_FUNC(evt_img_alloc_capture);
USER_FUNC(evt_img_clear_virtual_point);
USER_FUNC(evt_img_entry);
USER_FUNC(evt_img_free_capture);
USER_FUNC(evt_img_onoff);
USER_FUNC(evt_img_release);
USER_FUNC(evt_img_set_paper);
USER_FUNC(evt_img_set_paper_anim);
USER_FUNC(evt_img_wait_animend);
USER_FUNC(evt_mario_adjust_dir);
USER_FUNC(evt_mario_cont_onoff);
USER_FUNC(evt_mario_dispflag_onoff);
USER_FUNC(evt_mario_get_motion);
USER_FUNC(evt_mario_get_pos);
USER_FUNC(evt_mario_goodbye_party);
USER_FUNC(evt_mario_init_camid);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_kill_party);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_normalize);
USER_FUNC(evt_mario_set_camid);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_enable_key);
USER_FUNC(evt_mario_set_party_pos);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mario_set_pose);
USER_FUNC(evt_mario_trigflag_onoff);
USER_FUNC(evt_mobj_delete);
USER_FUNC(evt_mobj_entry);
USER_FUNC(evt_mobj_flag_onoff);
USER_FUNC(evt_mobj_get_position);
USER_FUNC(evt_mobj_set_camid);
USER_FUNC(evt_mobj_set_gravity_bound);
USER_FUNC(evt_mobj_set_position);
USER_FUNC(evt_msg_continue);
USER_FUNC(evt_msg_print);
USER_FUNC(evt_msg_print_add);
USER_FUNC(evt_msg_print_insert);
USER_FUNC(evt_msg_select);
USER_FUNC(evt_msg_toge);
USER_FUNC(evt_npc_flag_onoff);
USER_FUNC(evt_npc_get_dir);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_set_anim);
USER_FUNC(evt_npc_set_autotalkpose);
USER_FUNC(evt_npc_set_camid);
USER_FUNC(evt_npc_set_position);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_start_for_event);
USER_FUNC(evt_npc_stop_for_event);
USER_FUNC(evt_paper_delete);
USER_FUNC(evt_paper_entry);
USER_FUNC(evt_party_cont_onoff);
USER_FUNC(evt_party_dispflg_onoff);
USER_FUNC(evt_party_get_pos);
USER_FUNC(evt_party_init_camid);
USER_FUNC(evt_party_move_pos2);
USER_FUNC(evt_party_set_camid);
USER_FUNC(evt_party_set_dir);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_set_pose);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_party_yoshi_fly);
USER_FUNC(evt_party_yoshi_ride);
USER_FUNC(evt_pouch_get_yoshiname);
USER_FUNC(evt_pouch_party_join);
USER_FUNC(evt_snd_bgmoff);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envoff);
USER_FUNC(evt_snd_envon);
USER_FUNC(evt_snd_sfxoff);
USER_FUNC(evt_snd_sfxon);
USER_FUNC(evt_snd_sfxon_3d);
USER_FUNC(evt_sub_random);
USER_FUNC(lect_cam_load);
USER_FUNC(lect_cam_save);
static USER_FUNC(lect_itemget_kesan);
static USER_FUNC(lect_mario_ido_sum);
static USER_FUNC(lect_mobj_hit_nasi);
static USER_FUNC(lect_mobj_set_nosysblend);
static USER_FUNC(lect_party_ido_sum);
static USER_FUNC(lect_push_b_check);
static USER_FUNC(lect_status_off);
static USER_FUNC(lect_status_on);
USER_FUNC(lect_test_systemlevel);
static USER_FUNC(lecture_button_flag_on_off);
static USER_FUNC(lecture_button_rtn);
static USER_FUNC(lecture_button_seq_wait);
static USER_FUNC(lecture_button_set_alpha);
static USER_FUNC(lecture_button_set_icon_id);
static USER_FUNC(lecture_button_set_pos);
static USER_FUNC(lecture_button_set_seq);
USER_FUNC(mainwindow_opendisable);
USER_FUNC(mainwindow_openenable);
static USER_FUNC(set_dir);
static USER_FUNC(x_flag_get);
static USER_FUNC(x_flag_set);
static USER_FUNC(bgm_start_wait);
static USER_FUNC(lect_push_a_check);
static USER_FUNC(juyoitem_goato);
static USER_FUNC(juyoitem_gogo);
static USER_FUNC(juyoitem_end);
static USER_FUNC(juyoitem_get_no);
static f32 lect_party_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);
static USER_FUNC(efffukidashichange_cam);
static USER_FUNC(lect_npc_balloon_on_off);
static USER_FUNC(effitemget_open_ptr);
static USER_FUNC(effitemgetchange_cam_ptr);
static USER_FUNC(lect_map_cam_chg);
static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings);
static USER_FUNC(lecture_button_get_icon_id);
static void lecture_button_disp(CameraId camera, void* param);
static USER_FUNC(x_flag_wait);
static USER_FUNC(x_flag_add);
static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start);

EvtWord yoshi_party_lecture_init[43];
EvtWord yoshi_party_lecture[271];
static EvtWord all_party_lecture_otosub[11];
EvtWord all_party_lecture[474];
EvtWord hammer_lv2_juyoitem_lecture_init[104];
EvtWord hammer_lv2_juyoitem_lecture_sub01[29];
EvtWord hammer_lv2_juyoitem_lecture_sub02[65];
EvtWord hammer_lv2_juyoitem_lecture[682];
EvtWord all_juyoitem_lecture_rel[252];

static f32 lect_mario_ido_sub(f32 elapsed, f32 total, f32 end, f32 start) {
    f32 rem = total - elapsed;

    return (rem * end + elapsed * start) / total;
}

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
    elapsed = OSTicksToMilliseconds((u64)(s32)((u32)event->lifetime - (u32)wwstart));
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

static USER_FUNC(lect_status_off) {
    statusWinForceClose();
    return EVT_RETURN_DONE;
}

static USER_FUNC(lect_status_on) {
    statusWinForceCloseClear();
    return EVT_RETURN_DONE;
}

static USER_FUNC(x_flag_set) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(x_flag_add) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    x_flag_data[index] += evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(x_flag_get) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);

    evtSetValue(event, args[1], x_flag_data[index]);
    return EVT_RETURN_DONE;
}

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

        if (!(btn->flags & 1)) {
            f32 ofs;
            Vec pos;

            switch (btn->iconId) {
                case ICON_075:
                case ICON_076:
                case ICON_077:
                case ICON_078:
                case ICON_079:
                case ICON_07A:
                case ICON_07B:
                case ICON_07C:
                case ICON_07D:
                    ofs = -15.0f;
                    break;
                default:
                    ofs = 0.0f;
                    break;
            }
            pos = disp->pos;
            pos.y += ofs;
            iconDispGxAlpha(&pos, 0x50, btn->iconId, btn->alpha, 1.0f);
        }
    }
}

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
                btn->iconId = ICON_068;
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

static USER_FUNC(lecture_button_set_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->iconId = value;
    return EVT_RETURN_DONE;
}

static USER_FUNC(lecture_button_set_alpha) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->alpha = value;
    return EVT_RETURN_DONE;
}

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

static USER_FUNC(lecture_button_set_seq) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    s32 value = evtGetValue(event, args[1]);
    LectButton* btn = &lbtn[index];

    btn->seq = value;
    return EVT_RETURN_DONE;
}

static USER_FUNC(lecture_button_get_icon_id) {
    EvtWord* args = event->args;
    s32 index = evtGetValue(event, args[0]);
    LectButton* btn = &lbtn[index];

    evtSetValue(event, args[1], btn->seq);
    return EVT_RETURN_DONE;
}

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

static void lect_map_cam_chg_sub(MapObject* obj, s32 camId, BOOL siblings) {
    obj->cameraId = camId;
    if (obj->child != NULL) {
        lect_map_cam_chg_sub(obj->child, camId, TRUE);
    }
    if (siblings && obj->next != NULL) {
        lect_map_cam_chg_sub(obj->next, camId, TRUE);
    }
}

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

static USER_FUNC(effitemgetchange_cam) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    ((ItemGetWork*)effNameToPtr(name)->userdata)->camera = camId;
    return EVT_RETURN_DONE;
}

static USER_FUNC(effitemgetchange_cam_ptr) {
    EvtWord* args = event->args;
    EffectEntry* eff = (EffectEntry*)evtGetValue(event, args[0]);

    ((ItemGetWork*)eff->userdata)->camera = evtGetValue(event, args[1]);
    return EVT_RETURN_DONE;
}

static USER_FUNC(effitemget_open) {
    EffectEntry* eff = effNameToPtr((const char*)evtGetValue(event, event->args[0]));

    ((ItemGetWork*)eff->userdata)->state = 1;
    return EVT_RETURN_DONE;
}

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

EVT_BEGIN(yoshi_party_lecture_init)
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 40)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(-2.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, 0)
    CALL(evt_party_cont_onoff, 0, 0)
    CALL(lect_party_ido_sum, 0, 1000)
    CALL(evt_party_cont_onoff, 0, 1)
    CALL(evt_party_set_dir, 0, 90, 300)
    RETURN
EVT_END()

EVT_BEGIN(yoshi_party_lecture)
    CALL(evt_msg_print, 0, STRING("lecture_friends_09_0_1"), 0, 0)
    CALL(evt_msg_print, 0, STRING("lecture_friends_09_1"), 0, 0)
    CALL(evt_cam_letter_box_disable, 1)
    WAIT_MS(300)
    CALL(evt_party_yoshi_ride, 0, 1)
    WAIT_MS(1000)
    CALL(evt_party_yoshi_ride, 0, 0)
    WAIT_MS(1000)
    CALL(evt_party_yoshi_ride, 0, 1)
    CALL(evt_msg_print_add, 0, STRING("lecture_friends_09_1_1"))
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(470.0f), FLOAT(71.0f), FLOAT(30.0f), FLOAT(-101.799805f), 1000, 11)
    WAIT_MS(1000)
    WAIT_MS(300)
    CALL(x_flag_set, 0, 1)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(3), 0)
        LOOP(0)
            IF_INT_LESS_OR_EQUAL(LW(3), 0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                SET_VALUE(LW(3), 10)
                ELSE
                SUBTRACT(LW(3), 1)
            END_IF
            CALL(x_flag_get, 0, LW(0))
            IF_INT_EQUAL(LW(0), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 140)
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(160.0f))
    CALL(x_flag_set, 0, 2)
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_party_set_dir, 0, 270, 300)
    WAIT_MS(300)
    CALL(x_flag_set, 0, 1)
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(3), 0)
        LOOP(0)
            IF_INT_LESS_OR_EQUAL(LW(3), 0)
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                SET_VALUE(LW(3), 10)
                ELSE
                SUBTRACT(LW(3), 1)
            END_IF
            CALL(x_flag_get, 0, LW(0))
            IF_INT_EQUAL(LW(0), 2)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    SUBTRACT(LW(0), 140)
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(160.0f))
    CALL(x_flag_set, 0, 2)
    CALL(evt_mario_set_dir, 90, 300, 0)
    CALL(evt_party_set_dir, 0, 90, 300)
    CALL(evt_msg_print_add, 0, STRING("lecture_friends_10"))
    LOOP(2)
        CALL(evt_party_yoshi_fly, 0, 1, 1000)
        WAIT_MS(300)
    END_LOOP
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(-2.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, 11)
    WAIT_MS(1000)
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_party_set_dir, 0, 270, 300)
    WAIT_MS(300)
    WAIT_MS(300)
    CALL(evt_party_yoshi_ride, 0, 0)
    CALL(evt_party_stop, 0)
    WAIT_MS(1000)
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 55)
    CALL(evt_party_move_pos2, 0, LW(0), LW(2), FLOAT(160.0f))
    CALL(evt_party_set_dir, 0, 90, 300)
    CALL(evt_cam_letter_box_disable, 0)
    CALL(evt_msg_print_add, 0, STRING("lecture_friends_11"))
    CALL(evt_party_set_pose, 0, STRING("PYS_O_1"))
    WAIT_MS(1000)
    CALL(evt_party_set_pose, 0, STRING("PYS_S_1"))
    RETURN
EVT_END()

static EVT_BEGIN(all_party_lecture_otosub)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_FF_GET_PARTY1"))
    WAIT_MS(2000)
    RETURN
EVT_END()

EVT_BEGIN(all_party_lecture)
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
    CALL(evt_party_set_camid, 0, 7)
    SPAWN_EVENT_GET_ID(PTR(all_party_lecture_otosub), LW(11))
    CALL(lect_itemget_kesan, 0, FLOAT(600.0f), LW(0), LW(1), LW(2))
    CALL(evt_eff, STRING("sub_bg"), STRING("itemget"), 1, LW(0), LW(1), LW(2), 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(effitemgetchange_cam, STRING("sub_bg"), 5)
    CALL(evt_msg_toge, 1, 0, 0, 0)
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
    IS_EVENT_RUNNING(LW(11), LW(12))
    IF_INT_EQUAL(LW(12), 1)
        DELETE_EVENT(LW(11))
        CALL(evt_snd_bgmoff, 513)
        CALL(evt_snd_bgmon, 288, 0)
    END_IF
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_mario_set_camid, 7)
    CALL(effitemget_open, STRING("sub_bg"))
    CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PARTY_LECTURE1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_stop_for_event)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_LECTURE1"))
    CALL(evt_snd_envoff, 1024)
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
    CALL(lect_test_systemlevel)
    SWITCH(LW(9))
        CASE_EQUAL(4)
        CALL_CHILD(PTR(yoshi_party_lecture_init))
    END_SWITCH
    LOOP(6)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LECTUREN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, 1)
        WAIT_MS(100)
        CALL(evt_mario_dispflag_onoff, 1, 1)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LECTUREN_BLINKING1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_dispflag_onoff, 0, 1)
    SWITCH(LW(9))
        CASE_EQUAL(4)
        CALL_CHILD(PTR(yoshi_party_lecture))
    END_SWITCH
    LOOP(6)
        CALL(evt_mario_dispflag_onoff, 1, 1)
        CALL(evt_party_dispflg_onoff, 0, 1, 1)
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LECTUREN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, 1)
        CALL(evt_party_dispflg_onoff, 0, 1, 0)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_party_dispflg_onoff, 0, 1, 1)
    CALL(evt_mario_init_camid)
    CALL(evt_party_init_camid, 0)
    CALL(lect_cam_load, 0)
    CALL(evt_mario_dispflag_onoff, 0, 1)
    CALL(evt_party_dispflg_onoff, 0, 1, 0)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
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

EVT_BEGIN(hammer_lv2_juyoitem_lecture_init)
    CALL(evt_mario_get_pos, 0, LW(3), LW(4), LW(5))
    CALL(evt_mario_set_pos, 0, 0, 120)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(0), 150)
    CALL(evt_npc_flag_onoff, 1, STRING("キノピコ"), 1536)
    CALL(evt_npc_flag_onoff, 1, STRING("キノピコ"), 1073741856)
    CALL(evt_npc_set_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    CALL(evt_npc_set_ry, STRING("キノピコ"), 270)
    CALL(evt_mario_set_pos, 610, 10, -14)
    CALL(evt_cam3d_evt_set_rel_dir, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(26.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, 0)
    CALL(evt_mario_cont_onoff, 0)
    CALL(lect_mario_ido_sum, 1000)
    CALL(evt_mario_cont_onoff, 1)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_pos, 0, 0, 120)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(26.0f), FLOAT(30.0f), FLOAT(-100.0f), 0, 11)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    WAIT_FRAMES(1)
    CALL(evt_mario_set_pos, 0, 0, 120)
    CALL(evt_mario_set_dir, 90, 300, 0)
    RETURN
EVT_END()

EVT_BEGIN(hammer_lv2_juyoitem_lecture_sub01)
    CALL(lecture_button_set_alpha, 0, 255)
    CALL(lecture_button_set_alpha, 1, 128)
    CALL(lecture_button_set_icon_id, 1, ICON_079)
    LOOP(0)
        CALL(lecture_button_set_icon_id, 0, ICON_067)
        WAIT_MS(300)
        CALL(lecture_button_set_icon_id, 0, ICON_066)
        WAIT_MS(300)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(hammer_lv2_juyoitem_lecture_sub02)
    CALL(lecture_button_set_alpha, 0, 128)
    CALL(lecture_button_set_alpha, 1, 255)
    CALL(lecture_button_set_icon_id, 0, ICON_067)
    LOOP(0)
        CALL(lecture_button_set_icon_id, 1, ICON_078)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_07B)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_07C)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_07D)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_07A)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_077)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_076)
        WAIT_MS(15)
        CALL(lecture_button_set_icon_id, 1, ICON_075)
        WAIT_MS(15)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(hammer_lv2_juyoitem_lecture)
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
    CALL(evt_msg_print, 0, STRING("lecture_s_hammer_04"), 0, STRING("キノピコ"))
    CALL(evt_msg_print_add, 0, STRING("lecture_s_hammer_05"))
    LOOP(0)
        CALL(evt_msg_toge, 1, 0, 0, 0)
        CALL(evt_msg_print, 0, STRING("lecture_s_hammer_06"), 0, 0)
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
                CALL(evt_msg_print, 0, STRING("lecture_s_hammer_07"), 0, STRING("キノピコ"))
                CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_3"))
            END_LOOP
            CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_1"), STRING("T_1"))
            CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("S_1"))
        END_INLINE_EVENT
        CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(85.0f), FLOAT(520.0f), FLOAT(0.0f), FLOAT(40.0f), FLOAT(-100.0f), 1000, 11)
        WAIT_MS(1000)
        CALL(lecture_button_flag_on_off, 0, 2, 1)
        CALL(lecture_button_set_icon_id, 0, ICON_066)
        CALL(lecture_button_set_seq, 0, 2)
        CALL(lecture_button_flag_on_off, 1, 2, 1)
        CALL(lecture_button_set_icon_id, 1, ICON_079)
        CALL(lecture_button_set_seq, 1, 2)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_seq_wait, 1, 1)
        SPAWN_EVENT_GET_ID(PTR(hammer_lv2_juyoitem_lecture_sub01), LW(11))
        SET_VALUE(LW(7), 0)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        SET_VALUE(LW(7), 0)
        SET_VALUE(LW(12), 0)
        CALL(mainwindow_opendisable)
        CALL(evt_mario_trigflag_onoff, 1, 67108864)
        CALL(evt_mario_set_enable_key, 512)
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
                        SPAWN_EVENT_GET_ID(PTR(hammer_lv2_juyoitem_lecture_sub02), LW(11))
                    END_IF
                    CASE_EQUAL(1)
                    CALL(lect_push_b_check, LW(7))
                    IF_INT_EQUAL(LW(7), 0)
                        DELETE_EVENT(LW(11))
                        SPAWN_EVENT_GET_ID(PTR(hammer_lv2_juyoitem_lecture_sub01), LW(11))
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
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_mario_key_onoff, 0)
        CALL(lect_status_on)
        CALL(evt_mario_trigflag_onoff, 0, 67108864)
        CALL(evt_mario_set_enable_key, 0)
        CALL(mainwindow_openenable)
        DELETE_EVENT(LW(11))
        CALL(lecture_button_set_seq, 0, 3)
        CALL(lecture_button_set_seq, 1, 3)
        CALL(evt_msg_continue)
        CALL(evt_mario_set_pose, STRING("M_V_2"))
        WAIT_MS(500)
        CALL(lecture_button_seq_wait, 0, 1)
        CALL(lecture_button_seq_wait, 1, 1)
        WAIT_MS(300)
        CALL(evt_mario_set_pose, STRING("M_S_1"))
        CALL(evt_mario_adjust_dir)
        CALL(evt_mario_set_dir, 90, 150, 0)
        CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(26.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, 11)
        WAIT_MS(1000)
        CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_2"), STRING("T_2"))
        CALL(evt_msg_print, 0, STRING("lecture_s_hammer_08"), 0, STRING("キノピコ"))
        CALL(evt_msg_select, 0, STRING("lecture_s_hammer_09"))
        CALL(evt_npc_set_autotalkpose, STRING("キノピコ"), STRING("S_1"), STRING("T_1"))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_add, 0, STRING("lecture_s_hammer_10"))
            ELSE
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_msg_print_add, 0, STRING("lecture_s_hammer_11"))
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(52.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, 11)
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    ADD(LW(0), 35)
    CALL(evt_npc_set_ry, STRING("キノピコ"), 90)
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_ry, STRING("キノピコ"), 270)
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("A_1"))
    WAIT_MS(200)
    SET_VALUE(LW(1), STRING("TESTMOBJ"))
    CALL(evt_mobj_entry, LW(1), STRING("MOBJ_Lv1BigBigBlock"))
    CALL(lect_mobj_hit_nasi, LW(1))
    CALL(lect_mobj_set_nosysblend, LW(1))
    CALL(evt_mobj_set_camid, LW(1), 7)
    CALL(evt_mario_get_pos, 0, LW(2), LW(3), LW(4))
    ADD(LW(2), 50)
    ADD(LW(3), 150)
    SUBTRACT(LW(4), 70)
    CALL(evt_mobj_flag_onoff, 1, LW(1), 64)
    CALL(evt_mobj_set_position, LW(1), LW(2), LW(3), LW(4))
    CALL(evt_mobj_set_gravity_bound, LW(1), FLOAT(0.16601563f), FLOAT(1.0f))
    CALL(evt_snd_sfxon, STRING("SFX_EVT_LECTURE_BLOCK_FALL1"), LW(7))
    BEGIN_INLINE_EVENT
        LOOP(0)
            CALL(evt_mobj_get_position, LW(1), LW(2), LW(3), LW(4))
            IF_INT_LESS_OR_EQUAL(LW(3), 10)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_snd_sfxon, STRING("SFX_EVT_LECTURE_BLOCK_DOWN1"), 0)
        CALL(evt_snd_sfxoff, LW(7))
        CALL(evt_cam_shake, 4, FLOAT(0.0f), FLOAT(0.009765625f), 300)
    END_INLINE_EVENT
    SET_VALUE(LW(0), FLOAT(0.0f))
    LOOP(0)
        CALL(evt_mobj_get_position, LW(1), LW(2), LW(3), LW(4))
        ADD_FLOAT(LW(0), FLOAT(5.0f))
        SUBTRACT_FLOAT(LW(3), LW(0))
        IF_INT_LESS_OR_EQUAL(LW(3), FLOAT(0.0f))
            SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
            CALL(evt_mobj_set_position, LW(1), LW(2), LW(3), LW(4))
            BREAK_LOOP
            ELSE
            CALL(evt_mobj_set_position, LW(1), LW(2), LW(3), LW(4))
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(300)
    CALL(evt_npc_set_anim, STRING("キノピコ"), STRING("A_2"))
    WAIT_MS(170)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(52.0f), FLOAT(55.0f), FLOAT(-100.0f), 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("lecture_s_hammer_12"), 0, STRING("キノピコ"))
    WAIT_MS(300)
    LOOP(6)
        CALL(evt_mobj_flag_onoff, 1, LW(1), 16384)
        WAIT_MS(100)
        CALL(evt_mobj_flag_onoff, 0, LW(1), 16384)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mobj_delete, LW(1))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set_rel, FLOAT(0.0f), FLOAT(80.0f), FLOAT(455.0f), FLOAT(52.0f), FLOAT(30.0f), FLOAT(-100.0f), 1000, 11)
    WAIT_MS(1000)
    CALL(evt_msg_print, 0, STRING("lecture_s_hammer_13"), 0, STRING("キノピコ"))
    CALL(evt_npc_get_position, STRING("キノピコ"), LW(0), LW(1), LW(2))
    ADD(LW(0), 100)
    CALL(evt_npc_set_ry, STRING("キノピコ"), 90)
    WAIT_MS(300)
    CALL(evt_npc_move_position, STRING("キノピコ"), LW(0), LW(2), 0, FLOAT(80.0f), 0)
    CALL(evt_npc_set_position, STRING("キノピコ"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(all_juyoitem_lecture_rel)
    CALL(evt_mario_normalize)
    CALL(juyoitem_get_no, LW(10))
    CALL(evt_mario_set_camid, 7)
    CALL(effitemget_open_ptr, LW(9))
    CALL(juyoitem_gogo)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_PARTY_LECTURE1_1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_npc_stop_for_event)
    CALL(evt_npc_start_for_one_event, STRING(/*"キノピコ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x52"))
    CALL(evt_party_stop, 0)
    WAIT_MS(1000)
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING("BGM_EVT_LECTURE1"))
    CALL(evt_snd_envoff, 1024)
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
    CALL(evt_npc_sound_data_set, STRING(/*"キノピコ"*/"\x83\x4C\x83\x6D\x83\x73\x83\x52"), STRING("SFX_FLD_NPC_KINOPIO_MOVE1L"), STRING("SFX_FLD_NPC_KINOPIO_MOVE1R"), EVT_NULLPTR, EVT_NULLPTR, EVT_NULLPTR)
    SWITCH(LW(10))
        CASE_EQUAL(10)
        CALL_CHILD(hammer_lv2_juyoitem_lecture_init)
    END_SWITCH
    SWITCH(LW(10))
        CASE_EQUAL(10)
        CALL_CHILD(hammer_lv2_juyoitem_lecture)
    END_SWITCH
    LOOP(6)
        CALL(evt_mario_dispflag_onoff, 1, 1)
        WAIT_MS(100)
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_EVT_LECTUREN_BLINKING1"), LW(0), LW(1), LW(2), 0)
        CALL(evt_mario_dispflag_onoff, 0, 1)
        WAIT_MS(100)
    END_LOOP
    CALL(evt_mario_dispflag_onoff, 1, 1)
    CALL(evt_mario_init_camid)
    CALL(lect_cam_load, 0)
    CALL(evt_cam3d_evt_off, 0, 11)
    CALL(evt_cam_road_reset)
    CALL(evt_mario_dispflag_onoff, 0, 1)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_snd_envoff, 513)
    CALL(evt_snd_envon, 288, 0)
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

static USER_FUNC(efffukidashichange_cam) {
    EvtWord* args = event->args;
    const char* name = (const char*)evtGetValue(event, args[0]);
    s32 camId = evtGetValue(event, args[1]);

    ((EffFukidashiWork*)effNameToPtr(name)->userdata)->camera = camId;
    return EVT_RETURN_DONE;
}

static f32 lect_party_ido_sub(f32 elapsed, f32 total, f32 end, f32 start) {
    f32 rem = total - elapsed;

    return (rem * end + elapsed * start) / total;
}

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
    elapsed = OSTicksToMilliseconds((u64)(s32)((u32)event->lifetime - (u32)wwstart));
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

static USER_FUNC(juyoitem_get_no) {
    evtSetValue(event, event->args[0], ((ItemEntry*)event->lwData[8])->itemId);
    return EVT_RETURN_DONE;
}

static USER_FUNC(juyoitem_end) {
    ItemEntry* item = (ItemEntry*)event->lwData[8];

    effSoftDelete((EffectEntry*)event->lwData[9]);
    marioSetCamId(CAMERA_3D);
    iconFlagOff(item->name, 0x200);
    return EVT_RETURN_DONE;
}

static USER_FUNC(juyoitem_gogo) {
    ItemEntry* item = (ItemEntry*)event->lwData[8];

    iconFlagOff(item->name, 0x200);
    itemSetPosition(item, 0.0f, -1000.0f, 0.0f);
    marioChgStayMotion();
    marioStSystemLevel(0);
    return EVT_RETURN_DONE;
}

static USER_FUNC(juyoitem_goato) {
    iconFlagOn(((ItemEntry*)event->lwData[8])->name, 2);
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
    animPoseSetMaterialFlagOn(mobj->poseId, 0x1800);
    return EVT_RETURN_DONE;
}

static USER_FUNC(lect_push_a_check) {
    EvtWord* args = event->args;

    if (keyGetButton(0) & PAD_BUTTON_A) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(lect_push_b_check) {
    EvtWord* args = event->args;

    if (keyGetButton(0) & PAD_BUTTON_B) {
        evtSetValue(event, args[0], TRUE);
    } else {
        evtSetValue(event, args[0], FALSE);
    }
    return EVT_RETURN_DONE;
}

static USER_FUNC(set_dir) {
    s32 dir = evtGetValue(event, event->args[0]);
    PartyEntry* party = partyGetPtr(0);

    if (party != NULL) {
        s32 angle = dir > 180 ? 270 : 90;
        party->displayDirectionTarget = party->displayDirection = 270 - angle;
    }
    return EVT_RETURN_DONE;
}


static USER_FUNC(bgm_start_wait) {
    return psndBGMStartCheck(1) ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

