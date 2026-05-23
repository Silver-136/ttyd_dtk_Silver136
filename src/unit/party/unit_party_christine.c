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
