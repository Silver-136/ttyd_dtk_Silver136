#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "moo.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/**
 * Module prolog of the moon: runs the static constructors and registers the init scripts of moo_00 to moo_07
 * and the battle setup table for "moo".
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("moo_00", moo_00_init_evt);
    relSetEvtAddr("moo_01", moo_01_init_evt);
    relSetEvtAddr("moo_02", moo_02_init_evt);
    relSetEvtAddr("moo_03", moo_03_init_evt);
    relSetEvtAddr("moo_04", moo_04_init_evt);
    relSetEvtAddr("moo_05", moo_05_init_evt);
    relSetEvtAddr("moo_06", moo_06_init_evt);
    relSetEvtAddr("moo_07", moo_07_init_evt);
    relSetBtlAddr("moo", btlsetup_moo_tbl, btl_moo_setup_no_tbl);
}

/** Module epilog of the moon: runs the static destructors. */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
/** Unresolved-symbol stub of the moon module; does nothing. */
void _unresolved(void) {
}
