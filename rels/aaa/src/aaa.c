#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "aaa.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/**
 * Module entry point: runs the static constructors
 * and registers aaa_00_init_evt for the map "aaa_00" (Mario's house).
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("aaa_00", aaa_00_init_evt);
}

/** Module exit point: frees the prologue's Mario's-house texture via mapdelete, then runs the static destructors. */
void _epilog(void) {
    voidfunctionptr* dtor;

    mapdelete();
    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
/** Stub the linker needs for unresolved symbols; does nothing. */
void _unresolved(void) {
}
