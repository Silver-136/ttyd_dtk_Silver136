// evt_case.c: script commands that register, run and remove case events (casedrv).

#pragma once

#include "evt/evt_cmd.h"
#include <dolphin/types.h>

USER_FUNC(evt_run_case_evt);
USER_FUNC(evt_run_case_evt_bero);
s32 evtRunCaseEntry(s32 conditionId, BOOL flag, const char* hitObjName, u32 swFlag, void* evtCode,
                    EvtWord* lwData);
USER_FUNC(evt_exit_case_evt);
USER_FUNC(evt_del_case_evt);
USER_FUNC(evt_set_case_wrk);
