#include "unit/unit_system.h"

/* Generic battle-system unit resources. */
__declspec(section ".rodata") char str_btl_un_system_802fddd8[] = "btl_un_system";
__declspec(section ".rodata") char str_SFX_BTL_DAMAGE1_802fdde8[] = "SFX_BTL_DAMAGE1";
__declspec(section ".rodata") char str_SFX_BTL_DAMAGE_FIRE1_802fddf8[] = "SFX_BTL_DAMAGE_FIRE1";
__declspec(section ".rodata") char str_SFX_BTL_DAMAGE_ICE1_802fde10[] = "SFX_BTL_DAMAGE_ICE1";
__declspec(section ".rodata") char str_SFX_BTL_DAMAGE_BIRIB_802fde24[] = "SFX_BTL_DAMAGE_BIRIBIRI1";
__declspec(section ".rodata") u8 gap_04_802FDE3D_rodata[3] = {0};

extern void btlevtcmd_SetEventWait(void);
extern void btlevtcmd_SetEventAttack(void);
extern void btlevtcmd_SetEventDamage(void);
extern void btlevtcmd_SetMaxMoveCount(void);
extern void btlevtcmd_StartWaitEvent(void);

/* The public battle structs are still partial, so preserve the proven word layout here. */
static u32 parts[19] = {
    1, (u32)str_btl_un_system_802fddd8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0x0014001E, 0x00FF0000, 0, 0,
    0x03010009, 0, 0,
};

static u8 defence[5] = {0};
static u8 defence_attr[5] = {2, 2, 2, 2, 2};
static u8 regist[22] = {0};
static u32 data_table[2] = {0, 2};
static u32 damage_event[2] = {0, 1};
static u32 attack_event[2] = {0, 2};
static u32 wait_event[2] = {0, 1};

static u32 init_event[21];

u32 unit_system[49] = {
    /* 0x00 */ 0x000000DC, (u32)str_btl_un_system_802fddd8, 0x03E70000, 0x01010000,
    /* 0x10 */ 0, 0x00090018, 0x00180000, 0x00140000,
    /* 0x20 */ 0, 0, 0, 0,
    /* 0x30 */ 0, 0, 0, 0,
    /* 0x40 */ 0, 0, 0, 0,
    /* 0x50 */ 0, 0, 0x41400000, 0,
    /* 0x60 */ 0, 0x41400000, 0x4179999A, 0,
    /* 0x70 */ 0, 0, 0, 0,
    /* 0x80 */ 0, 0, 0x0000FF00, 0x64140000,
    /* 0x90 */ 0, 0, 0, (u32)str_SFX_BTL_DAMAGE1_802fdde8,
    /* 0xA0 */ (u32)str_SFX_BTL_DAMAGE_FIRE1_802fddf8,
               (u32)str_SFX_BTL_DAMAGE_ICE1_802fde10,
               (u32)str_SFX_BTL_DAMAGE_BIRIB_802fde24, 0,
    /* 0xB0 */ (u32)(regist + 8), 0x01000000, (u32)parts, (u32)init_event,
    /* 0xC0 */ (u32)regist,
};

static u32 init_event[21] = {
    0x0003005B, (u32)btlevtcmd_SetEventWait, 0xFFFFFFFE, (u32)wait_event,
    0x0003005B, (u32)btlevtcmd_SetEventAttack, 0xFFFFFFFE, (u32)attack_event,
    0x0003005B, (u32)btlevtcmd_SetEventDamage, 0xFFFFFFFE, (u32)damage_event,
    0x0003005B, (u32)btlevtcmd_SetMaxMoveCount, 0xFFFFFFFE, 0,
    0x0002005B, (u32)btlevtcmd_StartWaitEvent, 0xFFFFFFFE, 2,
    1,
};
