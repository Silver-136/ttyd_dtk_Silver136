// unit_party_yoshi.c: Yoshi's battle weapons and unit kind as a partner, and the Stampede part YoshiGundanPart.

#pragma once

#include "evt/evt_cmd.h"
#include "battle/battle_database_common.h"
#include "dolphin/types.h"
#include "battle/battle_unit.h"

// One running Yoshi of the Stampede move
typedef struct YoshiGundanPart {
    f32 x, y, z;
    f32 vx, vy, vz;
    u8 pad18[0xC]; // 0x18, never accessed
    u8 frameCount;
    u8 frame;
    s32 delay;
    s32 state;
} YoshiGundanPart;

// The swarm's work: 256 running Yoshis.
typedef struct YoshiGundanWork {
    s32 state;
    YoshiGundanPart parts[256];
    s32 pad3004; // 0x3004, never accessed
    f32 meanX;   // sum of the parts' x, then the mean (/ 256)
} YoshiGundanWork;

USER_FUNC(btl_yoshi_yoroyoro_jump_calc_param);
USER_FUNC(btl_yoshi_yoroyoro_jump_move);
USER_FUNC(yoshi_original_color_anim_set);
extern BattleWeapon partyWeapon_YoshiCallGuard;
extern BattleWeapon partyWeapon_YoshiEggAttack_Minimini;
extern BattleWeapon partyWeapon_YoshiNomikomi_Involved;
extern BattleWeapon partyWeapon_YoshiNomikomi_Shot;
extern BattleWeapon partyWeapon_YoshiNormalAttack;
extern PoseTableEntry pose_table_yoshi_stay[];
extern BattleUnitKind unitdata_Party_Yoshi;
