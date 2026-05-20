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