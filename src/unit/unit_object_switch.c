#include "unit/unit_object_switch.h"
#include "battle/battle_stage_object.h"

extern void* _battleWorkPointer;

extern s32 btlevtcmd_SetEventWait();
extern s32 btlevtcmd_SetEventAttack();
extern s32 btlevtcmd_SetEventDamage();
extern s32 btlevtcmd_SetEventConfusion();
extern u32 btldefaultevt_Confuse[];
extern s32 btlevtcmd_SetMaxMoveCount();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_CheckDamageCode();
extern s32 evt_map_playanim();

s32 _rumble_switch(void* event, s32 reset);

extern const char str_btl_un_switch_802fa7f8[];
extern const char str_SFX_BTL_DAMAGE1_802fa808[];
extern const char str_SFX_BTL_DAMAGE_FIRE1_802fa818[];
extern const char str_SFX_BTL_DAMAGE_ICE1_802fa830[];
extern const char str_SFX_BTL_DAMAGE_BIRIB_802fa844[];
extern const char str_b_mario_802fa860[];
extern const char str_switch_a_802fa868[];

extern s32 evtGetValue(void* event, s32 value);
extern s32 BattleTransID(void* event, s32 id);
extern void* BattleGetUnitPtr(void* battleWork, s32 id);

__declspec(section ".sdata2") const char str_M_S_1_80424b08[6] __attribute__ ((aligned(4))) = "M_S_1";
__declspec(section ".sdata2") const char str_M_O_1_80424b10[6] __attribute__ ((aligned(4))) = "M_O_1";

__declspec(section ".sdata") static u8 defence[5] __attribute__ ((aligned(4))) = {
    0x00, 0x00, 0x00, 0x00, 0x00,
};

__declspec(section ".sdata") static u8 defence_attr[5] __attribute__ ((aligned(4))) = {
    0x00, 0x00, 0x00, 0x00, 0x00,
};

__declspec(section ".sdata") static u8 regist[22] __attribute__ ((aligned(4))) = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

__declspec(section ".sdata") static u32 pose_table[8] __attribute__ ((aligned(4))) = {
    0x0000001C, (u32)(char*)str_M_S_1_80424b08, 0x0000001F, (u32)(char*)str_M_S_1_80424b08,
    0x00000027, (u32)(char*)str_M_O_1_80424b10, 0x00000045, (u32)(char*)str_M_S_1_80424b08,
};

__declspec(section ".sdata") static u32 data_table[2] __attribute__ ((aligned(4))) = {
    0x00000000, 0x00000000,
};

__declspec(section ".sdata") static u32 attack_event[5] __attribute__ ((aligned(4))) = {
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

__declspec(section ".sdata") static u32 wait_event[2] __attribute__ ((aligned(4))) = {
    0x00000002, 0x00000001,
};

static u32 parts[19];
static u32 init_event[25];
static u32 damage_event[22];

__declspec(section ".data") u32 unit_object_switch[49] = {
    0x000000D9, (u32)(char*)str_btl_un_switch_802fa7f8, 0x03E70000, 0x01010200,
    0x00000064, 0x00090018, 0x00180000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x41400000,
    0x00000000, 0x00000000, 0x41400000, 0x4179999A,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_BTL_DAMAGE1_802fa808,
    (u32)(char*)str_SFX_BTL_DAMAGE_FIRE1_802fa818, (u32)(char*)str_SFX_BTL_DAMAGE_ICE1_802fa830, (u32)(char*)str_SFX_BTL_DAMAGE_BIRIB_802fa844, 0x12003001,
    (u32)regist, 0x01000000, (u32)parts, (u32)init_event,
    (u32)data_table,
};

__declspec(section ".data") static u32 parts[19] = {
    0x00000001, (u32)(char*)str_btl_un_switch_802fa7f8, (u32)(char*)str_b_mario_802fa860, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x41C80000,
    0x00000000, 0x00000000, 0x41F00000, 0x00000000,
    0x0014001E, 0x00FF0000, (u32)defence, (u32)defence_attr,
    0x03000009, 0x00000000, (u32)pose_table,
};

__declspec(section ".data") static u32 init_event[25] = {
    0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event,
    0x0003005B, (u32)btlevtcmd_SetEventAttack, 0xFFFFFFFE, (u32)attack_event,
    0x0003005B, (u32)btlevtcmd_SetEventDamage, 0xFFFFFFFE, (u32)damage_event,
    0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse,
    0x0003005B, (u32)btlevtcmd_SetMaxMoveCount, 0xFFFFFFFE, 0x00000000,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001,
};

__declspec(section ".data") static u32 damage_event[22] = {
    0x0004005B, (u32)btlevtcmd_CheckDamageCode, 0xFFFFFFFE, 0x00000100,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x0002005B, (u32)_rumble_switch, 0xFFFFFFFE, 0x00000002,
    0x00000021, 0x0004005B, (u32)evt_map_playanim, (u32)(char*)str_switch_a_802fa868,
    0x00000000, 0x00000000, 0x0001000A, 0x000001F4,
    0x00000002, 0x00000001,
};

static s32 _data[] = {
    -5, 0, 15, 0, -15, 0, 14, 0, -13, 0, 12, 0, -11, 0, 10, 0, -9, 0,
    8, 0, -7, 0, 6, 0, -6, 0, 5, 0, -4, 0, 2, 0, -2, 0, 0
};

__declspec(section ".rodata") const char str_btl_un_switch_802fa7f8[14] __attribute__ ((aligned(4))) = "btl_un_switch";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE1_802fa808[16] __attribute__ ((aligned(4))) = "SFX_BTL_DAMAGE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_FIRE1_802fa818[21] __attribute__ ((aligned(4))) = "SFX_BTL_DAMAGE_FIRE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_ICE1_802fa830[20] __attribute__ ((aligned(4))) = "SFX_BTL_DAMAGE_ICE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_BIRIB_802fa844[25] __attribute__ ((aligned(4))) = "SFX_BTL_DAMAGE_BIRIBIRI1";
__declspec(section ".rodata") const char str_b_mario_802fa860[8] __attribute__ ((aligned(4))) = "b_mario";
__declspec(section ".rodata") const char str_switch_a_802fa868[9] __attribute__ ((aligned(4))) = "switch_a";

#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
s32 _rumble_switch(void* event, s32 reset) {
    void* unit;
    void* object;
    s32* args;
    s32 counter;
    s32 value;

    args = *(s32**)((s32)event + 0x18);
    value = evtGetValue(event, args[0]);
    unit = BattleGetUnitPtr(_battleWorkPointer, BattleTransID(event, value));
    if (unit == NULL) {
        return 2;
    }

    object = BattleGetObjectPtr(*(s32*)((s32)unit + 0xB30));
    if (reset != 0) {
        *(s32*)((s32)event + 0x78) = 0;
    }

    counter = *(s32*)((s32)event + 0x78);
    *(f32*)((s32)object + 8) += _data[counter];
    *(s32*)((s32)event + 0x78) += 1;
    if (*(u32*)((s32)event + 0x78) >= 0x22) {
        return 2;
    }
    return 0;
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset
