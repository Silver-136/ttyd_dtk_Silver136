#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

// Registered by the REL prolog; this map init event immediately returns.
EvtWord gon_bt00_init_evt[2];

EVT_BEGIN(gon_bt00_init_evt)
    RETURN
EVT_END()

