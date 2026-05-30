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


void __makeTechMenuFunc(void* commandWork, s32 param_2) {
}


s32 _tsuranuki_effect_control(int param_1, int param_2) {
    return 0;
}


s32 _check_guard_koura(int param_1) {
    return 0;
}


s32 _color_lv_set(int param_1) {
    return 0;
}
