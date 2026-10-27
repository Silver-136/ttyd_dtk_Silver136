#include "evt/evt_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_fade.h"
#include "evt/evt_npc.h"
#include "evt/evt_snd.h"
#include "driver/animdrv.h"
#include "driver/camdrv.h"
#include "driver/itemdrv.h"
#include "driver/npcdrv.h"
#include "mario/mario.h"
#include "mario/mariost.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "pmario_sound.h"
#include <string.h>
#include "evt/evt_kinopio.h"
#include "eff/eff_magic1_n64.h"
#include "libc_decls.h"
#include "evt/evt_eff.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "party/party_motion.h"
#include "evt/evt_sub.h"
#include "mario/mario_pouch.h"
#include "driver/mapdrv.h"

ItemEntry* itemEntry(const char* name, s32 itemId, u16 type, s32 collectFlag, EvtWord* evtCode, f32 x, f32 y, f32 z);

extern USER_FUNC(evt_mario_dispflag_onoff);
extern USER_FUNC(evt_mario_flag_onoff);
extern USER_FUNC(evt_pouch_add_coin);
extern USER_FUNC(evt_win_coin_off);
extern USER_FUNC(evt_win_coin_on);

typedef struct KinopioWork {
    s32 breakfastItem; // 0x0
    Vec breakfastPos;  // 0x4
    s32 ryoukin;       // 0x10
    Vec bed;           // 0x14
    Vec bedside;       // 0x20
    f32 cam[6];        // 0x2C
    f32 bedDeg;        // 0x44
    Vec npcTalk;       // 0x48
    char* npcName;     // 0x54
    s32 msg[14];       // 0x58
    s32 toBedEvt;      // 0x90
    s32 fromBedEvt;    // 0x94
} KinopioWork;

//.sbss
static KinopioWork* work;

//.sdata
static KinopioWork** wp = &work;

USER_FUNC(evt_kinopio_setup) {
    KinopioWork* w = (KinopioWork*)evtGetValue(event, event->args[0]);
    NpcEntry* npc = npcNameToPtr(w->npcName);
    if (!animGroupBaseAsync("p_slit", 0, NULL)) {
        return EVT_RETURN_BLOCK;
    }
    npc->talkEvt = kinopio_house_talk_evt;
    *wp = w;
    return EVT_RETURN_DONE;
}

USER_FUNC(msg_no) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    evtSetValue(event, args[1], (*wp)->msg[idx]);
    return EVT_RETURN_DONE;
}

