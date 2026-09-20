#include "unit/unit_bomzou.h"
#include "battle/battle_unit.h"

extern u32 btldefaultevt_Confuse[];
extern u32 btldefaultevt_Damage[];
extern s32 btlevtcmd_AfterReactionEntry();
extern s32 btlevtcmd_AnimeChangePose();
extern s32 btlevtcmd_AnimeChangePoseFromTable();
extern s32 btlevtcmd_AnimeSetPoseTable();
extern s32 btlevtcmd_CheckDamage();
extern s32 btlevtcmd_CheckPhase();
extern s32 btlevtcmd_ChoiceSamplingEnemy();
extern s32 btlevtcmd_GetHitPos();
extern s32 btlevtcmd_GetPos();
extern s32 btlevtcmd_GetSelectNextEnemy();
extern s32 btlevtcmd_GetUnitId();
extern s32 btlevtcmd_GetUnitWork();
extern s32 btlevtcmd_KillUnit();
extern s32 btlevtcmd_OnAttribute();
extern s32 btlevtcmd_OnPartsAttribute();
extern s32 btlevtcmd_PhaseEventStartDeclare();
extern s32 btlevtcmd_PreCheckDamage();
extern s32 btlevtcmd_SamplingEnemy();
extern s32 btlevtcmd_SetEventAttack();
extern s32 btlevtcmd_SetEventConfusion();
extern s32 btlevtcmd_SetEventDamage();
extern s32 btlevtcmd_SetEventPhase();
extern s32 btlevtcmd_SetEventWait();
extern s32 btlevtcmd_SetMaxMoveCount();
extern s32 btlevtcmd_SetUnitWork();
extern s32 btlevtcmd_StageDispellFog();
extern s32 btlevtcmd_StartWaitEvent();
extern s32 btlevtcmd_snd_se();
extern s32 evt_btl_camera_shake_h();
extern s32 evt_eff();
extern s32 evt_eff64();
extern s32 evt_snd_sfxoff();
extern s32 evt_sub_random();
extern s32 weaponGetPowerDefault();

static u32 parts[19];
static u32 init_event[70];
static u32 phase_event[67];
static u32 wait_event[57];
static u32 data_table[8];
static u32 damage_event[10];
static u32 attack_event[5];
static u32 dead_event[6];
extern u32 end_battle_evt[19];
extern u32 explosion_event[173];

__declspec(section ".rodata") const char str_btl_un_bomzou_802f9ac0[] = "btl_un_bomzou";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE1_802f9ad0[] = "SFX_BTL_DAMAGE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_FIRE1_802f9ae0[] = "SFX_BTL_DAMAGE_FIRE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_ICE1_802f9af8[] = "SFX_BTL_DAMAGE_ICE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_BIRIB_802f9b0c[] = "SFX_BTL_DAMAGE_BIRIBIRI1";
__declspec(section ".rodata") const char str_c_bakudan_802f9b28[] = "c_bakudan";
__declspec(section ".rodata") const char str_sweat_n64_802f9b34[] = "sweat_n64";
__declspec(section ".rodata") const char str_hibashira_802f9b40[] = "hibashira";
__declspec(section ".rodata") const char str_SFX_BTL_THUNDERS_BOM_802f9b4c[] = "SFX_BTL_THUNDERS_BOMB2";

__declspec(section ".sdata2") const char str_B_2_80424800[4] = "B_2";
__declspec(section ".sdata2") const char str_B_3_80424804[4] = "B_3";
__declspec(section ".sdata2") const char str_B_4_80424808[4] = "B_4";
__declspec(section ".sdata2") const char str_B_1_8042480c[4] = "B_1";
__declspec(section ".sdata2") const u8 zero_80424810 = 0;

static u8 defence[5] = {0};
static u8 defence_attr[5] = {0};
static u8 regist[22] = {0};

BattlePoseEntry pose_table_3[4] = {
    {0x1C, str_B_2_80424800},
    {0x1F, str_B_2_80424800},
    {0x27, str_B_2_80424800},
    {0x45, str_B_2_80424800},
};

