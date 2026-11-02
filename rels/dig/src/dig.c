// dig.rel (ダイジェスト, digest movie): registers dig_00 and starts the THP movie player on load, stops it on unload.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"
#include "rels/dig/include/dig.h"

#include "data/mapdata.h"
#include "dig.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/**
 * REL module prolog, run when dig.rel is loaded:
 * runs the static constructors, registers the dig_00 init script and opens the digest movie (dig_thpInit).
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("dig_00", dig_00_init_evt);
    dig_thpInit();
}

/**
 * REL module epilog, run when dig.rel is unloaded:
 * runs the static destructors and stops and closes the digest movie (dig_thpStop).
 */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    dig_thpStop();
}

/**
 * Module unresolved-symbol stub: if a relocation could not be resolved the REL linker points it here;
 * it does nothing.
 */
void _unresolved(void) {
}
