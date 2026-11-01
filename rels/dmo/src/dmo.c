// dmo.rel (オープニングデモ, opening demo): registers dmo_00; unloading frees the demo's texture file.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "dmo.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/** REL module prolog, run when dmo.rel is loaded: runs the static constructors and registers the dmo_00 init script. */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("dmo_00", dmo_00_init_evt);
}

/**
 * REL module epilog, run when dmo.rel is unloaded:
 * runs the static destructors and releases the demo's resources (demo_exit).
 */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    demo_exit();
}
/**
 * Module unresolved-symbol stub: if a relocation could not be resolved the REL linker points it here;
 * it does nothing.
 */
void _unresolved(void) {
}
