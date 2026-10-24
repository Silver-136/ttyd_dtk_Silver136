#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "manager/evtmgr_cmd.h"
#include "dolphin/mtx.h"
#include "tik.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

USER_FUNC(evt_npc_add_dirdist);
USER_FUNC(evt_npc_get_loiter_dir);
USER_FUNC(evt_npc_get_position);
USER_FUNC(evt_npc_move_position);
USER_FUNC(evt_npc_reverse_ry);
USER_FUNC(evt_npc_set_ry);
USER_FUNC(evt_npc_status_onoff);
USER_FUNC(evt_sub_get_dir);
USER_FUNC(evt_sub_random);
USER_FUNC(evt_tik_villager_urouro_check_func);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

f32 fd_tik_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_tik_neg_one_382;
f32 fd_tik___local_PI_383;
f32 fd_tik___local_3PIO2_385;
f32 fd_tik_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_tik___local_PIO2_384;
f32 fd_tik_one_381;
f32 fd_tik_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_tik_neg_one_365;
f32 fd_tik___local_PI_366;
f32 fd_tik___local_3PIO2_368;
f32 fd_tik_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_tik___local_PIO2_367;
f32 fd_tik_one_364;
f64 fd_tik__three_localstatic4_sqrtf;
f64 fd_tik__half_localstatic3_sqrtf;
EvtWord tik_villager_urouro[101];

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("tik_00", tik_00_init_evt);
    relSetEvtAddr("tik_01", tik_01_init_evt);
    relSetEvtAddr("tik_02", tik_02_init_evt);
    relSetEvtAddr("tik_03", tik_03_init_evt);
    relSetEvtAddr("tik_04", tik_04_init_evt);
    relSetEvtAddr("tik_05", tik_05_init_evt);
    relSetEvtAddr("tik_06", tik_06_init_evt);
    relSetEvtAddr("tik_07", tik_07_init_evt);
    relSetEvtAddr("tik_08", tik_08_init_evt);
    relSetEvtAddr("tik_09", tik_09_init_evt);
    relSetEvtAddr("tik_10", tik_10_init_evt);
    relSetEvtAddr("tik_11", tik_11_init_evt);
    relSetEvtAddr("tik_12", tik_12_init_evt);
    relSetEvtAddr("tik_13", tik_13_init_evt);
    relSetEvtAddr("tik_14", tik_14_init_evt);
    relSetEvtAddr("tik_15", tik_15_init_evt);
    relSetEvtAddr("tik_16", tik_16_init_evt);
    relSetEvtAddr("tik_17", tik_17_init_evt);
    relSetEvtAddr("tik_18", tik_18_init_evt);
    relSetEvtAddr("tik_19", tik_19_init_evt);
    relSetEvtAddr("tik_20", tik_20_init_evt);
    relSetEvtAddr("tik_21", tik_21_init_evt);
    relSetBtlAddr("tik", btlsetup_tik_tbl, btl_tik_setup_no_tbl);
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

void _unresolved(void) {
}

USER_FUNC(evt_tik_villager_urouro_check_func) {
    EvtWord* args = event->args;
    s32 xVar = args[0];
    s32 zVar = args[1];
    f32 cx = evtGetFloat(event, args[2]);
    f32 cz = evtGetFloat(event, args[3]);
    f32 range = evtGetFloat(event, args[4]);
    f32 x = evtGetFloat(event, xVar);
    f32 z = evtGetFloat(event, zVar);
    Vec pos;
    Vec center;

    pos = (Vec){x, 0.0f, z};
    center = (Vec){cx, 0.0f, cz};
    if (PSVECDistance(&center, &pos) <= range) {
        return EVT_RETURN_DONE;
    }
    evtSetFloat(event, xVar, cx);
    evtSetFloat(event, zVar, cz);
    return EVT_RETURN_DONE;
}

f32 fd_tik_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_neg_one_382 = -1.0f;

f32 fd_tik___local_PI_383 = 3.1415927f;

f32 fd_tik___local_3PIO2_385 = 4.712389f;

f32 fd_tik_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik___local_PIO2_384 = 1.5707964f;

f32 fd_tik_one_381 = 1.0f;

f32 fd_tik_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_tik_neg_one_365 = -1.0f;

f32 fd_tik___local_PI_366 = 3.1415927f;

f32 fd_tik___local_3PIO2_368 = 4.712389f;

f32 fd_tik_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_tik___local_PIO2_367 = 1.5707964f;

f32 fd_tik_one_364 = 1.0f;

f64 fd_tik__three_localstatic4_sqrtf = 3.0;

f64 fd_tik__half_localstatic3_sqrtf = 0.5;

EVT_BEGIN(tik_villager_urouro)
    CALL(evt_npc_status_onoff, 1, STRING("me"), 2)
    CALL(evt_npc_get_position, STRING("me"), LW(6), LW(7), LW(8))
    CALL(evt_sub_random, 1000, LW(0))
    ADD(LW(0), 100)
    WAIT_MS(LW(0))
    LOOP(0)
        CALL(evt_npc_reverse_ry, STRING("me"))
        CALL(evt_sub_random, 200, LW(0))
        ADD(LW(0), 400)
        WAIT_MS(LW(0))
        CALL(evt_npc_reverse_ry, STRING("me"))
        CALL(evt_sub_random, 200, LW(0))
        ADD(LW(0), 800)
        WAIT_MS(LW(0))
        CALL(evt_npc_get_position, STRING("me"), LW(0), LW(1), LW(2))
        CALL(evt_npc_get_loiter_dir, LW(3), FLOAT(120.0f), FLOAT(240.0f))
        CALL(evt_npc_add_dirdist, LW(0), LW(2), LW(3), FLOAT(50.0f))
        CALL(evt_tik_villager_urouro_check_func, LW(0), LW(2), LW(6), LW(8), FLOAT(70.0f))
        CALL(evt_npc_get_position, STRING("me"), LW(10), LW(11), LW(12))
        CALL(evt_sub_get_dir, LW(10), LW(12), LW(0), LW(2), LW(3))
        CALL(evt_npc_set_ry, STRING("me"), LW(3))
        WAIT_MS(500)
        CALL(evt_npc_move_position, STRING("me"), LW(0), LW(2), 0, FLOAT(30.0f), 4)
        WAIT_MS(1000)
    END_LOOP
EVT_END()

