#include "version.h"
#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "manager/evtmgr_cmd.h"
#include "evt/evt_cam.h"
#include "evt/evt_eff.h"
#include "evt/evt_ext.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_item.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_party.h"
#include "evt/evt_pouch.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "libc_decls.h"
#include "poly_trig.h"
#include "driver/mapdrv.h"

#include "string.h"
#include "mario/mariost.h"
#include "dolphin/os.h"
#include "stdlib.h"
#include "memory.h"
#include "pmario_sound.h"
#include "system.h"
#include "dolphin/mtx.h"
#include "driver/camdrv.h"
#include "mario/mario.h"
#include "party/party_cloud.h"
#include "driver/npcdrv.h"
#include "driver/hitdrv.h"
#include "driver/dispdrv.h"
#include "party.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "data_tables.h"
#include "evt/evt_npc.h"
#include "driver/extdrv.h"
#ifdef VERSION_G8ME01
USER_FUNC(N_evt_sub_status_gauge_force_update);
USER_FUNC(evt_sub_get_language);
#endif

typedef struct Fan {
    u16 flags;
    s16 state;
    s16 animIdx;
    u8 pad_0x6[2];
    u64 time;
    Vec pos;
    Vec vel;
    f32 field_0x28;
    Vec target;
    Vec param;
    s16 timer;
} Fan;
typedef struct FanAnim {
    char name[16];
    char anim[8];
    s16 loop;
    f32 rate;
    s32 start;
    s32 count;
} FanAnim;
typedef struct FanWork {
    s32 num;
    Fan* fans;
    s32 animNum;
    FanAnim anim[32];
    s32 hammerTimer;
    s32 count;
    s32 sfxId;
    u8 vol;
} FanWork;
FanWork* fan_wp;
static void pose_init(void);
static void pose_change(Fan* fan, const char* name, const char* anim);
static f32 randf(f32 min, f32 max);


s32 check_hammer(Fan* fan);
void psndSFX_vol(s32 id, u8 vol);
static void pose_main(void);
void fan_disp(CameraId camera, void* param);

s32 N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_cloud_ent(EventEntry* event, BOOL isFirstCall);
s32 evt_cloud_init(EventEntry* event, BOOL isFirstCall);
s32 evt_end_count(EventEntry* event, BOOL isFirstCall);
s32 evt_item_entry(EventEntry* event, BOOL isFirstCall);
s32 evt_item_get_item_end_wait(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_bound_rate(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_gravity(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_jump_power(EventEntry* event, BOOL isFirstCall);
s32 evt_item_set_move_dir_speed(EventEntry* event, BOOL isFirstCall);
s32 evt_map_checkanim(EventEntry* event, BOOL isFirstCall);
s32 evt_mapobj_rotate(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_balloon_bikkuri(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_balloon_off(EventEntry* event, BOOL isFirstCall);
s32 evt_mario_get_pos(EventEntry* event, BOOL isFirstCall);
s32 evt_msg_print_insert(EventEntry* event, BOOL isFirstCall);
s32 evt_npc_reverse_ry(EventEntry* event, BOOL isFirstCall);
s32 evt_party_yoshi_ride(EventEntry* event, BOOL isFirstCall);
s32 evt_pouch_get_yoshiname(EventEntry* event, BOOL isFirstCall);
s32 evt_pouch_open_mail(EventEntry* event, BOOL isFirstCall);
s32 evt_pouch_receive_mail(EventEntry* event, BOOL isFirstCall);
s32 evt_pouch_remove_item(EventEntry* event, BOOL isFirstCall);
s32 evt_tou_get_ranking(EventEntry* event, BOOL isFirstCall);
void ext_fan_dispent(void);
void ext_fan_main(void);
void ext_fan_init(void);

static EvtWord s_door_open[58];
static EvtWord s_door_close[58];
EvtWord n_1_door_open[22];
EvtWord n_1_door_close[22];
static EvtWord n_2_door_open[22];
static EvtWord n_2_door_close[22];
EvtWord n_3_door_open[12];
EvtWord n_3_door_close[12];
EvtWord n_4_door_open[12];
EvtWord n_4_door_close[12];
static BeroEntry bero_data[6];
EvtWord evt_move_g[63];
EvtWord init_gard1[39];
EvtWord talk_gard1[70];
EvtWord init_gard2[39];
EvtWord talk_gard2[70];
static EvtWord init_madam[39];
static EvtWord talk_madam[67];
EvtWord init_repo[8];
EvtWord talk_repo[112];
EvtWord init_cam1[8];
EvtWord talk_cam1[112];
EvtWord init_cam2[19];
EvtWord talk_cam2[112];
EvtWord init_kino4[13];
EvtWord reg_kino4[31];
EvtWord talk_kino4[112];
EvtWord init_kino5[8];
EvtWord talk_kino5[236];
EvtWord init_kinopiko[8];
EvtWord talk_kinopiko[112];
EvtWord init_kinopio_f[8];
EvtWord talk_kinopio_f[112];
EvtWord init_chu[20];
EvtWord talk_chu[67];
static EvtWord init_champ[32];
s32 move_pos[26];
EvtWord move_champ[155];
EvtWord talk_champ[34];
EvtWord init_shikowa[32];
EvtWord talk_shikowa[13];
static EvtWord init_uranoko[32];
static EvtWord talk_uranoko[13];
EvtWord init_ironA[38];
EvtWord talk_ironA[13];
EvtWord init_ironB[38];
EvtWord talk_ironB[13];
EvtWord init_borodo[32];
EvtWord talk_borodo[13];
static NpcSetupInfo npcEnt[17];
EvtWord evt_tougi2[285];
ExtTexInfo ext_fan_data[28];
EvtWord mario_momikucha_msg[111];
EvtWord evt_momikucha[105];
static EvtWord evt_key[173];
static EvtWord evt_poster_1[5];
static EvtWord evt_poster_2[5];
static EvtWord evt_poster_3[5];
static EvtWord evt_poster_4[5];
static EvtWord evt_poster_5[5];
static EvtWord evt_poster_6[5];
static EvtWord evt_poster_7[5];
static EvtWord evt_poster_8[VERSION_PICK(46, 60)];
static EvtWord evt_poster_9[5];
static EvtWord evt_after_peach[188];
EvtWord tou_02_init_evt[VERSION_PICK(576, 919)];

int __cvt_ull_flt();
static void pose_init(void) {
    char name[32];
    char anim[32];
    ExtTexInfo* d = ext_fan_data;
    s32 i = 0;
    s32 start;

    strcpy(name, "");
    strcpy(anim, "");
    fan_wp->animNum = -1;
    do {
        if (strcmp(d->group, name) != 0) {
            strcpy(name, d->group);
            strcpy(anim, "");
        }
        if (strcmp(d->anim, anim) != 0) {
            strcpy(anim, d->anim);
            start = i;
            fan_wp->animNum++;
        }
        strcpy(fan_wp->anim[fan_wp->animNum].name, name);
        strcpy(fan_wp->anim[fan_wp->animNum].anim, anim);
        fan_wp->anim[fan_wp->animNum].start = start;
        fan_wp->anim[fan_wp->animNum].count = i - start + 1;
        fan_wp->anim[fan_wp->animNum].rate = d->frame;
        fan_wp->anim[fan_wp->animNum].loop = 1;
        i++;
        d++;
    } while (d->group != NULL);
    fan_wp->animNum++;
}

static void pose_main(void) {
    s32 i;
    ExtdrvPose* pose;
    Fan* fan;
    FanAnim* a;
    f32 t;

    pose = extGetPosePtr();
    fan = fan_wp->fans;

    for (i = 0; i < fan_wp->num; i++, fan++, pose++) {
        a = &fan_wp->anim[fan->animIdx];
        t = 60.0f * OSTicksToMilliseconds((u64)(gp->renderTime - fan->time)) / 1000.0f / a->rate;
        if (t >= 1.0f) {
            fan->time = gp->renderTime;
            t = 0.0f;
        }
        pose->texId = a->start + (s32)((a->count - 1) * t);
    }
}

static void pose_change(Fan* fan, const char* name, const char* anim) {
    FanAnim* a;
    s32 i;

    for (i = 0, a = fan_wp->anim; i < fan_wp->animNum; i++, a++) {
        if (strcmp(a->name, name) == 0 && strcmp(a->anim, anim) == 0) {
            break;
        }
    }
    if (i < fan_wp->animNum && fan->animIdx != i) {
        fan->animIdx = i;
        fan->time = gp->renderTime;
    }
}

static f32 randf(f32 min, f32 max) {
    f32 r = rand() / 32767.0f;

    return r * (max - min) + min;
}

static EVT_BEGIN(s_door_open)
    IF_FLOAT_EQUAL(LW(0), FLOAT((0.0f)))
        IF_INT_EQUAL(GSWF(2526), 0)
            SET_VALUE(GSWF(2526), 1)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_AUTO_DOOR1")), LW(0), LW(1), LW(2), GW(13))
            END_INLINE_EVENT
        END_IF
    END_IF
    IF_FLOAT_EQUAL(LW(0), FLOAT(180.0f))
        SET_VALUE(GSWF(2526), 0)
        CALL(evt_snd_sfxoff, GW(13))
    END_IF
    MULTIPLY_FLOAT(LW(0), FLOAT(0.3330078f))
    CALL(evt_mapobj_trans, 1, STRING(("door_1_b")), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING(("door_1_a")), LW(0), 0, 0)
    RETURN
EVT_END()

static EVT_BEGIN(s_door_close)
    IF_FLOAT_EQUAL(LW(0), FLOAT(180.0f))
        IF_INT_EQUAL(GSWF(2526), 0)
            SET_VALUE(GSWF(2526), 1)
            BEGIN_INLINE_EVENT
                CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
                CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_AUTO_DOOR1")), LW(0), LW(1), LW(2), GW(13))
            END_INLINE_EVENT
        END_IF
    END_IF
    IF_FLOAT_EQUAL(LW(0), FLOAT((0.0f)))
        SET_VALUE(GSWF(2526), 0)
        CALL(evt_snd_sfxoff, GW(13))
    END_IF
    MULTIPLY_FLOAT(LW(0), FLOAT(0.3330078f))
    CALL(evt_mapobj_trans, 1, STRING(("door_1_b")), LW(0), 0, 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_trans, 1, STRING(("door_1_a")), LW(0), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(n_1_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_b")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_a")), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(n_1_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_b")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_2_a")), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_2_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_b")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_a")), 0, LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(n_2_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.38867188f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_b")), 0, LW(0), 0)
    MULTIPLY_FLOAT(LW(0), FLOAT(-1.0f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_3_a")), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(n_3_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_4")), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(n_3_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_4")), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(n_4_door_open)
    MULTIPLY_FLOAT(LW(0), FLOAT(0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_5")), 0, LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(n_4_door_close)
    MULTIPLY_FLOAT(LW(0), FLOAT(-0.44433594f))
    CALL(evt_mapobj_rotate, 1, STRING(("door_5")), 0, LW(0), 0)
    RETURN
EVT_END()

static BeroEntry bero_data[] = {
    {
        ("s_bero"), // hitName
        1, // type
        0, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_01", // nextMapName
        ("a_door_mon"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        s_door_close, // enterAnimeArg
        s_door_open, // exitAnimeArg
    },
    {
        ("n_bero_1"), // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_03", // nextMapName
        ("s_bero_1"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_1_door_close, // enterAnimeArg
        n_1_door_open, // exitAnimeArg
    },
    {
        ("n_bero_2"), // hitName
        1, // type
        6, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_03", // nextMapName
        ("s_bero_2"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_2_door_close, // enterAnimeArg
        n_2_door_open, // exitAnimeArg
    },
    {
        ("n_bero_3"), // hitName
        1, // type
        4, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_04", // nextMapName
        ("e_bero"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_3_door_close, // enterAnimeArg
        n_3_door_open, // exitAnimeArg
    },
    {
        ("n_bero_4"), // hitName
        1, // type
        4, // sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "tou_04", // nextMapName
        ("w_bero"), // nextBeroName
        5, // enterAnimeType
        5, // exitAnimeType
        n_4_door_close, // enterAnimeArg
        n_4_door_open, // exitAnimeArg
    },
    {0},
};

EVT_BEGIN(evt_move_g)
    CALL(evt_npc_get_position, STRING(("me")), LW(0), LW(1), LW(2))
    ADD(LW(2), 100)
    CALL(evt_mario_mov_pos, LW(0), LW(2), 500)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_move_position, STRING(("\203K\201[\203h\203}\203\223\202P")), -530, -185, 0, FLOAT(80.0f), 1)
        CALL(evt_set_dir_to_target, STRING(("\203K\201[\203h\203}\203\223\202P")), STRING(("mario")))
    END_INLINE_EVENT
    CALL(evt_npc_move_position, STRING(("\203K\201[\203h\203}\203\223\202Q")), 530, -185, 0, FLOAT(80.0f), 1)
    CALL(evt_set_dir_to_target, STRING(("\203K\201[\203h\203}\203\223\202Q")), STRING(("mario")))
    SET_VALUE(LW(0), 3)
    CALL_CHILD(PTR(bero_case_switch_on))
    SET_VALUE(LW(0), 4)
    CALL_CHILD(PTR(bero_case_switch_on))
    IF_INT_LESS(GSW_STORY_PROGRESS, 129)
        SET_VALUE(GSW_STORY_PROGRESS, 129)
        ELSE
        SET_VALUE(GSWF(2497), 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(init_gard1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(129)
        CALL(evt_npc_set_position, STRING(("me")), -470, 0, -185)
        CASE_LESS(171)
        CALL(evt_npc_set_position, STRING(("me")), -530, 0, -185)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2497), 0)
            CALL(evt_npc_set_position, STRING(("me")), -470, 0, -185)
            ELSE
            CALL(evt_npc_set_position, STRING(("me")), -530, 0, -185)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_gard1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(128)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_24")), 0, 0)
        CASE_LESS(129)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_37")), 0, 0)
        CALL_CHILD(PTR(evt_move_g))
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_37_01")), 0, 0)
        CASE_LESS(171)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_37_02")), 0, 0)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_re_002_1")), 0, 0)
            ELSE
            IF_INT_EQUAL(GSWF(2497), 0)
                CALL(evt_msg_print, 0, STRING(("stg3_tou_re_001")), 0, 0)
                CALL_CHILD(PTR(evt_move_g))
                ELSE
                CALL(evt_msg_print, 0, STRING(("stg3_tou_re_002")), 0, 0)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_gard2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(129)
        CALL(evt_npc_set_position, STRING(("me")), 470, 0, -185)
        CASE_LESS(171)
        CALL(evt_npc_set_position, STRING(("me")), 530, 0, -185)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2497), 0)
            CALL(evt_npc_set_position, STRING(("me")), 470, 0, -185)
            ELSE
            CALL(evt_npc_set_position, STRING(("me")), 530, 0, -185)
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_gard2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(128)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_24")), 0, 0)
        CASE_LESS(129)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_37")), 0, 0)
        CALL_CHILD(PTR(evt_move_g))
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_37_01")), 0, 0)
        CASE_LESS(171)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_37_02")), 0, 0)
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_re_002_1")), 0, 0)
            ELSE
            IF_INT_EQUAL(GSWF(2497), 0)
                CALL(evt_msg_print, 0, STRING(("stg3_tou_re_001")), 0, 0)
                CALL_CHILD(PTR(evt_move_g))
                ELSE
                CALL(evt_msg_print, 0, STRING(("stg3_tou_re_002")), 0, 0)
            END_IF
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_madam)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(172)
        CALL(evt_npc_set_position, STRING(("me")), 525, 0, 0)
        CASE_EQUAL(172)
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING(("me")), 525, 0, 0)
            ELSE
            CALL(evt_npc_set_position, STRING(("me")), 0, -1000, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING(("me")), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_madam)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_01_01")), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_01_02")), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_01_03")), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_01_04")), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_01_05")), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_01_06")), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_01_07")), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_repo)
    CALL(evt_npc_set_position, STRING(("me")), -100, 0, -25)
    RETURN