USER_FUNC(power_chk) {
    EvtWord* args = event->args;
    s32 ok;
    s32 i;
    PouchPartyData* party = pouchGetPtr()->partyData;
    ok = 1;
    for (i = 0; i < PARTNER_COUNT; i++) {
        if ((party[i].flags & 1) && party[i].currentHP != party[i].maximumHP) {
            ok = 0;
            break;
        }
    }
    if (pouchGetHP() == pouchGetMaxHP() && pouchGetFP() == pouchGetMaxFP() && pouchGetAP() == pouchGetMaxAP() && ok) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(coin_chk) {
    EvtWord* args = event->args;
    if (pouchGetCoin() < (*wp)->ryoukin) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(get_ryoukin) {
    evtSetValue(event, event->args[0], (*wp)->ryoukin);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_npcname) {
    evtSetValue(event, event->args[0], (EvtWord)(*wp)->npcName);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_bed) {
    EvtWord* args = event->args;
    evtSetFloat(event, args[0], (*wp)->bed.x);
    evtSetFloat(event, args[1], (*wp)->bed.y);
    evtSetFloat(event, args[2], (*wp)->bed.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_bedside) {
    EvtWord* args = event->args;
    evtSetFloat(event, args[0], (*wp)->bedside.x);
    evtSetFloat(event, args[1], (*wp)->bedside.y);
    evtSetFloat(event, args[2], (*wp)->bedside.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_beddeg) {
    evtSetFloat(event, event->args[0], (*wp)->bedDeg);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_npctalk) {
    EvtWord* args = event->args;
    evtSetFloat(event, args[0], (*wp)->npcTalk.x);
    evtSetFloat(event, args[1], (*wp)->npcTalk.y);
    evtSetFloat(event, args[2], (*wp)->npcTalk.z);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_cam) {
    EvtWord* args = event->args;
    evtSetFloat(event, args[0], (*wp)->cam[0]);
    evtSetFloat(event, args[1], (*wp)->cam[1]);
    evtSetFloat(event, args[2], (*wp)->cam[2]);
    evtSetFloat(event, args[3], (*wp)->cam[3]);
    evtSetFloat(event, args[4], (*wp)->cam[4]);
    evtSetFloat(event, args[5], (*wp)->cam[5]);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_to_bed_evt) {
    evtSetValue(event, event->args[0], (*wp)->toBedEvt);
    return EVT_RETURN_DONE;
}

USER_FUNC(get_from_bed_evt) {
    evtSetValue(event, event->args[0], (*wp)->fromBedEvt);
    return EVT_RETURN_DONE;
}

static USER_FUNC(paper_on) {
    EvtWord* args = event->args;
    f32 dir = evtGetFloat(event, *args++);
    MarioWork* mario = marioGetPtr();
    if (isFirstCall) {
        marioPaperOn("p_slit");
        marioChgPose("M_S_1");
        marioChgPaper("PM_S_3A");
        event->userdata[0] = 0;
    }
    if (event->userdata[0] < 30) {
        f32 ang;
        if (strcmp(gp->currentMapName, "pik_04") == 0) {
            ang = 6.2831855f * dir / 360.0f;
            mario->position.x -= 0.2f * (f32)cos(ang);
            mario->position.z -= 0.2f * (f32)sin(ang);
        } else {
            ang = 6.2831855f * dir / 360.0f;
            mario->position.x += 0.2f * (f32)cos(ang);
            mario->position.z += 0.2f * (f32)sin(ang);
        }
        event->userdata[0]++;
    }
    mario->displayDirectionTarget = dir;
    mario->displayDirection = dir;
    return EVT_RETURN_BLOCK;
}

static USER_FUNC(paper_off) {
    marioPaperOff();
    return EVT_RETURN_DONE;
}

USER_FUNC(offset) {
    EvtWord* args = event->args;
    f32 dist = evtGetFloat(event, args[0]);
    f32 ang = 6.2831855f * (90.0f + camGetPtr(CAMERA_3D)->viewYaw) / 360.0f;
    evtSetFloat(event, args[1], dist * -(f32)sin(ang));
    evtSetFloat(event, args[2], dist * (f32)cos(ang));
    return EVT_RETURN_DONE;
}

USER_FUNC(breakfast) {
    ItemEntry* item = itemNameToPtr("breakfast");
    if (item == NULL || !(item->flags & ITEM_FLAG_ACTIVE)) {
        KinopioWork* w = *wp;
        if (w->breakfastItem != 0) {
            itemEntry("breakfast", w->breakfastItem, ITEM_TYPE_STILL, -1, NULL, w->breakfastPos.x, w->breakfastPos.y,
                      w->breakfastPos.z);
        }
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(snd_off) {
    psndSetFlag(PSND_FLAG_MUTE_SFX);
    return EVT_RETURN_DONE;
}

USER_FUNC(snd_on) {
    psndClearFlag(PSND_FLAG_MUTE_SFX);
    return EVT_RETURN_DONE;
}

USER_FUNC(N_cameraman_on) {
    NpcEntry* npc = npcNameToPtr("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223");
    animPoseSetMaterialFlagOn(npc->poseId, (MAP_OBJ_FLAG_NO_MAP_BLEND | MAP_OBJ_FLAG_NO_MAP_BLEND2));
    return EVT_RETURN_DONE;
}

//.data
EVT_BEGIN(kinopio_house_talk_evt)
    CALL(evt_mario_key_onoff, 0)
    CALL(get_ryoukin, LW(10))
    CALL(get_npcname, LW(11))
    CALL(evt_win_coin_on, 0, LW(12))
    CALL(msg_no, 0, LW(0))
    CALL(evt_msg_print_insert, 0, LW(0), 0, LW(11), LW(10))
    CALL(msg_no, 6, LW(0))
    CALL(evt_msg_select, 0, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(msg_no, 2, LW(0))
        CALL(evt_msg_print_add, 0, LW(0))
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_win_coin_off, LW(12))
        RETURN
    END_IF
    CALL(power_chk, LW(0))
    IF_INT_EQUAL(LW(0), 1)
        CALL(msg_no, 1, LW(0))
        CALL(evt_msg_print_add, 0, LW(0))
        CALL(msg_no, 6, LW(0))
        CALL(evt_msg_select, 0, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(msg_no, 2, LW(0))
            CALL(evt_msg_print_add, 0, LW(0))
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_win_coin_off, LW(12))
            RETURN
        END_IF
    END_IF
    SET_VALUE(LF(0), 0)
    CALL(evt_pouch_check_item, ITEM_INN_COUPON, LW(0))
    IF_INT_GREATER(LW(0), 0)
        CALL(msg_no, 7, LW(0))
        CALL(evt_msg_print_add, 0, LW(0))
        CALL(msg_no, 8, LW(0))
        CALL(evt_msg_select, 0, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            SET_VALUE(LF(0), 1)
            CALL(evt_pouch_remove_item, ITEM_INN_COUPON, LW(0))
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(coin_chk, LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(msg_no, 4, LW(0))
            CALL(evt_msg_print_add, 0, LW(0))
            CALL(evt_mario_key_onoff, 1)
            CALL(evt_win_coin_off, LW(12))
            RETURN
        END_IF
        CALL(get_ryoukin, LW(10))
        MULTIPLY(LW(10), -1)
        CALL(evt_pouch_add_coin, LW(10))
        WAIT_MS(1000)
    END_IF
    CALL(evt_win_coin_off, LW(12))
    CALL(msg_no, 3, LW(0))
    CALL(evt_msg_print_add, 0, LW(0))
    CALL(get_npcname, LW(0))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_NO_HIT)
    CALL(evt_mario_get_party, LW(11))
    CALL(evt_mario_get_exparty, LW(12))
    CALL(evt_mario_get_pos, 0, LW(13), LW(14), LW(15))
    CALL(evt_mario_goodbye_party, 1)
    CALL(evt_mario_goodbye_party, 0)
    CALL(get_to_bed_evt, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        BEGIN_INLINE_EVENT
            WAIT_MS(1000)
            CALL(get_cam, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
            CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 0, EASING_SINE_IN_OUT)
        END_INLINE_EVENT
        CALL(get_bedside, LW(0), LW(1), LW(2))
        CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(90.0f))
    ELSE
        BEGIN_INLINE_EVENT_GET_ID(LW(0))
            CALL_CHILD(LW(0))
        END_INLINE_EVENT
        LOOP(0)
            IS_EVENT_RUNNING(LW(0), LW(1))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(get_cam, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5))
        CALL(evt_cam3d_evt_set, LW(0), LW(1), LW(2), LW(3), LW(4), LW(5), 0, EASING_SINE_IN_OUT)
        WAIT_MS(500)
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, 131230, LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_flag_onoff, 1, 1792)
    CALL(get_bed, LW(0), LW(1), LW(2))
    CALL(evt_mario_jump_pos, LW(0), LW(1), LW(2), FLOAT(400.0f), 0, -1000)
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    CALL(evt_mario_adjust_dir)
    WAIT_MS(200)
    CALL(evt_mario_dispflag_onoff, 1, MARIO_DISP_FLAG_SHADOW_OFF)
    BEGIN_INLINE_EVENT_GET_ID(LW(10))
        CALL(get_beddeg, LW(0))
        CALL(paper_on, LW(0))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, 132026, LW(0), LW(1), LW(2), 0)
        WAIT_MS(200)
        CALL(evt_snd_sfxon_3d, 132027, LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(snd_off)
    CALL(evt_snd_bgmoff, PSND_PAUSE)
    CALL(evt_snd_bgmon, PSND_SLOT(1), STRING("BGM_FF_GO_SLEEPING1"))
#ifdef VERSION_G8ME01
    CALL(evt_fade_entry, 4, 6000, 0, 0, 0)
#else
    CALL(evt_fade_entry, 4, 3000, 0, 0, 0)
#endif
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    CALL(evt_fade_end_wait)
    CALL(evt_fade_entry, 12, 0, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_pouch_mario_recovery)
    CALL(evt_pouch_all_party_recovery)
    CALL(breakfast)
    WAIT_MS(3000)
    DELETE_EVENT(LW(10))
    CALL(paper_off)
    CALL(evt_mario_dispflag_onoff, 0, MARIO_DISP_FLAG_SHADOW_OFF)
    IF_INT_NOT_EQUAL(LW(11), 0)
        CALL(get_bed, LW(0), LW(1), LW(2))
        CALL(evt_mario_set_party_pos, 0, LW(11), LW(0), LW(1), LW(2))
    END_IF
    IF_INT_NOT_EQUAL(LW(12), 0)
        CALL(get_bed, LW(0), LW(1), LW(2))
        CALL(offset, FLOAT(30.0f), LW(3), LW(4))
        ADD_FLOAT(LW(0), LW(3))
        ADD_FLOAT(LW(2), LW(4))
        CALL(evt_mario_set_party_pos, 1, LW(12), LW(0), LW(1), LW(2))
    END_IF
    CALL(evt_mario_flag_onoff, 0, 1792)
    CALL(get_bedside, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    CALL(get_npcname, LW(0))
    CALL(evt_set_dir_to_target, LW(0), STRING("mario"))
    CALL(evt_mario_set_dir_npc, LW(0))
    CALL(evt_party_set_dir_npc, 0, LW(0))
    CALL(evt_party_set_dir_npc, 1, LW(0))
    CALL(evt_npc_flag_onoff, 1, LW(0), NPC_FLAG_INSTANT_TURN)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_INSTANT_TURN)
    CALL(evt_snd_bgmoff, PSND_FADEOUT | PSND_SLOT(1))
    CALL(evt_snd_bgmon, PSND_CONTINUE | PSND_FADEIN, 0)
    CALL(snd_on)
    ADD(GSW(1), 1)
    IF_INT_GREATER(GSW(1), 100)
        SET_VALUE(GSW(1), 100)
    END_IF
    IF_INT_EQUAL(GSW(1), 50)
        CALL_CHILD(evt_50)
        CALL(get_npcname, LW(0))
        CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_NO_HIT)
        RETURN
    END_IF
    CALL(evt_mario_adjust_dir)
    CALL(evt_fade_entry, 11, 1000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(get_from_bed_evt, LW(0))
    IF_INT_NOT_EQUAL(LW(0), 0)
        BEGIN_INLINE_EVENT_GET_ID(LW(10))
            CALL_CHILD(LW(0))
        END_INLINE_EVENT
        LOOP(0)
            IS_EVENT_RUNNING(LW(10), LW(11))
            IF_INT_EQUAL(LW(11), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_IF
    CALL(evt_party_run, 0)
    CALL(evt_party_run, 1)
    CALL(get_npctalk, LW(0), LW(1), LW(2))
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(90.0f))
    CALL(evt_fade_end_wait)
    CALL(get_npcname, LW(11))
    CALL(evt_mario_set_dir_npc, LW(11))
    CALL(evt_party_set_dir_npc, 0, LW(11))
    CALL(get_npcname, LW(11))
    CALL(msg_no, 5, LW(0))
    CALL(evt_msg_print, 0, LW(0), 0, LW(11))
    CALL(get_npcname, LW(0))
    CALL(evt_npc_flag_onoff, 0, LW(0), NPC_FLAG_NO_HIT)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_50)
    CALL(get_npcname, LW(10))
    CALL(evt_npc_get_position, LW(10), LW(13), LW(14), LW(15))
    CALL(evt_npc_entry, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), STRING("c_jugem_c"))
    CALL(evt_npc_set_tribe, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), STRING("\203J\203\201\203\211\203}\203\223"))
    CALL(N_cameraman_on)
    CALL(evt_npc_flag_onoff, 1, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), NPC_FLAG_INSTANT_TURN)
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("bom_02"))
        CALL(evt_npc_set_position, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), -365, 30, -170)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("gor_01"))
        CALL(evt_npc_set_position, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), 168, 154, -229)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("muj_01"))
        CALL(evt_npc_set_position, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), 270, 20, -145)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("nok_00"))
        CALL(evt_npc_set_position, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), -385, 25, -145)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("pik_04"))
        CALL(evt_npc_set_position, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), 310, 1329, -100)
        CALL(evt_npc_set_position, LW(10), 310, 1329, -65)
    END_IF
    IF_STR_EQUAL(LW(0), STRING("usu_00"))
        CALL(evt_npc_set_position, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), 268, 0, -210)
    END_IF
    CALL(evt_set_dir_to_target, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"), STRING("mario"))
    CALL(evt_set_dir_to_target, LW(10), STRING("mario"))
    CALL(evt_mario_set_dir_npc, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"))
    CALL(evt_party_set_dir_npc, 0, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"))
    CALL(msg_no, 9, LW(0))
    CALL(evt_msg_print, 0, LW(0), 0, LW(10))
    CALL(get_bed, LW(0), LW(1), LW(2))
    CALL(evt_mario_set_pos, LW(0), LW(1), LW(2))
    ADD(LW(2), 5)
    CALL(evt_party_set_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_fade_entry, 9, 1000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 100)
    CALL(evt_eff, 0, STRING("confetti"), 3, LW(0), LW(1), LW(2), 600, 0, 0, 0, 0, 0, 0, 0)
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    WAIT_MS(1000)
    CALL(msg_no, 10, LW(0))
    CALL(evt_msg_print, 0, LW(0), 0, LW(10))
    CALL(get_bedside, LW(0), LW(1), LW(2))
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(90.0f))
    CALL(get_npctalk, LW(0), LW(1), LW(2))
    CALL(evt_mario_mov_pos2, LW(0), LW(2), FLOAT(90.0f))
    CALL(evt_mario_set_dir_npc, LW(10))
    CALL(msg_no, 11, LW(0))
    CALL(evt_msg_print, 0, LW(0), 0, LW(10))
    CALL(evt_mario_set_dir_npc, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"))
    CALL(evt_party_set_dir_npc, 0, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"))
    WAIT_MS(300)
    CALL(evt_snd_sfxon, STRING("SFX_EVT_50_HOTEL_SHUTTER1"), 0)
    CALL(evt_fade_entry, 8, 150, 255, 255, 255)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    ADD(LW(1), 40)
    CALL(evt_fade_set_spot_pos, LW(0), LW(1), LW(2))
    CALL(evt_fade_end_wait)
    CALL(evt_fade_entry, 7, 150, 255, 255, 255)
    CALL(evt_fade_end_wait)
    CALL(msg_no, 12, LW(0))
    CALL(evt_msg_print, 0, LW(0), 0, LW(10))
    CALL(evt_mario_set_dir_npc, LW(10))
    CALL(evt_party_set_dir_npc, 0, LW(10))
    WAIT_MS(300)
    CALL(evt_sub_get_coin, 200)
    CALL(msg_no, 13, LW(0))
    CALL(evt_msg_print, 0, LW(0), 0, LW(10))
    CALL(evt_fade_entry, 10, 1000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_npc_check_delete, STRING("\217h\211\256\202\314\203J\203\201\203\211\203}\203\223"))
    CALL(evt_npc_set_position, LW(10), LW(13), LW(14), LW(15))
    CALL(evt_set_dir_to_target, LW(10), STRING("mario"))
    CALL(evt_fade_entry, 9, 1000, 0, 0, 0)
    CALL(evt_fade_end_wait)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

