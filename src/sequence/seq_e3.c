#include "sequence/seq_e3.h"

void alarm_handler(void) {
    extern void* gp;

    *(u32*)gp |= 0x2000;
}

void seq_e3Exit(void) {
    extern void* wp;
    extern void fileFree(void* ptr);
    extern void psndStopAllFadeOut(void);

    fileFree(*(void**)wp);
    psndStopAllFadeOut();
}
