#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "evt/evt_bero.h"
#include "evt/evt_sub.h"
#include "gra.h"
#include "data_tables.h"

#pragma force_active on

USER_FUNC(evt_bero_get_info);
USER_FUNC(evt_gra_vivian_pose_set);
USER_FUNC(evt_kagemario_init);
USER_FUNC(evt_mario_key_onoff);
USER_FUNC(evt_mario_mov_pos2);
USER_FUNC(evt_mario_set_dir);
USER_FUNC(evt_mario_set_pos);
USER_FUNC(evt_mobj_badgeblk);
USER_FUNC(evt_mobj_brick);
USER_FUNC(evt_msg_print_party);
USER_FUNC(evt_npc_change_fbat_mode);
USER_FUNC(evt_party_run);
USER_FUNC(evt_party_set_homing_dist);
USER_FUNC(evt_party_set_pos);
USER_FUNC(evt_party_stop);
USER_FUNC(evt_seq_wait);
USER_FUNC(evt_snd_bgmon);
USER_FUNC(evt_snd_envon);

f32 fd_gra_01_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_gra_01_neg_one_382;
f32 fd_gra_01___local_PI_383;
f32 fd_gra_01___local_3PIO2_385;
f32 fd_gra_01_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_gra_01___local_PIO2_384;
f32 fd_gra_01_one_381;
f32 fd_gra_01_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_gra_01_neg_one_365;
f32 fd_gra_01___local_PI_366;
f32 fd_gra_01___local_3PIO2_368;
f32 fd_gra_01_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_gra_01___local_PIO2_367;
f32 fd_gra_01_one_364;
static BeroEntry bero_entry_data[3];
EvtWord gra_01_after_ranpel2[94];
static TreeAccess tree_tbl[4];
EvtWord gra_01_init_evt[107];

f32 fd_gra_01_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gra_01_neg_one_382 = -1.0f;

f32 fd_gra_01___local_PI_383 = 3.1415927f;

f32 fd_gra_01___local_3PIO2_385 = 4.712389f;

f32 fd_gra_01_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gra_01___local_PIO2_384 = 1.5707964f;

f32 fd_gra_01_one_381 = 1.0f;

f32 fd_gra_01_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gra_01_neg_one_365 = -1.0f;

f32 fd_gra_01___local_PI_366 = 3.1415927f;

f32 fd_gra_01___local_3PIO2_368 = 4.712389f;

f32 fd_gra_01_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gra_01___local_PIO2_367 = 1.5707964f;

f32 fd_gra_01_one_364 = 1.0f;

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
    CALL(evt_seq_wait, 2)
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
    CALL(evt_snd_bgmon, 512, STRING("BGM_STG4_GRA1"))
    CALL(evt_snd_envon, 272, STRING("ENV_STG4_USU1"))
    CALL(evt_kagemario_init)
    SET_VALUE(LW(0), PTR(tree_tbl))
    CALL_CHILD(PTR(evt_sub_tree_access_entry))
    SET_VALUE(LW(0), PTR(bero_entry_data))
    CALL(evt_bero_get_info)
    CALL(evt_mobj_badgeblk, STRING("blk00"), 0, 0, -100, 121, 0, GSWF(2082), 0)
    CALL(evt_mobj_brick, STRING("blk02"), -350, 0, -100, 0, 0, 0, LSWF(2))
    CALL(evt_mobj_brick, STRING("blk03"), -200, 60, 100, 0, 0, 0, LSWF(3))
    CALL(evt_mobj_brick, STRING("blk04"), 200, 60, 100, 0, 0, 0, LSWF(4))
    CALL(evt_mobj_brick, STRING("blk05"), 0, 85, -100, 149, 11, 0, GSWF(2087))
    CALL(evt_mobj_brick, STRING("blk06"), 475, 60, -100, 0, 3, 0, GSWF(2088))
    SET_VALUE(LF(0), 0)
    IF_INT_GREATER_OR_EQUAL(GSW(0), 206)
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