BattlePoseEntry pose_table_2[4] = {
    {0x1C, str_B_3_80424804},
    {0x1F, str_B_3_80424804},
    {0x27, str_B_3_80424804},
    {0x45, str_B_3_80424804},
};

BattlePoseEntry pose_table_1[4] = {
    {0x1C, str_B_4_80424808},
    {0x1F, str_B_4_80424808},
    {0x27, str_B_4_80424808},
    {0x45, str_B_4_80424808},
};

__declspec(section ".sdata") static u32 data_table[8] = {
    0x00000031, (u32)dead_event, 0x00000035, (u32)explosion_event,
    0x0000003F, (u32)end_battle_evt, 0x00000000, 0x00000000
};

__declspec(section ".sdata") static u32 damage_event[10] = {
    0x00020032, 0xFE363C8A, 0xFFFFFFFE, 0x00020032,
    0xFE363C8B, 0x00000001, 0x0001005E, (u32)btldefaultevt_Damage,
    0x00000002, 0x00000001
};

__declspec(section ".sdata") static u32 attack_event[5] = {
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000002,
    0x00000001
};

__declspec(section ".sdata") static u32 dead_event[6] = {
    0x0003005B, (u32)btlevtcmd_AfterReactionEntry, 0xFFFFFFFE, 0x00000035,
    0x00000002, 0x00000001
};

__declspec(section ".data") u32 unit_bomzou[49] = {
    0x000000DB, (u32)(char*)str_btl_un_bomzou_802f9ac0, 0x00010000, 0x01010100,
    0x00000064, 0x00090018, 0x00180000, 0x00140000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x41400000,
    0x00000000, 0x00000000, 0x41400000, 0x4179999A,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x0000FF00, 0x64140000,
    0x00000000, 0x00000000, 0x00000000, (u32)(char*)str_SFX_BTL_DAMAGE1_802f9ad0,
    (u32)(char*)str_SFX_BTL_DAMAGE_FIRE1_802f9ae0, (u32)(char*)str_SFX_BTL_DAMAGE_ICE1_802f9af8, (u32)(char*)str_SFX_BTL_DAMAGE_BIRIB_802f9b0c, 0x12400000,
    (u32)regist, 0x01000000, (u32)parts, (u32)init_event,
    (u32)data_table
};

__declspec(section ".data") u32 weapon_bomzou_explosion[48] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x64000000, 0x00000000, 0x00000000, (u32)weaponGetPowerDefault,
    0x00000003, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x02200020, 0x20000000, 0x03000302,
    0x00000000, 0x00020080, 0x000007FF, 0x00000004,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x000A0A64, 0x1E050505, 0x05000000
};

__declspec(section ".data") static u32 parts[19] = {
    0x00000001, (u32)(char*)str_btl_un_bomzou_802f9ac0, (u32)(char*)str_c_bakudan_802f9b28, 0x00000000,
    0x00000000, 0x00000000, 0x41700000, 0x41C80000,
    0x00000000, 0x00000000, 0x41F00000, 0x00000000,
    0x0014001E, 0x00FF0000, (u32)defence, (u32)defence_attr,
    0x00080009, 0x00000000, (u32)pose_table_3
};

