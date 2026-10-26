#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "mario/mario.h"
#include "evt/evt_cam.h"
#include "evt/evt_hit.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_npc.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "driver/mapdrv.h"

#include "manager/evtmgr_cmd.h"
#include "driver/mobjdrv.h"
#include "driver/animdrv.h"
#include "mario/mariost.h"
#include "mario/mario_motion.h"
#include "pmario_sound.h"
#include "string.h"
#include "evt/evt_bero.h"
#include "eki.h"

static s32 lock;

USER_FUNC(sw_entry);
s32 sw_main(MapObjectEntry* mobj);
void sw_reset(void);
s32 sw_cnt(MapObjectEntry* mobj);
void sw_cntup(MapObjectEntry* mobj);

static EvtWord e_close[12];
static EvtWord e_open[12];
static BeroEntry bero_data[3];
EvtWord evt_box_open[115];
EvtWord evt_sw_complete[272];
EvtWord eki_06_init_evt[132];

static EVT_BEGIN(e_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door"), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(e_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 0, STRING("door"), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        "e_bero_1", // hitName
        1, // type
        3, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_05", // nextMapName
        "w_bero_2", // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        e_close, // enterAnimeArg
        e_open, // exitAnimeArg
    },
    {
        "e_bero_2", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "eki_05", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_box_open)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG6_34_3_BLUE_BLOCKS)
        RETURN
    END_IF
    LABEL(0)
    CALL(evt_npc_get_ReactionOfLivingBody, 0, LW(0))
    IF_INT_GREATER(LW(0), 0)
        WAIT_FRAMES(1)
        GOTO(0)
    END_IF
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_cam3d_evt_set, -140, 152, 528, -140, 57, -12, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GW(0), 0)
    SET_VALUE(GW(1), 0)
    SET_VALUE(GW(2), 0)
    CALL(sw_entry, STRING("MOBJ_eki_06_su3"), -240, 0, 40)
    CALL(sw_entry, STRING("MOBJ_eki_06_su2"), -140, 0, 40)
    CALL(sw_entry, STRING("MOBJ_eki_06_su1"), -40, 0, 40)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_BOX_OPEN1"), -240, 0, 40, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_BOX_OPEN1"), -140, 0, 40, 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_BOX_OPEN1"), -40, 0, 40, 0)
    CALL(evt_map_playanim, STRING("anm_hako"), MAP_ANIM_PLAY_ONCE, 0)
    CALL(evt_map_checkanim, STRING("anm_hako"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_hitobj_onoff, STRING("a_hako"), 1, 0)
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_34_3_BLUE_BLOCKS)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_sw_complete)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam3d_evt_set, -140, 152, 528, -140, 57, -12, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    BEGIN_INLINE_EVENT
        CALL(evt_mobj_set_anim, STRING("MOBJ_eki_06_su1"), STRING("ANIM_6"))
        CALL(evt_mobj_get_position, STRING("MOBJ_eki_06_su1"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_OBJ_SWITCH_SMALL1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(600)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_OBJ_SWITCH_JUMP1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(400)
        CALL(evt_mobj_set_anim, STRING("MOBJ_eki_06_su2"), STRING("ANIM_8"))
        CALL(evt_mobj_get_position, STRING("MOBJ_eki_06_su2"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_OBJ_SWITCH_SMALL1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(600)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_OBJ_SWITCH_JUMP1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        WAIT_MS(800)
        CALL(evt_mobj_set_anim, STRING("MOBJ_eki_06_su3"), STRING("ANIM_8"))
        CALL(evt_mobj_get_position, STRING("MOBJ_eki_06_su3"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_OBJ_SWITCH_SMALL1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(600)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_OBJ_SWITCH_JUMP1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_mobj_wait_animation_end, STRING("MOBJ_eki_06_su1"))
    CALL(evt_mobj_wait_animation_end, STRING("MOBJ_eki_06_su2"))
    CALL(evt_mobj_wait_animation_end, STRING("MOBJ_eki_06_su3"))
    CALL(evt_mobj_delete, STRING("MOBJ_eki_06_su1"))
    CALL(evt_mobj_delete, STRING("MOBJ_eki_06_su2"))
    CALL(evt_mobj_delete, STRING("MOBJ_eki_06_su3"))
    CALL(evt_cam3d_evt_set, -80, 202, 448, -80, 107, -92, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("origami"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_origami"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_hitobj_onoff, STRING("a_ori"), 1, 1)
    BEGIN_INLINE_EVENT
        CALL(evt_mapobj_get_position, STRING("origami"), LW(0), LW(1), LW(2))
        LOOP(3)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_OPEN1"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(300)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_OPEN1_2"), LW(0), LW(1), LW(2), 0)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        IF_INT_LESS(LW(2), -60)
            CALL(evt_mario_mov_pos2, LW(0), -25, FLOAT(80.0f))
        END_IF
    END_INLINE_EVENT
    CALL(evt_map_playanim, STRING("anm_kaidan167"), MAP_ANIM_PLAY_ONCE, 0)
    BEGIN_INLINE_EVENT
        WAIT_MS(900)
        CALL(evt_mapobj_get_position, STRING("origami"), LW(0), LW(1), LW(2))
        LOOP(6)
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG6_PERA_KAI_OPEN2"), LW(0), LW(1), LW(2), 0)
            WAIT_MS(300)
        END_LOOP
    END_INLINE_EVENT
    WAIT_FRAMES(100)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("oku_1"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_FRAMES(32)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("oku_2"), MAP_OBJ_FLAG_HIDDEN)
    WAIT_FRAMES(34)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("oku_3"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_map_checkanim, STRING("anm_kaidan167"), LW(0), LW(1))
    WAIT_MS(LW(1))
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_FF_ENV_SUCCESS1"))
    CALL(evt_cam3d_evt_off, 500, EASING_SINE_IN_OUT)
    WAIT_MS(500)
    SET_VALUE(GSW_STORY_PROGRESS, STG6_35_ORIGAMI_STAIRS)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

void sw_cntup(MapObjectEntry* mobj) {
    if (strcmp(mobj->name, "MOBJ_eki_06_su1") == 0) {
        evtSetValue(NULL, GW(0), evtGetValue(NULL, GW(0)) + 1);
    } else if (strcmp(mobj->name, "MOBJ_eki_06_su2") == 0) {
        evtSetValue(NULL, GW(1), evtGetValue(NULL, GW(1)) + 1);
    } else if (strcmp(mobj->name, "MOBJ_eki_06_su3") == 0) {
        evtSetValue(NULL, GW(2), evtGetValue(NULL, GW(2)) + 1);
    }
}

s32 sw_cnt(MapObjectEntry* mobj) {
    if (strcmp(mobj->name, "MOBJ_eki_06_su1") == 0) {
        return evtGetValue(NULL, GW(0));
    }
    if (strcmp(mobj->name, "MOBJ_eki_06_su2") == 0) {
        return evtGetValue(NULL, GW(1));
    }
    if (!strcmp(mobj->name, "MOBJ_eki_06_su3")) {
        return evtGetValue(NULL, GW(2));
    }
    return 0;
}

void sw_reset(void) {
    MapObjectEntry* mobj;

    mobj = mobjNameToPtr("MOBJ_eki_06_su1");
    mobj->flags &= ~MOBJ_FLAG_NO_SHADOW;
    switch (sw_cnt(mobj)) {
        case 2:
            animPoseSetAnim(mobj->poseId, "ANIM_5", 1);
            evtSetValue(NULL, GW(0), 0);
            break;
    }

    mobj = mobjNameToPtr("MOBJ_eki_06_su2");
    mobj->flags &= ~MOBJ_FLAG_NO_SHADOW;
    switch (sw_cnt(mobj)) {
        case 11:
            animPoseSetAnim(mobj->poseId, "ANIM_7", 1);
            evtSetValue(NULL, GW(1), 0);
            break;
    }

    mobj = mobjNameToPtr("MOBJ_eki_06_su3");
    mobj->flags &= ~MOBJ_FLAG_NO_SHADOW;
    switch (sw_cnt(mobj)) {
        case 4:
            animPoseSetAnim(mobj->poseId, "ANIM_7", 1);
            evtSetValue(NULL, GW(2), 0);
            break;
    }
}

s32 sw_main(MapObjectEntry* mobj) {
    switch (mobj->state) {
        case 2:
            break;
        case 0:
            if (!(mobj->flags & MOBJ_FLAG_HIT)) {
                break;
            }
            if (lock != 0) {
                break;
            }
            lock = 1;
            if (mobj->hitFlag == MOBJ_FLAG_HIT_STAND) {
                marioChgMot(MARIO_MOTION_JUMP_SW);
            }
            psndSFXOn_3D("SFX_STG6_OBJ_SWITCH_ON1", &mobj->position);
            sw_cntup(mobj);
            if (strcmp(mobj->name, "MOBJ_eki_06_su1") == 0) {
                if (sw_cnt(mobj) == 1) {
                    animPoseSetAnim(mobj->poseId, "ANIM_2", 1);
                } else {
                    sw_reset();
                }
            }
            if (strcmp(mobj->name, "MOBJ_eki_06_su2") == 0) {
                if (sw_cnt(mobj) < 10) {
                    animPoseSetAnim(mobj->poseId, "ANIM_2", 1);
                } else if (sw_cnt(mobj) == 10) {
                    animPoseSetAnim(mobj->poseId, "ANIM_4", 1);
                } else {
                    sw_reset();
                }
            }
            if (strcmp(mobj->name, "MOBJ_eki_06_su3") == 0) {
                if (sw_cnt(mobj) < 3) {
                    animPoseSetAnim(mobj->poseId, "ANIM_2", 1);
                } else if (sw_cnt(mobj) == 3) {
                    animPoseSetAnim(mobj->poseId, "ANIM_4", 1);
                } else {
                    sw_reset();
                }
            }
            mobj->state++;
            break;
        case 1:
            if (animPoseGetLoopTimes(mobj->poseId) < 1.0f) {
                break;
            }
            if (evtGetValue(NULL, GW(0)) == 1 && evtGetValue(NULL, GW(1)) == 10 && evtGetValue(NULL, GW(2)) == 3) {
                evtEntry(evt_sw_complete, 0, 0);
                mobj->state++;
                break;
            }
            mobjNameToPtr("MOBJ_eki_06_su1")->flags |= 2;
            mobjNameToPtr("MOBJ_eki_06_su2")->flags |= 2;
            mobjNameToPtr("MOBJ_eki_06_su3")->flags |= 2;
            if (strcmp(mobj->name, "MOBJ_eki_06_su1") == 0) {
                if (sw_cnt(mobj) == 1) {
                    animPoseSetAnim(mobj->poseId, "ANIM_3", 1);
                } else {
                    animPoseSetAnim(mobj->poseId, "ANIM_1", 1);
                }
            }
            if (strcmp(mobj->name, "MOBJ_eki_06_su2") == 0) {
                if (sw_cnt(mobj) == 10) {
                    animPoseSetAnim(mobj->poseId, "ANIM_5", 1);
                } else {
                    animPoseSetAnim(mobj->poseId, "ANIM_1", 1);
                }
            }
            if (strcmp(mobj->name, "MOBJ_eki_06_su3") == 0) {
                if (sw_cnt(mobj) == 3) {
                    animPoseSetAnim(mobj->poseId, "ANIM_5", 1);
                } else {
                    animPoseSetAnim(mobj->poseId, "ANIM_1", 1);
                }
            }
            mobj->flags &= ~MOBJ_FLAG_HIT;
            mobj->state = 0;
            lock = 0;
            break;
    }
    return 0;
}

USER_FUNC(sw_entry) {
    EvtWord* args = event->args;
    const char* name;
    f32 x, y, z;
    MapObjectEntry* mobj;
    Vec pos;

    name = (const char*)evtGetValue(event, args[0]);
    x = evtGetFloat(event, args[1]);
    y = evtGetFloat(event, args[2]);
    z = evtGetFloat(event, args[3]);
    if (!animGroupBaseAsync(name, gp->inBattle != 0, NULL)) {
        return EVT_RETURN_BLOCK;
    }
    mobjEntry(name, name);
    mobj = mobjNameToPtr(name);
    mobj->flags |= (MOBJ_FLAG_NO_SHADOW | MOBJ_FLAG_UPDATE_MTX);
    pos = (Vec){0.0f, 0.0f, 0.0f};
    pos.x = x;
    pos.y = y;
    pos.z = z;
    mobj->position = pos;
    mobj->mainFunc = sw_main;
    mobj->flags |= (MOBJ_FLAG_HIT_HAMMER | MOBJ_FLAG_HIT_STAND | MOBJ_FLAG_HIT_PARTY);
    mobj->state = 0;
    mobjCalcMtx(mobj);
    animPoseSetAnim(mobj->poseId, "ANIM_1", 1);
    lock = 0;
    return EVT_RETURN_DONE;
}

EVT_BEGIN(eki_06_init_evt)
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    CALL(evt_snd_bgmoff, PSND_FADEOUT)
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG6_EKI2"))
    CALL(evt_npc_change_fbat_mode, 1)
    SPAWN_EVENT(PTR(evt_box_open))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(STG6_34_3_BLUE_BLOCKS)
        CALL(evt_hitobj_onoff, STRING("a_ori"), 1, 0)
        CASE_EQUAL(STG6_34_3_BLUE_BLOCKS)
        SET_VALUE(GW(0), 0)
        SET_VALUE(GW(1), 0)
        SET_VALUE(GW(2), 0)
        CALL(sw_entry, STRING("MOBJ_eki_06_su3"), -240, 0, 40)
        CALL(sw_entry, STRING("MOBJ_eki_06_su2"), -140, 0, 40)
        CALL(sw_entry, STRING("MOBJ_eki_06_su1"), -40, 0, 40)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("origami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_hitobj_onoff, STRING("a_ori"), 1, 0)
        CASE_DEFAULT
        CALL(evt_map_playanim, STRING("anm_kaidan167"), MAP_ANIM_PLAY_AT_END, 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("n_origami"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("oku_1"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("oku_2"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("oku_3"), MAP_OBJ_FLAG_HIDDEN)
    END_SWITCH
    IF_INT_LESS(GSW_STORY_PROGRESS, STG6_34_3_BLUE_BLOCKS)
        ELSE
        CALL(evt_map_playanim, STRING("anm_hako"), MAP_ANIM_PLAY_AT_END, 0)
        CALL(evt_hitobj_onoff, STRING("a_hako"), 1, 0)
    END_IF
    CALL(evt_mapobj_trans, 1, STRING("HAKO"), 0, FLOAT(0.099609375f), 0)
    CALL(evt_mobj_powerupblk, STRING("pblk01"), 95, 295, -80, 0, GSWF_SHINE_37, 0)
    RETURN
EVT_END()
