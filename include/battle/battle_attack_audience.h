// Declares the audience attack events and lookup functions.

#pragma once

#include "evt/evt_cmd.h"

USER_FUNC(_attack_aud);
USER_FUNC(_get_attack_aud_no);
USER_FUNC(_check_aud_item_type);

EvtWord* btlataudGetAttackEventPtr(s32 unitId);
void* btlataudGetMoveEventPtr(s32 id);