EVT_END()

EVT_BEGIN(talk_repo)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_02_01")), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_02_02")), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_02_03")), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_02_04")), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_02_05")), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_02_06")), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_02_07")), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_024")), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_024_1")), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_023")), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_024_1")), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_cam1)
    CALL(evt_npc_set_position, STRING(("me")), -150, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(talk_cam1)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_03_01")), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_03_02")), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_03_03")), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_03_04")), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_03_05")), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_03_06")), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_03_07")), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_026")), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_026_1")), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_025")), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_026_1")), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_cam2)
    IF_INT_LESS(GSW_STORY_PROGRESS, 128)
        CALL(evt_npc_set_position, STRING(("me")), 80, 200, -125)
        ELSE
        CALL(evt_npc_set_position, STRING(("me")), 30, 200, -60)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(talk_cam2)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_04_01")), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_04_02")), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_04_03")), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_04_04")), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_04_05")), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_04_06")), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_04_07")), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_028")), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_028_1")), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_027")), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_028_1")), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_kino4)
    CALL(evt_npc_set_position, STRING(("me")), -350, 0, 75)
    CALL(evt_npc_status_onoff, 1, STRING(("me")), 2)
    RETURN
EVT_END()

EVT_BEGIN(reg_kino4)
    LOOP(0)
        CALL(evt_npc_move_position, STRING(("me")), -350, 75, 0, FLOAT(48.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_reverse_ry, STRING(("me")))
        CALL(evt_npc_move_position, STRING(("me")), 350, 75, 0, FLOAT(48.0f), 0)
        WAIT_MS(1000)
        CALL(evt_npc_reverse_ry, STRING(("me")))
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(talk_kino4)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_05_01")), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_05_02")), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_05_03")), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_05_04")), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_05_05")), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_05_06")), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_05_07")), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_030")), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_030_1")), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_029")), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_030_1")), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_kino5)
    CALL(evt_npc_set_position, STRING(("me")), -520, 0, 25)
    RETURN
EVT_END()

