#include "unit/unit_koura.h"
#include "battle/battle_unit.h"

extern u32 btldefaultevt_Damage[];
extern s32 btlevtcmd_AddPartsDispOffset();
extern s32 btlevtcmd_AddPartsRotate();
extern s32 btlevtcmd_AddPartsRotateOffset();
extern s32 btlevtcmd_AddRotate();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 btlevtcmd_AnimeChangePoseType();
extern s32 btlevtcmd_AnimeSetPoseTable();
extern s32 btlevtcmd_CheckPhase();
extern s32 btlevtcmd_GetHp();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetUnitWork();
extern s32 btlevtcmd_KillUnit();
extern s32 btlevtcmd_OffAttribute();
extern s32 btlevtcmd_OffPartsAttribute();
extern s32 btlevtcmd_OnAttribute();
extern s32 btlevtcmd_OnPartsAttribute();
extern s32 btlevtcmd_PhaseEventStartDeclare();
extern s32 btlevtcmd_SetBaseScale();
extern s32 btlevtcmd_SetEventAttack();
extern s32 btlevtcmd_SetEventDamage();
extern s32 btlevtcmd_SetEventPhase();
extern s32 btlevtcmd_SetEventWait();
extern s32 btlevtcmd_SetMaxMoveCount();
extern s32 btlevtcmd_SetPartsRotateOffset();
extern s32 btlevtcmd_SetRotate();
extern s32 btlevtcmd_SetRotateOffset();
extern s32 btlevtcmd_SetUnitWork();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_snd_se();
extern s32 evt_eff();

__declspec(section ".rodata") const char str_btl_un_koura_802f9c90[] = "btl_un_koura";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE1_802f9ca0[] = "SFX_BTL_DAMAGE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_FIRE1_802f9cb0[] = "SFX_BTL_DAMAGE_FIRE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_ICE1_802f9cc8[] = "SFX_BTL_DAMAGE_ICE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_BIRIB_802f9cdc[] = "SFX_BTL_DAMAGE_BIRIBIRI1";
__declspec(section ".rodata") const char str_PKO_S_1_802f9cf8[] = "PKO_S_1";
__declspec(section ".rodata") const char str_PKO_D_1_802f9d00[] = "PKO_D_1";
__declspec(section ".rodata") const char str_PKO_Z_1_802f9d08[] = "PKO_Z_1";
__declspec(section ".rodata") const char str_PKO_S_2_802f9d10[] = "PKO_S_2";
__declspec(section ".rodata") const char str_PKO_D_2_802f9d18[] = "PKO_D_2";
__declspec(section ".rodata") const char str_PKO_Z_2_802f9d20[] = "PKO_Z_2";
__declspec(section ".rodata") const char str_PKO_S_3_802f9d28[] = "PKO_S_3";
__declspec(section ".rodata") const char str_PKO_D_3_802f9d30[] = "PKO_D_3";
__declspec(section ".rodata") const char str_PKO_Z_3_802f9d38[] = "PKO_Z_3";
__declspec(section ".rodata") const char str_c_koura_802f9d40[] = "c_koura";
__declspec(section ".rodata") const char str_SFX_BTL_NOKO_GUARD3_802f9d48[] = "SFX_BTL_NOKO_GUARD3";
__declspec(section ".rodata") const char str_SFX_BTL_NOKO_GUARD4_802f9d5c[] = "SFX_BTL_NOKO_GUARD4";
__declspec(section ".rodata") const char str_PKO_H_1_802f9d70[] = "PKO_H_1";
__declspec(section ".rodata") const char str_PKO_H_2_802f9d78[] = "PKO_H_2";
__declspec(section ".rodata") const char str_kemuri_test_802f9d80[] = "kemuri_test";

__declspec(section ".sdata2") const u8 zero_80424940 = 0;

static u8 defence[5] = {0};
static u8 defence_attr[5] = {0};
static u8 regist[22] = {0};

static BattlePoseEntry pose_table[5] = {
    {0x1C, str_PKO_S_1_802f9cf8},
    {0x1F, str_PKO_S_1_802f9cf8},
    {0x27, str_PKO_D_1_802f9d00},
    {0x44, str_PKO_Z_1_802f9d08},
    {0x45, str_PKO_S_1_802f9cf8},
};

