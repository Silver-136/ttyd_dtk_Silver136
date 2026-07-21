#include "unit/party/unit_party_sanders.h"

s32 _sanders_make_extra_work_area(void* evt) {
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


void __makeTechMenuFunc(void* commandWork, s32* count) {
    extern void* _battleWorkPointer;
    extern void* BattleGetPartyPtr(void* battleWork);
    extern s32 BattleTransPartyId(s32 id);
    extern s32 partyGetTechLv(s32 partyId);
    extern char* msgSearch(char* msg);
    extern u8 lbl_803875D0[];
    s32 techLv;
    void* party;
    u8* entry;

    party = BattleGetPartyPtr(_battleWorkPointer);
    techLv = partyGetTechLv(BattleTransPartyId(*(s32*)((s32)party + 8)));

    entry = (u8*)commandWork + *count * 0x1C;
    *(s32*)(entry + 0x90) = -1;
    *(s32*)(entry + 0x94) = 0;
    *(void**)(entry + 0x80) = lbl_803875D0 + 0x650;
    *(s32*)(entry + 0x84) = 0;
    *(u16*)(entry + 0x8C) = *(u16*)(*(s32*)(entry + 0x80) + 4);
    *(void**)(entry + 0x88) = msgSearch(**(char***)(entry + 0x80));
    *count = *count + 1;

    if (techLv >= 0) {
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x90) = -1;
        *(s32*)(entry + 0x94) = 0;
        *(void**)(entry + 0x80) = lbl_803875D0 + 0x710;
        *(s32*)(entry + 0x84) = 0;
        *(u16*)(entry + 0x8C) = *(u16*)(*(s32*)(entry + 0x80) + 4);
        *(void**)(entry + 0x88) = msgSearch(**(char***)(entry + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 1) {
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x90) = -1;
        *(s32*)(entry + 0x94) = 0;
        *(void**)(entry + 0x80) = lbl_803875D0 + 0x7D0;
        *(s32*)(entry + 0x84) = 0;
        *(u16*)(entry + 0x8C) = *(u16*)(*(s32*)(entry + 0x80) + 4);
        *(void**)(entry + 0x88) = msgSearch(**(char***)(entry + 0x80));
        *count = *count + 1;
    }
    if (techLv >= 2) {
        entry = (u8*)commandWork + *count * 0x1C;
        *(s32*)(entry + 0x90) = -1;
        *(s32*)(entry + 0x94) = 0;
        *(void**)(entry + 0x80) = lbl_803875D0 + 0x890;
        *(s32*)(entry + 0x84) = 0;
        *(u16*)(entry + 0x8C) = *(u16*)(*(s32*)(entry + 0x80) + 4);
        *(void**)(entry + 0x88) = msgSearch(**(char***)(entry + 0x80));
        *count = *count + 1;
    }
}

s32 _get_bomb_hit_position(int param_1) {
    extern void* _battleWorkPointer;
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern f32 evtSetFloat(void* evt, s32 arg, f32 value);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);

    void* evt = (void*)param_1;
    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 id0;
    s32 id1;
    s32 outX;
    s32 outY;
    s32 outZ;
    void* unit;
    s8 dir;
    f32 z;
    f32 y;
    f32 x;
    f32 scale;

    id0 = evtGetValue(evt, args[0]);
    id1 = evtGetValue(evt, args[1]);
    evtGetValue(evt, args[2]);
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    BattleGetUnitPtr(battleWork, BattleTransID(evt, id0));
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id1));
    dir = *(s8*)((s32)unit + 0x189);
    BtlUnit_GetPos(unit, &x, &y, &z);
    scale = *(f32*)((s32)unit + 0x114);
    x += (f32)dir * (*(f32*)((s32)unit + 0xEC) * scale);
    y += *(f32*)((s32)unit + 0xF0) * scale;
    z += *(f32*)((s32)unit + 0xF4) * scale;
    evtSetFloat(evt, outX, x);
    evtSetFloat(evt, outY, y);
    evtSetFloat(evt, outZ, z);
    return 2;
}

#pragma no_register_save_helpers off
#pragma use_lmw_stmw on
s32 _make_counterset_weapon(void* evt) {
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
    *(u8*)((s32)weapon + 0xA7) = flag;
    evtSetValue(evt, out, (s32)weapon);
    return 2;
}