EVT_BEGIN(talk_kino5)
    CALL(evt_pouch_get_yoshiname, LW(10))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_06_01")), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_06_02")), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_06_03")), 0, 0)
            CASE_GREATER(5)
            CALL(evt_mario_get_party, LW(0))
            IF_INT_EQUAL(LW(0), 4)
                CALL(evt_msg_print_insert, 0, STRING(("stg3_npc_02_06_04_yos")), 0, 0, LW(10))
                ELSE
                CALL(evt_msg_print_insert, 0, STRING(("stg3_npc_02_06_04")), 0, 0, LW(10))
            END_IF
            CASE_GREATER(0)
            CALL(evt_mario_get_party, LW(0))
            IF_INT_EQUAL(LW(0), 4)
                CALL(evt_msg_print_insert, 0, STRING(("stg3_npc_02_06_05_yos")), 0, 0, LW(10), LW(10))
                ELSE
                CALL(evt_msg_print_insert, 0, STRING(("stg3_npc_02_06_05")), 0, 0, LW(10))
            END_IF
            CASE_DEFAULT
            CALL(evt_mario_get_party, LW(0))
            IF_INT_EQUAL(LW(0), 4)
                CALL(evt_msg_print_insert, 0, STRING(("stg3_npc_02_06_06_yos")), 0, 0, LW(10))
                ELSE
                CALL(evt_msg_print_insert, 0, STRING(("stg3_npc_02_06_06")), 0, 0, LW(10))
            END_IF
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_mario_get_party, LW(0))
            IF_INT_EQUAL(LW(0), 4)
                CALL(evt_msg_print_insert, 0, STRING(("stg3_npc_02_06_07_yos")), 0, 0, LW(10))
                ELSE
                CALL(evt_msg_print, 0, STRING(("stg3_npc_02_06_07")), 0, 0)
            END_IF
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_mario_get_party, LW(0))
                IF_INT_EQUAL(LW(0), 4)
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_032_yos")), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_032")), 0, 0)
                END_IF
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_mario_get_party, LW(0))
                IF_INT_EQUAL(LW(0), 4)
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_032_1_yos")), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_032_1")), 0, 0)
                END_IF
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_mario_get_party, LW(0))
                    IF_INT_EQUAL(LW(0), 4)
                        CALL(evt_msg_print_insert, 0, STRING(("stg3_npc_re_031_yos")), 0, 0, LW(10))
                        ELSE
                        CALL(evt_msg_print, 0, STRING(("stg3_npc_re_031")), 0, 0)
                    END_IF
                    ELSE
                    CALL(evt_mario_get_party, LW(0))
                    IF_INT_EQUAL(LW(0), 4)
                        CALL(evt_msg_print, 0, STRING(("stg3_npc_re_032_1_yos")), 0, 0)
                        ELSE
                        CALL(evt_msg_print, 0, STRING(("stg3_npc_re_032_1")), 0, 0)
                    END_IF
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_kinopiko)
    CALL(evt_npc_set_position, STRING(("me")), -490, 0, 25)
    RETURN
EVT_END()

EVT_BEGIN(talk_kinopiko)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_07_01")), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_07_02")), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_07_03")), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_07_04")), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_07_05")), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_07_06")), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_07_07")), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_034")), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_034_1")), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_033")), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_034_1")), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_kinopio_f)
    CALL(evt_npc_set_position, STRING(("\203L\203m\203s\203I\217\227")), 100, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(talk_kinopio_f)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_08_01")), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_08_02")), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_08_03")), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_08_04")), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_08_05")), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_08_06")), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        IF_INT_EQUAL(GSWF(2500), 0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_08_07")), 0, 0)
            ELSE
            CALL(evt_tou_get_ranking, LW(0))
            SWITCH(LW(0))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_036")), 0, 0)
                CASE_LESS_OR_EQUAL(10)
                CALL(evt_msg_print, 0, STRING(("stg3_npc_re_036_1")), 0, 0)
                CASE_DEFAULT
                IF_INT_EQUAL(GSWF(2504), 0)
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_035")), 0, 0)
                    ELSE
                    CALL(evt_msg_print, 0, STRING(("stg3_npc_re_036_1")), 0, 0)
                END_IF
            END_SWITCH
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_chu)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(144)
        CALL(evt_npc_set_position, STRING(("me")), 245, 0, 125)
        CASE_DEFAULT
        CALL(evt_npc_set_position, STRING(("me")), 0, -1000, 0)
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_chu)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(131)
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_10_01")), 0, 0)
        CASE_LESS(164)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_GREATER(15)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_10_02")), 0, 0)
            CASE_GREATER(10)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_10_03")), 0, 0)
            CASE_GREATER(5)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_10_04")), 0, 0)
            CASE_GREATER(0)
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_10_05")), 0, 0)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING(("stg3_npc_02_10_06")), 0, 0)
        END_SWITCH
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING(("stg3_npc_02_10_07")), 0, 0)
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_champ)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING(("me")), -15, 0, 25)
            CALL(evt_npc_flag_onoff, 1, STRING(("me")), 32768)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                CALL(evt_npc_flag_onoff, 0, STRING(("me")), 32768)
            END_INLINE_EVENT
        END_IF
    END_SWITCH
    RETURN
EVT_END()

s32 move_pos[] = {
    0, 0, -100, -70, -225, 50, -230, 135,
    -105, 110, 10, -10, 135, 135, 220, 40,
    138, -65, 35, -50, 0, 110, -80, -40,
    110, -40,
};

EVT_BEGIN(move_champ)
    SET_VALUE(LW(10), 0)
    LOOP(0)
        SET_INT_READ_ADDR(PTR(move_pos))
        LOOP(13)
            READ_INT2(LW(0), LW(1))
            CALL(evt_sub_random, 5, LW(0))
            ADD(LW(0), 2)
            MULTIPLY(LW(0), 1000)
            WAIT_MS(LW(0))
            CALL(evt_npc_get_position, STRING(("me")), LW(5), LW(6), LW(7))
            CALL(evt_sub_random, 100, LW(0))
            CALL(evt_sub_random, 100, LW(1))
            SUBTRACT(LW(0), 50)
            ADD(LW(5), LW(0))
            ADD(LW(6), 40)
            SUBTRACT(LW(7), LW(1))
            CALL(evt_msg_toge, 2, LW(5), LW(6), LW(7))
            CALL(evt_end_count, LW(0))
            IF_INT_LESS(LW(0), 10)
                RETURN
            END_IF
            BEGIN_INLINE_EVENT
                SWITCH(LW(10))
                    CASE_EQUAL(0)
                    CALL(evt_msg_print, 0, STRING(("stg3_tou_182")), 0, 0)
                    CASE_EQUAL(1)
                    CALL(evt_msg_print, 0, STRING(("stg3_tou_183")), 0, 0)
                    CASE_EQUAL(2)
                    CALL(evt_msg_print, 0, STRING(("stg3_tou_184")), 0, 0)
                    CASE_EQUAL(3)
                    CALL(evt_msg_print, 0, STRING(("stg3_tou_185")), 0, 0)
                    CASE_EQUAL(4)
                    CALL(evt_msg_print, 0, STRING(("stg3_tou_186")), 0, 0)
                    CASE_EQUAL(5)
                    CALL(evt_msg_print, 0, STRING(("stg3_tou_187")), 0, 0)
                    CASE_EQUAL(6)
                    CALL(evt_msg_print, 0, STRING(("stg3_tou_188")), 0, 0)
                    CASE_EQUAL(7)
                    CALL(evt_msg_print, 0, STRING(("stg3_tou_189")), 0, 0)
                    CASE_EQUAL(8)
                    CALL(evt_msg_print, 0, STRING(("stg3_tou_186_01")), 0, 0)
                END_SWITCH
            END_INLINE_EVENT
            ADD(LW(10), 1)
            IF_INT_GREATER(LW(10), 8)
                SET_VALUE(LW(10), 0)
            END_IF
        END_LOOP
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(talk_champ)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_LESS(171)
        CALL(evt_end_count, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_msg_print, 0, STRING(("stg3_tou_190_01")), 0, STRING(("me")))
            ELSE
            CALL(evt_msg_print, 0, STRING(("stg3_tou_190")), 0, STRING(("me")))
        END_IF
        CASE_DEFAULT
        CALL(evt_msg_print, 0, STRING(("stg3_tou_479_02")), 0, STRING(("me")))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_shikowa)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING(("me")), -25, 0, 75)
            CALL(evt_npc_flag_onoff, 1, STRING(("me")), 32768)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                CALL(evt_npc_flag_onoff, 0, STRING(("me")), 32768)
            END_INLINE_EVENT
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_shikowa)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_479_01")), 0, STRING(("me")))
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(init_uranoko)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING(("me")), -75, 0, 50)
            CALL(evt_npc_flag_onoff, 1, STRING(("me")), 32768)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                CALL(evt_npc_flag_onoff, 0, STRING(("me")), 32768)
            END_INLINE_EVENT
        END_IF
    END_SWITCH
    RETURN
EVT_END()

static EVT_BEGIN(talk_uranoko)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_479_03")), 0, STRING(("me")))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_ironA)
    CALL(evt_npc_set_scale, STRING(("me")), FLOAT(1.5f), FLOAT(1.5f), FLOAT((1.0f)))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING(("me")), -30, 0, -50)
            CALL(evt_npc_flag_onoff, 1, STRING(("me")), 32768)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                CALL(evt_npc_flag_onoff, 0, STRING(("me")), 32768)
            END_INLINE_EVENT
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_ironA)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_479_04")), 0, STRING(("me")))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_ironB)
    CALL(evt_npc_set_scale, STRING(("me")), FLOAT(1.5f), FLOAT(1.5f), FLOAT((1.0f)))
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING(("me")), -60, 0, -25)
            CALL(evt_npc_flag_onoff, 1, STRING(("me")), 32768)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                CALL(evt_npc_flag_onoff, 0, STRING(("me")), 32768)
            END_INLINE_EVENT
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_ironB)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_479_05")), 0, STRING(("me")))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(init_borodo)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        IF_INT_EQUAL(GSWF(2496), 0)
            CALL(evt_npc_set_position, STRING(("me")), -100, 0, 100)
            CALL(evt_npc_flag_onoff, 1, STRING(("me")), 32768)
            BEGIN_INLINE_EVENT
                CALL(evt_seq_wait, 2)
                CALL(evt_npc_flag_onoff, 0, STRING(("me")), 32768)
            END_INLINE_EVENT
        END_IF
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_borodo)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_EQUAL(171)
        CALL(evt_msg_print, 0, STRING(("stg3_tou_479_06")), 0, STRING(("me")))
    END_SWITCH
    RETURN
EVT_END()

