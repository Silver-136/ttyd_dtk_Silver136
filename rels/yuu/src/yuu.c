#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "yuu.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("yuu_00", yuu_00_init_evt);
    relSetEvtAddr("yuu_01", yuu_01_init_evt);
    relSetEvtAddr("yuu_02", yuu_02_init_evt);
    relSetEvtAddr("yuu_03", yuu_03_init_evt);
}

void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
void _unresolved(void) {
}
