#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "gon.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _prolog(void);
void _epilog(void);
void _unresolved(void);

f32 fd_gon_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_gon_neg_one_382;
f32 fd_gon___local_PI_383;
f32 fd_gon___local_3PIO2_385;
f32 fd_gon_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_gon___local_PIO2_384;
f32 fd_gon_one_381;
f32 fd_gon_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_gon_neg_one_365;
f32 fd_gon___local_PI_366;
f32 fd_gon___local_3PIO2_368;
f32 fd_gon_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_gon___local_PIO2_367;
f32 fd_gon_one_364;
f64 fd_gon__three_localstatic4_sqrtf;
f64 fd_gon__half_localstatic3_sqrtf;

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
    relSetEvtAddr("gon_00", gon_00_init_evt);
    relSetEvtAddr("gon_01", gon_01_init_evt);
    relSetEvtAddr("gon_02", gon_02_init_evt);
    relSetEvtAddr("gon_03", gon_03_init_evt);
    relSetEvtAddr("gon_04", gon_04_init_evt);
    relSetEvtAddr("gon_05", gon_05_init_evt);
    relSetEvtAddr("gon_06", gon_06_init_evt);
    relSetEvtAddr("gon_07", gon_07_init_evt);
    relSetEvtAddr("gon_08", gon_08_init_evt);
    relSetEvtAddr("gon_09", gon_09_init_evt);
    relSetEvtAddr("gon_10", gon_10_init_evt);
    relSetEvtAddr("gon_11", gon_11_init_evt);
    relSetEvtAddr("gon_12", gon_12_init_evt);
    relSetEvtAddr("gon_13", gon_13_init_evt);
    relSetEvtAddr("gon_bt00", gon_bt00_init_evt);
    relSetBtlAddr("gon", btlsetup_gon_tbl, btl_gon_setup_no_tbl);
}

f32 fd_gon_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gon_neg_one_382 = -1.0f;

f32 fd_gon___local_PI_383 = 3.1415927f;

f32 fd_gon___local_3PIO2_385 = 4.712389f;

f32 fd_gon_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gon___local_PIO2_384 = 1.5707964f;

f32 fd_gon_one_381 = 1.0f;

f32 fd_gon_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_gon_neg_one_365 = -1.0f;

f32 fd_gon___local_PI_366 = 3.1415927f;

f32 fd_gon___local_3PIO2_368 = 4.712389f;

f32 fd_gon_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_gon___local_PIO2_367 = 1.5707964f;

f32 fd_gon_one_364 = 1.0f;

f64 fd_gon__three_localstatic4_sqrtf = 3.0;

f64 fd_gon__half_localstatic3_sqrtf = 0.5;

