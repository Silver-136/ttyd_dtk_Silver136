#include "effect/n64/eff_pturn_n64.h"

void effPturnDisp(void* camera, void* effect) {
    extern void pturn_main_dispp(void* camera, void* effect);
    extern void pturn_num_dispp(void* camera, void* effect);

    void* work = *(void**)((s32)effect + 0xC);

    if (*(s32*)((s32)work + 4) != 0) {
        pturn_main_dispp(camera, effect);
        if (*(s32*)work != 1) {
            pturn_num_dispp(camera, effect);
        }
    }
}


void pturn_num_dispp(void* camera, void* effect) {
    ;
}


void pturn_main_dispp(void* camera, void* effect) {
    ;
}


u8 effPturnMain(void) {
    return 0;
}


u8 effPturnN64Entry(void) {
    return 0;
}
