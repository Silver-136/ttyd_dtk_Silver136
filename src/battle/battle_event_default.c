#include "battle/battle_event_default.h"
extern void* _battleWorkPointer;

extern s32 BattleTransID(void* evt, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 unitId);
extern void* BattleGetMarioPtr(void* battleWork);
extern void* BattleGetPartyPtr(void* battleWork);

extern void BattleAudience_Case_Escape_Bad(void);
extern void BattleAudience_Case_Escape_Good(void);
extern void BattleAudience_Case_Escape(void);

extern void BattleAcHelpSetHelp(const char* msg);
extern void BattleAcHelpSetDispType(s32 type);

extern s32 BattlePartyAnimeLoad(void* partyAnimeWork);

extern void evtSetValue(void* evt, s32 var, s32 value);

extern const char str_msg_ac_escape_802ee90c[];

s32 _get_flower_suitoru_point(void* evt) {
    s32* args;
    void* battleWork;
    s32 id;
    s32 dst;
    void* unit;
    s32 value;
    s32 max;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    id = args[0];
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));

    value = 0;
    if (unit != 0) {
        value = *(u8*)((s32)unit + 0x2FA);
        max = *(s32*)((s32)unit + 0x284);
        if (value > max) {
            value = max;
        }
    }

    evtSetValue(evt, dst, value);

    return 2;
}

s32 _get_heart_suitoru_point(void* evt) {
    s32* args;
    void* battleWork;
    s32 id;
    s32 dst;
    void* unit;
    s32 value;
    s32 max;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;
    id = args[0];
    dst = args[1];

    unit = BattleGetUnitPtr(battleWork, BattleTransID(evt, id));

    value = 0;
    if (unit != 0) {
        value = *(u8*)((s32)unit + 0x2F9);
        max = *(s32*)((s32)unit + 0x284);
        if (value > max) {
            value = max;
        }
    }

    evtSetValue(evt, dst, value);

    return 2;
}

s32 _get_escape_rate(void* evt) {
    s32* args;
    void* battleWork;
    void* data;

    battleWork = _battleWorkPointer;
    args = *(s32**)((s32)evt + 0x18);

    data = *(void**)((s32)battleWork + 0x2738);
    data = *(void**)((s32)data + 0xC);
    data = *(void**)((s32)data + 0x18);
    data = *(void**)((s32)data + 0x4);
    data = *(void**)data;

    evtSetValue(evt, args[0], *(u8*)((s32)data + 0x13));

    return 2;
}

s32 _set_escape_ac_hlp_msg(void) {
    BattleAcHelpSetHelp(str_msg_ac_escape_802ee90c);
    BattleAcHelpSetDispType(2);

    return 2;
}

s32 _btl_escape_reset_move_color_lv_player_unit_all(void) {
    void* battleWork;
    void* unit;

    battleWork = _battleWorkPointer;

    unit = BattleGetMarioPtr(battleWork);
    *(u8*)((s32)unit + 0x311) = 0xFF;

    unit = BattleGetPartyPtr(battleWork);
    if (unit != 0) {
        *(u8*)((s32)unit + 0x311) = 0xFF;
    }

    return 2;
}

s32 btlevtcmd_audience_code_escape_fail(void) {
    BattleAudience_Case_Escape_Bad();

    return 2;
}

s32 btlevtcmd_audience_code_escape_success(void) {
    BattleAudience_Case_Escape_Good();

    return 2;
}

s32 btlevtcmd_audience_code_escape(void) {
    BattleAudience_Case_Escape();

    return 2;
}

s32 _anime_load_wait(void) {
    return BattlePartyAnimeLoad(*(void**)((s32)_battleWorkPointer + 0x1C68)) != 0 ? 2 : 0;
}

s32 _anime_load(void) {
    BattlePartyAnimeLoad(*(void**)((s32)_battleWorkPointer + 0x1C68));

    return 2;
}


s32 _backfire(void* evt, s32 firstCall) {
    extern void BtlUnit_GetPos(void* unit, f32* x, f32* y, f32* z);
    extern void* effFireEntry(f32 x, f32 y, f32 z, f32 scale, s32 type, s32 duration);
    extern void effDelete(void* effect);
    extern void BattleDamageDirect(s32 attacker, void* unit, s32 part, s32 damage, s32 zero, s32 flags, s32 arg7, s32 arg8);
    extern s32 evtGetValue(void* evt, s32 value);
    extern f32 sinfd(f32 value);
    extern f32 float_1_804222fc;
    extern f32 float_0p4_80422300;
    extern f32 float_1p6_80422304;
    s32* args;
    void* battleWork;
    s32 unitId;
    void* unit;
    void* effectWork;
    s32 counter;
    s32 value;
    f32 x;
    f32 y;
    f32 z;

    args = *(s32**)((s32)evt + 0x18);
    battleWork = _battleWorkPointer;

    value = evtGetValue(evt, args[0]);
    unitId = BattleTransID(evt, value);
    unit = BattleGetUnitPtr(battleWork, unitId);
    if (unit == 0) {
        return 2;
    }

    if (firstCall != 0) {
        BtlUnit_GetPos(unit, &x, &y, &z);
        *(void**)((s32)evt + 0x78) = effFireEntry(x, y, z, float_1_804222fc, 0, 0);
        *(s32*)((s32)evt + 0x7C) = 0;
    }

    effectWork = *(void**)((s32)*(void**)((s32)evt + 0x78) + 0xC);
    *(s32*)((s32)evt + 0x7C) = *(s32*)((s32)evt + 0x7C) + 1;
    counter = *(s32*)((s32)evt + 0x7C);

    if (*(s32*)((s32)evt + 0x7C) >= 60) {
        effDelete(*(void**)((s32)evt + 0x78));
        BattleDamageDirect(-5, unit, 0, 1, 0, 0x118, 0, 1);
        if (BattleTransID(evt, *(s32*)((s32)evt + 0x160)) != unitId) {
            return 2;
        }
        return 0xFF;
    }

    *(f32*)((s32)effectWork + 0x24) = float_1p6_80422304 * sinfd((f32)(counter * 3)) + float_0p4_80422300;
    return 0;
}

s32 _check_pose_stay(void* evt) {
    s32* args = *(s32**)((s32)evt + 0x18);
    void* battleWork = _battleWorkPointer;
    s32 dst = args[0];
    s32 unitId = BattleTransID(evt, -2);
    void* unit = BattleGetUnitPtr(battleWork, unitId);
    s32 value = 0;

    if (*(s32*)((s32)unit + 8) == 0xDE) {
        value = 1;
        if ((*(u32*)((s32)battleWork + 0x163F4) & 1) != 0 &&
            (*(u32*)((s32)unit + 0x27C) & 0x10) == 0) {
            value = 0;
        }
    }
    evtSetValue(evt, dst, value);

    return 2;
}