BattlePoseEntry pose_table_crack_lv1[5] = {
    {0x1C, str_PKO_S_2_802f9d10},
    {0x1F, str_PKO_S_2_802f9d10},
    {0x27, str_PKO_D_2_802f9d18},
    {0x44, str_PKO_Z_2_802f9d20},
    {0x45, str_PKO_S_2_802f9d10},
};

BattlePoseEntry pose_table_crack_lv2[5] = {
    {0x1C, str_PKO_S_3_802f9d28},
    {0x1F, str_PKO_S_3_802f9d28},
    {0x27, str_PKO_D_3_802f9d30},
    {0x44, str_PKO_Z_3_802f9d38},
    {0x45, str_PKO_S_3_802f9d28},
};

__declspec(section ".sdata") static u32 damage_event[10] = {
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032,
    0xFE363C8B, 0x00000001, 0x0001005E, (u32)btldefaultevt_Damage,
    0x00000002, 0x00000001
};

__declspec(section ".sdata") static u32 wait_event[6] = {
    0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001,
    0x00000002, 0x00000001
};

__declspec(section ".sdata") static u32 attack_event[2] = {
    0x00000002, 0x00000001
};

static u32 data_table[14];
static u32 parts[38];
static u32 init_event[46];
static u32 phase_event[38];
extern u32 koura_open[72];
extern u32 koura_close[79];
extern u32 damage_core[92];
extern u32 pose_tbl_reset[58];
static u32 dead_event[243];
extern u32 destroy_event[47];

__declspec(section ".data") u32 unit_koura[49] = {
    0x000000DF, (u32)(char*)str_btl_un_koura_802f9c90, 0x00080000, 0x05010100,
    0x00000064, 0x00090018, 0x00180018, 0x00180000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x41C80000,
    0x00000000, 0x00000000, 0x41C80000, 0x4179999A,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_BTL_DAMAGE1_802f9ca0,
    (u32)(char*)str_SFX_BTL_DAMAGE_FIRE1_802f9cb0, (u32)(char*)str_SFX_BTL_DAMAGE_ICE1_802f9cc8, (u32)(char*)str_SFX_BTL_DAMAGE_BIRIB_802f9cdc, 0x10000000,
    (u32)regist, 0x02000000, (u32)parts, (u32)init_event,
    (u32)data_table
};

__declspec(section ".data") static u32 data_table[14] = {
    0x0000003B, (u32)koura_open, 0x0000003C, (u32)koura_close,
    0x00000009, (u32)damage_core, 0x0000003D, (u32)pose_tbl_reset,
    0x0000003E, (u32)destroy_event, 0x00000031, (u32)dead_event,
    0x00000000, 0x00000000
};

__declspec(section ".data") static u32 parts[38] = {
    0x00000001, (u32)(char*)str_btl_un_koura_802f9c90, (u32)(char*)str_c_koura_802f9d40, 0x00000000,
    0x00000000, 0x00000000, 0x41A00000, 0x42200000,
    0x00000000, 0x00000000, 0x42200000, 0x00000000,
    0x0014001E, 0x00FF0000, (u32)defence, (u32)defence_attr,
    0x00080009, 0x00000000, (u32)pose_table, 0x00000002,
    (u32)(char*)str_btl_un_koura_802f9c90, (u32)(char*)str_c_koura_802f9d40, 0x00000000, 0x00000000,
    0x00000000, 0x41700000, 0x41C80000, 0x00000000,
    0x00000000, 0x41F00000, 0x00000000, 0x0014001E,
    0x00FF0000, (u32)defence, (u32)defence_attr, 0x03010000,
    0x00000000, (u32)pose_table
};

