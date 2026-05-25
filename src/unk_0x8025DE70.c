#include "unk_0x8025DE70.h"

void N_setupErrorHandler(void) {
    extern void OSSetErrorHandler(s32 error, void* handler);
    extern void N_systemErrorHandler(void);

    OSSetErrorHandler(2, N_systemErrorHandler);
    OSSetErrorHandler(3, N_systemErrorHandler);
    OSSetErrorHandler(6, N_systemErrorHandler);
}
