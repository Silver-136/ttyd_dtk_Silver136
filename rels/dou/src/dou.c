#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "manager/evtmgr_cmd.h"
#include "mario/mario.h"
#include "system.h"
#include "dou.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

USER_FUNC(dou_mario_pos_osreport_func);
USER_FUNC(evt_dou_normal_puka);
USER_FUNC(evt_sub_intpl_msec_get_value_para);
USER_FUNC(evt_sub_intpl_msec_init);
void _unresolved(void);
void _epilog(void);
void _prolog(void);

f32 fd_dou_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_dou_neg_one_382;
f32 fd_dou___local_PI_383;
f32 fd_dou___local_3PIO2_385;
f32 fd_dou_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_dou___local_PIO2_384;
f32 fd_dou_one_381;
f32 fd_dou_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_dou_neg_one_365;
f32 fd_dou___local_PI_366;
f32 fd_dou___local_3PIO2_368;
f32 fd_dou_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_dou___local_PIO2_367;
f32 fd_dou_one_364;
f64 fd_dou__three_localstatic4_sqrtf;
f64 fd_dou__half_localstatic3_sqrtf;
f32 dou_normal_puka_data[16];
EvtWord dou_normal_puka[19];
EvtWord dou_puka_event_sub[54];
EvtWord dou_puka_event[40];
EvtWord dou_mario_pos_osreport[4];

f32 fd_dou_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_neg_one_382 = -1.0f;

f32 fd_dou___local_PI_383 = 3.1415927f;

f32 fd_dou___local_3PIO2_385 = 4.712389f;

f32 fd_dou_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou___local_PIO2_384 = 1.5707964f;

f32 fd_dou_one_381 = 1.0f;

f32 fd_dou_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_dou_neg_one_365 = -1.0f;

f32 fd_dou___local_PI_366 = 3.1415927f;

f32 fd_dou___local_3PIO2_368 = 4.712389f;

f32 fd_dou_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_dou___local_PIO2_367 = 1.5707964f;

f32 fd_dou_one_364 = 1.0f;

f64 fd_dou__three_localstatic4_sqrtf = 3.0;

f64 fd_dou__half_localstatic3_sqrtf = 0.5;

f32 dou_normal_puka_data[] = {
    -0.0f, -0.5f, -1.0f, -1.5f, -2.0f, -2.2f, -2.5f, -2.7f,
    -2.7f, -2.7f, -2.5f, -2.2f, -2.0f, -1.5f, -1.0f, -0.5f,
};

EVT_BEGIN(dou_normal_puka)
    SET_VALUE(LW(0), 1)
    LOOP(0)
        CALL(evt_dou_normal_puka, LW(1))
        SET_FLOAT_INDEXED(LW(6), LW(1))
        ADD(LW(0), 1)
        WAIT_MS(130)
    END_LOOP
    RETURN
EVT_END()

EVT_BEGIN(dou_puka_event_sub)
    DEBUG_PUT_MSG(STRING("しずむ"))
    CALL(evt_sub_intpl_msec_init, 4, 0, LW(2), 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_INDEXED(LW(6), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    WAIT_MS(LW(3))
    DEBUG_PUT_MSG(STRING("うく"))
    CALL(evt_sub_intpl_msec_init, 1, LW(2), 0, 300)
    LOOP(0)
        CALL(evt_sub_intpl_msec_get_value_para, LW(0), LW(1))
        SET_FLOAT_INDEXED(LW(6), LW(0))
        IF_INT_EQUAL(LW(1), 0)
            BREAK_LOOP
        END_IF
        WAIT_FRAMES(1)
    END_LOOP
    RETURN
EVT_END()

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("dou_00", dou_00_init_evt);
    relSetEvtAddr("dou_01", dou_01_init_evt);
    relSetEvtAddr("dou_02", dou_02_init_evt);
    relSetEvtAddr("dou_03", dou_03_init_evt);
    relSetEvtAddr("dou_04", dou_04_init_evt);
    relSetEvtAddr("dou_05", dou_05_init_evt);
    relSetEvtAddr("dou_06", dou_06_init_evt);
    relSetEvtAddr("dou_07", dou_07_init_evt);
    relSetEvtAddr("dou_08", dou_08_init_evt);
    relSetEvtAddr("dou_09", dou_09_init_evt);
    relSetEvtAddr("dou_10", dou_10_init_evt);
    relSetEvtAddr("dou_11", dou_11_init_evt);
    relSetEvtAddr("dou_12", dou_12_init_evt);
    relSetEvtAddr("dou_13", dou_13_init_evt);
    relSetBtlAddr("dou", btlsetup_dou_tbl, btl_dou_setup_no_tbl);
}

EVT_BEGIN(dou_puka_event)
    SET_VALUE(LW(7), LW(0))
    SET_VALUE(LW(8), LW(1))
    SET_VALUE(LW(2), -10)
    SET_VALUE(LW(3), 200)
    CALL_CHILD(PTR(dou_puka_event_sub))
    SET_VALUE(LW(2), -8)
    SET_VALUE(LW(3), 150)
    CALL_CHILD(PTR(dou_puka_event_sub))
    SET_VALUE(LW(2), -5)
    SET_VALUE(LW(3), 100)
    CALL_CHILD(PTR(dou_puka_event_sub))
    SET_VALUE(LW(2), -2)
    SET_VALUE(LW(3), 50)
    CALL_CHILD(PTR(dou_puka_event_sub))
    RETURN
EVT_END()

EVT_BEGIN(dou_mario_pos_osreport)
    CALL(dou_mario_pos_osreport_func)
    RETURN
EVT_END()

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
void _unresolved(void) {
}
USER_FUNC(evt_dou_normal_puka) {
    evtSetFloat(event, event->args[0], dou_normal_puka_data[event->lwData[0] & 0xF]);
    return EVT_RETURN_DONE;
}
USER_FUNC(dou_mario_pos_osreport_func) {
    MarioWork* mario = marioGetPtr();

    keyGetButtonTrg(0);
    return EVT_RETURN_BLOCK;
}
