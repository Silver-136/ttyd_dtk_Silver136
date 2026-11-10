// jon (１００階ダンジョン, Pit of 100 Trials; jon is the "dungeon"): shared code and data for every floor: enemy setup
// per floor, the item boxes, the mover (移動屋) and the trader (行商人), the troubles (irai) solved here and the battle
// setup tables.

#include "version.h"
#include "battle/battle_unit.h"
#include "driver/fadedrv.h"
#include "evt/evt_story.h"
#include "evt/evt_window.h"
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
#include "pmario_sound.h"
#include "driver/itemdrv.h"

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

static void raster_disp(CameraId cameraId, void* param);
typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

static USER_FUNC(gonbaba_wait_landon);
static void iri_12_msgcb(s32 type, void* param);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

#if VERSION >= VERSION_US
EvtWord jon_init_evt[400];
#else
#if VERSION == VERSION_DEMO_JP
EvtWord jon_init_evt[329];
#else
EvtWord jon_init_evt[391];
#endif
#endif

u8 enemy_100[100];
static NpcSetupInfo npcEnt_kuriboo[2];
static NpcSetupInfo npcEnt_patakuri[2];
static NpcSetupInfo npcEnt_hannya[2];
static NpcSetupInfo npcEnt_chorobon[2];
static NpcSetupInfo npcEnt_sinnosuke[2];
static NpcSetupInfo npcEnt_sinemon[2];
static NpcSetupInfo npcEnt_nokonoko[2];
static NpcSetupInfo npcEnt_patapata[2];
static NpcSetupInfo npcEnt_met[2];
static NpcSetupInfo npcEnt_patamet[2];
static NpcSetupInfo npcEnt_sambo[2];
static NpcSetupInfo npcEnt_honenoko[2];
static NpcSetupInfo npcEnt_karon[2];
static NpcSetupInfo npcEnt_dokugassun[2];
static NpcSetupInfo npcEnt_piders[2];
static NpcSetupInfo npcEnt_teresa[2];
static NpcSetupInfo npcEnt_hbom[2];
static NpcSetupInfo npcEnt_bubble[2];
NpcSetupInfo npcEnt_wanwan[2];
static NpcSetupInfo npcEnt_mahoon[2];
static NpcSetupInfo npcEnt_pakkun[2];
static NpcSetupInfo npcEnt_togenoko[2];
static NpcSetupInfo npcEnt_basabasa2[2];
static NpcSetupInfo npcEnt_togedaruma[2];
static NpcSetupInfo npcEnt_pansy[2];
static NpcSetupInfo npcEnt_zonbaba[2];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord enemy_setup[];
#else
EvtWord enemy_setup[1055];
#endif

static s32 treasure_tbl[10];
static EvtWord evt_open_box[46];
static EvtWord move_event[55];
static EvtWord get_map_name[111];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord talk_idouya[];
#else
EvtWord talk_idouya[141];
#endif

static NpcSetupInfo npcEnt_idouya[2];
static EvtWord idouya_setup[38];

#if VERSION >= VERSION_US
static EvtWord talk_gyousyou[219];
#else
#if VERSION == VERSION_DEMO_JP
static EvtWord talk_gyousyou[186];
#else
static EvtWord talk_gyousyou[207];
#endif
#endif

static NpcSetupInfo npcEnt_gyousyou[2];
static EvtWord gyousyou_setup[47];
static EvtWord shikake_setup[117];
static EvtWord dokan_open[74];
static EvtWord dokan_open2[70];
static EvtWord evt_kanban[25];
static EvtWord evt_kanban2[20];
static EvtWord floor_inc[10];
static EvtWord floor_clear[8];
static BeroEntry bero_boss[3];
static EvtWord setup_boss[9];
static BeroEntry bero_return[4];
static BeroEntry bero_return2[2];
static BeroEntry bero_return3[2];
static EvtWord setup_return[190];
static BeroEntry bero_prev_return[3];
static BeroEntry bero_prev_return2[2];
static BeroEntry bero_prev_return3[2];
static BeroEntry bero_prev_return4[2];
static EvtWord setup_prev_return[69];
static BeroEntry bero_normal[3];
static BeroEntry bero_normal2[2];
static BeroEntry bero_normal3[2];
static EvtWord setup_normal[53];
static EvtWord iri_12_dokan_in_sub[202];
static EvtWord iri_12_dokan_in[16];
static EvtWord iri_12_makkino_return_dokan[193];
static EvtWord iri_12_makkino_return[31];
static EvtWord iri_12_makkino_return_b100[42];
static EvtWord iri_12_makkino_return_zonbaba[44];
static EvtWord iri_12_makkino_move_dokan_side[94];
static EvtWord iri_12_enemy_dead_event[18];
static EvtWord iri_12_makkino_urouro[25];
static EvtWord iri_12_makkino_buruburu[44];
EvtWord iri_12_makkino_okiru[33];
static EvtWord iri_12_makkino_talk[55];
static EvtWord iri_12_makkino_fall[79];
static EvtWord iri_12_makkino_fall_return[71];
static EvtWord iri_12_init_fall[38];
static EvtWord iri_12_init[65];
static EvtWord evt_iri_30_touch_rakugaki[14];
static EvtWord evt_iri_30_bomb_rakugaki[154];
static EvtWord gonbaba_standup_shake[23];
static EvtWord gonbaba_fog_change[87];
static EvtWord gonbaba_down_shake[83];

#if VERSION <= VERSION_DEMO_JP
extern EvtWord zonbaba_first_event[];
#else
EvtWord zonbaba_first_event[677];
#endif

EVT_BEGIN(jon_init_evt)
    // When the map is entered without an entrance (GW_FLAG_NO_ENTRANCE, a direct map warp), the floor number is
    // derived from the name of the map.
    CALL(evt_sub_get_system_flag, LW(0))
    IF_BITS_SET(LW(0), GW_FLAG_NO_ENTRANCE)
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

    // Remember the deepest floor reached, then set up the signboard, the mover, the trader and the enemies of
    // this floor.
    SET_VALUE(LSW_JON_PIT_FLOOR, GSW_PIT_FLOOR)
    SET_VALUE(LW(0), LSW_JON_PIT_FLOOR)
    ADD(LW(0), 1)
    IF_INT_GREATER(LW(0), GSW_JON_DEEPEST_LEVEL)
        SET_VALUE(GSW_JON_DEEPEST_LEVEL, LW(0))
    END_IF
    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_kanban"), 0, PTR(evt_kanban), 0)
    CALL_CHILD(idouya_setup)
    CALL_CHILD(gyousyou_setup)
    CALL_CHILD(enemy_setup)

    // Choose the floor layout from the floor number: the boss floor, a rest-room floor, the floor before one,
    // or a plain floor.
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SET_VALUE(LW(1), LW(0))
    MODULO(LW(1), 10)
    SET_VALUE(LW(2), LW(0))
    DIVIDE(LW(2), 10)
    SWITCH(LW(1))
        CASE_EQUAL(0)
        IF_INT_EQUAL(LW(2), 10)
            CALL_CHILD(setup_boss)
        ELSE
            CALL_CHILD(setup_return)
            CALL(evt_mobj_floatswitch_blue, 0, STRING("sw"), -75, 60, 0, PTR(dokan_open2), 0)
        END_IF
        CASE_EQUAL(9)
        CALL_CHILD(setup_prev_return)
        CALL(evt_npc_check, STRING("敵"), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 1)
            CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
            CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, 30, 0)
            CALL(evt_hit_bind_update, STRING("a_dokan_1"))
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
#if VERSION >= VERSION_JP
        CALL(evt_npc_check, STRING("移動屋"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
            CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, -30, 0)
            CALL(evt_hit_bind_update, STRING("a_dokan_1"))
            CALL(evt_mobj_floatswitch_blue, 0, STRING("idou_sw"), 75, 60, 0, PTR(dokan_open), 0)
        END_IF
#endif
        CASE_DEFAULT
        CALL_CHILD(setup_normal)
        CALL(evt_npc_check, STRING("敵"), LW(0))
        IF_INT_NOT_EQUAL(LW(0), 1)
            CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
            CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, 30, 0)
            CALL(evt_hit_bind_update, STRING("a_dokan_1"))
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
#if VERSION >= VERSION_JP
        CALL(evt_npc_check, STRING("移動屋"), LW(0))
        IF_INT_EQUAL(LW(0), 1)
            CALL(evt_hit_bind_mapobj, STRING("a_dokan_1"), STRING("dokan_1_s"))
            CALL(evt_mapobj_trans, 1, STRING("dokan_1_s"), 0, -30, 0)
            CALL(evt_hit_bind_update, STRING("a_dokan_1"))
            CALL(evt_mobj_floatswitch_blue, 0, STRING("idou_sw"), 75, 60, 0, PTR(dokan_open), 0)
        END_IF
