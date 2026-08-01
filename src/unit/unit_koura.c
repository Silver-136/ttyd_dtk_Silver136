#include "unit/unit_koura.h"
#include "battle/battle_unit.h"

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
__declspec(section ".rodata") const u8 gap_04_802F9D8C_rodata[4] = {0};

__declspec(section ".sdata2") const u8 zero_80424940 = 0;
__declspec(section ".sdata2") const u8 gap_09_80424941_sdata2[7] = {0};

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
