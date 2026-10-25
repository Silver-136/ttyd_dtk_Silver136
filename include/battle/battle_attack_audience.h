#pragma once

#include "evt/evt_cmd.h"

USER_FUNC(_attack_aud);
USER_FUNC(_get_attack_aud_no);

extern EvtWord btlataudevtPresentItem_Damage[];
extern EvtWord btlataudevtPresentItem_Get[];
EvtWord* btlataudGetAttackEventPtr(s32 unitId);
void* btlataudGetMoveEventPtr(s32 id);
