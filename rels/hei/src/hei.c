#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "hei.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

f32 fd_hei_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_hei_neg_one_382;
f32 fd_hei___local_PI_383;
f32 fd_hei___local_3PIO2_385;
f32 fd_hei_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_hei___local_PIO2_384;
f32 fd_hei_one_381;
f32 fd_hei_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_hei_neg_one_365;
f32 fd_hei___local_PI_366;
f32 fd_hei___local_3PIO2_368;
f32 fd_hei_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_hei___local_PIO2_367;
f32 fd_hei_one_364;
f64 fd_hei__three_localstatic4_sqrtf;
f64 fd_hei__half_localstatic3_sqrtf;

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("hei_00", hei_00_init_evt);
    relSetEvtAddr("hei_01", hei_01_init_evt);
    relSetEvtAddr("hei_02", hei_02_init_evt);
    relSetEvtAddr("hei_03", hei_03_init_evt);
    relSetEvtAddr("hei_04", hei_04_init_evt);
    relSetEvtAddr("hei_05", hei_05_init_evt);
    relSetEvtAddr("hei_06", hei_06_init_evt);
    relSetEvtAddr("hei_07", hei_07_init_evt);
    relSetEvtAddr("hei_08", hei_08_init_evt);
    relSetEvtAddr("hei_09", hei_09_init_evt);
    relSetEvtAddr("hei_10", hei_10_init_evt);
    relSetEvtAddr("hei_11", hei_11_init_evt);
    relSetEvtAddr("hei_12", hei_12_init_evt);
    relSetEvtAddr("hei_13", hei_13_init_evt);
    relSetEvtAddr("hei_bt00", hei_bt00_init_evt);
    relSetBtlAddr("hei", btlsetup_hei_tbl, btl_hei_setup_no_tbl);
}

f32 fd_hei_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hei_neg_one_382 = -1.0f;

f32 fd_hei___local_PI_383 = 3.1415927f;

f32 fd_hei___local_3PIO2_385 = 4.712389f;

f32 fd_hei_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hei___local_PIO2_384 = 1.5707964f;

f32 fd_hei_one_381 = 1.0f;

f32 fd_hei_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hei_neg_one_365 = -1.0f;

f32 fd_hei___local_PI_366 = 3.1415927f;

f32 fd_hei___local_3PIO2_368 = 4.712389f;

f32 fd_hei_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hei___local_PIO2_367 = 1.5707964f;

f32 fd_hei_one_364 = 1.0f;

f64 fd_hei__three_localstatic4_sqrtf = 3.0;

f64 fd_hei__half_localstatic3_sqrtf = 0.5;

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
void _unresolved(void) {
}
