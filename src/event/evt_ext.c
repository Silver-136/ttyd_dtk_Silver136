#include "event/evt_ext.h"

s32 evt_ext_reset(void) {
    extern void extReset(void);

    extReset();
    return 2;
}


s32 evt_ext_entry(void* pEvt) {
    return 0;
}
