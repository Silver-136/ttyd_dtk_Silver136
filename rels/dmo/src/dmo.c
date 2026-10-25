#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "dmo.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];
void demo_exit(void);

void _unresolved(void);
void _epilog(void);
void _prolog(void);

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("dmo_00", dmo_00_init_evt);
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    demo_exit();
}
void _unresolved(void) {
}
