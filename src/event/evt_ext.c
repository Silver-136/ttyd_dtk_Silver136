#include "event/evt_ext.h"


s32 evt_ext_entry(void* pEvt) {
    extern s32 evtGetValue(void* evt, s32 value);
    extern void extEntry(s32 a, s32 b, s32 c, s32 d, s32 e);
    s32* args;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;

    args = *(s32**)((s32)pEvt + 0x18);
    a = evtGetValue(pEvt, args[0]);
    b = evtGetValue(pEvt, args[1]);
    c = evtGetValue(pEvt, args[2]);
    d = evtGetValue(pEvt, args[3]);
    e = evtGetValue(pEvt, args[4]);
    extEntry(a, b, c, d, e);
    return 2;
}

s32 evt_ext_reset(void) {
    extern void extReset(void);

    extReset();
    return 2;
}

