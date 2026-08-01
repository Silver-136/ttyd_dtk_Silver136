#include "unit/unit_bomzou.h"
#include "battle/battle_unit.h"

__declspec(section ".rodata") const char str_btl_un_bomzou_802f9ac0[] = "btl_un_bomzou";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE1_802f9ad0[] = "SFX_BTL_DAMAGE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_FIRE1_802f9ae0[] = "SFX_BTL_DAMAGE_FIRE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_ICE1_802f9af8[] = "SFX_BTL_DAMAGE_ICE1";
__declspec(section ".rodata") const char str_SFX_BTL_DAMAGE_BIRIB_802f9b0c[] = "SFX_BTL_DAMAGE_BIRIBIRI1";
__declspec(section ".rodata") const char str_c_bakudan_802f9b28[] = "c_bakudan";
__declspec(section ".rodata") const char str_sweat_n64_802f9b34[] = "sweat_n64";
__declspec(section ".rodata") const char str_hibashira_802f9b40[] = "hibashira";
__declspec(section ".rodata") const char str_SFX_BTL_THUNDERS_BOM_802f9b4c[] = "SFX_BTL_THUNDERS_BOMB2";
__declspec(section ".rodata") const u8 gap_04_802F9B63_rodata[5] = {0};

__declspec(section ".sdata2") const char str_B_2_80424800[4] = "B_2";
__declspec(section ".sdata2") const char str_B_3_80424804[4] = "B_3";
__declspec(section ".sdata2") const char str_B_4_80424808[4] = "B_4";
__declspec(section ".sdata2") const char str_B_1_8042480c[4] = "B_1";
__declspec(section ".sdata2") const u8 zero_80424810 = 0;
__declspec(section ".sdata2") const u8 gap_09_80424811_sdata2[7] = {0};

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
