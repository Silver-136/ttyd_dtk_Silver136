#include "unit/party/unit_party_nokotarou.h"
#include "effect/eff_nokotarou.h"

extern void* _battleWorkPointer;
extern void evtSetValue(void* evt, s32 arg, s32 value);
extern s32 evtGetValue(void* evt, s32 arg);
extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void __makeTechMenuFunc(void* commandWork, s32* count) {
    typedef struct TechMenuEntry {
        char** name;
        s32 value;
        char* message;
        u16 icon;
        u16 pad_e;
        s32 field_10;
        s32 field_14;
        s32 pad_18;
    } TechMenuEntry;
    typedef struct TechMenuWork {
        u8 pad_0[0x80];
        TechMenuEntry entries[1];
    } TechMenuWork;
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_8038ED18[];
    s32 techLv;
    void* party;
    u8* data;
    TechMenuWork* work;

    data = lbl_8038ED18;
    work = commandWork;
    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    work->entries[*count].field_10 = -1;
    work->entries[*count].field_14 = 0;
    work->entries[*count].name = (char**)(data + 0x844);
    work->entries[*count].value = 0;
    work->entries[*count].icon = 0x158;
    work->entries[*count].message = msgSearch(*work->entries[*count].name);
    *count = *count + 1;

    if (techLv >= 0) {
        work->entries[*count].field_10 = -1;
        work->entries[*count].field_14 = 0;
        work->entries[*count].name = (char**)(data + 0x904);
        work->entries[*count].value = 0;
        work->entries[*count].icon = 0x159;
        work->entries[*count].message = msgSearch(*work->entries[*count].name);
        *count = *count + 1;
    }
    if (techLv >= 1) {
        work->entries[*count].field_10 = -1;
        work->entries[*count].field_14 = 0;
        work->entries[*count].name = (char**)(data + 0x9C4);
        work->entries[*count].value = 0;
        work->entries[*count].icon = *(u16*)(work->entries[*count].name + 1);
        work->entries[*count].message = msgSearch(*work->entries[*count].name);
        *count = *count + 1;
    }
    if (techLv >= 2) {
        work->entries[*count].field_10 = -1;
        work->entries[*count].field_14 = 0;
        work->entries[*count].name = (char**)(data + 0xA84);
        work->entries[*count].value = 0;
        work->entries[*count].icon = *(u16*)(work->entries[*count].name + 1);
        work->entries[*count].message = msgSearch(*work->entries[*count].name);
        *count = *count + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _check_mario_move_count(void* evt) {
    extern void* BattleGetMarioPtr(void* battleWork);

    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 out = args[0];
    void* mario = BattleGetMarioPtr(battleWork);
    evtSetValue(evt, out, *(s8*)((s32)mario + 0x22));
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 _color_lv_set(void* evt) {

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
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _tsuranuki_effect_control(void* evt, s32 isFirstCall) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* BtlUnit_GetPartsPtr(void* unit, s32 partsId);
    extern void effSoftDelete(void* eff);
    extern f32 float_2_804243a8;
    extern f32 float_0p006_804243ac;
    s32* args;
    void* battleWork;
    void* unit;
    void* parts;
    void* eff;
    s32 target;
    f32 z;
    f32 y;
    f32 x;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    target = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, target));
    BtlUnit_GetPos(unit, &x, &y, &z);
    parts = BtlUnit_GetPartsPtr(unit, 1);
    if (isFirstCall != 0) {
        *(void**)((s32)evt + 0x78) = effNokotarouEntry(
            0, 0x3E8, x, y, z, *(f32*)((s32)unit + 0x114));
        *(s32*)((s32)parts + 0x138) = 0;
        *(s32*)((s32)parts + 0x134) = 0;
        *(s32*)((s32)evt + 0x7C) = 0;
    }
    target = *(s32*)((s32)parts + 0x134) * 100;
    if (*(s32*)((s32)evt + 0x7C) != target) {
        if (*(s32*)((s32)evt + 0x7C) < target) {
            *(s32*)((s32)evt + 0x7C) += 10;
            if (*(s32*)((s32)evt + 0x7C) > target) {
                *(s32*)((s32)evt + 0x7C) = target;
            }
        } else {
            *(s32*)((s32)evt + 0x7C) -= 10;
            if (*(s32*)((s32)evt + 0x7C) < target) {
                *(s32*)((s32)evt + 0x7C) = target;
            }
        }
    }
    eff = *(void**)((s32)evt + 0x78);
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x10) = x;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x14) = y;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x18) = z;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x1C) =
        (float_2_804243a8 +
         float_0p006_804243ac * (f32)*(s32*)((s32)evt + 0x7C)) *
        *(f32*)((s32)unit + 0x114);
    if (*(s32*)((s32)parts + 0x138) != 0) {
        *(s32*)((s32)parts + 0x138) = 0;
        effSoftDelete(eff);
        return 2;
    }
    return 0;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _check_guard_koura(void* evt) {
    extern void* BtlUnit_GetGuardKouraPtr(void* unit);

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
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
