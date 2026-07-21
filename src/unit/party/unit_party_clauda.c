#include "unit/party/unit_party_clauda.h"

s32 _clauda_make_extra_work_area(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void* BattleAlloc(u32 size);
    void* battleWork;
    s32 id;
    void* unit;

    battleWork = _battleWorkPointer;
    id = BattleTransID(evt, -2);
    unit = BattleGetUnitPtr(battleWork, id);
    *(void**)((s32)unit + 0x314) = BattleAlloc(0xC0);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void __makeTechMenuFunc(void* commandWork, s32* count) {
    extern void* _battleWorkPointer;
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_80378D60[];
    s32 techLv;
    void* party;
    u8* data;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    data = lbl_80378D60;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
    *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x40;
    *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
    *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
        *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
    *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
        msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
    *count = *count + 1;

    if (techLv >= 0) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x100;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 1) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x1C0;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 2) {
        *(s32*)((s32)commandWork + *count * 0x1C + 0x90) = -1;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x94) = 0;
        *(void**)((s32)commandWork + *count * 0x1C + 0x80) = data + 0x280;
        *(s32*)((s32)commandWork + *count * 0x1C + 0x84) = 0;
        *(u16*)((s32)commandWork + *count * 0x1C + 0x8C) =
            *(u16*)(*(s32*)((s32)commandWork + *count * 0x1C + 0x80) + 4);
        *(void**)((s32)commandWork + *count * 0x1C + 0x88) =
            msgSearch(**(char***)((s32)commandWork + *count * 0x1C + 0x80));
        *count = *count + 1;
    }
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
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

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _clauda_breath_effect_ready(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* effKumokumoN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 angle, f32 speed, f32 scale);
    extern f32 float_25_80424088;
    extern f32 float_0_80424074;
    extern f32 float_2_8042408c;
    extern f32 float_1_8042407c;
    extern f32 float_180_80424080;

    void* battleWork = _battleWorkPointer;
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    f32 z;
    f32 y;
    f32 x;
    void* eff;
    void* work;
    s8 dir;
    f32 scale;

    BtlUnit_GetPos(unit, &x, &y, &z);
    dir = *(s8*)((s32)unit + 0x189);
    scale = *(f32*)((s32)unit + 0x114);
    x += (f32)(dir * 20) * scale;
    y += float_25_80424088 * scale;
    if (dir >= 0) {
        eff = effKumokumoN64Entry(0, 60, x, y, z, float_0_80424074, float_2_8042408c, float_1_8042407c);
    } else {
        eff = effKumokumoN64Entry(0, 60, x, y, z, float_180_80424080, float_2_8042408c, float_1_8042407c);
    }
    work = *(void**)((s32)eff + 0xC);
    *(s32*)((s32)work + 0x18) = 0x4C;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x1C) = 0xA7;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x20) = 0xCA;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x28) = 0x7D;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x2C) = 0x1B;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x30) = 0xF5;
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _clauda_breath_effect_fire(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* effKumokumoN64Entry(s32 type, s32 time, f32 x, f32 y, f32 z, f32 angle, f32 speed, f32 scale);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern f32 float_15_80424070;
    extern f32 float_0_80424074;
    extern f32 float_neg2_80424078;
    extern f32 float_1_8042407c;
    extern f32 float_180_80424080;

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    f32 z;
    f32 y;
    f32 x;
    void* eff;
    void* work;
    s8 dir;
    f32 scale;

    BtlUnit_GetPos(unit, &x, &y, &z);
    dir = *(s8*)((s32)unit + 0x189);
    scale = *(f32*)((s32)unit + 0x114);
    x += (f32)(dir * 25) * scale;
    y += float_15_80424070 * scale;
    if (dir >= 0) {
        eff = effKumokumoN64Entry(0, 0, x, y, z, float_0_80424074, float_neg2_80424078, float_1_8042407c);
    } else {
        eff = effKumokumoN64Entry(0, 0, x, y, z, float_180_80424080, float_neg2_80424078, float_1_8042407c);
    }
    work = *(void**)((s32)eff + 0xC);
    *(s32*)((s32)work + 0x18) = 0x4C;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x1C) = 0xA7;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x20) = 0xCA;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x28) = 0x7D;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x2C) = 0x1B;
    *(s32*)((s32)*(void**)((s32)eff + 0xC) + 0x30) = 0xF5;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x58) = -10.0f;
    evtSetValue(evt, out, (s32)eff);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 _make_breath_weapon(void* pEvt) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern void* memcpy(void* dest, const void* src, u32 size);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)pEvt + 0x18);
    s32 id = BattleTransID(pEvt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    void* weapon = *(void**)((s32)unit + 0x314);
    s32 src = evtGetValue(pEvt, out);
    s32 flag = evtGetValue(pEvt, args[1]);
    memcpy(weapon, (void*)src, 0xC0);
    *(u8*)((s32)weapon + 0xA5) = flag;
    evtSetValue(pEvt, out, (s32)weapon);
    return 2;
}

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _make_kiss_weapon(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern void* memcpy(void* dest, const void* src, u32 size);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    void* weapon = *(void**)((s32)unit + 0x314);
    s32 src = evtGetValue(evt, out);
    evtGetValue(evt, args[1]);
    memcpy(weapon, (void*)src, 0xC0);
    evtSetValue(evt, out, (s32)weapon);
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

s32 _get_clauda_kiss_hit_position(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern f32 BattleGetFloorHeight(void* battleWork, f32 x, f32 y, f32 z);
    extern void evtSetFloat(void* evt, s32 arg, f32 value);
    extern f32 float_30_8042406c;
    s32* args;
    void* battleWork;
    s32 ownerId;
    s32 targetId;
    s32 outX;
    s32 outY;
    s32 outZ;
    void* owner;
    void* target;
    f32 x;
    f32 y;
    f32 z;
    f32 floor;
    s8 dir;
    f32 scale;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    ownerId = evtGetValue(evt, args[0]);
    targetId = evtGetValue(evt, args[1]);
    evtGetValue(evt, args[2]);
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    owner = BattleGetUnitPtr(battleWork, BattleTransID(evt, ownerId));
    target = BattleGetUnitPtr(battleWork, BattleTransID(evt, targetId));
    dir = *(s8*)((s32)target + 0x189);
    BtlUnit_GetPos(target, &x, &y, &z);
    scale = *(f32*)((s32)target + 0x114);
    x += (f32)dir * (*(f32*)((s32)target + 0xF8) * scale);
    y += *(f32*)((s32)target + 0xFC) * scale;
    z += *(f32*)((s32)target + 0x100) * scale;
    y -= float_30_8042406c * *(f32*)((s32)owner + 0x114);
    floor = BattleGetFloorHeight(battleWork, x, y, z);
    if (floor > y) {
        y = floor;
    }
    evtSetFloat(evt, outX, x);
    evtSetFloat(evt, outY, y);
    evtSetFloat(evt, outZ, z);
    return 2;
}
s32 _make_kumoguard_weapon(void* evt) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern void* memcpy(void* dest, const void* src, u32 size);

    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    s32 id = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, id);
    s32 out = args[0];
    void* weapon = *(void**)((s32)unit + 0x314);
    s32 src = evtGetValue(evt, out);
    s32 flag = evtGetValue(evt, args[1]);
    memcpy(weapon, (void*)src, 0xC0);
    *(u8*)((s32)weapon + 0x8E) = flag;
    evtSetValue(evt, out, (s32)weapon);
    return 2;
}

