#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "gra.h"
#include "data_tables.h"
#include "evt/evt_mario.h"
#include "evt/evt_msg.h"
#include "evt/evt_npc.h"
#include "evt/evt_party.h"
#include "evt/evt_seq.h"
#include "evt/evt_snd.h"
#include "party/party_motion.h"
#include "evt/evt_mobj.h"
#include "driver/seqdrv.h"
#include "pmario_sound.h"

USER_FUNC(evt_kagemario_init);

static BeroEntry bero_entry_data[3];
EvtWord gra_01_after_ranpel2[94];
static TreeAccess tree_tbl[4];
EvtWord gra_01_init_evt[107];

static BeroEntry bero_entry_data[] = {
    {
        "w_bero1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gra_00", // nextMapName
        "e_bero", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {
        "e_bero1", // hitName
        0, 0, // type..sfxId
        20000, // directionId
        100000, // centerPosition
        0, 0, // centerPositionY..centerPositionZ
        -1, // length
        NULL, // enterEvent
        -1, // caseType
        NULL, // exitEvent
        "gra_02", // nextMapName
        "w_bero_1", // nextBeroName
        1, // enterAnimeType
        1, // exitAnimeType
        0, 0, // enterAnimeArg..exitAnimeArg
    },
    {0},
};

EVT_BEGIN(gra_01_after_ranpel2)
    CALL(evt_mario_key_onoff, 0)
    CALL(evt_party_run, 0)
    CALL(evt_party_set_pos, 0, -640, 0, 0)
    CALL(evt_mario_set_pos, -610, 0, 0)
    CALL(evt_gra_vivian_pose_set, 1, STRING("PTR_S_4"))
    CALL(evt_gra_vivian_pose_set, 2, STRING("PTR_S_4"))
    CALL(evt_gra_vivian_pose_set, 3, STRING("PTR_S_4"))
    CALL(evt_gra_vivian_pose_set, 7, STRING("PTR_T_5"))
    CALL(evt_party_set_homing_dist, 0, FLOAT(20.0f))
    CALL(evt_seq_wait, SEQ_GAME)
    CALL(evt_mario_mov_pos2, -540, 0, FLOAT(150.0f))
    CALL(evt_mario_set_dir, 270, 300, 0)
    CALL(evt_party_stop, 0)
    WAIT_MS(500)
    CALL(evt_msg_print_party, STRING("stg4_gra_16_01"))
    CALL(evt_gra_vivian_pose_set, 1, STRING("PTR_S_1"))
    CALL(evt_gra_vivian_pose_set, 2, STRING("PTR_W_1"))
    CALL(evt_gra_vivian_pose_set, 3, STRING("PTR_R_1"))
    CALL(evt_gra_vivian_pose_set, 7, STRING("PTR_T_1"))
    SET_VALUE(GSWF(2075), 1)
    CALL_CHILD(PTR(bero_case_entry))
    CALL(evt_party_set_homing_dist, 0, FLOAT(50.0f))
    CALL(evt_party_run, 0)
    CALL(evt_mario_key_onoff, 1)
    CALL(evt_npc_change_fbat_mode, 1)
    RETURN
EVT_END()

static TreeAccess tree_tbl[] = {
    {2, "A_ki_1", "S_gra01_ki01", NULL, 0, NULL},
    {2, "A_ki_2", "S_gra01_ki02", NULL, 0, NULL},
    {2, "A_ki_3", "S_gra01_ki03", NULL, 0, NULL},
    {7, NULL, NULL, NULL, 0, NULL},
};

EVT_BEGIN(gra_01_init_evt)
    CALL(evt_snd_bgmon, PSND_FADEOUT, STRING("BGM_STG4_GRA1"))
    CALL(evt_snd_envon, PSND_RESTART | PSND_FADEIN, STRING("ENV_STG4_USU1"))
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL(evt_mobj_badgeblk, STRING("blk00"), 0, 0, -100, ITEM_COIN, 0, GSWF(2082), 0)
    CALL(evt_mobj_brick, STRING("blk02"), -350, 0, -100, ITEM_NULL, MOBJ_BRICK_PLAIN, 0, LSWF(2))
    CALL(evt_mobj_brick, STRING("blk03"), -200, 60, 100, ITEM_NULL, MOBJ_BRICK_PLAIN, 0, LSWF(3))
    CALL(evt_mobj_brick, STRING("blk04"), 200, 60, 100, ITEM_NULL, MOBJ_BRICK_PLAIN, 0, LSWF(4))
    CALL(evt_mobj_brick, STRING("blk05"), 0, 85, -100, ITEM_SUPER_SHROOM, MOBJ_BRICK_HIDDEN_ITEM, 0, GSWF(2087))
    CALL(evt_mobj_brick, STRING("blk06"), 475, 60, -100, ITEM_NULL, MOBJ_BRICK_TEN_COIN, 0, GSWF(2088))
    SET_VALUE(LF(0), 0)
    IF_INT_GREATER_OR_EQUAL(GSW_STORY_PROGRESS, STG4_13_VIVIAN_JOINS)
        IF_INT_EQUAL(GSWF(2075), 0)
            SET_VALUE(LF(0), 1)
        END_IF
    END_IF
    IF_INT_EQUAL(LF(0), 0)
        CALL_CHILD(PTR(evt_bero_info_run))
        ELSE
        SPAWN_EVENT(PTR(gra_01_after_ranpel2))
    END_IF
    CALL_CHILD(PTR(evt_usu_kagemario_party_kill))
    RETURN
EVT_END()

