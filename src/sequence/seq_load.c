#include "sequence/seq_load.h"

extern s32* wp;
extern void* memset(void*, int, unsigned long);
extern void fileFree(void*);
extern void fadeReset(s32);


u8 loadDraw(void) {
    return 0;
}


u8 continueGame(void) {
    return 0;
}


u8 seq_loadExit(void) {
    void* file = (void*)wp[1];

    if (file != NULL) {
        fileFree(file);
    }
    wp[1] = 0;
}


u8 seq_loadInit(void) {
    memset(wp, 0, 0x4C);
}


s32 unk_800f72e4(void* param_1) {
    fadeReset(4);
    wp[0x12] = *(s32*)((s32)param_1 + 0x178);
    return 2;
}


u8 loadMain(void* param_1) {
    return 0;
}


u8 seq_loadMain(void* param_1) {
    return 0;
}
