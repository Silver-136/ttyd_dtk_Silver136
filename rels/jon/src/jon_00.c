// jon_00 (１００階ダンジョン, Pit of 100 Trials): the floor map for floors 1-49 except the rest floors 10, 20, 30 and
// 40 (jon.c picks the map by floor; this file only has the empty init script).

#include "battle/battle_unit.h"
#include "evt/evt_cmd.h"

EvtWord jon_00_init_evt[2];

/** Empty placeholder: jon.c registers jon_init_evt for every jon map, so this script is not used. */
EVT_BEGIN(jon_00_init_evt)
    RETURN
EVT_END()
