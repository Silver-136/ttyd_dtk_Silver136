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

// FAKE: stand-in for the weak constant pair of MSL sqrtf (see docs/FAKES.md)
#pragma push
#pragma dollar_identifiers on
static f64 _three$localstatic4$sqrtf = 3.0;

static f64 _half$localstatic3$sqrtf = 0.5;
#pragma pop

