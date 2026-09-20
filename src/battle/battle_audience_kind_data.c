#include "battle/battle_audience_kind_data.h"

typedef struct AudienceItemWeightLocal {
    u32 item_id;
    u32 weight;
} AudienceItemWeightLocal;

typedef struct AudienceKindDataLocal {
    u32 flags;
    u32** anim_table;
    AudienceItemWeightLocal* item_table;
    f32 field_0c;
    f32 field_10;
    f32 field_14;
    f32 field_18;
    f32 field_1c;
} AudienceKindDataLocal;

/* Audience animation command streams stored in .data. */
__declspec(section ".data") u32 audience_kinopio_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000008,
    0x00000004, 0xFFFFFFFE, 0x00000009, 0x00000004,
    0xFFFFFFFE, 0x0000000A, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_daisan_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000018,
    0x00000004, 0xFFFFFFFE, 0x00000019, 0x00000004,
    0xFFFFFFFE, 0x0000001A, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_teresa_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000028,
    0x00000004, 0xFFFFFFFE, 0x00000029, 0x00000004,
    0xFFFFFFFE, 0x0000002A, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_hammer_bros_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000003A,
    0x00000004, 0xFFFFFFFE, 0x0000003B, 0x00000004,
    0xFFFFFFFE, 0x0000003C, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_honenoko_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000004A,
    0x00000004, 0xFFFFFFFE, 0x0000004B, 0x00000004,
    0xFFFFFFFE, 0x0000004C, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_heiho_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000005A,
    0x00000004, 0xFFFFFFFE, 0x0000005B, 0x00000004,
    0xFFFFFFFE, 0x0000005C, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_pansy_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000006C,
    0x00000004, 0xFFFFFFFE, 0x0000006D, 0x00000004,
    0xFFFFFFFE, 0x0000006E, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_puni_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000007E,
    0x00000004, 0xFFFFFFFE, 0x0000007F, 0x00000004,
    0xFFFFFFFE, 0x00000080, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_nokonoko_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000008E,
    0x00000004, 0xFFFFFFFE, 0x0000008F, 0x00000004,
    0xFFFFFFFE, 0x00000090, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_heavy_bomb_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000009F,
    0x00000004, 0xFFFFFFFE, 0x000000A0, 0x00000004,
    0xFFFFFFFE, 0x000000A1, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_kuriboo_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000B3,
    0x00000004, 0xFFFFFFFE, 0x000000B4, 0x00000004,
    0xFFFFFFFE, 0x000000B5, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_pakkun_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000C3,
    0x00000004, 0xFFFFFFFE, 0x000000C4, 0x00000004,
    0xFFFFFFFE, 0x000000C5, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

__declspec(section ".data") u32 audience_luigi_run_front[14] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000008,
    0x00000004, 0xFFFFFFFE, 0x00000009, 0x00000004,
    0xFFFFFFFE, 0x0000000A, 0x00000004, 0xFFFFFFFB,
    0x00000000, 0xFFFFFFFF
};

