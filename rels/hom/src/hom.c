#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#pragma force_active on
#include "data/mapdata.h"
#include "hom.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

f32 fd_hom_negone_one_374[2] __attribute__((aligned(8)));
f32 fd_hom_neg_one_382;
f32 fd_hom___local_PI_383;
f32 fd_hom___local_3PIO2_385;
f32 fd_hom_one_negone_373[2] __attribute__((aligned(8)));
f32 fd_hom___local_PIO2_384;
f32 fd_hom_one_381;
f32 fd_hom_negone_one_360[2] __attribute__((aligned(8)));
f32 fd_hom_neg_one_365;
f32 fd_hom___local_PI_366;
f32 fd_hom___local_3PIO2_368;
f32 fd_hom_one_negone_359[2] __attribute__((aligned(8)));
f32 fd_hom___local_PIO2_367;
f32 fd_hom_one_364;
f64 fd_hom__three_localstatic4_sqrtf;
f64 fd_hom__half_localstatic3_sqrtf;

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("hom_00", hom_00_init_evt);
    relSetEvtAddr("hom_10", hom_10_init_evt);
    relSetEvtAddr("hom_11", hom_11_init_evt);
    relSetEvtAddr("hom_12", hom_12_init_evt);
}

f32 fd_hom_negone_one_374[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hom_neg_one_382 = -1.0f;

f32 fd_hom___local_PI_383 = 3.1415927f;

f32 fd_hom___local_3PIO2_385 = 4.712389f;

f32 fd_hom_one_negone_373[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hom___local_PIO2_384 = 1.5707964f;

f32 fd_hom_one_381 = 1.0f;

f32 fd_hom_negone_one_360[2] __attribute__((aligned(8))) = {-1.0f, 1.0f};

f32 fd_hom_neg_one_365 = -1.0f;

f32 fd_hom___local_PI_366 = 3.1415927f;

f32 fd_hom___local_3PIO2_368 = 4.712389f;

f32 fd_hom_one_negone_359[2] __attribute__((aligned(8))) = {1.0f, -1.0f};

f32 fd_hom___local_PIO2_367 = 1.5707964f;

f32 fd_hom_one_364 = 1.0f;

f64 fd_hom__three_localstatic4_sqrtf = 3.0;

f64 fd_hom__half_localstatic3_sqrtf = 0.5;

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
void _unresolved(void) {
}
