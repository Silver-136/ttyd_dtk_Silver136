#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

// gon_bt00: registered but never used; the init event only returns.
EvtWord gon_bt00_init_evt[2];

EVT_BEGIN(gon_bt00_init_evt)
    RETURN
EVT_END()

