#pragma once

#include <dolphin/types.h>

struct EventEntry;

void winMgrInit(void);
void winMgrReInit(void);
s32 winMgrEntry(void* tpl);
void winMgrSetParam(s32 id, s32 param);
void winMgrOpen(s32 id);
void winMgrCloseAutoDelete(s32 id);
s32 winMgrAction(s32 id);
s32* winMgrSelectEntry(u32 selectType, s32 newItem, s32 isCancellable);
s32 winMgrSelectOther(void* select, struct EventEntry* event);
s32 winMgrSelect(void* select);
s32 unk_8023cf04(void* select);
void winMgrSelectDelete(void* select);
