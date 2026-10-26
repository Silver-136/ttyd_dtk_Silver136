#pragma once

#include <dolphin/types.h>
#include "evt/evt_cmd.h"

// evt_eff_fukidashi first argument: where the speech-bubble effect sits
typedef enum FukidashiTarget {
    FUKIDASHI_AT_MARIO = 0,
    FUKIDASHI_AT_NPC = 1,      // the NPC named in the third argument ("me" = the event's owner)
    FUKIDASHI_AT_POS = 2,      // at the given x, y, z
    FUKIDASHI_AT_PARTNER = 3,
    FUKIDASHI_AT_FOLLOWER = 4, // the extra party member (marioGetExtraPartyId)
} FukidashiTarget;

USER_FUNC(evt_eff);
USER_FUNC(evt_eff64);
USER_FUNC(evt_eff_delete);
USER_FUNC(evt_eff_delete_ptr);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_eff_softdelete_ptr);
s32 evt_eff64(EventEntry *event, BOOL isFirstCall);
USER_FUNC(evt_eff_softdelete);