#endif
    END_SWITCH

    // Trouble Center job 12 (the sailor) is set up here on every floor map; iri_12_init does something only while
    // that job is accepted.
    CALL_CHILD(iri_12_init)
    CALL(evt_sub_get_mapname, LW(0))

    // On floor 50 the graffiti wall of Trouble Center job 30 shows its before or after state and reacts to
    // Bobbery.
    IF_STR_EQUAL(LW(0), STRING("jon_04"))
        IF_INT_EQUAL(GSW_PIT_FLOOR, 49)
            IF_INT_EQUAL(GSWF_TROUBLE_DONE(TROUBLE_ERASE_THAT_GRAFFITI), 0)
#if VERSION >= VERSION_US
                IF_INT_NOT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ERASE_THAT_GRAFFITI)
                    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
                    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
                ELSE
#endif
                SET_VALUE(LF(0), 0)
                IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ERASE_THAT_GRAFFITI)
                    IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
                        SET_VALUE(LF(0), 1)
                    END_IF
                END_IF
                IF_INT_EQUAL(LF(0), 0)
                    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
                    CALL(evt_run_case_evt, CASE_FRONT_A, 1, STRING("a_rakugaki"), 0, PTR(evt_iri_30_touch_rakugaki), LW(0))
#if VERSION < VERSION_US
                    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_ERASE_THAT_GRAFFITI)
                        IF_INT_EQUAL(GSWF_TROUBLE_WORK(0), 1)
#endif
                            CALL(evt_run_case_evt, CASE_BOBBERY, 1, STRING("a_rakugaki"), 0, PTR(evt_iri_30_bomb_rakugaki), LW(1))
                            CALL(evt_set_case_wrk, LW(1), 15, LW(0))
#if VERSION < VERSION_US
                        END_IF
                    END_IF
#endif
                    ELSE
                    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("before"), MAP_OBJ_FLAG_HIDDEN)
                    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
#if VERSION >= VERSION_US
                    END_IF
#endif
                END_IF
                ELSE
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("before"), MAP_OBJ_FLAG_HIDDEN)
                CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
            END_IF
            ELSE
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
            CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
        END_IF
    END_IF
    IF_INT_NOT_EQUAL(GSW_PIT_FLOOR, 99)
        CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG0_100DN1"))
        CALL(evt_snd_envoff, PSND_FADEOUT)
        CALL(evt_snd_set_rev_mode, 1)
    ELSE
        CALL(evt_snd_bgmoff, PSND_FADEOUT)
        CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG0_DAN1"))
        CALL(evt_snd_set_rev_mode, 2)
    END_IF
    RETURN
EVT_END()

/**
 * Enemy number for each of the 100 floors of the Pit of 100 Trials: every group of twenty floors cycles through five
 * numbers twice and then the next five twice, so the number grows by ten every twenty floors. The last twenty floors
 * break the pattern: they stop at 49 and end with 46 and 51.
 */
