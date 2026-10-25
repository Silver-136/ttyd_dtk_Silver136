#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "aaa.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
void mapdelete(void);

void _unresolved(void);
void _epilog(void);
void _prolog(void);

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("aaa_00", aaa_00_init_evt);
}

// FAKE: stand-in for the weak constant pair of MSL sqrtf (see docs/FAKES.md)
#pragma push
#pragma dollar_identifiers on
static f64 _three$localstatic4$sqrtf = 3.0;

static f64 _half$localstatic3$sqrtf = 0.5;
#pragma pop

void _epilog(void) {
    voidfunctionptr* dtor;

    mapdelete();
    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
void _unresolved(void) {
}