static NpcSetupInfo npcEnt[] = {
    {
        /*"ガードマン１"*/"\203K\201[\203h\203}\203\223\202P", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_gard1, // initEvent
        NULL, // regularEvent
        talk_gard1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ガードマン２"*/"\203K\201[\203h\203}\203\223\202Q", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_gard2, // initEvent
        NULL, // regularEvent
        talk_gard2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"マダム"*/"\203}\203_\203\200", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_madam, // initEvent
        NULL, // regularEvent
        talk_madam, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"レポーター"*/"\203\214\203|\201[\203^\201[", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_repo, // initEvent
        NULL, // regularEvent
        talk_repo, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カメラマン１"*/"\203J\203\201\203\211\203}\203\223\202P", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_cam1, // initEvent
        NULL, // regularEvent
        talk_cam1, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"カメラマン２"*/"\203J\203\201\203\211\203}\203\223\202Q", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_cam2, // initEvent
        NULL, // regularEvent
        talk_cam2, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピオ４"*/"\203L\203m\203s\203I\202S", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_kino4, // initEvent
        reg_kino4, // regularEvent
        talk_kino4, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピオ５"*/"\203L\203m\203s\203I\202T", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_kino5, // initEvent
        NULL, // regularEvent
        talk_kino5, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノピコ"*/"\203L\203m\203s\203R", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_kinopiko, // initEvent
        NULL, // regularEvent
        talk_kinopiko, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"バッジ屋"*/"\203o\203b\203W\211\256", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_chu, // initEvent
        NULL, // regularEvent
        talk_chu, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"チャンピョン"*/"\203`\203\203\203\223\203s\203\207\203\223", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_champ, // initEvent
        NULL, // regularEvent
        talk_champ, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"キノシコワ"*/"\203L\203m\203V\203R\203\217", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_shikowa, // initEvent
        NULL, // regularEvent
        talk_shikowa, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ウラノコ"*/"\203E\203\211\203m\203R", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_uranoko, // initEvent
        NULL, // regularEvent
        talk_uranoko, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"アイアンＡ"*/"\203A\203C\203A\203\223\202`", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_ironA, // initEvent
        NULL, // regularEvent
        talk_ironA, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"アイアンＢ"*/"\203A\203C\203A\203\223\202a", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_ironB, // initEvent
        NULL, // regularEvent
        talk_ironB, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {
        /*"ボロドー"*/"\203{\203\215\203h\201[", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_SHADOW_NO_FLOOR | NPC_FLAG_NO_WALL_COLLISION, // flags
        0, // reactionFlags
        init_borodo, // initEvent
        NULL, // regularEvent
        talk_borodo, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(evt_tougi2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    SET_VALUE(LF(0), 0)
    CALL(evt_bero_get_entername, LW(0))
    IF_STR_EQUAL(LW(0), STRING(("n_bero_2")))
        SET_VALUE(LF(0), 1)
    END_IF
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_mario_set_pos, -25, 200, -220)
        CALL(evt_party_set_pos, 0, 25, 200, -220)
        ELSE
        CALL(evt_mario_set_pos, -25, 0, -220)
        CALL(evt_party_set_pos, 0, 25, 0, -220)
    END_IF
    CALL(evt_mario_set_dir, 90, 0, 0)
    CALL(evt_party_set_dir, 0, 270, -1)
    IF_INT_EQUAL(LF(0), 1)
        CALL(evt_cam3d_evt_set, 0, 290, 190, 0, 238, -158, 0, 11)
        ELSE
        CALL(evt_cam3d_evt_set, 0, 90, 190, 0, 38, -158, 0, 11)
    END_IF
    CALL(evt_seq_wait, 2)
    SET_VALUE(LW(1), 2)
    SPAWN_EVENT(PTR(bero_open_door_play_se))
    CALL(evt_sub_intpl_msec_init, 11, 0, -180, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        IF_INT_EQUAL(LF(0), 1)
            CALL_CHILD(PTR(n_2_door_open))
            ELSE
            CALL_CHILD(PTR(n_1_door_open))
        END_IF
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(2), 100)
        CALL(evt_mario_mov_pos, LW(0), LW(2), 1000)
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_party_get_pos, 0, LW(0), LW(1), LW(2))
        ADD(LW(2), 100)
        CALL(evt_party_move_pos, 0, LW(0), LW(2), 1000)
    END_INLINE_EVENT
    WAIT_MS(700)
    SET_VALUE(LW(1), 2)
    SPAWN_EVENT(PTR(bero_close_door_play_se))
    CALL(evt_sub_intpl_msec_init, 11, 180, 0, 500)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value)
        IF_INT_EQUAL(LF(0), 1)
            CALL_CHILD(PTR(n_2_door_close))
            ELSE
            CALL_CHILD(PTR(n_1_door_close))
        END_IF
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_msg_print_party, STRING(("stg3_tou_33")))
    CALL(evt_msg_select, 0, STRING(("stg3_tou_33_yn")))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_34_kur")))
            CASE_EQUAL(2)
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_34_nok")))
            CASE_EQUAL(5)
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_34_win")))
        END_SWITCH
        ELSE
        CALL(evt_mario_get_party, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(1)
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_35_kur")))
            CASE_EQUAL(2)
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_35_nok")))
            CASE_EQUAL(5)
            CALL(evt_msg_print_add, 0, STRING(("stg3_tou_35_win")))
        END_SWITCH
    END_IF
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_NOD1_4")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING(("M_N_2")))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL(evt_msg_print_party, STRING(("stg3_tou_36")))
    CALL(evt_cam3d_evt_off, 500, 11)
    WAIT_MS(500)
    SET_VALUE(GSWF(2374), 0)
    SET_VALUE(GSW_STORY_PROGRESS, 128)
    CALL(N_evt_bero_set_mario_bottomless_respawn_pos_to_current_mario_pos)
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

ExtTexInfo ext_fan_data[] = {
    {("c_kinopio"), ("KNP_W_1"), 2.0f},
    {("c_kinopio"), ("KNP_W_1"), 6.0f},
    {("c_kinopio"), ("KNP_W_1"), 10.0f},
    {("c_kinopio"), ("KNP_W_1"), 14.0f},
    {("c_kinopio"), ("KNP_W_1"), 18.0f},
    {("c_kinopio"), ("KNP_W_1"), 22.0f},
    {("c_kinopio"), ("KNP_W_1"), 26.0f},
    {("c_kinopio"), ("KNP_W_1"), 30.0f},
    {("c_kinopio"), ("KNP_W_1"), 34.0f},
    {("c_kinopiko"), ("KNF_W_1"), 2.0f},
    {("c_kinopiko"), ("KNF_W_1"), 6.0f},
    {("c_kinopiko"), ("KNF_W_1"), 10.0f},
    {("c_kinopiko"), ("KNF_W_1"), 14.0f},
    {("c_kinopiko"), ("KNF_W_1"), 18.0f},
    {("c_kinopiko"), ("KNF_W_1"), 22.0f},
    {("c_kinopiko"), ("KNF_W_1"), 26.0f},
    {("c_kinopiko"), ("KNF_W_1"), 30.0f},
    {("c_kinopiko"), ("KNF_W_1"), 34.0f},
    {("c_kinopiko"), ("KNF_W_1"), 38.0f},
    {("c_jugemu"), ("JGM_W_1"), 0.0f},
    {("c_jugemu"), ("JGM_W_1"), 4.0f},
    {("c_jugemu"), ("JGM_W_1"), 8.0f},
    {("c_jugemu"), ("JGM_W_1"), 12.0f},
    {("c_jugemu"), ("JGM_W_1"), 16.0f},
    {("c_jugemu"), ("JGM_W_1"), 20.0f},
    {("c_jugemu"), ("JGM_W_1"), 24.0f},
    {("c_jugemu"), ("JGM_W_1"), 28.0f},
    {0},
};

void ext_fan_init(void) {
    s32 i;
    ExtdrvPose* pose;
    Fan* fan;

    fan_wp = _mapAlloc(sizeof(FanWork));
    memset(fan_wp, 0, sizeof(FanWork));
    fan_wp->num = extGetPoseNum();
    fan_wp->fans = _mapAlloc(fan_wp->num * sizeof(Fan));
    memset(fan_wp->fans, 0, fan_wp->num * sizeof(Fan));
    pose_init();
    pose = extGetPosePtr();
    fan = fan_wp->fans;
    for (i = 0; i < fan_wp->num; i++, pose++, fan++) {
        pose->flags |= 7;
        PSMTXIdentity(*pose->mtx);
        (*pose->mtx)[0][3] = 0.0f;
        (*pose->mtx)[1][3] = 0.0f;
        (*pose->mtx)[2][3] = 0.0f;
        pose->dir = randf(0.0f, 360.0f);
        switch (irand(1000) % 3) {
            case 0:
                pose_change(fan, "c_kinopio", "KNP_W_1");
                break;
            case 1:
                pose_change(fan, "c_kinopiko", "KNF_W_1");
                break;
            case 2:
            default:
                pose_change(fan, "c_jugemu", "JGM_W_1");
                break;
        }
        fan->state = 0;
        {
            Vec pos = {0.0f, 0.0f, 0.0f};

            pos.x = randf(-100.0f, 100.0f);
            pos.z = randf(-100.0f, 100.0f);
            fan->pos = pos;
            fan->vel = (Vec){0.0f, 0.0f, 0.0f};
        }
    }
    fan_wp->sfxId = psndSFXOn_3D("SFX_STG3_AUDIENCE_FAN1", &(Vec){0.0f, 0.0f, 0.0f});
    fan_wp->vol = psndSFX_get_vol(fan_wp->sfxId);
}

void fan_disp(CameraId camera, void* param) {
    extLoadRenderMode();
    extLoadVertex();
    extLoadTexture();
    extLoadTev();
    extDraw();
    extLoadTextureExit();
}

#pragma opt_propagation off
static Vec fan_zero;
void ext_fan_main(void) {
    MarioWork* mario = marioGetPtr();
    Fan* fan;
    ExtdrvPose* pose;
    s32 li;
    Fan* lfan;
    s32 i;
    Vec dir;
    Vec target = {0.0f, 0.0f, 0.0f};

    if (mario->currMotionId == MARIO_MOTION_HAMMER) {
        fan_wp->hammerTimer++;
    } else {
        fan_wp->hammerTimer = 0;
    }
    if (evtGetValue(NULL, GF(10))) {
        ExtdrvPose* pose = extGetPosePtr();
        Fan* fan = fan_wp->fans;

        for (i = 0; i < fan_wp->num; i++, fan++, pose++) {
            if (fan->state == 0) {
                fan->pos = mario->position;
                target = fan->pos;
                pose->flags &= ~1;
                fan->flags |= 1;
                break;
            }
        }
    } else {
        s32 i;
        s32 found = 0;

        pose = extGetPosePtr();
        fan = fan_wp->fans;

        for (i = 0; i < fan_wp->num; i++, fan++, pose++) {
            if (fan->state == 0) {
                if (!found) {
                    found = 1;
                    fan->pos = mario->position;
                    pose->flags &= ~1;
                    fan->flags |= 1;
                } else {
                    fan->pos = npcNameToPtr("\203`\203\203\203\223\203s\203\207\203\223")->position;
                    target = fan->pos;
                    pose->flags &= ~1;
                    fan->flags |= 1;
                    break;
                }
            }
        }
    }

    {

        Fan* fan;
        s32 i;

        for (i = 0, fan = fan_wp->fans; i < fan_wp->num; i++, fan++) {
            if (fan->state == 0) {
                PSVECSubtract(&target, &fan->pos, &dir);
                if (PSVECMag(&dir)) {
                    PSVECNormalize(&dir, &dir);
                    dir.y = 0.0f;
                    PSVECScale(&dir, &dir, 0.1f);
                    PSVECAdd(&fan->vel, &dir, &fan->vel);
                }
            }
        }
    }

    {

        for (i = 0, fan = fan_wp->fans; i < fan_wp->num; i++, fan++) {
            if (fan->state == 0) {
                s32 j;
                Fan* other;

                for (j = 0, other = fan_wp->fans; j < fan_wp->num; j++, other++) {
                    if (other->state == 0 && i != j && PSVECSquareDistance(&fan->pos, &other->pos) < 900.0f) {
                        PSVECSubtract(&fan->pos, &other->pos, &dir);
                        dir.y = 0.0f;
                        PSVECNormalize(&dir, &dir);
                        PSVECScale(&dir, &dir, 5.0f);
                        PSVECAdd(&fan->vel, &dir, &fan->vel);
                        PSVECSubtract(&other->vel, &dir, &other->vel);
                    }
                }
            }
        }
    }

    {
        f32 angle;
        f32 dist;
        f32 dy;
        f32 spd;
        f32 x;
        f32 z;
        f32 dir2;
        for (i = 0, fan = fan_wp->fans; i < fan_wp->num; i++, fan++) {
            if (fan->state == 0) {
                switch (check_hammer(fan)) {
                    case 0:
                        break;
                    case 1:
                        fan->state = 10;
                        { // FAKE: hand-inlined randf(135.0f, 225.0f) (same rand() call, r*90+135), 6.28 first so its pool address is created before 90/135 as in the target
                            f32 k;
                            f32 mn;
                            f32 r = rand() / 32767.0f;
                            angle = (6.2831855f * ((r * (k = 225.0f - 135.0f)) + (mn = 135.0f))) / 360.0f;
                        }
                        dist = randf(400.0f, 500.0f);
                        x = fan->pos.x + dist * polySinf(angle);
                        z = fan->pos.z - dist * polyCosf(angle);
                        {
                            Vec tgt = {0.0f, 0.0f, 0.0f};

                            tgt.x = x;
                            tgt.y = -100.0f;
                            tgt.z = z;
                            fan->target = tgt;
                        }
                        fan->param.y = randf(4.0f, 6.0f);
                        fan->param.z = randf(0.1f, 0.2f);
                        dy = fan->target.y - fan->pos.y;
                        dist = distABf(fan->pos.x, fan->pos.z, fan->target.x, fan->target.z);
                        angleABf(fan->pos.x, fan->pos.z, fan->target.x, fan->target.z);
                        fan->timer = dist / fan->param.y;
                        fan->param.x = dy / fan->timer + fan->param.z * fan->timer / 2.0f;
                        // FAKE: integer cast makes the literal a CSE/loop-invariant temp (target addi r30 + mr r29 copy)
                        psndSFXOn_3D(SFX_NAME_WORD("SFX_STG3_AUDIENCE_FAN_FLY1"), &fan->pos);
                        break;
                    case 2: {

                        angle = angleABf(mario->position.x, mario->position.z, fan->pos.x, fan->pos.z);
                        dir2 = cloudGetBreathDir();
                        fan->state = 20;
                        angle = (angle + dir2) / 2.0f;
                        fan->target = fan_zero;
                        spd = randf(0.1f, 0.2f);
                        angle = 6.2831855f * angle / 360.0f;
                        fan->param.x = spd * polySinf(angle);
                        fan->param.y = randf(0.03f, 0.06f);
                        fan->param.z = spd * -polyCosf(angle);
                        fan->timer = 120;
                        // FAKE: integer cast makes the literal a CSE/loop-invariant temp (target addi r30 + mr r29 copy)
                        psndSFXOn_3D(SFX_NAME_WORD("SFX_STG3_AUDIENCE_FAN_FLY1"), &fan->pos);
                        break;
                    }
                }
            }
        }
    }
    fan_wp->count = fan_wp->num;

    {
        f32 angle;
        f32 dist;
        f32 h;

        pose = extGetPosePtr();
        lfan = fan_wp->fans;
        for (li = 0; li < fan_wp->num; li++, pose++, lfan++) {
            switch (lfan->state) {
                case 0: {
                    HitCheckQuery query;
                    HitCheckQuery* q = &query;
                    PSVECNormalize(&lfan->vel, &lfan->vel);
                    dist = PSVECMag(&lfan->vel);
                    query.targetPos = lfan->pos;
                    query.targetPos.y += 4.0f;
                    query.targetDir = lfan->vel;
                    query.targetDir.y = 0.0f;
                    query.targetDistance = 25.0f + dist;
                    if (hitCheckVecFilter(&query, NULL)) {
                        C_VECReflect(&lfan->vel, &q->hitNormal, &lfan->vel);
                    } else {
                        lfan->pos.x += lfan->vel.x;
                        lfan->pos.z += lfan->vel.z;
                        lfan->vel.x *= 0.6f;
                        lfan->vel.z *= 0.6f;
                    }
                    if (lfan->pos.x < -350.0f) {
                        lfan->pos.x = -350.0f;
                    }
                    if (lfan->pos.x > 350.0f) {
                        lfan->pos.x = 350.0f;
                    }
                    if (lfan->pos.z > (h = 195.0f + li / 100.0f)) {
                        lfan->pos.z = h;
                    }
                    break;
                }
                case 10:
                    lfan->pos.y += lfan->param.x;
                    lfan->param.x -= lfan->param.z;
                    angle = angleABf(lfan->pos.x, lfan->pos.z, lfan->target.x, lfan->target.z);
                    angle = 6.2831855f * angle / 360.0f;
                    lfan->pos.x += lfan->param.y * polySinf(angle);
                    lfan->pos.z += lfan->param.y * -polyCosf(angle);
                    if (--lfan->timer < 0) {
                        lfan->state = 100;
                    }
                    break;
                case 20:
                    if (--lfan->timer > 0) {
                        lfan->target.x += lfan->param.x;
                        lfan->target.y += lfan->param.y;
                        lfan->target.z += lfan->param.z;
                        lfan->pos.x += lfan->target.x;
                        lfan->pos.y += lfan->target.y;
                        lfan->pos.z += lfan->target.z;
                    } else {
                        lfan->state = 100;
                    }
                    break;
                case 100:
                    pose->flags &= ~3;
                    lfan->state++;
                    break;
                case 101:
                    break;
            }
            if (lfan->flags & 1) {
                fan_wp->count--;
            } else if (lfan->state != 0) {
                fan_wp->count--;
            }
            (*pose->mtx)[0][3] = lfan->pos.x;
            (*pose->mtx)[1][3] = lfan->pos.y;
            (*pose->mtx)[2][3] = lfan->pos.z;
            pose->dir = angleABf(target.x, target.z, lfan->pos.x, lfan->pos.z);
        }
    }
    psndSFX_pos(fan_wp->sfxId, &target);
    psndSFX_vol(fan_wp->sfxId, fan_wp->vol * fan_wp->count / fan_wp->num);
}

static __declspec(section ".rodata") Vec fan_zero = {0.0f, 0.0f, 0.0f};
#pragma opt_propagation reset

EVT_BEGIN(mario_momikucha_msg)
    SET_VALUE(LW(10), 0)
    LOOP(0)
        CALL(evt_sub_random, 5, LW(0))
        ADD(LW(0), 2)
        MULTIPLY(LW(0), 1000)
        WAIT_MS(LW(0))
        CALL(evt_sub_random, 200, LW(0))
        CALL(evt_sub_random, 200, LW(1))
        SUBTRACT(LW(0), 50)
        SUBTRACT(LW(1), 50)
        CALL(evt_msg_toge, 2, LW(0), 30, LW(1))
        CALL(evt_end_count, LW(0))
        IF_INT_LESS(LW(0), 10)
            RETURN
        END_IF
        BEGIN_INLINE_EVENT
            SWITCH(LW(10))
                CASE_EQUAL(0)
                CALL(evt_msg_print, 0, STRING(("stg3_tou_236")), 0, 0)
                CASE_EQUAL(1)
                CALL(evt_msg_print, 0, STRING(("stg3_tou_237")), 0, 0)
                CASE_EQUAL(2)
                CALL(evt_msg_print, 0, STRING(("stg3_tou_238")), 0, 0)
                CASE_EQUAL(3)
                CALL(evt_msg_print, 0, STRING(("stg3_tou_239")), 0, 0)
                CASE_EQUAL(4)
                CALL(evt_msg_print, 0, STRING(("stg3_tou_240")), 0, 0)
                CASE_EQUAL(5)
                CALL(evt_msg_print, 0, STRING(("stg3_tou_240_01")), 0, 0)
            END_SWITCH
        END_INLINE_EVENT
        ADD(LW(10), 1)
        IF_INT_GREATER(LW(10), 5)
            SET_VALUE(LW(10), 0)
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(evt_momikucha)
    SET_VALUE(LF(0), 0)
    SET_VALUE(GF(10), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 133)
        IF_INT_EQUAL(GSWF(2402), 0)
            SET_VALUE(GSWF(2402), 1)
            SET_VALUE(LF(0), 1)
            SET_VALUE(GF(10), 0)
        END_IF
    END_IF
    CALL(evt_tou_get_ranking, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        IF_INT_EQUAL(GSWF(2403), 0)
            SET_VALUE(GSWF(2403), 1)
            SET_VALUE(LF(0), 1)
            SET_VALUE(GF(10), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        RETURN
    END_IF
    CALL(evt_npc_entry, STRING(("fan")), STRING(("c_kinopio")))
    CALL(evt_npc_entry, STRING(("fan2")), STRING(("c_kinopiko")))
    CALL(evt_npc_entry, STRING(("fan3")), STRING(("c_jugemu")))
    CALL(evt_npc_set_position, STRING(("fan")), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING(("fan2")), 0, -1000, 0)
    CALL(evt_npc_set_position, STRING(("fan3")), 0, -1000, 0)
    CALL(evt_ext_entry, 200, PTR(ext_fan_data), PTR(ext_fan_init), PTR(ext_fan_main), PTR(ext_fan_dispent))
    IF_INT_EQUAL(GF(10), 0)
        CALL(evt_npc_set_position, STRING(("\203`\203\203\203\223\203s\203\207\203\223")), 0, 0, 0)
        CALL(evt_npc_change_interrupt, STRING(("\203`\203\203\203\223\203s\203\207\203\223")), 1, PTR(move_champ))
        ELSE
        SPAWN_EVENT(PTR(mario_momikucha_msg))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_key)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_party_yoshi_ride, 0, 0)
    SET_VALUE(GSW_STORY_PROGRESS, 154)
    CALL(evt_item_get_item_end_wait, STRING(("key")))
    CALL(evt_party_stop, 0)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING(("stg3_tou_307_08")))
    WAIT_MS(500)
    BEGIN_INLINE_EVENT_GET_ID(LW(11))
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_FIND1_3")), LW(0), LW(1), LW(2), 0)
        CALL(evt_eff_fukidashi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        WAIT_MS(1500)
    END_INLINE_EVENT
    CALL(evt_snd_bgmoff, 1024)
    CALL(evt_snd_bgmon, 1, STRING(("BGM_FF_MAIL_RECEPTION2")))
    SPAWN_EVENT_GET_ID(PTR(evt_sub_mail_vibration), LW(10))
    WAIT_MS(500)
    LOOP(0)
        IS_EVENT_RUNNING(LW(11), LW(12))
        IF_INT_EQUAL(LW(12), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_bikkuri)
    LOOP(0)
        CALL(evt_key_get_buttontrg, 0, LW(0))
        IF_INT_EQUAL(LW(0), 256)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_mario_balloon_off)
    CALL(evt_snd_bgmoff, 513)
    CALL(evt_snd_bgmon, 288, 0)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_MAIL_RECEPTION1")), LW(0), LW(1), LW(2), 0)
    DELETE_EVENT(LW(10))
    WAIT_FRAMES(1)
    CALL(evt_sub_rumble_onoff, 1, 0)
    CALL(evt_mario_set_pose, STRING(("M_N_9")))
    WAIT_MS(500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_MAIL_RECEPTION2")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING(("M_N_10")))
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_307_09")), 0, 0)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL(evt_pouch_receive_mail, 15)
    CALL(evt_pouch_open_mail, 15)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_party_run, 0)
    RETURN
