// eki.rel (駅, Riverside Station): REL prolog/epilog; registers the init events of eki_00 to eki_06 and the area battle tables.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "eki.h"

typedef void (*voidfunctionptr)(void);
extern voidfunctionptr _ctors[];
extern voidfunctionptr _dtors[];

void _unresolved(void);
void _epilog(void);
void _prolog(void);

/**
 * REL prolog for the Riverside Station module: runs the static constructors,
 * then registers the init event of each of the seven rooms (eki_00 to eki_06)
 * and the area's battle setup tables.
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("eki_00", eki_00_init_evt);
    relSetEvtAddr("eki_01", eki_01_init_evt);
    relSetEvtAddr("eki_02", eki_02_init_evt);
    relSetEvtAddr("eki_03", eki_03_init_evt);
    relSetEvtAddr("eki_04", eki_04_init_evt);
    relSetEvtAddr("eki_05", eki_05_init_evt);
    relSetEvtAddr("eki_06", eki_06_init_evt);
    relSetBtlAddr("eki", btlsetup_eki_tbl, btl_eki_setup_no_tbl);
}

/** REL epilog: runs the static destructors when the Riverside Station module is unloaded. */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
/** Target of unresolved relocations; the module has no code to run for them, so it is empty. */
void _unresolved(void) {
}
