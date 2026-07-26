#include "event/evt_seq.h"
#include "driver/seqdrv.h"

extern s32 evtGetValue(void* evt, s32 arg);

s32 evt_seq_set_seq(void* evt) {
    extern void seqSetSeq(s32 seq, s32 arg1, s32 arg2);

    s32* args = *(s32**)((s32)evt + 0x18);
    s32 seq = evtGetValue(evt, args[0]);
    s32 arg1 = evtGetValue(evt, args[1]);
    s32 arg2 = evtGetValue(evt, args[2]);

    seqSetSeq(seq, arg1, arg2);
    return 0;
}


u32 evt_seq_wait(void* pEvt) {
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 seq = evtGetValue(pEvt, args[0]);
    s32 cur = seqGetSeq();
    s32 diff = (cur - seq) | (seq - cur);

    return 2 & ~(diff >> 31);
}

