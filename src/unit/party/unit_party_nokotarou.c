#include "unit/party/unit_party_nokotarou.h"

s32 _check_mario_move_count(void* evt) {
    extern void* _battleWorkPointer;
    extern void* BattleGetMarioPtr(void* battleWork);
    extern void evtSetValue(void* evt, s32 arg, s32 value);

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 out = args[0];
    void* mario = BattleGetMarioPtr(battleWork);
    evtSetValue(evt, out, *(s8*)((s32)mario + 0x22));
    return 2;
}