__declspec(section ".data") static u32 init_event[46] = {
    0x0005005B, (u32)btlevtcmd_SetBaseScale, 0xFFFFFFFE, 0xF24A8080,
    0xF24A8080, 0xF24A8080, 0x0003005B, (u32)btlevtcmd_SetEventWait,
    0xFFFFFFFE, (u32)wait_event, 0x0003005B, (u32)btlevtcmd_SetEventPhase,
    0xFFFFFFFE, (u32)phase_event, 0x0003005B, (u32)btlevtcmd_SetEventAttack,
    0xFFFFFFFE, (u32)attack_event, 0x0003005B, (u32)btlevtcmd_SetEventDamage,
    0xFFFFFFFE, (u32)damage_event, 0x0003005B, (u32)btlevtcmd_SetMaxMoveCount,
    0xFFFFFFFE, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000001, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002, 0x00000000,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003,
    0x00000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE,
    0x00000002, 0x00000001
};

__declspec(section ".data") static u32 phase_event[38] = {
    0x0003005B, (u32)btlevtcmd_CheckPhase, 0xFE363C80, 0x04000001,
    0x00020019, 0xFE363C80, 0x00000000, 0x0003005B,
    (u32)btlevtcmd_GetHp, 0xFFFFFFFE, 0xFE363C80, 0x0002001B,
    0xFE363C80, 0x00000000, 0x0001005E, (u32)koura_open,
    0x00000021, 0x00000021, 0x0003005B, (u32)btlevtcmd_CheckPhase,
    0xFE363C80, 0x04000003, 0x00020019, 0xFE363C80,
    0x00000000, 0x0003005B, (u32)btlevtcmd_GetHp, 0xFFFFFFFE,
    0xFE363C80, 0x0002001B, 0xFE363C80, 0x00000000,
    0x0001005E, (u32)koura_close, 0x00000021, 0x00000021,
    0x00000002, 0x00000001
};