u8 enemy_100[] = {
    1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 41, 42, 43, 44, 45, 46, 47, 48, 49, 46, 47, 48, 49, 46, 51,
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Goomba that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_kuriboo[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Goomba family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Paragoomba that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_patakuri[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Paragoomba family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Spania that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_hannya[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Spania family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Fuzzy that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_chorobon[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Fuzzy family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Bald Cleft that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_sinnosuke[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Bald Cleft family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Moon Cleft that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_sinemon[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Moon Cleft family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Koopa Troopa that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_nokonoko[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Koopa Troopa family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Paratroopa that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_patapata[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Paratroopa family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Buzzy Beetle that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_met[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Buzzy Beetle family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Parabuzzy that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_patamet[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Parabuzzy family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Pokey that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_sambo[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Pokey family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Dull Bones that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_honenoko[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Dull Bones family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Dry Bones that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_karon[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Dry Bones family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Poison Puff that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_dokugassun[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Poison Puff family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Pider that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_piders[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Pider family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Boo that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_teresa[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Boo family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials for the Bob-omb species, a wandering enemy that starts a
 * battle on contact (picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_hbom[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Bob-omb family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Lava Bubble that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_bubble[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Lava Bubble family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Chain-Chomp that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
NpcSetupInfo npcEnt_wanwan[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Chain-Chomp family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials for the Wizzerd species, a wandering enemy that starts a
 * battle on contact (picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_mahoon[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Wizzerd family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials for the Piranha Plant species, a wandering enemy that
 * starts a battle on contact (picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_pakkun[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Piranha Plant family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Koopatrol that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_togenoko[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Koopatrol family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials for the Swooper species, a wandering enemy that starts a
 * battle on contact (picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_basabasa2[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Swooper family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Bristle that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_togedaruma[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Bristle family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Floor enemy setup of the Pit of 100 Trials: a wandering Crazee Dayzee that starts a battle on contact
 * (the species picked by enemy_info from enemy_100).
 */
static NpcSetupInfo npcEnt_pansy[] = {
    {
        "敵", // name (Pit of 100 Trials floor enemy (Crazee Dayzee family))
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
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

/**
 * Setup of Bonetail, the boss of floor 100 of the Pit of 100 Trials; his first appearance runs
 * zonbaba_first_event.
 */
static NpcSetupInfo npcEnt_zonbaba[] = {
    {
        "敵", // name (Bonetail, the floor-100 boss of the Pit)
        NPC_FLAG_READY | NPC_FLAG_ENEMY | NPC_FLAG_STOP_FOR_EVENT, // flags
        0, NULL, NULL, NULL, // reactionFlags..talkEvent
        enemy_common_dead_event, // deadEvent
        NULL, NULL, NULL, NULL, // findEvent..blowEvent
        2, // territoryType
        {0}, // territoryBase
#if VERSION <= VERSION_DEMO_JP
        {200.0f, 100.0f, 140.0f},
#else
        {150.0f, 100.0f, 100.0f}, // territoryLoiter
#endif
        200.0f, // searchRange
        300.0f, // searchAngle
        1000.0f, // homingRange
        360.0f, // homingAngle
        1, // battleInfoId
    },
    {0},
};

EVT_BEGIN(enemy_setup)
    // No enemy on a floor where the mover is, nor on the rest-room floors (every tenth floor except floor
    // 100).
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

    // Look up this floor's species and battle, create the enemy NPC and pick a random spot for it on the left
    // or right half of the room.
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

    // Goomba family and their Hyper and Dark versions: one setup, placed on the ground.
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

    // Paragoomba family: flying, so placed above the ground.
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

    // Spania family (Spinia, Spania and Spunia).
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

    // Fuzzy family and the Green and Flower Fuzzies.
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

    // Moon Cleft and Bald Cleft families.
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

    // Koopa Troopa and Paratroopa families.
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

    // Buzzy Beetle and Parabuzzy families.
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

    // Pokey and Poison Pokey, then the bony enemies.
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

    // Poison Puff family (the other puffy clouds share this setup).
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

    // Pider family: the spiders hang high above the floor and keep that height as their home position.
    IF_STR_EQUAL(LW(1), STRING("パイダース"))
        CALL(evt_npc_setup, PTR(npcEnt_piders))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 140, LW(4))
#if VERSION >= VERSION_JP
        CALL(evt_npc_set_home_position, STRING("敵"), LW(3), 140, LW(4))
#endif
    END_IF
    IF_STR_EQUAL(LW(1), STRING("チュランタラー"))
        CALL(evt_npc_setup, PTR(npcEnt_piders))
        CALL(evt_npc_set_position, STRING("敵"), LW(3), 140, LW(4))
#if VERSION >= VERSION_JP
        CALL(evt_npc_set_home_position, STRING("敵"), LW(3), 140, LW(4))
#endif
    END_IF

    // The Bandit and Badge Bandit share the Goomba setup. The Lakitu is set up here with the Poison Puff setup and
    // again further down with the Lava Bubble setup.
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

    // Bob-omb, Boo, bubble and Chain-Chomp style enemies.
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

    // Piranha Plants, the Dark Craw (Goomba setup), Koopatrols, the Swoopers, Bristles and Crazee Dayzees.
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

    // Bonetail on floor 100: an NPC at the room centre that is not culled or turned toward the camera and has paper
    // bending turned off; zonbaba_first_event scripts his appearance.
    IF_STR_EQUAL(LW(1), STRING("ゾンババ"))
        CALL(evt_npc_setup, PTR(npcEnt_zonbaba))
        CALL(evt_npc_set_position, STRING("敵"), 0, 0, 0)
        CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_H_3"))
        CALL(evt_npc_flag_onoff, 1, STRING("敵"), NPC_FLAG_NO_CULL | NPC_FLAG_NO_VIEW_CORRECTION)
        CALL(evt_npc_pera_onoff, STRING("敵"), 0)
        CALL(evt_npc_set_ry, STRING("敵"), 0)
        SPAWN_EVENT(zonbaba_first_event)
    END_IF

    // In the early demos the chosen spot becomes the enemy's home. Then arm its battle and set its flags.
    CALL(evt_npc_get_position, STRING("敵"), LW(3), LW(4), LW(5))
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_npc_set_home_position, STRING("敵"), LW(3), LW(4), LW(5))
#endif
    CALL(evt_npc_set_battle_info, STRING("敵"), LW(2))
    CALL(setup_npc_ex_para, STRING("敵"))
    CALL(yattukeFlag)

    // Wait until the enemy is dead, then open the exit pipe (on floor 100 set the Bonetail-defeated flag instead)
    // and report the kill to Trouble Center job 12.
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
            SPAWN_EVENT(dokan_open)
        ELSE
            SET_VALUE(GSWF_PIT_BONETAIL_DEFEATED, 1)
        END_IF
        SPAWN_EVENT(iri_12_enemy_dead_event)
    END_INLINE_EVENT
    RETURN
EVT_END()

/**
 * Item ids of the rest-room chest rewards in the Pit of 100 Trials, one for every tenth floor
 * (read by evt_open_box).
 */
static s32 treasure_tbl[] = {
    ITEM_SLEEPY_STOMP, ITEM_FIRE_DRIVE, ITEM_ZAP_TAP, ITEM_PITY_FLOWER, ITEM_STRANGE_SACK,
    ITEM_DOUBLE_DIP, ITEM_DOUBLE_DIP_P, ITEM_BUMP_ATTACK, ITEM_LUCKY_DAY, ITEM_RETURN_POSTAGE,
};

/**
 * Opens a rest-room chest: locks controls, takes the reward for this floor from treasure_tbl (entry (floor + 1)
 * / 10 - 1, so one reward per tenth floor), has the item pop out of the chest and gives it to Mario.
 */
static EVT_BEGIN(evt_open_box)
    CALL(evt_mario_key_onoff, 0)
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    DIVIDE(LW(0), 10)
    SET_INT_READ_ADDR(PTR(treasure_tbl))
    LOOP(LW(0))
        READ_INT(LW(1))
    END_LOOP
    CALL(evt_mobj_get_position, STRING("box"), LW(10), LW(11), LW(12))
    CALL(evt_item_entry, STRING("item"), LW(1), LW(10), LW(11), LW(12), ITEM_TYPE_STILL_NO_TOUCH, -1, 0)
    CALL(evt_mobj_wait_animation_end, STRING("box"))
    CALL(evt_item_get_item, STRING("item"))
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Regular (idle) event of the mover and the trader NPCs:
 * gives them floor collision, gravity and the automatic movement animation and makes them face their movement
 * direction, then loops forever:
 * turns around, waits, turns back and wanders 100 units in a random direction between 120
 * and 240 degrees of the loiter direction at speed 40, so they pace around the room.
 */
static EVT_BEGIN(move_event)
    CALL(evt_npc_flag_onoff, 1, STRING("me"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_AUTO_MOVE_ANIM | NPC_FLAG_GRAVITY)
    CALL(evt_npc_status_onoff, 1, STRING("me"), NPC_STATUS_FACE_MOVE_DIR)
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

static EVT_BEGIN(get_map_name)
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

/**
 * Mover conversation. The early demos' mover takes 10 coins for every trip and picks the destination before checking the coins.
 * Retail charges 10, 30 or 10 coins for the three travel choices and picks the destination after payment.
 */
EVT_BEGIN(talk_idouya)
    CALL(evt_msg_print, 0, STRING("msg_jon_mover"), 0, 0)
    CALL(evt_msg_select, 0, STRING("msg_jon_mover_select"))
    SWITCH(LW(0))
        CASE_EQUAL(0)
#if VERSION <= VERSION_DEMO_JP
        ADD(GSW_PIT_FLOOR, 2)
        CALL_CHILD(get_map_name)
        SET_VALUE(LW(1), STRING("dokan_2"))
#endif
        SET_VALUE(LW(2), 10)
        CASE_EQUAL(1)
#if VERSION <= VERSION_DEMO_JP
        ADD(GSW_PIT_FLOOR, 5)
        CALL_CHILD(get_map_name)
        SET_VALUE(LW(1), STRING("dokan_2"))
        SET_VALUE(LW(2), 10)
#else
        SET_VALUE(LW(2), 30)
#endif
        CASE_EQUAL(2)
#if VERSION <= VERSION_DEMO_JP
        SET_VALUE(LW(0), STRING("tik_06"))
        SET_VALUE(LW(1), STRING("dokan_3"))
#endif
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
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_msg_continue)
#endif
    MULTIPLY(LW(2), -1)
    CALL(evt_pouch_add_coin, LW(2))
#if VERSION >= VERSION_JP
    SWITCH(LW(0))
        CASE_EQUAL(0)
        ADD(GSW_PIT_FLOOR, 2)
        CALL_CHILD(get_map_name)
        SET_VALUE(LW(1), STRING("dokan_2"))
        CASE_EQUAL(1)
        ADD(GSW_PIT_FLOOR, 5)
        CALL_CHILD(get_map_name)
        SET_VALUE(LW(1), STRING("dokan_2"))
        CASE_EQUAL(2)
        SET_VALUE(LW(0), STRING("tik_06"))
        SET_VALUE(LW(1), STRING("dokan_3"))
    END_SWITCH
#endif
    IF_STR_EQUAL(LW(0), STRING("tik_06"))
#if VERSION >= VERSION_JP
        IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_MY_DADDY)
#endif
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
                SET_VALUE(GSWF_TROUBLE_WORK(2), 1)
            END_IF
        END_IF
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_msg_print, 0, STRING("msg_jon_mover_2"), 0, 0)
#else
    END_IF
    CALL(evt_msg_print_add, MSG_PRINT_DIRECT_TEXT, STRING("<scrl_auto><once_stop>"))
    CALL(evt_msg_print_add, 0, STRING("msg_jon_mover_2"))
#endif
    BEGIN_INLINE_EVENT
        CALL(evt_raster)
    END_INLINE_EVENT
    WAIT_MS(5000)
    CALL(evt_fade_set_mapchange_type, 0, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    CALL(evt_fade_set_mapchange_type, 1, FADE_OUT_BLACK, 300, FADE_IN_BLACK, 300)
    IF_STR_EQUAL(LW(0), STRING("tik_06"))
        SET_VALUE(GSW_PIT_FLOOR, 0)
    END_IF
#if VERSION >= VERSION_JP
    CALL(evt_mario_goodbye_party, 0)
#endif
    CALL(evt_bero_mapchange, LW(0), LW(1))
    RETURN
EVT_END()

/**
 * The mover (idouya) that may stand on a floor (see idouya_setup) and, for a fee, takes Mario 2 or 5 floors down or
 * back to Rogueport; talk_idouya handles the conversation.
 */
static NpcSetupInfo npcEnt_idouya[] = {
    {
        "移動屋", // name (a Mover)
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

/**
 * Gives each floor a 5% chance of a mover Puni (c_pjoho model) standing at x = -100.
 * Never from floor 96 on (which includes floor 100).
 */
static EVT_BEGIN(idouya_setup)
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

/**
 * Talk script of the peddler (行商人, tribe ボッタクール, a rip-off merchant):
 * offers an item shop window with coin display.
 * Mario picks an item at its price, the peddler checks the coins and the item pouch,
 * and he can keep buying until he declines or is out of coins.
 * From the JP version on, the price message is chosen by language (0 is Japanese): the other languages get the item
 * name and the price in the opposite order, and US and later build the price message of the items below id 240 from a
 * format string for them.
 */
static EVT_BEGIN(talk_gyousyou)
    // The trader pitches his "hot products" to Mario and asks whether he wants to see them (yes or no).
    CALL(evt_msg_print, 0, STRING("100kai_item_01"), 0, STRING("me"))
    CALL(evt_msg_select, 0, STRING("100kai_item_00"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("100kai_item_02"))
        RETURN
    END_IF
    CALL(evt_msg_continue)

    // Show the coin window and the list of goods; closing it ends the sale with a goodbye.
    LABEL(0)
    CALL(evt_win_coin_on, COIN_WINDOW_COINS, LW(12))
    CALL(evt_win_other_select, SELECT_TYPE_BADGE_CHARLIETON_PIT)
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print, 0, STRING("100kai_item_10"), 0, STRING("me"))
        RETURN
    END_IF

    // Not enough coins for the chosen item: the trader says so and the list comes back.
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_LESS(LW(0), LW(3))
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print, 0, STRING("100kai_item_03"), 0, STRING("me"))
        GOTO(0)
    END_IF

    // The trader names the item and its price and asks Mario to confirm (outside Japanese the name and price
    // arguments are swapped, except for the early demos).
    IF_INT_GREATER_OR_EQUAL(LW(1), 240)
#if VERSION <= VERSION_DEMO_JP
        CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(2), LW(3), LW(4))
    ELSE
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(2), LW(3))
#else
        CALL(evt_sub_get_language, LW(0))
#if VERSION >= VERSION_US
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(2), LW(3), LW(4))
            CASE_DEFAULT
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(3), LW(2), LW(4))
        END_SWITCH
        ELSE
        CALL(evt_sub_get_language, LW(0))
        SWITCH(LW(0))
            CASE_EQUAL(0)
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(2), LW(3))
            CASE_DEFAULT
            CALL(evt_msg_fmt_int, 0, LW(14), STRING("100kai_item_04"), LW(3))
            CALL(evt_msg_fmt_str, 1, LW(14), LW(14), LW(2))
            CALL(evt_msg_print, MSG_PRINT_DIRECT_TEXT, LW(14), 0, STRING("me"))
        END_SWITCH
#else
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(3), LW(2), LW(4))
        ELSE
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(2), LW(3), LW(4))
        END_IF
        ELSE
        CALL(evt_sub_get_language, LW(0))
        IF_INT_NOT_EQUAL(LW(0), 0)
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(3), LW(2))
        ELSE
            CALL(evt_msg_print_insert, 0, STRING("100kai_item_04"), 0, STRING("me"), LW(2), LW(3))
        END_IF
#endif
#endif
    END_IF
    CALL(evt_msg_select, 0, STRING("100kai_item_00"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_win_coin_off, LW(12))
        CALL(evt_msg_print_add, 0, STRING("100kai_item_05"))
        RETURN
    END_IF

    // Mario agrees: the item goes into his pouch, or the trader says Mario cannot carry any more.
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

    // Take the price in coins and thank Mario; if he has coins left, ask whether he wants to see more and show the
    // list again on yes.
    MULTIPLY(LW(3), -1)
    CALL(evt_pouch_add_coin, LW(3))
    CALL(evt_win_coin_wait, LW(12))
    WAIT_MS(200)
    CALL(evt_win_coin_off, LW(12))
#if VERSION <= VERSION_DEMO_JP
    CALL(evt_msg_continue)
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print, 0, STRING("100kai_item_06"), 0, STRING("me"))
        RETURN
    END_IF
    CALL(evt_msg_print, 0, STRING("100kai_item_06"), 0, STRING("me"))
    CALL(evt_msg_print, 0, STRING("100kai_item_07"), 0, STRING("me"))
#else
    CALL(evt_pouch_get_coin, LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL(evt_msg_print_add, 0, STRING("100kai_item_06"))
        RETURN
    END_IF
    CALL(evt_msg_print_add, 0, STRING("100kai_item_07"))
#endif
    CALL(evt_msg_select, 0, STRING("100kai_item_00"))
    IF_INT_EQUAL(LW(0), 1)
        CALL(evt_msg_print_add, 0, STRING("100kai_item_08"))
        RETURN
    END_IF
    CALL(evt_msg_continue)
    GOTO(0)
    RETURN
EVT_END()

/**
 * The traveling trader (gyousyou) of the Pit of 100 Trials, who sells items to Mario; talk_gyousyou runs the
 * shop.
 */
static NpcSetupInfo npcEnt_gyousyou[] = {
    {
        "行商人", // name (Charlieton)
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

/** Gives each rest-room floor (every 10th except 100) a 33% chance of a peddler (c_botta model) standing at x = 100. */
static EVT_BEGIN(gyousyou_setup)
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

/**
 * Floor "device" (shikake) set-up; nothing in the tree starts it.
 * 20% of the time, when the floor has an enemy,
 * it would flash the screen white on arrival and show one of four system messages (msg_jon_trick_1 to 4, "the
 * device
 * on this level is now active"), each setting a battle rule on the enemy via shikakeFlag:
 * no ally change, no hammer,
 * no jump, or double coins after the battle.
 */
static EVT_BEGIN(shikake_setup)
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
        CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
        CALL(evt_npc_stop_for_event)
        CALL(evt_mario_wait_landon)
        WAIT_MS(100)
        CALL(evt_fade_entry, FADE_OUT_WHITE, 150, 0, 0, 0)
        CALL(evt_fade_end_wait)
        CALL(evt_fade_entry, FADE_IN_WHITE, 150, 0, 0, 0)
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

/**
 * Raises the exit pipe (a_dokan_1 / dokan_1_s) 30 units out of the floor over half a second,
 * with the pipe-appear sound, once the floor's enemy is beaten. Controls are locked meanwhile.
 * Spawned from enemy_setup, and in the JP version on also run by the blue float switch ("idou_sw") that jon_init_evt
 * puts on floors where the mover stands.
 */
static EVT_BEGIN(dokan_open)
#if VERSION <= VERSION_DEMO_JP
    WAIT_MS(1000)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_cam_letter_box_wait)
#else
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(1000)
#if VERSION >= VERSION_US
    WAIT_MS(1000)
#else
    CALL(evt_cam_letter_box_wait)
#endif
#endif
    WAIT_MS(500)
    SET_VALUE(LW(2), STRING("a_dokan_1"))
    SET_VALUE(LW(3), STRING("dokan_1_s"))
    CALL(evt_mapobj_get_position, LW(3), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(7), LW(8), LW(9), 0)
    CALL(evt_hit_bind_mapobj, LW(2), LW(3))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 300, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_trans, 1, LW(3), 0, LW(0), 0)
    CALL(evt_hit_bind_update, LW(2))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Variant used in rest rooms: pressing the blue float switch ("sw") raises the exit pipe a_dokan_3 the same way
 * after a 0.4 s pause.
 */
static EVT_BEGIN(dokan_open2)
    CALL(evt_mario_key_onoff, 0)
    WAIT_MS(400)
    SET_VALUE(LW(2), STRING("a_dokan_3"))
    SET_VALUE(LW(3), STRING("dokan_1_s"))
    CALL(evt_mapobj_get_position, LW(3), LW(7), LW(8), LW(9))
    CALL(evt_snd_sfxon_3d, STRING("SFX_DOKAN_APPEAR1"), LW(7), LW(8), LW(9), 0)
    CALL(evt_hit_bind_mapobj, LW(2), LW(3))
    CALL(evt_sub_intpl_msec_init, EASING_SINE_IN_OUT, 0, 300, 500)
    LABEL(0)
    CALL(evt_sub_intpl_msec_get_value)
    DIVIDE_FLOAT(LW(0), FLOAT(10.0f))
    CALL(evt_mapobj_trans, 1, LW(3), 0, LW(0), 0)
    CALL(evt_hit_bind_update, LW(2))
    WAIT_FRAMES(1)
    IF_INT_EQUAL(LW(1), 1)
        GOTO(0)
    END_IF
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("dokan_1_k"), MAP_OBJ_FLAG_HIDDEN)
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

/**
 * Signboard event of the floor's wall board (the "board" signboard set in the map's setup):
 * stops NPC events, shows msg_jon_kanban_2 and gives control back.
 */
static EVT_BEGIN(evt_kanban2)
    CALL(evt_npc_stop_for_event)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_msg_print, 0, STRING("msg_jon_kanban_2"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_start_for_event)
    RETURN
EVT_END()

static EVT_BEGIN(floor_inc)
    SPAWN_EVENT(iri_12_dokan_in)
    ADD(GSW_PIT_FLOOR, 1)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

static EVT_BEGIN(floor_clear)
    SET_VALUE(GSW_PIT_FLOOR, 0)
    SET_VALUE(LW(0), 0)
    RETURN
EVT_END()

/**
 * Pipes of floor 100: the entrance pipe at the top (dokan_2, filled in by script) and the exit pipe to
 * Rogueport (tik_06).
 */
static BeroEntry bero_boss[] = {
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_UP, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Floor 100 pipes: registers the entry pipe (dokan_2)
 * and the exit pipe to Rogueport (dokan_1 to tik_06, which resets the floor counter).
 */
static EVT_BEGIN(setup_boss)
    SET_VALUE(LW(0), PTR(bero_boss))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    RETURN
EVT_END()

/**
 * Pipes of a rest-room floor: the entrance above (dokan_2, set by script), the exit to Rogueport (tik_06)
 * and the pipe down to the next floor map (jon_00).
 */
static BeroEntry bero_return[] = {
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_UP, // directionId
        BERO_POS_AUTO, // centerPosition
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
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Replacement for the rest-room floor pipe down (dokan_3) on floors 50 to 70: it leads to jon_01. */
static BeroEntry bero_return2[] = {
    {
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Replacement for the rest-room floor pipe down (dokan_3) above floor 70 (floors 80 and 90): it leads to jon_02. */
static BeroEntry bero_return3[] = {
    {
        "dokan_3", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Rest-room setup (floors 10, 20, ... 90; floor 100 uses setup_boss): registers the pipes,
 * points the downward pipe at the next map (jon_00, or jon_01 from floor 50 on, jon_02 after floor 70),
 * and places that floor's treasure chest (own opened flag per floor, running evt_open_box) and the signboard.
 */
static EVT_BEGIN(setup_return)
    SET_VALUE(LW(0), PTR(bero_return))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SWITCH(LW(0))
        CASE_LESS(50)
        CASE_BETWEEN(50, 70)
        SET_VALUE(LW(0), STRING("dokan_3"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("dokan_3"), PTR(bero_return2))
        SET_VALUE(LW(0), STRING("dokan_3"))
        CALL_CHILD(bero_case_switch_on)
        CASE_GREATER(70)
        SET_VALUE(LW(0), STRING("dokan_3"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("dokan_3"), PTR(bero_return3))
        SET_VALUE(LW(0), STRING("dokan_3"))
        CALL_CHILD(bero_case_switch_on)
    END_SWITCH
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SWITCH(LW(0))
        CASE_EQUAL(10)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_10_SLEEPY_STOMP_CHEST)
        CASE_EQUAL(20)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_20_FIRE_DRIVE_CHEST)
        CASE_EQUAL(30)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_30_ZAP_TAP_CHEST)
        CASE_EQUAL(40)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_40_PITY_FLOWER_CHEST)
        CASE_EQUAL(50)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_50_STRANGE_SACK_CHEST)
        CASE_EQUAL(60)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_60_DOUBLE_DIP_CHEST)
        CASE_EQUAL(70)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_70_DOUBLE_DIP_P_CHEST)
        CASE_EQUAL(80)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_80_BUMP_ATTACK_CHEST)
        CASE_EQUAL(90)
        CALL(evt_mobj_itembox, STRING("box"), 0, 20, -100, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_90_LUCKY_DAY_CHEST)
        CASE_EQUAL(100)
        CALL(evt_mobj_itembox, STRING("box"), 270, 0, 200, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_100_RETURN_POSTAGE_CHEST)
    END_SWITCH
    CALL(evt_mobj_signboard, STRING("board"), 150, 0, -75, PTR(evt_kanban2), LSWF_JON_SIGNBOARD_UNUSED)
    RETURN
EVT_END()

/**
 * Pipes of the floor before a rest room: the entrance above (dokan_2, set by script) and the pipe down to
 * jon_03.
 */
static BeroEntry bero_prev_return[] = {
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_UP, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Replacement for the pipe down (dokan_1) of the floor before a rest room on floors 49, 59 and 69: it leads to jon_04. */
static BeroEntry bero_prev_return2[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Replacement for the pipe down (dokan_1) of the floor before a rest room on floors 79 and 89: it leads to jon_05.
 */
static BeroEntry bero_prev_return3[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Replacement for the pipe down (dokan_1) of the floor before floor 100 (floor 99): it leads to jon_06.
 */
static BeroEntry bero_prev_return4[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Setup for the floor before a rest room: registers the pipes
 * and sends exit pipe dokan_1 to the rest-room map for that stretch:
 * jon_03 up to floor 48, jon_04 for floors 49-69, jon_05 for floors 70-98 and jon_06 (Bonetail) for floor 99.
 */
static EVT_BEGIN(setup_prev_return)
    SET_VALUE(LW(0), PTR(bero_prev_return))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SWITCH(LW(0))
        CASE_LESS(49)
        CASE_BETWEEN(49, 69)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_prev_return2))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_on)
        CASE_BETWEEN(70, 98)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_prev_return3))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_on)
        CASE_DEFAULT
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_prev_return4))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_on)
    END_SWITCH
    RETURN
EVT_END()

/** Pipes of an ordinary floor: the entrance above (dokan_2, set by script) and the pipe down to jon_00. */
static BeroEntry bero_normal[] = {
    {
        "dokan_2", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_UP, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Replacement for the pipe down (dokan_1) of an ordinary floor on floors 50 to 80: it leads to jon_01. */
static BeroEntry bero_normal2[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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

/** Replacement for the pipe down (dokan_1) of an ordinary floor above floor 80: it leads to jon_02. */
static BeroEntry bero_normal3[] = {
    {
        "dokan_1", // hitName
        2, // type
        0, // sfxId
        BERO_DIR_DOWN, // directionId
        BERO_POS_AUTO, // centerPosition
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

/**
 * Setup for ordinary floors: registers the pipes and points exit pipe dokan_1 at the next map of the stretch:
 * jon_00 up to floor 49, jon_01 for floors 50-80 and jon_02 beyond.
 */
static EVT_BEGIN(setup_normal)
    SET_VALUE(LW(0), PTR(bero_normal))
    CALL(evt_bero_get_info)
    CALL_CHILD(evt_bero_info_run)
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    SWITCH(LW(0))
        CASE_LESS(50)
        CASE_BETWEEN(50, 80)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_normal2))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_on)
        CASE_GREATER(80)
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_off)
        CALL(evt_bero_overwrite, STRING("dokan_1"), PTR(bero_normal3))
        SET_VALUE(LW(0), STRING("dokan_1"))
        CALL_CHILD(bero_case_switch_on)
    END_SWITCH
    RETURN
EVT_END()

/**
 * Trouble Center job 12 (the lost sailor, マッキノ):
 * when Mario enters a pipe with the sailor following, he hops in too, spinning and shrinking as a paper model,
 * and sinks into the pipe.
 */
static EVT_BEGIN(iri_12_dokan_in_sub)
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

static EVT_BEGIN(iri_12_dokan_in)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_MY_DADDY)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)
                SPAWN_EVENT(iri_12_dokan_in_sub)
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Shared tail of the sailor's farewell scripts:
 * he turns into a paper model, walks into the pipe to the side spinning and shrinking,
 * and is moved far below the map.
 */
static EVT_BEGIN(iri_12_makkino_return_dokan)
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
        CALL(evt_npc_status_onoff, 0, STRING("マッキノ"), NPC_STATUS_FACE_MOVE_DIR)
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

/** Sailor's farewell at a rest room: he walks over, thanks Mario (irai_12_011) and leaves through the pipe. */
static EVT_BEGIN(iri_12_makkino_return)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("マッキノ"), 72, 0, 0, 120, 4)
    CALL(evt_msg_print, 0, STRING("irai_12_011"), 0, STRING("マッキノ"))
    CALL(evt_npc_move_position, STRING("マッキノ"), 98, -123, 0, 120, 4)
    CALL_CHILD(iri_12_makkino_return_dokan)
    RETURN
EVT_END()

/**
 * Sailor's farewell on floor 100 after Bonetail:
 * he walks over, thanks Mario (irai_12_011), controls return, and he leaves through the pipe.
 */
static EVT_BEGIN(iri_12_makkino_return_b100)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_move_position, STRING("マッキノ"), -205, -75, 0, 120, 4)
    CALL(evt_msg_print, 0, STRING("irai_12_011"), 0, STRING("マッキノ"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_move_position, STRING("マッキノ"), -76, -310, 0, 180, 4)
    CALL(evt_npc_move_position, STRING("マッキノ"), 98, -486, 0, 180, 4)
    CALL_CHILD(iri_12_makkino_return_dokan)
    RETURN
EVT_END()

/**
 * Sailor's farewell when Bonetail falls while the job is running:
 * he walks up, thanks Mario (irai_12_011), then leaves through the pipe.
 */
static EVT_BEGIN(iri_12_makkino_return_zonbaba)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    WAIT_MS(500)
    CALL(evt_npc_move_position, STRING("マッキノ"), -337, -124, 0, 120, 4)
    CALL(evt_msg_print, 0, STRING("irai_12_011"), 0, STRING("マッキノ"))
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_move_position, STRING("マッキノ"), -76, -310, 0, 180, 4)
    CALL(evt_npc_move_position, STRING("マッキノ"), 98, -486, 0, 180, 4)
    CALL_CHILD(iri_12_makkino_return_dokan)
    RETURN
EVT_END()

/**
 * Moves the sailor to the side of the exit pipe (x = -40), stopping his own movement script first.
 * If Mario stands in the way he is nudged aside. Afterwards the sailor talks again (iri_12_makkino_talk).
 */
static EVT_BEGIN(iri_12_makkino_move_dokan_side)
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

/**
 * Death hook of the floor's enemy: while job 12 is on and the sailor follows Mario,
 * wait 0.8 s and send the sailor to the pipe (iri_12_makkino_move_dokan_side).
 */
static EVT_BEGIN(iri_12_enemy_dead_event)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_MY_DADDY)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)
                WAIT_MS(800)
                SPAWN_EVENT(iri_12_makkino_move_dokan_side)
            END_IF
        END_IF
    END_IF
    RETURN
EVT_END()

/** Sailor's nervous pacing while an enemy is still alive: walks back and forth between x = -175 and x = -73 forever. */
static EVT_BEGIN(iri_12_makkino_urouro)
    LOOP(0)
        CALL(evt_npc_move_position, STRING("マッキノ"), -175, 109, 0, 120, 4)
        WAIT_FRAMES(1)
        CALL(evt_npc_move_position, STRING("マッキノ"), -73, 109, 0, 120, 4)
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

/** Sailor trembling in fear on floor 100 before Bonetail is beaten: walks to x = -440, then shakes sideways forever. */
static EVT_BEGIN(iri_12_makkino_buruburu)
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

/**
 * Wakes the sailor: plays his wake-up animation,
 * restores normal talk and move animations and turns him toward Mario.
 * Started by the message callback when his text reaches page 2.
 */
EVT_BEGIN(iri_12_makkino_okiru)
    CALL(evt_npc_set_anim, STRING("マッキノ"), STRING("O_1"))
    CALL(evt_npc_wait_anim, STRING("マッキノ"), 1)
    CALL(evt_npc_set_balloontype, STRING("マッキノ"), 1)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_flag_onoff, 0, STRING("マッキノ"), NPC_FLAG_NO_TALK_TURN)
    CALL(evt_npc_status_onoff, 1, STRING("マッキノ"), NPC_STATUS_FACE_MOVE_DIR)
    CALL(evt_set_dir_to_target, STRING("マッキノ"), STRING("mario"))
    RETURN
EVT_END()

/**
 * Talk script of the sailor Makkino (マッキノ) in Trouble Center job 12.
 * With no enemy left on the floor, his first talk plays irai_12_008, sets GSWF_TROUBLE_WORK(1),
 * drops his talk script and sends him to the pipe side; later talks play irai_12_010.
 * With an enemy still around he says irai_12_007 before the first talk and irai_12_009 after it.
 */
static EVT_BEGIN(iri_12_makkino_talk)
    CALL(evt_npc_check, STRING("敵"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
            CALL(evt_msg_print, 0, STRING("irai_12_008"), PTR(iri_12_msgcb), STRING("マッキノ"))
            SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
            CALL(evt_npc_change_interrupt, STRING("マッキノ"), 6, 0)
            SPAWN_EVENT(iri_12_makkino_move_dokan_side)
        ELSE
            CALL(evt_msg_print, 0, STRING("irai_12_010"), 0, STRING("マッキノ"))
        END_IF
    ELSE
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
            CALL(evt_msg_print, 0, STRING("irai_12_007"), 0, STRING("マッキノ"))
        ELSE
            CALL(evt_msg_print, 0, STRING("irai_12_009"), 0, STRING("マッキノ"))
        END_IF
    END_IF
    RETURN
EVT_END()

/**
 * Sailor drops in on an ordinary floor after Mario descends:
 * appears at the pipe start, lands, and either walks to the pipe side (no enemy) or paces nervously
 * until the enemy is beaten. Re-enables his talk script.
 */
static EVT_BEGIN(iri_12_makkino_fall)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, BERO_EXEC_MARIO_MOVING)
    CALL(evt_bero_get_start_position, LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY)
    CALL(wait_landon)
    CALL(evt_npc_check, STRING("敵"), LW(0))
    IF_INT_EQUAL(LW(0), 0)
        CALL_CHILD(iri_12_makkino_move_dokan_side)
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

/**
 * Sailor drops in on a rest-room floor: lands, then does his farewell (iri_12_makkino_return),
 * or the Bonetail variants on floor 100, and marks the job's return flag (GSWF_TROUBLE_WORK(2)).
 */
static EVT_BEGIN(iri_12_makkino_fall_return)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_bero_exec_wait, BERO_EXEC_MARIO_MOVING)
    CALL(evt_bero_get_start_position, LW(0), LW(1), LW(2))
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_set_position, STRING("マッキノ"), LW(0), LW(1), LW(2))
    CALL(evt_bero_exec_wait, BERO_EXEC_ENTRY_DONE)
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_NO_HIT | NPC_FLAG_GRAVITY)
    CALL(wait_landon)
    IF_INT_EQUAL(GSW_PIT_FLOOR, 99)
        IF_INT_EQUAL(GSWF_PIT_BONETAIL_DEFEATED, 1)
            SPAWN_EVENT(iri_12_makkino_return_b100)
            SET_VALUE(GSWF_TROUBLE_WORK(2), 1)
        ELSE
            CALL(evt_npc_change_interrupt, STRING("マッキノ"), 1, PTR(iri_12_makkino_buruburu))
            CALL(evt_npc_restart_regular_event, STRING("マッキノ"))
            CALL(evt_mario_key_onoff, 1)
        END_IF
    ELSE
        CALL_CHILD(iri_12_makkino_return)
        SET_VALUE(GSWF_TROUBLE_WORK(2), 1)
        CALL(evt_mario_key_onoff, 1)
    END_IF
    RETURN
EVT_END()

/**
 * Sets up the sailor model (c_suifu_k, tribe 水夫)
 * and starts the drop-in script for a rest room (iri_12_makkino_fall_return)
 * or an ordinary floor (iri_12_makkino_fall).
 */
static EVT_BEGIN(iri_12_init_fall)
    CALL(evt_npc_entry, STRING("マッキノ"), STRING("c_suifu_k")) // Pine T. Sr.
    CALL(evt_npc_set_tribe, STRING("マッキノ"), STRING("水夫"))
    CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_TALK_POSE | NPC_FLAG_AUTO_MOVE_ANIM)
    CALL(evt_npc_status_onoff, 1, STRING("マッキノ"), NPC_STATUS_FACE_MOVE_DIR)
    SET_VALUE(LW(0), GSW_PIT_FLOOR)
    ADD(LW(0), 1)
    MODULO(LW(0), 10)
    IF_INT_EQUAL(LW(0), 0)
        SPAWN_EVENT(iri_12_makkino_fall_return)
    ELSE
        SPAWN_EVENT(iri_12_makkino_fall)
    END_IF
    RETURN
EVT_END()

static EVT_BEGIN(iri_12_init)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_MY_DADDY)
        SWITCH(GSW_PIT_FLOOR)
            CASE_EQUAL(17)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 0)
                CALL(evt_npc_entry, STRING("マッキノ"), STRING("c_suifu_k"))
                CALL(evt_npc_set_tribe, STRING("マッキノ"), STRING("水夫"))
                CALL(evt_npc_set_position, STRING("マッキノ"), -50, 0, -120)
                CALL(evt_npc_set_ry, STRING("マッキノ"), 90)
                CALL(evt_npc_flag_onoff, 1, STRING("マッキノ"), NPC_FLAG_NO_TALK_TURN)
                CALL(evt_npc_change_interrupt, STRING("マッキノ"), 6, PTR(iri_12_makkino_talk))
                CALL(evt_npc_set_anim, STRING("マッキノ"), STRING("D_2"))
                CALL(evt_npc_set_balloontype, STRING("マッキノ"), 0)
            ELSE
                CALL_CHILD(iri_12_init_fall)
            END_IF
            CASE_DEFAULT
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
                IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)
                    CALL_CHILD(iri_12_init_fall)
                END_IF
            END_IF
        END_SWITCH
    END_IF
    RETURN
EVT_END()

/**
 * Examining the graffiti wall on floor 50 (Trouble Center job 30):
 * a short message (irai_30_007), then control is returned.
 */
static EVT_BEGIN(evt_iri_30_touch_rakugaki)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_msg_print, 0, STRING("irai_30_007"), 0, 0)
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/**
 * Bobbery's bomb hits the floor-50 graffiti (job 30):
 * closes the message, plays the wall-break sound,
 * captures the wall into an offscreen image and animates it away,
 * swapping the "before" objects for the "after" ones. Sets GSWF_TROUBLE_WORK(1).
 */
static EVT_BEGIN(evt_iri_30_bomb_rakugaki)
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
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_img_clear_virtual_point, STRING("img"))
        CALL(evt_img_onoff, STRING("img"), 1)
        CALL(evt_img_set_paper_anim, STRING("img"), STRING("A_1"))
        CALL(evt_img_wait_animend, STRING("img"))
    ELSE
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("s_rakugaki"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("before"), MAP_OBJ_FLAG_HIDDEN)
        CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("after"), MAP_OBJ_FLAG_HIDDEN)
    END_IF
    CALL(evt_img_release, STRING("img"))
    CALL(evt_mapobj_clear_offscreen, 1, STRING("s_rakugaki"))
    CALL(evt_paper_delete, STRING("OFF_d_bom_ver2"))
    CALL(evt_offscreen_delete, STRING("ofs"))
    SET_VALUE(GSWF_TROUBLE_WORK(1), 1)
    CALL(evt_exit_case_evt)
    RETURN
EVT_END()

/**
 * Part of the Bonetail (Gonbaba) intro: 3.7 seconds in,
 * plays its footstep sound at the enemy and shakes the camera for 800 ms.
 */
static EVT_BEGIN(gonbaba_standup_shake)
    WAIT_MS(3700)
    CALL(evt_npc_get_position, STRING("敵"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG1_GNB_STEP1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.009765625f), FLOAT(0.0f), 800)
    RETURN
EVT_END()

/**
 * Part of the Bonetail intro: over 8 seconds it pushes the map fog's near
 * and far distances out (by 400 and 1600 units), so the room's fog thins as he rises.
 */
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
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, 400, LW(10))
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
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 0, 1600, LW(10))
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

