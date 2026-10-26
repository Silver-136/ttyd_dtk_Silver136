#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

// qiz_00 (JP release only; the US release drops this REL): a leftover room with nothing in it. The JP demo
// already has it empty, and its purpose is unknown.
EvtWord qiz_00_init_evt[2];

EVT_BEGIN(qiz_00_init_evt)
    RETURN
EVT_END()