EVT_END()

void ext_fan_dispent(void) {
    pose_main();
    dispEntry(CAMERA_3D, 0.0f, 1, fan_disp, NULL);
}

#pragma opt_propagation off
s32 check_hammer(Fan* fan) {
    MarioWork* mario = marioGetPtr();
    PartyEntry* party;
    f32 rot;
    f32 x;
    f32 z;
    f32 angle;
    f32 dist;
    Vec pos;

    if (mario->position.y > 0.0f) {
        return 0;
    }
    if (fan->flags & 1) {
        return 0;
    }
    if (mario->currMotionId == MARIO_MOTION_HAMMER && fan_wp->hammerTimer > 10) {
        rot = 6.2831855f * reviseAngle(mario->displayDirection - 90.0f + camGetPtr(CAMERA_3D)->viewYaw) / 360.0f;

        x = mario->position.x + 35.0f * polySinf(rot);
        z = mario->position.z - 35.0f * polyCosf(rot);
        if (distABf(fan->pos.x, fan->pos.z, x, z) < 35.0f) {
            return 1;
        }
    }
    if (mario->currMotionId == MARIO_MOTION_HAMMER_2 && (s32)mario->currSubMotionId == 0x14 && PSVECDistance(&mario->position, &fan->pos) < 65.0f) {
        return 1;
    }
    party = partyGetPtr(0);
    if (party != NULL && party->currentMemberId == PARTY_MEMBER_FLURRIE && (party->flags & PARTY_FLAG_IS_BEING_USED)) {
        angle = 6.2831855f * cloudGetBreathDir() / 360.0f;
        dist = cloudGetBreathDist();
        pos.x = party->position.x + dist * polySinf(angle);
        pos.y = party->position.y;
        pos.z = party->position.z - dist * polyCosf(angle);
        if (angle >= 0.0f && PSVECDistance(&pos, &fan->pos) < 30.0f) {
            return 2;
        }
    }
    return 0;
}
#pragma opt_propagation reset

