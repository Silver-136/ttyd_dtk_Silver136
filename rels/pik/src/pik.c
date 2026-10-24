#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "pik.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

f32 fd_pik_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_pik_neg_one_382;
f32 fd_pik___local_PI_383;
f32 fd_pik___local_3PIO2_385;
f32 fd_pik_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_pik___local_PIO2_384;
f32 fd_pik_one_381;
f32 fd_pik_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_pik_neg_one_365;
f32 fd_pik___local_PI_366;
f32 fd_pik___local_3PIO2_368;
f32 fd_pik_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_pik___local_PIO2_367;
f32 fd_pik_one_364;
f64 fd_pik__three_localstatic4_sqrtf;
f64 fd_pik__half_localstatic3_sqrtf;

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("pik_00", pik_00_init_evt);
    relSetEvtAddr("pik_01", pik_01_init_evt);
    relSetEvtAddr("pik_02", pik_02_init_evt);
    relSetEvtAddr("pik_03", pik_03_init_evt);
    relSetEvtAddr("pik_04", pik_04_init_evt);
    relSetBtlAddr("pik", btlsetup_pik_tbl, btl_pik_setup_no_tbl);
}

f32 fd_pik_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_pik_neg_one_382 = -1.0f;

f32 fd_pik___local_PI_383 = 3.1415927f;

f32 fd_pik___local_3PIO2_385 = 4.712389f;

f32 fd_pik_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_pik___local_PIO2_384 = 1.5707964f;

f32 fd_pik_one_381 = 1.0f;

f32 fd_pik_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_pik_neg_one_365 = -1.0f;

f32 fd_pik___local_PI_366 = 3.1415927f;

f32 fd_pik___local_3PIO2_368 = 4.712389f;

f32 fd_pik_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_pik___local_PIO2_367 = 1.5707964f;

f32 fd_pik_one_364 = 1.0f;

f64 fd_pik__three_localstatic4_sqrtf = 3.0;

f64 fd_pik__half_localstatic3_sqrtf = 0.5;

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
void _unresolved(void) {
}