u8 _shot_move(void* evt, s32 isFirstCall) {
    extern void* _battleWorkPointer;
    extern s32 evtGetValue(void* evt, s32 arg);
    extern f32 evtSetFloat(void* evt, s32 arg, f32 value);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void BtlUnit_SetPos(void* unit, f32 x, f32 y, f32 z);
    extern f32 angleABf(f32 x0, f32 z0, f32 x1, f32 z1);
    extern f64 cosfd(f64 deg);
    extern f64 sinfd(f64 deg);
    extern void btlMovePos(f32* x, f32* z, f32 speed, f32 angle);
    extern f32 BattleGetFloorHeight(void* battleWork, f32 x, f32 y, f32 z);
    extern f32 float_0p1_804241cc;
    extern f32 float_0_804241d0;
    extern f32 float_1_804241d4;
    extern f32 float_0p2_804241d8;
    extern f32 float_0p8_804241dc;
    extern f32 float_0p9_804241e0;
    extern f32 float_0p75_804241e4;
    s32* args;
    void* battleWork;
    s32 unitId;
    s32 angle;
    f32 speed;
    void* unit;
    s32 outX;
    s32 outY;
    s32 outZ;
    s32 outSpeed;
    f32 x;
    f32 y;
    f32 z;
    f32 floor;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    unitId = BattleTransID(evt, evtGetValue(evt, args[0]));
    angle = evtGetValue(evt, args[1]);
    speed = (f32)evtGetValue(evt, args[2]) * float_0p1_804241cc;
    outX = args[3];
    outY = args[4];
    outZ = args[5];
    outSpeed = args[6];
    unit = BattleGetUnitPtr(battleWork, unitId);

    if (isFirstCall != 0) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        *(f32*)((s32)unit + 0x154) = x;
        *(f32*)((s32)unit + 0x158) = y;
        *(f32*)((s32)unit + 0x15C) = z;
        *(f32*)((s32)unit + 0x17C) = angleABf(float_0_804241d0, float_0_804241d0, float_1_804241d4, float_0_804241d0);
        *(f32*)((s32)unit + 0x174) = float_0p2_804241d8;
        *(f32*)((s32)unit + 0x178) = float_0p8_804241dc * (float_0p9_804241e0 * (speed * (f32)cosfd((f64)angle)));
        *(f32*)((s32)unit + 0x170) = float_0p8_804241dc * (float_0p75_804241e4 * (speed * (f32)sinfd((f64)angle)));
    }

    *(f32*)((s32)unit + 0x158) += *(f32*)((s32)unit + 0x178);
    *(f32*)((s32)unit + 0x178) -= *(f32*)((s32)unit + 0x174);
    btlMovePos((f32*)((s32)unit + 0x154), (f32*)((s32)unit + 0x15C), *(f32*)((s32)unit + 0x170), *(f32*)((s32)unit + 0x17C));
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
    floor = BattleGetFloorHeight(battleWork, *(f32*)((s32)unit + 0x154), *(f32*)((s32)unit + 0x158), *(f32*)((s32)unit + 0x15C));
    if (*(f32*)((s32)unit + 0x158) > floor) {
        return 0;
    }
    BtlUnit_SetPos(unit, *(f32*)((s32)unit + 0x154), floor, *(f32*)((s32)unit + 0x15C));
    evtSetValue(evt, outX, (s32)*(f32*)((s32)unit + 0x154));
    evtSetValue(evt, outY, (s32)floor);
    evtSetValue(evt, outZ, (s32)*(f32*)((s32)unit + 0x15C));
    evtSetFloat(evt, outSpeed, *(f32*)((s32)unit + 0x170));
    return 1;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _judge_on_stage(int param_1) {
    extern void* _battleWorkPointer;
    extern void* BattleStageGetPtr(void);
    extern s32 evtGetValue(void* evt, s32 arg);
    extern void evtSetValue(void* evt, s32 arg, s32 value);
    extern s32 BattleTransID(void* evt, s32 id);
    extern void* BattleGetUnitPtr(void* battleWork, s32 id);
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern f32 __fabsf(f32 value);
    extern f32 float_40_804241c8;

    void* evt = (void*)param_1;
    void* battleWork = _battleWorkPointer;
    s32* args = *(s32**)((s32)evt + 0x18);
    void* stage = BattleStageGetPtr();
    s32 id = evtGetValue(evt, args[0]);
    s32 out = args[1];
    void* unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));
    f32 x;
    f32 y;
    f32 z;

    BtlUnit_GetPos(unit, &x, &y, &z);
    if (__fabsf(x) < float_40_804241c8 + *(f32*)((s32)stage + 0xB24)) {
        evtSetValue(evt, out, 1);
    } else {
        evtSetValue(evt, out, 0);
    }
    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on

