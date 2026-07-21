#include "driver/seqdrv.h"

s32 now_seq;
s32 next_seq;
s32 prev_seq;
s32 next_p0;
s32 next_p1;
u8 seqWork[0x20];

void* memset(void*, int, unsigned long);

void seqInit_MARIOSTORY(void) {
    memset(seqWork, 0, 0x20);
    now_seq = -1;
    next_seq = -1;
    prev_seq = -1;
}


void seqMain(void) {
    extern void sysWaitDrawSync(void);
    extern void (*seq_data_80304400[][3])(void*);
    void (*func)(void*);

    if (now_seq != next_seq) {
        if (now_seq != -1) {
            sysWaitDrawSync();
            func = seq_data_80304400[now_seq][2];
            func(seqWork);
        }

        prev_seq = now_seq;
        memset(seqWork, 0, 0x20);
        now_seq = next_seq;
        *(s32*)seqWork = next_seq;
        *(s32*)((s32)seqWork + 4) = 0;
        *(s32*)((s32)seqWork + 8) = next_p0;
        *(s32*)((s32)seqWork + 0xC) = next_p1;

        func = seq_data_80304400[next_seq][0];
        func(seqWork);
    }

    func = seq_data_80304400[now_seq][1];
    func(seqWork);
}

void seqSetSeq(s32 seq, s32 p0, s32 p1) {
    next_seq = seq;
    next_p0 = p0;
    next_p1 = p1;
}

s32 seqGetSeq(void) {
    return now_seq;
}

s32 seqGetPrevSeq(void) {
    return prev_seq;
}

s32 seqGetNextSeq(void) {
    return next_seq;
}

s32 seqCheckSeq(void) {
    return ((u32)(next_seq - now_seq) | (u32)(now_seq - next_seq)) >> 31;
}
