#include "battle/battle_item_data.h"

extern void* _battleWorkPointer;
s32 evtGetValue(void* event, s32 arg);
s32 BattleTransID(void* event, s32 id);
void* BattleGetUnitPtr(void* battleWork, s32 unitId);
void BattleStatusWindowSystemOn(void);
void N_statusClearBlink(void);
void statusMarioHPBlink(s32 frames);
void statusPartyHPBlink(s32 frames);
void statusFPBlink(s32 frames);
s32 sysMsec2Frame(s32 msec);
f32 intplGetValue(s32 mode, s32 current, s32 total, f32 start, f32 end);
void* camGetPtr(s32 cameraId);
extern f32 float_720_80422824;
extern f32 float_0_80422804;

void _disp_item_icon(void* unused, void* work) {
    extern u8 itemDataTable[];
    extern f32 float_1_80422800;
    extern void iconDispGx(void* pos, s32 flags, s32 iconId, f32 scale);

    void* pos = (void*)((s32)*(void**)((s32)work + 0x78) + 0xC);
    s32 item = *(s32*)((s32)work + 0x7C);
    s32 offset;
    u8* data = itemDataTable;
    s32 icon;
    f32 scale = float_1_80422800;

    offset = item * 0x28;
    data += offset;
    icon = *(u16*)(data + 0x20);
    iconDispGx(pos, 0, icon, scale);
}


u8 _throw_item_icon(void) {
    return 0;
}


u32 _naniga_okorukana_check(int param_1, int param_2) {
    return 0;
}


u8 _nancy_disp(void) {
    return 0;
}


s32 _nancy_mirage(int param_1, int param_2) {
    return 0;
}


s32 _get_throw_first_pos(int param_1) {
    return 0;
}


s32 _sampling_random_target(int param_1) {
    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _hpfp_change_declare_2(void* event) {
    s32* args;
    void* battleWork;
    void* unit;
    s32 id;
    s32 unitKind;

    args = *(s32**)((s32)event + 0x18);
    battleWork = _battleWorkPointer;
    id = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, id));

    if (unit != 0) {
        unitKind = *(s32*)((s32)unit + 8);
        if (unitKind == 0xDE) {
            N_statusClearBlink();
            statusMarioHPBlink(0x78);
            statusFPBlink(0x78);
        } else if (unitKind >= 0xE0 && unitKind < 0xE7) {
            N_statusClearBlink();
            statusPartyHPBlink(0x78);
            statusFPBlink(0x78);
        }
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


u8 _cam_rot(void* work, s32 isFirstCall) {
    f32 rot;
    f32 start;
    f32 end;

    if (isFirstCall != 0) {
        *(s32*)((s32)work + 0x78) = sysMsec2Frame(2000);
        *(s32*)((s32)work + 0x7C) = *(s32*)((s32)work + 0x78);
    }

    start = float_720_80422824;
    end = float_0_80422804;
    rot = intplGetValue(4, *(s32*)((s32)work + 0x78), *(s32*)((s32)work + 0x7C), start, end);
    *(f32*)((s32)camGetPtr(4) + 0x14C) = rot;

    *(s32*)((s32)work + 0x78) -= 1;
    if (*(s32*)((s32)work + 0x78) <= 0) {
        *(f32*)((s32)camGetPtr(4) + 0x14C) = float_0_80422804;
        return 1;
    }

    return 0;
}


#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _hpfp_change_declare_1(void* event) {
    s32* args;
    void* battleWork;
    void* unit;
    s32 id;
    s32 unitKind;

    args = *(s32**)((s32)event + 0x18);
    battleWork = _battleWorkPointer;
    id = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(battleWork, BattleTransID(event, id));

    if (unit != 0) {
        unitKind = *(s32*)((s32)unit + 8);
        if (unitKind == 0xDE) {
            BattleStatusWindowSystemOn();
        } else if (unitKind >= 0xE0 && unitKind < 0xE7) {
            BattleStatusWindowSystemOn();
        }
    }

    return 2;
}
#pragma no_register_save_helpers off
#pragma use_lmw_stmw on


s32 _rank_up(int param_1) {
    return 0;
}


s32 _full_ap_recover(int param_1) {
    return 0;
}
