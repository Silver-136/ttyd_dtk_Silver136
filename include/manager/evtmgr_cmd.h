// evtmgr_cmd.c: the script interpreter's opcode handler (evtmgrCmd) and the value accessors evtGetValue/evtSetValue.

#pragma once

#include <dolphin/types.h>
#include "evt/evt_word.h"
#include "manager/evtmgr.h"

s32 evtmgrCmd(EventEntry* entry);
EvtWord evtGetValue(EventEntry* entry, EvtWord index);
EvtWord evtGetNumber(EventEntry* entry, EvtWord index);
EvtWord evtSetValue(EventEntry* entry, EvtWord index, EvtWord value);
f32 evtGetFloat(EventEntry* entry, EvtWord index);
f32 evtSetFloat(EventEntry* entry, EvtWord index, f32 value);
