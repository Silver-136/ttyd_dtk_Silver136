#include "unit/party/unit_party_chuchurina.h"

extern s32 mono_alpha;
extern void* _battleWorkPointer;
extern f32 float_910_8042412c;
extern s32 BattleTransID();
extern void* BattleGetUnitPtr(void*, s32);
extern void* BtlUnit_GetPartsPtr(void*, s32);
extern void dispEntry(s32, s32, void*, void*, f32);


void mono_disp(int param_1, void* unit) {
}


void __makeTechMenuFunc(void* commandWork, s32 param_2) {
}


s32 _chuchu_item_steal(void* pEvt) {
    return 0;
}


s32 _get_binta_hit_position(int param_1) {
    return 0;
}


s32 _get_itemsteal_param(void* pEvt) {
    return 0;
}


void mono_capture(s32 param_1, void* unit) {
}


s32 _make_madowase_weapon(int param_1) {
    return 0;
}


s32 mono_off(int param_1) {
    return 0;
}


s32 mono_capture_event(int param_1) {
    s32 id = BattleTransID(param_1, -2);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    BtlUnit_GetPartsPtr(unit, 1);
    dispEntry(8, 0, mono_capture, unit, float_910_8042412c);
    return 2;
}


s32 mono_main(int param_1) {
    s32 id = BattleTransID(param_1, -2);
    void* unit = BattleGetUnitPtr(_battleWorkPointer, id);

    dispEntry(8, 0, mono_disp, unit, float_910_8042412c);
    return 0;
}


s32 _chuchu_make_extra_work_area(int param_1) {
    return 0;
}


s32 mono_on(s32 param_1, int param_2) {
    if (param_2 != 0) {
        mono_alpha = 0;
    }
    mono_alpha += 10;
    if (mono_alpha > 0xFF) {
        mono_alpha = 0xFF;
        return 2;
    }
    return 0;
}
