// hei_bt00 (平原, Petal Meadows): battle-stage map file; its init script does nothing.

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

// Registered battle-test room; its init event only returns.
EvtWord hei_bt00_init_evt[2];

/** Map init for the battle-stage map; empty. */
EVT_BEGIN(hei_bt00_init_evt)
    RETURN
EVT_END()

