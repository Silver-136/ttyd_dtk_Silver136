#include "unit/party/unit_party_christine.h"

s32 _monosiri_flag_on(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void battleSetUnitMonosiriFlag(void* unit);

    s32 id = evtGetValue(evt, **(s32**)((s32)evt + 0x18));
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);
    battleSetUnitMonosiriFlag(*(void**)((s32)unit + 8));
    return 2;
}


void __makeTechMenuFunc(void* commandWork, s32 param_2) {
}


s32 _dictionary(int param_1) {
    return 0;
}


u8 krb_get_dir(void) {
    return 0;
}


s32 _set_hustle(int param_1) {
    return 0;
}


s32 btlevtcmd_get_monosiri_msg_no(int param_1) {
    return 0;
}
