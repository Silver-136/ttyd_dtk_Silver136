#pragma once

#include <dolphin/types.h>
#include "evt/evt_cmd.h"

USER_FUNC(evt_eff);
USER_FUNC(evt_eff64);
USER_FUNC(evt_eff_delete);
USER_FUNC(evt_eff_delete_ptr);
USER_FUNC(evt_eff_fukidashi);
USER_FUNC(evt_eff_softdelete_ptr);
s32 evt_eff64(EventEntry *event, BOOL isFirstCall);
