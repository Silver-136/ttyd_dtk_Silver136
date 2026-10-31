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

/**
 * REL entry point: runs the static constructors,
 * then registers the init event scripts of the four Pianta Parlor maps (yuu_00 to yuu_03) with relSetEvtAddr
 * so the map loader can start them.
 */
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

/** REL exit point: runs the static destructors when the yuu module is unloaded. */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}
/** Linker stub for unresolved imports; does nothing. */
void _unresolved(void) {
}
