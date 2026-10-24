#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "moo.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

f32 fd_moo_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_moo_neg_one_382;
f32 fd_moo___local_PI_383;
f32 fd_moo___local_3PIO2_385;
f32 fd_moo_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_moo___local_PIO2_384;
f32 fd_moo_one_381;
f32 fd_moo_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_moo_neg_one_365;
f32 fd_moo___local_PI_366;
f32 fd_moo___local_3PIO2_368;
f32 fd_moo_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_moo___local_PIO2_367;
f32 fd_moo_one_364;

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("moo_00", moo_00_init_evt);
    relSetEvtAddr("moo_01", moo_01_init_evt);
    relSetEvtAddr("moo_02", moo_02_init_evt);
    relSetEvtAddr("moo_03", moo_03_init_evt);
    relSetEvtAddr("moo_04", moo_04_init_evt);
    relSetEvtAddr("moo_05", moo_05_init_evt);
    relSetEvtAddr("moo_06", moo_06_init_evt);
    relSetEvtAddr("moo_07", moo_07_init_evt);
    relSetBtlAddr("moo", btlsetup_moo_tbl, btl_moo_setup_no_tbl);
}

f32 fd_moo_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_moo_neg_one_382 = -1.0f;

f32 fd_moo___local_PI_383 = 3.1415927f;

f32 fd_moo___local_3PIO2_385 = 4.712389f;

f32 fd_moo_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_moo___local_PIO2_384 = 1.5707964f;

f32 fd_moo_one_381 = 1.0f;

f32 fd_moo_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_moo_neg_one_365 = -1.0f;

f32 fd_moo___local_PI_366 = 3.1415927f;

f32 fd_moo___local_3PIO2_368 = 4.712389f;

f32 fd_moo_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_moo___local_PIO2_367 = 1.5707964f;

f32 fd_moo_one_364 = 1.0f;

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
void _unresolved(void) {
}
