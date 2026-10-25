#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

EvtWord qiz_00_init_evt[2];

// FAKE: stand-in for the weak constant pair of MSL sqrtf (see docs/FAKES.md)
#pragma push
#pragma dollar_identifiers on
static f64 _three$localstatic4$sqrtf = 3.0;

static f64 _half$localstatic3$sqrtf = 0.5;
#pragma pop

EVT_BEGIN(qiz_00_init_evt)
    RETURN
EVT_END()

