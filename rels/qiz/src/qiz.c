#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "qiz.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/**
 * Module entry point: runs the static constructors,
 * then registers the init script of qiz_00 with the REL loader (there are no battle tables).
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("qiz_00", qiz_00_init_evt);
}

/** Module exit point: runs the static destructors. */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
/** Placeholder the linker points unresolved relocations at; it does nothing. */
void _unresolved(void) {
}
