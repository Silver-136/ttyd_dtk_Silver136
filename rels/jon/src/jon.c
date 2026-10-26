#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "eff/eff_magic1_n64.h"
#include "evt/evt_cam.h"
#include "evt/evt_case.h"
#include "evt/evt_eff.h"
#include "evt/evt_fade.h"
#include "evt/evt_hit.h"
#include "evt/evt_img.h"
#include "evt/evt_map.h"
#include "evt/evt_mario.h"
#include "evt/evt_mobj.h"
#include "evt/evt_msg.h"
#include "evt/evt_offscreen.h"
#include "evt/evt_paper.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "party/party_motion.h"
#include "evt/evt_snd.h"
#include "evt/evt_sub.h"
#include "evt/evt_item.h"
#include "evt/evt_pouch.h"
#include "driver/mobjdrv.h"
#include "driver/mapdrv.h"
#include "system.h"
#include "driver/casedrv.h"

#include "driver/npcdrv.h"
#include "manager/evtmgr.h"
#include "manager/evtmgr_cmd.h"
#include "driver/animdrv.h"
#include "evt/evt_npc.h"
#include "string.h"
#include "driver/dispdrv.h"
#include "memory.h"
#include "mario/mariost.h"
#include "driver/camdrv.h"
#include "dolphin/gx.h"
#include "dolphin/os.h"
#include "data/mapdata.h"
#include "evt/evt_bero.h"
#include "data/npc_data.h"
#include "npc_event.h"
#include "jon.h"
#include "driver/seqdrv.h"

void raster_disp(CameraId cameraId, void* param);
typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

USER_FUNC(evt_pouch_add_coin);
USER_FUNC(evt_pouch_get_coin);
USER_FUNC(evt_snd_sfxon_3d_ex);
USER_FUNC(evt_win_coin_off);
USER_FUNC(evt_win_coin_on);
USER_FUNC(evt_win_coin_wait);
USER_FUNC(evt_win_other_select);
static USER_FUNC(gonbaba_wait_landon);
#ifdef VERSION_G8ME01
#endif
void iri_12_msgcb(s32 type, void* param);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

#ifdef VERSION_G8ME01
EvtWord jon_init_evt[400];
#else
EvtWord jon_init_evt[391];
#endif
u8 enemy_100[100];
NpcSetupInfo npcEnt_kuriboo[2];
NpcSetupInfo npcEnt_patakuri[2];
NpcSetupInfo npcEnt_hannya[2];
NpcSetupInfo npcEnt_chorobon[2];
NpcSetupInfo npcEnt_sinnosuke[2];
NpcSetupInfo npcEnt_sinemon[2];
NpcSetupInfo npcEnt_nokonoko[2];
NpcSetupInfo npcEnt_patapata[2];
NpcSetupInfo npcEnt_met[2];
NpcSetupInfo npcEnt_patamet[2];
NpcSetupInfo npcEnt_sambo[2];
NpcSetupInfo npcEnt_honenoko[2];
NpcSetupInfo npcEnt_karon[2];
NpcSetupInfo npcEnt_dokugassun[2];
NpcSetupInfo npcEnt_piders[2];
NpcSetupInfo npcEnt_teresa[2];
NpcSetupInfo npcEnt_hbom[2];
NpcSetupInfo npcEnt_bubble[2];
NpcSetupInfo npcEnt_wanwan[2];
NpcSetupInfo npcEnt_mahoon[2];
NpcSetupInfo npcEnt_pakkun[2];
NpcSetupInfo npcEnt_togenoko[2];
NpcSetupInfo npcEnt_basabasa2[2];
NpcSetupInfo npcEnt_togedaruma[2];
NpcSetupInfo npcEnt_pansy[2];
NpcSetupInfo npcEnt_zonbaba[2];
EvtWord enemy_setup[1055];
s32 treasure_tbl[10];
EvtWord evt_open_box[46];
static EvtWord move_event[55];
EvtWord get_map_name[111];
EvtWord talk_idouya[141];
NpcSetupInfo npcEnt_idouya[2];
EvtWord idouya_setup[38];
#ifdef VERSION_G8ME01
EvtWord talk_gyousyou[219];
#else
EvtWord talk_gyousyou[207];
#endif
NpcSetupInfo npcEnt_gyousyou[2];
EvtWord gyousyou_setup[47];
EvtWord shikake_setup[117];
EvtWord dokan_open[74];
EvtWord dokan_open2[70];
static EvtWord evt_kanban[25];
static EvtWord evt_kanban2[20];
EvtWord floor_inc[10];
EvtWord floor_clear[8];
BeroEntry bero_boss[3];
EvtWord setup_boss[9];
BeroEntry bero_return[4];
BeroEntry bero_return2[2];
BeroEntry bero_return3[2];
EvtWord setup_return[190];
BeroEntry bero_prev_return[3];
BeroEntry bero_prev_return2[2];
BeroEntry bero_prev_return3[2];
BeroEntry bero_prev_return4[2];
EvtWord setup_prev_return[69];
BeroEntry bero_normal[3];
BeroEntry bero_normal2[2];
BeroEntry bero_normal3[2];
EvtWord setup_normal[53];
EvtWord iri_12_dokan_in_sub[202];
EvtWord iri_12_dokan_in[16];
EvtWord iri_12_makkino_return_dokan[193];
EvtWord iri_12_makkino_return[31];
EvtWord iri_12_makkino_return_b100[42];
EvtWord iri_12_makkino_return_zonbaba[44];
EvtWord iri_12_makkino_move_dokan_side[94];
EvtWord iri_12_enemy_dead_event[18];
EvtWord iri_12_makkino_urouro[25];
EvtWord iri_12_makkino_buruburu[44];
EvtWord iri_12_makkino_okiru[33];
static EvtWord iri_12_makkino_talk[55];
EvtWord iri_12_makkino_fall[79];
EvtWord iri_12_makkino_fall_return[71];
EvtWord iri_12_init_fall[38];
static EvtWord iri_12_init[65];
EvtWord evt_iri_30_touch_rakugaki[14];
EvtWord evt_iri_30_bomb_rakugaki[154];
static EvtWord gonbaba_standup_shake[23];
static EvtWord gonbaba_fog_change[87];
static EvtWord gonbaba_down_shake[83];
EvtWord zonbaba_first_event[677];

EVT_BEGIN(jon_init_evt)
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), 2)
        CALL(evt_sub_get_mapname, LW(0))
        IF_STR_EQUAL(LW(0), STRING("jon_00"))
            SET_VALUE(GSW_PIT_FLOOR, 0)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("jon_01"))
            SET_VALUE(GSW_PIT_FLOOR, 50)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("jon_02"))
            SET_VALUE(GSW_PIT_FLOOR, 80)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("jon_03"))
            SET_VALUE(GSW_PIT_FLOOR, 9)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("jon_04"))
            SET_VALUE(GSW_PIT_FLOOR, 49)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("jon_05"))
            SET_VALUE(GSW_PIT_FLOOR, 79)
        END_IF
        IF_STR_EQUAL(LW(0), STRING("jon_06"))
            SET_VALUE(GSW_PIT_FLOOR, 99)
        END_IF
    END_IF
    SET_VALUE(LSW(0), GSW_PIT_FLOOR)
    SET_VALUE(LW(0), LSW(0))
    ADD(LW(0), 1)
    IF_INT_GREATER(LW(0), GSW(33))
        SET_VALUE(GSW(33), LW(0))
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_kanban"), 0, PTR(evt_kanban), 0)
    CALL_CHILD(PTR(idouya_setup))
    CALL_CHILD(PTR(gyousyou_setup))
    CALL_CHILD(PTR(enemy_setup))
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SET_VALUE(LW(1), LW(0))
    MODULO(LW(1), 10)
    SET_VALUE(LW(2), LW(0))
    DIVIDE(LW(2), 10)
    SWITCH(LW(1))
        CASE_EQUAL(0)
        IF_INT_EQUAL(LW(2), 10)
            CALL_CHILD(PTR(setup_boss))
            ELSE
            CALL_CHILD(PTR(setup_return))
            CALL(evt_mobj_floatswitch_blue, 0, STRING("sw"), -75, 60, 0, PTR(dokan_open2), 0)
        END_IF
        CASE_EQUAL(9)
        CALL_CHILD(PTR(setup_prev_return))
        CALL(evt_npc_check, STRING("敵"), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 1)
            CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
            CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, 30, 0)
            CALL(evt_hit_bind_update, STRING("a_dokan_1"))
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        CALL(evt_npc_check, STRING("移動屋"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
            CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, -30, 0)
            CALL(evt_hit_bind_update, STRING("a_dokan_1"))
            CALL(evt_mobj_floatswitch_blue, 0, STRING("idou_sw"), 75, 60, 0, PTR(dokan_open), 0)
        END_IF
        CASE_DEFAULT
        CALL_CHILD(PTR(setup_normal))
        CALL(evt_npc_check, STRING("敵"), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 1)
            CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
            CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, 30, 0)
            CALL(evt_hit_bind_update, STRING("a_dokan_1"))
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
        CALL(evt_npc_check, STRING("移動屋"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
            CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, -30, 0)
            CALL(evt_hit_bind_update, STRING("a_dokan_1"))
            CALL(evt_mobj_floatswitch_blue, 0, STRING("idou_sw"), 75, 60, 0, PTR(dokan_open), 0)
        END_IF
    END_SWITCH
    CALL_CHILD(PTR(iri_12_init))
    CALL(evt_sub_get_mapname, LW(0))
    IF_STR_EQUAL(LW(0), STRING("jon_04"))
        IF_INT_EQUAL(GSW_PIT_FLOOR, 49)
            IF_INT_EQUAL(GSWF(5404), 0)
#ifdef VERSION_G8ME01
                IF_INT_NOT_EQUAL(GSW(1420), 30)
                    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
                    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
                    ELSE
#endif
                SET_VALUE(LF(0), 0)
                IF_INT_EQUAL(GSW(1420), 30)
                    IF_INT_EQUAL(GSWF(5406), 1)
                        SET_VALUE(LF(0), 1)
                    END_IF
                END_IF
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
                    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_rakugaki"), 0, PTR(evt_iri_30_touch_rakugaki), LW(0))
#ifndef VERSION_G8ME01
                    IF_INT_EQUAL(GSW(1420), 30)
                        IF_INT_EQUAL(GSWF(5405), 1)
#endif
                            CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("a_rakugaki"), 0, PTR(evt_iri_30_bomb_rakugaki), LW(1))
                            CALL(evt_set_case_wrk, LW(1), 15, LW(0))