s32 evt_end_count(EventEntry* event, BOOL isFirstCall) {
    evtSetValue(event, event->args[0], fan_wp->count);
    return EVT_RETURN_DONE;
}

static EVT_BEGIN(evt_poster_1)
    SET_VALUE(GSWF(2456), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_2)
    SET_VALUE(GSWF(2457), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_3)
    SET_VALUE(GSWF(2458), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_4)
    SET_VALUE(GSWF(2459), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_5)
    SET_VALUE(GSWF(2460), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_6)
    SET_VALUE(GSWF(2461), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_7)
    SET_VALUE(GSWF(2462), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_8)
    SET_VALUE(GSWF(2463), 1)
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 153)
#ifdef VERSION_G8ME01
        CALL(evt_sub_get_language, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            CALL(evt_mapobj_get_position, STRING("p_m_8"), LW(0), LW(1), LW(2))
            ELSE
            CALL(evt_mapobj_get_position, STRING("p_m_8_e"), LW(0), LW(1), LW(2))
        END_IF
        CALL(evt_item_entry, STRING("key"), 19, LW(0), LW(1), LW(2), 4, -1, PTR(evt_key))
        CALL(evt_item_set_move_dir_speed, STRING("key"), FLOAT(180.0f), FLOAT(90.0f))
        CALL(evt_item_set_gravity, STRING("key"), FLOAT(1.0f))
        CALL(evt_item_set_bound_rate, STRING("key"), FLOAT(0.5996094f))
        CALL(evt_item_set_jump_power, STRING("key"), FLOAT(500.0f))
        CALL(evt_item_status_on, STRING("key"), 16)
#else
        CALL(evt_mapobj_get_position, STRING(("p_m_8")), LW(0), LW(1), LW(2))
        CALL(evt_item_entry, STRING(("key")), 19, LW(0), LW(1), LW(2), 4, -1, PTR(evt_key))
        CALL(evt_item_set_move_dir_speed, STRING(("key")), FLOAT(180.0f), FLOAT((90.0f)))
        CALL(evt_item_set_gravity, STRING(("key")), FLOAT((1.0f)))
        CALL(evt_item_set_bound_rate, STRING(("key")), FLOAT(0.5996094f))
        CALL(evt_item_set_jump_power, STRING(("key")), FLOAT(500.0f))
        CALL(evt_item_status_on, STRING(("key")), 16)
#endif
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(evt_poster_9)
    SET_VALUE(GSWF(2464), 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_after_peach)
    CALL(evt_cam3d_evt_set, 0, 120, 494, 0, 40, 0, 0, 11)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_stop, 0)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 270, 0)
    CALL(evt_mario_set_pos, 25, 0, 75)
    CALL(evt_party_set_pos, 0, 60, 0, 100)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("door_1_a")), 1)
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("door_1_b")), 1)
    CALL(evt_seq_wait, 2)
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_fade_end_wait)
    WAIT_MS(2000)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_479_00_01")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING(("SFX_VOICE_MARIO_NOD1_4")), LW(0), LW(1), LW(2), 0)
    CALL(evt_mario_set_pose, STRING(("M_N_2")))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL(evt_cam3d_evt_set, 0, 100, 395, 0, 40, 0, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_479_00_02")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, -40, 100, 395, -40, 40, 0, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_479_00_03")), 0, STRING(("\203`\203\203\203\223\203s\203\207\203\223")))
    WAIT_MS(300)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_479_00_04")), 0, STRING(("\203E\203\211\203m\203R")))
    WAIT_MS(300)
    CALL(evt_cam3d_evt_set, 0, 100, 395, 0, 40, 0, 500, 11)
    WAIT_MS(500)
    CALL(evt_msg_print, 0, STRING(("stg3_tou_479_00_05")), 0, STRING(("\203L\203m\203V\203R\203\217")))
    WAIT_MS(300)
    CALL(evt_mario_set_pose, STRING(("M_I_2")))
    WAIT_MS(800)
    CALL(evt_mario_set_pose, STRING(("M_S_1")))
    CALL(evt_cam3d_evt_set, 0, 120, 494, 0, 40, 0, 500, 11)
    CALL(evt_party_run, 0)
    CALL(evt_mario_mov_pos2, 0, 190, FLOAT(80.0f))
    CALL(evt_bero_mapchange, STRING(("tou_01")), STRING(("a_door_mon")))
    RETURN