/**
 * Bonetail slumping to the ground after it is beaten on floor 100:
 * plays its two falling sounds (the second is positional and fades out over 10 seconds) and shakes the camera,
 * starting strong and dying down to a rumble.
 */
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
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.049804688f), FLOAT(0.0f), 500)
    CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 50, 5, 50)
    LOOP(0)
        CALL(evt_sub_intpl_get_value)
        DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
        CALL(evt_cam_shake, CAMERA_3D, LW(0), FLOAT(0.0f), 50)
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
    END_LOOP
    CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 2000)
    RETURN
EVT_END()

EVT_BEGIN(zonbaba_first_event)
    // If Bonetail was already beaten, he is moved far below the map and the room is set up with only the chest.
    IF_INT_EQUAL(GSWF_PIT_BONETAIL_DEFEATED, 1)
        CALL(evt_npc_set_position, STRING("敵"), 0, -1000, 0)
        CALL(evt_npc_flag_onoff, 0, STRING("敵"), NPC_FLAG_ENEMY | NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(evt_mobj_itembox, STRING("box"), 270, 0, 200, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_100_RETURN_POSTAGE_CHEST)
        CALL(evt_hitobj_onoff, STRING("baba"), 1, 0)
        RETURN
    END_IF

    // Fog closes in and control is taken from Mario while Bonetail is placed in front of him, asleep.
    CALL(evt_map_set_fog, 2, 400, 1000, 0, 10, 40)
    CALL(evt_map_fog_onoff, 1)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_mario_normalize)
    CALL(evt_npc_set_position, STRING("敵"), 175, 0, 0)
    CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_S_1"))
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 1, STRING("oFF_jon_06"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_seq_wait, SEQ_GAME)
#if VERSION <= VERSION_DEMO_JP
    WAIT_MS(4000)