#ifndef VERSION_G8ME01
                        END_IF
                    END_IF
#endif
                    ELSE
                    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("before"), MAP_OBJ_FLAG_HIDDEN)
                    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
#ifdef VERSION_G8ME01
                    END_IF
#endif
                END_IF
                ELSE
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("before"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
            END_IF
            ELSE
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(GSW_PIT_FLOOR, 99)
        CALL(evt_snd_bgmon, 512, STRING("BGM_STG0_100DN1"))
        CALL(evt_snd_envoff, 512)
        CALL(evt_snd_set_rev_mode, 1)
        ELSE
        CALL(evt_snd_bgmoff, 512)
        CALL(evt_snd_envon, 272, STRING("ENV_STG0_DAN1"))
        CALL(evt_snd_set_rev_mode, 2)
    END_IF
    RETURN
EVT_END()

u8 enemy_100[] = {
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 41, 42, 43, 44, 45, 46, 47, 48, 49, 46, 47, 48, 49, 46, 51,
};

NpcSetupInfo npcEnt_kuriboo[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        kuriboo_init_event, // initEvent
        kuriboo_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        kuriboo_find_event, // findEvent
        kuriboo_lost_event, // lostEvent
        kuriboo_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_patakuri[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        patakuri_init_event, // initEvent
        patakuri_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        patakuri_find_event, // findEvent
        patakuri_lost_event, // lostEvent
        patakuri_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_hannya[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        hannya_init_event, // initEvent
        hannya_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        hannya_find_event, // findEvent
        hannya_lost_event, // lostEvent
        hannya_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_chorobon[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        chorobon_init_event, // initEvent
        chorobon_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        chorobon_find_event, // findEvent
        chorobon_lost_event, // lostEvent
        chorobon_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_sinnosuke[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        sinnosuke_init_event, // initEvent
        sinnosuke_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        sinnosuke_find_event, // findEvent
        sinnosuke_lost_event, // lostEvent
        sinnosuke_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_sinemon[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        sinemon_init_event, // initEvent
        sinemon_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        sinemon_find_event, // findEvent
        sinemon_lost_event, // lostEvent
        sinemon_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_nokonoko[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        nokonoko_init_event, // initEvent
        nokonoko_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        nokonoko_find_event, // findEvent
        nokonoko_lost_event, // lostEvent
        nokonoko_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_patapata[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        patapata_init_event, // initEvent
        patapata_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        patapata_find_event, // findEvent
        patapata_lost_event, // lostEvent
        patapata_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_met[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        met_init_event, // initEvent
        met_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        met_find_event, // findEvent
        met_lost_event, // lostEvent
        met_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_patamet[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        patamet_init_event, // initEvent
        patamet_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        patamet_find_event, // findEvent
        patamet_lost_event, // lostEvent
        patamet_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_sambo[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        sambo_init_event, // initEvent
        sambo_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        sambo_find_event, // findEvent
        sambo_lost_event, // lostEvent
        sambo_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_honenoko[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        honenoko2_init_event, // initEvent
        honenoko2_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        honenoko2_find_event, // findEvent
        honenoko2_lost_event, // lostEvent
        honenoko2_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_karon[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        karon_init_event, // initEvent
        karon_move_event, // regularEvent
        NULL, // talkEvent
        karon_dead_event, // deadEvent
        karon_find_event, // findEvent
        karon_lost_event, // lostEvent
        karon_return_event, // returnEvent
        karon_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_dokugassun[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        dokugassun_init_event, // initEvent
        dokugassun_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        dokugassun_find_event, // findEvent
        dokugassun_lost_event, // lostEvent
        dokugassun_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_piders[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        piders_init_event, // initEvent
        piders_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        piders_find_event, // findEvent
        NULL, NULL, // lostEvent..returnEvent
        piders_blow_event, // blowEvent
        2, // territoryType
        {0.0f, 140.0f, 0.0f}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_teresa[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        teresa_init_event, // initEvent
        teresa_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        teresa_find_event, // findEvent
        teresa_lost_event, // lostEvent
        teresa_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_hbom[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        hbom_init_event, // initEvent
        hbom_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        hbom_find_event, // findEvent
        hbom_lost_event, // lostEvent
        hbom_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_bubble[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        bubble_init_event, // initEvent
        bubble_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        bubble_find_event, // findEvent
        bubble_lost_event, // lostEvent
        bubble_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_wanwan[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        wanwan_init_event, // initEvent
        wanwan_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        wanwan_find_event, // findEvent
        NULL, NULL, // lostEvent..returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_mahoon[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        mahoon_init_event, // initEvent
        mahoon_move_event, // regularEvent
        NULL, // talkEvent
        mahoon_dead_event, // deadEvent
        mahoon_find_event, // findEvent
        mahoon_lost_event, // lostEvent
        mahoon_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_pakkun[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        pakkun_init_event, // initEvent
        pakkun_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        pakkun_find_event, // findEvent
        NULL, // lostEvent
        pakkun_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_togenoko[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        togenoko_init_event, // initEvent
        togenoko_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        togenoko_find_event, // findEvent
        togenoko_lost_event, // lostEvent
        togenoko_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_basabasa2[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        basabasa2_init_event, // initEvent
        basabasa2_move_event, // regularEvent
        NULL, // talkEvent
        basabasa2_dead_event, // deadEvent
        basabasa2_find_event, // findEvent
        basabasa2_lost_event, // lostEvent
        basabasa2_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_togedaruma[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        togedaruma_init_event, // initEvent
        togedaruma_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        togedaruma_find_event, // findEvent
        togedaruma_lost_event, // lostEvent
        togedaruma_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_pansy[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, // reactionFlags
        pansy_init_event, // initEvent
        pansy_move_event, // regularEvent
        NULL, // talkEvent
        enemy_common_dead_event, // deadEvent
        pansy_find_event, // findEvent
        pansy_lost_event, // lostEvent
        pansy_return_event, // returnEvent
        enemy_common_blow_event, // blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

NpcSetupInfo npcEnt_zonbaba[] = {
    {
        /*"敵"*/"\x93\x47", // name
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, NULL, NULL, NULL, // reactionFlags..talkEvent
        enemy_common_dead_event, // deadEvent
        NULL, NULL, NULL, NULL, // findEvent..blowEvent
        2, // territoryType
        {0}, // territoryBase
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

EVT_BEGIN(enemy_setup)
    CALL(evt_npc_check, STRING("移動屋"), LW(0))
    IF_INT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    IF_INT_NOT_EQUAL(LW(0), 100)
        MODULO(LW(0), 10)
        IF_INT_EQUAL(LW(0), 0)
            RETURN
        END_IF
    END_IF
    CALL(enemy_info, GSW_PIT_FLOOR, LW(0), LW(1), LW(2))
    CALL(evt_npc_entry, STRING("敵"), LW(0))
    CALL(evt_npc_set_tribe, STRING("敵"), LW(1))
    CALL(evt_sub_random, 100, LW(3))
    IF_INT_LESS(LW(3), 50)
        CALL(evt_sub_random, 50, LW(3))
        ADD(LW(3), 80)
        CALL(evt_sub_random, 200, LW(4))
        ADD(LW(4), -100)
        ELSE
        CALL(evt_sub_random, 50, LW(3))
        ADD(LW(3), -130)
        CALL(evt_sub_random, 200, LW(4))
        ADD(LW(4), -100)
    END_IF
    IF_STR_EQUAL(LW(1), STRING("クリボー"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("トゲクリ"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ハイパークリボー"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ハイパートゲクリ"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ヤミクリボー"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ヤミトゲクリ"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("パタクリ"))
        CALL(evt_npc_setup, PTR(npcEnt_patakuri))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 50, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ハイパーパタクリ"))
        CALL(evt_npc_setup, PTR(npcEnt_patakuri))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 50, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ヤミパタクリ"))
        CALL(evt_npc_setup, PTR(npcEnt_patakuri))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 50, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ヒンニャー"))
        CALL(evt_npc_setup, PTR(npcEnt_hannya))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ハンニャー"))
        CALL(evt_npc_setup, PTR(npcEnt_hannya))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ヘンニャー"))
        CALL(evt_npc_setup, PTR(npcEnt_hannya))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("チョロボン"))
        CALL(evt_npc_setup, PTR(npcEnt_chorobon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("グリーンチョロボン"))
        CALL(evt_npc_setup, PTR(npcEnt_chorobon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("フラワーチョロボン"))
        CALL(evt_npc_setup, PTR(npcEnt_chorobon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("シンエモン"))
        CALL(evt_npc_setup, PTR(npcEnt_sinemon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ハイパーシンエモン"))
        CALL(evt_npc_setup, PTR(npcEnt_sinemon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("モノクロシンエモン"))
        CALL(evt_npc_setup, PTR(npcEnt_sinemon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("シンノスケ"))
        CALL(evt_npc_setup, PTR(npcEnt_sinnosuke))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ハイパーシンノスケ"))
        CALL(evt_npc_setup, PTR(npcEnt_sinnosuke))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ノコノコ敵"))
        CALL(evt_npc_setup, PTR(npcEnt_nokonoko))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ヤミノコノコ"))
        CALL(evt_npc_setup, PTR(npcEnt_nokonoko))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ウラノコ"))
        CALL(evt_npc_setup, PTR(npcEnt_nokonoko))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("パタパタ"))
        CALL(evt_npc_setup, PTR(npcEnt_patapata))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 50, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ヤミパタパタ"))
        CALL(evt_npc_setup, PTR(npcEnt_patapata))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 50, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("メット"))
        CALL(evt_npc_setup, PTR(npcEnt_met))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("パタメット"))
        CALL(evt_npc_setup, PTR(npcEnt_patamet))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 50, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("パタトゲメット"))
        CALL(evt_npc_setup, PTR(npcEnt_patamet))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 50, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("サンボ"))
        CALL(evt_npc_setup, PTR(npcEnt_sambo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("サンボマミー"))
        CALL(evt_npc_setup, PTR(npcEnt_sambo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ホネノコ"))
        CALL(evt_npc_setup, PTR(npcEnt_honenoko))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("カロン"))
        CALL(evt_npc_setup, PTR(npcEnt_karon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ドクガッスン"))
        CALL(evt_npc_setup, PTR(npcEnt_dokugassun))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 10, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("クロクモーン"))
        CALL(evt_npc_setup, PTR(npcEnt_dokugassun))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 10, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ブリザード"))
        CALL(evt_npc_setup, PTR(npcEnt_dokugassun))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 10, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("モノクロクロクモーン"))
        CALL(evt_npc_setup, PTR(npcEnt_dokugassun))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 10, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("パイダース"))
        CALL(evt_npc_setup, PTR(npcEnt_piders))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 140, LW(4))
        CALL(evt_npc_set_home_position, STRING("敵"), LW(3), 140, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("チュランタラー"))
        CALL(evt_npc_setup, PTR(npcEnt_piders))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 140, LW(4))
        CALL(evt_npc_set_home_position, STRING("敵"), LW(3), 140, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ボロドー"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("バッジボロドー"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ジュゲム"))
        CALL(evt_npc_setup, PTR(npcEnt_dokugassun))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 10, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ボムへい"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("テレサ"))
        CALL(evt_npc_setup, PTR(npcEnt_teresa))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 30, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("パープルテレサ"))
        CALL(evt_npc_setup, PTR(npcEnt_teresa))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 30, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ヘビーボム"))
        CALL(evt_npc_setup, PTR(npcEnt_hbom))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ジャイアントボム"))
        CALL(evt_npc_setup, PTR(npcEnt_hbom))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("バブル"))
        CALL(evt_npc_setup, PTR(npcEnt_bubble))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 20, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ファントム"))
        CALL(evt_npc_setup, PTR(npcEnt_bubble))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 20, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ワンワン"))
        CALL(wanwan_set_home_pos, LW(3), 0, LW(4))
        CALL(evt_npc_setup, PTR(npcEnt_wanwan))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("バーストワンワン"))
        CALL(wanwan_set_home_pos, LW(3), 0, LW(4))
        CALL(evt_npc_setup, PTR(npcEnt_wanwan))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ジュゲム"))
        CALL(evt_npc_setup, PTR(npcEnt_bubble))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 20, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ハイパージュゲム"))
        CALL(evt_npc_setup, PTR(npcEnt_bubble))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 20, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("マホーン"))
        CALL(evt_npc_setup, PTR(npcEnt_mahoon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 20, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("スーパーマホーン"))
        CALL(evt_npc_setup, PTR(npcEnt_mahoon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 20, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("マホーンカスタム"))
        CALL(evt_npc_setup, PTR(npcEnt_mahoon))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 20, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("パックンフラワー"))
        CALL(evt_npc_setup, PTR(npcEnt_pakkun))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("アイスパックン"))
        CALL(evt_npc_setup, PTR(npcEnt_pakkun))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ダークキーパー"))
        CALL(evt_npc_setup, PTR(npcEnt_kuriboo))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("トゲノコ"))
        CALL(evt_npc_setup, PTR(npcEnt_togenoko))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("トゲノコエース"))
        CALL(evt_npc_setup, PTR(npcEnt_togenoko))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("バサバサ"))
        CALL(evt_npc_setup, PTR(npcEnt_basabasa2))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 80, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("バサバサチュルル"))
        CALL(evt_npc_setup, PTR(npcEnt_basabasa2))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 80, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("バサバサグリーン"))
        CALL(evt_npc_setup, PTR(npcEnt_basabasa2))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 80, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("トゲダルマー"))
        CALL(evt_npc_setup, PTR(npcEnt_togedaruma))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ヤミトゲダルマー"))
        CALL(evt_npc_setup, PTR(npcEnt_togedaruma))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("パンジーさん"))
        CALL(evt_npc_setup, PTR(npcEnt_pansy))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("きらめくパンジーさん"))
        CALL(evt_npc_setup, PTR(npcEnt_pansy))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 0, LW(4))
    END_IF
    IF_STR_EQUAL(LW(1), STRING("ゾンババ"))
        CALL(evt_npc_setup, PTR(npcEnt_zonbaba))
        CALL(evt_npc_set_position, STRING("敵"), 0, 0, 0)
        CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_H_3"))
        CALL(evt_npc_flag_onoff, 1, STRING("敵"), NPC_FLAG_NO_CULL | NPC_FLAG_NO_VIEW_CORRECTION)
        CALL(evt_npc_pera_onoff, STRING("敵"), 0)
        CALL(evt_npc_set_ry, STRING("敵"), 0)
        SPAWN_EVENT(PTR(zonbaba_first_event))
    END_IF
    CALL(evt_npc_get_position, STRING("敵"), LW(3), LW(4), LW(5))
    CALL(evt_npc_set_battle_info, STRING("敵"), LW(2))
    CALL(setup_npc_ex_para, STRING("敵"))
    CALL(yattukeFlag)
    BEGIN_INLINE_EVENT
        LABEL(1)
        WAIT_FRAMES(1)
        CALL(evt_npc_status_check, STRING("敵"), 4, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            GOTO(1)
        END_IF
        SET_VALUE(LW(0), GSW_PIT_FLOOR)
        ADD(LW(0), 1)
        IF_INT_NOT_EQUAL(LW(0), 100)
            SPAWN_EVENT(PTR(dokan_open))
            ELSE
            SET_VALUE(GSWF(5085), 1)
        END_IF
        SPAWN_EVENT(PTR(iri_12_enemy_dead_event))
    END_INLINE_EVENT
    RETURN
EVT_END()

s32 treasure_tbl[] = {
    245, 252, 287, 306, 1, 254, 255, 322,
    303, 291,
};

EVT_BEGIN(evt_open_box)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    DIVIDE(LW(0), 10)
    SET_INT_READ_ADDR(PTR(treasure_tbl))
    LOOP(LW(0))
        READ_INT(LW(1))
    END_LOOP
    CALL(evt_mobj_get_position, STRING("box"), LW(10), LW(11), LW(12))
    CALL(evt_item_entry, STRING("item"), LW(1), LW(10), LW(11), LW(12), 17, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("item"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(move_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_sub_random, 360, LW(3))
    LABEL(0)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(500)
    CALL(evt_npc_reverse_ry, STRING("me"))
    WAIT_MS(1000)
    CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
    CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
    CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(100.0f))
    CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 2000, FLOAT(40.0f), 4)
    GOTO(0)
    RETURN
EVT_END()

EVT_BEGIN(get_map_name)
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SWITCH(LW(0))
        CASE_LESS(10)
        SET_VALUE(LW(0), STRING("jon_00"))
        CASE_EQUAL(10)
        SET_VALUE(LW(0), STRING("jon_03"))
        CASE_LESS(20)
        SET_VALUE(LW(0), STRING("jon_00"))
        CASE_EQUAL(20)
        SET_VALUE(LW(0), STRING("jon_03"))
        CASE_LESS(30)
        SET_VALUE(LW(0), STRING("jon_00"))
        CASE_EQUAL(30)
        SET_VALUE(LW(0), STRING("jon_03"))
        CASE_LESS(40)
        SET_VALUE(LW(0), STRING("jon_00"))
        CASE_EQUAL(40)
        SET_VALUE(LW(0), STRING("jon_03"))
        CASE_LESS(50)
        SET_VALUE(LW(0), STRING("jon_00"))
        CASE_EQUAL(50)
        SET_VALUE(LW(0), STRING("jon_04"))
        CASE_LESS(60)
        SET_VALUE(LW(0), STRING("jon_01"))
        CASE_EQUAL(60)
        SET_VALUE(LW(0), STRING("jon_04"))
        CASE_LESS(70)
        SET_VALUE(LW(0), STRING("jon_01"))
        CASE_EQUAL(70)
        SET_VALUE(LW(0), STRING("jon_04"))
        CASE_LESS(80)
        SET_VALUE(LW(0), STRING("jon_01"))
        CASE_EQUAL(80)
        SET_VALUE(LW(0), STRING("jon_05"))
        CASE_LESS(90)
        SET_VALUE(LW(0), STRING("jon_02"))
        CASE_EQUAL(90)
        SET_VALUE(LW(0), STRING("jon_05"))
        CASE_LESS(100)
        SET_VALUE(LW(0), STRING("jon_02"))
        CASE_EQUAL(100)
        SET_VALUE(LW(0), STRING("jon_06"))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(talk_idouya)
    CALL(evt_msg_print, 0, STRING("msg_jon_mover"), 0, 0)
    CALL(evt_msg_select, 0, STRING("msg_jon_mover_select"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        SET_VALUE(LW(2), 10)
        CASE_EQUAL(1)
        SET_VALUE(LW(2), 30)
        CASE_EQUAL(2)
        SET_VALUE(LW(2), 10)
        CASE_EQUAL(3)
        CALL(evt_msg_continue)
        RETURN
    END_SWITCH
    CALL(evt_pouch_get_coin, LW(3))
    IF_INT_LESS(LW(3), LW(2))
        CALL(evt_msg_print_add, 0, STRING("msg_jon_mover_nocoin"))
        RETURN
    END_IF
    MULTIPLY(LW(2), -1)
    CALL(evt_pouch_add_coin, LW(2))
    SWITCH(LW(0))
        CASE_EQUAL(0)
        ADD(GSW_PIT_FLOOR, 2)
        CALL_CHILD(PTR(get_map_name))
        SET_VALUE(LW(1), STRING("dokan_2"))
        CASE_EQUAL(1)
        ADD(GSW_PIT_FLOOR, 5)
        CALL_CHILD(PTR(get_map_name))
        SET_VALUE(LW(1), STRING("dokan_2"))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("tik_06"))
        SET_VALUE(LW(1), STRING("dokan_3"))
    END_SWITCH
    IF_STR_EQUAL(LW(0), STRING("tik_06"))
        IF_INT_EQUAL(GSW(1420), 12)
            IF_INT_EQUAL(GSWF(5406), 1)
                SET_VALUE(GSWF(5407), 1)
            END_IF
        END_IF
    END_IF
    CALL(evt_msg_print_add, 1, STRING("<scrl_auto><once_stop>"))
    CALL(evt_msg_print_add, 0, STRING("msg_jon_mover_2"))
    BEGIN_INLINE_EVENT
        CALL(evt_raster)
    END_INLINE_EVENT
    WAIT_MS(5000)
    CALL(evt_fade_set_mapchange_type, 0, 10, 300, 9, 300)
    CALL(evt_fade_set_mapchange_type, 1, 10, 300, 9, 300)
    IF_STR_EQUAL(LW(0), STRING("tik_06"))
        SET_VALUE(GSW_PIT_FLOOR, 0)
    END_IF
    CALL(evt_mario_goodbye_party, 0)
    CALL(evt_bero_mapchange, LW(0), LW(1))
    RETURN
EVT_END()

NpcSetupInfo npcEnt_idouya[] = {
    {
        /*"移動屋"*/"\x88\xDA\x93\xAE\x89\xAE", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, NULL, // reactionFlags..initEvent
        move_event, // regularEvent
        talk_idouya, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(idouya_setup)
    IF_INT_EQUAL(GSW_PIT_FLOOR, 99)
        RETURN
    END_IF
    IF_INT_GREATER_OR_EQUAL(GSW_PIT_FLOOR, 95)
        RETURN
    END_IF
    CALL(evt_sub_random, 1000, LW(0))
    IF_INT_GREATER(LW(0), 50)
        RETURN
    END_IF
    CALL(evt_npc_entry, STRING("移動屋"), STRING("c_pjoho"))
    CALL(evt_npc_set_tribe, STRING("移動屋"), STRING("プニ情報屋"))
    CALL(evt_npc_setup, PTR(npcEnt_idouya))
    CALL(evt_npc_set_position, STRING("移動屋"), -100, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(talk_gyousyou)
    CALL(evt_msg_print, 0, STRING("100kai_item_01"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("100kai_item_00"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("100kai_item_02"))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    LABEL(0)
    CALL(evt_win_coin_on, 0, LW(12))
    CALL(evt_win_other_select, 15)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print, 0, STRING("100kai_item_10"), 0, STRING("me"))
        RETURN
    END_IF
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print, 0, STRING("100kai_item_03"), 0, STRING("me"))
        GOTO(0)
    END_IF
    IF_INT_GREATER_OR_EQUAL(LW(1), 240)
        CALL(evt_sub_get_language, LW(0))
#ifdef VERSION_G8ME01
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(2), LW(3), LW(4))
            CASE_DEFAULT
#else
        IF_INT_NOT_EQUAL(LW(0), 0)
#endif
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(3), LW(2), LW(4))
#ifdef VERSION_G8ME01
        END_SWITCH
#else
            ELSE
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(2), LW(3), LW(4))
        END_IF
#endif
        ELSE
        CALL(evt_sub_get_language, LW(0))
#ifdef VERSION_G8ME01
        SWITCH(LW(0))
            CASE_EQUAL(0)
#else
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(3), LW(2))
            ELSE
#endif
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(2), LW(3))
#ifdef VERSION_G8ME01
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("100kai_item_04"), LW(3))
            CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
            CALL(evt_msg_print, 1, LW(14), 0, STRING("me"))
        END_SWITCH
#else
        END_IF
#endif
    END_IF
    CALL(evt_msg_select, 0, STRING("100kai_item_00"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print_add, 0, STRING("100kai_item_05"))
        RETURN
    END_IF
    CALL(evt_pouch_add_item, LW(1), LW(0))
    IF_INT_EQUAL(LW(0), -1)
        CALL(evt_win_coin_off, LW(12))
        IF_INT_GREATER_OR_EQUAL(LW(1), 240)
            CALL(evt_msg_print_add, 0, STRING("100kai_item_09"))
            ELSE
            CALL(evt_msg_print_add, 0, STRING("100kai_item_09"))
        END_IF
        RETURN
    END_IF
    MULTIPLY(LW(3), -1)
    CALL(evt_pouch_add_coin, LW(3))
    CALL(evt_win_coin_wait, LW(12))
    WAIT_MS(200)
    CALL(evt_win_coin_off, LW(12))
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("100kai_item_06"))
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("100kai_item_07"))
    CALL(evt_msg_select, 0, STRING("100kai_item_00"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("100kai_item_08"))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    GOTO(0)
    RETURN
EVT_END()

NpcSetupInfo npcEnt_gyousyou[] = {
    {
        /*"行商人"*/"\x8D\x73\x8F\xA4\x90\x6C", // name
        NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, NULL, // reactionFlags..initEvent
        move_event, // regularEvent
        talk_gyousyou, // talkEvent
        NULL, NULL, NULL, NULL, NULL, 0, // deadEvent..territoryType
        {0}, // territoryBase
        {0}, // territoryLoiter
        0.0f, 0.0f, 0.0f, 0.0f, 0, // searchRange..battleInfoId
    },
    {0},
};

EVT_BEGIN(gyousyou_setup)
    IF_INT_EQUAL(GSW_PIT_FLOOR, 99)
        RETURN
    END_IF
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    MODULO(LW(0), 10)
    IF_INT_NOT_EQUAL(LW(0), 0)
        RETURN
    END_IF
    CALL(evt_sub_random, 1000, LW(0))
    IF_INT_GREATER(LW(0), 330)
        RETURN
    END_IF
    CALL(evt_npc_entry, STRING("行商人"), STRING("c_botta"))
    CALL(evt_npc_set_tribe, STRING("行商人"), STRING("ボッタクール"))
    CALL(evt_npc_setup, PTR(npcEnt_gyousyou))
    CALL(evt_npc_set_position, STRING("行商人"), 100, 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(shikake_setup)
    IF_INT_EQUAL(GSW_PIT_FLOOR, 99)
        RETURN
    END_IF
    CALL(evt_npc_check, STRING("敵"), LW(0))
    IF_INT_NOT_EQUAL(LW(0), 1)
        RETURN
    END_IF
    CALL(evt_sub_random, 1000, LW(0))
    IF_INT_GREATER(LW(0), 200)
        RETURN
    END_IF
    BEGIN_INLINE_EVENT
        CALL(evt_mario_key_onoff, 0)
        CALL(evt_seq_wait, SEQ_GAME)
        CALL(evt_bero_exec_wait, 65536)
        CALL(evt_npc_stop_for_event)
        CALL(evt_mario_wait_landon)
        WAIT_MS(100)
        CALL(evt_fade_entry, 17, 150, 0, 0, 0)
        CALL(evt_fade_end_wait)
        CALL(evt_fade_entry, 16, 150, 0, 0, 0)
        CALL(evt_fade_end_wait)
        WAIT_MS(200)
        CALL(evt_sub_random, 1000, LW(0))
        SWITCH(LW(0))
            CASE_LESS(200)
            CALL(evt_msg_print, 0, STRING("msg_jon_trick_1"), 0, 0)
            CALL(shikakeFlag, 0)
            CASE_LESS(400)
            CALL(evt_msg_print, 0, STRING("msg_jon_trick_2"), 0, 0)
            CALL(shikakeFlag, 1)
            CASE_LESS(600)
            CALL(evt_msg_print, 0, STRING("msg_jon_trick_3"), 0, 0)
            CALL(shikakeFlag, 2)
            CASE_DEFAULT
            CALL(evt_msg_print, 0, STRING("msg_jon_trick_4"), 0, 0)
            CALL(shikakeFlag, 3)
        END_SWITCH
        CALL(evt_mario_key_onoff, 1)
        CALL(evt_npc_start_for_event)
    END_INLINE_EVENT
    RETURN
EVT_END()

EVT_BEGIN(dokan_open)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1000)
#ifdef VERSION_G8ME01
    WAIT_MS(1000)
#else
    CALL(evt_cam_letter_box_wait)
#endif
    WAIT_MS(500)
    SET_VALUE(LW(2), STRING("a_dokan_1"))
    SET_VALUE(LW(3), STRING("dokan_1_s"))
    CALL(evt_mapobj_get_position, LW(3), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(7), LW(8), LW(9), 0)
    CALL(evt_hit_bind_mapobj, LW(2), LW(3))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 300, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_trans, 1, LW(3), 0, LW(0), 0)
    CALL(evt_hit_bind_update, LW(2))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(dokan_open2)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(400)
    SET_VALUE(LW(2), STRING("a_dokan_3"))
    SET_VALUE(LW(3), STRING("dokan_1_s"))
    CALL(evt_mapobj_get_position, LW(3), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(7), LW(8), LW(9), 0)
    CALL(evt_hit_bind_mapobj, LW(2), LW(3))
    CALL(evt_sub_intpl_msec_init, EASING_COS_IN_OUT, 0, 300, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_trans, 1, LW(3), 0, LW(0), 0)
    CALL(evt_hit_bind_update, LW(2))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kanban)
    CALL(evt_npc_stop_for_event)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    CALL(evt_msg_print_insert, 0, STRING("msg_jon_kanban_1"), 0, 0, LW(0))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_start_for_event)
    RETURN
EVT_END()

static EVT_BEGIN(evt_kanban2)
    CALL(evt_npc_stop_for_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_msg_print, 0, STRING("msg_jon_kanban_2"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_start_for_event)
    RETURN
EVT_END()

EVT_BEGIN(floor_inc)
    SPAWN_EVENT(PTR(iri_12_dokan_in))
    ADD(GSW_PIT_FLOOR, 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

EVT_BEGIN(floor_clear)
    SET_VALUE(GSW_PIT_FLOOR, 0)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

BeroEntry bero_boss[] = {
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        8, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, NULL, // exitEvent..nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        floor_clear, // exitEvent
        "tik_06", // nextMapName
        "dokan_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(setup_boss)
    SET_VALUE(LW(0), PTR(bero_boss))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    RETURN
EVT_END()

BeroEntry bero_return[] = {
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        8, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, NULL, // exitEvent..nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        2, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        10, // caseType
        floor_clear, // exitEvent
        "tik_06", // nextMapName
        "dokan_3", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_00", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

BeroEntry bero_return2[] = {
    {
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_01", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

BeroEntry bero_return3[] = {
    {
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_02", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(setup_return)
    SET_VALUE(LW(0), PTR(bero_return))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SWITCH(LW(0))
        CASE_LESS(50)
        CASE_BETWEEN(50, 70)
        SET_VALUE(LW(0), STRING("dokan_3"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan_3"), PTR(bero_return2))
        SET_VALUE(LW(0), STRING("dokan_3"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CASE_GREATER(70)
        SET_VALUE(LW(0), STRING("dokan_3"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan_3"), PTR(bero_return3))
        SET_VALUE(LW(0), STRING("dokan_3"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_SWITCH
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SWITCH(LW(0))
        CASE_EQUAL(10)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, 1, 0, PTR(evt_open_box), GSWF(5075))
        CASE_EQUAL(20)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, 1, 0, PTR(evt_open_box), GSWF(5076))
        CASE_EQUAL(30)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, 1, 0, PTR(evt_open_box), GSWF(5077))
        CASE_EQUAL(40)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, 1, 0, PTR(evt_open_box), GSWF(5078))
        CASE_EQUAL(50)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, 1, 0, PTR(evt_open_box), GSWF(5079))
        CASE_EQUAL(60)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, 1, 0, PTR(evt_open_box), GSWF(5080))
        CASE_EQUAL(70)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, 1, 0, PTR(evt_open_box), GSWF(5081))
        CASE_EQUAL(80)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, 1, 0, PTR(evt_open_box), GSWF(5082))
        CASE_EQUAL(90)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, 1, 0, PTR(evt_open_box), GSWF(5083))
        CASE_EQUAL(100)
        CALL(evt_mobj_itembox, STRING("box"), 270, 0, 200, 1, 0, PTR(evt_open_box), GSWF(5084))
    END_SWITCH
    CALL(evt_mobj_signboard, STRING("board"), 150, 0, -75, PTR(evt_kanban2), LSWF(0))
    RETURN
EVT_END()

BeroEntry bero_prev_return[] = {
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        8, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, NULL, // exitEvent..nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_03", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

BeroEntry bero_prev_return2[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_04", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

BeroEntry bero_prev_return3[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_05", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

BeroEntry bero_prev_return4[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_06", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(setup_prev_return)
    SET_VALUE(LW(0), PTR(bero_prev_return))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SWITCH(LW(0))
        CASE_LESS(49)
        CASE_BETWEEN(49, 69)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_prev_return2))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CASE_BETWEEN(70, 98)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_prev_return3))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_prev_return4))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_SWITCH
    RETURN
EVT_END()

BeroEntry bero_normal[] = {
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        8, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        NULL, 0, NULL, // exitEvent..nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_00", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

BeroEntry bero_normal2[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_01", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

BeroEntry bero_normal3[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        9, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        6, // caseType
        floor_inc, // exitEvent
        "jon_02", // nextMapName
        "dokan_2", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(setup_normal)
    SET_VALUE(LW(0), PTR(bero_normal))
    CALL(evt_bero_get_info)
    CALL_CHILD(PTR(evt_bero_info_run))
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SWITCH(LW(0))
        CASE_LESS(50)
        CASE_BETWEEN(50, 80)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_normal2))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_on))
        CASE_GREATER(80)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_off))
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_normal3))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(PTR(bero_case_switch_on))
    END_SWITCH
    RETURN
EVT_END()

EVT_BEGIN(iri_12_dokan_in_sub)
    LOOP(0)
        CALL(evt_npc_get_unitwork, STRING("マッキノ"), 0, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_ry, STRING("マッキノ"), 90)
    CALL(evt_npc_jump_position_nohit, STRING("マッキノ"), 0, 29, 0, 400, 29)
    WAIT_MS(200)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        SET_FLOAT_VALUE(LW(0), FLOAT(1.0f))
        LOOP(0)
            SUBTRACT_FLOAT(LW(0), FLOAT(0.0068359375f))
            CALL(evt_npc_set_scale, STRING("マッキノ"), LW(0), LW(0), LW(0))
            WAIT_FRAMES(1)
            IF_FLOAT_LESS(LW(0), FLOAT(0.0f))
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_npc_set_scale, STRING("マッキノ"), FLOAT(0.0f), FLOAT(0.0f), FLOAT(0.0f))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_npc_pera_onoff, STRING("マッキノ"), 0)
        CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_SHADOW | NPC_FLAG_NO_VIEW_CORRECTION)
        CALL(evt_paper_entry, STRING("p_dokan_y"))
        CALL(evt_npc_set_paper, STRING("マッキノ"), STRING("p_dokan_y"))
        CALL(evt_npc_set_paper_anim, STRING("マッキノ"), STRING("PM_D_1A"))
        CALL(iri_12_paper_init)
        CALL(evt_npc_set_ry, STRING("マッキノ"), 0)
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(1), FLOAT(0.0f))
        SET_VALUE(LW(2), 0)
        LOOP(0)
            ADD_FLOAT(LW(1), FLOAT(1.0f))
            IF_FLOAT_GREATER(LW(1), FLOAT(30.0f))
                SET_FLOAT_VALUE(LW(1), FLOAT(30.0f))
            END_IF
            ADD(LW(2), 1)
            IF_INT_GREATER(LW(2), 10)
                SUBTRACT_FLOAT(LW(0), LW(1))
                ADD_FLOAT(LW(1), FLOAT(0.099609375f))
                IF_FLOAT_GREATER(LW(1), FLOAT(30.0f))
                    SET_FLOAT_VALUE(LW(1), FLOAT(30.0f))
                END_IF
            END_IF
            IF_FLOAT_LESS(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            CALL(evt_npc_set_ry, STRING("マッキノ"), LW(0))
            WAIT_FRAMES(1)
            IF_INT_GREATER(LW(2), 18)
                BREAK_LOOP
            END_IF
        END_LOOP
        LOOP(0)
            SUBTRACT_FLOAT(LW(0), LW(1))
            IF_FLOAT_LESS(LW(0), FLOAT(0.0f))
                ADD_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            CALL(evt_npc_set_ry, STRING("マッキノ"), LW(0))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    WAIT_MS(300)
    CALL(evt_npc_get_position, STRING("マッキノ"), LW(2), LW(3), LW(4))
    LOOP(0)
        SUBTRACT_FLOAT(LW(3), FLOAT(1.0f))
        CALL(evt_npc_set_position, STRING("マッキノ"), LW(2), LW(3), LW(4))
        WAIT_FRAMES(1)
        IF_FLOAT_LESS_OR_EQUAL(LW(3), FLOAT(-30.0f))
            BREAK_LOOP
        END_IF
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(iri_12_dokan_in)
    IF_INT_EQUAL(GSW(1420), 12)
        IF_INT_EQUAL(GSWF(5406), 1)
            IF_INT_EQUAL(GSWF(5407), 0)
                SPAWN_EVENT(PTR(iri_12_dokan_in_sub))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_return_dokan)
    CALL(evt_npc_set_ry, STRING("マッキノ"), 90)
    WAIT_FRAMES(1)
    CALL(evt_npc_wait_pera, STRING("マッキノ"))
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_SHADOW | NPC_FLAG_NO_VIEW_CORRECTION)
    CALL(evt_npc_flag_onoff, 0, STRING("マッキノ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    CALL(evt_paper_entry, STRING("p_dokan_x"))
    CALL(evt_npc_set_paper, STRING("マッキノ"), STRING("p_dokan_x"))
    CALL(evt_npc_set_paper_anim, STRING("マッキノ"), STRING("PM_D_1A"))
    CALL(iri_12_paper_init)
    CALL(evt_npc_set_ry, STRING("マッキノ"), 180)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_WALL_COLLISION)
    CALL(evt_npc_get_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(2), 3)
    CALL(evt_npc_set_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
    SET_VALUE(GF(0), 1)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_status_onoff, 0, STRING("マッキノ"), 2)
        CALL(evt_npc_get_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
        ADD(LW(0), 60)
        CALL(evt_npc_move_position, STRING("マッキノ"), LW(0), LW(2), 0, 60, 0)
        SET_VALUE(GF(0), 0)
    END_INLINE_EVENT
    WAIT_FRAMES(20)
    CALL(evt_npc_set_rotate_offset, STRING("マッキノ"), 0, 16, 0)
    BEGIN_SIBLING_EVENT
        CALL(evt_npc_get_position, STRING("マッキノ"), LW(2), LW(3), LW(4))
        SET_FLOAT_VALUE(LW(0), FLOAT(1.0f))
        LOOP(0)
            SUBTRACT_FLOAT(LW(0), FLOAT(0.01953125f))
            IF_FLOAT_LESS_OR_EQUAL(LW(0), FLOAT(0.0f))
                CALL(evt_npc_set_scale, STRING("マッキノ"), 0, 0, 0)
                BREAK_LOOP
            END_IF
            CALL(evt_npc_set_scale, STRING("マッキノ"), LW(0), LW(0), LW(0))
            SET_FLOAT_VALUE(LW(1), LW(0))
            MULTIPLY_FLOAT(LW(1), FLOAT(16.0f))
            CALL(evt_npc_set_rotate_offset, STRING("マッキノ"), 0, LW(1), 0)
            SET_FLOAT_VALUE(LW(2), FLOAT(16.0f))
            SUBTRACT_FLOAT(LW(2), LW(1))
            ADD_FLOAT(LW(2), LW(3))
            CALL(iri_12_makkino_set_y, LW(2))
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    BEGIN_SIBLING_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(1), FLOAT(30.0f))
        LOOP(0)
            ADD_FLOAT(LW(0), LW(1))
            IF_FLOAT_GREATER_OR_EQUAL(LW(0), FLOAT(360.0f))
                SUBTRACT_FLOAT(LW(0), FLOAT(360.0f))
            END_IF
            CALL(evt_npc_set_rotate, STRING("マッキノ"), LW(0), 0, 0)
            WAIT_FRAMES(1)
        END_LOOP
    END_SIBLING_EVENT
    WAIT_WHILE(GF(0))
    CALL(evt_npc_set_position, STRING("マッキノ"), 0, -1000, 0)
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_return)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("マッキノ"), 72, 0, 0, 120, 4)
    CALL(evt_msg_print, 0, STRING("irai_12_011"), 0, STRING("マッキノ"))
    CALL(evt_npc_move_position, STRING("マッキノ"), 98, -123, 0, 120, 4)
    CALL_CHILD(PTR(iri_12_makkino_return_dokan))
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_return_b100)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("マッキノ"), -205, -75, 0, 120, 4)
    CALL(evt_msg_print, 0, STRING("irai_12_011"), 0, STRING("マッキノ"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_move_position, STRING("マッキノ"), -76, -310, 0, 180, 4)
    CALL(evt_npc_move_position, STRING("マッキノ"), 98, -486, 0, 180, 4)
    CALL_CHILD(PTR(iri_12_makkino_return_dokan))
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_return_zonbaba)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("マッキノ"), -337, -124, 0, 120, 4)
    CALL(evt_msg_print, 0, STRING("irai_12_011"), 0, STRING("マッキノ"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_move_position, STRING("マッキノ"), -76, -310, 0, 180, 4)
    CALL(evt_npc_move_position, STRING("マッキノ"), 98, -486, 0, 180, 4)
    CALL_CHILD(PTR(iri_12_makkino_return_dokan))
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_move_dokan_side)
    CALL(evt_npc_set_unitwork, STRING("マッキノ"), 0, 1)
    CALL(evt_npc_get_reglid, STRING("マッキノ"), LW(0))
    DELETE_EVENT(LW(0))
    CALL(evt_npc_change_interrupt, STRING("マッキノ"), 1, 0)
    CALL(evt_npc_change_interrupt, STRING("マッキノ"), 6, 0)
    LOOP(0)
        CALL(evt_npc_status_check, STRING("マッキノ"), 16, LW(0))
        IF_INT_EQUAL(LW(0), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    BEGIN_INLINE_EVENT
        CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
        CALL(evt_sub_area_check, 1, -40, 0, 15, LW(0), LW(2), LW(3))
        IF_INT_EQUAL(LW(3), 1)
            CALL(evt_mario_mov_pos2, -40, -40, 60)
        END_IF
    END_INLINE_EVENT
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("マッキノ"), -40, 0, 0, 150, 4)
    CALL(evt_npc_flag_onoff, 0, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(wait_landon)
    CALL(evt_npc_change_interrupt, STRING("マッキノ"), 6, PTR(iri_12_makkino_talk))
    CALL(evt_npc_set_unitwork, STRING("マッキノ"), 0, 0)
    RETURN
EVT_END()

EVT_BEGIN(iri_12_enemy_dead_event)
    IF_INT_EQUAL(GSW(1420), 12)
        IF_INT_EQUAL(GSWF(5406), 1)
            IF_INT_EQUAL(GSWF(5407), 0)
                WAIT_MS(800)
                SPAWN_EVENT(PTR(iri_12_makkino_move_dokan_side))
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_urouro)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("マッキノ"), -175, 109, 0, 120, 4)
        WAIT_FRAMES(1)
        CALL(evt_npc_move_position, STRING("マッキノ"), -73, 109, 0, 120, 4)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_buruburu)
    CALL(evt_npc_move_position, STRING("マッキノ"), -440, -124, 0, 120, 4)
    CALL(evt_npc_get_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
    SUBTRACT(LW(0), 1)
    LOOP(0)
        ADD(LW(0), 2)
        CALL(evt_npc_set_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
        WAIT_MS(50)
        SUBTRACT(LW(0), 2)
        CALL(evt_npc_set_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
        WAIT_MS(50)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_okiru)
    CALL(evt_npc_set_anim, STRING("マッキノ"), STRING("O_1"))
    CALL(evt_npc_wait_anim, STRING("マッキノ"), 1)
    CALL(evt_npc_set_balloontype, STRING("マッキノ"), 1)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 0, STRING("マッキノ"), NPC_FLAG_NO_TALK_TURN)
    CALL(evt_npc_status_onoff, 1, STRING("マッキノ"), 2)
    CALL(evt_set_dir_to_target, STRING("マッキノ"), STRING("mario"))
    RETURN
EVT_END()

static EVT_BEGIN(iri_12_makkino_talk)
    CALL(evt_npc_check, STRING("敵"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        IF_INT_EQUAL(GSWF(5406), 0)
            CALL(evt_msg_print, 0, STRING("irai_12_008"), PTR(iri_12_msgcb), STRING("マッキノ"))
            SET_VALUE(GSWF(5406), 1)
            CALL(evt_npc_change_interrupt, STRING("マッキノ"), 6, 0)
            SPAWN_EVENT(PTR(iri_12_makkino_move_dokan_side))
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_12_010"), 0, STRING("マッキノ"))
        END_IF
        ELSE
        IF_INT_EQUAL(GSWF(5406), 0)
            CALL(evt_msg_print, 0, STRING("irai_12_007"), 0, STRING("マッキノ"))
            ELSE
            CALL(evt_msg_print, 0, STRING("irai_12_009"), 0, STRING("マッキノ"))
        END_IF
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_fall)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 2)
    CALL(evt_bero_get_start_position, LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY)
    CALL(wait_landon)
    CALL(evt_npc_check, STRING("敵"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(PTR(iri_12_makkino_move_dokan_side))
        ELSE
        CALL(evt_npc_move_position, STRING("マッキノ"), -55, 109, 0, 120, 4)
        CALL(evt_npc_check, STRING("敵"), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_npc_change_interrupt, STRING("マッキノ"), 1, PTR(iri_12_makkino_urouro))
            CALL(evt_npc_restart_regular_event, STRING("マッキノ"))
        END_IF
    END_IF
    CALL(evt_npc_change_interrupt, STRING("マッキノ"), 6, PTR(iri_12_makkino_talk))
    CALL(evt_npc_flag_onoff, 0, STRING("マッキノ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
    RETURN
EVT_END()

EVT_BEGIN(iri_12_makkino_fall_return)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, 2)
    CALL(evt_bero_get_start_position, LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
    CALL(evt_bero_exec_wait, 65536)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY)
    CALL(wait_landon)
    IF_INT_EQUAL(GSW_PIT_FLOOR, 99)
        IF_INT_EQUAL(GSWF(5085), 1)
            SPAWN_EVENT(PTR(iri_12_makkino_return_b100))
            SET_VALUE(GSWF(5407), 1)
            ELSE
            CALL(evt_npc_change_interrupt, STRING("マッキノ"), 1, PTR(iri_12_makkino_buruburu))
            CALL(evt_npc_restart_regular_event, STRING("マッキノ"))
            CALL(evt_mario_key_onoff, 1)
        END_IF
        ELSE
        CALL_CHILD(PTR(iri_12_makkino_return))
        SET_VALUE(GSWF(5407), 1)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(iri_12_init_fall)
    CALL(evt_npc_entry, STRING("マッキノ"), STRING("c_suifu_k"))
    CALL(evt_npc_set_tribe, STRING("マッキノ"), STRING("水夫"))
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 1, STRING("マッキノ"), 2)
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    MODULO(LW(0), 10)
    IF_INT_EQUAL(LW(0), 0)
        SPAWN_EVENT(PTR(iri_12_makkino_fall_return))
        ELSE
        SPAWN_EVENT(PTR(iri_12_makkino_fall))
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(iri_12_init)
    IF_INT_EQUAL(GSW(1420), 12)
        SWITCH(GSW_PIT_FLOOR)
            CASE_EQUAL(17)
            IF_INT_EQUAL(GSWF(5406), 0)
                CALL(evt_npc_entry, STRING("マッキノ"), STRING("c_suifu_k"))
                CALL(evt_npc_set_tribe, STRING("マッキノ"), STRING("水夫"))
                CALL(evt_npc_set_position, STRING("マッキノ"), -50, 0, -120)
                CALL(evt_npc_set_ry, STRING("マッキノ"), 90)
                CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_TALK_TURN)
                CALL(evt_npc_change_interrupt, STRING("マッキノ"), 6, PTR(iri_12_makkino_talk))
                CALL(evt_npc_set_anim, STRING("マッキノ"), STRING("D_2"))
                CALL(evt_npc_set_balloontype, STRING("マッキノ"), 0)
                ELSE
                CALL_CHILD(PTR(iri_12_init_fall))
            END_IF
            CASE_DEFAULT
            IF_INT_EQUAL(GSWF(5406), 1)
                IF_INT_EQUAL(GSWF(5407), 0)
                    CALL_CHILD(PTR(iri_12_init_fall))
                END_IF
            END_IF
        END_SWITCH
    END_IF
    RETURN
EVT_END()

EVT_BEGIN(evt_iri_30_touch_rakugaki)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("irai_30_007"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

EVT_BEGIN(evt_iri_30_bomb_rakugaki)
    CALL(evt_msg_close, STRING("irai_30_007"))
    CALL(evt_del_case_evt, 1, LW(15))
    BEGIN_INLINE_EVENT
        WAIT_MS(200)
        CALL(evt_mapobj_get_position, STRING("s_rakugaki"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_OFF_WALL_BREAK1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_paper_entry, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_entry, STRING("ofs"))
    CALL(evt_img_entry, STRING("img"))
    CALL(evt_img_onoff, STRING("img"), 0)
    CALL(evt_img_set_paper, STRING("img"), STRING("OFF_d_bom_ver2"))
    CALL(evt_mapobj_set_offscreen, 1, STRING("s_rakugaki"), STRING("ofs"))
    CALL(evt_offscreen_get_boundingbox2, STRING("ofs"), LW(0), LW(1), LW(2), LW(3))
    SET_VALUE(LF(0), 0)
    IF_INT_EQUAL(LW(2), 0)
        IF_INT_EQUAL(LW(3), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL(evt_img_alloc_capture, STRING("img"), 0, STRING("ofs"), 1, LW(0), LW(1), LW(2), LW(3))
        WAIT_FRAMES(1)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_img_clear_virtual_point, STRING("img"))
        CALL(evt_img_onoff, STRING("img"), 1)
        CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
        CALL(evt_img_wait_animend, STRING("img"))
        ELSE
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 1, STRING("before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, 1, 0, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_img_release, STRING("img"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("s_rakugaki"))
    CALL(evt_paper_delete, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_delete, STRING("ofs"))
    SET_VALUE(GSWF(5406), 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

static EVT_BEGIN(gonbaba_standup_shake)
    WAIT_MS(3700)
    CALL(evt_npc_get_position, STRING("敵"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_STEP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_cam_shake, 4, FLOAT(0.009765625f), FLOAT(0.0f), 800)
    RETURN
EVT_END()

static EVT_BEGIN(gonbaba_fog_change)
    CALL(evt_map_get_fog, LW(4), LW(5), LW(6), LW(7), LW(8), LW(9))
    SET_VALUE(GW(0), LW(5))
    SET_VALUE(GW(1), LW(6))
    CONVERT_MS_TO_FRAME(LW(10), 8000)
    ADD(LW(10), 1)
    BEGIN_INLINE_EVENT
        LOOP(LW(10))
            CALL(evt_map_set_fog, LW(4), GW(0), GW(1), LW(7), LW(8), LW(9))
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 0, 400, LW(10))
        LOOP(LW(10))
            CALL(evt_sub_intpl_get_value)
            SET_VALUE(GW(0), LW(0))
            ADD(GW(0), LW(5))
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
    END_INLINE_EVENT
    CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 0, 1600, LW(10))
    LOOP(LW(10))
        CALL(evt_sub_intpl_get_value)
        SET_VALUE(GW(1), LW(0))
        ADD(GW(1), LW(6))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

static EVT_BEGIN(gonbaba_down_shake)
    CALL(evt_npc_get_position, STRING("敵"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon, STRING("SFX_STG1_GNB_DOWN1"), 0)
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_DOWN2"), LW(0), LW(1), LW(2), LW(8))
    BEGIN_INLINE_EVENT
        CALL(evt_sub_intpl_msec_init, EASING_LINEAR, 90, 0, 10000)
        LABEL(0)
        CALL(evt_sub_intpl_msec_get_value)
        CALL(evt_snd_sfx_vol, LW(8), LW(0))
        WAIT_FRAMES(1)
        IF_INT_EQUAL(LW(1), 1)
            GOTO(0)
        END_IF
        CALL(evt_snd_sfxoff, LW(8))
    END_INLINE_EVENT
    CALL(evt_cam_shake, 4, FLOAT(0.049804688f), FLOAT(0.0f), 500)
    CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 50, 5, 50)
    LOOP(0)
        CALL(evt_sub_intpl_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 50)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 2000)
    RETURN
EVT_END()

EVT_BEGIN(zonbaba_first_event)
    IF_INT_EQUAL(GSWF(5085), 1)
        CALL(evt_npc_set_position, STRING("敵"), 0, -1000, 0)
        CALL(evt_npc_flag_onoff, 0, STRING("敵"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(evt_mobj_itembox, STRING("box"), 270, 0, 200, 1, 0, PTR(evt_open_box), GSWF(5084))
        CALL(evt_hitobj_onoff, STRING("baba"), 1, 0)
        RETURN
    END_IF
    CALL(evt_map_set_fog, 2, 400, 1000, 0, 10, 40)
    CALL(evt_map_fog_onoff, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_npc_set_position, STRING("敵"), 175, 0, 0)
    CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_S_1"))
    CALL(evt_mapobj_flag_onoff, 1, 1, STRING("oFF_jon_06"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_seq_wait, SEQ_GAME)
    WAIT_MS(3500)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), -40)
        CALL(evt_party_move_pos, 0, LW(3), LW(2), 250)
    END_INLINE_EVENT
    WAIT_MS(2000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_set_dir_npc, STRING("敵"))
    CALL(evt_party_set_dir_npc, 0, STRING("敵"))
    CALL(evt_snd_bgmon, 512, STRING("BGM_EVT_GONBABA_FLY2"))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d_ex, STRING("SFX_STG1_GNB_ROAR2"), 0, 0, 0, 255, 255, 4300, 16, 0)
        CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_U_1"))
        CALL(evt_npc_wait_anim, STRING("敵"), FLOAT(1.0f))
        CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_S_1"))
    END_INLINE_EVENT
    SPAWN_EVENT(PTR(gonbaba_standup_shake))
    SPAWN_EVENT(PTR(gonbaba_fog_change))
    CALL(evt_cam3d_evt_set, -755, 24, 376, -84, 114, -22, 4500, 11)
    WAIT_MS(4000)
    LOOP(30)
        CALL(evt_cam3d_evt_set, -755, 24, 376, -84, 114, -22, 15, 11)
        WAIT_MS(15)
        CALL(evt_cam3d_evt_set, -745, 23, 366, -84, 114, -22, 15, 11)
        WAIT_MS(15)
    END_LOOP
    WAIT_MS(3000)
    CALL(evt_npc_set_battle_info, STRING("敵"), 51)
    CALL(evt_npc_battle_start, STRING("敵"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_cam3d_evt_set, -769, 50, 527, -22, 114, 7, 1, 11)
    CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_H_1"))
    CALL(evt_map_fog_onoff, 0)
    CALL(evt_npc_set_position, STRING("敵"), 0, 60, 0)
    CALL(evt_mario_set_pos, 25, 10, 150)
    CALL(evt_party_set_pos, 0, -25, 10, 150)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    SPAWN_EVENT(PTR(gonbaba_down_shake))
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_flag_onoff, 1, STRING("敵"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(gonbaba_wait_landon)
        CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_H_2"))
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("敵"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GNB_DOWN2"), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_cam_shake, 4, FLOAT(0.049804688f), FLOAT(0.0f), 500)
        CALL(evt_sub_intpl_init, EASING_COS_IN_OUT, 50, 5, 50)
        LOOP(0)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(evt_cam_shake, 4, LW(0), FLOAT(0.0f), 50)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_cam_shake, 4, FLOAT(0.0048828125f), FLOAT(0.0f), 2000)
        CALL(evt_snd_sfxoff, LW(3))
    END_INLINE_EVENT
    WAIT_MS(1000)
    WAIT_MS(5000)
    CALL(evt_npc_get_position, STRING("敵"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GNB_DOWN3"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(600)
    CALL(evt_cam3d_evt_set, -145, 80, 882, 104, 114, 81, 1000, 11)
    WAIT_MS(1000)
    CALL(evt_mobj_itembox, STRING("box"), 250, 60, 0, 1, 0, PTR(evt_open_box), GSWF(5084))
    CALL(evt_mobj_flag_onoff, 0, STRING("box"), MOBJ_FLAG_GRAVITY)
    BEGIN_INLINE_EVENT
        CALL(evt_npc_get_position, STRING("敵"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GNB_THROAT_MOVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1000)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GMB_MOUTH_MOVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(200)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GMB_MOUTH_MOVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(200)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GMB_MOUTH_MOVE1"), LW(0), LW(1), LW(2), 0)
        WAIT_MS(1100)
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GMB_MOUTH_OPEN1"), LW(0), LW(1), LW(2), 0)
    END_INLINE_EVENT
    CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_H_4"))
    CALL(evt_npc_wait_anim, STRING("敵"), FLOAT(0.7792969f))
    BEGIN_INLINE_EVENT
        WAIT_MS(100)
        CALL(evt_mobj_get_position, STRING("box"), LW(0), LW(1), LW(2))
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_TBOX_JUMP1"), LW(0), LW(1), LW(2), GW(0))
    END_INLINE_EVENT
    BEGIN_INLINE_EVENT
        SET_FLOAT_VALUE(LW(0), FLOAT(60.0f))
        SET_FLOAT_VALUE(LW(1), FLOAT(25.0f))
        SET_FLOAT_VALUE(LW(2), FLOAT(-0.5996094f))
        SET_FLOAT_VALUE(LW(3), FLOAT(0.0f))
        SET_FLOAT_VALUE(LW(4), FLOAT(1.2998047f))
        SET_VALUE(LF(0), 0)
        LOOP(0)
            ADD_FLOAT(LW(0), LW(1))
            ADD_FLOAT(LW(3), LW(4))
            CALL(evt_mobj_set_y_position, STRING("box"), LW(0))
            CALL(evt_mobj_set_z_position, STRING("box"), LW(3))
            ADD_FLOAT(LW(1), LW(2))
            IF_INT_LESS(LW(1), -14)
                SET_FLOAT_VALUE(LW(1), FLOAT(-14.0f))
            END_IF
            IF_INT_LESS_OR_EQUAL(LW(0), 0)
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_snd_sfxoff, GW(0))
                    SET_VALUE(LF(0), 1)
                END_IF
                SET_FLOAT_VALUE(LW(0), FLOAT(0.0f))
                MULTIPLY_FLOAT(LW(1), FLOAT(-0.5f))
                CALL(evt_mobj_get_position, STRING("box"), LW(5), LW(6), LW(7))
                CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_TBOX_LANDING1"), LW(5), LW(6), LW(7), 0)
                IF_INT_LESS_OR_EQUAL(LW(1), 2)
                    BREAK_LOOP
                END_IF
            END_IF
            WAIT_FRAMES(1)
        END_LOOP
        CALL(evt_mobj_set_y_position, STRING("box"), 0)
    END_INLINE_EVENT
    WAIT_MS(1000)
    CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_H_3"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, 255, 105, 648, 255, 55, 150, 500, 11)
    WAIT_MS(1800)
    CALL(evt_cam3d_evt_off, 0, 11)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, 0, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, 3, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, 1, 0, STRING("oFF_jon_06"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_flag_onoff, 1, STRING("敵"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("敵"), NPC_FLAG_ENEMY)
    SET_VALUE(GSWF(5085), 1)
    IF_INT_EQUAL(GSW(1420), 12)
        IF_INT_EQUAL(GSWF(5406), 1)
            IF_INT_EQUAL(GSWF(5407), 0)
                CALL(evt_npc_get_reglid, STRING("マッキノ"), LW(0))
                DELETE_EVENT(LW(0))
                CALL(evt_npc_change_interrupt, STRING("マッキノ"), 1, 0)
                CALL(evt_mario_key_onoff, 0)
                SPAWN_EVENT(PTR(iri_12_makkino_return_zonbaba))
                SET_VALUE(GSWF(5407), 1)
            END_IF
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

static USER_FUNC(gonbaba_wait_landon) {
    // 敵
    return npcNameToPtr("\223G")->groundHit ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

void iri_12_msgcb(s32 type, void* param) {
    u8* msg = *(u8**)param;

    if (type == 3 && *(u16*)(msg + 8) == 1) {
        evtEntry(iri_12_makkino_okiru, 0, 0);
    }
}

USER_FUNC(wait_landon) {
    // マッキノ
    NpcEntry* npc = npcNameToPtr("\203}\203b\203L\203m");

    if (!(npc->flags & NPC_FLAG_FLOOR_COLLISION) || npc->groundHit != NULL) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

USER_FUNC(iri_12_makkino_set_y) {
    f32 y = evtGetFloat(event, event->args[0]);

    npcNameToPtr("\203}\203b\203L\203m")->position.y = y;
    return EVT_RETURN_DONE;
}

USER_FUNC(iri_12_paper_init) {
    NpcEntry* npc = npcNameToPtr("\203}\203b\203L\203m");

    animPosePaperPeraOff(npc->poseId);
    animPoseSetMaterialLightFlagOff(npc->poseId, 1);
    return EVT_RETURN_DONE;
}

USER_FUNC(shikakeFlag) {
    s32 type = evtGetValue(event, event->args[0]);
    NpcEntry* npc = npcNameToPtr("\223G");

    switch (type) {
        case 0:
            npc->battleInfo.battleFlags |= 0x4000000;
            break;
        case 1:
            npc->battleInfo.battleFlags |= 0x2000000;
            break;
        case 2:
            npc->battleInfo.battleFlags |= 0x1000000;
            break;
        case 3:
            npc->battleInfo.battleFlags |= 0x100;
            break;
    }
    return EVT_RETURN_DONE;
}

USER_FUNC(wanwan_set_home_pos) {
    EvtWord* args = event->args;
    s32 x = evtGetValue(event, args[0]);
    s32 y = evtGetValue(event, args[1]);
    s32 z = evtGetValue(event, args[2]);

    npcEnt_wanwan[0].territoryBase.x = x;
    npcEnt_wanwan[0].territoryBase.y = y;
    npcEnt_wanwan[0].territoryBase.z = z;
    return EVT_RETURN_DONE;
}

USER_FUNC(setup_npc_ex_para) {
    const char* name = (const char*)evtGetValue(event, event->args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);

    npc->territoryLoiter.y = 50.0f;
    npc->territoryHoming.y = 180.0f;
    return EVT_RETURN_DONE;
}

USER_FUNC(yattukeFlag) {
    npcNameToPtr("\223G")->battleInfo.battleFlags |= 0x10000000;
    return EVT_RETURN_DONE;
}

USER_FUNC(enemy_info) {
    EvtWord* args = event->args;
    s32 idx = evtGetValue(event, args[0]);
    s32 no = enemy_100[idx];
    NpcSetupBattleInfo* setup = &btlsetup_jon_tbl[no];
    NpcTribeInfo* tribe = npcTribe;
    const char* name;
    BOOL found = FALSE;

    for (; tribe->nameJp != NULL; tribe++) {
        if (strcmp(tribe->nameJp, setup->name) == 0) {
            found = TRUE;
            break;
        }
    }
    if (!found) {
        // クリボー
        name = "\203N\203\212\203{\201[";
        tribe = npcGetTribe(name);
    } else {
        name = tribe->nameJp;
    }
    evtSetValue(event, args[1], (EvtWord)tribe->modelName);
    evtSetValue(event, args[2], (EvtWord)name);
    evtSetValue(event, args[3], no);
    return EVT_RETURN_DONE;
}

USER_FUNC(evt_raster) {
    f32* work = event->lwDataF;

    if (isFirstCall) {
        work[0] = 0.0f;
        work[1] = 0.0f;
    }
    work[0] += 0.2f;
    work[1] += 0.01f;
    dispEntry(CAMERA_2D, 0.0f, 3, raster_disp, event);
    return EVT_RETURN_BLOCK;
}

inline s32 ia8_ofs(s32 x, s32 y) {
    int off = (y / 4) << 5;
    off += (x / 4) << 9;
    off += ((y % 4) + ((x % 4) << 2)) * 2;
    return off;
}

void raster_disp(CameraId cameraId, void* param) {
    EventEntry* event = param;
    CameraEntry* cam = camGetPtr(cameraId);
    f32 amp = event->lwDataF[0];
    f32 phase = event->lwDataF[1];
    f32 mtx[2][3];
    GXTexObj obj;
    void* copyBuf;
    u8* buf;
    f32 h;
    f32 w;
    s32 i;
    s32 j;
    if (amp > 127.0f) {
        amp = 127.0f;
    }
    copyBuf = smartAlloc(GXGetTexBufferSize(608, 480, GX_TF_RGBA8, GX_FALSE, 0), 3)->address;
    buf = smartAlloc(GXGetTexBufferSize(64, 64, GX_TF_IA8, GX_FALSE, 0), 3)->address;
    phase = 6.2831855f * phase;
    for (i = 0; i < 64; i++) {
        for (j = 0; j < 64; j++) {
            f32 v = sin(j + (gp->retraceCount / 4.0f + phase));
            s32 o;

            o = i / 4;
            o = (o << 5) + ((j / 4) << 9);
            o += ((i % 4) + ((j % 4) << 2)) * 2;

            buf[o] = amp * v + 128.0f;
            buf[o + 1] = 0x80;
        }
    }
    DCFlushRange(buf, 0x2000);
    GXSetTexCopySrc(0, 0, 608, 480);
    GXSetTexCopyDst(608, 480, GX_TF_RGBA8, GX_FALSE);
    GXCopyTex(copyBuf, GX_FALSE);
    GXPixModeSync();
    GXInitTexObj(&obj, copyBuf, 608, 480, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP0);
    GXInitTexObj(&obj, buf, 64, 64, GX_TF_IA8, GX_REPEAT, GX_REPEAT, GX_FALSE);
    GXInitTexObjLOD(&obj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&obj, GX_TEXMAP1);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    mtx[0][0] = 0.5f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 0.5f;
    mtx[1][2] = 0.0f;
    GXSetIndTexMtx(GX_ITM_0, mtx, 1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD0, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_8, GX_ITS_8);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetNumIndStages(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetCullMode(GX_CULL_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXLoadPosMtxImm(cam->view, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    w = 608.0f;
    h = 480.0f;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-w / 2, h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(w / 2, h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(w / 2, -h / 2, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(-w / 2, -h / 2, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
}

void _unresolved(void) {
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("jon_00", jon_init_evt);
    relSetEvtAddr("jon_01", jon_init_evt);
    relSetEvtAddr("jon_02", jon_init_evt);
    relSetEvtAddr("jon_03", jon_init_evt);
    relSetEvtAddr("jon_04", jon_init_evt);
    relSetEvtAddr("jon_05", jon_init_evt);
    relSetEvtAddr("jon_06", jon_init_evt);
    relSetBtlAddr("jon", btlsetup_jon_tbl, btl_jon_setup_no_tbl);
}

