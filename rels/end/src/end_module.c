// end.rel, Japanese demo only: the module entry points, in a unit of their own after end_00. The other versions have
// them in end_00.c. The math constants of the precompiled header follow end_00's data in this version.

#include "evt/evt_cmd.h"
#include "data/mapdata.h"
#include "rel_module.h"
#include "end.h"

extern EvtWord end_00_init_evt[];
void end_staff_roll_exit(void);

/**
 * REL prolog of the ending module (Japanese demo):
 * runs the static constructors and registers the init event of the one room, end_00 (the staff roll).
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("end_00", end_00_init_evt);
}

/**
 * REL epilog of the ending module: runs the static destructors and then end_staff_roll_exit,
 * which frees the ending picture file and restores the sound and font settings the staff roll changed.
 */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
    end_staff_roll_exit();
}

/** Target of unresolved relocations; the module has no code to run for them, so it is empty. */
void _unresolved(void) {
}
