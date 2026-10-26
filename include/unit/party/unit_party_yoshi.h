#pragma once

#include "evt/evt_cmd.h"
#include "battle/battle_database_common.h"
#include "dolphin/types.h"
#include "battle/battle_unit.h"

USER_FUNC(btl_yoshi_yoroyoro_jump_calc_param);
USER_FUNC(btl_yoshi_yoroyoro_jump_move);
extern BattleWeapon partyWeapon_YoshiCallGuard;
extern BattleWeapon partyWeapon_YoshiEggAttack_Minimini;
extern BattleWeapon partyWeapon_YoshiNomikomi_Involved;
extern BattleWeapon partyWeapon_YoshiNomikomi_Shot;
extern BattleWeapon partyWeapon_YoshiNormalAttack;
extern PoseTableEntry pose_table_yoshi_stay[];
extern BattleUnitKind unitdata_Party_Yoshi;
