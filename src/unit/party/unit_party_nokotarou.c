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


s32 _check_guard_koura(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern void* BtlUnit_GetGuardKouraPtr(void* unit);
    extern void evtSetValue(void* evt, s32 arg, s32 value);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = evtGetValue(evt, args[0]);
    s32 out = args[1];
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));
    void* guard;

    if (!(*(u32*)((s32)unit + 0x104) & 0x20)) {
        evtSetValue(evt, out, -1);
        return 2;
    }
    guard = BtlUnit_GetGuardKouraPtr(unit);
    if (guard == 0) {
        evtSetValue(evt, out, -1);
        return 2;
    }
    evtSetValue(evt, out, *(s32*)guard);
    return 2;
}


s32 _color_lv_set(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
    extern void evtSetValue(void* evt, s32 arg, s32 value);

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id = evtGetValue(evt, args[0]);
    s32 mode = evtGetValue(evt, args[1]);
    s32 out = args[2];
    s32 value = evtGetValue(evt, out);
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));

    if (mode != 0) {
        *(u8*)((s32)unit + 0x311) = value;
    } else {
        evtSetValue(evt, out, *(u8*)((s32)unit + 0x311));
    }
    return 2;
}