EVT_END()

EVT_BEGIN(tou_02_init_evt)
    SWITCH(GSW_STORY_PROGRESS)
        CASE_BETWEEN(127, 137)
#ifdef VERSION_G8ME01
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU2"))
        CALL(evt_snd_bgmoff, 26624)
        CASE_GREATER_OR_EQUAL(165)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU2"))
        CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU2"))
        CALL(evt_snd_bgmoff, 26624)
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG3_TOU1"))
        CALL(evt_snd_bgmon, 160, 0)
        CALL(evt_snd_envon, 272, STRING("ENV_STG3_TOU2"))
#else
        CALL(evt_snd_bgmon, 512, STRING(("BGM_STG3_TOU2")))
        CALL(evt_snd_envon, 272, STRING(("ENV_STG3_TOU2")))
        CALL(evt_snd_bgmoff, 26624)
        CASE_GREATER_OR_EQUAL(165)
        CALL(evt_snd_bgmon, 512, STRING(("BGM_STG3_TOU2")))
        CALL(evt_snd_envon, 272, STRING(("ENV_STG3_TOU2")))
        CALL(evt_snd_bgmoff, 26624)
        CASE_DEFAULT
        CALL(evt_snd_bgmon, 512, STRING(("BGM_STG3_TOU1")))
        CALL(evt_snd_bgmon, 160, 0)
        CALL(evt_snd_envon, 272, STRING(("ENV_STG3_TOU2")))
#endif
    END_SWITCH
    SET_VALUE(LW(0), PTR(bero_data))
    CALL(evt_bero_get_info)
    IF_INT_EQUAL(GSWF(2374), 1)
#ifdef VERSION_G8ME01
        CALL_CHILD(bero_case_entry)
        SPAWN_EVENT(PTR(evt_tougi2))
        ELSE
        CALL_CHILD(evt_bero_info_run)
#else
        CALL_CHILD(PTR(bero_case_entry))
        SPAWN_EVENT(PTR(evt_tougi2))
        ELSE
        CALL_CHILD(PTR(evt_bero_info_run))
#endif
    END_IF
    CALL(evt_npc_setup, PTR(npcEnt))
    CALL_CHILD(PTR(init_kinopio_f))
#ifdef VERSION_G8ME01
    CALL(evt_npc_change_interrupt, STRING("キノピオ女"), 6, PTR(talk_kinopio_f))
    CALL(evt_mobj_kururing_floor, STRING("kururin1"), -260, 0, -120, STRING("sita"), 125, GSWF(5623))
    IF_INT_LESS(GSW_STORY_PROGRESS, 129)
        SET_VALUE(LW(0), 3)
        CALL_CHILD(bero_case_switch_off)
        SET_VALUE(LW(0), 4)
        CALL_CHILD(bero_case_switch_off)
#else
    CALL(evt_npc_change_interrupt, STRING(("\203L\203m\203s\203I\217\227")), 6, PTR(talk_kinopio_f))
    CALL(evt_mobj_kururing_floor, STRING(("kururin1")), -260, 0, -120, STRING(("sita")), 125, GSWF(5623))
    IF_INT_LESS(GSW_STORY_PROGRESS, 129)
        SET_VALUE(LW(0), 3)
        CALL_CHILD(PTR(bero_case_switch_off))
        SET_VALUE(LW(0), 4)
        CALL_CHILD(PTR(bero_case_switch_off))
#endif
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, 172)
        IF_INT_EQUAL(GSWF(2497), 0)
            SET_VALUE(LW(0), 3)
#ifdef VERSION_G8ME01
            CALL_CHILD(bero_case_switch_off)
            SET_VALUE(LW(0), 4)
            CALL_CHILD(bero_case_switch_off)
#else
            CALL_CHILD(PTR(bero_case_switch_off))
            SET_VALUE(LW(0), 4)
            CALL_CHILD(PTR(bero_case_switch_off))
#endif
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 171)
        IF_INT_EQUAL(GSWF(2496), 0)
#ifdef VERSION_G8ME01
            CALL(evt_npc_set_position, STRING("キノピオ女"), 0, -1000, 0)
            CALL(evt_npc_set_position, STRING("キノピオ４"), 0, -1000, 0)
#else
            CALL(evt_npc_set_position, STRING(("\203L\203m\203s\203I\217\227")), 0, -1000, 0)
            CALL(evt_npc_set_position, STRING(("\203L\203m\203s\203I\202S")), 0, -1000, 0)
#endif
        END_IF
    END_IF
    IF_INT_EQUAL(GSW_STORY_PROGRESS, 171)
        CALL(evt_mario_get_party, LW(15))
        IF_INT_EQUAL(LW(15), 0)
            CALL(evt_mario_set_party, 0, 1)
            CALL(evt_party_run, 0)
#ifdef VERSION_G8ME01
        END_IF
        CALL(evt_pouch_mario_recovery)
        CALL(evt_pouch_all_party_recovery)
        CALL(N_evt_sub_status_gauge_force_update)
#else
            CALL(evt_pouch_mario_recovery)
            CALL(evt_pouch_all_party_recovery)
        END_IF
#endif
        CALL(evt_pouch_remove_item, 65, LW(0))
        SPAWN_EVENT(PTR(evt_after_peach))
        SET_VALUE(GSWF(2402), 1)
        SET_VALUE(GSWF(2403), 1)
    END_IF
#ifdef VERSION_G8ME01
    CALL(evt_map_playanim, STRING("anm_paneru"), 1, 0)
#else
    CALL(evt_map_playanim, STRING(("anm_paneru")), 1, 0)
#endif
    BEGIN_INLINE_EVENT
        LOOP(0)
            LOOP(0)
#ifdef VERSION_G8ME01
                CALL(evt_map_checkanim, STRING("anm_paneru"), LW(0), LW(1))
                IF_INT_LESS(LW(1), 5333)
                    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AMB_OBJ_SCREEN2"), 178, 0, -98, 0)
#else
                CALL(evt_map_checkanim, STRING(("anm_paneru")), LW(0), LW(1))
                IF_INT_LESS(LW(1), 5333)
                    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_AMB_OBJ_SCREEN2")), 178, 0, -98, 0)
#endif
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_FRAMES(10)
#ifdef VERSION_G8ME01
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AMB_OBJ_SCREEN1"), 178, 0, -98, 0)
            LOOP(0)
                CALL(evt_map_checkanim, STRING("anm_paneru"), LW(0), LW(1))
                IF_INT_LESS(LW(1), 2000)
                    CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AMB_OBJ_SCREEN2"), 178, 0, -98, 0)
#else
            CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_AMB_OBJ_SCREEN1")), 178, 0, -98, 0)
            LOOP(0)
                CALL(evt_map_checkanim, STRING(("anm_paneru")), LW(0), LW(1))
                IF_INT_LESS(LW(1), 2000)
                    CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_AMB_OBJ_SCREEN2")), 178, 0, -98, 0)
#endif
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
            WAIT_FRAMES(5)
#ifdef VERSION_G8ME01
            CALL(evt_snd_sfxon_3d, STRING("SFX_STG3_AMB_OBJ_SCREEN1"), 178, 0, -98, 0)
            LOOP(0)
                CALL(evt_map_checkanim, STRING("anm_paneru"), LW(0), LW(1))
#else
            CALL(evt_snd_sfxon_3d, STRING(("SFX_STG3_AMB_OBJ_SCREEN1")), 178, 0, -98, 0)
            LOOP(0)
                CALL(evt_map_checkanim, STRING(("anm_paneru")), LW(0), LW(1))
#endif
                IF_INT_GREATER(LW(1), 6333)
                    BREAK_LOOP
                END_IF
                WAIT_FRAMES(1)
            END_LOOP
        END_LOOP
    END_INLINE_EVENT
    CALL_CHILD(PTR(evt_momikucha))
