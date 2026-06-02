#include "event/evt_yuugijou.h"

void* yuwp;
void* unk_8041ed24;
s32 evtSetValue(EventEntry* evt, s32 target, s32 value);
s32 evtGetValue(EventEntry* evt, s32 value);
void evtEntryType(void* script, s32 a, s32 b, s32 type);
extern s32 minigame_end[];
extern s32 window_open[];

void unk_802295a8(void* value) {
    unk_8041ed24 = value;
}

void evtYuuWindow(void) {
    void* script = window_open;

    *(u32*)yuwp &= ~8;
    evtEntryType(script, 0, 0, 0);
}

u32 evtYuuWindowEndChk(void) {
    return (*(u32*)yuwp >> 3) & 1;
}

USER_FUNC(unk_802295b0) {
    s32* args = event->args;
    evtSetValue(event, args[0], (s32)unk_8041ed24);
    return 2;
}

USER_FUNC(yuuminigame_end) {
    evtEntryType(minigame_end, 0, 0, 0xEF);
    return 2;
}

USER_FUNC(yuuminigame_window_end) {
    *(u32*)yuwp |= 8;
    return 2;
}

USER_FUNC(yuugijou_set_yupflag) {
    s32 value = evtGetValue(event, event->args[0]);
    if (value != 0) {
        *(u32*)yuwp |= 4;
    } else {
        *(u32*)yuwp &= ~4;
    }
    return 2;
}

USER_FUNC(yuugijou_get_takeoutflag) {
    s32* args = event->args;
    if (*(u32*)yuwp & 2) {
        evtSetValue(event, args[0], 1);
    } else {
        evtSetValue(event, args[0], 0);
    }
    return 2;
}

USER_FUNC(yuugijou_montemedal_return) {
    if (!(*(u32*)yuwp & 2)) {
        s32 value = *(s32*)((s32)yuwp + 4);
        *(s32*)((s32)yuwp + 8) = value;
        *(s32*)((s32)yuwp + 0x10) = value;
        *(u32*)yuwp |= 2;
        *(u32*)yuwp |= 1;
    }
    return 2;
}

USER_FUNC(yuugijou_montemedal_keep) {
    if (*(u32*)yuwp & 2) {
        *(s32*)((s32)yuwp + 4) = *(s32*)((s32)yuwp + 8);
        *(s32*)((s32)yuwp + 8) = 0;
        *(s32*)((s32)yuwp + 0x10) = 0;
        *(u32*)yuwp &= ~2;
        *(u32*)yuwp &= ~1;
    }
    return 2;
}

USER_FUNC(yuugijou_montemedal_wait) {
    if (*(s32*)((s32)yuwp + 0x10) != *(s32*)((s32)yuwp + 8)) {
        return 0;
    }
    return 2;
}

USER_FUNC(yuugijou_montemedal_disp_onoff) {
    s32 value = evtGetValue(event, event->args[0]);
    if (value != 0) {
        *(u32*)yuwp |= 1;
        *(s32*)((s32)yuwp + 0x10) = *(s32*)((s32)yuwp + 8);
    } else {
        *(u32*)yuwp &= ~1;
    }
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
USER_FUNC(yuugijou_add_montemedal_moto) {
    s32* args = event->args;
    evtGetValue(event, args[0]);
    if (*(u32*)yuwp & 2) {
        *(s32*)((s32)yuwp + 4) += evtGetValue(event, args[1]);
    }
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

USER_FUNC(yuugijou_get_montemedal_keep) {
    s32* args = event->args;
    if (*(u32*)yuwp & 2) {
        evtSetValue(event, args[0], 0);
    } else {
        evtSetValue(event, args[0], *(s32*)((s32)yuwp + 4));
    }
    return 2;
}

USER_FUNC(yuugijou_get_montemedal) {
    evtSetValue(event, event->args[0], *(s32*)((s32)yuwp + 8));
    return 2;
}