#else
    WAIT_MS(3500)
#endif
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_SURPRISED2_3"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 96)
    CALL(evt_mario_set_pose, STRING("M_N_5B"))
#if VERSION >= VERSION_JP
    BEGIN_INLINE_EVENT
        SET_VALUE(LW(3), LW(0))
        ADD(LW(3), -40)
        CALL(evt_party_move_pos, 0, LW(3), LW(2), 250)
    END_INLINE_EVENT
#endif
    WAIT_MS(2000)
    CALL(evt_mario_set_pose, STRING("M_S_1"))
    CALL(evt_mario_set_dir_npc, STRING("敵"))
    CALL(evt_party_set_dir_npc, 0, STRING("敵"))

    // Bonetail wakes with a roar and stands up while the ground shakes.
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_EVT_GONBABA_FLY2"))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d_ex, STRING("SFX_STG1_GNB_ROAR2"), 0, 0, 0, 255, 255, 4300, 16, 0)
        CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_U_1"))
        CALL(evt_npc_wait_anim, STRING("敵"), FLOAT(1.0f))
        CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_S_1"))
    END_INLINE_EVENT
    SPAWN_EVENT(gonbaba_standup_shake)
    SPAWN_EVENT(gonbaba_fog_change)

    // The camera pulls back to show his size; the screen shakes.
    CALL(evt_cam3d_evt_set, -755, 24, 376, -84, 114, -22, 4500, EASING_SINE_IN_OUT)
    WAIT_MS(4000)
    LOOP(30)
        CALL(evt_cam3d_evt_set, -755, 24, 376, -84, 114, -22, 15, EASING_SINE_IN_OUT)
        WAIT_MS(15)
        CALL(evt_cam3d_evt_set, -745, 23, 366, -84, 114, -22, 15, EASING_SINE_IN_OUT)
        WAIT_MS(15)
    END_LOOP
    WAIT_MS(3000)

    // The Bonetail battle starts and the script waits for it to end.
    CALL(evt_npc_set_battle_info, STRING("敵"), 51)
    CALL(evt_npc_battle_start, STRING("敵"))
    CALL(evt_npc_wait_battle_end)
    CALL(evt_cam3d_evt_set, -769, 50, 527, -22, 114, 7, 1, EASING_SINE_IN_OUT)

    // Bonetail is beaten: he is placed at the room centre and collapses, shaking the room as he falls.
    CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_H_1"))
    CALL(evt_map_fog_onoff, 0)
    CALL(evt_npc_set_position, STRING("敵"), 0, 60, 0)
    CALL(evt_mario_set_pos, 25, 10, 150)
    CALL(evt_party_set_pos, 0, -25, 10, 150)
    CALL(evt_mario_set_dir, 270, 0, 0)
    CALL(evt_party_set_dir, 0, 90, -1)
    SPAWN_EVENT(gonbaba_down_shake)
    BEGIN_INLINE_EVENT
        WAIT_MS(300)
        CALL(evt_npc_flag_onoff, 1, STRING("敵"), NPC_FLAG_FLOOR_COLLISION | NPC_FLAG_GRAVITY)
        CALL(gonbaba_wait_landon)
        CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_H_2"))
    END_INLINE_EVENT
    CALL(evt_npc_get_position, STRING("敵"), LW(0), LW(1), LW(2))
    BEGIN_INLINE_EVENT
        CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GNB_DOWN2"), LW(0), LW(1), LW(2), LW(3))
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.049804688f), FLOAT(0.0f), 500)
        CALL(evt_sub_intpl_init, EASING_SINE_IN_OUT, 50, 5, 50)
        LOOP(0)
            CALL(evt_sub_intpl_get_value)
            DIVIDE_FLOAT(LW(0), FLOAT(1000.0f))
            CALL(evt_cam_shake, CAMERA_3D, LW(0), FLOAT(0.0f), 50)
            IF_INT_EQUAL(LW(1), 0)
                BREAK_LOOP
            END_IF
        END_LOOP
        CALL(evt_cam_shake, CAMERA_3D, FLOAT(0.0048828125f), FLOAT(0.0f), 2000)
        CALL(evt_snd_sfxoff, LW(3))
    END_INLINE_EVENT
    WAIT_MS(1000)
    WAIT_MS(5000)
    CALL(evt_npc_get_position, STRING("敵"), LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_STG8_GNB_DOWN3"), LW(0), LW(1), LW(2), 0)
    WAIT_MS(600)
    CALL(evt_cam3d_evt_set, -145, 80, 882, 104, 114, 81, 1000, EASING_SINE_IN_OUT)
    WAIT_MS(1000)

    // His mouth opens and the treasure chest tumbles out, bouncing across the floor.
    CALL(evt_mobj_itembox, STRING("box"), 250, 60, 0, MOBJ_ITEMBOX_BIG, 0, PTR(evt_open_box), GSWF_PIT_100_RETURN_POSTAGE_CHEST)
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

    // Mario and his partner react to the chest; the room returns to normal.
    CALL(evt_npc_set_anim, STRING("敵"), STRING("GNB_H_3"))
    WAIT_MS(200)
    CALL(evt_cam3d_evt_set, 255, 105, 648, 255, 55, 150, 500, EASING_SINE_IN_OUT)
    WAIT_MS(1800)
    CALL(evt_cam3d_evt_off, 0, EASING_SINE_IN_OUT)
    WAIT_FRAMES(1)
    CALL(evt_mario_set_dir, 90, 300, 0)
    WAIT_MS(300)
    CALL(evt_mario_get_pos, 0, LW(0), LW(1), LW(2))
    CALL(evt_snd_sfxon_3d, STRING("SFX_VOICE_MARIO_FIND1"), LW(0), LW(1), LW(2), 0)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_MARIO, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    CALL(evt_eff_fukidashi, FUKIDASHI_AT_PARTNER, STRING(""), 0, 0, 0, 0, 0, 0, 0, 0, 60)
    WAIT_MS(1000)
    CALL(evt_mapobj_flag_onoff, MAPOBJ_GROUP, 0, STRING("oFF_jon_06"), MAP_OBJ_FLAG_HIDDEN)
    CALL(evt_npc_flag_onoff, 1, STRING("敵"), NPC_FLAG_NO_HIT)
    CALL(evt_npc_flag_onoff, 0, STRING("敵"), NPC_FLAG_ENEMY)
    SET_VALUE(GSWF_PIT_BONETAIL_DEFEATED, 1)
    IF_INT_EQUAL(GSW_TROUBLE_ACCEPTED, TROUBLE_HELP_MY_DADDY)
        IF_INT_EQUAL(GSWF_TROUBLE_WORK(1), 1)
            IF_INT_EQUAL(GSWF_TROUBLE_WORK(2), 0)
                CALL(evt_npc_get_reglid, STRING("マッキノ"), LW(0))
                DELETE_EVENT(LW(0))
                CALL(evt_npc_change_interrupt, STRING("マッキノ"), 1, 0)
                CALL(evt_mario_key_onoff, 0)
                SPAWN_EVENT(iri_12_makkino_return_zonbaba)
                SET_VALUE(GSWF_TROUBLE_WORK(2), 1)
            END_IF
        END_IF
    END_IF
    CALL(evt_mario_key_onoff, 1)
    RETURN
EVT_END()

/** Script helper: blocks until Bonetail's NPC is standing on the ground (groundHit set). */
static USER_FUNC(gonbaba_wait_landon) {
    // 敵
    return npcNameToPtr("\223G")->groundHit ? EVT_RETURN_DONE : EVT_RETURN_BLOCK;
}

/**
 * Message window callback (msgdrv calls it with the window's smart-heap block):
 * starts iri_12_makkino_okiru (マッキノ wakes up) when the message turns to its second page (type 3 = page
 * advanced).
 */
static void iri_12_msgcb(s32 type, void* param) {
    u8* msg = *(u8**)param;

    if (type == 3 && *(u16*)(msg + 8) == 1) {
        evtEntry(iri_12_makkino_okiru, 0, 0);
    }
}

/** Script helper: blocks until the sailor NPC has landed on the floor (returns at once if he cannot land). */
USER_FUNC(wait_landon) {
    // マッキノ
    NpcEntry* npc = npcNameToPtr("\203}\203b\203L\203m");

    if (!(npc->flags & NPC_FLAG_FLOOR_COLLISION) || npc->groundHit != NULL) {
        return EVT_RETURN_DONE;
    }
    return EVT_RETURN_BLOCK;
}

/** Script helper: args[0] = y; sets the sailor NPC's height directly. */
USER_FUNC(iri_12_makkino_set_y) {
    f32 y = evtGetFloat(event, event->args[0]);

    npcNameToPtr("\203}\203b\203L\203m")->position.y = y;
    return EVT_RETURN_DONE;
}

/** Script helper: turns off paper-peraform bending and paper lighting on the sailor's model, for the pipe jump. */
USER_FUNC(iri_12_paper_init) {
    NpcEntry* npc = npcNameToPtr("\203}\203b\203L\203m");

    animPosePaperPeraOff(npc->poseId);
    animPoseSetMaterialLightFlagOff(npc->poseId, MATERIAL_LIGHT_PAPER);
    return EVT_RETURN_DONE;
}

USER_FUNC(shikakeFlag) {
    s32 type = evtGetValue(event, event->args[0]);
    NpcEntry* npc = npcNameToPtr("\223G");

    switch (type) {
        case 0:
            npc->battleInfo.battleFlags |= NPC_BATTLE_NO_PARTY_CHANGE;
            break;
        case 1:
            npc->battleInfo.battleFlags |= NPC_BATTLE_NO_HAMMER;
            break;
        case 2:
            npc->battleInfo.battleFlags |= NPC_BATTLE_NO_JUMP;
            break;
        case 3:
            npc->battleInfo.battleFlags |= NPC_BATTLE_EXTRA_COINS;
            break;
    }
    return EVT_RETURN_DONE;
}

/** Script helper: args[0..2] = x, y, z; writes them into the Chain Chomp setup's territoryBase before it is spawned. */
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

/**
 * Script helper: args[0] = NPC name; sets its loiter height to 50 and homing height to 180
 * so flyers can chase over the room.
 */
USER_FUNC(setup_npc_ex_para) {
    const char* name = (const char*)evtGetValue(event, event->args[0]);
    NpcEntry* npc = evtNpcNameToPtr(event, name);

    npc->territoryLoiter.y = 50.0f;
    npc->territoryHoming.y = 180.0f;
    return EVT_RETURN_DONE;
}

/** Script helper: sets NPC_BATTLE_NO_BUMP_ATTACK on the floor enemy's battle info. */
USER_FUNC(yattukeFlag) {
    npcNameToPtr("\223G")->battleInfo.battleFlags |= NPC_BATTLE_NO_BUMP_ATTACK;
    return EVT_RETURN_DONE;
}

/**
 * Script helper: args[0] = floor index (0-99).
 * Looks up its battle number in enemy_100 and returns the tribe's model name (args[1]),
 * Japanese name (args[2]) and battle setup number (args[3]) from btlsetup_jon_tbl;
 * falls back to Goomba when the name is not a tribe.
 */
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

/**
 * Script helper: the warp wobble shown during the mover's trip.
 * Blocks forever, growing the amplitude 0.2 and phase 0.01 per frame,
 * and queues raster_disp each frame on the 2D camera.
 */
USER_FUNC(evt_raster) {
    f32* work = event->lwDataF;

    if (isFirstCall) {
        work[0] = 0.0f;
        work[1] = 0.0f;
    }
    work[0] += 0.2f;
    work[1] += 0.01f;
    dispEntry(CAMERA_2D, 0.0f, DISP_MODE_OPAQUE_NO_Z, raster_disp, event);
    return EVT_RETURN_BLOCK;
}

/**
 * Offset of the pixel (x, y) in a 4x4-tiled IA8 texture 16 tiles (64 pixels) wide, with the roles of x and y
 * swapped: y runs along a row of tiles and x down the rows. raster_disp writes the same arithmetic out by hand.
 */
inline s32 ia8_ofs(s32 x, s32 y) {
    int off = (y / 4) << 5;
    off += (x / 4) << 9;
    off += ((y % 4) + ((x % 4) << 2)) * 2;
    return off;
}

/**
 * Draw callback of the mover's warp: copies the screen to a texture
 * and draws it back through a 64x64 sine-wave indirect texture (amplitude capped at 127),
 * producing the wavy distortion.
 */
static void raster_disp(CameraId cameraId, void* param) {
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
    copyBuf = smartAlloc(GXGetTexBufferSize(608, 480, GX_TF_RGBA8, GX_FALSE, 0), SMART_GROUP_TEMP)->address;
    buf = smartAlloc(GXGetTexBufferSize(64, 64, GX_TF_IA8, GX_FALSE, 0), SMART_GROUP_TEMP)->address;
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

/** Unresolved-import stub for the REL (empty). */
void _unresolved(void) {
}

/** REL exit point: runs the static destructors. */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

/**
 * REL entry point: runs the static constructors
 * and registers jon_init_evt as the init script of the seven jon maps (jon_00..jon_06).
 */
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