#ifdef VERSION_G8ME01
    CALL(evt_sub_get_language, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_1_e"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_2_e"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_3_e"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_LESS(2)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_1"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_2"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("p_p_3"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_hitobj_onoff, STRING("a_p_p_2"), 1, 0)
            CALL(evt_hitobj_onoff, STRING("a_p_p_3"), 1, 1)
            CALL(evt_cloud_init)
            IF_INT_EQUAL(GSWF(2457), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_2"), STRING("a_p_m_2"), PTR(evt_poster_2))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_2"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_2"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2458), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_3"), STRING("a_p_m_3"), PTR(evt_poster_3))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_3"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_3"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2459), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_4"), STRING("a_p_m_4"), PTR(evt_poster_4))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_4"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_4"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2460), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_5"), STRING("a_p_m_5"), PTR(evt_poster_5))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_5"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_5"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2461), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_6"), STRING("a_p_m_6"), PTR(evt_poster_6))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_6"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_6"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2462), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_7"), STRING("a_p_m_7"), PTR(evt_poster_7))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_7"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_7"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2463), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_8"), STRING("a_p_m_8"), PTR(evt_poster_8))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_8"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_8"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2464), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_9"), STRING("a_p_m_9"), PTR(evt_poster_9))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_9"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_9"), 1, 0)
            END_IF
            CASE_LESS(11)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_1"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("p_p_2"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_3"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_hitobj_onoff, STRING("a_p_p_2"), 1, 1)
            CALL(evt_hitobj_onoff, STRING("a_p_p_3"), 1, 0)
            CALL(evt_cloud_init)
            IF_INT_EQUAL(GSWF(2456), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_1"), STRING("a_p_m_1"), PTR(evt_poster_1))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_1"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_1"), 1, 0)
            END_IF
            CASE_DEFAULT
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("p_p_1"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_2"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_3"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_hitobj_onoff, STRING("a_p_p_2"), 1, 0)
            CALL(evt_hitobj_onoff, STRING("a_p_p_3"), 1, 0)
        END_SWITCH
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_1"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_2"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_3"), MAP_OBJ_FLAG_DISABLED)
        CALL(evt_tou_get_ranking, LW(0))
        SWITCH(LW(0))
            CASE_LESS(2)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_1_e"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_2_e"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("p_p_3_e"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_hitobj_onoff, STRING("a_p_p_2"), 1, 0)
            CALL(evt_hitobj_onoff, STRING("a_p_p_3"), 1, 1)
            CALL(evt_cloud_init)
            IF_INT_EQUAL(GSWF(2457), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_2_e"), STRING("a_p_m_2"), PTR(evt_poster_2))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_2_e"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_2"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2458), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_3_e"), STRING("a_p_m_3"), PTR(evt_poster_3))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_3_e"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_3"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2459), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_4_e"), STRING("a_p_m_4"), PTR(evt_poster_4))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_4_e"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_4"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2460), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_5_e"), STRING("a_p_m_5"), PTR(evt_poster_5))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_5_e"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_5"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2461), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_6_e"), STRING("a_p_m_6"), PTR(evt_poster_6))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_6_e"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_6"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2462), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_7_e"), STRING("a_p_m_7"), PTR(evt_poster_7))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_7_e"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_7"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2463), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_8_e"), STRING("a_p_m_8"), PTR(evt_poster_8))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_8_e"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_8"), 1, 0)
            END_IF
            IF_INT_EQUAL(GSWF(2464), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_9_e"), STRING("a_p_m_9"), PTR(evt_poster_9))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_9_e"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_9"), 1, 0)
            END_IF
            CASE_LESS(11)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_1_e"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("p_p_2_e"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_3_e"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_hitobj_onoff, STRING("a_p_p_2"), 1, 1)
            CALL(evt_hitobj_onoff, STRING("a_p_p_3"), 1, 0)
            CALL(evt_cloud_init)
            IF_INT_EQUAL(GSWF(2456), 0)
                CALL(evt_cloud_ent, 0, STRING("p_m_1_e"), STRING("a_p_m_1"), PTR(evt_poster_1))
                ELSE
                CALL(evt_mapobj_flag_onoff, 0, 1, STRING("p_m_1_e"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_hitobj_onoff, STRING("a_p_m_1"), 1, 0)
            END_IF
            CASE_DEFAULT
            CALL(evt_mapobj_flag_onoff, 1, 0, STRING("p_p_1_e"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_2_e"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("p_p_3_e"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_hitobj_onoff, STRING("a_p_p_2"), 1, 0)
            CALL(evt_hitobj_onoff, STRING("a_p_p_3"), 1, 0)
        END_SWITCH
    END_IF
    IF_INT_EQUAL(GSWF(2463), 1)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 153)
            CALL(evt_item_entry, STRING("key"), 19, -110, 200, -180, 16, -1, PTR(evt_key))
        END_IF
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("s_kaidan_7"), MAP_OBJ_FLAG_CLIP)
#else
    CALL(evt_tou_get_ranking, LW(0))
    SWITCH(LW(0))
        CASE_LESS(2)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("p_p_1")), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("p_p_2")), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING(("p_p_3")), 1)
        CALL(evt_hitobj_onoff, STRING(("a_p_p_2")), 1, 0)
        CALL(evt_hitobj_onoff, STRING(("a_p_p_3")), 1, 1)
        CALL(evt_cloud_init)
        IF_INT_EQUAL(GSWF(2457), 0)
            CALL(evt_cloud_ent, 0, STRING(("p_m_2")), STRING(("a_p_m_2")), PTR(evt_poster_2))
            ELSE
            CALL(evt_mapobj_flag_onoff, 0, 1, STRING(("p_m_2")), 1)
            CALL(evt_hitobj_onoff, STRING(("a_p_m_2")), 1, 0)
        END_IF
        IF_INT_EQUAL(GSWF(2458), 0)
            CALL(evt_cloud_ent, 0, STRING(("p_m_3")), STRING(("a_p_m_3")), PTR(evt_poster_3))
            ELSE
            CALL(evt_mapobj_flag_onoff, 0, 1, STRING(("p_m_3")), 1)
            CALL(evt_hitobj_onoff, STRING(("a_p_m_3")), 1, 0)
        END_IF
        IF_INT_EQUAL(GSWF(2459), 0)
            CALL(evt_cloud_ent, 0, STRING(("p_m_4")), STRING(("a_p_m_4")), PTR(evt_poster_4))
            ELSE
            CALL(evt_mapobj_flag_onoff, 0, 1, STRING(("p_m_4")), 1)
            CALL(evt_hitobj_onoff, STRING(("a_p_m_4")), 1, 0)
        END_IF
        IF_INT_EQUAL(GSWF(2460), 0)
            CALL(evt_cloud_ent, 0, STRING(("p_m_5")), STRING(("a_p_m_5")), PTR(evt_poster_5))
            ELSE
            CALL(evt_mapobj_flag_onoff, 0, 1, STRING(("p_m_5")), 1)
            CALL(evt_hitobj_onoff, STRING(("a_p_m_5")), 1, 0)
        END_IF
        IF_INT_EQUAL(GSWF(2461), 0)
            CALL(evt_cloud_ent, 0, STRING(("p_m_6")), STRING(("a_p_m_6")), PTR(evt_poster_6))
            ELSE
            CALL(evt_mapobj_flag_onoff, 0, 1, STRING(("p_m_6")), 1)
            CALL(evt_hitobj_onoff, STRING(("a_p_m_6")), 1, 0)
        END_IF
        IF_INT_EQUAL(GSWF(2462), 0)
            CALL(evt_cloud_ent, 0, STRING(("p_m_7")), STRING(("a_p_m_7")), PTR(evt_poster_7))
            ELSE
            CALL(evt_mapobj_flag_onoff, 0, 1, STRING(("p_m_7")), 1)
            CALL(evt_hitobj_onoff, STRING(("a_p_m_7")), 1, 0)
        END_IF
        IF_INT_EQUAL(GSWF(2463), 0)
            CALL(evt_cloud_ent, 0, STRING(("p_m_8")), STRING(("a_p_m_8")), PTR(evt_poster_8))
            ELSE
            CALL(evt_mapobj_flag_onoff, 0, 1, STRING(("p_m_8")), 1)
            CALL(evt_hitobj_onoff, STRING(("a_p_m_8")), 1, 0)
        END_IF
        IF_INT_EQUAL(GSWF(2464), 0)
            CALL(evt_cloud_ent, 0, STRING(("p_m_9")), STRING(("a_p_m_9")), PTR(evt_poster_9))
            ELSE
            CALL(evt_mapobj_flag_onoff, 0, 1, STRING(("p_m_9")), 1)
            CALL(evt_hitobj_onoff, STRING(("a_p_m_9")), 1, 0)
        END_IF
        CASE_LESS(11)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("p_p_1")), 1)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING(("p_p_2")), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("p_p_3")), 1)
        CALL(evt_hitobj_onoff, STRING(("a_p_p_2")), 1, 1)
        CALL(evt_hitobj_onoff, STRING(("a_p_p_3")), 1, 0)
        CALL(evt_cloud_init)
        IF_INT_EQUAL(GSWF(2456), 0)
            CALL(evt_cloud_ent, 0, STRING(("p_m_1")), STRING(("a_p_m_1")), PTR(evt_poster_1))
            ELSE
            CALL(evt_mapobj_flag_onoff, 0, 1, STRING(("p_m_1")), 1)
            CALL(evt_hitobj_onoff, STRING(("a_p_m_1")), 1, 0)
        END_IF
        CASE_DEFAULT
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING(("p_p_1")), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("p_p_2")), 1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING(("p_p_3")), 1)
        CALL(evt_hitobj_onoff, STRING(("a_p_p_2")), 1, 0)
        CALL(evt_hitobj_onoff, STRING(("a_p_p_3")), 1, 0)
    END_SWITCH
    IF_INT_EQUAL(GSWF(2463), 1)
        IF_INT_EQUAL(GSW_STORY_PROGRESS, 153)
            CALL(evt_item_entry, STRING(("key")), 19, -110, 200, -180, 16, -1, PTR(evt_key))
        END_IF
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING(("s_kaidan_7")), 2)
#endif
    RETURN
EVT_END()

