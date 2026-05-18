#include "sequence/seq_game.h"
extern s32 gp;
extern u64 OSGetTime(void);
static u64 none_key;
extern s32 dat_8041f498;
extern s32 dat_8041f49c;
extern s32 dat_8041f4a0;
extern s32 maku_spread;
extern void fadeEntry(s32 type, s32 time, void* data);
extern void evtEntryType(void* script, s32 type, s32 a, s32 b);
extern s32 fadeIsFinish(void);
extern s32 seqCheckSeq(void);
extern void seqSetSeq(s32 seq, s32 arg1, s32 arg2);
extern void* marioGetPtr(void);
extern void* camGetPtr(s32 id);
extern void psndSetPosDirListener(void* pos, void* cam, f32 dir);
typedef struct SeqGameVecRaw {
    u32 x;
    u32 y;
    u32 z;
} SeqGameVecRaw;

void seq_gameExit(void) {
    *(u32*)gp &= ~2;
}
void seq_gameMain(void* seq) {
    s32 fadeData0;
    s32 fadeData1;
    s32 fadeData2;
    SeqGameVecRaw pos;
    void* cam;
    u32 flags;

    switch (*(s32*)((s32)seq + 4)) {
        case 0:
            if (*(u32*)gp & 0x10) {
                fadeData0 = dat_8041f498;
                fadeEntry(0x12, 0x384, &fadeData0);
                evtEntryType(&maku_spread, 0, 0, 0);

                *(u32*)gp &= ~0x10;
            } else {
                if (*(s32*)(gp + 0x118) == 0) {
                    fadeData1 = dat_8041f49c;
                    fadeEntry(*(s32*)(gp + 0xF8), *(s32*)(gp + 0xFC), &fadeData1);
                } else {
                    fadeData2 = dat_8041f4a0;
                    fadeEntry(*(s32*)(gp + 0x108), *(s32*)(gp + 0x10C), &fadeData2);
                }
            }

            *(s32*)(gp + 0xF8) = 9;
            *(s32*)(gp + 0xFC) = 0x12C;
            *(s32*)(gp + 0x108) = 9;
            *(s32*)(gp + 0x10C) = 0x12C;

            *(s32*)((s32)seq + 4) += 1;
            break;

        case 1:
            flags = *(u32*)gp;

            if ((flags & 0x1000) && (flags & 0x2000) && fadeIsFinish() && !seqCheckSeq()) {
                *(u32*)gp &= ~0x2000;
                seqSetSeq(5, 0, 0);
            }

            marioGetPtr();

cam = camGetPtr(4);
pos = *(SeqGameVecRaw*)((s32)cam + 0xC);

cam = camGetPtr(4);
psndSetPosDirListener(&pos, cam, *(f32*)((s32)cam + 0x114));
            break;
    }
}
void seq_gameInit(void) {
    none_key = OSGetTime();

    *(u32*)(gp + 0x18) &= ~0x10;
}