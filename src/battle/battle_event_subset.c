#include "battle/battle_event_subset.h"
#include "battle/battle_sub.h"

extern void* _battleWorkPointer;
extern s32 evtGetValue(void* evt, s32 var);
extern void evtSetValue(void* evt, s32 var, s32 value);
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
extern void* effWhirlwindN64Entry(f32 height, s32 flip, s32 unk, s32 duration);

s32 _disable_restore_command_cursor(void) {
    void* battleWork;

    battleWork = _battleWorkPointer;
    *(s32*)((s32)battleWork + 0x1C70) = 0;
    *(u8*)((s32)battleWork + 0x1C74) = 0;

    return 2;
}


s32 _check_at_dm_event_wait(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    void* unit;
    s32 evtId;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    if (unit == 0) {
        return 2;
    }

    evtId = *(s32*)((s32)unit + 0x2A8);
    if (evtId != 0 && evtCheckID(evtId) != 0) {
        return 0;
    }

    evtId = *(s32*)((s32)unit + 0x2B4);
    if (evtId != 0 && evtCheckID(evtId) != 0) {
        return 0;
    }

    return 2;
}


s32 _add_star_point_disp_offset(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    void* unit;
    s32 xVar;
    s32 yVar;
    s32 zVar;
    f32 x;
    f32 y;
    f32 z;
    void* part;

    battleWork = _battleWorkPointer;
    args = *(s32**)((s32)evt + 0x18);
    type = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    xVar = args[1];
    x = (f32)evtGetValue(evt, xVar);
    yVar = args[2];
    y = (f32)evtGetValue(evt, yVar);
    zVar = args[3];
    z = (f32)evtGetValue(evt, zVar);
    part = *(void**)((s32)unit + 0x10);
    x += *(f32*)((s32)part + 0x90);
    y += *(f32*)((s32)part + 0x94);
    z += *(f32*)((s32)part + 0x98);
    evtSetValue(evt, xVar, (s32)x);
    evtSetValue(evt, yVar, (s32)y);
    evtSetValue(evt, zVar, (s32)z);

    return 2;
}

s32 unk_80113f1c(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    void* unit;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    if (unit != 0) {
        *(u16*)((s32)unit + 0x1FE) |= 1;
    }

    return 2;
}


s32 _check_through_pos(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    s32 pos;
    s32 a;
    s32 b;
    s32 dst;
    void* unit;
    s32 min;
    s32 max;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    pos = evtGetValue(evt, args[1]);
    a = evtGetValue(evt, args[2]);
    b = evtGetValue(evt, args[3]);
    dst = args[4];
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));

    if (a > b) {
        min = b;
        max = a;
    } else {
        min = a;
        max = b;
    }

    if (pos >= min && pos <= max) {
        evtSetValue(evt, dst, 1);
    } else {
        evtSetValue(evt, dst, 0);
        if (*(s8*)((s32)unit + 0x189) >= 0) {
            if (pos > max) {
                evtSetValue(evt, dst, 1);
            }
        } else if (pos < min) {
            evtSetValue(evt, dst, 1);
        }
    }

    return 2;
}


s32 _binta_effect(void* evt) {
    s32* args;
    void* battleWork;
    s32 type;
    s32 dir;
    void* unit;
    void* eff;
    f32 z;
    f32 y;
    f32 x;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    type = evtGetValue(evt, args[0]);
    dir = evtGetValue(evt, args[1]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, type));
    BtlUnit_GetPos(unit, &x, &y, &z);
    eff = effWhirlwindN64Entry(*(f32*)((s32)unit + 0x114), (u32)dir >> 31, 0, 0x5A);
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 8) = x;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0xC) = y;
    *(f32*)((s32)*(void**)((s32)eff + 0xC) + 0x10) = z;

    return 2;
}

const char str_SFX_PARTY_BATTLE_DIE_802ee928[] = "SFX_PARTY_BATTLE_DIE1";
const char str_recovery_802ee940[] = "recovery";
const char str_SFX_BTL_ENEMY_DIE1_1_802ee94c[] = "SFX_BTL_ENEMY_DIE1_1";
const char str_SFX_BTL_ENEMY_DIE1_2_802ee964[] = "SFX_BTL_ENEMY_DIE1_2";
const char str_kemuri_test_802ee97c[] = "kemuri_test";
const char str_star_point_802ee988[] = "star_point";
const char str_SFX_BTL_DAMAGE_PRESS_802ee994[] = "SFX_BTL_DAMAGE_PRESS1";
const char str_SFX_BTL_ATTACK_MISS2_802ee9ac[] = "SFX_BTL_ATTACK_MISS2";
const char str_SFX_VOICE_MARIO_DAMA_802ee9c4[] = "SFX_VOICE_MARIO_DAMAGED1";
const char str_SFX_BTL_ATTACK_MISS3_802ee9e0[] = "SFX_BTL_ATTACK_MISS3";
const char str_SFX_BTL_POS_CHANGE1_802ee9f8[] = "SFX_BTL_POS_CHANGE1";
const char str_SFX_ENM_ESCAPE1_802eea0c[] = "SFX_ENM_ESCAPE1";
const char str_SFX_BTL_CLAUD_BREATH_802eea1c[] = "SFX_BTL_CLAUD_BREATH3";
const f32 float_0_80422308 = 0.0f;