/* Audience animation command streams stored in .sdata. */
__declspec(section ".sdata") u32 audience_kinopio_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000000,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000001,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000002,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000003,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000004,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000006,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_itemthrow_back[4] = {
    0xFFFFFFFE, 0x00000007, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000B,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_damage_front[4] = {
    0xFFFFFFFE, 0x0000000B, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000C,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000D,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000E,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kinopio_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000F,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000010,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000011,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000012,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000013,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000015,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000016,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_itemthrow_back[4] = {
    0xFFFFFFFE, 0x00000017, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000001B,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_damage_front[4] = {
    0xFFFFFFFE, 0x0000001B, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000001C,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000001D,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000001E,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_daisan_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000001F,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000020,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000021,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000022,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000023,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000024,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000026,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_itemthrow_back[4] = {
    0xFFFFFFFE, 0x00000027, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000002B,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_damage_front[4] = {
    0xFFFFFFFE, 0x0000002B, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000002C,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000002D,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000002E,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_teresa_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000002F,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000032,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000033,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000034,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000035,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000037,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000038,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_itemthrow_back[4] = {
    0xFFFFFFFE, 0x00000039, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000003D,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_damage_front[4] = {
    0xFFFFFFFE, 0x0000003D, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000003E,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000003F,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000040,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_hammer_bros_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000041,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000042,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000043,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000044,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000045,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000047,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000048,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_itemthrow_back[4] = {
    0xFFFFFFFE, 0x00000049, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000004D,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_damage_front[4] = {
    0xFFFFFFFE, 0x0000004D, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000004E,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000004F,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000050,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_honenoko_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000051,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000052,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000053,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000054,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000055,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000057,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000058,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_itemthrow_back[4] = {
    0xFFFFFFFE, 0x00000059, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000005D,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_damage_front[4] = {
    0xFFFFFFFE, 0x0000005D, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000005E,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000005F,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000060,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heiho_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000061,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000064,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000065,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000066,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000067,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000068,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000006A,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_itemthrow_back[4] = {
    0xFFFFFFFE, 0x0000006B, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000006F,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_damage_front[4] = {
    0xFFFFFFFE, 0x0000006F, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000070,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000071,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000072,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000073,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pansy_sing_front[11] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000074,
    0x00000010, 0xFFFFFFFE, 0x00000075, 0x00000010,
    0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000076,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000077,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000078,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000079,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000007B,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000007C,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_itemthrow_back[4] = {
    0xFFFFFFFE, 0x0000007D, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000081,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_damage_front[4] = {
    0xFFFFFFFE, 0x00000081, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000082,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000083,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000084,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_puni_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000085,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000086,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000087,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000088,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000089,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000008A,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000008C,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_itemthrow_back[4] = {
    0xFFFFFFFE, 0x0000008D, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000091,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_damage_front[4] = {
    0xFFFFFFFE, 0x00000091, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000092,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000093,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000094,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000095,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_nokonoko_shell_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000096,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000097,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000098,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000099,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000009A,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000009C,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000009D,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_itemthrow_back[4] = {
    0xFFFFFFFE, 0x0000009E, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000A2,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_damage_front[4] = {
    0xFFFFFFFE, 0x000000A2, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000A3,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000A4,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000A5,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000A6,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_ignite_front[11] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000A7,
    0x00000002, 0xFFFFFFFE, 0x000000A8, 0x00000002,
    0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_heavy_bomb_ignite_back[11] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000A9,
    0x00000002, 0xFFFFFFFE, 0x000000AA, 0x00000002,
    0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000AB,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000AC,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000AD,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000AE,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000AF,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000B1,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_itemthrow_back[4] = {
    0xFFFFFFFE, 0x000000B2, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000B6,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_damage_front[4] = {
    0xFFFFFFFE, 0x000000B6, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000B7,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000B8,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000B9,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_kuriboo_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000BA,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000BB,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000BC,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000BD,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000BE,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000C0,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000C1,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_itemthrow_back[4] = {
    0xFFFFFFFE, 0x000000C2, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000C6,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_damage_front[4] = {
    0xFFFFFFFE, 0x000000C6, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000C7,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000C8,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000C9,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x000000CA,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_pakkun_eat_front[7] = {
    0xFFFFFFFE, 0x000000CB, 0x0000000A, 0xFFFFFFFE,
    0x000000CC, 0x0000000A, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_wait_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000000,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_wait_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000001,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_jump_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000002,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_jump_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000003,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_raise_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000005,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_itemon_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x00000006,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_itemthrow_back[4] = {
    0xFFFFFFFE, 0x00000007, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_tumble_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000B,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_damage_front[4] = {
    0xFFFFFFFE, 0x0000000B, 0x00000020, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_sleep_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000C,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_sleep_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000D,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_confuse_front[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000E,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

__declspec(section ".sdata") u32 audience_luigi_confuse_back[8] = {
    0xFFFFFFFC, 0x00000000, 0xFFFFFFFE, 0x0000000F,
    0x00000008, 0xFFFFFFFB, 0x00000000, 0xFFFFFFFF
};

/* Audience item-weight tables stored in .sdata. */
__declspec(section ".sdata") AudienceItemWeightLocal audience_item_daisan[3] = {
    { 0x000000ED, 0x0000000A },
    { 0x000000EC, 0x0000000A },
    { 0x00000000, 0x00000000 }
};

__declspec(section ".sdata") AudienceItemWeightLocal audience_item_teresa[6] = {
    { 0x00000088, 0x0000000A },
    { 0x00000086, 0x00000014 },
    { 0x000000B1, 0x0000000A },
    { 0x000000ED, 0x0000000A },
    { 0x000000EC, 0x0000000A },
    { 0x00000000, 0x00000000 }
};

__declspec(section ".sdata") AudienceItemWeightLocal audience_item_hammer_bros[2] = {
    { 0x000000EF, 0x0000000A },
    { 0x00000000, 0x00000000 }
};

__declspec(section ".sdata") AudienceItemWeightLocal audience_item_honenoko[4] = {
    { 0x000000EE, 0x0000000A },
    { 0x000000ED, 0x0000000A },
    { 0x000000EC, 0x0000000A },
    { 0x00000000, 0x00000000 }
};

__declspec(section ".sdata") AudienceItemWeightLocal audience_item_pansy[1] = {
    { 0x00000000, 0x00000000 }
};

__declspec(section ".sdata") AudienceItemWeightLocal audience_item_heavy_bomb[1] = {
    { 0x00000000, 0x00000000 }
};

__declspec(section ".sdata") AudienceItemWeightLocal audience_item_kuriboo[3] = {
    { 0x000000ED, 0x00000014 },
    { 0x000000EC, 0x00000014 },
    { 0x00000000, 0x00000000 }
};

__declspec(section ".sdata") AudienceItemWeightLocal audience_item_pakkun[1] = {
    { 0x00000000, 0x00000000 }
};

/* Animation pointer tables. */
__declspec(section ".data") u32* audience_anim_kinopio[19] = {
    audience_kinopio_wait_front, audience_kinopio_wait_back, audience_kinopio_jump_front, audience_kinopio_jump_back,
    audience_kinopio_raise_back, audience_kinopio_itemon_back, audience_kinopio_itemthrow_back, audience_kinopio_run_front,
    audience_kinopio_tumble_front, audience_kinopio_damage_front, audience_kinopio_sleep_front, audience_kinopio_sleep_back,
    audience_kinopio_confuse_front, audience_kinopio_confuse_back, 0, 0,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_daisan[19] = {
    audience_daisan_wait_front, audience_daisan_wait_back, audience_daisan_jump_front, audience_daisan_jump_back,
    audience_daisan_raise_back, audience_daisan_itemon_back, audience_daisan_itemthrow_back, audience_daisan_run_front,
    audience_daisan_tumble_front, audience_daisan_damage_front, audience_daisan_sleep_front, audience_daisan_sleep_back,
    audience_daisan_confuse_front, audience_daisan_confuse_back, 0, 0,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_teresa[19] = {
    audience_teresa_wait_front, audience_teresa_wait_back, audience_teresa_jump_front, audience_teresa_jump_back,
    audience_teresa_raise_back, audience_teresa_itemon_back, audience_teresa_itemthrow_back, audience_teresa_run_front,
    audience_teresa_tumble_front, audience_teresa_damage_front, audience_teresa_sleep_front, audience_teresa_sleep_back,
    audience_teresa_confuse_front, audience_teresa_confuse_back, 0, 0,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_hammer_bros[19] = {
    audience_hammer_bros_wait_front, audience_hammer_bros_wait_back, audience_hammer_bros_jump_front, audience_hammer_bros_jump_back,
    audience_hammer_bros_raise_back, audience_hammer_bros_itemon_back, audience_hammer_bros_itemthrow_back, audience_hammer_bros_run_front,
    audience_hammer_bros_tumble_front, audience_hammer_bros_damage_front, audience_hammer_bros_sleep_front, audience_hammer_bros_sleep_back,
    audience_hammer_bros_confuse_front, audience_hammer_bros_confuse_back, 0, 0,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_honenoko[19] = {
    audience_honenoko_wait_front, audience_honenoko_wait_back, audience_honenoko_jump_front, audience_honenoko_jump_back,
    audience_honenoko_raise_back, audience_honenoko_itemon_back, audience_honenoko_itemthrow_back, audience_honenoko_run_front,
    audience_honenoko_tumble_front, audience_honenoko_damage_front, audience_honenoko_sleep_front, audience_honenoko_sleep_back,
    audience_honenoko_confuse_front, audience_honenoko_confuse_back, 0, 0,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_heiho[19] = {
    audience_heiho_wait_front, audience_heiho_wait_back, audience_heiho_jump_front, audience_heiho_jump_back,
    audience_heiho_raise_back, audience_heiho_itemon_back, audience_heiho_itemthrow_back, audience_heiho_run_front,
    audience_heiho_tumble_front, audience_heiho_damage_front, audience_heiho_sleep_front, audience_heiho_sleep_back,
    audience_heiho_confuse_front, audience_heiho_confuse_back, 0, 0,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_pansy[19] = {
    audience_pansy_wait_front, audience_pansy_wait_back, audience_pansy_jump_front, audience_pansy_jump_back,
    audience_pansy_raise_back, audience_pansy_itemon_back, audience_pansy_itemthrow_back, audience_pansy_run_front,
    audience_pansy_tumble_front, audience_pansy_damage_front, audience_pansy_sleep_front, audience_pansy_sleep_back,
    audience_pansy_confuse_front, audience_pansy_confuse_back, audience_pansy_sing_front, 0,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_puni[19] = {
    audience_puni_wait_front, audience_puni_wait_back, audience_puni_jump_front, audience_puni_jump_back,
    audience_puni_raise_back, audience_puni_itemon_back, audience_puni_itemthrow_back, audience_puni_run_front,
    audience_puni_tumble_front, audience_puni_damage_front, audience_puni_sleep_front, audience_puni_sleep_back,
    audience_puni_confuse_front, audience_puni_confuse_back, 0, 0,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_nokonoko[19] = {
    audience_nokonoko_wait_front, audience_nokonoko_wait_back, audience_nokonoko_jump_front, audience_nokonoko_jump_back,
    audience_nokonoko_raise_back, audience_nokonoko_itemon_back, audience_nokonoko_itemthrow_back, audience_nokonoko_run_front,
    audience_nokonoko_tumble_front, audience_nokonoko_damage_front, audience_nokonoko_sleep_front, audience_nokonoko_sleep_back,
    audience_nokonoko_confuse_front, audience_nokonoko_confuse_back, 0, audience_nokonoko_shell_back,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_heavy_bomb[19] = {
    audience_heavy_bomb_wait_front, audience_heavy_bomb_wait_back, audience_heavy_bomb_jump_front, audience_heavy_bomb_jump_back,
    audience_heavy_bomb_raise_back, audience_heavy_bomb_itemon_back, audience_heavy_bomb_itemthrow_back, audience_heavy_bomb_run_front,
    audience_heavy_bomb_tumble_front, audience_heavy_bomb_damage_front, audience_heavy_bomb_sleep_front, audience_heavy_bomb_sleep_back,
    audience_heavy_bomb_confuse_front, audience_heavy_bomb_confuse_back, 0, 0,
    audience_heavy_bomb_ignite_front, audience_heavy_bomb_ignite_back, 0
};

__declspec(section ".data") u32* audience_anim_kuriboo[19] = {
    audience_kuriboo_wait_front, audience_kuriboo_wait_back, audience_kuriboo_jump_front, audience_kuriboo_jump_back,
    audience_kuriboo_raise_back, audience_kuriboo_itemon_back, audience_kuriboo_itemthrow_back, audience_kuriboo_run_front,
    audience_kuriboo_tumble_front, audience_kuriboo_damage_front, audience_kuriboo_sleep_front, audience_kuriboo_sleep_back,
    audience_kuriboo_confuse_front, audience_kuriboo_confuse_back, 0, 0,
    0, 0, 0
};

__declspec(section ".data") u32* audience_anim_pakkun[19] = {
    audience_pakkun_wait_front, audience_pakkun_wait_back, audience_pakkun_jump_front, audience_pakkun_jump_back,
    audience_pakkun_raise_back, audience_pakkun_itemon_back, audience_pakkun_itemthrow_back, audience_pakkun_run_front,
    audience_pakkun_tumble_front, audience_pakkun_damage_front, audience_pakkun_sleep_front, audience_pakkun_sleep_back,
    audience_pakkun_confuse_front, audience_pakkun_confuse_back, 0, 0,
    0, 0, audience_pakkun_eat_front
};

__declspec(section ".data") u32* audience_anim_luigi[19] = {
    audience_luigi_wait_front, audience_luigi_wait_back, audience_luigi_jump_front, audience_luigi_jump_back,
    audience_luigi_raise_back, audience_luigi_itemon_back, audience_luigi_itemthrow_back, audience_luigi_run_front,
    audience_luigi_tumble_front, audience_luigi_damage_front, audience_luigi_sleep_front, audience_luigi_sleep_back,
    audience_luigi_confuse_front, audience_luigi_confuse_back, 0, 0,
    0, 0, 0
};

/* Audience item-weight tables stored in .data. */
__declspec(section ".data") AudienceItemWeightLocal audience_item_kinopio[12] = {
    { 0x00000079, 0x0000000A },
    { 0x0000007B, 0x0000000A },
    { 0x0000007C, 0x0000000A },
    { 0x00000094, 0x00000032 },
    { 0x00000095, 0x0000000A },
    { 0x00000096, 0x00000001 },
    { 0x00000097, 0x00000005 },
    { 0x0000009A, 0x0000000A },
    { 0x00000084, 0x0000000A },
    { 0x00000081, 0x00000005 },
    { 0x00000082, 0x00000005 },
    { 0x00000000, 0x00000000 }
};

__declspec(section ".data") AudienceItemWeightLocal audience_item_heiho[17] = {
    { 0x00000083, 0x0000000A },
    { 0x00000085, 0x0000000A },
    { 0x00000087, 0x0000000A },
    { 0x0000008A, 0x0000000A },
    { 0x0000008D, 0x0000000A },
    { 0x00000089, 0x0000000A },
    { 0x0000008E, 0x0000000A },
    { 0x0000008C, 0x0000000A },
    { 0x00000092, 0x0000000A },
    { 0x00000093, 0x0000000A },
    { 0x000000A1, 0x0000000A },
    { 0x000000A2, 0x0000000A },
    { 0x000000A3, 0x0000000A },
    { 0x00000090, 0x0000000A },
    { 0x000000ED, 0x0000001E },
    { 0x000000EC, 0x0000001E },
    { 0x00000000, 0x00000000 }
};

__declspec(section ".data") AudienceItemWeightLocal audience_item_puni[11] = {
    { 0x00000079, 0x0000000A },
    { 0x0000007B, 0x0000000A },
    { 0x0000007C, 0x0000000A },
    { 0x00000098, 0x0000001E },
    { 0x0000009A, 0x00000014 },
    { 0x0000009B, 0x00000005 },
    { 0x0000009C, 0x00000001 },
    { 0x00000099, 0x0000000A },
    { 0x0000009D, 0x0000000A },
    { 0x0000009E, 0x0000000A },
    { 0x00000000, 0x00000000 }
};

__declspec(section ".data") AudienceItemWeightLocal audience_item_nokonoko[8] = {
    { 0x00000079, 0x0000000A },
    { 0x0000007B, 0x0000000A },
    { 0x0000007C, 0x0000000A },
    { 0x0000008F, 0x0000000A },
    { 0x0000008B, 0x0000000A },
    { 0x000000ED, 0x0000000A },
    { 0x000000EC, 0x0000000A },
    { 0x00000000, 0x00000000 }
};

__declspec(section ".data") AudienceItemWeightLocal audience_item_luigi[12] = {
    { 0x00000079, 0x0000000A },
    { 0x0000007B, 0x0000000A },
    { 0x0000007C, 0x0000000A },
    { 0x00000094, 0x0000000A },
    { 0x00000095, 0x0000000A },
    { 0x00000096, 0x0000000A },
    { 0x0000009A, 0x0000000A },
    { 0x0000009B, 0x0000000A },
    { 0x0000009C, 0x0000000A },
    { 0x00000082, 0x0000000A },
    { 0x000000ED, 0x0000000A },
    { 0x00000000, 0x00000000 }
};

/* Top-level audience-kind records. */
__declspec(section ".data") AudienceKindDataLocal audience_kind[13] = {
    {
        0x00000000, audience_anim_kinopio, audience_item_kinopio,
        10.0f, 25.0f, 3.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_daisan, audience_item_daisan,
        10.0f, 25.0f, 1.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_teresa, audience_item_teresa,
        0.0f, 30.0f, 1.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_hammer_bros, audience_item_hammer_bros,
        10.0f, 20.0f, 1.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_honenoko, audience_item_honenoko,
        10.0f, 25.0f, 0.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_heiho, audience_item_heiho,
        10.0f, 25.0f, 1.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_pansy, audience_item_pansy,
        10.0f, 10.0f, 1.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_puni, audience_item_puni,
        10.0f, 20.0f, 1.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_nokonoko, audience_item_nokonoko,
        10.0f, 25.0f, 1.0f, 1.0f, 1.0f
    },
    {
        0x01000000, audience_anim_heavy_bomb, audience_item_heavy_bomb,
        0.0f, 50.0f, 1.0f, 1.5f, 1.5f
    },
    {
        0x00000000, audience_anim_kuriboo, audience_item_kuriboo,
        10.0f, 25.0f, 1.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_pakkun, audience_item_pakkun,
        0.0f, 20.0f, 1.0f, 1.0f, 1.0f
    },
    {
        0x00000000, audience_anim_luigi, audience_item_luigi,
        0.0f, 40.0f, 1.0f, 1.2f, 1.2f
    }
};
