// pik.rel (ピカリーヒルズ, Poshley Heights): registers pik_00..pik_04 and the battle database.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "pik.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/**
 * Module entry point: runs the static constructors,
 * then registers the init scripts of pik_00 to pik_04
 * and the battle setup tables for Poshley Heights with the REL loader.
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("pik_00", pik_00_init_evt);
    relSetEvtAddr("pik_01", pik_01_init_evt);
    relSetEvtAddr("pik_02", pik_02_init_evt);
    relSetEvtAddr("pik_03", pik_03_init_evt);
    relSetEvtAddr("pik_04", pik_04_init_evt);
    relSetBtlAddr("pik", btlsetup_pik_tbl, btl_pik_setup_no_tbl);
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