__declspec(section ".data") u32 koura_open[72] = {
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000001,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000001,
    0x00000002, 0x00000021, 0x0002005B, (u32)btlevtcmd_PhaseEventStartDeclare,
    0xFFFFFFFE, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE,
    0xFFFFFFE9, 0x00000000, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x00000000, 0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE,
    0x00000000, 0xFE363C83, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute,
    0xFE363C83, 0x00000001, 0x01000000, 0x00010005,
    0x0000001E, 0x0005005B, (u32)btlevtcmd_AddRotate, 0xFFFFFFFE,
    0x00000000, 0x00000000, 0x00000002, 0x00010009,
    0x00000001, 0x00000006, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000001, 0x00000001, 0x0003005B,
    (u32)btlevtcmd_OffAttribute, 0xFE363C83, 0x00000040, 0x0004005B,
    (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000001, 0x00010000,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000002,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE,
    0x00000001, 0x00000044, 0x00000002, 0x00000001
};

__declspec(section ".data") u32 koura_close[79] = {
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000001,
    0xFE363C80, 0x00020018, 0xFE363C80, 0x00000000,
    0x00000002, 0x00000021, 0x0002005B, (u32)btlevtcmd_PhaseEventStartDeclare,
    0xFFFFFFFE, 0x0005005B, (u32)btlevtcmd_SetRotateOffset, 0xFFFFFFFE,
    0xFFFFFFE9, 0x00000000, 0x00000000, 0x0005005B,
    (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0x0000003C, 0x00010005, 0x0000001E, 0x0005005B,
    (u32)btlevtcmd_AddRotate, 0xFFFFFFFE, 0x00000000, 0x00000000,
    0xFFFFFFFE, 0x00010009, 0x00000001, 0x00000006,
    0x0005005B, (u32)btlevtcmd_SetRotate, 0xFFFFFFFE, 0x00000000,
    0x00000000, 0x00000000, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000001, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000, 0xFE363C83,
    0x0003005B, (u32)btlevtcmd_OnAttribute, 0xFE363C83, 0x00000040,
    0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000001,
    0x00010000, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFE363C83,
    0x00000001, 0x01000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000002, 0x00000000, 0x0002005B, (u32)btlevtcmd_StartWaitEvent,
    0xFFFFFFFE, 0x00000002, 0x00000001
};

__declspec(section ".data") u32 damage_core[92] = {
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000003,
    0xFE363C81, 0x0003005B, (u32)btlevtcmd_GetHp, 0xFFFFFFFE,
    0xFE363C80, 0x00010022, 0xFE363C80, 0x00010028,
    0x00000003, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE,
    0x00000001, (u32)pose_table_crack_lv2, 0x00020019, 0xFE363C81,
    0x00000002, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_NOKO_GUARD3_802f9d48, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x00000021, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000003, 0x00000002, 0x00010028, 0x00000006,
    0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0x00000001,
    (u32)pose_table_crack_lv1, 0x00020019, 0xFE363C81, 0x00000001,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_NOKO_GUARD3_802f9d48,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x00000021,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003,
    0x00000001, 0x0000002A, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable,
    0xFFFFFFFE, 0x00000001, (u32)pose_table, 0x00020019,
    0xFE363C81, 0x00000000, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_NOKO_GUARD3_802f9d48, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x00000021, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000003, 0x00000000, 0x00000031,
    0x0004005B, (u32)btlevtcmd_AnimeChangePoseType, 0xFFFFFFFE, 0x00000001,
    0x00000027, 0x0001000A, 0x000001F4, 0x0002005B,
    (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002, 0x00000001
};

__declspec(section ".data") u32 pose_tbl_reset[58] = {
    0x0003005B, (u32)btlevtcmd_GetHp, 0xFFFFFFFE, 0xFE363C80,
    0x00010022, 0xFE363C80, 0x00010028, 0x00000003,
    0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0x00000001,
    (u32)pose_table_crack_lv2, 0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE,
    (u32)(char*)str_SFX_BTL_NOKO_GUARD3_802f9d48, 0xF1194D80, 0x00000000, 0xF1194D80,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003,
    0x00000002, 0x00010028, 0x00000006, 0x0004005B,
    (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0x00000001, (u32)pose_table_crack_lv1,
    0x0006005B, (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_NOKO_GUARD3_802f9d48,
    0xF1194D80, 0x00000000, 0xF1194D80, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000003, 0x00000001,
    0x0000002A, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE,
    0x00000001, (u32)pose_table, 0x0004005B, (u32)btlevtcmd_SetUnitWork,
    0xFFFFFFFE, 0x00000003, 0x00000000, 0x00000031,
    0x00000002, 0x00000001
};

__declspec(section ".data") static u32 dead_event[243] = {
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000,
    0xFE363C83, 0x0003005B, (u32)btlevtcmd_OffAttribute, 0xFE363C83,
    0x00000060, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFE363C83,
    0x00000001, 0x01000000, 0x0006005B, (u32)btlevtcmd_snd_se,
    0xFFFFFFFE, (u32)(char*)str_SFX_BTL_NOKO_GUARD4_802f9d5c, 0xF1194D80, 0x00000000,
    0xF1194D80, 0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE,
    0x00000001, (u32)(char*)str_PKO_H_1_802f9d70, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000002, (u32)(char*)str_PKO_H_2_802f9d78, 0x0004005B,
    (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000002, 0x01000000,
    0x0006005B, (u32)btlevtcmd_SetPartsRotateOffset, 0xFFFFFFFE, 0x00000001,
    0x00000017, 0x00000000, 0x00000000, 0x0006005B,
    (u32)btlevtcmd_SetPartsRotateOffset, 0xFFFFFFFE, 0x00000002, 0xFFFFFFE9,
    0x00000000, 0x00000000, 0x0000006E, 0x00010005,
    0x00000005, 0x0006005B, (u32)btlevtcmd_AddPartsDispOffset, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x00000001, 0x00000000,
    0x0006005B, (u32)btlevtcmd_AddPartsDispOffset, 0xFFFFFFFE, 0x00000002,
    0x00000000, 0x00000001, 0x00000000, 0x00010009,
    0x00000002, 0x00000006, 0x00010005, 0x00000006,
    0x0006005B, (u32)btlevtcmd_AddPartsDispOffset, 0xFFFFFFFE, 0x00000001,
    0x00000000, 0x00000001, 0x00000000, 0x0006005B,
    (u32)btlevtcmd_AddPartsDispOffset, 0xFFFFFFFE, 0x00000002, 0x00000000,
    0x00000001, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x00010005, 0x00000002, 0x0006005B,
    (u32)btlevtcmd_AddPartsDispOffset, 0xFFFFFFFE, 0x00000001, 0x00000000,
    0x00000001, 0x00000000, 0x0006005B, (u32)btlevtcmd_AddPartsDispOffset,
    0xFFFFFFFE, 0x00000002, 0x00000000, 0x00000001,
    0x00000000, 0x00010009, 0x00000002, 0x00000006,
    0x00000070, 0x0000006E, 0x00010005, 0x0000000A,
    0x0006005B, (u32)btlevtcmd_AddPartsDispOffset, 0xFFFFFFFE, 0x00000001,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0x0006005B,
    (u32)btlevtcmd_AddPartsDispOffset, 0xFFFFFFFE, 0x00000002, 0x00000001,
    0x00000000, 0x00000000, 0x00010009, 0x00000001,
    0x00000006, 0x00010005, 0x00000005, 0x0006005B,
    (u32)btlevtcmd_AddPartsDispOffset, 0xFFFFFFFE, 0x00000001, 0xFFFFFFFF,
    0x00000000, 0x00000000, 0x0006005B, (u32)btlevtcmd_AddPartsDispOffset,
    0xFFFFFFFE, 0x00000002, 0x00000001, 0x00000000,
    0x00000000, 0x00010009, 0x00000002, 0x00000006,
    0x00000070, 0x0000006E, 0x00010005, 0x0000000A,
    0x0006005B, (u32)btlevtcmd_AddPartsRotateOffset, 0xFFFFFFFE, 0x00000001,
    0x00000000, 0x00000001, 0x00000000, 0x0006005B,
    (u32)btlevtcmd_AddPartsRotateOffset, 0xFFFFFFFE, 0x00000002, 0x00000000,
    0x00000001, 0x00000000, 0x00010009, 0x00000002,
    0x00000006, 0x00000070, 0x0000006E, 0x00010005,
    0x0000000E, 0x0006005B, (u32)btlevtcmd_AddPartsRotateOffset, 0xFFFFFFFE,
    0x00000001, 0xFFFFFFFF, 0x00000000, 0x00000000,
    0x0006005B, (u32)btlevtcmd_AddPartsRotateOffset, 0xFFFFFFFE, 0x00000002,
    0x00000001, 0x00000000, 0x00000000, 0x00010009,
    0x00000001, 0x00000006, 0x00000070, 0x00010005,
    0x00000014, 0x0006005B, (u32)btlevtcmd_AddPartsRotate, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x00000000, 0x00000002,
    0x0006005B, (u32)btlevtcmd_AddPartsRotate, 0xFFFFFFFE, 0x00000002,
    0x00000000, 0x00000000, 0xFFFFFFFE, 0x00010009,
    0x00000001, 0x00000006, 0x00010005, 0x00000008,
    0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE, 0x00000001,
    0x01000000, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFFFFFFFE,
    0x00000002, 0x01000000, 0x00010009, 0x00000002,
    0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000001,
    0x01000000, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE,
    0x00000002, 0x01000000, 0x00010009, 0x00000002,
    0x00000006, 0x0003005B, (u32)btlevtcmd_KillUnit, 0xFFFFFFFE,
    0x00000000, 0x00000002, 0x00000001
};

__declspec(section ".data") u32 destroy_event[47] = {
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000,
    0xFE363C83, 0x0003005B, (u32)btlevtcmd_OffAttribute, 0xFE363C83,
    0x00000060, 0x0004005B, (u32)btlevtcmd_OffPartsAttribute, 0xFE363C83,
    0x00000001, 0x01000000, 0x0004005B, (u32)btlevtcmd_OnPartsAttribute,
    0xFFFFFFFE, 0x00000001, 0x01000000, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x000F005B, (u32)evt_eff, (u32)(u8*)&zero_80424940,
    (u32)(char*)str_kemuri_test_802f9d80, 0x00000002, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0xF24A7E80, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x0003005B, (u32)btlevtcmd_KillUnit, 0xFFFFFFFE,
    0x00000000, 0x00000002, 0x00000001
};

__declspec(section ".data") f32 lbl_8039C840[2] = {-1.0f, 1.0f};
