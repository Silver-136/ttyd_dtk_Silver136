#pragma once

#include "evt/evt_cmd.h"
#include "battle/battle_database_common.h"
#include "dolphin/types.h"
#include "battle/battle_unit.h"

USER_FUNC(battle_evt_majo_disp_off);
USER_FUNC(battle_evt_majo_disp_on);
extern BattleWeapon partyWeapon_VivianCharmKissAttack;
extern BattleWeapon partyWeapon_VivianMagicalPowder;
extern BattleWeapon partyWeapon_VivianNormalAttack;
extern BattleWeapon partyWeapon_VivianShadowGuard;
extern PoseTableEntry pose_table_vivian_stay[];
extern BattleUnitKind unitdata_Party_Vivian;
extern EvtWord vivian_shadow_tail_event[];
