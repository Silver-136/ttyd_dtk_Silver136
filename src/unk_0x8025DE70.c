#include "unk_0x8025DE70.h"

void N_setupErrorHandler(void) {
    extern void OSSetErrorHandler(s32 error, void* handler);
    extern void N_systemErrorHandler(void);

    OSSetErrorHandler(2, N_systemErrorHandler);
    OSSetErrorHandler(3, N_systemErrorHandler);
    OSSetErrorHandler(6, N_systemErrorHandler);
}


/* CHATGPT STUB FILL: main/unk_0x8025DE70 20260624_185035 */

/* stub-fill: N_systemErrorHandler | prototype_only | source_prototype */
void N_systemErrorHandler(void) {
    return;
}
