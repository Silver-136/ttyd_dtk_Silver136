// tst.rel (PAL and the Japanese store demo): the developers' test maps (tst_*), registered with their init events from evt_init.c, and
// the test battle setups of battle_bgset_test.c with copies of many enemy units (unit/).
#include "evt/evt_cmd.h"

#include "data/mapdata.h"
#include "evt_init.h"
#include "tst.h"
#include "rel_module.h"

/**
 * Script helper: calls mobjKoopaOn(), which switches the map objects to the separate 128-entry table of Bowser's
 * side-scrolling stages; the map-object test maps (tst_moj_00 and tst_moj_01) call it before placing their
 * objects.
 */
USER_FUNC(tst_mobj_koopa_on) {
    mobjKoopaOn();
    return EVT_RETURN_DONE;
}

/** REL stub for unresolved imports: does nothing. */
void _unresolved(void) {
}

/** REL unload entry: runs the global destructors listed in _dtors. */
void _epilog(void) {
    voidfunctionptr* dtor;

    for (dtor = _dtors; *dtor != NULL; dtor++) {
        (*dtor)();
    }
}

/**
 * REL load entry: runs the global constructors listed in _ctors,
 * then registers each tst map's init event with relSetEvtAddr so the map loader finds it.
 */
void _prolog(void) {
    voidfunctionptr* ctor;

    for (ctor = _ctors; *ctor != NULL; ctor++) {
        (*ctor)();
    }
    relSetEvtAddr("tst_bt00", tst_bt00_init_evt);
    relSetEvtAddr("tst_00", tst_00_init_evt);
    relSetEvtAddr("tst_01", tst_01_init_evt);
    relSetEvtAddr("tst_02", tst_02_init_evt);
    relSetEvtAddr("tst_moj_00", tst_moj_00_init_evt);
    relSetEvtAddr("tst_moj_01", tst_moj_01_init_evt);
    relSetEvtAddr("tst_moj_02", tst_moj_02_init_evt);
    relSetEvtAddr("tst_moj_03", tst_moj_03_init_evt);
    relSetEvtAddr("tst_off_01", tst_off_01_init_evt);
    relSetEvtAddr("tst_npc", tst_npc_init_evt);
    // tst_nakayama in the Japanese store demo (G8MJ01_DEMO): a reference to an event that does not exist.
    //
    // What the original did: this call is the same in every version that has tst.rel, but the Japanese demo's
    // evt_init.c has no tst_nakayama_init_evt (the event is under VERSION >= VERSION_JP there; PAL has it). The
    // original linker did not stop on the undefined symbol: it left the address load below (a lis/addi pair) with
    // both 16-bit fields at 0 and wrote no relocation for them, so in the shipped tst.rel this registers
    // "tst_nakayama" with a NULL init event.
    //
    // Why the C is written this way: writing NULL here for that version would compile to a single `li r4, 0`, not
    // the lis/addi pair the original has, so the source keeps the original reference. evt_init.c must not define
    // the event for that version either, since a definition would add the relocation the original REL lacks.
    //
    // How the build reproduces it: mwld also leaves the field at 0, but `dtk rel make` (which turns the linked
    // tst.plf into tst.rel) rejects undefined symbols, and defining the symbol (absolute at 0, in the linker script
    // or weak) either adds a relocation or makes rel make fail. So for this one version and REL the build runs
    // tools/drop_relocations.py between the link and rel make; it removes the relocations against the symbols
    // listed in config/G8MJ01_DEMO/unresolved_symbols.txt and leaves the zeros the linker wrote. configure.py
    // enables it through config.rel_dropped_relocations; docs/BUILDING.md describes the step.
    relSetEvtAddr("tst_nakayama", tst_nakayama_init_evt);
    relSetEvtAddr("tst_item_00", tst_item_00_init_evt);
    relSetEvtAddr("btl_tst_00", btl_tst_00_init_evt);
    relSetBtlAddr("tst", btlsetup_tst_tbl, btl_tst_setup_no_tbl);
}
