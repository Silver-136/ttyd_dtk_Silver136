#include "event/evt_eff.h"

extern s32 evtGetValue();
extern void* effNameToPtr();
extern void effDelete();
extern void effSoftDelete();

u8 evt_eff64(void) {
    return 0;
}


u8 evt_eff(s32 pEvt) {
    return 0;
}


u8 evt_eff_fukidashi(s32 pEvt) {
    return 0;
}


s32 evt_eff_delete(void* pEvt) {
    effDelete(effNameToPtr(evtGetValue(pEvt, **(s32***)((s32)pEvt + 0x18))));
    return 2;
}


s32 evt_eff_softdelete(void* pEvt) {
    effSoftDelete(effNameToPtr(evtGetValue(pEvt, **(s32***)((s32)pEvt + 0x18))));
    return 2;
}


s32 evt_eff_delete_ptr(void* pEvt) {
    effDelete(evtGetValue(pEvt, **(s32***)((s32)pEvt + 0x18)));
    return 2;
}


s32 evt_eff_softdelete_ptr(void* pEvt) {
    effSoftDelete(evtGetValue(pEvt, **(s32***)((s32)pEvt + 0x18)));
    return 2;
}