__declspec(section ".data") static u32 init_event[70] = {
    0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event,
    0x0003005B, (u32)btlevtcmd_SetEventPhase, 0xFFFFFFFE, (u32)phase_event,
    0x0003005B, (u32)btlevtcmd_SetEventAttack, 0xFFFFFFFE, (u32)attack_event,
    0x0003005B, (u32)btlevtcmd_SetEventDamage, 0xFFFFFFFE, (u32)damage_event,
    0x0003005B, (u32)btlevtcmd_SetEventConfusion, 0xFFFFFFFE, (u32)btldefaultevt_Confuse,
    0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000,
    0x00000002, 0x0004005B, (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE,
    0x00000001, 0x00000000, 0x0003005B, (u32)btlevtcmd_SetMaxMoveCount,
    0xFFFFFFFE, 0x00000000, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x00010022,
    0xFE363C80, 0x00010029, 0x00000003, 0x0004005B,
    (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0x00000001, (u32)pose_table_3,
    0x00010029, 0x00000002, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable,
    0xFFFFFFFE, 0x00000001, (u32)pose_table_2, 0x00010029,
    0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE,
    0x00000001, (u32)pose_table_1, 0x00000031, 0x0004005B,
    (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001, (u32)(char*)str_B_1_8042480c,
    0x00000002, 0x00000001
};

__declspec(section ".data") static u32 phase_event[67] = {
    0x0003005B, (u32)btlevtcmd_CheckPhase, 0xFE363C80, 0x04000001,
    0x00020019, 0xFE363C80, 0x00000000, 0x0004005B,
    (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000000, 0xFE363C80,
    0x00020036, 0xFE363C80, 0x00000001, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000000, 0xFE363C80,
    0x00010022, 0xFE363C80, 0x00010029, 0x00000003,
    0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0x00000001,
    (u32)pose_table_3, 0x00010029, 0x00000002, 0x0004005B,
    (u32)btlevtcmd_AnimeSetPoseTable, 0xFFFFFFFE, 0x00000001, (u32)pose_table_2,
    0x00010029, 0x00000001, 0x0004005B, (u32)btlevtcmd_AnimeSetPoseTable,
    0xFFFFFFFE, 0x00000001, (u32)pose_table_1, 0x00000031,
    0x0002001C, 0xFE363C80, 0x00000000, 0x0002005B,
    (u32)btlevtcmd_PhaseEventStartDeclare, 0xFFFFFFFE, 0x0004005B, (u32)btlevtcmd_AnimeChangePose,
    0xFFFFFFFE, 0x00000001, (u32)(char*)str_B_2_80424800, 0x00010009,
    0x00000024, 0x0001005E, (u32)explosion_event, 0x00000020,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 0x00000021,
    0x00000021, 0x00000002, 0x00000001
};

__declspec(section ".data") static u32 wait_event[57] = {
    0x0003005B, (u32)btlevtcmd_AnimeChangePoseFromTable, 0xFFFFFFFE, 0x00000001,
    0x00010003, 0x00000000, 0x0004005B, (u32)btlevtcmd_GetUnitWork,
    0xFFFFFFFE, 0x00000000, 0xFE363C80, 0x0002001B,
    0xFE363C80, 0x00000001, 0x00010009, 0x00000001,
    0x00010004, 0x00000000, 0x00000021, 0x0005005B,
    (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80, 0xFE363C81,
    0xFE363C82, 0x00020035, 0xFE363C81, 0x0000000A,
    0x000F005B, (u32)evt_eff64, 0x00000000, (u32)(char*)str_sweat_n64_802f9b34,
    0x00000001, 0xFE363C80, 0xFE363C81, 0xFE363C82,
    0x0000000F, 0xFFFFFFD3, 0x0000000F, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x0003005B, (u32)evt_sub_random, 0x0000002D, 0xFE363C80,
    0x00020035, 0xFE363C80, 0x0000002D, 0x00010009,
    0xFE363C80, 0x00010004, 0x00000000, 0x00000002,
    0x00000001
};

__declspec(section ".data") u32 end_battle_evt[19] = {
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000001,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x00000000,
    0x00000021, 0x00000002, 0x00000001
};

__declspec(section ".data") u32 explosion_event[173] = {
    0x0004005B, (u32)btlevtcmd_GetUnitWork, 0xFFFFFFFE, 0x00000001,
    0xFE363C80, 0x00020019, 0xFE363C80, 0x00000000,
    0x0002005B, (u32)evt_snd_sfxoff, 0xFE363C80, 0x0004005B,
    (u32)btlevtcmd_SetUnitWork, 0xFFFFFFFE, 0x00000001, 0x00000000,
    0x00000021, 0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0x000F005B,
    (u32)evt_eff, (u32)(u8*)&zero_80424810, (u32)(char*)str_hibashira_802f9b40, 0x00000002,
    0xFE363C80, 0xFE363C81, 0xFE363C82, 0xFE363C80,
    0xFE363C81, 0xFE363C82, 0x00000001, 0xF24A7E80,
    0x0000003C, 0x00000000, 0x00000000, 0x0006005B,
    (u32)btlevtcmd_snd_se, 0xFFFFFFFE, (u32)(char*)str_SFX_BTL_THUNDERS_BOM_802f9b4c, 0xF1194D80,
    0x00000000, 0xF1194D80, 0x0001005B, (u32)btlevtcmd_StageDispellFog,
    0x0003005B, (u32)btlevtcmd_OnAttribute, 0xFFFFFFFE, 0x01000000,
    0x0004005B, (u32)btlevtcmd_OnPartsAttribute, 0xFFFFFFFE, 0x00000001,
    0x03000000, 0x0006005B, (u32)evt_btl_camera_shake_h, 0x00000000,
    0x0000000A, 0x00000000, 0x0000003C, 0x00000004,
    0x0004005B, (u32)btlevtcmd_AnimeChangePose, 0xFFFFFFFE, 0x00000001,
    (u32)(char*)str_B_2_80424800, 0x0004005B, (u32)btlevtcmd_SamplingEnemy, 0xFFFFFFFE,
    0x00000001, (u32)weapon_bomzou_explosion, 0x0004005B, (u32)btlevtcmd_ChoiceSamplingEnemy,
    (u32)weapon_bomzou_explosion, 0xFE363C83, 0xFE363C84, 0x00020018,
    0xFE363C83, 0xFFFFFFFF, 0x00010004, 0x00000063,
    0x00000021, 0x00020032, 0xFE363C8A, 0x00000000,
    0x00010003, 0x0000000A, 0x0003005B, (u32)btlevtcmd_GetUnitId,
    0xFFFFFFFE, 0xFE363C80, 0x00020018, 0xFE363C80,
    0xFE363C83, 0x00010004, 0x00000050, 0x00000021,
    0x0005005B, (u32)btlevtcmd_GetPos, 0xFFFFFFFE, 0xFE363C80,
    0xF1194D80, 0xF1194D80, 0x0006005B, (u32)btlevtcmd_GetHitPos,
    0xFE363C83, 0xFE363C84, 0xFE363C81, 0xF1194D80,
    0xF1194D80, 0x00020036, 0xFE363C80, 0xFE363C81,
    0x0002001C, 0xFE363C80, 0xFFFFFFFF, 0x00020037,
    0xFE363C80, 0xFFFFFFFF, 0x00000021, 0x0002001B,
    0xFE363C80, 0x00000032, 0x00010004, 0x00000050,
    0x00000021, 0x0007005B, (u32)btlevtcmd_PreCheckDamage, 0xFFFFFFFE,
    0xFE363C83, 0xFE363C84, (u32)weapon_bomzou_explosion, 0x00000100,
    0xFE363C85, 0x00020018, 0xFE363C85, 0x00000001,
    0x0007005B, (u32)btlevtcmd_CheckDamage, 0xFFFFFFFE, 0xFE363C83,
    0xFE363C84, (u32)weapon_bomzou_explosion, 0x00000100, 0xFE363C85,
    0x00000021, 0x00010003, 0x00000050, 0x0003005B,
    (u32)btlevtcmd_GetSelectNextEnemy, 0xFE363C83, 0xFE363C84, 0x00020019,
    0xFE363C83, 0xFFFFFFFF, 0x00020035, 0xFE363C8A,
    0x00000001, 0x00010004, 0x0000000A, 0x00000021,
    0x00010003, 0x00000063, 0x0003005B, (u32)btlevtcmd_KillUnit,
    0xFFFFFFFE, 0x00000000, 0x00000002, 0x00000001,
    0x00000000
};

__declspec(section ".data") f32 lbl_8039B860[2] = {-1.0f, 1.0f};
