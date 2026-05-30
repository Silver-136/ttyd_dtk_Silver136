#include "unit/party/unit_party_clauda.h"

s32 _check_blow_rate(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void evtSetValue(void* evt, s32 arg, s32 value);

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id = evtGetValue(evt, args[0]);
    s32 out = args[1];
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));
    evtSetValue(evt, out, *(u8*)((s32)*(void**)((s32)unit + 0x144) + 0x10));
    return 2;
}


void __makeTechMenuFunc(void* commandWork, s32 param_2) {
}


s32 _get_clauda_kiss_hit_position(int param_1) {
    return 0;
}


s32 _clauda_breath_effect_fire(int param_1) {
    return 0;
}


s32 _clauda_breath_effect_ready(int param_1) {
    return 0;
}


s32 _make_kiss_weapon(int param_1) {
    return 0;
}


s32 _make_kumoguard_weapon(int param_1) {
    return 0;
}


s32 _make_breath_weapon(void* pEvt) {
    return 0;
}


s32 _clauda_make_extra_work_area(int param_1) {
    return 0;
}
