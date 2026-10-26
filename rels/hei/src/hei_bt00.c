#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

// hei_bt00: registered but never used; the init event only returns.
EvtWord hei_bt00_init_evt[2];

EVT_BEGIN(hei_bt00_init_evt)
    RETURN
EVT_END()

