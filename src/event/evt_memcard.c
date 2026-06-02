#include "event/evt_memcard.h"
#include "event/evt_cmd.h"

extern void* gp;

s32 evtGetValue(EventEntry* event, s32 value);
void unk_800b2c08(void);
void unk_800b2c2c(void);
void cardBufReset(void);
void cardWriteHeader(void);
void cardCreate(void);
void cardFormat(void);
void cardErase(s32 fileNo);
void cardWrite(void* saveData);
void cardReadAll(void);
s32 cardIsExec(void);
s32 unk_800b2bdc(void);
s32 cardGetCode(void);

s32 memcard_ipl(void) {
    *(s32*)((s32)gp + 0x1278) = 2;
    return 0;
}

USER_FUNC(unk_8025c380) {
    unk_800b2c08();
    return 2;
}

USER_FUNC(unk_8025c40c) {
    unk_800b2c2c();
    return 2;
}

USER_FUNC(unk_8025c430) {
    cardBufReset();
    return 2;
}

s32 memcard_header_write(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardWriteHeader();
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

s32 memcard_create(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardCreate();
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

s32 memcard_format(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardFormat();
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

s32 memcard_delete(EventEntry* event, BOOL firstRun) {
    s32 fileNo = evtGetValue(event, event->args[0]) - 1;
    if (firstRun != 0) {
        cardErase(fileNo);
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

s32 memcard_write(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardWrite(*(void**)((s32)gp + 0x11D0));
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

s32 memcard_load(EventEntry* event, BOOL firstRun) {
    if (firstRun != 0) {
        cardBufReset();
        cardReadAll();
    }
    if (cardIsExec() != 0) {
        return 0;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(unk_8025c3a4) {
    s32* args;

    args = event->args;
    if (unk_800b2bdc() != 0) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(memcard_code) {
    s32* args;
    s32 code;

    args = event->args;
    code = cardGetCode();
    if (cardIsExec() != 0) {
        return 0;
    }
    evtSetValue(event, args[0], code);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
